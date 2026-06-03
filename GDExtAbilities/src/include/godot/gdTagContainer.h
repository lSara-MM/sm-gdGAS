#pragma once
#include "core/GameplayTag.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdTagData.h"
#include "internal/Event.h"
#include "internal/Types.h"

namespace sm
{
	struct TagSet
	{
		sm::BitSet<MAX_TAGS> tags;
		uint16 stack[MAX_TAGS] = {};
	};

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

		void SetIniTags();
		godot::TypedArray<TagData> GetTags() const;
		void SetTags(const godot::TypedArray<TagData>& tags);

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
		TagSet GetTagSet() const;
		bool HasTag(TagID id) const;

	private:
		void InitRootResource();
		void RevertParenting();

	public:
		std::function<void(TagContainer*, TagID)> OnTagAdded;
		std::function<void(TagContainer*, TagID)> OnTagRemoved;

	private:
		godot::TypedArray<TagData> m_gdTags;
		godot::Node* prevParent = nullptr;
		godot::StringName prevName;

		TagSet m_TagsSet;
		static bool s_HasLoadedRegistry;
	};
}