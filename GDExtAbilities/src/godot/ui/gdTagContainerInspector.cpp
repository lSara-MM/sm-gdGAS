#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagContainerInspector.h"

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

void sm::TagContainerInspector::_bind_methods()
{}

bool sm::TagContainerInspector::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<TagContainer>(object) ||
		godot::Object::cast_to<EffectData>(object) ||
		godot::Object::cast_to<AbilityData>(object);
}

void sm::TagContainerInspector::_parse_begin(godot::Object* object)
{
	m_Contexts.clear();

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

bool sm::TagContainerInspector::_parse_property(
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
		ShowTagTreeEditor(Inspect::CONTAINER, object, "Tags");

#ifndef DEBUG_ENABLED
		return true;
#endif // DEBUG_ENABLED
	}

	if (name == "tags_to_add" && godot::Object::cast_to<EffectData>(object))
	{
		ShowTagTreeEditor(Inspect::EFFECT_ADD, object, "Tags to Add");
		return true;
	}

	if (name == "tags_to_remove" && godot::Object::cast_to<EffectData>(object))
	{
		ShowTagTreeEditor(Inspect::EFFECT_REMOVE, object, "Tags to Remove");
		return true;
	}

	if (name == "tags" && godot::Object::cast_to<AbilityData>(object))
	{
		ShowTagTreeEditor(Inspect::ABILITY, object, "Tags");
		return true;
	}

	if (name == "block_tags" && godot::Object::cast_to<AbilityData>(object))
	{
		ShowTagTreeEditor(Inspect::ABILITY, object, "Block Tags");
		return true;
	}

	if (name == "activation_tags" && godot::Object::cast_to<AbilityData>(object))
	{
		ShowTagTreeEditor(Inspect::ABILITY, object, "Activation Tags");
		return true;
	}

	if (name == "activation_blocked" && godot::Object::cast_to<AbilityData>(object))
	{
		ShowTagTreeEditor(Inspect::ABILITY, object, "Activation Blocked Tags");
		return true;
	}

	return false;
}

void sm::TagContainerInspector::SetEditorPlugin(TagRegistryEditor* editor)
{
	m_Editor = editor;
	m_Editor->deleteTag.SubscribeEvent(this, &TagContainerInspector::DeleteTags);
}

void sm::TagContainerInspector::ShowTagTreeEditor(Inspect current, godot::Object* object, const godot::String& title)
{
	TagTreeContext context;
	context.current = current;
	context.ownerId = object->get_instance_id();

	if (auto* data = godot::Object::cast_to<EffectData>(object))
	{
		context.effect = godot::Ref<EffectData>(data);
	}
	else if (auto* data = godot::Object::cast_to<AbilityData>(object))
	{
		context.ability = godot::Ref<AbilityData>(data);
	}

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

	auto* gui = m_Editor->get_editor_interface()->get_base_control();
	auto* hbox = memnew(godot::HBoxContainer);
	root->add_child(hbox);
	const int tagsSize = AddAvailableTagsTree(root, context);
	AddSearchControls(hbox, gui, context);

	if (current == Inspect::CONTAINER)
	{
		auto* addButton = memnew(godot::Button);
		addButton->set_text("Add Tag");
		addButton->set_custom_minimum_size(godot::Vector2(50, 24));
		addButton->connect("pressed", callable_mp(this, &TagContainerInspector::_OnAddButtonClicked).bind(context.availableTree));
		root->add_child(addButton);

		AddSelectedTagsTree(gui, mainSplit, context);
	}

	const int rowHeight = current == Inspect::CONTAINER ? 50 : 12;
	const int extraHeight = 100;
	mainSplit->set_custom_minimum_size(godot::Vector2(100, tagsSize * rowHeight + extraHeight));

	m_Contexts.emplace(context.availableTree, std::move(context));
	add_custom_control(mainSplit);
}

