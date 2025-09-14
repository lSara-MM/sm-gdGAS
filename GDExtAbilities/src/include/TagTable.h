#pragma once
#include "utils.h"
#include <unordered_map>

namespace sm
{
	class TagTable final
	{
	public:
		TagTable();
		~TagTable();

		static TagTable& Get();
		sm::TagID GetOrAddTag(const std::string& name);
		std::string GetTagName(const sm::TagID id);

	private:
		TagTable(const TagTable&) = delete;
		TagTable& operator=(const TagTable&) = delete;

	private:
		std::unordered_map<std::string, sm::TagID> m_TagTable;
		std::vector<std::string> m_Indexes;
	};
}