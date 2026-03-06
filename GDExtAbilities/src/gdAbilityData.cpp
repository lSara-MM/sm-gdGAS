#include "gdAbilityData.h"

#include "TagRegistry.h"

sm::AbilityData::AbilityData()
{
	m_CooldownData.SetDuration(m_Cooldown);
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
