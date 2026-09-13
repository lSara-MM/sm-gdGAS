extends GAS_Entity

@onready var ability_container = $AbilityContainer

func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.pressed and not event.echo:
			match event.keycode:
				KEY_0:
					ability_container.try_activate(Tags._Ability_BasicAttack)
				KEY_1:
					ability_container.try_activate(Tags._Ability_Execution)
				KEY_2:
					ability_container.try_activate(Tags._Ability_Freeze)
				KEY_3:
					ability_container.try_activate(Tags._Ability_FrostNova)
				KEY_4:
					ability_container.try_activate(Tags._Ability_Haste)
				KEY_5:
					ability_container.try_activate(Tags._Ability_Invulnerability)
				KEY_6:
					ability_container.try_activate(Tags._Ability_MassHeal)
				KEY_7:
					ability_container.try_activate(Tags._Ability_MassPurge)
				KEY_8:
					ability_container.try_activate(Tags._Ability_PoisonDart)
				KEY_9:
					ability_container.try_activate(Tags._Ability_PoisonExplosion)
				KEY_Q:
					ability_container.try_activate(Tags._Ability_PoisonExplosion)
				KEY_W:
					ability_container.try_activate(Tags._Ability_Regeneration)
				KEY_E:
					ability_container.try_activate(Tags._Ability_Slow)
				KEY_E:
					ability_container.try_activate(Tags._Ability_Taunt)
					
