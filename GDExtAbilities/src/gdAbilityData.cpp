#include "gdAbilityData.h"

#include "TagRegistry.h"

sm::AbilityData::AbilityData()
{
	m_CooldownData.SetDuration(m_Cooldown);
}

void sm::AbilityData::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_id"), &GetAbilityID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_id", "id"), &SetAbilityID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cooldown"), &GetCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cooldown", "value"), &SetCooldown);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "ID"),
		"set_ability_id", "get_ability_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "cooldown"),
		"set_cooldown", "get_cooldown"
	);
}

void sm::AbilityData::SetAbilityID(godot::StringName name)
{
#ifdef EDITOR_MODE
	TagRegistry& registry = TagRegistry::Instance();
	if (!registry.RegisterTag(m_ID))
	{
		return;
	}
#endif // EDITOR_MODE

	m_ID = name;
	m_CooldownData.AddTag(m_CooldownData.GetName());
}

void sm::AbilityData::SetCooldown(float value)
{
	m_Cooldown = value;
	m_CooldownData.SetDuration(m_Cooldown);
}
