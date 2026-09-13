extends GameplayAbility

var rng = RandomNumberGenerator.new()
var target : GAS_Entity

## Default: On activate ability, effects are applied to self or if overridden, to the return value of _calculate_targets(), automatically and then the ability ends instantly. To override, uncomment and call commit_ability(), apply_effects_to_target() and try_end(bool cancelled) manually.
## Return true if it was successfully activated.
## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_active() method.
#func _activate_ability() -> bool:
	#return true

## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_end(bool cancelled) method.
func _end_ability(_was_cancelled: bool) -> bool:
	print("End Execution")
	if !is_instance_valid(target):
		return true
		
	var node = target.get_attribute_container()
	print("Hp changed (expected less): ", node.get_attribute_current_value(&"CurrentHp"))
	return true

## ## Called in CanActivate(). Adds custom conditionals to ability activation. Default: Abilities check state, cost, cooldown and tags.
#func _check_availability() -> bool:
	#return true

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	rng.seed = StressTestConfig.RNG_SEED
	var entities = get_entity_owner().get_world().get_entities()
	target = entities[rng.randi_range(0, entities.size() - 1)]
	var node = target.get_attribute_container()
	var attr = node.get_attribute_current_value(&"CurrentHp")
	var attr_max = node.get_attribute_current_value(&"MaxHp")
	attr = 10
	
	if attr > attr_max * 0.2:
		target = null
	print("Execution")
	
	return [target]

## Called in AppllyEffectsToTarget(). Intercept effects to get their instance id if needed.
#func _get_effect_id(effect: EffectData, instance_id: int) -> void:
	#pass
