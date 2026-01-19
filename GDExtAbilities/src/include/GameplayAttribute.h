#pragma once
#include "Types.h"

#include <godot_cpp/classes/resource.hpp>
#include <functional>
#include <vector>

namespace sm
{
	struct GameplayAttribute
	{
		GameplayAttribute(uint32 id = 0, float base = 0.0f, float min = 0.0f, float max = FLT_MAX) :
			UID(id), baseValue(base), currentValue(base), minValue(min), maxValue(max)
		{

		}

	private:
		uint32 UID;

	public:

		float baseValue;
		float currentValue;

		float minValue;
		float maxValue;
	};

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
		float GetBaseValue() { return baseValue; };
		void SetBaseValue(float value) { baseValue = value; };
		godot::StringName GetName() { return name; };
		void SetName(godot::StringName n) { name = n; };

		// TODO: hacer esto mejor?
		void SubscribeSetNameEvent(std::function<void(godot::StringName)> callback);

	private:
		void _NotifySetNameListeners(godot::StringName newName) const;

	private:

		// Godot
		float baseValue;
		float minValue;
		float maxValue;
		godot::StringName name;

		// Internal
		std::vector<std::function<void(godot::StringName)>> m_SetNameListeners;
	};
}