extends GameplayAbility

var rng
var target : GAS_Entity
	
## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	var world = get_entity_owner().get_world()
	rng = world.rng
	
	var entities = world.get_entities()
	target = entities[rng.randi_range(0, entities.size() - 1)]
	
	if target.get_tag_container().has_tag(Tags._Immunity_Poison):
		return []
	return [target]