void sm::TagContainerInspector::AddSearchControls(godot::BoxContainer* root, godot::Control* gui, TagTreeContext& context)
{
	auto* search = memnew(godot::LineEdit);
	search->set_placeholder("Search tag");
	search->set_right_icon(gui->get_theme_icon("Search", "EditorIcons"));
	search->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	root->add_child(search);

	auto visibleIcon = gui->get_theme_icon("GuiVisibilityVisible", "EditorIcons");

	if (context.current == Inspect::CONTAINER)
	{
		search->connect("text_changed", callable_mp(this, &TagContainerInspector::_OnSearchTextChanged).bind(context.availableTree));
		return;
	}

	auto invisibleIcon = gui->get_theme_icon("GuiVisibilityHidden", "EditorIcons");

	auto* showEnabled = memnew(godot::Button);
	showEnabled->set_text("Enabled");
	showEnabled->set_toggle_mode(true);
	showEnabled->set_pressed_no_signal(context.showEnabled);
	showEnabled->set_button_icon(context.showEnabled ? visibleIcon : invisibleIcon);
	root->add_child(showEnabled);

	auto* showDisabled = memnew(godot::Button);
	showDisabled->set_text("Disabled");
	showDisabled->set_toggle_mode(true);
	showDisabled->set_pressed_no_signal(context.showDisabled);
	showDisabled->set_button_icon(context.showDisabled ? visibleIcon : invisibleIcon);
	root->add_child(showDisabled);

	search->connect("text_changed", callable_mp(this, &TagContainerInspector::_OnSearchTextChanged).bind(context.availableTree));
	showEnabled->connect("toggled", callable_mp(this, &TagContainerInspector::_OnShowEnabledToggled).bind(context.availableTree));
	showDisabled->connect("toggled", callable_mp(this, &TagContainerInspector::_OnShowDisabledToggled).bind(context.availableTree));
}

int sm::TagContainerInspector::AddAvailableTagsTree(godot::VBoxContainer* root, TagTreeContext& context)
{
	context.availableTree = memnew(godot::Tree);
	context.availableTree->set_columns(1);
	context.availableTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	context.availableTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	context.availableTree->set_custom_minimum_size(godot::Vector2(100, 70));
	context.availableTree->connect("item_edited", callable_mp(this, &TagContainerInspector::_OnCheckboxChanged).bind(context.availableTree));
	context.availableTree->connect("item_activated", callable_mp(this, &TagContainerInspector::_OnItemActivated).bind(context.availableTree));

	godot::TreeItem* rootItem = context.availableTree->create_item();
	rootItem->set_selectable(0, false);
	context.availableTree->set_hide_root(true);
	context.availableTree->set_hide_folding(true);

	auto& tags = m_Editor->GetTags();
	for (auto& tag : tags)
	{
		auto gdName = tag->GetTagFullPath();
		TagID tagId = tag->GetInternalID();

		godot::TreeItem* item = context.availableTree->create_item(rootItem);
		item->set_metadata(0, tag);
		item->set_cell_mode(0, godot::TreeItem::CELL_MODE_CHECK);
		item->set_editable(0, true);
		item->set_checked(0, false);
		item->set_text(0, gdName);

		m_IdToResource[tagId] = gdName;
		context.itemsByName[gdName] = item;

#ifdef DEBUG_ENABLED
		m_ItemsByNameDebug[ToStdString(gdName)] = item;
#endif // DEBUG_ENABLED

		RefreshTagVisibility(context, tag, item);
	}

	root->add_child(context.availableTree);

	return tags.size();
}

void sm::TagContainerInspector::AddSelectedTagsTree(godot::Control* gui, godot::VSplitContainer* mainSplit, TagTreeContext& context)
{
	context.selectedTree = memnew(godot::Tree);
	context.selectedTree->set_columns(1);
	context.selectedTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	context.selectedTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	context.selectedTree->set_custom_minimum_size(godot::Vector2(100, 70));

	godot::TreeItem* rootItem = context.selectedTree->create_item();
	rootItem->set_selectable(0, false);
	context.selectedTree->set_hide_root(true);
	context.selectedTree->set_hide_folding(true);

	const auto tags = GetTags(context);
	auto remove = gui->get_theme_icon("Remove", "EditorIcons");

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagID tagId = tags[i];
		godot::TreeItem* item = context.selectedTree->create_item(rootItem);
		item->set_metadata(0, tagId);
		item->set_text(0, m_IdToResource[tagId]);
		item->add_button(0, remove);
	}

	context.selectedTree->connect("button_clicked", callable_mp(this, &TagContainerInspector::_OnRemoveButtonClicked).bind(context.availableTree));
	mainSplit->add_child(context.selectedTree);
}

