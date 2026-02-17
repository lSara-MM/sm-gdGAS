#include "gdGASWorld.h"

#include "EffectSystem.h"

namespace sm
{
	GAS_World* GAS_World::m_Instance = nullptr;
}

sm::GAS_World::GAS_World()
{
	if (m_Instance && m_Instance != this)
	{
		ERR_PRINT("GAS: Only one GAS_World allowed.");
		queue_free();
		return;
	}

	m_Instance = this;
}

void sm::GAS_World::_bind_methods()
{

}

void sm::GAS_World::_notification(int notification)
{
	switch (notification)
	{
	case NOTIFICATION_ENTER_TREE:
		break;
	case NOTIFICATION_EXIT_TREE:
		break;
	case NOTIFICATION_READY:
	{
		if (enableEffects)
		{
			m_EffectsSystem = std::make_unique<EffectSystem>();
		}

		/*if (enableAbility)
		{

		}*/
	}
	break;
	case NOTIFICATION_PROCESS:
	{
		if (enableEffects)
		{
			m_EffectsSystem->Update(get_process_delta_time());
		}
		break;
	}
	}
}