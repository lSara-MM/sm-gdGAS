#include "core/EffectSystem.h"

#include "godot/gdAttribute.h"
#include "godot/gdAttributeContainer.h"
#include "godot/gdGASEntity.h"
#include "godot/gdGASWorld.h"
#include "godot/gdTagContainer.h"
#include "internal/smAssert.h"

void sm::EffectSystem::Update(float dt)
{
	for (size_t i = 0; i < m_ActiveEffects.size(); ++i)
	{
		auto& effect = m_ActiveEffects[i];

		effect.Tick(dt);
		if (effect.HasExpired())
		{
			RemoveEffect(effect);
			--i;
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

	return (it != m_ActiveEffects.end()) ? it._Ptr : nullptr;
}

void sm::EffectSystem::AddActiveEffect(GameplayEffect& effect)
{
	m_ActiveEffects.push_back(std::move(effect));
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

void sm::EffectSystem::RemoveEffect(GameplayEffect& effect)
{
	GAS_World* world = GAS_World::Instance();
	SM_ASSERT(world != nullptr, "Critical error: Could not remove effect. World not created.");

	GAS_Entity* entity = world->GetEntity(effect.GetTargetID());
	SM_ASSERT(entity != nullptr, "Critical error: Could not remove effect. Entity %d was not found.", effect.GetTargetID());
	
	RemoveEffectModifiers(entity, effect);

	entity->RemoveTags(effect.GetTagsToAdd());
}

void sm::EffectSystem::RemoveEffectModifiers(GAS_Entity* entity, std::vector<GameplayEffect>::iterator& itr)
{
	AttributeContainer* attrContainer = entity->GetAttributeContainer();

	for (auto itr2 = itr; itr2 != m_ActiveEffects.end(); ++itr2)
	{
		for (ModifierHandle& handle : (itr2)->GetModifierHandles())
		{
			GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
			attr->RemoveModifier(handle);
		}
	}

	m_ActiveEffects.erase(itr, m_ActiveEffects.end());
}

void sm::EffectSystem::RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect& effect)
{
	AttributeContainer* attrContainer = entity->GetAttributeContainer();
	ERR_FAIL_NULL_MSG(attrContainer,
		godot::vformat("RemoveEffect: Could not remove '%s'. The AttributeContainer was not found.", effect.GetID()));

	for (ModifierHandle& handle : effect.GetModifierHandles())
	{
		GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
		attr->RemoveModifier(handle);
	}

	std::swap(effect, m_ActiveEffects.back());
	m_ActiveEffects.pop_back();
}