#include "godot/ui/gdTagRegistryEditor.h"

#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/v_split_container.hpp>

sm::TagRegistryEditor::TagRegistryEditor() : m_SettingsPath("GDExtAbilities/tag_registry_path")
{}

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
	_make_visible(false);
}

void sm::TagRegistryEditor::_exit_tree()
{
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

void sm::TagRegistryEditor::CreateTreeBoxContainer()
{
	m_TreeContainer = memnew(godot::VBoxContainer);

	m_Picker = memnew(godot::EditorResourcePicker);
	m_Picker->set_base_type("TagData");
	m_Picker->connect("resource_changed", callable_mp(this, &TagRegistryEditor::_OnResourceChanged));

	m_TreeContainer->add_child(m_Picker);
	m_MainSplit->add_child(m_TreeContainer);
}

void sm::TagRegistryEditor::CreateTree(const godot::Ref<TagData> resource)
{
	if (m_Tree)
	{
		m_Tree->clear();
	}
	else
	{
		m_Tree = memnew(godot::Tree);

		m_Tree->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_v_size_flags(godot::Control::SIZE_EXPAND_FILL);
		m_Tree->set_custom_minimum_size(godot::Size2(200, 300));
		m_Tree->set_columns(2);

		m_TreeContainer->add_child(m_Tree);
		m_Tree->connect("button_clicked", callable_mp(this, &TagRegistryEditor::_OnButtonClicked));
	}

	godot::TreeItem* root = m_Tree->create_item();
	root->set_text(0, "Tags");
	root->set_tooltip_text(0, "Root tag");
	root->add_button(1, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");
	root->add_button(1, m_Icons.removeInternal, static_cast<int>(ButtonId::DELETE_ALL), false, "Delete all tags");

	std::vector<std::pair<godot::Ref<sm::TagData>, godot::TreeItem*>> stack;
	stack.push_back({ resource, root });

	godot::TreeItem* treeTag = nullptr;

	while (!stack.empty())
	{
		std::pair<godot::Ref<sm::TagData>, godot::TreeItem*> tagPair = stack.back();
		stack.pop_back();

		treeTag = m_Tree->create_item(tagPair.second);
		treeTag->set_text(0, tagPair.first->GetName());
		treeTag->set_tooltip_text(0, tagPair.first->GetTagFullPath());

		treeTag->add_button(1, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");

		treeTag->add_button(1, m_Icons.edit, static_cast<int>(ButtonId::EDIT), false, "Rename tag.");
		treeTag->add_button(1, m_Icons.remove, static_cast<int>(ButtonId::DELETE_SELF), false, "Delete tag. This will also delete its children.");

		godot::TypedArray<TagData> children = tagPair.first->GetChildren();
		for (int i = 0; i < children.size(); i++)
		{
			stack.push_back({ children[i], treeTag });
		}
	}
}

void sm::TagRegistryEditor::_OnResourceChanged(const godot::Ref<godot::Resource> resource)
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

	godot::String resPath = editorSettings->get_setting(m_SettingsPath);

	if (resPath.is_empty() || !godot::ResourceLoader::get_singleton()->exists(resPath))
	{
		editorSettings->erase(m_SettingsPath);
		m_Tree->clear();
		return;
	}

	CreateTree(resource);
}

void sm::TagRegistryEditor::_OnButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex)
{
	switch (static_cast<ButtonId>(id))
	{
	case ButtonId::ADD:
	{
		godot::AcceptDialog* menu = memnew(godot::AcceptDialog);
		menu->set_min_size(godot::Size2(300, 200));
		menu->set_initial_position(godot::Window::WINDOW_INITIAL_POSITION_CENTER_OTHER_SCREEN);
		menu->set_title("Create new tag");
		menu->add_cancel_button("Cancel");
		menu->set_exclusive(true);

		menu->connect("confirmed", callable_mp(this, &TagRegistryEditor::_OnCreateTagClicked).bind(menu));
		menu->connect("canceled", godot::Callable(menu, "queue_free"));

		m_TreeContainer->add_child(menu);
		godot::VBoxContainer* popupBox = memnew(godot::VBoxContainer);
		menu->add_child(popupBox);

		godot::HBoxContainer* box = memnew(godot::HBoxContainer);
		popupBox->add_child(box);

		godot::Label* label = memnew(godot::Label);
		label->set_text("New tag:");
		box->add_child(label);

		godot::LineEdit* newTagName = memnew(godot::LineEdit);
		newTagName->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
		box->add_child(newTagName);

		godot::Label* labelResult = memnew(godot::Label);
		labelResult->set_text("New tag:");
		popupBox->add_child(labelResult);

		newTagName->connect(
			"text_changed",
			callable_mp(this, &sm::TagRegistryEditor::_OnTagNameChanged).bind(labelResult, item)
		);

		menu->show();
	}
	break;
	case ButtonId::DELETE_ALL:
	{
		m_Tree->clear();

		godot::TreeItem* root = m_Tree->create_item();
		root->set_text(0, "Tags");
		root->set_tooltip_text(0, "Root tag");
		root->add_button(1, m_Icons.add, static_cast<int>(ButtonId::ADD), false, "Add child tag.");
		root->add_button(1, m_Icons.removeInternal, static_cast<int>(ButtonId::DELETE_ALL), false, "Delete all tags");
	}
	break;
	case ButtonId::DELETE_SELF:
	{
		memdelete(item);
	}
	break;
	case ButtonId::EDIT:
	{

	}
	break;
	default:
		break;
	}

	ERR_PRINT("button _OnButtonClicked");
}

void sm::TagRegistryEditor::_OnCreateTagClicked(godot::AcceptDialog* menu)
{
	ERR_PRINT("button _OnCreateTagClicked");



	/*godot::TreeItem* root = m_Tree->create_item();
	root->set_text(0, "");*/

	menu->queue_free();
}

void sm::TagRegistryEditor::_OnTagNameChanged(const godot::String& newText, godot::LineEdit* result, godot::TreeItem* item)
{

}

void sm::TagRegistryEditor::DeleteTree()
{
	if (m_Tree)
	{
		m_TreeContainer->remove_child(m_Tree);
		memdelete(m_Tree);
		m_Tree = nullptr;
	}
}
