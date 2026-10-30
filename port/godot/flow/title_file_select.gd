extends RefCounted
## Boot path of a fresh New Game: boot_loop from game_state_init through the
## boot movie (mode 0x10), the boot loads (mode 0x00), the title (mode 0x12),
## the NAME_ENT overlay load (mode 0x02), the no-card file select (mode 0x13)
## and the NAME_ENT load stage (mode 0x14 stage 0). It stops at the first
## dispatch of mode 0x14 stage 1, where flow/new_game_entry.gd takes over.
##
## Sources: src/main/main_loop.c (boot_loop, timer_tick), game_mode.c,
## boot_assets.c, title_screen.c, overlay_loader.c, movie_player.c,
## pad_input.c (pad_poll, pad_code_update), src/ovl/name_entry/file_select.c
## and name_entry.c (name_entry_mode_stage_load).
##
## One step() is one boot_loop iteration: the pad word latched by the previous
## draw_frame (pad_update), game_mode_dispatch, game_mode_timer += 1 and
## frame_count += 1. While mode 0x10 plays the movie, one step() is one
## movie_play pad poll instead (the movie loop polls the pad once per decoded
## frame and counts frame_count itself).
##
## Measured timing (not derived from source): the latency of CD reads, sound
## bank loads, the memory-card check and the NAME_ENT overlay load in logic
## iterations, the timer ticks of the movie frames and the few iterations that
## do not take exactly one vblank tick. Every value comes from the original
## under PCSX-Redux/OpenBIOS (.asset-cache/emu/title-filesel-golden-v1, runs
## movie-full, setA..setE; tools/emu/title_filesel/). The runs issue the same
## requests at different times and give the same latencies, except the card
## check, whose rule is below.
##
## The movie, title models, file-select graphics and NAME_ENT load screen are
## not presented here; this is the logic and clock owner only. Paths the port
## does not support end with an explicit refusal: the attract opening (title
## timeout, SELECT, the pad code), the option screen, the soft reset chord and
## a pad word above 16 bits.

const Core = preload("res://core/game_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")

const SCHEMA := "tokimemo.title-file-select.v1"
const MODE_MOVIE := 0x10
const MODE_BOOT := 0x00
const MODE_OVERLAY := 0x02
const MODE_TITLE := 0x12
const MODE_FILE_SELECT := 0x13
const MODE_NAME_ENTRY := 0x14
const PAD_START := 0x800
const PAD_SELECT := 0x100
const PAD_CIRCLE := 0x20
const PAD_CROSS := 0x40
const PAD_UP := 0x1000
const PAD_DOWN := 0x4000
const PAD_LEFT := 0x8000
const PAD_RIGHT := 0x2000
const PAD_SKIP := 0x860 # START | CROSS | CIRCLE (title_screen.c, movie_player.c)
const PAD_ACCEPT := 0x820 # START | CIRCLE (title_menu_select_step)
const PAD_RESET_CHORD := 0x90C # pad_poll soft reset: START | SELECT | L1 | R1
const NAME_ENT_LBA := 0x95EA
const IRIS_UPDATE_STOP := 0x4001 # FILE_SELECT_IRIS_UPDATE_STOP, include/ovl/name_entry.h

## pad_code_table (D_800B3CA8, config/renames/remaining-symbols.txt): up,
## release, up, release, down, release, down, release, left, release, right,
## release, left, release, right, release, cross, circle.
const PAD_CODE_TABLE := [PAD_UP, 0, PAD_UP, 0, PAD_DOWN, 0, PAD_DOWN, 0, PAD_LEFT, 0, PAD_RIGHT, 0,
	PAD_LEFT, 0, PAD_RIGHT, 0, PAD_CROSS, PAD_CIRCLE]

