extends CharacterBody2D

var prevDir = Vector2.DOWN

func _physics_process(_delta: float) -> void:
	GetInput()
	move_and_slide()

func GetInput() -> void:
	var inputDirection = Input.get_vector("left", "right", "up", "down")
	velocity = inputDirection * get_meta("speed")
	SetAnim(inputDirection)
	
func SetAnim(dir: Vector2) -> void:
	if dir != prevDir:
		if dir == Vector2.ZERO:
			print("idle")
			$AnimationTree.set("parameters/Idle/blend_position", prevDir)
			$AnimationTree.get("parameters/playback").travel("Idle")
		else:
			print("walk")
			$AnimationTree.set("parameters/Walk/blend_position", dir)
			$AnimationTree.get("parameters/playback").travel("Walk")
		
		prevDir = dir
