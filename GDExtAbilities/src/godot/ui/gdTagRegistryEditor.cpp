#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagRegistryEditor.h"

#include "core/TagRegistry.h"
#include "godot/gdTagContainer.h"
#include "godot/ui/gdTagsInContainer.h"
#include "godot/ui/gdTagsInEffect.h"
#include "godot/ui/gdTagsInAbility.h"
#include <fstream>
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_system_dock.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_split_container.hpp>
#include <regex>

sm::TagRegistryEditor::TagRegistryEditor()
{
	m_TagsCache.reserve(realMaxTags);
}

void sm::TagRegistryEditor::_bind_methods()
{}

void sm::TagRegistryEditor::_enter_tree()
{
	//
	m_ProjectSettings = godot::ProjectSettings::get_singleton();

	//
	//m_Inspector.instantiate();
	//add_inspector_plugin(m_Inspector);
	//m_Inspector->SetEditorPlugin(this);

	m_InspectorRefs.emplace_back(memnew(TagsInContainer));
	m_InspectorRefs.emplace_back(memnew(TagsInEffect()));
	m_InspectorRefs.emplace_back(memnew(TagsInEffect(false)));
	//m_InspectorRefs.emplace_back(memnew(TagsInAbility));

	for (auto& ref : m_InspectorRefs)
	{
		add_inspector_plugin(ref);
		ref->SetEditorPlugin(this);
	}

	CreateTab();

	m_FileSystemDock = get_editor_interface()->get_file_system_dock();

	if (m_FileSystemDock)
	{
		m_FileSystemDock->connect("files_moved", callable_mp(this, &TagRegistryEditor::_OnFileMoved));
		m_FileSystemDock->connect("file_removed", callable_mp(this, &TagRegistryEditor::_OnFileRemoved));
		m_FileSystemDock->connect("folder_moved", callable_mp(this, &TagRegistryEditor::_OnFolderMoved));
	}

	_make_visible(false);
}

void sm::TagRegistryEditor::_exit_tree()
{
	//
	for (auto& ref : m_InspectorRefs)
	{
		remove_inspector_plugin(ref);
	}

	//
	if (m_FileSystemDock)
	{
		godot::Callable filesMovedCb = callable_mp(this, &TagRegistryEditor::_OnFileMoved);
		if (m_FileSystemDock->is_connected("files_moved", filesMovedCb))
		{
			m_FileSystemDock->disconnect("files_moved", filesMovedCb);
		}

		godot::Callable filesRemovedCb = callable_mp(this, &TagRegistryEditor::_OnFileRemoved);
		if (m_FileSystemDock->is_connected("file_removed", filesRemovedCb))
		{
			m_FileSystemDock->disconnect("file_removed", filesRemovedCb);
		}

		godot::Callable foldersMovedCb = callable_mp(this, &TagRegistryEditor::_OnFolderMoved);
		if (m_FileSystemDock->is_connected("folder_moved", foldersMovedCb))
		{
			m_FileSystemDock->disconnect("folder_moved", foldersMovedCb);
		}
		m_FileSystemDock = nullptr;
	}

	ClearTagData(m_TagRegistry);

	if (m_MainSplit)
	{
		remove_control_from_docks(m_MainSplit);
		m_MainSplit->queue_free();
		m_MainSplit = nullptr;
	}
}

void sm::TagRegistryEditor::_make_visible(bool visible)
{
	m_MainSplit->set_visible(visible);
}

const std::vector<godot::Ref<sm::TagData>> sm::TagRegistryEditor::GetTags() const
{
	return m_TagDatas;
}

void sm::TagRegistryEditor::CreateTab()
{
	m_MainSplit = memnew(godot::VSplitContainer);
	m_MainSplit->set_name("Tags");

	auto gui = get_editor_interface()->get_base_control();
	m_Icons.add = gui->get_theme_icon("Add", "EditorIcons");
	m_Icons.edit = gui->get_theme_icon("Edit", "EditorIcons");
	m_Icons.remove = gui->get_theme_icon("Remove", "EditorIcons");
	m_Icons.removeInternal = gui->get_theme_icon("RemoveInternal", "EditorIcons");

	CreateTreeBoxContainer();
	CreateInfoBoxContainer();

	m_MainSplit->connect(
		"visibility_changed",
		callable_mp(this, &TagRegistryEditor::BindContainersSignals)
	);

	add_control_to_dock(EditorPlugin::DOCK_SLOT_LEFT_UL, m_MainSplit);
}

