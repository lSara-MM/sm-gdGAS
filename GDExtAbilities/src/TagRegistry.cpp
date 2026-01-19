#include "include/TagRegistry.h"

#include <cassert>
#include <regex>
#include <sstream>

sm::TagRegistry::TagRegistry() : ROOT("<")
{
	GameplayTag& newTag = m_Tags.emplace_back(_GenerateUID(), ROOT);
	m_NameToID.try_emplace(ROOT, newTag.GetUID());
	m_IDtoIndex[newTag.GetUID()] = m_Tags.size() - 1;

#ifdef TOOLS_DEBUG_VS
	m_StdNameToID.try_emplace(ToStdString(ROOT), newTag.GetUID());
#endif //  TOOLS_DEBUG_VS
}

//void sm::TagRegistry::_bind_methods()
//{
//#ifdef DEBUG_MODE
//	godot::ClassDB::bind_method(godot::D_METHOD("get_tag", "tag_name"), &GetTag);
//	godot::ClassDB::bind_method(godot::D_METHOD("get_parent", "tag_name"), &GetParent);
//	godot::ClassDB::bind_method(godot::D_METHOD("get_ascendants", "tag_name"), &GetAscendants);
//	godot::ClassDB::bind_method(godot::D_METHOD("get_children", "tag_name"), &GetChildren);
//	godot::ClassDB::bind_method(godot::D_METHOD("get_descendants", "tag_name"), &GetDescendants);
//
//	godot::ClassDB::bind_method(godot::D_METHOD("register_tag", "tag_name"), &RegisterTag);
//	// Due to overloading, Godot does not deduce which one it refers to so it needs casting
//	godot::ClassDB::bind_method(godot::D_METHOD("unregister_tag", "tag_name"),
//		static_cast<void (TagRegistry::*)(godot::StringName)>(&UnregisterTag));
//
//	godot::ClassDB::bind_method(godot::D_METHOD("rename_tag", "tagId", "newName"), &RenameTag);
//	godot::ClassDB::bind_method(godot::D_METHOD("is_name_valid", "tag_name"), &IsNameValid);
//#endif // DEBUG_MODE
//
//	godot::ClassDB::bind_method(godot::D_METHOD("has_child", "tag_name", "child_name"), &HasChild);
//	godot::ClassDB::bind_method(godot::D_METHOD("has_descendant", "tag_name", "child_name"), &HasDescendant);
//	godot::ClassDB::bind_method(godot::D_METHOD("is_parent_of", "tag_name", "child_name"), &IsParentOf);
//	godot::ClassDB::bind_method(godot::D_METHOD("is_child_of", "tag_name", "parent_name"), &IsChildOf);
//
//	ADD_SIGNAL(godot::MethodInfo("tag_renamed",
//		godot::PropertyInfo(godot::Variant::INT, "tag_id"),
//		godot::PropertyInfo(godot::Variant::STRING_NAME, "new_name")));
//}

sm::TagRegistry& sm::TagRegistry::GetInstance()
{
	static TagRegistry instance;
	return instance;
}

#ifdef DEBUG_MODE
godot::StringName sm::TagRegistry::GetTag(godot::StringName tagName)
{
	GameplayTag* tag = _GetTag(tagName);
	ERR_FAIL_COND_V_MSG(!tag, godot::StringName(), godot::vformat("Tag not found: %s", ToStdString(tagName).c_str()));

	return godot::StringName(tag->name.substr(ROOT.length() + 1));
}

godot::StringName sm::TagRegistry::GetParent(godot::StringName tagName)
{
	GameplayTag* tag = _GetTag(tagName);
	ERR_FAIL_COND_V_MSG(!tag, godot::StringName(), godot::vformat("Tag not found: %s", tagName));

	GameplayTag* tagParent = _GetTag(tag->parentID);

	return godot::StringName(tagParent->name.substr(ROOT.length() + 1));
}

