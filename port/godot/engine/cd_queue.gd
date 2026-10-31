extends RefCounted
## Port of src/main/cd_queue.c (original object 0x800451D0-0x80046500, T-39301): the CD command ring and
## its pump, queued sector reads, the read/audio status bytes and the XA clip request. The libcd calls of
## the C (CdSync, CdControl, CdRead, ...) go to the engine's CD drive model (engine/_cd_drive.gd), whose
## reads come from the installed disc pack (DiscPack.read_into_memory) at floor(14 + 0.4 x sectors) frames.
## Contract: wiki/engine-porting-contract.md. Tests: config/port/engine-diff/cd_queue.json (captures) and
## tests/engine/cd_queue_test.gd (status sequence of the pump on an invented disc pack).
##
## The ring and every status byte live in game memory (core/game_layout.gd GLOBALS: cd_queue_head,
## cd_queue_tail, cd_read_status, cd_queue_phase, cd_command_ring, cd_state, cd_read_count, ...), so a caller
## that reads them directly (the C does, through cd_read_sync_status and friends) sees what the C shows.
##
## Frames: engine.call_c reserves each function's original frame (engine/_frame_sizes.gd, from the
## `addiu sp, sp, -N` of the asm); functions whose C frame holds a buffer that is passed by address put it at
## engine.frame_base() + its original offset, so the buffer sits where the original's did and the bytes it
## never initialises are the same stale stack bytes the original copies into the ring (cd_command_push
## copies 16 bytes of a 4-byte parameter).

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/cd_queue.c"
const C_FUNCTIONS: Array[String] = [
	"cd_load_destination_base_get", "cd_load_destination_base_set", "cd_read_sync_poll", "cd_sync_poll",
	"cd_queue_idle", "cd_command_reset", "cd_command_push", "cd_audio_command", "cd_command_pump",
	"cd_last_command", "cd_read_sync_status", "cd_done_count", "cd_audio_state", "cd_position_lba",
	"cd_audio_ticks", "cd_audio_request_get", "cd_audio_request_set", "cd_read_param_pack",
	"cd_read_sectors_nowait", "cd_read_sectors", "cd_file_checksum_ok", "cd_busy_marker_draw",
	"cd_error_recover",
]
## "ret_defined": values the C returns on its defined paths; a captured v0 outside them is the register
## left over when the function falls off its end, which the harness does not compare. "ret_undefined": the
## C function has a non-void type but no return value on any path (a reason is required).
const C_SIGNATURES: Dictionary = {
	"cd_load_destination_base_get": {"args": [], "ret": "s32"},
	"cd_load_destination_base_set": {"args": ["s32"], "ret": "void"},
	"cd_read_sync_poll": {"args": [], "ret": "void"},
	"cd_sync_poll": {"args": [], "ret": "void"},
	"cd_queue_idle": {"args": [], "ret": "s32"},
	"cd_command_reset": {"args": [], "ret": "void"},
	"cd_command_push": {"args": ["u8", "s32", "s32"], "ret": "s32", "ret_defined": [1, 0xFFFFFFFF]},
	"cd_audio_command": {"args": ["u8", "s32", "ptr"], "ret": "void"},
	"cd_command_pump": {"args": [], "ret": "void"},
	"cd_last_command": {"args": [], "ret": "u8"},
	"cd_read_sync_status": {"args": [], "ret": "u8"},
	"cd_done_count": {"args": [], "ret": "u8"},
	"cd_audio_state": {"args": [], "ret": "u8"},
	"cd_position_lba": {"args": [], "ret": "s32"},
	"cd_audio_ticks": {"args": [], "ret": "s32"},
	"cd_audio_request_get": {"args": [], "ret": "ptr"},
	"cd_audio_request_set": {"args": ["s32", "s32", "s32"], "ret": "void"},
	"cd_read_param_pack": {"args": ["u8", "s32", "ptr"], "ret": "void"},
	"cd_read_sectors_nowait": {"args": ["u8", "s32", "s32"], "ret": "void"},
	"cd_read_sectors": {"args": ["u8", "s32", "s32"], "ret": "void"},
	"cd_file_checksum_ok": {"args": ["u8", "ptr"], "ret": "s32"},
	"cd_busy_marker_draw": {"args": [], "ret": "s32", "ret_undefined": "s32 in the C with no return statement (T-9710): v0 is whatever the register held"},
	"cd_error_recover": {"args": [], "ret": "void"},
}

