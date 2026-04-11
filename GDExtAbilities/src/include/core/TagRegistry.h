#pragma once
#include "core/GameplayTag.h"
#include "godot/gdTagData.h"
#include "internal/smBitSet.h"
#include "internal/smUID.h"
#include "internal/Types.h"

#include <godot_cpp/classes/object.hpp>
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

		//private:

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		bool RegisterTags(const godot::TypedArray<sm::TagData>& tag);
		GameplayTag* CreateTag(const godot::StringName& fullName, const godot::StringName& parentName);

		TagID FindTagID(const godot::StringName& name) const;

		GameplayTag* FindGameplayTag(const godot::Ref<TagData>& tag);

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` is a direct or indirect child of `childID`.
		bool HasDescendant(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` directly contains `childID` as a child.
		bool IsParentOf(TagID tagID, TagID childID) const;
		bool IsChildOf(TagID tagID, TagID parentID) const;

	private:
		static TagRegistry* m_Instance;

		DumbUID m_IDs;
		sm::BitSet<TAG_BITSET_SIZE> m_TagsSet;
		std::vector<GameplayTag> m_Tags;
		std::unordered_map<godot::StringName, TagID> m_TagsDictionary;

		std::unordered_map<std::string, TagID> m_TagsDictionaryDebug;
	};
}