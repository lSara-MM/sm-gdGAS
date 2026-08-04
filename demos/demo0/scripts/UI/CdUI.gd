extends ProgressBar

@export var ability_tag : StringName
var ability_container : AbilityContainer
var tag_container : TagContainer
var tag_id : int

func setup(entity: Node, world = null) -> void:
	ability_container = entity.get_node("AbilityContainer")
	tag_container = entity.get_node("TagContainer")
	tag_id = world.find_tag(ability_tag)
	tag_container.tag_added.connect(_on_tag_added)
	value = ability_container.get_cooldown(tag_id)
	max_value = ability_container.get_cooldown(tag_id)
	
func _process(_delta: float) -> void:
	if ability_container.get_current_cooldown(tag_id) != max_value:
		value = max_value - ability_container.get_current_cooldown(tag_id)

func _on_tag_added(_entity_owner: TagContainer, tag: int) -> void:
	if tag == tag_id:
		value =  max_value - ability_container.get_current_cooldown(tag_id)
