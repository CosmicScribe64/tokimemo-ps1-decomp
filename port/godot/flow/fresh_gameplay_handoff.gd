extends RefCounted
## Gameplay side of the fresh New Game handoff (opening stage 21 -> command
## select -> school-day controller). Source: src/ovl/name_entry/prologue.c
## prologue_finish_run substep 1 (k_speed_set(3), menu_mode = MENU_MODE_A,
## game_mode_set(0x34), mode_stage_set(0xFF)), src/main/home_screens.c
## settings_command_screen_mode_stage_reload (steps 0-3) and
## settings_command_screen_build, src/main/game_init.c game_state_init and
## boot_game_data_init. Spec: wiki/spec/opening-stages-21-end.md.
##
## The session state is not rebuilt here. On the original, GameState is built
## once by game_state_init(1) -> boot_game_data_init at boot and by the
## NAME_ENT commit (name_entry_new_game_setup); the prologue writes none of
## the gameplay records. The handoff takes that state at the stage 21 mode
## switch, checks that every record with a single boot writer still holds the
## boot value, drives the HUD session mirrors through the reload in source
## order and attaches the same state, session and RNG to the gameplay
## controller. It never reads a captured development boundary or a captured
## HUD context.
##
## The reload runs inside GameSession.step: the owner binds mode_handler as the
## 0x34 handler (bind), so game_mode_dispatch's post-handler increments of
## game_mode_timer and frame_count happen in source order.
##
## Order of HUD session mirrors (presentation/hud_session_context.gd):
## process_image and game_state_init(1) at boot (checked here), then per
## reload frame boot_frame_begin + the stage tail hud_icon_bar_update(0) /
## hud_cursor_update, command_screen_build inside step 2, and
## enter_command_select once the reload commits stage 1 step 0. The 0x34
## handler tail hud_screen_frame_draw(0) then advances D_800B5BC8
## (hud_panels.c:1462-1466). After attach the school-day controller keeps
## running the per-frame mirrors on the same HUD session.
##
## Interface for flow/opening_command_handoff.gd (T-39002): an instance is a
## controller with attach(session, state) -> String.
##
## Rejected explicitly (no state change):
## - any state other than 0x34 / 0xFF / step 0 / substep 0 with menu_mode 1
##   and text speed 3, game_mode_timer 1 and the other mode timers 0 (the frame
##   after stage 21 substep 1, after game_mode_dispatch's increment);
## - a state without a fresh route origin or the adapter's unsourced-field
##   record, or one that carries captured boundary/HUD-context identities;
## - no NAME_ENT result (the name buffers still hold boot_player_init's zeros);
## - a boot-owned record that differs from its boot value;
## - a HUD session not driven from process_image/game_state_init, already
##   built, or whose menu mode/icon layout differs from the state;
## - settings_command_screen_build branches other than the ordinary one, and a
##   reload build whose icons or cursor differ from the source build;
## - a reload frame whose HUD tail cannot be mirrored (for example a foreign
##   menu 0 from a file-select or title menu_set that the route forwarded);
## - a reload frame that follows a frame the session rolled back after the
##   handler had advanced the HUD session.
##
## HUD menu 0 and cursor at the switch: on the original, NAME_ENT's
## file_select_init_menus (src/ovl/name_entry/file_select.c:827/836) registers
## menu_set(0, 14, ...) and menu_set(1, 4, ...) after boot, and the title and
## NAME_ENT frames move the cursor (golden a5: menu 0 item 0 = (-144, -18, 32,
## 16), counts 14/4, cursor (-89, 79)). The port's entry does not run the file
## select, so a session driven only from boot still holds boot_hud_init's icon
## bar menu. Its menu_cursor_0 (0), menu_mode (1) and D_800B5A9C/D_800B5AA0
## (-1/0) equal the golden a5 values, so the hovered icon and the hover pulse
## follow the source; the menu 0 records and the cursor position on the
## pre-build reload frames do not (the source snaps the cursor to file select
## item 0, (-116, -10); this session snaps it to icon item 0, (-132, -43)).
## open() records that in hud_unsourced; it clears on the frame whose
## settings_command_screen_build rewrites menu 0 (hud_icon_bar_init) and whose
## hud_icon_bar_update places the cursor from it, after which both sides hold
## the same records (golden a6). Consumers must not present the pre-build
## cursor or menu 0 records while hud_unsourced is set.
##
## Unsourced values are reported, not filled in: `unsourced` lists the fields
## the state adapter stored without a source (NAME_ENT rand() results, the
## background cache, scene_work). attach copies the list to
## rule_state.resources.unsourced, where consumers refuse to read them.

