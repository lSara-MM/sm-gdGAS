#include "Attribute.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>

sm::Attribute::Attribute()
{}

sm::Attribute::~Attribute()
{}

void sm::Attribute::_bind_methods()
{}

// ---------------------------------
//sm::FloatAttribute::FloatAttribute(float base, float current)
//{
//	Initialize(base, current);
//}
//
//sm::FloatAttribute::~FloatAttribute()
//{}
//
//void sm::FloatAttribute::Initialize(float base, float current)
//{
//	m_BaseValue = base;
//	m_CurrentValue = std::isnan(current) ? base : current;
//}
//
//void sm::FloatAttribute::_bind_methods()
//{
//	godot::ClassDB::bind_method(godot::D_METHOD("initialize", "base", "current"), &Initialize, DEFVAL(0.0f), DEFVAL(NAN));
//
//	godot::ClassDB::bind_method(godot::D_METHOD("get_base_value"), &GetBaseValue);
//	godot::ClassDB::bind_method(godot::D_METHOD("set_base_value", "value"), &SetBaseValue);
//	
//	ADD_PROPERTY(godot::PropertyInfo(
//		godot::Variant::FLOAT,
//		"BaseValue"),
//		"set_base_value", "get_base_value"
//	);
//
//	godot::ClassDB::bind_method(godot::D_METHOD("get_current_value"), &GetCurrentValue);
//	godot::ClassDB::bind_method(godot::D_METHOD("set_current_value", "value"), &SetCurrentValue);
//	
//	ADD_PROPERTY(godot::PropertyInfo(
//		godot::Variant::FLOAT,
//		"CurrentValue"),
//		"set_current_value", "get_current_value"
//	);
//}