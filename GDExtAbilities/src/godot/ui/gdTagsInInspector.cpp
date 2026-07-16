#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagsInInspector.h"

#include "godot/gdTagContainer.h"
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

void sm::TagsInInspector::SetEditorPlugin(TagRegistryEditor* editor)
{
	registryEditor = editor;
	registryEditor->deleteTag.SubscribeEvent(this, &TagsInInspector::DeleteTags);
}

void sm::TagsInInspector::ShowTagTreeEditor(godot::Object* object, const godot::String& title)
{
	auto* mainSplit = memnew(godot::VSplitContainer);

	auto* root = memnew(godot::VBoxContainer);
	root->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->set_custom_minimum_size(godot::Vector2(100, 70));
	mainSplit->add_child(root);

	auto* separator = memnew(godot::HSeparator);
	root->add_child(separator);

	auto* titleLabel = memnew(godot::Label);
	titleLabel->set_text(title);
	root->add_child(titleLabel);

	auto* gui = registryEditor->get_editor_interface()->get_base_control();
	auto* hbox = memnew(godot::HBoxContainer);
	root->add_child(hbox);
	const int tagsSize = AddAvailableTagsTree(root);
	AddSearchControls(hbox, gui);

	SetCustomProperties(root, gui, mainSplit, tagsSize);
	RefreshAvailableTree();

	add_custom_control(mainSplit);
}

void sm::TagsInInspector::SetCustomProperties(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit, const int tagsSize)
{
	const int rowHeight = 30;
	const int extraHeight = 100;
	mainSplit->set_custom_minimum_size(godot::Vector2(100, tagsSize * rowHeight + extraHeight));
}

int sm::TagsInInspector::AddAvailableTagsTree(godot::VBoxContainer* root)
{
	availableTree = memnew(godot::Tree);
	availableTree->set_columns(1);
	availableTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	availableTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	availableTree->set_custom_minimum_size(godot::Vector2(100, 70));
	availableTree->connect("item_edited", callable_mp(this, &TagsInInspector::_OnCheckboxChanged));
	availableTree->connect("item_activated", callable_mp(this, &TagsInInspector::_OnItemActivated));

	godot::TreeItem* rootItem = availableTree->create_item();
	rootItem->set_selectable(0, false);
	availableTree->set_hide_root(true);
	availableTree->set_hide_folding(true);

	auto& tags = registryEditor->GetTags();
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

		m_IdToResource[tagId] = gdName;
		itemsByName[gdName] = item;

#ifdef DEBUG_ENABLED
		m_ItemsByNameDebug[ToStdString(gdName)] = item;
#endif // DEBUG_ENABLED

		RefreshTagVisibility(tag, item);
	}

	root->add_child(availableTree);
	return tags.size();
}

void sm::TagsInInspector::AddSearchControls(godot::BoxContainer* root, godot::Control* gui)
{
	auto* search = memnew(godot::LineEdit);
	search->set_placeholder("Search tag");
	search->set_right_icon(gui->get_theme_icon("Search", "EditorIcons"));
	search->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->add_child(search);

	auto visibleIcon = gui->get_theme_icon("GuiVisibilityVisible", "EditorIcons");

	auto invisibleIcon = gui->get_theme_icon("GuiVisibilityHidden", "EditorIcons");

	enabledButton.button = memnew(godot::Button);
	enabledButton.button->set_text("Enabled");
	enabledButton.button->set_toggle_mode(true);
	enabledButton.button->set_pressed_no_signal(showEnabled);
	enabledButton.visibleIcon = visibleIcon;
	enabledButton.invisibleIcon = invisibleIcon;
	enabledButton.button->set_button_icon(showEnabled ? visibleIcon : invisibleIcon);
	root->add_child(enabledButton.button);

	disabledButton.button = memnew(godot::Button);
	disabledButton.button->set_text("Disabled");
	disabledButton.button->set_toggle_mode(true);
	disabledButton.button->set_pressed_no_signal(showDisabled);
	disabledButton.visibleIcon = visibleIcon;
	disabledButton.invisibleIcon = invisibleIcon;
	disabledButton.button->set_button_icon(showDisabled ? visibleIcon : invisibleIcon);
	root->add_child(disabledButton.button);

	search->connect("text_changed", callable_mp(this, &TagsInInspector::_OnSearchTextChanged));
	enabledButton.button->connect("toggled", callable_mp(this, &TagsInInspector::_OnShowEnabledToggled));
	disabledButton.button->connect("toggled", callable_mp(this, &TagsInInspector::_OnShowDisabledToggled));
}

void sm::TagsInInspector::RefreshAvailableTree()
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

void sm::TagsInInspector::RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item)
{
	const bool hasTag = HasTag(tag->GetInternalID());
	const bool matchesSearch = search.is_empty() || tag->GetTagFullPath().contains(search);
	bool visible = matchesSearch;

	visible = visible &&
		((hasTag && showEnabled) || (!hasTag && showDisabled));
	item->set_checked(0, hasTag);

	item->set_visible(visible);
	if (visible)
	{
		visibleItems.push_back(item);
	}
}

std::vector<TagID> sm::TagsInInspector::SetCheckbox(godot::TreeItem* item, bool value)
{
	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<TagID> ids;
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

void sm::TagsInInspector::DeleteTags(const std::vector<godot::Ref<TagData>>& tags)
{
	for (const auto& tag : tags)
	{
		RemoveTag(tag->GetInternalID());
	}
}

void sm::TagsInInspector::_OnCheckboxChanged()
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
}

void sm::TagsInInspector::_OnSearchTextChanged(const godot::String& text)
{
	search = text;
	RefreshAvailableTree();
}

void sm::TagsInInspector::_OnShowEnabledToggled(bool toggled)
{
	showEnabled = toggled;
	enabledButton.button->set_button_icon(showEnabled ? enabledButton.visibleIcon : enabledButton.invisibleIcon);
	RefreshAvailableTree();
}

void sm::TagsInInspector::_OnShowDisabledToggled(bool toggled)
{
	showDisabled = toggled;
	disabledButton.button->set_button_icon(showDisabled ? disabledButton.visibleIcon : disabledButton.invisibleIcon);
	RefreshAvailableTree();
}

#endif // TOOLS_ENABLED
