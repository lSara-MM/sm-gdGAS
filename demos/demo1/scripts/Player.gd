extends CharacterBody2D
class_name Player

@onready var animation_tree = $AnimationTree

func _physics_process(_delta: float) -> void:
	GetInput()
	move_and_slide()
	
func GetInput() -> void:
	if Input.is_action_just_pressed("lshift"):
		pass
		
	if Input.is_action_just_pressed("ability1"):
		pass
		
	if Input.is_action_just_pressed("basic_attack"):
		pass
		
	var input_direction = Input.get_vector("left", "right", "up", "down")
	#velocity = input_direction * entity.GetAttributeCurrentValue(&"Speed")
	
	if animation_tree:
		animation_tree.SetAnim(input_direction)
