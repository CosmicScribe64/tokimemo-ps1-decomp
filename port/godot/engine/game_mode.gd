extends RefCounted
## Port of src/main/game_mode.c (original object 0x80042540-0x80042A00, T-39301).
## Game mode dispatch and the mode/stage/step setters, on the game memory (core/game_memory.gd).
## Contract: wiki/engine-porting-contract.md. Test: config/port/engine-diff/game_mode.json (captures) and
## tests/engine/game_mode_test.gd (invented-memory checks written from the C).

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/game_mode.c"
## Every function of the C file; all are ported. A listed name must be a method of this class.
const C_FUNCTIONS: Array[String] = [
	"game_mode_dispatch", "game_mode_next", "mode_stage_next", "mode_step_next", "game_mode_set",
	"mode_stage_set", "mode_step_set", "game_mode_request_overlay",
]
## Argument widths ("s32"/"u8"/...) and the return type ("void" or the C type) of each function, read by the
## differential harness: v0 is compared only for a non-void return.
## mode_stage_next, mode_step_next, mode_stage_set and mode_step_set are s32 functions without a return
## statement (the "FAKE: z" idiom of the C): the original leaves v0 = 0 (observed in every capture).
const C_SIGNATURES: Dictionary = {
	"game_mode_dispatch": {"args": [], "ret": "void"},
	"game_mode_next": {"args": [], "ret": "void"},
	"mode_stage_next": {"args": [], "ret": "s32"},
	"mode_step_next": {"args": [], "ret": "s32"},
	"game_mode_set": {"args": ["s32"], "ret": "void"},
	"mode_stage_set": {"args": ["s32"], "ret": "s32"},
	"mode_step_set": {"args": ["s32"], "ret": "s32"},
	"game_mode_request_overlay": {"args": [], "ret": "void"},
}

## "LOAD FUNC EXEC", the first string of this object's rodata (0x800AF390 in config/objects/main.txt;
## the bytes are in the executable image at that address).
const STR_LOAD_FUNC_EXEC: int = 0x800AF390
## ovl_entry (config/symbol_addrs.txt 0x80123110): a u32 holding the code address of the loaded overlay's entry.
const OVL_ENTRY: String = "ovl_entry"

# include/game_enums.h GameMode
const GAME_MODE_BOOT: int = 0x00
const GAME_MODE_OVERLAY_LOADER: int = 0x02

## The case list that game_mode_dispatch (prints "LOAD FUNC EXEC", jumps through ovl_entry) and
## game_mode_request_overlay (asks the loader) share in the C: modes whose code lives in an overlay.
## (GDScript match cannot spread a list over lines, so the shared list is a constant used with a guard.)
const OVERLAY_MODES: Array[int] = [
	0x11, 0x13, 0x14, 0x15, 0x20, 0x21, 0x22, 0x35,
	0x36, 0x37, 0x3A, 0x43, 0x45, 0x50, 0x51, 0x52,
	0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
	0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62,
	0x63, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x80, 0x81, 0x82, 0x90, 0x91, 0xC2, 0xC3, 0xC4,
]

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func game_mode_dispatch() -> void:
	var gs = engine.memory.game_state()
	match gs.get_field("game_mode"): # *(u8 *)&game_state.game_mode
		0x00:
			engine.call_c("boot_mode_handler")
		0x01:
			if Math.u8(gs.get_field("boot_hold")) == 0: # *(u8 *)&game_state.boot_hold
				engine.call_c("game_mode_set", [GAME_MODE_BOOT])
		0xFF:
			engine.call_c("exit_function")
		0xFE:
			engine.call_c("menu_vram_view_mode_handler")
		0x30:
			engine.call_c("schedule_init")
		0x31:
			engine.call_c("schedule_day_screen_handler")
		0x34:
			engine.call_c("settings_command_screen_mode_handler")
		0x12:
			engine.call_c("title_mode_handler")
		0x40:
			engine.call_c("week_day")
		0x46:
			engine.call_c("holiday_mode_handler")
		0x44:
			engine.call_c("uwasa_main")
		0x41:
			engine.call_c("settings_new_year_prep_mode_handler")
		0x42:
			engine.call_c("settings_christmas_prep_mode_handler")
		0x03:
			_call_ovl_entry()
		0x10:
			engine.call_c("game_mode_start_mode_handler")
		0x38:
			engine.call_c("schedule_month_start_mode_handler")
		0x39:
			engine.call_c("schedule_day_notice_mode_handler")
		0x32:
			engine.call_c("schedule_week_end_mode_handler")
		0x02:
			engine.call_c("ovl_loader_mode_handler")
		0xD0:
			engine.call_c("game_mode_tt_game_mode_handler")
		var overlay_mode when overlay_mode in OVERLAY_MODES:
			engine.call_c("debug_print_string", [STR_LOAD_FUNC_EXEC, 0x14, 0x2C])
			if engine.halted():
				return
			_call_ovl_entry()
		_:
			engine.call_c("game_mode_reset_to_title")
	if engine.halted():
		return
	gs.set_field("game_mode_timer", Math.add32(gs.get_field("game_mode_timer"), 1))