const RouteState = preload("res://rules/new_game_route_state.gd")
const CoreState = preload("res://core/game_state.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")

const MODE_COMMAND_SCREEN := 0x34
const STAGE_RELOAD := 0xFF
const STAGE_SELECT := 1
const MENU_MODE_A := 1 # include/game_enums.h:263
const TEXT_SPEED_SLOW := 3 # include/main/text.h:83
const FULL_BRIGHTNESS := 0x80
const CHECKER_OT_SLOT := 0x0F # D_800B5BD4 set by stage 21 substep 0
## Origins of the fresh no-card route: flow/new_game_state_adapter.gd, and
## flow/interactive_prologue.gd, which runs the same adapter output.
const FRESH_ORIGINS: Array[String] = ["source-fresh-no-card-direct-entry-development-v1",
	"interactive-entry-owned-state-v1"]
const NAME_ENT_OVERLAY_MODE := 0x15
## game_mode_dispatch increments game_mode_timer after the handler that ran
## game_mode_set(0x34) (golden a4/a5: 1).
const SWITCH_GAME_MODE_TIMER := 1
const ICON_COUNT_ORDINARY := 14
const ICON_ARRAY_SIZE := 15
const CLUB_NONE := 0xB

const NOT_AT_SWITCH := "fresh handoff starts on the frame after stage 21 substep 1: mode 0x34 stage 0xFF step 0, game_mode_timer 1, menu_mode 1, text speed 3"
const CAPTURED_BOUNDARY := "fresh New Game handoff refuses captured development-boundary or HUD-context state"
const NAME_ENTRY_MISSING := "fresh handoff requires the NAME_ENT commit (name buffers still hold boot_player_init zeros)"
const HUD_NOT_FRESH := "fresh handoff requires an intact HUD session driven from process_image and game_state_init, not yet built"
const NOT_RELOADING := "handoff reload frames run only between the stage 21 switch and command select"
const NOT_AT_SELECT := "gameplay attach requires the handoff to have reached command select (0x34 stage 1 step 0) with the HUD session"
const HUD_MENU_UNSOURCED := "HUD menu 0 and cursor before the reload build are unsourced: the session holds boot_hud_init's icon bar, not NAME_ENT file_select_init_menus' menu_set(0, 14)/menu_set(1, 4) and the title/NAME_ENT cursor moves"
const FAILED := "handoff refused an earlier frame; the HUD session cannot be rolled back, start a new session"

## Launch inputs that carry captured state. TOKIMEMO_BOUNDARY_SHA is not one:
## it carries no state, and app/boundary_save_controls.gd profile_versions
## still reads it as the save content identity on every launch, New Game
## included (T-39007). Refusing it would stop every fresh New Game boot.
const CAPTURED_INPUTS: Array[String] = ["TOKIMEMO_BOUNDARY",
	"TOKIMEMO_HUD_CONTEXT", "TOKIMEMO_HUD_CONTEXT_SHA"]

var error: String = ""
var phase: String = "" # reload, command_select, attached, failed
var state: RouteState
var hud: HudSession
var gameplay: Object
var reload_frames: int = 0
var build_branch: String = ""
## Fields stored without a sourced value (from the state adapter).
var unsourced: Dictionary = {}
## HUD_MENU_UNSOURCED from open() until the build frame commits; empty after.
var hud_unsourced: String = ""
var _positions: Array = []
## frame_count the next handler call must see: GameSession increments it
## after a committed frame. Anything else means the previous frame was rolled
## back after the HUD session had advanced.
var _expected_frame: int = -1
## Reload services of the opening owner (Reload.REQUIRED), given to bind.
var _services: Dictionary = {}


