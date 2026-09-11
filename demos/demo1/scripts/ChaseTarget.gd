extends EnemyBehaviourCore
class_name ChaseTarget

@onready var animation_tree : AnimationTree = get_node("../../AnimationTree")
@onready var character = get_node("../..")

var target : CharacterBody2D

func InitSpecs(own: Node2D) -> void:
	target = own.player

func _physics_process(_delta):
	character.velocity = GetVelocity(character)
	
	if animation_tree:
		animation_tree.SetAnim(character.velocity.normalized())
		
	character.move_and_slide()

func GetVelocity(this: CharacterBody2D) -> Vector2:
	if is_instance_valid(target):
		var speed = 12
		return (target.global_position - this.global_position).normalized() * speed
	
	return Vector2.ZERO
