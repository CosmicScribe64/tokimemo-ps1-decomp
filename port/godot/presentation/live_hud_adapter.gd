extends RefCounted
## T-29010. Postcommit presentation for the genuine captured-boundary app.
## Rebuild phase from committed history; no session, trace or save mutation.
## T-39013: open_fresh is the separate fresh-session path (HudSession state).

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const MANIFEST_SHA := ContentIdentity.HUD_MANIFEST_SHA
const CONTEXT_SHA := "2ec736f93177c47d7d0dce13444183215cbc65a88b40a065319c73bee9ce0451"
const BOUNDARY_SHA := "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5"
var error: String = ""
var presenter: Presenter = Presenter.new()
var profile: Dictionary = {}
var fresh_session: HudSession = null
var initial_icons: Array = []
var initial_params: Array = []
var _opened: bool = false

## T-39013: fresh-session HUD. The HUD manifest path, its identity and the font
## root come from Profile.read_pack over the installed root, which re-checks the
## inventory on disk; the caller passes no path or hash. Mutable state comes
## only from an intact HudSession driven by the live route. Dictionaries,
## captured boundary contexts and caller-built opening records are rejected.
func open_fresh(installed_root: String, rule: Dictionary, core: Dictionary, session: Variant) -> bool:
	_clear()
	error = "fresh HUD requires a source-driven HUD session at the command select boundary"
	if not session is HudSession:
		return false
	var live: HudSession = session
	if not live.at_command_select():
		return false
	if core.get("game_mode") != 0x34 or core.get("mode_stage") != 1 or core.get("mode_step") != 0:
		error = "fresh HUD core state is not mode 0x34 stage 1 step 0"
		return false
	if not rule.get("menu_cursor") is int or int(rule["menu_cursor"]) != live.menu_cursor():
		error = "fresh HUD rule cursor differs from the HUD session menu cursor"
		return false
	var family_hex := _family_hex(rule.get("profile_family_bytes"))
	if family_hex.is_empty():
		error = "fresh HUD requires a terminated one-to-three code-unit family name"
		return false
	var installed := _installed_hud(installed_root)
	if installed.is_empty():
		return false
	var candidate = Presenter.new()
	if not candidate.open_manifest(installed["manifest"], installed["sha256"], installed["font_root"]):
		error = candidate.error
		return false
	var context: Dictionary = live.hud_context(0x34, 1, 0, family_hex)
	var view: Dictionary = candidate.create_state(rule, context)
	if view.is_empty():
		error = candidate.error
		return false
	profile = {"fresh": true, "family_name_hex": family_hex,
		"icon_mode": live.icon_mode, "icon_count": live.icon_count}
	presenter = candidate
	fresh_session = live
	_opened = true
	error = reset(rule)
	if not error.is_empty():
		_clear()
		return false
	return true

func open(manifest_path: String, manifest_sha: String, font_root: String,
	context_path: String, context_sha: String) -> bool:
	_clear()
	if manifest_sha != MANIFEST_SHA or context_sha != CONTEXT_SHA or not Presenter._safe_path(context_path) or FileAccess.get_sha256(context_path) != context_sha:
		error = "live HUD needs the reviewed content and genuine boundary context identities"
		return false
	if not presenter.open_manifest(manifest_path, manifest_sha, font_root):
		error = presenter.error
		presenter = Presenter.new()
		return false
	var parsed: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(context_path)))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.development-hud-context.v1" or parsed.get("boundary_sha256") != BOUNDARY_SHA:
		_clear()
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
	if fresh_session != null:
		return _compose_fresh(rule, core)
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

## Command select frame from the live HUD session: actual hover phase, hover
## record, cursor position/style and icon sequence phase. A scaled pulse phase is
## withheld by the presenter with an explicit error.
func _compose_fresh(rule: Dictionary, core: Dictionary) -> Dictionary:
	if core["game_mode"] != 0x34 or core["mode_stage"] != 1 or core["mode_step"] != 0 or not fresh_session.at_command_select():
		error = "fresh HUD session has no source-owned transition beyond command select"
		return {}
	if not rule.get("menu_cursor") is int or int(rule["menu_cursor"]) != fresh_session.menu_cursor():
		error = "fresh HUD rule cursor differs from the HUD session menu cursor"
		return {}
	var context: Dictionary = fresh_session.hud_context(0x34, 1, 0, profile["family_name_hex"])
	var state: Dictionary = presenter.create_state(rule, context)
	if state.is_empty():
		error = presenter.error
		return {}
	error = presenter.set_animation_phase(state, fresh_session.icon_steps, fresh_session.icon_elapsed)
	if not error.is_empty():
		return {}
	var frame_cnt: int = int(core.get("frame_count", 0))
	var health_mode: int = (int(rule.get("player_flags", 0)) >> 8) & 15
	presenter._compositor.set_checker_uv(
		Presenter.NativeCompositor.compute_checker_uv(frame_cnt, health_mode)
	)
	var command: Dictionary = presenter.canvas_command(state)
	if command.is_empty():
		error = presenter.error
	return command

static func _installed_inventory(installed_root: String) -> Dictionary:
	if not Presenter._safe_path(installed_root.path_join("manifest.json")):
		return {}
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(installed_root.path_join("manifest.json")))
	return parsed if parsed is Dictionary else {}

func _installed_hud(installed_root: String) -> Dictionary:
	var pack: Dictionary = Profile.read_pack(installed_root, _installed_inventory(installed_root))
	if pack.get("valid") != true:
		error = "fresh HUD requires a valid installed runtime: " + str(pack.get("message", ""))
		return {}
	var paths: Variant = pack.get("paths")
	var identities: Variant = pack.get("file_identities")
	if not paths is Dictionary or not identities is Dictionary or not paths.get("hud_manifest") is String or not paths.get("font_root") is String or not identities.get("hud_manifest") is Dictionary or not identities["hud_manifest"].get("sha256") is String:
		error = "installed runtime has no authenticated HUD manifest and font root"
		return {}
	return {"manifest": paths["hud_manifest"], "sha256": identities["hud_manifest"]["sha256"], "font_root": paths["font_root"]}

func _clear() -> void:
	profile.clear()
	initial_icons.clear()
	initial_params.clear()
	fresh_session = null
	_opened = false
	presenter = Presenter.new()

static func _family_hex(raw_name: Variant) -> String:
	if not raw_name is Array or raw_name.size() != 8:
		return ""
	var name_bytes := PackedByteArray()
	for byte: Variant in raw_name:
		if not byte is int or byte < 0 or byte > 255:
			return ""
		name_bytes.append(byte)
	for offset: int in range(0, 8, 2):
		if name_bytes[offset] == 0 and name_bytes[offset + 1] == 0:
			return name_bytes.slice(0, offset).hex_encode() if offset >= 2 else ""
	return ""

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
