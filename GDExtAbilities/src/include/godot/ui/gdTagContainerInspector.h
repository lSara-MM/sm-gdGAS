#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>

namespace godot
{
	class Tree;
	class TreeItem;
}

namespace sm
{
	class TagContainer;
	class TagRegistryEditor;

	class TagContainerInspector : public godot::EditorInspectorPlugin
	{
		GDCLASS(TagContainerInspector, godot::EditorInspectorPlugin)

	protected:
		static void _bind_methods();

	public:

		bool _can_handle(godot::Object* object) const override;
		void _parse_begin(godot::Object* object) override;
		bool _parse_property(Object* object,
			godot::Variant::Type type,
			const godot::String& name,
			godot::PropertyHint hintType,
			const godot::String& hintString,
			godot::BitField<godot::PropertyUsageFlags> usageFlags,
			bool wide) override;

		void SetEditorPlugin(TagRegistryEditor* editor);

	private:
		void RefreshTreeSetter();

		void _OnCheckboxChanged();
		void _OnAddButtonClicked();
		void _OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);
		void _OnSearchTextChanged(const godot::String& text);

	private:
		TagContainer* m_Container;
		godot::Tree* m_Tree;
		TagRegistryEditor* m_Editor = nullptr;

		godot::String m_Search;

		std::vector<godot::TreeItem*> m_VisibleTreeItems;
	};
}

#endif // TOOLS_ENABLED