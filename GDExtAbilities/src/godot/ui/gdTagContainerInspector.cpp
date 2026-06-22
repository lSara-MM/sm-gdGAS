#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagContainerInspector.h"

#include "core/TagRegistry.h"
#include "godot/gdTagContainer.h"
#include "godot/gdTagData.h"
#include "godot/ui/gdTagRegistryEditor.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_split_container.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/h_separator.hpp>

void sm::TagContainerInspector::_bind_methods()
{

}

bool sm::TagContainerInspector::_can_handle(godot::Object* object) const
{
	if (godot::Object::cast_to<TagContainer>(object))
	{
		return true;
	}

	return false;
}

void sm::TagContainerInspector::_parse_begin(godot::Object* object)
{
	if (auto* node = godot::Object::cast_to<TagContainer>(object))
	{
		m_Container = node;
		m_Search = "";
		m_VisibleTreeItems.clear();
	}
}

bool sm::TagContainerInspector::_parse_property(
	Object* object,
	godot::Variant::Type type,
	const godot::String& name,
	godot::PropertyHint hintType,
	const godot::String& hintString,
	godot::BitField<godot::PropertyUsageFlags> usageFlags,
	bool wide)
{
	if (name == "tags")
	{
		auto* container = Object::cast_to<TagContainer>(object);
		auto* mainSplit = memnew(godot::VSplitContainer);

		auto* root = memnew(godot::VBoxContainer);
		root->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		root->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
		root->set_custom_minimum_size(godot::Vector2(100, 70));
		mainSplit->add_child(root);

		auto gui = m_Editor->get_editor_interface()->get_base_control();

		auto* search = memnew(godot::LineEdit);
		search->set_placeholder("Search tag");
		auto searchIcon = gui->get_theme_icon("Search", "EditorIcons");
		search->set_right_icon(searchIcon);
		search->connect("text_changed", callable_mp(this, &TagContainerInspector::_OnSearchTextChanged));
		root->add_child(search);

		m_Tree = memnew(godot::Tree);
		m_Tree->set_columns(1);
		m_Tree->connect(
			"item_edited",
			callable_mp(this, &TagContainerInspector::_OnCheckboxChanged)
		);
		m_Tree->connect(
			"item_activated",
			callable_mp(this, &TagContainerInspector::_OnItemActivated)
		);

		m_Tree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_custom_minimum_size(godot::Vector2(100, 24));

		godot::TreeItem* rootItem = m_Tree->create_item();
		rootItem->set_selectable(0, false);
		m_Tree->set_hide_root(true);
		m_Tree->set_hide_folding(true);

		auto& tags = m_Editor->GetTags();

		for (auto& tag : tags)
		{
			auto gdName = tag->GetTagFullPath();

			godot::TreeItem* item = m_Tree->create_item(rootItem);
			item->set_metadata(0, tag);

			item->set_cell_mode(0, godot::TreeItem::CELL_MODE_CHECK);
			item->set_editable(0, true);
			item->set_checked(0, false);

			item->set_text(0, gdName);
			m_ItemsByName[gdName] = item;
			m_ItemsByNameDebug[ToStdString(gdName)] = item;

			if (m_Container->HasTag(tag) ||
				(!m_Search.is_empty() && !gdName.contains(m_Search)))
			{
				item->set_visible(false);
			}
			else
			{
				item->set_visible(true);
				m_VisibleTreeItems.push_back(item);
			}
		}

		m_Tree->set_custom_minimum_size(godot::Vector2(100, 70));
		root->add_child(m_Tree);

		auto* addButton = memnew(godot::Button);
		addButton->set_text("Add Tag");
		addButton->set_custom_minimum_size((godot::Vector2(50, 24)));
		addButton->connect("pressed", callable_mp(this, &TagContainerInspector::_OnAddButtonClicked));
		root->add_child(addButton);

		auto* tree = memnew(godot::Tree);
		tree->set_columns(1);
		tree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		tree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);

		godot::TreeItem* rootItem2 = tree->create_item();
		rootItem2->set_selectable(0, false);
		tree->set_hide_root(true);
		tree->set_hide_folding(true);

		const auto& tagsInContainer = m_Container->GetTags();
		auto remove = gui->get_theme_icon("Remove", "EditorIcons");

		for (size_t i = 0; i < tagsInContainer.size(); i++)
		{
			godot::Ref<TagData> tag = tagsInContainer[i];

			godot::TreeItem* item = tree->create_item(rootItem2);
			item->set_metadata(0, tag);

			auto gdName = tag->GetTagFullPath();
			item->set_text(0, gdName);
			item->add_button(0, remove);
		}

		tree->set_custom_minimum_size(godot::Vector2(100, 70));

		tree->connect("button_clicked", callable_mp(this, &TagContainerInspector::_OnRemoveButtonClicked));

		int tagsFullSize = tags.size() * 50 + 50;
		mainSplit->set_custom_minimum_size(godot::Vector2(100, tagsFullSize + 50));
		mainSplit->add_child(tree);

		add_custom_control(mainSplit);

		//#ifndef DEBUG_ENABLED
		//		return true;
		//#endif // DEBUG_ENABLED
	}

	return false;
}