## Data of this object that the layout does not list (declared as local externs in cd_queue.c):
## cd_read_retry_count (u16, config/symbol_addrs.txt) and cd_read_end_word_pointer (u32 pointer,
## config/symbol_addrs_main.txt).
const CD_READ_RETRY_COUNT: int = 0x800B3D64
const CD_READ_END_WORD_POINTER: int = 0x801255BC

const RING_ENTRIES: int = 0x30
const RING_ENTRY_SIZE: int = 0x18
const RING_COMMAND_OFFSET: int = 0x0
const RING_LBA_OFFSET: int = 0x4
const RING_PARAM_OFFSET: int = 0x8
const RING_PARAM_SIZE: int = 0x10
## cd_command_ring param[15]: 0xFF marks a ReadS whose caller passed 1 instead of a parameter pointer.
const RING_PARAM_FLAG: int = 0xFF
const MAX_LBA: int = 0x4AB50
## The XA end-of-file marker word of a checked read: the high half of the last word of the destination.
const READ_END_MARKER: int = 0x10120000

## Offset of the `u8 buf[0x20]` of cd_read_sectors and cd_audio_command in their 0x48-byte frames.
const BUF_OFFSET: int = 0x28

# CD ring commands (the numbers cd_audio_command pushes; most are libcd command codes)
## ring command 0 pauses like 9 but leaves cd_read_status alone
const CMD_PAUSE_PLAIN: int = 0
const CMD_SETLOC: int = 2
const CMD_PLAY: int = 3
const CMD_READ: int = 6
const CMD_PAUSE: int = 9
const CMD_INIT: int = 10
const CMD_SETFILTER: int = 13
const CMD_SETMODE: int = 14
const CMD_SEEK: int = 21
const CMD_READS: int = 27

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

# ---- helpers (not C functions)

func _rd(global_name: String, index: int = 0) -> int:
	return engine.memory.read_global(global_name, index)

func _wr(global_name: String, value: int, index: int = 0) -> void:
	engine.memory.write_global(global_name, value, index)

func _ring_address(index: int) -> int:
	return engine.memory.global_address("cd_command_ring") + index * RING_ENTRY_SIZE

func _ring_command(index: int) -> int:
	return engine.memory.u8(_ring_address(index) + RING_COMMAND_OFFSET)

func _ring_lba(index: int) -> int:
	return engine.memory.s32(_ring_address(index) + RING_LBA_OFFSET)

func _ring_param_address(index: int) -> int:
	return _ring_address(index) + RING_PARAM_OFFSET

func _result_address() -> int:
	return engine.memory.global_address("cd_result")

## cd_queue_head += 1 (u8).
func _advance_head() -> void:
	_wr("cd_queue_head", _rd("cd_queue_head") + 1)

# ---- simple accessors

func cd_load_destination_base_get() -> int:
	return _rd("cd_load_destination_base")

func cd_load_destination_base_set(arg0: int) -> void:
	_wr("cd_load_destination_base", arg0)

## Return the CD read status byte (bit 0 = read finished).
func cd_read_sync_status() -> int:
	return _rd("cd_read_status")

func cd_done_count() -> int:
	return _rd("cd_read_count")

func cd_audio_state() -> int:
	return _rd("cd_state")

func cd_audio_ticks() -> int:
	return _rd("cd_audio_position")

func cd_audio_request_get() -> int:
	return engine.memory.global_address("cd_audio_request")

