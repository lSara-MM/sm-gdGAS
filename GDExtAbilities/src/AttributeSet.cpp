#include "AttributeSet.h"

sm::GameplayAttributeSet::GameplayAttributeSet()
{}

sm::GameplayAttributeSet::~GameplayAttributeSet()
{}

void sm::AttributeSet::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributesSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "arr"), &SetAttributesSet);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"attribute_set",
		godot::PROPERTY_HINT_TYPE_STRING,
		godot::String::num(godot::Variant::OBJECT) + "/" + godot::String::num(godot::PROPERTY_HINT_RESOURCE_TYPE) + ":Attribute"),
		"set_attributes_set", "get_attributes_set"
	);
}

godot::TypedArray<sm::Attribute> sm::AttributeSet::GetAttributesSet()
{
	return m_gdAttributes;
}

void sm::AttributeSet::SetAttributesSet(const godot::TypedArray<sm::Attribute>& attr)
{
	m_gdAttributes = attr;
}
