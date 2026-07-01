#include "core/EffectSystem.h"

#include "godot/gdAttribute.h"
#include "godot/gdAttributeContainer.h"
#include "godot/gdGASEntity.h"
#include "godot/gdGASWorld.h"
#include "godot/gdTagContainer.h"
#include "internal/smAssert.h"

void sm::EffectSystem::Update(float dt)
{
	std::vector<std::pair<GameplayEffect*, size_t>> pendingToDelete;

	for (size_t i = 0; i < m_ActiveEffects.size(); ++i)
	{
		GameplayEffect* effect = &m_ActiveEffects[i];

		effect->Tick(dt);
		if (effect->HasExpired())
		{
			pendingToDelete.push_back({ effect,  i });
		}
	}

	for (auto const [effect, index] : pendingToDelete)
	{
		RemoveEffect(effect, index);
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
	if (auto itr = m_Effects.find(effectID); itr != m_Effects.end())
	{
		return itr->second;
	}

	return nullptr;
}

EffectInstanceID sm::EffectSystem::AddActiveEffect(GameplayEffect& effect)
{
	auto id = effect.GetInstanceID();
	GameplayEffect* ptr = &m_ActiveEffects.emplace_back(std::move(effect));
	m_Effects[id] = ptr;

	return id;
}

//void sm::EffectSystem::RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect)
//{
//	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
//		[&](const std::unique_ptr<sm::GameplayEffect>& effect)
//		{
//			return effect->GetID() == gdEffect->GetName();
//		});
//
//	RemoveEffectModifiers(id, itr);
//}

//void sm::EffectSystem::RemoveEffect(EffectID gdEffectID)
//{
//	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
//		[&](const EffectPtr& effect)
//		{
//			return effect->GetID() == gdEffectID;
//		});
//
//	RemoveEffectModifiers((*itr)->GetTargetID(), itr);
//}

void sm::EffectSystem::RemoveEffect(GameplayEffect* effect, size_t index)
{
	SM_ASSERT(_world != nullptr, "Critical error: Could not remove effect. World not created.");

	GAS_Entity* entity = _world->GetEntity(effect->GetTargetID());
	SM_ASSERT(entity != nullptr, "Critical error: Could not remove effect. Entity %d was not found.", effect->GetTargetID());

	RemoveEffectModifiers(entity, effect);

	entity->AddTags(effect->GetTagsToRemove());
	entity->RemoveTags(effect->GetTagsToAdd());

	m_ActiveEffects.erase(m_ActiveEffects.begin() + index);
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
