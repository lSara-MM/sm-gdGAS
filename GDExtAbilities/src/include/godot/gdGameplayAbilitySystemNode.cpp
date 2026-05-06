#include "gdGameplayAbilitySystemNode.h"

#include "godot/gdGASWorld.h"
#include <godot_cpp/classes/window.hpp>

void sm::GameplayAbilitySystem::_notification(int notification)
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
	case NOTIFICATION_PARENTED:
		OnParented();
		break;
	case NOTIFICATION_UNPARENTED:
		OnUnparented();
		break;
	case NOTIFICATION_CHILD_ORDER_CHANGED:
		OnChildOrderChanged();
		break;
	}
}

godot::Node* sm::GameplayAbilitySystem::GetActiveSceneRootOrWorld(godot::Node* owner)
{
	if (!owner || !owner->is_inside_tree())
	{
		return nullptr;
	}

	godot::SceneTree* tree = owner->get_tree();
	if (!tree)
	{
		return nullptr;
	}

	godot::Node* globalRoot = tree->get_root();
	godot::Node* current = owner;

	while (current->get_parent() && current->get_parent() != globalRoot)
	{
		current = current->get_parent();

		if (auto* world = godot::Object::cast_to<GAS_World>(current); world != owner)
		{
			return current;
		}
	}

	return NodeUtils::GetChildNodeOfType<GAS_World>(current);
}