#pragma once
#include "core/GameplayTag.h"
#include "godot/gdTagData.h"
#include "internal/smBitSet.h"
#include "internal/smUID.h"
#include "internal/Types.h"

#include <unordered_map>
#include <vector>

namespace sm
{
	class TagRegistry final
	{
	public:
		static TagRegistry& Instance()
		{
			static TagRegistry instance;
			return instance;
		}

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		void Essentials();
		void Init();
		void Reset();

		bool RegisterTags(const godot::Ref<sm::TagData>& tagRoot);
		GameplayTag* CreateTag(const godot::StringName& fullName, const godot::StringName& parentName);
		void RenameTag(const godot::StringName& fullName, const godot::StringName& newName);

		std::vector<GameplayTag> GetTags() const;
		TagID FindTagID(const godot::StringName& name) const;

	private:
		GameplayTag* FindGameplayTag(const godot::Ref<TagData>& tag);

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` is a direct or indirect child of `childID`.
		bool HasDescendant(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` directly contains `childID` as a child.
		bool IsParentOf(TagID tagID, TagID childID) const;
		bool IsChildOf(TagID tagID, TagID parentID) const;

	private:
		DumbUID m_IDs;
		sm::BitSet<MAX_TAGS> m_TagsSet;
		std::vector<GameplayTag> m_Tags;
		std::unordered_map<godot::StringName, TagID> m_TagsDictionary;

#ifdef DEBUG_ENABLED
		std::unordered_map<std::string, TagID> m_TagsDictionaryDebug;
#endif // DEBUG_ENABLED
	};
}