void sm::TagRegistryEditor::CreateTreeBoxContainer()
{
	m_TreeContainer = memnew(godot::VBoxContainer);

	m_Picker = memnew(godot::EditorResourcePicker);
	m_Picker->set_base_type("TagData");
	m_Picker->connect("resource_changed", callable_mp(this, &TagRegistryEditor::_OnRegistryResourceChanged));
	m_Picker->connect("resource_selected", callable_mp(this, &TagRegistryEditor::_OnRegistryResourceSelected));

	m_TreeContainer->add_child(m_Picker);

	auto* hbox = memnew(godot::HBoxContainer);
	hbox->set_alignment(hbox->ALIGNMENT_CENTER);
	m_TreeContainer->add_child(hbox);
	auto gui = get_editor_interface()->get_base_control();

	auto genConstantsButton = memnew(godot::Button);
	genConstantsButton->set_text("Generate tags");
	auto icon = gui->get_theme_icon("ShaderGlobalsOverride", "EditorIcons");
	genConstantsButton->set_button_icon(icon);
	genConstantsButton->connect("pressed", callable_mp(this, &TagRegistryEditor::GenerateConstants));
	genConstantsButton->set_tooltip_text("Generate tags for use");
	hbox->add_child(genConstantsButton);

	auto refreshButton = memnew(godot::Button);
	refreshButton->set_text("Refresh");
	icon = gui->get_theme_icon("Reload", "EditorIcons");
	refreshButton->set_button_icon(icon);
	refreshButton->connect("pressed", callable_mp(this, &TagRegistryEditor::RefreshTreeFromEditorChanges));
	refreshButton->set_tooltip_text("Changes made outside this editor may not appear in the tag tree automatically. Refresh to update it.");
	hbox->add_child(refreshButton);

	m_MainSplit->add_child(m_TreeContainer);

	godot::String resPath = m_ProjectSettings->get_setting(SETTINGS_PATH);

	if (resPath.is_empty() || !godot::ResourceLoader::get_singleton()->exists(resPath))
	{
		SetSetting(SETTINGS_PATH, "");
		return;
	}

	godot::Ref<TagData> resource = godot::ResourceLoader::get_singleton()->load(resPath);
	if (resource.is_valid())
	{
		m_Picker->set_edited_resource(resource);
		CreateTagRegistryData(resource);
		CreateOrUpdateTree();
	}
}

void sm::TagRegistryEditor::CreateInfoBoxContainer()
{
	m_InfoContainer = memnew(godot::VBoxContainer);
	m_MainSplit->add_child(m_InfoContainer);

	godot::MarginContainer* margin = memnew(godot::MarginContainer);
	margin->add_theme_constant_override("margin_left", 24);
	m_InfoContainer->add_child(margin);

	auto* vContainer = memnew(godot::VBoxContainer);
	margin->add_child(vContainer);

	m_CurrentTagInfo = memnew(godot::Label);
	vContainer->add_child(m_CurrentTagInfo);
	m_ParentTagInfo = memnew(godot::Label);
	vContainer->add_child(m_ParentTagInfo);
	m_ReferencesSize = memnew(godot::Label);
	vContainer->add_child(m_ReferencesSize);

	m_ReferencesTree = memnew(godot::Tree);
	m_ReferencesTree->set_columns(1);
	m_ReferencesTree->set_hide_root(true);
	m_ReferencesTree->set_hide_folding(true);
	m_ReferencesTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_ReferencesTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_ReferencesTree->set_custom_minimum_size(godot::Vector2(100, 70));
	m_ReferencesTree->connect(
		"item_activated",
		callable_mp(this, &TagRegistryEditor::_OnReferenceActivated)
	);

	m_InfoContainer->add_child(m_ReferencesTree);
}

void sm::TagRegistryEditor::CreateTagRegistryData(const godot::Ref<TagData>& resource)
{
	m_TagRegistry = resource;
	m_TagRegistryPath = resource->get_path();

	auto cb = callable_mp(this, &TagRegistryEditor::_OnRegistryEdited);
	if (m_TagRegistry->is_connected("changed", cb))
	{
		m_TagRegistry->connect("changed", cb);
	}
}