## Measured: timer ticks that elapse between sys_init's timer_event_start and
## game_state_init's bzero (timer_ticks restarts at 0 there; system_data.seed
## does not). Every logic row of every run has seed = 0x377 * (timer_ticks + 6).
const PRE_INIT_TICKS := 6
## Measured: game_state.timer_ticks at each movie_play pad_update of the boot
## movie (movie_play(0, 0xA7, 0x190), 152 frames). Run movie-full, movie.jsonl.
const MOVIE_POLL_TICKS: Array[int] = [25,29,33,37,41,45,49,53,57,61,65,69,73,77,81,85,90,93,97,102,106,110,113,
	118,122,126,129,134,138,142,145,150,154,158,161,166,170,173,177,181,185,189,193,197,201,205,209,213,217,221,
	225,229,233,237,241,246,249,253,257,261,265,269,273,278,282,286,289,294,298,302,305,310,314,318,321,326,330,
	334,336,341,345,349,353,358,362,366,370,374,378,382,386,390,394,398,401,406,410,414,417,422,426,430,433,438,
	442,446,449,454,458,462,465,470,474,478,481,486,490,494,497,502,506,510,513,518,522,526,529,534,538,542,545,
	550,554,558,561,566,570,574,577,582,586,590,593,598,602,606,609,614,617,620,622,625]
## Measured: ticks from the last poll to the next dispatch. A natural end
## decodes one more frame before the loop test fails (movie-full: 625 -> 627);
## a skip leaves at once (setC: START seen at the poll with 221, mode 0x00
## dispatch at 221).
const MOVIE_NATURAL_END_TICKS := 2
const MOVIE_SKIP_TICKS := 0
## Measured: memory_card_check_poll() with no card inserted returns 0xFF on
## the first poll that is at least the 9th and runs while timer_ticks is odd
## (OpenBIOS services the card every other vblank). Runs setA and setE begin
## the check on an even tick and finish at poll 9; setB, setC and setD begin
## on an odd tick and finish at poll 10, all on an odd tick.
const CARD_CHECK_MIN_POLLS := 9
## Measured poll counts: the iteration that sees completion is poll N (polls
## start on the iteration after the request).
const WAITS := {
	"boot_read_0x9D85": 44, # boot_data_load_step cd_read_sectors(0x35, 0x80180000, 0x9D85)
	"boot_read_0x9C97": 17, # cd_read_sectors_nowait(8, 0x801F0000, 0x9C97)
	"title_bank_1": 141, # audio_bank_load_start(1, 0xC592, ...) to audio_bank_load_poll() == 1
	"title_read_0xBE13": 3, # cd_read_sectors(1, 0x801F0000, 0xBE13)
	"title_read_0xA0CC": 99, # cd_read_sectors(0xD6, 0x80180000, 0xA0CC)
	"overlay_read_0x95EA": 52, # ovl_load_start cd_read_sectors(0x60, 0x80132000, 0x95EA) to ovl_load_poll
	"file_select_bank_0": 132, # audio_bank_load_start(0, 0xBF98, ...)
	"file_select_read_0x9DBA": 16, # cd_read_sectors(5, 0x801E0000, 0x9DBA)
	"file_select_read_0x9CB5": 68, # cd_read_sectors(0x89, 0x80180000, 0x9CB5)
	"name_entry_read_0xAA18": 18, # cd_read_sectors_nowait(0xA, 0x80162000, 0xAA18)
	"name_entry_read_0xA9E2": 35, # cd_read_sectors(0x36, 0x80190000, 0xA9E2)
	"name_entry_read_0x9DBF": 16, # cd_read_sectors(6, 0x801E0000, 0x9DBF)
}

var state := Core.new()
var rng := Rng.new()
var scene_work: int = 0 # s32 at 0x801230D0
var memory_card_result: int = 0
var failure: String = ""
var refusal: String = ""
var finished: bool = false
var logic_iterations: int = 0
var trace: Array[PackedInt64Array] = []
var movie_trace: Array[PackedInt64Array] = []
var keep_trace: bool = true
# False once any row was dropped (keep_trace off): trace_sha256 then no longer
# covers the row stream, and handoff() leaves it out.
var trace_complete: bool = true

