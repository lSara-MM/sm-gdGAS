extends GameplayAbility

var rng
var target : GAS_Entity
var faction 

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	var world = get_entity_owner().get_world()
	rng = world.rng
	
	var entities = world.get_entities()
	target = entities[rng.randi_range(0, entities.size() - 1)]
	var tag_node = target.get_tag_container()
	if tag_node.has_tag(get_entity_owner().faction):
		return []
	
	return [target]
