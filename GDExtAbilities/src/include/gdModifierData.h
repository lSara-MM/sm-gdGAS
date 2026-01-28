#pragma once
#include "Types.h"

#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class ModifierData : public godot::Resource
	{
		GDCLASS(ModifierData, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		enum OperationType : uint8_t
		{
			Add = 0,
			Multiply,
			Percent,
			Override
		};

		ModifierData::OperationType GetOperationType() { return operation; };
		void SetOperationType(ModifierData::OperationType t) { operation = t; };
		uint32 GetTargetID() { return targetID; };
		void SetTargetID(uint32 id) { targetID = id; };
		uint32 GetSourceID() { return sourceID; };
		void SetSourceID(uint32 id) { sourceID = id; };
		float GetValue() { return value; };
		void SetValue(float v) { value = v; };

	private:
		OperationType operation;
		uint32 targetID;
		uint32 sourceID;
		float value;
	};
}

VARIANT_ENUM_CAST(sm::ModifierData::OperationType);