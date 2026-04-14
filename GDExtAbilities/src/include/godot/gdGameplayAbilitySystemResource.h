#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class GameplayAbilitySystemResource : public godot::Resource
	{
		GDCLASS(GameplayAbilitySystemResource, godot::Resource)

	protected:
		static void _bind_methods() {};

	public:
		virtual void _notification(int notification)
		{
			switch (notification)
			{
			case NOTIFICATION_POSTINITIALIZE:
				OnPostInit();
				break;
			case NOTIFICATION_PREDELETE:
				OnPreDelete();
				break;
			case NOTIFICATION_EXTENSION_RELOADED:
				OnExtensionReloaded();
				break;
			}
		};

		virtual void OnPostInit()
		{
#ifdef DEBUG_ENABLED
			//ERR_PRINT(godot::vformat("Resource loaded: '%s'", get_class()));
#endif // DEBUG_ENABLED
		};

		virtual void OnPreDelete()
		{
#ifdef DEBUG_ENABLED
			//ERR_PRINT(godot::vformat("Resource deleted: '%s'", get_class()));
#endif // DEBUG_ENABLED
		};

		virtual void OnExtensionReloaded() {};
	};
}