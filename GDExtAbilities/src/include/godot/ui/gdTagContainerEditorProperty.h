#pragma once
#ifdef TOOLS_ENABLED
#include "godot/gdTagData.h"
#include "godot/ui/gdTagArrayEditorProperty.h"
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>

namespace godot
{
	class Control;
	class BoxContainer;
	class Tree;
	class TreeItem;
}

namespace sm
{
	class TagContainer;
	class TagRegistryEditor;

	class TagContainerEditorProperty : public TagArrayEditorProperty
	{
		GDCLASS(TagContainerEditorProperty, TagArrayEditorProperty)

	protected:
		static void _bind_methods() {};

	public:
		TagContainerEditorProperty(TagRegistryEditor* registry = nullptr);

		void ShowInspector(Object* object, const godot::String& title) override;
		void AddSearchControls(godot::BoxContainer* root, godot::Control* gui) override;
		godot::Control* AddSelectedTagsTree();

		void SelectedTree();
		void RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item) override;
		void _OnSearchTextChanged(const godot::String& text) override;

		void Refresh() override;
		void SetSize() override;

	private:
		void _OnCheckboxChanged();
		void _OnAddButtonClicked();
		void _OnDeselectButtonClicked();
		void _OnRemoveAllButtonClicked();
		void _OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);
		void _OnItemActivated(godot::Tree* tree);

	private:
		godot::Tree* m_SelectedTree = nullptr;
	};
}
#endif