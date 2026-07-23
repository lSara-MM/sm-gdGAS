#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagArrayEditorProperty.h"

#include "godot/ui/gdTagRegistryEditor.h"
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

sm::TagArrayEditorProperty::TagArrayEditorProperty(TagRegistryEditor* registry) : editor(registry)
{}

void sm::TagArrayEditorProperty::ShowInspector(Object* object, const godot::String& title)
{
	godot::StringName propName = get_edited_property();
	tags = object->get(propName);
	auto* root = ShowTagTreeEditor(title);

	add_child(root);
	set_bottom_editor(root);
}

void sm::TagArrayEditorProperty::_update_property()
{
	if (get_edited_object())
	{
		tags = get_edited_object()->get(get_edited_property());
	}

	Refresh();
}

godot::Control* sm::TagArrayEditorProperty::ShowTagTreeEditor(const godot::String& title)
{
	root = memnew(godot::VBoxContainer);
	root->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);

	auto* gui = editor->get_editor_interface()->get_base_control();
	auto* hbox = memnew(godot::HBoxContainer);
	root->add_child(hbox);
	AddAvailableTagsTree(root);
	AddSearchControls(hbox, gui);

	SetSize();

	auto* separator = memnew(godot::HSeparator);
	root->add_child(separator);
	return root;
}

void sm::TagArrayEditorProperty::AddAvailableTagsTree(godot::Control* root)
{
	availableTree = memnew(godot::Tree);
	availableTree->set_columns(1);
	availableTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	availableTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	availableTree->set_custom_minimum_size(godot::Vector2(100, 70));
	availableTree->connect("item_edited", callable_mp(this, &TagArrayEditorProperty::_OnCheckboxChanged));
	availableTree->connect("item_activated", callable_mp(this, &TagArrayEditorProperty::_OnItemActivated));

	godot::TreeItem* rootItem = availableTree->create_item();
	rootItem->set_selectable(0, false);
	availableTree->set_hide_root(true);
	availableTree->set_hide_folding(true);

	auto& tags = editor->GetTags();
	for (auto& tag : tags)
	{
		auto gdName = tag->GetTagFullPath();
		TagID tagId = tag->GetInternalID();

		godot::TreeItem* item = availableTree->create_item(rootItem);
		item->set_metadata(0, tag);
		item->set_cell_mode(0, godot::TreeItem::CELL_MODE_CHECK);
		item->set_editable(0, true);
		item->set_checked(0, false);
		item->set_text(0, gdName);

		idToResource[tagId] = gdName;
		itemsByName[gdName] = item;
		RefreshTagVisibility(tag, item);
	}

	root->add_child(availableTree);
}

void sm::TagArrayEditorProperty::AddSearchControls(godot::BoxContainer* root, godot::Control* gui)
{
	auto* search = memnew(godot::LineEdit);
	search->set_placeholder("Search tag");
	search->set_right_icon(gui->get_theme_icon("Search", "EditorIcons"));
	search->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->add_child(search);

	auto visibleIcon = gui->get_theme_icon("GuiVisibilityVisible", "EditorIcons");

	auto invisibleIcon = gui->get_theme_icon("GuiVisibilityHidden", "EditorIcons");

	auto* showEnabled = memnew(godot::Button);
	showEnabled->set_text("Enabled");
	showEnabled->set_toggle_mode(true);
	showEnabled->set_pressed_no_signal(m_ShowEnabled);
	showEnabled->set_button_icon(m_ShowEnabled ? visibleIcon : invisibleIcon);
	root->add_child(showEnabled);

	auto* showDisabled = memnew(godot::Button);
	showDisabled->set_text("Disabled");
	showDisabled->set_toggle_mode(true);
	showDisabled->set_pressed_no_signal(m_ShowDisabled);
	showDisabled->set_button_icon(m_ShowDisabled ? visibleIcon : invisibleIcon);
	root->add_child(showDisabled);

	search->connect("text_changed", callable_mp(this, &TagArrayEditorProperty::_OnSearchTextChanged));
	showEnabled->connect("toggled", callable_mp(this, &TagArrayEditorProperty::_OnShowEnabledToggled));
	showDisabled->connect("toggled", callable_mp(this, &TagArrayEditorProperty::_OnShowDisabledToggled));
}

