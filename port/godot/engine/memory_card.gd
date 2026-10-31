extends RefCounted
## Port of src/main/memory_card.c (original object 0x80053650-0x80056300, T-39313 / T-39301 contract).
## BIOS memory-card events, the card job pollers (check, scan, system/save read and write), the file jobs on
## "bu00:" and the system / game save images in the 0x2000-byte card block (memory_card_block), plus the two
## MDEC still-image helpers (bg_still_*) that share the original object.
## Contract: wiki/engine-porting-contract.md. Tests: tests/engine/memory_card_test.gd (invented memory, from the
## C) and config/port/engine-diff/memory_card.json (emulator captures).
##
## Every SDK card call (OpenEvent, TestEvent, _card_info, open, read, write, close, format, delete, ...) goes
## through engine.call_c by its symbol name: no module lists them, so each is a refusal until a card backend is
## attached (tests and the harness install doubles; the port's card image model services/memory_card_image.gd is
## the intended backend, see the ticket's facade request). Blocking spins (Hw_Test, memory_card_status_wait,
## the file_exists retry) stop at a bound and on a refusal.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/memory_card.c"
const C_FUNCTIONS: Array[String] = [
	"bg_still_buffers_set", "bg_still_decode_to_vram", "card_ev_set", "Sw_Start", "Hw_Start", "Sw_Test",
	"Sw_Clear", "Hw_Test", "Hw_Clear", "Hw_Stop", "memory_card_step_set", "memory_card_step_next",
	"memory_card_start", "memory_card_stop", "memory_card_event_handle", "memory_card_status_wait",
	"memory_card_check_poll", "memory_card_format", "memory_card_scan_poll", "memory_card_system_read_poll",
	"memory_card_system_write_poll", "memory_card_save_read_poll", "memory_card_save_write_poll",
	"memory_card_file_create", "memory_card_file_delete", "memory_card_file_read", "memory_card_file_write",
	"memory_card_file_exists", "memory_card_system_file_probe", "memory_card_save_files_probe",
	"memory_card_block_clear", "memory_card_file_write_sync", "memory_card_slot_summary_store",
	"memory_card_system_bank_build", "memory_card_system_bank_load", "memory_card_game_bank_build",
	"memory_card_bank_validate", "memory_card_game_bank_load", "memory_card_file_name_build",
	"memory_card_state_reset",
]
const C_UNPORTED: Array[String] = []
## memory_card_system_bank_build, memory_card_system_bank_load (except its -1 exit) and
## memory_card_game_bank_build are s32 functions whose C has no final return: v0 is not defined.
const C_SIGNATURES: Dictionary = {
	"bg_still_buffers_set": {"args": ["s32", "s32", "s32"], "ret": "void"},
	"bg_still_decode_to_vram": {"args": ["u16", "u16", "u16", "u16"], "ret": "s32"},
	"card_ev_set": {"args": [], "ret": "void"},
	"Sw_Start": {"args": [], "ret": "void"},
	"Hw_Start": {"args": [], "ret": "void"},
	"Sw_Test": {"args": [], "ret": "u8"},
	"Sw_Clear": {"args": [], "ret": "void"},
	"Hw_Test": {"args": [], "ret": "u8"},
	"Hw_Clear": {"args": [], "ret": "void"},
	"Hw_Stop": {"args": [], "ret": "void"},
	"memory_card_step_set": {"args": ["u8"], "ret": "void"},
	"memory_card_step_next": {"args": [], "ret": "void"},
	"memory_card_start": {"args": [], "ret": "void"},
	"memory_card_stop": {"args": [], "ret": "void"},
	"memory_card_event_handle": {"args": ["u8"], "ret": "void"},
	"memory_card_status_wait": {"args": [], "ret": "s32"},
	"memory_card_check_poll": {"args": [], "ret": "s32"},
	"memory_card_format": {"args": [], "ret": "s32"},
	"memory_card_scan_poll": {"args": [], "ret": "s32"},
	"memory_card_system_read_poll": {"args": [], "ret": "u8"},
	"memory_card_system_write_poll": {"args": [], "ret": "s32"},
	"memory_card_save_read_poll": {"args": [], "ret": "u8"},
	"memory_card_save_write_poll": {"args": [], "ret": "u8"},
	"memory_card_file_create": {"args": ["s32"], "ret": "s32"},
	"memory_card_file_delete": {"args": ["s32"], "ret": "s32"},
	"memory_card_file_read": {"args": ["s32"], "ret": "s32"},
	"memory_card_file_write": {"args": ["s32"], "ret": "s32"},
	"memory_card_file_exists": {"args": ["s32"], "ret": "s32"},
	"memory_card_system_file_probe": {"args": [], "ret": "void"},
	"memory_card_save_files_probe": {"args": [], "ret": "s32"},
	"memory_card_block_clear": {"args": [], "ret": "void"},
	"memory_card_file_write_sync": {"args": ["s32"], "ret": "s32"},
	"memory_card_slot_summary_store": {"args": [], "ret": "void"},
	"memory_card_system_bank_build": {"args": ["s32"], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
	"memory_card_system_bank_load": {"args": ["s32"], "ret": "s32", "ret_defined": [-1]},
	"memory_card_game_bank_build": {"args": ["s32"], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
	"memory_card_bank_validate": {"args": ["s32"], "ret": "s32"},
	"memory_card_game_bank_load": {"args": ["s32"], "ret": "s32"},
	"memory_card_file_name_build": {"args": ["ptr", "s32"], "ret": "s32"},
	"memory_card_state_reset": {"args": [], "ret": "void"},
}

# String literals of the object (config/objects/main.txt rodata 0x800AFBF0-0x800AFDE0; the bytes are in the
# executable image).
const STR_BU00: int = 0x800AFBF0 # "bu00:"
const STR_SC: int = 0x800AFBF8 # "SC" (memory_card_file_write)
const STR_TITLE: int = 0x800AFBFC # "ときめきメモリアル" (Shift-JIS), file_write
const STR_SYSTEM_SUFFIX: int = 0x800AFC10 # "・システム", file_write
const STR_SC_SYNC: int = 0x800AFC1C # "SC", file_write_sync
const STR_TITLE_SYNC: int = 0x800AFC20 # file_write_sync
const STR_SYSTEM_SUFFIX_SYNC: int = 0x800AFC34 # file_write_sync
const STR_DIGIT_BASE: int = 0x800AFC40 # "0".."F", one 4-byte cell each (memory_card_file_name_build)

# Data symbols (the generated layout names them; addresses are the layout's).
const BLOCK: int = 0x80123120 # memory_card_block
const BLOCK_VERSION: int = 0x80123122 # D_80123122
const BLOCK_ICON_FLAGS: int = 0x80123123 # D_80123123
const BLOCK_TITLE: int = 0x80123124 # D_80123124
const BLOCK_RESERVED: int = 0x80123164 # D_80123164
const BLOCK_PALETTE: int = 0x80123180 # D_80123180
const BLOCK_ICON: int = 0x801231A0 # D_801231A0
const BANKS: int = 0x80123320 # D_80123320 (two MemoryCardBank of 0xF00)
const BANK_SIZE: int = 0xF00
const BLOCK_SIZE: int = 0x2000
const SYSTEM_DATA: int = 0x800E7D10 # system_data
const SLOT_STATUS: int = 0x800E7D14 # D_800E7D14 == system_data.slot_status
const SYS_SAVE_SPIN_LIMIT: int = 0x4000 # bound of the polling spins (a hardware wait cannot be modelled)

var engine # GameEngine; set when the engine registers the module

# ---- small helpers

func _gs(field: String) -> int:
	return engine.gs_address(field)

## D_800B32CC[file_index - 1]: D_800B32CC is a `u8 **` (a word holding the address of a table of string pointers).
func _slot_label(file_index: int) -> int:
	return engine.memory.u32(engine.memory.read_global("D_800B32CC") + (file_index - 1) * 4)

## An SDK call whose C result is an s32 compared with -1 (open, read, write): call_c leaves SDK results
## unnarrowed (they have no C_SIGNATURES), so the port narrows them where the C reads them as signed.
func _sdk_s32(function_name: String, args: Array) -> int:
	return Math.s32(engine.call_c(function_name, args))

func _copy(dst: int, src: int, count: int) -> void:
	engine.memory.write_bytes(dst, engine.memory.read_bytes(src, count))

# ---- MDEC still-image helpers

func bg_still_buffers_set(arg0: int, arg1: int, arg2: int) -> void:
	engine.call_c("DecDCTReset", [0])
	engine.call_c("func_800869C8", [0])
	engine.call_c("func_800869A4", [0])
	engine.memory.write_global("bg_still_variable_length_code_buffer", arg0)
	engine.memory.write_global("bg_still_out_buffer", arg1)
	engine.memory.write_global("bg_still_bitstream", arg2)

## RECT rect lives at sp+0x50 of the 0x58 frame (asm); the C passes &rect to func_8009C884.
func bg_still_decode_to_vram(arg0: int, arg1: int, arg2: int, arg3: int) -> int:
	var x: int = Math.u16(arg0)
	var y: int = Math.u16(arg1)
	var width: int = Math.u16(arg2)
	var height: int = Math.u16(arg3)
	var m = engine.memory
	var stream: int = Math.u32(m.read_global("bg_still_bitstream"))
	if (m.u32(stream) & 0xFFFF0000) != 0x38000000 or (m.u32(stream + 4) & 0xFF010000) != 0x10000:
		return -1
	engine.call_c("DecDCTReset", [0])
	engine.call_c("func_800869C8", [0])
	engine.call_c("func_800869A4", [0])
	var vlc: int = Math.u32(m.read_global("bg_still_variable_length_code_buffer"))
	var out_buffer: int = Math.u32(m.read_global("bg_still_out_buffer"))
	engine.call_c("func_80086870", [stream, vlc])
	engine.call_c("func_800868BC", [vlc, 2])
	var rect: int = engine.frame_base() + 0x50
	m.write_u16(rect + 2, y)
	m.write_u16(rect + 4, 0x10)
	m.write_u16(rect, x)
	m.write_u16(rect + 6, height)
	var end: int = Math.u32(x + width)
	var v: int = ((height + 8) >> 4) << 7
	var guard: int = 0
	# `if (rect.x < end) do { ... } while (rect.x < end)`: rect.x is an s16 compared as unsigned with end.
	while Math.u32(m.s16(rect)) < end:
		engine.call_c("func_80086944", [out_buffer, v])
		engine.call_c("func_80086984", [0])
		engine.call_c("func_8009C884", [rect, out_buffer])
		m.write_u16(rect, m.s16(rect) + 0x10)
		guard += 1
		if engine.halted() or guard > 0x10000:
			break
	return 1

# ---- events

func card_ev_set() -> void:
	engine.call_c("Sw_Start")
	engine.call_c("Hw_Start")
	engine.call_c("InitCARD", [1])
	engine.call_c("StartCARD")
	engine.call_c("_bu_init")
	engine.call_c("_card_auto", [0])

func _open_events(kind: int, names: Array) -> void:
	engine.call_c("EnterCriticalSection")
	var specs: Array = [4, 0x8000, 0x100, 0x2000]
	for i: int in 4:
		var handle: int = engine.call_c("OpenEvent", [kind, specs[i], 0x2000, 0])
		engine.memory.write_global(names[i], handle)
	for i: int in 4:
		engine.call_c("EnableEvent", [engine.memory.read_global(names[i])])
	engine.call_c("ExitCriticalSection")

func Sw_Start() -> void:
	_open_events(0xF4000001, ["memory_card_software_event_done", "memory_card_software_event_error",
		"memory_card_software_event_timeout", "memory_card_software_event_new_card"])

func Hw_Start() -> void:
	_open_events(0xF0000011, ["memory_card_hardware_event_done", "memory_card_hardware_event_error",
		"memory_card_hardware_event_timeout", "memory_card_hardware_event_new_card"])

func Sw_Test() -> int:
	engine.gs_set("memory_card_result", 0)
	if engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_done")]) != 0:
		engine.gs_set("memory_card_result", 1)
	elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_error")]) != 0:
		engine.gs_set("memory_card_result", 2)
	elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_timeout")]) != 0:
		engine.gs_set("memory_card_result", 4)
	elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_new_card")]) != 0:
		engine.gs_set("memory_card_result", 8)
	if engine.gs_get("memory_card_result") != 0:
		engine.call_c("Sw_Clear")
	return engine.gs_get("memory_card_result")

func Sw_Clear() -> void:
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_done")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_error")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_timeout")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_software_event_new_card")])

