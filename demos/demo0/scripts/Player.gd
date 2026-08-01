extends CharacterBody2D
class_name Player

@onready var entity = $GAS_Entity
@onready var animation_tree = $AnimationTree

func _ready() -> void:
	pass

func _physics_process(_delta: float) -> void:
	GetInput()
	move_and_slide()
	
func GetInput() -> void:
	if Input.is_action_just_pressed("lshift"):
		print(entity.TryDash())
	
	var input_direction = Input.get_vector("left", "right", "up", "down")
	velocity = input_direction * entity.GetAttributeCurrentValue(&"Speed")
	print(velocity)
	
	if animation_tree:
		animation_tree.SetAnim(input_direction)
