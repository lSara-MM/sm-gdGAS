extends Node2D
class_name EnemyBehaviourCore

var node_owner: CharacterBody2D
var entity : GAS_Entity
var player : CharacterBody2D
var player_entity : GAS_Entity

func Init(own: Node2D) -> void:
	node_owner = own
	entity = own.entity
	player = own.player
	player_entity = player.find_child("GAS_Entity")
	
	InitSpecs(own)

# Override Init Specs if necessary
func InitSpecs(_own: Node2D) -> void:
	pass
