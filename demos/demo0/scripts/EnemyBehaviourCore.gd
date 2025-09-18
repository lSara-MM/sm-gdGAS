extends Node2D

class_name EnemyBehaviourCore

var m_Parent: CharacterBody2D
var m_Player : CharacterBody2D
var m_Stats : Resource

func Init(parent: Node2D) -> void:
	m_Parent = parent
	m_Player = parent.m_Player
	m_Stats = parent.m_Stats
	
	InitSpecs(parent)

# Override Init Specs if necessary
func InitSpecs(parent: Node2D) -> void:
	pass
