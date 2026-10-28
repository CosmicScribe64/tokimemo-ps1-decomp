extends SceneTree
## Verification suite for:
## 1. AudioService.create_pcm_wav_stream (bgm_play, se_play, voice_play 16-bit PCM 22050 Hz WAV synthesis).
## 2. new_game_entry_boot.gd NativeCanvas 320x240 VnScenePresenter rendering & full profile completion (stage == 2, step_id == 5, profile_complete == true).
## 3. playable_boot.gd multi-week progression (allow_multi_week = true) across Week 1 into Week 2 (1995-04-11+) & AudioStreamPlayer wiring.

const AudioService = preload("res://services/audio_service.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")
const NewGameEntryBoot = preload("res://app/new_game_entry_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")

var checks: int = 0
var failures: Array[String] = []


func check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)
		push_error("FAIL: " + message)


func _initialize() -> void:
	call_deferred("_run")


func _repo_root() -> String:
	var candidate: String = ProjectSettings.globalize_path("res://../..").simplify_path()
	if DirAccess.dir_exists_absolute(candidate.path_join(".asset-cache/port")):
		return candidate
	if DirAccess.dir_exists_absolute("/workspace/.asset-cache/port"):
		return "/workspace"
	if DirAccess.dir_exists_absolute("/work/.asset-cache/port"):
		return "/work"
	return candidate


func _ensure_env_defaults() -> void:
	var repo: String = _repo_root()
	var defaults: Dictionary = {
		"TOKIMEMO_NEW_GAME_RULES": repo.path_join(".asset-cache/port/t29012-tables-v3-final/manifest.json"),
		"TOKIMEMO_NEW_GAME_TABLES": repo.path_join(".asset-cache/port/t29012-tables-v3-final/manifest.json"),
		"TOKIMEMO_NEW_GAME_ENTRY": repo.path_join(".asset-cache/port/new-game-entry-v1/entry.json"),
		"TOKIMEMO_NEW_GAME_CONTENT": repo.path_join(".asset-cache/port/new-game-entry-v1/entry.json"),
		"TOKIMEMO_NEW_GAME_SEED": "19950404",
		"TOKIMEMO_NATIVE_FONT": repo.path_join(".asset-cache/port/fonts/native-font-v2"),
		"TOKIMEMO_BOUNDARY": repo.path_join(".claude/orchestrator/state/T-29004/evidence/development-boundary.json"),
		"TOKIMEMO_BOUNDARY_SHA": PlayableBoot.BOUNDARY_SHA,
		"TOKIMEMO_TABLES": repo.path_join(".asset-cache/port/school-day-tables-v1-final/manifest.json"),
		"TOKIMEMO_TABLES_SHA": PlayableBoot.TABLES_SHA,
		"TOKIMEMO_MEDIA_PACK": repo.path_join(".asset-cache/port/holiday-command0-current-964ce"),
		"T29014_SPRING_PACK": repo.path_join(".asset-cache/port/spring-command-current-236ab3"),
		"T29014_WEEKDAY56_PACK": repo.path_join(".asset-cache/port/weekday56-current-e593"),
		"T29014_CLUB_PACK": repo.path_join(".asset-cache/port/t29014-command4-club-v1"),
		"TOKIMEMO_HUD_MANIFEST": repo.path_join(".asset-cache/port/hud-production-v1/manifest.json"),
		"TOKIMEMO_HUD_SHA": PlayableBoot.LiveHud.MANIFEST_SHA,
		"TOKIMEMO_FONT_ROOT": repo.path_join(".asset-cache/port/fonts/native-font-v2"),
		"TOKIMEMO_HUD_CONTEXT": repo.path_join(".claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json"),
		"TOKIMEMO_HUD_CONTEXT_SHA": PlayableBoot.LiveHud.CONTEXT_SHA,
		"TOKIMEMO_HOLIDAY_EXTENSION": repo.path_join(".claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json"),
		"TOKIMEMO_HOLIDAY_TEXT": repo.path_join(".asset-cache/port/holiday-hud-text-v1/manifest.json"),
		"TOKIMEMO_HOLIDAY_TEXT_SHA": PlayableBoot.HolidayHud.TEXT_SHA,
		"TOKIMEMO_DATE_PACK": repo.path_join(".asset-cache/port/date-preview-v1"),
		"TOKIMEMO_REQUIRE_HUD": "1",
		"TOKIMEMO_REQUIRE_HOLIDAY": "1",
	}
	for key: String in defaults:
		if OS.get_environment(key).is_empty():
			OS.set_environment(key, str(defaults[key]))


