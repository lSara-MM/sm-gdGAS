extends CharacterBody2D

@onready var berserk_particles = $BasicParticles/Fire
@onready var effect_particles = $BasicParticles/EnemyExplosion
@onready var hurt_particles = $BasicParticles/Hurt

signal on_death
	
func _on_enemy_area_2d_body_entered(body: Node2D) -> void:
	pass

func get_entity() -> GAS_Entity:
	return find_child("GAS_Entity") as GAS_Entity

func _on_attribute_changed(attribute_name: StringName, new_value: float, old_value: float) -> void:
	match attribute_name:
		"CurrentHealth":
			if new_value == 0:
				queue_free()
			if new_value < old_value:
				hurt_particles.emitting = true
