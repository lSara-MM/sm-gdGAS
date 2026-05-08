#include "core/TagRegistry.h"

#include "internal/smAssert.h"
#include <godot_cpp/variant/utility_functions.hpp>

sm::TagRegistry::TagRegistry()
{
	godot::StringName invalid("invalid");
	GameplayTag newInvalidTag = GameplayTag(m_IDs.GenerateUID(), invalid);
	m_TagsSet.Set(newInvalidTag.GetUID());
	m_TagsDictionary.emplace(invalid, newInvalidTag.GetUID());

	godot::StringName root(">");
	GameplayTag newTag = GameplayTag(m_IDs.GenerateUID(), root);
	m_TagsSet.Set(newTag.GetUID());
	m_TagsDictionary.emplace(root, newTag.GetUID());

#ifdef DEBUG_ENABLED
	m_TagsDictionaryDebug.emplace(ToStdString(invalid), newInvalidTag.GetUID());
	m_TagsDictionaryDebug.emplace(ToStdString(root), newTag.GetUID());
#endif // DEBUG_ENABLED

	m_Tags.push_back(std::move(newInvalidTag));
	m_Tags.push_back(std::move(newTag));
}

bool sm::TagRegistry::RegisterTags(const godot::Ref<sm::TagData>& tagRoot)
{
	std::vector<godot::Ref<sm::TagData>> stack;
	auto tagsTree = tagRoot->GetChildren();

	for (int64_t i = 0; i < tagsTree.size(); i++)
	{
		stack.push_back(tagsTree[i]);
	}

	while (!stack.empty())
	{
		const godot::Ref<sm::TagData> tagData = stack.back();
		stack.pop_back();

		GameplayTag* tag = CreateTag(tagData->GetTagFullPath(), tagData->GetPath());

		ERR_CONTINUE_MSG(!tag, godot::vformat("AddTag failed: Unknown tag '%s'", tagData->GetTagFullPath()));

		tagData->SetInternalID(tag->GetUID());

		const auto& children = tagData->GetChildren();

		for (int64_t j = 0; j < children.size(); j++)
		{
			const godot::Ref<sm::TagData>& tagChild = children[j];

			if (tagChild.is_null())
			{
				continue;
			}

			stack.push_back(tagChild);
		}
	}

	return true;
}

sm::GameplayTag* sm::TagRegistry::CreateTag(const godot::StringName& fullName, const godot::StringName& parentName)
{
	TagID parentTag = GameplayTag::ROOT_TAG;

	if (!parentName.is_empty())
	{
		auto it = m_TagsDictionary.find(parentName);
		parentTag = (it != m_TagsDictionary.end()) ? it->second : GameplayTag::INVALID_TAG;
	}

	ERR_FAIL_COND_V_MSG(parentTag == GameplayTag::INVALID_TAG, nullptr, godot::vformat("ParentTag does not exists: %s", parentName));

	GameplayTag& newTag = m_Tags.emplace_back(m_IDs.GenerateUID(), fullName, parentTag);
	m_TagsSet.Set(newTag.GetUID());

	// inspector idea:
	// the idea is to have a tag registry with an array of tags
	// those tags should be: 
	// - Tag (name)
	// -- Tags (All children)
	// basically, tags should have array of tags

	m_TagsDictionary.emplace(fullName, newTag.GetUID());

#ifdef DEBUG_ENABLED
	m_TagsDictionaryDebug.emplace(ToStdString(fullName), newTag.GetUID());
#endif // DEBUG_ENABLED

	return &newTag;
}

TagID sm::TagRegistry::FindTagID(const godot::StringName& name) const
{
	if (auto it = m_TagsDictionary.find(name); it != m_TagsDictionary.end())
	{
		return it->second;
	}

	return GameplayTag::INVALID_TAG;
}

sm::GameplayTag* sm::TagRegistry::FindGameplayTag(const godot::Ref<TagData>& tag)
{
	ERR_FAIL_COND_V_MSG(tag->GetInternalID() > m_Tags.size(), nullptr, godot::vformat("Tag not found: %s", tag->GetName()));

	return &m_Tags[tag->GetInternalID()];
}

bool sm::TagRegistry::HasChild(TagID tagID, TagID childID) const
{
	return m_Tags[tagID].HasChild(childID);
}