## Error when a fresh New Game launch also names captured development inputs.
static func captured_input_error(values: Dictionary) -> String:
	for key: String in CAPTURED_INPUTS:
		if not str(values.get(key, "")).is_empty():
			return CAPTURED_BOUNDARY + ": " + key
	return ""


## The check app/playable_boot.gd _load_inputs runs for a New Game launch.
static func launch_input_error() -> String:
	var values: Dictionary = {}
	for key: String in CAPTURED_INPUTS:
		values[key] = OS.get_environment(key)
	return captured_input_error(values)


## Source boot values of the records that only game_state_init /
## boot_game_data_init write before the command screen on a fresh New Game
## (src/main/game_init.c:294-468). NAME_ENT writes the names, birthdays, girl
## records and, for cheat names, the parameters; those are not pinned here.
static func boot_records() -> Dictionary:
	var calendars: Array = []
	for i in 2:
		var kinds: Array = []
		kinds.resize(32)
		kinds.fill(0)
		var markers: Array = kinds.duplicate()
		calendars.append({"day_kind": kinds, "marker": markers})
	# boot_calendar_init (game_init.c:301-331).
	calendars[0]["day_kind"][0] = 0x1E
	calendars[0]["marker"][0] = 5
	for day: int in [2, 9, 16, 23, 30]:
		calendars[0]["day_kind"][day] = 1
	calendars[0]["day_kind"][29] = 2
	calendars[0]["marker"][4] = 1
	calendars[1]["day_kind"][0] = 0x1F
	for day: int in [7, 14, 21, 28]:
		calendars[1]["day_kind"][day] = 1
	for day: int in [3, 4, 5]:
		calendars[1]["day_kind"][day] = 2
	var clubs: Array = []
	for i in 12:
		clubs.append({"practice_points": 0, "level": 0, "flags": 0})
	var event_log: Array = []
	for i in 256:
		event_log.append({"girl": 0, "flag": 0, "scene": 0, "rank": 0,
			"year": 0, "month": 0, "day": 0, "weekday": 0})
	var day_events: Array = []
	for i in 32:
		day_events.append([0, 0, 0, 0])
	return {
		"vacation": 0, "year": 0x5F, "month": 4, "day": 4, "weekday": 1, "day_count": 0,
		"calendar": calendars,
		"player_flags": CLUB_NONE << 12, # boot_player_init: b[0] = 0, nibbles 0 / 0xB
		"player_flags_upper": 0,
		"clubs": clubs,
		"event_log": event_log, "event_log_count": 0,
		"day_events": day_events, "day_event_index": 0,
		"appointment_words": [0, 0, 0, 0, 0, 0, 0, 0],
		"scene_flags": _zeros(0xFC),
		"girl_last_date_spot_timer": _zeros(16),
		"last_command": 0, "sd_anim_fast": 0,
		"day_event_done_bits": 0, "day_event_count_today": 0, "daily_check_done": 0,
		"memory_card_job": 0, "memory_card_busy": 0,
	}


## settings_command_screen_build branch (home_screens.c:249-323) from the
## records it reads at build time.
static func source_build_branch(data: Dictionary) -> String:
	var day: int = int(data["day"])
	# month_calendar[2] is two {day_kind[0x20], marker[0x20]} records
	# (include/game/calendar.h): marker[i] for i >= 0x20 reads past the first
	# record into month_calendar[1].day_kind[i - 0x20], as the source's
	# marker[day - 1 + 7] does late in a month (home_screens.c:271).
	var markers: Array = (data["calendar"][0]["marker"] as Array).duplicate()
	if data["calendar"].size() > 1 and (data["calendar"][1] as Dictionary).get("day_kind") is Array:
		markers.append_array(data["calendar"][1]["day_kind"])
	if day < 1 or day - 1 + 7 >= markers.size():
		return "unsupported: marker read past month_calendar[1].day_kind"
	var marker: int = int(markers[day - 1 + 2])
	var club: int = Math.u16(int(data["player_flags"])) >> 12
	var health: int = (Math.u16(int(data["player_flags"])) >> 8) & 0xF
	if marker == 5:
		return "test"
	if marker == 8 and club != CLUB_NONE:
		return "camp"
	if int(markers[day - 1 + 7]) == 0xC and club < 6:
		return "festival_sick" if health == 3 else "festival"
	if marker == 0xB:
		return "trip"
	if (int(data["player_flags_upper"]) & 1) != 0:
		return "remedial"
	if health == 3:
		return "sick"
	return "ordinary"


