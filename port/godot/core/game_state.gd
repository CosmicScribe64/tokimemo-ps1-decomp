extends RefCounted
## Bounded projection, NOT a New Game initializer or complete save schema.
## Widths: include/game/game_state.h; resets: src/main/game_mode.c.

const Math = preload("res://core/native_math.gd")

var game_mode: int = 0
var mode_stage: int = 0
var mode_step: int = 0
var substep: int = 0
var game_mode_scratch: int = 0
var mode_stage_scratch: int = 0
var ovl_target_game_mode: int = 0
var frame_count: int = 0
var timer_ticks: int = 0
var system_seed: int = 0
var game_mode_timer: int = 0
var mode_stage_timer: int = 0
var mode_step_timer: int = 0
var paused: bool = false
var restart_request: bool = false
var sound_active: bool = false

func timer_interrupt() -> void:
	# main_loop.c::timer_tick; independent of libc RNG and frame_count.
	timer_ticks = Math.add32(timer_ticks, 1)
	system_seed = Math.add32(system_seed, 0x377)

func timer_seed_step() -> int:
	system_seed = Math.add32(system_seed, 0x377)
	return system_seed

func reset_game_mode_fields(native_mode: int) -> void:
	# Field-reset portion only; overlay, voice and message effects need the router.
	game_mode = Math.u8(native_mode)
	mode_stage = 0
	mode_step = 0
	game_mode_timer = 0
	mode_stage_timer = 0
	mode_step_timer = 0
	game_mode_scratch = 0
	mode_stage_scratch = 0
	substep = 0

func set_stage(stage: int) -> void:
	mode_stage = Math.u8(stage)
	mode_step = 0
	mode_stage_timer = 0
	mode_step_timer = 0
	mode_stage_scratch = 0
	substep = 0

func next_stage() -> void:
	set_stage(mode_stage + 1)

func set_step(step: int) -> void:
	mode_step = Math.u8(step)
	mode_step_timer = 0
	substep = 0

func next_step() -> void:
	set_step(mode_step + 1)

func to_words() -> Array[int]:
	# Fixed field order is the trace format, not raw PS1 memory layout.
	return [game_mode, mode_stage, mode_step, substep, game_mode_scratch,
		mode_stage_scratch, ovl_target_game_mode, frame_count, timer_ticks,
		system_seed, game_mode_timer, mode_stage_timer, mode_step_timer,
		int(paused), int(restart_request), int(sound_active)]

func projection() -> Dictionary:
	return {"game_mode": game_mode, "mode_stage": mode_stage, "mode_step": mode_step,
		"substep": substep, "game_mode_scratch": game_mode_scratch,
		"mode_stage_scratch": mode_stage_scratch, "ovl_target_game_mode": ovl_target_game_mode,
		"frame_count": frame_count, "timer_ticks": timer_ticks, "system_seed": system_seed,
		"game_mode_timer": game_mode_timer, "mode_stage_timer": mode_stage_timer,
		"mode_step_timer": mode_step_timer, "paused": paused,
		"restart_request": restart_request, "sound_active": sound_active}

func restore(checkpoint_data: Dictionary) -> void:
	# The projection is the complete owned state for this bounded tranche.
	# Extend it when adding a field; unprojected records cannot be rolled back.
	for field: String in projection():
		set(field, checkpoint_data[field])