## do { poll the four hardware events } while (result == 0): a hardware wait. The loop stops at a refusal
## (every TestEvent then returns 0 at once) and at SYS_SAVE_SPIN_LIMIT passes, which is a refusal naming the wait.
func Hw_Test() -> int:
	engine.gs_set("memory_card_result", 0)
	var passes: int = 0
	while true:
		if engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_done")]) != 0:
			engine.gs_set("memory_card_result", 0x81)
		elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_error")]) != 0:
			engine.gs_set("memory_card_result", 0x82)
		elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_timeout")]) != 0:
			engine.gs_set("memory_card_result", 0x84)
		elif engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_new_card")]) != 0:
			engine.gs_set("memory_card_result", 0x88)
		if engine.gs_get("memory_card_result") != 0 or engine.halted():
			break
		passes += 1
		if passes >= SYS_SAVE_SPIN_LIMIT:
			engine.refuse("Hw_Test", "no hardware memory-card event after %d polls (blocking wait)" % passes)
			break
	engine.call_c("Hw_Clear")
	return engine.gs_get("memory_card_result")

func Hw_Clear() -> void:
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_done")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_error")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_timeout")])
	engine.call_c("TestEvent", [engine.memory.read_global("memory_card_hardware_event_new_card")])

func Hw_Stop() -> void:
	engine.call_c("EnterCriticalSection")
	engine.call_c("CloseEvent", [engine.memory.read_global("memory_card_hardware_event_done")])
	engine.call_c("CloseEvent", [engine.memory.read_global("memory_card_hardware_event_error")])
	engine.call_c("CloseEvent", [engine.memory.read_global("memory_card_hardware_event_timeout")])
	engine.call_c("CloseEvent", [engine.memory.read_global("memory_card_hardware_event_new_card")])
	engine.call_c("ExitCriticalSection")

