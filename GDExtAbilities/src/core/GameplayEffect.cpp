#include "core/GameplayEffect.h"

sm::GameplayEffect::GameplayEffect(EffectID name, EffectInstanceID id,
	GameplayEffect::Type type, EntityID target, EntityID source, float time)
	: m_Name(name), m_ID(id), m_TargetID(target), m_SourceID(source), m_EffectType(type), m_RemainingTime(time)
{}

bool sm::GameplayEffect::HasExpired() const
{
	return m_RemainingTime <= 0 && m_EffectType == GameplayEffect::Type::Temporary;
}

void sm::GameplayEffect::Tick(float dt)
{
	if (m_EffectType == GameplayEffect::Type::Temporary)
	{
		m_RemainingTime -= dt;
	}
}

void sm::GameplayEffect::AddModifier(ModifierHandle handle)
{
	m_Modifiers.push_back(handle);
}