var _pad_held: int = 0
var _pad_pressed: int = 0
var _pad_code_entered: int = 0
var _pad_code_index: int = 0
var _pad_code_timer: int = 0
var _pad_reset_frames: int = 0
var _movie_frame: int = -1
var _card_polls: int = 0
var _wait_name: String = ""
var _wait_polls: int = 0
var _extra_ticks: int = 0
var _ovl_load_state: int = 0
var _ovl_load_lba: int = 0
var _ovl_target: int = 0
var _iris_radius: int = 0
var _iris_x: int = 0
var _iris_y: int = 0
var _iris_x_speed: int = 0
var _iris_y_speed: int = 0
var _screen_brightness: int = 0

func begin() -> String:
	if not rng.configure(Rng.OPENBIOS_PROFILE, 0):
		return _fail("BIOS RNG profile is unavailable: " + rng.error)
	# boot_start bzero and game_state_init(1): frame_count, timer_ticks and
	# the mode words start at 0; the OpenBIOS rand word is 0 before any srand.
	state.frame_count = 0
	state.timer_ticks = 0
	state.system_seed = 0
	for _i: int in PRE_INIT_TICKS: state.system_seed = Math.add32(state.system_seed, 0x377)
	state.game_mode = MODE_MOVIE # game_state_init stores 0x10
	state.mode_stage = 0; state.mode_step = 0; state.substep = 0
	state.game_mode_timer = 0; state.mode_stage_timer = 0; state.mode_step_timer = 0
	# Iteration 1 dispatches mode 0x10 with the pad words game_state_init
	# cleared (no draw_frame has run): game_mode_start_mode_handler enters
	# movie_play, which blocks the iteration; step() continues it per poll.
	_pad_held = 0
	_pad_pressed = 0
	_record_row()
	_movie_frame = 0
	return ""

## True while the next step() is a movie pad poll.
func in_movie() -> bool:
	return state.game_mode == MODE_MOVIE and failure.is_empty() and refusal.is_empty()

## Timer ticks until the next pad sample, for a presenter that keeps the
## original's wall clock (the movie polls about every 4 vblanks).
func ticks_until_next_sample() -> int:
	if not in_movie() or _movie_frame < 0 or _movie_frame >= MOVIE_POLL_TICKS.size(): return 1
	return maxi(1, MOVIE_POLL_TICKS[_movie_frame] - state.timer_ticks)

## One boot_loop iteration (or one movie poll) with the pad word that the
## previous draw_frame latched.
func step(held: int) -> String:
	if not failure.is_empty() or not refusal.is_empty() or finished: return failure + refusal
	if held < 0 or held > 0xffff: return _fail("pad words above 16 bits (second pad, mouse) are not modelled")
	if state.game_mode == MODE_MOVIE:
		return _movie_poll(held)
	_pad_poll(held)
	if not refusal.is_empty(): return refusal
	_record_row()
	_extra_ticks = 0
	var ticks: int = 1
	match state.game_mode:
		MODE_BOOT: _boot_mode()
		MODE_TITLE: _title_mode()
		MODE_OVERLAY: _overlay_mode()
		MODE_FILE_SELECT: _file_select_mode()
		MODE_NAME_ENTRY: _name_entry_load()
		_: return _fail("mode 0x%02x is outside the modelled boot path" % state.game_mode)
	if not failure.is_empty() or not refusal.is_empty(): return failure + refusal
	state.game_mode_timer = Math.add32(state.game_mode_timer, 1)
	state.frame_count = Math.u32(state.frame_count + 1)
	logic_iterations += 1
	ticks += _extra_ticks
	for _i: int in ticks: state.timer_interrupt()
	# The next iteration dispatches NAME_ENT stage 1, the entry controller's.
	if state.game_mode == MODE_NAME_ENTRY and state.mode_stage == 1: finished = true
	return ""

## The NAME_ENT input-stage context, available once finished.
func handoff() -> Dictionary:
	if not finished: return {"error": "the boot path has not reached NAME_ENT stage 1"}
	var result := {"error": "", "schema": SCHEMA, "frame_count": state.frame_count, "timer_ticks": state.timer_ticks,
		"system_seed": state.system_seed, "scene_work": scene_work, "rng": rng, "held_input": _pad_held,
		"logic_iterations": logic_iterations,
		"provenance": {"clock_origin": "game_state_init (boot path modelled from power-on, movie unpresented)",
			"timing": "measured latencies, .asset-cache/emu/title-filesel-golden-v1", "memory_card": "none inserted"}}
	if trace_complete: result["trace_sha256"] = trace_sha256()
	return result

