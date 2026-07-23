#include "godot/gdAbilityData.h"

#include "core/TagRegistry.h"
#include "godot/gdGameplayAbility.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <regex>

sm::AbilityData::AbilityData() : m_AbilityTag(0)
{}

void sm::AbilityData::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_name"), &GetAbilityName);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_name", "name"), &SetAbilityName);
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_id"), &GetAbilityID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_id", "id"), &SetAbilityID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_ability"), &GetAbilityScript);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability", "ability"), &SetAbilityScript);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cooldown"), &GetCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cooldown", "value"), &SetCooldown);

	godot::ClassDB::bind_method(godot::D_METHOD("get_cost"), &GetCost);
	godot::ClassDB::bind_method(godot::D_METHOD("set_cost", "value"), &SetCost);

	//
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
		godot::Variant::STRING_NAME, "Tag_Name"),
		"set_ability_name", "get_ability_name"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "Tag_ID", godot::PROPERTY_HINT_NONE, "",
		godot::PROPERTY_USAGE_STORAGE | godot::PROPERTY_USAGE_NO_EDITOR),
		"set_ability_id", "get_ability_id"
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
		godot::Variant::PACKED_INT32_ARRAY, "activation_blocked_tags"),
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

void sm::AbilityData::SetAbilityName(AbilityID name)
{

	if (name.is_empty())
	{
		if (!get_path().is_empty())
		{
			const auto fileName = get_path().get_file().get_basename();
			SetNameToFileName(fileName);
		}
		else
		{
			WARN_PRINT_ED(godot::vformat("Warning: ability name is <null>"));
		}
	}

	if (m_AbilityName == name)
	{
		return;
	}

	SetNameToFileName(name);
	m_AbilityName = name;
}

bool sm::AbilityData::IsNameValid(const godot::String& name)
{
	std::regex rgx(R"(^[a-zA-Z0-9]+$)");
	ERR_FAIL_COND_V_MSG(!std::regex_match(ToStdString(name), rgx),
		false, godot::vformat("Warning: Ability name cannot include special characters."));

	return true;
}

void sm::AbilityData::SetNameToFileName(const godot::String& name)
{
	if (!IsNameValid(name))
	{
		return;
	}

	godot::String fullTagName = "Ability." + name;
	godot::String registryPath = godot::ProjectSettings::get_singleton()->get_setting(SETTINGS_PATH);

	const godot::Ref<TagData>& tagRegistry = godot::ResourceLoader::get_singleton()->load(registryPath);

	if (tagRegistry.is_null())
	{
		return;
	}

	godot::Ref<TagData> abilityData = tagRegistry->FindChildByName("Ability");
	if (abilityData.is_null())
	{
		abilityData.instantiate();
		abilityData->SetName("Ability");
		abilityData->SetPath(tagRegistry->GetTagFullPath());

		tagRegistry->AddChild(abilityData);
	}

	godot::Ref<TagData> prev = abilityData->FindChildByName(name);
	m_AbilityNameDupe = m_AbilityName;
	if (prev.is_valid())
	{
		ERR_FAIL_MSG(godot::vformat("Error: Ability [%s] already exists.", name));
	}

	godot::Ref<TagData> data = abilityData->FindChildByName(m_AbilityNameDupe);
	if (data.is_null())
	{
		data.instantiate();
		abilityData->AddChild(data);
	}

	data->SetName(name);
	data->SetPath(abilityData->GetTagFullPath());

#ifdef DEBUG_ENABLED
	auto a = ToStdString(name);
	auto ae = ToStdString(m_AbilityNameDupe);
	auto ab = ToStdString(abilityData->GetTagFullPath());
	auto abd = ToStdString(tagRegistry->GetTagFullPath());
	auto abc = ToStdString(fullTagName);
#endif // DEBUG_ENABLED

	tagRegistry->emit_changed();
	godot::ResourceSaver::get_singleton()->save(tagRegistry, registryPath);

	emit_changed();
}

void sm::AbilityData::SetAbilityID(TagID id)
{
	m_AbilityTag = id;
}

void sm::AbilityData::SetCooldown(float value)
{
	if (m_CooldownData.is_null())
	{
		m_CooldownData.instantiate();
	}

	m_Cooldown = value;
	m_CooldownData->SetDuration(m_Cooldown);
}

void sm::AbilityData::SetCost(float value)
{
	if (m_CostData.is_null())
	{
		m_CostData.instantiate();
	}

	m_Cost = value;
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

void sm::AbilityData::_validate_property(godot::PropertyInfo& property)
{
	/*if (m_AbilityTag == 0 && !get_path().is_empty())
	{
		SetNameToFileName();
	}*/
}