extends CharacterBody2D
class_name Enemy

@onready var entity = $"GAS_Entity"
@onready var player : CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var on_collision = $Behaviour/OnCollision
@onready var particles = $Fire

signal on_death

func _ready() -> void:
	var choice = randi() % 4 == 0
	if choice:
		var mod = ModifierData.new()
		mod.operation_type = ModifierData.Multiply
		mod.value = 2
		get_entity().get_attribute_container().add_base_modifier("Speed", mod)
	particles.emitting = choice
	
	$Behaviour/Movement.Init(self)
	on_collision.Init(self)
	
func _on_enemy_area_2d_body_entered(body: Node2D) -> void:
	on_collision.OnCollision(body)

func get_entity() -> GAS_Entity:
	return find_child("GAS_Entity") as GAS_Entity

func _on_attribute_changed(attribute_name: StringName, new_value: float, _old_value: float) -> void:
	match attribute_name:
		"CurrentHealth":
			if new_value == 0:
				on_death.emit()
				queue_free()
