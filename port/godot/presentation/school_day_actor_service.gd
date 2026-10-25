extends RefCounted
## T-29005: deterministic actor draw adapter. Authoritative actor_state lives in
## SchoolDayState.rule_state and is covered by the session rollback/state hash.
## Resources are immutable validated content. Images never enter saved rule state.

const Sequence = preload("res://presentation/native_sequence.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

func initialize(actors: Array, resources: Dictionary) -> Dictionary:
	var prepared: Dictionary = {}
	for value: Variant in actors:
		if not value is Dictionary:
			return {"error": "school-day actor descriptor must be an object"}
		var actor: Dictionary = value
		var ident: int = int(actor.get("id", -1))
		if ident != 96 and ident != 97 or prepared.has(str(ident)):
			return {"error": "unsupported or duplicate school-day actor ID"}
		if int(actor.get("flags", 5)) != 5 or int(actor.get("draw_flags", 0x84)) != 0x84 or int(actor.get("quad_flags", 0x01000000)) != 0x01000000:
			return {"error": "unsupported school-day actor flags; do not approximate the operation"}
		var slot: int = int(actor.get("sequence_slot", -1))
		var steps: Variant = resources.get("sequences", {}).get(str(slot))
		if not steps is Array:
			return {"error": "required school-day sequence slot is unproved: %d" % slot}
		var sequence = Sequence.new()
		if not sequence.configure(steps, int(actor.get("blink_period", 1))):
			return {"error": sequence.error}
		for step: Dictionary in steps:
			if not resources.get("frames", {}).has(str(step["frame_index"])):
				return {"error": "required school-day frame missing: %s" % step["frame_index"]}
		prepared[str(ident)] = {"id": ident, "sequence_slot": slot,
			"x": int(actor.get("x", 0)), "y": int(actor.get("y", 0)),
			"flags": 5, "draw_flags": 0x84, "quad_flags": 0x01000000,
			"sequence": sequence.snapshot()}
	if not prepared.has("96"):
		return {"error": "school-day actor96 is required for source completion gate"}
	return {"error": "", "actor_state": prepared}

func native_draw(actor_state: Dictionary, resources: Dictionary, iteration: int,
	draw_origin: Vector2i) -> Dictionary:
	var prepared: Dictionary = actor_state.duplicate(true)
	var commands: Array[Dictionary] = []
	var services: Array[Dictionary] = []
	# Source actor pool is traversed ascending. Native part ordering is retained
	# in the resource reader's exported composite, separately from actor ordering.
	for ident: String in ["96", "97"]:
		if not prepared.has(ident):
			continue
		var actor: Dictionary = prepared[ident]
		var steps: Variant = resources.get("sequences", {}).get(str(actor["sequence_slot"]))
		if not steps is Array:
			return {"error": "required school-day sequence disappeared"}
		var sequence = Sequence.new()
		if not sequence.configure(steps, int(actor["sequence"]["period"])) or not sequence.restore(actor["sequence"]):
			return {"error": sequence.error}
		var sample: Dictionary = sequence.native_draw(iteration)
		if sample.is_empty():
			return {"error": sequence.error}
		var frame: Variant = resources.get("frames", {}).get(str(sample["frame_index"]))
		if not frame is Dictionary or not frame.get("image") is Image or not frame.get("bounds") is Rect2i or not frame.get("origin") is Vector2i:
			return {"error": "required school-day frame geometry/image disappeared"}
		commands.append(Canvas.actor_command(frame["image"], str(frame["id"]),
			frame["bounds"], Vector2i(actor["x"], actor["y"]), frame["origin"], draw_origin))
		actor["sequence"] = sequence.snapshot()
		if sample["completed"]:
			actor["flags"] = 0 # Native flags5 terminal clears bits0 and2.
			if ident == "96":
				# D_80120658 aliases actor96+8, the same duration/result counter;
				# D_80120666 aliases actor96+0x16, the source sequence slot.
				services.append({"service": "school_day.actor96", "payload": {
					"active": false, "counter": sequence.counter,
					"sequence_slot": actor["sequence_slot"]}})
	return {"error": "", "actor_state": prepared, "draw_commands": commands, "services": services}
