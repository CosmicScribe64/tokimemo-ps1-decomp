extends SceneTree
## T-29014: Integration test suite for seasonal, condition, and vacation command variants.
## Verifies:
## 1. SchoolDay.resolve_bg_and_sd exact native pair selection across all commands, clubs, seasons, condition, and vacation.
## 2. SeasonalCommandContentStore pack loading, with image/frame checks on selected
##    samples from the 33 backgrounds and 44 SD sets; extraction and full rendering are not tested here.
## 3. OrdinaryCommandMediaAdapter variant validation and media delivery with the seasonal pack.
## 4. SchoolDayController setup and load descriptor emission for seasonal and condition variants.

const SchoolDay = preload("res://rules/school_day.gd")
const SeasonalStore = preload("res://services/seasonal_command_content_store.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

var failures: Array[String] = []
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
		push_error("FAIL: " + message)

func _initialize() -> void:
	print("--- T-29014 Seasonal & Condition Variants Test Suite ---")
	_test_native_resolution_rules()
	_test_seasonal_content_store()
	_test_media_adapter_seasonal_integration()
	_test_controller_seasonal_descriptors()
	_finish_suite()

func _test_native_resolution_rules() -> void:
	print("[Phase 1] Native SchoolDay.resolve_bg_and_sd verification")

	# 1. Weekday Summer vs Non-Summer ordinary commands 1–3, 5–6, 0, 7
	var expected_summer_sd: Dictionary = {0: 2, 1: 6, 2: 10, 3: 13, 5: 23, 6: 19, 7: 15}
	var expected_winter_sd: Dictionary = {0: 3, 1: 7, 2: 11, 3: 13, 5: 24, 6: 20, 7: 16}
	var expected_weekday_bg: Dictionary = {0: 16131, 1: 16141, 2: 16151, 3: 16160, 5: 16268, 6: 16277, 7: 16283}

	for cmd: int in [0, 1, 2, 3, 5, 6, 7]:
		# Summer
		var res_sum: Dictionary = SchoolDay.resolve_bg_and_sd(cmd, 0, true, 0, 0, 1, 0)
		check(not res_sum.has("error"), "cmd %d summer resolved with no error" % cmd)
		check(res_sum["background_lba"] == expected_weekday_bg[cmd], "cmd %d summer background LBA matches" % cmd)
		check(res_sum["sd_set_id"] == expected_summer_sd[cmd], "cmd %d summer SD set ID matches" % cmd)
		check(res_sum["sd_lba"] == 42418 + 41 * expected_summer_sd[cmd], "cmd %d summer SD LBA matches" % cmd)
		check(res_sum["sectors"] == 41, "cmd %d summer SD sectors 41" % cmd)

		# Non-Summer (Winter/Spring)
		var res_win: Dictionary = SchoolDay.resolve_bg_and_sd(cmd, 0, false, 0, 0, 0, 0)
		check(not res_win.has("error"), "cmd %d winter resolved with no error" % cmd)
		check(res_win["background_lba"] == expected_weekday_bg[cmd], "cmd %d winter background LBA matches" % cmd)
		check(res_win["sd_set_id"] == expected_winter_sd[cmd], "cmd %d winter SD set ID matches" % cmd)
		check(res_win["sd_lba"] == 42418 + 41 * expected_winter_sd[cmd], "cmd %d winter SD LBA matches" % cmd)

	# 2. Club command 4: all 11 clubs in summer and non-summer
	var club_bgs: Array[int] = [16215, 16224, 16234, 16244, 16252, 16260, 16168, 16177, 16206, 16197, 16187]
	var club_summer_sets: Array[int] = [2, 31, 6, 10, 28, 24, 13, 15, 21, 19, 17]
	var club_winter_sets: Array[int] = [3, 31, 7, 11, 29, 25, 13, 15, 21, 19, 17]

	for cid: int in range(11):
		var r_sum: Dictionary = SchoolDay.resolve_bg_and_sd(4, cid, true, 0, 0, 1, 0)
		check(r_sum["background_lba"] == club_bgs[cid], "club %d summer background LBA matches" % cid)
		check(r_sum["sd_set_id"] == club_summer_sets[cid], "club %d summer SD set ID matches" % cid)
		check(r_sum["sd_lba"] == 27834 + 41 * club_summer_sets[cid], "club %d summer SD LBA matches" % cid)

		var r_win: Dictionary = SchoolDay.resolve_bg_and_sd(4, cid, false, 0, 0, 0, 0)
		check(r_win["background_lba"] == club_bgs[cid], "club %d winter background LBA matches" % cid)
		check(r_win["sd_set_id"] == club_winter_sets[cid], "club %d winter SD set ID matches" % cid)
		check(r_win["sd_lba"] == 27834 + 41 * club_winter_sets[cid], "club %d winter SD LBA matches" % cid)

	# 3. Sick Rest: condition_bucket == 1, command 7 across all 4 seasons
	var weekday_sick_bgs: Array[int] = [16361, 16370, 16343, 16352]
	var vacation_sick_bgs: Array[int] = [16318, 16326, 16301, 16310]
	for s_idx: int in range(4):
		# Weekday sick rest
		var r_sick: Dictionary = SchoolDay.resolve_bg_and_sd(7, 0, false, 1, 0, s_idx, 0)
		check(r_sick["background_lba"] == weekday_sick_bgs[s_idx], "season %d weekday sick BG matches" % s_idx)
		check(r_sick["sd_lba"] == 32138, "season %d sick SD LBA is 32138" % s_idx)
		check(r_sick["sectors"] == 9, "season %d sick SD sectors is 9" % s_idx)

		# Vacation sick rest
		var r_vac_sick: Dictionary = SchoolDay.resolve_bg_and_sd(7, 0, false, 1, 1, s_idx, 0)
		check(r_vac_sick["background_lba"] == vacation_sick_bgs[s_idx], "season %d vacation sick BG matches" % s_idx)
		check(r_vac_sick["sd_lba"] == 32138, "season %d vacation sick SD LBA is 32138" % s_idx)
		check(r_vac_sick["sectors"] == 9, "season %d vacation sick SD sectors is 9" % s_idx)

	# 4. Vacation Activity across commands 0..7
	var holiday_bgs: Dictionary = {0: 16379, 1: 16379, 2: 16379, 3: 16387, 5: 16396, 6: 16277, 7: 16283}
	var holiday_summer_sd: Dictionary = {0: 0, 1: 2, 2: 4, 3: 12, 5: 10, 6: 8, 7: 6}
	var holiday_winter_sd: Dictionary = {0: 1, 1: 3, 2: 5, 3: 12, 5: 11, 6: 9, 7: 7}
	var holiday_sectors: Array[int] = [41446, 41487, 41528, 41569, 41610, 41651, 41692, 41733, 41774, 41815, 41856, 41897, 42910]

	for cmd: int in [0, 1, 2, 3, 5, 6, 7]:
		var r_v_sum: Dictionary = SchoolDay.resolve_bg_and_sd(cmd, 0, true, 0, 1, 1, 0)
		check(r_v_sum["background_lba"] == holiday_bgs[cmd], "vacation cmd %d summer background matches" % cmd)
		check(r_v_sum["sd_set_id"] == holiday_summer_sd[cmd], "vacation cmd %d summer SD set matches" % cmd)
		check(r_v_sum["sd_lba"] == holiday_sectors[holiday_summer_sd[cmd]], "vacation cmd %d summer SD LBA matches" % cmd)

		var r_v_win: Dictionary = SchoolDay.resolve_bg_and_sd(cmd, 0, false, 0, 1, 0, 0)
		check(r_v_win["background_lba"] == holiday_bgs[cmd], "vacation cmd %d winter background matches" % cmd)
		check(r_v_win["sd_set_id"] == holiday_winter_sd[cmd], "vacation cmd %d winter SD set matches" % cmd)
		check(r_v_win["sd_lba"] == holiday_sectors[holiday_winter_sd[cmd]], "vacation cmd %d winter SD LBA matches" % cmd)

	# 5. Negative cases
	var r_bad_cmd: Dictionary = SchoolDay.resolve_bg_and_sd(8, 0, false, 0, 0, 0, 0)
	check(r_bad_cmd.has("error"), "command 8 rejected with error")
	var r_bad_club: Dictionary = SchoolDay.resolve_bg_and_sd(4, 11, false, 0, 0, 0, 0)
	check(r_bad_club.has("error"), "club 11 rejected with error")
	var r_companion: Dictionary = SchoolDay.resolve_bg_and_sd(1, 0, false, 0, 0, 0, 1)
	check(r_companion.has("error"), "companion slot 1 rejected with error")

func _test_seasonal_content_store() -> void:
	print("[Phase 2] SeasonalCommandContentStore authentic pack loading")
	var seasonal_path: String = OS.get_environment("T29014_SEASONAL_PACK")
	if seasonal_path.is_empty():
		seasonal_path = ProjectSettings.globalize_path("res://.asset-cache/port/seasonal-command-v1")
	if not DirAccess.dir_exists_absolute(seasonal_path):
		seasonal_path = "/workspace/.asset-cache/port/seasonal-command-v1"
	if not DirAccess.dir_exists_absolute(seasonal_path):
		seasonal_path = "/work/.asset-cache/port/seasonal-command-v1"

	var store: SeasonalStore = SeasonalStore.new()
	var ok: bool = store.open_pack(seasonal_path)
	check(ok, "seasonal content store opens pack at " + seasonal_path + ": " + store.error_message())

	# Verify sample backgrounds load into valid Godot Images
	var sample_bgs: Array[String] = [
		"weekday_command_1_bg",
		"weekday_sick_season_0_bg",
		"weekday_sick_season_1_bg",
		"vacation_sick_season_1_bg",
		"vacation_command_1_bg",
		"command_4_club_0_bg",
	]
	for bg_id: String in sample_bgs:
		var img: Image = store.get_background(bg_id)
		check(img != null, "background %s loaded" % bg_id)
		check(img != null and img.get_size() == Vector2i(320, 88), "background %s size is 320x88" % bg_id)

	# Verify sample SD sets load valid frames and sequences
	var sample_sds: Array[String] = [
		"weekday_cmd_1_summer_sd",
		"club_0_summer_sd",
		"sick_rest_sd",
		"vacation_cmd_1_summer_sd",
	]
	for sd_id: String in sample_sds:
		var res: Dictionary = store.get_sd_resources(sd_id)
		check(res.has("sequences") and res.has("frames"), "SD set %s has sequences and frames" % sd_id)
		check(res["sequences"].has("0"), "SD set %s has sequence 0" % sd_id)
		check(not res["frames"].is_empty(), "SD set %s has non-empty frames" % sd_id)
		var frame0: Dictionary = res["frames"].get("0", {})
		check(frame0.has("image") and frame0["image"] is Image, "SD set %s frame 0 has Image" % sd_id)
		check(frame0.has("bounds") and frame0["bounds"] is Rect2i, "SD set %s frame 0 has Rect2i bounds" % sd_id)

func _test_media_adapter_seasonal_integration() -> void:
	print("[Phase 3] OrdinaryCommandMediaAdapter with Seasonal pack integration")
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	if media_path.is_empty(): media_path = "/workspace/.asset-cache/port/school-day-command7-current"
	var spring_path: String = OS.get_environment("T29014_SPRING_PACK")
	if spring_path.is_empty(): spring_path = "/workspace/.asset-cache/port/spring-command-current-236ab3"
	var weekday56_path: String = OS.get_environment("T29014_WEEKDAY56_PACK")
	if weekday56_path.is_empty(): weekday56_path = "/workspace/.asset-cache/port/weekday56-current-e593"
	var club_path: String = OS.get_environment("T29014_CLUB_PACK")
	if club_path.is_empty(): club_path = "/workspace/.asset-cache/port/t29014-command4-club-v1"
	var seasonal_path: String = OS.get_environment("T29014_SEASONAL_PACK")
	if seasonal_path.is_empty(): seasonal_path = "/workspace/.asset-cache/port/seasonal-command-v1"

	var adapter: MediaAdapter = MediaAdapter.new()
	var err: String = adapter.open_packs(media_path, spring_path, weekday56_path, club_path, seasonal_path)
	check(err.is_empty(), "adapter opens all 5 packs (including seasonal): " + err)
	check(adapter.has_seasonal_pack(), "adapter recognizes seasonal pack is loaded")

	# Verify validate_variant accepts summer variants
	for cmd: int in [1, 2, 3, 5, 6]:
		var v_err: String = adapter.validate_variant(cmd, 11, 7, 15, 0, 0) # July (summer)
		check(v_err.is_empty(), "cmd %d accepted in summer July: %s" % [cmd, v_err])

	# Verify validate_variant accepts summer clubs
	for cid: int in range(11):
		var c_err: String = adapter.validate_variant(4, cid, 7, 15, 0, 0)
		check(c_err.is_empty(), "club %d accepted in summer July: %s" % [cid, c_err])

	# Verify validate_variant accepts vacation variants
	var vac_err: String = adapter.validate_variant(1, 11, 4, 15, 1, 0)
	check(vac_err.is_empty(), "cmd 1 accepted in vacation: %s" % vac_err)

	# Verify validate_variant accepts sick rest
	var sick_err: String = adapter.validate_variant(7, 11, 4, 15, 0, 3)
	check(sick_err.is_empty(), "cmd 7 accepted with sick health=3: %s" % sick_err)

	# Verify validate_variant rejects companion variants
	var comp_err: String = adapter.validate_variant(1, 11, 7, 0, 0, 0)
	check(comp_err.contains("companion variants for command 1 are not supported"), "companion variant rejected: " + comp_err)

	# Verify variant background draw commands
	var bg_cmds: Array[Dictionary] = adapter.get_variant_background_commands(1, 0, true, 0, 0, 1)
	check(bg_cmds.size() == 2, "summer cmd 1 has 2 background geometry commands")
	check(bg_cmds[0]["id"].ends_with(":left") and bg_cmds[1]["id"].ends_with(":right"), "summer cmd 1 left/right IDs")

	# Verify variant SD resources
	var res: Dictionary = adapter.get_variant_resources(1, 0, true, 0, 0, 1)
	check(res.has("sequences") and res.has("frames"), "summer cmd 1 has sequences and frames")
	check(res["sequences"].has("0"), "summer cmd 1 has sequence 0")

func _test_controller_seasonal_descriptors() -> void:
	print("[Phase 4] SchoolDayController seasonal load descriptor generation")
	var tables_path: String = OS.get_environment("TOKIMEMO_TABLES")
	if tables_path.is_empty(): tables_path = "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json"
	var tables_raw: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(tables_path)))
	var tables: Dictionary = tables_raw.get("tables", {})

	var controller: Controller = Controller.new(tables)
	check(controller.error.is_empty(), "controller initialized with tables: " + controller.error)

	var state: State = State.new()
	state.rule_state = {
		"year": 95, "month": 7, "day": 10, "vacation": 0,
		"player_flags": 0, "day_event_index": 0,
		"day_events": [[0, 1, 0, 15]], # Command 1, solo
		"background_cache": [0, 0, 0, 0],
		"background_show": 0,
		"resources": {},
		"actor_state": {},
	}

	# Test summer descriptor emission
	var desc: Dictionary = controller._load_descriptor(state, "background")
	check(desc["kind"] == "school_day_load", "descriptor kind is school_day_load")
	check(desc["command"] == 1, "descriptor command is 1")
	check(desc["background_lba"] == 16141, "summer cmd 1 background LBA is 16141")

	var sd_desc: Dictionary = controller._load_descriptor(state, "sd")
	check(sd_desc["sd_set_id"] == 6, "summer cmd 1 SD set ID is 6")
	check(sd_desc["sd_lba"] == 42664, "summer cmd 1 SD LBA is 42664")
	check(sd_desc["sectors"] == 41, "summer cmd 1 SD sectors is 41")

	# Test sick rest descriptor emission
	state.rule_state["day_events"] = [[0, 7, 0, 15]] # Command 7 rest
	state.rule_state["player_flags"] = 0x0300 # health = 3 (sick)
	var sick_desc: Dictionary = controller._load_descriptor(state, "sd")
	check(sick_desc["sd_set_id"] == -1, "sick rest SD set ID is -1")
	check(sick_desc["sd_lba"] == 32138, "sick rest SD LBA is 32138")
	check(sick_desc["sectors"] == 9, "sick rest SD sectors is 9")

func _finish_suite() -> void:
	print("--- Seasonal & Condition Integration Suite Results ---")
	print("Total checks: %d" % checks)
	if failures.is_empty():
		print("ALL CHECKS PASSED (100% OK)")
		quit(0)
	else:
		print("FAILED with %d error(s):" % failures.size())
		for f: String in failures:
			print("  - " + f)
		quit(1)

static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var output: Dictionary = {}
		for key: String in value:
			output[key] = _integers(value[key])
		return output
	if value is Array:
		var output: Array = []
		for item: Variant in value:
			output.append(_integers(item))
		return output
	if value is float and is_finite(value) and value == floor(value) and abs(value) <= 4294967295.0:
		return int(value)
	return value
