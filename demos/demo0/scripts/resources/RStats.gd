extends Resource
class_name RStats

signal onDie

@export var m_MaxHealth : float = 100.0
var m_CurrentHealth : float = m_MaxHealth

@export var m_MovementSpeed : float = 100.0
@export var m_EffectSpeed : float = 2
@export var m_CurrentLvl : int = 0
@export var m_MinLvl : int = 0
@export var m_MaxLvl : int = 0

func TakeDmg(dmg: float) -> void:
	m_CurrentHealth -= dmg
	
	if m_CurrentHealth <= 0:
		emit_signal("onDie")
		
	print("HP: ", m_CurrentHealth)