## icon_enabled and menu_cursor_0 after the ordinary build: hud_icon_bar_init
## enables icons 0..icon_count-1, settings_command_screen_icons_reset(0) clears
## 10, 8, 9 (14 for icon_mode 2, 4 without a club), and 1995-04-04 clears 13.
## icon_enabled[14] keeps its earlier value unless icons_reset clears it.
static func source_ordinary_build(data: Dictionary, icon_mode: int, icon_count: int, prior_icon_14: int) -> Dictionary:
	var icons: Array = []
	icons.resize(ICON_ARRAY_SIZE)
	icons.fill(0)
	for i in range(icon_count):
		icons[i] = 1
	if icon_count < ICON_ARRAY_SIZE:
		icons[14] = prior_icon_14
	for i: int in [0xA, 8, 9]:
		icons[i] = 0
	if icon_mode == 2:
		icons[0xE] = 0
	if (Math.u16(int(data["player_flags"])) >> 12) == CLUB_NONE:
		icons[4] = 0
	if [int(data["year"]), int(data["month"]), int(data["day"])] == [0x5F, 4, 4]:
		icons[0xD] = 0
	return {"icon_enabled": icons, "menu_cursor": int(data["last_command"]) & 0xF}


## Opens the handoff on the frame after stage 21 substep 1. On refusal the
## error is returned, the handoff stays unusable and nothing is changed.
func open(route_state: Variant, hud_session: Variant, icon_positions: Array, controller: Object) -> String:
	phase = "failed"
	error = _switch_error(route_state, hud_session, icon_positions, controller)
	if not error.is_empty():
		return error
	state = route_state
	hud = hud_session
	gameplay = controller
	_positions = icon_positions.duplicate(true)
	unsourced = (state.opening["unsourced"] as Dictionary).duplicate(true)
	_take_background_cache()
	# A session with any menu_set after boot is refused on the first reload
	# frame (command_screen_frame); one without holds the boot icon bar menu.
	hud_unsourced = HUD_MENU_UNSOURCED if not hud.trace().has("menu_set") else ""
	_expected_frame = -1
	phase = "reload"
	return ""


## Binds mode_handler as the 0x34 handler of the session that owns the state,
## with the opening's reload services (the same ones Reload.step requires).
## bg_lba_cache / dec_bg_show[0] at the switch, from the opening's media
## owner. Every prologue background read is layer 1 (prologue.c
## dec_bg_cd_read(x, 1)) after its first dec_bg_reset (prologue.c:218), so
## cache[0..1] and dec_bg_show[0] stay 0, and cache[2 + b] is the LBA the
## media bridge holds in names_media.buffer_b (0 after a dec_bg_reset).
## Golden a1-a5: [0, 0, 0x4973, 0x496A], dec_bg_show[0] 0. Without both
## owned buffers the fields stay unsourced.
func _take_background_cache() -> void:
	var names: Variant = state.opening.get("names_media")
	if not names is Dictionary: return
	var cache: Array = [0, 0]
	for key: String in ["buffer_0", "buffer_1"]:
		var buffer: Variant = names.get(key)
		if not buffer is Dictionary or not buffer.get("lba") is int or buffer.lba < 0:
			return
		cache.append(int(buffer.lba))
	state.rule_state["background_cache"] = cache
	state.rule_state["background_show"] = 0
	unsourced.erase("background_cache")
	unsourced.erase("background_show")