func cd_audio_request_set(clip_start_index: int, clip_end_index: int, speaker_id: int) -> void:
	var idx: int = Math.s32(speaker_id) & 0xF
	_wr("cd_audio_request", idx, 0)
	_wr("cd_audio_request", Math.s32(clip_start_index), 2)
	_wr("cd_audio_request", Math.s32(clip_end_index), 3)
	_wr("cd_audio_request", _rd("cd_audio_speaker_offset_table", idx), 1)

func cd_queue_idle() -> int:
	if _rd("cd_queue_tail") == _rd("cd_queue_head") and _rd("cd_queue_phase") == -1:
		return 1
	return 0

func cd_last_command() -> int:
	return _ring_command((_rd("cd_queue_head") + 0x2F) % RING_ENTRIES)

func cd_command_reset() -> void:
	_wr("cd_queue_head", 0)
	_wr("cd_queue_tail", 0)
	_wr("cd_read_status", 2)
	_wr("cd_queue_phase", -1)
	_wr("cd_audio_position", 0)
	_wr("cd_state", 0)
	_wr("cd_read_count", 0)
	_wr("cd_read_checked", 0)
	_wr("cd_last_read_lba", 0)
	# The C resets the ring state only; the drive model is the hardware (reads still queued in the ring
	# will never be issued, so their request times go).
	engine.cd_drive.forget_read_requests()

# ---- polls

func cd_read_sync_poll() -> void:
	_wr("cd_read_status", 0x80)
	var left: int = engine.cd_drive.cd_read_sync(1, engine.memory.global_address("cd_read_sync_result"))
	if not engine.halted():
		_wr("cd_sectors_left", left)

func cd_sync_poll() -> void:
	match engine.cd_drive.cd_sync(1, engine.memory.global_address("cd_sync_result")):
		2:
			_wr("cd_read_status", _rd("cd_read_status") | 2)
		5:
			_wr("cd_queue_phase", -1)
	# (a refusal inside cd_sync returns 0, which matches neither case)

# ---- the ring

## Appends a command; -1 when the ring is full, 1 when stored. `arg2`: 0 (no parameter), 1 (marks
## param[15] with 0xFF) or the PS1 address of 16 parameter bytes to copy.
## The C falls off the end of the function when cd_queue_tail wraps to 0 (no return value); the port
## returns 0 there and the harness does not compare v0 for it (ret_defined).
func cd_command_push(arg0: int, arg1: int, arg2: int) -> int:
	var command: int = Math.u8(arg0)
	var src: int = Math.s32(arg2)
	var tail: int = _rd("cd_queue_tail")
	if (tail + 1) % RING_ENTRIES == _rd("cd_queue_head") % RING_ENTRIES:
		return -1
	var entry: int = _ring_address(tail)
	engine.memory.write_u8(entry + RING_COMMAND_OFFSET, command)
	engine.memory.write_u32(entry + RING_LBA_OFFSET, arg1)
	engine.memory.write_u8(entry + RING_PARAM_OFFSET + 15, 0)
	if src == 1:
		engine.memory.write_u8(entry + RING_PARAM_OFFSET + 15, RING_PARAM_FLAG)
	elif src != 0:
		engine.memory.write_bytes(entry + RING_PARAM_OFFSET, engine.memory.read_bytes(Math.u32(src), RING_PARAM_SIZE))
	_wr("cd_queue_tail", _rd("cd_queue_tail") + 1) # cd_queue_tail = cd_queue_tail + 1 (a u8 store: 0xFF + 1 is 0)
	if command == CMD_READ:
		engine.cd_drive.note_read_request(engine.frame(), engine.gs_get("game_mode"), engine.gs_get("mode_stage"))
	if _rd("cd_queue_tail") < RING_ENTRIES: # compares the stored byte
		return 1
	_wr("cd_queue_tail", 0)
	return 0

