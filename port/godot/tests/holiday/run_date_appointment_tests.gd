extends SceneTree

const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const DateAppointmentPresenter = preload("res://presentation/date_appointment_presenter.gd")
const NativeCanvas = preload("res://presentation/native_canvas.gd")
const NativeRng = preload("res://core/native_rng.gd")


static func _make_rng(seed_val: int) -> NativeRng:
	var rng := NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, seed_val)
	return rng

var _failures: Array[String] = []


func _initialize() -> void:
	call_deferred("_run")


func _check(cond: bool, msg: String) -> void:
	if not cond:
		_failures.append(msg)
		printerr("FAIL: ", msg)


func _run() -> void:
	_test_appointment_bitfield_roundtrip()
	_test_hud_appointment_find_today()
	_test_single_appointment_flow()
	_test_multi_appointment_2_and_3_place_permutations()
	await _test_render_and_save_captures()

	if not _failures.is_empty():
		printerr("Date appointment test suite failed with %d error(s)." % _failures.size())
		quit(1)
		return

	print("Date appointment test suite passed (all rules + 4 visual captures verified).")
	quit(0)


func _test_appointment_bitfield_roundtrip() -> void:
	var raw := {
		"place": 3,
		"month": 4,
		"day": 9,
		"year": 0,
		"flag": 1,
		"girl": 0,
		"active": 1,
		"pad2": 2,
	}
	var w := HolidayAppointmentRules.pack_appointment_word(raw)
	var unpacked := HolidayAppointmentRules.unpack_appointment_word(w)
	_check(int(unpacked["place"]) == 3, "bitfield place roundtrip")
	_check(int(unpacked["month"]) == 4, "bitfield month roundtrip")
	_check(int(unpacked["day"]) == 9, "bitfield day roundtrip")
	_check(int(unpacked["year"]) == 0, "bitfield year roundtrip")
	_check(int(unpacked["flag"]) == 1, "bitfield flag roundtrip")
	_check(int(unpacked["girl"]) == 0, "bitfield girl roundtrip")
	_check(int(unpacked["active"]) == 1, "bitfield active roundtrip")
	_check(int(unpacked["pad2"]) == 2, "bitfield pad2 roundtrip")


func _test_hud_appointment_find_today() -> void:
	var words := [0, 0, 0, 0, 0, 0, 0, 0]
	_check(HolidayAppointmentRules.hud_appointment_find_today(95, 4, 9, words) == 0, "no appointment returns 0")
	_check(HolidayAppointmentRules.hud_appointment_find_today(95, 12, 24, words) == -1, "Dec 24 blackout returns -1")
	_check(HolidayAppointmentRules.hud_appointment_find_today(96, 1, 1, words) == -1, "Jan 1 blackout returns -1")
	_check(HolidayAppointmentRules.hud_appointment_find_today(98, 3, 1, words) == -1, "Grad year March blackout returns -1")

	words[2] = HolidayAppointmentRules.pack_appointment_word({"place": 0, "month": 1, "day": 15, "girl": 0})
	_check(HolidayAppointmentRules.hud_appointment_find_today(95, 13, 15, words) == 3, "month 13 normalizes to month 1 and returns slot+1")


