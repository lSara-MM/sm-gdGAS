#pragma once
#include <memory>
#include "utils.h"

namespace sm
{
	typedef uint32_t TagID;

	struct TagNode
	{
		TagID GetTag() { return m_Tag; }
		TagID GetCompleteTag() { return m_CompleteTag; }

		TagID m_Tag;
		TagID m_CompleteTag;

		std::vector<TagNode> m_Children;
		std::weak_ptr<TagNode> m_Parent;
	};
}