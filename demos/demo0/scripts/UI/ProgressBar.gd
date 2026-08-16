extends ProgressBar

@export var current_attr : StringName
@export var max_attr : StringName
var attr_container : AttributeContainer

func setup(entity: Node, _world = null) -> void:
	attr_container = entity.get_node("AttributeContainer")
	attr_container.attribute_changed.connect(_on_attribute_changed)
	value = attr_container.get_attribute_current_value(current_attr)
	max_value = attr_container.get_attribute_current_value(max_attr)

func _on_attribute_changed(attr_name: StringName, new_val: float, _old_val: float) -> void:
	match attr_name:
			current_attr:
				value = new_val
			max_attr:
				max_value = new_val
