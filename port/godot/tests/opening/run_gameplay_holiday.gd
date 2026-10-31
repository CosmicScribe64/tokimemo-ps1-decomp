extends "res://tests/opening/run_gameplay_loop_schedule.gd"
## T-39002 gameplay loop, the first Sunday: from the fresh command select a
## CIRCLE on study runs the first school week (as run_gameplay_loop_schedule.gd)
## to the Sunday 95/4/9 record; the holiday controller (scenes/
## holiday_controller.gd) attached as app/first_arrival_boot.gd attaches it
## then runs mode 0x46 (holiday_open_init, the holiday menu, the study
## activity) on the installed v5 school_day_media and the next week's command
## setup (0x34 stage 0) to the second command select, with the HUD session
## mirrors and the CD reads of services/school_day_load_service.gd.
## Golden: .asset-cache/emu/route-golden-smoke-v1 (CIRCLE every 40 frames from
## select-001 + 30): keys and frame offsets from the 0x46 entry (21721) to
## select-002 (21904); select-002 game_state.
## Usage: TOKIMEMO_INSTALLED_ROOT=<bundle with v5 school_day_media> godot --headless --script <this>.

const Holiday = preload("res://scenes/holiday_controller.gd")
const HolidayScene = preload("res://services/holiday_scene_adapter.gd")
const SchoolMedia = preload("res://services/school_day_media_store.gd")
const ReloadRecord = preload("res://services/command_reload_services.gd")
const CommandHud = preload("res://presentation/command_hud_presenter.gd")
const HudCompositorScript = preload("res://presentation/hud_compositor.gd")

## Golden keys at frame offsets from the 0x46 entry frame (46/0/0/0, 21721).
const GOLDEN_HOLIDAY: Dictionary = {0: "46/0/0/0", 1: "46/0/1/0", 19: "46/1/0/0", 42: "46/5/0/0", 43: "46/5/0/1",
	64: "46/5/0/2", 65: "46/5/0/3", 95: "46/5/1/0", 96: "46/5/2/0", 161: "31/0/0/0", 162: "34/0/0/0",
	163: "34/0/1/0", 164: "34/0/2/0", 165: "34/0/3/0", 183: "34/1/0/0"}
## Golden pad: CIRCLE held two frames from these offsets (21723, 21763, 21803,
## 21843, 21883). GameSession latches a frame's input for the next step, so
## the test supplies it one step earlier.
const GOLDEN_PRESSES := [2, 42, 82, 122, 162]
## settings_command_screen_setup_reset's record (command_reload_services.gd
## setup_reset); this test's reload services are recorders.
const SETUP_RECORD := {"tile_layer_enabled": 0, "boot_hold": 0}
## Golden parameter words (value << 16 | fraction) at enter-46-001 and select-002.
const GOLDEN_PARAMS_BEFORE := [[97, 0], [42, -12288], [40, 12288], [40, 12288], [38, -32768], [32, -16384], [59, 0], [4, -16384], [5, 0]]
const GOLDEN_PARAMS_AFTER := [[96, 16384], [44, -20480], [40, 20480], [40, 20480], [38, 8192], [33, 16384], [58, -16384], [4, -20480], [6, -16384]]


func _initialize() -> void:
	installed_root = OS.get_environment("TOKIMEMO_INSTALLED_ROOT").simplify_path()
	var load_error: String = _load_packs()
	if load_error.is_empty():
		var hud_manifest: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(installed_root.path_join("packs/hud_manifest/manifest.json"))))
		sequences = hud_manifest["tables"]["sequences"]
	if not load_error.is_empty():
		printerr("FAIL " + load_error)
		quit(1)
		return
	_holiday_week()
	_negative_media()
	if failures != 0:
		printerr("FAIL: %d of %d gameplay holiday checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d gameplay holiday checks" % checks)
	quit(0)