## ((s32 (*)()) ovl_entry)(): the code address is the value of the u32 global ovl_entry.
func _call_ovl_entry() -> void:
	engine.call_address(OVL_ENTRY, engine.memory.read_global("ovl_entry"))

func game_mode_next() -> void:
	var gs = engine.memory.game_state()
	gs.set_field("game_mode", gs.get_field("game_mode") + 1) # u8: wraps at 0x100
	_clear_mode_progress(gs)
	engine.call_c("game_mode_request_overlay")

## The nine stores shared by game_mode_next and game_mode_set, in the C order (mode_stage .. substep).
func _clear_mode_progress(gs) -> void:
	gs.set_field("mode_stage", 0)
	gs.set_field("mode_step", 0)
	gs.set_field("game_mode_timer", 0)
	gs.set_field("mode_stage_timer", 0)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("game_mode_scratch", 0)
	gs.set_field("mode_stage_scratch", 0)
	gs.set_field("substep", 0)

## s32 with no return statement; the original leaves v0 = 0 (the `z` read through `p`, T-11610).
func mode_stage_next() -> int:
	var gs = engine.memory.game_state()
	gs.set_field("mode_stage", gs.get_field("mode_stage") + 1) # u8
	gs.set_field("mode_step", 0)
	gs.set_field("mode_stage_timer", 0)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("mode_stage_scratch", 0)
	gs.set_field("substep", 0) # game_state.substep = *p, with p = &z, z = 0
	return 0

func mode_step_next() -> int:
	var gs = engine.memory.game_state()
	gs.set_field("mode_step", gs.get_field("mode_step") + 1) # u8
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("substep", 0)
	return 0

## Switch to a mode and reset stage, step, timers, substep.
func game_mode_set(mode: int) -> void:
	var gs = engine.memory.game_state()
	gs.set_field("game_mode", mode) # u8 store keeps the low byte of the s32 argument
	_clear_mode_progress(gs)
	engine.call_c("game_mode_request_overlay")
	var target: int = gs.get_field("ovl_target_game_mode") # u32 t = (u8)
	engine.memory.write_global("talk_voice_wait_frames", 0)
	if target != 0x5E and target != 0x50:
		engine.call_c("message_window_title_show", [0])

func mode_stage_set(stage: int) -> int:
	var gs = engine.memory.game_state()
	gs.set_field("mode_stage", stage)
	gs.set_field("mode_step", 0)
	gs.set_field("mode_stage_timer", 0)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("mode_stage_scratch", 0)
	gs.set_field("substep", 0)
	return 0

func mode_step_set(step: int) -> int:
	var gs = engine.memory.game_state()
	gs.set_field("mode_step", step)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("substep", 0)
	return 0

## s32 in the C, but every path ends in `return;` or falls off the end: no return value.
func game_mode_request_overlay() -> void:
	var gs = engine.memory.game_state()
	match gs.get_field("game_mode"): # *(u8 *)&game_state.game_mode
		var overlay_mode when overlay_mode in OVERLAY_MODES:
			gs.set_field("ovl_target_game_mode", gs.get_field("game_mode"))
			gs.set_field("game_mode", GAME_MODE_OVERLAY_LOADER)
