extends EnemyBehaviourCore
class_name ChaseTarget

@onready var m_gd_AnimationTree : AnimationTree = get_node("../../AnimationTree")

var m_Target : CharacterBody2D

func InitSpecs(parent: Node2D) -> void:
	m_Target = parent.m_Player

func _physics_process(_delta):
	m_Parent.velocity = GetVelocity(m_Parent, m_Stats.m_MoveSpeed)
	
	if	m_gd_AnimationTree:
		m_gd_AnimationTree.SetAnim(m_Parent.velocity.normalized())
		
	m_Parent.move_and_slide()

func GetVelocity(this: CharacterBody2D, speed: float = 100) -> Vector2:
	if	m_Target:
		return (m_Target.global_position - this.global_position).normalized() * speed
	
	return Vector2.ZERO
