#include "gdGameplayAbility.h"

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

	V_StartAbility();

	return true;
}

bool sm::GameplayAbility::CheckCost()
{
	const AttributeContainer* attrContainer = m_Entity->GetAttributeContainer();

	GameplayAttribute* attr = attrContainer->FindAttribute(abilityData->GetCostAttributeID());

	return attr->GetCurrent() <= abilityData->GetCost();
}

bool sm::GameplayAbility::CheckCooldown() const
{
	return m_CurrentCooldownRemaining > abilityData->GetCooldown();
}

bool sm::GameplayAbility::CommitAbility()
{
	return false;
}

void sm::GameplayAbility::ApplyCost()
{

}

void sm::GameplayAbility::ApplyCooldown()
{
	//m_Entity.lock()->AddEffect();
	
	m_CurrentCooldownRemaining = abilityData->GetCooldown();
}

bool sm::GameplayAbility::CanActivate()
{
	return (state == E_AbilityState::Idle) &&
		CheckCost() &&
		V_CheckAvailability();
}

bool sm::GameplayAbility::IsActive() const
{
	return false;
}
