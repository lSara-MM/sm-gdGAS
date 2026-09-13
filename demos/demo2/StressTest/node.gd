extends Node

@onready var world = $"../GAS_World"

@export var activations_per_second := 500
@export var measure_duration := 10.0

var rng
var number
var entities: Array[GAS_Entity] = []

var activation_requests := 0
var successful_activations := 0

var total_activation_time_usec := 0

var request_accumulator := 0.0
var test_start_time_usec := 0
var previous_frame_time_usec := 0

var test_running := false

# Frame timing
var total_frame_time_usec := 0
var frame_count := 0
var longest_frame_time_usec := 0


func _ready() -> void:
	number = world.entities_num
	rng = world.rng


func StartTest() -> void:
	if test_running:
		return

	# Refresh the entity list because spawning may not have finished yet.
	entities = world.get_entities()

	if entities.size() < number:
		print(
			"Waiting for entities... ",
			entities.size(),
			"/",
			number
		)
		return

	print("")
	print("========== ABILITY STRESS TEST ==========")
	print("Entities: ", entities.size())
	print("Target activation requests/second: ", activations_per_second)
	print("Duration: ", measure_duration, " seconds")
	print("")

	activation_requests = 0
	successful_activations = 0
	total_activation_time_usec = 0

	request_accumulator = 0.0

	frame_count = 0
	total_frame_time_usec = 0
	longest_frame_time_usec = 0

	test_start_time_usec = Time.get_ticks_usec()
	previous_frame_time_usec = test_start_time_usec

	test_running = true

	set_process(true)


func _process(delta: float) -> void:
	# Start only once when Shift is pressed.
	if not test_running:
		if Input.is_action_just_pressed("lshift"):
			StartTest()

		return

	# Measure real time between frames.
	var current_time_usec := Time.get_ticks_usec()

	var frame_time_usec := (
		current_time_usec - previous_frame_time_usec
	)

	previous_frame_time_usec = current_time_usec

	total_frame_time_usec += frame_time_usec
	frame_count += 1
	longest_frame_time_usec = max(
		longest_frame_time_usec,
		frame_time_usec
	)

	# Measure actual wall-clock duration.
	var elapsed_seconds := float(
		current_time_usec - test_start_time_usec
	) / 1_000_000.0

	if elapsed_seconds >= measure_duration:
		test_running = false
		set_process(false)
		PrintResults(elapsed_seconds)
		return

	# Use delta only to determine how many requests should be made
	# during this frame.
	request_accumulator += activations_per_second * delta

	var requests_this_frame := int(request_accumulator)

	request_accumulator -= requests_this_frame

	for i in requests_this_frame:
		var index = rng.randi_range(0, entities.size() - 1)
		var entity = entities[index]

		if not is_instance_valid(entity):
			continue

		activation_requests += 1

		var start_time := Time.get_ticks_usec()

		var success := TryActivateRandomAbility(entity)

		var activation_time_usec := (
			Time.get_ticks_usec() - start_time
		)

		total_activation_time_usec += activation_time_usec

		if success:
			successful_activations += 1


func TryActivateRandomAbility(entity: GAS_Entity) -> bool:
	var ability_container = entity.ability_container

	if ability_container == null:
		return false

	var abilities = ability_container.get_abilities()

	if abilities.is_empty():
		return false

	var ability_tag = abilities[
		rng.randi_range(0, abilities.size() - 1)
	]

	var ability_id = ability_tag.get_ability_id()

	return ability_container.try_activate(ability_id)


func PrintResults(actual_duration: float) -> void:
	var average_activation_time_usec := 0.0

	if activation_requests > 0:
		average_activation_time_usec = (
			float(total_activation_time_usec)
			/ activation_requests
		)

	var requests_per_second = (
		float(activation_requests)
		/ max(actual_duration, 0.000001)
	)

	var successful_per_second = (
		float(successful_activations)
		/ max(actual_duration, 0.000001)
	)

	var average_frame_time_ms = 0.0

	if frame_count > 0:
		average_frame_time_ms = (
			float(total_frame_time_usec)
			/ frame_count
			/ 1000.0
		)

	var average_fps := 0.0

	if average_frame_time_ms > 0.0:
		average_fps = 1000.0 / average_frame_time_ms

	var lowest_fps = 0.0

	if longest_frame_time_usec > 0:
		lowest_fps = (
			1_000_000.0
			/ float(longest_frame_time_usec)
		)

	var throughput_ratio = (
		requests_per_second
		/ max(float(activations_per_second), 0.000001)
	)

	var success_rate = 0.0

	if activation_requests > 0:
		success_rate = (
			float(successful_activations)
			/ activation_requests
			* 100.0
		)

	print("")
	print("========== ABILITY STRESS RESULTS ==========")
	print("Entities: ", entities.size())
	print("Duration: ", "%.3f" % actual_duration, " seconds")
	print("")

	print(
		"Target requests/second: ",
		activations_per_second
	)

	print(
		"Actual requests/second: ",
		"%.1f" % requests_per_second
	)

	print(
		"Throughput achieved: ",
		"%.1f" % (throughput_ratio * 100.0),
		"%"
	)

	print("")

	print(
		"Activation requests: ",
		activation_requests
	)

	print(
		"Successful activations: ",
		successful_activations
	)

	print(
		"Success rate: ",
		"%.1f" % success_rate,
		"%"
	)

	print("")

	print(
		"Successful activations/second: ",
		"%.1f" % successful_per_second
	)

	print(
		"Average activation time: ",
		"%.3f" % (
			average_activation_time_usec / 1000.0
		),
		" ms"
	)

	print(
		"Total activation processing time: ",
		"%.3f" % (
			float(total_activation_time_usec)
			/ 1_000_000.0
		),
		" seconds"
	)

	print("")

	print(
		"Average FPS: ",
		"%.1f" % average_fps
	)

	print(
		"Lowest FPS: ",
		"%.1f" % lowest_fps
	)

	print(
		"Average frame time: ",
		"%.3f" % average_frame_time_ms,
		" ms"
	)

	print("============================================")
