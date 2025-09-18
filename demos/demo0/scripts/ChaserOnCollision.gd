extends EnemyBehaviourCore

func OnCollision(body: Node2D) -> void:
	if	body == m_Player:
		m_Parent.call_deferred("free")
