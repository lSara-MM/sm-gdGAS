#pragma once
#include "Event.h"
#include "Types.h"
#include "gdGameplayAbilitySystemResource.h"

#include <functional>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <vector>

namespace sm
{
	class AttributeData : public GameplayAbilitySystemResource
	{
		GDCLASS(AttributeData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		AttributeData() : baseValue(0.0f), minValue(0.0f), maxValue(FLT_MAX), name("") {};
		AttributeData(float base, float min = 0.0f, float max = FLT_MAX, godot::StringName n = "") :
			baseValue(base), minValue(min), maxValue(max), name(n)
		{};
		~AttributeData() = default;

		float GetBaseValue() const { return baseValue; };
		void SetBaseValue(float value) { baseValue = value; };

		float GetMinValue() const { return minValue; };
		void SetMinValue(float value) { minValue = value; };

		float GetMaxValue() const { return maxValue; };
		void SetMaxValue(float value) { maxValue = value; };

		godot::StringName GetName() const { return name; };
		void SetName(godot::StringName n);

	public:
		sm::Event<godot::StringName> eventSetName;

	private:
		// Godot
		float baseValue = 0.0f;
		float minValue = 0.0f;
		float maxValue = FLT_MAX;
		AttributeID name;
	};
}