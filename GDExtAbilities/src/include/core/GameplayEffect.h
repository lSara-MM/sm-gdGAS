#pragma once
#include "internal/smUID.h"
#include "internal/smBitSet.h"
#include "core/GameplayModifier.h"

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

		GameplayEffect(EffectID name, EffectInstanceID id,
			GameplayEffect::Type type, EntityID target, EntityID source, float time = 0);

		EffectID GetID() const { return m_Name; }
		EffectInstanceID GetInstanceID() const { return m_ID; }
		EntityID GetTargetID() const { return m_TargetID; }
		EntityID GetSourceUID() const { return m_SourceID; }

		BitSet<MAX_TAGS> GetTagsToAdd() const { return m_TagsToAdd; }
		BitSet<MAX_TAGS> GetTagsToRemove() const { return m_TagsToRemove; }

		bool HasExpired() const;
		float GetCurrentCooldown() const;

		void Tick(float dt);

		void AddModifier(ModifierHandle id);
		std::vector<ModifierHandle>& GetModifierHandles() { return m_Modifiers; };

	private:
		/*const*/ EffectID m_Name;
		/*const*/ EffectInstanceID m_ID;
		std::vector<ModifierHandle> m_Modifiers;
		EntityID m_TargetID;
		EntityID m_SourceID;
		float m_RemainingTime;

		DumbUID m_ModifiersUID;
		GameplayEffect::Type m_EffectType;

		BitSet<MAX_TAGS> m_TagsToAdd;
		BitSet<MAX_TAGS> m_TagsToRemove;
	};
}