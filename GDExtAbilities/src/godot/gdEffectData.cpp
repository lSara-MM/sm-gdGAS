#include "godot/gdEffectData.h"
#include <godot_cpp/classes/engine.hpp>

void sm::EffectData::_bind_methods()
{
	// Type
	BIND_ENUM_CONSTANT(Permanent);
	BIND_ENUM_CONSTANT(Temporary);
	BIND_ENUM_CONSTANT(Persistent);

	godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GetName);
	godot::ClassDB::bind_method(godot::D_METHOD("set_name", "id"), &SetName);

	godot::ClassDB::bind_method(godot::D_METHOD("get_modifiers"), &GetModifiers);
	godot::ClassDB::bind_method(godot::D_METHOD("set_modifiers", "modifiers"), &SetModifiers);

	godot::ClassDB::bind_method(godot::D_METHOD("get_tags_to_add"), &GetTagsToAdd);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tags_to_add", "tags"), &SetTagsToAdd);

	godot::ClassDB::bind_method(godot::D_METHOD("get_tags_to_remove"), &GetTagsToRemove);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tags_to_remove", "tags"), &SetTagsToRemove);

	godot::ClassDB::bind_method(godot::D_METHOD("get_effect_type"), &GetEffectType);
	godot::ClassDB::bind_method(godot::D_METHOD("set_effect_type", "type"), &SetEffectType);

	godot::ClassDB::bind_method(godot::D_METHOD("get_duration"), &GetDuration);
	godot::ClassDB::bind_method(godot::D_METHOD("set_duration", "time"), &SetDuration);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "name"),
		"set_name", "get_name"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "effect_type",
		godot::PROPERTY_HINT_ENUM, "Permanent, Temporary, Persistent"),
		"set_effect_type", "get_effect_type"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "duration"),
		"set_duration", "get_duration"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"modifiers",
		godot::PROPERTY_HINT_ARRAY_TYPE,
		"24/17:ModifierData"),
		"set_modifiers", "get_modifiers"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "tags_to_add"),
		"set_tags_to_add", "get_tags_to_add"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "tags_to_remove"),
		"set_tags_to_remove", "get_tags_to_remove"
	);
}

void sm::EffectData::SetModifiers(const godot::TypedArray<ModifierData>& modifiers)
{
	for (int i = 0; i < modifiers.size(); i++)
	{
		godot::Ref<ModifierData> data = modifiers[i];

		if (data.is_null())
		{
			continue;
		}

		data->SetSourceID(m_Name);
	}

	m_Modifiers = modifiers;
}

void sm::EffectData::AddModifier(const godot::Ref<ModifierData>& modifier)
{
	m_Modifiers.push_back(modifier);

	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		notify_property_list_changed();
	}
}

void sm::EffectData::ClearModifiers()
{
	m_Modifiers.clear();

	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		notify_property_list_changed();
	}
}

void sm::EffectData::SetEffectType(EffectData::Type lt)
{
	m_EffectType = lt;

	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		notify_property_list_changed();
	}
	}

bool sm::EffectData::AddTagToAdd(TagID id)
{
	if (!m_TagsToAdd.has(id))
	{
		m_TagsToAdd.push_back(id);
		return true;
	}

	return false;
}

bool sm::EffectData::AddTagToRemove(TagID id)
{
	if (!m_TagsToRemove.has(id))
	{
		m_TagsToRemove.push_back(id);
		return true;
	}

	return false;
}

bool sm::EffectData::HasTag(TagID id, const godot::PackedInt32Array& arr) const
{
	return arr.has(id);
}

void sm::EffectData::_validate_property(godot::PropertyInfo& property) const
{
	if (property.name.match("duration"))
	{
		if (m_EffectType != EffectData::Type::Temporary)
		{
			property.usage = godot::PROPERTY_USAGE_NO_EDITOR;
		}
	}
}