godot::TypedArray<godot::StringName> sm::TagRegistry::GetAscendants(godot::StringName tagName)
{
	godot::TypedArray<godot::StringName> ascendants;
	auto itrTagID = m_NameToID.find(_GetFullName(tagName));
	ERR_FAIL_COND_V_MSG(itrTagID == m_NameToID.end(), godot::TypedArray<godot::StringName>(), godot::vformat("Tag not found: %s", tagName));

	if (auto itr = m_AscendantsCache.find(itrTagID->second); itr != m_AscendantsCache.end())
	{
		return itr->second;
	}

	// If result not cached, find ascendants
	_GetAscendantsTree(itrTagID->second, ascendants);

	return ascendants;
}

void sm::TagRegistry::_GetAscendantsTree(uint32 itrTagID, godot::TypedArray<godot::StringName>& ascendants)
{
	std::vector<uint32> stack;
	stack.push_back(itrTagID);

	while (!stack.empty())
	{
		uint32 id = stack.back();
		stack.pop_back();

		const GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		stack.push_back(tag->parentID);

		if (const GameplayTag* parentTag = _GetTag(tag->parentID);
			parentTag && parentTag->GetUID() != 0)
		{
			ascendants.push_back(godot::StringName(parentTag->name.substr(ROOT.length() + 1)));
		}
		else
		{
			break;
		}
	}
	m_AscendantsCache.try_emplace(itrTagID, ascendants);
}

godot::TypedArray<godot::StringName> sm::TagRegistry::GetChildren(godot::StringName tagName)
{
	auto itrTagID = m_NameToID.find(_GetFullName(tagName));
	ERR_FAIL_COND_V_MSG(itrTagID == m_NameToID.end(), godot::TypedArray<godot::StringName>(), godot::vformat("Tag not found: %s", tagName));

	return _GetChildren(itrTagID->second);
}

godot::TypedArray<godot::StringName> sm::TagRegistry::GetDescendants(godot::StringName tagName)
{
	godot::TypedArray<godot::StringName> descendants;
	auto itrTagID = m_NameToID.find(_GetFullName(tagName));
	ERR_FAIL_COND_V_MSG(itrTagID == m_NameToID.end(), godot::TypedArray<godot::StringName>(), godot::vformat("Tag not found: %s", tagName));

	if (auto itr = m_DescendantsCache.find(itrTagID->second); itr != m_DescendantsCache.end())
	{
		return itr->second;
	}

	_GetDescendantsTree(itrTagID->second, descendants);

	return descendants;
}

void sm::TagRegistry::_GetDescendantsTree(uint32 itrTagID, godot::TypedArray<godot::StringName>& descendants)
{
	std::vector<uint32> stack;
	stack.push_back(itrTagID);

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
			stack.push_back(child);

			if (const GameplayTag* childTag = _GetTag(child))
			{
				descendants.push_back(godot::StringName(childTag->name.substr(ROOT.length() + 1)));
			}
		}
	}

	m_DescendantsCache.try_emplace(itrTagID, descendants);
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

		_ExtractSubTags(fullName);

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

void sm::TagRegistry::RenameTag(uint32 tagID, godot::StringName newName)
{
	ERR_FAIL_COND_MSG(_GetTag(newName), godot::vformat("Tag with this name already exists: %s", ToStdString(newName).c_str()));

	GameplayTag* tag = _GetTag(tagID);
	ERR_FAIL_COND_MSG(!tag, godot::vformat("Tag to rename not found: %d", tagID));

	godot::StringName prevName = tag->name;
	m_NameToID.erase(prevName);

	m_NameToID.try_emplace(newName, tag->GetUID());

	//emit_signal("tag_renamed", tagID, newName);
}
#endif // DEBUG_MODE