func _has_non_silent_pcm16(data: PackedByteArray) -> bool:
	for i: int in range(0, data.size() - 1, 2):
		var lo: int = data[i]
		var hi: int = data[i + 1]
		var val: int = lo | (hi << 8)
		if val >= 0x8000:
			val -= 0x10000
		if absi(val) > 256:
			return true
	return false


func _test_audio_service_pcm_synthesis() -> void:
	print("[Phase 1] AudioService.create_pcm_wav_stream verification")
	var svc := AudioService.new()
	for kind: String in ["bgm_play", "se_play", "voice_play"]:
		var stream: AudioStreamWAV = svc.create_pcm_wav_stream(kind, 1, 180)
		check(stream != null, "%s stream is non-null" % kind)
		if stream == null:
			continue
		check(stream.format == AudioStreamWAV.FORMAT_16_BITS, "%s stream is 16-bit PCM" % kind)
		check(stream.mix_rate == 22050, "%s stream mix_rate is 22050 Hz" % kind)
		check(not stream.stereo, "%s stream is mono" % kind)
		check(not stream.data.is_empty() and stream.data.size() % 2 == 0, "%s stream has frame-aligned non-empty PCM data (%d bytes)" % [kind, stream.data.size()])
		check(_has_non_silent_pcm16(stream.data), "%s stream contains non-silent synthesized waveform samples" % kind)
		if kind == "bgm_play":
			check(stream.loop_mode == AudioStreamWAV.LOOP_FORWARD, "bgm_play stream enables forward looping")
		else:
			check(stream.loop_mode == AudioStreamWAV.LOOP_DISABLED, "%s stream disables looping" % kind)


func _tick_entry(entry_scene: Node, steps: int = 3) -> void:
	for _i: int in range(steps):
		entry_scene._physics_process(1.0 / 60.0)
		await process_frame


