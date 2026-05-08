#pragma once
#include "internal/Types.h"
#include "core/GameplayTag.h"
#include "godot/gdTagData.h"
#include "godot/gdGameplayAbilitySystemNode.h"

namespace sm
{
	class TagContainer final : public GameplayAbilitySystem
	{
		GDCLASS(TagContainer, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		TagContainer();
		~TagContainer();

		void _ready();
		void OnEnterTree() override;
		void OnExitTree() override;
		void OnParented() override;
		void OnUnparented() override;
		void OnChildOrderChanged() override;

		//godot::TypedArray<godot::StringName> GetTags() const { return m_gdTags; };
		//void SetTags(const godot::TypedArray<godot::StringName>& tags) { m_gdTags = tags; };

		godot::TypedArray<TagData> GetTags() const { return m_gdTags; };
		void SetTags(const godot::TypedArray<TagData>& tags) { m_gdTags = tags; };

		void AddTag(const godot::Ref<TagData>& tag);
		void AddTagFromPath(const godot::String& tag);
		void RemoveTag(const godot::Ref<TagData>& tag);
		void RemoveTagFromPath(const godot::String& tag);

		bool HasTag(const godot::Ref<TagData>& tag) const;
		bool HasTagFromPath(const godot::String& tag) const;

		bool HasAllTags(const godot::Array& tags) const;
		bool HasAnyTag(const godot::Array& tags) const;

		//
		void SetTag(TagID id, bool value = true);
		void AddTags(BitSet<MAX_TAGS> tags);
		void RemoveTags(BitSet<MAX_TAGS> tags);

		// non godot
		bool HasTag(TagID id) const;

	private:
		void RevertParenting();

	private:
		//godot::TypedArray<godot::StringName> m_gdTags;
		godot::TypedArray<TagData> m_gdTags;
		godot::Node* prevParent = nullptr;
		godot::StringName prevName;

		sm::BitSet<MAX_TAGS> m_TagsSet;
		uint16 m_TagsStack[MAX_TAGS] = {};
	};
}