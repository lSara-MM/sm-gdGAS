#pragma once
#include "internal/Types.h"

namespace sm
{
	enum class ModifierOperationType : uint8_t
	{
		Add = 0,		// Add a flat value to CurrentValue
		Multiply,		// Raw Multiply to CurrentValue
		PercentAdd,		// Add x% of Base to CurrentValue (ex. 20%)
		PercentStack,	// Multiply CurrentValue by a percentage (ex. 20%)
		Override,		// Ignore all modifiers and replace CurrentValue
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
			ID(id), value(val), sourceID(source), operation(op)
		{
		};*/

		GameplayModifier(ModifierID id, ModifierOperationType op, float val, EffectID source, ModifierHandle& hd) :
			ID(id), value(val), sourceID(source), operation(op), handle(hd)
		{};

		/*const */ModifierID ID;
		ModifierOperationType operation;
		float value;
		EffectID sourceID;
		ModifierHandle handle;
	};
}