extends EnemyBehaviourCore

@onready var ability_container = $"../../GAS_Entity/AbilityContainer"

func OnCollision(body: Node2D) -> void:
	if	body == player:
		ability_container.try_activate(Tags._Ability_Damage)
		#node_owner.queue_free()
