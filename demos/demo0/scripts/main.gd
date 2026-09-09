extends Node

@onready var player: Node2D = $Player
@onready var world: GAS_World = $GAS_World
@onready var current_health_bar: ProgressBar = $CanvasLayer/Health/CurrentBar
@onready var max_health_bar: ProgressBar = $CanvasLayer/Health/MaxBar
@onready var stamina_bar: ProgressBar = $CanvasLayer/Stamina/CurrentBar
@onready var max_stamina_bar: ProgressBar = $CanvasLayer/Stamina/MaxBar
@onready var dash_bar: ProgressBar = $CanvasLayer/DashBar

func _ready() -> void:
	var player_entity = player.get_node("GAS_Entity")
	current_health_bar.setup(player_entity)
	max_health_bar.setup(player_entity)
	stamina_bar.setup(player_entity, world)
	max_stamina_bar.setup(player_entity, world)
	dash_bar.setup(player_entity, world)
