#include "gdAttributeContainer.h"

#include "GameplayAttribute.h"
#include "GameplayAttributeSet.h"
#include "GameplayModifier.h"
#include "gdAttribute.h"

#include <godot_cpp/variant/signal.hpp>

sm::AttributeContainer::AttributeContainer()
{
	m_AttributeSetPtr = std::make_unique<sm::GameplayAttributeSet>();
	m_EffectSystemPtr = std::make_unique<sm::EffectSystem>();
}

sm::AttributeContainer::~AttributeContainer()
{

}

void sm::AttributeContainer::_bind_methods()
{
	// Methods
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributeSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "attr"), &SetAttributeSet);

	godot::ClassDB::bind_method(godot::D_METHOD("add_modifier", "attribute_id", "modifier"), &AddModifier);
	godot::ClassDB::bind_method(godot::D_METHOD("remove_modifier", "attribute_id", "modifier"), &RemoveModifier);

	// Properties
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSetData"),
		"set_attributes_set", "get_attributes_set"
	);

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
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_added",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "effect_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "effect_data")
	));
}

void sm::AttributeContainer::_notification(int notification)
{
	// When node container is loaded, get editor changes and apply them to the C++ AttributeData Set
	if (notification == NOTIFICATION_READY)
	{
		if (m_gdAttributeSetData == nullptr)
		{
			return;
		}

		m_gdAttributeSetData->ValidateSetData(m_gdAttributeSetData->GetAttributesSet());

		std::vector<godot::Ref<sm::AttributeData>> attrs = m_gdAttributeSetData->SortByName();

		for (size_t i = 0; i < attrs.size(); ++i)
		{
			AddAttribute(attrs[i]->GetName(), attrs[i]->GetBaseValue());
		}
	}
}

godot::Ref<sm::Attribute> sm::AttributeContainer::GetAttribute(AttributeID id) const
{
	return godot::Ref<Attribute>();
}

godot::Ref<sm::AttributeSetData> sm::AttributeContainer::GetAttributeSet() const
{
	return m_gdAttributeSetData;
}

void sm::AttributeContainer::SetAttributeSet(const godot::Ref<AttributeSetData>& attrSet)
{
	m_gdAttributeSetData = attrSet;
}

void sm::AttributeContainer::AddModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	size_t t = attr->AddModifier(mod);
	ERR_PRINT("added");
	ERR_PRINT(std::to_string(t).c_str());

	emit_signal("modifier_added", this, id, mod);
}

void sm::AttributeContainer::RemoveModifier(AttributeID id,  godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	attr->RemoveModifier(mod);
	ERR_PRINT("added");

	emit_signal("modifier_removed", this, id, mod);
}

void sm::AttributeContainer::AddEffect(AttributeID id, godot::Ref<EffectData> effect)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	m_EffectSystemPtr->AddEffect(effect);
}

void sm::AttributeContainer::RemoveEffect(AttributeID id, godot::Ref<EffectData> effect)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);

	godot::TypedArray<ModifierData> modifiers = effect->GetModifiers();
	for (size_t i = 0; i < modifiers.size(); i++)
	{
		emit_signal("modifier_removed", this, id, modifiers[i]);
	}

	m_EffectSystemPtr->RemoveEffect(*attr, effect);
	emit_signal("effect_removed", this, id, effect);
}

void sm::AttributeContainer::RemoveEffect(AttributeID id, EffectID effectID)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	auto effect = m_EffectSystemPtr->FindEffect(effectID);

	// TODO: modifier_removed signal
	/*godot::TypedArray<ModifierData> modifiers = attr->GetModifiers();
	for (size_t i = 0; i < modifiers.size(); i++)
	{
		emit_signal("modifier_removed", this, id, modifiers[i]);
	}*/

	m_EffectSystemPtr->RemoveEffect(*attr, effectID);
}

void sm::AttributeContainer::AddAttribute(godot::StringName id, float baseValue)
{
	GameplayAttribute* addedAttr = &m_AttributeSetPtr->AddAttribute(id, baseValue);
	m_AttributesByName.try_emplace(id, addedAttr);
}

void sm::AttributeContainer::ModifyAttribute(AttributeID id, float newValue)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	float oldValue = attr->GetBase();
	attr->SetBase(newValue);

	emit_signal("attribute_modified", this, id, oldValue, newValue);
}