void sm::TagRegistryEditor::CreateOrUpdateTree()
{
	if (m_Tree)
	{
		m_Tree->clear();
		m_TagsCache.clear();
		m_TagDatas.clear();

#ifdef DEBUG_ENABLED
		m_TagsCacheDebug.clear();
#endif // DEBUG_ENABLED
	}
	else
	{
		m_Tree = memnew(godot::Tree);

		m_Tree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_custom_minimum_size(godot::Size2(200, 200));
		m_Tree->set_columns(1);

		m_TreeContainer->add_child(m_Tree);
		m_Tree->connect("button_clicked", callable_mp(this, &TagRegistryEditor::_OnButtonClicked));

		m_Tree->connect("item_selected", callable_mp(this, &TagRegistryEditor::_OnItemSelected));

		// Don't allow direct edit to avoid dupes
		//m_Tree->connect("item_edited", callable_mp(this, &TagRegistryEditor::_OnItemEdited));
	}

	auto children = m_TagRegistry->GetChildren();
	auto root = CreateRootItem();
	for (size_t i = 0; i < children.size(); i++)
	{
		CreateTag(children[i], root);
	}


}

godot::TreeItem* sm::TagRegistryEditor::CreateRootItem()
{
	godot::TreeItem* root = m_Tree->create_item();
	root->set_text(0, "Tags");
	root->set_tooltip_text(0, "Root tag");
	root->set_metadata(0, m_TagRegistry);
	root->add_button(0, m_Icons.add, static_cast<int>(ButtonId::Add), false, "Add child tag.");
	root->add_button(0, m_Icons.removeInternal, static_cast<int>(ButtonId::DeleteAll), false, "Delete all tags");
	return root;
}

void sm::TagRegistryEditor::CreateTag(const godot::Ref<TagData>& resource, godot::TreeItem* parent)
{
	if (resource.is_null())
	{
		return;
	}

	std::vector<std::pair<godot::Ref<TagData>, godot::TreeItem*>> stack;
	stack.push_back({ resource, parent });

	while (!stack.empty())
	{
		std::pair<godot::Ref<TagData>, godot::TreeItem*> tagPair = stack.back();
		stack.pop_back();

		godot::TreeItem* treeTag = m_Tree->create_item(tagPair.second);
		treeTag->set_text(0, tagPair.first->GetName());
		treeTag->set_metadata(0, tagPair.first);
		treeTag->set_tooltip_text(0, tagPair.first->GetTagFullPath());
		treeTag->set_editable(0, false);

		treeTag->add_button(0, m_Icons.add, static_cast<int>(ButtonId::Add), false, "Add child tag.");

		treeTag->add_button(0, m_Icons.edit, static_cast<int>(ButtonId::Edit), false, "Rename tag.");

		treeTag->add_button(0, m_Icons.remove, static_cast<int>(ButtonId::DeleteSelf), false, "Delete tag. This will also delete its children.");

		m_TagDatas.push_back(tagPair.first);

#ifdef DEBUG_ENABLED
		auto pathDebug = ToStdString(tagPair.first->GetTagFullPath());
#endif // DEBUG_ENABLED

		AddToCache(tagPair.first->GetTagFullPath());

		godot::TypedArray<TagData> children = tagPair.first->GetChildren();
		for (int i = children.size() - 1; i >= 0; --i)
		{
			stack.push_back({ children[i], treeTag });
		}
	}
}

void sm::TagRegistryEditor::ClearTagData(godot::Ref<TagData>& resource)
{
	if (!resource.is_valid())
	{
		return;
	}

	std::vector<godot::Ref<TagData>> stack;
	stack.push_back(resource);

	while (!stack.empty())
	{
		godot::Ref<TagData> tag = stack.back();
		stack.pop_back();

		m_TagsCache.erase(tag->GetTagFullPath());

#ifdef DEBUG_ENABLED
		m_TagsCacheDebug.erase(ToStdString(tag->GetTagFullPath()));
#endif

		auto cb = callable_mp(this, &TagRegistryEditor::CreateOrUpdateTree);
		if (tag->is_connected("changed", cb))
		{
			tag->disconnect("changed", cb);
		}

		godot::TypedArray<TagData> children = tag->GetChildren();
		for (int i = 0; i < children.size(); i++)
		{
			stack.push_back(children[i]);
		}
	}

	resource.unref();
}

void sm::TagRegistryEditor::_OnRegistryResourceChanged(const godot::Ref<godot::Resource>& resource)
{
	if (!resource.is_valid())
	{
		SetSetting(SETTINGS_PATH, "");
		m_Tree->clear();
		return;
	}

	godot::String path = resource->get_path();

	if (path.is_empty())
	{
		path = generatedPath + godot::String("/tag_registry.tres");

		godot::Ref<godot::DirAccess> dir = godot::DirAccess::open("res://");
		dir->make_dir_recursive(generatedPath);
	}

	godot::ResourceSaver::get_singleton()->save(resource, path);

	SetSetting(SETTINGS_PATH, path);

	// If it wasn't saved properly, fail
	if (path.is_empty() || !godot::ResourceLoader::get_singleton()->exists(path))
	{
		SetSetting(SETTINGS_PATH, "");
		m_Tree->clear();
		return;
	}

	CreateTagRegistryData(resource);
	CreateOrUpdateTree();
	GenerateConstants();
}