func _test_single_appointment_flow() -> void:
	var state := HolidayAppointmentRules.HolidayState.new()
	state.year = 95
	state.month = 4
	state.day = 9
	HolidayAppointmentRules.holiday_icons_set_available(state)

	var words := [0, 0, 0, 0, 0, 0, 0, 0]
	words[0] = HolidayAppointmentRules.pack_appointment_word({
		"place": 0,
		"month": 4,
		"day": 9,
		"year": 0,
		"flag": 1,
		"girl": 0,
		"active": 0,
	})

	var notice := HolidayAppointmentRules.holiday_open_init_appointment_notice(state, words)
	_check(bool(notice["has_appointment"]), "single appointment detected on open_init")
	_check(state.icon_enabled[9] == 1, "icon 9 enabled when appointment exists today")
	_check(state.menu_cursor == 9, "menu_cursor auto-positioned to 9 on appointment day")
	var notice_lines: Array = notice["sndisp_lines"]
	_check(notice_lines.size() == 1 and String(notice_lines[0]["text"]) == "（今日は、詩織とデートだ）", "morning notice line text matches")

	var click_res := HolidayAppointmentRules.StepResult.new()
	var clicked := HolidayAppointmentRules.holiday_menu_click_appointment(state, words, click_res)
	_check(clicked, "clicking icon 9 starts appointment stage")
	_check(state.mode_stage == 0xFE and state.stage_step == 0 and state.substep == 0, "mode_stage transitions to 0xFE")
	_check((state.last_command & 0x0F) == 9, "last_command low nibble set to 9")

	var start_res := HolidayAppointmentRules.StepResult.new()
	var start_info := HolidayAppointmentRules.appointment_start(state, words, start_res)
	_check(int(start_info["distinct_place_count"]) == 1, "1 distinct place candidate")
	_check(state.chosen_place == 0, "single candidate immediately sets chosen_place = 0")
	_check(state.stage_step == 2, "single candidate skips stage_step 1 and jumps to 2 (confirm)")

	var confirm_res := HolidayAppointmentRules.StepResult.new()
	HolidayAppointmentRules.appointment_place_confirm(state, words, false, confirm_res)
	_check(state.substep == 1, "confirm substep 0 -> 1")
	var confirm_step1 := HolidayAppointmentRules.appointment_place_confirm(state, words, false, confirm_res)
	var pres: Dictionary = confirm_step1["presentation"]
	var clines: Array = pres["sndisp_lines"]
	_check(clines.size() == 3, "confirm splits into 3 sndisp lines")
	_check(String(clines[0]["text"]) == "（それじゃ、", "confirm line 0 matches")
	_check(String(clines[1]["text"]) == "近所の公園で、", "confirm line 1 matches")
	_check(String(clines[2]["text"]) == "詩織を待つか。）", "confirm line 2 matches")
	HolidayAppointmentRules.appointment_place_confirm(state, words, false, confirm_res, true)
	_check(state.stage_step == 3 and state.substep == 0, "confirm substep 2 -> stage_step 3 (launch)")

	var launch_res := HolidayAppointmentRules.StepResult.new()
	var launch_payload := HolidayAppointmentRules.appointment_launch(state, words, launch_res, 9)
	_check(int(launch_payload["game_mode"]) == HolidayAppointmentRules.GAME_MODE_DATE, "launches GAME_MODE_DATE (0x50)")
	_check(launch_payload["day_event"] == [0x80, 0, 0, 0], "day_event set to [0x80, 0, 0, 0]")
	_check(int(launch_payload["current_girl"]) == 0 and int(launch_payload["date_girl"]) == 0, "current_girl and date_girl set to Shiori (0)")
	_check(int(launch_payload["appointment_flag"]) == 1, "appointment_flag unpacked from bit 24")


