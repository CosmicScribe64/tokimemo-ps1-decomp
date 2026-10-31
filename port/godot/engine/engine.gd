extends RefCounted
## T-39301 (M2, layer 2 of port-completion-plan): the engine facade.
##
## Owns the game memory (core/game_memory.gd), the disc pack (services/disc_pack.gd), the generic renderer
## (presentation/ps1_renderer.gd), the CD drive model (_cd_drive.gd), the libc layer (_libc.gd) and the
## slots for the existing text layer and native audio. Every engine/<c_file>.gd (one GDScript file per
## src/main C file, see wiki/engine-porting-contract.md) is a module: this facade finds them, checks them,
## and lets them call each other by C name:
##
##     engine.call_c("mode_step_next")
##     var lba: int = engine.call_c("cd_position_lba")
##
## call_c owns the C calling convention: it checks the argument count, reserves the callee's original stack
## frame (config of tools/port/gen_engine_tables.py: _frame_sizes.gd) around the call, restores sp on every
## exit and narrows the return value to the C return type of C_SIGNATURES.
##
## A call to a C function no module lists is a refusal, never a silent no-op: it latches `refusal` naming the
## caller and the callee, FREEZES the game memory (no write of any kind succeeds until clear_failure()),
## returns 0, and every later call returns 0 without running. Services other than RAM (the CD drive, renderer,
## audio, text layer) check halted() before they act. The driver (frame loop, test) checks `halted()` after
## each top-level call and stops the run. A memory fault freezes the same way. Because writes cannot land after
## the failing point, a module needs no check before its stores; it only has to stop LOOPS (see the contract).
##
## Module discovery: every res://engine/*.gd except this file and files starting with "_" is a module;
## adding a C file therefore never edits a shared file (workers do not conflict on a module list).

const Memory = preload("res://core/game_memory.gd")
const Math = preload("res://core/native_math.gd")
const DiscPack = preload("res://services/disc_pack.gd")
const Renderer = preload("res://presentation/ps1_renderer.gd")
const CdDrive = preload("res://engine/_cd_drive.gd")
const Libc = preload("res://engine/_libc.gd")
const SdkGpu = preload("res://engine/_sdk_gpu.gd")
const SdkEtc = preload("res://engine/_sdk_etc.gd")
const SdkCard = preload("res://engine/_sdk_card.gd")
const SdkGte = preload("res://engine/_sdk_gte.gd")
const Frames = preload("res://engine/_frame_sizes.gd")
const Symbols = preload("res://engine/_symbols.gd")

const MODULE_DIR: String = "res://engine"
const SELF_FILE: String = "engine.gd"
## A C call chain this deep is a port bug (the C stack of the original is 0x1FFF00 down; the real
## chains are under a dozen), so it is refused instead of recursing until GDScript fails.
const MAX_CALL_DEPTH: int = 64
## SYSTEM.CNF: STACK = 801FFF00.
const STACK_TOP: int = 0x801FFF00

var memory: Memory = Memory.new()
var disc_pack: DiscPack = DiscPack.new()
var renderer: Renderer = Renderer.new()
## The CD drive behind libcd (engine/_cd_drive.gd): reads come from `disc_pack`.
var cd_drive: CdDrive = CdDrive.new()
## The services below are attached by the integrating code (null until then). A module that needs one
## and finds null refuses with the function name; it never skips the work.
var text_layer = null # services/text_layer_store.gd instance
var audio = null # services/audio_service.gd instance (native audio engine behind it)
## T-39314: the virtual memory card behind the SDK card calls (services/memory_card_service.gd). Null until the
## integrating code attaches one; the BIOS card calls then refuse by name (engine/_sdk_card.gd).
var memory_card = null

## First refusal, "" while the run is healthy. Format: "<caller> -> <callee>: <reason>".
var refusal: String = ""
## Every module found: C file stem ("game_mode") -> instance.
var modules: Dictionary = {}
## Problems found while registering the modules (a listed function that is not a method, two modules
## listing one name, an argument count that differs from C_SIGNATURES, a function without a frame size).
## Empty when the engine is consistent; tests assert it.
var registration_errors: Array[String] = []

## The PS1 stack pointer of the C function currently running, AFTER its prologue (call_c subtracts the
## original frame size before the call and restores sp after it). A function whose C frame holds an object
## whose address is taken uses frame_base() + the original local offset. Tests set sp to the captured entry
## sp before the top-level call. The default is the top of the original's stack (SYSTEM.CNF).
var sp: int = STACK_TOP
## The lowest sp any call reached since reset_stack_trace() (the harness checks the port wrote no stack
## below it).
var min_sp: int = STACK_TOP
## Second return register of a function returning a pair (64-bit); modules set it, the harness compares it
## when the signature declares "ret_v1".
var ret_v1: int = 0

