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

}

void sm::GAS_World::_notification(int notification)
{
	switch (notification)
	{
	case NOTIFICATION_ENTER_TREE:
		OnEnterTree();
		break;
	case NOTIFICATION_EXIT_TREE:
		OnExitTree();
		break;
	case NOTIFICATION_READY:
		OnReady();
		break;
	case NOTIFICATION_PROCESS:
		OnProcess();
		break;
	}
}

void sm::GAS_World::OnEnterTree()
{
	if (GAS_World::GetSingleton())
	{
		queue_free();
		ERR_FAIL_MSG("GAS: Only one GAS_World allowed.");
	}

	m_Instance = this;
}

void sm::GAS_World::OnExitTree()
{
	if (!m_Entities.empty())
	{
		// TODO: Pop up to delete all entities as well
		ERR_FAIL_MSG("Could not delete GAS_World. Remove");
	}

	m_Instance = nullptr;
}

void sm::GAS_World::OnReady()
{
	if (enableEffects)
	{
		m_EffectsSystem = std::make_unique<EffectSystem>();
	}

	/*if (enableAbility)
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
	m_Entities.emplace(id, entity);

	return id;
}
