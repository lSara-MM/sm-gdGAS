extends Node2D
class_name EnemyBehaviourCore

var node_owner: CharacterBody2D
var player : CharacterBody2D

func Init(own: Node2D) -> void:
	node_owner = own
	player = own.player
	
	InitSpecs(own)

# Override Init Specs if necessary
func InitSpecs(_own: Node2D) -> void:
	pass
