#pragma once
#include "Types.h"

#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class Modifier : public godot::Resource
	{
		GDCLASS(Modifier, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		enum OperationType : int
		{
			Add = 0,
			Multiply,
			Percent,
			Override
		};

	private:
		Modifier::OperationType GetOperationType() { return operation; };
		void SetOperationType(Modifier::OperationType t) { operation = t; };
		uint32 GetTargetID() { return targetID; };
		void SetTargetID(uint32 id) { targetID = id; };
		uint32 GetSourceID() { return sourceID; };
		void SetSourceID(uint32 id) { sourceID = id; };
		float GetValue() { return value; };
		void SetValue(float v) { value = v; };

	public:
		OperationType operation;
		uint32 targetID;
		uint32 sourceID;
		float value;
	};
}

VARIANT_ENUM_CAST(sm::Modifier::OperationType);