#include "godot/gdGASWorld.h"

#include "core/EffectSystem.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/accept_dialog.hpp>

sm::GAS_World::GAS_World()
{

}

void sm::GAS_World::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_entity_count"), &GetEntitiesCount);

	godot::ClassDB::bind_method(godot::D_METHOD("get_effects_availability"), &GetEffectsAvailability);
	godot::ClassDB::bind_method(godot::D_METHOD("set_effects_availability", "value"), &SetEffectsAvailability);

	godot::ClassDB::bind_method(godot::D_METHOD("get_abilities_availability"), &GetAbilitiesAvailability);
	godot::ClassDB::bind_method(godot::D_METHOD("set_abilities_availability", "value"), &SetAbilitiesAvailability);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "entity_count",
		godot::PROPERTY_HINT_NONE,
		"",
		godot::PROPERTY_USAGE_DEFAULT | godot::PROPERTY_USAGE_READ_ONLY),
		"", "get_entity_count"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::BOOL, "enable_effects"),
		"set_effects_availability", "get_effects_availability"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::BOOL, "enable_abilities"),
		"set_abilities_availability", "get_abilities_availability"
	);
}

void sm::GAS_World::OnEnterTree()
{
	godot::SceneTree* tree = get_tree();
	if (!tree)
	{
		return;
	}

	godot::Node* globalRoot = tree->get_root();
	std::vector<GAS_World*> worldsInScene = GetAllChildNodesOfType<GAS_World>(globalRoot);

	if (worldsInScene.empty())
	{
		queue_free();
		ERR_FAIL_MSG("GAS: Could not create world.");
	}

	for (GAS_World* world : worldsInScene)
	{
		if (world != this)
		{
			queue_free();
			ERR_FAIL_MSG("GAS: Only 1 World allowed in scene.");
		}
	}

	m_EntitiesRegistry.emplace(m_EntityUIDs.GenerateUID(), nullptr);
}

void sm::GAS_World::OnExitTree()
{
	int entityNum = m_EntitiesRegistry.size();

	for (const auto& [id, entity] : m_EntitiesRegistry)
	{
		if (entity)
		{
			entity->queue_free();
		}
		else
		{
			m_EntitiesRegistry.erase(id);
		}
	}

	m_Entities.clear();

	if (entityNum > 1)
	{
		WARN_PRINT(godot::vformat("%d entities were also deleted.", entityNum - 1));
	}
}

void sm::GAS_World::OnReady()
{
	set_process(true);

	if (enableEffects && !m_EffectsSystem)
	{
		m_EffectsSystem = std::make_unique<EffectSystem>(this);
	}

	/*if (enableAbilities)
	{

	}*/
}

void sm::GAS_World::OnProcess()
{
	if (enableEffects)
	{
		m_EffectsSystem->Update(get_process_delta_time());
	}
}

void sm::GAS_World::SetEffectsAvailability(bool value)
{
	enableEffects = value;

	if (enableEffects)
	{
		m_EffectsSystem = std::make_unique<EffectSystem>(this);
	}
	else
	{
		m_EffectsSystem.reset();
	}
}

void sm::GAS_World::SetAbilitiesAvailability(bool value)
{
	enableAbilities = value;

	/*if (enableAbilities)
	{
		m_EffectsSystem = std::make_unique<EffectSystem>();
	}
	else
	{
		m_EffectsSystem.reset();
	}*/
}

sm::GAS_Entity* sm::GAS_World::GetEntity(EntityID id)
{
	if (auto it = m_EntitiesRegistry.find(id); it != m_EntitiesRegistry.end())
	{
		return it->second;
	}

	return nullptr;
}

EntityID sm::GAS_World::RegisterEntity(GAS_Entity* entity)
{
	if (m_Entities.find(entity) != m_Entities.end())
	{
		return entity->GetID();
	}

	EntityID id = m_EntityUIDs.GenerateUID();
	entity->SetID(id);
	m_Entities.emplace(entity);
	m_EntitiesRegistry.emplace(id, entity);
	notify_property_list_changed();

	return id;
}

void sm::GAS_World::UnregisterEntity(GAS_Entity* entity)
{
	m_Entities.erase(entity);
	m_EntitiesRegistry.erase(entity->GetID());
	notify_property_list_changed();
}