void sm::TagContainerInspector::SetEditorPlugin(TagRegistryEditor* editor)
{
	m_Editor = editor;
	m_Editor->deleteTag.SubscribeEvent(this, &TagContainerInspector::DeleteTags);
}

void sm::TagContainerInspector::RefreshTreeSetter()
{
	for (godot::TreeItem* item = m_Tree->get_root()->get_first_child();
		item;
		item = item->get_next())
	{
		godot::Ref<TagData> tag = item->get_metadata(0);
		if (m_Container->HasTag(tag) ||
			(!m_Search.is_empty() && !tag->GetTagFullPath().contains(m_Search)))
		{
			item->set_visible(false);
		}
		else if (!item->is_visible())
		{
			item->set_visible(true);
			m_VisibleTreeItems.push_back(item);
		}
	}
}

void sm::TagContainerInspector::DeleteTags(const std::vector<godot::Ref<TagData>>& tags)
{
	for (const auto& tag : tags)
	{
		m_Container->RemoveTag(tag);
	}

	m_Container->notify_property_list_changed();
}

void sm::TagContainerInspector::_OnCheckboxChanged()
{
	godot::TreeItem* item = m_Tree->get_edited();

	if (!item)
	{
		return;
	}

	if (item->is_checked(0))
	{
		SetCheckbox(item);
	}
}

void sm::TagContainerInspector::_OnAddButtonClicked()
{
	for (auto* item : m_VisibleTreeItems)
	{
		if (item->is_checked(0))
		{
			godot::Ref<TagData> tag = item->get_metadata(0);
			m_Container->AddTag(tag);
		}
	}

	m_Container->notify_property_list_changed();
}

void sm::TagContainerInspector::_OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex)
{
	godot::Ref<TagData> tag = item->get_metadata(0);
	if (tag.is_valid())
	{
		m_Container->RemoveTag(tag);
		m_Container->notify_property_list_changed();
	}
}

void sm::TagContainerInspector::_OnSearchTextChanged(const godot::String& text)
{
	m_Search = text;
	RefreshTreeSetter();
}

void sm::TagContainerInspector::_OnItemActivated()
{
	godot::TreeItem* item = m_Tree->get_selected();

	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<godot::Ref<TagData>> toAdd{ tag };

	while (tag.is_valid())
	{
		auto it = m_ItemsByName.find(tag->GetPath());
		if (it == m_ItemsByName.end() || !it->second)
		{
			break;
		}

		godot::TreeItem* parentItem = parentItem = it->second;
		tag = parentItem->get_metadata(0);
		toAdd.push_back(tag);
	}

	for (godot::Ref<TagData> t : toAdd)
	{
		m_Container->AddTag(t);
	}

	m_Container->notify_property_list_changed();
}

void sm::TagContainerInspector::SetCheckbox(godot::TreeItem* item, bool value)
{
	godot::Ref<TagData> tag = item->get_metadata(0);
	while (tag.is_valid())
	{
		auto it = m_ItemsByName.find(tag->GetPath());

		if (it == m_ItemsByName.end() || !it->second)
		{
			return;
		}
		auto a = ToStdString(it->first);

		godot::TreeItem* parentItem = it->second;
		parentItem->set_checked(0, value);

		tag = parentItem->get_metadata(0);
	}
}

#endif // TOOLS_ENABLED