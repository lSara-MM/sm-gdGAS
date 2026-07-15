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
		BitSet<MAX_TAGS> tags;
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

		void OnEnterTree() override;
		void OnExitTree() override;
		void OnParented() override;
		void OnUnparented() override;
		void OnChildOrderChanged() override;

		void SetIniTags();
		godot::PackedInt32Array GetTags() const;
		void SetTags(const godot::PackedInt32Array& tags);

		void AddTag(TagID id);
		void RemoveTag(TagID id);

		bool HasTag(TagID id) const;
		bool HasAllTags(const godot::Array& tags) const;
		bool HasAnyTag(const godot::Array& tags) const;

		//
		bool SetTag(TagID id, bool value = true);
		void AddTags(BitSet<MAX_TAGS> tags);
		void RemoveTags(BitSet<MAX_TAGS> tags);

		// non godot
		TagSet GetTagSet() const;

	private:
		void RevertParenting();

	public:
		std::function<void(TagID, TagContainer*)> OnTagAdded;
		std::function<void(TagID, TagContainer*)> OnTagRemoved;

	private:
		godot::PackedInt32Array m_gdTags;
		godot::Node* prevParent = nullptr;
		godot::StringName prevName;

		TagSet m_TagsSet;
	};
}