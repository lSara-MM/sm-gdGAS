extends Node2D
class_name Spawn

@export var objects : Array[PackedScene]
@export var objects_probs : PackedFloat32Array

@export var parent_name : String
@export var parent : Node = self
@export var object_spawn_in_rad : Node

@export var spawn_radius : float = 600.0
@export var min_distance : float = 300.0

@export var timer : Timer
@export var spawn_interval : float = 0

var spawn : bool = true
var rng

func _ready():
	if timer:
		timer.wait_time = spawn_interval
		timer.timeout.connect(SpawnObject)
	
	if !objects_probs.is_empty():
		rng = RandomNumberGenerator.new()

func SpawnObject():
	if !spawn || objects.is_empty():
		return
	
	var object_to_spawn
	if objects_probs.is_empty():
		object_to_spawn = objects.pick_random()
	else:
		var index = rng.rand_weighted(objects_probs)
		object_to_spawn = objects[index]
	
	if is_instance_valid(object_to_spawn):
		var object = object_to_spawn.instantiate()
		var add_later = func():
			parent.add_child(object)
		add_later.call_deferred()

		object.global_position = ObjectPosition()

func ObjectPosition() -> Vector2:
	var angle = randf() * TAU
	var distance = randf_range(min_distance, spawn_radius)
	return object_spawn_in_rad.global_position + Vector2(cos(angle), sin(angle)) * distance

func _on_player_died() -> void:
	spawn = false
	timer.stop()

func _on_enemy_on_death() -> void:
	SpawnObject()
