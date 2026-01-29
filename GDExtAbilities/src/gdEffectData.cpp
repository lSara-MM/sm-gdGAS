#include "gdEffectData.h"

void sm::EffectData::_bind_methods()
{
	// Type
	BIND_ENUM_CONSTANT(Permanent);
	BIND_ENUM_CONSTANT(Temporary);
	BIND_ENUM_CONSTANT(Persistent);

	godot::ClassDB::bind_method(godot::D_METHOD("get_modifiers"), &GetModifiers);
	godot::ClassDB::bind_method(godot::D_METHOD("set_modifiers", "modifiers"), &SetModifiers);

	godot::ClassDB::bind_method(godot::D_METHOD("get_effect_type"), &GetEffectType);
	godot::ClassDB::bind_method(godot::D_METHOD("set_effect_type", "type"), &SetEffectType);
	
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "effect_type",
		godot::PROPERTY_HINT_ENUM, "Permanent, Temporary, Persistent"),
		"set_effect_type", "get_effect_type"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"modifiers",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:ModifierData"),
		"set_modifiers", "get_modifiers");

	/*ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "source_id"),
		"set_source_id", "get_source_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "value"),
		"set_value", "get_value"
	);*/
}