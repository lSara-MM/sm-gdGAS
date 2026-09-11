extends Node

@onready var player: Node2D = $Player
@onready var world: GAS_World = $GAS_World
@onready var current_health_bar: Node = $CanvasLayer/Health
@onready var stamina_bar: Node = $CanvasLayer/Stamina
@onready var dash_bar: ProgressBar = $CanvasLayer/DashBar

func _ready() -> void:
	var player_entity = player.get_node("GAS_Entity")
	current_health_bar.setup(player_entity)
	stamina_bar.setup(player_entity, world)
	dash_bar.setup(player_entity, world)
