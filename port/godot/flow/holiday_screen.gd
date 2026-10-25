extends RefCounted
## T-29013. holiday.c::holiday_menu_click, followed by source HUD menu update.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const Command = preload("res://flow/command_screen.gd")
const Rng = preload("res://core/native_rng.gd")

static func step(state: State, input: InputFrame, tables: Dictionary, rng: Rng, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if (input.pressed & 0x100) != 0 or (input.held & 0x10f) != 0:
		result.error = "holiday menu-mode change is outside the captured menu-modeA branch"
		return
	if (input.pressed & 0x20) != 0:
		var selected: int = data["menu_cursor"]
		if selected < 0 or selected >= 14:
			result.error = "holiday source icon cursor is outside fourteen records"
			return
		# The source confirms the old cursor first; disabled icons remain no-ops.
		if selected != 0 and (data["icon_enabled"][selected] != 0 or selected in [11,12]):
			result.error = "reachable holiday choice requires an unimplemented handler: %d" % selected
			return
		result.audio.append({"kind":"native_cd_audio_command","command":9})
		result.audio.append({"kind":"native_audio_command","command":3})
		result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
		if data["icon_enabled"][selected] != 0 and selected == 0:
			if not data["resources"].get("available_holiday_commands",[]).has(0):
				result.error = "required original holiday command0 resources are unavailable"
				return
			data["last_command"] = (data["last_command"] & ~15) | selected
			result.audio.append({"kind":"native_audio_command","command":0x500})
			var event: Array = data["day_events"][data["day_event_index"]]
			event[1] = selected
			event[2] = Scheduler.poor_result(state, selected, 1, tables, rng)
			state.set_stage(5)
			result.transitions.append({"kind":"native_stage","stage":5})
	Command.select_cursor(state,input,result)