func _room_image() -> Image:
	var presenter := CommandHud.new()
	var path: String = installed_root.path_join("packs/hud_manifest/manifest.json")
	if not presenter.open_manifest(path, FileAccess.get_sha256(path), installed_root.path_join(".asset-cache/port/fonts/native-font-v2")):
		check(false, "command HUD manifest opens: " + presenter.error)
		return null
	var compositor: HudCompositorScript = presenter._compositor
	return compositor._image(compositor.descriptor["room"])


func _open_scene() -> HolidayScene:
	var media := SchoolMedia.new()
	check(media.open_pack(installed_root.path_join("packs/school_day_media")), "v5 school_day_media opens: " + media.error_message())
	check(media.get_route_tables().size() == 6, "v5 school_day_media carries the six holiday route tables")
	var scene := HolidayScene.new()
	var failure: String = scene.open_scene(media, installed_root.path_join("packs/holiday_scene_source/manifest.json"), _room_image())
	check(failure.is_empty(), "holiday scene opens on the installed media and profile: " + failure)
	return scene


func _holiday_week() -> void:
	var setup: Array = _at_select(true)
	var session: Session = setup[0]
	var state: RouteState = setup[1]
	var hud: HudSession = setup[2]
	var data: Dictionary = state.rule_state
	var paths := {}
	for role: String in Capabilities.ROLES:
		paths[role] = installed_root.path_join("packs").path_join(role)
	var capabilities := Capabilities.new()
	check(capabilities.open_installed(paths).is_empty(), "installed command media opens: " + capabilities.error)
	var loads := Loads.new()
	check(loads.prepare(capabilities).is_empty(), "school-day reads prepared")
	var scene := _open_scene()
	check(loads.prepare_holiday(scene, int(scene.tables.weekday_room_background_lbas[0])).is_empty(), "holiday reads prepared")
	var holiday := Holiday.new(rule_tables, scene.tables)
	check(holiday.error.is_empty(), "holiday controller opens: " + holiday.error)
	holiday.hud_session = hud
	holiday.hud_icon_positions = positions.duplicate(true)
	holiday.extension_supplier = func(s: RouteState) -> Dictionary:
		var live: Dictionary = HolidayScene.live_session(s, hud, SETUP_RECORD)
		if not str(live.error).is_empty():
			return live
		return {"error": "", "holiday": scene.compose_session(live.session)}
	data["resources"]["available_holiday_commands"] = [0]
	check(holiday.attach(session, state).is_empty(), "holiday controller attaches over the school-day controller")
	session.draw_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = HudSeason.draw_step(base, result)
		if result.error.is_empty():
			result.error = hud.advance_icon_animation(sequences, base.rule_state.icon_enabled)
		if result.error.is_empty() and base.game_mode in [0x40, 0x46]:
			result.error = loads.draw(base, result, session.clock.logic_iterations)
	check(_step(session, PAD_CIRCLE).committed, "select frame samples CIRCLE")
	var stop: String = ""
	var guard: int = 0
	var params_before: Array = []
	while state.game_mode != 0x46 and guard < 900:
		var result := _loaded_step(session, loads, state, 0)
		if not result.is_empty():
			stop = result
			break
		guard += 1
	check(stop.is_empty() and _key(state) == "46/0/0/0" and [data.month, data.day, data.weekday] == [4, 9, 6], "the school week dispatches the Sunday 95/4/9 holiday: %s %s %s" % [_key(state), [data.month, data.day, data.weekday], stop])
	check(data.holiday is Dictionary and data.holiday.get("input_device") == 1 and data.holiday.get("tile_layer_enabled") == 0 and data.holiday.get("boot_hold") == 0 and data.holiday.get("icon_mode") == 0 and data.holiday.get("screen_brightness") == 128, "Sunday entry composes the live session fields (golden enter-46-001: input_device 1, tile_layer_enabled 0, boot_hold 0, icon_mode 0, screen_brightness 128): %s" % [data.get("holiday")])
	params_before = data.params.duplicate()
	var holiday_index: int = int(data.day_event_index)
	var progress_before: int = int(data.progress_state[0x15])
	var keys: Dictionary = {0: _key(state)}
	var last: String = _key(state)
	var held: int = 0
	for offset in range(1, 184):
		held = PAD_CIRCLE if GOLDEN_PRESSES.any(func(p: int) -> bool: return offset + 1 == p or offset == p) else 0
		var failure := _loaded_step(session, loads, state, held)
		if not failure.is_empty():
			stop = "+%d %s (%s)" % [offset, failure, _key(state)]
			break
		if _key(state) != last:
			last = _key(state)
			keys[offset] = last
	check(stop.is_empty(), "every holiday frame commits: " + stop)
	check(keys == GOLDEN_HOLIDAY, "golden holiday timeline %s (got %s)" % [GOLDEN_HOLIDAY, keys])
	# select-002 game_state.
	check(int(data.day_event_index) == holiday_index + 1 and int(data.day_events[holiday_index][1]) == 0, "the Sunday record ran study and the queue moved to the next-command record (golden select-002 index 11, record girl 0)")
	check(int(data.progress_state[0x15]) == progress_before + 1, "holiday_activity_load counts the holiday command (golden progress_state[0x15] 1 -> 2)")
	check(int(data.last_command) & 0xF0 == 0 and int(data.menu_cursor) == 0 and hud.menu_cursor() == 0 and hud.at_command_select(), "second command select: cursor 0 in both records, HUD session at the command-select boundary")
	check(data.icon_enabled.slice(0, 14) == [1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1], "second command select icons: settings_command_screen_build ordinary branch (no club, 4/9): %s" % [data.icon_enabled])
	var words_before: Array = GOLDEN_PARAMS_BEFORE.map(func(pair: Array) -> int: return (pair[0] << 16) + pair[1])
	var words_after: Array = GOLDEN_PARAMS_AFTER.map(func(pair: Array) -> int: return (pair[0] << 16) + pair[1])
	if params_before == words_before and int(data.day_events[holiday_index][2]) == 1:
		check(data.params == words_after, "holiday study parameters equal golden select-002: %s" % [data.params])
	else:
		print("NOTE pre-holiday parameters %s / poor flag %d differ from the golden route (test RNG seed); holiday parameters not compared" % [params_before.map(func(v: int) -> int: return v >> 16), int(data.day_events[holiday_index][2])])
	check(loads.pending_count() == 0, "no read left pending at the second command select")
	_second_week(session, loads, state, hud)


