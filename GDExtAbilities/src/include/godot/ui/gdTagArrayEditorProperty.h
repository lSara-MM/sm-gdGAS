#pragma once
#ifdef TOOLS_ENABLED
#include "godot/gdTagData.h"
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <unordered_map>
#include <vector>

namespace godot
{
	class Control;
	class BoxContainer;
	class Button;
	class Tree;
	class TreeItem;
}

namespace sm
{
	class TagRegistryEditor;

	class TagArrayEditorProperty : public godot::EditorProperty
	{
		GDCLASS(TagArrayEditorProperty, godot::EditorProperty)

	protected:
		static void _bind_methods() {};

	public:
		TagArrayEditorProperty(TagRegistryEditor* registry = nullptr);

		virtual void ShowInspector(godot::Object* object);

		godot::Control* ShowTagTreeEditor();
		void AddAvailableTagsTree(godot::Control* root);
		virtual void AddSearchControls(godot::BoxContainer* root, godot::Control* gui);

		void _update_property() override;

		godot::PackedInt32Array GetCurrentTags();
		void SetCurrentTags();
		bool HasTag(TagID tagId);
		void DeleteTags(const std::vector<godot::Ref<TagData>>& tagsToDelete);

		int GetTagsSize() const;

		virtual void Refresh();
		void RefreshAvailableTree();
		virtual void SetSize();
		virtual void RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item);

		virtual void _OnSearchTextChanged(const godot::String& text);

		godot::PackedInt32Array SetCheckbox(godot::TreeItem* item, bool value = true);

	private:
		virtual void _OnCheckboxChanged();
		virtual void _OnItemActivated();
		void _OnShowEnabledToggled(bool toggled);
		void _OnShowDisabledToggled(bool toggled);

	public:
		godot::Tree* availableTree = nullptr;
		TagRegistryEditor* editor = nullptr;
		godot::Control* root = nullptr;
		godot::String search;
		std::unordered_map<godot::StringName, godot::TreeItem*> itemsByName;
		std::vector<godot::TreeItem*> visibleItems;
		godot::PackedInt32Array tags;
		std::unordered_map<TagID, godot::StringName> idToResource;

	private:
		godot::Button* m_ShowEnabledButton = nullptr;
		godot::Button* m_ShowDisabledButton = nullptr;

		bool m_ShowEnabled = true;
		bool m_ShowDisabled = true;
	};
}
#endif