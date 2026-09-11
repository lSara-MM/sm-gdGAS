extends GameplayAbility

var target : GAS_Entity

## Default: On activate ability, effects are applied to self or if overridden, to the return value of _calculate_targets(), automatically and then the ability ends instantly. To override, uncomment and call commit_ability(), apply_effects_to_target() and try_end(bool cancelled) manually.
## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_active() method.
func _activate_ability() -> bool:
	if commit_ability():
		var bullet = get_entity_owner().Shoot()
		bullet.bullet_collision.connect(_on_bullet_collision)
		bullet.bullet_die.connect(_on_bullet_die)
		return true
	return false

## Warning: This method shouldn't be called manually as it gets called automatically by the ability_container::try_end(bool cancelled) method.
#func _end_ability(_was_cancelled: bool):
	#pass

## ## Called in CanActivate(). Adds custom conditionals to ability activation. Default: Abilities check state, cost, cooldown and tags.
#func _check_availability() -> bool:
	#return true

## Called in TryActivate(). Must return the entities affected by the ability's effects. Default: Abilities apply effects to owner entity.
func _calculate_targets() -> Array[GAS_Entity]:
	return [target]

## Called in AppllyEffectsToTarget(). Intercept effects to get their instance id if needed.
#func _get_effect_id(effect: EffectData, instance_id: int) -> void:
	#pass
	
func _on_bullet_collision(body: Node2D) -> void:
	target = body.get_entity()
	apply_effects_to_target(target)
	try_end(Tags._Ability_Shoot)
	
func _on_bullet_die() -> void:
	try_end(Tags._Ability_Shoot)
