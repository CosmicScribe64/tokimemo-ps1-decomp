extends RefCounted
## T-39301: the CD drive behind libcd, as src/main/cd_queue.c drives it (not a C file of the game: the
## underscore keeps engine.gd from registering it as a module).
##
## cd_queue.c calls PsyQ libcd (SDK code, gcc objects): CdSync, CdControl, CdControlB, CdRead, CdReadSync,
## CdIntToPos, CdPosToInt, CdRead2. This class replaces them for the port; the function names below are
## the SDK names the wiki infers for the unnamed `func_800xxxxx` symbols (wiki/spec/h-cdio.md: 80087954 =
## CdSync, 800879D0 = CdControl, 80087C64 = CdControlB, 80087E4C = CdReadSync, 80087E6C = CdRead, 80087EE8 =
## CdIntToPos, 80087FF0 = CdPosToInt, 800880C0 = CdRead2; inferred from signatures, not from an SDK symbol
## table). 80087B1C (CdControlF), 800879A8/80087E08 (CdSyncCallback/CdReadCallback cleared with 0) and
## 80088070 (a flush before reads) are identified from their call sites only.
##
## What is modelled (all as functions of game_state.frame_count, never wall-clock time):
##  * reads: CdRead after Setloc lands count sectors from the disc pack (DiscPack.read_into_memory) at
##    the model latency floor(14 + 0.4 x sectors) frames after the request (the cd_command_push of the
##    read; reads are serialised), the model of services/prologue_media_queue.gd (2x speed, 150
##    sectors/s; measured against 22 of the 25 opening reads); CdReadSync reports the
##    sectors still to come, then 0 once the bytes have landed;
##  * the commands cd_queue.c issues (Nop/GetStat 0x01, Setloc 0x02, Pause 0x09, Init 0x0A, Setfilter 0x0D,
##    Setmode 0x0E, GetlocL 0x10, SeekL 0x15, ReadS 0x1B) with their status and GetlocL results; the
##    reading position advances 0.4 frame per sector while XA is read (the same 2x speed);
##  * a command completes, and its result bytes reach the caller's buffer, at the first drive call at or
##    after its completion frame (the ACK interrupt). COMMAND_LATENCY_FRAMES is 0: no CD command latency
##    other than the reads is modelled yet (T-39002 measured Pause 4 and XA start 6 frames; those belong
##    to the audio work package that owns the XA timeline).
## Anything else (CD-DA Play 0x03, blocking CdSync(0), ...) refuses with a precise message, because
## answering it would invent a hardware result.

@warning_ignore_start("integer_division")

const Memory = preload("res://core/game_memory.gd")
const MediaQueue = preload("res://services/prologue_media_queue.gd")

const COMMAND_LATENCY_FRAMES: int = 0
const SECTOR_SIZE: int = 2048

const CDL_NOP: int = 0x01
const CDL_SETLOC: int = 0x02
const CDL_PLAY: int = 0x03
const CDL_PAUSE: int = 0x09
const CDL_INIT: int = 0x0A
const CDL_SETFILTER: int = 0x0D
const CDL_SETMODE: int = 0x0E
const CDL_GETLOCL: int = 0x10
const CDL_SEEKL: int = 0x15
const CDL_READS: int = 0x1B

const STATUS_MOTOR_ON: int = 0x02
const STATUS_READING: int = 0x20
## CdSync results (libcd): 0 = a command is still executing, 2 = complete, 5 = disc error.
const SYNC_BUSY: int = 0
const SYNC_COMPLETE: int = 2
const SYNC_ERROR: int = 5
## Sector positions are 150-sector offset in MSF (CdIntToPos adds 150).
const PREGAP_SECTORS: int = 150

var engine # GameEngine (engine/engine.gd)

var mode: int = 0
var filter_file: int = 0
var filter_channel: int = 0
## Absolute sector of the last Setloc (the read/seek target) and of the head.
var loc_lba: int = -1
var head_lba: int = 0
## The motor is on from the start: the game runs after its boot code has initialised libcd (CdInit), and the
## captured drive states all report it spinning.
var motor_on: bool = true

## The command in flight: {"command", "result_address", "bytes": PackedByteArray, "complete_frame"} or {}.
var _pending: Dictionary = {}
var _last_result := PackedByteArray()
## Frames at which cd_command_push queued a read (command 6), oldest first: the model latency counts from
## the request (audio_command(6), cd_read_sectors), as the measurements of T-39002 do.
var _read_requests: Array[int] = []
## Completion frame of the read scheduled last (reads are serialised: the next starts when it ends).
var _last_deadline: int = 0
## A read in flight: {"count", "dest", "lba", "start_frame", "deadline_frame"} or {}.
var _read: Dictionary = {}
## XA streaming (ReadS/CdRead2): {"start_lba", "start_frame"} or {}.
var _xa: Dictionary = {}