# ---- job state

func memory_card_step_set(arg0: int) -> void:
	engine.gs_set("memory_card_result", 0)
	engine.gs_set("memory_card_step", Math.u8(arg0))

func memory_card_step_next() -> void:
	engine.gs_set("memory_card_result", 0)
	engine.gs_set("memory_card_step", engine.gs_get("memory_card_step") + 1)

func memory_card_start() -> void:
	engine.gs_set("memory_card_busy", 1)
	engine.gs_set("memory_card_step", 0)
	engine.gs_set("memory_card_result", 0)
	engine.gs_set("memory_card_retry_count", 0)
	engine.gs_set("memory_card_timer", 0)

func memory_card_stop() -> void:
	engine.gs_set("memory_card_busy", 0)
	engine.gs_set("memory_card_timer", 0)

## case 4 falls through into case 0 (return) when the retry count stays below 3.
func memory_card_event_handle(arg0: int) -> void:
	match engine.call_c("Sw_Test"):
		1:
			engine.call_c("memory_card_step_next")
		8:
			engine.call_c("memory_card_step_set", [Math.u8(arg0)])
		4:
			engine.gs_set("memory_card_retry_count", engine.gs_get("memory_card_retry_count") + 1)
			engine.call_c("memory_card_step_set", [0])
			if engine.gs_get("memory_card_retry_count") >= 3:
				engine.gs_set("memory_card_busy", 0)
				engine.gs_set("memory_card_result", 0xFF)
		0:
			pass
		_:
			engine.call_c("memory_card_step_set", [0])

## while (status == 0) status = _card_status(0): a spin on card hardware (bounded, stops at a refusal).
func memory_card_status_wait() -> int:
	var status: int = engine.call_c("_card_status", [0])
	var passes: int = 0
	while status == 0 and not engine.halted():
		status = engine.call_c("_card_status", [0])
		passes += 1
		if passes >= SYS_SAVE_SPIN_LIMIT:
			engine.refuse("memory_card_status_wait", "_card_status stayed 0 for %d polls (blocking wait)" % passes)
			break
	match status:
		1, 2, 4, 8:
			engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
		0x11, 0x21:
			engine.gs_set("memory_card_retry_count", engine.gs_get("memory_card_retry_count") + 1)
			engine.call_c("memory_card_step_set", [0])
			engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	return status

