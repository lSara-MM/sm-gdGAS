#pragma once
#include "Types.h"

namespace sm
{
	enum class ModifierOperationType : uint8_t
	{
		Add = 0,		// Add to CurrentValue
		Multiply,		// Multiply to CurrentValue
		PercentAdd,		// Add x% of Base to CurrentValue
		PercentStack,	// Multiply x% of CurrentValue to CurrentValue
		Override,		// Ignore all modifiers and substitute CurrentValue
		Max
	};

	struct ModifierHandle
	{
		ModifierID id;
		ModifierOperationType op;
		int index;
	};

	struct GameplayModifier
	{
		GameplayModifier(ModifierID id, ModifierOperationType op, float val, EffectID source, int idx) :
			UID(id), value(val), sourceID(source), operation(op), handle({ id, op, idx })
		{
		};

		const ModifierID UID;
		ModifierOperationType operation;
		float value;
		EffectID sourceID;
		ModifierHandle handle;
	};
}