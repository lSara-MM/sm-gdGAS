extends GameplayAbility

var rng
var targets : Array[GAS_Entity]

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	var world = get_entity_owner().get_world()
	rng = world.rng
	var arr: Array[Node] = world.all_with_tags([Tags._Race_Plant])
	targets = []
	
	for node in arr:
		if node is GAS_Entity:
			targets.append(node as GAS_Entity)

	return targets