var _functions: Dictionary = {} # C function name -> module instance
var _callables: Dictionary = {} # C function name -> cached Callable
var _arity: Dictionary = {} # C function name -> argument count
var _returns: Dictionary = {} # C function name -> C return type ("void", "s32", ...)
## Real (not padded) variadic arguments of the variadic C function now running (T-39314): call_c pads missing ones
## with 0 so the method can be called, but a formatter must refuse to consume one of them.
var variadic_supplied: int = 0
var _required: Dictionary = {} # variadic C function name -> number of required arguments (T-39314)
var _stubs: Dictionary = {} # C function (or pointer slot) name -> Callable(args: Array) -> int, test doubles
var _stack: Array[String] = []
var _by_address: Dictionary = {} # code address -> C function name, built on first use
var _field_cache: Dictionary = {} # GameState field -> [address, width, signed]

func _init() -> void:
	cd_drive.engine = self
	var libc := Libc.new()
	libc.engine = self
	_add_module("libc", libc)
	# T-39314: the SDK layer (PsyQ libgpu/libgs, libetc/libapi, libcard, libgte), one helper module per library.
	for entry: Array in [["sdk_gpu", SdkGpu], ["sdk_etc", SdkEtc], ["sdk_card", SdkCard], ["sdk_gte", SdkGte]]:
		var sdk: RefCounted = (entry[1] as GDScript).new()
		sdk.set("engine", self)
		_add_module(entry[0], sdk)
	_register_modules()

## Breaks the engine <-> module reference cycles so the engine can be freed (modules hold `engine`).
## Call it when the engine is no longer used (tests do).
func shutdown() -> void:
	for stem: String in modules:
		modules[stem].set("engine", null)
	cd_drive.engine = null
	modules.clear()
	_functions.clear()
	_callables.clear()
	_stubs.clear()

# ---- module registry

func _register_modules() -> void:
	var dir: DirAccess = DirAccess.open(MODULE_DIR)
	if dir == null:
		registration_errors.append("cannot open " + MODULE_DIR)
		return
	var files: PackedStringArray = dir.get_files()
	files.sort()
	for file_name: String in files:
		# Exported projects list "x.gd.remap" (or "x.gdc"); the loader wants the .gd name.
		var source: String = file_name.trim_suffix(".remap").trim_suffix(".gdc")
		if source.ends_with(".gd.uid") or not source.ends_with(".gd"):
			continue
		if source == SELF_FILE or source.begins_with("_"):
			continue
		var script: Script = load(MODULE_DIR.path_join(source)) as Script
		if script == null or not script.can_instantiate():
			registration_errors.append("%s does not compile" % source)
			continue
		var module: RefCounted = script.new()
		module.set("engine", self)
		_add_module(source.get_basename(), module)

## Registers one module (also used by tests to add a module outside res://engine). `check_frames` false
## skips the frame-size requirement (test modules with invented function names).
func _add_module(stem: String, module: RefCounted, check_frames: bool = true) -> void:
	var names: Variant = module.get("C_FUNCTIONS")
	var c_file: Variant = module.get("C_FILE")
	if not names is Array or not c_file is String or (c_file as String).is_empty():
		registration_errors.append("%s lacks C_FILE or C_FUNCTIONS" % stem)
		return
	var signatures: Variant = module.get("C_SIGNATURES")
	var methods: Dictionary = {}
	for method: Dictionary in module.get_script().get_script_method_list():
		methods[method.name] = (method.args as Array).size()
	modules[stem] = module
	for function_name: Variant in names:
		if not function_name is String or not module.has_method(function_name):
			registration_errors.append("%s lists %s, which is not a method" % [stem, function_name])
		elif _functions.has(function_name):
			registration_errors.append("%s and %s both list %s" % [_stem_of(_functions[function_name]), stem, function_name])
		else:
			_functions[function_name] = module
			_callables[function_name] = Callable(module, function_name)
			_arity[function_name] = int(methods.get(function_name, 0))
			var signature: Variant = signatures.get(function_name) if signatures is Dictionary else null
			if signature is Dictionary:
				_returns[function_name] = str(signature.get("ret", "s32"))
				var listed: int = (signature.get("args", []) as Array).size()
				if signature.has("variadic"):
					# a variadic C function (printf, sprintf): "args" are the required arguments, "variadic" the
					# number of extra integer arguments the method takes; call_c pads the missing ones with 0
					_required[function_name] = listed
					if listed + int(signature["variadic"]) != _arity[function_name]:
						registration_errors.append("%s.%s takes %d argument(s), C_SIGNATURES lists %d required + %d variadic" % [
							stem, function_name, _arity[function_name], listed, int(signature["variadic"])])
				elif listed != _arity[function_name]:
					registration_errors.append("%s.%s takes %d argument(s), C_SIGNATURES lists %d" % [
						stem, function_name, _arity[function_name], listed])
			if check_frames and not Frames.FRAME_SIZES.has(function_name):
				registration_errors.append("%s.%s has no frame size in engine/_frame_sizes.gd (regenerate with tools/port/gen_engine_tables.py)" % [stem, function_name])

