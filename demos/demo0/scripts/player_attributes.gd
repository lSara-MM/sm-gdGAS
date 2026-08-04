extends AttributeContainer

func _pre_attribute_change(attribute_id: StringName, calculated_value: float) -> float:
	match attribute_id:
		"CurrentStamina":
			var maxValue = get_attribute_current_value(&"MaxStamina")
			if calculated_value > maxValue:
				return maxValue
	return calculated_value
