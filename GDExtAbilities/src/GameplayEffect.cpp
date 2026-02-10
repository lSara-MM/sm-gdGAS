#include "GameplayEffect.h"

sm::GameplayEffect::GameplayEffect(EffectID id, GameplayEffect::Type type/*, uint32 target, uint32 source*/) : m_UID(id)/*, m_TargetID(target), m_SourceID(source)*/, m_EffectType(type)
{
}

void sm::GameplayEffect::Tick(float dt)
{
	m_RemainingTime -= dt;
}
