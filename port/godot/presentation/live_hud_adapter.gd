extends RefCounted
## T-29010. Postcommit presentation for the genuine captured-boundary app.
## Rebuild phase from committed history; no session, trace or save mutation.

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const MANIFEST_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"
const CONTEXT_SHA := "2ec736f93177c47d7d0dce13444183215cbc65a88b40a065319c73bee9ce0451"
const BOUNDARY_SHA := "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5"
var error: String = ""
var presenter: Presenter = Presenter.new()
var profile: Dictionary = {}
var initial_icons: Array = []
var initial_params: Array = []
var _opened: bool = false

func open(manifest_path: String, manifest_sha: String, font_root: String,
	context_path: String, context_sha: String) -> bool:
	profile.clear()
	_opened = false
	initial_icons.clear()
	initial_params.clear()
	presenter = Presenter.new()
	if manifest_sha != MANIFEST_SHA or context_sha != CONTEXT_SHA or not Presenter._safe_path(context_path) or FileAccess.get_sha256(context_path) != context_sha:
		error = "live HUD needs the reviewed content and genuine boundary context identities"
		return false
	if not presenter.open_manifest(manifest_path, manifest_sha, font_root):
		error = presenter.error
		return false
	var parsed: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(context_path)))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.development-hud-context.v1" or parsed.get("boundary_sha256") != BOUNDARY_SHA:
		error = "live HUD context is not the genuine captured boundary"
		return false
	profile = parsed
	_opened = true
	error = ""
	return true

func reset(rule: Dictionary) -> String:
	if not _opened:
		return "live HUD capability is not open"
	initial_icons = rule["icon_enabled"].slice(0, 14).duplicate()
	initial_params = rule["params"].duplicate()
	return ""

func compose(rule: Dictionary, core: Dictionary, history: Array[Dictionary],
	scene_commands: Array[Dictionary]) -> Dictionary:
	error = ""
	if not _opened:
		error = "live HUD capability is not open"
		return {}
	var mode: int = core["game_mode"]
	if mode not in [0x30, 0x34, 0x40, 0x31, 0x32, 0x38]:
		error = "live HUD has no source presentation for mode 0x%02x" % mode
		return {}
	var context: Dictionary = {"game_mode": mode, "mode_stage": core["mode_stage"],
		"mode_step": core["mode_step"], "icon_mode": profile["icon_mode"],
		"icon_count": profile["icon_count"], "cursor_style": profile["cursor_style"],
		"hover_ticks": 32, "family_name_hex": profile["family_name_hex"]}
	var state: Dictionary = presenter.create_state(rule, context)
	if state.is_empty():
		error = presenter.error
		return {}
	var steps: Array = profile["icon_steps"].duplicate()
	var elapsed: Array = profile["icon_elapsed"].duplicate()
	var flags: Array = initial_icons.duplicate()
	var previous_mode: int = 0x34
	var previous_substep: int = -1
	var previous_params: Array = initial_params.duplicate()
	var colors: Array = [0, 0, 0, 0, 0, 0, 0, 0, 0]
	var selected_command: int = int(rule["last_command"]) & 15
	for row: Dictionary in history:
		var row_mode: int = row["mode"]
		for intent: Dictionary in row["presentation"]:
			if intent.get("kind") == "week_end_hud_init":
				colors.fill(0)
		if row_mode == 0x40 and previous_mode == 0x40 and previous_substep == 0 and row["stage"] == 0 and row["step"] == 0 and row["substep"] in [1, 2]:
			colors.fill(0)
			steps.fill(0)
			elapsed.fill(0)
			for icon: int in range(14):
				flags[icon] = 1 if icon == selected_command else 0
		for stat: int in range(9):
			var before: int = int(previous_params[stat]) >> 16
			var after: int = int(row["params"][stat]) >> 16
			if before != after:
				colors[stat] = 0x43 if after > before else 0x44
		previous_params = row["params"]
		if row_mode in [0x30, 0x34, 0x40, 0x31, 0x32, 0x38]:
			for icon: int in range(14):
				if flags[icon] != 1:
					continue
				var sequence: Array = presenter._manifest["tables"]["sequences"][icon]["steps"]
				elapsed[icon] += 1
				if elapsed[icon] >= int(sequence[steps[icon]]["duration"]):
					steps[icon] = (steps[icon] + 1) % sequence.size()
					elapsed[icon] = 0
		previous_mode = row_mode
		previous_substep = row["substep"]
	error = presenter.set_animation_phase(state, steps, elapsed)
	if not error.is_empty():
		return {}
	var frame_cnt: int = int(core.get("frame_count", history.size()))
	var health_mode: int = (int(rule.get("player_flags", 0)) >> 8) & 15
	presenter._compositor.set_checker_uv(
		Presenter.NativeCompositor.compute_checker_uv(frame_cnt, health_mode)
	)
	var command: Dictionary = presenter.canvas_command(state) if mode in [0x30, 0x34] else presenter.activity_command(state, scene_commands, colors)
	if command.is_empty():
		error = presenter.error
		return {}
	return command

static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var result: Dictionary = {}
		for key: String in value:
			result[key] = _integers(value[key])
		return result
	if value is Array:
		var result: Array = []
		for item: Variant in value:
			result.append(_integers(item))
		return result
	return int(value) if value is float and value == floor(value) else value
