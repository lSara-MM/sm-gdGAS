extends CharacterBody2D
class_name Enemy

# Movement
@export var movement: Resource
@export var speed: float = 100
var prevDir = Vector2.DOWN

# Life
@export var maxHealth: int = 100
@export var currentHealth = maxHealth

# Targeting
@onready var player: Node2D = get_tree().get_first_node_in_group("player")

func _ready() -> void:
	if movement:
		movement.InitResource(self)
	else:
		print("Movement was nil")

func _physics_process(_delta):
	if movement:
		velocity = movement.GetVelocity(self, speed)
		SetAnim(velocity.normalized())
		move_and_slide()
		
func SetAnim(dir: Vector2) -> void:
	if dir != prevDir:
		if dir == Vector2.ZERO:
			print("idle")
			$AnimationTree.set("parameters/Idle/blend_position", prevDir)
			$AnimationTree.get("parameters/playback").travel("Idle")
		else:
			print("walk")
			$AnimationTree.set("parameters/Walk/blend_position", dir)
			$AnimationTree.get("parameters/playback").travel("Walk")
		
		prevDir = dir