func bind(session: Session, services: Dictionary) -> String:
	if phase != "reload":
		return FAILED if phase == "failed" else NOT_RELOADING
	if session == null or session.state != state:
		return "the handoff binds only the session that owns its route state"
	for key: String in Reload.REQUIRED:
		if not services.get(key) is Callable or not (services[key] as Callable).is_valid():
			return "command reload needs service: " + key
	var failure: String = session.router.bind_mode(MODE_COMMAND_SCREEN, mode_handler)
	if failure.is_empty():
		_services = services.duplicate()
	return failure


static func _switch_error(route_state: Variant, hud_session: Variant, icon_positions: Array, controller: Object) -> String:
	if not route_state is RouteState:
		return CAPTURED_BOUNDARY + " (state is not the fresh opening route state)"
	var s: RouteState = route_state
	if not str(s.opening.get("origin", "")) in FRESH_ORIGINS:
		return CAPTURED_BOUNDARY + " (origin %s)" % str(s.opening.get("origin", ""))
	if not s.opening.get("unsourced") is Dictionary:
		return "fresh session lacks the state adapter's unsourced-field record"
	for key: String in ["boundary_sha256", "development_boundary", "hud_context", "hud_context_sha256"]:
		if s.opening.has(key) or s.rule_state.has(key):
			return CAPTURED_BOUNDARY + " (" + key + ")"
	var failure: String = s.structural_error()
	if not failure.is_empty():
		return failure
	if [s.game_mode, s.mode_stage, s.mode_step, s.substep] != [MODE_COMMAND_SCREEN, STAGE_RELOAD, 0, 0] \
			or s.game_mode_timer != SWITCH_GAME_MODE_TIMER or s.mode_stage_timer != 0 or s.mode_step_timer != 0 \
			or s.game_mode_scratch != 0 or s.mode_stage_scratch != 0 or s.ovl_target_game_mode != NAME_ENT_OVERLAY_MODE:
		return NOT_AT_SWITCH
	if s.rule_state.get("menu_mode") != MENU_MODE_A or s.opening.get("text_speed") != TEXT_SPEED_SLOW \
			or s.opening.get("D_800B3DB0") != 0 or s.opening.get("D_800B5BD4") != CHECKER_OT_SLOT \
			or s.opening.get("screen_brightness") != FULL_BRIGHTNESS or s.opening.get("command_ready") != false:
		return NOT_AT_SWITCH
	# NAME_ENT result: boot_player_init bzeroes 0xC4..0x140; the commit always
	# stores a non-empty family, given and nickname (blank input takes a preset).
	for key: String in ["profile_family_bytes", "profile_given_bytes", "profile_nickname_bytes"]:
		var bytes: Variant = s.rule_state.get(key)
		if not bytes is Array or (bytes as Array).size() != (16 if key == "profile_nickname_bytes" else 8):
			return NAME_ENTRY_MISSING + " (" + key + ")"
		if int(bytes[0]) == 0 and int(bytes[1]) == 0:
			return NAME_ENTRY_MISSING + " (" + key + ")"
	var birthday: int = Math.u32(int(s.rule_state.get("player_birthday", 0)))
	if (birthday & 15) < 1 or (birthday & 15) > 12 or ((birthday >> 4) & 31) < 1:
		return NAME_ENTRY_MISSING + " (player_birthday)"
	var boot: Dictionary = boot_records()
	for key: String in boot:
		if not s.rule_state.has(key):
			return "fresh session lacks boot record " + key
		if s.rule_state[key] != boot[key]:
			return "fresh session record %s differs from its game_init.c boot value" % key
	if s.opening.get("icon_mode") != 0 or s.opening.get("icon_count") != ICON_COUNT_ORDINARY:
		return "fresh session icon layout differs from boot_settings_defaults_set (TypeA, 14)"
	if s.rule_state.get("icon_enabled") != boot_icon_enabled():
		return "fresh session icon_enabled differs from boot_hud_init's hud_icon_bar_init"
	if not hud_session is HudSession:
		return HUD_NOT_FRESH
	var h: HudSession = hud_session
	var entries: Array = h.trace()
	if not h.intact() or entries.is_empty() or entries[0] != "process_image" \
			or not entries.any(func(e: Variant) -> bool: return str(e).begins_with("game_state_init(1)")) \
			or entries.has("command_screen_build") or entries.has("enter_command_select"):
		return HUD_NOT_FRESH
	# Stage 21 stores menu_mode = MENU_MODE_A; the HUD session has no mirror of
	# that store, so it must already hold the same value.
	if h.menu_mode != MENU_MODE_A:
		return "stage 21 menu_mode store is not mirrored: HUD session menu_mode is %d" % h.menu_mode
	if h.icon_mode != int(s.opening["icon_mode"]) or h.icon_count != int(s.opening["icon_count"]):
		return "HUD session icon layout differs from the session state"
	if icon_positions.size() != ICON_COUNT_ORDINARY:
		return "TypeA icon bar requires the fourteen positions of the authenticated HUD manifest"
	if controller == null or not controller.has_method("attach"):
		return "fresh handoff requires the school-day controller"
	return ""


