#pragma once
#include "Types.h"
#include "GameplayTag.h"

#include <unordered_map>
#include <vector>
#include <godot_cpp/classes/object.hpp>

namespace sm
{
	class TagRegistry final : public godot::Object
	{
		GDCLASS(TagRegistry, godot::Object)

	protected:
		static void _bind_methods();

	public:
		static TagRegistry& GetInstance();

		godot::StringName GetTag(godot::StringName name);
		godot::StringName GetParent(godot::StringName name);
		godot::TypedArray<godot::StringName> GetChildren(godot::StringName name);
		godot::TypedArray<godot::StringName> GetDescendants(godot::StringName name);

		void RegisterTag(godot::StringName tagKey);
		void UnregisterTag(uint32 tagID);
		void UnregisterTag(godot::StringName tagKey);

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(uint32 tagID, uint32 childID) const;

		/// Returns true if `tagID` is a direct or indirect child of `childID`.
		bool HasDescendant(uint32 tagID, uint32 childID) const;

		/// Returns true if `tagID` directly contains `childID` as a child.
		bool IsParentOf(uint32 tagID, uint32 childID) const;
		bool IsChildOf(uint32 tagID, uint32 parentID) const;
		bool IsNameValid(godot::StringName name) const;

		void RenameTag(uint32 id, godot::StringName newName);

		//private:

		TagRegistry();
		~TagRegistry() = default;
		TagRegistry(const TagRegistry& obj) = delete;

		uint32 _GenerateUID() { return m_NextUID++; }

		GameplayTag* _GetTag(uint32 id);
		const GameplayTag* _GetTag(uint32 id) const;
		GameplayTag* _GetTag(godot::StringName name);
		const GameplayTag* _GetTag(godot::StringName name) const;
		GameplayTag& _GetTagRef(uint32 id);

		godot::StringName _GetParent(uint32 id);
		godot::TypedArray<godot::StringName> _GetChildren(uint32 id);

		void _ExtractSubTags(godot::StringName& fullName);
		godot::StringName _NormalizeName(godot::StringName name) const;
		godot::StringName _AddRoot(godot::StringName name) const;
		godot::StringName _GetFullName(godot::StringName name) const;

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

#ifdef TOOLS_DEBUG
		std::unordered_map<std::string, uint32> m_StdNameToID;
		std::unordered_map<std::string, std::string> m_StdSuffixToFullPaths;
#endif //  TOOLS_DEBUG
	};
}