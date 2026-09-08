#include "core/EffectSystem.h"

#include "godot/gdAttributeContainer.h"
#include "godot/gdGASEntity.h"
#include "godot/gdGASWorld.h"
#include "godot/gdTagContainer.h"
#include "internal/smAssert.h"

void sm::EffectSystem::Update(float dt)
{
	for (auto it = m_ActiveEffects.begin(); it != m_ActiveEffects.end(); )
	{
		it->Tick(dt);

		if (it->HasExpired())
		{
			RemoveEffect(&(*it));
		}
		else
		{
			++it;
		}
	}
}

sm::GameplayEffect* sm::EffectSystem::FindEffect(EffectID effectID)
{
	auto it = std::find_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const GameplayEffect& e)
		{
			return e.GetID() == effectID;
		}
	);

	return (it != m_ActiveEffects.end()) ? &(*it) : nullptr;
}

sm::GameplayEffect* sm::EffectSystem::FindEffect(EffectInstanceID effectID)
{
	if (auto itr = m_EffectsIndex.find(effectID); itr != m_EffectsIndex.end())
	{
		return &m_ActiveEffects[itr->second];
	}

	return nullptr;
}

EffectInstanceID sm::EffectSystem::AddActiveEffect(GameplayEffect& effect)
{
	auto id = effect.GetInstanceID();
	m_EffectsIndex[id] = m_ActiveEffects.size();
	&m_ActiveEffects.emplace_back(std::move(effect));

	return id;
}

void sm::EffectSystem::RemoveEffect(GameplayEffect* effect)
{
	SM_ASSERT(_world != nullptr, "Critical error: Could not remove effect. World not created.");

	GAS_Entity* entity = _world->GetEntity(effect->GetTargetID());
	if (entity)
	{
		RemoveEffectModifiers(entity, effect);

		entity->AddTags(effect->GetTagsToRemove());
		entity->RemoveTags(effect->GetTagsToAdd());
	}

	m_EffectsIndex.erase(effect->GetInstanceID());
	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const GameplayEffect& activeEffect)
		{
			return activeEffect.GetInstanceID() == effect->GetInstanceID();
		});

	m_ActiveEffects.erase(itr, m_ActiveEffects.end());
}

void sm::EffectSystem::RemoveEffect(EffectInstanceID effectID, GAS_Entity* entity)
{
	auto* effect = FindEffect(effectID);
	RemoveEffectModifiers(entity, effect);

	entity->AddTags(effect->GetTagsToRemove());
	entity->RemoveTags(effect->GetTagsToAdd());

	m_EffectsIndex.erase(effect->GetInstanceID());
	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const GameplayEffect& activeEffect)
		{
			return activeEffect.GetInstanceID() == effect->GetInstanceID();
		});

	m_ActiveEffects.erase(itr, m_ActiveEffects.end());
}

void sm::EffectSystem::RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect* effect)
{
	AttributeContainer* attrContainer = entity->GetAttributeContainer();
	ERR_FAIL_NULL_MSG(attrContainer,
		godot::vformat("RemoveEffect: Could not remove '%s'. The AttributeContainer was not found.", effect->GetID()));

	for (ModifierHandle& handle : effect->GetModifierHandles())
	{
		GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
		attr->RemoveModifier(handle);
	}
}

void sm::EffectSystem::ClearEffects(GAS_Entity* entity)
{
	std::vector<GameplayEffect*> pendingToDelete;

	for (auto& effect : m_ActiveEffects)
	{
		if (effect.GetTargetID() == entity->GetID())
		{
			pendingToDelete.push_back(&effect);
		}
	}

	for (auto* effect : pendingToDelete)
	{
		RemoveEffect(effect->GetInstanceID(), entity);
	}
}
