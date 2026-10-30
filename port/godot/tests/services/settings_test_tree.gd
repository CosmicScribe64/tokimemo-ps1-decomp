extends SceneTree
## Failed checks and unfinished tests exit nonzero, including asynchronous tests.
var failures := 0
var _elapsed := 0.0
func check(condition: bool) -> void:
	if not condition:
		failures += 1
		push_error("Settings regression check failed")
		print_stack()
func finish() -> void:
	quit(1 if failures > 0 else 0)
func _process(delta: float) -> bool:
	_elapsed += delta
	if failures > 0: quit(1)
	elif _elapsed > 10.0:
		printerr("Settings test did not finish within ten seconds")
		quit(1)
	return false
