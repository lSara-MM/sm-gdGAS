extends Resource
class_name RAbility

@export var m_Cooldown : float
@export var m_LastTimeUsed : float
@export var m_Duration : float
@export var m_Instances : int
@export var m_Radius : float

func Activate(_owner) -> void:
	pass

func Desactivate(_owner) -> void:
	pass
	
func CanActivate(now: float) -> bool:
	return now - m_LastTimeUsed >= m_Cooldown
