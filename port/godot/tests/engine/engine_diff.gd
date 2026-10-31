extends RefCounted
## T-39301: function-differential harness (testing tier 1 of wiki/port-completion-plan.md).
##
## Replays one captured hit of a C function (tools/emu/func_capture) against the ported function:
## loads the captured 2 MiB RAM at entry into the engine's GameMemory, calls the function by its C name
## with the captured arguments (a0-a3, then the captured stack words), and compares
##   * the return value (v0) when the C signature returns one, and
##   * the WHOLE RAM at return with the original's, byte for byte.
## Differing addresses are reported with GameState field names (and global names) from the layout.
##
## Callees the port has not ported are replayed from the capture: when the port reaches one, the harness
## checks that the memory equals the original's memory at that call (entry RAM + pre_diff) and that the
## arguments equal, applies the recorded effect (effect_diff) and returns the recorded v0. A callee the C
## did not call at that point, a missing call, or a refusal is a failure. Test doubles live only here.
##
## Excluded RAM is explicit and needs a reason (an entry of `exclude` without one is an error):
##   {"field": "GameState.frame_count", "reason": ...}        every byte of the field
##   {"global": "cd_sectors_left", "reason": ...}              every byte of a global of core/game_layout.gd
##   {"start": "0x80123456", "length": 16, "reason": ...}      an address range
##   {"sp_offset": 16, "length": 8, "reason": ...}             a range relative to the entry sp (stack arguments)
## Any entry may add "when": {"global": "cd_state", "equals": 4}: it then applies only to hits whose entry RAM
## has that value.
## Always excluded, each with its own reason: the dead stack below the entry sp (the C frames of the function
## and its callees spill registers there; nothing reads them after the return) and the 16-byte o32 argument
## home area at [sp, sp+16) in the caller's frame (the callee may spill a0-a3 there).
## A signature may list "ret_defined": values the C returns on its defined paths; when the original's v0 is
## not one of them the function fell off its end and v0 is not compared.

const Memory = preload("res://core/game_memory.gd")
const Layout = preload("res://core/game_layout.gd")
const Narrow = preload("res://tests/engine/_narrow.gd")
const Math = preload("res://core/native_math.gd")

const DEAD_STACK_BYTES: int = 0x4000
const DEAD_STACK_REASON: String = "dead stack below the entry sp: spills of the C frames of the function and its callees"
const ARG_HOME_BYTES: int = 16
const ARG_HOME_REASON: String = "o32 argument home area [sp, sp+16) in the caller's frame: a callee may spill a0-a3 there (the K&R code does); dead after the return"
const CHUNK: int = 4096
const MAX_REPORTED: int = 12

var _names: Array = [] # [address, size, label], sorted, built on first use
## The differing byte runs [start, end) (RAM offsets) of the last "at return" comparison, after exclusions.
var last_runs: Array = []

# ---- capture reading

## Parses a hit JSON text. Numbers come back as ints (Godot's JSON yields floats for all numbers).
static func parse_hit(text: String) -> Dictionary:
	var json := JSON.new()
	if json.parse(text) != OK or not json.data is Dictionary:
		return {}
	return _ints(json.data)

static func _ints(value: Variant) -> Variant:
	if value is Dictionary:
		var out: Dictionary = {}
		for key: Variant in value:
			out[key] = _ints(value[key])
		return out
	if value is Array:
		var items: Array = []
		for item: Variant in value:
			items.append(_ints(item))
		return items
	if value is float and value == floor(value):
		return int(value)
	return value

## The RAM image of a hit: gzip at <capture>/<fn>/hitNNNN.entry.ram.gz (tools/emu/func_capture/pack.py).
static func read_entry_ram(path: String) -> PackedByteArray:
	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
	if file == null:
		return PackedByteArray()
	var packed: PackedByteArray = file.get_buffer(file.get_length())
	file.close()
	return packed.decompress(Memory.RAM_SIZE, FileAccess.COMPRESSION_GZIP)

## Applies a diff ([[offset, "hex"], ...]) to a RAM image.
static func apply_diff(ram: PackedByteArray, diff: Array) -> void:
	for run: Variant in diff:
		var bytes: PackedByteArray = String(run[1]).hex_decode()
		var offset: int = run[0]
		for i: int in bytes.size():
			ram[offset + i] = bytes[i]

## Applies a diff to the live game memory (the recorded effect of a callee).
static func apply_diff_to_memory(memory, diff: Array) -> void:
	for run: Variant in diff:
		memory.write_bytes(Memory.RAM_BASE + int(run[0]), String(run[1]).hex_decode())

