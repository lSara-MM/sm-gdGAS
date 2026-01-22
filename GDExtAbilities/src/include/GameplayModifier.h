#pragma once
#include "Types.h"

namespace sm
{
	enum class OperationType
	{
		Add = 0,
		Multiply,
		Percent,
		Override
	};

	struct GameplayModifier
	{
		GameplayModifier(ModifierID id, OperationType op, uint32 target, uint32 source, float val) : 
			UID(id), operation(op), targetID(target), sourceID(source), value(val) {};

		const ModifierID UID;
		OperationType operation;
		uint32 targetID;
		uint32 sourceID;
		float value;
	};
}