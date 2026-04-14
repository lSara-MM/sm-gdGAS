extends AttributeContainer

var mod : ModifierData

func _ready() -> void:
	mod = preload("res://test/new_modifier_data.tres")
	print("gdscript")
	pass
func _physics_process(_delta: float) -> void:
	GetInput()

func _process(_delta):
	
	pass

func GetInput() -> void:
	if Input.is_action_just_pressed("lshift"):
		AddEffect()
		
func AddEffect() -> void:
	print("Attribtues before")
	for i in attribute_set.attributes:
		print(i.name + ": " + str(get_attribute_current_value(i.name)))

	add_modifier(&"hhh", mod)
	print("Attribtues after")
	for i in attribute_set.attributes:
		print(i.name + ": " + str(get_attribute_current_value(i.name)))
