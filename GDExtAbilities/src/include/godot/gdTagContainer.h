#pragma once
#include "internal/Types.h"
#include "core/GameplayTag.h"
#include "godot/gdTagData.h"
#include "godot/gdGameplayAbilitySystemNode.h"

namespace sm
{
	class TagContainer final : public sm::GameplayAbilitySystem
	{
		GDCLASS(TagContainer, sm::GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		TagContainer();
		~TagContainer();

		void _ready();

		godot::TypedArray<godot::StringName> GetTags() const { return m_gdTags; };
		void SetTags(const godot::TypedArray<godot::StringName>& tags) { m_gdTags = tags; };

		void RegisterTags(const godot::TypedArray<sm::TagData>& tags);

		void AddTag(const godot::Ref<TagData>& tag);
		void AddTagFromPath(const godot::String& tag);
		void RemoveTag(const godot::Ref<TagData>& tag);
		void RemoveTagFromPath(const godot::String& tag);

		bool HasTag(const godot::Ref<TagData>& tag) const;
		bool HasTagFromPath(const godot::String& tag) const;

		bool HasAllTags(const godot::Array& tags) const;
		bool HasAnyTag(const godot::Array& tags) const;

		//
		void AddTags(BitSet<TAG_BITSET_SIZE> tags);
		void RemoveTags(BitSet<TAG_BITSET_SIZE> tags);

	private:
		godot::TypedArray<godot::StringName> m_gdTags;
		std::vector<godot::StringName> m_GameplayTags;

		sm::BitSet<TAG_BITSET_SIZE> m_TagsSet;
		uint16 m_TagsStack[TAG_BITSET_SIZE];
	};
}