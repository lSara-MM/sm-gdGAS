extends ProgressBar
class_name UI_Bar

@export var current_attr : StringName
@export var max_attr : StringName
@export var size_mult : float = 2.5

var player_entity : GAS_Entity
signal attr_maxed(attr_name: StringName)

var attr_container : AttributeContainer

func setup(entity: Node, _world : GAS_World = null) -> void:
	player_entity = entity
	attr_maxed.connect(player_entity._on_attribute_maxed)
	attr_container = entity.get_node("AttributeContainer")
	attr_container.attribute_changed.connect(_on_attribute_changed)
	value = attr_container.get_attribute_current_value(current_attr)
	max_value = attr_container.get_attribute_current_value(max_attr)
	size.x = max_value * size_mult

func _on_attribute_changed(attr_name: StringName, new_val: float, _old_val: float) -> void:
	match attr_name:
			current_attr:
				value = new_val
				if value == max_value:
					attr_maxed.emit(attr_name)
			max_attr:
				if value > new_val:
					max_value = new_val
					value = new_val
					size.x = max_value * size_mult
