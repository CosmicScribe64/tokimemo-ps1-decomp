extends "res://tests/opening/run_fresh_gameplay_handoff.gd"
## T-39002 gameplay loop: from the fresh command select (the same stage 21
## switch and source reload as run_fresh_gameplay_handoff.gd) a CIRCLE on the
## hovered icon 0 (study) runs schedule_init (0x30) and the day screen (0x31)
## into the first school day (0x40) under the school-day controller, with the
## HUD session's hud_icon_bar_update(0) tail (scheduler.c:66-71, 1534-1540)
## and the app's logical draw (hud_screen_frame_draw fade step, then
## actor_draw_all of the icon actors).
## Golden: .asset-cache/emu/route-golden-smoke-v1 (original game from the set A
## command select): the confirm frame enters 30/0/0/0, then one key per frame
## 30/0/0/1, 30/0/0/2, 30/0/1/0 .. 30/0/4/0, 31/0/0/0, 40/0/0/0; enter-31-001
## is 95/4/5 weekday 2 day_count 1; D_800B5AA0, D_800B5A9C and D_800B5BC8 are
## 0 at enter-30-001, enter-31-001 and enter-40-001.
## Usage: TOKIMEMO_INSTALLED_ROOT=<bundle> godot --headless --script <this>.

const CommandScreen = preload("res://flow/command_screen.gd")
const Capabilities = preload("res://services/school_day_command_capabilities.gd")
const Loads = preload("res://services/school_day_load_service.gd")
## Golden route-golden-smoke-v1 transitions, frames after the first 0x40
## entry (40/0/0/0 at 21371): day 1 (Wednesday 95/4/5) reads the background
## (+1..+19) and the SD set (+21..+51), animates (+52..+98) and exits; day 2
## (95/4/6) finds the background cached (40/0/0/2 at once) and rereads the SD.
const GOLDEN_DAY: Dictionary = {1: "40/0/0/1", 19: "40/0/0/2", 20: "40/0/1/0", 21: "40/0/1/1", 51: "40/0/2/0", 52: "40/1/0/0",
	98: "40/2/0/0", 99: "31/0/0/0", 100: "40/0/0/0", 101: "40/0/0/2", 102: "40/0/1/0", 103: "40/0/1/1", 133: "40/0/2/0",
	134: "40/1/0/0", 180: "40/2/0/0", 181: "31/0/0/0"}
## Golden parameters (integer parts) at enter-31-002, after day 1, when day 1's
## queue record is the golden [0, 0, 0, 15].
const GOLDEN_PARAMS_DAY1 := [99, 40, 40, 40, 39, 32, 59, 4, 0]
const PAD_CIRCLE := 0x20
## LCG steps of the golden rand word between select-001 and enter-31-001:
## per school day one hud_command_roll_poor_result draw and one encounter roll
## (scheduler.c:594, 658), plus the Miharu roll of each resolved encounter
## (golden queue: one 0xFA record).
const GOLDEN_SCHEDULER_DRAWS := 9
const GOLDEN_KEYS := ["30/0/0/0", "30/0/0/1", "30/0/0/2", "30/0/1/0", "30/0/2/0", "30/0/3/0", "30/0/4/0", "31/0/0/0", "40/0/0/0"]

var sequences: Array = []


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
	_schedule_route(0, "study, no cursor input")
	_schedule_route(PAD_RIGHT, "RIGHT held through the scheduler")
	_tail_negative()
	_school_day_reads()
	if failures != 0:
		printerr("FAIL: %d of %d gameplay loop schedule checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d gameplay loop schedule checks" % checks)
	quit(0)


