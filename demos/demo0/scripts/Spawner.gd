extends Node2D
class_name Spawn

@export var m_Objects : Array[PackedScene]

@export var m_SpawnRadius : float = 600.0
@export var m_MinDistance : float = 300.0
@export var m_SpawnInterval : float = 2.0

@onready var m_Player : CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var m_Timer : Timer = $Timer

func _ready():
	m_Timer.wait_time = m_SpawnInterval
	m_Timer.timeout.connect(SpawnObject)

func SpawnObject():
	if m_Objects.is_empty():
		return
	
	var objectToSpawn = m_Objects.pick_random()
	var object = objectToSpawn.instantiate()
	add_child(object)

	var angle = randf() * TAU
	var distance = randf_range(m_MinDistance, m_SpawnRadius)
	var pos = m_Player.global_position + Vector2(cos(angle), sin(angle)) * distance

	object.global_position = pos