## switch (*(u8 *)&memory_card_step). Step 0x20 calls func_800AD950 (VSync(0)), a blocking wait that refuses.
func memory_card_check_poll() -> int:
	engine.gs_set("memory_card_timer", engine.gs_get("memory_card_timer") + 1)
	if engine.gs_get("memory_card_busy") != 0:
		match engine.gs_get("memory_card_step"):
			0x0:
				engine.gs_set("memory_card_result", 0)
				engine.call_c("Sw_Clear")
				if engine.call_c("_card_info", [0]) == 1:
					engine.call_c("memory_card_step_next")
			0x1:
				engine.call_c("memory_card_event_handle", [0x20])
			0x2:
				if engine.call_c("_card_load", [0]) == 1:
					engine.call_c("memory_card_step_next")
			0x3:
				engine.call_c("memory_card_event_handle", [0x40])
			0x4:
				engine.gs_set("memory_card_busy", 0)
				engine.gs_set("memory_card_result", 1)
			0x20:
				engine.call_c("func_800AD950", [0])
				engine.call_c("Hw_Clear")
				if engine.call_c("_card_clear", [0]) == 0:
					engine.call_c("memory_card_step_set", [0])
				else:
					engine.call_c("Hw_Test")
					if engine.gs_get("memory_card_result") == 0x88:
						engine.call_c("memory_card_step_set", [0x30])
					else: # default, 0x0 and 0x81
						engine.call_c("memory_card_step_set", [0])
			0x30:
				if engine.call_c("_card_load", [0]) == 1:
					engine.call_c("memory_card_step_next")
			0x31:
				engine.call_c("memory_card_event_handle", [0x40])
			0x32:
				engine.gs_set("memory_card_busy", 0)
				engine.gs_set("memory_card_result", 1)
			0x40:
				engine.gs_set("memory_card_busy", 0)
				engine.gs_set("memory_card_result", 0xF1)
	return Math.s32(engine.gs_get("memory_card_result"))

func memory_card_format() -> int:
	if engine.call_c("format", [STR_BU00]) == 1:
		return 1
	return 0

func memory_card_scan_poll() -> int:
	engine.gs_set("memory_card_timer", engine.gs_get("memory_card_timer") + 1)
	if engine.gs_get("memory_card_busy") != 0:
		var step: int = engine.gs_get("memory_card_step")
		if step == 0:
			engine.gs_set("memory_card_result", 0)
			engine.gs_set("save_slot", step)
			if engine.call_c("memory_card_file_exists", [step]) == 1:
				engine.memory.write_global("slot_info_from_system_data", 1)
				engine.call_c("memory_card_step_next")
			else:
				engine.gs_set("memory_card_retry_count", engine.gs_get("memory_card_retry_count") + 1)
				if engine.gs_get("memory_card_retry_count") >= 3:
					engine.memory.write_global("slot_info_from_system_data", 0)
					engine.call_c("memory_card_step_next")
		elif step < 0xF:
			engine.gs_set("save_slot", step)
			var slot: int = engine.gs_get("save_slot")
			var present: int = 1 if engine.call_c("memory_card_file_exists", [slot]) == 1 else 0
			engine.memory.write_u8(SLOT_STATUS + engine.gs_get("save_slot"), present)
			engine.gs_set("memory_card_step", engine.gs_get("memory_card_step") + 1)
		else:
			engine.gs_set("memory_card_busy", 0)
			engine.gs_set("memory_card_result", 2)
	return Math.s32(engine.gs_get("memory_card_result"))

## The four file pollers share one shape: step 0 starts the file job, step 1 waits for the card status, step 2
## publishes the result, step 16 is the error exit. `slot_zero` pins save_slot to 0 (the system file).
func _file_poll(job: String, slot_zero: bool, done_result: int) -> void:
	engine.gs_set("memory_card_timer", engine.gs_get("memory_card_timer") + 1)
	if engine.gs_get("memory_card_busy") == 0:
		return
	match engine.gs_get("memory_card_step"):
		0:
			engine.call_c("Sw_Clear")
			if slot_zero:
				engine.gs_set("save_slot", 0)
			if engine.call_c(job, [0 if slot_zero else engine.gs_get("save_slot")]) == 1:
				engine.call_c("memory_card_step_next")
			else:
				engine.gs_set("memory_card_result", 0xD0)
				engine.gs_set("memory_card_busy", 0)
		1:
			if engine.call_c("memory_card_status_wait") == 1:
				engine.call_c("memory_card_step_next")
		2:
			engine.gs_set("memory_card_result", done_result)
			engine.gs_set("memory_card_busy", 0)
		16:
			engine.gs_set("memory_card_busy", 0)
			engine.gs_set("memory_card_result", 0xF0)

func memory_card_system_read_poll() -> int:
	_file_poll("memory_card_file_read", true, 3)
	return engine.gs_get("memory_card_result")

func memory_card_system_write_poll() -> int:
	_file_poll("memory_card_file_write", true, 0x13)
	return Math.s32(engine.gs_get("memory_card_result"))

func memory_card_save_read_poll() -> int:
	_file_poll("memory_card_file_read", false, 4)
	return engine.gs_get("memory_card_result")

func memory_card_save_write_poll() -> int:
	_file_poll("memory_card_file_write", false, 0x14)
	return engine.gs_get("memory_card_result")

# ---- file jobs on bu00:

## Name buffer: sp+0x2C of the 0x50 frame.
func memory_card_file_create(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x2C
	engine.call_c("memory_card_file_name_build", [name, file_index])
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	var fd: int = _sdk_s32("open", [name, 0x10200])
	engine.memory.write_global("memory_card_file_descriptor", fd)
	if fd == -1:
		return -1
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	return 1

## Name buffer sp+0x3C, retry counter sp+0x5C of the 0x60 frame. The counter is read before it is written
## (FAKE in the C: the original reads stale stack), so the loop starts from whatever the frame holds.
func memory_card_file_delete(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x3C
	var counter: int = engine.frame_base() + 0x5C
	engine.call_c("memory_card_file_name_build", [name, file_index])
	if engine.call_c("delete", [name]) != 1:
		var retry: int = engine.memory.s32(counter)
		var guard: int = 0
		while true:
			retry = Math.add32(retry, 1)
			if retry >= 3:
				return -1
			if engine.call_c("delete", [name]) == 1:
				break
			guard += 1
			if engine.halted() or guard > 0x1000:
				return -1
		engine.memory.write_u32(counter, retry)
	return 1

## Name buffer sp+0x4C of the 0x78 frame; retry counters are registers.
func memory_card_file_read(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x4C
	var retry: int = 0
	engine.call_c("memory_card_block_clear")
	engine.call_c("memory_card_file_name_build", [name, file_index])
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	engine.memory.write_global("memory_card_file_descriptor", _sdk_s32("open", [name, 0x8001]))
	if engine.memory.read_global("memory_card_file_descriptor") == -1:
		while true:
			retry += 1
			if retry >= 16:
				return -1
			engine.memory.write_global("memory_card_file_descriptor", _sdk_s32("open", [name, 0x8001]))
			if engine.memory.read_global("memory_card_file_descriptor") != -1:
				break
			if engine.halted():
				return -1
		retry = 0
	if _sdk_s32("read", [engine.memory.read_global("memory_card_file_descriptor"), BLOCK, BLOCK_SIZE]) == -1:
		while true:
			retry += 1
			if retry >= 16:
				return -1
			if _sdk_s32("read", [engine.memory.read_global("memory_card_file_descriptor"), BLOCK, BLOCK_SIZE]) != -1:
				break
			if engine.halted():
				return -1
	return 1

## Frame 0x70: name buffer sp+0x54, the two u8 outputs of hud_event_log_scan at sp+0x47 (last qualifying girl)
## and sp+0x46.
func memory_card_file_write(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x54
	var girl_out: int = engine.frame_base() + 0x47
	var other_out: int = engine.frame_base() + 0x46
	var retry: int = 0
	engine.call_c("memory_card_block_clear")
	engine.call_c("memory_card_file_name_build", [name, file_index])
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	engine.memory.write_global("memory_card_file_descriptor", _sdk_s32("open", [name, 0x8002]))
	if engine.memory.read_global("memory_card_file_descriptor") == -1:
		while true:
			retry += 1
			if engine.call_c("memory_card_file_create", [file_index]) == -1:
				return -1
			if retry >= 16:
				return -1
			engine.memory.write_global("memory_card_file_descriptor", _sdk_s32("open", [name, 0x8002]))
			if engine.memory.read_global("memory_card_file_descriptor") != -1:
				break
			if engine.halted():
				return -1
		retry = 0
	engine.call_c("strncpy", [BLOCK, STR_SC, 2])
	engine.memory.write_u8(BLOCK_VERSION, 0x13)
	engine.memory.write_u8(BLOCK_ICON_FLAGS, 1)
	engine.call_c("bzero", [BLOCK_TITLE, 0x40])
	engine.call_c("strncpy", [BLOCK_TITLE, STR_TITLE, 0x14])
	if file_index != 0:
		engine.call_c("strncat", [BLOCK_TITLE, _slot_label(file_index), 6])
	else:
		engine.call_c("strncat", [BLOCK_TITLE, STR_SYSTEM_SUFFIX, 0xC])
	engine.call_c("bzero", [BLOCK_RESERVED, 0x1C])
	engine.call_c("hud_event_log_scan", [girl_out, other_out, file_index])
	var idx: int = engine.memory.u8(girl_out)
	if idx >= 0xD:
		idx = 0
	engine.call_c("memcpy", [BLOCK_PALETTE, 0x800B4390 + (idx << 5), 0x20]) # memory_card_icon_palettes
	engine.call_c("memcpy", [BLOCK_ICON, engine.memory.u32(0x800B58B0 + idx * 4), 0x180]) # memory_card_icon_images[idx]
	if file_index != 0:
		engine.call_c("memory_card_game_bank_build", [0])
	else:
		engine.call_c("memory_card_system_bank_build", [0])
	_copy(BANKS + BANK_SIZE, BANKS, BANK_SIZE) # D_80123320[1] = D_80123320[0]
	var guard: int = 0
	while _sdk_s32("write", [engine.memory.read_global("memory_card_file_descriptor"), BLOCK, BLOCK_SIZE]) == -1:
		retry += 1
		if retry >= 16:
			return -1
		guard += 1
		if engine.halted() or guard > 0x1000:
			return -1
	return 1

## Name buffer sp+0x44 of the 0x60 frame. Tries open(name, 2) up to max+1 times (8 or 0x10 + 1).
func memory_card_file_exists(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x44
	var limit: int = 8 if file_index != 0 else 0x10
	engine.call_c("memory_card_file_name_build", [name, file_index])
	var i: int = 0
	var fd: int = _sdk_s32("open", [name, 2])
	engine.memory.write_global("memory_card_file_descriptor", fd)
	while fd == -1:
		i += 1
		if limit < i or engine.halted():
			return 0
		fd = _sdk_s32("open", [name, 2])
		engine.memory.write_global("memory_card_file_descriptor", fd)
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	return 1

func memory_card_system_file_probe() -> void:
	if engine.call_c("memory_card_file_exists", [0]) != 0:
		engine.memory.write_global("slot_info_from_system_data", 1)
		return
	engine.memory.write_global("slot_info_from_system_data", 0)

func memory_card_save_files_probe() -> int:
	var n: int = 0
	var p: int = 0x800E7D11 # D_800E7D11
	var i: int = 1
	while true:
		var t: int = engine.call_c("memory_card_file_exists", [i])
		i += 1
		p += 1
		engine.memory.write_u8(p + 3, t)
		n += 1
		if i == 0xF or engine.halted():
			break
	return n

func memory_card_block_clear() -> void:
	engine.call_c("bzero", [BLOCK, BLOCK_SIZE])

## Name buffer sp+0x44 of the 0x60 frame. Unlike file_write it uses the plain strcpy/strcat title, copies
## palette 0 / icon 0 and writes the block with one write that must return 0x2000.
func memory_card_file_write_sync(file_index: int) -> int:
	var name: int = engine.frame_base() + 0x44
	engine.call_c("memory_card_block_clear")
	engine.call_c("memory_card_file_name_build", [name, file_index])
	engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
	engine.memory.write_global("memory_card_file_descriptor", _sdk_s32("open", [name, 2]))
	if engine.memory.read_global("memory_card_file_descriptor") == -1:
		return -1
	engine.call_c("strncpy", [BLOCK, STR_SC_SYNC, 2])
	engine.memory.write_u8(BLOCK_VERSION, 0x13)
	engine.memory.write_u8(BLOCK_ICON_FLAGS, 1)
	engine.call_c("bzero", [BLOCK_TITLE, 0x40])
	engine.call_c("strcpy", [BLOCK_TITLE, STR_TITLE_SYNC])
	if file_index != 0:
		engine.call_c("strcat", [BLOCK_TITLE, _slot_label(file_index)])
	else:
		engine.call_c("strcat", [BLOCK_TITLE, STR_SYSTEM_SUFFIX_SYNC])
	engine.call_c("bzero", [BLOCK_RESERVED, 0x1C])
	engine.call_c("memcpy", [BLOCK_PALETTE, 0x800B4390, 0x20])
	engine.call_c("memcpy", [BLOCK_ICON, engine.memory.u32(0x800B58B0), 0x180])
	if file_index != 0:
		engine.call_c("memory_card_game_bank_build", [0])
	else:
		engine.call_c("memory_card_system_bank_build", [0])
	_copy(BANKS + BANK_SIZE, BANKS, BANK_SIZE)
	# The C's one-pass loop: write(fd, block, 0x2000) must return 0x2000.
	if _sdk_s32("write", [engine.memory.read_global("memory_card_file_descriptor"), BLOCK, BLOCK_SIZE]) != BLOCK_SIZE:
		engine.call_c("close", [engine.memory.read_global("memory_card_file_descriptor")])
		return -1
	return 1

## Copies the current game into the per-slot summary tables of system_data (offsets: include/game/system_data.h).
func memory_card_slot_summary_store() -> void:
	var m = engine.memory
	var slot: int = m.u8(SYSTEM_DATA + 0x26) & 0xF # selected_slot
	m.write_u8(SYSTEM_DATA + 0x858 + slot, engine.gs_get("year"))
	m.write_u8(SYSTEM_DATA + 0x868 + slot, engine.gs_get("month"))
	m.write_u8(SYSTEM_DATA + 0x878 + slot, engine.gs_get("day"))
	engine.call_c("memcpy", [SYSTEM_DATA + 0x888 + slot * 8, _gs("player_family_name"), 8])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x908 + slot * 8, _gs("player_given_name"), 8])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x988 + slot * 16, _gs("player_nickname"), 0x10])
	var params: int = _gs("param_condition")
	for i: int in 9:
		m.write_u32(SYSTEM_DATA + 0xA88 + (i * 16 + slot) * 4, m.u32(params + i * 4)) # metrics[i][slot]
	var marks: int = _gs("girls") + 15 * 0x38 + 0x14 # girls[15].scene_counters
	for i: int in 16:
		m.write_u8(SYSTEM_DATA + 0xCC8 + slot * 16 + i, m.u8(marks + i))
	m.write_u8(SYSTEM_DATA + 0xEC8 + slot, m.u16(_gs("player_flags")) >> 12)

# ---- save images (offsets: include/game/save_image.h, include/game/system_data.h)

## Byte sum of the bank's data (bytes 4..0xEFF), stored as the bank's first word.
func _bank_checksum_store(arg0: int) -> void:
	var m = engine.memory
	var base: int = BLOCK + arg0 * BANK_SIZE + 0x200
	var sum: int = 0
	for offset: int in range(4, 0xF00):
		sum += m.u8(base + offset)
	m.write_u32(base, sum)

func memory_card_system_bank_build(arg0: int) -> int:
	var m = engine.memory
	var bank: int = BANKS # always bank 0 for the data (checksum below uses arg0)
	var selected: int = m.u8(SYSTEM_DATA + 0x26)
	m.write_u8(SYSTEM_DATA + 4 + selected, 1) # slot_status[selected_slot] = 1
	engine.call_c("memcpy", [bank + 4, SYSTEM_DATA + 4, 16])
	engine.call_c("memcpy", [bank + 0x14, SYSTEM_DATA + 0x14, 16])
	m.write_u8(bank + 0x24, m.u8(SYSTEM_DATA + 0x24))
	m.write_u8(bank + 0x25, m.u8(SYSTEM_DATA + 0x25))
	if selected != 0:
		m.write_u8(bank + 0x26, selected % 15)
	_copy(bank + 0x28, SYSTEM_DATA + 0x2C, 0x14) # config0 (SystemDataSaveView +0x2C -> SaveConfigBlock +0x28)
	_copy(bank + 0x3C, SYSTEM_DATA + 0x40, 0x18) # config1
	m.write_u8(bank + 0x54, m.u8(SYSTEM_DATA + 0x27)) # album_state[0]
	m.write_u8(bank + 0x55, m.u8(SYSTEM_DATA + 0x28))
	engine.call_c("memcpy", [bank + 0x56, SYSTEM_DATA + 0x58, 0x800])
	engine.call_c("memcpy", [bank + 0x856, SYSTEM_DATA + 0x858, 16])
	engine.call_c("memcpy", [bank + 0x866, SYSTEM_DATA + 0x868, 16])
	engine.call_c("memcpy", [bank + 0x876, SYSTEM_DATA + 0x878, 16])
	engine.call_c("memcpy", [bank + 0x886, SYSTEM_DATA + 0x888, 0x80])
	engine.call_c("memcpy", [bank + 0x906, SYSTEM_DATA + 0x908, 0x80])
	engine.call_c("memcpy", [bank + 0x986, SYSTEM_DATA + 0x988, 0x100])
	engine.call_c("memcpy", [bank + 0xA86, SYSTEM_DATA + 0xEC8, 16])
	engine.call_c("memcpy", [bank + 0xA98, SYSTEM_DATA + 0xA88, 0x240])
	engine.call_c("memcpy", [bank + 0xCD8, SYSTEM_DATA + 0xCC8, 0x100])
	if engine.gs_get("options_dirty") == 0:
		if m.u8(SYSTEM_DATA + 0x26) != 0:
			var layout: int = SYSTEM_DATA + 0xDC8
			engine.call_c("memcpy", [layout, m.global_address("D_80125C10"), 0x40])
			engine.call_c("memcpy", [layout + 0xF0, m.global_address("mascot_anchor_x"), 4])
			m.write_u8(layout + 0xF4, m.read_global("hud_mascot_choice"))
			m.write_u8(layout + 0xF5, m.read_global("mascot_length"))
			m.write_u8(layout + 0xF6, m.read_global("mascot_swing"))
			engine.call_c("memcpy", [bank + 0xDD8, layout, 0x100])
	_bank_checksum_store(arg0)
	return 0

func memory_card_system_bank_load(arg0: int) -> int:
	var m = engine.memory
	if engine.call_c("memory_card_bank_validate", [0]) == -1:
		return -1
	var bank: int = Math.u32((engine.gs_get("album_slot") * 0xF00 + 0x200) + BLOCK)
	_copy(SYSTEM_DATA + 0x14, bank + 0x14, 16)
	m.write_u8(SYSTEM_DATA + 0x24, m.u8(SYSTEM_DATA + 0x24) | m.u8(bank + 0x24))
	m.write_u8(SYSTEM_DATA + 0x25, m.u8(bank + 0x25))
	m.write_u8(SYSTEM_DATA + 0x26, m.u8(bank + 0x26))
	if (m.u8(SYSTEM_DATA + 0x2C + 0x10 + 3) & 1) == 0: # config0.flags.b[3]: byte 3 of the word at config0 +0x10
		_copy(SYSTEM_DATA + 0x2C, bank + 0x28, 0x14)
	if (m.u8(SYSTEM_DATA + 0x40) & 1) == 0: # system_data.bits.flag
		_copy(SYSTEM_DATA + 0x40, bank + 0x3C, 0x18)
	m.write_u8(SYSTEM_DATA + 0x27, m.u8(bank + 0x54))
	m.write_u8(SYSTEM_DATA + 0x28, m.u8(bank + 0x55))
	engine.call_c("memcpy", [SYSTEM_DATA + 0x58, bank + 0x56, 0x800])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x858, bank + 0x856, 16])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x868, bank + 0x866, 16])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x878, bank + 0x876, 16])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x888, bank + 0x886, 0x80])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x908, bank + 0x906, 0x80])
	engine.call_c("memcpy", [SYSTEM_DATA + 0x988, bank + 0x986, 0x100])
	engine.call_c("memcpy", [SYSTEM_DATA + 0xEC8, bank + 0xA86, 16])
	engine.call_c("memcpy", [SYSTEM_DATA + 0xA88, bank + 0xA98, 0x240])
	engine.call_c("memcpy", [SYSTEM_DATA + 0xCC8, bank + 0xCD8, 0x100])
	var layout: int = SYSTEM_DATA + 0xDC8
	engine.call_c("memcpy", [layout, bank + 0xDD8, 0x100])
	engine.call_c("memcpy", [m.global_address("D_80125C10"), layout, 0x40])
	engine.call_c("memcpy", [m.global_address("mascot_anchor_x"), layout + 0xF0, 4])
	m.write_global("hud_mascot_choice", m.u8(layout + 0xF4))
	m.write_global("mascot_length", m.u8(layout + 0xF5))
	m.write_global("mascot_swing", m.u8(layout + 0xF6))
	return 0

