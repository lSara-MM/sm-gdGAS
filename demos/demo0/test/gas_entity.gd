extends GAS_Entity

func _physics_process(_delta: float) -> void:
	GetInput()
		
#func _process(_delta):
	#GetInput()
	#pass

func GetInput() -> void:
	if Input.is_action_just_pressed("rclick"):
		print("hoalknfkewmnfkemfe")
		add_effect(preload("res://test/new_effect_data.tres"))
		add_effect(preload("res://test/effect_temporary.tres"))
		
	if Input.is_action_just_pressed("lclick"):
		var val : float = get_attribute_container().get_attribute_current_value(&"Health")
		print(val)
		
		var val2 : float = get_attribute_container().get_attribute_current_value(&"Def")
		print(val2)
