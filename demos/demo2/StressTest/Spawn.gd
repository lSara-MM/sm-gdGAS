extends Node

@export var prefab: PackedScene
@export var number: int = 100
@export var batch_size: int = 100
@onready var world = $"../GAS_World"

var expected_races := {
	Tags._Race_Beast: 0,
	Tags._Race_Undead: 0,
	Tags._Race_Machine: 0,
	Tags._Race_Elemental: 0,
	Tags._Race_Plant: 0,
}

var all: Array[Dictionary] = []

var abilities = {
	Tags._Ability_BasicAttack: preload("res://StressTest/abilities/basic_attack.tres"),
	Tags._Ability_Haste: preload("res://StressTest/abilities/haste.tres"),
	Tags._Ability_Taunt: preload("res://StressTest/abilities/taunt.tres"),
	Tags._Ability_Invulnerability: preload("res://StressTest/abilities/invulnerability.tres"),
	Tags._Ability_PoisonDart: preload("res://StressTest/abilities/poison_dart.tres"),
	Tags._Ability_PoisonExplosion: preload("res://StressTest/abilities/poison_explosion.tres"),
	Tags._Ability_FrostNova: preload("res://StressTest/abilities/frost_nova.tres"),
	Tags._Ability_MassPurge: preload("res://StressTest/abilities/mass_purge.tres"),
	Tags._Ability_Freeze: preload("res://StressTest/abilities/freeze.tres"),
	Tags._Ability_Slow: preload("res://StressTest/abilities/slow.tres"),
	Tags._Ability_MassHeal: preload("res://StressTest/abilities/mass_heal.tres"),
	Tags._Ability_Regeneration: preload("res://StressTest/abilities/regeneration.tres"),
	Tags._Ability_Execution: preload("res://StressTest/abilities/execution.tres")
}

func _ready() -> void:
	call_deferred("RunTests")

func RunTests() -> void:
	await Spawn(number)
	RunTagTests()
	RunAbilityTests()

func Spawn(amount: int) -> void:
	if not is_instance_valid(prefab):
		push_error("Stress test prefab is not valid.")
		return

	if not is_instance_valid(world):
		push_error("GAS_World is not valid.")
		return

	print("")
	print("Spawning ", amount, " entities...")

	all.clear()

	for race in expected_races:
		expected_races[race] = 0

	var expected_undead := 0

	var start_time := Time.get_ticks_usec()

	for i in amount:
		var object := prefab.instantiate() as GAS_Entity

		if object == null:
			push_error("Prefab does not contain a GAS_Entity.")
			continue

		add_child(object)

		var dictionary: Dictionary = object.SetUp(world.rng, abilities)
		dictionary["entity"] = object
		all.push_back(dictionary)

		var race = dictionary["race"]
		expected_races[race] += 1

		if race == Tags._Race_Undead:
			expected_undead += 1

		var attribute_container := object.get_attribute_container()

		if attribute_container != null:
			attribute_container.attribute_changed.connect(
				_on_attribute_changed.bind(object)
			)

		if batch_size > 0 and (i + 1) % batch_size == 0:
			await get_tree().process_frame

	var elapsed_usec := Time.get_ticks_usec() - start_time
	var elapsed_seconds := elapsed_usec / 1_000_000.0

	print("")
	print("========== STRESS TEST ==========")
	print("Created: ", all.size(), " entities")
	print("Creation time: ", "%.3f" % elapsed_seconds, " seconds")
	print(
		"Entities/second: ",
		"%.1f" % (all.size() / max(elapsed_seconds, 0.000001))
	)

	print("Race distribution: ", expected_races)

	print("Expected world entity count: ", all.size())
	print("Actual world entity count: ", world.entity_count)

	assert(
		world.entity_count == all.size(),
		"World entity count does not match number of spawned entities."
	)

	print("Entity count verification: PASS")

	print("Expected undead: ", expected_undead)

	var undead = world.all_with_tags(
		PackedInt32Array([Tags._Race_Undead])
	)

	print("Actual undead: ", undead.size())

	assert(
		undead.size() == expected_undead,
		"Undead tag query returned an unexpected number of entities."
	)

	print("Single-tag query verification: PASS")
	print("=================================")

func RunTagTests() -> void:
	print("")
	print("========== TAG TESTS ==========")

	if all.is_empty():
		print("No entities to test.")
		return

	TestAllTagQuery()
	TestAnyTagQuery()
	TestNoneTagQuery()
	TestMultiTagQuery()
	TestTagMutation()

	print("All tag tests passed.")
	print("================================")

func TestAllTagQuery() -> void:
	for race in expected_races:
		var result = world.all_with_tags(
			PackedInt32Array([race])
		)

		var expected: int = expected_races[race]
		print(
			"ALL query - ",
			race,
			": ",
			result.size(),
			" / expected: ",
			expected
		)

		assert(
			result.size() == expected,
			"ALL tag query returned an unexpected number of entities."
		)

	print("ALL tag queries: PASS")

