extends Area2D

@export var life_time : float = 1
@export var item_tag : int

func _ready() -> void:
	get_tree().create_timer(life_time).timeout.connect(queue_free)
