extends CharacterBody2D

@export var m_Speed = 200
@onready var m_gd_AnimationTree = $AnimationTree
	
func _physics_process(_delta: float) -> void:
	GetInput()
	move_and_slide()

func GetInput() -> void:
	var inputDirection = Input.get_vector("left", "right", "up", "down")
	velocity = inputDirection * m_Speed
	
	if	m_gd_AnimationTree:
		m_gd_AnimationTree.SetAnim(inputDirection)
