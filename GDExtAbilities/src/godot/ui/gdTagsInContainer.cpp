#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagsInContainer.h"

#include "godot/gdTagContainer.h"
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

bool sm::TagsInContainer::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<TagContainer>(object);
}

void sm::TagsInContainer::_parse_begin(godot::Object* object)
{
	m_Container = godot::Object::cast_to<TagContainer>(object);

	if (!m_Container)
	{
		m_IdToResource.clear();

#ifdef DEBUG_ENABLED
		m_ItemsByNameDebug.clear();
#endif // DEBUG_ENABLED
	}
}

bool sm::TagsInContainer::_parse_property(
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

void sm::TagsInContainer::SetCustomProperties(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit, const int tagsSize)
{
	AddTagButtonToContainer(root, gui, mainSplit);
	mainSplit->set_custom_minimum_size(godot::Vector2(100, tagsSize * 50 + 100));
}

void sm::TagsInContainer::AddSearchControls(godot::BoxContainer* root, godot::Control* gui)
{
	auto* search = memnew(godot::LineEdit);
	search->set_placeholder("Search tag");
	search->set_right_icon(gui->get_theme_icon("Search", "EditorIcons"));
	search->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->add_child(search);

	search->connect("text_changed", callable_mp(this, &TagsInContainer::_OnSearchTextChanged));
}

void sm::TagsInContainer::AddTagButtonToContainer(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit)
{
	auto* hbox2 = memnew(godot::HBoxContainer);
	hbox2->set_alignment(hbox2->ALIGNMENT_CENTER);
	root->add_child(hbox2);

	auto* addButton = memnew(godot::Button);
	addButton->set_text("Add Tag");
	addButton->set_custom_minimum_size(godot::Vector2(100, 30));
	addButton->connect("pressed", callable_mp(this, &TagsInContainer::_OnAddButtonClicked));
	hbox2->add_child(addButton);

	auto* clearButton = memnew(godot::Button);
	clearButton->set_text("Clear");
	clearButton->set_custom_minimum_size(godot::Vector2(100, 30));
	//clearButton->connect("pressed", callable_mp(this, &TagsInInspector::_OnAddButtonClicked));
	hbox2->add_child(clearButton);

	AddSelectedTagsTree(gui, mainSplit);
}

void sm::TagsInContainer::AddSelectedTagsTree(godot::Control* gui, godot::VSplitContainer* mainSplit)
{
	m_SelectedTree = memnew(godot::Tree);
	m_SelectedTree->set_columns(1);
	m_SelectedTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_SelectedTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_SelectedTree->set_custom_minimum_size(godot::Vector2(100, 70));

	godot::TreeItem* rootItem = m_SelectedTree->create_item();
	rootItem->set_selectable(0, false);
	m_SelectedTree->set_hide_root(true);
	m_SelectedTree->set_hide_folding(true);

	const auto tags = m_Container->GetTags();
	auto remove = gui->get_theme_icon("Remove", "EditorIcons");

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagID tagId = tags[i];
		godot::TreeItem* item = m_SelectedTree->create_item(rootItem);
		item->set_metadata(0, tagId);
		item->set_text(0, m_IdToResource[tagId]);
		item->add_button(0, remove);
	}

	m_SelectedTree->connect("button_clicked", callable_mp(this, &TagsInContainer::_OnRemoveButtonClicked));
	mainSplit->add_child(m_SelectedTree);
}

void sm::TagsInContainer::RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item)
{
	const bool hasTag = HasTag(tag->GetInternalID());
	bool visible = search.is_empty() || tag->GetTagFullPath().contains(search);
	visible = visible && !hasTag;

	item->set_visible(visible);
	if (visible)
	{
		visibleItems.push_back(item);
	}
}

void sm::TagsInContainer::AddTag(TagID id)
{
	if (HasTag(id) || !m_Container)
	{
		return;
	}

	m_Container->AddTag(id);
}

void sm::TagsInContainer::RemoveTag(TagID id)
{
	if (!m_Container)
	{
		return;
	}

	m_Container->RemoveTag(id);
	m_Container->notify_property_list_changed();
}

godot::PackedInt32Array sm::TagsInContainer::GetTags() const
{
	return m_Container ? m_Container->GetTags() : godot::PackedInt32Array();
}

bool sm::TagsInContainer::HasTag(TagID id) const
{
	return m_Container && m_Container->HasTag(id);
}

void sm::TagsInContainer::_OnAddButtonClicked()
{
	for (auto* item : visibleItems)
	{
		if (item->is_checked(0))
		{
			godot::Ref<TagData> tag = item->get_metadata(0);
			m_Container->AddTag(tag->GetInternalID());
		}
	}

	m_Container->notify_property_list_changed();
}

void sm::TagsInContainer::_OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex)
{
	TagID tagId = item->get_metadata(0);
	m_Container->RemoveTag(tagId);
}

void sm::TagsInContainer::_OnItemActivated(godot::Tree* tree)
{
	godot::TreeItem* item = tree->get_selected();
	if (!item)
	{
		return;
	}

	_OnCheckboxChanged();
}

void sm::TagsInContainer::_OnSearchTextChanged(const godot::String& text)
{
	search = text;
	RefreshAvailableTree();
}
#endif // TOOLS_ENABLED