# ---- one hit

## Runs one captured hit. `entry` is the 2 MiB RAM at entry, `hit` the parsed JSON, `signature` the
## module's C_SIGNATURES entry, `exclude` the spec's exclusions (Array of Dictionaries).
## Returns {"ok", "problems": Array[String], "compared_bytes", "excluded_bytes", "callees_replayed"}.
func run_hit(engine, function_name: String, hit: Dictionary, entry: PackedByteArray, signature: Dictionary,
		exclude: Array, allow: Array = [], allow_ret: bool = false) -> Dictionary:
	var problems: Array[String] = []
	var regions: Array = _exclusions(exclude, int(hit.sp), problems, entry)
	last_runs = []
	var result: Dictionary = {"ok": false, "problems": problems, "compared_bytes": 0, "excluded_bytes": 0,
		"callees_replayed": 0, "runs": []}
	if not problems.is_empty():
		return result
	engine.clear_failure()
	engine.clear_stubs()
	engine.memory.restore(entry)
	engine.reset_stack_trace(int(hit.sp)) # the C stack pointer at entry (call_c reserves the frame below it)
	engine.ret_v1 = 0
	var allow_problems: Array[String] = []
	var allowed: Array = _exclusions(allow, int(hit.sp), allow_problems, entry)
	var records: Array = hit.callees
	var state: Dictionary = {"next": 0, "touched": []}
	for name: String in _callee_names(records):
		engine.set_stub(name, _make_stub(engine, name, records, state, entry, regions, problems))
	var args: Array[int] = _arguments(hit, signature)
	var got: int = engine.call_c(function_name, args)
	engine.clear_stubs()
	if not engine.refusal.is_empty():
		problems.append("refused: " + engine.refusal)
	elif not engine.memory.error.is_empty():
		problems.append("memory fault: " + engine.memory.error)
	elif int(state.next) != records.size():
		problems.append("the C called %d unported callee(s), the port reached %d (next missing: %s)" % [
			records.size(), state.next, records[int(state.next)].name])
	var ret_ok: bool = true
	if problems.is_empty():
		var ret_type: String = str(signature.get("ret", "void"))
		var defined: Array = signature.get("ret_defined", [])
		var original_v0: int = int(hit.ret_v0) & 0xffffffff
		if ret_type != "void" and not signature.has("ret_undefined") and (defined.is_empty() or original_v0 in defined):
			var narrowed_original: int = Narrow.apply(original_v0, ret_type) & 0xffffffff
			if narrowed_original != original_v0:
				problems.append("the original's v0 = 0x%X has bits outside the C return type %s (narrowed 0x%X)" % [original_v0, ret_type, narrowed_original])
				ret_ok = false
			elif (got & 0xffffffff) != narrowed_original:
				problems.append("v0 = 0x%X, the original returned 0x%X" % [got & 0xffffffff, narrowed_original])
				ret_ok = false
		if signature.get("ret_v1", false) and (engine.ret_v1 & 0xffffffff) != (int(hit.get("ret_v1", 0)) & 0xffffffff):
			problems.append("v1 = 0x%X, the original returned 0x%X" % [engine.ret_v1 & 0xffffffff, int(hit.get("ret_v1", 0)) & 0xffffffff])
			ret_ok = false
		var expected: PackedByteArray = entry.duplicate()
		apply_diff(expected, hit.exit_diff)
		var ram_problems: Array[String] = _compare_ram(engine.memory.ram, expected, regions, "at return", true)
		problems.append_array(ram_problems)
		var written: Array = [[int(engine.min_sp) & 0x1fffffff, int(hit.sp) & 0x1fffffff]] # frames the port reserved
		written.append_array(state.touched) # and what the replayed callees wrote
		written.sort_custom(func(a: Array, b: Array) -> bool: return a[0] < b[0])
		for problem: String in _port_untouched(engine.memory.ram, entry, regions, written):
			problems.append(problem)
	result.ok = problems.is_empty()
	result.runs = last_runs
	# a hit the spec lists as not comparable passes only if every difference is inside its allowance
	var outside: int = 0
	for run: Array in last_runs:
		outside += _subtract(run, allowed).size()
	result.within_allowance = outside == 0 and (ret_ok or allow_ret) and engine.refusal.is_empty() and engine.memory.error.is_empty() \
		and int(state.next) == records.size() and _only_allowed_problems(problems)
	result.callees_replayed = int(state.next)
	var excluded: int = 0
	for region: Array in regions:
		excluded += int(region[1]) - int(region[0])
	result.excluded_bytes = excluded
	result.compared_bytes = Memory.RAM_SIZE - excluded
	return result

