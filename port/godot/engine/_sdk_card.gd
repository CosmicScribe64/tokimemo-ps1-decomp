extends RefCounted
## T-39314: the BIOS event and memory-card calls of the facade (asm/libapi_0.s .. libapi_4.s, asm/libcard_card.s).
## Not a C file of the game; engine.gd registers it as module "sdk_card".
##
## What is modelled
## - Critical sections: EnterCriticalSection returns 1 when interrupts were enabled and disables them,
##   ExitCriticalSection enables them again (the COP0 status IE bit is hardware state kept here).
## - BIOS events (OpenEvent, EnableEvent, CloseEvent, TestEvent): the event control block table lives in kernel RAM
##   exactly as the BIOS keeps it (psx-spx "Kernel Memory": the table pointer at 0x80000120 and its byte size at
##   0x80000124; one 7-word entry per event: class, status, spec, mode, function, 2 reserved; the handle is
##   0xF1000000 + the entry index). Status values: 0x1000 disabled, 0x2000 enabled, 0x4000 delivered.
##   TestEvent(h) returns 1 and re-arms (0x4000 -> 0x2000) a delivered event. A saved RAM image carries the table,
##   so the captures replay: 8 card events of OpenBIOS (entry index 1..8) are in every capture's RAM.
## - The software memory-card events of _card_info/_card_load: the job finishes a fixed number of frames after the
##   call (`INFO_FRAMES`, `LOAD_FRAMES`, measured below) and then delivers EvSpIOE on the SwCARD class
##   (0xF4000001, spec 4) through the event table. The result is a function of `frame_count` only: the check runs
##   when a TestEvent/EnableEvent/_card_* call asks (`_deliver_due`), never from a clock of its own.
##
## What is refused (blocking-wait rule / unmodelled hardware), each by its symbol name
## - _card_status, _card_clear, the BIOS file calls open/close/read/write/format/delete: the captures
##   (card2-*, card-*; tools/emu/func_capture, OpenBIOS) only show a blank card: open fails for ~100 frames after
##   the load event and then returns descriptor 2, _card_status answers 2 or 4 every frame, read/write start a job
##   that finishes after hundreds of frames. A success path with a formatted card, and the frame count a file job
##   takes, were never observed, so no value is invented: the calls refuse and the run stops at them.
## - Event delivery to a callback (mode 0x1000) and the other BIOS event classes.
##
## Measured profile (OpenBIOS in PCSX-Redux, blank card, capture card2-load): _card_info called at frame 192, the
## event is seen at 194 (INFO_FRAMES = 2, OpenBIOS, blank card only); _card_load called at frame 195, the event is seen at 267
## (LOAD_FRAMES = 72). This is the emulated BIOS, not hardware parity; both numbers come from one capture each.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "libapi/libcard (asm/libapi_0.s .. libapi_4.s, asm/libcard_card.s)"
const C_FUNCTIONS: Array[String] = [
	"EnterCriticalSection", "ExitCriticalSection", "OpenEvent", "EnableEvent", "CloseEvent", "TestEvent",
	"InitCARD", "StartCARD", "_bu_init", "_card_auto", "_card_info", "_card_load",
	"_card_status", "_card_clear", "open", "close", "read", "write", "format", "delete",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"EnterCriticalSection": {"args": [], "ret": "s32"},
	"ExitCriticalSection": {"args": [], "ret": "void"},
	"OpenEvent": {"args": ["u32", "s32", "s32", "ptr"], "ret": "s32"},
	"EnableEvent": {"args": ["s32"], "ret": "s32"},
	"CloseEvent": {"args": ["s32"], "ret": "s32"},
	"TestEvent": {"args": ["s32"], "ret": "s32"},
	"InitCARD": {"args": ["s32"], "ret": "void"},
	"StartCARD": {"args": [], "ret": "s32"},
	"_bu_init": {"args": [], "ret": "void"},
	"_card_auto": {"args": ["s32"], "ret": "void"},
	"_card_info": {"args": ["s32"], "ret": "s32"},
	"_card_load": {"args": ["s32"], "ret": "s32"},
	"_card_status": {"args": ["s32"], "ret": "s32"},
	"_card_clear": {"args": ["s32"], "ret": "s32"},
	"open": {"args": ["ptr", "s32"], "ret": "s32"},
	"close": {"args": ["s32"], "ret": "s32"},
	"read": {"args": ["s32", "ptr", "s32"], "ret": "s32"},
	"write": {"args": ["s32", "ptr", "s32"], "ret": "s32"},
	"format": {"args": ["ptr"], "ret": "s32"},
	"delete": {"args": ["ptr"], "ret": "s32"},
}