## sha256 over the canonical rows (tools/port/title_filesel_golden.py
## builds the same stream from the emulator logs). Empty when rows were
## dropped (keep_trace was off for part of the run).
func trace_sha256() -> String:
	if not trace_complete: return ""
	var ctx := HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	# Movie rows hash poll, frame_count, timer_ticks and seed: the emulator logs
	# them at pad_update entry, before that poll's pad_pressed exists.
	for row: PackedInt64Array in movie_trace: ctx.update(("M" + ",".join(Array(row).slice(0, 4).map(str)) + "\n").to_utf8_buffer())
	for row: PackedInt64Array in trace: ctx.update(("L" + ",".join(Array(row).map(str)) + "\n").to_utf8_buffer())
	return ctx.finish().hex_encode()

func _fail(reason: String) -> String:
	if failure.is_empty(): failure = reason
	return failure

func _refuse(reason: String) -> String:
	if refusal.is_empty(): refusal = reason
	return refusal

func _record_row() -> void:
	if not keep_trace:
		trace_complete = false
		return
	# l, mode, stage, step, substep, frame_count, timer_ticks, seed (u32),
	# game_mode_timer, mode_stage_timer, mode_step_timer, scene_work (u32),
	# rand word, pad_pressed, memory_card_result
	trace.append(PackedInt64Array([logic_iterations + 1, state.game_mode, state.mode_stage, state.mode_step,
		state.substep, state.frame_count, state.timer_ticks, Math.u32(state.system_seed),
		Math.u32(state.game_mode_timer), Math.u32(state.mode_stage_timer), Math.u32(state.mode_step_timer),
		Math.u32(scene_work), rng.state, _pad_pressed, memory_card_result]))

# pad_input.c pad_poll (pad device only) and pad_code_update.
func _pad_poll(held: int) -> void:
	var prev: int = _pad_held
	_pad_held = held
	_pad_pressed = Math.u32(~prev & held)
	if (held & PAD_RESET_CHORD) == PAD_RESET_CHORD:
		var count: int = _pad_reset_frames
		_pad_reset_frames = Math.u8(count + 1)
		if count >= 0x81:
			_refuse("SOFT_RESET_UNSUPPORTED: the reset chord was held for 130 frames")
			return
	else:
		_pad_reset_frames = 0
	if _pad_code_entered != 0:
		_pad_code_timer = Math.u8(_pad_code_timer + 1)
		if _pad_code_timer == 0:
			_pad_code_entered = 0
			_pad_code_index = 0
	else:
		if held == PAD_CODE_TABLE[_pad_code_index]:
			_pad_code_index = Math.u8(_pad_code_index + 1)
			if _pad_code_index >= 0x12: _pad_code_entered = 1
			_pad_code_timer = 0x10
		_pad_code_timer = Math.u8(_pad_code_timer - 1)
		if _pad_code_timer == 0: _pad_code_index = 0

func _wait(name: String) -> bool:
	if _wait_name != name:
		_wait_name = name
		_wait_polls = 0
	_wait_polls += 1
	if _wait_polls < int(WAITS[name]): return false
	_wait_name = ""
	_wait_polls = 0
	return true

func _game_mode_set(mode: int) -> void:
	state.game_mode = mode
	state.mode_stage = 0; state.mode_step = 0; state.substep = 0
	state.game_mode_timer = 0; state.mode_stage_timer = 0; state.mode_step_timer = 0
	state.game_mode_scratch = 0; state.mode_stage_scratch = 0
	# game_mode_request_overlay: 0x11 and 0x13..0x15 run from NAME_ENT.
	if mode in [0x11, MODE_FILE_SELECT, MODE_NAME_ENTRY, 0x15]:
		_ovl_target = mode
		state.ovl_target_game_mode = mode
		state.game_mode = MODE_OVERLAY

func _stage_next() -> void:
	state.mode_stage += 1
	state.mode_step = 0; state.substep = 0
	state.mode_stage_timer = 0; state.mode_step_timer = 0; state.mode_stage_scratch = 0

