extends SceneTree
## Fresh New Game handoff, gameplay side (flow/fresh_gameplay_handoff.gd).
##
## Bounded handoff test, not actual-route acceptance. The session state comes
## from the original installed packs: NAME_ENT rules and content
## (name_entry_rules, name_entry_content), the commit through the production
## Entry controller and state adapter, the prologue constant (prologue_text),
## the HUD icon positions (hud_manifest) and the school-day rule tables
## (rule_tables). Every frame runs through GameSession.step, so
## game_mode_dispatch's post-handler increments happen in source order:
## stage 21 through the production Arrival.step after the source START-skip
## store mode_stage_set(0x15) (prologue.c:2812-2821), the reload (0x34 stage
## 0xFF) through the handoff's 0x34 handler and the production Reload.step,
## then command select through the school-day controller.
##
## The reload services are recording doubles: the bank reads, the HUD read,
## CALROM and the room read are not performed. Their completion frames are the
## ones the original takes on this route (newgame-golden-v1 logic.jsonl, sets
## A and B: 131 bank polls, 73 CD syncs, 18 room polls; stage 0xFF lasts 225
## frames), so the frame-count-dependent HUD and timer state can be compared
## with the golden checkpoints a4-a8. The actual installed route still stops at
## the stage 21 bank transfer (FINISH_BANK_TRANSFER_UNSUPPORTED, T-39002 v2).
##
## Environment: TOKIMEMO_INSTALLED_ROOT = an installed runtime bundle
## (.asset-cache/port/installed-runtime-v13).

const Handoff = preload("res://flow/fresh_gameplay_handoff.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Adapter = preload("res://flow/new_game_state_adapter.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const DayState = preload("res://rules/school_day_state.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const PrologueText = preload("res://services/prologue_text_store.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

const ENTRY_ACTIONS: Array[String] = ["tick", "start", "tick", "start", "tick", "start", "tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]
const PAD_RIGHT := 0x2000

# Golden completion frames of the stage 0xFF services on this route
# (.asset-cache/emu/newgame-golden-v1/runs/set{A,B}/logic.jsonl, identical).
const GOLDEN_BANK_POLLS := 131
const GOLDEN_CD_SYNCS := 73
const GOLDEN_ROOM_POLLS := 18
const GOLDEN_RELOAD_FRAMES := 225
## Pre-dispatch (mode_stage, mode_step, substep) by 0x34 frame index.
const GOLDEN_TIMELINE: Dictionary = {0: [0xFF, 0, 0], 1: [0xFF, 1, 0], 131: [0xFF, 1, 0],
	132: [0xFF, 1, 1], 133: [0xFF, 1, 2], 205: [0xFF, 1, 2], 206: [0xFF, 2, 0],
	207: [0xFF, 3, 0], 224: [0xFF, 3, 0], 225: [1, 0, 0]}
# Golden values (both sets) at a6/a7 (first stage 1 dispatch) and a8.
const GOLDEN_A7_GAME_MODE_TIMER := 226
const GOLDEN_A8_GAME_MODE_TIMER := 406
const GOLDEN_CURSOR := [-132, -43]
const GOLDEN_CURSOR_STYLE_SAVED := 0x9C
const GOLDEN_ICONS := [1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0]
## girls[1] club for the default 1/1 type-A Shiori answers: DRAMA (golden
## game_state.girls[1].flags 0x00100000, a1-a8, both sets). It comes from a
## rand() draw whose BIOS state is not sourced.
const GOLDEN_GIRL1_CLUB := 1
const COMMAND_FRAMES := 180

var checks: int = 0
var failures: int = 0
var installed_root: String = ""
var tables: Dictionary = {}
var content: Dictionary = {}
var rule_tables: Dictionary = {}
var positions: Array = []
var mascot: int = -1
var log_calls: Array = []
var sections: Array[String] = []


func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL " + label)


func _initialize() -> void:
	installed_root = OS.get_environment("TOKIMEMO_INSTALLED_ROOT").simplify_path()
	var load_error: String = _load_packs()
	if not load_error.is_empty():
		printerr("FAIL " + load_error)
		quit(1)
		return
	_success_route()
	_cursor_route()
	_switch_negatives()
	_reload_negatives()
	_static_rules()
	_save_identity()
	check(sections == ["success", "cursor", "switch", "reload", "static", "save"], "every section ran to its end: %s" % [sections])
	if failures != 0:
		printerr("FAIL: %d of %d fresh handoff checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d fresh gameplay handoff checks" % checks)
	quit(0)


static func _integers(value: Variant) -> Variant:
	if value is float and value == floorf(value):
		return int(value)
	if value is Array:
		var out: Array = []
		for item: Variant in value:
			out.append(_integers(item))
		return out
	if value is Dictionary:
		var out: Dictionary = {}
		for key: Variant in value:
			out[key] = _integers(value[key])
		return out
	return value


func _load_packs() -> String:
	if installed_root.is_empty() or not DirAccess.dir_exists_absolute(installed_root.path_join("packs")):
		return "TOKIMEMO_INSTALLED_ROOT must name an installed runtime bundle"
	var packs: String = installed_root.path_join("packs")
	tables = Entry.Rules.load_tables(packs.path_join("name_entry_rules/manifest.json"))
	content = Entry.load_content(packs.path_join("name_entry_content/entry.json"))
	if tables.is_empty() or content.is_empty():
		return "original NAME_ENT rule/content packs failed authentication"
	var text := PrologueText.new()
	var text_root: String = packs.path_join("prologue_text")
	var text_error: String = text.open(text_root, FileAccess.get_sha256(text_root.path_join("manifest.json")))
	if not text_error.is_empty():
		return "prologue text pack: " + text_error
	mascot = int(text.initial_constants().get("initial_mascot", -1))
	var hud: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(packs.path_join("hud_manifest/manifest.json"))))
	if not hud is Dictionary or not hud.get("tables") is Dictionary:
		return "HUD manifest unreadable"
	positions = hud["tables"]["icon_positions"]
	var rules: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(packs.path_join("rule_tables/manifest.json"))))
	if not rules is Dictionary or rules.get("schema") != "tokimemo.port-rule-tables.v1":
		return "rule table pack unreadable"
	rule_tables = rules["tables"]
	return ""


