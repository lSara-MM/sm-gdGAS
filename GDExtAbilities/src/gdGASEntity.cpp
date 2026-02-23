#include "gdGASEntity.h"

#include "gdGASWorld.h"

sm::GAS_Entity::GAS_Entity()
{

}

void sm::GAS_Entity::_bind_methods()
{

}

void sm::GAS_Entity::_notification(int notification)
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
	}
}

void sm::GAS_Entity::OnEnterTree()
{
	sm::GAS_World* world = sm::GAS_World::GetSingleton();
	
	if (!world)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create Entity. GAS_World Node required");
	}

	m_Id = world->RegisterEntity(this);
}

void sm::GAS_Entity::OnExitTree()
{
	
}

void sm::GAS_Entity::OnReady()
{

}

void sm::GAS_Entity::OnProcess()
{

}