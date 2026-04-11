#pragma once
#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class GAS_World;

	class GameplayAbilitySystem : public godot::Node
	{
		GDCLASS(GameplayAbilitySystem, godot::Node)

	protected:
		static void _bind_methods() {};

	public:
		GAS_World* GetWorld() { return m_World; };
		void SetWorld(GAS_World* world) { m_World = world; };

		virtual void _notification(int notification)
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
		};

		virtual void OnEnterTree() {};
		virtual void OnExitTree()
		{
			//print_orphan_nodes();
		};
		virtual void OnReady() {};
		virtual void OnProcess() {};

	private:
		GAS_World* m_World = nullptr;
	};
}