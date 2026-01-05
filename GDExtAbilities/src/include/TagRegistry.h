#pragma once
#include "Types.h"
#include "GameplayTag.h"

#include <unordered_map>
#include <vector>

namespace sm
{
	class TagRegistry
	{
	public:
		static TagRegistry& GetInstance();

		godot::StringName GetTag(uint32 id);
		godot::StringName GetTag(godot::StringName name);
		godot::StringName GetTagName(uint32 id);

		godot::StringName GetParent(uint32 id);
		godot::StringName GetParent(godot::StringName name);
		godot::TypedArray<godot::StringName> GetChildren(uint32 id);
		godot::TypedArray<godot::StringName> GetChildren(godot::StringName name);
		//godot::TypedArray<godot::StringName> GetDescendants(uint32 id);
		//godot::TypedArray<godot::StringName> GetDescendants(godot::StringName name);

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
		void operator =(const TagRegistry& obj) = delete;

		uint32 _GenerateUID() { return m_NextUID++; }

		GameplayTag* _GetTag(uint32 id);
		const GameplayTag* _GetTag(uint32 id) const;
		GameplayTag* _GetTag(godot::StringName name);
		GameplayTag& _GetTagRef(uint32 id);

		godot::StringName _NormalizeName(godot::StringName name) const;
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
	};
}