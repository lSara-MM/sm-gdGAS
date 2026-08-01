extends ProgressBar

@export var current_attr : StringName
@export var max_attr : StringName

func setup(entity: Node) -> void:
	var attr_container = entity.get_node("AttributeContainer")
	attr_container._on_attribute_modified.connect(_on_attribute_changed)

func _on_attribute_changed(entity_owner: Object, attr_name: String, new_val: float, old_val: float) -> void:
	if attr_name == current_attr:
		value = new_val
		
	if attr_name == max_attr:
		max_value = new_val
