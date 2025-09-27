#include "Ability.h"

#include "CooldownAttribute.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <utility>

sm::Ability::Ability()
{}

sm::Ability::~Ability()
{}

void sm::Ability::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_active"), &GetActive);
	godot::ClassDB::bind_method(godot::D_METHOD("set_active", "active"), &SetActive);
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::BOOL, "is_active"), "set_active", "get_active");

	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes"), &GetAttributeSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes", "attributes"), &SetAttributeSet);
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attributes",
		godot::PROPERTY_HINT_TYPE_STRING,
		godot::String::num(godot::Variant::OBJECT) + "/" + godot::String::num(godot::PROPERTY_HINT_RESOURCE_TYPE) + ":AttributeSet"), "set_attributes", "get_attributes");
}

void sm::Ability::Activate()
{
	m_IsActive = true;
}

void sm::Ability::Deactivate()
{}

bool sm::Ability::CanBeActivated() const
{
	return false;
}