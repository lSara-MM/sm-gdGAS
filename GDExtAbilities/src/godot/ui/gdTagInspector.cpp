#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagInspector.h"

#include "godot/gdAbilityData.h"
#include "godot/gdEffectData.h"
#include "godot/gdTagContainer.h"
#include "godot/gdTagData.h"
#include "godot/ui/gdTagArrayEditorProperty.h"
#include "godot/ui/gdTagContainerEditorProperty.h"
#include "godot/ui/gdTagRegistryEditor.h"

void sm::TagInspector::_bind_methods()
{}

bool sm::TagInspector::_can_handle(godot::Object* object) const
{
	return godot::Object::cast_to<TagContainer>(object) ||
		godot::Object::cast_to<EffectData>(object) ||
		godot::Object::cast_to<AbilityData>(object);
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
#endif // TOOLS_ENABLED