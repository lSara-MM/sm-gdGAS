#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagInspector.h"

#include "godot/gdAbilityData.h"
#include "godot/gdEffectData.h"
#include "godot/gdTagContainer.h"
#include "godot/gdTagData.h"
#include "godot/ui/gdTagArrayEditorProperty.h"
#include "godot/ui/gdTagContainerEditorProperty.h"
#include "godot/ui/gdTagRegistryEditor.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

void sm::TagInspector::_bind_methods()
{}

bool sm::TagInspector::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<TagContainer>(object) ||
		godot::Object::cast_to<EffectData>(object) ||
		godot::Object::cast_to<AbilityData>(object);
}

void sm::TagInspector::_parse_begin(godot::Object* object)
{
	auto* gui = m_Editor->get_editor_interface()->get_base_control();
	auto icon = gui->get_theme_icon("Reload", "EditorIcons");

	auto* refreshButton = memnew(godot::Button);
	refreshButton->set_text("Refresh");
	refreshButton->set_button_icon(icon);
	refreshButton->set_tooltip_text("Refresh inspector");

	refreshButton->connect("pressed", callable_mp(this, &TagInspector::RefreshInspector).bind(object));
	add_custom_control(refreshButton);
}

bool sm::TagInspector::_parse_property(
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
		godot::String title = name.capitalize();

		auto* editor = memnew(TagContainerEditorProperty(m_Editor));
		add_property_editor(name, editor);
		editor->ShowInspector(object, title);
		return true;
	}
	else if (type == godot::Variant::PACKED_INT32_ARRAY && name.contains("tag"))
	{
		godot::String title = name.capitalize();

		auto* editor = memnew(TagArrayEditorProperty(m_Editor));
		add_property_editor(name, editor);
		editor->ShowInspector(object, title);
		m_Editor->deleteTag.SubscribeEvent(editor, &TagArrayEditorProperty::DeleteTags);
		return true;
	}

	return false;
}

void sm::TagInspector::SetEditorPlugin(TagRegistryEditor* editor)
{
	m_Editor = editor;
}

void sm::TagInspector::RefreshInspector(godot::Object* object)
{
	if (object)
	{
		object->notify_property_list_changed();
	}
}
#endif // TOOLS_ENABLED