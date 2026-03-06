#pragma once
#include "gdGameplayAbilitySystemNode.h"
#include "gdGASEntity.h"

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
		static GAS_World* Instance()
		{
			return m_Instance;
		}

		bool GetEffectsAvailability() const { return enableEffects; };
		void SetEffectsAvailability(bool value);

		bool GetAbilitiesAvailability() const { return enableAbilities; };
		void SetAbilitiesAvailability(bool value);

		EffectSystem* GetEffectSystem() const { return m_EffectsSystem.get(); };
		GAS_Entity* GetEntity(EntityID id);

		EntityID RegisterEntity(GAS_Entity* entity);
		void UnregisterEntity(GAS_Entity* entity);

	private:
		void OnEnterTree() override;
		void OnExitTree() override;
		void OnReady() override;
		void OnProcess() override;

	public:
		bool enableEffects = false;
		bool enableAbilities = false;

	private:
		static GAS_World* m_Instance;
		DumbUID m_EntityUIDs;

		std::unique_ptr<EffectSystem> m_EffectsSystem;
		std::unordered_map<EntityID, GAS_Entity*> m_Entities;
	};
}