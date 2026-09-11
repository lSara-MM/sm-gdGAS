extends Area2D

@export var speed = 10
@export var life_time = 3.0

var dir : Vector2
signal bullet_collision
signal bullet_die

func _ready():
	get_tree().create_timer(life_time).timeout.connect(_on_timer_timeout)

func _process(delta: float) -> void:
	position -= dir * speed * delta

func _on_bullet_entered(body: Node2D) -> void:
	pass

func _on_timer_timeout():
	bullet_die.emit()
	queue_free()
