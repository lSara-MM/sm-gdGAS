extends GameplayAbility

## Default: On activate ability, effects are applied to self or if overridden, to the return value of _calculate_targets(), automatically and then the ability ends instantly. To override, uncomment and call commit_ability(), apply_effects_to_target() and try_end(bool cancelled) manually.
## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_active() method.
func _activate_ability() -> bool:
	if commit_ability():
		var object = get_entity_owner().Shoot()
		object.connect("bullet_collision", _on_bullet_collision)
		return true
	return false

## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_end(bool cancelled) method.
#func _end_ability(_was_cancelled: bool):
	#pass

## ## Called in CanActivate(). Adds custom conditionals to ability activation. Default: Abilities check state, cost, cooldown and tags.
#func _check_availability() -> bool:
	#return true

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
#func _calculate_targets() -> Array[GAS_Entity]:
	#return [target.get_entity()]

## Called in AppllyEffectsToTarget(). Intercept effects to get their instance id if needed.
#func _get_effect_id(effect: EffectData, instance_id: int) -> void:
	#pass
	
func _on_bullet_collision(body: Node2D) -> void:
	apply_effects_to_target(body.get_entity())
	try_end(Tags._Ability_Shoot)
