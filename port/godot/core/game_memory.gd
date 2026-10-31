extends RefCounted
## T-39300 (M1): the PS1 main RAM as the port's game memory, with the original layout.
## 2 MiB at 0x80000000 (KSEG0; KUSEG and KSEG1 aliases accepted). Access widths and
## signedness go through core/native_math.gd. Typed, named views of GameState and the
## gameplay globals come from core/game_layout.gd (tools/port/gen_game_memory.py).
##
## Original bytes (the executable image) are read at runtime from the user's installed
## content (load_executable_file) and never committed. Nothing here invents values: RAM
## starts zeroed, like .bss, and only loads and explicit writes change it.
##
## Faults are latched, not silent: an access outside RAM or a misaligned halfword/word
## sets `error` (first one wins), returns 0 and writes nothing. Callers that must not
## continue past a fault check `error` (the R3000 would raise an address error).

const Math = preload("res://core/native_math.gd")
const Layout = preload("res://core/game_layout.gd")

const RAM_SIZE: int = 0x200000
const RAM_BASE: int = 0x80000000
const GAME_STATE_BASE: int = 0x800E6280
const GAME_STATE_SIZE: int = 0x1A90
## SHA-256 of the main executable (SLPM_86.053), the EXE_sha256 identity that
## audio/first-arrival/source.json of an installed bundle records.
const EXECUTABLE_SHA256: String = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
## PS-X EXE header (offsets are file offsets): "PS-X EXE", pc0, gp0, t_addr, t_size.
const EXE_MAGIC: String = "PS-X EXE"
const EXE_HEADER_SIZE: int = 0x800

var ram := PackedByteArray()
var error: String = ""
## True after a fault or when the engine freezes the memory at a refusal (engine/engine.gd): every write
## (write_*, write_bytes, fill) returns without changing RAM, so nothing computed after the failing point
## can leave a trace. clear_error() (the engine's clear_failure) unfreezes. restore() and load_executable()
## are explicit external loads and still work.
var frozen: bool = false
## Loaded executable facts (not RAM): entry point, gp, text base and size.
var executable: Dictionary = {}

func _init() -> void:
	ram.resize(RAM_SIZE)
	ram.fill(0)

func reset() -> void:
	ram.fill(0)
	error = ""
	frozen = false
	executable = {}

# ---- addressing

## RAM offset of `address` for an access of `width` bytes, or -1 (and `error`).
func _offset(address: int, width: int) -> int:
	var physical: int = address & 0x1fffffff
	if address < 0 or address > 0xffffffff or (address >> 29) not in [0, 4, 5] \
			or physical + width > RAM_SIZE:
		_fault("address 0x%08x (width %d) is outside main RAM" % [address, width])
		return -1
	if physical & (width - 1) != 0:
		_fault("address 0x%08x is not aligned to %d bytes" % [address, width])
		return -1
	return physical

func _fault(message: String) -> void:
	frozen = true
	if error.is_empty():
		error = message

func clear_error() -> void:
	error = ""
	frozen = false

# ---- scalar access (R3000 little endian)

func u8(address: int) -> int:
	var o: int = _offset(address, 1)
	return ram[o] if o >= 0 else 0

func s8(address: int) -> int:
	return Math.s8(u8(address))

func u16(address: int) -> int:
	var o: int = _offset(address, 2)
	return ram.decode_u16(o) if o >= 0 else 0

func s16(address: int) -> int:
	return Math.s16(u16(address))

func u32(address: int) -> int:
	var o: int = _offset(address, 4)
	return ram.decode_u32(o) if o >= 0 else 0

func s32(address: int) -> int:
	return Math.s32(u32(address))

func write_u8(address: int, value: int) -> void:
	if frozen:
		return
	var o: int = _offset(address, 1)
	if o >= 0:
		ram[o] = Math.u8(value)

func write_u16(address: int, value: int) -> void:
	if frozen:
		return
	var o: int = _offset(address, 2)
	if o >= 0:
		ram.encode_u16(o, Math.u16(value))

func write_u32(address: int, value: int) -> void:
	if frozen:
		return
	var o: int = _offset(address, 4)
	if o >= 0:
		ram.encode_u32(o, Math.u32(value))

## Signed and unsigned stores keep only the low bits, as sb/sh/sw do.
func write_s8(address: int, value: int) -> void:
	write_u8(address, value)

func write_s16(address: int, value: int) -> void:
	write_u16(address, value)

func write_s32(address: int, value: int) -> void:
	write_u32(address, value)