func reset() -> void:
	mode = 0
	filter_file = 0
	filter_channel = 0
	loc_lba = -1
	head_lba = 0
	motor_on = true
	_pending = {}
	_last_result = PackedByteArray()
	_read = {}
	_xa = {}
	_read_requests = []
	_last_deadline = 0

## cd_command_push queued a read at this frame (the start of its latency).
func note_read_request(frame: int) -> void:
	_read_requests.append(frame)

## cd_command_reset emptied the ring: queued reads will never be issued.
func forget_read_requests() -> void:
	_read_requests = []
	_last_deadline = 0

# ---- libcd entry points

## CdSync(1, result): 0 while a command executes, 2 when the last one is complete. `result_address` (0 =
## none) receives the result bytes of the command that completed.
func cd_sync(sync_mode: int, result_address: int) -> int:
	if sync_mode != 1:
		engine.refuse("CdSync", "CdSync(%d) blocks until the command completes; the port never blocks (only mode 1 is modelled)" % sync_mode)
		return SYNC_BUSY
	_service()
	if not _pending.is_empty():
		return SYNC_BUSY
	if result_address != 0 and not _last_result.is_empty():
		engine.memory.write_bytes(result_address, _last_result)
	return SYNC_COMPLETE

## CdControl(command, param, result): issues the command; 1 when issued, 0 when the previous one is
## still executing. The result bytes are stored when the command completes.
func cd_control(command: int, param_address: int, result_address: int) -> int:
	_service()
	if not _pending.is_empty():
		return 0
	_issue(command, param_address, result_address)
	return 0 if engine.halted() else 1

## CdControlF(command, param): CdControl without a result buffer (80087B1C).
func cd_control_f(command: int, param_address: int) -> int:
	return cd_control(command, param_address, 0)

## CdControlB(command, param, result): the blocking form. The model completes the command in the call,
## so it returns 1 at once; `result_address` receives the result bytes.
func cd_control_b(command: int, param_address: int, result_address: int) -> int:
	_service()
	if not _pending.is_empty():
		_complete_pending(true)
	_issue(command, param_address, result_address)
	_complete_pending(true)
	return 0 if engine.halted() else 1

## CdRead(count, dest, mode): ReadN from the last Setloc into `dest`. 1 when started. The bytes land when
## the model latency has elapsed (CdReadSync then returns 0).
func cd_read(count: int, dest_address: int, read_mode: int) -> int:
	_service()
	if loc_lba < 0:
		engine.refuse("CdRead", "CdRead without a Setloc: the position is undefined")
		return 0
	var vsyncs: int = MediaQueue.model_read_vsyncs(count)
	if vsyncs < 0:
		engine.refuse("CdRead", "CdRead of %d sectors is outside the latency model (1..255)" % count)
		return 0
	var now: int = engine.frame()
	# latency counts from the request frame (a retry has none: from now), after the read before it
	var requested: int = _read_requests.pop_front() if not _read_requests.is_empty() else now
	var start: int = maxi(requested, _last_deadline)
	_last_deadline = start + vsyncs
	_read = {"count": count, "dest": dest_address, "lba": loc_lba, "start_frame": start, "deadline_frame": start + vsyncs}
	motor_on = true
	_acknowledge()
	return 1

## CdReadSync(1, result): sectors still to come (> 0), 0 when the read has landed. The disc pack read
## happens here, at the deadline; a pack failure refuses (the original would retry forever).
func cd_read_sync(sync_mode: int, result_address: int) -> int:
	if sync_mode != 1:
		engine.refuse("CdReadSync", "CdReadSync(%d) blocks until the read completes; only mode 1 is modelled" % sync_mode)
		return 0
	_service()
	if _read.is_empty():
		return 0
	var now: int = engine.frame()
	if now < int(_read.deadline_frame):
		var total: int = maxi(1, int(_read.deadline_frame) - int(_read.start_frame))
		var landed: int = int(_read.count) * (now - int(_read.start_frame)) / total
		return maxi(1, int(_read.count) - landed)
	var job: Dictionary = _read
	_read = {}
	var landed_result: Dictionary = engine.disc_pack.read_into_memory(engine.memory, job.dest, job.lba, job.count)
	if not landed_result.ok:
		engine.refuse("CdRead", "disc read of %d sector(s) at LBA 0x%X: %s" % [job.count, job.lba, landed_result.error])
		return -1
	head_lba = int(job.lba) + int(job.count)
	if result_address != 0:
		engine.memory.write_u8(result_address, _status())
	return 0

## CdRead2(mode): starts XA streaming from the last Setloc (ReadS). 1 when started.
func cd_read2(read_mode: int) -> int:
	_service()
	if loc_lba < 0:
		engine.refuse("CdRead2", "CdRead2 without a Setloc: the position is undefined")
		return 0
	_xa = {"start_lba": loc_lba, "start_frame": engine.frame()}
	motor_on = true
	_acknowledge()
	return 1

