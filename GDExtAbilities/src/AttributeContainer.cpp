#include "AttributeContainer.h"

#include "AttributeSet.h"

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

	// Due to overloading, Godot does not deduce which one it refers to so it needs casting
	godot::ClassDB::bind_method(godot::D_METHOD("add_attribute", "attr"), static_cast<void (AttributeContainer::*)(const godot::Ref<sm::Attribute>&)>(&AddAttribute));

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"),
		"set_attributes_set", "get_attributes_set"
	);
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

		std::vector <godot::Ref< sm::Attribute>> attrs = m_gdAttributeSet->SortByName();

		for (int i = 0; i < attrs.size(); ++i)
		{
			m_AttributeSetPtr->AddAttribute(_GenerateUID(), attrs[i]->GetBaseValue());
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

void sm::AttributeContainer::AddAttribute(godot::StringName name, float baseValue)
{
	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		m_gdAttributeSet->AddAttribute(baseValue, name);
	}

	m_AttributeSetPtr->AddAttribute(_GenerateUID(), baseValue);
}

void sm::AttributeContainer::AddAttribute(const godot::Ref<sm::Attribute>& attr)
{
	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		m_gdAttributeSet->AddAttribute(attr);
	}

	m_AttributeSetPtr->AddAttribute(_GenerateUID(), attr->GetBaseValue());
}

float sm::AttributeContainer::GetAttributeID(godot::StringName name)
{
	if (godot::Ref<sm::Attribute> ret = m_gdAttributeSet->GetAttribute(name); ret != godot::Ref<sm::Attribute>())
	{
		return ret->GetBaseValue();
	}

	return -1.0f;
}
