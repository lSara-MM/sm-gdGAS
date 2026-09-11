extends Node
class_name UI_Bar

@export var current_attr: StringName
@export var max_attr: StringName
@export var size_mult: float = 2.5

@onready var current_bar: ProgressBar = $CurrentBar
@onready var max_bar: ProgressBar = $MaxBar
@onready var label: Label = $CurrentBar/Label

func setup(entity: Node) -> void:
	UpdatePosition()
	UpdateText()

func update_current(new_val: float) -> void:
	current_bar.value = new_val
	UpdateText()

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
