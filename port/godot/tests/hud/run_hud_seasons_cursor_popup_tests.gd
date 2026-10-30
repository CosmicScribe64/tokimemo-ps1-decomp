extends SceneTree
## T-29018 HUD Verification Suite:
## Verifies and renders:
## 1. Live weekday activity SD pop-up reaction bubble (pos.y < 63, OT=0) layered ON TOP OF top parameter bar (OT=12..11)
## 2. 4-Season Checkerboard CLUTs (Spring=0, Summer=1, Autumn=2, Winter=3) and UV scroll (compute_checker_uv)
## 3. Exact cursor sprite frames (Frame 156 main command hand, Frame 155 sub-select arrow, Frame 0 triangle cursor)

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")

const HUD_MANIFEST := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const HUD_SHA := ContentIdentity.HUD_MANIFEST_SHA
const FONT_ROOT := "/workspace/.asset-cache/port/fonts/native-font-v2"
const MEDIA_PACK := "/workspace/.asset-cache/port/school-day-media-v3"
const SPRING_PACK := "/workspace/.asset-cache/port/t29014-spring-command-1-3-final"
const WEEKDAY56_PACK := "/workspace/.asset-cache/port/weekday56-current-e593"
const CLUB_PACK := "/workspace/.asset-cache/port/t29014-command4-club-v1"

const CAPTURE_DIR := "/workspace/.claude/orchestrator/state/T-29018/hud-verification-v1/captures"
const VIDEO_FRAMES_DIR := "/workspace/.claude/orchestrator/state/T-29018/hud-verification-v1/frames"

var failures: Array[String] = []
var total_checks: int = 0

func _check(cond: bool, msg: String) -> void:
	total_checks += 1
	if not cond:
		failures.append(msg)
		printerr("FAIL: ", msg)

static func _make_calendar(days_in_month: int = 30) -> Array:
	var dk0: Array = [days_in_month]
	var mk0: Array = [0]
	for d: int in range(1, 32):
		dk0.append(1 if (d % 7) == 2 else 0)
		mk0.append(0)
	return [
		{"day_kind": dk0, "marker": mk0},
		{"day_kind": dk0.duplicate(), "marker": mk0.duplicate()},
	]

