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

		static const godot::StringName& s_RootTag()
		{
			static godot::StringName root(">");
			return root;
		}

//#ifdef EDITOR_MODE
//		TagID GetTag(TagID tagID);
//		TagID GetParent(TagID tagID);
//		godot::TypedArray<TagID> GetAscendants(TagID tagID);
//		godot::TypedArray<TagID> GetChildren(TagID tagID);
//		godot::TypedArray<TagID> GetDescendants(TagID tagID);
//
		//bool RegisterTag(TagID tagID);
//		void UnregisterTag(TagID tagID);
//
//		void RenameTag(TagID tagID, TagID newName);
//#endif // EDITOR_MODE
//
//		GameplayTag* GetGameplayTag(TagID id);
//		const GameplayTag* GetGameplayTag(TagID id) const;
//
//		bool IsNameValid(godot::StringName name) const;
//
//		/// Returns true if `tagID` is a direct child of `childID`.
//		bool HasChild(TagID tagID, TagID childID) const;
//
//		/// Returns true if `tagID` is a direct or indirect child of `childID`.
//		bool HasDescendant(TagID tagID, TagID childID) const;
//
//		/// Returns true if `tagID` directly contains `childID` as a child.
//		bool IsParentOf(TagID tagID, TagID childID) const;
//		bool IsChildOf(TagID tagID, TagID parentID) const;

	//private:

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		/*void _GetAscendantsTree(TagID itrTagID, godot::TypedArray<TagID>& ascendants);
		void _GetDescendantsTree(TagID itrTagID, godot::TypedArray<TagID>& descendants);

		void _ExtractSubTags(TagID fullName);
		godot::StringName _NormalizeID(godot::StringName name) const;
		godot::StringName _AddRoot(godot::StringName name) const;
		TagID _GetFullID(TagID tagID) const;

		GameplayTag& _AddEntry(godot::StringName name, TagID idParent = godot::StringName("<"));
		void _AddChild(GameplayTag* tag, TagID idChild);*/

		bool RegisterTags(const godot::TypedArray<sm::TagData>& tag);
		GameplayTag* CreateTag(const godot::StringName& fullName, const godot::StringName& parentName);

		TagID FindTagID(const godot::StringName& name) const;

		GameplayTag* FindGameplayTagFromData(const godot::Ref<TagData>& tag);

	private:
		static TagRegistry* m_Instance;

		DumbUID m_IDs;
		sm::BitSet<TAG_BITSET_SIZE> m_TagsSet;
		std::vector<GameplayTag> m_Tags;
		std::unordered_map<godot::StringName, TagID> m_TagsDictionary;

		std::unordered_map<std::string, TagID> m_TagsDictionaryDebug;
//		std::unordered_map<TagID, size_t> m_NameToIndex;
//		std::unordered_map<godot::StringName, TagID> m_DisplayNamesToID;
//		std::unordered_map<TagID, godot::StringName> m_SuffixToFullPaths;
//
//		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_AscendantsCache;
//		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_DescendantsCache;
//
//#ifdef TOOLS_DEBUG_VS
//		std::unordered_map<std::string, TagID> m_StdNameToID;
//		std::unordered_map<std::string, std::string> m_StdSuffixToFullPaths;
//#endif //  TOOLS_DEBUG_VS
	};
}