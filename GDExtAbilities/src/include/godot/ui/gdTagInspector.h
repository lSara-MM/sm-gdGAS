#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_inspector_plugin.hpp>

namespace godot
{
	class Object;
}

namespace sm
{
	class TagRegistryEditor;

	class TagInspector : public godot::EditorInspectorPlugin
	{
		GDCLASS(TagInspector, godot::EditorInspectorPlugin)

	protected:
		static void _bind_methods();

	public:
		bool _can_handle(godot::Object* object) const override;
		void _parse_begin(godot::Object* object) override;
		bool _parse_property(godot::Object* object,
			godot::Variant::Type type,
			const godot::String& name,
			godot::PropertyHint hintType,
			const godot::String& hintString,
			godot::BitField<godot::PropertyUsageFlags> usageFlags,
			bool wide) override;

		void SetEditorPlugin(TagRegistryEditor* editor);
		void RefreshInspector(godot::Object* object);

	private:
		TagRegistryEditor* m_Editor;
	};
}
#endif // TOOLS_ENABLED
