#pragma once
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdGASEntity.h"
#include "internal/smUID.h"

#include <unordered_set>

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
		~GAS_World() = default;

	public:
		int GetEntitiesCount() const { return m_EntitiesRegistry.size() - 1; };

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
		DumbUID m_EntityUIDs;

		std::unique_ptr<EffectSystem> m_EffectsSystem;

		// ID: 0 = invalid entity
		std::unordered_map<EntityID, GAS_Entity*> m_EntitiesRegistry;
		std::unordered_set<GAS_Entity*> m_Entities;
	};
}