func cd_audio_command(cd_operation_id: int, sector_lba: int, command_params: int) -> void:
	var buf: int = engine.frame_base() + BUF_OFFSET
	var lba: int = Math.s32(sector_lba)
	if Math.u32(lba) >= 0x4AB51:
		lba = MAX_LBA
	match Math.u8(cd_operation_id):
		0:
			engine.call_c("cd_command_push", [0, 0, 0])
		9:
			engine.call_c("cd_command_push", [9, 0, 0])
		6:
			_wr("cd_last_read_lba", lba)
			engine.call_c("cd_command_push", [6, lba, command_params])
		14:
			engine.call_c("cd_command_push", [0xE, 0, command_params])
		13:
			if lba == 0:
				engine.call_c("cd_command_push", [0xD, 0, command_params])
			else:
				engine.call_c("cd_command_push", [0xD, 0, command_params])
				engine.call_c("cd_command_push", [2, lba, 0])
				engine.call_c("cd_command_push", [0x1B, lba, 0])
				_wr("cd_last_read_lba", lba)
		2:
			engine.call_c("cd_command_push", [2, lba, 0])
		21:
			engine.call_c("cd_command_push", [2, lba, 0])
			engine.call_c("cd_command_push", [0x15, lba, 0])
		10:
			engine.call_c("cd_command_push", [0xA, lba, 0])
		3:
			engine.memory.write_u8(buf + 0x10, 5) # buf[0x10] = 5: the other 15 parameter bytes stay as they were
			engine.call_c("cd_command_push", [0xE, 0, buf + 0x10])
			engine.call_c("cd_command_push", [2, lba, 0])
			engine.call_c("cd_command_push", [3, 0, 0])
		27:
			pass

## word 0 of a read parameter: the destination address with the sector count in the top byte.
func cd_read_param_pack(sector_count: int, destination_address: int, read_parameter_out: int) -> void:
	engine.memory.write_u32(read_parameter_out, destination_address)
	engine.memory.write_u8(read_parameter_out + 3, sector_count)

func cd_read_sectors_nowait(arg0: int, arg1: int, arg2: int) -> void:
	_read_request(arg0, arg1, arg2, 0)

## Read n sectors to a buffer from an LBA through the CD command ring and wait.
func cd_read_sectors(arg0: int, arg1: int, arg2: int) -> void:
	_read_request(arg0, arg1, arg2, 1)

## The shared body of the two read entry points (identical C except the final store of cd_read_checked).
func _read_request(count: int, destination: int, lba: int, checked: int) -> void:
	var buf: int = engine.frame_base() + BUF_OFFSET
	engine.call_c("audio_command", [6])
	engine.call_c("cd_read_param_pack", [Math.u8(count), destination, buf])
	engine.call_c("cd_audio_command", [6, lba, buf])
	_wr("cd_read_checked", checked)

## True when the 16-bit sum of all bytes of a loaded file except the last 4 equals the big-endian u16
## stored at its end.
func cd_file_checksum_ok(blocks: int, buf: int) -> int:
	var count: int = Math.u8(blocks)
	var limit: int = (count << 11) - 4
	if limit <= 0:
		# blocks == 0: the C loop bound wraps to 0xFFFFFFFC and runs off the buffer. Never answer for that.
		engine.refuse("cd_file_checksum_ok", "blocks == 0 (the original runs past the buffer)")
		return 0
	var bytes: PackedByteArray = engine.memory.read_bytes(buf, limit + 2)
	if engine.halted():
		return 0
	var sum: int = 0
	var i: int = 0
	while i < limit:
		sum = Math.u16(sum + bytes[i] + bytes[i + 1])
		i += 2
	return 0 if sum != (bytes[limit] << 8) + bytes[limit + 1] else 1

