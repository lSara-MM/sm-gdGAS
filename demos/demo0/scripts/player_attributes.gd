extends AttributeContainer

func _pre_attribute_change(attribute_id: StringName, calculated_value: float) -> float:
	var maxValue : float
	
	match attribute_id:
		"CurrentHealth":
			maxValue = get_attribute_current_value(&"MaxHealth")
		"CurrentStamina":
			maxValue = get_attribute_current_value(&"MaxStamina")
		_:
			return calculated_value
	
	if calculated_value > maxValue:
		return maxValue
		
	return calculated_value
