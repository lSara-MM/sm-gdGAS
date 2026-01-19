#pragma once
#include "Types.h"
#include "GameplayTag.h"

#include <unordered_map>
#include <vector>
#include <godot_cpp/classes/object.hpp>

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
		void UnregisterTag(uint32 tagID);
		void UnregisterTag(godot::StringName tagName);

		void RenameTag(uint32 tagID, godot::StringName newName);
#endif // DEBUG_MODE

		bool IsNameValid(godot::StringName name) const;

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(uint32 tagID, uint32 childID) const;

		/// Returns true if `tagID` is a direct or indirect child of `childID`.
		bool HasDescendant(uint32 tagID, uint32 childID) const;

		/// Returns true if `tagID` directly contains `childID` as a child.
		bool IsParentOf(uint32 tagID, uint32 childID) const;
		bool IsChildOf(uint32 tagID, uint32 parentID) const;

		private:

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		uint32 _GenerateUID() { return m_NextUID++; }

		GameplayTag* _GetTag(uint32 id);
		const GameplayTag* _GetTag(uint32 id) const;
		GameplayTag* _GetTag(godot::StringName tagName);
		const GameplayTag* _GetTag(godot::StringName tagName) const;
		GameplayTag& _GetTagRef(uint32 id);

		godot::StringName _GetParent(uint32 id);
		godot::TypedArray<godot::StringName> _GetChildren(uint32 id);

		void _GetAscendantsTree(uint32 itrTagID, godot::TypedArray<godot::StringName>& ascendants);
		void _GetDescendantsTree(uint32 itrTagID, godot::TypedArray<godot::StringName>& descendants);

		void _ExtractSubTags(godot::StringName& fullName);
		godot::StringName _NormalizeName(godot::StringName name) const;
		godot::StringName _AddRoot(godot::StringName name) const;
		godot::StringName _GetFullName(godot::StringName tagName) const;

		sm::GameplayTag& _AddEntry(godot::StringName name, uint32 idParent = 0);
		void _AddChild(GameplayTag& tag, uint32 idChild);

	private:
		const godot::StringName ROOT;
		static TagRegistry* m_InstancePtr;
		uint32 m_NextUID = 0;

		std::vector<GameplayTag> m_Tags;
		std::unordered_map<uint32, size_t> m_IDtoIndex;
		std::unordered_map<godot::StringName, uint32> m_NameToID;
		std::unordered_map<godot::StringName, godot::StringName> m_SuffixToFullPaths;

		std::unordered_map<uint32, godot::TypedArray<godot::StringName>> m_AscendantsCache;
		std::unordered_map<uint32, godot::TypedArray<godot::StringName>> m_DescendantsCache;

#ifdef TOOLS_DEBUG_VS
		std::unordered_map<std::string, uint32> m_StdNameToID;
		std::unordered_map<std::string, std::string> m_StdSuffixToFullPaths;
#endif //  TOOLS_DEBUG_VS
	};
}