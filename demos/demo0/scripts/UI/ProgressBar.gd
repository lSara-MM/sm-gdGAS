extends Node
class_name UI_Bar

@export var current_attr: StringName
@export var max_attr: StringName
@export var size_mult: float = 2.5

@onready var current_bar: ProgressBar = $CurrentBar
@onready var max_bar: ProgressBar = $MaxBar
@onready var label: Label = $CurrentBar/Label

var player_entity: GAS_Entity
var attr_container: AttributeContainer

signal attr_maxed(attr_name: StringName)

func setup(entity: Node, _world: GAS_World = null) -> void:
	player_entity = entity
	attr_container = entity.get_node("AttributeContainer")

	attr_container.attribute_changed.connect(_on_attribute_changed)

	current_bar.value = attr_container.get_attribute_current_value(current_attr)
	current_bar.max_value = attr_container.get_attribute_current_value(max_attr)
	max_bar.value = current_bar.max_value
	max_bar.max_value = current_bar.max_value

	UpdatePosition()
	UpdateText()

func _on_attribute_changed(attr_name: StringName, new_val: float, old_val: float) -> void:
	match attr_name:
		current_attr:
			update_current(new_val)

		max_attr:
			update_max(new_val, old_val)

func update_current(new_val: float) -> void:
	current_bar.value = new_val
	UpdateText()

	if is_equal_approx(current_bar.value, current_bar.max_value):
		attr_maxed.emit(current_attr)

func update_max(new_val: float, _old_val: float) -> void:
	current_bar.max_value = new_val
	if current_bar.value > new_val:
		current_bar.value = new_val
	
	if new_val > max_bar.max_value:
		max_bar.max_value = new_val
		UpdatePosition()

	max_bar.value = new_val
	UpdateText()

func UpdatePosition()-> void:
	var width := max_bar.max_value * size_mult

	max_bar.size.x = width
	current_bar.size.x = width

func UpdateText() -> void:
	label.text = str(current_bar.value) + "/" + str(max_bar.value)
	label.set_position(Vector2(max_bar.size.x + 10, 0))