func _test_new_game_entry_native_and_completion() -> void:
	print("[Phase 2] new_game_entry_boot.gd NativeCanvas 320x240 & profile completion flow")
	var entry_scene: Control = load("res://app/new_game_entry.tscn").instantiate()
	entry_scene.set("entry_clock_policy", {
		"clock_origin": "desktop-fresh-entry-local; native boot/title prefix unmeasured",
		"timer_observation_source": "desktop-one-timer-per-logic-v1",
		"native_clock_parity": false,
	})
	root.add_child(entry_scene)
	await _tick_entry(entry_scene, 2)

	check(entry_scene.failure.is_empty(), "new_game_entry_boot loaded with no failure: " + entry_scene.failure)
	check(entry_scene.controller != null and entry_scene.glyphs != null and entry_scene.rng != null, "preserved controller, glyphs, and rng hooks")
	check(entry_scene.load_diagnostics is Dictionary and entry_scene.queue is Array, "preserved load_diagnostics and queue hooks")
	check(NewGameEntryBoot.profile_text({"month": 3, "day": 3, "blood": 0}) == "Birthday: 4/4   Blood type: A", "preserved profile_text static helper")
	check(entry_scene.canvas != null and entry_scene.canvas.scale == Vector2(2, 2), "NativeCanvas is present and scaled 2x")
	check(entry_scene.presenter != null, "VnScenePresenter is initialized on new_game_entry_boot")
	check(entry_scene.last_rendered_image != null and entry_scene.last_rendered_image.get_size() == Vector2i(320, 240), "rendered native 320x240 image on new_game_entry_boot")
	if entry_scene.presenter != null:
		check(entry_scene.presenter.bounding_box_violations.is_empty(), "zero glyph bounding-box violations on new_game_entry_boot")

	# Step through family, given, and nickname entry (select a kana cell then finish each part)
	for _part_idx: int in range(3):
		entry_scene._send({"kind": "cell", "column": 0, "row": 0})
		await _tick_entry(entry_scene, 2)
		entry_scene._send({"kind": "done"})
		await _tick_entry(entry_scene, 3)

	check(entry_scene.controller.stage == 1 and entry_scene.controller.step_id == 7, "reached player birthday/blood-type step (stage 1, step 7)")

	# Finalize player birthday/blood-type and confirm
	entry_scene._send({"kind": "start"})
	await _tick_entry(entry_scene, 3)
	check(entry_scene.controller.stage == 1 and entry_scene.controller.step_id == 9, "reached player profile confirmation (stage 1, step 9)")
	entry_scene._send({"kind": "yes"})
	await _tick_entry(entry_scene, 3)

	# Finalize Shiori birthday/blood-type and confirm
	check(entry_scene.controller.stage == 2 and entry_scene.controller.step_id == 1, "reached Shiori birthday/blood-type step (stage 2, step 1)")
	entry_scene._send({"kind": "start"})
	await _tick_entry(entry_scene, 3)
	check(entry_scene.controller.stage == 2 and entry_scene.controller.step_id == 3, "reached Shiori profile confirmation (stage 2, step 3)")
	entry_scene._send({"kind": "yes"})
	await _tick_entry(entry_scene, 4)

	check(entry_scene.controller.stage == 2 and entry_scene.controller.step_id == 5, "reached Profile complete (stage == 2 and step_id == 5)")
	check(entry_scene.profile_complete, "profile_complete flag is true at stage 2 step 5")
	check(entry_scene.last_rendered_image != null and entry_scene.last_rendered_image.get_size() == Vector2i(320, 240), "final profile complete state rendered to 320x240 NativeCanvas")

	root.remove_child(entry_scene)
	entry_scene.free()
	await process_frame


func _press_confirm(app: PlayableBoot, tick: int) -> void:
	var ev_down := InputEventKey.new()
	ev_down.physical_keycode = KEY_ENTER
	ev_down.pressed = true
	app.pad._input(ev_down)
	app._physics_process(1.0 / 60.0)
	var ev_up := InputEventKey.new()
	ev_up.physical_keycode = KEY_ENTER
	ev_up.pressed = false
	app.pad._input(ev_up)
	app._physics_process(1.0 / 60.0)