## Width-generic read/write used by the typed views.
func read_int(address: int, width: int, signed: bool) -> int:
	match width:
		1:
			return s8(address) if signed else u8(address)
		2:
			return s16(address) if signed else u16(address)
		4:
			return s32(address) if signed else u32(address)
	_fault("unsupported scalar width %d" % width)
	return 0

func write_int(address: int, width: int, value: int) -> void:
	match width:
		1:
			write_u8(address, value)
		2:
			write_u16(address, value)
		4:
			write_u32(address, value)
		_:
			_fault("unsupported scalar width %d" % width)

# ---- byte ranges

func read_bytes(address: int, length: int) -> PackedByteArray:
	var o: int = _range(address, length)
	return ram.slice(o, o + length) if o >= 0 else PackedByteArray()

func write_bytes(address: int, data: PackedByteArray) -> void:
	if frozen:
		return
	var o: int = _range(address, data.size())
	if o < 0:
		return
	for i: int in data.size():
		ram[o + i] = data[i]

func fill(address: int, length: int, value: int) -> void:
	if frozen:
		return
	var o: int = _range(address, length)
	if o < 0:
		return
	for i: int in length:
		ram[o + i] = value & 0xff

func _range(address: int, length: int) -> int:
	if length < 0:
		_fault("negative length %d" % length)
		return -1
	if length == 0:
		return 0 if _offset(address & ~3, 4) >= 0 else -1
	var first: int = _offset(address, 1)
	if first < 0 or _offset(address + length - 1, 1) < 0:
		return -1
	return first

# ---- executable image

## Loads the PS-X EXE the way the BIOS does: the text image (code and data) at its
## t_addr. .bss is not part of the file and stays zero. Returns "" or the refusal.
func load_executable(image: PackedByteArray) -> String:
	if image.size() < EXE_HEADER_SIZE or image.slice(0, 8).get_string_from_ascii() != EXE_MAGIC:
		return "not a PS-X EXE image"
	var t_addr: int = image.decode_u32(0x18)
	var t_size: int = image.decode_u32(0x1c)
	if t_size == 0 or t_size % 4 != 0 or image.size() < EXE_HEADER_SIZE + t_size:
		return "PS-X EXE text size 0x%x does not fit the %d-byte file" % [t_size, image.size()]
	var physical: int = t_addr & 0x1fffffff
	if (t_addr >> 29) != 4 or physical + t_size > RAM_SIZE or t_addr & 3 != 0:
		return "PS-X EXE text 0x%08x+0x%x lies outside main RAM" % [t_addr, t_size]
	for i: int in t_size:
		ram[physical + i] = image[EXE_HEADER_SIZE + i]
	executable = {"pc": image.decode_u32(0x10), "gp": image.decode_u32(0x14),
		"text_address": t_addr, "text_size": t_size}
	return ""

## Reads the executable from an installed bundle (for example
## <bundle>/audio/first-arrival/executable.bin) and checks its pinned SHA-256 first.
## The bytes stay in this object; nothing is written back to disk.
func load_executable_file(path: String) -> String:
	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
	if file == null:
		return "cannot open the installed executable"
	var image: PackedByteArray = file.get_buffer(file.get_length())
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(image)
	if hash.finish().hex_encode() != EXECUTABLE_SHA256:
		return "the installed executable is not the pinned SLPM_86.053"
	return load_executable(image)

# ---- typed views

