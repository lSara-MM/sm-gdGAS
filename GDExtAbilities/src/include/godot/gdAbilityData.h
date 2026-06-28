#pragma once
#include "godot/gdEffectData.h"
#include "godot/gdGameplayAbilitySystemResource.h"
#include "internal/Types.h"

#include <vector>

namespace sm
{
	class AbilityData : public GameplayAbilitySystemResource
	{
		GDCLASS(AbilityData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:

#pragma region Godot public
		AbilityID GetAbilityID() const { return m_ID; };
		void SetAbilityID(godot::StringName name);

		TagID GetAbilityTag() const { return m_AbilityTag; };
		void SetAbilityTag(TagID id);

		float GetCooldown() const { return m_Cooldown; };
		void SetCooldown(float value);

		float GetCost() const { return m_Cost; };
		void SetCost(float value) { m_Cost = value; };

		AttributeID GetCostAttributeID() const { return m_CostAttributeID; };
		void SetCostAttributeID(AttributeID value) { m_CostAttributeID = value; };

		// Tags
		godot::PackedInt32Array GetAbilityTags() const { return m_AbilityTags; };
		void SetAbilityTags(godot::PackedInt32Array arr);

		/*godot::PackedInt32Array GetCancelAbilityTags() const { return m_CancelAbilitiesWithTag; };*/
		godot::PackedInt32Array GetBlockAbilityTags() const { return m_BlockAbilitiesWithTag; };
		void SetBlockAbilityTags(godot::PackedInt32Array arr);

		godot::PackedInt32Array GetActivationTags() const { return m_ActivationRequirements; };
		void SetActivationTags(godot::PackedInt32Array arr);

		godot::PackedInt32Array GetActivationBlockedTags() const { return m_ActivationBlocked; };
		void SetActivationBlockedTags(godot::PackedInt32Array arr);
#pragma endregion Godot public 

	private:
		AbilityData();
		~AbilityData() = default;
		//void _OnAbilityName(godot::StringName newName);

	private:
		TagID m_AbilityTag;

		// GameplayTags that the GameplayAbility owns.
		// These are just GameplayTags to describe the GameplayAbility.
		godot::PackedInt32Array m_AbilityTags;

		//// Other GameplayAbilities that have these GameplayTags in their Ability Tags will be canceled when this GameplayAbility is activated.
		//godot::PackedInt32Array m_CancelAbilitiesWithTag;

		// Other GameplayAbilities that have these GameplayTags in their Ability Tags are blocked from activating while this GameplayAbility is active.
		godot::PackedInt32Array m_BlockAbilitiesWithTag;

		// This GameplayAbility can only be activated if the owner has all of these GameplayTags.
		godot::PackedInt32Array m_ActivationRequirements;

		// This GameplayAbility cannot be activated if the owner has any of these GameplayTags.
		godot::PackedInt32Array m_ActivationBlocked;

		/* TODO:
		Source Required Tags
		This GameplayAbility can only be activated if the Source has all of these GameplayTags.
		The Source GameplayTags are only set if the GameplayAbility is triggered by an event.
		*
		Source Blocked Tags
		This GameplayAbility cannot be activated if the Source has any of these GameplayTags.
		The Source GameplayTags are only set if the GameplayAbility is triggered by an event.
		*
		Target Required Tags
		This GameplayAbility can only be activated if the Target has all of these GameplayTags.
		The Target GameplayTags are only set if the GameplayAbility is triggered by an event.
		*
		Target Blocked Tags
		This GameplayAbility cannot be activated if the Target has any of these GameplayTags.
		The Target GameplayTags are only set if the GameplayAbility is triggered by an event.
		*/

		godot::Ref<EffectData> m_CooldownData;

		AbilityID m_ID;
		AttributeID m_CostAttributeID;
		float m_Cooldown = 0.0f;
		float m_Cost = 0.0f;
	};
}