#include "gdModifier.h"

#include "GameplayModifier.h"

void sm::Modifier::_bind_methods()
{
	BIND_ENUM_CONSTANT(Modifier::OperationType::Add);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Multiply);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Percent);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Override);

	godot::ClassDB::bind_method(godot::D_METHOD("get_operation_type"), &GetOperationType);
	godot::ClassDB::bind_method(godot::D_METHOD("set_operation_type", "type"), &SetOperationType);

	godot::ClassDB::bind_method(godot::D_METHOD("get_target_id"), &GetTargetID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_target_id", "id"), &SetTargetID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_source_id"), &GetSourceID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_source_id", "id"), &SetSourceID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_value"), &GetValue);
	godot::ClassDB::bind_method(godot::D_METHOD("set_value", "value"), &SetValue);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "operation_type",
		godot::PROPERTY_HINT_ENUM, "Add, Multiply, Percent, Override"),
		"set_operation_type", "get_operation_type"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "target_id"),
		"set_target_id", "get_target_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "source_id"),
		"set_source_id", "get_source_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "value"),
		"set_value", "get_value"
	);
}