## GameSaveImage of bank arg0 from GameState, field by field in the C order. The one store that matters for
## order is saved_settings[3]/[2] (inside the scene_flags source range 0x56C..0x66C) before the scene_flags copy.
func memory_card_game_bank_build(arg0: int) -> int:
	var m = engine.memory
	var img: int = BLOCK + arg0 * BANK_SIZE + 0x200
	m.write_u8(img + 0x4, engine.gs_get("vacation"))
	m.write_u8(img + 0x5, engine.gs_get("year"))
	m.write_u8(img + 0x6, engine.gs_get("month"))
	m.write_u8(img + 0x7, engine.gs_get("day"))
	m.write_u8(img + 0x8, engine.gs_get("weekday"))
	m.write_u16(img + 0xA, engine.gs_get("day_count"))
	_copy(img + 0xC, _gs("month_calendar"), 0x80)
	_copy(img + 0x8C, _gs("player_full_name"), 0x7C)
	_copy(img + 0x108, _gs("girls"), 16 * 0x38)
	_copy(img + 0x488, _gs("girl_cleared"), 16)
	_copy(img + 0x498, _gs("girls") - 0x1BC + 0x54C, 8 * 4) # appointments (unk_54C)
	_copy(img + 0x4B8, _gs("clubs"), 12 * 4)
	var speed: int = engine.call_c("get_k_speed")
	m.write_u8(_gs("saved_settings") + 3, speed)
	m.write_u8(_gs("saved_settings") + 2, m.read_global("cursor_style"))
	_copy(img + 0x4E8, _gs("girls") - 0x1BC + 0x56C, 0x100) # scene_flags
	_copy(img + 0x5E8, _gs("date_place_history"), 4)
	_copy(img + 0x5EC, _gs("girl_last_date_spot"), 16)
	_copy(img + 0x5FC, _gs("girl_last_date_spot_timer"), 16)
	_copy(img + 0x60C, _gs("girl_scene_bits"), 16)
	_copy(img + 0x61C, _gs("event_log"), 256 * 8)
	m.write_u8(img + 0xE1C, engine.gs_get("icon_mode"))
	m.write_u8(img + 0xE1D, engine.gs_get("icon_count"))
	m.write_u8(img + 0xE1E, engine.gs_get("menu_mode"))
	m.write_u8(img + 0xE1F, engine.gs_get("sd_anim_fast"))
	m.write_u8(img + 0xE20, engine.gs_get("voice_mode"))
	_copy(img + 0xE24, _gs("last_command"), 4)
	m.write_u8(img + 0xE28, engine.gs_get("cursor_speed"))
	m.write_u8(img + 0xE29, engine.gs_get("click_mode"))
	m.write_u8(img + 0xE2A, engine.gs_get("double_click_interval"))
	m.write_u8(img + 0xE2B, engine.gs_get("confirm_button"))
	_copy(img + 0xE2C, _gs("quarter_flags"), 4)
	_bank_checksum_store(arg0)
	return 0

