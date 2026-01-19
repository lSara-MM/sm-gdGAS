#include "GameplayAttribute.h"

#include <godot_cpp/classes/engine.hpp>

void sm::Attribute::_bind_methods()
{
	godot::ClassDB::bind_static_method("Attribute", godot::D_METHOD("create", "base_value", "name"), &Create);
	godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GetName);
	godot::ClassDB::bind_method(godot::D_METHOD("set_name", "name"), &SetName);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "name"),
		"set_name", "get_name"
	);

	godot::ClassDB::bind_method(godot::D_METHOD("get_base_value"), &GetBaseValue);
	godot::ClassDB::bind_method(godot::D_METHOD("set_base_value", "value"), &SetBaseValue);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "base_value"),
		"set_base_value", "get_base_value"
	);
}

godot::Ref<sm::Attribute> sm::Attribute::Create(float base, godot::StringName n)
{
	godot::Ref<sm::Attribute> attr;
	attr.instantiate();
	attr->SetBaseValue(base);
	attr->SetName(n);

	return attr;
}

void sm::Attribute::SetName(godot::StringName n)
{
	if (godot::Engine::get_singleton()->is_editor_hint())
	{
		if (n != name)
		{
			_NotifySetNameListeners(n);
		}
	}

	name = n;
}

void sm::Attribute::SubscribeSetNameEvent(std::function<void(godot::StringName)> callback)
{
	m_SetNameListeners.push_back(callback);
}

void sm::Attribute::_NotifySetNameListeners(godot::StringName newName) const
{
	for (auto& cb : m_SetNameListeners)
	{
		cb(newName);
	}
}
