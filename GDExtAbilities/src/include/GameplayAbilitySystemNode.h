#pragma once

#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class GameplayAbilitySystem : public godot::Node
	{
		GDCLASS(GameplayAbilitySystem, godot::Node)

	protected:
		static void _bind_methods() {};

	private:
	};
}