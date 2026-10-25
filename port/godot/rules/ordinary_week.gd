extends RefCounted
## T-29009. Bounded initial-April day_event_dispatch; retains the complete queue.
## Event overlays and holiday mode require further modules and stop explicitly.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const DayEnd = preload("res://rules/day_end.gd")

# hud_panels.c::parameter_show_init: memcpy(progress_state+0x20,
# GameState+0xc4,0x7c). ParamValue words at GameState+0xfc..0x11f map
# to progress_state+0x58..0x7b. Other profile bytes remain the captured model.
const PROFILE_SNAPSHOT_START: int = 0x20
const PROFILE_SNAPSHOT_BYTES: int = 0x7c
const PARAMETER_SNAPSHOT_START: int = PROFILE_SNAPSHOT_START + 0xfc - 0xc4
const PARAMETER_COUNT: int = 9
const PARAMETER_WORD_BYTES: int = 4
const PARAMETER_SNAPSHOT_END: int = PARAMETER_SNAPSHOT_START + PARAMETER_COUNT * PARAMETER_WORD_BYTES
const PROGRESS_STATE_BYTES: int = PROFILE_SNAPSHOT_START + PROFILE_SNAPSHOT_BYTES
const PROFILE_HASH_FIELD: String = "ordinary_week_immutable_profile_hash"

static func snapshot_parameters(state: State) -> String:
	var data: Dictionary = state.rule_state
	if not data.get("progress_state") is Array or data["progress_state"].size() != PROGRESS_STATE_BYTES:
		return "parameter profile snapshot needs the exact native progress byte extent"
	for value: Variant in data["progress_state"]:
		if not value is int or value < 0 or value > 255:
			return "parameter profile snapshot contains a non-native progress byte"
	if not data.get("params") is Array or data["params"].size() != PARAMETER_COUNT:
		return "parameter profile snapshot needs nine native parameter words"
	for value: Variant in data["params"]:
		if not value is int or value < -2147483648 or value > 2147483647:
			return "parameter profile snapshot contains a non-native signed parameter word"
	for key: String in ["player_flags", "player_flags_upper", "player_birthday"]:
		if not data.get(key) is int:
			return "parameter profile snapshot lacks the immutable profile field: " + key
	# This slice has no name/profile editing or non-parameter profile updates.
	# Keep the original captured prefix/suffix; a state-owned hash rejects later
	# edits instead of inventing an incomplete profile memcpy. No name is embedded.
	var signature: String = JSON.stringify([data["progress_state"].slice(0, PARAMETER_SNAPSHOT_START),
		data["progress_state"].slice(PARAMETER_SNAPSHOT_END, PROGRESS_STATE_BYTES),
		data["player_flags"], data["player_flags_upper"], data["player_birthday"]]).sha256_text()
	if data.has(PROFILE_HASH_FIELD) and data[PROFILE_HASH_FIELD] != signature:
		return "ordinary-week immutable profile edit requires the unimplemented full native profile snapshot"
	data[PROFILE_HASH_FIELD] = signature
	for index: int in range(PARAMETER_COUNT):
		for byte: int in range(PARAMETER_WORD_BYTES):
			data["progress_state"][PARAMETER_SNAPSHOT_START + index * PARAMETER_WORD_BYTES + byte] = (data["params"][index] >> (8 * byte)) & 255
	return ""

static func dispatch(state: State, result: Session.FrameResult) -> void:
	if state.mode_stage != 0 or state.mode_step != 0:
		result.error = "ordinary-week ending/stage branch is unimplemented"
		return
	result.error = DayEnd.branch_error(state)
	if not result.error.is_empty():
		return
	var data: Dictionary = state.rule_state
	DayEnd.clamp_girls(data)
	var index: int = data["day_event_index"]
	# Native birthday/skip handling is one ordered pass, never a loop that drops events.
	if data["day_events"][index][0] == 0x32:
		if data["day_events"][index][1] == 0 and (data["day_event_done_bits"] & 1) != 0:
			index += 1
		# The source re-reads the advanced record, without another type check.
		if index < 32 and data["day_events"][index][1] != 0 and (data["day_event_done_bits"] & 2) != 0:
			index += 1
	if index < 32 and data["day_events"][index][0] == 0x33 and (data["day_event_done_bits"] & 4) != 0:
		index += 1
	if index < 32 and data["day_events"][index][0] == 0xfa:
		index += 1
	if index >= 32:
		result.error = "ordinary-week queue overrun after native skip/birthday pass"
		return
	data["day_event_index"] = index
	if data["day_events"][index][0] == 0xfb:
		result.error = DayEnd.finish(state)
		if not result.error.is_empty():
			return
		index = data["day_event_index"]
	var event: Array = data["day_events"][index]
	var mode: int
	match event[0]:
		0:
			if event[1] not in [0, 7]:
				result.error = "ordinary-week activity command is unimplemented: %d" % event[1]
				return
			for icon: int in range(14):
				data["icon_enabled"][icon] = 1 if icon == event[1] else 0
			mode = 0x40
		0xfc: mode = 0x32
		0xff: mode = 0x34
		_:
			result.error = "ordinary-week next branch requires native type0x%02x at index%d (%d-%02d-%02d)" % [event[0], index, data["year"], data["month"], data["day"]]
			return
	state.reset_game_mode_fields(mode)
	data["voice_wait_frames"] = 0
	data["message_title_visible"] = false
	result.transitions.append({"kind": "native_mode", "mode": mode, "day_event_index": index})
