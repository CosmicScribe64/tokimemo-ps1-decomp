extends RefCounted
## T-29009 / T-29014. Bounded initial-April day_event_dispatch; retains the complete queue.
## Event overlays and holiday mode require further modules and stop explicitly.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const DayEnd = preload("res://rules/day_end.gd")
const Calendar = preload("res://rules/calendar.gd")
const EventModes = preload("res://rules/day_event_modes.gd")

# hud_panels.c::parameter_show_init: memcpy(progress_state+0x20,
# GameState+0xc4,0x7c). ParamValue words at GameState+0xfc..0x11f map
# to progress_state+0x58..0x7b. Other profile bytes remain the captured model.
const PROFILE_SNAPSHOT_START: int = 0x20
const PROFILE_SNAPSHOT_BYTES: int = 0x7c
const PARAMETER_SNAPSHOT_START: int = PROFILE_SNAPSHOT_START + 0xfc - 0xc4
const PARAMETER_COUNT: int = 9
const PARAMETER_WORD_BYTES: int = 4
const PARAMETER_SNAPSHOT_END: int = PARAMETER_SNAPSHOT_START + PARAMETER_COUNT * PARAMETER_WORD_BYTES
const MIRROR_SNAPSHOT_START: int = PROFILE_SNAPSHOT_START + 0x120 - 0xc4
const MIRROR_SOURCE_BYTES: int = 0x20
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
	# Verify the old source and mirrored bytes before refreshing any slice;
	# an edited mirror must never be repaired silently by the following copy.
	var signature: String = _profile_signature(data)
	if not data.get("allow_multi_week", false) and data.has(PROFILE_HASH_FIELD) and data[PROFILE_HASH_FIELD] != signature:
		return "ordinary-week immutable profile edit requires the unimplemented full native profile snapshot"
	for index: int in range(PARAMETER_COUNT):
		for byte: int in range(PARAMETER_WORD_BYTES):
			data["progress_state"][PARAMETER_SNAPSHOT_START + index * PARAMETER_WORD_BYTES + byte] = (data["params"][index] >> (8 * byte)) & 255
	# The final 0x20 source bytes of memcpy(+0xc4,0x7c) are the first 0x20
	# progress bytes at GameState+0x120. They map to progress+0x7c..0x9b.
	# Initial weekly command flag+0x15 consequently mirrors at+0x91.
	for offset: int in range(MIRROR_SOURCE_BYTES):
		data["progress_state"][MIRROR_SNAPSHOT_START + offset] = data["progress_state"][offset]
	data[PROFILE_HASH_FIELD] = _profile_signature(data)
	return ""

## day_event_queue_build_week (scheduler.c) mutates two profile records
## outside parameter_show_init: the weekly command counter
## progress_state[0x15 + command] and the recovery countdown in
## player_flags_upper bits 5-7. Like increment_holiday_command_counter, the
## builder checks the anchored profile before (profile_guard) and re-anchors it
## after (reanchor_profile); the anchor exists once the command reload's
## parameter_show_init has run (flow/new_game_command_reload.gd).
static func profile_guard(data: Dictionary) -> String:
	if not data.get("allow_multi_week", false) and data.has(PROFILE_HASH_FIELD) and data[PROFILE_HASH_FIELD] != _profile_signature(data):
		return "weekly queue build refused an edited native profile"
	return ""

static func reanchor_profile(data: Dictionary) -> void:
	if data.has(PROFILE_HASH_FIELD):
		data[PROFILE_HASH_FIELD] = _profile_signature(data)

static func _profile_signature(data: Dictionary) -> String:
	return JSON.stringify([data["progress_state"].slice(0, PARAMETER_SNAPSHOT_START),
		data["progress_state"].slice(PARAMETER_SNAPSHOT_END, PROGRESS_STATE_BYTES),
		data["player_flags"], data["player_flags_upper"], data["player_birthday"]]).sha256_text()

static func increment_holiday_command_counter(state: State, command: int) -> String:
	# holiday.c::holiday_activity_load: one proven source mutation between
	# native snapshot sites. Validate the old guard before updating/reanchoring;
	# do not copy or repair the mirrored tail until parameter_show_init runs.
	var data: Dictionary = state.rule_state
	var multi: bool = data.get("allow_multi_week", false)
	if not multi and command != 0:
		return "holiday command counter supports only the captured command0 branch"
	if multi and command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return "holiday command counter supports only commands 0..7"
	if not multi and data.get(PROFILE_HASH_FIELD) != _profile_signature(data):
		return "holiday command counter refused an unanchored or edited native profile"
	var slot: int = 0x15 + (command % 9 if multi else 0)
	data["progress_state"][slot] = (data["progress_state"][slot] + 1) & 255
	data[PROFILE_HASH_FIELD] = _profile_signature(data)
	return ""