func _step_next() -> void:
	state.mode_step += 1
	state.mode_step_timer = 0; state.substep = 0

func _movie_poll(held: int) -> String:
	# One movie frame: frame_count += 1, pad_update, then the 0x860 test.
	_movie_frame += 1
	state.frame_count = Math.u32(state.frame_count + 1)
	_advance_ticks_to(MOVIE_POLL_TICKS[_movie_frame - 1])
	_pad_poll(held)
	if not refusal.is_empty(): return refusal
	if not keep_trace: trace_complete = false
	else:
		movie_trace.append(PackedInt64Array([_movie_frame, state.frame_count, state.timer_ticks,
			Math.u32(state.system_seed), _pad_pressed]))
	var skipped: bool = (_pad_pressed & PAD_SKIP) != 0
	if not skipped and _movie_frame < MOVIE_POLL_TICKS.size(): return ""
	# Leave movie_play: the rest of game_mode_start_mode_handler, then
	# game_mode_set(GAME_MODE_BOOT); dispatch and boot_loop finish iteration 1.
	for _i: int in (MOVIE_SKIP_TICKS if skipped else MOVIE_NATURAL_END_TICKS): state.timer_interrupt()
	_game_mode_set(MODE_BOOT)
	_movie_frame = -1
	state.game_mode_timer = Math.add32(state.game_mode_timer, 1)
	state.frame_count = Math.u32(state.frame_count + 1)
	logic_iterations += 1
	return ""

func _advance_ticks_to(target: int) -> void:
	while state.timer_ticks < target: state.timer_interrupt()

# mode 0x00, boot_mode_handler / boot_mode_step / boot_data_load_step.
func _boot_mode() -> void:
	if state.mode_stage != 0:
		_game_mode_set(MODE_TITLE)
		return
	match state.mode_step:
		0:
			match state.substep:
				0: state.substep = 1 # cd_read_sectors(0x35, 0x80180000, 0x9D85)
				1: if _wait("boot_read_0x9D85"): state.substep = 2 # boot_hud_init; nowait read 0x9C97
				2: if _wait("boot_read_0x9C97"): _step_next()
		1, 2: _step_next() # boot_hud_textures_step, debug_font_load
		_: _stage_next()

# mode 0x12, title_mode_handler.
func _title_mode() -> void:
	state.mode_stage_timer = Math.add32(state.mode_stage_timer, 1)
	match state.mode_stage:
		0: _title_setup()
		1: _title_intro()
		2: _title_menu()
		3: _title_exit()
		_: _fail("title stage %d is game_mode_reset_to_title" % state.mode_stage)
	if not refusal.is_empty() or not failure.is_empty(): return
	# The pad code at the menu: every read of this route has completed by stage
	# 2, so cd_queue_idle() is 1 there.
	if _pad_code_entered != 0 and state.mode_stage >= 2 and state.game_mode == MODE_TITLE:
		_refuse("ATTRACT_UNSUPPORTED: the pad code starts the attract opening (mode 0x11)")

func _title_setup() -> void:
	match state.substep:
		0:
			_pad_code_entered = 0; _pad_code_timer = 0; _pad_code_index = 0
			_screen_brightness = 0
			state.substep = 1 # audio_bank_load_start(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83)
		1:
			# Measured: the first poll iteration takes no timer tick (the setup
			# iteration before it overran into the next vblank).
			if _wait_name != "title_bank_1": _extra_ticks = -1
			if _wait("title_bank_1"): state.substep = 2 # cd_read_sectors(1, 0x801F0000, 0xBE13)
		2:
			if _wait("title_read_0xBE13"): state.substep = 3 # cd_read_sectors(0xD6, 0x80180000, 0xA0CC)
		3:
			if _wait("title_read_0xA0CC"):
				rng.reseed(Math.u32(state.timer_ticks)) # srand(game_state.timer_ticks)
				rng.next("title_models_link") # r = rand(): model order offset
				_stage_next()

