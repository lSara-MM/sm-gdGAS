#pragma once
#include "godot/gdEffectData.h"
#include "godot/gdGameplayAbility.h"
#include "godot/gdGameplayAbilitySystemResource.h"
#include "internal/Types.h"

#include <godot_cpp/classes/script.hpp>
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
		TagID GetAbilityID() const { return m_AbilityTag; };
		void SetAbilityID(TagID id);

		godot::Ref<godot::Script> GetAbilityScript() const { return m_AbilityScript; };
		void SetAbilityScript(const godot::Ref<godot::Script>& script);

		godot::Ref<GameplayAbility> GetAbilityInstance() const { return m_AbilityInstance; };
		void SetAbilityInstance(const godot::Ref<GameplayAbility>& script);

		float GetCooldown() const { return m_Cooldown; };
		void SetCooldown(float value);
		godot::Ref<EffectData> GetCooldonwData() const { return m_CooldownData; };

		float GetCost() const { return m_Cost; };
		void SetCost(float value);
		godot::Ref<EffectData> GetCostData() const { return m_CostData; };
		void SetCostData(godot::Ref<EffectData> value) { m_CostData = value; };

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

		godot::TypedArray<EffectData> GetEffects();
		void SetEffects(const godot::TypedArray<EffectData>& effects);
#pragma endregion Godot public 

	private:
		AbilityData();
		~AbilityData() = default;
		//void _OnAbilityName(godot::StringName newName);

	private:
		godot::Ref<godot::Script> m_AbilityScript;
		godot::Ref<GameplayAbility> m_AbilityInstance;

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

		godot::TypedArray<EffectData> m_Effects;

		godot::Ref<EffectData> m_CooldownData;
		godot::Ref<EffectData> m_CostData;
		AttributeID m_CostAttributeID;
		TagID m_AbilityTag;
		float m_Cooldown = 0.0f;
		float m_Cost = 0.0f;
	};
}