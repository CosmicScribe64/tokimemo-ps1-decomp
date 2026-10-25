extends RefCounted
## Source: home_screens.c::settings_command_screen_mode_stage_reload.
## Callbacks must perform actual service work and report completion explicitly.

const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")
const REQUIRED = [
	"setup_reset", "bank_poll", "bank_start", "hud_read_start", "cd_sync",
	"calendar_upload", "mascot_set", "command_build", "room_finish", "frame_draw", "outer_draw"
]


static func step(state: State, services: Dictionary) -> Session.FrameResult:
	var result := Session.FrameResult.new()
	if state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step not in [0, 1, 2, 3]:
		result.error = "command reload requires native mode 0x34 stage FF"
		return result

	for key: String in REQUIRED:
		if not services.get(key) is Callable or not services[key].is_valid():
			result.error = "command reload needs service: " + key
			return result

	var before: Dictionary = state.projection()
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)

	match state.mode_step:
		0:
			# Owns rendering/text reset and next_step, exactly as source setup_reset.
			services.setup_reset.call(state, result)
		1:
			if state.substep == 0:
				var bank_status: Variant = services.bank_poll.call(state, result)
				if result.error.is_empty() and typeof(bank_status) != TYPE_INT:
					result.error = "bank poll lacks actual native status"
				elif result.error.is_empty() and bank_status == 1:
					services.bank_start.call(state, 0, 0, result)
					state.substep += 1
			elif state.substep == 1:
				services.hud_read_start.call(state, 0x95, 0x801a0000, 0x9b94, result)
				state.substep += 1
			elif state.substep == 2:
				var cd_status: Variant = services.cd_sync.call(state, result)
				if result.error.is_empty() and typeof(cd_status) != TYPE_INT:
					result.error = "CD sync lacks actual native status"
				elif result.error.is_empty() and (cd_status & 1):
					services.calendar_upload.call(state, int(state.rule_state.year), int(state.rule_state.month), 0, result)
					state.rule_state.menu_mode &= 3
					state.opening["D_800B5BC8"] = 0xfe
					if int(state.opening.hud_mascot_choice) < 14:
						services.mascot_set.call(state, int(state.opening.hud_mascot_choice), 1, result)
					state.next_step()
			else:
				result.error = "command reload substep outside source dispatch"
		2:
			services.command_build.call(state, result)
		3:
			services.room_finish.call(state, result)

	if result.error.is_empty():
		# The source checks the step AFTER dispatch, so advancing 1->2 matters.
		services.frame_draw.call(state, state.mode_step >= 2, result)
		if state.mode_step >= 2 and int(state.opening.screen_brightness) < 128:
			state.opening.screen_brightness = Math.u8(int(state.opening.screen_brightness) + 4)
		if result.error.is_empty():
			# Mode handler's mascot update and hud_screen_frame_draw follow reload.
			services.outer_draw.call(state, result)

	if not result.error.is_empty():
		state.restore(before)
		result.audio.clear()
		result.presentation.clear()
		result.transitions.clear()

	return result
