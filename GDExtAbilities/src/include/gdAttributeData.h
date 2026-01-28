#pragma once
#include "Event.h"
#include "Types.h"

#include <functional>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <vector>

namespace sm{
	class AttributeData : public godot::Resource
	{
		GDCLASS(AttributeData, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		AttributeData() : baseValue(0.0f), minValue(0.0f), maxValue(FLT_MAX), name("") {};
		AttributeData(float base, float min = 0.0f, float max = FLT_MAX, godot::StringName n = "") :
			baseValue(base), minValue(min), maxValue(max), name(n)
		{};
		~AttributeData() = default;

		float GetBaseValue() { return baseValue; };
		void SetBaseValue(float value) { baseValue = value; };
		godot::StringName GetName() { return name; };
		void SetName(godot::StringName n);

	public:
		sm::Event<godot::StringName> eventSetName;

	private:
		// Godot
		float baseValue;
		float minValue;
		float maxValue;
		godot::StringName name;
	};
}