func _stem_of(module: RefCounted) -> String:
	for stem: String in modules:
		if modules[stem] == module:
			return stem
	return "?"

## True when a module lists the C function.
func is_ported(function_name: String) -> bool:
	return _functions.has(function_name)

## Every ported C function name, sorted.
func ported_functions() -> PackedStringArray:
	var out := PackedStringArray(_functions.keys())
	out.sort()
	return out

## The module that ported `function_name`, or null.
func module_of(function_name: String) -> RefCounted:
	return _functions.get(function_name)

# ---- renderer (T-39314)

var _renderer_tried: bool = false

## True when the generic renderer can take work: it is opened on first use (the GDExtension is not installed
## in every environment). A caller that needs the renderer and gets false refuses naming its function; it never
## skips the draw.
func renderer_ready() -> bool:
	if renderer.is_open():
		return true
	if not _renderer_tried:
		_renderer_tried = true
		# Ps1Renderer.open() reports a missing library as an engine error: look for it first (res://bin holds it)
		var present: bool = ClassDB.class_exists(Renderer.NATIVE_CLASS)
		for file_name: String in DirAccess.get_files_at("res://bin"):
			if file_name.begins_with("liboriginal_render") and file_name.get_extension() in ["so", "dylib", "dll", "remap"]:
				present = true
		if present:
			renderer.open()
		else:
			renderer.last_error = "native PS1 renderer library is not installed under res://bin"
	return renderer.is_open()

# ---- name <-> address (generated from config/symbol_addrs*.txt and the asm labels)

## The PS1 code address of a C function, or -1.
func function_address(function_name: String) -> int:
	return int(Symbols.FUNCTIONS.get(function_name, -1))

## The C function at a code address ("" when none is known, e.g. overlay code).
func function_at(address: int) -> String:
	if _by_address.is_empty():
		for function_name: String in Symbols.FUNCTIONS:
			_by_address[Symbols.FUNCTIONS[function_name]] = function_name
	return str(_by_address.get(address & 0xffffffff, ""))

# ---- calls

## Calls a C function by name with its arguments (integers; pointers are PS1 addresses). Returns its
## return value narrowed to its C return type, 0 for void. After a refusal (or a memory fault) it returns 0
## at once. An argument count that differs from the function's is a refusal.
func call_c(function_name: String, args: Array = []) -> int:
	if halted():
		return 0
	if _stubs.has(function_name):
		return _run_stub(function_name, _stubs[function_name], args)
	var callable: Callable = _callables.get(function_name, Callable())
	if callable.is_null():
		refuse(function_name, "C function %s is not ported (no engine module lists it)" % function_name)
		return 0
	if args.size() != _arity[function_name]:
		if _required.has(function_name) and args.size() >= int(_required[function_name]) and args.size() < _arity[function_name]:
			variadic_supplied = args.size() - int(_required[function_name])
			args = args.duplicate()
			while args.size() < _arity[function_name]:
				args.append(0)
		else:
			refuse(function_name, "%s takes %d argument(s), called with %d" % [function_name, _arity[function_name], args.size()])
			return 0
	elif _required.has(function_name):
		variadic_supplied = args.size() - int(_required[function_name])
	if _stack.size() >= MAX_CALL_DEPTH:
		refuse(function_name, "call depth %d exceeded (%s)" % [MAX_CALL_DEPTH, " > ".join(_stack)])
		return 0
	var saved_sp: int = sp
	sp -= int(Frames.FRAME_SIZES.get(function_name, 0))
	min_sp = mini(min_sp, sp)
	_stack.push_back(function_name)
	var result: Variant = callable.callv(args)
	_stack.pop_back()
	sp = saved_sp
	return _narrow(result, _returns.get(function_name, "s32"))

## The C return type applied to a value: sign-extension of s8/s16, masks of u8/u16/u32/ptr, s32 wrap.
static func _narrow(value: Variant, c_type: String) -> int:
	if value == null:
		return 0
	var number: int = int(value)
	match c_type:
		"void":
			return 0
		"s8":
			return Math.s8(number)
		"u8":
			return Math.u8(number)
		"s16":
			return Math.s16(number)
		"u16":
			return Math.u16(number)
		"u32", "ptr":
			return Math.u32(number)
	return Math.s32(number)

