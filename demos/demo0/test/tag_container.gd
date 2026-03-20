extends TagContainer

func _ready() -> void:
	var a = preload("res://test/new_tag_data.tres")
	for i in a.children:
		print(i.get_name())
	
	var array :Array[TagData] = [a]
	RegisterTags(array)
	print("holaaa")
	pass
