#pragma once
#include "godot/gdGameplayAbilitySystemResource.h"
#include "internal/Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <vector>

namespace sm
{
	class TagData : public GameplayAbilitySystemResource
	{
		GDCLASS(TagData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		TagData();
		~TagData() = default;

		godot::String GetName() const { return m_Name; };
		void SetName(godot::String value);

		godot::String GetPath() const { return m_Path; };
		void SetPath(godot::String value);

		godot::String GetTagFullPath() const { return m_TagFullPath; };

		godot::TypedArray<TagData> GetChildren() const { return m_Children; };
		void SetChildren(const godot::TypedArray<TagData>& value);

		TagID GetInternalID() const { return m_InternalID; };
		void SetInternalID(TagID value) { m_InternalID = value; };

		void AddChild(godot::Ref<TagData> child);
		void RemoveChild(godot::Ref<TagData> child);
		void Reset();

	private:
		void SetTagFullPath(godot::String value);

		void UpdateChildrenParents();

	private:
		godot::String m_Name;
		godot::String m_Path;
		godot::StringName m_TagFullPath;
		godot::TypedArray<TagData> m_Children;

		TagID m_InternalID;
	};
}