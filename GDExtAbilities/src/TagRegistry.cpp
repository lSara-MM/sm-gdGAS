#include "include/TagRegistry.h"

#include <cassert>
#include <regex>
#include <sstream>

sm::TagRegistry::TagRegistry() : ROOT("<")
{
	GameplayTag& newTag = m_Tags.emplace_back(_GenerateUID(), ROOT);
	m_NameToID.try_emplace(ROOT, newTag.GetUID());
	m_IDtoIndex[newTag.GetUID()] = m_Tags.size() - 1;
}

sm::TagRegistry& sm::TagRegistry::GetInstance()
{
	static TagRegistry instance;
	return instance;
}

godot::StringName sm::TagRegistry::GetTag(godot::StringName tagName)
{
	GameplayTag* tag = _GetTag(tagName);
	ERR_FAIL_COND_V_MSG(!tag, godot::StringName(), godot::vformat("Tag not found: %s", ToStdString(tagName).c_str()));

	return godot::StringName(tag->name.substr(ROOT.length() + 1));
}

godot::StringName sm::TagRegistry::GetParent(uint32 id)
{
	GameplayTag* tag = _GetTag(id);
	GameplayTag* tagParent = _GetTag(id);

	ERR_FAIL_COND_V_MSG(!tag || !tagParent, godot::StringName(), godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagParent->GetUID()));

	return godot::StringName(tagParent->name.substr(ROOT.length() + 1));
}

godot::StringName sm::TagRegistry::GetParent(godot::StringName name)
{
	GameplayTag* tag = _GetTag(name);
	GameplayTag* tagParent = _GetTag(name);

	ERR_FAIL_COND_V_MSG(!tag || !tagParent, godot::StringName(), godot::vformat("Tag not found: %s", tag ? ToStdString(tag->name).c_str() : ToStdString(tagParent->name).c_str()));

	return godot::StringName(tagParent->name.substr(ROOT.length() + 1));
}

godot::TypedArray<godot::StringName> sm::TagRegistry::GetChildren(uint32 id)
{
	GameplayTag* tag = _GetTag(id);
	ERR_FAIL_COND_V_MSG(!tag, godot::TypedArray<godot::StringName>(), godot::vformat("Tag not found: %d", id));

	godot::TypedArray<godot::StringName> children;
	for (uint32 child : tag->children)
	{
		GameplayTag* tagChild = _GetTag(child);
		godot::StringName relativeName = godot::StringName(tagChild->name.substr(ROOT.length() + 1));

		children.push_back(relativeName);
	}

	return children;
}

godot::TypedArray<godot::StringName> sm::TagRegistry::GetChildren(godot::StringName name)
{
	GameplayTag* tag = _GetTag(name);
	return GetChildren(tag->GetUID());
}

void sm::TagRegistry::RegisterTag(godot::StringName tagName)
{
	ERR_FAIL_COND_MSG(_GetTag(tagName), godot::vformat("Tag already exists: %s", ToStdString(tagName).c_str()));

	std::istringstream iss(ToStdString(tagName));
	std::string prevTagName = ToStdString(ROOT);
	std::string token;
	uint32 prevTag = 0;

	while (std::getline(iss, token, '.'))
	{
		token = prevTagName + "." + token;

		godot::StringName fullName = godot::StringName(token.c_str());

		if (const GameplayTag* tag = _GetTag(fullName))
		{
			prevTag = tag->GetUID();
			prevTagName = token;
			continue;
		}

		godot::String fullTag = fullName;
		godot::String prevRTag;
		auto arr = fullTag.rsplit(".");

		std::string str2 = ToStdString(fullTag);
		for (size_t i = arr.size() - 1; i > 1; --i)
		{
			prevRTag = arr[i] + prevRTag;

			if (!m_SuffixToFullPaths.try_emplace(godot::StringName(prevRTag), fullName).second)
			{
				break;
			}

#ifdef TOOLS_DEBUG
			m_StdSuffixToFullPaths.try_emplace(ToStdString(prevRTag), ToStdString(fullName));
#endif //  TOOLS_DEBUG

			prevRTag = "." + prevRTag;
		}

		GameplayTag& t = _AddEntry(fullName, prevTag);
		prevTag = t.GetUID();
		prevTagName = token;
	}
}

void sm::TagRegistry::UnregisterTag(uint32 tagID)
{
	std::vector<uint32> stack;
	stack.push_back(tagID);

	while (!stack.empty())
	{
		uint32 id = stack.back();
		stack.pop_back();

		GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		for (uint32 childID : tag->children)
		{
			stack.push_back(childID);
		}

		// TODO: Remove from all containers

		GameplayTag* parentTag = _GetTag(tag->parentID);

		if (parentTag)
		{
			auto childToErase = std::find(parentTag->children.begin(), parentTag->children.end(), id);
			if (childToErase != parentTag->children.end())
			{
				parentTag->children.erase(childToErase);
			}
		}

		m_NameToID.erase(tag->name);

		uint32 indexToErase = m_IDtoIndex[id];
		uint32 lastIndex = m_Tags.size() - 1;

		GameplayTag& tagToErase = m_Tags[indexToErase];
		GameplayTag& lastTag = m_Tags[lastIndex];

		uint32 lastUID = lastTag.GetUID();

		std::swap(tagToErase, lastTag);
		m_IDtoIndex[lastUID] = indexToErase;

		m_Tags.pop_back();
		m_IDtoIndex.erase(id);
		tag = nullptr;
	}
}

void sm::TagRegistry::UnregisterTag(godot::StringName tagName)
{
	auto tagID = m_NameToID.find(_GetFullName(tagName));

	if (tagID == m_NameToID.end())
	{
		return;
	}

	UnregisterTag(tagID->second);
}

