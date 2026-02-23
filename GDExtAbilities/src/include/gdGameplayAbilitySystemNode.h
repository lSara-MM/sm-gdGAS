#pragma once

#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class GAS_World;

	class GameplayAbilitySystem : public godot::Node
	{
		GDCLASS(GameplayAbilitySystem, godot::Node)

	protected:
		static void _bind_methods() {};
		
	public:
		GAS_World* GetWorld() { return m_World; };
		void SetWorld(GAS_World* world) { m_World = world; };

	private:
		GAS_World* m_World = nullptr;
	};
}