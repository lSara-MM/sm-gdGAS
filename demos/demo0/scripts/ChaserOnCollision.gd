extends EnemyBehaviourCore

func OnCollision(body: Node2D) -> void:
	if	body == m_Player:
		print(m_Owner)
		m_Player.m_Stats.TakeDmg(m_Owner.m_Stats.m_ExpDropped)
		m_Owner.queue_free()
