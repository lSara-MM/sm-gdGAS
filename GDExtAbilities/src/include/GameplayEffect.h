#pragma once
#include "Types.h"

namespace sm
{
	class GameplayEffect
	{
	public:
		enum class Type : uint8_t
		{
			Permanent,	// Modify BaseValue. (Level ups, instant dmg/heal...)
			Temporary,	// Remove when expires. (Buffs/debuffs) 
			Persistent,	// Must be removed manually (Equipment, status effects...)
			Max
		};

	private:
		const EffectID UID;
		Type m_EffectType;
		uint32 targetID;
		uint32 sourceID;
	};
}