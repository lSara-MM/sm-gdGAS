#include "core/GameplayEffect.h"

sm::GameplayEffect::GameplayEffect(EffectID name, EffectInstanceID id,
	GameplayEffect::Type type, EntityID target, EntityID source, float time)
	: m_Name(name), m_ID(id), m_TargetID(target), m_SourceID(source), m_EffectType(type), m_RemainingTime(time), m_Time(time)
{
#ifdef DEBUG_ENABLED
	m_NameDebug = ToStdString(name);
#endif // DEBUG_ENABLED
}

bool sm::GameplayEffect::HasExpired() const
{
	return m_RemainingTime <= 0 && m_EffectType == GameplayEffect::Type::Temporary;
}

float sm::GameplayEffect::GetCurrentCooldown() const
{
	return m_RemainingTime;
}

void sm::GameplayEffect::Tick(float dt)
{
	if (m_EffectType == GameplayEffect::Type::Temporary)
	{
		m_RemainingTime -= dt;
	}
}

void sm::GameplayEffect::ResetCooldown()
{
	m_RemainingTime = m_Time;
}

void sm::GameplayEffect::AddModifier(ModifierHandle handle)
{
	m_Modifiers.push_back(handle);
}

void sm::GameplayEffect::SetTagsToAdd(godot::PackedInt32Array arr)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		m_TagsToAdd.Set(arr[i]);
	}
}

void sm::GameplayEffect::SetTagsToRemove(godot::PackedInt32Array arr)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		m_TagsToAdd.Set(arr[i], false);
	}
}