void sm::TagRegistryEditor::_OnRegistryEdited()
{
	RefreshTreeFromEditorChanges();
}

void sm::TagRegistryEditor::_OnRegistryResourceSelected(const godot::Ref<godot::Resource>& resource, bool inspect)
{
	if (resource.is_valid())
	{
		get_editor_interface()->inspect_object(resource.ptr());
	}
}

void sm::TagRegistryEditor::_OnButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex)
{
	switch (static_cast<ButtonId>(id))
	{
	case ButtonId::Add:
	{
		AddTagButton(item);
	}
	break;
	case ButtonId::DeleteAll:
	{
		m_TagRegistry->Reset();

		m_TagsCache.clear();

#ifdef DEBUG_ENABLED
		m_TagsCacheDebug.clear();
#endif // DEBUG_ENABLED

		m_Tree->clear();
		CreateRootItem();
	}
	break;
	case ButtonId::DeleteSelf:
	{
		if (m_DontShowAgain)
		{
			_OnDeleteTagClicked(item);
		}
		else
		{
			DeleteTagButton(item);
		}
	}
	break;
	case ButtonId::Edit:
	{

	}
	break;
	default:
		break;
	}
}

void sm::TagRegistryEditor::_OnItemSelected()
{
	godot::Ref<TagData> item = GetSelectedItem();
	if (item.is_null())
	{
		m_CurrentTagInfo->set_text("Tag: <null>");
		m_ParentTagInfo->set_text("Path: <null>");
		m_ReferencesTree->clear();
		m_ReferencesSize->set_text(godot::vformat("References: 0"));
		return;
	}

	m_CurrentTagInfo->set_text(godot::vformat("Tag: %s", item->GetTagFullPath()));
	m_ParentTagInfo->set_text(godot::vformat("Path: %s", item->GetPath()));

	const auto& it = m_CurrentReferences.find(item->GetInternalID());

	if (it == m_CurrentReferences.end())
	{
		m_ReferencesTree->clear();
		m_ReferencesSize->set_text(godot::vformat("References: 0"));
		return;
	}

	auto& vec = it->second;
	m_ReferencesTree->clear();

	auto icon = get_editor_interface()->get_base_control()->get_theme_icon("Node", "EditorIcons");

	godot::TreeItem* rootItem = m_ReferencesTree->create_item();
	m_ReferencesTree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_ReferencesTree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
	m_ReferencesTree->set_custom_minimum_size(godot::Size2(100, 70));

	size_t i;
	for (i = 0; i < vec.size(); ++i)
	{
		auto id = vec[i];
		auto* object = godot::ObjectDB::get_instance(id);
		auto* node = godot::Object::cast_to<godot::Node>(object);
		if (!node)
		{
			vec.erase(vec.begin() + i);
			i--;
			continue;
		}

		auto* parent = node->get_parent();
		if (!parent)
		{
			vec.erase(vec.begin() + i);
			i--;
			continue;
		}

		godot::TreeItem* row = m_ReferencesTree->create_item(rootItem);
		get_editor_interface()->edit_node(node);

		row->set_text(0, parent->get_name());
		row->set_icon(0, icon);
		row->set_metadata(0, id);
	}

	m_ReferencesSize->set_text(godot::vformat("References: %d", vec.size()));
}

void sm::TagRegistryEditor::BindContainersSignals()
{
	auto* editedSceneRoot = get_editor_interface()->get_edited_scene_root();
	if (!m_MainSplit->is_visible_in_tree() || !editedSceneRoot)
	{
		return;
	}

	std::vector<TagContainer*> containers = NodeUtils::GetAllChildNodesOfType<TagContainer>(editedSceneRoot);

	for (auto* container : containers)
	{
		auto callAdded = callable_mp(this, &TagRegistryEditor::_TagAddedToContainer);
		if (!container->is_connected("tag_added", callAdded))
		{
			container->connect("tag_added", callAdded);

			godot::PackedInt32Array arr = container->GetTags();
			for (size_t i = 0; i < arr.size(); i++)
			{
				_TagAddedToContainer(arr[i], container);
			}
		}

		auto callRemoved = callable_mp(this, &TagRegistryEditor::_TagRemovedFromContainer);
		if (!container->is_connected("tag_removed", callRemoved))
		{
			container->connect("tag_removed", callRemoved);
		}
	}

	_OnItemSelected();
}

