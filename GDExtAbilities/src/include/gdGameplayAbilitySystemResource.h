#pragma once
#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class GameplayAbilitySystemResource : public godot::Resource
	{
		GDCLASS(GameplayAbilitySystemResource, godot::Resource)

	protected:
		static void _bind_methods() {};

	private:
	};
}