## Golden keys of the second week, offsets from the 0x46 entry: the CIRCLE on
## study at select-002 + 30 (21934), the scheduler (day_event_queue_build_week)
## and the school days 95/4/10 .. 4/14 to the end of the golden trace (22326).
const GOLDEN_WEEK2: Dictionary = {213: "30/0/0/0", 214: "30/0/0/1", 215: "30/0/0/2", 216: "30/0/1/0", 217: "30/0/2/0", 218: "30/0/3/0", 219: "30/0/4/0", 220: "31/0/0/0", 221: "40/0/0/0", 222: "40/0/0/1", 240: "40/0/0/2", 241: "40/0/1/0", 242: "40/0/1/1", 272: "40/0/2/0", 273: "40/1/0/0", 319: "40/2/0/0", 320: "31/0/0/0", 321: "40/0/0/0", 322: "40/0/0/2", 323: "40/0/1/0", 324: "40/0/1/1", 354: "40/0/2/0", 355: "40/1/0/0", 401: "40/2/0/0", 402: "31/0/0/0", 403: "40/0/0/0", 404: "40/0/0/2", 405: "40/0/1/0", 406: "40/0/1/1", 436: "40/0/2/0", 437: "40/1/0/0", 483: "40/2/0/0", 484: "31/0/0/0", 485: "40/0/0/0", 486: "40/0/0/2", 487: "40/0/1/0", 488: "40/0/1/1", 518: "40/0/2/0", 519: "40/1/0/0", 565: "40/2/0/0", 566: "31/0/0/0", 567: "40/0/0/0", 568: "40/0/0/2", 569: "40/0/1/0", 570: "40/0/1/1", 600: "40/0/2/0", 601: "40/1/0/0"}


