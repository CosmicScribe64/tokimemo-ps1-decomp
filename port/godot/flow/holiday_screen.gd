extends RefCounted
## T-29013. holiday.c::holiday_menu_click, followed by source HUD menu update.
## Supports icons 0 (Literature/Activity), 8 (Phone Book -> 0x10), 9 (Date Appointment -> 0xFE),
## 10 (Magazine -> 0x20), and 12 (Calendar -> 0x30) when subscreens or available_holiday_commands enable them.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const Command = preload("res://flow/command_screen.gd")
const Rng = preload("res://core/native_rng.gd")
const AppointmentRules = preload("res://rules/holiday_appointment.gd")

static func step(state: State, input: InputFrame, tables: Dictionary, rng: Rng, result: Session.FrameResult, allow_subscreens: bool = false) -> void:
	var data: Dictionary = state.rule_state
	if (input.pressed & 0x100) != 0 or (input.held & 0x10f) != 0:
		result.error = "holiday menu-mode change is outside the captured menu-modeA branch"
		return
	if (input.pressed & 0x20) != 0:
		var selected: int = data["menu_cursor"]
		if selected < 0 or selected >= 14:
			result.error = "holiday source icon cursor is outside fourteen records"
			return
		var avail: Array = data["resources"].get("available_holiday_commands", [])
		var sub_ok: bool = allow_subscreens or avail.has(selected)

		# Check if selected icon is a supported sub-screen command
		if selected in [8, 9, 10, 12] and sub_ok:
			if selected == 8 and data["icon_enabled"][8] != 0:
				result.audio.append({"kind":"native_cd_audio_command","command":9})
				result.audio.append({"kind":"native_audio_command","command":3})
				result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
				data["last_command"] = (data["last_command"] & ~15) | 8
				result.audio.append({"kind":"native_audio_command","command":0x50d})
				state.set_stage(0x10)
				result.transitions.append({"kind":"native_stage","stage":0x10})
				Command.select_cursor(state,input,result)
				return
			elif selected == 9 and data["icon_enabled"][9] != 0:
				var found_appt := AppointmentRules.hud_appointment_find_today(data["year"], data["month"], data["day"], data["appointment_words"])
				if found_appt > 0:
					result.audio.append({"kind":"native_cd_audio_command","command":9})
					result.audio.append({"kind":"native_audio_command","command":3})
					result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
					data["last_command"] = (data["last_command"] & ~15) | 9
					result.audio.append({"kind":"native_audio_command","command":0x500})
					state.set_stage(0xFE)
					result.transitions.append({"kind":"native_stage","stage":0xFE})
					Command.select_cursor(state,input,result)
					return
			elif selected == 10 and data["icon_enabled"][10] != 0:
				result.audio.append({"kind":"native_cd_audio_command","command":9})
				result.audio.append({"kind":"native_audio_command","command":3})
				result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
				result.audio.append({"kind":"native_audio_command","command":0x50d})
				state.set_stage(0x20)
				result.transitions.append({"kind":"native_stage","stage":0x20})
				Command.select_cursor(state,input,result)
				return
			elif selected == 12:
				result.audio.append({"kind":"native_cd_audio_command","command":9})
				result.audio.append({"kind":"native_audio_command","command":3})
				result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
				result.audio.append({"kind":"native_audio_command","command":0x50d})
				state.set_stage(0x30)
				result.transitions.append({"kind":"native_stage","stage":0x30})
				Command.select_cursor(state,input,result)
				return

		# The source confirms the old cursor first; disabled icons remain no-ops.
		if selected != 0 and (data["icon_enabled"][selected] != 0 or selected in [11,12]):
			result.error = "reachable holiday choice requires an unimplemented handler: %d" % selected
			return
		result.audio.append({"kind":"native_cd_audio_command","command":9})
		result.audio.append({"kind":"native_audio_command","command":3})
		result.audio.append({"kind":"native_restore_bgm","bank":0,"sequence":0})
		if data["icon_enabled"][selected] != 0 and selected == 0:
			if not avail.has(0):
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
