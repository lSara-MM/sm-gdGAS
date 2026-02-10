extends AttributeContainer

func _ready() -> void:
	var mod = ModifierData.new()
	mod.operation_type = ModifierData.OperationType.Add
	mod.source_id = 1
	mod.target_id = 4
	mod.value = 2
	add_modifier(&"6yu", mod)
	pass
