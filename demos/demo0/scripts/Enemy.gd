extends CharacterBody2D
class_name Enemy

@export var m_Stats : Resource
@export var m_CurrentHealth : float

@onready var m_Player : CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var m_OnCollision = $Behaviour/OnCollision

func _ready() -> void:
	$Behaviour/Movement.Init(self)
	m_OnCollision.Init(self)
	m_CurrentHealth = m_Stats.m_MaxHealth

func _on_enemy_area_2d_body_entered(body: Node2D) -> void:
	
	print(body)
	m_OnCollision.OnCollision(body)
