#include "godot/gdAbilityData.h"

#include "core/TagRegistry.h"
#include "godot/gdGameplayAbility.h"
#include <godot_cpp/classes/engine.hpp>

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

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability"), &GetAbilityScript);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability", "ability"), &SetAbilityScript);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cooldown"), &GetCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cooldown", "value"), &SetCooldown);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cost"), &GetCost);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cost", "value"), &SetCost);

	//
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_tag_id"), &GetAbilityID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_tag_id", "id"), &SetAbilityID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_tags"), &GetAbilityTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_tags", "ids"), &SetAbilityTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_block_ability_tags"), &GetBlockAbilityTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_block_ability_tags", "ids"), &SetBlockAbilityTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_activation_tags"), &GetActivationTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_activation_tags", "ids"), &SetActivationTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_activation_blocked"), &GetActivationBlockedTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_activation_blocked", "ids"), &SetActivationBlockedTags);

	godot::ClassDB::bind_method(godot::D_METHOD("get_effects"), &GetEffects);
	godot::ClassDB::bind_method(godot::D_METHOD("set_effects", "effects"), &SetEffects);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "Tag_ID", godot::PROPERTY_HINT_NONE, "",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"set_ability_tag_id", "get_ability_tag_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT, "ability",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"Script"),
		"set_ability", "get_ability"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "cooldown"),
		"set_cooldown", "get_cooldown"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::FLOAT, "cost"),
		"set_cost", "get_cost"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"effect", godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:EffectData"),
		"set_effects", "get_effects"
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

void sm::AbilityData::SetAbilityScript(const godot::Ref<godot::Script>& script)
{
#ifdef TOOLS_ENABLED
	if (!godot::Engine::get_singleton()->is_editor_hint())
	{
		return;
	}

	if (script.is_valid())
	{
		auto code = script->get_source_code().strip_edges();

		if (code.is_empty())
		{
			godot::String templateCode =
				"extends GameplayAbility\n\n"
				"func _activate_ability():\n"
				"\tpass\n\n"
				"func _end_ability(_was_cancelled : bool):\n"
				"\tpass\n\n"
				"# opt\n"
				"#func _check_availability():\n"
				"\t#pass\n\n"
				"#func _calculate_targets() -> TypedArray<GAS_Entity>:\n"
				"\t#pass\n\n"
				;

			script->set_source_code(templateCode);
			script->reload();

			m_AbilityScript = script;
		}
		else if (code.contains("extends GameplayAbility"))
		{
			m_AbilityScript = script;
		}
		else
		{
			ERR_FAIL_MSG("Script must inherit from GameplayAbility.");
		}
	}
	else
	{
		m_AbilityScript = script;
	}

#endif

	//#ifndef TOOLS_ENABLED
	//	m_AbilityScript = script;
	//#endif
}

void sm::AbilityData::SetAbilityInstance(const godot::Ref<GameplayAbility>& ability)
{
	m_AbilityInstance = ability;
}

void sm::AbilityData::SetAbilityID(TagID id)
{
	m_AbilityTag = id;
}

void sm::AbilityData::SetCooldown(float value)
{
	m_Cooldown = value;
	m_CooldownData->SetDuration(m_Cooldown);
}

void sm::AbilityData::SetCost(float value)
{
	m_Cost = value;

	if (m_CostData.is_null())
	{
		m_CostData.instantiate();
	}

	m_CostData->ClearModifiers();

	auto modifier = memnew(ModifierData);
	modifier->SetValue(-value);
	modifier->SetOperationType(ModifierData::OperationType::Add);
	modifier->SetTargetID(m_CostAttributeID);
	m_CostData->AddModifier(modifier);
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

godot::TypedArray<sm::EffectData> sm::AbilityData::GetEffects()
{
	return m_Effects;
}

void sm::AbilityData::SetEffects(const godot::TypedArray<EffectData>& effects)
{
	m_Effects = effects;
}
