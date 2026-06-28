#include "godot/gdAbilityData.h"

#include "core/TagRegistry.h"

sm::AbilityData::AbilityData()
{
	if (m_CooldownData.is_null())
	{
		m_CooldownData.instantiate();
	}

	m_CooldownData->SetDuration(m_Cooldown);
}

void sm::AbilityData::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_id"), &GetAbilityID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_id", "id"), &SetAbilityID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cooldown"), &GetCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cooldown", "value"), &SetCooldown);

	//
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_tag_id"), &GetAbilityTag);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_tag_id", "id"), &SetAbilityTag);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_tags"), &GetAbilityTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_tags", "ids"), &SetAbilityTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_block_ability_tags"), &GetBlockAbilityTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_block_ability_tags", "ids"), &SetBlockAbilityTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_activation_tags"), &GetActivationTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_activation_tags", "ids"), &SetActivationTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_activation_blocked"), &GetActivationBlockedTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_activation_blocked", "ids"), &SetActivationBlockedTags);

	/*ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "ID"),
		"set_ability_id", "get_ability_id"
	);*/

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "cooldown"),
		"set_cooldown", "get_cooldown"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "Tag_ID", godot::PROPERTY_HINT_NONE, "",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"set_ability_tag_id", "get_ability_tag_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "tags"),
		"set_ability_tags", "get_ability_tags"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "block_tags"),
		"set_block_ability_tags", "get_block_ability_tags"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "activation_tags"),
		"set_activation_tags", "get_activation_tags"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "activation_blocked"),
		"set_ability_activation_blocked", "get_ability_activation_blocked"
	);
}

void sm::AbilityData::SetAbilityID(godot::StringName name)
{
	//#ifdef TOOLS_ENABLED
	//	TagRegistry& registry = TagRegistry::Instance();
	//	if (!registry.RegisterTag(m_ID))
	//	{
	//		return;
	//	}
	//#endif // TOOLS_ENABLED

	m_ID = name;
	//m_CooldownData.AddTag(m_CooldownData.GetName());
}

void sm::AbilityData::SetAbilityTag(TagID id)
{
	m_AbilityTag = id;
}

void sm::AbilityData::SetCooldown(float value)
{
	m_Cooldown = value;
	m_CooldownData->SetDuration(m_Cooldown);
}

void sm::AbilityData::SetAbilityTags(godot::PackedInt32Array arr)
{
	m_AbilityTags = arr;
}

void sm::AbilityData::SetBlockAbilityTags(godot::PackedInt32Array arr)
{
	m_BlockAbilitiesWithTag = arr;
}

void sm::AbilityData::SetActivationTags(godot::PackedInt32Array arr)
{
	m_ActivationRequirements = arr;
}

void sm::AbilityData::SetActivationBlockedTags(godot::PackedInt32Array arr)
{
	m_ActivationBlocked = arr;
}
