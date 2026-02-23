extends AttributeContainer

func _ready() -> void:
	var mod = ModifierData.new()
	mod.operation_type = ModifierData.OperationType.Add
	mod.source_id = 1
	mod.target_id = 4
	mod.value = 2
	pass


func _on_attribute_container_2_modifier_added(_owner: Object, attribute_name: StringName, _modifier_data: Object) -> void:
	print("Attribute added from 2")
	print(attribute_name)
	pass # Replace with function body.
