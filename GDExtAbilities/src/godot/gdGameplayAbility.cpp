#include "godot/gdGameplayAbility.h"

#include "godot/gdAttributeContainer.h"

void sm::GameplayAbility::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_data"), &GetAbilityData);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_data", "data"), &SetAbilityData);
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

	V_TryActivateAbility();

	return true;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();
	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

	return attr->GetCurrent() <= abilityData->GetCost();
}

bool sm::GameplayAbility::CheckTags()
{
	const TagContainer* tagContainer = m_Entity->GetTagContainer();

	/*if (tagContainer)
	{

	}*/

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

	return true;
}

bool sm::GameplayAbility::ApplyCost()
{
	return false;
}

bool sm::GameplayAbility::ApplyCooldown()
{
	m_CurrentCooldownRemaining = abilityData->GetCooldown();
	return false;
}

bool sm::GameplayAbility::CanActivate()
{
	return (state == AbilityState::Idle) &&
		CheckCost() &&
		CheckTags() &&
		V_CheckAvailability();
}

bool sm::GameplayAbility::IsActive() const
{
	return false;
}
