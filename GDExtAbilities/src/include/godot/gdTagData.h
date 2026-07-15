#pragma once
#include "godot/gdGameplayAbilitySystemResource.h"
#include "internal/Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <vector>

namespace sm
{
	inline const char* SETTINGS_PATH = "GDExtAbilities/tag_registry_path";

	class TagData : public GameplayAbilitySystemResource
	{
		GDCLASS(TagData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		TagData();
		~TagData() = default;

		godot::String GetName() const { return m_Name; };
		void SetName(const godot::String& value);

		godot::String GetPath() const { return m_Path; };
		void SetPath(const godot::String& value);
		void SetFullPath(const godot::String& value);

		godot::StringName GetTagFullPath() const { return m_TagFullPath; }

		godot::TypedArray<TagData> GetChildren() const { return m_Children; };
		void SetChildren(const godot::TypedArray<TagData>& value);

		TagID GetInternalID() const { return m_InternalID; };
		void SetInternalID(TagID value);

		void AddChild(const godot::Ref<TagData>& child);
		void RemoveChild(const godot::Ref<TagData>& child);
		void Reset();

		godot::Ref<TagData> FindChildByName(godot::StringName name) const;
	private:
		void SetTagFullPath(godot::String value);

		void UpdateChildrenParents();

	private:
		godot::String m_Name;
		godot::String m_Path;
		godot::StringName m_TagFullPath;
#ifdef DEBUG_ENABLED
		std::string m_DebugName;
		std::string m_DebugPath;
#endif // DEBUG_ENABLED

		godot::TypedArray<TagData> m_Children;

		TagID m_InternalID;
	};
}