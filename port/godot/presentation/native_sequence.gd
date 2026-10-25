extends RefCounted
## T-29005: actor_draw_all's school-day flags=5, draw_flags=0x84 path.
## Call once per native draw after the mode handler. Rendering never calls it.

const Math = preload("res://core/native_math.gd")

var error: String = ""
var steps: Array[Dictionary] = []
var frame: int = 0
var counter: int = 0
var active: bool = false
var period: int = 1
var last_iteration: int = -1

func configure(sequence: Array, blink_period: int = 1) -> bool:
	error = ""
	if sequence.is_empty() or sequence.size() > 128:
		return _fail("school-day sequence requires 1..128 proved steps")
	if blink_period != 1 and blink_period != 0x82:
		return _fail("unsupported school-day actor blink_period: %d" % blink_period)
	var prepared: Array[Dictionary] = []
	for value: Variant in sequence:
		if not value is Dictionary or not value.get("frame_index") is int or not value.get("duration") is int:
			return _fail("sequence step requires integer frame_index and duration")
		if value["frame_index"] < 0 or value["duration"] < 0 or value["duration"] > 32767:
			return _fail("sequence frame/duration exceeds sourced signed-halfword range")
		prepared.append(value.duplicate(true))
	steps = prepared
	frame = 0
	counter = 0
	period = blink_period
	active = true
	last_iteration = -1
	return true

func native_draw(iteration: int) -> Dictionary:
	if steps.is_empty() or iteration <= last_iteration:
		error = "sequence draw requires increasing native iteration and configured steps"
		return {}
	last_iteration = iteration
	# The original enqueues the old frame before changing the actor counters.
	var output: Dictionary = {"frame_index": steps[frame]["frame_index"],
		"active_before": active, "completed": false}
	if active:
		# actor_draw_all stores the increment through sh, then reloads with lh
		# before comparing the duration. Wrap before the comparison, not after.
		counter = Math.s16(counter + (2 if period == 0x82 else 1))
		if counter >= steps[frame]["duration"]:
			frame += 1
			counter = 0
		if frame >= steps.size():
			# flags=5 takes terminal flag4: clear bits0/2 and retain final frame.
			active = false
			frame -= 1
			output["completed"] = true
	output["active"] = active
	return output

func snapshot() -> Dictionary:
	return {"frame": frame, "counter": counter, "active": active,
		"period": period, "last_iteration": last_iteration}

func restore(saved: Dictionary) -> bool:
	if saved.size() != 5 or not saved.get("frame") is int or not saved.get("counter") is int or not saved.get("active") is bool or not saved.get("period") is int or not saved.get("last_iteration") is int:
		return _fail("actor sequence checkpoint fields differ from sourced snapshot")
	if saved["frame"] < 0 or saved["frame"] >= steps.size() or saved["counter"] < -32768 or saved["counter"] > 32767 or saved["last_iteration"] < -1 or saved["period"] != period:
		return _fail("actor sequence checkpoint exceeds configured sequence")
	frame = saved["frame"]
	counter = saved["counter"]
	active = saved["active"]
	last_iteration = saved["last_iteration"]
	return true

func _fail(message: String) -> bool:
	error = message
	return false
