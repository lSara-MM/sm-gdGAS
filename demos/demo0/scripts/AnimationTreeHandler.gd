extends AnimationTree
class_name gdAnimationTreeHandler

var prevDir = Vector2.DOWN

func SetAnim(dir: Vector2) -> void:
	if dir != prevDir:
		if dir == Vector2.ZERO:
			set("parameters/Idle/blend_position", prevDir)
			get("parameters/playback").travel("Idle")
		else:
			set("parameters/Walk/blend_position", dir)
			get("parameters/playback").travel("Walk")
		
		prevDir = dir
