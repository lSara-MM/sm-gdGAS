#include "core/GameplayTag.h"

sm::GameplayTag::GameplayTag(TagID id, godot::StringName name, TagID parent)
	: m_UID(id), m_Name(name), m_ParentUID(parent)
{
#ifdef DEBUG_ENABLED
	stdName = ToStdString(name);
#endif //  DEBUG_ENABLED
}

TagID sm::GameplayTag::GetUID()
{
	return m_UID;
}

TagID sm::GameplayTag::GetParentUID()
{
	return m_ParentUID;
}

godot::StringName sm::GameplayTag::GetName()
{
	return m_Name;
}

std::vector<TagID>& sm::GameplayTag::GetChildren()
{
	return m_Children;
}

void sm::GameplayTag::AddChild(TagID id)
{
	if (!HasChild(id))
	{
		m_Children.push_back(id);
	}
}

size_t sm::GameplayTag::GetChildIndex(TagID id) const
{
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] == id)
		{
			return i;
		}
	}

	return m_Children.size();
}

bool sm::GameplayTag::HasChild(TagID childID) const
{
	for (TagID id : m_Children)
	{
		if (id == childID)
		{
			return true;
		}
	}

	return false;
}

bool sm::GameplayTag::HasDescendant(TagID childID) const
{
	return true;
}
