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
		AttributeID targetID;
		ModifierOperationType op;
		size_t index;
	};

	struct GameplayModifier
	{
		/*GameplayModifier(ModifierID id, ModifierOperationType op, float val, EffectID source) :
			UID(id), value(val), sourceID(source), operation(op)
		{
		};*/

		GameplayModifier(ModifierID id, ModifierOperationType op, float val, EffectID source, ModifierHandle& hd) :
			UID(id), value(val), sourceID(source), operation(op), handle(hd)
		{
		};

		const ModifierID UID;
		ModifierOperationType operation;
		float value;
		EffectID sourceID;
		ModifierHandle handle;
	};
}