func cd_busy_marker_draw() -> void:
	if engine.memory.read_global("D_8011ED17") & 0x80 != 0 and engine.frame() & 4 != 0:
		engine.call_c("prim_gradient_line", [0x9E, 0x5A, 0x9E, 0x62, 0xFFFF00, 0xFFFF00, 0x80])
		engine.call_c("prim_gradient_line", [0x9D, 0x5A, 0x9D, 0x62, 0xFFFF00, 0xFFFF00, 0x80])

## Resets the CD mode after a failed XA read. The 512 VSync(0) waits of the C are blocking and not ported
## (func_800AD950 refuses), so the function stops there with cd_mode_scratch already set.
func cd_error_recover() -> void:
	_wr("cd_mode_scratch", 0x84)
	if _spin_control_b(0xE, engine.memory.global_address("cd_mode_scratch"), 0) == 0:
		return
	for i: int in 0x200:
		if engine.halted():
			break
		engine.call_c("func_800AD950", [0])

## The position of the head in sectors (GetlocL of the drive, 0 when the BCD time is out of range).
func cd_position_lba() -> int:
	var frame: int = engine.frame_base()
	var status_buffer: int = frame + 0x34
	var local: int = frame + 0x44
	var state: int = _result_address()
	var saved: int = engine.memory.u32(state)
	var retries: int = 0
	engine.memory.write_u32(state, 0) # bytes[3] = bytes[2] = bytes[1] = bytes[0] = 0
	if engine.cd_drive.cd_sync(1, status_buffer) != 0:
		while engine.cd_drive.cd_control_b(0x10, 0, state) == 0:
			if engine.halted():
				break
			retries += 1
			if retries >= 16:
				if engine.call_c("cd_last_command") == 0x1B:
					_wr("cd_queue_phase", -1)
					var head: int = _rd("cd_queue_head")
					_wr("cd_queue_head", head - 1 if head != 0 else 0x2F)
				engine.memory.write_u32(state, saved)
				break
	var bytes: PackedByteArray = engine.memory.read_bytes(state, 3)
	if engine.halted():
		return 0
	engine.memory.write_u8(local, bytes[0])
	engine.memory.write_u8(local + 1, bytes[1])
	engine.memory.write_u8(local + 2, bytes[2])
	retries = engine.cd_drive.cd_pos_to_int(local)
	var result: int = 0
	if engine.call_c("cd_binary_coded_decimal_to_integer", [bytes[0]]) >= 0x4B:
		result = 0
	elif engine.call_c("cd_binary_coded_decimal_to_integer", [bytes[1]]) >= 0x3C:
		result = 0
	elif engine.call_c("cd_binary_coded_decimal_to_integer", [bytes[2]]) >= 0x4B:
		result = 0
	elif retries > 0:
		result = retries
	return result

# ---- the pump

## Called once per frame by the main loop (after the mode handler): advances the command at the head of the
## ring one step. The two phases are -1 (issue the command) and 1 (wait for it to finish); the
## status bytes callers poll (cd_state, cd_read_status, cd_read_count) change here.
func cd_command_pump() -> void:
	var frame: int = engine.frame_base()
	var drive = engine.cd_drive
	if _rd("cd_queue_tail") != _rd("cd_queue_head"):
		engine.call_c("cd_busy_marker_draw")
		if not engine.halted():
			var phase: int = _rd("cd_queue_phase")
			if phase == -1:
				_pump_issue(frame + 0x30, frame + 0x38)
			elif phase == 1:
				_pump_wait()
	elif drive.cd_sync(1, 0) == 5:
		if engine.call_c("cd_last_command") == 0x1B:
			_wr("cd_queue_phase", -1)
			var head: int = _rd("cd_queue_head")
			_wr("cd_queue_head", head - 1 if head != 0 else 0x2F)
			engine.call_c("cd_error_recover")
		else:
			drive.cd_control(1, 0, 0)
	elif _rd("cd_state") == 4:
		_wr("cd_audio_position", engine.call_c("cd_position_lba"))
	if _rd("cd_queue_head") >= RING_ENTRIES:
		_wr("cd_queue_head", 0)

