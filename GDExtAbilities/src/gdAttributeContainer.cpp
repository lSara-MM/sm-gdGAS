#include "gdAttributeContainer.h"

#include "GameplayAttribute.h"
#include "GameplayAttributeSet.h"
#include "GameplayModifier.h"
#include "gdAttributeSetData.h"

#include <classes/engine.hpp>

sm::AttributeContainer::AttributeContainer()
{
	m_AttributeSetPtr = std::make_unique<sm::GameplayAttributeSet>();
}

sm::AttributeContainer::~AttributeContainer()
{

}

void sm::AttributeContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributeSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "attr"), &SetAttributeSet);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSetData"),
		"set_attributes_set", "get_attributes_set"
	);

	// Methods
	godot::ClassDB::bind_method(godot::D_METHOD("add_modifier", "attribute_id", "modifier"), &AddModifier);
	godot::ClassDB::bind_method(godot::D_METHOD("remove_modifier", "attribute_id", "modifier"), &AddModifier);

	// Signals
	ADD_SIGNAL(godot::MethodInfo("attribute_modified",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::FLOAT, "old_value"),
		godot::PropertyInfo(godot::Variant::FLOAT, "new_value")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_added",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier")
	));
}

void sm::AttributeContainer::_notification(int notification)
{
	// When node container is loaded, get editor changes and apply them to the C++ AttributeData Set
	if (notification == NOTIFICATION_READY)
	{
		if (m_gdAttributeSet == nullptr)
		{
			return;
		}

		m_gdAttributeSet->ValidateSetData(m_gdAttributeSet->GetAttributesSet());

		std::vector<godot::Ref<sm::AttributeData>> attrs = m_gdAttributeSet->SortByName();

		for (size_t i = 0; i < attrs.size(); ++i)
		{
			sm::GameplayAttribute* addedAttr = &m_AttributeSetPtr->AddAttribute(attrs[i]->GetName(), attrs[i]->GetBaseValue());
			m_AttributesByName.try_emplace(attrs[i]->GetName(), addedAttr);
		}
	}
}

godot::Ref<sm::AttributeSetData> sm::AttributeContainer::GetAttributeSet()
{
	return m_gdAttributeSet;
}

void sm::AttributeContainer::SetAttributeSet(const godot::Ref<sm::AttributeSetData>& attrSet)
{
	m_gdAttributeSet = attrSet;
}

void sm::AttributeContainer::AddModifier(AttributeID id, const godot::Ref<sm::ModifierData>& mod)
{
	sm::GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	attr->AddModifier(mod);
	ERR_PRINT("added");

	emit_signal("modifier_added", this, id, mod);
}

void sm::AttributeContainer::RemoveModifier(AttributeID id, const  godot::Ref<sm::ModifierData>& mod)
{
	sm::GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	attr->RemoveModifier(mod);
	ERR_PRINT("added");

	emit_signal("modifier_removed", this, id, mod);
}

void sm::AttributeContainer::ModifyAttribute(AttributeID id, float newValue)
{
	sm::GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	float oldValue = attr->GetBase();
	attr->SetBase(newValue);

	emit_signal("attribute_modified", this, id, oldValue, newValue);
}

void sm::AttributeContainer::_OnAttributeModified(sm::AttributeContainer& attributeContainer, AttributeID attrID, float oldValue, float newValue)
{

}

void sm::AttributeContainer::_OnModifierAdded(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::ModifierData> mod)
{

}

void sm::AttributeContainer::_OnModifierRemoved(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::ModifierData> mod)
{

}
