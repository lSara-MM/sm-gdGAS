extends CharacterBody2D
class_name Enemy

@onready var entity = $"GAS_Entity"
@onready var player : CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var on_collision = $Behaviour/OnCollision

func _ready() -> void:
	$Behaviour/Movement.Init(self)
	on_collision.Init(self)
	
func _on_enemy_area_2d_body_entered(body: Node2D) -> void:
	on_collision.OnCollision(body)
