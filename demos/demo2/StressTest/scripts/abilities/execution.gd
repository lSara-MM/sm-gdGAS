extends GameplayAbility

var rng
var target : GAS_Entity

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	var world = get_entity_owner().get_world()
	rng = world.rng
	
	var entities = world.get_entities()
	target = entities[rng.randi_range(0, entities.size() - 1)]
	var node = target.get_attribute_container()
	var attr = node.get_attribute_current_value(&"CurrentHp")
	var attr_max = node.get_attribute_current_value(&"MaxHp")
	
	if attr > attr_max * 0.2:
		target = null
	return [target]