## True when every problem is a RAM or v0 difference (the allowance covers those), not a callee mismatch.
static func _only_allowed_problems(problems: Array[String]) -> bool:
	for problem: String in problems:
		if not (problem.begins_with("RAM differs at return") or problem.begins_with("  0x") or problem.begins_with("v0 = ") \
				or problem.begins_with("port wrote")):
			return false
	return true

## Bytes the port must not have changed: every exclusion that is not marked "port": "free" is one-sided (the
## original may differ there, the port's bytes must still equal the entry RAM) except where the port reserved
## stack frames (`written`, RAM offsets).
func _port_untouched(got: PackedByteArray, entry: PackedByteArray, regions: Array, written: Array) -> Array[String]:
	var out: Array[String] = []
	var changed: Array = _diff_runs(got, entry)
	for run: Array in changed:
		for region: Array in regions:
			if region.size() > 3 and region[3]:
				continue
			var lo: int = maxi(int(run[0]), int(region[0]))
			var hi: int = mini(int(run[1]), int(region[1]))
			if lo >= hi:
				continue
			for piece: Array in _subtract([lo, hi], written):
				if out.size() < MAX_REPORTED:
					out.append("port wrote 0x%08X..0x%08X %s (excluded for the original, but the port must leave it as at entry: %s)" % [
						Memory.RAM_BASE + piece[0], Memory.RAM_BASE + piece[1], describe(Memory.RAM_BASE + piece[0]), region[2]])
	return out

static func _callee_names(records: Array) -> Array[String]:
	var names: Array[String] = []
	for record: Dictionary in records:
		if not names.has(record.name):
			names.append(record.name)
	return names

static func _return_mask(c_type: String) -> int:
	match c_type:
		"u8", "s8":
			return 0xff
		"u16", "s16":
			return 0xffff
	return 0xffffffff

static func _arguments(hit: Dictionary, signature: Dictionary) -> Array[int]:
	var raw: Array[int] = [int(hit.a0), int(hit.a1), int(hit.a2), int(hit.a3)]
	var stack: Array = hit.stack
	for i: int in stack.size():
		raw.append(int(stack[i]))
	var count: int = (signature.get("args", []) as Array).size() + int(signature.get("variadic", 0)) # T-39314: printf, sprintf
	return raw.slice(0, count)

## A test double that replays the next recorded callee call.
func _make_stub(engine, name: String, records: Array, state: Dictionary, entry: PackedByteArray,
		regions: Array, problems: Array[String]) -> Callable:
	return func(args: Array) -> int:
		var index: int = int(state.next)
		if index >= records.size():
			problems.append("the port called %s, which the original did not call again" % name)
			engine.refuse(name, "not called here by the original")
			return 0
		var record: Dictionary = records[index]
		if str(record.name) != name:
			problems.append("the port called %s, the original called %s next" % [name, record.name])
			engine.refuse(name, "original called %s next" % record.name)
			return 0
		state.next = index + 1
		var before: PackedByteArray = entry.duplicate()
		apply_diff(before, record.pre_diff)
		for problem: String in _compare_ram(engine.memory.ram, before, regions, "at the call of " + name):
			problems.append(problem)
		var raw: Array[int] = [int(record.a0), int(record.a1), int(record.a2), int(record.a3)]
		for i: int in mini(args.size(), 4):
			if (int(args[i]) & 0xffffffff) != (raw[i] & 0xffffffff):
				problems.append("%s argument %d = 0x%X, the original passed 0x%X" % [name, i, int(args[i]) & 0xffffffff, raw[i]])
		var stack: Array = record.get("stack", [])
		for i: int in range(4, mini(args.size(), 4 + stack.size())):
			if (int(args[i]) & 0xffffffff) != (int(stack[i - 4]) & 0xffffffff):
				problems.append("%s stack argument %d = 0x%X, the original passed 0x%X" % [name, i, int(args[i]) & 0xffffffff, int(stack[i - 4]) & 0xffffffff])
		# the bytes behind a pointer argument that points into the stack (buffers the callee reads)
		var pointed: Dictionary = record.get("ptr_mem", {})
		for key: String in pointed:
			var arg_index: int = int(key.substr(1))
			if arg_index < args.size():
				var want: PackedByteArray = String(pointed[key]).hex_decode()
				var have: PackedByteArray = engine.memory.read_bytes(int(args[arg_index]), want.size())
				if have != want:
					problems.append("%s argument %d points at bytes that differ from the original's: port %s, original %s" % [
						name, arg_index, have.hex_encode().left(48), want.hex_encode().left(48)])
		apply_diff_to_memory(engine.memory, record.effect_diff)
		for run: Variant in record.effect_diff: # the callee's own writes (its frames, its data) are not the port's
			state.touched.append([int(run[0]), int(run[0]) + String(run[1]).length() / 2])
		engine.ret_v1 = int(record.get("ret_v1", 0))
		return int(record.ret_v0)

