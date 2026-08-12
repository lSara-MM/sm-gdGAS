extends GAS_Entity
class_name PlayerEntity

@onready var attribute_container = get_attribute_container()
@onready var tag_container = get_tag_container()
@onready var ability_container = $AbilityContainer

func _process(_delta: float) -> void:
	if ability_container.iscooldown(Tags._Ability_Dash) && !ability_container.is_active(Tags._Ability_StaminaRegen) && attribute_container.get_attribute_current_value("CurrentStamina") < attribute_container.get_attribute_current_value("MaxStamina")  :
		#if Input.is_action_just_pressed("lctrl"):
			#ability_container.try_activate(Tags._Ability_StaminaRegen)
		
		ability_container.try_activate(Tags._Ability_StaminaRegen)
	pass

func IsDashing() -> bool:
	return tag_container.has_tag(Tags._Ability_Dash)

func TryDash() -> bool:
	return ability_container.try_activate(Tags._Ability_Dash)

func GetAttributeCurrentValue(attr: StringName) -> float:
	return attribute_container.get_attribute_current_value(attr)
