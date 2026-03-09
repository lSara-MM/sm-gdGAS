#include "gdGASWorld.h"

#include "EffectSystem.h"

namespace sm
{
	GAS_World* GAS_World::m_Instance = nullptr;
}

sm::GAS_World::GAS_World()
{

}

void sm::GAS_World::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_effects_availability"), &GetEffectsAvailability);
	godot::ClassDB::bind_method(godot::D_METHOD("set_effects_availability", "value"), &SetEffectsAvailability);

	godot::ClassDB::bind_method(godot::D_METHOD("get_abilities_availability"), &GetAbilitiesAvailability);
	godot::ClassDB::bind_method(godot::D_METHOD("set_abilities_availability", "value"), &SetAbilitiesAvailability);

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
	if (GAS_World::Instance())
	{
		queue_free();
		ERR_FAIL_MSG("GAS: Only one GAS_World allowed.");
	}

	m_Instance = this;
}

void sm::GAS_World::OnExitTree()
{
	m_Entities.clear();
	m_Instance = nullptr;
}

void sm::GAS_World::OnReady()
{
	set_process(true);
	
	if (enableEffects && !m_EffectsSystem)
	{
		m_EffectsSystem = std::make_unique<EffectSystem>();
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
		m_EffectsSystem = std::make_unique<EffectSystem>();
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
	if (auto it = m_Entities.find(id); it != m_Entities.end())
	{
		return it->second;
	}

	return nullptr;
}

EntityID sm::GAS_World::RegisterEntity(GAS_Entity* entity)
{
	EntityID id = m_EntityUIDs.GenerateUID();
	entity->SetID(id);
	m_Entities.emplace(id, entity);

	return id;
}

void sm::GAS_World::UnregisterEntity(GAS_Entity* entity)
{
	m_Entities.erase(entity->GetID());
}