## Two banks of the image; a bank is valid when the byte sum of its bytes 4..0xEFF equals its checksum word and
## it has a nonzero byte. The slot of the first valid bank goes to album_slot.
func memory_card_bank_validate(_arg0: int) -> int:
	var m = engine.memory
	for b: int in 2:
		var base: int = BANKS + b * BANK_SIZE
		var n: int = 0
		var sum: int = 0
		for j: int in range(4, 0xF00):
			var v: int = m.u8(base + j)
			if v != 0:
				n += 1
			sum += v
		if Math.s32(sum) == m.s32(BLOCK + b * BANK_SIZE + 0x200) and n != 0:
			engine.gs_set("album_slot", b)
			return 1
	return -1

## The load half of memory_card_game_bank_build (T-25310), in the same field order.
func memory_card_game_bank_load(arg0: int) -> int:
	var m = engine.memory
	if engine.call_c("memory_card_bank_validate", [0]) == -1:
		return -1
	var slot: int = engine.gs_get("album_slot") & 1
	var img: int = BLOCK + slot * BANK_SIZE + 0x200
	if m.u8(img + 5) == 0 or m.u8(img + 6) == 0: # year, month
		return -1
	var gs_base: int = _gs("girls") - 0x1BC # GameState base
	engine.gs_set("vacation", m.u8(img + 4))
	engine.gs_set("year", m.u8(img + 5))
	engine.gs_set("month", m.u8(img + 6))
	engine.gs_set("day", m.u8(img + 7))
	engine.gs_set("weekday", m.u8(img + 8))
	engine.gs_set("day_count", m.u16(img + 0xA))
	_copy(_gs("month_calendar"), img + 0xC, 0x80)
	_copy(_gs("player_full_name"), img + 0x8C, 0x7C)
	_copy(_gs("girls"), img + 0x108, 16 * 0x38)
	_copy(_gs("girl_cleared"), img + 0x488, 16)
	_copy(gs_base + 0x54C, img + 0x498, 8 * 4)
	_copy(_gs("clubs"), img + 0x4B8, 12 * 4)
	_copy(gs_base + 0x56C, img + 0x4E8, 0x100)
	engine.call_c("k_speed_set", [m.u8(_gs("saved_settings") + 3)])
	m.write_global("cursor_style", m.u8(_gs("saved_settings") + 2))
	_copy(_gs("date_place_history"), img + 0x5E8, 4)
	_copy(_gs("girl_last_date_spot"), img + 0x5EC, 16)
	_copy(_gs("girl_last_date_spot_timer"), img + 0x5FC, 16)
	_copy(_gs("girl_scene_bits"), img + 0x60C, 16)
	engine.call_c("bzero", [_gs("event_log"), 0x800])
	_copy(_gs("event_log"), img + 0x61C, 256 * 8)
	engine.gs_set("event_log_count", engine.call_c("hud_event_log_scan",
		[_gs("current_girl"), _gs("date_girl"), 1]))
	engine.gs_set("icon_mode", m.u8(img + 0xE1C))
	engine.gs_set("icon_count", m.u8(img + 0xE1D))
	engine.gs_set("menu_mode", m.u8(img + 0xE1E))
	engine.gs_set("sd_anim_fast", m.u8(img + 0xE1F))
	engine.gs_set("voice_mode", m.u8(img + 0xE20))
	_copy(_gs("last_command"), img + 0xE24, 4)
	engine.gs_set("cursor_speed", m.u8(img + 0xE28))
	engine.gs_set("click_mode", m.u8(img + 0xE29))
	engine.gs_set("double_click_interval", m.u8(img + 0xE2A))
	engine.gs_set("confirm_button", m.u8(img + 0xE2B))
	_copy(_gs("quarter_flags"), img + 0xE2C, 4)
	return 1

