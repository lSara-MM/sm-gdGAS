#pragma once
#include "Types.h"
#include "UID.h"
#include "GameplayModifier.h"

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

		GameplayEffect(EffectID id, GameplayEffect::Type type/*, uint32 target, uint32 source*/, float time = 0);

		EffectID GetUID() const { return m_UID; }
		void Tick(float dt);

		void AddModifier(ModifierHandle id);
		std::vector<ModifierHandle> GetModifierHandles() const { return m_Modifiers; };

	private:
		const EffectID m_UID;
		std::vector<ModifierHandle> m_Modifiers;
		/*uint32 m_TargetID;
		uint32 m_SourceID;*/
		float m_RemainingTime;

		sm::DumbUID m_ModifiersUID;
		GameplayEffect::Type m_EffectType;
	};
}