extends RefCounted
## Prologue stage dispatch (stages 0..21); production service coverage is incomplete.
## Source: src/ovl/name_entry/prologue.c, lines 1..2822.
## Source-flow checks do not establish connected playback through every stage.
## PAD_START (0x800) requests the original skip after stage 0 step 0.

const State = preload("res://rules/new_game_route_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Session = preload("res://core/game_session.gd")

const MODE_PROLOGUE: int = 0x15
const MODE_COMMAND_SCREEN: int = 0x34
const STAGE_ARRIVAL: int = 0
const STAGE_FINISH: int = 21
const STAGE_COMMAND_RELOAD: int = 0xff
const PAD_START_MASK: int = 0x0800
const FULL_BRIGHTNESS: int = 128
const FLASH_MAX: int = 0x0100
const BOARD_SCROLL_LIMIT: int = 0x0140
const ARRIVAL_TIMER_LIMIT: int = 17
const ARRIVAL_LINE_HOLD_FRAMES: int = 61
const CLOSING_HOLD_FRAMES: int = 0x3d
const SHORT_HOLD_FRAMES: int = 0x20
const SHIORI_WAIT_FRAMES: int = 0x41
const PETAL_COUNT: int = 8
const PETAL_WRAP_Y: int = 41
const ACTOR_DEST_PRIMARY: int = 0x80180000
const ACTOR_DEST_SECONDARY: int = 0x8018c800
const BLINK_MASK_7F: int = 0x7f
const BLINK_MASK_FF: int = 0xff
const AUDIO_CMD_SYSTEM_FLAG: int = 5
const AUDIO_CMD_FADE_B1: int = 0xb1
const AUDIO_CMD_FADE_C1: int = 0xc1
const AUDIO_CMD_FADE_D1: int = 0xd1
const AUDIO_CMD_SE_503: int = 0x0503
const AUDIO_CMD_SE_604: int = 0x0604
const AUDIO_CMD_SE_605: int = 0x0605

const REQUIRED: Array[String] = [
	"bank_load_start",
	"scene_reset",
	"bank_poll",
	"bank_start",
	"background_read",
	"cd_sync",
	"background_decode",
	"actor_read",
	"actor_palette_setup",
	"line_start",
	"line_run",
	"arrival_draw",
	"outer_draw",
]


static func step(state: State, rng: Rng, pressed: int, services: Dictionary) -> Session.FrameResult:
	var out: Session.FrameResult = Session.FrameResult.new()
	state.opening.erase("rei_banner_primitive")
	if state.game_mode != MODE_PROLOGUE or state.mode_stage < STAGE_ARRIVAL or state.mode_stage > STAGE_FINISH or not rng.configuration_error().is_empty():
		out.error = "prologue requires native mode 0x15 stage 0..21 and configured RNG"
		return out

	if state.mode_stage == STAGE_ARRIVAL and state.mode_step not in [0, 1, 2]:
		out.error = "arrival stage 0 requires mode_step 0, 1 or 2"
		return out

	for key: String in REQUIRED:
		if not services.get(key) is Callable or not services[key].is_valid():
			out.error = "normal arrival needs service: " + key
			return out

	var before: Dictionary = state.projection()
	var random_before: Dictionary = rng.checkpoint()
	match state.mode_stage:
		0:
			_stage_00_arrival(state, rng, services, out)
		1:
			_stage_01_board(state, rng, services, out)
		2:
			_stage_02_names(state, rng, services, out)
		3:
			_stage_03_line_03(state, services, out)
		4:
			_stage_04_line_04(state, rng, services, out)
		5:
			_stage_05_transition(state, services, out)
		6:
			_stage_06_yoshio_greeting(state, services, out)
		7:
			_stage_07_line_11(state, services, out)
		8:
			_stage_08_rei_banner(state, services, out)
		9:
			_stage_09_rei_lines(state, rng, services, out)
		10:
			_stage_10_lines_15_16(state, services, out)
		11:
			_stage_11_line_17(state, services, out)
		12:
			_stage_12_shiori_load(state, services, out)
		13:
			_stage_13_shiori_line_18(state, rng, services, out)
		14:
			_stage_14_shiori_line_19(state, rng, services, out)
		15:
			_stage_15_lines_20_to_31(state, services, out)
		16:
			_stage_16_lines_32_to_36(state, services, out)
		17:
			_stage_17_line_37(state, services, out)
		18:
			_stage_18_lines_38_to_41(state, services, out)
		19:
			_stage_19_flash(state, services, out)
		20:
			_stage_20_closing(state, services, out)
		21:
			_stage_21_finish(state, services, out)

	if out.error.is_empty():
		if (state.mode_step != 0 or state.mode_stage != STAGE_ARRIVAL) and (pressed & PAD_START_MASK) != 0:
			state.set_stage(STAGE_FINISH)

	if not out.error.is_empty():
		state.restore(before)
		rng.restore(random_before)
		out.audio.clear()
		out.presentation.clear()
		out.transitions.clear()

	return out


static func _emit_audio_command(state: State, command: int, services: Dictionary, out: Session.FrameResult) -> void:
	if services.get("audio_command") is Callable and services["audio_command"].is_valid():
		services["audio_command"].call(state, command, out)
	else:
		out.audio.append({"kind": "native_audio_command", "command": command})


static func _run_line_pair(state: State, line_id: int, start_substep: int, services: Dictionary, out: Session.FrameResult) -> bool:
	if state.substep == start_substep:
		state.opening["prologue_line"] = line_id
		services["line_start"].call(state, line_id, out)
		state.substep += 1
		return true
	if state.substep == start_substep + 1:
		services["line_run"].call(state, out)
		return true
	return false


static func _poll_cd_ready(state: State, services: Dictionary, out: Session.FrameResult, label: String) -> bool:
	var sync: Variant = services["cd_sync"].call(state, out)
	if out.error.is_empty() and typeof(sync) != TYPE_INT:
		out.error = label + " CD status is unknown"
		return false
	if not out.error.is_empty():
		return false
	return ( int(sync) & 1 ) != 0


static func _decode_bg_and_check(state: State, slot: int, lba: int, services: Dictionary, out: Session.FrameResult, label: String) -> bool:
	if not _poll_cd_ready(state, services, out, label):
		return false
	var decoded: Variant = services["background_decode"].call(state, slot, out)
	if out.error.is_empty() and typeof(decoded) != TYPE_INT:
		out.error = label + " background decode result is unknown"
		return false
	if not out.error.is_empty():
		return false
	if int(decoded) == -1:
		services["background_read"].call(state, lba, slot, out, true)
		return false
	return true


static func _petals_init(state: State, rng: Rng) -> void:
	var petals: Array = []
	for index: int in range(PETAL_COUNT):
		var raw_unk_18: int = rng.next("prologue_petals_init:unk_18:" + str(index))
		var raw_x: int = rng.next("prologue_petals_init:x:" + str(index))
		var raw_y: int = rng.next("prologue_petals_init:y:" + str(index))
		var raw_following_brightness: int = rng.next("prologue_petals_init:following_brightness:" + str(index))
		petals.append({
			"unk_18": raw_unk_18 & 7,
			"unk_07": Math.u8(raw_following_brightness),
			"unk_08": 0,
			"x_word": ((raw_x % 440) - 0xa0) << 16,
			"y_word": (-0x78 - (raw_y % 120)) << 16,
			"brightness": Math.u8(raw_following_brightness),
			"draw_flags": 0xa4,
		})
	state.opening["prologue_petals"] = petals


static func _petals_update(state: State, rng: Rng, allow_wrap: int, services: Dictionary, out: Session.FrameResult) -> void:
	var sine_callback: Variant = services.get("prologue_sine")
	if not sine_callback is Callable or not sine_callback.is_valid():
		out.error = "prologue petal update needs source-backed prologue_sine service"
		return
	var petals: Array = state.opening.get("prologue_petals", [])
	if petals.size() != PETAL_COUNT:
		return
	for index: int in range(PETAL_COUNT):
		var petal: Dictionary = petals[index]
		var next_unk_07: int = Math.u8(int(petal["unk_07"]) + 1)
		petal["unk_07"] = next_unk_07
		if petal.has("brightness"):
			petal["brightness"] = next_unk_07
		var sine_value: Variant = sine_callback.call(next_unk_07 << 5)
		if typeof(sine_value) != TYPE_INT:
			out.error = "prologue sine service returned an unknown value"
			return
		var next_x_word: int = Math.add32(int(petal["x_word"]), int(sine_value) * 8)
		if allow_wrap == 0:
			next_x_word = Math.sub32(next_x_word, 0x10000)
		var next_y_word: int = Math.add32(int(petal["y_word"]), 0x10000)
		if Math.s16(Math.u32(next_y_word) >> 16) >= PETAL_WRAP_Y:
			var raw_x: int = rng.next("prologue_petals_update:x:" + str(index))
			var raw_y: int = rng.next("prologue_petals_update:y:" + str(index))
			next_x_word = (Math.s16((raw_x % 320) - 160) << 16) | (next_x_word & 0xffff)
			next_y_word = (Math.s16(-120 - (raw_y % 48)) << 16) | (next_y_word & 0xffff)
			if allow_wrap == 1 and index >= 4:
				petal["draw_flags"] = 0
		petal["x_word"] = next_x_word
		petal["y_word"] = next_y_word
		petals[index] = petal
	state.opening["prologue_petals"] = petals


static func _stage_00_arrival(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	match state.mode_step:
		0:
			services["bank_load_start"].call(state, "prologue_arrival", out)
			state.next_step()
		1:
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			match state.substep:
				0:
					state.opening["D_8011ECD3"] = 0
					state.opening["D_800B5BC8"] = 0xfe
					state.opening["prologue_line"] = 255
					rng.reseed(state.frame_count)
					services["scene_reset"].call(state, out)
					state.opening["screen_brightness"] = 0
					state.substep += 1
				1:
					var status: Variant = services["bank_poll"].call(state, out)
					if out.error.is_empty() and typeof(status) != TYPE_INT:
						out.error = "arrival bank poll status is unknown"
					elif out.error.is_empty() and int(status) == 1:
						services["bank_start"].call(state, 0, 0, out)
						if out.error.is_empty():
							services["background_read"].call(state, 0x494b, 1, out)
						state.substep += 1
				2:
					if Math.u32(state.mode_step_timer) >= ARRIVAL_TIMER_LIMIT:
						state.substep += 1
				3:
					if _decode_bg_and_check(state, 1, 0x494b, services, out, "arrival"):
						services["actor_read"].call(state, 13, ACTOR_DEST_PRIMARY, 0xabab, out)
						state.substep += 1
				4:
					if _poll_cd_ready(state, services, out, "arrival actor"):
						services["actor_palette_setup"].call(state, out)
						state.next_step()
				_:
					out.error = "arrival scene substep outside source dispatch"
			if int(state.opening.get("screen_brightness", 0)) < FULL_BRIGHTNESS:
				state.opening["screen_brightness"] = Math.u8(int(state.opening.get("screen_brightness", 0)) + 1)
		2:
			match state.substep:
				0:
					state.opening["screen_brightness"] = FULL_BRIGHTNESS
					state.opening["prologue_line"] = 0
					services["line_start"].call(state, 0, out)
					state.substep += 1
				1:
					services["line_run"].call(state, out)
				2, 3:
					if state.substep == 2:
						state.substep += 1
						state.mode_step_timer = 0
					var old_timer: int = state.mode_step_timer
					state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
					if Math.u32(old_timer) >= ARRIVAL_LINE_HOLD_FRAMES:
						state.opening["actor96_draw_flags"] = int(state.opening.get("actor96_draw_flags", 0)) | FULL_BRIGHTNESS
						state.next_stage()
				_:
					out.error = "arrival line substep outside source dispatch"

	if out.error.is_empty():
		services["arrival_draw"].call(state, 1, out)
	if out.error.is_empty():
		services["outer_draw"].call(state, 0 if state.mode_stage == STAGE_ARRIVAL else 2, out)


static func _stage_01_board(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	match state.mode_step:
		0:
			match state.substep:
				0:
					state.opening["D_800B3DB0"] = 1
					if state.opening.get("prologue_hud_live") is Dictionary:
						state.opening["prologue_hud_live"]["date_visible"] = true
					if services.has("board_read"):
						services["board_read"].call(state, 9, 0x80162000, 0x4954, out)
					else:
						services["actor_read"].call(state, 9, 0x80162000, 0x4954, out)
					if out.error.is_empty():
						state.substep += 1
				1:
					if _poll_cd_ready(state, services, out, "board load"):
						state.opening.erase("actor96")
						state.opening["actor96_draw_flags"] = 0
						state.opening.erase("actor96_source_draw")
						if state.opening.get("prologue_hud_live") is Dictionary:
							state.opening["prologue_hud_live"]["date_visible"] = false
						_petals_init(state, rng)
						if services.has("board_decode"):
							services["board_decode"].call(state, out)
						if out.error.is_empty():
							state.opening["prologue_scene_counter"] = 0
							state.next_step()
					elif out.error.is_empty():
						if state.opening.get("prologue_hud_live") is Dictionary:
							state.opening["prologue_hud_live"]["date_visible"] = true
				_:
					out.error = "prologue stage 1 step 0 substep outside source dispatch"
			if out.error.is_empty():
				services["arrival_draw"].call(state, 1, out)
		1:
			var counter: int = int(state.opening.get("prologue_scene_counter", 0))
			if (state.frame_count & 1) != 0:
				counter = Math.s16(counter + 1)
				state.opening["prologue_scene_counter"] = counter
			if not _run_line_pair(state, 1, 0, services, out):
				match state.substep:
					2:
						if counter >= 0x141:
							counter = BOARD_SCROLL_LIMIT
							state.opening["prologue_scene_counter"] = counter
							state.substep += 1
					3:
						counter = BOARD_SCROLL_LIMIT
						state.opening["prologue_scene_counter"] = counter
						state.mode_step_timer = 0
						services["background_read"].call(state, 0x495d, 1, out)
						state.substep += 1
					4:
						counter = BOARD_SCROLL_LIMIT
						state.opening["prologue_scene_counter"] = counter
						var old_timer: int = state.mode_step_timer
						state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
						if Math.u32(old_timer) >= 0x79:
							state.substep += 1
					5:
						counter = BOARD_SCROLL_LIMIT
						state.opening["prologue_scene_counter"] = counter
						if _decode_bg_and_check(state, 1, 0x495d, services, out, "stage 1 bg"):
							state.next_stage()
					_:
						out.error = "prologue stage 1 step 1 substep outside source dispatch"
			if out.error.is_empty() and services.has("board_scroll_draw"):
				services["board_scroll_draw"].call(state, counter, out)
			if out.error.is_empty():
				_petals_update(state, rng, 1 if counter == BOARD_SCROLL_LIMIT else 0, services, out)
		_:
			out.error = "prologue stage 1 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_02_names(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	_petals_update(state, rng, 1, services, out)
	if not out.error.is_empty():
		return
	if state.mode_step == 0:
		var counter: int = int(state.opening.get("prologue_scene_counter", 0))
		match state.substep:
			0:
				services["background_read"].call(state, 0x4966, 1, out)
				state.opening["prologue_line"] = 2
				state.opening["prologue_scene_counter"] = 0
				if out.error.is_empty():
					services["line_start"].call(state, 2, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
				if out.error.is_empty():
					state.substep += 1
			1:
				if _poll_cd_ready(state, services, out, "stage 2 bg"):
					var decoded: Variant = services["background_decode"].call(state, 1, out)
					if out.error.is_empty() and typeof(decoded) != TYPE_INT:
						out.error = "stage 2 bg background decode result is unknown"
					elif out.error.is_empty():
						state.opening["prologue_scene_counter"] = FULL_BRIGHTNESS
						state.substep += 1
						state.opening["bg_fade_amount"] = FULL_BRIGHTNESS
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
				if out.error.is_empty():
					services["line_run"].call(state, out)
			2:
				if counter >= 0x11:
					counter = Math.s16(counter - 3)
					state.opening["prologue_scene_counter"] = counter
					state.opening["bg_fade_amount"] = counter & 0xff
				else:
					state.substep += 1
				if out.error.is_empty() and services.has("crossfade_draw"):
					services["crossfade_draw"].call(state, 1, out)
				if out.error.is_empty():
					services["line_run"].call(state, out)
			3:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("crossfade_draw"):
					services["crossfade_draw"].call(state, 1, out)
			4:
				if out.error.is_empty() and services.has("crossfade_draw"):
					services["crossfade_draw"].call(state, 1, out)
				if out.error.is_empty():
					if counter < FULL_BRIGHTNESS:
						state.opening["bg_fade_amount"] = counter & 0xff
						counter = Math.s16(counter + 4)
						state.opening["prologue_scene_counter"] = counter
					else:
						counter = FULL_BRIGHTNESS
						state.opening["prologue_scene_counter"] = counter
						state.opening["bg_fade_amount"] = FULL_BRIGHTNESS
						state.opening["bg_brightness"] = FULL_BRIGHTNESS
						services["actor_read"].call(state, 0x31, 0x801a4000, 0xab18, out)
						state.substep += 1
			5:
				if counter >= 3:
					counter = Math.s16(counter - 1)
					state.opening["prologue_scene_counter"] = counter
					state.opening["bg_brightness"] = counter & 0xff
				else:
					state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			6:
				if _poll_cd_ready(state, services, out, "stage 2 actor 0"):
					if services.has("prologue_palette_adjust"):
						services["prologue_palette_adjust"].call(state, out)
					if out.error.is_empty():
						services["actor_read"].call(state, 0x11, ACTOR_DEST_PRIMARY, 0xac3f, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			7:
				if _poll_cd_ready(state, services, out, "stage 2 actor 1"):
					state.substep += 1
					services["background_read"].call(state, 0xacdd, 1, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			8:
				if _decode_bg_and_check(state, 1, 0xacdd, services, out, "stage 2 bg 2"):
					if services.has("o12_backup"):
						services["o12_backup"].call(state, out)
					if out.error.is_empty():
						state.next_stage()
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			_:
				out.error = "prologue stage 2 substep outside source dispatch"
	else:
		out.error = "prologue stage 2 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_03_line_03(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if state.mode_step == 0:
		match state.substep:
			0:
				state.opening["prologue_line"] = 3
				state.opening["prologue_scene_counter"] = 0
				if services.has("stage_03_setup"):
					services["stage_03_setup"].call(state, out)
				else:
					state.opening["D_801220E0"] = 0
					state.opening["D_801220E1"] = 0
					state.opening["D_801220E2"] = 0
					state.opening["bg_brightness"] = 0
				if out.error.is_empty():
					services["line_start"].call(state, 3, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
				if out.error.is_empty():
					state.substep += 1
			1:
				var d0: int = int(state.opening.get("D_801220E0", 0))
				if d0 < 0x81:
					var next_d: int = Math.u8(d0 + 4)
					state.opening["D_801220E0"] = next_d
					state.opening["D_801220E1"] = next_d
					state.opening["D_801220E2"] = next_d
					state.opening["bg_brightness"] = Math.u8(int(state.opening.get("bg_brightness", 0)) + 4)
				services["arrival_draw"].call(state, 1, out)
				if out.error.is_empty():
					services["line_run"].call(state, out)
			2:
				services["actor_read"].call(state, 0x31, 0x801a4000, 0xaa23, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			3:
				if _poll_cd_ready(state, services, out, "stage 3 actor"):
					services["background_read"].call(state, 0xace6, 1, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			4:
				var ready: bool = _poll_cd_ready(state, services, out, "stage 3 bg")
				var decoded: int = -1
				if ready and out.error.is_empty():
					var dec_res: Variant = services["background_decode"].call(state, 1, out)
					if out.error.is_empty() and typeof(dec_res) != TYPE_INT:
						out.error = "stage 3 bg background decode result is unknown"
					elif out.error.is_empty():
						decoded = int(dec_res)
				if ready and out.error.is_empty():
					if decoded == -1:
						services["background_read"].call(state, 0xace6, 1, out, true)
					else:
						state.next_stage()
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			_:
				out.error = "prologue stage 3 substep outside source dispatch"
	else:
		out.error = "prologue stage 3 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_04_line_04(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	if state.mode_step == 0:
		match state.substep:
			0:
				services["actor_read"].call(state, 9, ACTOR_DEST_PRIMARY, 0xabb8, out)
				if out.error.is_empty():
					state.opening["prologue_line"] = 4
					state.opening["prologue_scene_counter"] = 0
					if services.has("prologue_palette_adjust"):
						services["prologue_palette_adjust"].call(state, out)
					if out.error.is_empty() and services.has("stage_03_to_04_setup"):
						services["stage_03_to_04_setup"].call(state, 0, out)
					if out.error.is_empty():
						services["line_start"].call(state, 4, out)
					if out.error.is_empty():
						state.substep += 1
					if out.error.is_empty():
						services["arrival_draw"].call(state, 1, out)
			1:
				if _poll_cd_ready(state, services, out, "stage 4 actor 0"):
					if services.has("stage_04_setup"):
						services["stage_04_setup"].call(state, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			2:
				if services.has("stage_04_blink_check"):
					services["stage_04_blink_check"].call(state, rng, out)
				else:
					var blink_draw: int = rng.next("prologue_stage4:blink")
					if (blink_draw & BLINK_MASK_7F) == BLINK_MASK_7F:
						state.opening["prologue_blink"] = 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
				if out.error.is_empty():
					services["line_run"].call(state, out)
			3:
				services["actor_read"].call(state, 0x31, 0x801a4000, 0xab18, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			4:
				if _poll_cd_ready(state, services, out, "stage 4 actor 1"):
					if services.has("prologue_palette_adjust"):
						services["prologue_palette_adjust"].call(state, out)
					if out.error.is_empty():
						if services.has("stage_04_to_05_setup"):
							services["stage_04_to_05_setup"].call(state, out)
					if out.error.is_empty():
						state.next_stage()
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			_:
				out.error = "prologue stage 4 substep outside source dispatch"
	else:
		out.error = "prologue stage 4 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_05_transition(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if state.mode_step == 0:
		var counter: int = int(state.opening.get("prologue_scene_counter", 0))
		match state.substep:
			0:
				if counter < 0x40:
					counter = Math.s16(counter + 1)
				var old_counter: int = counter
				counter = Math.s16(counter + 1)
				state.opening["prologue_scene_counter"] = counter
				if old_counter < 0x80:
					if state.opening.get("actor97") is Dictionary:
						state.opening.actor97["x"] = -0x20 - counter
					if out.error.is_empty():
						services["arrival_draw"].call(state, 1, out)
				else:
					_emit_audio_command(state, AUDIO_CMD_SE_605, services, out)
					if out.error.is_empty() and services.has("stage_05_arm_enter"):
						services["stage_05_arm_enter"].call(state, out)
					if out.error.is_empty():
						services["background_read"].call(state, 0xacf6, 1, out)
					if out.error.is_empty():
						state.substep += 1
					if out.error.is_empty():
						services["arrival_draw"].call(state, 1, out)
			1:
				var a97: Variant = state.opening.get("actor97")
				var step_97: int = int(a97.get("seq_step", 0)) if a97 is Dictionary else -1
				var unk08_97: int = int(a97.get("sequence_counter", -1)) if a97 is Dictionary else -1
				if step_97 == 3 and unk08_97 == 0:
					_emit_audio_command(state, AUDIO_CMD_SE_605, services, out)
				var flags_97: int = int(a97.get("flags", 0)) if a97 is Dictionary else 0
				if out.error.is_empty() and (flags_97 & 1) == 0:
					if _poll_cd_ready(state, services, out, "stage 5 bg 0"):
						state.opening["prologue_scene_counter"] = 0x80
						var dec_0: Variant = services["background_decode"].call(state, 1, out)
						if out.error.is_empty() and typeof(dec_0) != TYPE_INT:
							out.error = "stage 5 bg 0 background decode result is unknown"
						elif out.error.is_empty():
							if services.has("stage_05_color_burst"):
								services["stage_05_color_burst"].call(state, int(dec_0), out)
							if out.error.is_empty():
								services["actor_read"].call(state, 0x31, 0x801a4000, 0xaa54, out)
							if out.error.is_empty():
								state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			2:
				if counter < -0x20:
					state.substep += 1
				else:
					counter = Math.s16(counter - 3)
					state.opening["prologue_scene_counter"] = counter
					if state.opening.get("actor97") is Dictionary:
						state.opening.actor97["x"] = -0x20 - counter
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			3:
				if _poll_cd_ready(state, services, out, "stage 5 actor 0"):
					services["background_read"].call(state, 0xacd4, 1, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			4:
				if _poll_cd_ready(state, services, out, "stage 5 bg 1"):
					services["actor_read"].call(state, 0x19, 0x801f0000, 0xabca, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			5:
				var ready: bool = _poll_cd_ready(state, services, out, "stage 5 actor 1")
				if ready and out.error.is_empty():
					if services.has("stage_05_cut06_setup"):
						services["stage_05_cut06_setup"].call(state, out)
					var decoded: int = -1
					if out.error.is_empty():
						var dec_res: Variant = services["background_decode"].call(state, 1, out)
						if out.error.is_empty() and typeof(dec_res) != TYPE_INT:
							out.error = "stage 5 bg 1 background decode result is unknown"
						elif out.error.is_empty():
							decoded = int(dec_res)
					if out.error.is_empty():
						if decoded == -1:
							services["background_read"].call(state, 0xacd4, 1, out, true)
						else:
							if services.has("stage_05_to_06_setup"):
								services["stage_05_to_06_setup"].call(state, decoded, out)
							if out.error.is_empty():
								state.next_stage()
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			_:
				out.error = "prologue stage 5 substep outside source dispatch"
	else:
		out.error = "prologue stage 5 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_06_yoshio_greeting(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if state.mode_step == 0:
		state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
		var s6_timer: int = Math.add32(int(state.opening.get("stage_06_timer", 0)), 1)
		state.opening["stage_06_timer"] = s6_timer
		match state.substep:
			0:
				state.opening["prologue_line"] = 5
				services["line_start"].call(state, 5, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			1:
				if services.has("stage_06_cd_wait"):
					services["stage_06_cd_wait"].call(state, out)
				else:
					state.substep += 1
					state.mode_step_timer = 0
					state.opening["stage_06_timer"] = 0
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			2:
				if services.has("stage_06_actor98_step"):
					services["stage_06_actor98_step"].call(state, out)
				if out.error.is_empty() and s6_timer == 4 and services.has("stage_06_actor102_set"):
					services["stage_06_actor102_set"].call(state, out)
				if out.error.is_empty():
					services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_06_lip_sync_yoshio"):
					services["stage_06_lip_sync_yoshio"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			3:
				state.opening["prologue_line"] = 6
				services["line_start"].call(state, 6, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			4:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_06_lip_sync_yoshio"):
					services["stage_06_lip_sync_yoshio"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			5:
				state.opening["prologue_line"] = 7
				services["line_start"].call(state, 7, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			6:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_06_lip_sync_yoshio"):
					services["stage_06_lip_sync_yoshio"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			7:
				state.opening["prologue_line"] = 8
				services["line_start"].call(state, 8, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			8:
				services["line_run"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			9:
				state.opening["prologue_line"] = 9
				services["line_start"].call(state, 9, out)
				if out.error.is_empty():
					state.substep += 1
					if services.has("stage_06_actor99_turn"):
						services["stage_06_actor99_turn"].call(state, out)
					if out.error.is_empty():
						_emit_audio_command(state, AUDIO_CMD_SE_503, services, out)
					if out.error.is_empty():
						services["arrival_draw"].call(state, 1, out)
			10:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_06_mouth_girl_a"):
					services["stage_06_mouth_girl_a"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			11:
				state.opening["prologue_line"] = 10
				services["line_start"].call(state, 10, out)
				if out.error.is_empty():
					state.substep += 1
					if services.has("stage_06_actor103_set"):
						services["stage_06_actor103_set"].call(state, out)
					if out.error.is_empty():
						services["arrival_draw"].call(state, 1, out)
			12:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_06_mouth_girl_b"):
					services["stage_06_mouth_girl_b"].call(state, out)
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			13:
				services["background_read"].call(state, 0xacec, 1, out)
				if out.error.is_empty():
					state.substep += 1
					services["arrival_draw"].call(state, 1, out)
			14:
				if _poll_cd_ready(state, services, out, "stage 6 bg"):
					services["actor_read"].call(state, 0x31, 0x801a4000, 0xaa85, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			15:
				if _poll_cd_ready(state, services, out, "stage 6 actor 0"):
					services["actor_read"].call(state, 0x19, 0x801f0000, 0xabe3, out)
					if out.error.is_empty():
						state.substep += 1
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			16:
				var ready: bool = _poll_cd_ready(state, services, out, "stage 6 actor 1")
				if ready and out.error.is_empty():
					var decoded: int = -1
					var dec_res: Variant = services["background_decode"].call(state, 1, out)
					if out.error.is_empty() and typeof(dec_res) != TYPE_INT:
						out.error = "stage 6 bg background decode result is unknown"
					elif out.error.is_empty():
						decoded = int(dec_res)
					if out.error.is_empty():
						if decoded == -1:
							services["background_read"].call(state, 0xacec, 1, out, true)
						else:
							if services.has("stage_06_to_07_setup"):
								services["stage_06_to_07_setup"].call(state, decoded, out)
							if out.error.is_empty():
								state.next_stage()
				if out.error.is_empty():
					services["arrival_draw"].call(state, 1, out)
			_:
				out.error = "prologue stage 6 substep outside source dispatch"
	else:
		out.error = "prologue stage 6 step outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_07_background_wait(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	# prologue_line_11_run case4; case3 falls through to this poll.
	if _poll_cd_ready(state, services, out, "stage 7 bg"):
		services["bank_load_start"].call(state, "prologue_rei", out)
		if out.error.is_empty(): _emit_audio_command(state, AUDIO_CMD_SYSTEM_FLAG, services, out)
		if out.error.is_empty(): _emit_audio_command(state, AUDIO_CMD_FADE_B1, services, out)
		if out.error.is_empty(): state.substep += 1

static func _stage_07_line_11(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if state.mode_step == 0:
		match state.substep:
			0:
				state.opening["prologue_line"] = 11
				services["line_start"].call(state, 11, out)
				if out.error.is_empty():
					state.substep += 1
			1:
				if services.has("stage_07_cd_wait"):
					services["stage_07_cd_wait"].call(state, out)
				else:
					state.substep += 1
				if out.error.is_empty():
					if state.opening.get("actor96") is Dictionary and int(state.opening.actor96.get("x", -160)) < -160:
						state.opening.actor96["x"] = int(state.opening.actor96["x"]) + 1
			2:
				services["line_run"].call(state, out)
				if out.error.is_empty() and services.has("stage_07_lip_sync_line_11"):
					services["stage_07_lip_sync_line_11"].call(state, out)
				if out.error.is_empty() and state.opening.get("actor96") is Dictionary and int(state.opening.actor96.get("x", -160)) < -160:
					var next_x: int = int(state.opening.actor96["x"]) + 1
					state.opening.actor96["x"] = next_x
					if state.opening.get("actor97") is Dictionary:
						state.opening.actor97["x"] = next_x
					if state.opening.get("actor98") is Dictionary:
						state.opening.actor98["x"] = next_x
			3:
				services["background_read"].call(state, 0xad00, 1, out)
				if out.error.is_empty():
					state.substep += 1
					_stage_07_background_wait(state, services, out)
			4:
				# Source defers decode/show until substep7, after both actor reads.
				_stage_07_background_wait(state, services, out)
			5:
				var status: Variant = services["bank_poll"].call(state, out)
				if out.error.is_empty() and typeof(status) != TYPE_INT:
					out.error = "stage 7 bank poll status is unknown"
				elif out.error.is_empty() and int(status) == 1:
					services["actor_read"].call(state, 0x31, 0x801a4000, 0xaab6, out)
					state.substep += 1
			6:
				if _poll_cd_ready(state, services, out, "stage 7 actor 0"):
					services["actor_read"].call(state, 9, 0x801f0000, 0xabfc, out)
					state.substep += 1
			7:
				if _poll_cd_ready(state, services, out, "stage 7 actor 1"):
					if not services.get("stage_07_to_08_setup") is Callable:
						out.error = "source line11 actor copy, decode and palette setup is unavailable"
						return
					services["stage_07_to_08_setup"].call(state, out)
					if out.error.is_empty():
						services["bank_start"].call(state, 1, 0, out)
					if out.error.is_empty(): state.next_stage()
			_:
				out.error = "prologue stage 7 substep outside source dispatch"
	else:
		out.error = "prologue stage 7 step outside source dispatch"
	if out.error.is_empty(): services["arrival_draw"].call(state,1,out)
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_08_tint(state: State, value: int) -> void:
	for key: String in ["D_801220E0", "D_801220E1", "D_801220E2"]:
		state.opening[key] = value & 255
	if state.opening.get("actor96") is Dictionary:
		state.opening.actor96["brightness"] = value & 255

static func _stage_08_rei_banner(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	# prologue_rei_banner_run: preserve pre/post increment and native read order.
	if state.mode_step != 0 or state.substep < 0 or state.substep > 7:
		out.error = "prologue stage 8 is outside source dispatch"
		return
	if not services.get("rei_banner_effect") is Callable:
		out.error = "source Rei banner presentation and actor setup is unavailable"
		return
	var effect: Callable = services.rei_banner_effect
	var counter: int = int(state.opening.get("prologue_scene_counter", 0))
	effect.call(state, "flash" if state.substep == 0 else "white", counter, out)
	if not out.error.is_empty(): return
	match state.substep:
		0:
			counter += 4
			if counter >= 0x100:
				state.substep += 1
				counter = 0
			else:
				_stage_08_tint(state, counter / 4)
			state.opening["prologue_scene_counter"] = counter
		1:
			state.opening["prologue_scene_counter"] = counter + 1
			if counter >= 0xB:
				effect.call(state, "color_cut", 0, out)
				if out.error.is_empty():
					state.substep += 1
					state.opening["prologue_scene_counter"] = 0
		2:
			_stage_08_tint(state, counter)
			counter += 8
			state.opening["prologue_scene_counter"] = counter
			if counter >= 0x81:
				state.opening["prologue_line"] = 12
				state.opening["prologue_scene_counter"] = 0
				services["line_start"].call(state, 12, out)
				if out.error.is_empty(): state.substep += 1
		3:
			if not services.get("cd_audio_state") is Callable:
				out.error = "Rei banner requires source voice playback status"
			else:
				var status: Variant = services.cd_audio_state.call(state)
				if not status is int: out.error = "Rei banner voice status is not an integer"
				elif (status & 4) != 0: state.substep += 1
		4:
			services["line_run"].call(state, out)
			if out.error.is_empty(): effect.call(state, "lip_sync", 0, out)
		5:
			services["actor_read"].call(state, 0x31, 0x801a4000, 0xaae7, out)
			if out.error.is_empty(): state.substep += 1
		6:
			if _poll_cd_ready(state, services, out, "stage 8 actor 0"):
				effect.call(state, "clear_for_read", 0, out)
				if out.error.is_empty(): services["actor_read"].call(state, 0x19, ACTOR_DEST_PRIMARY, 0xac05, out)
				if out.error.is_empty(): state.substep += 1
		7:
			if _poll_cd_ready(state, services, out, "stage 8 actor 1"):
				effect.call(state, "next_scene", 0, out)
				if out.error.is_empty(): state.next_stage()
	if out.error.is_empty(): services["arrival_draw"].call(state, 1, out)
	if out.error.is_empty(): services["outer_draw"].call(state, 2, out)


static func _stage_09_rei_lines(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	# prologue_rei_lines_13_and_14_run: twelve source substeps, including
	# native text-page and actor-animation boundaries; never timer stand-ins.
	if state.mode_step != 0 or state.substep < 0 or state.substep > 11:
		out.error = "prologue stage9 is outside source dispatch"; return
	if not services.get("rei_lines_effect") is Callable:
		out.error = "source Rei dialogue actor services are unavailable"; return
	var effect: Callable = services.rei_lines_effect
	var counter: int = int(state.opening.get("prologue_scene_counter",0))
	match state.substep:
		0:
			state.opening["prologue_line"] = 13
			services.line_start.call(state,13,out)
			if not out.error.is_empty(): return
			state.opening["prologue_scene_counter"] = 0
			state.substep += 1
			services.arrival_draw.call(state,1,out)
		1:
			if counter < 0xff:
				counter += 1; state.opening["prologue_scene_counter"] = counter
				effect.call(state,"flash",counter,out)
			if not out.error.is_empty(): return
			effect.call(state,"wait_voice",0,out)
			if not out.error.is_empty(): return
			if not services.get("cd_audio_state") is Callable:
				out.error = "Rei line13 requires original CD playback state"; return
			var status: Variant = services.cd_audio_state.call(state)
			if not status is int:
				out.error = "Rei line13 CD state is not an integer"; return
			if (status & 4) != 0: state.substep += 1
			services.arrival_draw.call(state,1,out)
		2:
			if counter < 0xff:
				counter += 1; state.opening["prologue_scene_counter"] = counter
			effect.call(state,"flash",counter,out)
			if not out.error.is_empty(): return
			services.line_run.call(state,out)
			if not out.error.is_empty(): return
			services.arrival_draw.call(state,1,out)
			if not out.error.is_empty(): return
			effect.call(state,"lip_sync",0,out)
			if not out.error.is_empty(): return
			var boundary: Variant = effect.call(state,"page_boundary",0,out)
			if not out.error.is_empty(): return
			if not boundary is bool:
				out.error = "Rei line13 requires actual get_now_k page boundary"; return
			if boundary:
				state.substep += 1
				effect.call(state,"page_transition",0,out)
				if not out.error.is_empty(): return
				effect.call(state,"flash",counter,out)
		3:
			if counter >= 4:
				effect.call(state,"flash",counter,out)
				if not out.error.is_empty(): return
				state.opening["prologue_scene_counter"] = counter-4
			services.line_run.call(state,out)
			if not out.error.is_empty(): return
			effect.call(state,"lip_sync",0,out)
		4:
			effect.call(state,"animate_exit",0,out)
			if out.error.is_empty(): state.substep += 1
		5:
			# C checks actor99.flags twice; actor100 must not become an added gate.
			var done: Variant = effect.call(state,"exit_done",0,out)
			if not out.error.is_empty(): return
			if not done is bool:
				out.error = "Rei exit requires actual actor99 completion"; return
			if done:
				state.substep += 1; state.opening["prologue_scene_counter"] = 0
		6:
			state.opening["prologue_scene_counter"] = counter+1
			if counter >= 0x1f: state.substep += 1
		7:
			effect.call(state,"line14_setup",0,out)
			if not out.error.is_empty(): return
			state.opening["prologue_line"] = 14
			services.line_start.call(state,14,out)
			if out.error.is_empty(): state.substep += 1
		8:
			effect.call(state,"line14_hold",0,out)
			if not out.error.is_empty(): return
			services.line_run.call(state,out)
			if not out.error.is_empty(): return
			effect.call(state,"lip_sync",0,out)
		9:
			services.actor_read.call(state,0x31,0x801a4000,0xaa85,out)
			if out.error.is_empty(): state.substep += 1
		10:
			if _poll_cd_ready(state,services,out,"stage9 O11"):
				services.actor_read.call(state,0x21,0x801e8000,0xac1e,out)
				if out.error.is_empty(): state.substep += 1
		11:
			if _poll_cd_ready(state,services,out,"stage9 O11 copy"):
				effect.call(state,"next_scene",0,out)
				if out.error.is_empty(): state.next_stage()
	if not out.error.is_empty(): return
	# Source tests the post-switch substep, including next_stage resetting it.
	if state.substep < 6:
		var blink_draw := rng.next("prologue_stage9:blink")
		if (blink_draw & 0xff) == 0xff: effect.call(state,"blink",0,out)
	if out.error.is_empty(): services.outer_draw.call(state,2,out)


static func _stage_10_lines_15_16(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if _run_line_pair(state, 15, 0, services, out):
		pass
	elif _run_line_pair(state, 16, 2, services, out):
		pass
	elif state.substep == 4:
		services["actor_read"].call(state, 25, ACTOR_DEST_PRIMARY, 0xab18, out)
		state.substep += 1
	elif state.substep == 5:
		if _poll_cd_ready(state, services, out, "stage 10 actor 0"):
			services["actor_read"].call(state, 9, ACTOR_DEST_SECONDARY, 0xac3f, out)
			state.substep += 1
	elif state.substep == 6:
		if _poll_cd_ready(state, services, out, "stage 10 actor 1"):
			services["background_read"].call(state, 0xacf6, 1, out)
			state.substep += 1
	elif state.substep == 7:
		if _decode_bg_and_check(state, 1, 0xacf6, services, out, "stage 10 bg"):
			_emit_audio_command(state, AUDIO_CMD_SYSTEM_FLAG, services, out)
			_emit_audio_command(state, AUDIO_CMD_FADE_C1, services, out)
			services["bank_load_start"].call(state, "prologue_shiori", out)
			state.next_stage()
	else:
		out.error = "prologue stage 10 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_11_line_17(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	var counter: int = int(state.opening.get("prologue_scene_counter", 0))
	if not _run_line_pair(state, 17, 0, services, out):
		match state.substep:
			2:
				var status: Variant = services["bank_poll"].call(state, out)
				if out.error.is_empty() and typeof(status) != TYPE_INT:
					out.error = "stage 11 bank poll status is unknown"
				elif out.error.is_empty() and int(status) == 1:
					services["bank_start"].call(state, 1, 0, out)
					services["actor_read"].call(state, 25, ACTOR_DEST_PRIMARY, 0xab18, out)
					state.substep += 1
			3:
				if _poll_cd_ready(state, services, out, "stage 11 actor 0"):
					_emit_audio_command(state, AUDIO_CMD_SE_604, services, out)
					services["background_read"].call(state, 0xad05, 0, out)
					state.substep += 1
			4:
				if _decode_bg_and_check(state, 0, 0xad05, services, out, "stage 11 bg"):
					services["actor_read"].call(state, 33, ACTOR_DEST_SECONDARY, 0xac50, out)
					state.substep += 1
			5:
				if _poll_cd_ready(state, services, out, "stage 11 actor 1"):
					state.opening["prologue_scene_counter"] = 0
					state.substep += 1
			6:
				counter += 1
				state.opening["prologue_scene_counter"] = counter
				if counter >= SHIORI_WAIT_FRAMES:
					state.next_stage()
			_:
				out.error = "prologue stage 11 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_12_shiori_load(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	var counter: int = int(state.opening.get("prologue_scene_counter", FULL_BRIGHTNESS))
	match state.substep:
		0:
			services["actor_read"].call(state, 25, ACTOR_DEST_PRIMARY, 0xab49, out)
			state.substep += 1
		1:
			if _poll_cd_ready(state, services, out, "stage 12 actor 0"):
				services["actor_read"].call(state, 5, ACTOR_DEST_SECONDARY, 0xac91, out)
				state.substep += 1
		2:
			if _poll_cd_ready(state, services, out, "stage 12 actor 1"):
				services["background_read"].call(state, 0xad0b, 1, out)
				state.substep += 1
		3:
			if _decode_bg_and_check(state, 1, 0xad0b, services, out, "stage 12 bg"):
				state.opening["prologue_scene_counter"] = FULL_BRIGHTNESS
				state.substep += 1
		4:
			counter -= 2
			state.opening["prologue_scene_counter"] = counter
			if counter < 0:
				state.next_stage()
		_:
			out.error = "prologue stage 12 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_13_shiori_line_18(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	var counter: int = int(state.opening.get("prologue_scene_counter", 0))
	match state.substep:
		0:
			state.opening["prologue_scene_counter"] = 0
			state.substep += 1
		1:
			counter += 2
			state.opening["prologue_scene_counter"] = counter
			if counter >= FULL_BRIGHTNESS:
				state.substep += 1
		2:
			state.opening["prologue_line"] = 18
			services["line_start"].call(state, 18, out)
			state.substep += 1
		3:
			var blink_draw: int = rng.next("prologue_stage13:blink")
			if (blink_draw & BLINK_MASK_FF) == BLINK_MASK_FF:
				state.opening["prologue_blink"] = 1
			services["line_run"].call(state, out)
		4:
			services["actor_read"].call(state, 17, ACTOR_DEST_SECONDARY, 0xac9a, out)
			state.substep += 1
		5:
			if _poll_cd_ready(state, services, out, "stage 13 actor"):
				state.next_stage()
		_:
			out.error = "prologue stage 13 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_14_shiori_line_19(state: State, rng: Rng, services: Dictionary, out: Session.FrameResult) -> void:
	match state.substep:
		0:
			state.opening["prologue_line"] = 19
			services["line_start"].call(state, 19, out)
			state.substep += 1
		1:
			var blink_draw: int = rng.next("prologue_stage14:blink")
			if (blink_draw & BLINK_MASK_FF) == BLINK_MASK_FF:
				state.opening["prologue_blink"] = 1
			services["line_run"].call(state, out)
		2:
			services["background_read"].call(state, 0xacec, 0, out)
			state.substep += 1
		3:
			if _decode_bg_and_check(state, 0, 0xacec, services, out, "stage 14 bg"):
				services["actor_read"].call(state, 25, ACTOR_DEST_PRIMARY, 0xab7a, out)
				state.substep += 1
		4:
			if _poll_cd_ready(state, services, out, "stage 14 actor 0"):
				services["actor_read"].call(state, 13, ACTOR_DEST_SECONDARY, 0xacbb, out)
				state.substep += 1
		5:
			if _poll_cd_ready(state, services, out, "stage 14 actor 1"):
				state.next_stage()
		_:
			out.error = "prologue stage 14 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_15_lines_20_to_31(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	var handled: bool = false
	for index: int in range(6):
		if _run_line_pair(state, 20 + index, index * 2, services, out):
			handled = true
			break
	if not handled:
		if state.substep == 12:
			state.opening["prologue_dynamic_line"] = "shiori_birthday"
			services["line_start"].call(state, 100, out)
			state.substep += 1
		elif state.substep == 13:
			services["line_run"].call(state, out)
		elif _run_line_pair(state, 26, 14, services, out):
			pass
		elif state.substep == 16:
			state.opening["prologue_dynamic_line"] = "shiori_blood"
			services["line_start"].call(state, 101, out)
			state.substep += 1
		elif state.substep == 17:
			services["line_run"].call(state, out)
		elif _run_line_pair(state, 27, 18, services, out):
			pass
		elif _run_line_pair(state, 28, 20, services, out):
			pass
		elif _run_line_pair(state, 29, 22, services, out):
			pass
		elif _run_line_pair(state, 30, 24, services, out):
			pass
		elif state.substep == 26:
			var actor_x: int = int(state.opening.get("prologue_actor_x", 0)) + 2
			state.opening["prologue_actor_x"] = actor_x
			if actor_x >= 0x40:
				state.mode_step_timer = 0
				state.substep += 1
		elif state.substep == 27:
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			if Math.u32(state.mode_step_timer) >= CLOSING_HOLD_FRAMES:
				state.substep += 1
		elif _run_line_pair(state, 31, 28, services, out):
			pass
		elif state.substep == 30:
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			if Math.u32(state.mode_step_timer) >= CLOSING_HOLD_FRAMES:
				state.substep += 1
		elif state.substep == 31:
			services["background_read"].call(state, 0xacd4, 1, out)
			state.substep += 1
		elif state.substep == 32:
			if _decode_bg_and_check(state, 1, 0xacd4, services, out, "stage 15 bg"):
				services["actor_read"].call(state, 25, ACTOR_DEST_PRIMARY, 0xaa54, out)
				state.substep += 1
		elif state.substep == 33:
			if _poll_cd_ready(state, services, out, "stage 15 actor 0"):
				services["actor_read"].call(state, 13, ACTOR_DEST_SECONDARY, 0xabca, out)
				state.substep += 1
		elif state.substep == 34:
			if _poll_cd_ready(state, services, out, "stage 15 actor 1"):
				state.next_stage()
		else:
			out.error = "prologue stage 15 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_16_lines_32_to_36(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if _run_line_pair(state, 32, 0, services, out):
		pass
	elif _run_line_pair(state, 33, 2, services, out):
		pass
	elif state.substep == 4:
		services["bank_start"].call(state, 0, 1, out)
		state.opening["prologue_line"] = 34
		services["line_start"].call(state, 34, out)
		state.substep += 1
	elif state.substep == 5:
		services["line_run"].call(state, out)
	elif _run_line_pair(state, 35, 6, services, out):
		pass
	elif _run_line_pair(state, 36, 8, services, out):
		pass
	elif state.substep == 10:
		services["background_read"].call(state, 0x496a, 0, out)
		state.substep += 1
	elif state.substep == 11:
		if _decode_bg_and_check(state, 0, 0x496a, services, out, "stage 16 bg"):
			state.next_stage()
	else:
		out.error = "prologue stage 16 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_17_line_37(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if not _run_line_pair(state, 37, 0, services, out):
		match state.substep:
			2:
				services["background_read"].call(state, 0x4973, 1, out)
				state.substep += 1
			3:
				if _decode_bg_and_check(state, 1, 0x4973, services, out, "stage 17 bg"):
					services["actor_read"].call(state, 33, ACTOR_DEST_SECONDARY, 0xac50, out)
					state.substep += 1
			4:
				if _poll_cd_ready(state, services, out, "stage 17 actor"):
					state.next_stage()
			_:
				out.error = "prologue stage 17 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_18_lines_38_to_41(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	if _run_line_pair(state, 38, 0, services, out):
		pass
	elif _run_line_pair(state, 39, 2, services, out):
		pass
	elif _run_line_pair(state, 40, 4, services, out):
		pass
	elif state.substep == 6:
		state.opening["prologue_line"] = 41
		services["line_start"].call(state, 41, out)
		state.substep += 1
	elif state.substep == 7:
		services["line_run"].call(state, out)
		if state.substep > 7:
			state.opening["prologue_scene_counter"] = 0
			state.next_stage()
	else:
		out.error = "prologue stage 18 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_19_flash(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	var counter: int = int(state.opening.get("prologue_scene_counter", 0))
	match state.substep:
		0, 2:
			counter += 4
			if counter >= FLASH_MAX:
				counter = FLASH_MAX
				state.substep += 1
			state.opening["prologue_scene_counter"] = counter
		1:
			counter -= 4
			if counter <= 0:
				counter = 0
				state.substep += 1
			state.opening["prologue_scene_counter"] = counter
		3:
			counter -= 2
			if counter <= 0:
				counter = 0
				state.opening["prologue_scene_counter"] = 0
				state.next_stage()
			else:
				state.opening["prologue_scene_counter"] = counter
		_:
			out.error = "prologue stage 19 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_20_closing(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	var counter: int = int(state.opening.get("prologue_scene_counter", 0))
	if not _run_line_pair(state, 42, 0, services, out):
		if state.substep == 2:
			counter += 1
			state.opening["prologue_scene_counter"] = counter
			if counter >= CLOSING_HOLD_FRAMES:
				_emit_audio_command(state, AUDIO_CMD_SYSTEM_FLAG, services, out)
				_emit_audio_command(state, AUDIO_CMD_FADE_D1, services, out)
				state.next_stage()
		else:
			out.error = "prologue stage 20 substep outside source dispatch"
	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _stage_21_finish(state: State, services: Dictionary, out: Session.FrameResult) -> void:
	match state.substep:
		0:
			if not _call_required_finish_service("finish_cd_reset", state, services, out):
				return
			state.opening["D_800B5BD4"] = 0x0f
			state.opening["screen_brightness"] = FULL_BRIGHTNESS
			if not _call_required_finish_service("finish_bank_reset", state, services, out):
				return
			_emit_audio_command(state, AUDIO_CMD_SYSTEM_FLAG, services, out)
			_emit_audio_command(state, AUDIO_CMD_FADE_D1, services, out)
			if out.error.is_empty():
				services["bank_load_start"].call(state, "normal_gameplay", out)
			if out.error.is_empty():
				state.substep += 1
		1:
			state.opening["D_800B3DB0"] = 0
			state.opening["text_speed"] = 3
			state.rule_state["menu_mode"] = 1
			state.reset_game_mode_fields(MODE_COMMAND_SCREEN)
			state.set_stage(STAGE_COMMAND_RELOAD)
		_:
			out.error = "prologue finish substep outside source dispatch"

	if out.error.is_empty():
		services["outer_draw"].call(state, 2, out)


static func _call_required_finish_service(name: String, state: State, services: Dictionary, out: Session.FrameResult) -> bool:
	var callback: Variant = services.get(name)
	if not callback is Callable or not callback.is_valid():
		out.error = "prologue finish needs service: " + name
		return false
	callback.call(state, out)
	return out.error.is_empty()
