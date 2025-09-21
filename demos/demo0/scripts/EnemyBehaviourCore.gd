extends Node2D
class_name EnemyBehaviourCore

var m_Owner: CharacterBody2D
var m_Player : CharacterBody2D
var m_Stats : RStats

func Init(own: Node2D) -> void:
	m_Owner = own
	m_Player = own.m_Player
	m_Stats = own.m_Stats
	
	InitSpecs(own)

# Override Init Specs if necessary
func InitSpecs(_own: Node2D) -> void:
	pass
