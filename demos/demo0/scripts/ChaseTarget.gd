extends EnemyBehaviourCore
class_name ChaseTarget

@onready var m_gd_AnimationTree : AnimationTree = get_node("../../AnimationTree")

var m_Target : CharacterBody2D

func InitSpecs(own: Node2D) -> void:
	m_Target = own.m_Player

func _physics_process(_delta):
	m_Owner.velocity = GetVelocity(m_Owner, m_Stats.m_MovementSpeed)
	
	if	m_gd_AnimationTree:
		m_gd_AnimationTree.SetAnim(m_Owner.velocity.normalized())
		
	m_Owner.move_and_slide()

func GetVelocity(this: CharacterBody2D, speed: float = 100) -> Vector2:
	if	m_Target:
		return (m_Target.global_position - this.global_position).normalized() * speed
	
	return Vector2.ZERO