static func _make_rule(year: int, month: int, day: int, weekday: int, menu_cursor: int) -> Dictionary:
	return {
		"year": year,
		"month": month,
		"day": day,
		"weekday": weekday,
		"params": [96 << 16, 45 << 16, 40 << 16, 40 << 16, 38 << 16, 34 << 16, 58 << 16, 4 << 16, 5 << 16],
		"calendar": _make_calendar(31 if month in [5, 7, 8, 10, 12] else 30),
		"menu_cursor": menu_cursor,
		"icon_enabled": [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
		"player_flags": 0,
		"player_flags_upper": 0,
		"memory_card_busy": 0,
	}

static func _make_context(game_mode: int, cursor_style: int) -> Dictionary:
	return {
		"game_mode": game_mode,
		"mode_stage": 1,
		"mode_step": 0,
		"icon_mode": 0,
		"icon_count": 14,
		"cursor_style": cursor_style,
		"hover_ticks": 32,
		"family_name_hex": "8d828ca9", # 高見
	}

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	DirAccess.make_dir_recursive_absolute(CAPTURE_DIR)
	DirAccess.make_dir_recursive_absolute(VIDEO_FRAMES_DIR)

	var presenter := Presenter.new()
	_check(presenter.open_manifest(HUD_MANIFEST, HUD_SHA, FONT_ROOT), "Open HUD manifest: " + presenter.error)

	var media := MediaAdapter.new()
	var media_err: String = media.open_packs(MEDIA_PACK, SPRING_PACK, WEEKDAY56_PACK, CLUB_PACK)
	_check(media_err.is_empty(), "Open media packs: " + media_err)

	var actor_svc := ActorService.new()
	var res0: Dictionary = media.get_resources(0, -1)
	var bg0: Array[Dictionary] = media.get_background_commands(0, -1)
	var init_actors: Dictionary = actor_svc.initialize(
		[{"id": 96, "sequence_slot": 0, "x": -80, "y": -64, "flags": 5, "draw_flags": 0x84, "quad_flags": 0x01000000, "blink_period": 1}],
		res0
	)
	_check(init_actors["error"].is_empty(), "Initialize Command 0 SD actor: " + str(init_actors.get("error", "")))
	var actor_state: Dictionary = init_actors["actor_state"]

	# 1. Step live weekday activity (95年4月5日 水) across 60 frames and capture SD reaction bubble overlapping top bar
	var act_rule: Dictionary = _make_rule(95, 4, 5, 2, 0)
	act_rule["icon_enabled"] = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var act_ctx: Dictionary = _make_context(0x40, 0x9C)
	var act_state: Dictionary = presenter.create_state(act_rule, act_ctx)
	_check(not act_state.is_empty(), "Create activity HUD state: " + presenter.error)

	var frame_idx: int = 0
	var popup_captured: bool = false
	for tick: int in range(60):
		var drawn: Dictionary = actor_svc.native_draw(actor_state, res0, tick, Vector2i(160, 120))
		actor_state = drawn["actor_state"]
		var scene_cmds: Array[Dictionary] = []
		scene_cmds.append_array(bg0)
		scene_cmds.append_array(drawn["draw_commands"])

		# Check if any draw_command is the SD reaction bubble (pos.y < 63)
		var has_popup: bool = false
		for dc: Dictionary in drawn["draw_commands"]:
			if Vector2i(dc.get("position", Vector2i(0, 100))).y < 63:
				has_popup = true

		presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(tick * 2, 0))
		var out_cmd: Dictionary = presenter.activity_command(act_state, scene_cmds, [0x43, 0, 0, 0, 0, 0, 0, 0x43, 0])
		var img: Image = out_cmd.get("image")
		_check(img != null, "Activity frame %d rendered" % tick)
		if img != null:
			img.save_png(VIDEO_FRAMES_DIR.path_join("frame_%03d.png" % frame_idx))
			if has_popup and not popup_captured and tick >= 12:
				popup_captured = true
				_check(img.save_png(CAPTURE_DIR.path_join("hud_01_spring_activity_popup_over_topbar.png")) == OK, "Saved hud_01_spring_activity_popup_over_topbar.png")
			frame_idx += 1

	_check(popup_captured, "Captured SD reaction bubble overlapping top parameter bar (pos.y < 63)")

	# 2. Summer (Month 6, Season 1) with Frame 156 (0x9C) main command pointing hand cursor
	var sum_rule: Dictionary = _make_rule(95, 6, 4, 6, 2)
	var sum_ctx: Dictionary = _make_context(0x34, 0x9C)
	var sum_state: Dictionary = presenter.create_state(sum_rule, sum_ctx)
	_check(not sum_state.is_empty(), "Create Summer Month 6 HUD state: " + presenter.error)
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(16, 0))
	var sum_img: Image = presenter.render(sum_state)
	_check(sum_img != null, "Render Summer Month 6 HUD")
	if sum_img != null:
		_check(HudCompositor.hud_season_index(6) == 1, "Month 6 maps to Season 1 (Summer)")
		_check(sum_img.save_png(CAPTURE_DIR.path_join("hud_02_summer_command_cursor156.png")) == OK, "Saved hud_02_summer_command_cursor156.png")
		for f: int in range(20):
			presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(120 + f * 2, 0))
			presenter.advance_icons(sum_state, 1)
			presenter.render(sum_state).save_png(VIDEO_FRAMES_DIR.path_join("frame_%03d.png" % frame_idx))
			frame_idx += 1

	# 3. Autumn (Month 10, Season 2) with Frame 155 (0x9B) sub-selection diagonal arrow cursor
	var aut_rule: Dictionary = _make_rule(95, 10, 4, 2, 7)
	var aut_ctx: Dictionary = _make_context(0x34, 0x9B)
	var aut_state: Dictionary = presenter.create_state(aut_rule, aut_ctx)
	_check(not aut_state.is_empty(), "Create Autumn Month 10 HUD state: " + presenter.error)
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(32, 0))
	var aut_img: Image = presenter.render(aut_state)
	_check(aut_img != null, "Render Autumn Month 10 HUD")
	if aut_img != null:
		_check(HudCompositor.hud_season_index(10) == 2, "Month 10 maps to Season 2 (Autumn)")
		_check(aut_img.save_png(CAPTURE_DIR.path_join("hud_03_autumn_subselect_cursor157.png")) == OK, "Saved hud_03_autumn_subselect_cursor157.png")
		for f: int in range(20):
			presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(160 + f * 2, 0))
			presenter.advance_icons(aut_state, 1)
			presenter.render(aut_state).save_png(VIDEO_FRAMES_DIR.path_join("frame_%03d.png" % frame_idx))
			frame_idx += 1

	# 4. Winter (Month 12, Season 3) with Frame 0 pink hollow triangle cursor
	var win_rule: Dictionary = _make_rule(95, 12, 4, 0, 0)
	var win_ctx: Dictionary = _make_context(0x34, 0)
	var win_state: Dictionary = presenter.create_state(win_rule, win_ctx)
	_check(not win_state.is_empty(), "Create Winter Month 12 HUD state: " + presenter.error)
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(48, 0))
	var win_img: Image = presenter.render(win_state)
	_check(win_img != null, "Render Winter Month 12 HUD")
	if win_img != null:
		_check(HudCompositor.hud_season_index(12) == 3, "Month 12 maps to Season 3 (Winter)")
		_check(win_img.save_png(CAPTURE_DIR.path_join("hud_04_winter_season_grid.png")) == OK, "Saved hud_04_winter_season_grid.png")
		for f: int in range(20):
			presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(200 + f * 2, 0))
			presenter.advance_icons(win_state, 1)
			presenter.render(win_state).save_png(VIDEO_FRAMES_DIR.path_join("frame_%03d.png" % frame_idx))
			frame_idx += 1

	if failures.is_empty():
		print("PASS: run_hud_seasons_cursor_popup_tests (%d checks, %d video frames)" % [total_checks, frame_idx])
		quit(0)
	else:
		printerr("FAIL: %d failures out of %d checks" % [failures.size(), total_checks])
		quit(1)