bool sm::TagRegistry::HasChild(uint32 tagID, uint32 childID) const
{
	const GameplayTag* tag = _GetTag(tagID);
	const GameplayTag* tagChild = _GetTag(tagID);

	ERR_FAIL_COND_V_MSG(!tag || !tagChild, false, godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagChild->GetUID()));

	return std::find(tag->children.begin(), tag->children.end(), childID) != tag->children.end();
}

bool sm::TagRegistry::HasDescendant(uint32 tagID, uint32 childID) const
{
	std::vector<uint32> stack;
	stack.push_back(tagID);

	while (!stack.empty())
	{
		uint32 id = stack.back();
		stack.pop_back();

		const GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		for (uint32 child : tag->children)
		{
			if (child == childID)
			{
				return true;
			}

			stack.push_back(child);
		}
	}

	return false;
}

bool sm::TagRegistry::IsParentOf(uint32 tagID, uint32 childID) const
{
	return HasChild(tagID, childID);
}

bool sm::TagRegistry::IsChildOf(uint32 tagID, uint32 parentID) const
{
	const GameplayTag* tag = _GetTag(tagID);
	const GameplayTag* tagParent = _GetTag(tagID);

	ERR_FAIL_COND_V_MSG(!tag || !tagParent, false, godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagParent->GetUID()));

	return tag->parentID == tagParent->parentID;
}

bool sm::TagRegistry::IsNameValid(godot::StringName name) const
{
	//Tags must follow this structure: <root>.tagParent.tagChild.other
	std::regex rgx(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*$)");
	return std::regex_match(ToStdString(name), rgx);
}

void sm::TagRegistry::RenameTag(uint32 id, godot::StringName newName)
{
	ERR_FAIL_COND_MSG(_GetTag(newName), godot::vformat("Tag with this name already exists: %s", ToStdString(newName).c_str()));

	GameplayTag* tag = _GetTag(id);
	ERR_FAIL_COND_MSG(!tag, godot::vformat("Tag to rename not found: %d", id));

	godot::StringName prevName = tag->name;
	m_NameToID.erase(prevName);

	m_NameToID.try_emplace(newName, tag->GetUID());
}

#pragma region Internal methods

sm::GameplayTag* sm::TagRegistry::_GetTag(uint32 id)
{
	auto index = m_IDtoIndex.find(id);

	if (index == m_IDtoIndex.end())
	{
		return nullptr;
	}

	return &m_Tags[index->second];
}

const sm::GameplayTag* sm::TagRegistry::_GetTag(uint32 id) const
{
	return const_cast<sm::GameplayTag*>(static_cast<const TagRegistry*>(this)->_GetTag(id));
}

sm::GameplayTag* sm::TagRegistry::_GetTag(godot::StringName name)
{
	godot::StringName fullName = _GetFullName(name);

	auto it = m_NameToID.find(fullName);
	if (it == m_NameToID.end())
	{
		return nullptr;
	}

	auto index = m_IDtoIndex.find(it->second);
	if (index == m_IDtoIndex.end())
	{
		m_NameToID.erase(it);
		return nullptr;
	}

	return &m_Tags[index->second];
}

const sm::GameplayTag* sm::TagRegistry::_GetTag(godot::StringName name) const
{
	auto itr = m_NameToID.find(name);

	if (itr == m_NameToID.end())
	{
		return nullptr;
	}

	return const_cast<sm::GameplayTag*>(
		static_cast<const TagRegistry*>(this)->_GetTag(itr->second));
}

sm::GameplayTag& sm::TagRegistry::_GetTagRef(uint32 id)
{
	auto itr = m_IDtoIndex.find(id);
	if (itr != m_IDtoIndex.end())
	{
		return m_Tags[itr->second];
	}

	SM_ASSERT(false, "Parent not found! This should NEVER happen");

	return m_Tags[0]; // Root
}

godot::StringName sm::TagRegistry::_NormalizeName(godot::StringName name) const
{
	godot::StringName finalName;

	if (!IsNameValid(name))
	{
		SM_ASSERT(false, "Critical error. Invalid Tag name: %s", ToStdString(name).c_str());
		return godot::StringName();
	}

	return _GetFullName(name).to_lower();
}

godot::StringName sm::TagRegistry::_AddRoot(godot::StringName name) const
{
	if (!name.begins_with("<"))
	{
		godot::String full = godot::String(ROOT) + "." + godot::String(name);
		return godot::StringName(full);
	}

	return name;
}

godot::StringName sm::TagRegistry::_GetFullName(godot::StringName name) const
{
	auto itr = m_NameToID.find(name);
	godot::String full = name;

	if (itr == m_NameToID.end() && !name.begins_with(ROOT))
	{
		m_SuffixToFullPaths.find(name);
	}

	return _AddRoot(full);
}

sm::GameplayTag& sm::TagRegistry::_AddEntry(godot::StringName name, uint32 idParent)
{
	godot::StringName fullName = _GetFullName(name);

	GameplayTag& newTag = m_Tags.emplace_back(_GenerateUID(), fullName);
	m_NameToID.try_emplace(fullName, newTag.GetUID());
	m_IDtoIndex[newTag.GetUID()] = m_Tags.size() - 1;

	newTag.parentID = idParent;

	GameplayTag& parent = _GetTagRef(idParent);
	_AddChild(parent, newTag.GetUID());

	return newTag;
}

void sm::TagRegistry::_AddChild(GameplayTag& tag, uint32 idChild)
{
	// Search if child already exists and adds it if it doesn't
	if (std::find(tag.children.begin(), tag.children.end(), idChild) == tag.children.end())
	{
		tag.children.push_back(idChild);
	}
}

#pragma endregion