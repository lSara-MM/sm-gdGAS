#include "AttributeContainer.h"

#include "GameplayAttribute.h"
#include "GameplayAttributeSet.h"
#include "GameplayModifier.h"
#include "gdAttributeSet.h"

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
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"),
		"set_attributes_set", "get_attributes_set"
	);

	// Signals
	ADD_SIGNAL(godot::MethodInfo("attribute_modified",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::FLOAT, "old_value"),
		godot::PropertyInfo(godot::Variant::FLOAT, "new_value")
	));
}

void sm::AttributeContainer::_notification(int notification)
{
	// When node container is loaded, get editor changes and apply them to the C++ Attribute Set
	if (notification == NOTIFICATION_READY)
	{
		if (m_gdAttributeSet == nullptr)
		{
			return;
		}

		m_gdAttributeSet->ValidateSetData(m_gdAttributeSet->GetAttributesSet());

		std::vector <godot::Ref<sm::Attribute>> attrs = m_gdAttributeSet->SortByName();

		for (int i = 0; i < attrs.size(); ++i)
		{
			sm::GameplayAttribute& addedAttr = m_AttributeSetPtr->AddAttribute(_GenerateUID(), attrs[i]->GetBaseValue());
			//m_AttributesByName.try_emplace(attrs[i]->GetName(), addedAttr);
		}
	}
}

godot::Ref<sm::AttributeSet> sm::AttributeContainer::GetAttributeSet()
{
	return m_gdAttributeSet;
}

void sm::AttributeContainer::SetAttributeSet(const godot::Ref<sm::AttributeSet>& attrSet)
{
	m_gdAttributeSet = attrSet;
}

AttributeID sm::AttributeContainer::GetAttributeID(godot::StringName name)
{
	//if (m_AttributesByName.find(name) != m_AttributesByName.end())
	//{
	//	return m_AttributesByName[name].GetUID();
	//}
	return 0;
}

void sm::AttributeContainer::AddModifier(AttributeID id, sm::Modifier mod)
{
	m_AttributeSetPtr->AddModifier(id, mod);
}

void sm::AttributeContainer::_OnAttributeModified(sm::AttributeContainer& attributeContainer, godot::StringName attrName, float oldValue, float newValue)
{

}
