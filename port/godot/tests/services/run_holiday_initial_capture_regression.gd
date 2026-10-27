extends SceneTree
## Real input/media preflight followed by initial holiday-enabled index-zero capture.
const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const State = preload("res://rules/school_day_state.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const SOURCE: String = "687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3"
const BEHAVIOR: String = "4d9f2a5cb57b6d3f10163552be9a3d2838d10a1a"

func _initialize() -> void:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	var error: String = app._load_inputs()
	if error.is_empty(): error = app._reset()
	if not error.is_empty():
		push_error("actual input/media preflight failed: " + error)
		_free_app(app)
		quit(1)
		return
	var media_sha: String = FileAccess.get_sha256(OS.get_environment("TOKIMEMO_MEDIA_PACK").path_join("manifest.json"))
	var versions: Dictionary = {"source_version": SOURCE,
		"content_version": JSON.stringify([OS.get_environment("TOKIMEMO_BOUNDARY_SHA"),
			OS.get_environment("TOKIMEMO_TABLES_SHA"), media_sha]).sha256_text(),
		"behavior_version": BEHAVIOR}
	var adapter: Adapter = Adapter.new(versions)
	var state: State = app.state
	var holiday_fixture: bool = app.holiday_controller != null \
		and state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0 \
		and state.rule_state.get("day_event_index") == 0 \
		and state.rule_state.get("resources", {}).get("available_holiday_commands") == [0]
	var captured: Dictionary = adapter.capture(app, "command") if holiday_fixture else {"error": "fixture identity did not match"}
	if not holiday_fixture or not captured.get("error", "fixture test failed").is_empty():
		push_error("holiday-enabled initial index-zero capture failed: " + str(captured.get("error", "fixture mismatch")))
		_free_app(app)
		quit(1)
		return
	print(JSON.stringify({"status": "PASS", "fixture": "holiday-enabled initial index-zero command", "capture_error": ""}))
	_free_app(app)
	quit(0)

func _free_app(app: Boot) -> void:
	app.canvas.free()
	app.pad.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()
