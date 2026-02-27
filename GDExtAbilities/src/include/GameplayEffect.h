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

		GameplayEffect(EffectID id, GameplayEffect::Type type, EntityID target, EntityID source, float time = 0);

		EffectID GetID() const { return m_ID; }
		EntityID GetTargetID() const { return m_TargetID; }
		EntityID GetSourceUID() const { return m_SourceID; }
		bool HasExpired() const;

		void Tick(float dt);

		void AddModifier(ModifierHandle id);
		std::vector<ModifierHandle>& GetModifierHandles() { return m_Modifiers; };

	private:
		const EffectID m_ID;
		std::vector<ModifierHandle> m_Modifiers;
		EntityID m_TargetID;
		EntityID m_SourceID;
		float m_RemainingTime;

		sm::DumbUID m_ModifiersUID;
		GameplayEffect::Type m_EffectType;
	};
}