void sm::TagRegistryEditor::AddTagButton(godot::TreeItem* item)
{
	godot::AcceptDialog* menu = memnew(godot::AcceptDialog);
	menu->set_min_size(godot::Size2(300, 200));
	menu->set_initial_position(godot::Window::WINDOW_INITIAL_POSITION_CENTER_OTHER_SCREEN);
	menu->set_title("Create new tag");
	menu->add_cancel_button("Cancel");
	menu->set_exclusive(true);

	m_TreeContainer->add_child(menu);

	godot::VBoxContainer* vertical = memnew(godot::VBoxContainer);
	menu->add_child(vertical);

	godot::TextureRect* img = memnew(godot::TextureRect);
	auto gui = get_editor_interface()->get_base_control();
	img->set_texture(gui->get_theme_icon("ToolAddNode", "EditorIcons"));
	img->set_expand_mode(godot::TextureRect::EXPAND_FIT_WIDTH);
	img->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	img->set_custom_minimum_size(godot::Size2(80, 80));
	vertical->add_child(img);

	godot::MarginContainer* margin = memnew(godot::MarginContainer);
	margin->add_theme_constant_override("margin_top", 12);
	margin->add_theme_constant_override("margin_bottom", 24);
	vertical->add_child(margin);

	godot::HBoxContainer* box = memnew(godot::HBoxContainer);
	margin->add_child(box);

	godot::Label* label = memnew(godot::Label);
	label->set_text("New tag:");
	box->add_child(label);

	godot::LineEdit* newTagName = memnew(godot::LineEdit);
	newTagName->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
	box->add_child(newTagName);

	//
	godot::MarginContainer* margin2 = memnew(godot::MarginContainer);
	margin2->add_theme_constant_override("margin_bottom", 36);
	vertical->add_child(margin2);

	godot::Label* labelResult = memnew(godot::Label);
	godot::Ref<TagData> tag = item->get_metadata(0);
	godot::String path = tag->GetTagFullPath();
	labelResult->set_text(godot::vformat("Full tag: %s.", path));
	margin2->add_child(labelResult);

	menu->connect("confirmed", callable_mp(this, &TagRegistryEditor::_OnCreateTagClicked).bind(newTagName, item, menu));

	menu->connect("canceled", godot::Callable(menu, "queue_free"));

	newTagName->connect(
		"text_changed",
		callable_mp(this, &sm::TagRegistryEditor::_OnCreateTagNameChanged).bind(item, labelResult)
	);

	menu->show();
}