func TestAnyTagQuery() -> void:
	var result = world.any_with_tags(
		PackedInt32Array([
			Tags._Race_Undead,
			Tags._Race_Machine
		])
	)

	var expected: int = (
		expected_races[Tags._Race_Undead]
		+ expected_races[Tags._Race_Machine]
	)

	print(
		"ANY query - Undead OR Machine: ",
		result.size(),
		" / expected: ",
		expected
	)

	assert(
		result.size() == expected,
		"ANY tag query returned an unexpected number of entities."
	)

	print("ANY tag query: PASS")

func TestNoneTagQuery() -> void:
	var result = world.none_with_tags(
		PackedInt32Array([
			Tags._Race_Undead,
			Tags._Race_Machine
		])
	)

	var expected: int = all.size() \
		- expected_races[Tags._Race_Undead] \
		- expected_races[Tags._Race_Machine]

	print(
		"NONE query - not Undead and not Machine: ",
		result.size(),
		" / expected: ",
		expected
	)

	assert(
		result.size() == expected,
		"NONE tag query returned an unexpected number of entities."
	)

	print("NONE tag query: PASS")

func TestMultiTagQuery() -> void:
	var result = world.all_with_tags(
		PackedInt32Array([
			Tags._Race_Undead,
			Tags._Tier_Elite
		])
	)

	var expected := 0

	for data in all:
		if data["race"] == Tags._Race_Undead \
		and data["tier"] == Tags._Tier_Elite:
			expected += 1

	print(
		"ALL query - Undead AND Elite: ",
		result.size(),
		" / expected: ",
		expected
	)

	assert(
		result.size() == expected,
		"Multi-tag ALL query returned an unexpected number of entities."
	)

	print("Multi-tag ALL query: PASS")

func TestTagMutation() -> void:
	var data: Dictionary = all[0]
	var entity: GAS_Entity = data["entity"]

	var original_race = data["race"]
	var tag_container := entity.get_tag_container()

	if tag_container == null:
		push_error("Entity has no TagContainer.")
		return

	var before = world.all_with_tags(
		PackedInt32Array([original_race])
	)

	assert(
		before.has(entity),
		"Entity was not found by its original race tag."
	)

	tag_container.remove_tag(original_race)
	
	var after_remove = world.all_with_tags(
		PackedInt32Array([original_race])
	)

	print(
		"Tag mutation - after removal: ",
		after_remove.size(),
		" / before: ",
		before.size()
	)

	assert(
		not after_remove.has(entity),
		"Entity is still returned after removing its tag."
	)

	tag_container.add_tag(original_race)

	var after_add = world.all_with_tags(
		PackedInt32Array([original_race])
	)

	print(
		"Tag mutation - after re-adding: ",
		after_add.size(),
		" / expected: ",
		before.size()
	)

	assert(
		after_add.has(entity),
		"Entity was not returned after re-adding its tag."
	)

	assert(
		after_add.size() == before.size(),
		"Tag query count did not return to its original value."
	)

	print("Tag mutation: PASS")

func RunAbilityTests() -> void:
	print("")
	print("========== ABILITY TESTS ==========")

	var failures := 0

	for data in all:
		var entity: GAS_Entity = data["entity"]
		var tier = data["tier"]

		var ability_container = entity.ability_container

		if ability_container == null:
			push_error("Entity has no AbilityContainer.")
			failures += 1
			continue

		var expected_ability_count := 0

		match tier:
			Tags._Tier_Lesser:
				expected_ability_count = 2

			Tags._Tier_Base:
				expected_ability_count = 3

			Tags._Tier_Elite:
				expected_ability_count = 4

		var actual_ability_count = ability_container.abilities.size()

		if actual_ability_count != expected_ability_count:
			failures += 1

			print(
				"Ability count mismatch - expected ",
				expected_ability_count,
				", got ",
				actual_ability_count
			)

	assert(
		failures == 0,
		"Some entities have an incorrect number of abilities."
	)

	print("Ability count verification: PASS")
	print("==================================")

func _on_attribute_changed(
	attribute_name: StringName,
	new_value: float,
	_old_value: float,
	entity: GAS_Entity
) -> void:

	if attribute_name == "CurrentHp" and new_value <= 0:
		var tag_container := entity.get_tag_container()

		if tag_container != null:
			tag_container.remove_tag(Tags._State_Alive)

func ResetWorld() -> void:
	print("")
	print("Resetting stress test...")

	for child in get_children():
		child.queue_free()

	all.clear()

	for race in expected_races:
		expected_races[race] = 0

	await get_tree().process_frame

	print("Reset complete.")
