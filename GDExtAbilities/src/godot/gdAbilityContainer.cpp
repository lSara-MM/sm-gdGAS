#include "godot/gdAbilityContainer.h"

#include "godot/gdGASWorld.h"

void sm::AbilityContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_abilities"), &GetAbilities);
	godot::ClassDB::bind_method(godot::D_METHOD("set_abilities", "abilities"), &SetAbilities);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"abilities",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:AbilityData"),
		"set_abilities", "get_abilities"
	);
}

void sm::AbilityContainer::SetAbilities(const godot::TypedArray<sm::AbilityData>& ability)
{
	m_gdAbilities = ability;
}

void sm::AbilityContainer::OnReady()
{
	sm::GAS_World* world = sm::GAS_World::Instance();

	if (!world)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create AbilityContainer. GAS_World Node required");
	}
}

void sm::AbilityContainer::OnExitTree()
{}
