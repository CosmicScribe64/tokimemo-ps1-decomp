extends SceneTree
## T-29013 / T-29010 / T-29006: Joined Sunday app integration, HUD composition, and save/restore proof.

const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const Save = preload("res://services/save_service.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const SceneAdapter = preload("res://services/holiday_scene_adapter.gd")

const EXPECTED_ROUTE_SHA: String = "2efddb9089659d2cbf0ecd27e73c744fe56d2536b835ac1bee430b25f8403c23"

var checks: Array[String] = []
var failures: Array[String] = []
var apps: Array[Boot] = []
var frame_hashes: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func check(ok: bool, label: String) -> void:
	checks.append(label)
	if not ok:
		failures.append(label)

func make_app(enable_holiday: bool = true) -> Boot:
	var saved_ext: String = OS.get_environment("TOKIMEMO_HOLIDAY_EXTENSION")
	var saved_txt: String = OS.get_environment("TOKIMEMO_HOLIDAY_TEXT")
	var saved_req: String = OS.get_environment("TOKIMEMO_REQUIRE_HOLIDAY")
	if not enable_holiday:
		OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", "")
		OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", "")
		OS.set_environment("TOKIMEMO_REQUIRE_HOLIDAY", "")
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.continue_week = true
	var err: String = app._load_inputs()
	if err.is_empty():
		err = app._reset()
	OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", saved_ext)
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", saved_txt)
	OS.set_environment("TOKIMEMO_REQUIRE_HOLIDAY", saved_req)
	check(err.is_empty(), "app preflight/reset (%s): %s" % ["holiday" if enable_holiday else "weekday", err])
	apps.append(app)
	return app

func pixels(app: Boot) -> String:
	var draws: Array = app.canvas.get("_commands")
	if draws.size() != 1:
		return ""
	var textures: Dictionary = app.canvas.get("_textures")
	if not textures.has(draws[0]["id"]):
		return ""
	var image: Image = (textures[draws[0]["id"]] as ImageTexture).get_image()
	var ctx: HashingContext = HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	ctx.update(image.get_data())
	return ctx.finish().hex_encode()

func feed_sunday_recipe(app: Boot, tick: int) -> void:
	if tick in [17, 23, 278, 284]:
		var event := InputEventKey.new()
		event.physical_keycode = KEY_ENTER
		event.pressed = tick in [17, 278]
		app.pad._input(event)

func step_app(app: Boot, rendered: int = 0, with_recipe: bool = true) -> Array:
	var tick: int = app.session.clock.logic_iterations
	if with_recipe:
		feed_sunday_recipe(app, tick)
	app.session.clock.rendered_frames += rendered
	var before: int = app.trace.size()
	app._physics_process(1.0 / 60.0)
	if app.trace.size() != before + 1 or not app.failure.is_empty():
		check(false, "step failed at tick %d: %s" % [tick, app.failure])
		return []
	var row: Dictionary = app.trace.back()
	return [row["state_hash"], row["event_hash"]]

func advance_until_tick(app: Boot, target_logic: int, rendered: int = 0) -> Array:
	var hashes: Array = []
	while app.session.clock.logic_iterations < target_logic:
		var pair: Array = step_app(app, rendered, true)
		if pair.is_empty():
			break
		hashes.append(pair)
	return hashes

func _run() -> void:
	var out_dir: String = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	var versions: Dictionary = {
		"source_version": "t29013-app-integration-v2",
		"content_version": JSON.stringify([
			OS.get_environment("TOKIMEMO_BOUNDARY_SHA"),
			OS.get_environment("TOKIMEMO_TABLES_SHA"),
			FileAccess.get_sha256(OS.get_environment("TOKIMEMO_MEDIA_PACK").path_join("manifest.json")),
		]).sha256_text(),
		"behavior_version": "sunday-command0-joined-v2",
	}
	var adapter: Adapter = Adapter.new(versions)
	var saves: Save = Save.new(out_dir.path_join("sunday-saves"))

	_test_preflight_and_staging_negatives()

	# 1. Ordinary weekday app invariance when holiday env vars are unset.
	var weekday_app: Boot = make_app(false)
	check(weekday_app.holiday_controller == null and weekday_app.holiday_scene == null and weekday_app.holiday_hud == null,
		"weekday-only launch leaves holiday capabilities null")
	var weekday_hashes: Array = []
	for i: int in range(300):
		var tick: int = weekday_app.session.clock.logic_iterations
		if tick in [17, 23]:
			var ev := InputEventKey.new()
			ev.physical_keycode = KEY_ENTER
			ev.pressed = tick == 17
			weekday_app.pad._input(ev)
		var before_size: int = weekday_app.trace.size()
		weekday_app._physics_process(1.0 / 60.0)
		if weekday_app.trace.size() == before_size + 1:
			var row: Dictionary = weekday_app.trace.back()
			weekday_hashes.append([row["state_hash"], row["event_hash"]])
		if weekday_app.stopped:
			break
	check(weekday_hashes.size() == 243 and weekday_app.stopped and weekday_app.failure.is_empty()
		and weekday_app.supported_stop.get("error") == Boot.HOLIDAY_STOP,
		"weekday-only launch preserves 243-step study week and explicit Sunday refusal")

	# 2. Continuous 355-step route across render schedules 0, 1, 3 with real LiveHud + HolidayHud.
	var ref_hashes: Array = []
	for rendered: int in [0, 1, 3]:
		var app: Boot = make_app(true)
		check(app.holiday_controller != null and app.holiday_scene != null and app.holiday_hud != null,
			"holiday launch opens HolidayController, HolidaySceneAdapter, and HolidayHud (schedule %d)" % rendered)
		var hashes: Array = []
		var pre_sunday_pixels: String = ""
		var retain_verified: bool = false
		var room_16318_verified: bool = false
		var actor96_verified: bool = false
		for step_idx: int in range(355):
			var tick: int = app.session.clock.logic_iterations
			if tick == 242:
				var pair_242: Array = step_app(app, rendered, true)
				hashes.append(pair_242)
				pre_sunday_pixels = pixels(app)
				continue
			var pair: Array = step_app(app, rendered, true)
			if pair.is_empty():
				break
			hashes.append(pair)
			if tick == 243:
				retain_verified = (pixels(app) == pre_sunday_pixels and not pre_sunday_pixels.is_empty())
			elif tick == 245:
				var p245: String = pixels(app)
				room_16318_verified = (not p245.is_empty() and p245 != pre_sunday_pixels
					and app.state.rule_state["icon_enabled"].slice(0, 14) == [1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0])
				if rendered == 0:
					frame_hashes["sunday_command_16318"] = p245
			elif tick == 284:
				var p284: String = pixels(app)
				actor96_verified = (not p284.is_empty() and app.state.rule_state["actor_state"].has("96")
					and app.state.rule_state["icon_enabled"].slice(0, 14) == [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
				if rendered == 0:
					frame_hashes["sunday_activity_16379_actor96"] = p284
			check(app.canvas.get("_textures").size() == 1, "bounded single texture on tick %d (schedule %d)" % [tick, rendered])
		var final_pixels: String = pixels(app)
		check(hashes.size() == 355, "355 committed steps completed (schedule %d)" % rendered)
		check(retain_verified, "tick 243 holiday entry retains committed Saturday frame (schedule %d)" % rendered)
		check(room_16318_verified, "tick 245 presents Sunday room 16318 and locks icons 9/13 (schedule %d)" % rendered)
		check(actor96_verified, "tick 284 presents Sunday activity 16379 with actor96 and solo icon 0 (schedule %d)" % rendered)
		check([app.state.game_mode, app.state.mode_stage, app.state.mode_step] == [0x34, 1, 0]
			and app.state.rule_state["day_event_index"] == 10
			and app.state.rule_state["background_cache"].slice(0, 2) == [16379, 16361]
			and app.state.rule_state["background_show"] == 1
			and app.state.rule_state["icon_enabled"].slice(0, 14) == [1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1],
			"tick 354 reaches returned next-week command 0x34 with room 16361 and weekday locks (schedule %d)" % rendered)
		if rendered == 0:
			ref_hashes = hashes
			frame_hashes["returned_command_16361"] = final_pixels
			var route_sha: String = JSON.stringify(ref_hashes).sha256_text()
			check(route_sha == EXPECTED_ROUTE_SHA, "355-step route SHA matches pinned oracle: " + route_sha)
		else:
			check(hashes == ref_hashes and final_pixels == frame_hashes["returned_command_16361"],
				"render schedule %d matches schedule 0 state/event hashes and final pixels" % rendered)

	# 3. Save/restore across all 5 boundaries (April 4 command, first_result, mid_week, Sunday command 0x46, post-Sunday command 0x34).
	_test_boundary_saves(adapter, saves, ref_hashes)

	# 4. BoundarySaveControls (F5/F9) at Sunday 0x46 command and post-Sunday 0x34 command, plus transactional rollback.
	_test_save_controls_and_rollback(adapter, ref_hashes)

	_finish(out_dir)

func _test_preflight_and_staging_negatives() -> void:
	var saved_ext: String = OS.get_environment("TOKIMEMO_HOLIDAY_EXTENSION")
	var saved_txt: String = OS.get_environment("TOKIMEMO_HOLIDAY_TEXT")
	var saved_hud: String = OS.get_environment("TOKIMEMO_HUD_MANIFEST")

	# Missing holiday text when holiday extension is set must fail preflight.
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", "/nonexistent/holiday-text.json")
	var bad_txt_app: Boot = Boot.new()
	var err1: String = bad_txt_app._load_inputs()
	check(not err1.is_empty() and bad_txt_app.holiday_controller == null and bad_txt_app.holiday_scene == null and bad_txt_app.holiday_hud == null,
		"bad holiday text path rejects preflight and leaves holiday fields null")
	bad_txt_app.free()
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", saved_txt)

	# Project-local res:// or double-slash path for holiday extension must fail preflight.
	for bad_path: String in ["res://app/playable_boot.gd", "//" + saved_ext.trim_prefix("/")]:
		OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", bad_path)
		var bad_ext_app: Boot = Boot.new()
		var err2: String = bad_ext_app._load_inputs()
		check(not err2.is_empty() and bad_ext_app.holiday_controller == null and bad_ext_app.holiday_scene == null,
			"non-external holiday extension path rejected: " + bad_path)
		bad_ext_app.free()
	OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", saved_ext)

	# Missing LiveHud when holiday is requested must fail preflight.
	OS.set_environment("TOKIMEMO_HUD_MANIFEST", "")
	OS.set_environment("TOKIMEMO_HUD_SHA", "")
	OS.set_environment("TOKIMEMO_FONT_ROOT", "")
	OS.set_environment("TOKIMEMO_HUD_CONTEXT", "")
	OS.set_environment("TOKIMEMO_HUD_CONTEXT_SHA", "")
	var no_hud_app: Boot = Boot.new()
	var err3: String = no_hud_app._load_inputs()
	check(err3 == "mandatory holiday preflight: complete native HUD capability is required" and no_hud_app.holiday_controller == null,
		"holiday preflight requires LiveHud capability")
	no_hud_app.free()
	OS.set_environment("TOKIMEMO_HUD_MANIFEST", saved_hud)
	OS.set_environment("TOKIMEMO_HUD_SHA", Boot.LiveHud.MANIFEST_SHA)
	OS.set_environment("TOKIMEMO_FONT_ROOT", "/runtime/.asset-cache/port/fonts/native-font-v2")
	OS.set_environment("TOKIMEMO_HUD_CONTEXT", "/workspace/.claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json")
	OS.set_environment("TOKIMEMO_HUD_CONTEXT_SHA", Boot.LiveHud.CONTEXT_SHA)

	# HolidaySceneAdapter staging negatives.
	var unready := SceneAdapter.new()
	var probe_app: Boot = make_app(true)
	var dummy_res := Session.FrameResult.new()
	check(not unready.stage(probe_app.state, 0, dummy_res)["error"].is_empty(),
		"unpreflighted HolidaySceneAdapter refuses stage()")
	check(not probe_app.holiday_scene.stage(probe_app.state, 0, dummy_res)["error"].is_empty(),
		"HolidaySceneAdapter refuses stage() outside Sunday route (April 4 0x34)")

func _test_boundary_saves(adapter: Adapter, saves: Save, ref_hashes: Array) -> void:
	var live: Boot = make_app(true)

	# Boundary 1: April 4 command (logic_iterations == 0).
	var snap_cmd0: Dictionary = adapter.capture(live, "command")
	check(snap_cmd0["error"].is_empty() and saves.save_slot("b1_command", snap_cmd0["snapshot"])["error"].is_empty(),
		"April 4 command boundary captures and saves with holiday enabled")

	# Advance to tick 80: first_result (0x31, day_event_index == 1).
	advance_until_tick(live, 80)
	check(live.state.game_mode == 0x31 and live.state.rule_state["day_event_index"] == 1, "reached first_result at tick 80")
	var snap_first: Dictionary = adapter.capture(live, "first_result")
	check(snap_first["error"].is_empty() and saves.save_slot("b2_first_result", snap_first["snapshot"])["error"].is_empty(),
		"first_result boundary captures and saves with holiday enabled")

	# Advance to tick 186: mid_week (0x31, day_event_index == 5).
	advance_until_tick(live, 186)
	check(live.state.game_mode == 0x31 and live.state.rule_state["day_event_index"] == 5, "reached mid_week index 5 at tick 186")
	var snap_mid: Dictionary = adapter.capture(live, "mid_week")
	check(snap_mid["error"].is_empty() and saves.save_slot("b3_mid_week", snap_mid["snapshot"])["error"].is_empty(),
		"mid_week boundary captures and saves with holiday enabled")

	# Advance to tick 244 (0x46 stage 0 resource wait) -> must refuse save.
	advance_until_tick(live, 244)
	check(not adapter.capture(live, "command")["error"].is_empty()
		and not adapter.capture(live, "holiday_command")["error"].is_empty(),
		"0x46 stage 0 resource wait refuses save")

	# Advance to tick 246 (after tick 245 commits 0x46 stage 1 step 0, room 16318).
	advance_until_tick(live, 246)
	var sunday_cmd_pixels: String = pixels(live)
	check([live.state.game_mode, live.state.mode_stage, live.state.mode_step] == [0x46, 1, 0]
		and live.state.rule_state["day_event_index"] == 9, "reached Sunday 0x46 command boundary at tick 246")
	var snap_sun_cmd: Dictionary = adapter.capture(live, "command")
	var snap_sun_hcmd: Dictionary = adapter.capture(live, "holiday_command")
	check(snap_sun_cmd["error"].is_empty() and snap_sun_hcmd["error"].is_empty()
		and saves.save_slot("b4_sunday_cmd", snap_sun_cmd["snapshot"])["error"].is_empty(),
		"Sunday 0x46 command boundary captures under both 'command' and 'holiday_command'")

	# Advance to tick 290 (0x46 stage 5 step 2 Sunday activity) -> must refuse save.
	advance_until_tick(live, 290)
	check(not adapter.capture(live, "command")["error"].is_empty()
		and not adapter.capture(live, "holiday_command")["error"].is_empty(),
		"0x46 stage 5 Sunday activity refuses save")

	# Advance to tick 352 (0x31 index 10 post-holiday transition) -> must refuse save.
	advance_until_tick(live, 352)
	check(not adapter.capture(live, "command")["error"].is_empty()
		and not adapter.capture(live, "next_command")["error"].is_empty(),
		"0x31 index 10 post-holiday transition refuses save")

	# Advance to tick 355 (after tick 354 commits 0x34 stage 1 step 0, index 10, room 16361).
	advance_until_tick(live, 355)
	var next_cmd_pixels: String = pixels(live)
	var snap_next_cmd: Dictionary = adapter.capture(live, "command")
	var snap_next_ncmd: Dictionary = adapter.capture(live, "next_command")
	check(snap_next_cmd["error"].is_empty() and snap_next_ncmd["error"].is_empty()
		and saves.save_slot("b5_next_cmd", snap_next_cmd["snapshot"])["error"].is_empty(),
		"April 10 returned 0x34 command boundary captures under both 'command' and 'next_command'")

	# Restore each saved slot into a fresh app, verify HUD pixels and full continuation to tick 355.
	for spec: Array in [
		["b1_command", 0],
		["b2_first_result", 80],
		["b3_mid_week", 186],
		["b4_sunday_cmd", 246],
		["b5_next_cmd", 355],
	]:
		var slot_name: String = spec[0]
		var start_tick: int = spec[1]
		var loaded: Dictionary = saves.load_slot(slot_name)
		var resumed: Boot = make_app(true)
		var restore_err: String = adapter.restore(resumed, loaded["snapshot"], resumed._present_committed)
		check(loaded["error"].is_empty() and restore_err.is_empty() and resumed.holiday_controller != null,
			"slot %s restores into fresh app with HolidayController attached: %s" % [slot_name, restore_err])
		if start_tick == 246:
			check(pixels(resumed) == sunday_cmd_pixels, "restored Sunday 0x46 command rebinds exact 16318 HUD pixels")
		elif start_tick == 355:
			check(pixels(resumed) == next_cmd_pixels, "restored April 10 0x34 command rebinds exact 16361 HUD pixels")
		var tail: Array = advance_until_tick(resumed, 355)
		check(tail == ref_hashes.slice(start_tick, 355) and pixels(resumed) == next_cmd_pixels,
			"slot %s continuation (%d steps) matches uninterrupted route hashes and final pixels" % [slot_name, 355 - start_tick])

func _test_save_controls_and_rollback(adapter: Adapter, ref_hashes: Array) -> void:
	var app: Boot = make_app(true)
	var user_dir: String = OS.get_user_data_dir().path_join("sunday-f5f9-saves")
	DirAccess.make_dir_recursive_absolute(user_dir)
	check(app.save_controls.configure(app, user_dir).is_empty(), "BoundarySaveControls configures in user_data_dir")

	# Advance to Sunday 0x46 command boundary (tick 246).
	advance_until_tick(app, 246)
	var sunday_pixels: String = pixels(app)
	var f5 := InputEventKey.new()
	f5.physical_keycode = KEY_F5
	f5.pressed = true
	check(app.save_controls.handle_input(app, f5) and app.save_controls.process_queued(app)
		and app.save_controls.last_error.is_empty() and app.save_controls.last_checkpoint == "command",
		"F5 quick-save succeeds at Sunday 0x46 command boundary")

	# Advance 30 ticks into Sunday, then press F9 to reload Sunday 0x46 command boundary.
	advance_until_tick(app, 276)
	var f9 := InputEventKey.new()
	f9.physical_keycode = KEY_F9
	f9.pressed = true
	check(app.save_controls.handle_input(app, f9) and app.save_controls.process_queued(app)
		and app.save_controls.last_error.is_empty() and app.session.clock.logic_iterations == 246
		and pixels(app) == sunday_pixels,
		"F9 quick-load restores Sunday 0x46 command boundary and 16318 HUD pixels")

	# Test transactional rollback on failed post_apply and tampered media/snapshots.
	var old_session: Session = app.session
	var old_controller: RefCounted = app.controller
	var old_holiday_controller: RefCounted = app.holiday_controller
	var old_hash: String = app.session.state_hash()
	var snap: Dictionary = adapter.capture(app, "command")["snapshot"]

	var fail_err: String = adapter.restore(app, snap, func(target: Node) -> String:
		target.get("session").state.rule_state["params"][0] = -999
		target.get("canvas").call("present", [] as Array[Dictionary])
		return "injected Sunday post_apply failure")
	check(fail_err == "injected Sunday post_apply failure"
		and is_same(app.session, old_session)
		and is_same(app.controller, old_controller)
		and is_same(app.holiday_controller, old_holiday_controller)
		and app.session.state_hash() == old_hash
		and pixels(app) == sunday_pixels,
		"failed Sunday post_apply rolls back exact session, controller, holiday_controller, state_hash, and canvas pixels")

	# Restoring a holiday snapshot into a weekday-only app (holiday_controller == null) must fail without mutation.
	var weekday_only: Boot = make_app(false)
	var wk_session: Session = weekday_only.session
	var wk_controller: RefCounted = weekday_only.controller
	check(not adapter.restore(weekday_only, snap, weekday_only._present_committed).is_empty()
		and is_same(weekday_only.session, wk_session)
		and is_same(weekday_only.controller, wk_controller)
		and weekday_only.holiday_controller == null,
		"holiday snapshot is rejected by weekday-only app without mutating session/controller")

	# Tampering with holiday draw descriptor in snapshot must fail without mutating app.
	var bad_draw: Dictionary = snap.duplicate(true)
	bad_draw["gameplay_state"]["draw"][0]["id"] = "tampered-room-id"
	check(not adapter.restore(app, bad_draw, app._present_committed).is_empty()
		and is_same(app.holiday_controller, old_holiday_controller)
		and pixels(app) == sunday_pixels,
		"tampered Sunday room draw descriptor is rejected without mutating live app")

	# Continue from the F9-reloaded Sunday boundary to tick 355 and verify F5 at April 10 returned command.
	var tail: Array = advance_until_tick(app, 355)
	check(tail == ref_hashes.slice(246, 355),
		"F9-reloaded Sunday boundary continues with identical state/event hashes to tick 355")
	check(app.save_controls.handle_input(app, f5) and app.save_controls.process_queued(app)
		and app.save_controls.last_error.is_empty() and app.save_controls.last_checkpoint == "command",
		"F5 quick-save succeeds at April 10 returned 0x34 command boundary")

func _finish(out_dir: String) -> void:
	var report: Dictionary = {
		"schema": "tokimemo.sunday-app-integration-proof.v2",
		"status": "PASS" if failures.is_empty() else "FAIL",
		"checks_count": checks.size(),
		"failures_count": failures.size(),
		"checks": checks,
		"failures": failures,
		"route_sha256": EXPECTED_ROUTE_SHA,
		"frame_hashes": frame_hashes,
	}
	if out_dir.is_absolute_path():
		DirAccess.make_dir_recursive_absolute(out_dir)
		var f: FileAccess = FileAccess.open(out_dir.path_join("sunday-app-integration-proof.json"), FileAccess.WRITE)
		if f != null:
			f.store_string(JSON.stringify(report, "\t"))
			f.close()
	for app: Boot in apps:
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
	if failures.is_empty():
		print("PASS: Sunday app integration v2; %d checks, 0 failures" % checks.size())
		quit(0)
	else:
		for msg: String in failures:
			push_error(msg)
		quit(1)
