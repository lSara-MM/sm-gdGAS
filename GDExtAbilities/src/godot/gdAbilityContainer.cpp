#include "godot/gdAbilityContainer.h"

#include "godot/gdGASWorld.h"
#include <godot_cpp/classes/engine.hpp>

void sm::AbilityContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_entity_node_path"), &GetEntityNodePath);
	godot::ClassDB::bind_method(godot::D_METHOD("set_entity_node_path", "path"), &SetEntityNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("get_abilities"), &GetAbilities);
	godot::ClassDB::bind_method(godot::D_METHOD("set_abilities", "abilities"), &SetAbilities);

	godot::ClassDB::bind_method(godot::D_METHOD("grant_ability", "ability"), &Grant);
	godot::ClassDB::bind_method(godot::D_METHOD("revoke_ability", "ability"), &Revoke);
	godot::ClassDB::bind_method(godot::D_METHOD("has_ability", "ability"), &Has);

	godot::ClassDB::bind_method(godot::D_METHOD("is_active", "ability"), &IsActive);
	godot::ClassDB::bind_method(godot::D_METHOD("is_on_cooldown", "ability"), &IsOnCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("try_activate", "ability"), &TryActivate);
	godot::ClassDB::bind_method(godot::D_METHOD("try_activate_abilities_with_tags", "ability"), &TryActivateAbilitiesWithTag);

	godot::ClassDB::bind_method(godot::D_METHOD("get_current_cooldown", "ability"), &GetCurrentCooldown);
	godot::ClassDB::bind_method(godot::D_METHOD("clear"), &Clear);

	GDVIRTUAL_BIND(_can_be_granted, "ability");
	GDVIRTUAL_BIND(_can_activate, "ability");
	GDVIRTUAL_BIND(_on_ability_already_granted, "ability");

	//
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"entity_node_path",
		godot::PROPERTY_HINT_NODE_PATH_VALID_TYPES,
		"GAS_Entity",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"set_entity_node_path", "get_entity_node_path"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"abilities",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:AbilityData"),
		"set_abilities", "get_abilities"
	);

	//
	ADD_SIGNAL(godot::MethodInfo("_on_ability_granted",
		godot::PropertyInfo(godot::Variant::OBJECT, "entity",
			godot::PROPERTY_HINT_NODE_TYPE, "GAS_Entity"),
		godot::PropertyInfo(godot::Variant::OBJECT, "ability",
			godot::PROPERTY_HINT_RESOURCE_TYPE, "AbilityData")
	));

	ADD_SIGNAL(godot::MethodInfo("_on_ability_revoked",
		godot::PropertyInfo(godot::Variant::OBJECT, "entity",
			godot::PROPERTY_HINT_NODE_TYPE, "GAS_Entity"),
		godot::PropertyInfo(godot::Variant::OBJECT, "ability",
			godot::PROPERTY_HINT_RESOURCE_TYPE, "AbilityData")
	));

	ADD_SIGNAL(godot::MethodInfo("_on_ability_activated",
		godot::PropertyInfo(godot::Variant::OBJECT, "entity",
			godot::PROPERTY_HINT_NODE_TYPE, "GAS_Entity"),
		godot::PropertyInfo(godot::Variant::OBJECT, "ability",
			godot::PROPERTY_HINT_RESOURCE_TYPE, "AbilityData")
	));

	ADD_SIGNAL(godot::MethodInfo("_on_ability_ended",
		godot::PropertyInfo(godot::Variant::OBJECT, "entity",
			godot::PROPERTY_HINT_NODE_TYPE, "GAS_Entity"),
		godot::PropertyInfo(godot::Variant::OBJECT, "ability",
			godot::PROPERTY_HINT_RESOURCE_TYPE, "AbilityData")
	));

	ADD_SIGNAL(godot::MethodInfo("_on_abilities_cleared",
		godot::PropertyInfo(godot::Variant::OBJECT, "entity",
			godot::PROPERTY_HINT_NODE_TYPE, "GAS_Entity")
	));
}

void sm::AbilityContainer::SetAbilities(const godot::TypedArray<AbilityData>& ability)
{
	m_gdAbilities = ability;
}

godot::Ref<sm::GameplayAbility> sm::AbilityContainer::GetAbilityInstance(const godot::Ref<AbilityData>& ability)
{

	return godot::Ref<GameplayAbility>();
}

void sm::AbilityContainer::OnReady()
{
	m_Owner = NodeUtils::GetParentNodeOfType<GAS_Entity>(this);

	if (!m_Owner)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create AbilityContainer. Node must be in a GAS_Entity node hierarchy.");
	}

	SetEntityNodePath(m_Owner->get_path());
}

void sm::AbilityContainer::OnExitTree()
{
	m_WorldBound.CleanUp();
}

godot::NodePath sm::AbilityContainer::GetEntityNodePath() const
{
	return m_EntityNodePath;
}

void sm::AbilityContainer::SetEntityNodePath(godot::NodePath path)
{
	ERR_FAIL_COND_MSG(path.is_empty(), "Could not set node path. Node must be in a GAS_Entity node hierarchy.");

	m_EntityNodePath = path;
}

void sm::AbilityContainer::AddAbility(const godot::Ref<AbilityData>& ability)
{
	m_gdAbilities.push_back(ability);

#ifdef TOOLS_ENABLED
	notify_property_list_changed();
#endif // TOOLS_ENABLED
}