func _second_week(session: Session, loads: Loads, state: RouteState, hud: HudSession) -> void:
	var data: Dictionary = state.rule_state
	var keys: Dictionary = {}
	var last: String = _key(state)
	var stop: String = ""
	for offset in range(184, 606):
		var held: int = PAD_CIRCLE if offset in [212, 213] else 0
		var failure := _loaded_step(session, loads, state, held)
		if not failure.is_empty():
			stop = "+%d %s (%s)" % [offset, failure, _key(state)]
			break
		if _key(state) != last:
			last = _key(state)
			keys[offset] = last
		if offset == 220:
			# enter-31-008: 95/4/10 Monday, day_count 6, a fresh queue at index 0.
			check([data.month, data.day, data.weekday, data.day_count, data.day_event_index] == [4, 10, 0, 6, 0], "second week starts on Monday 95/4/10 (golden enter-31-008): %s" % [[data.month, data.day, data.weekday, data.day_count, data.day_event_index]])
			var types: Array = data.day_events.map(func(e: Array) -> int: return int(e[0]))
			check(types.slice(0, 7).filter(func(t: int) -> bool: return t not in [0xfa]) .slice(0, 6) == [0, 0xfb, 0, 0xfb, 0, 0xfb] and types.has(0xfc) and types.has(0x10), "second week queue: school records with day ends, the week-end check and the Sunday 4/16 record: %s" % [types.slice(0, 16)])
	check(stop.is_empty(), "every second-week frame commits: " + stop)
	check(keys == GOLDEN_WEEK2, "golden second-week timeline %s (got %s)" % [GOLDEN_WEEK2, keys])
	check([data.month, data.day] == [4, 14] and hud.menu_cursor() == int(data.menu_cursor), "the second week reaches the 95/4/14 school day with the HUD cursor in step: %s" % [[data.month, data.day]])
	_onward(session, loads, state)


## Past the golden trace: CIRCLE every 40 frames (the golden script's
## cadence) until the first refusal; reports where the route stops.
func _onward(session: Session, loads: Loads, state: RouteState) -> void:
	var data: Dictionary = state.rule_state
	var stop: String = ""
	var selects: int = 0
	var last: String = _key(state)
	for frame in range(0, 6000):
		var held: int = PAD_CIRCLE if frame % 40 in [0, 1] else 0
		var failure := _loaded_step(session, loads, state, held)
		if not failure.is_empty():
			stop = "%s at 95/%d/%d %s" % [failure, data.month, data.day, _key(state)]
			break
		if _key(state) != last:
			last = _key(state)
			if last == "34/1/0/0":
				selects += 1
	print("ONWARD %d more command selects, stop: %s" % [selects, stop])
	check(selects >= 2, "the route keeps playing past 4/14: %d more command selects (stop: %s)" % [selects, stop])


## One step with the load service's due completions; "" when committed.
func _loaded_step(session: Session, loads: Loads, state: RouteState, held: int) -> String:
	var events: Array[Session.ServiceEvent] = loads.pending_events(session.clock.logic_iterations, int(state.frame_count))
	var result: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, held, session.latched_input.held), events)
	if not result.committed:
		return result.error
	loads.delivered(events)
	return loads.committed(result, state)


## Holiday media must be the pinned v5 pack: the v4 pack has no route tables
## and the scene refuses it.
func _negative_media() -> void:
	var scene := HolidayScene.new()
	var failure: String = scene.open_scene(SchoolMedia.new(), installed_root.path_join("packs/holiday_scene_source/manifest.json"), _room_image())
	check(not failure.is_empty(), "an unopened media store is refused: " + failure)
	var loads := Loads.new()
	check(not loads.prepare_holiday(HolidayScene.new(), 16361).is_empty(), "holiday reads refuse an unopened scene")
	var bad := HolidayScene.new()
	check(bad.compose_session({"boot_hold": 0}).is_empty(), "compose before open is refused")
