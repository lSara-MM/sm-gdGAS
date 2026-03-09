#include "gdAttributeData.h"

#include <godot_cpp/classes/engine.hpp>

void sm::AttributeData::_bind_methods()
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

	// TODO: Hacer un tooltip para explicar que es el min y max.
	godot::ClassDB::bind_method(godot::D_METHOD("get_min_value"), &GetMinValue);
	godot::ClassDB::bind_method(godot::D_METHOD("set_min_value", "value"), &SetMinValue);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "min_value"),
		"set_min_value", "get_min_value"
	);

	godot::ClassDB::bind_method(godot::D_METHOD("get_max_value"), &GetMaxValue);
	godot::ClassDB::bind_method(godot::D_METHOD("set_max_value", "value"), &SetMaxValue);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "max_value"),
		"set_max_value", "get_max_value"
	);
}

void sm::AttributeData::SetName(godot::StringName n)
{
	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		if (n != name)
		{
			eventSetName.Notify(n);
		}
	}

	name = n;
}