#pragma once
#include "Event.h"
#include "Types.h"

#include <functional>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <vector>

namespace sm{
	class Attribute : public godot::Resource
	{
		GDCLASS(Attribute, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		Attribute() : baseValue(0.0f), minValue(0.0f), maxValue(FLT_MAX), name("") {};
		Attribute(float base, float min = 0.0f, float max = FLT_MAX, godot::StringName n = "") :
			baseValue(base), minValue(min), maxValue(max), name(n)
		{};
		~Attribute() = default;

		static godot::Ref<sm::Attribute> Create(float base, godot::StringName n);
		uint32 GetID() { return UID; };
		void SetID(uint32 id) { UID = id; };
		float GetBaseValue() { return baseValue; };
		void SetBaseValue(float value) { baseValue = value; };
		godot::StringName GetName() { return name; };
		void SetName(godot::StringName n);

	public:
		sm::Event<godot::StringName> eventSetName;

	private:
		uint32 UID;

		// Godot
		float baseValue;
		float minValue;
		float maxValue;
		godot::StringName name;

		// Internal
		std::vector<std::function<void(godot::StringName)>> m_SetNameListeners;
	};
}