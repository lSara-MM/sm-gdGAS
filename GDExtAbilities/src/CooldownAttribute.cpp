#include "CooldownAttribute.h"

sm::CooldownAttribute::CooldownAttribute(const float cd) : m_Cooldown(cd)
{}

sm::CooldownAttribute::~CooldownAttribute()
{}

void sm::CooldownAttribute::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("_init", "cd"), &_init);
	godot::ClassDB::bind_method(godot::D_METHOD("get_cooldown"), &GetCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cooldown", "cd"), &SetCooldown);

	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "Cooldown"), "set_cooldown", "get_cooldown");

	godot::ClassDB::bind_method(godot::D_METHOD("reset_cooldown"), &ResetCooldown);

}

void sm::CooldownAttribute::_init(const float cd)
{
	m_Cooldown = cd;
}

void sm::CooldownAttribute::ResetCooldown()
{
	m_CurrentTimer = m_Cooldown;
}
