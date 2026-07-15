#include "godot/ui/gdTagsInAbility.h"

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

sm::TagsInAbility::TagsInAbility(Type t) : type(t)
{}

bool sm::TagsInAbility::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<TagContainer>(object) ||
		godot::Object::cast_to<EffectData>(object) ||
		godot::Object::cast_to<AbilityData>(object);
}

void sm::TagsInAbility::_parse_begin(godot::Object* object)
{
	if (!godot::Object::cast_to<TagContainer>(object) &&
		!godot::Object::cast_to<EffectData>(object) &&
		!godot::Object::cast_to<AbilityData>(object))
	{
		m_IdToResource.clear();

#ifdef DEBUG_ENABLED
		m_ItemsByNameDebug.clear();
#endif // DEBUG_ENABLED
	}
}

bool sm::TagsInAbility::_parse_property(
	Object* object,
	godot::Variant::Type type,
	const godot::String& name,
	godot::PropertyHint hintType,
	const godot::String& hintString,
	godot::BitField<godot::PropertyUsageFlags> usageFlags,
	bool wide)
{
	if (name == "tags" && godot::Object::cast_to<TagContainer>(object))
	{
		ShowTagTreeEditor(object, "Tags");
		return true;
	}

	return false;
}

godot::PackedInt32Array sm::TagsInAbility::GetTags() const
{

}

void sm::TagsInAbility::SetTags(const godot::PackedInt32Array& tags)
{

}

bool sm::TagsInAbility::HasTag(TagID id) const
{
	//return m_Ability.is_valid() && m_Ability->HasTag(id, GetTags());
}

void sm::TagsInAbility::AddTag(TagID id)
{
	if (m_Ability.is_null())
	{
		return;
	}


}

void sm::TagsInAbility::RemoveTag(TagID id)
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
	if (m_Ability.is_valid())
	{
		m_Ability->emit_changed();
	}
}

void sm::TagsInAbility::_OnCheckboxChanged()
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
	m_Ability->emit_changed();
}

void sm::TagsInAbility::_OnItemActivated(godot::Tree* tree)
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
