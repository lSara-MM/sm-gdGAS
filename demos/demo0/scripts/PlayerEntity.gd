extends GAS_Entity
class_name PlayerEntity

@onready var attribute_container = get_attribute_container()
@onready var tag_container = get_tag_container()
@onready var ability_container = $AbilityContainer

var is_berserk = false
@export var bullet_prefab : PackedScene

func TryAbility(ability: int) -> bool:
	return ability_container.try_activate(ability)
	
func IsAbilityActive(ability: int) -> bool:
	return tag_container.has_tag(ability)
	
func TryBerserk() -> bool:
	if is_berserk:
		is_berserk = ability_container.try_end(Tags._Ability_Berserk, false)
	else:
		is_berserk = ability_container.try_activate(Tags._Ability_Berserk)
	return is_berserk

func GetAttributeCurrentValue(attr: StringName) -> float:
	return attribute_container.get_attribute_current_value(attr)

func _on_ability_activated(_entity: Object, ability: int) -> void:
	if ability == Tags._Ability_Dash:
		ability_container.try_activate(Tags._Ability_StaminaRegen)
	pass
	
func Shoot() -> Node:
	if bullet_prefab.is_empty():
		return
	
	var player = get_parent()
	var object = bullet_prefab.instantiate()
	player.add_child(object)
	object.global_position = player.global_position
	
	object.dir = player.position - player.get_global_mouse_position()
	return object
