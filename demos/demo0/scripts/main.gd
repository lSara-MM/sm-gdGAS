extends Node

@onready var player: Node2D = $Player
@onready var health_bar: TextureProgressBar = $CanvasLayer/HealthBar
@onready var stamina_bar: TextureProgressBar = $CanvasLayer/StaminaBar

func _ready() -> void:
	var player_entity = player.get_node("GAS_Entity")
	health_bar.setup(player_entity)
	stamina_bar.setup(player_entity)
