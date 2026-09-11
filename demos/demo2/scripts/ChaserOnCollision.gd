extends EnemyBehaviourCore

var ability_container : AbilityContainer

func InitSpecs(_own: Node2D) -> void:
	ability_container = entity.find_child("AbilityContainer")
	
func OnCollision(body: Node2D) -> void:
	if body == player:
		entity.targets.clear()
		entity.targets = [player_entity] as Array[GAS_Entity]
		ability_container.try_activate(Tags._Ability_EnemyBasicDamage)
		node_owner.queue_free()
