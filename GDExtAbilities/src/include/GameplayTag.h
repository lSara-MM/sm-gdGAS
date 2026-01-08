#pragma once
#include "Types.h"

#include <godot_cpp/variant/string_name.hpp>

namespace sm
{
	struct GameplayTag
	{
		GameplayTag() : UID(-1), parentID(-1) {};
		GameplayTag(uint32 id, godot::StringName n) : UID(id), parentID(-1), name(n)
		{
#ifdef TOOLS_DEBUG
			stdName = ToStdString(n);
#endif //  TOOLS_DEBUG
		};

		GameplayTag(uint32 id, const godot::String& n) : UID(id), parentID(-1), name(godot::StringName(n))
		{

#ifdef TOOLS_DEBUG
			stdName = ToStdString(n);
#endif //  TOOLS_DEBUG
		}

		uint32 GetUID() { return UID; };
		const uint32 GetUID() const { return static_cast<const uint32>(UID); };

	private:
		uint32 UID;

	public:

		uint32 parentID;

#ifdef TOOLS_DEBUG
		std::string stdName;
#endif //  TOOLS_DEBUG

		godot::StringName name;
		std::vector<uint32> children;
	};
}