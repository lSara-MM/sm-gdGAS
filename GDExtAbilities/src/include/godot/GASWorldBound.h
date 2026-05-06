#pragma once
#include "godot/gdGameplayAbilitySystemNode.h"

namespace sm
{
	class GAS_World;

	class WorldBound
	{
	public:
		// Get world if set. If not set, search for it and set it.
		GAS_World* GetOrInitWorld(GameplayAbilitySystem* owner);

		GAS_World* GetWorld(GameplayAbilitySystem* owner) { return m_World; };
		void SetWorld(GAS_World* world);

		void OnExitTree() { m_World = nullptr; };

		void PrintWorld();

	protected:
		GAS_World* m_World = nullptr;
	};

}