const EVENT_TABLE_POINTER: int = 0x80000120
const EVENT_TABLE_SIZE: int = 0x80000124
## The table the BIOS of the captures keeps (pointer 0x8DD0, 0x578 bytes = 50 entries); used when a fresh memory has
## no kernel yet. The table is private to the BIOS, so the address does not matter to the game.
const EVENT_TABLE_DEFAULT: int = 0x80008DD0
const EVENT_TABLE_DEFAULT_SIZE: int = 0x578
const EVENT_ENTRY: int = 0x1C
const HANDLE_BASE: int = 0xF1000000
const STATUS_DISABLED: int = 0x1000
const STATUS_ENABLED: int = 0x2000
const STATUS_DELIVERED: int = 0x4000
const MODE_CALLBACK: int = 0x1000

const CLASS_SW_CARD: int = 0xF4000001
const SPEC_IOE: int = 0x4
const INFO_FRAMES: int = 2
const LOAD_FRAMES: int = 72

var engine # GameEngine

## COP0 status IE: true while interrupts are enabled (EnterCriticalSection clears it).
var interrupts_enabled: bool = true
## Pending software card jobs: {"due": frame, "class": int, "spec": int}.
var _jobs: Array[Dictionary] = []
var driver_started: bool = false

func _refuse(function_name: String, reason: String) -> void:
	engine.refuse(function_name, reason)

# ---- critical sections

func EnterCriticalSection() -> int:
	var was: bool = interrupts_enabled
	interrupts_enabled = false
	return 1 if was else 0

func ExitCriticalSection() -> void:
	interrupts_enabled = true

# ---- event control blocks (kernel RAM)

## Table base and entry count; sets the default table up when the kernel pointers are empty.
func _table() -> Array:
	var m = engine.memory
	if m.u32(EVENT_TABLE_POINTER) == 0:
		m.write_u32(EVENT_TABLE_POINTER, EVENT_TABLE_DEFAULT)
		m.write_u32(EVENT_TABLE_SIZE, EVENT_TABLE_DEFAULT_SIZE)
	return [m.u32(EVENT_TABLE_POINTER), m.u32(EVENT_TABLE_SIZE) / EVENT_ENTRY]

## Address of the entry of `handle`, or -1 when it is not a handle of an open event.
func _entry(handle: int) -> int:
	var table: Array = _table()
	var h: int = Math.u32(handle)
	if (h & 0xFF000000) != HANDLE_BASE:
		return -1
	var index: int = h & 0xFFFFFF
	if index >= int(table[1]):
		return -1
	var at: int = int(table[0]) + index * EVENT_ENTRY
	return at if engine.memory.u32(at) != 0 else -1

## OpenEvent(class, spec, mode, func): the first free entry (class 0) takes the event, disabled; returns its handle.
func OpenEvent(event_class: int, spec: int, mode: int, function_pointer: int) -> int:
	var m = engine.memory
	var table: Array = _table()
	for index: int in int(table[1]):
		var at: int = int(table[0]) + index * EVENT_ENTRY
		if m.u32(at) == 0:
			m.write_u32(at, event_class)
			m.write_u32(at + 4, STATUS_DISABLED)
			m.write_u32(at + 8, spec)
			m.write_u32(at + 12, mode)
			m.write_u32(at + 16, function_pointer)
			return Math.s32(HANDLE_BASE | index)
	return -1

## EnableEvent(h): the event becomes active (status 0x2000); returns 1.
func EnableEvent(handle: int) -> int:
	var at: int = _entry(handle)
	if at < 0:
		return 0
	_deliver_due()
	engine.memory.write_u32(at + 4, STATUS_ENABLED)
	return 1

## CloseEvent(h): frees the entry; returns 1.
func CloseEvent(handle: int) -> int:
	var at: int = _entry(handle)
	if at < 0:
		return 0
	engine.memory.fill(at, EVENT_ENTRY, 0)
	return 1

## TestEvent(h): 1 once after the event was delivered (and the event is re-armed), else 0.
func TestEvent(handle: int) -> int:
	_deliver_due()
	var at: int = _entry(handle)
	if at < 0:
		return 0
	var m = engine.memory
	if m.u32(at + 4) == STATUS_DELIVERED:
		m.write_u32(at + 4, STATUS_ENABLED)
		return 1
	return 0

