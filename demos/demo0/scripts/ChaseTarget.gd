extends EnemyBehaviourCore
class_name ChaseTarget

@onready var animation_tree : AnimationTree = get_node("../../AnimationTree")
@onready var attribute_container : AttributeContainer = get_node("../../GAS_Entity/AttributeContainer")
@onready var character = get_node("../..")

var target : CharacterBody2D

func InitSpecs(own: Node2D) -> void:
	target = own.player

func _physics_process(_delta):
	character.velocity = GetVelocity(character)
	
	if	animation_tree:
		animation_tree.SetAnim(character.velocity.normalized())
		
	character.move_and_slide()

func GetVelocity(this: CharacterBody2D) -> Vector2:
	if	target:
		var speed = attribute_container.get_attribute_current_value(&"Speed")
		return (target.global_position - this.global_position).normalized() * speed
	
	return Vector2.ZERO
