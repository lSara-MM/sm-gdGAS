#include "EffectSystem.h"

#include "Assert.h"
#include "gdAttribute.h"
#include "gdAttributeContainer.h"
#include "gdGASEntity.h"
#include "gdGASWorld.h"
#include "gdTagContainer.h"

sm::EffectSystem::EffectSystem()
{
	ERR_PRINT("effect created");
}

sm::EffectSystem::~EffectSystem()
{
	ERR_PRINT("effect deleted");
}

void sm::EffectSystem::Update(float dt)
{
	for (size_t i = 0; i < m_ActiveEffects.size(); ++i)
	{
		auto& effect = m_ActiveEffects[i];

		effect->Tick(dt);
		if (effect->HasExpired())
		{
			ERR_PRINT(effect->GetID());

			RemoveEffect(effect);
			--i;
		}
	}
}

sm::GameplayEffect* sm::EffectSystem::FindEffect(EffectID effectID)
{
	auto it = std::find_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const EffectPtr& e)
		{
			return e->GetID() == effectID;
		}
	);

	return (it != m_ActiveEffects.end()) ? it->get() : nullptr;
}

void sm::EffectSystem::AddActiveEffect(EffectPtr& effect)
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

void sm::EffectSystem::RemoveEffect(EffectPtr& effect)
{
	GAS_World* world = GAS_World::Instance();
	GAS_Entity* entity = world->GetEntity(effect->GetTargetID());
	SM_ASSERT(entity != nullptr, "Critical error: Could not remove effect. Entity %d was not found.", effect->GetTargetID());
	
	RemoveEffectModifiers(entity, effect);
}

void sm::EffectSystem::RemoveEffectModifiers(EntityID id, std::vector<sm::EffectSystem::EffectPtr>::iterator& itr)
{
	GAS_Entity* entity = sm::GAS_World::Instance()->GetEntity(id);
	AttributeContainer* attrContainer = entity->GetAttributeContainer();

	for (auto itr2 = itr; itr2 != m_ActiveEffects.end(); ++itr2)
	{
		for (ModifierHandle& handle : (*itr2)->GetModifierHandles())
		{
			GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
			attr->RemoveModifier(handle);
		}
	}

	m_ActiveEffects.erase(itr, m_ActiveEffects.end());
}

void sm::EffectSystem::RemoveEffectModifiers(GAS_Entity* entity, EffectPtr& effect)
{
	AttributeContainer* attrContainer = entity->GetAttributeContainer();
	ERR_FAIL_NULL_MSG(attrContainer,
		godot::vformat("RemoveEffect: Could not remove %s. The AttributeContainer was not found.",
			ToStdString(effect->GetID()).c_str()));

	for (ModifierHandle& handle : effect->GetModifierHandles())
	{
		GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
		attr->RemoveModifier(handle);
	}

	 

	std::swap(effect, m_ActiveEffects.back());
	m_ActiveEffects.pop_back();
}
