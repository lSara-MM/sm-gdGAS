extends Area2D

@export var life_time : float = 1
@export var item_tag : int
@export var value : int
@export var operation : ModifierData.OperationType

func _ready() -> void:
	get_tree().create_timer(life_time).timeout.connect(queue_free)

func _on_body_entered(body: Node2D) -> void:
	if not body.has_method("get_entity"):
		return
		
	var entity = body.get_entity()
	if not is_instance_valid(entity):
		return
	
	var tags = entity.get_tag_container()
	if is_instance_valid(tags) and tags.has_tag(Tags._Player):
		if entity.has_method("collect_item"):
			entity.collect_item(item_tag, value, operation)
		queue_free()