void sm::AbilityContainer::RemoveAbility(const godot::Ref<AbilityData>& ability)
{
	m_gdAbilities.erase(ability);

#ifdef TOOLS_ENABLED
	notify_property_list_changed();
#endif // TOOLS_ENABLED}
}

bool sm::AbilityContainer::Grant(const godot::Ref<AbilityData>& ability)
{
	ERR_FAIL_COND_V_MSG(ability.is_null(), false, "Grant failed. AbilityData is null.");
	bool ret = true;

	if (GDVIRTUAL_IS_OVERRIDDEN(_can_be_granted))
	{
		GDVIRTUAL_CALL(_can_be_granted, ability, ret);
	}

	if (!ret)
	{
		WARN_PRINT("Ability can't be granted");
		return false;
	}

	if (m_gdAbilities.has(ability) && GDVIRTUAL_IS_OVERRIDDEN(_on_ability_already_granted))
	{
		GDVIRTUAL_CALL(_on_ability_already_granted, ability, ret);
	}

	if (!ret)
	{
		WARN_PRINT("Ability already granted");
		return false;
	}

	AddAbility(ability);

	godot::Ref<godot::Script> script = ability->GetAbilityScript();
	ERR_FAIL_COND_V_MSG(script.is_null(), false, "Grant failed. AbilityData has no valid script.");

	godot::Ref<GameplayAbility> abilityInstance = memnew(GameplayAbility);
	abilityInstance->set_script(script);
	abilityInstance->SetOwner(m_Owner);
	abilityInstance->SetAbilityData(ability);

	//ability->SetAbilityInstance(abilityInstance);

	m_Scripts.emplace(ability->GetAbilityID(), abilityInstance);

	emit_signal("_on_ability_granted", ability, m_Owner);
	return ret;
}

bool sm::AbilityContainer::Revoke(const godot::Ref<AbilityData>& ability)
{
	ERR_FAIL_COND_V_MSG(ability.is_null(), false, "Revoke failed. AbilityData is null.");
	auto itr = m_Scripts.find(ability->GetAbilityID());

	if (itr == m_Scripts.end())
	{
		return false;
	}

	godot::Ref<GameplayAbility> abilityInstance = itr->second;
	ERR_FAIL_COND_V_MSG(abilityInstance.is_null(), false, "Revoke failed. GameplayAbility is null.");

	if (abilityInstance->TryEnd(true))
	{
		emit_signal("_on_ability_ended", ability, m_Owner);
	}

	RemoveAbility(ability);
	emit_signal("_on_ability_revoked", ability, m_Owner);
	return false;
}

void sm::AbilityContainer::Clear()
{
	for (auto& [tagId, abilityInstance] : m_Scripts)
	{
		if (abilityInstance.is_null())
		{
			continue;
		}

		godot::Ref<AbilityData> data = abilityInstance->GetAbilityData();

		if (IsActive(data))
		{
			abilityInstance->TryEnd(true);
		}
	}

	m_gdAbilities.clear();
	m_Scripts.clear();

#ifdef TOOLS_ENABLED
	notify_property_list_changed();
#endif // TOOLS_ENABLED

	emit_signal("_on_abilities_cleared", m_Owner);
}

bool sm::AbilityContainer::IsActive(const godot::Ref<AbilityData>& ability) const
{
	godot::Ref<GameplayAbility> abilityInstance = ability->GetAbilityInstance();
	return abilityInstance->IsActive();
}

bool sm::AbilityContainer::IsOnCooldown(const godot::Ref<AbilityData>& ability) const
{
	godot::Ref<GameplayAbility> abilityInstance = ability->GetAbilityInstance();
	return abilityInstance->IsOnCooldown();
}

float sm::AbilityContainer::GetCurrentCooldown(const godot::Ref<AbilityData>& ability) const
{
	if (auto itr = m_Scripts.find(ability->GetAbilityID());
		itr != m_Scripts.end())
	{
		return itr->second->GetCooldown();
	}

	return -1;
}

bool sm::AbilityContainer::TryActivate(const godot::Ref<AbilityData>& ability)
{
	if (auto itr = m_Scripts.find(ability->GetAbilityID());
		itr != m_Scripts.end())
	{
		return itr->second->TryActivate();
	}

	ERR_PRINT(godot::vformat("TryActivate failed: ability %d not find", ability->GetAbilityID()));

	return false;
}

bool sm::AbilityContainer::TryActivateAbilitiesWithTag(godot::PackedInt32Array tags)
{
	for (size_t i = 0; i < m_gdAbilities.size(); i++)
	{
		godot::Ref<AbilityData> ability = m_gdAbilities[i];
		auto tagsInAbility = ability->GetAbilityTags();
		bool hasMatchingTag = false;

		for (size_t j = 0; j < tags.size(); j++)
		{
			if (tagsInAbility.has(tags[j]))
			{
				hasMatchingTag = true;
				break;
			}
		}

		if (hasMatchingTag)
		{
			auto itr = m_Scripts.find(ability->GetAbilityID());
			if (itr != m_Scripts.end())
			{
				itr->second->TryActivate();
			}
		}
	}

	return true;
}

bool sm::AbilityContainer::Has(const godot::Ref<AbilityData>& ability) const
{
	return m_Scripts.find(ability->GetAbilityID()) != m_Scripts.end();
}
