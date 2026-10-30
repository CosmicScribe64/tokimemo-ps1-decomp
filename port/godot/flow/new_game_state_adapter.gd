extends RefCounted
## Fresh unsaved/no-card development route only. Never imports captured RAM.
## Read-before-write exceptions are recorded; consumed bytes require ownership.

const Entry = preload("res://flow/new_game_entry.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Math = preload("res://core/native_math.gd")


class Outcome extends RefCounted:
	var error: String = ""
	var state: State


## boot_context: optional, from the modelled boot path (flow/title_file_select.gd
## through the entry's profile context): {"scene_work": s32}. Without it,
## scene_work and the NAME_ENT rand draws stay unsourced.
static func from_entry(entry: Entry, frame_count: int, system_seed: int, constants: Dictionary, boot_context: Dictionary = {}) -> Outcome:
	var out := Outcome.new()
	if not boot_context.is_empty() and (boot_context.size() != 1 or not boot_context.get("scene_work") is int or boot_context.scene_work < -0x80000000 or boot_context.scene_work > 0x7fffffff):
		out.error = "boot-path context must hold exactly one s32 scene_work"
		return out
	if typeof(constants.get("initial_mascot")) != TYPE_INT or constants.initial_mascot < 0 or constants.initial_mascot > 255:
		out.error = "fresh no-card route requires authenticated original initial mascot state"
		return out

	if entry.stage != 2 or entry.step_id != 5 or entry.state_bytes.size() != 0x1a8c or entry.known_bytes.size() != 0x1a8c:
		out.error = "fresh no-card route requires the independently committed entry controller"
		return out

	var raw: PackedByteArray = entry.state_bytes.duplicate()
	var known: PackedByteArray = entry.known_bytes.duplicate()

	# game_state_init(1)'s bzero; these fields have no writes on the fresh
	# no-card name-entry/early-prologue route before consumption. Retained/card
	# imports and a return-to-title New Game are deliberately unsupported here.
	for span in [[0x35, 1], [0x71d, 3], [0x73c, 16], [0xf68, 4], [0x111b, 1]]:
		for i in span[1]:
			raw[span[0] + i] = 0
			known[span[0] + i] = 255

	# boot_settings_defaults_set's source scalar assignment, not an array default.
	raw[0xf6e] = 0
	known[0xf6e] = 255

	# parameter_show_init's complete profile snapshot at command build.
	for i in 0x7c:
		raw[0x140 + i] = raw[0xc4 + i]
		known[0x140 + i] = known[0xc4 + i]

	# 0x75E..0xF5E: event_log[256] and event_log_count, cleared by
	# boot_event_state_init (game_init.c:441-468) and projected below.
	for span in [[0x3d, 0x4ff], [0x54c, 0x1d4], [0x73c, 16], [0x75e, 0x801], [0xf68, 4], [0xf6e, 1], [0x111b, 1]]:
		for i in span[1]:
			if known[span[0] + i] != 255:
				out.error = "opening consumed native byte lacks source ownership at 0x%x" % (span[0] + i)
				return out

	var state := State.new()
	state.reset_game_mode_fields(0x15)
	# game_mode_set(0x15) at the NAME_ENT exit calls game_mode_request_overlay,
	# which stores the target even though NAME_ENT stays resident.
	state.ovl_target_game_mode = 0x15
	state.frame_count = Math.u32(frame_count)
	state.system_seed = Math.s32(system_seed)
	state.sound_active = true # game_state_init assigns this before any mode.

	var calendars: Array = []
	for i in 2:
		calendars.append({
			"day_kind": Array(raw.slice(0x44 + i * 64, 0x64 + i * 64)),
			"marker": Array(raw.slice(0x64 + i * 64, 0x84 + i * 64)),
		})

	var params: Array = []
	var girls: Array = []
	var events: Array = []
	var appointments: Array = []

	for i in 9:
		params.append(raw.decode_s32(0xfc + i * 4))

	for i in 16:
		var base: int = 0x1bc + i * 56
		girls.append({
			"tokimeki_word": raw.decode_s32(base),
			"affection_word": raw.decode_s32(base + 4),
			"syoushin_word": raw.decode_s32(base + 8),
			"flags": raw.decode_u32(base + 12),
			"birthday": raw.decode_u32(base + 16),
			"counters": Array(raw.slice(base + 20, base + 56)),
		})

	for i in 32:
		events.append(Array(raw.slice(0x69c + i * 4, 0x6a0 + i * 4)))

	for i in 8:
		appointments.append(raw.decode_u32(0x54c + i * 4))

	# GameState.clubs (+0x66C, ClubRecord[12], include/game/club.h): bzero(0x30)
	# in boot_calendar_init and boot_event_state_init. Read by
	# culture_result_apply and incremented by the school-day club command.
	var clubs: Array = []
	for i in 12:
		var club_base: int = 0x66c + i * 4
		clubs.append({"practice_points": raw.decode_u16(club_base),
			"level": int(raw[club_base + 2]), "flags": int(raw[club_base + 3])})

	# GameState.event_log (+0x75E, EventLogEntry[256], include/game/day_event.h)
	# and event_log_count (+0xF5E): boot_event_state_init sets the count to 0
	# and bzeroes the 0x800-byte log.
	var event_log: Array = []
	for i in 256:
		var entry_base: int = 0x75e + i * 8
		event_log.append({"girl": int(raw[entry_base]), "flag": int(raw[entry_base + 1]),
			"scene": int(raw[entry_base + 2]), "rank": int(raw[entry_base + 3]),
			"year": int(raw[entry_base + 4]), "month": int(raw[entry_base + 5]),
			"day": int(raw[entry_base + 6]), "weekday": int(raw[entry_base + 7])})

	# icon_enabled (a global outside GameState): boot_hud_init's
	# hud_icon_bar_init (hud_panels.c:834-836) sets entries 0..icon_count-1 with
	# the boot TypeA count 14; entry 14 keeps its zeroed .bss value. Nothing on
	# the fresh no-card route writes it again before the command build.
	var icons: Array = []
	icons.resize(15)
	icons.fill(0)
	for i in 14:
		icons[i] = 1

	var disabled: Array = []
	disabled.resize(14)
	disabled.fill(0)

	state.rule_state = {
		"year": int(raw[0x3e]),
		"month": int(raw[0x3f]),
		"day": int(raw[0x40]),
		"weekday": int(raw[0x41]),
		"day_count": raw.decode_u16(0x42),
		"calendar": calendars,
		"params": params,
		"player_flags": raw.decode_u16(0xf4),
		"player_flags_upper": raw.decode_u16(0xf6),
		"player_birthday": raw.decode_u32(0xf8),
		"girls": girls,
		"progress_state": Array(raw.slice(0x120, 0x1bc)),
		"day_events": events,
		"day_event_index": int(raw[0x71c]),
		"scene_flags": Array(raw.slice(0x56c, 0x668)),
		"day_event_done_bits": int(raw[0x71e]),
		"day_event_count_today": int(raw[0x71d]),
		"daily_check_done": int(raw[0x71f]),
		"last_command": raw.decode_u32(0xf68),
		"menu_cursor": 0,
		# file_select.c:3133: the no-card system-file stage leaves
		# MENU_MODE_MOUSE_LIKE (0). Stage 21 substep 1 stores MENU_MODE_A.
		"menu_mode": 0,
		"menu_disabled": disabled,
		"icon_enabled": icons,
		"vacation": int(raw[0x3d]),
		"sd_anim_fast": int(raw[0xf6e]),
		"memory_card_job": int(raw[0x111b]),
		"memory_card_busy": int(raw[0x35]),
		"girl_last_date_spot_timer": Array(raw.slice(0x73c, 0x74c)),
		"appointment_words": appointments,
		"clubs": clubs,
		"event_log": event_log,
		"event_log_count": int(raw[0xf5e]),
		"last_picked_girl": 0,
		"actor_state": {},
		"resources": {"available_commands": []},
		"scene_work": int(boot_context.get("scene_work", 0)),
		"background_cache": [0, 0, 0, 0],
		"background_show": 0,
		"profile_family_bytes": Array(raw.slice(0xd4, 0xdc)),
		"profile_given_bytes": Array(raw.slice(0xdc, 0xe4)),
		"profile_nickname_bytes": Array(raw.slice(0xe4, 0xf4)),
		"voice_wait_frames": 0,
		"message_title_visible": false,
	}

	state.opening = {
		"origin": "source-fresh-no-card-direct-entry-development-v1",
		"unread_before_owned_write": ["last_picked_girl", "icon_enabled[14]"],
		"unsourced": _unsourced(entry, not boot_context.is_empty()),
		"icon_mode": 0,
		"icon_count": 14,
		"screen_brightness": 0,
		"hud_mascot_choice": constants.initial_mascot,
		"command_ready": false,
		"clock_profile": "boot path from power-on (flow/title_file_select.gd), then entry logical iterations" if not boot_context.is_empty() else "direct-entry logical iterations; native title/timer latency not claimed",
		"audio_profile": "unset",
	}

	out.error = state.structural_error()
	if out.error.is_empty():
		out.state = state
	return out


## Fields this adapter stores without a sourced value, with the reason. The
## values stay in rule_state only because the school-day record requires them;
## a consumer that depends on one must refuse (State.unsourced_error).
static func _unsourced(entry: Entry, boot_path: bool = false) -> Dictionary:
	var out: Dictionary = {
		# 0x801230D0 holds -1024 at the stage 21 switch on the original
		# (.asset-cache/emu/newgame-golden-v1, sets A and B). Its writers are
		# the mode 0x13 file select: file_select_init_settings sets 0xFF
		# (file_select.c:745) and file_select_flash_update subtracts 1 per
		# frame (file_select.c:3250); 0xFF - 1279 frames = -1024. The port's
		# entry does not run file select (whether to model it is pending).
		"scene_work": "set by mode 0x13 file select (file_select_init_settings 0xFF, file_select_flash_update -1 per frame), which the port's entry does not run; 0 is a placeholder",
		# bg_lba_cache/dec_bg_show: the prologue's layer-1 loads fill slots 2-3
		# and the command-room read (home_screens.c:333-363, background_images.c
		# dec_bg_cd_read) fills slot 1 - dec_bg_show[0]. Neither read runs here.
		"background_cache": "prologue background loads and the command-room read are not performed",
		"background_show": "the command-room decode is not performed",
	}
	if boot_path:
		# The boot path ran mode 0x13 and srand(system_data.seed) itself
		# (flow/title_file_select.gd, verified against the original's runs in
		# .asset-cache/emu/title-filesel-golden-v1), so scene_work and the BIOS
		# rand state the entry draws from are sourced.
		out.erase("scene_work")
		return out
	# NAME_ENT rand() draws (name_entry.c:1132, 1997-2029, 2503-2533). The entry
	# seeds like file_select_init_settings, srand(system_data.seed)
	# (file_select.c:735), so the draws are deterministic for a given input.
	# They still differ from the original: there system_data.seed carries the
	# boot/title timer history (the port installs 0), and the file-select iris
	# update draws rand() after the srand (file_select.c:584-593), which the
	# port's entry does not run. Every draw-dependent value stays unsourced.
	var rng: Variant = entry.get("_rng")
	if rng != null and rng.get("trace") is Array:
		for record: Variant in rng.trace:
			if not record is Dictionary:
				continue
			var caller: String = str(record.get("caller", ""))
			if caller.begins_with("name_entry_new_game_setup:girl"):
				out["girls[%s].flags.club" % caller.trim_prefix("name_entry_new_game_setup:girl")] = "rand() & 1 from srand(system_data.seed) without the original's boot/title/file-select seed history (%s)" % caller
			elif caller.begins_with("name_entry_"):
				out["rand:" + caller] = "a value chosen by this rand() draw comes from srand(system_data.seed) without the original's boot/title/file-select seed history"
	return out
