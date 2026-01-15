#pragma once
#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	struct GameplayAttribute
	{
		GameplayAttribute(float base = 0.0f) : baseValue(base), currentValue(base)
		{

		}

		float baseValue;
		float currentValue;
	};

	class Attribute : public godot::Resource
	{
		GDCLASS(Attribute, godot::Resource)

	protected:
		static void _bind_methods();

	public:

	private:
		Attribute() = default;
		~Attribute() = default;

		float GetBaseValue();
		void SetBaseValue(float value);

		godot::StringName GetName();
		void SetName(godot::StringName n);

	private:
		float baseValue;
		godot::StringName name;
	};
}