void sm::TagRegistryEditor::DeleteTagButton(godot::TreeItem* item)
{
	godot::AcceptDialog* menu = memnew(godot::AcceptDialog);
	menu->set_min_size(godot::Size2(300, 200));
	menu->set_initial_position(godot::Window::WINDOW_INITIAL_POSITION_CENTER_OTHER_SCREEN);
	menu->add_cancel_button("Cancel");
	menu->set_exclusive(true);

	m_TreeContainer->add_child(menu);

	godot::MarginContainer* margin = memnew(godot::MarginContainer);
	margin->add_theme_constant_override("margin_left", 12);
	margin->add_theme_constant_override("margin_right", 12);
	margin->add_theme_constant_override("margin_top", 12);
	margin->add_theme_constant_override("margin_bottom", 12);
	menu->add_child(margin);

	godot::VBoxContainer* vertical = memnew(godot::VBoxContainer);
	margin->add_child(vertical);

	godot::TextureRect* img = memnew(godot::TextureRect);
	img->set_texture(m_Icons.remove);
	img->set_expand_mode(godot::TextureRect::EXPAND_FIT_WIDTH);
	img->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	img->set_custom_minimum_size(godot::Size2(80, 80));
	vertical->add_child(img);

	godot::Label* label = memnew(godot::Label);
	label->set_text("Delete tag?");
	label->set_horizontal_alignment(godot::HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	label->add_theme_font_size_override("font_size", 24);
	vertical->add_child(label);

	godot::Label* labelDescription = memnew(godot::Label);
	labelDescription->set_text("Your tag will be permanently deleted.\nThis includes unsaved tag children.");
	labelDescription->set_horizontal_alignment(godot::HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	vertical->add_child(labelDescription);

	godot::MarginContainer* hMargin = memnew(godot::MarginContainer);
	margin->add_theme_constant_override("margin_top", 24);
	margin->add_theme_constant_override("margin_bottom", 12);
	vertical->add_child(hMargin);

	godot::HBoxContainer* checkBoxContainer = memnew(godot::HBoxContainer);
	hMargin->add_child(checkBoxContainer);

	godot::CheckBox* dontShowCheckbox = memnew(godot::CheckBox);
	checkBoxContainer->add_child(dontShowCheckbox);
	dontShowCheckbox->set_h_size_flags(godot::Control::SIZE_SHRINK_BEGIN);
	dontShowCheckbox->set_v_size_flags(godot::Control::SIZE_SHRINK_CENTER);

	godot::Label* labelCheckBox = memnew(godot::Label);
	labelCheckBox->set_text("Don't ask again.");
	checkBoxContainer->add_child(labelCheckBox);

	menu->connect("confirmed", callable_mp(this, &TagRegistryEditor::_OnDeleteTagClicked).bind(item, dontShowCheckbox, menu));

	menu->connect("canceled", godot::Callable(menu, "queue_free"));

	menu->show();
}

void sm::TagRegistryEditor::SaveRegistryResource()
{
	if (!m_TagRegistryPath.is_empty() && m_TagRegistry.is_valid())
	{
		godot::ResourceSaver::get_singleton()->save(m_TagRegistry, m_TagRegistryPath);
	}
}

void sm::TagRegistryEditor::SetSetting(const char* settingPath, godot::String  value)
{
	m_ProjectSettings->set_setting(settingPath, value);
	m_ProjectSettings->save();
}

//void sm::TagRegistryEditor::_OnItemEdited()
//{
//	godot::TreeItem* edited = m_Tree->get_edited();
//
//
//}

void sm::TagRegistryEditor::_OnCreateTagClicked(godot::LineEdit* newText, godot::TreeItem* parentItem, godot::AcceptDialog* menu)
{
	ERR_FAIL_COND_MSG((!newText || !parentItem || !menu), "CreateTag failed: Null references");
	ERR_FAIL_COND_MSG(!m_CanBeCreated, "CreateTag failed: conditions were not met.");

	godot::Ref<TagData> parentData = parentItem->get_metadata(0);
	ERR_FAIL_COND_MSG(parentData.is_null(), "CreateTag failed: Tag parent null");

	godot::Ref<TagData> data;
	data.instantiate();
	data->SetName(newText->get_text());
	data->SetFullPath(parentData->GetTagFullPath());

	CreateTag(data, parentItem);
	parentData->AddChild(data);
	SaveRegistryResource();

	menu->queue_free();
}

void sm::TagRegistryEditor::_OnCreateTagNameChanged(const godot::String& newText, godot::TreeItem* item, godot::Label* labelResult)
{
	godot::Ref<TagData> parentResource = item->get_metadata(0);

	godot::String path;
	m_CanBeCreated = false;

	ERR_FAIL_COND_MSG(parentResource.is_null(), "CreateTag failed: parent resource is null.");

	path = godot::String(parentResource->GetTagFullPath()) + "." + newText;

	if (!IsNameValid(newText))
	{
		labelResult->set_text(godot::vformat("Full tag: %s\nWarning: Tag name must follow this structure: {tagParent.tagChild.other} and cannot include special characters.", path));
		labelResult->add_theme_color_override("font_color", godot::Color(1, 0, 0, 1));
		return;
	}

	if (newText.is_empty())
	{
		labelResult->set_text(godot::vformat("Full tag: %s\nWarning: Tag name can't be empty", path));
		labelResult->add_theme_color_override("font_color", godot::Color(1, 0, 0, 1));
		return;
	}

#ifdef DEBUG_ENABLED
	auto pathDebug = ToStdString(path);
#endif // DEBUG

	if (HasTagInCache(path))
	{
		labelResult->set_text(godot::vformat("Full tag: %s\nWarning: This tag already exists", path));
		labelResult->add_theme_color_override("font_color", godot::Color(1, 0, 0, 1));
		return;
	}

	labelResult->set_text(godot::vformat("Full tag: %s", path));
	labelResult->add_theme_color_override("font_color", godot::Color(1, 1, 1, 1));

	m_CanBeCreated = true;
}

void sm::TagRegistryEditor::_OnDeleteTagClicked(godot::TreeItem* item, godot::CheckBox* checkbox, godot::AcceptDialog* menu)
{
	if (checkbox)
	{
		m_DontShowAgain = checkbox->is_pressed();
	}

	godot::Ref<TagData> resource = item->get_metadata(0);
	godot::Ref<TagData> parentResource = item->get_parent()->get_metadata(0);

	DeleteFromVector(resource);
	parentResource->RemoveChild(resource);
	ClearTagData(resource);
	_OnItemSelected();
	SaveRegistryResource();

	m_TagRegistry->emit_changed();

	memdelete(item);
	menu->queue_free();
}

void sm::TagRegistryEditor::DeleteFromVector(const godot::Ref<TagData> resource)
{
	std::vector<godot::Ref<TagData>> toDelete;
	std::vector<godot::Ref<TagData>> stack;
	stack.push_back(resource);

	while (!stack.empty())
	{
		godot::Ref<TagData> t = stack.back();
		stack.pop_back();

		toDelete.push_back(t);

		auto children = t->GetChildren();
		for (size_t i = 0; i < children.size(); i++)
		{
			stack.push_back(children[i]);
		}
	}

	deleteTag.Notify(toDelete);

	std::unordered_set<uint64_t> ids;
	for (auto& tag : toDelete)
	{
		ids.insert(tag->GetInternalID());
	}

	std::erase_if(m_TagDatas,
		[&](const godot::Ref<TagData>& tag)
		{
			return ids.contains(tag->GetInternalID());
		});
}

void sm::TagRegistryEditor::AddToCache(const godot::StringName& tag)
{
	ERR_FAIL_COND_MSG(m_TagsCache.size() == realMaxTags, "Tag count exceeds the number of tags allowed.");

	if (HasTagInCache(tag))
	{
		return;
	}

	m_TagsCache.emplace(tag);

#ifdef DEBUG_ENABLED
	m_TagsCacheDebug.emplace(ToStdString(tag));
#endif // DEBUG_ENABLED
}

bool sm::TagRegistryEditor::HasTagInCache(const godot::StringName& tag)
{
	return m_TagsCache.find(tag) != m_TagsCache.end();
}

void sm::TagRegistryEditor::RemoveFromCache(const godot::Ref<TagData> tag)
{

}

void sm::TagRegistryEditor::DeleteTree()
{
	if (m_Picker)
	{
		m_Picker->set_edited_resource(godot::Ref<TagData>());
	}

	if (m_Tree)
	{
		m_Tree->queue_free();
		m_TreeContainer->remove_child(m_Tree);
		m_Tree = nullptr;
	}

	if (m_TagRegistry.is_valid())
	{
		ClearTagData(m_TagRegistry);
		m_TagRegistryPath = "";
	}

	_OnItemSelected();
}

void sm::TagRegistryEditor::GenerateConstants()
{
	godot::String path = generatedPath + godot::String("/gen_tags.gd");
	godot::String globalPath = godot::ProjectSettings::get_singleton()->globalize_path(path);

	godot::Ref<godot::DirAccess> dir = godot::DirAccess::open("res://");
	dir->make_dir_recursive(generatedPath);

	std::ofstream outfile;
	std::string outputFilePath = ToStdString(globalPath);
	outfile.open(outputFilePath);

	outfile << "# This is a generated file.\n\nclass_name Tags\n\n";

	if (m_TagRegistry.is_null())
	{
		WARN_PRINT_ED("Warning: Invalid Tag Registry.");
		outfile.close();
		return;
	}

	TagRegistry& registry = TagRegistry::Instance();
	registry.Reset();
	registry.RegisterTags(m_TagRegistry);

	std::vector<GameplayTag> tags = registry.GetTags();

	if (tags.empty())
	{
		WARN_PRINT_ED("Warning: 0 Tags created.");
		outfile.close();
		return;
	}

	for (size_t i = 0; i < tags.size(); i++)
	{
		auto gdName = tags[i].GetName();
		auto replaced = gdName.replace(".", "_");

		outfile << "const " << ToStdString(replaced) << " = " << tags[i].GetUID() << "\n";
	}

	outfile.close();
}

void sm::TagRegistryEditor::_TagAddedToContainer(TagID id, const TagContainer* container)
{
	m_CurrentReferences[id].push_back(container->get_instance_id());
}

void sm::TagRegistryEditor::_TagRemovedFromContainer(TagID id, const TagContainer* container)
{
	if (auto map = m_CurrentReferences.find(id); map != m_CurrentReferences.end())
	{
		std::erase(map->second, container->get_instance_id());
	}
}

godot::TreeItem* sm::TagRegistryEditor::GetRoot()
{
	return 	m_Tree->get_root();
}

//godot::TreeItem* sm::TagRegistryEditor::GetAbilityRoot()
//{
//	if (!m_Ability)
//	{
//		auto root = m_Tree->get_root();
//		godot::Ref<TagData> parentData = root->get_metadata(0);
//		ERR_FAIL_COND_MSG(parentData.is_null(), nullptr, "CreateTag failed: Tag parent null");
//
//		godot::Ref<TagData> data;
//		data.instantiate();
//		data->SetName("Ability");
//		data->SetFullPath(m_TagRegistry->GetTagFullPath());
//
//		m_Ability = m_Tree->create_item(root);
//		m_Ability->set_text(0, "Ability");
//		m_Ability->set_metadata(0, data);
//		m_Ability->set_tooltip_text(0, data->GetTagFullPath());
//		m_Ability->set_editable(0, false);
//
//		m_Ability->add_button(0, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag. WARNING: Ability tags are generated, creating them manually is not recommended.");
//
//		m_Ability->add_button(0, m_Icons.remove, static_cast<int>(ButtonId::DELETE_SELF), false, "Delete tag. This will also delete its children. WARNING: Ability tags are generated, deleting them manually is not recommended.");
//
//		m_TagRegistry->AddChild(data);
//		SaveRegistryResource();
//	}
//
//	return m_Ability;
//}

void sm::TagRegistryEditor::_OnFileMoved(const godot::String& oldFile, const godot::String& newFile)
{
	if (!m_TagRegistry.is_valid())
	{
		return;
	}

	if (m_TagRegistry->get_path() != oldFile)
	{
		return;
	}

	SetSetting(SETTINGS_PATH, newFile);

	godot::Ref<TagData> movedResource = godot::ResourceLoader::get_singleton()->load(newFile);
	if (movedResource.is_valid())
	{
		m_TagRegistry = movedResource;
		if (m_Picker)
		{
			m_Picker->set_edited_resource(movedResource);
		}
	}

	RefreshTreeFromEditorChanges();
}

void sm::TagRegistryEditor::_OnFileRemoved(const godot::String& removedFile)
{
	if (!m_TagRegistry.is_valid())
	{
		return;
	}

	if (m_TagRegistryPath != removedFile)
	{
		return;
	}

	SetSetting(SETTINGS_PATH, "");

	DeleteTree();
}

void sm::TagRegistryEditor::_OnFolderMoved(const godot::String& oldFolder, const godot::String& newFolder)
{
	if (!m_TagRegistry.is_valid())
	{
		return;
	}

	godot::String currentPath = m_TagRegistry->get_path();
	godot::String oldPrefix = oldFolder.path_join("subfolder");
	if (currentPath.begins_with(oldPrefix))
	{
		godot::String suffix = currentPath.substr(oldFolder.length(), currentPath.length() - oldFolder.length());
		godot::String movedPath = newFolder + suffix;

		SetSetting(SETTINGS_PATH, movedPath);

		godot::Ref<TagData> movedResource = godot::ResourceLoader::get_singleton()->load(movedPath);
		if (movedResource.is_valid())
		{
			CreateTagRegistryData(movedResource);

			if (m_Picker)
			{
				m_Picker->set_edited_resource(movedResource);
			}
		}
	}

	RefreshTreeFromEditorChanges();
}

void sm::TagRegistryEditor::_OnReferenceActivated()
{
	godot::TreeItem* item = m_ReferencesTree->get_selected();
	if (!item)
	{
		return;
	}

	godot::ObjectID id = item->get_metadata(0);
	auto* object = godot::ObjectDB::get_instance(id);
	auto* node = godot::Object::cast_to<godot::Node>(object);

	if (node)
	{
		get_editor_interface()->edit_node(node);
	}
}

void sm::TagRegistryEditor::RefreshTreeFromEditorChanges()
{
	if (!m_MainSplit || !m_TagRegistry.is_valid())
	{
		return;
	}

#ifdef DEBUG_ENABLED
	m_TagsCacheDebug.clear();
#endif // DEBUG_ENABLED

	CreateOrUpdateTree();
	_OnItemSelected();
}

godot::Ref<sm::TagData> sm::TagRegistryEditor::GetSelectedItem() const
{
	if (!m_Tree)
	{
		return {};
	}

	godot::TreeItem* item = m_Tree->get_selected();
	if (!item)
	{
		return {};
	}

	return item->get_metadata(0);
}

bool sm::TagRegistryEditor::IsNameValid(const godot::String& name) const
{
	//Tags must follow this structure: <root>.tagParent.tagChild.other
	std::regex rgx(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*$)");
	return std::regex_match(ToStdString(name), rgx);
}

#endif // TOOLS_ENABLED