void sm::TagContainerInspector::RefreshAvailableTree(TagTreeContext& context)
{
	context.visibleItems.clear();

	for (godot::TreeItem* item = context.availableTree->get_root()->get_first_child();
		item;
		item = item->get_next())
	{
		godot::Ref<TagData> tag = item->get_metadata(0);
		RefreshTagVisibility(context, tag, item);
	}
}

void sm::TagContainerInspector::RefreshTagVisibility(TagTreeContext& context, const godot::Ref<TagData>& tag, godot::TreeItem* item)
{
	const bool hasTag = HasTag(context, tag->GetInternalID());
	const bool matchesSearch = context.search.is_empty() || tag->GetTagFullPath().contains(context.search);
	bool visible = matchesSearch;

	if (context.current == Inspect::CONTAINER)
	{
		visible = visible && !hasTag;
	}
	else
	{
		visible = visible &&
			((hasTag && context.showEnabled) || (!hasTag && context.showDisabled));
		item->set_checked(0, hasTag);
	}

	item->set_visible(visible);
	if (visible)
	{
		context.visibleItems.push_back(item);
	}
}

std::vector<TagID> sm::TagContainerInspector::SetCheckbox(TagTreeContext& context, godot::TreeItem* item, bool value)
{
	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<TagID> ids;
	ids.push_back(tag->GetInternalID());

	while (tag.is_valid())
	{
		auto it = context.itemsByName.find(tag->GetPath());
		if (it == context.itemsByName.end() || !it->second)
		{
			return ids;
		}

		godot::TreeItem* parentItem = it->second;
		tag = parentItem->get_metadata(0);
		ids.push_back(tag->GetInternalID());
		ERR_PRINT(godot::vformat("adding %d, %s", tag->GetInternalID(), tag->GetTagFullPath()));
		parentItem->set_checked(0, value);
	}

	return ids;
}

godot::PackedInt32Array sm::TagContainerInspector::GetTags(const TagTreeContext& context) const
{
	switch (context.current)
	{
	case Inspect::CONTAINER:
	{
		TagContainer* container = GetContainer(context);
		return container ? container->GetTags() : godot::PackedInt32Array();
	}
	case Inspect::EFFECT_ADD:
		return context.effect.is_valid() ? context.effect->GetTagsToAdd() : godot::PackedInt32Array();
	case Inspect::EFFECT_REMOVE:
		return context.effect.is_valid() ? context.effect->GetTagsToRemove() : godot::PackedInt32Array();
	default:
		return {};
	}
}

void sm::TagContainerInspector::SetTags(const TagTreeContext& context, const godot::PackedInt32Array& tags)
{
	switch (context.current)
	{
	case Inspect::EFFECT_ADD:
		if (context.effect.is_valid())
		{
			context.effect->SetTagsToAdd(tags);
		}
		break;
	case Inspect::EFFECT_REMOVE:
		if (context.effect.is_valid())
		{
			context.effect->SetTagsToRemove(tags);
		}
		break;
	default:
		break;
	}
}

bool sm::TagContainerInspector::HasTag(const TagTreeContext& context, TagID id) const
{
	if (context.current == Inspect::CONTAINER)
	{
		TagContainer* container = GetContainer(context);
		return container && container->HasTag(id);
	}

	return context.effect.is_valid() && context.effect->HasTag(id, GetTags(context));
}

void sm::TagContainerInspector::AddTag(TagTreeContext& context, TagID id)
{
	if (HasTag(context, id))
	{
		return;
	}

	switch (context.current)
	{
	case Inspect::CONTAINER:
	{
		if (TagContainer* container = GetContainer(context))
		{
			container->AddTag(id);
		}
		return;
	}
	break;
	case Inspect::EFFECT_ADD:
		if (context.effect.is_valid())
		{
			context.effect->AddTagToAdd(id);
		}
		break;
	case Inspect::EFFECT_REMOVE:
		if (context.effect.is_valid())
		{
			context.effect->AddTagToRemove(id);
		}
		break;
	default:
		break;
	}
}

