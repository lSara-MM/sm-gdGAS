extends GAS_World

@export var activations_per_second := 500
var rng := RandomNumberGenerator.new()
var entities: Array[GAS_Entity] = []

func _ready() -> void:
	rng.seed = StressTestConfig.RNG_SEED
	entities = get_entities()

func _process(_delta: float) -> void:
	if entities.is_empty():
		return

	for i in activations_per_second:
		var index = rng.randi_range(0, entities.size() - 1)
		var entity = entities[index]

		if is_instance_valid(entity):
			TryActivateRandomAbility(entity)
			
func TryActivateRandomAbility(entity: GAS_Entity) -> void:
	var abilities = entity.ability_container.get_abilities()
	if abilities.is_empty():
		return

	var ability_tag = abilities[rng.randi_range(0, abilities.size() - 1)]
	var ability_id = ability_tag.get_ability_id()
	entity.ability_container.try_activate(ability_id)