func _test_multi_appointment_2_and_3_place_permutations() -> void:
	# 2-place appointment: Park (0, Shiori=0) and Aquarium (3, Saki=4)
	var state2 := HolidayAppointmentRules.HolidayState.new()
	state2.year = 95
	state2.month = 4
	state2.day = 9
	var words2 := [
		HolidayAppointmentRules.pack_appointment_word({"place": 0, "month": 4, "day": 9, "flag": 1, "girl": 0}),
		HolidayAppointmentRules.pack_appointment_word({"place": 3, "month": 4, "day": 9, "flag": 0, "girl": 4}),
		0, 0, 0, 0, 0, 0,
	]
	var res2 := HolidayAppointmentRules.StepResult.new()
	HolidayAppointmentRules.appointment_start(state2, words2, res2)
	_check(state2.stage_step == 1 and state2.chosen_place == 0xFF, "2-place appointment enters stage_step 1 (choice menu)")

	var rng2 := _make_rng(0) # first rand() -> even -> permutation [0, 1]
	var menu2 := HolidayAppointmentRules.appointment_place_menu_build(state2, words2, rng2, res2)
	_check(int(menu2["count"]) == 2, "2-place menu has count 2")
	var rows2: Array = menu2["rows"]
	_check(String(rows2[0]["text"]) == "『近所の公園で、待ち合わせ』", "row 0 text matches Park")
	_check(String(rows2[1]["text"]) == "『水族館で、待ち合わせ』", "row 1 text matches Aquarium")

	# Choose row 0 (Park) and launch
	state2.substep = 2
	HolidayAppointmentRules.appointment_place_choose(state2, words2, HolidayAppointmentRules.PAD_CONFIRM, rng2, false, res2, 0)
	_check(state2.chosen_place == 0 and state2.stage_step == 2, "choosing row 0 selects Park (0) and advances to confirm")
	var launch2 := HolidayAppointmentRules.appointment_launch(state2, words2, res2, 9)
	_check(int(launch2["appointment_place_1"]) == 0 and int(launch2["appointment_place_2"]) == 3 and int(launch2["appointment_place_3"]) == 0xFF, "unchosen place packed into appointment_place_2")

	# 3-place appointment: Park (0), Aquarium (3), Library (8)
	var state3 := HolidayAppointmentRules.HolidayState.new()
	state3.year = 95
	state3.month = 4
	state3.day = 9
	var words3 := [
		HolidayAppointmentRules.pack_appointment_word({"place": 0, "month": 4, "day": 9, "flag": 1, "girl": 0, "active": 1}),
		HolidayAppointmentRules.pack_appointment_word({"place": 3, "month": 4, "day": 9, "flag": 0, "girl": 4, "active": 1}),
		HolidayAppointmentRules.pack_appointment_word({"place": 8, "month": 4, "day": 9, "flag": 0, "girl": 2, "active": 1}),
		0, 0, 0, 0, 0,
	]
	var rng3 := _make_rng(1)
	var res3 := HolidayAppointmentRules.StepResult.new()
	var menu3 := HolidayAppointmentRules.appointment_place_menu_build(state3, words3, rng3, res3)
	_check(int(menu3["count"]) == 3, "3-place menu has count 3")
	state3.chosen_place = 3
	var launch3 := HolidayAppointmentRules.appointment_launch(state3, words3, res3, 9)
	_check(int(launch3["appointment_place_1"]) == 3 and int(launch3["appointment_place_2"]) == 0 and int(launch3["appointment_place_3"]) == 8, "3-place launch packs remaining 2 unchosen places in order")


