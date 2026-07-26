#include "core/TagRegistry.h"

#include "internal/smAssert.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>

const char* ROOT_TAG = "_root";

sm::TagRegistry::TagRegistry()
{
	Essentials();
}

void sm::TagRegistry::Essentials()
{
	godot::StringName invalid("_invalid");
	GameplayTag newInvalidTag = GameplayTag(m_IDs.GenerateUID(), invalid);
	m_TagsSet.Set(newInvalidTag.GetUID());
	m_TagsDictionary.emplace(invalid, newInvalidTag.GetUID());

	godot::StringName root(ROOT_TAG);
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

void sm::TagRegistry::Init()
{
	godot::ProjectSettings* ps = godot::ProjectSettings::get_singleton();
	const godot::String fallbackPath = "res://gen/tag_registry.tres";
	godot::String path = fallbackPath;

	if (ps->has_setting(SETTINGS_PATH))
	{
		path = ps->get_setting(SETTINGS_PATH);
	}

	godot::ResourceLoader* rl = godot::ResourceLoader::get_singleton();
	if (path.is_empty() || !rl->exists(path, "tag_data"))
	{
		WARN_PRINT_ONCE_ED("Create a TagData and assign it in the Tag Editor before creating tags.\nEditor > Editor Docks > Tags.");
		return;
	}

	godot::Ref<TagData> tagRoot = rl->load(path);
	if (tagRoot.is_null())
	{
		WARN_PRINT_ONCE_ED("Warning: Create a TagData and assign it in the Tag Editor. Editor > Editor Docks > Tags.");

		return;
	}

	RegisterTags(tagRoot);
}

void sm::TagRegistry::Reset()
{
	m_IDs.Reset();
	m_TagsSet.Clear();
	m_Tags.clear();

	m_TagsDictionary.clear();
#ifdef DEBUG_ENABLED
	m_TagsDictionaryDebug.clear();
#endif // DEBUG_ENABLED

	Essentials();
}

bool sm::TagRegistry::RegisterTags(const godot::Ref<TagData>& tagRoot)
{
	std::vector<godot::Ref<TagData>> stack;
	auto tagsTree = tagRoot->GetChildren();

	for (int64_t i = tagsTree.size() - 1; i >= 0; --i)
	{
		stack.push_back(tagsTree[i]);
	}

	while (!stack.empty())
	{
		const godot::Ref<TagData> tagData = stack.back();
		stack.pop_back();

		GameplayTag* tag = CreateTag(tagData->GetTagFullPath(), tagData->GetPath());

		ERR_CONTINUE_MSG(!tag, godot::vformat("AddTag failed: Unknown tag '%s'", tagData->GetTagFullPath()));

		tagData->SetInternalID(tag->GetUID());

		const auto& children = tagData->GetChildren();

		for (int64_t j = children.size() - 1; j >= 0; --j)
		{
			const godot::Ref<TagData>& tagChild = children[j];

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
#ifdef DEBUG_ENABLED
	auto tagDebug = ToStdString(fullName);
	auto parentDebug = ToStdString(parentName);
#endif // DEBUG_ENABLED

	TagID parentTagID = GameplayTag::ROOT_TAG;

	if (!parentName.is_empty() && !parentName.begins_with("."))
	{
		auto it = m_TagsDictionary.find(parentName);
		parentTagID = (it != m_TagsDictionary.end()) ? it->second : GameplayTag::INVALID_TAG;
	}

	ERR_FAIL_COND_V_MSG(parentTagID == GameplayTag::INVALID_TAG, nullptr, godot::vformat("ParentTag does not exists: %s", parentName));

	GameplayTag& newTag = m_Tags.emplace_back(m_IDs.GenerateUID(), fullName, parentTagID);
	m_TagsSet.Set(newTag.GetUID());

	GameplayTag& parent = m_Tags[parentTagID];
	parent.AddChild(newTag.GetUID());

	newTag.ascendantsMask = parent.ascendantsMask;
	newTag.ascendantsMask.Set(parentTagID);

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

void sm::TagRegistry::RenameTag(const godot::StringName& fullName, const godot::StringName& newName)
{
	auto tag = m_TagsDictionary.extract(fullName);
	tag.key() = newName;
	m_TagsDictionary.insert(std::move(tag));

#ifdef DEBUG_ENABLED
	auto tagDebug = m_TagsDictionaryDebug.extract(ToStdString(fullName));
	tagDebug.key() = ToStdString(newName);
	m_TagsDictionaryDebug.insert(std::move(tagDebug));
#endif // DEBUG_ENABLED
}

std::vector<sm::GameplayTag> sm::TagRegistry::GetTags() const
{
	return m_Tags;
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
