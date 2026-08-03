#include "godot/gdGameplayAbility.h"

#include "godot/gdAttributeContainer.h"
#include "godot/gdAbilityData.h"
#include "godot/gdGASWorld.h"
#include "core/EffectSystem.h"
#include "godot/gdTagContainer.h"

void sm::GameplayAbility::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_data"), &GetAbilityData);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_data", "data"), &SetAbilityData);

	godot::ClassDB::bind_method(godot::D_METHOD("try_activate"), &TryActivate);
	godot::ClassDB::bind_method(godot::D_METHOD("try_end", "wasCancelled"), &TryEnd);
	godot::ClassDB::bind_method(godot::D_METHOD("get_entity_owner"), &GetOwner);
	godot::ClassDB::bind_method(godot::D_METHOD("commit_ability"), &CommitAbility);
	godot::ClassDB::bind_method(godot::D_METHOD("is_on_cooldown"), &IsOnCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("can_activate"), &CanActivate);

	godot::ClassDB::bind_method(godot::D_METHOD("apply_effects_to_target", "entity"), &ApplyEffectsToTarget, DEFVAL(nullptr));

	GDVIRTUAL_BIND(_check_availability);
	GDVIRTUAL_BIND(_activate_ability);
	GDVIRTUAL_BIND(_end_ability, "was_cancelled");
	GDVIRTUAL_BIND(_calculate_targets);
}

sm::GAS_Entity* sm::GameplayAbility::GetOwner() const
{
	return m_Entity;
}

void sm::GameplayAbility::SetOwner(GAS_Entity* entity)
{
	m_Entity = entity;
	m_World = entity->GetWorld();
}

sm::GAS_World* sm::GameplayAbility::GetWorld() const
{
	return m_World;
}

void sm::GameplayAbility::SetWorld(GAS_World* world)
{
	m_World = world;
}

void sm::GameplayAbility::CleanUp()
{}

bool sm::GameplayAbility::TryActivate()
{
	if (!CanActivate())
	{
		return false;
	}

	bool ret = false;

	if (GDVIRTUAL_IS_OVERRIDDEN(_activate_ability))
	{
		GDVIRTUAL_CALL(_activate_ability, ret);

		if (ret)
		{
			state = AbilityState::Active;
		}
	}
	else if (ret = CommitAbility())
	{
		godot::TypedArray<GAS_Entity> targets;
		if (GDVIRTUAL_IS_OVERRIDDEN(_calculate_targets))
		{
			GDVIRTUAL_CALL(_calculate_targets, targets);

			for (size_t i = 0; i < targets.size(); i++)
			{
				godot::Object* obj = targets[i];
				GAS_Entity* entity = godot::Object::cast_to<GAS_Entity>(obj);
				ApplyEffectsToTarget(entity);
			}
		}
		else
		{
			ApplyEffectsToTarget();
		}

		state = AbilityState::Active;
		TryEnd(false);
	}

	return ret;
}

bool sm::GameplayAbility::TryEnd(bool wasCancelled)
{
	if (IsActive())
	{
		state = AbilityState::Ending;

		if (GDVIRTUAL_IS_OVERRIDDEN(_end_ability))
		{
			GDVIRTUAL_CALL(_end_ability, wasCancelled);
		}

		state = AbilityState::Idle;
	}

	return true;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();
	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

#ifdef DEBUG_ENABLED
	if (!(attr->GetCurrent() >= abilityData->GetCost()))
	{
		WARN_PRINT_ED("Ability cost too high. Not enough resources.");
	}
#endif // DEBUG_ENABLED

	return attr->GetCurrent() >= abilityData->GetCost();
}

bool sm::GameplayAbility::CheckTags()
{
	bool ret = true;
	const TagContainer* tagContainer = m_Entity->GetTagContainer();

	godot::PackedInt32Array required = abilityData->GetActivationTags();

	if (!tagContainer->HasAllTags(required))
	{
		ret = false;

#ifdef DEBUG_ENABLED
		WARN_PRINT_ED("Ability does not have all required tags.");
#endif // DEBUG_ENABLED
	}

	godot::PackedInt32Array blocking = abilityData->GetActivationBlockedTags();
	if (ret && tagContainer->HasAnyTag(blocking))
	{
		ret = false;

#ifdef DEBUG_ENABLED
		WARN_PRINT_ED("Ability has one of the blocking tags.");
#endif // DEBUG_ENABLED
	}

	return ret;
}

bool sm::GameplayAbility::IsOnCooldown() const
{
	EffectSystem* es = m_World->GetEffectSystem();
	ERR_FAIL_COND_V_MSG(!es, false, "EffectSystem is null");
	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	if (effect)
	{
		TagID tag = effect->GetTagsToAdd()[0];
		auto* container = m_Entity->GetTagContainer();
		return container->HasTag(tag);
	}


	//	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	//	if (effect)
	//	{
	//#ifdef DEBUG_ENABLED
	//		if (!effect->HasExpired())
	//		{
	//			WARN_PRINT_ED("Ability is on cooldown.");
	//		}
	//#endif // DEBUG_ENABLED
	//		return !effect->HasExpired();
	//	}

	return false;
}

float sm::GameplayAbility::GetCooldown() const
{
	EffectSystem* es = m_World->GetEffectSystem();
	ERR_FAIL_COND_V_MSG(!es, false, "EffectSystem is null");

	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	ERR_FAIL_COND_V_MSG(!effect, false, "GameplayEffect not found");

	return effect->GetCurrentCooldown();
}

bool sm::GameplayAbility::CommitAbility()
{
	if (!CanActivate())
	{
		return false;
	}

	ApplyCost();
	ApplyCooldown();

	return true;
}

void sm::GameplayAbility::EndAbility()
{
	state = AbilityState::Idle;

	if (GDVIRTUAL_IS_OVERRIDDEN(_end_ability))
	{
		GDVIRTUAL_CALL(_end_ability, false);
	}
}

bool sm::GameplayAbility::ApplyCost()
{
	godot::Ref<EffectData> effect = abilityData->GetCostData();

	if (effect.is_valid())
	{
		m_CostEffect = m_Entity->AddEffect(effect);
		return true;
	}

	return false;
}

bool sm::GameplayAbility::ApplyCooldown()
{
	godot::Ref<EffectData> effect = abilityData->GetCooldonwData();
	if (effect.is_valid())
	{
		m_CooldownEffect = m_Entity->AddEffect(effect);
		return true;
	}

	return false;
}

void sm::GameplayAbility::ApplyEffectsToTarget(GAS_Entity* entity)
{
	if (!entity)
	{
		entity = m_Entity;
	}

	ERR_FAIL_NULL_MSG(entity, "ApplyEffects failed. Target was <null>");

	godot::TypedArray<EffectData> effectsToApply = abilityData->GetEffects();

	for (int i = 0; i < effectsToApply.size(); i++)
	{
		godot::Ref<EffectData> effect = effectsToApply[i];
		if (effect.is_valid())
		{
			entity->AddEffect(effect);
		}
	}
}

bool sm::GameplayAbility::CanActivate()
{
	bool ret = true;
	if (GDVIRTUAL_IS_OVERRIDDEN(_check_availability))
	{
		GDVIRTUAL_CALL(_check_availability, ret);
	}

	return (state == AbilityState::Idle) &&
		CheckCost() &&
		CheckTags() &&
		!IsOnCooldown() &&
		ret;
}

bool sm::GameplayAbility::IsActive() const
{
	return state == AbilityState::Active;
}