extends GAS_World

func _ready() -> void:
	var a = all_with_tags([Tags._abc_a])
	print(a.size())
	
	pass


func _on_tag_container_tag_container_added(_owner: Object) -> void:
	pass # Replace with function body.
