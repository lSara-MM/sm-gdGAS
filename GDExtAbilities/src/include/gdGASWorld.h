#pragma once
#include "gdGameplayAbilitySystemNode.h"

namespace sm
{
	class EffectSystem;

	class GAS_World : public GameplayAbilitySystem
	{
		GDCLASS(GAS_World, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	private:
		GAS_World();
		GAS_World(const GAS_World&) = delete;

	public:
		static GAS_World* GetSingleton()
		{
			return m_Instance;
		}

		void _notification(int notification);

	public:
		bool enableEffects = false;
		bool enableAbility = false;

	private:
		static GAS_World* m_Instance;
		std::unique_ptr<EffectSystem> m_EffectsSystem;
	};
}