## CdIntToPos(lba, pos): three BCD bytes (minute, second, frame) of lba + 150 at `pos_address`.
func cd_int_to_pos(lba: int, pos_address: int) -> int:
	var absolute: int = lba + PREGAP_SECTORS
	var minute: int = absolute / 4500
	var second: int = (absolute / 75) % 60
	var frame_in_second: int = absolute % 75
	engine.memory.write_u8(pos_address, _bcd(minute))
	engine.memory.write_u8(pos_address + 1, _bcd(second))
	engine.memory.write_u8(pos_address + 2, _bcd(frame_in_second))
	return pos_address

## CdPosToInt(pos): sector number of three BCD bytes (minus the 150-sector offset).
func cd_pos_to_int(pos_address: int) -> int:
	var memory: Memory = engine.memory
	var minute: int = _unbcd(memory.u8(pos_address))
	var second: int = _unbcd(memory.u8(pos_address + 1))
	var frame_in_second: int = _unbcd(memory.u8(pos_address + 2))
	return (minute * 60 + second) * 75 + frame_in_second - PREGAP_SECTORS

## The callbacks cd_queue.c clears with 0 (CdSyncCallback, CdReadCallback) and the flush before reads:
## the model has none of them.
func cd_callbacks_clear() -> void:
	pass

# ---- internals

func _status() -> int:
	var status: int = STATUS_MOTOR_ON if motor_on else 0
	if not _read.is_empty() or not _xa.is_empty():
		status |= STATUS_READING
	return status

## The head position in sectors: where XA streaming has reached (0.4 frame per sector, 2.5 sectors per
## frame), else where the last read or seek left the head.
func position_lba() -> int:
	if _xa.is_empty():
		return head_lba
	return int(_xa.start_lba) + (engine.frame() - int(_xa.start_frame)) * 5 / 2

static func _bcd(value: int) -> int:
	return ((value / 10) << 4) | (value % 10)

static func _unbcd(value: int) -> int:
	return (value >> 4) * 10 + (value & 15)

func _issue(command: int, param_address: int, result_address: int) -> void:
	var bytes: PackedByteArray
	var memory: Memory = engine.memory
	match command:
		CDL_NOP:
			bytes = PackedByteArray([_status()])
		CDL_SETLOC:
			loc_lba = cd_pos_to_int(param_address)
			bytes = PackedByteArray([_status()])
		CDL_SEEKL:
			head_lba = maxi(0, loc_lba)
			bytes = PackedByteArray([_status()])
		CDL_PAUSE:
			head_lba = position_lba()
			_xa = {}
			_read = {}
			bytes = PackedByteArray([_status()])
		CDL_INIT:
			mode = 0
			_xa = {}
			_read = {}
			motor_on = true
			bytes = PackedByteArray([_status()])
		CDL_SETFILTER:
			filter_file = memory.u8(param_address)
			filter_channel = memory.u8(param_address + 1)
			bytes = PackedByteArray([_status()])
		CDL_SETMODE:
			mode = memory.u8(param_address)
			bytes = PackedByteArray([_status()])
		CDL_GETLOCL:
			var absolute: int = position_lba() + PREGAP_SECTORS
			# Minute, second, sector of the header (BCD).
			# Bytes 3-7 (mode, file, channel, submode, coding info) are not modelled: zero. cd_queue.c reads
			# bytes 0-2 only.
			bytes = PackedByteArray([_bcd(absolute / 4500), _bcd((absolute / 75) % 60), _bcd(absolute % 75), 0, 0, 0, 0, 0])
		_:
			engine.refuse("CdControl", "CD command 0x%02X is not modelled (CD-DA Play, Stop, Mute, ... are not used by the ported paths yet)" % command)
			return
	_pending = {"command": command, "result_address": result_address, "bytes": bytes,
		"complete_frame": engine.frame() + COMMAND_LATENCY_FRAMES}

## ReadN and ReadS are commands to the drive too: their ACK carries the status (reading), which is the
## byte CdSync copies into cd_result while a read runs.
func _acknowledge() -> void:
	_pending = {"command": CDL_READS, "result_address": 0, "bytes": PackedByteArray([_status()]),
		"complete_frame": engine.frame() + COMMAND_LATENCY_FRAMES}

## Completes the command in flight when its frame has come: result bytes to the caller's buffer.
func _service() -> void:
	if _pending.is_empty():
		return
	if engine.frame() >= int(_pending.complete_frame):
		_complete_pending(false)

func _complete_pending(_forced: bool) -> void:
	if _pending.is_empty():
		return
	var done: Dictionary = _pending
	_pending = {}
	_last_result = done.bytes
	if int(done.result_address) != 0:
		engine.memory.write_bytes(int(done.result_address), done.bytes)
