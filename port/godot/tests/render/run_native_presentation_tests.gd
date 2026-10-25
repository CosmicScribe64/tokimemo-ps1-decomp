extends SceneTree

const Sequence = preload("res://presentation/native_sequence.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Parts = preload("res://presentation/native_parts.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const Pad = preload("res://ui/native_pad.gd")

func _initialize() -> void:
	var sequence = Sequence.new()
	assert(sequence.configure([{"frame_index": 3, "duration": 2}, {"frame_index": 4, "duration": 1}]))
	var first: Dictionary = sequence.native_draw(1)
	assert(first["frame_index"] == 3 and first["active"])
	assert(sequence.native_draw(2)["frame_index"] == 3)
	var terminal: Dictionary = sequence.native_draw(3)
	assert(terminal["frame_index"] == 4 and terminal["completed"] and not terminal["active"])
	assert(not sequence.native_draw(4)["completed"])
	assert(sequence.native_draw(4).is_empty())
	assert(sequence.configure([{"frame_index": 0, "duration": 3}], 0x82))
	assert(sequence.native_draw(7)["active"])
	assert(sequence.native_draw(8)["completed"])
	assert(not sequence.configure([{"frame_index": 0, "duration": 1}], 0))
	_test_signed_counter_boundaries()
	var image: Image = Image.create(320, 88, false, Image.FORMAT_RGBA8)
	var commands: Array[Dictionary] = Canvas.background_commands(image, "fixture")
	assert(commands.size() == 2 and commands[0]["position"] == Vector2i(0, 72))
	assert(commands[1]["position"] == Vector2i(192, 72))
	assert(commands[0]["image"].get_size() == Vector2i(192, 88))
	var top: Image = Image.create(2, 1, false, Image.FORMAT_RGBA8)
	top.set_pixel(0, 0, Color.RED)
	top.set_pixel(1, 0, Color.TRANSPARENT)
	var bottom: Image = Image.create(2, 1, false, Image.FORMAT_RGBA8)
	bottom.fill(Color.BLUE)
	var composer = Parts.new()
	var composed: Image = composer.compose([
		{"image": top, "position": Vector2i.ZERO, "flip_flags": 2},
		{"image": bottom, "position": Vector2i.ZERO}], Rect2i(0, 0, 2, 1))
	assert(composed.get_pixel(0, 0) == Color.BLUE and composed.get_pixel(1, 0) == Color.RED)
	var actor: Dictionary = Canvas.actor_command(top, "fixture", Rect2i(64, 32, 2, 1),
		Vector2i(-80, -64), Vector2i.ZERO, Vector2i(160, 120))
	assert(actor["position"] == Vector2i(144, 88))
	var resources: Dictionary = {"sequences": {"0": [{"frame_index": 0, "duration": 1}]},
		"frames": {"0": {"image": top, "id": "fixture", "bounds": Rect2i(64, 32, 2, 1), "origin": Vector2i.ZERO}}}
	var service = ActorService.new()
	var initial: Dictionary = service.initialize([{"id": 96, "sequence_slot": 0, "x": -80, "y": -64}], resources)
	assert(initial["error"].is_empty())
	var saved: String = JSON.stringify(initial["actor_state"])
	var completed: Dictionary = service.native_draw(initial["actor_state"], resources, 1, Vector2i(160, 120))
	assert(completed["error"].is_empty() and completed["services"].size() == 1)
	assert(completed["services"][0] == {"service": "school_day.actor96", "payload": {"active": false, "counter": 0, "sequence_slot": 0}})
	assert(JSON.stringify(initial["actor_state"]) == saved)
	assert(completed["actor_state"]["96"]["flags"] == 0)
	var again: Dictionary = service.native_draw(completed["actor_state"], resources, 2, Vector2i(160, 120))
	assert(again["error"].is_empty() and again["services"].is_empty())
	assert(service.native_draw(initial["actor_state"], {}, 1, Vector2i.ZERO)["error"] != "")
	var pad = Pad.new()
	var press: InputEventKey = InputEventKey.new()
	press.physical_keycode = KEY_ENTER
	press.pressed = true
	pad._input(press)
	assert(pad.sample(0).pressed == 0x20)
	assert(pad.sample(1).pressed == 0)
	press.pressed = false
	pad._input(press)
	assert(pad.sample(2).released == 0x20)
	pad.free()
	print("PASS: native sequence timing, one-shot terminal, part order/flip/alpha and logical geometry")
	quit(0)

func _test_signed_counter_boundaries() -> void:
	var sequence = Sequence.new()
	assert(sequence.configure([{"frame_index": 3, "duration": 32767}]))
	var saved: Dictionary = sequence.snapshot()
	saved["counter"] = 32766
	assert(sequence.restore(saved))
	assert(sequence.native_draw(1)["completed"] and sequence.counter == 0)
	# Normal +1 also wraps before comparing when the stored counter is MAX16.
	assert(sequence.configure([{"frame_index": 3, "duration": 32767}]))
	saved = sequence.snapshot()
	saved["counter"] = 32767
	assert(sequence.restore(saved))
	var wrapped: Dictionary = sequence.native_draw(1)
	assert(wrapped["active"] and not wrapped["completed"])
	assert(sequence.counter == -32768 and sequence.frame == 0)
	assert(sequence.restore(sequence.snapshot()))
	assert(sequence.native_draw(2)["active"] and sequence.counter == -32767)
	# Fast +2 reaches 32768 from 32766, stores -32768, and stays active.
	assert(sequence.configure([{"frame_index": 3, "duration": 32767}], 0x82))
	saved = sequence.snapshot()
	saved["counter"] = 32766
	assert(sequence.restore(saved))
	wrapped = sequence.native_draw(1)
	assert(wrapped["active"] and not wrapped["completed"])
	assert(sequence.counter == -32768 and sequence.frame == 0)
	assert(sequence.restore(sequence.snapshot()))
	assert(sequence.native_draw(2)["active"] and sequence.counter == -32766)
	# A legitimate odd starting counter reaches MAX16 without overflowing.
	assert(sequence.configure([{"frame_index": 3, "duration": 32767}], 0x82))
	saved = sequence.snapshot()
	saved["counter"] = 32765
	assert(sequence.restore(saved))
	assert(sequence.native_draw(1)["completed"] and sequence.counter == 0)
	assert(sequence.configure([{"frame_index": 3, "duration": 1}]))
	saved = sequence.snapshot()
	saved["counter"] = -1
	assert(sequence.restore(saved))
	assert(sequence.native_draw(1)["active"] and sequence.counter == 0)
	assert(sequence.native_draw(2)["completed"] and sequence.counter == 0)
	for invalid: Variant in [-32769, 32768, 1.0, true]:
		saved = sequence.snapshot()
		saved["counter"] = invalid
		assert(not sequence.restore(saved))
	print("PASS: signed16 normal/fast counter boundaries and full signed checkpoint range")
