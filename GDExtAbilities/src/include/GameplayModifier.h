#pragma once
#include "Types.h"
#include "gdModifier.h"

namespace sm
{
	struct GameplayModifier
	{
		enum class OperationType : int
		{
			Add = 0,
			Multiply,
			Percent,
			Override
		};

		GameplayModifier(ModifierID id, OperationType op, uint32 target, uint32 source, float val) : 
			UID(id), operation(op), targetID(target), sourceID(source), value(val) {};

		const ModifierID UID;
		OperationType operation;
		uint32 targetID;
		uint32 sourceID;
		float value;
	};
}