## Named, typed access to a struct at an address, from the generated layout.
class View extends RefCounted:
	var memory # the GameMemory that owns the bytes (this script has no class_name)
	var struct_name: String
	var base: int

	func _init(owner, name: String, address: int) -> void:
		memory = owner
		struct_name = name
		base = address

	func _row(field: String) -> Array:
		var table: Dictionary = Layout.STRUCTS.get(struct_name, {})
		var rows: Dictionary = table.get("fields", {})
		if not rows.has(field):
			memory._fault("%s has no field %s" % [struct_name, field])
			return []
		return rows[field]

	func has_field(field: String) -> bool:
		return Layout.STRUCTS.get(struct_name, {}).get("fields", {}).has(field)

	## Address of element `index` of `field`.
	func address_of(field: String, index: int = 0) -> int:
		var row: Array = _row(field)
		if row.is_empty():
			return -1
		if index < 0 or index >= row[3]:
			memory._fault("%s.%s index %d outside %d elements" % [struct_name, field, index, row[3]])
			return -1
		return base + row[0] + index * row[5]

	func get_field(field: String, index: int = 0) -> int:
		var row: Array = _row(field)
		var address: int = address_of(field, index)
		if row.is_empty() or address < 0:
			return 0
		if not (row[4] as String).is_empty():
			memory._fault("%s.%s is a struct; use at()" % [struct_name, field])
			return 0
		var bits: int = row[7]
		if bits == 0:
			return memory.read_int(address, row[1], row[2])
		var raw: int = memory.read_int(address, row[1], false) >> row[6]
		raw &= (1 << bits) - 1
		if row[2] and raw & (1 << (bits - 1)) != 0:
			raw -= 1 << bits
		return raw

	func set_field(field: String, value: int, index: int = 0) -> void:
		var row: Array = _row(field)
		var address: int = address_of(field, index)
		if row.is_empty() or address < 0:
			return
		if not (row[4] as String).is_empty():
			memory._fault("%s.%s is a struct; use at()" % [struct_name, field])
			return
		var bits: int = row[7]
		if bits == 0:
			memory.write_int(address, row[1], value)
			return
		var mask: int = ((1 << bits) - 1) << row[6]
		var word: int = memory.read_int(address, row[1], false)
		memory.write_int(address, row[1], (word & ~mask) | ((value << row[6]) & mask))

	## View of element `index` of a nested-struct field.
	func at(field: String, index: int = 0) -> View:
		var row: Array = _row(field)
		var address: int = address_of(field, index)
		if row.is_empty() or address < 0 or (row[4] as String).is_empty():
			if not row.is_empty():
				memory._fault("%s.%s is not a struct" % [struct_name, field])
			return View.new(memory, "", 0)
		return View.new(memory, row[4], address)

	## Number of elements of `field` (1 for a scalar).
	func count_of(field: String) -> int:
		var row: Array = _row(field)
		return row[3] if not row.is_empty() else 0

	## Raw bytes of the whole field (every element).
	func bytes_of(field: String) -> PackedByteArray:
		var row: Array = _row(field)
		if row.is_empty():
			return PackedByteArray()
		return memory.read_bytes(base + row[0], row[3] * row[5])

func view(struct_name: String, address: int) -> View:
	return View.new(self, struct_name, address)

## GameState at 0x800E6280.
func game_state() -> View:
	return View.new(self, "GameState", GAME_STATE_BASE)

## SystemData at 0x800E7D10.
func system_data() -> View:
	return View.new(self, "SystemData", Layout.SYSTEM_DATA_BASE)

# ---- globals

func has_global(name: String) -> bool:
	return Layout.GLOBALS.has(name)

func global_address(name: String) -> int:
	if not Layout.GLOBALS.has(name):
		_fault("unknown global %s" % name)
		return -1
	return Layout.GLOBALS[name][0]

## Element `index` of a scalar global (or array of scalars).
func read_global(name: String, index: int = 0) -> int:
	if not Layout.GLOBALS.has(name):
		_fault("unknown global %s" % name)
		return 0
	var row: Array = Layout.GLOBALS[name]
	if not (row[4] as String).is_empty():
		_fault("global %s is a struct; use global_view()" % name)
		return 0
	if index < 0 or (row[3] > 0 and index >= row[3]):
		_fault("global %s index %d outside %d elements" % [name, index, row[3]])
		return 0
	return read_int(row[0] + index * row[5], row[1], row[2])

func write_global(name: String, value: int, index: int = 0) -> void:
	if not Layout.GLOBALS.has(name):
		_fault("unknown global %s" % name)
		return
	var row: Array = Layout.GLOBALS[name]
	if not (row[4] as String).is_empty():
		_fault("global %s is a struct; use global_view()" % name)
		return
	if index < 0 or (row[3] > 0 and index >= row[3]):
		_fault("global %s index %d outside %d elements" % [name, index, row[3]])
		return
	write_int(row[0] + index * row[5], row[1], value)

func global_view(name: String) -> View:
	if not Layout.GLOBALS.has(name) or (Layout.GLOBALS[name][4] as String).is_empty():
		_fault("global %s is not a struct" % name)
		return View.new(self, "", 0)
	return View.new(self, Layout.GLOBALS[name][4], Layout.GLOBALS[name][0])

# ---- snapshots

func snapshot() -> PackedByteArray:
	return ram.duplicate()

func restore(bytes: PackedByteArray) -> void:
	if bytes.size() != RAM_SIZE:
		_fault("snapshot is %d bytes, RAM is %d" % [bytes.size(), RAM_SIZE])
		return
	ram = bytes.duplicate()

## Writes the whole RAM image (development dumps for tools/port/compare_game_memory.py).
func dump_to_file(path: String) -> String:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return "cannot write " + path
	file.store_buffer(ram)
	file.close()
	return ""
