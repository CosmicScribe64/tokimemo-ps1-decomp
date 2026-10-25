extends SceneTree
## Original external-state integration; synthetic edits are isolated below.
const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const Save = preload("res://services/save_service.gd")
const Recipe = preload("res://tests/app/run_command7_app_tests.gd")
var failures: Array[String] = []
var checks: Array[String] = []
var apps: Array[Boot] = []
var images: Dictionary = {}

func check(ok: bool, label: String) -> void:
	checks.append(label)
	if not ok:
		failures.append(label)

func app() -> Boot:
	var result: Boot = Boot.new()
	result.canvas = Canvas.new()
	result.pad = Pad.new()
	result.status_label = Label.new()
	result.detail_label = Label.new()
	result.continue_week = false
	var message: String = result._load_inputs()
	if message.is_empty():
		message = result._reset()
	check(message.is_empty(), "original boundary/content/HUD preflight: " + message)
	apps.append(result)
	return result

func pixels(value: Boot) -> String:
	var draws: Array = value.canvas.get("_commands")
	if draws.size() != 1:
		return ""
	var texture: ImageTexture = value.canvas.get("_textures")[draws[0]["id"]]
	var image: Image = texture.get_image()
	var hash: HashingContext = HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(image.get_data())
	return hash.finish().hex_encode()

func run_recipe(value: Boot, command: int, rendered: int) -> void:
	for tick: int in range(601):
		Recipe.device_recipe(value, command, tick)
		value.session.clock.rendered_frames += rendered
		value._physics_process(1.0 / 60.0)
		if value.stopped:
			break
	check(value.stopped and value.failure.is_empty(), "command%d actual mapped-input route reaches first result, render schedule%d" % [command, rendered])

func original_cursor_matches(value: Boot, command: int) -> bool:
	# Existing hash-pinned original first-activity RAM has actor0 frame156 for
	# both commands. hud_cursor_actor_init(0) selects the stored cursor style.
	var expected: Image = Image.create(320, 240, false, Image.FORMAT_RGBA8)
	expected.fill(Color.TRANSPARENT)
	value.hud.presenter._compositor._draw_actor(expected, {"id": 0, "frame": 156,
		"position": Vector2i(-132, -43) if command == 0 else Vector2i(-84, -11),
		"variant": "base", "semi": false})
	var draws: Array = value.canvas.get("_commands")
	var actual: Image = value.canvas.get("_textures")[draws[0]["id"]].get_image()
	for y: int in range(240):
		for x: int in range(320):
			if expected.get_pixel(x, y).a > 0 and expected.get_pixel(x, y) != actual.get_pixel(x, y):
				return false
	return true

func _initialize() -> void:
	call_deferred("run")

