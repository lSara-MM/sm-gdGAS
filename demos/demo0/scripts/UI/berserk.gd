extends ProgressBar

@export var ability_tag : StringName
var ability_container : AbilityContainer
var tag_container : TagContainer
var tag_id : int
var to_fill : bool

func setup(entity: Node, world = null) -> void:
	ability_container = entity.get_node("AbilityContainer")
	tag_container = entity.get_node("TagContainer")
	tag_id = world.find_tag(ability_tag)
	tag_container.tag_added.connect(_on_tag_added)
	tag_container.tag_removed.connect(_on_tag_removed)
	value = 0
	max_value = 1
	
func _on_tag_added(_entity_owner: TagContainer, tag: int) -> void:
	if tag == tag_id:
		value = max_value
	
func _on_tag_removed(_entity_owner: TagContainer, tag: int) -> void:
	if tag == tag_id:
		value = 0
