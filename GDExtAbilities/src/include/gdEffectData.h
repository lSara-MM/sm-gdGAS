#pragma once
#include "gdModifierData.h"
#include "gdGameplayAbilitySystemResource.h"

namespace sm
{
	class EffectData : public GameplayAbilitySystemResource
	{
		GDCLASS(EffectData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		enum Type : uint8_t
		{
			Permanent,	// Modify BaseValue. (Level ups, instant dmg/heal...)
			Temporary,	// Remove when expires. (Buffs/debuffs) 
			Persistent,	// Must be removed manually (Equipment, status effects...)
			Max
		};

		godot::TypedArray<ModifierData> GetModifiers() const { return m_Modifiers; };
		void SetModifiers(godot::TypedArray<ModifierData> modifiers) { m_Modifiers = modifiers; };

		godot::StringName GetName() const { return m_Name; };
		void SetName(godot::StringName id) { m_Name = id; };

		EffectData::Type GetEffectType() const { return m_EffectType; };
		void SetEffectType(EffectData::Type lt);

		float GetDuration() const { return m_Duration; };
		void SetDuration(float time) { m_Duration = time; };

	private:
		void _validate_property(godot::PropertyInfo& property) const;

	private:
		EffectID m_Name;
		godot::TypedArray<ModifierData> m_Modifiers;
		Type m_EffectType = Type::Permanent;
		float m_Duration;
	};
}

VARIANT_ENUM_CAST(sm::EffectData::Type);