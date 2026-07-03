extends Node

func _ready() -> void:
	print("-------------------------------------------------------------------------------------")
	print(FileAccess.file_exists("res://addons/GDExtAbilities/bin/libGDExtAbilities.windows.template_debug.x86_64.dll"))
	print(ClassDB.class_exists("GameplayAbility"))
	print(ClassDB.can_instantiate("GameplayAbility"))
	pass