## Phase -1: the command at the head is started. `sp30` and `sp38` are the C locals of those frame
## offsets (an 8-byte result buffer and a 4-byte MSF position).
func _pump_issue(sp30: int, sp38: int) -> void:
	var drive = engine.cd_drive
	var memory = engine.memory
	var head: int = _rd("cd_queue_head")
	var result: int = _result_address()
	match _ring_command(head):
		CMD_PAUSE_PLAIN:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_state", 0)
				if drive.cd_control(9, 0, result) == 1:
					_wr("cd_queue_phase", 1)
		CMD_PLAY:
			engine.call_c("func_8008B750", [0x7F, 0x7F]) # the SPU CD volume: not ported, refuses
			if not engine.halted():
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				if drive.cd_control(3, 0, 0) == 1:
					_wr("cd_queue_phase", -1)
					_advance_head()
		CMD_SETFILTER, CMD_SETMODE:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				var libcd_command: int = 0xD if _ring_command(head) == CMD_SETFILTER else 0xE
				if drive.cd_control(libcd_command, _ring_param_address(head), result) == 1:
					_wr("cd_queue_phase", -1)
					_advance_head()
		CMD_PAUSE:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				if drive.cd_control(9, 0, result) == 1:
					_wr("cd_queue_phase", 1)
		CMD_SETLOC:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				drive.cd_int_to_pos(_ring_lba(head), sp38)
				if drive.cd_control(2, sp38, result) == 1:
					_wr("cd_queue_phase", -1)
					_advance_head()
		CMD_SEEK:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				drive.cd_int_to_pos(_ring_lba(head), sp38)
				if drive.cd_control_f(0x15, sp38) == 1:
					_wr("cd_queue_phase", 1)
		CMD_READS:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				drive.cd_callbacks_clear()
				drive.cd_int_to_pos(_ring_lba(head), sp38)
				if _spin_control_b(2, sp38, 0) != 0 and _spin_started(drive.cd_read2(0xC8), "CdRead2"):
					_wr("cd_state", 0)
					_wr("cd_queue_phase", 1)
		CMD_INIT:
			_wr("cd_read_status", 0)
			_wr("cd_state", 0)
			if _spin_control(0xA, 0, 0) != 0:
				_wr("cd_queue_phase", -1)
				_advance_head()
		CMD_READ:
			if drive.cd_sync(1, sp30) != 0:
				_wr("cd_read_status", 0)
				_wr("cd_state", 0)
				var word: int = memory.u32(_ring_param_address(head))
				var destination: int = (word & 0xFFFFFF) + 0x80000000 # sp44
				var sectors: int = word >> 24 # sp48
				memory.write_u32(CD_READ_END_WORD_POINTER, (word & 0xFFFFFF) + (sectors << 11) + 0x7FFFFFFC)
				drive.cd_callbacks_clear()
				_wr("cd_mode_scratch", 0x84)
				if _spin_control_b(0xE, memory.global_address("cd_mode_scratch"), 0) == 0:
					return
				drive.cd_int_to_pos(_ring_lba(head), sp38)
				if _spin_control_b(2, sp38, 0) == 0:
					return
				if _spin_started(drive.cd_read(sectors, destination, 0x80), "CdRead"):
					_wr("cd_queue_phase", 1)