## A call through a function pointer or code address, for example `((s32 (*)()) ovl_entry)()` in
## game_mode_dispatch: `via` names the C expression ("ovl_entry"), `address` is the PS1 code address it
## holds. An address of a known main-executable function (the generated table) dispatches to call_c, so a
## ported function runs and an unported one refuses by name; any other address is overlay or unmapped code
## and refuses (overlay code is ported per overlay) unless a test double is installed under `via`.
func call_address(via: String, address: int, args: Array = []) -> int:
	if halted():
		return 0
	if _stubs.has(via):
		return _run_stub(via, _stubs[via], args)
	var function_name: String = function_at(address)
	if not function_name.is_empty():
		return call_c(function_name, args)
	refuse(via, "code at 0x%08X called through %s is not ported (overlay or unmapped code)" % [address, via])
	return 0

func _run_stub(function_name: String, stub: Callable, args: Array) -> int:
	_stack.push_back(function_name)
	var result: Variant = stub.call(args)
	_stack.pop_back()
	return int(result) if result != null else 0

## Latches a refusal (the first one wins) and freezes the game memory. `callee` is the C function (or
## expression) that cannot run.
func refuse(callee: String, reason: String) -> void:
	if not refusal.is_empty():
		return
	var caller: String = _stack[_stack.size() - 1] if not _stack.is_empty() else "<top level>"
	refusal = "%s -> %s: %s" % [caller, callee, reason]
	memory.frozen = true

## True after a refusal or a memory fault: nothing computed from here on is valid, no write lands, and
## services other than RAM must not act.
func halted() -> bool:
	return not refusal.is_empty() or not memory.error.is_empty()

## The first problem, refusal before memory fault, "" when healthy.
func failure() -> String:
	if not refusal.is_empty():
		return refusal
	return "memory fault: " + memory.error if not memory.error.is_empty() else ""

## Clears the latched refusal and memory fault and unfreezes the memory (a test between two calls).
func clear_failure() -> void:
	refusal = ""
	memory.clear_error()
	_stack.clear()

## The C functions currently running (outermost first).
func call_stack() -> PackedStringArray:
	return PackedStringArray(_stack)

# ---- stack frames

## The sp of the function now running (after its prologue). Locals are at frame_base() + their offset in
## the original frame (the offsets are in the C comments and asm/matchings/main/<file>/<function>.s).
func frame_base() -> int:
	return sp

## Starts a fresh stack trace for a top-level call (the harness): sp and min_sp.
func reset_stack_trace(entry_sp: int) -> void:
	sp = entry_sp
	min_sp = entry_sp

# ---- clock

## The game's frame counter (game_state.frame_count, incremented once per main-loop iteration). Every timer
## the engine models (the CD drive, ...) is a function of this value, never of wall-clock time.
func frame() -> int:
	return gs_get("frame_count")

# ---- hot-path accessors (cached addresses; the View API does a string lookup and an allocation per access)

func _gs_row(field: String) -> Array:
	var row: Variant = _field_cache.get(field)
	if row == null:
		var view: Memory.View = memory.game_state()
		var address: int = view.address_of(field)
		var layout_row: Array = view._row(field)
		if layout_row.is_empty() or address < 0 or layout_row[7] != 0 or not (layout_row[4] as String).is_empty():
			memory._fault("GameState.%s is not a plain scalar field (use game_state() views for bit-fields and structs)" % field)
			return [0, 4, false]
		row = [address, layout_row[1], layout_row[2]]
		_field_cache[field] = row
	return row

## GameState scalar field (element 0) by cached address.
func gs_get(field: String) -> int:
	var row: Array = _gs_row(field)
	return memory.read_int(row[0], row[1], row[2])

func gs_set(field: String, value: int) -> void:
	var row: Array = _gs_row(field)
	memory.write_int(row[0], row[1], value)

## Address of a GameState field (element 0), cached; a nested struct field gives the struct's address.
func gs_address(field: String) -> int:
	var row: Variant = _field_cache.get(field)
	if row != null:
		return row[0]
	return memory.game_state().address_of(field)

# ---- test doubles (never used by the game)

## Installs a double for an unported C function (or a pointer slot name given to call_address). The
## Callable receives the arguments array and returns the C return value. Used by the function-
## differential harness to replay a callee's recorded effect; the game never installs one.
func set_stub(function_name: String, stub: Callable) -> void:
	_stubs[function_name] = stub

func clear_stubs() -> void:
	_stubs.clear()
