extends Node

@onready var player: Node2D = $Player
@onready var world: GAS_World = $GAS_World
@onready var health_bar: ProgressBar = $CanvasLayer/HealthBar
@onready var stamina_bar: ProgressBar = $CanvasLayer/StaminaBar
@onready var dash_bar: ProgressBar = $CanvasLayer/DashBar

func _ready() -> void:
	var player_entity = player.get_node("GAS_Entity")
	health_bar.setup(player_entity)
	stamina_bar.setup(player_entity)
	dash_bar.setup(player_entity, world)
