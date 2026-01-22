#pragma once
#include "GameplayTag.h"
#include "Types.h"

#include <godot_cpp/classes/object.hpp>
#include <unordered_map>
#include <vector>

namespace sm
{
	class TagRegistry final
	{
	public:
		static TagRegistry& GetInstance();

#ifdef DEBUG_MODE
		TagID GetTag(TagID tagID);
		TagID GetParent(TagID tagID);
		godot::TypedArray<TagID> GetAscendants(TagID tagID);
		godot::TypedArray<TagID> GetChildren(TagID tagID);
		godot::TypedArray<TagID> GetDescendants(TagID tagID);

		void RegisterTag(TagID tagID);
		void UnregisterTag(TagID tagID);

		void RenameTag(TagID tagID, TagID newName);
#endif // DEBUG_MODE

		bool IsNameValid(godot::StringName name) const;

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` is a direct or indirect child of `childID`.
		bool HasDescendant(TagID tagID, TagID childID) const;

		/// Returns true if `tagID` directly contains `childID` as a child.
		bool IsParentOf(TagID tagID, TagID childID) const;
		bool IsChildOf(TagID tagID, TagID parentID) const;

		private:

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		GameplayTag* _GetTag(TagID id);
		const GameplayTag* _GetTag(TagID id) const;

		void _GetAscendantsTree(TagID itrTagID, godot::TypedArray<TagID>& ascendants);
		void _GetDescendantsTree(TagID itrTagID, godot::TypedArray<TagID>& descendants);

		void _ExtractSubTags(TagID fullName);
		godot::StringName _NormalizeID(godot::StringName name) const;
		godot::StringName _AddRoot(godot::StringName name) const;
		TagID _GetFullID(TagID tagID) const;

		sm::GameplayTag& _AddEntry(godot::StringName name, TagID idParent = godot::StringName("<"));
		void _AddChild(GameplayTag* tag, TagID idChild);

	private:
		const godot::StringName ROOT;
		static TagRegistry* m_InstancePtr;

		std::vector<GameplayTag> m_Tags;
		std::unordered_map<TagID, size_t> m_NameToIndex;
		std::unordered_map<godot::StringName, TagID> m_DisplayNamesToID;
		std::unordered_map<TagID, godot::StringName> m_SuffixToFullPaths;

		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_AscendantsCache;
		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_DescendantsCache;

#ifdef TOOLS_DEBUG_VS
		std::unordered_map<std::string, TagID> m_StdNameToID;
		std::unordered_map<std::string, std::string> m_StdSuffixToFullPaths;
#endif //  TOOLS_DEBUG_VS
	};
}