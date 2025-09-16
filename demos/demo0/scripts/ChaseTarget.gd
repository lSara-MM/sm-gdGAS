extends Resource
class_name ChaseTarget

@export var m_TargetPath: NodePath
var m_Target

func InitResource(target: CharacterBody2D) -> void:
	m_Target = target.get_node_or_null(m_TargetPath)

func GetVelocity(owner: CharacterBody2D, speed: float = 100) -> Vector2:
	if	m_Target:
		return (m_Target.global_position - owner.global_position).normalized() * speed
	
	return Vector2.ZERO
