#include "AttributeContainer.h"

#include "AttributeSet.h"

void sm::AttributeContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributeSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "arr"), &SetAttributeSet);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSet"),
		"set_attributes_set", "get_attributes_set"
	);
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
	
}
