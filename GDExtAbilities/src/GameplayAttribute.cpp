#include "GameplayAttribute.h"

void sm::Attribute::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GetName);
	godot::ClassDB::bind_method(godot::D_METHOD("set_name", "name"), &SetName);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "name"),
		"set_name", "get_name"
	);

	godot::ClassDB::bind_method(godot::D_METHOD("get_base_value"), &GetBaseValue);
	godot::ClassDB::bind_method(godot::D_METHOD("set_base_value", "value"), &SetBaseValue);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "base_value"),
		"set_base_value", "get_base_value"
	);
}

float sm::Attribute::GetBaseValue()
{
	return baseValue;
}

void sm::Attribute::SetBaseValue(float value)
{
	baseValue = value;
}

godot::StringName sm::Attribute::GetName()
{
	return name;
}

void sm::Attribute::SetName(godot::StringName n)
{
	name = n;
}