func _title_intro() -> void:
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	match state.mode_step:
		0:
			if _screen_brightness < 0x80: _screen_brightness += 1
			if state.substep == 0 and Math.u32(state.mode_step_timer) >= 0x259: _step_next()
			if _pad_pressed & PAD_SKIP:
				_screen_brightness = 0x80
				_stage_next()
		1:
			if (Math.u32(state.mode_step_timer) >> 1) >= 0xFF or (_pad_pressed & PAD_SKIP):
				if _pad_pressed & PAD_SKIP: _stage_next()
				else: _step_next()
		2:
			if Math.u32(state.mode_step_timer) >= 0xFF: _stage_next()
			elif _pad_pressed & PAD_SKIP: _stage_next()

func _title_menu_cursor_update() -> void:
	# Pad branch (input_device is the pad); the mouse branch is not modelled.
	match _pad_pressed & 0x5000:
		PAD_UP:
			state.mode_step_timer = 0
			scene_work = 0
		PAD_DOWN:
			state.mode_step_timer = 0
			scene_work = 1

func _title_menu() -> void:
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	match state.mode_step:
		0:
			scene_work = 0
			_screen_brightness = 0x80
			_title_menu_cursor_update()
			_step_next()
		1:
			_title_menu_cursor_update()
			if _pad_pressed & PAD_ACCEPT: _step_next() # audio_command(0x501)
			if Math.u32(state.mode_step_timer) >= 0x259 or (_pad_pressed & PAD_SELECT):
				_refuse("ATTRACT_UNSUPPORTED: the title menu timed out or SELECT started the attract opening (mode 0x11)")
		2:
			if Math.u32(state.mode_step_timer) >= 0x21: _stage_next()

func _title_exit() -> void:
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	match state.mode_step:
		0:
			if Math.u32(state.mode_step_timer) >= 0x80: _step_next()
		1:
			match Math.u32(scene_work):
				0: _game_mode_set(MODE_FILE_SELECT)
				1: _refuse("OPTION_UNSUPPORTED: the title option screen (mode 0x16) is not modelled")
				_: _fail("title exit with scene_work %d is game_mode_reset_to_title" % scene_work)

# mode 0x02, ovl_loader_mode_handler.
func _overlay_mode() -> void:
	match _ovl_load_state:
		0:
			var resident: int = _ovl_load_lba
			if _ovl_target not in [0x11, MODE_FILE_SELECT, MODE_NAME_ENTRY, 0x15]:
				_fail("overlay target 0x%02x is outside the modelled boot path" % _ovl_target)
				return
			_ovl_load_lba = NAME_ENT_LBA
			_ovl_load_state = 3 if resident == _ovl_load_lba else 1
		1:
			if _wait("overlay_read_0x95EA"): _ovl_load_state = 2
		2:
			# ovl_load_verify: cd_file_checksum_ok over 0x60 sectors. Measured: this
			# iteration takes 4 timer ticks. The port reads authenticated packs, so
			# the checksum of the original load always passes.
			_extra_ticks = 3
			_ovl_load_state = 3
		3:
			# ovl_load_enter: game_mode = target, mode_stage_set(stage),
			# mode_step_set(step), substep restored (all 0 after game_mode_set).
			_ovl_load_state = 0
			var saved_step: int = state.mode_step
			var saved_substep: int = state.substep
			state.game_mode = state.ovl_target_game_mode
			state.mode_stage_timer = 0; state.mode_stage_scratch = 0
			state.mode_step = saved_step; state.mode_step_timer = 0
			state.substep = saved_substep

# mode 0x13, file_select_mode_handler (no memory card inserted).
func _file_select_mode() -> void:
	state.mode_stage_timer = Math.add32(state.mode_stage_timer, 1)
	match state.mode_stage:
		0: _file_select_init()
		1: _file_select_card_check()
		_: _fail("file select stage 0x%02x needs a memory card, which the port does not have" % state.mode_stage)
	if not failure.is_empty() or not refusal.is_empty(): return
	_file_select_iris_update()
	# file_select_flash_update
	if (state.mode_stage | state.mode_step) != 0: scene_work = Math.s32(scene_work - 1)

