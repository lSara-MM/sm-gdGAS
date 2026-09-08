extends GAS_Entity
class_name PlayerEntity

@onready var attribute_container = get_attribute_container()
@onready var tag_container = get_tag_container()
@onready var ability_container = $AbilityContainer

@export var ability_tag_names : Array[StringName]
var ability_tags : Array[int]

var is_berserk = false
var is_stamina_regen = false
@export var bullet_prefab : PackedScene

func _ready() -> void:
	for tag_name in ability_tag_names:
		var tag_id = get_world().find_tag(tag_name)
		ability_tags.append(tag_id)

func _process(_delta: float) -> void:
	if is_stamina_regen:
		ability_container.try_activate(Tags._Ability_StaminaRegen)

func TryAbility(ability: int) -> bool:
	return ability_container.try_activate(ability)
	
func IsAbilityActive(ability: int) -> bool:
	return tag_container.has_tag(ability)
	
func TryBerserk() -> bool:
	if is_berserk:
		is_berserk = false
		return ability_container.try_end(Tags._Ability_Berserk, false)
	else:
		is_berserk = true
		return ability_container.try_activate(Tags._Ability_Berserk)

func Shoot() -> Node:
	if bullet_prefab == null:
		return
	
	var player = get_parent()
	var object = bullet_prefab.instantiate()
	player.add_child(object)
	object.global_position = player.global_position
	
	object.dir = player.position - object.get_global_mouse_position()
	return object

func GetAttributeCurrentValue(attr: StringName) -> float:
	return attribute_container.get_attribute_current_value(attr)

func _on_ability_activated(_entity: Object, ability: int) -> void:
	if ability_tags.has(ability):
		is_stamina_regen = true

func _on_attribute_maxed(attr_name: StringName):
	match attr_name:
			"CurrentStamina":
				is_stamina_regen = false
