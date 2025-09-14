#pragma once
#include "utils.h"
#include "TagTable.h"

namespace sm
{
	struct TagNode;
}

namespace sm
{
	class TagManager
	{
	public:

		TagManager();
		~TagManager();

		void AddTag(const std::string& tag);

	private:

		sm::TagTable* m_TagTable;
		sm::TagNode* m_RootNode;
	};
}