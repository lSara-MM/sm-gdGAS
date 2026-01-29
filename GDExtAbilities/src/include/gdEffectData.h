#pragma once
#include "gdModifierData.h"

#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class EffectData : public godot::Resource
	{
		GDCLASS(EffectData, godot::Resource)

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

		godot::TypedArray<ModifierData> GetModifiers() { return m_Modifiers; };
		void SetModifiers(godot::TypedArray<ModifierData> modifiers) { m_Modifiers = modifiers; };

		EffectData::Type GetEffectType() { return m_EffectType; };
		void SetEffectType(EffectData::Type lt) { m_EffectType = lt; };
	
	private:
		Type m_EffectType;
		godot::TypedArray<ModifierData> m_Modifiers;
	};
}

VARIANT_ENUM_CAST(sm::EffectData::Type);