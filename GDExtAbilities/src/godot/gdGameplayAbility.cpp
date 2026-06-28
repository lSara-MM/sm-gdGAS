#include "godot/gdGameplayAbility.h"

#include "godot/gdAttributeContainer.h"
#include "godot/gdTagContainer.h"

void sm::GameplayAbility::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_data"), &GetAbilityData);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_data", "data"), &SetAbilityData);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_data"), &TryActivate);
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_data"), &TryActivate);

	GDVIRTUAL_BIND(_check_availability);
	GDVIRTUAL_BIND(_try_activate);
	GDVIRTUAL_BIND(_end_ability, "was_cancelled");
}

void sm::GameplayAbility::Grant()
{}

void sm::GameplayAbility::Revoke()
{}

void sm::GameplayAbility::CleanUp()
{}

bool sm::GameplayAbility::TryActivate()
{
	if (!CanActivate())
	{
		return false;
	}

	if (!CommitAbility())
	{

	}

	if (GDVIRTUAL_IS_OVERRIDDEN(_try_activate))
	{
		GDVIRTUAL_CALL(_try_activate);
	}

	return true;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();
	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

	return attr->GetCurrent() >= abilityData->GetCost();
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

	return false;
}

bool sm::GameplayAbility::CheckCooldown() const
{
	return m_CurrentCooldownRemaining > abilityData->GetCooldown();
}

bool sm::GameplayAbility::CommitAbility()
{
	if (!ApplyCost() || !ApplyCooldown())
	{
		return false;
	}

	// 

	return true;
}

bool sm::GameplayAbility::ApplyCost()
{
	return false;
}

bool sm::GameplayAbility::ApplyCooldown()
{
	m_CurrentCooldownRemaining = abilityData->GetCooldown();
	return true;
}

bool sm::GameplayAbility::CanActivate()
{
	bool ret = false;
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_activate))
	{
		GDVIRTUAL_CALL(_check_availability, ret);
	}

	return (state == AbilityState::Idle) &&
		CheckCost() &&
		CheckTags() &&
		CheckCooldown() &&
		ret;
}

bool sm::GameplayAbility::IsActive() const
{
	return false;
}
