extends CharacterBody2D
class_name  Player

@export var m_Stats : RStats
@export var m_EffectSpeed : float = 10
@export var m_Abilities : Array[Resource]
@onready var m_gd_AnimationTree = $AnimationTree

func _ready() -> void:
	m_Stats.onDie.connect(_on_die)

func _physics_process(_delta: float) -> void:
	GetInput()
	move_and_slide()
				
func GetInput() -> void:
	if !m_Abilities.is_empty():
		m_Abilities[0].Desactivate(self)
			
	if Input.is_action_just_pressed("lshift"):
		if !m_Abilities.is_empty():
			m_Abilities[0].Activate(self)
			print(m_EffectSpeed)

	var inputDirection = Input.get_vector("left", "right", "up", "down")
	velocity = inputDirection * m_Stats.m_MovementSpeed + Vector2(m_EffectSpeed, m_EffectSpeed)
	
	if	m_gd_AnimationTree:
		m_gd_AnimationTree.SetAnim(inputDirection)

func _on_die() -> void:
	print("You died")
	queue_free()
