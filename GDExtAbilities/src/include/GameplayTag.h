#pragma once
#include "Types.h"

#include <vector>

namespace sm
{
	struct GameplayTag
	{
		GameplayTag() : m_UID(TagID("<")), parentID(TagID("<")) {};
		GameplayTag(TagID id) : m_UID(id), parentID(TagID("<"))
		{
#ifdef TOOLS_DEBUG_VS
			stdName = ToStdString(id);
#endif //  TOOLS_DEBUG_VS
		};

		TagID GetUID() { return m_UID; };
		const TagID GetUID() const { return static_cast<const TagID>(m_UID); };

	private:
		TagID m_UID;

	public:

		TagID parentID;

#ifdef TOOLS_DEBUG_VS
		std::string stdName;
#endif //  TOOLS_DEBUG_VS

		std::vector<TagID> children;
	};
}