func _recorder(name: String) -> Callable:
	return func(_s: Variant = null, a: Variant = null, b: Variant = null, c: Variant = null, d: Variant = null) -> void:
		log_calls.append([name, a, b, c, d])


func _stage21_services() -> Dictionary:
	var services: Dictionary = {}
	for key: String in Arrival.REQUIRED:
		services[key] = _recorder(key)
	# prologue_finish_run case0 (Arrival.FINISH_SERVICES): cd_command_reset,
	# audio_bank_state_reset, audio commands 5/0xD1 and the five-read
	# audio_bank_load_start; recording doubles like the reload services.
	for key: String in Arrival.FINISH_SERVICES:
		services[key] = _recorder(key)
	return services


## Reload service doubles. They do the mode-field effects the port's Reload
## expects, complete on the golden frames and record every other call; nothing
## here writes a gameplay record.
func _reload_services(build: Callable = Callable()) -> Dictionary:
	var polls: Array[int] = [0, 0, 0]
	var services: Dictionary = {}
	for key: String in ["bank_start", "hud_read_start", "calendar_upload", "mascot_set", "frame_draw", "outer_draw"]:
		services[key] = _recorder(key)
	services["setup_reset"] = func(s: RouteState, _r: Session.FrameResult) -> void:
		log_calls.append(["setup_reset"])
		s.next_step()
	services["bank_poll"] = func(_s: RouteState, _r: Session.FrameResult) -> int:
		polls[0] += 1
		return 1 if polls[0] >= GOLDEN_BANK_POLLS else 0
	services["cd_sync"] = func(_s: RouteState, _r: Session.FrameResult) -> int:
		polls[1] += 1
		return 1 if polls[1] >= GOLDEN_CD_SYNCS else 0
	services["command_build"] = build if build.is_valid() else func(s: RouteState, r: Session.FrameResult) -> void:
		r.error = Reload.build_command_screen(s)
	services["room_finish"] = func(s: RouteState, _r: Session.FrameResult) -> void:
		polls[2] += 1
		if polls[2] >= GOLDEN_ROOM_POLLS:
			Reload.finish_room_load(s)
	return services


func _boot_hud() -> HudSession:
	var hud := HudSession.new() # process_image
	hud.game_state_init(true) # main boot, game_init.c:142
	hud.hud_icon_bar_init(positions) # boot_hud_init (boot_assets.c:136)
	return hud


