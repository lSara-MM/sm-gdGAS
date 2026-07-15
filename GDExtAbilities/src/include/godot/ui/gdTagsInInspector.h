#pragma once
#ifdef TOOLS_ENABLED
#include "godot/gdAbilityData.h"
#include "godot/gdEffectData.h"
#include "godot/gdTagData.h"

#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/object_id.hpp>

namespace godot
{
	class BoxContainer;
	class Button;
	class Control;
	class Tree;
	class TreeItem;
	class VBoxContainer;
	class VSplitContainer;
}

namespace sm
{
	class TagContainer;
	class TagRegistryEditor;

	class TagsInInspector : public godot::EditorInspectorPlugin
	{
		GDCLASS(TagsInInspector, godot::EditorInspectorPlugin)

	protected:
		static void _bind_methods() {};

	public:
		void SetEditorPlugin(TagRegistryEditor* editor);

		void ShowTagTreeEditor(godot::Object* object, const godot::String& title);
		virtual void SetCustomProperties(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit, const int tagsSize);
		virtual void AddSearchControls(godot::BoxContainer* root, godot::Control* gui);

		int AddAvailableTagsTree(godot::VBoxContainer* root);
		void RefreshAvailableTree();
		virtual void RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item);
		std::vector<TagID> SetCheckbox(godot::TreeItem* item, bool value = true);

		virtual godot::PackedInt32Array GetTags() const { return godot::PackedInt32Array{}; };
		virtual bool HasTag(TagID id) const { return false; };
		virtual void AddTag(TagID id) {};
		virtual void RemoveTag(TagID id) {};

		void DeleteTags(const std::vector<godot::Ref<TagData>>& tags);

		virtual void _OnCheckboxChanged();
		virtual void _OnSearchTextChanged(const godot::String& text);
		virtual void _OnItemActivated(godot::Tree* tree) {};
		void _OnShowEnabledToggled(bool toggled);
		void _OnShowDisabledToggled(bool toggled);

	public:

		struct VisibilityButton
		{
			godot::Button* button;
			godot::Ref<godot::Texture2D> visibleIcon;
			godot::Ref<godot::Texture2D> invisibleIcon;
		};

		TagRegistryEditor* registryEditor = nullptr;
		std::unordered_map<TagID, godot::StringName> m_IdToResource;
#ifdef DEBUG_ENABLED
		std::unordered_map<std::string, godot::TreeItem*> m_ItemsByNameDebug;
#endif // DEBUG_ENABLED

		godot::Tree* availableTree = nullptr;
		godot::String search;
		bool showEnabled = true;
		bool showDisabled = true;
		std::vector<godot::TreeItem*> visibleItems;
		std::unordered_map<godot::StringName, godot::TreeItem*> itemsByName;

		VisibilityButton enabledButton;
		VisibilityButton disabledButton;
	};
}

#endif // TOOLS_ENABLED