func run() -> void:
	var output: String = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	var versions: Dictionary = {"source_version": "t29010-app-integration-v1",
		"content_version": (Boot.LiveHud.MANIFEST_SHA + Boot.LiveHud.CONTEXT_SHA).sha256_text(),
		"behavior_version": "t29010-captured-boundary-hud-v1"}
	var adapter: Adapter = Adapter.new(versions)
	var storage: Save = Save.new(output.path_join("saves"))
	var source: Boot = app()
	if not source.failure.is_empty() or source.hud.profile.is_empty():
		finish()
		return
	var initial: String = pixels(source)
	images["initial"] = initial
	var before: String = source.session.state_hash()
	var trace_before: Array = source.trace.duplicate(true)
	check(source._present_committed().is_empty() and pixels(source) == initial and source.session.state_hash() == before and source.trace == trace_before,
		"repeated presentation preserves frame/state/RNG/clock/trace")
	var original: Dictionary = source.state.rule_state.duplicate(true)
	var original_name: String = source.hud.profile["family_name_hex"]
	source.state.rule_state["params"][0] += 17 << 16
	check(source._present_committed().is_empty() and pixels(source) != initial, "native parameter words drive live digit pixels")
	source.state.rule_state = original.duplicate(true)
	source.hud.profile["family_name_hex"] = source.hud.profile["family_name_hex"].substr(0, 4)
	check(source._present_committed().is_empty() and pixels(source) != initial, "caller family name drives supported source glyphs")
	source.hud.profile["family_name_hex"] = original_name
	source.state.rule_state["day"] = 5
	source.state.rule_state["weekday"] = 2
	check(source._present_committed().is_empty() and pixels(source) != initial, "live calendar date drives vertical source actors")
	source.state.rule_state = original.duplicate(true)
	source.state.rule_state["menu_cursor"] = 7
	check(source._present_committed().is_empty() and pixels(source) != initial, "live cursor drives plate/icon/cursor composition")
	source.state.rule_state = original.duplicate(true)
	source._present_committed()
	var unsupported: Dictionary = source.state.projection()
	unsupported["game_mode"] = 0x46
	check(source.hud.compose(source.state.rule_state, unsupported, source.trace, []).is_empty()
		and not source.hud.error.is_empty(), "holiday mode refuses the ordinary HUD capability")
	var refused: Boot.LiveHud = Boot.LiveHud.new()
	check(not refused.open(OS.get_environment("TOKIMEMO_HUD_MANIFEST"), "bad", OS.get_environment("TOKIMEMO_FONT_ROOT"),
		OS.get_environment("TOKIMEMO_HUD_CONTEXT"), Boot.LiveHud.CONTEXT_SHA)
		and refused.compose(source.state.rule_state, source.state.projection(), source.trace, []).is_empty(),
		"failed capability open cannot render or advance the app")
	var command: Dictionary = adapter.capture(source, "command")
	check(command["error"].is_empty(), "HUD composition keeps original staged render IDs save-compatible")
	check(storage.save_slot("command", command["snapshot"])["error"].is_empty(), "command snapshot publishes using existing save service")
	var target: Boot = app()
	var loaded: Dictionary = storage.load_slot("command")
	check(loaded["error"].is_empty() and adapter.restore(target, loaded["snapshot"], target._present_committed).is_empty()
		and pixels(target) == initial, "fresh saved command rebinds identical HUD pixels without new save fields")
	for selected: int in [0, 7]:
		var reference_hashes: Array = []
		var reference_pixels: String = ""
		for rendered: int in [0, 1, 3]:
			var live: Boot = app()
			run_recipe(live, selected, rendered)
			var hashes: Array = live.trace.map(func(row: Dictionary) -> Array: return [row["state_hash"], row["event_hash"]])
			var frame: String = pixels(live)
			check(original_cursor_matches(live, selected), "command%d live activity preserves original cursor frame156 and position" % selected)
			if rendered == 0:
				reference_hashes = hashes
				reference_pixels = frame
				images[str(selected)] = frame
				var result: Dictionary = adapter.capture(live, "first_result")
				check(result["error"].is_empty(), "command%d first result captures original staged media IDs" % selected)
				check(storage.save_slot("result%d" % selected, result["snapshot"])["error"].is_empty(), "command%d first result saved to disk" % selected)
				var fresh: Boot = app()
				var disk: Dictionary = storage.load_slot("result%d" % selected)
				check(disk["error"].is_empty() and adapter.restore(fresh, disk["snapshot"], fresh._present_committed).is_empty()
					and pixels(fresh) == frame and fresh.trace == live.trace and fresh.session.state_hash() == live.session.state_hash(),
					"command%d disk restore rebuilds live phase/colors/frame and exact session/trace" % selected)
				var unchanged: String = pixels(fresh)
				var fail_restore: String = adapter.restore(fresh, command["snapshot"], func(value: Node) -> String:
					value.call("_present_committed")
					return "synthetic post-rebind rejection")
				check(not fail_restore.is_empty() and pixels(fresh) == unchanged and fresh._present_committed().is_empty() and pixels(fresh) == unchanged,
					"command%d rejected post-rebind rolls back renderer and next presentation" % selected)
			else:
				check(hashes == reference_hashes and frame == reference_pixels, "command%d trace and live pixels independent of render count%d" % [selected, rendered])
			check(live.canvas.get("_textures").size() == 1, "dynamic image texture ownership remains bounded")
	finish()

func finish() -> void:
	var report: Dictionary = {"checks": checks, "failures": failures, "pixel_hashes": images,
		"scope": "actual captured-boundary live app; source compositor, save rebind and render schedule; no original GPU parity claim"}
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("hud-app.json"), FileAccess.WRITE)
	output.store_string(JSON.stringify(report, "\t"))
	output.close()
	for failure: String in failures:
		push_error(failure)
	print("HUD app checks: %d, failures: %d" % [checks.size(), failures.size()])
	for value: Boot in apps:
		value.canvas.free()
		value.pad.free()
		value.status_label.free()
		value.detail_label.free()
		value.free()
	quit(0 if failures.is_empty() else 1)