## Session wiring: 0x15 runs the production Arrival.step; the draw callback
## runs the production HUD season step (app/playable_boot.gd _native_draw).
## held is latched from the start, so every handler sees it with no edge.
func _wire(session: Session, state: RouteState, held: int, draw_failure: Array = []) -> void:
	var services: Dictionary = _stage21_services()
	session.state = state
	session.router.bind_mode(0x15, func(base: Variant, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
		var out: Session.FrameResult = Arrival.step(base, rng, input.pressed, services)
		result.error = out.error
		result.audio.append_array(out.audio)
		result.presentation.append_array(out.presentation)
		result.transitions.append_array(out.transitions))
	session.draw_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = HudSeason.draw_step(base, result)
		if not draw_failure.is_empty() and int(draw_failure[0]) == base.frame_count:
			result.error = "test: refused draw after the handler"
	session.cd_logic = func(_b: Variant, _r: Rng, _res: Session.FrameResult) -> void:
		pass
	session.audio_logic = func(_b: Variant, _r: Rng, _res: Session.FrameResult) -> void:
		pass
	session.latched_input = InputFrame.new(-1, held, held)


func _step(session: Session, held: int) -> Session.FrameResult:
	var events: Array[Session.ServiceEvent] = []
	return session.step(InputFrame.new(session.clock.logic_iterations, held, session.latched_input.held), events)


## Returns [session, entry, state at the switch, projection before substep 1].
func _fresh_switch(held: int = 0, draw_failure: Array = []) -> Array:
	var session := Session.new()
	check(session.rng.configure(Rng.OPENBIOS_PROFILE, 19950404), "native RNG configured")
	var entry := Entry.new()
	check(entry.begin(tables, content, session.rng).is_empty(), "NAME_ENT begins on the original packs")
	for kind: String in ENTRY_ACTIONS:
		var failure: String = entry.act({"kind": kind})
		check(failure.is_empty(), "NAME_ENT action %s: %s" % [kind, failure])
	# frame_count / system_seed are placeholders here; the interactive opening
	# passes the live EntryRuntimeContext words. Nothing below compares them.
	var adapted: Adapter.Outcome = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": mascot})
	check(adapted.error.is_empty() and adapted.state != null, "state adapter accepts the committed entry: " + adapted.error)
	var state: RouteState = adapted.state
	check(state.rule_state.menu_mode == 0 and state.ovl_target_game_mode == 0x15 and state.rule_state.icon_enabled == Handoff.boot_icon_enabled(), "adapter: no-card menu_mode 0, NAME_ENT overlay target 0x15, boot icon_enabled")
	state.set_stage(21) # START skip tail, prologue.c:2812-2821
	_wire(session, state, held, draw_failure)
	var first: Session.FrameResult = _step(session, held)
	check(first.committed and state.substep == 1, "stage 21 substep 0 runs in the session: " + first.error)
	var before_switch: Dictionary = state.projection()
	var second: Session.FrameResult = _step(session, held)
	check(second.committed and state.game_mode == 0x34, "stage 21 substep 1 switches to 0x34 in the session: " + second.error)
	return [session, entry, state, before_switch]


func _clone(state: RouteState) -> RouteState:
	var copy := RouteState.new()
	copy.restore(state.projection())
	return copy


func _success_route() -> void:
	var fresh: Array = _fresh_switch()
	var session: Session = fresh[0]
	var entry: Entry = fresh[1]
	var state: RouteState = fresh[2]
	var draws: int = session.rng.draw_count
	# a4/a5: after the switch frame's game_mode_dispatch increment.
	check([state.game_mode, state.mode_stage, state.mode_step, state.substep, state.game_mode_timer, state.mode_stage_timer, state.mode_step_timer] == [0x34, 0xFF, 0, 0, 1, 0, 0], "switch: 0x34/FF/0/0, game_mode_timer 1, stage/step timers 0 (golden a4/a5)")
	check(int(state.opening.get("D_800B5BC8", 0)) == 0, "D_800B5BC8 0 at the switch (golden a4)")
	var hud: HudSession = _boot_hud()
	check(hud.cursor_style_saved == GOLDEN_CURSOR_STYLE_SAVED, "boot hud_cursor_actor_init stores cursor_style in D_8011ECE8 (golden a1 0x9C)")
	var controller := Controller.new(rule_tables)
	check(controller.error.is_empty(), "school-day controller accepts the original rule tables: " + controller.error)
	var handoff := Handoff.new()
	check(handoff.open(state, hud, positions, controller).is_empty(), "handoff opens at the stage 21 switch: " + handoff.error)
	# NAME_ENT rand() results are reported, not claimed.
	var girl1_club: int = (int(state.rule_state.girls[1].flags) >> 20) & 0xF
	check(handoff.unsourced.has("girls[1].flags.club") and handoff.unsourced.has("girls[2].flags.club") and handoff.unsourced.has("girls[4].flags.club") and not handoff.unsourced.has("girls[3].flags.club"), "rand()-chosen clubs of girls 1, 2 and 4 are reported unsourced; girl 3 is fixed for Shiori club 4: %s" % [handoff.unsourced.keys()])
	check(girl1_club == GOLDEN_GIRL1_CLUB or handoff.unsourced.has("girls[1].flags.club"), "girls[1] club %d differs from the golden DRAMA only as a reported unsourced value" % girl1_club)
	check(handoff.unsourced.has("background_cache") and handoff.unsourced.has("scene_work"), "background cache and scene_work are reported unsourced")
	check(handoff.attach(session, state) == Handoff.NOT_AT_SELECT, "attach before command select is refused")
	var services: Dictionary = _reload_services()
	check(handoff.bind(session, services).is_empty(), "handoff binds the 0x34 handler on the opening session")
	var p0: Array = positions[0]
	var cursor_x: int = p0[0] - 10 + 20 - 4
	var cursor_y: int = p0[1] - 10 + 8
	var timeline: Dictionary = {}
	var fades: Array = []
	var per_frame: Array[String] = []
	var frames: int = 0
	while handoff.phase == "reload" and frames < 400:
		timeline[frames] = [state.mode_stage, state.mode_step, state.substep]
		var step_before: int = state.mode_step
		var result: Session.FrameResult = _step(session, 0)
		check(result.committed, "reload frame %d: %s" % [frames, result.error])
		if not result.committed:
			break
		# Per-frame HUD state against the source order: boot_frame_begin, step,
		# hud_icon_bar_update(0), hud_cursor_update; on the build frame the
		# tail runs after hud_icon_bar_init, so item 0 is hovered again.
		var countdown: int = maxi(0, 32 - frames)
		if hud.menu_current != 0 or hud.hover_index() != 0 or hud.cursor_guard != 1 \
				or [hud.cursor_x, hud.cursor_y] != [cursor_x, cursor_y] or hud.hover_countdown != countdown:
			per_frame.append("frame %d (step %d): current %d hover %d guard %d cursor %d,%d countdown %d (want %d)" % [
				frames, step_before, hud.menu_current, hud.hover_index(), hud.cursor_guard, hud.cursor_x, hud.cursor_y, hud.hover_countdown, countdown])
		fades.append(int(state.opening.get("D_800B5BC8", 0)))
		frames += 1
	timeline[frames] = [state.mode_stage, state.mode_step, state.substep]
	check(per_frame.is_empty(), "HUD state after every reload frame follows the source order: %s" % [per_frame.slice(0, 3)])
	check(handoff.phase == "command_select" and frames == GOLDEN_RELOAD_FRAMES and handoff.reload_frames == frames, "reload reaches command select after the golden 225 frames (got %d)" % frames)
	for index: int in GOLDEN_TIMELINE:
		check(timeline.get(index) == GOLDEN_TIMELINE[index], "golden reload timeline at 0x34 frame %d: %s (got %s)" % [index, str(GOLDEN_TIMELINE[index]), str(timeline.get(index))])
	check(handoff.build_branch == "ordinary", "fresh 1995-04-04 build takes the ordinary branch")
	check(session.rng.draw_count == draws, "stage 21 and the reload draw no random numbers")
	# a6/a7: the first stage 1 dispatch.
	check(state.game_mode_timer == GOLDEN_A7_GAME_MODE_TIMER and state.mode_stage_timer == 0 and state.mode_step_timer == 0, "game_mode_timer %d at command select (golden 226)" % state.game_mode_timer)
	# The calendar frame stores 0xFE; the same frame's tail may already step it.
	var stored: int = -1
	for index: int in fades.size():
		if fades[index] != 0:
			stored = index
			break
	check(stored == 205 and fades[stored] in [0xFE, 0xFF] and fades.slice(stored).all(func(v: int) -> bool: return v in [0xFE, 0xFF, 0]) and fades.slice(stored + 8).all(func(v: int) -> bool: return v == 0), "D_800B5BC8: 0xFE stored with the calendar on frame 205, faded to 0 by hud_screen_frame_draw (from %d: %s)" % [stored, fades.slice(maxi(stored, 0), maxi(stored, 0) + 10)])
	check(state.opening.D_800B5BC8 == 0, "D_800B5BC8 0 at command select (golden a6-a8)")
	check(hud.hover_countdown == 0 and hud.cursor_style_saved == GOLDEN_CURSOR_STYLE_SAVED, "hover countdown 0 and D_8011ECE8 0x9C at command select (golden a6/a7)")
	check([hud.cursor_x, hud.cursor_y] == GOLDEN_CURSOR and hud.menu_count == 14, "cursor at item 0 (golden -132,-43), fourteen items")
	check(handoff.attach(session, _clone(state)) != "", "attach with another state object is refused")
	check(handoff.attach(session, state).is_empty(), "attach hands the same state and session to the controller: " + handoff.error)
	check(session.state == state and session.router.handler(0x34) != Callable(handoff, "mode_handler"), "controller bound 0x34 on the opening session")
	check(controller.hud_session == hud, "the controller keeps the HUD session")
	var data: Dictionary = state.rule_state
	check(data.resources.get(HudSeason.KEY) == 0, "D_800B5BC8 continues as resources.hud_season_fade")
	check(data.resources.get("unsourced") is Dictionary and data.resources.unsourced.has("background_cache"), "unsourced fields travel with the attached state")
	# Core fields at the boundary.
	check([state.game_mode, state.mode_stage, state.mode_step, state.substep] == [0x34, 1, 0, 0], "core boundary 0x34/1/0/0")
	for key: String in ["vacation", "year", "month", "day", "weekday", "day_count", "player_flags_upper",
			"event_log_count", "day_event_index", "last_command", "sd_anim_fast", "day_event_done_bits",
			"day_event_count_today", "daily_check_done", "memory_card_job", "memory_card_busy"]:
		check(data[key] == Handoff.boot_records()[key], "%s = %s" % [key, str(Handoff.boot_records()[key])])
	check([data.year, data.month, data.day, data.weekday] == [0x5F, 4, 4, 1], "calendar date 95/4/4 Tuesday")
	check(data.player_flags == 0xB000, "player_flags: club nibble 0xB (none), health 0")
	check(data.calendar[0].day_kind[0] == 0x1E and data.calendar[0].marker[0] == 5 and data.calendar[0].marker[4] == 1 and data.calendar[1].day_kind[0] == 0x1F and data.calendar[1].day_kind[3] == 2, "boot_calendar_init April/May records")
	check(data.clubs.size() == 12, "twelve ClubRecord entries")
	for i in 12:
		check(data.clubs[i] == {"practice_points": 0, "level": 0, "flags": 0}, "clubs[%d] is bzeroed with practice_points" % i)
	check(data.event_log.size() == 256 and data.event_log_count == 0, "event_log has 256 records, count 0")
	check(data.event_log[0] == {"girl": 0, "flag": 0, "scene": 0, "rank": 0, "year": 0, "month": 0, "day": 0, "weekday": 0} and data.event_log[255] == data.event_log[0], "event_log records are zero")
	check(data.day_events.size() == 32 and data.day_events.all(func(e: Array) -> bool: return e == [0, 0, 0, 0]), "day_events cleared")
	check(data.appointment_words == [0, 0, 0, 0, 0, 0, 0, 0] and data.scene_flags.count(0) == 0xFC and data.girl_last_date_spot_timer.count(0) == 16, "appointment, scene and date-spot records cleared")
	var params: Array = []
	for value: int in [100, 40, 40, 40, 40, 32, 60, 5, 0]:
		params.append(value << 16)
	check(data.params == params, "boot_player_init parameters %s (got %s)" % [str(params), str(data.params)])
	# NAME_ENT results.
	var player: Dictionary = entry.player
	var birthday: int = (player.month + 1) | ((player.day + 1) << 4) | (((player.blood + 1) & 3) << 9)
	check(data.player_birthday == birthday, "player birthday from the NAME_ENT selections")
	check(data.profile_family_bytes[0] != 0 and data.profile_given_bytes[0] != 0 and data.profile_nickname_bytes[0] != 0, "NAME_ENT names stored")
	check((data.girls[0].flags & 2) != 0 and (data.girls[12].flags & 2) != 0 and (data.girls[1].flags & 2) == 0, "met bits of girls 0 and 12 only")
	check((data.girls[0].birthday & 15) == entry.shiori.month + 1 and ((data.girls[0].birthday >> 4) & 31) == entry.shiori.day + 1, "Shiori birthday from the NAME_ENT selections")
	check(not data.has("timer_ticks"), "timer_ticks stays the core word; rule_state holds no copy")
	# Stage 21 and reload stores (golden a6/a7).
	check(data.menu_mode == 1 and state.opening.text_speed == 3 and state.opening.D_800B3DB0 == 0 and state.opening.D_800B5BD4 == 15, "stage 21: menu_mode 1, text speed 3, D_800B3DB0 0, D_800B5BD4 15")
	check(state.opening.screen_brightness == 0x80 and state.opening.command_ready == true, "reload: brightness 0x80, command ready")
	check(data.icon_enabled == GOLDEN_ICONS, "ordinary build icons: 4 (no club), 8/9/10 and 13 (April 4) off")
	check(data.menu_cursor == 0, "menu_cursor_0 = last_command & 15")
	check(state.opening.icon_mode == 0 and state.opening.icon_count == 14, "TypeA fourteen-icon bar")
	var trace: Array = hud.trace()
	check(trace.size() >= 4 and trace[0] == "process_image" and trace[1] == "game_state_init(1)" and trace[-2] == "command_screen_build" and trace[-1] == "enter_command_select", "HUD trace runs process_image -> game_state_init -> build -> command select: %s" % [trace])
	check(hud.at_command_select() and hud.menu_cursor() == 0 and hud.hover_index() == 0, "HUD at command select, cursor 0 hovered")
	# a8: COMMAND_FRAMES idle command frames through the controller.
	var command_errors: Array[String] = []
	for i in COMMAND_FRAMES:
		var frame: Session.FrameResult = _step(session, 0)
		if not frame.committed:
			command_errors.append(frame.error)
			break
	check(command_errors.is_empty() and [state.game_mode, state.mode_stage, state.mode_step] == [0x34, 1, 0], "idle command frames run on the handed-off state: %s" % [command_errors])
	check(state.game_mode_timer == GOLDEN_A8_GAME_MODE_TIMER, "game_mode_timer %d after 180 command frames (golden a8 406)" % state.game_mode_timer)
	check([hud.cursor_x, hud.cursor_y] == GOLDEN_CURSOR and hud.hover_index() == 0 and hud.menu_cursor() == 0 and hud.hover_countdown == 0, "HUD session still driven at a8: cursor, hover and countdown")
	# boot_frame_begin runs at the start of the port handler, so between frames
	# the guard holds hud_cursor_update's 1 (the golden a8 read is after
	# boot_frame_begin, 0). The cursor route shows the guard is cleared.
	check(hud.cursor_guard == 1, "hud_cursor_update ran on the last command frame")
	check(data.resources.get(HudSeason.KEY) == 0, "hud_season_fade stays 0")
	# The first school-day setup reads the unsourced background cache: refused.
	var setup_state: RouteState = _clone(state)
	setup_state.mode_step = 0
	setup_state.substep = 0
	var setup := Session.FrameResult.new()
	controller._setup(setup_state, setup)
	check(setup.error.contains("background_cache is not sourced"), "school-day background lookup refuses the unsourced cache: " + setup.error)
	var late := Session.FrameResult.new()
	handoff.mode_handler(state, InputFrame.new(0), session.rng, late)
	check(late.error == Handoff.NOT_RELOADING, "no reload frame after the handoff")
	sections.append("success")


## Cursor movement shows a missing boot_frame_begin: with right held and no
## edge, every frame moves the cursor 3 past item 0.
func _cursor_route() -> void:
	var fresh: Array = _fresh_switch(PAD_RIGHT)
	var session: Session = fresh[0]
	var state: RouteState = fresh[2]
	var hud: HudSession = _boot_hud()
	var controller := Controller.new(rule_tables)
	var handoff := Handoff.new()
	check(handoff.open(state, hud, positions, controller).is_empty() and handoff.bind(session, _reload_services()).is_empty(), "handoff opens with right held")
	var p0: Array = positions[0]
	var moved: int = mini(p0[0] - 10 + 20 - 4 + 3, 0x8C)
	var wrong: Array[String] = []
	var frames: int = 0
	while handoff.phase == "reload" and frames < 400:
		var result: Session.FrameResult = _step(session, PAD_RIGHT)
		if not result.committed:
			wrong.append("frame %d: %s" % [frames, result.error])
			break
		if hud.cursor_x != moved or hud.menu_current != 0:
			wrong.append("frame %d: cursor %d current %d" % [frames, hud.cursor_x, hud.menu_current])
		frames += 1
	check(wrong.is_empty() and frames == GOLDEN_RELOAD_FRAMES, "every reload frame clears the guard and moves the cursor: %s" % [wrong.slice(0, 3)])
	check(handoff.attach(session, state).is_empty(), "attach after the held-right reload: " + handoff.error)
	for i in 3:
		check(_step(session, PAD_RIGHT).committed and hud.cursor_x == moved, "command frame %d moves the cursor through the controller's HUD mirror" % i)
	# Release, then press right: menu_cursor_update moves both records to item 1.
	check(_step(session, 0).committed, "release frame")
	check(_step(session, PAD_RIGHT).committed and _step(session, PAD_RIGHT).committed, "press frames")
	check(hud.menu_cursor() == 1 and state.rule_state.menu_cursor == 1, "a right press moves the HUD and command cursors together to item 1")
	sections.append("cursor")


func _switch_negatives() -> void:
	var fresh: Array = _fresh_switch()
	var state: RouteState = fresh[2]
	var before_switch: Dictionary = fresh[3]
	var controller := Controller.new(rule_tables)
	var open := func(s: Variant, h: Variant) -> String:
		return Handoff.new().open(s, h, positions, controller)
	# Missing NAME_ENT result.
	for key: String in ["profile_family_bytes", "profile_given_bytes", "profile_nickname_bytes"]:
		var blank: RouteState = _clone(state)
		var zeros: Array = []
		zeros.resize((blank.rule_state[key] as Array).size())
		zeros.fill(0)
		blank.rule_state[key] = zeros
		check(String(open.call(blank, _boot_hud())).begins_with(Handoff.NAME_ENTRY_MISSING), "missing NAME_ENT %s is refused" % key)
	var no_names: RouteState = _clone(state)
	no_names.rule_state.erase("profile_given_bytes")
	check(String(open.call(no_names, _boot_hud())).begins_with(Handoff.NAME_ENTRY_MISSING), "absent name buffer is refused")
	# Captured boundary in the fresh path.
	var captured := DayState.new()
	captured.rule_state = state.rule_state.duplicate(true)
	captured.game_mode = 0x34
	captured.mode_stage = 0xFF
	check(String(open.call(captured, _boot_hud())).begins_with(Handoff.CAPTURED_BOUNDARY), "captured-boundary State is refused")
	var foreign: RouteState = _clone(state)
	foreign.opening["origin"] = "tokimemo.development-boundary.v1"
	check(String(open.call(foreign, _boot_hud())).begins_with(Handoff.CAPTURED_BOUNDARY), "non-fresh origin is refused")
	var interactive: RouteState = _clone(state)
	interactive.opening["origin"] = "interactive-entry-owned-state-v1"
	check(String(open.call(interactive, _boot_hud())).is_empty(), "the interactive opening's origin is a fresh route")
	var pinned: RouteState = _clone(state)
	pinned.opening["boundary_sha256"] = "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5"
	check(String(open.call(pinned, _boot_hud())).begins_with(Handoff.CAPTURED_BOUNDARY), "captured boundary identity is refused")
	var unlisted: RouteState = _clone(state)
	unlisted.opening.erase("unsourced")
	check(String(open.call(unlisted, _boot_hud())).contains("unsourced"), "a state without the unsourced-field record is refused")
	check(Handoff.captured_input_error({"TOKIMEMO_HUD_CONTEXT": "/x/context.json"}).begins_with(Handoff.CAPTURED_BOUNDARY), "captured HUD context launch input is refused")
	check(Handoff.captured_input_error({"TOKIMEMO_BOUNDARY": "/x/boundary.json"}).begins_with(Handoff.CAPTURED_BOUNDARY), "captured boundary launch input is refused")
	check(Handoff.captured_input_error({"TOKIMEMO_BOUNDARY": "", "TOKIMEMO_HUD_CONTEXT": ""}).is_empty(), "empty launch inputs pass")
	check(String(open.call(state, null)) == Handoff.HUD_NOT_FRESH, "no HUD session is refused")
	# Wrong mode or dispatch position.
	var before: RouteState = RouteState.new()
	before.restore(before_switch)
	check(String(open.call(before, _boot_hud())) == Handoff.NOT_AT_SWITCH, "stage 21 substep 1 (before the switch) is refused")
	var outside: RouteState = _clone(state)
	outside.game_mode_timer = 0
	check(String(open.call(outside, _boot_hud())) == Handoff.NOT_AT_SWITCH, "game_mode_timer 0 (stage 21 run outside game_mode_dispatch) is refused")
	var no_target: RouteState = _clone(state)
	no_target.ovl_target_game_mode = 0
	check(String(open.call(no_target, _boot_hud())) == Handoff.NOT_AT_SWITCH, "ovl_target_game_mode other than 0x15 is refused")
	var later: RouteState = _clone(state)
	later.set_stage(1)
	check(String(open.call(later, _boot_hud())) == Handoff.NOT_AT_SWITCH, "0x34 stage 1 is refused")
	var menu: RouteState = _clone(state)
	menu.rule_state["menu_mode"] = 0
	check(String(open.call(menu, _boot_hud())) == Handoff.NOT_AT_SWITCH, "menu_mode 0 (stage 21 store missing) is refused")
	var speed: RouteState = _clone(state)
	speed.opening["text_speed"] = 2
	check(String(open.call(speed, _boot_hud())) == Handoff.NOT_AT_SWITCH, "text speed 2 is refused")
	var other_mode: RouteState = _clone(state)
	other_mode.reset_game_mode_fields(0x15)
	other_mode.set_stage(0xFF)
	check(String(open.call(other_mode, _boot_hud())) == Handoff.NOT_AT_SWITCH, "mode 0x15 is refused")
	# Boot records.
	var clubs: RouteState = _clone(state)
	clubs.rule_state["clubs"][3]["practice_points"] = 5
	check(String(open.call(clubs, _boot_hud())).contains("clubs"), "a club with practice points is refused")
	var logged: RouteState = _clone(state)
	logged.rule_state["event_log_count"] = 1
	check(String(open.call(logged, _boot_hud())).contains("event_log_count"), "a non-zero event log count is refused")
	var no_clubs: RouteState = _clone(state)
	no_clubs.rule_state.erase("clubs")
	check(String(open.call(no_clubs, _boot_hud())).contains("clubs"), "a session without clubs is refused")
	var icons: RouteState = _clone(state)
	icons.rule_state["icon_enabled"] = Handoff._zeros(15)
	check(String(open.call(icons, _boot_hud())).contains("icon_enabled"), "icon_enabled other than the boot hud_icon_bar_init value is refused")
	# HUD session not fresh.
	var bare := HudSession.new()
	check(String(open.call(state, bare)) == Handoff.HUD_NOT_FRESH, "HUD session without game_state_init is refused")
	var built: HudSession = _boot_hud()
	built.command_screen_build(positions, 0, true)
	check(String(open.call(state, built)) == Handoff.HUD_NOT_FRESH, "already built HUD session is refused")
	check(Handoff.new().open(state, _boot_hud(), positions.slice(0, 13), controller) != "", "thirteen icon positions are refused")
	check(Handoff.new().open(state, _boot_hud(), positions, null) != "", "missing controller is refused")
	check(_clone(state).projection() == state.projection(), "refusals leave the state unchanged")
	sections.append("switch")


func _reload_negatives() -> void:
	# A foreign menu 0 (for example a forwarded file-select menu_set) makes the
	# pre-build HUD tail unmirrorable: the first frame is refused.
	var fresh: Array = _fresh_switch()
	var session: Session = fresh[0]
	var state: RouteState = fresh[2]
	var controller := Controller.new(rule_tables)
	var foreign_hud: HudSession = _boot_hud()
	foreign_hud.menu_set([[-144, -18, 32, 16]])
	var foreign_trace: Array = foreign_hud.trace()
	var handoff := Handoff.new()
	check(handoff.open(state, foreign_hud, positions, controller).is_empty() and handoff.bind(session, _reload_services()).is_empty(), "handoff opens with a foreign menu 0")
	var snapshot: Dictionary = state.projection()
	var refused: Session.FrameResult = _step(session, 0)
	check(not refused.committed and not refused.error.is_empty() and state.projection() == snapshot and handoff.phase == "failed", "foreign menu 0 refuses the frame and the session restores the state: " + refused.error)
	check(foreign_hud.trace() == foreign_trace and foreign_hud.menu_count == 1 and foreign_hud.intact(), "the refused frame left the HUD session as it was")
	check(_step(session, 0).error == Handoff.FAILED, "a failed handoff refuses later frames")
	check(handoff.attach(session, state) == Handoff.FAILED, "a failed handoff refuses attach")
	# A reload build that diverges from settings_command_screen_build.
	var bad_fresh: Array = _fresh_switch()
	var bad_session: Session = bad_fresh[0]
	var bad_state: RouteState = bad_fresh[2]
	var diverging := func(s: RouteState, r: Session.FrameResult) -> void:
		r.error = Reload.build_command_screen(s)
		s.rule_state["icon_enabled"][13] = 1
	var bad := Handoff.new()
	var bad_hud: HudSession = _boot_hud()
	check(bad.open(bad_state, bad_hud, positions, controller).is_empty() and bad.bind(bad_session, _reload_services(diverging)).is_empty(), "handoff opens for the diverging build")
	var error: String = ""
	var at_build: Dictionary = {}
	for i in 400:
		if bad_state.mode_step == 2:
			at_build = bad_state.projection()
		var result: Session.FrameResult = _step(bad_session, 0)
		if not result.committed:
			error = result.error
			break
	check(error.contains("differs from settings_command_screen_build") and bad_state.projection() == at_build, "diverging build is refused and the build frame rolled back: " + error)
	check(not bad_hud.trace().has("command_screen_build"), "refused build never reached the HUD session")
	# A later phase of a frame fails after the handler advanced the HUD session:
	# the next handler call sees the rolled-back frame_count and ends the handoff.
	var draw_failure: Array = []
	var late_fresh: Array = _fresh_switch(0, draw_failure)
	var late_session: Session = late_fresh[0]
	var late_state: RouteState = late_fresh[2]
	var late := Handoff.new()
	check(late.open(late_state, _boot_hud(), positions, controller).is_empty() and late.bind(late_session, _reload_services()).is_empty(), "handoff opens for the rolled-back frame case")
	check(_step(late_session, 0).committed, "first reload frame commits")
	draw_failure.append(late_state.frame_count + 1) # draw sees the incremented count
	check(not _step(late_session, 0).committed, "draw refusal rolls the frame back after the handler ran")
	draw_failure.clear()
	var after: Session.FrameResult = _step(late_session, 0)
	check(not after.committed and after.error.contains("rolled back") and late.phase == "failed", "the next frame is refused because the HUD session is ahead: " + after.error)
	sections.append("reload")


func _static_rules() -> void:
	var boot: Dictionary = Handoff.boot_records()
	var data: Dictionary = {"day": 4, "year": 0x5F, "month": 4, "calendar": boot["calendar"].duplicate(true),
		"player_flags": 0xB000, "player_flags_upper": 0, "last_command": 0x13}
	check(Handoff.source_build_branch(data) == "ordinary", "boot April markers give the ordinary branch")
	data["calendar"][0]["marker"][5] = 5
	check(Handoff.source_build_branch(data) == "test", "marker 5 tomorrow is the exam branch")
	data["calendar"][0]["marker"][5] = 8
	check(Handoff.source_build_branch(data) == "ordinary", "marker 8 without a club stays ordinary")
	data["player_flags"] = 0x6000
	check(Handoff.source_build_branch(data) == "camp", "marker 8 with a club is the camp branch")
	data["calendar"][0]["marker"][5] = 0
	data["player_flags"] = 0xB300
	check(Handoff.source_build_branch(data) == "sick", "health 3 is the sick branch")
	data["player_flags"] = 0xB000
	data["player_flags_upper"] = 1
	check(Handoff.source_build_branch(data) == "remedial", "player_flags_upper bit 0 is the remedial branch")
	# marker[day - 1 + 7] past month_calendar[0].marker reads
	# month_calendar[1].day_kind (T-39002): day 31 reads next day_kind[5].
	data["player_flags_upper"] = 0
	data["day"] = 31
	data["calendar"][1]["day_kind"][5] = 0xC
	data["player_flags"] = 0x2000
	check(Handoff.source_build_branch(data) == "festival", "day 31 marker[37] is month_calendar[1].day_kind[5]")
	data["calendar"][1]["day_kind"][5] = 0
	var single: Dictionary = data.duplicate(true)
	single["calendar"] = [data["calendar"][0]]
	check(Handoff.source_build_branch(single).begins_with("unsupported"), "without month_calendar[1] the read past the marker array is unsupported")
	var built: Dictionary = Handoff.source_ordinary_build({"player_flags": 0x2000, "year": 0x5F, "month": 4, "day": 5, "last_command": 0x13}, 2, 14, 1)
	check(built["icon_enabled"] == [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0] and built["menu_cursor"] == 3, "icon mode 2 clears 14, a club keeps 4, other days keep 13, cursor = last_command & 15")
	# hud_screen_frame_draw's D_800B5BC8 step on the pre-increment frame_count.
	var opening: Dictionary = {"D_800B5BC8": 0xFE}
	Handoff.hud_screen_fade(opening, 2)
	check(opening.D_800B5BC8 == 0xFE, "no fade step unless frame_count & 3 == 3")
	Handoff.hud_screen_fade(opening, 3)
	Handoff.hud_screen_fade(opening, 7)
	check(opening.D_800B5BC8 == 0, "0xFE -> 0xFF -> 0x00 on two frames with low bits 3")
	Handoff.hud_screen_fade(opening, 11)
	check(opening.D_800B5BC8 == 0, "0 stays 0")
	# hud_cursor_actor_init's unk_18 rule through the SQUARE cycle.
	var hud: HudSession = _boot_hud()
	for i in 0x10:
		hud.begin_outer_frame()
		hud.hud_cursor_update(0, 0x80)
	check(hud.cursor_style == 0xAC and hud.cursor_style_saved == 0xAC, "SQUARE advances cursor_style and D_8011ECE8 to 0xAC")
	hud.begin_outer_frame()
	hud.hud_cursor_update(0, 0x80)
	check(hud.cursor_style == 0 and hud.cursor_style_saved == 0, "0xAC wraps to 0")
	hud.game_state_init(false)
	check(hud.cursor_style_saved == 0, "hud_cursor_actor_init stores 0 for cursor_style 0")
	hud.begin_outer_frame()
	hud.hud_cursor_update(0, 0x80)
	hud.game_state_init(false)
	check(hud.cursor_style == 0x9B and hud.cursor_style_saved == 0x9B, "hud_cursor_actor_init stores cursor_style 0x9B")
	sections.append("static")


## T-39007 / review R13: the fresh New Game launch check and the save-control
## preflight (playable_boot.gd _load_inputs, then save_controls.configure) can
## both pass in one environment.
func _save_identity() -> void:
	var keys: Array[String] = Handoff.CAPTURED_INPUTS.duplicate()
	keys.append_array(["TOKIMEMO_BOUNDARY_SHA", "TOKIMEMO_TABLES_SHA", "TOKIMEMO_MEDIA_PACK"])
	var saved: Dictionary = {}
	for key: String in keys:
		saved[key] = OS.get_environment(key) if OS.has_environment(key) else null
		OS.unset_environment(key)
	var media: String = OS.get_user_data_dir().path_join("fresh-handoff-media")
	DirAccess.make_dir_recursive_absolute(media)
	var manifest := FileAccess.open(media.path_join("manifest.json"), FileAccess.WRITE)
	manifest.store_string("{}")
	manifest.close()
	OS.set_environment("TOKIMEMO_MEDIA_PACK", media)
	OS.set_environment("TOKIMEMO_BOUNDARY_SHA", "a".repeat(64))
	OS.set_environment("TOKIMEMO_TABLES_SHA", "b".repeat(64))
	check(Handoff.launch_input_error().is_empty(), "a New Game launch with the save content identity passes the captured-input check")
	var controls := Controls.new()
	var host := Node.new()
	check(controls.configure(host, OS.get_user_data_dir().path_join("fresh-handoff-saves")).is_empty(), "save_controls.configure accepts the same environment")
	host.free()
	for key: String in Handoff.CAPTURED_INPUTS:
		OS.set_environment(key, "/x/captured")
		check(Handoff.launch_input_error().begins_with(Handoff.CAPTURED_BOUNDARY), "%s is refused for a New Game launch" % key)
		OS.unset_environment(key)
	for key: String in keys:
		if saved[key] == null:
			OS.unset_environment(key)
		else:
			OS.set_environment(key, saved[key])
	sections.append("save")
