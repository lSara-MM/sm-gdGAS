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
		godot::StringName GetTag(godot::StringName tagName);
		godot::StringName GetParent(godot::StringName tagName);
		godot::TypedArray<godot::StringName> GetAscendants(godot::StringName tagName);
		godot::TypedArray<godot::StringName> GetChildren(godot::StringName tagName);
		godot::TypedArray<godot::StringName> GetDescendants(godot::StringName tagName);

		void RegisterTag(godot::StringName tagName);
		void UnregisterTag(TagID tagID);
		void UnregisterTag(godot::StringName tagName);

		void RenameTag(TagID tagID, godot::StringName newName);
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

		TagID _GenerateUID() { return m_NextUID++; }

		GameplayTag* _GetTag(TagID id);
		const GameplayTag* _GetTag(TagID id) const;
		GameplayTag* _GetTag(godot::StringName tagName);
		const GameplayTag* _GetTag(godot::StringName tagName) const;
		GameplayTag& _GetTagRef(TagID id);

		godot::StringName _GetParent(TagID id);
		godot::TypedArray<godot::StringName> _GetChildren(TagID id);

		void _GetAscendantsTree(TagID itrTagID, godot::TypedArray<godot::StringName>& ascendants);
		void _GetDescendantsTree(TagID itrTagID, godot::TypedArray<godot::StringName>& descendants);

		void _ExtractSubTags(godot::StringName& fullName);
		godot::StringName _NormalizeName(godot::StringName name) const;
		godot::StringName _AddRoot(godot::StringName name) const;
		godot::StringName _GetFullName(godot::StringName tagName) const;

		sm::GameplayTag& _AddEntry(godot::StringName name, TagID idParent = 0);
		void _AddChild(GameplayTag& tag, TagID idChild);

	private:
		const godot::StringName ROOT;
		static TagRegistry* m_InstancePtr;
		TagID m_NextUID = 0;

		std::vector<GameplayTag> m_Tags;
		std::unordered_map<TagID, size_t> m_IDtoIndex;
		std::unordered_map<godot::StringName, TagID> m_NameToID;
		std::unordered_map<godot::StringName, godot::StringName> m_SuffixToFullPaths;

		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_AscendantsCache;
		std::unordered_map<TagID, godot::TypedArray<godot::StringName>> m_DescendantsCache;

#ifdef TOOLS_DEBUG_VS
		std::unordered_map<std::string, TagID> m_StdNameToID;
		std::unordered_map<std::string, std::string> m_StdSuffixToFullPaths;
#endif //  TOOLS_DEBUG_VS
	};
}