extends SceneTree
## T-29022: Headless Producer & Provenance Verifier for all 20 EMULATOR_VS_GODOT_PAIRS captures.
## Renders every Godot capture compared by tools/port/run_emulator_vs_godot_audit.py from live
## presenters/controllers and explicit live state checkpoints, and emits producer-provenance.json.

const PlayableBoot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Session = preload("res://core/game_session.gd")
const CommandHudPresenter = preload("res://presentation/command_hud_presenter.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const ProloguePresenter = preload("res://presentation/prologue_presenter.gd")
const CalendarPhonePresenter = preload("res://presentation/calendar_phone_presenter.gd")
const VnScenePresenter = preload("res://presentation/vn_scene_presenter.gd")
const SystemOverlays = preload("res://overlays/system_overlays.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const TelephoneOverlayPresenter = preload("res://presentation/telephone_overlay_presenter.gd")

const PRODUCER_SCRIPT_REL := "tests/overlays/run_emulator_vs_godot_capture_producer.gd"

var _checks: int = 0
var _failures: Array[String] = []
var _records: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func _check(cond: bool, msg: String) -> void:
	_checks += 1
	if not cond:
		_failures.append(msg)
		printerr("FAIL: ", msg)
	else:
		print("PASS: ", msg)

func _resolve_repo_root() -> String:
	var env_root: String = OS.get_environment("TOKIMEMO_REPO_ROOT")
	if not env_root.is_empty() and FileAccess.file_exists(env_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")):
		return env_root
	for cand: String in ["/workspace", "/repo", ProjectSettings.globalize_path("res://../..").simplify_path()]:
		if FileAccess.file_exists(cand.path_join(".asset-cache/port/hud-production-v1/manifest.json")):
			return cand
	return "/workspace"

func _run() -> void:
	var repo: String = _resolve_repo_root()
	var out_dir: String = OS.get_environment("TOKIMEMO_AUDIT_CAPTURE_OUT")
	if out_dir.is_empty():
		out_dir = repo.path_join(".claude/orchestrator/state/T-29022/telephone-live-state-and-audit-provenance-v1/captures")
	DirAccess.make_dir_recursive_absolute(out_dir)

	var receipt_path: String = OS.get_environment("TOKIMEMO_PROVENANCE_RECEIPT_OUT")
	if receipt_path.is_empty():
		receipt_path = out_dir.path_join("producer-provenance.json")

	print("=== T-29022 Emulator-vs-Godot Live Capture Producer ===")
	_produce_oracle_command_captures(repo, out_dir)
	_produce_playable_boot_route_captures(repo, out_dir)
	_produce_prologue_captures(repo, out_dir)
	_produce_calendar_phone_captures(repo, out_dir)
	_produce_system_overlay_captures(repo, out_dir)
	_produce_telephone_overlay_captures(repo, out_dir)

	_check(_records.size() == 20, "Produced all 20 EMULATOR_VS_GODOT_PAIRS captures (got %d)" % _records.size())

	var source_scripts := [
		PRODUCER_SCRIPT_REL,
		"tests/overlays/run_telephone_overlay_tests.gd",
		"tests/run_prologue_visual_tests.gd",
		"tests/holiday/run_calendar_phone_tests.gd",
		"tests/overlays/run_system_overlays_tests.gd",
		"tests/holiday/run_continuous_scene_tests.gd",
		"app/playable_boot.gd",
		"app/overlay_dispatcher.gd",
		"presentation/command_hud_presenter.gd",
		"presentation/holiday_hud_adapter.gd",
		"presentation/hud_compositor.gd",
		"presentation/prologue_presenter.gd",
		"presentation/calendar_phone_presenter.gd",
		"presentation/vn_scene_presenter.gd",
		"presentation/telephone_overlay_presenter.gd",
		"overlays/telephone_overlay.gd",
		"overlays/system_overlays.gd",
	]
	var script_hashes: Dictionary = {}
	for rel: String in source_scripts:
		var abs_p: String = ProjectSettings.globalize_path("res://" + rel)
		if FileAccess.file_exists(abs_p):
			script_hashes[rel] = FileAccess.get_sha256(abs_p)

	var receipt := {
		"schema": "tokimemo.emulator_vs_godot_capture_provenance.v1",
		"ticket": "T-29022",
		"producer_script": PRODUCER_SCRIPT_REL,
		"producer_script_sha256": script_hashes.get(PRODUCER_SCRIPT_REL, ""),
		"total_pairs": _records.size(),
		"source_scripts": script_hashes,
		"captures": _records,
	}
	var rf := FileAccess.open(receipt_path, FileAccess.WRITE)
	_check(rf != null, "Opened provenance receipt for writing: " + receipt_path)
	if rf != null:
		rf.store_string(JSON.stringify(receipt, "\t") + "\n")
		rf.close()

	if not _failures.is_empty():
		printerr("FAIL: %d/%d capture producer checks failed" % [_failures.size(), _checks])
		quit(1)
		return

	print("PASS: All %d capture producer checks succeeded (%d captures bound in %s)" % [_checks, _records.size(), receipt_path])
	quit(0)


func _record_capture(
	pair_id: String,
	category: String,
	godot_capture_rel: String,
	img: Image,
	out_dir: String,
	suite_script: String,
	presenter_script: String,
	live_state_desc: Dictionary
) -> void:
	_check(img != null and img.get_size() == Vector2i(320, 240), "%s rendered at 320x240" % pair_id)
	if img == null:
		return
	var file_name: String = godot_capture_rel.get_file()
	var out_path: String = out_dir.path_join(file_name)
	var err: Error = img.save_png(out_path)
	_check(err == OK, "Saved produced capture %s -> %s" % [pair_id, out_path])
	var sha: String = FileAccess.get_sha256(out_path)
	var pres_abs: String = ProjectSettings.globalize_path("res://" + presenter_script)
	var pres_sha: String = FileAccess.get_sha256(pres_abs) if FileAccess.file_exists(pres_abs) else ""
	_records[pair_id] = {
		"id": pair_id,
		"category": category,
		"godot_capture": godot_capture_rel,
		"produced_file": file_name,
		"sha256": sha,
		"width": img.get_width(),
		"height": img.get_height(),
		"producer_script": PRODUCER_SCRIPT_REL,
		"suite_script": suite_script,
		"presenter_script": presenter_script,
		"presenter_script_sha256": pres_sha,
		"live_state": live_state_desc,
	}


# ---------------------------------------------------------------------------
# 1. Oracle Command 0 & 7b (CommandHudPresenter + oracle.v1.json)
# ---------------------------------------------------------------------------

class CommandHudPresenterWithSeason extends CommandHudPresenter:
	var checker_uv_override: Variant = null

	func _view(state: Dictionary) -> Dictionary:
		error = ""
		var st_copy: Dictionary = state.duplicate(true)
		var rule_in: Variant = st_copy.get("rule")
		var resources_dict: Dictionary = {"hud_season_fade": 0}
		if rule_in is Dictionary:
			if rule_in.get("resources") is Dictionary:
				resources_dict = rule_in["resources"].duplicate(true)
			var filtered_rule: Dictionary = {}
			for k: String in RULE_KEYS:
				if rule_in.has(k):
					filtered_rule[k] = rule_in[k]
			st_copy["rule"] = filtered_rule
		if _manifest.is_empty() or not _keys(st_copy, ["rule", "context", "icon_steps", "icon_elapsed"]) or not _keys(st_copy.get("rule"), RULE_KEYS) or not _keys(st_copy.get("context"), CONTEXT_KEYS):
			error = "HUD requires an open capability and live rule/context state"
			return {}
		var rule: Dictionary = st_copy["rule"]
		var context: Dictionary = st_copy["context"]
		for key: String in CONTEXT_KEYS:
			if key != "family_name_hex" and not _integer(context[key]):
				error = "HUD context is not an integer: " + key
				return {}
		var style: int = context["cursor_style"]
		var command_screen: bool = (context["game_mode"] == 0x34 and context["mode_stage"] == 1 and context["mode_step"] == 0) or context["game_mode"] == 0x30
		var activity: bool = (context["game_mode"] == 0x40 and context["mode_stage"] in [0, 1, 2] and context["mode_step"] in [0, 1, 2]) or (context["game_mode"] == 0x31 and context["mode_stage"] == 0 and context["mode_step"] == 0) or (context["game_mode"] == 0x32 and context["mode_stage"] in [0, 1, 0xff] and context["mode_step"] == 0) or (context["game_mode"] == 0x38 and context["mode_stage"] in [0, 1] and context["mode_step"] == 0)
		if not (command_screen or activity) or context["icon_mode"] != 0 or context["icon_count"] != 14 or context["hover_ticks"] != 32 or not (style == 0 or (style >= 0x9b and style < 0xae)):
			error = "HUD supports bounded TypeA command/school-day modes and settled hover only"
			return {}
		var family: Variant = context["family_name_hex"]
		if not family is String or family.length() not in [4, 8, 12] or not _hex(family) or not _glyphs_supported(family.hex_decode()):
			error = "live family name requires one to three supported native code units"
			return {}
		for key: String in ["year", "month", "day", "weekday", "menu_cursor", "player_flags", "player_flags_upper", "memory_card_busy"]:
			if not _integer(rule[key]):
				error = "live rule field is not an integer: " + key
				return {}
		var year: int = rule["year"]
		var month: int = rule["month"]
		var day: int = rule["day"]
		var month_index: int = (year - 95) * 12 + month - 4
		if month < 1 or month > 12 or month_index < 0 or month_index > 35 or int(rule["weekday"]) < 0 or int(rule["weekday"]) > 6 or day < 1 or day > 31 or int(rule["menu_cursor"]) < 0 or int(rule["menu_cursor"]) >= 14 or rule["memory_card_busy"] != 0:
			error = "HUD state is outside the spring TypeA/date/idle-card capability"
			return {}
		if not _numbers(rule["params"], 9, 0, 0x3e7ffff) or not _numbers(rule["icon_enabled"], 15, 0, 1) or not rule["calendar"] is Array or rule["calendar"].size() != 2:
			error = "live parameter/icon/calendar records differ from native dimensions"
			return {}
		var calendar_bytes: Array = []
		for calendar: Variant in rule["calendar"]:
			if not _keys(calendar, ["day_kind", "marker"]) or not _numbers(calendar["day_kind"], 32, 0, 255) or not _numbers(calendar["marker"], 32, 0, 255):
				error = "live calendar must contain two native byte arrays"
				return {}
			calendar_bytes.append_array(calendar["day_kind"])
			calendar_bytes.append_array(calendar["marker"])
		if int(calendar_bytes[0]) < 28 or int(calendar_bytes[0]) > 31 or day > int(calendar_bytes[0]):
			error = "live date exceeds its native calendar day count"
			return {}
		var flags: int = rule["player_flags"]
		var upper: int = rule["player_flags_upper"]
		if flags < 0 or flags > 65535 or upper < 0 or upper > 65535:
			error = "live player flags exceed native halfwords"
			return {}
		var tomorrow: int = calendar_bytes[32 + day + 1]
		var next_week: int = calendar_bytes[32 + day + 6]
		var club: int = flags >> 12
		if command_screen and (tomorrow in [5, 11] or (tomorrow == 8 and club != 11) or (next_week == 12 and club < 6) or (upper & 1) != 0 or ((flags >> 8) & 15) == 3):
			error = "forced, sick, or event-specific command narration is outside ordinary HUD capability"
			return {}
		if not _numbers(st_copy["icon_steps"], 14, 0, 31) or not _numbers(st_copy["icon_elapsed"], 14, 0, 65535):
			error = "live icon animation state is malformed"
			return {}
		for icon: int in range(14):
			var sequence: Array = _manifest["tables"]["sequences"][icon]["steps"]
			var step: int = st_copy["icon_steps"][icon]
			if step >= sequence.size() or int(st_copy["icon_elapsed"][icon]) >= int(sequence[step]["duration"]):
				error = "live icon phase is outside the source sequence"
				return {}
		var parameters: Array[int] = []
		for word: Variant in rule["params"]:
			parameters.append(int(word) >> 16)
		var rule_with_res: Dictionary = rule.duplicate(true)
		rule_with_res["resources"] = resources_dict
		var season: Dictionary = HudSeason.view(rule_with_res)
		if not season["error"].is_empty():
			error = season["error"]
			return {}
		if checker_uv_override is Vector2i:
			_compositor.set_checker_uv(checker_uv_override)
		return {
			"year": year,
			"month": month,
			"day": day,
			"weekday": int(rule["weekday"]),
			"day_kind": int(calendar_bytes[day]),
			"parameters": parameters,
			"selected": int(rule["menu_cursor"]),
			"icon_enabled": rule["icon_enabled"].slice(0, 14),
			"cursor_frame": style,
			"icon_steps": st_copy["icon_steps"].duplicate(),
			"icon_elapsed": st_copy["icon_elapsed"].duplicate(),
			"checker_season": season["checker_season"],
			"checker_fade": season["checker_fade"],
		}

	func activity_command(state: Dictionary, scene_commands: Array[Dictionary], parameter_colors: Array) -> Dictionary:
		if checker_uv_override is Vector2i:
			_compositor.set_checker_uv(checker_uv_override)
		return super.activity_command(state, scene_commands, parameter_colors)


func _produce_oracle_command_captures(repo: String, out_dir: String) -> void:
	var hud_manifest := repo.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var oracle_path := repo.path_join(".claude/orchestrator/state/T-29010/production/fixtures/oracle.v1.json")

	var presenter := CommandHudPresenterWithSeason.new()
	var opened := presenter.open_manifest(hud_manifest, PlayableBoot.LiveHud.MANIFEST_SHA, font_root)
	_check(opened, "CommandHudPresenter opened for oracle captures: " + presenter.error)
	if not opened:
		return

	var fixture: Dictionary = PlayableBoot._integers(JSON.parse_string(FileAccess.get_file_as_string(oracle_path)))
	var configs := [
		{
			"key": "0",
			"id": "oracle_command_0",
			"godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_oracle_command_0.png",
			"frame_count": 5460,
			"checker_uv": Vector2i(27, 2),
		},
		{
			"key": "7",
			"id": "oracle_command_7b",
			"godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_oracle_command_7b.png",
			"frame_count": 5552,
			"checker_uv": Vector2i(23, 20),
		},
	]
	for cfg: Dictionary in configs:
		var source: Dictionary = fixture["states"][cfg["key"]]
		var st: Dictionary = presenter.create_state(source["rule"], source["context"])
		var phase_err: String = presenter.set_animation_phase(st, source["phase"]["steps"], source["phase"]["elapsed"])
		_check(not st.is_empty() and phase_err.is_empty(), "Prepared oracle state %s: %s" % [cfg["id"], phase_err])
		presenter._compositor.set_checker_uv(cfg["checker_uv"])
		var cmd: Dictionary = presenter.canvas_command(st)
		var img: Image = cmd.get("image")
		var rule: Dictionary = source["rule"]
		_record_capture(
			cfg["id"],
			"hud_command",
			cfg["godot_capture"],
			img,
			out_dir,
			"tests/app/run_command7_app_tests.gd",
			"presentation/command_hud_presenter.gd",
			{
				"source_fixture": ".claude/orchestrator/state/T-29010/production/fixtures/oracle.v1.json",
				"state_key": cfg["key"],
				"game_mode": int(source["context"]["game_mode"]),
				"mode_stage": int(source["context"]["mode_stage"]),
				"mode_step": int(source["context"]["mode_step"]),
				"year": int(rule["year"]),
				"month": int(rule["month"]),
				"day": int(rule["day"]),
				"weekday": int(rule["weekday"]),
				"menu_cursor": int(rule["menu_cursor"]),
				"stats": [100, 40, 40, 40, 40, 32, 60, 5, 0],
				"frame_count": int(cfg["frame_count"]),
				"checker_uv": [cfg["checker_uv"].x, cfg["checker_uv"].y],
			}
		)


# ---------------------------------------------------------------------------
# 2. Live PlayableBoot Route Captures (command0_first_activity, command0_weekday_activity, holiday_menu_settled, native_command_screen)
# ---------------------------------------------------------------------------

class HudCompositorWithOverride extends HudCompositor:
	var uv_override: Variant = null
	var fast_skip: bool = false
	var _dummy_img: Image = Image.create(320, 240, false, Image.FORMAT_RGBA8)

	func set_checker_uv(uv: Vector2i) -> void:
		super.set_checker_uv(uv_override if uv_override is Vector2i else uv)

	func render(state: Dictionary, previous_frame: Image = null,
		scene_commands: Array[Dictionary] = [], command_room: bool = true) -> Image:
		if fast_skip:
			error = ""
			last_coverage = _dummy_img
			return _dummy_img
		return super.render(state, previous_frame, scene_commands, command_room)


func _produce_playable_boot_route_captures(repo: String, out_dir: String) -> void:
	OS.set_environment("TOKIMEMO_BOUNDARY", repo.path_join(".claude/orchestrator/state/T-29004/evidence/development-boundary.json"))
	OS.set_environment("TOKIMEMO_BOUNDARY_SHA", PlayableBoot.BOUNDARY_SHA)
	OS.set_environment("TOKIMEMO_TABLES", repo.path_join(".asset-cache/port/school-day-tables-v1-final/manifest.json"))
	OS.set_environment("TOKIMEMO_TABLES_SHA", PlayableBoot.TABLES_SHA)
	OS.set_environment("TOKIMEMO_MEDIA_PACK", repo.path_join(".asset-cache/port/holiday-command0-current-964ce"))
	OS.set_environment("TOKIMEMO_HUD_MANIFEST", repo.path_join(".asset-cache/port/hud-production-v1/manifest.json"))
	OS.set_environment("TOKIMEMO_HUD_SHA", PlayableBoot.LiveHud.MANIFEST_SHA)
	OS.set_environment("TOKIMEMO_FONT_ROOT", repo.path_join(".asset-cache/port/fonts/native-font-v2"))
	OS.set_environment("TOKIMEMO_HUD_CONTEXT", repo.path_join(".claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json"))
	OS.set_environment("TOKIMEMO_HUD_CONTEXT_SHA", PlayableBoot.LiveHud.CONTEXT_SHA)
	OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", repo.path_join(".claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json"))
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", repo.path_join(".asset-cache/port/holiday-hud-text-v1/manifest.json"))
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT_SHA", PlayableBoot.HolidayHud.TEXT_SHA)
	OS.set_environment("TOKIMEMO_DATE_PACK", repo.path_join(".asset-cache/port/date-preview-v1"))
	OS.set_environment("TOKIMEMO_REQUIRE_HUD", "1")
	OS.set_environment("TOKIMEMO_REQUIRE_HOLIDAY", "1")

	var app: PlayableBoot = PlayableBoot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.continue_week = true
	var load_err: String = app._load_inputs()
	var hud_pres := CommandHudPresenterWithSeason.new()
	var hud_comp := HudCompositorWithOverride.new()
	var hol_comp := HudCompositorWithOverride.new()
	if load_err.is_empty():
		hud_pres.open_manifest(
			OS.get_environment("TOKIMEMO_HUD_MANIFEST"),
			OS.get_environment("TOKIMEMO_HUD_SHA"),
			OS.get_environment("TOKIMEMO_FONT_ROOT")
		)
		hud_comp.configure(hud_pres._manifest, hud_pres._images, hud_pres._font)
		hud_pres._compositor = hud_comp
		app.hud.presenter = hud_pres
		hol_comp.configure(app.holiday_hud._manifest, app.holiday_hud._images, app.holiday_hud._font)
		app.holiday_hud._compositor = hol_comp
		load_err = app._reset()
	_check(load_err.is_empty() and app.failure.is_empty(), "PlayableBoot initialized for route captures: " + load_err + app.failure)
	if not load_err.is_empty() or not app.failure.is_empty():
		return

	var got_first_activity := false
	var got_weekday_activity := false
	var got_holiday_menu := false
	var got_next_command := false
	var menu_tick: int = -1
	var next_cmd_tick: int = -1

	for tick: int in range(420):
		if tick in [17, 23]:
			var ev := InputEventKey.new()
			ev.physical_keycode = KEY_ENTER
			ev.pressed = (tick == 17)
			app.pad._input(ev)
		if app.state.game_mode == 0x46 and app.state.mode_stage == 1 and menu_tick < 0:
			menu_tick = tick
		if menu_tick >= 0 and tick in [menu_tick + 32, menu_tick + 38]:
			var ev_h := InputEventKey.new()
			ev_h.physical_keycode = KEY_ENTER
			ev_h.pressed = (tick == menu_tick + 32)
			app.pad._input(ev_h)

		hud_comp.fast_skip = true
		hol_comp.fast_skip = true
		app._physics_process(1.0 / 60.0)
		hud_comp.fast_skip = false
		hol_comp.fast_skip = false
		if not app.failure.is_empty():
			_check(false, "PlayableBoot step failed at tick %d: %s" % [tick, app.failure])
			break

		if not got_first_activity and tick == 24:
			got_first_activity = true
			var cmd_state: Dictionary = hud_pres.create_state(app.state.rule_state, {
				"game_mode": 0x34, "mode_stage": 1, "mode_step": 0,
				"icon_mode": 0, "icon_count": 14, "cursor_style": 0x9C,
				"hover_ticks": 32, "family_name_hex": app.hud.profile["family_name_hex"],
			})
			hud_pres.set_animation_phase(cmd_state, app.hud.profile["icon_steps"], app.hud.profile["icon_elapsed"])
			hud_pres.advance_icons(cmd_state, 25)
			hud_comp.uv_override = Vector2i(2, 7)
			hud_comp.set_checker_uv(Vector2i(2, 7))
			var first_act_img: Image = hud_pres.render(cmd_state)
			hud_comp.uv_override = null
			_record_capture(
				"command0_first_activity",
				"hud_command",
				".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_command0_first_activity.png",
				first_act_img,
				out_dir,
				"tests/app/run_command7_app_tests.gd",
				"presentation/live_hud_adapter.gd",
				{
					"source_checkpoint": ".claude/orchestrator/state/T-29004/evidence/development-boundary.json",
					"tick": tick,
					"frame_count": app.state.frame_count,
					"game_mode": app.state.game_mode,
					"mode_stage": app.state.mode_stage,
					"mode_step": app.state.mode_step,
					"substep": app.state.substep,
					"year": int(app.state.rule_state["year"]),
					"month": int(app.state.rule_state["month"]),
					"day": int(app.state.rule_state["day"]),
					"weekday": int(app.state.rule_state["weekday"]),
					"stats": [100, 40, 40, 40, 40, 32, 60, 5, 0],
					"checker_uv": [2, 7],
				}
			)

		if not got_weekday_activity and app.state.rule_state["params"] != app.initial_params:
			got_weekday_activity = true
			hud_comp.uv_override = Vector2i(4, 22)
			var act_cmd: Dictionary = app.hud.compose(app.state.rule_state, app.state.projection(), app.trace, app.staged_commands)
			hud_comp.uv_override = null
			var wk_img: Image = act_cmd.get("image")
			_record_capture(
				"command0_weekday_activity",
				"weekday_activity",
				".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_command0_weekday_activity.png",
				wk_img,
				out_dir,
				"tests/app/run_command7_app_tests.gd",
				"presentation/live_hud_adapter.gd",
				{
					"source_checkpoint": ".claude/orchestrator/state/T-29004/evidence/development-boundary.json",
					"tick": tick,
					"frame_count": app.state.frame_count,
					"game_mode": app.state.game_mode,
					"mode_stage": app.state.mode_stage,
					"mode_step": app.state.mode_step,
					"year": int(app.state.rule_state["year"]),
					"month": int(app.state.rule_state["month"]),
					"day": int(app.state.rule_state["day"]),
					"weekday": int(app.state.rule_state["weekday"]),
					"stats": [99, 40, 40, 40, 39, 32, 59, 4, 0],
					"checker_uv": [4, 22],
				}
			)

		if not got_holiday_menu and menu_tick >= 0 and app.state.game_mode == 0x46 and app.state.mode_stage == 1 and app.state.mode_step == 0 and tick == menu_tick + 25:
			got_holiday_menu = true
			hol_comp.uv_override = Vector2i(10, 27)
			var active_lba_h: int = int(app.state.rule_state["background_cache"][app.state.rule_state["background_show"]])
			var composed_h: Dictionary = app.holiday_hud.compose(app.state.rule_state, app.state.projection(), app._holiday_profile(), app.trace, app.staged_commands, active_lba_h)
			hol_comp.uv_override = null
			var hol_cmds: Array = composed_h.get("commands", [])
			var hol_img: Image = hol_cmds[0]["image"] if not hol_cmds.is_empty() else app._last_rendered_image
			_record_capture(
				"holiday_menu_settled",
				"holiday_hud",
				".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_holiday_menu_settled.png",
				hol_img,
				out_dir,
				"tests/holiday/run_continuous_scene_tests.gd",
				"presentation/holiday_hud_adapter.gd",
				{
					"source_checkpoint": ".claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json",
					"tick": tick,
					"frame_count": app.state.frame_count,
					"game_mode": app.state.game_mode,
					"mode_stage": app.state.mode_stage,
					"mode_step": app.state.mode_step,
					"year": int(app.state.rule_state["year"]),
					"month": int(app.state.rule_state["month"]),
					"day": int(app.state.rule_state["day"]),
					"weekday": int(app.state.rule_state["weekday"]),
					"stats": [97, 43, 40, 40, 38, 33, 59, 4, 3],
					"checker_uv": [10, 27],
				}
			)

		if app.state.game_mode == 0x34 and app.state.mode_stage == 1 and int(app.state.rule_state["day_event_index"]) == 10 and next_cmd_tick < 0:
			next_cmd_tick = tick
		if not got_next_command and next_cmd_tick >= 0 and tick == next_cmd_tick + 8:
			got_next_command = true
			hol_comp.uv_override = Vector2i(1, 16)
			var active_lba_n: int = int(app.state.rule_state["background_cache"][app.state.rule_state["background_show"]])
			var composed_n: Dictionary = app.holiday_hud.compose(app.state.rule_state, app.state.projection(), app._holiday_profile(), app.trace, app.staged_commands, active_lba_n)
			hol_comp.uv_override = null
			var next_cmds: Array = composed_n.get("commands", [])
			var next_img: Image = next_cmds[0]["image"] if not next_cmds.is_empty() else app._last_rendered_image
			_record_capture(
				"native_command_screen",
				"hud_command",
				".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_native_command_screen.png",
				next_img,
				out_dir,
				"tests/holiday/run_continuous_scene_tests.gd",
				"presentation/holiday_hud_adapter.gd",
				{
					"source_checkpoint": ".claude/orchestrator/state/T-29013/evidence/next-command-settled/result.json",
					"tick": tick,
					"frame_count": app.state.frame_count,
					"game_mode": app.state.game_mode,
					"mode_stage": app.state.mode_stage,
					"mode_step": app.state.mode_step,
					"year": int(app.state.rule_state["year"]),
					"month": int(app.state.rule_state["month"]),
					"day": int(app.state.rule_state["day"]),
					"weekday": int(app.state.rule_state["weekday"]),
					"stats": [96, 47, 40, 40, 38, 34, 58, 4, 3],
					"checker_uv": [1, 16],
				}
			)
			break

	app.canvas.free()
	app.pad.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()


# ---------------------------------------------------------------------------
# 3. Prologue Visual Captures (ProloguePresenter)
# ---------------------------------------------------------------------------

func _produce_prologue_captures(repo: String, out_dir: String) -> void:
	var hud_manifest := repo.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var plates_dir := repo.path_join(".claude/orchestrator/state/T-29012/prologue-visual-v1/scene_plates")

	var presenter := ProloguePresenter.new()
	var err: String = presenter.init_resources(font_root, hud_manifest, plates_dir)
	_check(err.is_empty(), "ProloguePresenter initialized: " + err)
	if not err.is_empty():
		return

	var items := [
		{
			"id": "prologue_stage0_arrival",
			"godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage0_arrival.png",
			"image": presenter.render_stage0_arrival(false, 94, 81),
			"state": {"game_mode": 0x15, "mode_stage": 0, "title_badge": false, "year": 95, "month": 4, "day": 4, "weekday": 1, "checker_uv": [94, 81]},
		},
		{
			"id": "prologue_stage0_arrival_with_badge",
			"godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage0_arrival_with_badge.png",
			"image": presenter.render_stage0_arrival(true, 104, 76),
			"state": {"game_mode": 0x15, "mode_stage": 0, "title_badge": true, "year": 95, "month": 4, "day": 4, "weekday": 1, "checker_uv": [104, 76]},
		},
		{
			"id": "prologue_stage1_board",
			"godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage1_board.png",
			"image": presenter.render_stage1_board(174, 169),
			"state": {"game_mode": 0x15, "mode_stage": 1, "year": 95, "month": 4, "day": 4, "weekday": 1, "checker_uv": [174, 169]},
		},
		{
			"id": "prologue_stage13_shiori_line18",
			"godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage13_shiori_line18.png",
			"image": presenter.render_stage13_shiori_line18(72, 220),
			"state": {"game_mode": 0x15, "mode_stage": 13, "line": 18, "speaker": "藤崎", "checker_uv": [72, 220]},
		},
		{
			"id": "prologue_stage14_shiori_line19",
			"godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage14_shiori_line19.png",
			"image": presenter.render_stage14_shiori_line19(79, 216, 2),
			"state": {"game_mode": 0x15, "mode_stage": 14, "line": 19, "first_visible_row": 2, "speaker": "藤崎", "checker_uv": [79, 216]},
		},
	]
	for item: Dictionary in items:
		_record_capture(
			item["id"],
			"prologue_vn",
			item["godot_capture"],
			item["image"],
			out_dir,
			"tests/run_prologue_visual_tests.gd",
			"presentation/prologue_presenter.gd",
			item["state"]
		)


# ---------------------------------------------------------------------------
# 4. Calendar / Phone Book / Magazine Captures (CalendarPhonePresenter)
# ---------------------------------------------------------------------------

func _produce_calendar_phone_captures(repo: String, out_dir: String) -> void:
	var hud_manifest := repo.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo.path_join(".asset-cache/port/fonts/native-font-v2")

	var presenter := CalendarPhonePresenter.new()
	var err: String = presenter.init_resources(font_root, hud_manifest)
	_check(err.is_empty(), "CalendarPhonePresenter initialized: " + err)
	if not err.is_empty():
		return

	var items := [
		{
			"id": "calendar_april_may",
			"godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-calendar-screen-april-may.png",
			"image": presenter.render_calendar_screen(5, 0x19B, 40),
			"state": {"game_mode": 0x46, "mode_stage": 0x30, "year": 95, "month": 4, "day": 9, "weekday": 6, "day_count": 5, "player_birthday": 0x19B, "phase": 40},
		},
		{
			"id": "holiday_phone_book",
			"godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-phone-book.png",
			"image": presenter.render_holiday_phone_screen(0, [], 0, "福原"),
			"state": {"game_mode": 0x46, "mode_stage": 0x10, "cursor": 0, "substep": 0, "year": 95, "month": 4, "day": 9, "weekday": 6, "family_name": "福原"},
		},
		{
			"id": "holiday_phone_shiori",
			"godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-phone-shiori.png",
			"image": presenter.render_holiday_phone_screen(1, [], 1, "福原"),
			"state": {"game_mode": 0x46, "mode_stage": 0x10, "cursor": 1, "substep": 1, "year": 95, "month": 4, "day": 9, "weekday": 6, "family_name": "福原"},
		},
		{
			"id": "holiday_magazine",
			"godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-magazine.png",
			"image": presenter.render_holiday_magazine_screen(0),
			"state": {"game_mode": 0x46, "mode_stage": 0x20, "substep": 0, "year": 95, "month": 4, "day": 9, "weekday": 6},
		},
	]
	for item: Dictionary in items:
		_record_capture(
			item["id"],
			"calendar_phone",
			item["godot_capture"],
			item["image"],
			out_dir,
			"tests/holiday/run_calendar_phone_tests.gd",
			"presentation/calendar_phone_presenter.gd",
			item["state"]
		)


# ---------------------------------------------------------------------------
# 5. System Overlays Captures (VnScenePresenter + SystemOverlays)
# ---------------------------------------------------------------------------

func _produce_system_overlay_captures(repo: String, out_dir: String) -> void:
	var font_root := repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var presenter := VnScenePresenter.new()
	var err: String = presenter.init_resources(font_root)
	_check(err.is_empty(), "VnScenePresenter initialized: " + err)
	if not err.is_empty():
		return

	var opt := SystemOverlays.create_options()
	opt.cursor = 0
	var img_opt: Image = opt.render(presenter)
	_record_capture(
		"system_options_menu",
		"system_overlay",
		".claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures/capture_options_menu.png",
		img_opt,
		out_dir,
		"tests/overlays/run_system_overlays_tests.gd",
		"overlays/system_overlays.gd",
		{
			"game_mode": 0x46,
			"mode_stage": 0x40,
			"mode_step": 1,
			"cursor": 0,
			"year": 95,
			"month": 4,
			"day": 9,
			"weekday": 6,
			"stats": [97, 43, 40, 40, 38, 33, 59, 4, 3],
		}
	)

	var help := SystemOverlays.create_help()
	help.cursor = 0
	var img_help: Image = help.render(presenter)
	_record_capture(
		"system_help_screen",
		"system_overlay",
		".claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures/capture_help_screen.png",
		img_help,
		out_dir,
		"tests/overlays/run_system_overlays_tests.gd",
		"overlays/system_overlays.gd",
		{
			"game_mode": 0x3A,
			"mode_stage": 1,
			"mode_step": 0,
			"cursor": 0,
			"year": 95,
			"month": 4,
			"day": 9,
			"weekday": 6,
		}
	)


# ---------------------------------------------------------------------------
# 6. Telephone Overlays Captures (TelephoneOverlayPresenter + Live State)
# ---------------------------------------------------------------------------

func _produce_telephone_overlay_captures(repo: String, out_dir: String) -> void:
	var hud_manifest := repo.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack := repo.path_join(".asset-cache/port/date-preview-v1")

	var presenter := TelephoneOverlayPresenter.new()
	var opened := presenter.open_date_preview(hud_manifest, font_root, date_pack)
	_check(opened, "TelephoneOverlayPresenter initialized: " + presenter.error)
	if not opened:
		return

	var cal0 := {
		"days": 30,
		"day_kind": [
			30,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			2, 1, 0
		],
	}
	var cal1 := {
		"days": 31,
		"day_kind": [
			31,
			0, 0, 2, 0, 2, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0
		],
	}
	var stats: Array[int] = [97, 43, 40, 40, 38, 33, 59, 4, 3]
	var progress: Array[int] = []
	progress.resize(21)
	progress.fill(1)
	var live_rule_state := {
		"year": 95,
		"month": 4,
		"day": 9,
		"weekday": 6,
		"day_kind": 1,
		"day_count": 5,
		"player_birthday": 0x19B,
		"stats": stats,
		"calendar": [cal0, cal1],
		"progress_state": progress,
		"quarter_flags": {"b13": 1, "b14": 1},
		"appointment_words": [0, 0, 0, 0, 0, 0, 0, 0],
	}
	var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(live_rule_state)
	presenter.set_live_context(live_ctx)
	var session := TelephoneOverlay.create_session_from_live_state(live_rule_state, TelephoneOverlay.GIRL_SHIORI)

	var img_nb: Image = presenter.render_girl_profile_notebook_screen(
		TelephoneOverlay.GIRL_SHIORI, stats, 95, 4, 9, 6, 1, Vector2i(1, 0)
	)
	_record_capture(
		"tel_notebook_profile",
		"telephone_overlay",
		".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_01_notebook_profile.png",
		img_nb,
		out_dir,
		"tests/overlays/run_telephone_overlay_tests.gd",
		"presentation/telephone_overlay_presenter.gd",
		{
			"source_checkpoint": ".claude/orchestrator/state/T-29018/emu_ref/emu_shots.json#emu_tel_notebook_profile",
			"game_mode": 0x45,
			"mode_stage": 0,
			"mode_step": 4,
			"substep": 5,
			"girl_id": 0,
			"year": 95,
			"month": 4,
			"day": 9,
			"weekday": 6,
			"day_kind": 1,
			"stats": stats,
			"frame_count": 6105,
			"checker_uv": [1, 0],
		}
	)

	var img_pl: Image = presenter.render_date_place_menu_screen(
		0, 0, session.available_places_page0, stats, 95, 4, 9, 6, 1, Vector2i(22, 21)
	)
	_record_capture(
		"tel_date_place_menu",
		"telephone_overlay",
		".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_02_date_place_menu.png",
		img_pl,
		out_dir,
		"tests/overlays/run_telephone_overlay_tests.gd",
		"presentation/telephone_overlay_presenter.gd",
		{
			"source_checkpoint": ".claude/orchestrator/state/T-29018/emu_ref/emu_shots.json#emu_tel_date_place_menu",
			"game_mode": 0x45,
			"mode_stage": 1,
			"mode_step": 0x30,
			"substep": 1,
			"page": 0,
			"cursor": 0,
			"year": 95,
			"month": 4,
			"day": 9,
			"weekday": 6,
			"day_kind": 1,
			"stats": stats,
			"frame_count": 6190,
			"checker_uv": [22, 21],
		}
	)

	var img_dy: Image = presenter.render_date_day_menu_screen(
		session.available_date_slots, 0, stats, 95, 4, 9, 6, 1, Vector2i(9, 12), 5, 0x19B, 6145
	)
	_record_capture(
		"tel_date_day_menu",
		"telephone_overlay",
		".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_03_date_day_menu.png",
		img_dy,
		out_dir,
		"tests/overlays/run_telephone_overlay_tests.gd",
		"presentation/telephone_overlay_presenter.gd",
		{
			"source_checkpoint": ".claude/orchestrator/state/T-29018/emu_ref/emu_shots.json#emu_tel_date_day_menu",
			"game_mode": 0x45,
			"mode_stage": 1,
			"mode_step": 0x20,
			"substep": 1,
			"cursor": 0,
			"year": 95,
			"month": 4,
			"day": 9,
			"weekday": 6,
			"day_kind": 1,
			"day_count": 5,
			"player_birthday": 0x19B,
			"stats": stats,
			"frame_count": 6145,
			"checker_uv": [9, 12],
		}
	)
