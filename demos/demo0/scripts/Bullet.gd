extends Area2D

@export var speed = 700
var dir : Vector2

func _process(delta: float) -> void:
	position.y -= dir * speed * delta
	pass

func _on_bullet_entered(body: Node2D) -> void:
	if body.is_in_group("Enemy"):
		emit_signal("bullet_collision", body)
	pass
