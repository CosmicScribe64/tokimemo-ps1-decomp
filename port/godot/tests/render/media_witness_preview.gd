extends Node2D
## Ignored originals are read only for this rendered evidence harness.
## This is a source-backed layout/sequence witness, not a playable route.

const Canvas = preload("res://presentation/native_canvas.gd")
const Sequence = preload("res://presentation/native_sequence.gd")
var _canvas: Node2D
var _frames: Dictionary = {}
var _background: Array[Dictionary] = []
var _sequence: RefCounted
var _iteration: int = 0
var _capture_iterations: Array[int] = [1, 10, 20, 36]
var _out: String = ""

func _ready() -> void:
	_canvas = Canvas.new()
	add_child(_canvas)
	RenderingServer.set_default_clear_color(Color.BLACK)
	call_deferred("_run")

func _run() -> void:
	var descriptor_path: String = OS.get_environment("TOKIMEMO_RENDER_WITNESS")
	_out = OS.get_environment("TOKIMEMO_RENDER_OUTPUT")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(descriptor_path))
	if not parsed is Dictionary or _out.is_empty():
		_fail("witness descriptor/output missing")
		return
	var descriptor: Dictionary = parsed
	var background: Image = _verified_png(descriptor["background"])
	if background == null:
		return
	_background = Canvas.background_commands(background, descriptor["background"]["sha256"])
	if _background.is_empty():
		_fail("background must be caller-active 320x88 extent")
		return
	for record: Dictionary in descriptor["frames"]:
		var image: Image = _verified_png(record)
		if image == null:
			return
		_frames[int(record["frame_index"])] = {"image": image, "record": record}
	_sequence = Sequence.new()
	# JSON numbers become floats; conversion belongs to this debug witness adapter.
	var steps: Array[Dictionary] = []
	for record: Dictionary in descriptor["sequence"]:
		if not _frames.has(int(record["frame_index"])):
			_fail("sequence references unexported frame")
			return
		steps.append({"frame_index": int(record["frame_index"]), "duration": int(record["duration"])})
	if not _sequence.configure(steps):
		_fail(_sequence.error)
		return
	# Let the X11 viewport finish activation before native draw1. This advances
	# no sequence state; a cold window may otherwise return its initial clear.
	for warmup: int in range(2):
		await get_tree().process_frame
		await RenderingServer.frame_post_draw
	var trace: Array[Dictionary] = []
	for iteration: int in range(1, 1000):
		var sample: Dictionary = _sequence.native_draw(iteration)
		var frame: Dictionary = _frames[sample["frame_index"]]
		var record: Dictionary = frame["record"]
		var bounds: Array = record["bounds"]
		var origin: Array = record["origin"]
		var commands: Array[Dictionary] = _background.duplicate()
		commands.append(Canvas.actor_command(frame["image"], record["sha256"],
			Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
			Vector2i(-80, -64), Vector2i(int(origin[0]), int(origin[1])), Vector2i(160, 120)))
		var error: String = _canvas.present(commands)
		if not error.is_empty():
			_fail(error)
			return
		trace.append({"native_draw": iteration, "frame": sample["frame_index"],
			"active": sample["active"], "completed": sample["completed"]})
		await get_tree().process_frame
		await RenderingServer.frame_post_draw
		if OS.get_environment("TOKIMEMO_CAPTURE_EVERY_FRAME") == "1" or iteration in _capture_iterations or sample["completed"]:
			var path: String = _out.path_join("godot-school-day-witness-%03d.png" % iteration)
			if get_viewport().get_texture().get_image().save_png(path) != OK:
				_fail("capture write failed")
				return
		if sample["completed"]:
			break
	var output: FileAccess = FileAccess.open(_out.path_join("godot-school-day-witness-trace.json"), FileAccess.WRITE)
	output.store_string(JSON.stringify({"scope": "source-backed activity witness; room/HUD not implemented; no route parity claim", "trace": trace}, "\t"))
	print("PASS: actual Godot GL render of original background and tick-driven CLUT sprite, ", trace.size(), " native draws")
	get_tree().quit(0)

func _verified_png(record: Dictionary) -> Image:
	var path: String = str(record.get("path", ""))
	if FileAccess.get_sha256(path) != str(record.get("sha256", "")):
		_fail("external witness payload hash differs: " + path)
		return null
	var image: Image = Image.new()
	if image.load_png_from_buffer(FileAccess.get_file_as_bytes(path)) != OK:
		_fail("external PNG decode failed: " + path)
		return null
	return image

func _fail(message: String) -> void:
	push_error(message)
	get_tree().quit(1)
