#pragma once
#include "godot/gdModifierData.h"
#include "godot/gdTagData.h"
#include "godot/gdGameplayAbilitySystemResource.h"

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

		godot::StringName GetName() const { return m_Name; };
		void SetName(godot::StringName id) { m_Name = id; };

		EntityID GetTargetID() const { return m_TargetID; };
		void SetTargetID(EntityID id) { m_TargetID = id; };
		EntityID GetSourceID() const { return m_SourceID; };

		godot::TypedArray<ModifierData> GetModifiers() const { return m_Modifiers; };
		void SetModifiers(const godot::TypedArray<ModifierData>& modifiers);

		godot::PackedInt32Array GetTagsToAdd() const { return m_TagsToAdd; };
		void SetTagsToAdd(const godot::PackedInt32Array& tags) { m_TagsToAdd = tags; };

		godot::PackedInt32Array GetTagsToRemove() const { return m_TagsToRemove; };
		void SetTagsToRemove(const godot::PackedInt32Array& tags) { m_TagsToRemove = tags; };

		EffectData::Type GetEffectType() const { return m_EffectType; };
		void SetEffectType(EffectData::Type lt);

		float GetDuration() const { return m_Duration; };
		void SetDuration(float time) { m_Duration = time; };

		bool AddTagToAdd(TagID id);
		bool AddTagToRemove(TagID id);
		bool HasTag(TagID id, const godot::PackedInt32Array& arr) const;

	private:
		void _validate_property(godot::PropertyInfo& property) const;

	private:
		EffectID m_Name;
		EntityID m_TargetID;
		EntityID m_SourceID;
		godot::TypedArray<ModifierData> m_Modifiers;
		godot::PackedInt32Array m_TagsToAdd;
		godot::PackedInt32Array m_TagsToRemove;
		Type m_EffectType = Type::Permanent;
		float m_Duration;
	};
}

VARIANT_ENUM_CAST(sm::EffectData::Type);