#pragma once
#include "Types.h"

namespace sm
{
	struct GameplayModifier
	{
		enum class OperationType : uint8_t
		{
			Add = 0,		// Add to CurrentValue
			Multiply,		// Multiply to CurrentValue
			PercentAdd,		// Add x% of Base to CurrentValue
			PercentStack,	// Multiply x% of CurrentValue to CurrentValue
			Override,		// Ignore all modifiers and substitute CurrentValue
			Max
		};

		GameplayModifier(ModifierID id, OperationType op, float val, EffectID source) :
			UID(id), value(val), sourceID(source), operation(op)
		{};

		const ModifierID UID;
		OperationType operation;
		float value;
		EffectID sourceID;
	};
}