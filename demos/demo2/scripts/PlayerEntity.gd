extends GAS_Entity
class_name PlayerEntity

@export var ability_regen_tags : PackedInt32Array
@export var berserk_particles : CPUParticles2D
@export var effect_particles : CPUParticles2D
@export var hurt_particles : CPUParticles2D

var is_stamina_regen = false
@export var bullet_prefab : PackedScene

@onready var attribute_container = get_attribute_container()
@onready var tag_container = get_tag_container()
@onready var ability_container = $AbilityContainer

func _process(_delta: float) -> void:
	if is_stamina_regen:
		ability_container.try_activate(Tags._Ability_StaminaRegen)

func TryAbility(ability: int) -> bool:
	return ability_container.try_activate(ability)
	
func IsAbilityActive(ability: int) -> bool:
	return tag_container.has_tag(ability)
	
func TryBerserk() -> bool:
	var ret = false
	if ability_container.is_active(Tags._Ability_Berserk):
		ret = ability_container.try_end(Tags._Ability_Berserk, false)
	else:
		ret = ability_container.try_activate(Tags._Ability_Berserk)
	
	if ret:
		berserk_particles.emitting = ability_container.is_active(Tags._Ability_Berserk)

	return ret

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

func CollectItem(_item: int, effect: EffectData):
	add_effect(effect)
	effect_particles.emitting = true
	
func _on_attribute_changed(attribute_name: StringName, new_value: float, old_value: float) -> void:
	match attribute_name:
		"CurrentHealth":
			if new_value < old_value:
				hurt_particles.emitting = true
				
			if new_value == 0:
				print("You died")
				get_parent().queue_free()

func _on_ability_activated(_entity: Object, ability: int) -> void:
	if ability_regen_tags.has(ability):
		is_stamina_regen = true

func _on_attribute_maxed(attr_name: StringName):
	match attr_name:
			"CurrentStamina":
				is_stamina_regen = false
