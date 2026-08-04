#pragma once
#include "godot/gdGASEntity.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace sm
{
	class AbilityData;
	class GAS_World;

	enum AbilityState
	{
		Idle,
		Activating,
		Active,
		Ending
	};

	class GameplayAbility : public godot::RefCounted
	{
		GDCLASS(GameplayAbility, godot::RefCounted)

	protected:
		static void _bind_methods();

	public:
		godot::Ref<AbilityData> GetAbilityData() const { return abilityData; }
		void SetAbilityData(const godot::Ref<AbilityData> data) { return abilityData = data; }

		GAS_Entity* GetOwner() const;
		void SetOwner(GAS_Entity* entity);

		GAS_World* GetWorld() const;
		void SetWorld(GAS_World* world);

#pragma region GDScript API

		GDVIRTUAL0R(bool, _check_availability)
			GDVIRTUAL0R(bool, _activate_ability)
			GDVIRTUAL1(_end_ability, bool)
			GDVIRTUAL0R(godot::TypedArray<GAS_Entity>, _calculate_targets)

#pragma endregion

			void CleanUp();

		bool TryActivate();
		bool TryEnd(bool wasCancelled = false);

		bool CheckCost();
		bool CheckTags();
		bool CommitAbility();
		void EndAbility();
		bool ApplyCost();
		bool ApplyCooldown();
		void ApplyEffectsToTarget(GAS_Entity* entity = nullptr);

		bool CanActivate();
		bool IsActive() const;
		bool IsOnCooldown() const;
		float GetCooldown() const;

	public:
		godot::Ref<AbilityData> abilityData;
		AbilityState state = AbilityState::Idle;

	private:
		EffectInstanceID m_CooldownEffect;
		EffectInstanceID m_CostEffect;

		GAS_Entity* m_Entity;
		GAS_World* m_World;
	};
}