#include "godot/ui/gdTagsInEffect.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_split_container.hpp>

sm::TagsInEffect::TagsInEffect(bool _isAdd) : isAdd(_isAdd)
{}

bool sm::TagsInEffect::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<EffectData>(object);
}

void sm::TagsInEffect::_parse_begin(godot::Object* object)
{
	m_Effect = godot::Ref<EffectData>(static_cast<EffectData*>(object));

	if (m_Effect.is_null())
	{
		m_IdToResource.clear();

#ifdef DEBUG_ENABLED
		m_ItemsByNameDebug.clear();
#endif // DEBUG_ENABLED
	}
}

bool sm::TagsInEffect::_parse_property(
	Object* object,
	godot::Variant::Type type,
	const godot::String& name,
	godot::PropertyHint hintType,
	const godot::String& hintString,
	godot::BitField<godot::PropertyUsageFlags> usageFlags,
	bool wide)
{
	if (name == "tags_to_add" && isAdd && godot::Object::cast_to<EffectData>(object))
	{
		ShowTagTreeEditor(object, "Tags to Add");
		return true;
	}

	if (name == "tags_to_remove" && godot::Object::cast_to<EffectData>(object))
	{
		ShowTagTreeEditor(object, "Tags to Remove");
		return true;
	}

	return false;
}

void sm::TagsInEffect::SetCustomProperties(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit, const int tagsSize)
{
	mainSplit->set_custom_minimum_size(godot::Vector2(100, tagsSize * 12 + 100));
}

godot::PackedInt32Array sm::TagsInEffect::GetTags() const
{
	if (isAdd)
	{
		return m_Effect.is_valid() ? m_Effect->GetTagsToAdd() : godot::PackedInt32Array();
	}

	return m_Effect.is_valid() ? m_Effect->GetTagsToRemove() : godot::PackedInt32Array();
}

void sm::TagsInEffect::SetTags(const godot::PackedInt32Array& tags)
{
	if (m_Effect.is_null())
	{
		return;
	}

	if (isAdd)
	{
		m_Effect->SetTagsToAdd(tags);
	}
	else
	{
		m_Effect->SetTagsToRemove(tags);
	}
}

bool sm::TagsInEffect::HasTag(TagID id) const
{
	return m_Effect.is_valid() && m_Effect->HasTag(id, GetTags());
}

void sm::TagsInEffect::AddTag(TagID id)
{
	if (m_Effect.is_null())
	{
		return;
	}

	if (isAdd)
	{
		m_Effect->AddTagToAdd(id);
	}
	else
	{
		m_Effect->AddTagToRemove(id);
	}
}

void sm::TagsInEffect::RemoveTag(TagID id)
{
	godot::PackedInt32Array tags = GetTags();
	for (int64_t i = tags.size() - 1; i >= 0; --i)
	{
		if (tags[i] == id)
		{
			tags.remove_at(i);
		}
	}

	SetTags(tags);
	if (m_Effect.is_valid())
	{
		m_Effect->emit_changed();
	}
}

void sm::TagsInEffect::_OnCheckboxChanged()
{
	godot::TreeItem* item = availableTree->get_edited();
	if (!item)
	{
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<TagID> ids;

	if (item->is_checked(0))
	{
		ids = SetCheckbox(item);
	}

	if (item->is_checked(0))
	{
		for (TagID id : ids)
		{
			AddTag(id);
		}
	}
	else
	{
		RemoveTag(tag->GetInternalID());
	}

	RefreshAvailableTree();
	m_Effect->emit_changed();
}

void sm::TagsInEffect::_OnItemActivated(godot::Tree* tree)
{
	godot::TreeItem* item = tree->get_selected();
	if (!item)
	{
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<godot::Ref<TagData>> toAdd{ tag };

	while (tag.is_valid())
	{
		auto it = itemsByName.find(tag->GetPath());
		if (it == itemsByName.end() || !it->second)
		{
			break;
		}

		godot::TreeItem* parentItem = it->second;
		tag = parentItem->get_metadata(0);
		toAdd.push_back(tag);
	}

	for (godot::Ref<TagData> tagToAdd : toAdd)
	{
		AddTag(tagToAdd->GetInternalID());
	}
}