# ---- exclusions and diffing

## [start, end) byte offsets (RAM offsets, sorted) of the always-dead stack and the spec's exclusions.
func _exclusions(exclude: Array, sp: int, problems: Array[String], entry_ram: PackedByteArray = PackedByteArray()) -> Array:
	var regions: Array = []
	var stack_end: int = sp & 0x1fffffff
	regions.append([maxi(0, stack_end - DEAD_STACK_BYTES), stack_end, DEAD_STACK_REASON, false])
	regions.append([stack_end, stack_end + ARG_HOME_BYTES, ARG_HOME_REASON, false])
	for entry: Variant in exclude:
		if not entry is Dictionary or str(entry.get("reason", "")).strip_edges().is_empty():
			problems.append("an exclusion needs a reason: " + str(entry))
			continue
		if entry.has("when") and not _condition_holds(entry.when, entry_ram, problems):
			continue
		if entry.has("global"):
			var global_row: Variant = Layout.GLOBALS.get(str(entry.global))
			if not global_row is Array:
				problems.append("unknown exclusion global " + str(entry.global))
				continue
			var global_start: int = int(global_row[0]) & 0x1fffffff
			regions.append([global_start, global_start + maxi(1, int(global_row[3])) * int(global_row[5]), str(entry.reason), _free(entry)])
		elif entry.has("field"):
			var range_of: Array = field_range(str(entry.field))
			if range_of.is_empty():
				problems.append("unknown exclusion field " + str(entry.field))
				continue
			regions.append([range_of[0], range_of[1], str(entry.reason), _free(entry)])
		elif entry.has("sp_offset") and entry.has("length"):
			var from: int = stack_end + int(entry.sp_offset)
			regions.append([from, from + int(entry.length), str(entry.reason), _free(entry)])
		elif entry.has("start") and entry.has("length"):
			var start: int = int(str(entry.start).hex_to_int()) if str(entry.start).begins_with("0x") else int(entry.start)
			regions.append([start & 0x1fffffff, (start & 0x1fffffff) + int(entry.length), str(entry.reason), _free(entry)])
		else:
			problems.append("an exclusion has neither field nor start/length: " + str(entry))
	regions.sort_custom(func(a: Array, b: Array) -> bool: return a[0] < b[0])
	return regions

## "port": "free" on an exclusion lets the port write there (it computes the field itself); otherwise the
## original may differ but the port's bytes must equal the entry RAM.
static func _free(entry: Dictionary) -> bool:
	return str(entry.get("port", "")) == "free"

## An exclusion's "when": {"global": name, "equals": value} read from the entry RAM (the exclusion applies
## only while the original was in that state, for example while XA is streaming).
func _condition_holds(when: Variant, entry_ram: PackedByteArray, problems: Array[String]) -> bool:
	if not when is Dictionary or not when.has("global") or not when.has("equals"):
		problems.append("an exclusion condition needs global and equals: " + str(when))
		return false
	var row: Variant = Layout.GLOBALS.get(str(when.global))
	if not row is Array or entry_ram.size() != Memory.RAM_SIZE:
		problems.append("unknown condition global " + str(when.global))
		return false
	var memory := Memory.new()
	memory.restore(entry_ram)
	return memory.read_global(str(when.global)) == int(when.equals)

## RAM [start, end) of "GameState.field" or "SystemData.field" (all elements), or [] when unknown.
static func field_range(path: String) -> Array:
	var parts: PackedStringArray = path.split(".")
	if parts.size() != 2:
		return []
	var base: int = Layout.GAME_STATE_BASE if parts[0] == "GameState" else Layout.SYSTEM_DATA_BASE if parts[0] == "SystemData" else -1
	var rows: Dictionary = Layout.STRUCTS.get(parts[0], {}).get("fields", {})
	if base < 0 or not rows.has(parts[1]):
		return []
	var row: Array = rows[parts[1]]
	var start: int = (base + row[0]) & 0x1fffffff
	return [start, start + maxi(1, row[3]) * row[5]]