## Command select reached through the production reload, then attached.
func _at_select(names_media: bool = false) -> Array:
	var fresh: Array = _fresh_switch()
	var session: Session = fresh[0]
	var state: RouteState = fresh[2]
	if names_media:
		# The opening's media owner record at the switch (golden a1-a5
		# bg_lba_cache [0, 0, 0x4973, 0x496A]); the live route has it from the
		# prologue's dec_bg_cd_read calls, this test has no prologue.
		state.opening["names_media"] = {"buffer_0": {"lba": 0x4973}, "buffer_1": {"lba": 0x496A}}
	var hud: HudSession = _boot_hud()
	var controller := Controller.new(rule_tables)
	var handoff := Handoff.new()
	check(handoff.open(state, hud, positions, controller).is_empty() and handoff.bind(session, _reload_services()).is_empty(), "handoff opens and binds: " + handoff.error)
	var frames: int = 0
	while handoff.phase == "reload" and frames < 400:
		if not _step(session, 0).committed:
			break
		frames += 1
	check(handoff.phase == "command_select" and handoff.attach(session, state).is_empty(), "command select reached and attached: " + handoff.error)
	# Lane B capability source (services/school_day_command_capabilities.gd) is
	# tested on its own; the confirm here needs only command 0 offered.
	controller.command_capabilities.assign([0, 1, 2, 3, 5, 6, 7])
	# The app's logical draw for gameplay frames (app/first_arrival_boot.gd).
	session.draw_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = HudSeason.draw_step(base, result)
		if result.error.is_empty():
			result.error = hud.advance_icon_animation(sequences, base.rule_state.icon_enabled)
	return [session, state, hud, controller]


func _key(state: RouteState) -> String:
	return "%02x/%d/%d/%d" % [state.game_mode, state.mode_stage, state.mode_step, state.substep]


func _schedule_route(held_after_confirm: int, label: String) -> void:
	var setup: Array = _at_select()
	var session: Session = setup[0]
	var state: RouteState = setup[1]
	var hud: HudSession = setup[2]
	var data: Dictionary = state.rule_state
	# The pad is latched for the next frame (draw_frame's pad_update): the
	# select frame samples CIRCLE, the following frame's handler confirms.
	check(_step(session, PAD_CIRCLE).committed and _key(state) == "34/1/0/0", label + ": select frame samples CIRCLE")
	var draws_at_select: int = session.rng.draw_count
	var keys: Array[String] = []
	var errors: Array[String] = []
	for i in GOLDEN_KEYS.size():
		var result: Session.FrameResult = _step(session, held_after_confirm)
		if not result.committed:
			errors.append(result.error)
			break
		keys.append(_key(state))
		if i == 0:
			check(data.last_command & 15 == 0, label + ": confirm stores command 0 in last_command")
			var kinds: Array = result.audio.map(func(a: Dictionary) -> String: return "%s:%s" % [a.kind, str(a.get("command", a.get("sequence", "")))])
			check(kinds == ["native_cd_audio_command:9", "native_audio_command:3", "native_restore_bgm:0", "native_audio_command:1280"], label + ": confirm audio is cd_audio_command(9), audio_command(3), restore, audio_command(0x500): %s" % [kinds])
		check(hud.menu_cursor() == int(data.menu_cursor), label + ": HUD and rule cursor agree after %s" % keys[-1])
	check(errors.is_empty(), label + ": every frame commits: %s" % [errors])
	check(keys == GOLDEN_KEYS, label + ": golden key per frame %s (got %s)" % [GOLDEN_KEYS, keys])
	check([data.year, data.month, data.day, data.weekday, data.day_count] == [95, 4, 5, 2, 1], label + ": day screen on 95/4/5 weekday 2 day_count 1 (golden enter-31-001)")
	check(data.resources.get(HudSeason.KEY) == 0, label + ": D_800B5BC8 0 (golden)")
	# Golden OpenBIOS rand word (kernel RAM 0x850C): select-001 3553743518 ->
	# enter-31-001 2896535927 is 9 LCG steps, all in the scheduler's queue
	# build; no other draw through the 0x40 entry (enter-40-001 unchanged).
	# The test seed is not the route's, so the draws are checked against the
	# source rule applied to this queue: one hud_command_roll_poor_result
	# draw per school day, one encounter roll per school day while fewer than
	# two encounters were queued (scheduler.c:657-662), and one Miharu roll per
	# encounter resolved to 0xFA. The golden queue (enter-31-006) has four
	# school days, one 0xFA: 4 + 4 + 1 = 9 = GOLDEN_SCHEDULER_DRAWS.
	var expected: int = 0
	var hits: int = 0
	var events: Array = data.day_events
	for index in events.size():
		if int(events[index][0]) != 0:
			continue
		expected += 1
		if hits < 2:
			expected += 1
			if index + 1 < events.size() and int(events[index + 1][0]) in [0x44, 0xfa]:
				hits += 1
				expected += 1
	check(session.rng.draw_count - draws_at_select == expected, label + ": %d RNG draws from select to the first school day (source rule on this queue: %d; golden 9)" % [session.rng.draw_count - draws_at_select, expected])
	if held_after_confirm == 0:
		check(hud.hover_countdown == 0 and hud.hover_index() == 0 and int(data.menu_cursor) == 0, label + ": D_800B5AA0 0, D_800B5A9C/menu_cursor 0 (golden enter-40-001)")
	else:
		# The RIGHT edge reaches the second scheduler frame: its tail
		# hud_icon_bar_update(0) -> menu_cursor_update(0) moves both cursors to
		# icon 1 once; held without a new edge it stays.
		check(int(data.menu_cursor) == 1 and hud.menu_cursor() == 1 and hud.hover_index() == 1, label + ": one RIGHT edge in the scheduler moves the cursor to 1 (got %d/%d)" % [int(data.menu_cursor), hud.menu_cursor()])
	check(state.game_mode == 0x40 and state.mode_stage == 0 and state.mode_step == 0 and state.substep == 0, label + ": first school day entered")