bool sm::TagRegistry::IsNameValid(godot::StringName name) const
{
	//Tags must follow this structure: <root>.tagParent.tagChild.other
	std::regex rgx(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*$)");
	return std::regex_match(ToStdString(name), rgx);
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
	if (auto itr = m_DescendantsCache.find(tagID); itr != m_DescendantsCache.end())
	{
		const GameplayTag* childTag = _GetTag(childID);
		return itr->second.has(childTag->name);
	}

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

#pragma region Internal methods

sm::GameplayTag* sm::TagRegistry::_GetTag(uint32 id)
{
	return const_cast<sm::GameplayTag*>(const_cast<const TagRegistry*>(this)->_GetTag(id));
}

const sm::GameplayTag* sm::TagRegistry::_GetTag(uint32 id) const
{
	auto index = m_IDtoIndex.find(id);

	if (index == m_IDtoIndex.end())
	{
		return nullptr;
	}

	return &m_Tags[index->second];
}

sm::GameplayTag* sm::TagRegistry::_GetTag(godot::StringName tagName)
{
	godot::StringName fullName = _GetFullName(tagName);

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

const sm::GameplayTag* sm::TagRegistry::_GetTag(godot::StringName tagName) const
{
	auto itr = m_NameToID.find(tagName);

	if (itr == m_NameToID.end())
	{
		return nullptr;
	}

	return _GetTag(itr->second);
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

godot::StringName sm::TagRegistry::_GetParent(uint32 id)
{
	GameplayTag* tag = _GetTag(id);
	GameplayTag* tagParent = _GetTag(id);

	ERR_FAIL_COND_V_MSG(!tag || !tagParent, godot::StringName(), godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagParent->GetUID()));

	return godot::StringName(tagParent->name.substr(ROOT.length() + 1));
}

godot::TypedArray<godot::StringName> sm::TagRegistry::_GetChildren(uint32 id)
{
	GameplayTag* tag = _GetTag(id);

	godot::TypedArray<godot::StringName> children;
	for (uint32 child : tag->children)
	{
		GameplayTag* tagChild = _GetTag(child);
		godot::StringName relativeName = godot::StringName(tagChild->name.substr(ROOT.length() + 1));

		children.push_back(relativeName);
	}

	return children;
}

void sm::TagRegistry::_ExtractSubTags(godot::StringName& fullName)
{
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

#ifdef TOOLS_DEBUG_VS
		m_StdSuffixToFullPaths.try_emplace(ToStdString(prevRTag), ToStdString(fullName));
#endif //  TOOLS_DEBUG_VS

		prevRTag = "." + prevRTag;
	}
}

godot::StringName sm::TagRegistry::_NormalizeName(godot::StringName name) const
{
	godot::StringName finalName;

	if (!IsNameValid(name))
	{
		// TODO: probably should happen when Editor window is made
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

godot::StringName sm::TagRegistry::_GetFullName(godot::StringName tagName) const
{
	godot::StringName full = tagName;

#ifdef TOOLS_DEBUG_VS
	auto a = ToStdString(tagName);
#endif //  TOOLS_DEBUG_VS

	if (auto itr = m_NameToID.find(tagName);
		itr == m_NameToID.end() && !tagName.begins_with(ROOT))
	{
		if (auto itr2 = m_SuffixToFullPaths.find(tagName); itr2 != m_SuffixToFullPaths.end())
		{
			return itr2->second;
		}
	}

	return _AddRoot(full);
}

sm::GameplayTag& sm::TagRegistry::_AddEntry(godot::StringName name, uint32 idParent)
{
	godot::StringName fullName = _GetFullName(name);

	GameplayTag& newTag = m_Tags.emplace_back(_GenerateUID(), fullName);
	m_NameToID.try_emplace(fullName, newTag.GetUID());

#ifdef DEBUG_MODE
	m_StdNameToID.try_emplace(ToStdString(fullName), newTag.GetUID());
#endif //  DEBUG_MODE

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

#pragma endregion Internal methods