extends RefCounted
## Fresh unsaved/no-card development route only. Never imports captured RAM.
## Read-before-write exceptions are recorded; consumed bytes require ownership.

const Entry = preload("res://flow/new_game_entry.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Math = preload("res://core/native_math.gd")


class Outcome extends RefCounted:
	var error: String = ""
	var state: State


static func from_entry(entry: Entry, frame_count: int, system_seed: int, constants: Dictionary) -> Outcome:
	var out := Outcome.new()
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

	for span in [[0x3d, 0x4ff], [0x54c, 0x1d4], [0x73c, 16], [0xf68, 4], [0xf6e, 1], [0x111b, 1]]:
		for i in span[1]:
			if known[span[0] + i] != 255:
				out.error = "opening consumed native byte lacks source ownership at 0x%x" % (span[0] + i)
				return out

	var state := State.new()
	state.reset_game_mode_fields(0x15)
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

	# icon/menu arrays are written by the command builder, not initialized here.
	var icons: Array = []
	icons.resize(15)
	icons.fill(0)

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
		"menu_mode": 1,
		"menu_disabled": disabled,
		"icon_enabled": icons,
		"vacation": int(raw[0x3d]),
		"sd_anim_fast": int(raw[0xf6e]),
		"memory_card_job": int(raw[0x111b]),
		"memory_card_busy": int(raw[0x35]),
		"girl_last_date_spot_timer": Array(raw.slice(0x73c, 0x74c)),
		"appointment_words": appointments,
		"last_picked_girl": 0,
		"actor_state": {},
		"resources": {"available_commands": []},
		"scene_work": 0,
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
		"unread_before_owned_write": ["last_picked_girl", "scene_work", "icon_enabled[14]"],
		"icon_mode": 0,
		"icon_count": 14,
		"screen_brightness": 0,
		"hud_mascot_choice": constants.initial_mascot,
		"command_ready": false,
		"clock_profile": "direct-entry logical iterations; native title/timer latency not claimed",
		"audio_profile": "unset",
	}

	out.error = state.structural_error()
	if out.error.is_empty():
		out.state = state
	return out