## DeliverEvent(class, spec): every enabled entry with that class and spec is marked delivered. An entry with a
## callback would call game code from an interrupt: refused.
func deliver_event(event_class: int, spec: int) -> void:
	var m = engine.memory
	var table: Array = _table()
	for index: int in int(table[1]):
		var at: int = int(table[0]) + index * EVENT_ENTRY
		if m.u32(at) == event_class and m.u32(at + 8) == spec and m.u32(at + 4) == STATUS_ENABLED:
			if m.u32(at + 12) == MODE_CALLBACK:
				_refuse("DeliverEvent", "event 0x%08X has a callback (mode 0x1000): interrupt callbacks are not modelled" % (HANDLE_BASE | index))
				return
			m.write_u32(at + 4, STATUS_DELIVERED)

## Delivers the card jobs whose frame has come (a function of frame_count and the jobs the game started).
func _deliver_due() -> void:
	var now: int = engine.frame()
	var remaining: Array[Dictionary] = []
	for job: Dictionary in _jobs:
		if int(job["due"]) <= now:
			deliver_event(int(job["class"]), int(job["spec"]))
		else:
			remaining.append(job)
	_jobs = remaining

# ---- the card driver

func InitCARD(_pad_enable: int) -> void:
	pass # the BIOS card driver is initialised at boot; nothing observable (no capture reaches it)

func StartCARD() -> int:
	driver_started = true
	return 1

func _bu_init() -> void:
	pass

func _card_auto(_flag: int) -> void:
	pass

## The 2 and 72 frame numbers are "OpenBIOS, blank (unformatted) card only": measured once each on a blank card.
## The model therefore accepts a card job only when the attached card is in that state, and only when no other job
## is pending (two jobs at once were never observed); anything else refuses naming the call and the missing
## measurement. The game's own waits are longer than the 72 frames (memory_card_timer gives up at 0x200 polls,
## src/ovl/*/album_save_flow.c, save_and_exit.c, settings_sound_test_and_extras.c), so the model cannot trip them.
func _card_job(function_name: String, frames: int) -> int:
	if engine.memory_card == null:
		_refuse(function_name, "no memory card is attached to the engine (engine.memory_card)")
		return 0
	var state: Dictionary = engine.memory_card.check()
	var result: int = int(state.get("result", 0))
	if result != 0xF1:
		_refuse(function_name, "the %d-frame SwCARD event was measured only on a blank (unformatted) card; this card reports 0x%02X (%s) and no timing was measured for it" % [
			frames, result, str(state.get("error", ""))])
		return 0
	if not _jobs.is_empty():
		_refuse(function_name, "a card job is already pending; two jobs at once were never observed")
		return 0
	_jobs.append({"due": engine.frame() + frames, "class": CLASS_SW_CARD, "spec": SPEC_IOE})
	return 1

## _card_info(port): starts the card information job; returns 1. The SwCARD IOE event follows INFO_FRAMES later.
func _card_info(_port: int) -> int:
	return _card_job("_card_info", INFO_FRAMES)

## _card_load(port): starts the card load job; returns 1. The SwCARD IOE event follows LOAD_FRAMES later.
func _card_load(_port: int) -> int:
	return _card_job("_card_load", LOAD_FRAMES)

func _card_status(_port: int) -> int:
	_refuse("_card_status", "the BIOS card status word was only observed on a blank card (2 and 4 every frame); the success path is unmeasured")
	return 0

func _card_clear(_port: int) -> int:
	_refuse("_card_clear", "never reached by a capture; the hardware-event timing is unmeasured")
	return 0

func open(_name_pointer: int, _mode: int) -> int:
	_refuse("open", "the BIOS file open was only observed on a blank card (fails ~100 frames, then descriptor 2); no formatted-card timing is measured")
	return 0

func close(_fd: int) -> int:
	_refuse("close", "the BIOS file close was only observed with the file jobs of a blank card; unmodelled with open")
	return 0

func read(_fd: int, _buffer: int, _count: int) -> int:
	_refuse("read", "the BIOS file read starts an asynchronous job whose frame count was only observed on a blank card")
	return 0

func write(_fd: int, _buffer: int, _count: int) -> int:
	_refuse("write", "the BIOS file write starts an asynchronous job whose frame count was only observed on a blank card")
	return 0

func format(_device_pointer: int) -> int:
	_refuse("format", "never reached by a capture; the format job timing is unmeasured")
	return 0

func delete(_name_pointer: int) -> int:
	_refuse("delete", "the BIOS file delete was observed once on a blank card; no formatted-card behaviour is measured")
	return 0
