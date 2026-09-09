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

	godot::ClassDB::bind_method(godot::D_METHOD("apply_effects_to_target", "entity"), &ApplyEffectsToTarget);

	GDVIRTUAL_BIND(_check_availability);
	GDVIRTUAL_BIND(_activate_ability);
	GDVIRTUAL_BIND(_end_ability, "was_cancelled");
	GDVIRTUAL_BIND(_can_apply_effect, "effect");
	GDVIRTUAL_BIND(_get_effect_id, "effect", "instance_id");
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

#ifdef DEV_BUILD
	auto script = abilityData->GetAbilityScript();
	auto code = script->get_source_code().strip_edges();
	auto debug = ToStdString(code);
#endif // DEV_BUILD

	bool ret = false;
	if (GDVIRTUAL_IS_OVERRIDDEN(_activate_ability))
	{
		GDVIRTUAL_CALL(_activate_ability, ret);

		if (ret)
		{
			state = AbilityState::Activating;
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
		return true;
	}

	return false;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();
	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

	if (!attr)
	{
		return true;
	}

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
	//GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	//if (effect)
	//{
	//	TagID tag = effect->GetTagsToAdd()[0];
	//	auto* container = m_Entity->GetTagContainer();
	//	return container->HasTag(tag);
	//}

	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	if (effect)
	{
		//#if defined(DEV_BUILD) && defined(DEBUG_ENABLED)
		//		if (!effect->HasExpired())
		//		{
		//			WARN_PRINT_ED("Ability is on cooldown.");
		//		}
		//#endif // DEV_BUILD && DEBUG_ENABLED
		return !effect->HasExpired();
	}

	return false;
}

float sm::GameplayAbility::GetCooldown() const
{
	EffectSystem* es = m_World->GetEffectSystem();
	ERR_FAIL_COND_V_MSG(!es, 0.0f, "EffectSystem is null");

	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	if (!effect)
	{
		return 0.0f;
	}

	return effect->GetCurrentCooldown();
}

bool sm::GameplayAbility::CommitAbility()
{
	if (!CanActivate())
	{
		return false;
	}

	state = AbilityState::Activating;

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
	state = AbilityState::Active;

	for (int i = 0; i < effectsToApply.size(); i++)
	{
		godot::Ref<EffectData> effect = effectsToApply[i];
		if (GDVIRTUAL_IS_OVERRIDDEN(_can_apply_effect))
		{
			bool ret = true;
			GDVIRTUAL_CALL(_can_apply_effect, effect, ret);

			if (!ret)
			{
				continue;
			}
		}

		if (effect.is_valid())
		{
			int idRet = entity->AddEffect(effect);
			if (GDVIRTUAL_IS_OVERRIDDEN(_get_effect_id))
			{
				GDVIRTUAL_CALL(_get_effect_id, effect, idRet);
			}
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
	return state == AbilityState::Active || state == AbilityState::Activating;
}