## School day 1 (study): the two CD reads answered by the installed command
## media through services/school_day_load_service.gd on the source latency
## model, with the HUD session's school_day_screen_init and
## hud_icon_bar_update(1) tail, up to the actor stage.
func _school_day_reads() -> void:
	var setup: Array = _at_select(true)
	var session: Session = setup[0]
	var state: RouteState = setup[1]
	var hud: HudSession = setup[2]
	var controller: Controller = setup[3]
	var data: Dictionary = state.rule_state
	check(controller.hud_icon_positions == positions, "attach hands the TypeA icon positions to the controller")
	var paths := {}
	for role: String in Capabilities.ROLES:
		paths[role] = installed_root.path_join("packs").path_join(role)
	var capabilities := Capabilities.new()
	check(capabilities.open_installed(paths).is_empty(), "installed command media opens: " + capabilities.error)
	var loads := Loads.new()
	check(loads.prepare(capabilities).is_empty(), "school-day reads prepared on the installed media")
	# The app's logical draw with the school-day actor owner.
	session.draw_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = HudSeason.draw_step(base, result)
		if result.error.is_empty():
			result.error = hud.advance_icon_animation(sequences, base.rule_state.icon_enabled)
		if result.error.is_empty() and base.game_mode == 0x40:
			result.error = loads.draw(base, result, session.clock.logic_iterations)
	check(_step(session, PAD_CIRCLE).committed, "reads: select frame samples CIRCLE")
	var guard: int = 0
	while state.game_mode != 0x40 and guard < 20:
		if not _step(session, 0).committed:
			break
		guard += 1
	check(_key(state) == "40/0/0/0" and data.background_cache == [0, 0, 0x4973, 0x496A], "reads: 0x40 entry with the sourced background cache: %s %s" % [_key(state), str(data.background_cache)])
	var keys: Dictionary = {}
	var errors: Array[String] = []
	var last: String = _key(state)
	var day1_event: Array = (data.day_events[0] as Array).duplicate()
	var params_day1: Array = []
	for offset in range(1, 182):
		var events: Array[Session.ServiceEvent] = loads.pending_events(session.clock.logic_iterations, int(state.frame_count))
		var result: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0, session.latched_input.held), events)
		if not result.committed:
			errors.append("+%d %s" % [offset, result.error])
			break
		loads.delivered(events)
		var queued: String = loads.committed(result, state)
		if not queued.is_empty():
			errors.append("+%d %s" % [offset, queued])
			break
		if offset == 1:
			check(hud.trace()[-1] == "school_day_screen_init" and hud.hover_index() == 0 and hud.menu_cursor() == 0, "reads: screen init rebuilt the icon bar in the HUD session: %s" % [hud.trace().slice(-2)])
		if _key(state) != last:
			last = _key(state)
			keys[offset] = last
			if last == "31/0/0/0" and params_day1.is_empty():
				params_day1 = data.params.map(func(v: int) -> int: return v >> 16)
	check(errors.is_empty(), "reads: every frame commits: %s" % [errors])
	check(keys == GOLDEN_DAY, "reads: golden two-day timeline %s (got %s)" % [GOLDEN_DAY, keys])
	check(data.icon_enabled.slice(0, 14) == [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], "reads: only the study icon stays enabled (golden enter-40-001 icon_enabled)")
	check(int(data.background_cache[data.background_show]) == 16131 and int(data.scene_work) == 3, "reads: study background 16131 shown and SD set 3 loaded")
	check(loads.pending_count() == 0, "reads: no read left pending")
	# The queue's 0xFA records depend on the scheduler's RNG draws (this test's
	# seed is not the route's), so only the date is compared.
	check([data.year, data.month, data.day, data.day_count] == [95, 4, 6, 2] and int(data.day_events[data.day_event_index - 1][0]) == 0, "reads: two school days done, day screen of 95/4/6 after a school record: %s" % [[data.year, data.month, data.day, data.day_count, data.day_event_index]])
	# The rest of the first week: days 3 and 4, the week-end check (0x32) and
	# the Sunday dispatch. The queue here follows this test's RNG seed.
	var seen32: bool = false
	var stop: String = ""
	for offset in range(182, 600):
		var events: Array[Session.ServiceEvent] = loads.pending_events(session.clock.logic_iterations, int(state.frame_count))
		var result: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0, session.latched_input.held), events)
		if not result.committed:
			stop = "%s at +%d (%s)" % [result.error, offset, _key(state)]
			break
		loads.delivered(events)
		var queued: String = loads.committed(result, state)
		if not queued.is_empty():
			stop = queued
			break
		seen32 = seen32 or state.game_mode == 0x32
		if state.game_mode not in [0x31, 0x32, 0x40]:
			stop = "left the school week into mode 0x%02x at +%d (%s)" % [state.game_mode, offset, _key(state)]
			break
	check(seen32 and hud.menu_cursor() == int(data.menu_cursor), "week: the week-end check ran with the HUD tail (stop: %s)" % stop)
	# Next wall (not modeled here): the Sunday queue record 0x10 needs the
	# holiday capability (resources.available_holiday_commands, mode 0x46).
	check([data.month, data.day, data.weekday] == [4, 9, 6] and _key(state) == "31/0/0/0" and stop.begins_with("ordinary-week next branch requires native type0x10"), "week: the school week ends at the Sunday 95/4/9 holiday record: %s %s %s" % [[data.month, data.day, data.weekday], _key(state), stop])
	if day1_event == [0, 0, 0, 15]:
		check(params_day1 == GOLDEN_PARAMS_DAY1, "reads: day 1 parameters %s (golden enter-31-002 %s)" % [params_day1, GOLDEN_PARAMS_DAY1])
	else:
		print("NOTE day 1 queue record %s differs from the golden [0, 0, 0, 15] (test RNG seed); parameters not compared" % [day1_event])


## A HUD session that disagrees with the rule cursor is refused unchanged.
func _tail_negative() -> void:
	var setup: Array = _at_select()
	var session: Session = setup[0]
	var state: RouteState = setup[1]
	var hud: HudSession = setup[2]
	check(_step(session, PAD_CIRCLE).committed and _step(session, 0).committed and state.game_mode == 0x30, "negative: confirm enters 0x30")
	state.rule_state["menu_cursor"] = 3
	var before: Dictionary = state.projection()
	var hud_before: Dictionary = hud.checkpoint()
	var refused: Session.FrameResult = _step(session, 0)
	check(not refused.committed and refused.error.contains("differs from the rule cursor"), "negative: diverged cursors are refused: " + refused.error)
	check(state.projection() == before and hud.checkpoint() == hud_before, "negative: state and HUD session unchanged after the refusal")
