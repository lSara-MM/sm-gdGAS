#include "GameplayEffect.h"

sm::GameplayEffect::GameplayEffect(EffectID id, GameplayEffect::Type type/*, uint32 target, uint32 source*/, float time) : m_UID(id)/*, m_TargetID(target), m_SourceID(source)*/, m_EffectType(type), m_RemainingTime(time)
{
}

void sm::GameplayEffect::Tick(float dt)
{
	m_RemainingTime -= dt;
}

void sm::GameplayEffect::AddModifier(ModifierHandle handle)
{
	m_Modifiers.push_back(handle);
}