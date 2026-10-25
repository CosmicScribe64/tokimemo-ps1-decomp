extends RefCounted
## Normal stage0 arrival & stage21 finish. Source: name_entry/prologue.c, lines 182..310, 2714..2740, 2809..2821.
## Backend callbacks are mandatory and transactional; missing audio cannot be
## converted into readiness, an elapsed-time escape, or an automatic START.

const State = preload("res://rules/new_game_route_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Session = preload("res://core/game_session.gd")
const REQUIRED = [
	"bank_load_start", "scene_reset", "bank_poll", "bank_start",
	"background_read", "cd_sync", "background_decode", "actor_read", "actor_palette_setup",
	"line_start", "line_run", "arrival_draw", "outer_draw"
]


static func step(state: State, rng: Rng, pressed: int, services: Dictionary) -> Session.FrameResult:
	var out := Session.FrameResult.new()
	if state.game_mode != 0x15 or (state.mode_stage not in [0, 21]) or not rng.configuration_error().is_empty():
		out.error = "normal arrival requires native mode 0x15 stage 0 or 21 and configured RNG"
		return out

	if state.mode_stage == 0 and state.mode_step not in [0, 1, 2]:
		out.error = "arrival stage 0 requires mode_step 0, 1 or 2"
		return out

	for key: String in REQUIRED:
		if not services.get(key) is Callable or not services[key].is_valid():
			out.error = "normal arrival needs service: " + key
			return out

	var before: Dictionary = state.projection()
	var random_before: Dictionary = rng.checkpoint()

	if state.mode_stage == 0:
		match state.mode_step:
			0:
				services.bank_load_start.call(state, "prologue_arrival", out)
				state.next_step()
			1:
				state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
				match state.substep:
					0:
						state.opening["D_8011ECD3"] = 0
						state.opening["D_800B5BC8"] = 0xfe
						state.opening["prologue_line"] = 255
						# Reset callback follows source ordering and performs the exact
						# renderer/text/HUD/palette resets, not a prebuilt image swap.
						rng.reseed(state.frame_count)
						services.scene_reset.call(state, out)
						state.opening.screen_brightness = 0
						state.substep += 1
					1:
						var status: Variant = services.bank_poll.call(state, out)
						if out.error.is_empty() and typeof(status) != TYPE_INT:
							out.error = "arrival bank poll status is unknown"
						elif out.error.is_empty() and status == 1:
							services.bank_start.call(state, 0, 0, out)
							if out.error.is_empty():
								services.background_read.call(state, 0x494b, 1, out)
							state.substep += 1
					2:
						if Math.u32(state.mode_step_timer) >= 17:
							state.substep += 1
					3:
						var sync: Variant = services.cd_sync.call(state, out)
						if out.error.is_empty() and typeof(sync) != TYPE_INT:
							out.error = "arrival CD status is unknown"
						elif out.error.is_empty() and (sync & 1):
							# Returns the actual bg_decode_result; callback owns show_set.
							var decoded: Variant = services.background_decode.call(state, 1, out)
							if out.error.is_empty() and typeof(decoded) != TYPE_INT:
								out.error = "arrival background decode result is unknown"
							elif out.error.is_empty() and decoded == -1:
								# background_read's retry flag requests native dec_bg_reset.
								services.background_read.call(state, 0x494b, 1, out, true)
							elif out.error.is_empty():
								services.actor_read.call(state, 13, 0x80180000, 0xabab, out)
								state.substep += 1
					4:
						var sync: Variant = services.cd_sync.call(state, out)
						if out.error.is_empty() and typeof(sync) != TYPE_INT:
							out.error = "arrival actor CD status is unknown"
						elif out.error.is_empty() and (sync & 1):
							services.actor_palette_setup.call(state, out)
							state.next_step()
					_:
						out.error = "arrival scene substep outside source dispatch"
				if int(state.opening.screen_brightness) < 128:
					state.opening.screen_brightness = Math.u8(int(state.opening.screen_brightness) + 1)
			2:
				match state.substep:
					0:
						state.opening.screen_brightness = 128
						state.opening.prologue_line = 0
						services.line_start.call(state, 0, out)
						state.substep += 1
					1:
						services.line_run.call(state, out)
					2, 3:
						if state.substep == 2:
							state.substep += 1
							state.mode_step_timer = 0
						var old_timer: int = state.mode_step_timer
						state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
						if Math.u32(old_timer) >= 61:
							state.opening["actor96_draw_flags"] = int(state.opening.get("actor96_draw_flags", 0)) | 128
							state.next_stage()
					_:
						out.error = "arrival line substep outside source dispatch"

		if out.error.is_empty():
			services.arrival_draw.call(state, 1, out)
		if out.error.is_empty():
			services.outer_draw.call(state, 0 if state.mode_stage == 0 else 2, out)
		if out.error.is_empty() and (state.mode_step != 0 or state.mode_stage != 0) and (pressed & 0x800):
			state.set_stage(21)
			state.mode_step = 0
			state.substep = 0
			state.mode_step_timer = 0

	elif state.mode_stage == 21:
		# prologue_mode_stage_finish / prologue_finish_run (prologue.c lines 2714..2740)
		match state.substep:
			0:
				state.opening.screen_brightness = 128
				services.bank_load_start.call(state, "normal_gameplay", out)
				state.substep += 1
			1:
				state.game_mode = 0x34
				state.set_stage(0xff)
				state.mode_step = 0
				state.substep = 0
				state.mode_step_timer = 0
				state.rule_state.menu_mode = 1
			_:
				out.error = "prologue finish substep outside source dispatch"

		if out.error.is_empty():
			services.outer_draw.call(state, 2, out)

	if not out.error.is_empty():
		state.restore(before)
		rng.restore(random_before)
		out.audio.clear()
		out.presentation.clear()
		out.transitions.clear()

	return out
