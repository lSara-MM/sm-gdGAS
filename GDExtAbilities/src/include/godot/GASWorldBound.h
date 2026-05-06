#pragma once
#include "godot/gdGameplayAbilitySystemNode.h"

namespace sm
{
	class GAS_World;

	class WorldBound
	{
	public:
		// Get world if set. If not set, search for it and set it.
		GAS_World* GetOrInitWorld(GameplayAbilitySystem* owner, godot::Node* sceneRootNode = nullptr);

		GAS_World* GetWorld(GameplayAbilitySystem* owner);
		void SetWorld(GAS_World* world);

		void CleanUp();

		void PrintWorld();

	protected:
		GAS_World* m_World = nullptr;
	};

}