## Phase 1: the command has been issued; this step polls it. Reads finish when the bytes have landed
## (CdReadSync = 0): the read status, cd_state and cd_read_count then show it.
func _pump_wait() -> void:
	var drive = engine.cd_drive
	var memory = engine.memory
	var head: int = _rd("cd_queue_head")
	var result: int = _result_address()
	match _ring_command(head):
		CMD_PAUSE_PLAIN, CMD_PAUSE:
			if drive.cd_sync(1, result) != 0 and drive.cd_control(1, 0, result) == 1 \
					and memory.u8(result) & 0x40 == 0:
				_wr("cd_state", 2)
				_advance_head()
				_wr("cd_queue_phase", -1)
		CMD_SEEK:
			if drive.cd_control(1, 0, result) == 1 and memory.u8(result) & 0x40 == 0:
				_wr("cd_state", 2)
				_advance_head()
				_wr("cd_queue_phase", -1)
		CMD_READ:
			if drive.cd_sync(1, result) != 0:
				var left: int = drive.cd_read_sync(1, memory.global_address("cd_read_sync_result"))
				if engine.halted():
					return
				_wr("cd_sectors_left", left)
				if left == 0:
					_finish_read(memory)
				elif left < 0:
					_wr("cd_queue_phase", -1)
					_wr("cd_state", 0)
					_wr("cd_read_status", 0)
		CMD_READS:
			if drive.cd_sync(1, result) == 5:
				_wr("cd_queue_phase", -1)
				engine.call_c("cd_error_recover")
			else:
				if _spin_control(1, 0, result) == 0:
					return
				if memory.u8(result) & 0x20 != 0:
					_wr("cd_read_status", 0)
					_wr("cd_queue_phase", -1)
					if memory.u8(_ring_param_address(head) + 15) != RING_PARAM_FLAG:
						_wr("cd_state", 4)
					_advance_head()

## A landed read: with cd_read_checked set, the last word of the destination must carry the end marker
## 0x1012xxxx, else the read is retried (the head stays); every retry bumps cd_read_retry_count.
func _finish_read(memory) -> void:
	if _rd("cd_read_checked") != 0 and memory.u32(memory.u32(CD_READ_END_WORD_POINTER)) & 0xFFFF0000 != READ_END_MARKER:
		_wr("cd_read_status", 0)
		var retries: int = Math.u16(memory.u16(CD_READ_RETRY_COUNT) + 1)
		memory.write_u16(CD_READ_RETRY_COUNT, retries)
		if retries & 3 == 3:
			# Every fourth failed attempt the original re-initialises the drive and waits 512 VSyncs.
			if _spin_control(0xA, 0, 0) == 0:
				return
			for i: int in 0x200:
				if engine.halted():
					break
				engine.call_c("func_800AD950", [0]) # blocking wait: not ported, refuses
		if not engine.halted():
			_wr("cd_queue_phase", -1)
	else:
		_wr("cd_queue_phase", -1)
		_advance_head()
		_wr("cd_state", 2)
		_wr("cd_read_status", _rd("cd_read_status") | 1)
		_wr("cd_read_count", _rd("cd_read_count") + 1) # ((u8 *)&cd_state)[1] += 1

# ---- the C's `if (f() == 0) do {} while (f() == 0)` spin loops

## The spin loops wait for libcd to accept a command or finish a step. The drive model accepts at the next
## call, so one retry suffices; a model that never accepts would hang the original too, so it refuses.
const SPIN_LIMIT: int = 16

func _spin_control(command: int, param: int, result: int) -> int:
	for attempt: int in SPIN_LIMIT:
		if engine.halted():
			return 0
		if engine.cd_drive.cd_control(command, param, result) != 0:
			return 1
	engine.refuse("CdControl", "CdControl(0x%02X) was not accepted in %d tries (the original spins here)" % [command, SPIN_LIMIT])
	return 0

func _spin_control_b(command: int, param: int, result: int) -> int:
	for attempt: int in SPIN_LIMIT:
		if engine.halted():
			return 0
		if engine.cd_drive.cd_control_b(command, param, result) != 0:
			return 1
	engine.refuse("CdControlB", "CdControlB(0x%02X) did not complete in %d tries (the original spins here)" % [command, SPIN_LIMIT])
	return 0

func _spin_started(value: int, what: String) -> bool:
	if value == 0 and not engine.halted():
		engine.refuse(what, "%s was not accepted (the original spins here)" % what)
	return value != 0 and not engine.halted()