func _test_render_and_save_captures() -> void:
	var repo_root := ProjectSettings.globalize_path("res://").simplify_path().get_base_dir().get_base_dir()
	var hud_manifest := repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack := repo_root.path_join(".asset-cache/port/date-preview-v1")
	var captures_dir := repo_root.path_join(".claude/orchestrator/state/T-29013/date-appointment-slice-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	var presenter := DateAppointmentPresenter.new()
	var opened := presenter.open_date_preview(hud_manifest, font_root, date_pack)
	_check(opened, "DateAppointmentPresenter opened HUD + font + date-preview-v1 pack: " + presenter.error)
	if not opened:
		return

	var state := HolidayAppointmentRules.HolidayState.new()
	state.year = 95
	state.month = 4
	state.day = 9
	HolidayAppointmentRules.holiday_icons_set_available(state)

	var words := [
		HolidayAppointmentRules.pack_appointment_word({"place": 0, "month": 4, "day": 9, "flag": 1, "girl": 0}),
		HolidayAppointmentRules.pack_appointment_word({"place": 3, "month": 4, "day": 9, "flag": 0, "girl": 4}),
		0, 0, 0, 0, 0, 0,
	]

	# 1. Morning appointment notice
	var notice := HolidayAppointmentRules.holiday_open_init_appointment_notice(state, words)
	var frame1 := presenter.render_holiday_appointment_notice(state, notice)
	_assert_text_runs_in_message_box(frame1["text_runs"], "01-holiday-appointment-notice")
	var img1: Image = await _render_via_native_canvas(frame1["commands"], frame1["image"])
	var path1 := captures_dir.path_join("01-holiday-appointment-notice.png")
	_check(img1.save_png(path1) == OK, "saved 01-holiday-appointment-notice.png")

	# 2. Multi-appointment place choice menu
	var step_res := HolidayAppointmentRules.StepResult.new()
	HolidayAppointmentRules.appointment_start(state, words, step_res)
	var rng := _make_rng(0)
	var menu_data := HolidayAppointmentRules.appointment_place_menu_build(state, words, rng, step_res)
	var frame2 := presenter.render_appointment_place_choice(state, menu_data, 0)
	_assert_text_runs_in_message_box(frame2["text_runs"], "02-appointment-place-choice")
	var img2: Image = await _render_via_native_canvas(frame2["commands"], frame2["image"])
	var path2 := captures_dir.path_join("02-appointment-place-choice.png")
	_check(img2.save_png(path2) == OK, "saved 02-appointment-place-choice.png")

	# 3. Appointment place confirmation
	state.chosen_place = 0
	var confirm_data := HolidayAppointmentRules.build_appointment_confirm_Presentation(state, words)
	var frame3 := presenter.render_appointment_place_confirm(state, confirm_data)
	_assert_text_runs_in_message_box(frame3["text_runs"], "03-appointment-place-confirm")
	var img3: Image = await _render_via_native_canvas(frame3["commands"], frame3["image"])
	var path3 := captures_dir.path_join("03-appointment-place-confirm.png")
	_check(img3.save_png(path3) == OK, "saved 03-appointment-place-confirm.png")

	# 4. Date arrival scene at Neighborhood Park (GAME_MODE_DATE = 0x50)
	var launch_payload := HolidayAppointmentRules.appointment_launch(state, words, step_res, 9)
	var frame4 := presenter.render_date_arrival_scene(state, launch_payload)
	_assert_text_runs_in_message_box(frame4["text_runs"], "04-date-arrival-scene")
	var img4: Image = await _render_via_native_canvas(frame4["commands"], frame4["image"])
	var path4 := captures_dir.path_join("04-date-arrival-scene.png")
	_check(img4.save_png(path4) == OK, "saved 04-date-arrival-scene.png")


func _assert_text_runs_in_message_box(runs: Array, label: String) -> void:
	# Message window inner gradient is Rect2i(27, 166, 266, 52) -> x in [27..293], y in [166..218].
	for run: Dictionary in runs:
		var pos: Array = run["position"]
		var sx: int = 160 + int(pos[0])
		var sy: int = 120 + int(pos[1])
		var glyph_count: int = String(run["hex"]).length() / 4
		var right_x: int = sx + (glyph_count - 1) * 14 + 12
		var bottom_y: int = sy + 12
		_check(sx >= 27 and right_x <= 293, "%s text run x bounds [%d..%d] inside [27..293]" % [label, sx, right_x])
		_check(sy >= 166 and bottom_y <= 218, "%s text run y bounds [%d..%d] inside [166..218]" % [label, sy, bottom_y])


func _render_via_native_canvas(commands: Array, fallback_image: Image) -> Image:
	if DisplayServer.get_name() == "headless":
		return fallback_image
	var viewport := SubViewport.new()
	viewport.size = Vector2i(320, 240)
	viewport.transparent_bg = false
	viewport.render_target_update_mode = SubViewport.UPDATE_ALWAYS
	root.add_child(viewport)
	var canvas := NativeCanvas.new()
	viewport.add_child(canvas)
	canvas.set_frame_commands(commands)
	await process_frame
	await process_frame
	var tex := viewport.get_texture()
	var captured := tex.get_image() if tex != null else fallback_image
	viewport.queue_free()
	await process_frame
	if captured == null or captured.get_size() != Vector2i(320, 240):
		return fallback_image
	return captured
