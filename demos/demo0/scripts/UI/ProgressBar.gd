extends ProgressBar

@export var current_attr : StringName
@export var max_attr : StringName
@export var attr_container : AttributeContainer

func setup(entity: Node) -> void:
	attr_container = entity.get_node("AttributeContainer")
	attr_container.modifier_added.connect(_on_attribute_changed)
	value = attr_container.get_attribute_current_value(current_attr)
	max_value = attr_container.get_attribute_current_value(max_attr)

func _on_attribute_changed(_entity_owner: AttributeContainer, attr_name: StringName, _mod: ModifierData) -> void:
	match attr_name:
			current_attr:
				value = attr_container.get_attribute_current_value(attr_name)
			max_attr:
				max_value = attr_container.get_attribute_current_value(attr_name)
