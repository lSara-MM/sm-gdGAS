extends Node2D
class_name Spawn

@export var objects : Array[PackedScene]

@export var spawn_radius : float = 600.0
@export var min_distance : float = 300.0
@export var spawn_interval : float = 2.0

@onready var player : CharacterBody2D = $"../Player"
@onready var timer : Timer = $Timer

func _ready():
	timer.wait_time = spawn_interval
	timer.timeout.connect(SpawnObject)

func SpawnObject():
	if objects.is_empty():
		return
	
	var object_to_spawn = objects.pick_random()
	var object = object_to_spawn.instantiate()
	add_child(object)

	var angle = randf() * TAU
	var distance = randf_range(min_distance, spawn_radius)
	var pos = player.global_position + Vector2(cos(angle), sin(angle)) * distance

	object.global_position = pos
