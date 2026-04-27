#include "godot/GASWorldBound.h"

#include "godot/gdGASWorld.h"

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

sm::GAS_World* sm::WorldBound::GetOrInitWorld(GameplayAbilitySystem* owner)
{
	if (m_World)
	{
		return m_World;
	}

	if (!owner)
	{
		return nullptr;
	}

	auto* world = GetParentNodeOfType<GAS_World>(owner);
	if (world)
	{
		m_World = world;
		return m_World;
	}

	godot::Node* scene = owner->get_tree()->get_root();
	m_World = GetChildNodeOfType<GAS_World>(scene);

	return m_World;
}

void sm::WorldBound::SetWorld(GAS_World* world)
{
	m_World = world;
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