extends GAS_Entity
class_name PlayerEntity

@onready var attribute_container = get_attribute_container()
@onready var tag_container = get_tag_container()
@onready var ability_container = $AbilityContainer

func IsDashing() -> bool:
	return tag_container.has_tag(Tags._Ability_Dash)

func TryDash() -> bool:
	return ability_container.try_activate(Tags._Ability_Dash)

func GetAttributeCurrentValue(attr: StringName) -> float:
	return attribute_container.get_attribute_current_value(attr)
