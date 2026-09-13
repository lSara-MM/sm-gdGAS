extends GAS_World

@export var activations_per_second := 500
@export var measure_duration := 10.0

@export var entities_num = 2000

var rng := RandomNumberGenerator.new()
var entities: Array[GAS_Entity] = []

var activation_requests := 0
var successful_activations := 0

var total_activation_time_usec := 0

var request_accumulator := 0.0
var test_start_time_usec := 0


func _ready() -> void:
	rng.seed = StressTestConfig.RNG_SEED
