#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagRegistryEditor.h"

#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/file_system_dock.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_split_container.hpp>
#include <regex>

sm::TagRegistryEditor::TagRegistryEditor() : m_SettingsPath("GDExtAbilities/tag_registry_path")
{
	m_TagsCache.reserve(realMaxTags);
}

void sm::TagRegistryEditor::_bind_methods()
{}

void sm::TagRegistryEditor::_enter_tree()
{
	m_MainSplit = memnew(godot::VSplitContainer);
	m_MainSplit->set_name("Tags");

	auto gui = get_editor_interface()->get_base_control();
	m_Icons.add = gui->get_theme_icon("Add", "EditorIcons");
	m_Icons.edit = gui->get_theme_icon("Edit", "EditorIcons");
	m_Icons.remove = gui->get_theme_icon("Remove", "EditorIcons");
	m_Icons.removeInternal = gui->get_theme_icon("RemoveInternal", "EditorIcons");

	CreateTreeBoxContainer();

	m_InfoContainer = memnew(godot::VBoxContainer);
	m_MainSplit->add_child(m_InfoContainer);

	add_control_to_dock(EditorPlugin::DOCK_SLOT_LEFT_UL, m_MainSplit);

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

void sm::TagRegistryEditor::ClearTagData(godot::Ref<sm::TagData>& resource)
{
	if (!resource.is_valid())
	{
		return;
	}

	std::vector<godot::Ref<sm::TagData>> stack;
	stack.push_back(resource);

	while (!stack.empty())
	{
		godot::Ref<sm::TagData> tag = stack.back();
		stack.pop_back();

		auto cb = callable_mp(this, &TagRegistryEditor::CreateOrUpdateTree);
		if (!tag->is_connected("changed", cb))
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

void sm::TagRegistryEditor::_make_visible(bool visible)
{
	m_MainSplit->set_visible(visible);
}

void sm::TagRegistryEditor::CreateTreeBoxContainer()
{
	m_TreeContainer = memnew(godot::VBoxContainer);

	m_Picker = memnew(godot::EditorResourcePicker);
	m_Picker->set_base_type("TagData");
	m_Picker->connect("resource_changed", callable_mp(this, &TagRegistryEditor::_OnRegistryResourceChanged));
	m_Picker->connect("resource_selected", callable_mp(this, &TagRegistryEditor::_OnRegistryResourceSelected));

	m_TreeContainer->add_child(m_Picker);
	m_MainSplit->add_child(m_TreeContainer);

	auto editorSettings = get_editor_interface()->get_editor_settings();
	godot::String resPath = editorSettings->get_setting(m_SettingsPath);

	if (resPath.is_empty() || !godot::ResourceLoader::get_singleton()->exists(resPath))
	{
		editorSettings->erase(m_SettingsPath);
	}
	else
	{
		godot::Ref<TagData> resource = godot::ResourceLoader::get_singleton()->load(resPath);
		if (resource.is_valid())
		{
			m_Picker->set_edited_resource(resource);
			m_TagRegistry = resource;
			m_TagRegistryPath = resource->get_path();
			CreateOrUpdateTree();
		}
	}
}

void sm::TagRegistryEditor::CreateOrUpdateTree()
{
	if (m_Tree)
	{
		m_Tree->clear();
		m_TagsCache.clear();

#ifdef DEBUG_ENABLED
		m_TagsCacheDebug.clear();
#endif // DEBUG_ENABLED
	}
	else
	{
		m_Tree = memnew(godot::Tree);

		m_Tree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_custom_minimum_size(godot::Size2(200, 300));
		m_Tree->set_columns(1);

		m_TreeContainer->add_child(m_Tree);
		m_Tree->connect("button_clicked", callable_mp(this, &TagRegistryEditor::_OnButtonClicked));

		m_Tree->connect("item_selected", callable_mp(this, &TagRegistryEditor::_OnItemSelected));

		// Don't allow direct edit to avoid dupes
		//m_Tree->connect("item_edited", callable_mp(this, &TagRegistryEditor::_OnItemEdited));
	}

	godot::TreeItem* root = m_Tree->create_item();
	root->set_text(0, "Tags");
	root->set_tooltip_text(0, "Root tag");
	root->set_metadata(0, m_TagRegistry);
	root->add_button(0, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");
	root->add_button(0, m_Icons.removeInternal, static_cast<int>(ButtonId::DELETE_ALL), false, "Delete all tags");

	CreateTag(m_TagRegistry, root);
}

void sm::TagRegistryEditor::CreateTag(const godot::Ref<sm::TagData> resource, godot::TreeItem* parent)
{
	std::vector<std::pair<godot::Ref<sm::TagData>, godot::TreeItem*>> stack;
	stack.push_back({ resource, parent });

	while (!stack.empty())
	{
		std::pair<godot::Ref<sm::TagData>, godot::TreeItem*> tagPair = stack.back();
		stack.pop_back();

		godot::TreeItem* treeTag = m_Tree->create_item(tagPair.second);
		treeTag->set_text(0, tagPair.first->GetName());
		treeTag->set_metadata(0, tagPair.first);
		treeTag->set_tooltip_text(0, tagPair.first->GetTagFullPath());
		treeTag->set_editable(0, false);

		// Refresh tree when resource children change
		auto cb = callable_mp(this, &TagRegistryEditor::CreateOrUpdateTree);
		if (!tagPair.first->is_connected("changed", cb))
		{
			tagPair.first->connect("changed", cb);
		}

		treeTag->add_button(0, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");

		treeTag->add_button(0, m_Icons.edit, static_cast<int>(ButtonId::EDIT), false, "Rename tag.");
		treeTag->add_button(0, m_Icons.remove, static_cast<int>(ButtonId::DELETE_SELF), false, "Delete tag. This will also delete its children.");

		AddToCache(tagPair.first->GetTagFullPath());

		godot::TypedArray<TagData> children = tagPair.first->GetChildren();
		for (int i = 0; i < children.size(); i++)
		{
			stack.push_back({ children[i], treeTag });
		}
	}
}

void sm::TagRegistryEditor::_OnRegistryResourceChanged(const godot::Ref<godot::Resource> resource)
{
	auto editorSettings = get_editor_interface()->get_editor_settings();

	if (!resource.is_valid())
	{
		editorSettings->erase(m_SettingsPath);
		m_Tree->clear();
		return;
	}

	godot::String path = resource->get_path();

	if (resource->get_path().is_empty())
	{
		path = "res://data/tag_registry.tres";

		godot::Ref<godot::DirAccess> dir = godot::DirAccess::open("res://");
		dir->make_dir_recursive("res://data");

		godot::ResourceSaver::get_singleton()->save(resource, path);
	}

	get_editor_interface()->get_editor_settings()->set(m_SettingsPath, path);

	// If it wasn't saved properly, fail
	if (path.is_empty() || !godot::ResourceLoader::get_singleton()->exists(path))
	{
		editorSettings->erase(m_SettingsPath);
		m_Tree->clear();
		return;
	}

	m_TagRegistry = resource;
	m_TagRegistryPath = resource->get_path();

	CreateOrUpdateTree();
}

void sm::TagRegistryEditor::_OnRegistryResourceSelected(const godot::Ref<godot::Resource> resource, bool inspect)
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
	case ButtonId::ADD:
	{
		AddTagButton(item);
	}
	break;
	case ButtonId::DELETE_ALL:
	{
		godot::Ref<TagData> resource = item->get_metadata(0);
		ClearTagData(resource);

		if (!resource->GetTagFullPath().is_empty())
		{
			resource->Reset();
		}

		m_Tree->clear();

		godot::TreeItem* root = m_Tree->create_item();
		root->set_text(0, "Tags");
		root->set_tooltip_text(0, "Root tag");
		root->add_button(0, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");
		root->add_button(0, m_Icons.removeInternal, static_cast<int>(ButtonId::DELETE_ALL), false, "Delete all tags");
	}
	break;
	case ButtonId::DELETE_SELF:
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
	case ButtonId::EDIT:
	{

	}
	break;
	default:
		break;
	}
}

void sm::TagRegistryEditor::_OnItemSelected()
{
	godot::TreeItem* item = m_Tree->get_selected();
	if (!item)
	{
		return;
	}

	// TODO: Info panel. Should show: 
	// Parent tag
	// Number of children and reference to them probably
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
	auto editorSettings = get_editor_interface()->get_editor_settings();
	godot::String registryPath = editorSettings->get_setting(m_SettingsPath);

	if (!registryPath.is_empty() && m_TagRegistry.is_valid())
	{
		godot::ResourceSaver::get_singleton()->save(m_TagRegistry, registryPath);
	}
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
	ERR_FAIL_COND_MSG(m_CanBeCreated, "CreateTag failed: conditions were not met.");

	godot::Ref<sm::TagData> parentData = parentItem->get_metadata(0);
	ERR_FAIL_COND_MSG(parentData.is_null(), "CreateTag failed: Tag parent null");

	godot::Ref<sm::TagData> data;
	data.instantiate();
	data->SetName(newText->get_text());

	data->SetPath(parentData->GetTagFullPath());

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

	if (parentResource->GetTagFullPath().is_empty())
	{
		path = newText;
	}
	else
	{
		path = parentResource->GetTagFullPath() + "." + newText;
	}

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
	ClearTagData(resource);

	godot::Ref<TagData> parentResource = item->get_parent()->get_metadata(0);
	parentResource->RemoveChild(resource);

	SaveRegistryResource();

	memdelete(item);
	menu->queue_free();
}

void sm::TagRegistryEditor::AddToCache(const godot::String& tag)
{
	ERR_FAIL_COND_MSG(m_TagsCache.size() == realMaxTags, "Tag count exceeds the number of tags allowed.");

	if (HasTagInCache(tag))
	{
		return;
	}

	m_TagsCache.push_back(tag);

#ifdef DEBUG_ENABLED
	m_TagsCacheDebug.push_back(ToStdString(tag));
#endif // DEBUG_ENABLED

}

bool sm::TagRegistryEditor::HasTagInCache(const godot::String& tag)
{
	for (auto tagToCheck : m_TagsCache)
	{
		if (tagToCheck == tag)
		{
			return true;
		}
	}

	return false;
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
}

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

	auto editorSettings = get_editor_interface()->get_editor_settings();
	editorSettings->set(m_SettingsPath, newFile);

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

	auto editorSettings = get_editor_interface()->get_editor_settings();
	editorSettings->erase(m_SettingsPath);

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

		auto editorSettings = get_editor_interface()->get_editor_settings();
		editorSettings->set(m_SettingsPath, movedPath);

		godot::Ref<TagData> movedResource = godot::ResourceLoader::get_singleton()->load(movedPath);
		if (movedResource.is_valid())
		{
			m_TagRegistry = movedResource;
			m_TagRegistryPath = movedResource->get_path();

			if (m_Picker)
			{
				m_Picker->set_edited_resource(movedResource);
			}
		}
	}

	RefreshTreeFromEditorChanges();
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
}

bool sm::TagRegistryEditor::IsNameValid(const godot::String& name) const
{
	//Tags must follow this structure: <root>.tagParent.tagChild.other
	std::regex rgx(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*$)");
	return std::regex_match(ToStdString(name), rgx);
}

#endif // TOOLS_ENABLED