func _file_select_init() -> void:
	match state.mode_step:
		0:
			match state.substep:
				0:
					# file_select_init_settings: options_dirty is 0 on a cold boot.
					rng.reseed(Math.u32(state.system_seed)) # srand(system_data.seed)
					_screen_brightness = 0
					scene_work = 0xFF
					_iris_radius = 1; _iris_x = -0x50; _iris_y = -0x50; _iris_x_speed = 8; _iris_y_speed = 8
					state.substep = 1 # audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1)
				1:
					if _wait("file_select_bank_0"): _step_next()
		1:
			# file_select_init_assets
			_screen_brightness = mini(_screen_brightness + 4, 0x80)
			match state.substep:
				0: state.substep = 1 # cd_read_sectors(5, 0x801E0000, 0x9DBA)
				1:
					# Measured: the first poll iteration takes no timer tick.
					if _wait_name != "file_select_read_0x9DBA": _extra_ticks = -1
					if _wait("file_select_read_0x9DBA"): state.substep = 2 # cd_read_sectors(0x89, 0x80180000, 0x9CB5)
				2:
					if _wait("file_select_read_0x9CB5"):
						_screen_brightness = 0x80
						state.substep = 3
				3: _step_next()
		2:
			_stage_next() # file_select_init_menus

func _file_select_card_check() -> void:
	match state.mode_step:
		0: _step_next() # memory_card_state_reset, check message
		1:
			memory_card_result = 0 # memory_card_check_poll step 0
			_card_polls += 1
			if _card_polls >= CARD_CHECK_MIN_POLLS and (state.timer_ticks & 1) == 1:
				_card_polls = 0
				memory_card_result = 0xFF # NO CARD
				_step_next()
		2: _step_next() # no-card message
		3: if _pad_pressed & PAD_SKIP: _step_next()
		4: _step_next() # "start a new game?" prompt
		5:
			if _pad_pressed & PAD_START or _pad_pressed & PAD_CIRCLE:
				# file_select_settings_restore: no saved options on a cold boot.
				_game_mode_set(MODE_NAME_ENTRY)
			elif _pad_pressed & PAD_CROSS:
				_step_next()
		6: _step_next() # insert-card message
		7:
			if _pad_pressed & PAD_CIRCLE:
				state.mode_step = 0; state.mode_step_timer = 0; state.substep = 0

func _file_select_iris_update() -> void:
	if (state.mode_stage != 0 or state.mode_step != 0) and _iris_radius < IRIS_UPDATE_STOP:
		_iris_radius = int((_iris_radius * 16 + 0x70) / 15)
		_iris_x += _iris_x_speed
		_iris_y += _iris_y_speed
		if _iris_x >= 0xA1: _iris_x_speed = -(rng.next("file_select_background_iris_update:x_high") & 0xF)
		if _iris_y >= 0x79: _iris_y_speed = -(rng.next("file_select_background_iris_update:y_high") & 0xF)
		if _iris_x < -0xA0: _iris_x_speed = rng.next("file_select_background_iris_update:x_low") & 0xF
		if _iris_y < -0x78: _iris_y_speed = rng.next("file_select_background_iris_update:y_low") & 0xF

# mode 0x14 stage 0, name_entry_mode_stage_load.
func _name_entry_load() -> void:
	if state.mode_stage != 0:
		_fail("NAME_ENT stage %d belongs to the entry controller" % state.mode_stage)
		return
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	match state.mode_step:
		0:
			match state.substep:
				0: state.substep = 1 # cd_read_sectors_nowait(0xA, 0x80162000, 0xAA18)
				1:
					if _wait("name_entry_read_0xAA18"):
						# bg_still_decode_to_vram; measured: this iteration takes 3 ticks.
						_extra_ticks = 2
						state.substep = 2 # cd_read_sectors(0x36, 0x80190000, 0xA9E2)
				2: if _wait("name_entry_read_0xA9E2"): _step_next() # cd_read_sectors(6, 0x801E0000, 0x9DBF)
		1:
			if state.substep == 0:
				if _wait("name_entry_read_0x9DBF"):
					_screen_brightness = 0
					state.substep = 1
			else:
				_screen_brightness = (_screen_brightness + 4) & 0xFF
				if _screen_brightness >= 0x80: _stage_next()