## Differences between two RAM images outside `regions`, as readable problems.
func _compare_ram(got: PackedByteArray, want: PackedByteArray, regions: Array, when: String, final: bool = false) -> Array[String]:
	var out: Array[String] = []
	var runs: Array = _diff_runs(got, want)
	var kept: Array = []
	for run: Array in runs:
		for piece: Array in _subtract(run, regions):
			kept.append(piece)
	if final:
		last_runs = kept
	if kept.is_empty():
		return out
	var total: int = 0
	for piece: Array in kept:
		total += int(piece[1]) - int(piece[0])
	out.append("RAM differs %s: %d byte(s) in %d run(s)" % [when, total, kept.size()])
	for i: int in mini(kept.size(), MAX_REPORTED):
		var start: int = kept[i][0]
		var end: int = kept[i][1]
		var shown: int = mini(end - start, 8)
		out.append("  0x%08X %s: port %s, original %s%s" % [Memory.RAM_BASE + start, describe(Memory.RAM_BASE + start),
			got.slice(start, start + shown).hex_encode(), want.slice(start, start + shown).hex_encode(),
			" ..." if end - start > shown else ""])
	return out

## Byte runs [start, end) where the images differ (chunked so equal 4 KiB blocks cost one compare).
static func _diff_runs(a: PackedByteArray, b: PackedByteArray) -> Array:
	var runs: Array = []
	if a == b:
		return runs
	var open_start: int = -1
	for chunk_start: int in range(0, Memory.RAM_SIZE, CHUNK):
		if a.slice(chunk_start, chunk_start + CHUNK) == b.slice(chunk_start, chunk_start + CHUNK):
			if open_start >= 0:
				runs.append([open_start, chunk_start])
				open_start = -1
			continue
		for i: int in range(chunk_start, chunk_start + CHUNK):
			if a[i] != b[i]:
				if open_start < 0:
					open_start = i
			elif open_start >= 0:
				runs.append([open_start, i])
				open_start = -1
	if open_start >= 0:
		runs.append([open_start, Memory.RAM_SIZE])
	return runs

## The parts of `run` ([start, end)) outside every region (regions are sorted [start, end, reason]).
static func _subtract(run: Array, regions: Array) -> Array:
	var pieces: Array = []
	var cursor: int = run[0]
	for region: Array in regions:
		if region[1] <= cursor or region[0] >= run[1]:
			continue
		if region[0] > cursor:
			pieces.append([cursor, region[0]])
		cursor = maxi(cursor, region[1])
	if cursor < run[1]:
		pieces.append([cursor, run[1]])
	return pieces

# ---- names

## "GameState.mode_step+0" / "global cd_queue_head" / "" for a PS1 address.
func describe(address: int) -> String:
	if _names.is_empty():
		_build_names()
	var low: int = 0
	var high: int = _names.size() - 1
	var found: int = -1
	while low <= high:
		var middle: int = (low + high) >> 1
		if _names[middle][0] <= address:
			found = middle
			low = middle + 1
		else:
			high = middle - 1
	if found >= 0 and address < int(_names[found][0]) + int(_names[found][1]):
		var entry: Array = _names[found]
		var offset: int = address - int(entry[0])
		return "%s%s" % [entry[2], "+%d" % offset if offset != 0 else ""]
	return ""

func _build_names() -> void:
	var rows: Array = []
	for struct_name: String in ["GameState", "SystemData"]:
		var base: int = Layout.GAME_STATE_BASE if struct_name == "GameState" else Layout.SYSTEM_DATA_BASE
		var fields: Dictionary = Layout.STRUCTS.get(struct_name, {}).get("fields", {})
		for field: String in fields:
			var row: Array = fields[field]
			rows.append([base + row[0], maxi(1, row[3]) * row[5], "%s.%s" % [struct_name, field]])
	for global_name: String in Layout.GLOBALS:
		var row: Array = Layout.GLOBALS[global_name]
		if global_name in ["game_state", "system_data"] or (row[0] >= Layout.GAME_STATE_BASE and row[0] < Layout.GAME_STATE_BASE + Memory.GAME_STATE_SIZE):
			continue # the structs themselves and aliases of their fields: the field name is the better label
		rows.append([row[0], maxi(1, row[3]) * row[5], "global " + global_name])
	# Later (more specific) rows win on overlap: GameState/SystemData fields first, then globals; the
	# lookup picks the last start <= address, so sort by start and keep the stable order.
	rows.sort_custom(func(a: Array, b: Array) -> bool: return a[0] < b[0])
	_names = rows