void sm::TagContainerInspector::RemoveTag(TagTreeContext& context, TagID id)
{
	if (context.current == Inspect::CONTAINER)
	{
		if (TagContainer* container = GetContainer(context))
		{
			container->RemoveTag(id);
			container->notify_property_list_changed();
		}
		return;
	}

	godot::PackedInt32Array tags = GetTags(context);
	for (int64_t i = tags.size() - 1; i >= 0; --i)
	{
		if (tags[i] == id)
		{
			tags.remove_at(i);
		}
	}

	SetTags(context, tags);
	if (context.effect.is_valid())
	{
		context.effect->emit_changed();
	}
}

sm::TagContainerInspector::TagTreeContext* sm::TagContainerInspector::GetContext(godot::Tree* tree)
{
	auto it = m_Contexts.find(tree);
	return it == m_Contexts.end() ? nullptr : &it->second;
}

sm::TagContainer* sm::TagContainerInspector::GetContainer(const TagTreeContext& context) const
{
	return godot::Object::cast_to<TagContainer>(godot::ObjectDB::get_instance(context.ownerId));
}

void sm::TagContainerInspector::DeleteTags(const std::vector<godot::Ref<TagData>>& tags)
{
	for (auto& [tree, context] : m_Contexts)
	{
		if (context.current != Inspect::CONTAINER)
		{
			continue;
		}

		for (const auto& tag : tags)
		{
			RemoveTag(context, tag->GetInternalID());
		}
	}
}

void sm::TagContainerInspector::_OnCheckboxChanged(godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	godot::TreeItem* item = tree->get_edited();
	if (!item)
	{
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<TagID> ids;

	if (item->is_checked(0))
	{
		ids = SetCheckbox(*context, item);
	}

	if (context->current == Inspect::CONTAINER)
	{
		return;
	}

	if (item->is_checked(0))
	{
		for (TagID id : ids)
		{
			AddTag(*context, id);
		}
	}
	else
	{
		RemoveTag(*context, tag->GetInternalID());
	}

	RefreshAvailableTree(*context);
	context->effect->emit_changed();
}

void sm::TagContainerInspector::_OnAddButtonClicked(godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	for (auto* item : context->visibleItems)
	{
		if (item->is_checked(0))
		{
			godot::Ref<TagData> tag = item->get_metadata(0);
			AddTag(*context, tag->GetInternalID());
		}
	}

	if (TagContainer* container = GetContainer(*context))
	{
		container->notify_property_list_changed();
	}
}

void sm::TagContainerInspector::_OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex, godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	TagID tagId = item->get_metadata(0);
	RemoveTag(*context, tagId);
}

void sm::TagContainerInspector::_OnSearchTextChanged(const godot::String& text, godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	context->search = text;
	RefreshAvailableTree(*context);
}

void sm::TagContainerInspector::_OnItemActivated(godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	godot::TreeItem* item = tree->get_selected();
	if (!item)
	{
		return;
	}

	if (context->current != Inspect::CONTAINER)
	{
		_OnCheckboxChanged(tree);
		return;
	}

	godot::Ref<TagData> tag = item->get_metadata(0);
	std::vector<godot::Ref<TagData>> toAdd{ tag };

	while (tag.is_valid())
	{
		auto it = context->itemsByName.find(tag->GetPath());
		if (it == context->itemsByName.end() || !it->second)
		{
			break;
		}

		godot::TreeItem* parentItem = it->second;
		tag = parentItem->get_metadata(0);
		toAdd.push_back(tag);
	}

	for (godot::Ref<TagData> tagToAdd : toAdd)
	{
		AddTag(*context, tagToAdd->GetInternalID());
	}
}

void sm::TagContainerInspector::_OnShowEnabledToggled(bool toggled, godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	context->showEnabled = toggled;
	RefreshAvailableTree(*context);
}

void sm::TagContainerInspector::_OnShowDisabledToggled(bool toggled, godot::Tree* tree)
{
	TagTreeContext* context = GetContext(tree);
	ERR_FAIL_NULL(context);

	context->showDisabled = toggled;
	RefreshAvailableTree(*context);
}

#endif // TOOLS_ENABLED