static func dispatch(state: State, result: Session.FrameResult) -> void:
	if state.mode_stage != 0 or state.mode_step != 0:
		result.error = "ordinary-week ending/stage branch is unimplemented"
		return
	result.error = DayEnd.branch_error(state)
	if not result.error.is_empty():
		return
	var data: Dictionary = state.rule_state
	var multi: bool = data.get("allow_multi_week", false)
	if multi and data["month"] == 5 and data["day"] == 6:
		for i: int in range(11):
			data["girls"][i]["flags"] &= ~0x10
	DayEnd.clamp_girls(data)
	if multi and data["year"] >= 98 and data["month"] >= 3 and data["day"] != 0:
		state.next_step()
		result.transitions.append({"kind": "native_step", "mode": state.game_mode, "step": state.mode_step})
		return
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
		if multi and data["year"] >= 98 and data["month"] >= 3 and data["day"] != 0:
			state.next_step()
			result.transitions.append({"kind": "native_step", "mode": state.game_mode, "step": state.mode_step})
			return
	var event: Array = data["day_events"][index]
	var mode: int
	match event[0]:
		0:
			if event[1] not in [0, 1, 2, 3, 4, 5, 6, 7]:
				result.error = "ordinary-week activity command is unimplemented: %d" % event[1]
				return
			for icon: int in range(14):
				data["icon_enabled"][icon] = 1 if icon == event[1] else 0
			mode = 0x40
		0xfc: mode = 0x32
		0x10:
			var holiday_commands: Variant=data["resources"].get("available_holiday_commands",[])
			if not multi and (not holiday_commands is Array or not holiday_commands.has(0)):
				result.error = "ordinary-week next branch requires native type0x10 at index%d (%d-%02d-%02d)" % [index,data["year"],data["month"],data["day"]]
				return
			mode = 0x46
		0xff: mode = 0x34
		_:
			if not multi:
				result.error = "ordinary-week next branch requires native type0x%02x at index%d (%d-%02d-%02d)" % [event[0], index, data["year"], data["month"], data["day"]]
				return
			mode = EventModes.mode_for(event[0], (data["player_flags"] >> 12) & 15)
			# These records clear the met-on-day bit before their scene starts.
			if event[0] in [0xfd, 0x20, 0x24, 0x28, 0x2c, 0x40, 0x41, 0x43]:
				for girl_index: int in range(11):
					data["girls"][girl_index]["flags"] &= ~1
			if event[0] in [0x20, 0x24]:
				var scratch: Array[int] = []
				scratch.resize(0x100)
				scratch.fill(0)
				data["overlay_scratch"] = scratch
			if event[0] in [0x44, 0x46, 0x47]:
				data["day_event_count_today"] = (data["day_event_count_today"] + 1) & 255
			if event[0] == 0x44:
				if event[1] < 0 or event[1] >= data["girls"].size():
					result.error = "walk-home queue girl is outside native records"
					return
				# Native queues place this after an activity, never at index zero.
				if index == 0:
					result.error = "walk-home queue lacks its preceding record"
					return
				if ((data["player_flags"] >> 8) & 15) == 3 or data["day_events"][index - 1][0] in [0x28, 0x47] or (data["girls"][event[1]]["flags"] & 2) == 0:
					_skip_queue_event(state, result)
					return
			if event[0] == 0x46 and ((data["appointment_words"][0] >> 29) & 1) != 0:
				_skip_queue_event(state, result)
				return
			if event[0] == 0x45:
				data["scene_work"] = event[1]
				data["talk_work"] = event[2]
			if event[0] == 0x48:
				if event[1] < 0 or event[1] >= data["girls"].size():
					result.error = "meeting queue girl is outside native records"
					return
				data["current_girl"] = event[1]
				var girl: Dictionary = data["girls"][event[1]]
				girl["affection_word"] = State.with_high(girl["affection_word"], 50)
				girl["syoushin_word"] = State.with_high(girl["syoushin_word"], 0)
				data["talk_work"] = event[2]
	state.reset_game_mode_fields(mode)
	data["voice_wait_frames"] = 0
	data["message_title_visible"] = false
	result.transitions.append({"kind": "native_mode", "mode": mode, "day_event_index": index})

static func _skip_queue_event(state: State, result: Session.FrameResult) -> void:
	# scheduler.c::day_event_next(0), normal (non-debug) gameplay.
	state.rule_state["day_event_index"] = (state.rule_state["day_event_index"] + 1) & 255
	state.reset_game_mode_fields(0x31)
	result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": state.rule_state["day_event_index"]})
