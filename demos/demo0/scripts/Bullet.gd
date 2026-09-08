extends Area2D

@export var speed = 10
@export var life_time = 5.0

var dir : Vector2
signal bullet_collision

func _ready():
	get_tree().create_timer(life_time).timeout.connect(queue_free)

func _process(delta: float) -> void:
	position -= dir * speed * delta
	print(position)

func _on_bullet_entered(body: Node2D) -> void:
	if not body.has_method("get_entity"):
		return
	
	var entity = body.get_entity()
	if not is_instance_valid(entity):
		return
	
	var tags = entity.get_tag_container()
	if is_instance_valid(tags) && tags.has_tag(Tags._Enemy):
		bullet_collision.emit(body)
