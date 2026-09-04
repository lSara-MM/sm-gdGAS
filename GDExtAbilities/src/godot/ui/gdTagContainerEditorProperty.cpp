#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagContainerEditorProperty.h"

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

sm::TagContainerEditorProperty::TagContainerEditorProperty(TagRegistryEditor* registry) : TagArrayEditorProperty(registry)
{}

void sm::TagContainerEditorProperty::ShowInspector(Object* object)
{
	godot::StringName propName = get_edited_property();
	tags = object->get(propName);

	auto* split = memnew(godot::VSplitContainer);

	root = TagArrayEditorProperty::ShowTagTreeEditor();
	TagArrayEditorProperty::SetSize();
	split->add_child(root);

	auto* hbox2 = memnew(godot::HBoxContainer);
	hbox2->set_alignment(hbox2->ALIGNMENT_CENTER);
	root->add_child(hbox2);

	auto* addButton = memnew(godot::Button);
	addButton->set_text("Add Tag");
	addButton->set_custom_minimum_size(godot::Vector2(100, 30));
	addButton->connect("pressed", callable_mp(this, &TagContainerEditorProperty::_OnAddButtonClicked));
	hbox2->add_child(addButton);

	auto* clearButton = memnew(godot::Button);
	clearButton->set_text("Deselect All");
	clearButton->set_custom_minimum_size(godot::Vector2(100, 30));
	clearButton->connect("pressed", callable_mp(this, &TagContainerEditorProperty::_OnDeselectButtonClicked));
	hbox2->add_child(clearButton);

	auto* removeAllButton = memnew(godot::Button);
	removeAllButton->set_text("Remove All Tags");
	removeAllButton->set_custom_minimum_size(godot::Vector2(100, 30));
	removeAllButton->connect("pressed", callable_mp(this, &TagContainerEditorProperty::_OnRemoveAllButtonClicked));
	hbox2->add_child(removeAllButton);

	auto* selectedTree = AddSelectedTagsTree();
	split->add_child(selectedTree);

	add_child(split);
	set_bottom_editor(split);
}

void sm::TagContainerEditorProperty::AddSearchControls(godot::BoxContainer* root, godot::Control* gui)
{
	auto* search = memnew(godot::LineEdit);
	search->set_placeholder("Search tag");
	search->set_right_icon(gui->get_theme_icon("Search", "EditorIcons"));
	search->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->add_child(search);

	search->connect("text_changed", callable_mp(this, &TagContainerEditorProperty::_OnSearchTextChanged));
}

godot::Control* sm::TagContainerEditorProperty::AddSelectedTagsTree()
{
	m_SelectedTree = memnew(godot::Tree);
	m_SelectedTree->set_columns(1);
	m_SelectedTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_SelectedTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_SelectedTree->set_custom_minimum_size(godot::Vector2(100, tags.size() * 25 + 100));
	m_SelectedTree->set_hide_root(true);
	m_SelectedTree->set_hide_folding(true);

	SelectedTree();

	m_SelectedTree->connect("button_clicked", callable_mp(this, &TagContainerEditorProperty::_OnRemoveButtonClicked));

	return m_SelectedTree;
}

void sm::TagContainerEditorProperty::SelectedTree()
{
	if (!m_SelectedTree)
	{
		return;
	}

	m_SelectedTree->clear();

	godot::TreeItem* rootItem = m_SelectedTree->create_item();
	rootItem->set_selectable(0, false);

	auto* gui = editor->get_editor_interface()->get_base_control();
	auto remove = gui->get_theme_icon("Remove", "EditorIcons");

	std::unordered_set<TagID> has;

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagID tagId = tags[i];
		if (has.find(tagId) != has.end())
		{
			tags.remove_at(i);
			continue;
		}

		godot::TreeItem* item = m_SelectedTree->create_item(rootItem);
		item->set_metadata(0, tagId);
		item->set_text(0, idToResource[tagId]);
		item->add_button(0, remove);
		has.emplace(tagId);
	}
}

void sm::TagContainerEditorProperty::RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item)
{
	const bool hasTag = HasTag(tag->GetInternalID());

	if (!hasTag && item->is_checked(0))
	{
		item->set_checked(0, false);
	}

	const bool matchesSearch = search.is_empty() || tag->GetTagFullPath().containsn(search);
	bool visible = matchesSearch;

	visible = visible && !hasTag;
	item->set_visible(visible);

	if (visible)
	{
		visibleItems.push_back(item);
	}
}

void sm::TagContainerEditorProperty::_OnCheckboxChanged()
{
	godot::TreeItem* item = availableTree->get_edited();
	if (!item)
	{
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	if (item->is_checked(0))
	{
		TagArrayEditorProperty::SetCheckbox(item);
	}
}

void sm::TagContainerEditorProperty::_OnSearchTextChanged(const godot::String& text)
{
	search = text;
	RefreshAvailableTree();
}

void sm::TagContainerEditorProperty::Refresh()
{
	RefreshAvailableTree();
	SelectedTree();
	SetSize();
}

void sm::TagContainerEditorProperty::SetSize()
{
	if (root)
	{
		root->set_custom_minimum_size(godot::Vector2(100, GetTagsSize() * 25 + 50));
	}

	if (m_SelectedTree)
	{
		m_SelectedTree->set_custom_minimum_size(godot::Vector2(100, tags.size() * 50 + 50));
	}
}

void sm::TagContainerEditorProperty::_OnAddButtonClicked()
{
	for (auto* item : visibleItems)
	{
		if (item->is_checked(0))
		{
			godot::Ref<TagData> tag = item->get_metadata(0);
			TagID id = tag->GetInternalID();

			if (!tags.has(id))
			{
#ifdef DEBUG_ENABLED
				WARN_PRINT_ED(godot::vformat("Added: %d", id));
#endif // DEBUG_ENABLED

				tags.push_back(id);
			}
		}
	}

	emit_changed(get_edited_property(), tags);
}

void sm::TagContainerEditorProperty::_OnDeselectButtonClicked()
{
	godot::TreeItem* root = availableTree->get_root();
	if (!root)
	{
		return;
	}

	godot::TreeItem* item = root;
	while (item)
	{
		item->set_checked(0, false);
		item = item->get_next_in_tree();
	}

	emit_changed(get_edited_property(), tags);
}

void sm::TagContainerEditorProperty::_OnRemoveAllButtonClicked()
{
	tags.clear();
	emit_changed(get_edited_property(), tags);
}

void sm::TagContainerEditorProperty::_OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex)
{
	TagID tagId = item->get_metadata(0);
	auto pos = tags.find(tagId);
	tags.remove_at(pos);

	emit_changed(get_edited_property(), tags);
}

void sm::TagContainerEditorProperty::_OnItemActivated(godot::Tree* tree)
{
	godot::TreeItem* item = tree->get_selected();
	if (!item)
	{
		return;
	}

	_OnCheckboxChanged();
}
#endif // TOOLS_ENABLED