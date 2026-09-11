extends CharacterBody2D
class_name Enemy

@onready var player : CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var on_collision = $Behaviour/OnCollision

signal on_death

func _ready() -> void:
	$Behaviour/Movement.Init(self)
	on_collision.Init(self)
