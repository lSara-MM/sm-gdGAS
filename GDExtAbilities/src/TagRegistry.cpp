#include "include/TagRegistry.h"

#include <cassert>
#include <regex>
#include <sstream>

sm::TagRegistry::TagRegistry() : ROOT("<")
{
	GameplayTag& newTag = m_Tags.emplace_back(ROOT);
	m_NameToIndex[newTag.GetUID()] = m_Tags.size() - 1;

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
TagID sm::TagRegistry::GetTag(TagID tagID)
{
	GameplayTag* tag = _GetTag(_GetFullID(tagID));
	ERR_FAIL_COND_V_MSG(!tag, godot::StringName(), godot::vformat("Tag not found: %s", ToStdString(tagID).c_str()));

	return godot::StringName(tag->GetUID().substr(ROOT.length() + 1));
}

TagID sm::TagRegistry::GetParent(TagID tagID)
{
	GameplayTag* tag = _GetTag(_GetFullID(tagID));
	ERR_FAIL_COND_V_MSG(!tag, godot::StringName(), godot::vformat("Tag not found: %s", tagID));

	GameplayTag* tagParent = _GetTag(tag->parentID);

	return godot::StringName(tagParent->GetUID().substr(ROOT.length() + 1));
}

godot::TypedArray<TagID> sm::TagRegistry::GetAscendants(TagID tagID)
{
	godot::TypedArray<godot::StringName> ascendants;

	TagID id = _GetFullID(tagID);

	ERR_FAIL_COND_V_MSG(m_NameToIndex.find(id) == m_NameToIndex.end(),
		godot::TypedArray<godot::StringName>(),
		godot::vformat("Tag not found: %s", tagID));

	if (auto itr = m_AscendantsCache.find(id); itr != m_AscendantsCache.end())
	{
		return itr->second;
	}

	// If result not cached, find ascendants
	_GetAscendantsTree(id, ascendants);

	return ascendants;
}

void sm::TagRegistry::_GetAscendantsTree(TagID itrTagID, godot::TypedArray<TagID>& ascendants)
{
	std::vector<TagID> stack;
	stack.push_back(itrTagID);

	while (!stack.empty())
	{
		TagID id = stack.back();
		stack.pop_back();

		const GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		stack.push_back(tag->parentID);

		if (const GameplayTag* parentTag = _GetTag(tag->parentID);
			parentTag && parentTag->GetUID() != ROOT)
		{
			TagID ascendantID = godot::StringName(parentTag->GetUID().substr(ROOT.length() + 1));
			ascendants.push_back(ascendantID);
		}
		else
		{
			break;
		}
	}
	m_AscendantsCache.try_emplace(itrTagID, ascendants);
}

godot::TypedArray<TagID> sm::TagRegistry::GetChildren(TagID tagID)
{
	GameplayTag* tag = _GetTag(_GetFullID(tagID));
	ERR_FAIL_COND_V_MSG(!tag, godot::TypedArray<godot::StringName>(), godot::vformat("Tag not found: %s", tagID));

	godot::TypedArray<godot::StringName> children;
	for (TagID child : tag->children)
	{
		GameplayTag* tagChild = _GetTag(child);
		godot::StringName relativeName = godot::StringName(tagChild->GetUID().substr(ROOT.length() + 1));

		children.push_back(relativeName);
	}

	return children;
}

godot::TypedArray<TagID> sm::TagRegistry::GetDescendants(TagID tagID)
{
	godot::TypedArray<godot::StringName> descendants;

	TagID id = _GetFullID(tagID);
	ERR_FAIL_COND_V_MSG(m_NameToIndex.find(id) == m_NameToIndex.end(),
		godot::TypedArray<godot::StringName>(),
		godot::vformat("Tag not found: %s", tagID));

	if (auto itr = m_DescendantsCache.find(id); itr != m_DescendantsCache.end())
	{
		return itr->second;
	}

	_GetDescendantsTree(id, descendants);

	return descendants;
}

void sm::TagRegistry::_GetDescendantsTree(TagID itrTagID, godot::TypedArray<TagID>& descendants)
{
	std::vector<TagID> stack;
	stack.push_back(itrTagID);

	while (!stack.empty())
	{
		TagID id = stack.back();
		stack.pop_back();

		const GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		for (TagID child : tag->children)
		{
			stack.push_back(child);

			if (const GameplayTag* childTag = _GetTag(child); childTag)
			{
				TagID descendantID = godot::StringName(childTag->GetUID().substr(ROOT.length() + 1));
				descendants.push_back(descendantID);
			}
		}
	}

	m_DescendantsCache.try_emplace(itrTagID, descendants);
}

void sm::TagRegistry::RegisterTag(TagID tagID)
{
	ERR_FAIL_COND_MSG(_GetTag(_GetFullID(tagID)), godot::vformat("Tag already exists: %s", ToStdString(tagID).c_str()));

	std::istringstream iss(ToStdString(tagID));
	std::string prevTagName = ToStdString(ROOT);
	std::string token;
	TagID prevTag = ROOT;

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

void sm::TagRegistry::UnregisterTag(TagID tagID)
{
	TagID id = _GetFullID(tagID);
	ERR_FAIL_COND_MSG(m_NameToIndex.find(id) == m_NameToIndex.end(),
		godot::vformat("Tag not found: %s", tagID));

	std::vector<TagID> stack;
	stack.push_back(tagID);

	while (!stack.empty())
	{
		TagID id = stack.back();
		stack.pop_back();

		GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		for (TagID childID : tag->children)
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

		size_t indexToErase = m_NameToIndex[id];
		size_t lastIndex = m_Tags.size() - 1;

		GameplayTag& tagToErase = m_Tags[indexToErase];
		GameplayTag& lastTag = m_Tags[lastIndex];

		TagID lastUID = lastTag.GetUID();

		std::swap(tagToErase, lastTag);
		m_NameToIndex[lastUID] = indexToErase;

		m_Tags.pop_back();
		m_NameToIndex.erase(id);
		tag = nullptr;
	}
}

void sm::TagRegistry::RenameTag(TagID tagID, TagID newName)
{
	TagID oldN = _GetFullID(tagID);
	TagID newN = _GetFullID(newName);
	
	ERR_FAIL_COND_MSG(m_NameToIndex.find(newN) == m_NameToIndex.end() ||
		m_DisplayNamesToID.find(newN) == m_DisplayNamesToID.end(),
		godot::vformat("Tag with this name already exists: %s", ToStdString(newN).c_str()));

	m_DisplayNamesToID.try_emplace(newN, oldN);
	//emit_signal("tag_renamed", tagID, newN);
}
#endif // DEBUG_MODE

bool sm::TagRegistry::IsNameValid(godot::StringName name) const
{
	//Tags must follow this structure: <root>.tagParent.tagChild.other
	std::regex rgx(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*$)");
	return std::regex_match(ToStdString(name), rgx);
}

bool sm::TagRegistry::HasChild(TagID tagID, TagID childID) const
{
	const GameplayTag* tag = _GetTag(_GetFullID(tagID));
	const GameplayTag* tagChild = _GetTag(_GetFullID(childID));

	ERR_FAIL_COND_V_MSG(!tag || !tagChild, false, godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagChild->GetUID()));

	return std::find(tag->children.begin(), tag->children.end(), childID) != tag->children.end();
}

bool sm::TagRegistry::HasDescendant(TagID tagID, TagID childID) const
{
	TagID tagFullID = _GetFullID(tagID);
	TagID childFullID = _GetFullID(childID);

	if (auto itr = m_DescendantsCache.find(_GetFullID(tagFullID)); itr != m_DescendantsCache.end())
	{
		const GameplayTag* childTag = _GetTag(childFullID);
		return itr->second.has(childTag->GetUID());
	}

	std::vector<TagID> stack;
	stack.push_back(tagFullID);

	while (!stack.empty())
	{
		TagID id = stack.back();
		stack.pop_back();

		const GameplayTag* tag = _GetTag(id);

		if (!tag)
		{
			continue;
		}

		for (TagID child : tag->children)
		{
			if (child == childFullID)
			{
				return true;
			}

			stack.push_back(child);
		}
	}

	return false;
}

bool sm::TagRegistry::IsParentOf(TagID tagID, TagID childID) const
{
	return HasChild(tagID, childID);
}

bool sm::TagRegistry::IsChildOf(TagID tagID, TagID parentID) const
{
	const GameplayTag* tag = _GetTag(_GetFullID(tagID));
	const GameplayTag* tagParent = _GetTag(_GetFullID(tagID));

	ERR_FAIL_COND_V_MSG(!tag || !tagParent, false, godot::vformat("Tag not found: %d", tag ? tag->GetUID() : tagParent->GetUID()));

	return tag->parentID == tagParent->parentID;
}

#pragma region Internal methods

sm::GameplayTag* sm::TagRegistry::_GetTag(TagID tagID)
{
	return const_cast<sm::GameplayTag*>(const_cast<const TagRegistry*>(this)->_GetTag(tagID));
}

const sm::GameplayTag* sm::TagRegistry::_GetTag(TagID tagID) const
{
#ifdef TOOLS_DEBUG_VS
	auto a = ToStdString(tagID);
#endif TOOLS_DEBUG_VS

	if (auto itr = m_NameToIndex.find(tagID); itr != m_NameToIndex.end())
	{
		return &m_Tags[itr->second];
	}

	if (auto itr = m_DisplayNamesToID.find(tagID); itr != m_DisplayNamesToID.end())
	{
		if (auto indexIt = m_NameToIndex.find(itr->second); indexIt != m_NameToIndex.end())
		{
			return &m_Tags[indexIt->second];
		}
	}

	return nullptr;
}

void sm::TagRegistry::_ExtractSubTags(TagID fullName)
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

godot::StringName sm::TagRegistry::_NormalizeID(godot::StringName name) const
{
	godot::StringName finalName;

	if (!IsNameValid(name))
	{
		// TODO: probably should happen when Editor window is made
		SM_ASSERT(false, "Critical error. Invalid Tag name: %s", ToStdString(name).c_str());
		return godot::StringName();
	}

	return _GetFullID(name).to_lower();
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

TagID sm::TagRegistry::_GetFullID(TagID tagID) const
{
	godot::StringName full = tagID;

#ifdef TOOLS_DEBUG_VS
	auto a = ToStdString(tagID);
#endif //  TOOLS_DEBUG_VS

	if (auto itr = m_NameToIndex.find(tagID);
		itr == m_NameToIndex.end() && !tagID.begins_with(ROOT))
	{
		if (auto itr2 = m_SuffixToFullPaths.find(tagID); itr2 != m_SuffixToFullPaths.end())
		{
			return itr2->second;
		}
	}

	return _AddRoot(full);
}

sm::GameplayTag& sm::TagRegistry::_AddEntry(godot::StringName name, TagID idParent)
{
	godot::StringName fullName = _GetFullID(name);

	GameplayTag& newTag = m_Tags.emplace_back(fullName);
	m_NameToIndex.try_emplace(fullName, m_Tags.size() - 1);

#ifdef DEBUG_MODE
	m_StdNameToID.try_emplace(ToStdString(fullName), newTag.GetUID());
#endif //  DEBUG_MODE

	newTag.parentID = idParent;

	GameplayTag* parent = _GetTag(idParent);
	_AddChild(parent, newTag.GetUID());

	return newTag;
}

void sm::TagRegistry::_AddChild(GameplayTag* tag, TagID idChild)
{
	// Search if child already exists and adds it if it doesn't
	if (std::find(tag->children.begin(), tag->children.end(), idChild) == tag->children.end())
	{
		tag->children.push_back(idChild);
	}
}

#pragma endregion Internal methods