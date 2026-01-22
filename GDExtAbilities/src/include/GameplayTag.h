#pragma once
#include "Types.h"

#include <vector>

namespace sm
{
	struct GameplayTag
	{
		GameplayTag() : m_UID(godot::StringName("<")), parentID(godot::StringName("<")) {};
		GameplayTag(godot::StringName id) : m_UID(id), parentID(godot::StringName("<"))
		{
#ifdef TOOLS_DEBUG_VS
			stdName = ToStdString(id);
#endif //  TOOLS_DEBUG_VS
		};

		godot::StringName GetUID() { return m_UID; };
		const godot::StringName GetUID() const { return static_cast<const godot::StringName>(m_UID); };

	private:
		godot::StringName m_UID;

	public:

		godot::StringName parentID;

#ifdef TOOLS_DEBUG_VS
		std::string stdName;
#endif //  TOOLS_DEBUG_VS

		std::vector<godot::StringName> children;
	};
}