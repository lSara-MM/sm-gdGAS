#include "TagTable.h"

sm::TagTable::TagTable()
{}

sm::TagTable::~TagTable()
{}

sm::TagTable& sm::TagTable::Get()
{
	static TagTable instance;
	return instance;
}

sm::TagID sm::TagTable::GetOrAddTag(const std::string& name)
{
	auto id = m_TagTable.find(name);

	if (id != m_TagTable.end())
	{
		return id->second;
	}

	TagID idx = static_cast<TagID>(m_TagTable.size());

	m_Indexes.emplace_back(name);
	m_TagTable[name] = idx;

	return idx;
}

std::string sm::TagTable::GetTagName(const sm::TagID id)
{
	return m_Indexes[id];
}