## buf[0..0x13] = 0, then the prefix global and one character cell (STR_DIGIT_BASE + 4 * index) for 0..15.
func memory_card_file_name_build(buf: int, arg1: int) -> int:
	for i: int in 0x14:
		engine.memory.write_u8(buf + i, 0)
	engine.call_c("strcpy", [buf, engine.memory.global_address("memory_card_file_prefix")])
	if arg1 < 0 or arg1 > 15:
		return 0
	engine.call_c("strcat", [buf, STR_DIGIT_BASE + arg1 * 4])
	return 1

func memory_card_state_reset() -> void:
	engine.call_c("memory_card_stop")
	engine.call_c("memory_card_block_clear")
	engine.memory.write_u8(0x800E7D14, 0)
	engine.memory.write_u8(0x800E7D15, 0)
	engine.memory.write_u8(0x800E7D16, 0)
	for i: int in 3:
		engine.memory.write_u8(0x800E7D13 + i * 4 + 5, 0)
		engine.memory.write_u8(0x800E7D13 + i * 4 + 6, 0)
		engine.memory.write_u8(0x800E7D13 + i * 4 + 7, 0)
		engine.memory.write_u8(0x800E7D13 + i * 4 + 4, 0)
	engine.call_c("memory_card_start")
	engine.memory.write_global("slot_info_from_system_data", 0)
