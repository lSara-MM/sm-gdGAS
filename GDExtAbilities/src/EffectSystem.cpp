#include "EffectSystem.h"

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
	for (auto& effect : m_ActiveEffects)
	{
		effect->Tick(dt);

		if (effect->HasExpired())
		{
			RemoveEffect(effect);
		}
	}
}

sm::GameplayEffect* sm::EffectSystem::FindEffect(EffectID effectID)
{
	auto it = std::find_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const EffectPtr& e)
		{
			return e->GetUID() == effectID;
		}
	);

	return (it != m_ActiveEffects.end()) ? it->get() : nullptr;
}

//void sm::EffectSystem::AddEffect(const godot::Ref<EffectData> gdEffect, sm::GAS_Entity* target)
//{
//	TagContainer* tagContainer = GetChildNodeOfType<TagContainer>(target);
//	ERR_FAIL_NULL_MSG(tagContainer,
//		godot::vformat("AddEffect: Could not add %s. The TagContainer was not found.",
//			ToStdString(gdEffect->GetName()).c_str()));
//
//	// TODO: Add/Remove tags
//	godot::TypedArray<TagID> tagsToAdd = gdEffect->GetTagsToAdd();
//	//tagContainer
//
//	godot::TypedArray<TagID> tagsToRemove = gdEffect->GetTagsToRemove();
//	//tagContainer
//
//	godot::TypedArray<ModifierData> modifiers = gdEffect->GetModifiers();
//	if (modifiers.is_empty())
//	{
//		return;
//	}
//
//	AttributeContainer* attrContainer = GetChildNodeOfType<AttributeContainer>(target);
//	ERR_FAIL_NULL_MSG(attrContainer,
//		godot::vformat("AddEffect: Could not add %s. The AttributeContainer was not found.",
//			ToStdString(gdEffect->GetName()).c_str()));
//
//	EffectData::Type type = gdEffect->GetEffectType();
//	if (type == EffectData::Type::Permanent)
//	{
//		for (size_t i = 0; i < modifiers.size(); i++)
//		{
//			godot::Ref<ModifierData> modifier = modifiers[i];
//			attrContainer->AddBaseModifier(modifier->GetTargetID(), modifier);
//		}
//
//		return;
//	}
//
//	auto effect = std::make_unique<GameplayEffect>(
//		gdEffect->GetName(),
//		static_cast<GameplayEffect::Type>(gdEffect->GetEffectType()),
//		gdEffect->GetTargetID(),
//		gdEffect->GetSourceID()
//	);
//
//	for (size_t i = 0; i < modifiers.size(); i++)
//	{
//		godot::Ref<ModifierData> modifier = modifiers[i];
//		GameplayAttribute* attr = attrContainer->FindAttribute(modifier->GetTargetID());
//		ModifierID id = attrContainer->AddModifier(attr, modifier);
//		ModifierHandle handle = {
//			id,
//			attr->GetUID(),
//			modifier->GetGameplayOperationType(),
//			attr->GetModifiersCount(modifier->GetGameplayOperationType()) - 1
//		};
//
//		effect->AddModifier(handle);
//	}
//
//	AddActiveEffect(effect);
//}

void sm::EffectSystem::AddActiveEffect(EffectPtr& effect)
{
	m_ActiveEffects.push_back(std::move(effect));
}

void sm::EffectSystem::RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect)
{
	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const std::unique_ptr<sm::GameplayEffect>& effect)
		{
			return effect->GetUID() == gdEffect->GetName();
		});

	RemoveEffectModifiers(id, itr);
}

void sm::EffectSystem::RemoveEffect(EffectID gdEffectID)
{
	auto itr = std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const EffectPtr& effect)
		{
			return effect->GetUID() == gdEffectID;
		});

	RemoveEffectModifiers((*itr)->GetTargetUID(), itr);
}

void sm::EffectSystem::RemoveEffect(EffectPtr& effect)
{
	GAS_World* world = GAS_World::GetSingleton();
	GAS_Entity* entity = world->GetEntity(effect->GetTargetUID());
	RemoveEffectModifiers(entity, effect);
}

void sm::EffectSystem::RemoveEffectModifiers(EntityID id, std::vector<sm::EffectSystem::EffectPtr>::iterator& itr)
{
	GAS_Entity* entity = sm::GAS_World::GetSingleton()->GetEntity(id);
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

	for (ModifierHandle& handle : effect->GetModifierHandles())
	{
		GameplayAttribute* attr = attrContainer->FindAttribute(handle.targetID);
		attr->RemoveModifier(handle);
	}

	std::swap(effect, m_ActiveEffects.back());
	m_ActiveEffects.pop_back();
}