## The 0x34 handler while the reload runs (bound by bind): stage 0xFF through
## the source reload, the HUD tail of the same frame, then the handler tail
## hud_screen_frame_draw(0). GameSession increments game_mode_timer and
## frame_count afterwards. A refusal restores the route state and the HUD
## session and ends the handoff.
func mode_handler(base: CoreState, input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	if phase != "reload":
		result.error = FAILED if phase == "failed" else NOT_RELOADING
		return
	if base != state:
		result.error = "the handoff handler runs only on its own route state"
		return
	if _expected_frame >= 0 and state.frame_count != _expected_frame:
		_fail(result, "the previous frame was rolled back after the HUD session advanced; start a new session")
		return
	if _services.is_empty():
		_fail(result, "the handoff needs the opening's reload services (bind)")
		return
	var before: Dictionary = state.projection()
	var hud_before: Dictionary = hud.checkpoint()
	var wrapped: Dictionary = _services.duplicate()
	var base_build: Variant = _services.get("command_build")
	if base_build is Callable:
		wrapped["command_build"] = func(s: RouteState, r: Session.FrameResult) -> void:
			_build(base_build, s, r)
	var message: String = "HUD session state was modified outside the source mirrors" if hud_before.is_empty() else hud.begin_outer_frame()
	var step: Session.FrameResult = null
	if message.is_empty():
		step = Reload.step(state, wrapped)
		message = step.error
	if message.is_empty():
		# home_screens.c:93-96: hud_icon_bar_update(0) and hud_cursor_update()
		# after the step function, every reload frame.
		message = hud.command_screen_frame(input.held, input.pressed)
	if message.is_empty() and state.mode_stage == STAGE_SELECT:
		if state.mode_step != 0 or state.opening.get("command_ready") != true:
			message = "reload reached stage 1 without the committed command-ready boundary"
		else:
			message = hud.enter_command_select(state.game_mode, state.mode_stage, state.mode_step)
	if message.is_empty():
		# settings_command_screen_mode_handler tail (home_screens.c:73).
		hud_screen_fade(state.opening, state.frame_count)
	if not message.is_empty():
		state.restore(before)
		if not hud_before.is_empty():
			hud.restore(hud_before)
		_fail(result, message)
		return
	result.audio.append_array(step.audio)
	result.presentation.append_array(step.presentation)
	result.transitions.append_array(step.transitions)
	reload_frames += 1
	if not hud_unsourced.is_empty() and hud.trace().has("command_screen_build"):
		hud_unsourced = ""
	_expected_frame = Math.u32(state.frame_count + 1)
	if state.mode_stage == STAGE_SELECT:
		phase = "command_select"


func _fail(result: Session.FrameResult, message: String) -> void:
	result.error = message
	error = message
	phase = "failed"


## hud_screen_frame_draw (hud_panels.c:1462-1466) for D_800B5BC8 only: on a
## frame whose pre-increment frame_count has low bits 3, a value >= 0xEF
## advances as a u8 (0xFE -> 0xFF swaps the season CLUT, 0xFF -> 0x00 ends
## the fade). Absent means no store has happened on this route.
static func hud_screen_fade(opening: Dictionary, frame_count: int) -> void:
	if not opening.get("D_800B5BC8") is int:
		return
	var fade: int = int(opening["D_800B5BC8"])
	if (frame_count & 3) == 3 and fade >= 0xEF:
		opening["D_800B5BC8"] = (fade + 1) & 0xFF


## icon_enabled after boot_hud_init's hud_icon_bar_init with icon_count 14.
static func boot_icon_enabled() -> Array:
	var icons: Array = _zeros(ICON_ARRAY_SIZE)
	for i in ICON_COUNT_ORDINARY:
		icons[i] = 1
	return icons


func _build(base_build: Callable, s: RouteState, r: Session.FrameResult) -> void:
	var data: Dictionary = s.rule_state
	build_branch = source_build_branch(data)
	if build_branch != "ordinary":
		r.error = "settings_command_screen_build branch %s is not modeled by the HUD session" % build_branch
		return
	var prior_icon_14: int = int(data["icon_enabled"][14]) if data.get("icon_enabled") is Array and (data["icon_enabled"] as Array).size() == ICON_ARRAY_SIZE else -1
	if prior_icon_14 < 0:
		r.error = "icon_enabled lacks the fifteen native entries before the build"
		return
	base_build.call(s, r)
	if not r.error.is_empty():
		return
	var expected: Dictionary = source_ordinary_build(data, int(s.opening["icon_mode"]), int(s.opening["icon_count"]), prior_icon_14)
	if data.get("icon_enabled") != expected["icon_enabled"] or data.get("menu_cursor") != expected["menu_cursor"]:
		r.error = "reload command build differs from settings_command_screen_build: icons %s cursor %s, source %s cursor %d" % [
			str(data.get("icon_enabled")), str(data.get("menu_cursor")), str(expected["icon_enabled"]), expected["menu_cursor"]]
		return
	var message: String = hud.command_screen_build(_positions, int(data["last_command"]), true)
	if not message.is_empty():
		r.error = message


## Controller interface for flow/opening_command_handoff.gd: attaches the same
## route state, session and RNG to the school-day controller.
func attach(session: Session, attached_state: Variant) -> String:
	if phase != "command_select":
		return FAILED if phase == "failed" else NOT_AT_SELECT
	if attached_state != state or session == null:
		return "gameplay attach must receive the handoff's own route state and session"
	if [state.game_mode, state.mode_stage, state.mode_step] != [MODE_COMMAND_SCREEN, STAGE_SELECT, 0] \
			or not hud.at_command_select() or hud.menu_cursor() != int(state.rule_state["menu_cursor"]):
		return NOT_AT_SELECT
	if not session.rng.configuration_error().is_empty():
		return "gameplay attach requires the configured native RNG of the opening session"
	if not "hud_session" in gameplay:
		return "the school-day controller cannot keep the HUD session mirrors"
	var failure: Variant = gameplay.call("attach", session, state)
	if not failure is String:
		return "school-day controller attach returned no status"
	if not (failure as String).is_empty():
		return failure
	# The same source global continues on the gameplay side: D_800B5BC8 is
	# resources.hud_season_fade there (services/native_hud_season.gd).
	var resources: Dictionary = state.rule_state["resources"]
	if state.opening.get("D_800B5BC8") is int:
		resources[HudSeason.KEY] = int(state.opening["D_800B5BC8"])
	resources["unsourced"] = unsourced.duplicate(true)
	gameplay.set("hud_session", hud)
	if "hud_icon_positions" in gameplay:
		gameplay.set("hud_icon_positions", _positions.duplicate(true))
	phase = "attached"
	return ""


static func _zeros(count: int) -> Array:
	var values: Array = []
	values.resize(count)
	values.fill(0)
	return values
