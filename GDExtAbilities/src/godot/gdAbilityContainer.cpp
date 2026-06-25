#include "godot/gdAbilityContainer.h"

#include "godot/gdGASWorld.h"

void sm::AbilityContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_entity_node_path"), &GetEntityNodePath);
	godot::ClassDB::bind_method(godot::D_METHOD("set_entity_node_path", "path"), &SetEntityNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("get_abilities"), &GetAbilities);
	godot::ClassDB::bind_method(godot::D_METHOD("set_abilities", "abilities"), &SetAbilities);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"entity_node_path",
		godot::PROPERTY_HINT_NODE_PATH_VALID_TYPES,
		"GAS_Entity",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"set_entity_node_path", "get_entity_node_path"
	);

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
	GAS_Entity* entity = NodeUtils::GetParentNodeOfType<GAS_Entity>(this);

	if (!entity)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create AbilityContainer. Node must be in a GAS_Entity node hierarchy.");
	}

	SetEntityNodePath(entity->get_path());
}

void sm::AbilityContainer::OnExitTree()
{
	m_WorldBound.CleanUp();
}

godot::NodePath sm::AbilityContainer::GetEntityNodePath() const
{
	return m_EntityNodePath;
}

void sm::AbilityContainer::SetEntityNodePath(godot::NodePath path)
{
	ERR_FAIL_COND_MSG(path.is_empty(), "Could not set node path. Node must be in a GAS_Entity node hierarchy.");

	m_EntityNodePath = path;
}