godot::PackedInt32Array sm::TagArrayEditorProperty::GetCurrentTags()
{
	return tags;
}

void sm::TagArrayEditorProperty::SetCurrentTags()
{
	emit_changed(get_edited_property(), tags);
}

bool sm::TagArrayEditorProperty::HasTag(int32_t tagId)
{
	godot::PackedInt32Array tags = GetCurrentTags();
	return tags.has(tagId);
}

int sm::TagArrayEditorProperty::GetTagsSize() const
{
	auto& tags = editor->GetTags();
	return tags.size();
}

void sm::TagArrayEditorProperty::RefreshAvailableTree()
{
	visibleItems.clear();

	for (godot::TreeItem* item = availableTree->get_root()->get_first_child();
		item;
		item = item->get_next())
	{
		godot::Ref<TagData> tag = item->get_metadata(0);
		RefreshTagVisibility(tag, item);
	}
}

void sm::TagArrayEditorProperty::SetSize()
{
	root->set_custom_minimum_size(godot::Vector2(100, GetTagsSize() * 12 + 100));
}

void sm::TagArrayEditorProperty::Refresh()
{
	RefreshAvailableTree();
	SetSize();
}

void sm::TagArrayEditorProperty::RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item)
{
	const bool hasTag = HasTag(tag->GetInternalID());
	const bool matchesSearch = search.is_empty() || tag->GetTagFullPath().containsn(search);
	bool visible = matchesSearch;

	visible = visible &&
		((hasTag && m_ShowEnabled) || (!hasTag && m_ShowDisabled));
	item->set_checked(0, hasTag);

	item->set_visible(visible);
	if (visible)
	{
		visibleItems.push_back(item);
	}
}

godot::PackedInt32Array sm::TagArrayEditorProperty::SetCheckbox(godot::TreeItem* item, bool value)
{
	godot::Ref<TagData> tag = item->get_metadata(0);
	godot::PackedInt32Array ids;
	ids.push_back(tag->GetInternalID());

	while (tag.is_valid())
	{
		auto it = itemsByName.find(tag->GetPath());
		if (it == itemsByName.end() || !it->second)
		{
			return ids;
		}

		godot::TreeItem* parentItem = it->second;
		tag = parentItem->get_metadata(0);
		ids.push_back(tag->GetInternalID());
		parentItem->set_checked(0, value);
	}

	return ids;
}

void sm::TagArrayEditorProperty::_OnCheckboxChanged()
{
	godot::TreeItem* item = availableTree->get_edited();
	if (!item)
	{
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	godot::PackedInt32Array ids;

	if (item->is_checked(0))
	{
		ids = SetCheckbox(item);
	}

	if (item->is_checked(0))
	{
		for (TagID id : ids)
		{
			if (!tags.has(id))
			{
				tags.push_back(id);
			}
		}
	}
	else
	{
		auto pos = tags.find(tag->GetInternalID());
		tags.remove_at(pos);
	}

	RefreshAvailableTree();
	emit_changed(get_edited_property(), tags);
}

void sm::TagArrayEditorProperty::_OnSearchTextChanged(const godot::String& text)
{
	search = text;
	RefreshAvailableTree();
}

void sm::TagArrayEditorProperty::_OnItemActivated(godot::Tree* tree)
{
	godot::TreeItem* item = tree->get_selected();
	if (!item)
	{
		return;
	}

	_OnCheckboxChanged();
}

void sm::TagArrayEditorProperty::_OnShowEnabledToggled(bool toggled)
{
	m_ShowEnabled = toggled;
	RefreshAvailableTree();
}

void sm::TagArrayEditorProperty::_OnShowDisabledToggled(bool toggled)
{
	m_ShowDisabled = toggled;
	RefreshAvailableTree();
}
#endif // TOOLS_ENABLED