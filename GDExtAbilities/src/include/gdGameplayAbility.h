#pragma once
#include "gdAbilityData.h"
#include "gdGASEntity.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace sm
{
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

#pragma region GDScript API
		virtual bool V_CheckAvailability() const {};

		virtual void V_TryActivateAbility() {};
		virtual void V_EndAbility([[maybe_unused]] bool wasCancelled = false) {};
#pragma endregion

		void Grant();
		void Revoke();
		void CleanUp();

		bool TryActivate();

		bool CheckCost();
		bool CheckTags();
		bool CheckCooldown() const;
		bool CommitAbility();
		bool ApplyCost();
		bool ApplyCooldown();

		bool CanActivate();
		bool IsActive() const;

	public:
		godot::Ref<AbilityData> abilityData;
		AbilityState state;

	private:
		float m_CurrentCooldownRemaining = 0.0f;
		bool  m_IsActive;
		


		GAS_Entity* m_Entity;
	};
}