func _test_playable_boot_multi_week_and_audio_players() -> void:
	print("[Phase 3] playable_boot.gd multi-week progression (Week 1 -> Week 2 1995-04-11+) & AudioStreamPlayer wiring")
	var app := PlayableBoot.new()
	app.allow_multi_week = true
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.add_child(app.canvas)
	app.add_child(app.pad)
	app.add_child(app.status_label)
	app.add_child(app.detail_label)

	var load_err: String = app._load_inputs()
	check(load_err.is_empty(), "playable_boot _load_inputs succeeded: " + load_err)
	var reset_err: String = app._reset()
	check(reset_err.is_empty(), "playable_boot _reset with allow_multi_week succeeded: " + reset_err)
	root.add_child(app)
	app.set_physics_process(false)

	check(bool(app.state.rule_state.get("allow_multi_week", false)), "state.rule_state['allow_multi_week'] is true")
	check(app.state.rule_state["resources"]["available_commands"] == [0, 1, 2, 3, 4, 5, 6, 7], "all weekday commands 0..7 enabled in available_commands")
	check(app.state.rule_state["resources"]["available_holiday_commands"] == [0], "holiday command [0] enabled in available_holiday_commands")
	check(app.bgm_player != null and app.se_player != null and app.voice_player != null, "bgm_player, se_player, and voice_player AudioStreamPlayer nodes created")

	# Confirm Week 1 command 0 on 1995-04-04
	_press_confirm(app, 0)
	check(app.state.game_mode == 0x30, "Week 1 command confirmation transitioned to scheduler mode 0x30")
	check(app.bgm_player.stream is AudioStreamWAV, "bgm_player assigned synthesized AudioStreamWAV on command confirm")
	check(app.se_player.stream is AudioStreamWAV, "se_player assigned synthesized AudioStreamWAV on command confirm")
	app._play_committed_audio([{"kind": "voice_play", "voice_id": 0}])
	check(app.voice_player.stream is AudioStreamWAV, "voice_player assigned synthesized AudioStreamWAV on voice_play")

	# Verify playback_pause_changed pauses and resumes active AudioStreamPlayers
	app._set_playback_paused(true)
	check(app.bgm_player.stream_paused and app.se_player.stream_paused and app.voice_player.stream_paused, "playback_pause_changed(true) pauses all AudioStreamPlayers")
	app._set_playback_paused(false)
	check(not app.bgm_player.stream_paused and not app.se_player.stream_paused and not app.voice_player.stream_paused, "playback_pause_changed(false) resumes all AudioStreamPlayers")

	# Step through Week 1 (1995-04-05..09) including Sunday Holiday menu (0x46) until Week 2 command screen (0x34 stage 1 on 1995-04-09)
	var guard: int = 0
	while guard < 500 and not (app.state.game_mode == 0x34 and app.state.mode_stage == 1 and int(app.state.rule_state["day"]) >= 9):
		if app.state.game_mode == 0x46 and app.state.mode_stage == 1 and app.state.mode_step == 0:
			app.state.rule_state["menu_cursor"] = 0
			_press_confirm(app, guard)
			guard += 2
		else:
			app._physics_process(1.0 / 60.0)
			guard += 1
		if app.stopped:
			break

	check(not app.stopped and app.failure.is_empty(), "Week 1 completed through Sunday 1995-04-09 without stopping: " + app.failure)
	check(app.state.game_mode == 0x34 and app.state.mode_stage == 1 and [app.state.rule_state["year"], app.state.rule_state["month"], app.state.rule_state["day"]] == [95, 4, 9],
		"reached Week 2 command selection screen on 1995-04-09")

	# Select command 1 for Week 2 and confirm
	app.state.rule_state["menu_cursor"] = 1
	_press_confirm(app, guard)
	check(app.state.game_mode == 0x30, "Week 2 command 1 confirmation transitioned to scheduler mode 0x30")

	# Step into Week 2 across 1995-04-10, 1995-04-11, 1995-04-12+
	var reached_april_11_activity: bool = false
	var max_day_seen: int = int(app.state.rule_state["day"])
	var week2_guard: int = 0
	while week2_guard < 500 and int(app.state.rule_state["day"]) < 12:
		app._physics_process(1.0 / 60.0)
		week2_guard += 1
		var cur_day: int = int(app.state.rule_state["day"])
		if cur_day > max_day_seen:
			max_day_seen = cur_day
		if cur_day >= 11 and app.state.game_mode == 0x40:
			reached_april_11_activity = true
		if app.stopped:
			break

	check(not app.stopped and app.failure.is_empty(), "Week 2 progressed past 1995-04-11 without error: " + app.failure)
	check(reached_april_11_activity, "executed weekday activity (mode 0x40) in Week 2 on 1995-04-11+")
	check(max_day_seen >= 11, "calendar advanced into Week 2 (1995-04-%02d)" % max_day_seen)

	root.remove_child(app)
	app.free()


func _run() -> void:
	print("--- Live Desktop Full Game Verification Suite ---")
	_ensure_env_defaults()
	_test_audio_service_pcm_synthesis()
	await _test_new_game_entry_native_and_completion()
	_test_playable_boot_multi_week_and_audio_players()

	if failures.is_empty():
		print("PASS: %d live desktop full-game checks passed" % checks)
		quit(0)
	else:
		for f: String in failures:
			printerr("FAIL: " + f)
		quit(1)
