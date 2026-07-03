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

	godot::ClassDB::bind_method(godot::D_METHOD("commit_ability"), &CommitAbility);

	godot::ClassDB::bind_method(godot::D_METHOD("apply_effects_to_target", "entity"), &ApplyEffectsToTarget);

	GDVIRTUAL_BIND(_check_availability);
	GDVIRTUAL_BIND(_activate_ability);
	GDVIRTUAL_BIND(_end_ability, "was_cancelled");
	GDVIRTUAL_BIND(_calculate_targets);
}

void sm::GameplayAbility::SetOwner(GAS_Entity* entity)
{
	m_Entity = entity;
	m_World = entity->GetWorld();
}

void sm::GameplayAbility::CleanUp()
{}

bool sm::GameplayAbility::TryActivate()
{
	if (!CanActivate())
	{
		return false;
	}

	state = AbilityState::Active;

	if (GDVIRTUAL_IS_OVERRIDDEN(_activate_ability))
	{
		GDVIRTUAL_CALL(_activate_ability);
		return true;
	}

	return false;
}

bool sm::GameplayAbility::TryEnd(bool wasCancelled)
{
	if (IsActive())
	{
		if (GDVIRTUAL_IS_OVERRIDDEN(_end_ability))
		{
			GDVIRTUAL_CALL(_end_ability, wasCancelled);
		}
	}

	return true;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();
	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

	godot::Ref<EffectData> effect = abilityData->GetCostData();
	auto modifiers = effect->GetModifiers();
	godot::Ref<ModifierData> m = modifiers[0];
	return attr->GetCurrent() >= m->GetValue();
}

bool sm::GameplayAbility::CheckTags()
{
	bool ret = true;
	const TagContainer* tagContainer = m_Entity->GetTagContainer();

	godot::PackedInt32Array required = abilityData->GetActivationTags();

	if (ret && !tagContainer->HasAllTags(required))
	{
		ret = false;
	}

	godot::PackedInt32Array blocking = abilityData->GetActivationBlockedTags();
	if (ret && tagContainer->HasAnyTag(blocking))
	{
		ret = false;
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
		return !effect->HasExpired();
	}

	return false;
}

float sm::GameplayAbility::GetCooldown() const
{
	EffectSystem* es = m_World->GetEffectSystem();
	ERR_FAIL_COND_V_MSG(!es, false, "EffectSystem is null");

	GameplayEffect* effect = es->FindEffect(m_CooldownEffect);
	ERR_FAIL_COND_V_MSG(!effect, false, "GameplayEffect not found");

	return !effect->GetCurrentCooldown();
}

bool sm::GameplayAbility::CommitAbility()
{
	if (!ApplyCost() || !ApplyCooldown())
	{
		return false;
	}

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