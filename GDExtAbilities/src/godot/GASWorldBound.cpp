#include "godot/GASWorldBound.h"

#include "godot/gdGASWorld.h"

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

sm::GAS_World* sm::WorldBound::GetOrInitWorld(GameplayAbilitySystem* owner, godot::Node* sceneRootNode)
{
	if (m_World)
	{
		return m_World;
	}

	if (!owner || !owner->is_inside_tree())
	{
		return nullptr;
	}

	auto* world = NodeUtils::GetParentNodeOfType<GAS_World>(owner);
	if (world)
	{
		m_World = world;
		return m_World;
	}

	godot::Node* scene = (sceneRootNode ? sceneRootNode : NodeUtils::GetSceneRoot(owner));
	m_World = NodeUtils::GetChildNodeOfType<GAS_World>(scene);

	return m_World;
}

sm::GAS_World* sm::WorldBound::GetWorld(GameplayAbilitySystem* owner)
{
	return m_World;
}

void sm::WorldBound::SetWorld(GAS_World* world)
{
	m_World = world;
}

void sm::WorldBound::CleanUp()
{
	m_World = nullptr;
}

void sm::WorldBound::PrintWorld()
{
	if (m_World)
	{
		godot::UtilityFunctions::print(m_World->get_name());
	}
	else
	{
		godot::UtilityFunctions::print("This node does not belong to any world.");
	}
}