extends RefCounted
## T-39301 (M2, layer 2 of port-completion-plan): the engine facade.
##
## Owns the game memory (core/game_memory.gd), the disc pack (services/disc_pack.gd), the generic
## renderer (presentation/ps1_renderer.gd) and the slots for the existing text layer and native audio.
## Every engine/<c_file>.gd (one GDScript file per src/main C file, see wiki/engine-porting-contract.md)
## is a module: this facade finds them, checks them, and lets them call each other by C name:
##
##     engine.call_c("mode_step_next")
##     var lba: int = engine.call_c("cd_position_lba")
##
## A call to a C function no module lists is a refusal, never a silent no-op: it latches `refusal`
## naming the caller and the callee, returns 0, and every later call returns 0 without running, so
## nothing downstream is computed from an invented value. The driver (frame loop, test) checks
## `halted()` after each top-level call and stops the run. Modules check it after any call whose
## result or side effects the rest of the function depends on and return at once.
##
## Module discovery: every res://engine/*.gd except this file and files starting with "_" is a module;
## adding a C file therefore never edits a shared file (workers do not conflict on a module list).

const Memory = preload("res://core/game_memory.gd")
const DiscPack = preload("res://services/disc_pack.gd")
const Renderer = preload("res://presentation/ps1_renderer.gd")
const CdDrive = preload("res://engine/_cd_drive.gd")

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

## First refusal, "" while the run is healthy. Format: "<caller> -> <callee>: <reason>".
var refusal: String = ""
## Every module found: C file stem ("game_mode") -> instance.
var modules: Dictionary = {}
## Problems found while registering the modules (a listed function that is not a method, two modules
## listing one name). Empty when the engine is consistent; tests assert it.
var registration_errors: Array[String] = []

## The PS1 stack pointer of the C function currently running (after its prologue). A function whose C
## frame holds an object whose address is taken (a local buffer passed to another function) reserves the
## frame with push_frame(size) and releases it with pop_frame(size) before it returns, with the frame size
## of the original (the `addiu sp, sp, -N` of its asm), so the buffer sits where the original's did.
## Tests set it to the captured entry sp. The default is the top of the original's stack (SYSTEM.CNF).
var sp: int = STACK_TOP

var _functions: Dictionary = {} # C function name -> module instance
var _stubs: Dictionary = {} # C function (or pointer slot) name -> Callable(args: Array) -> int, test doubles
var _stack: Array[String] = []

func _init() -> void:
	cd_drive.engine = self
	_register_modules()

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

## Registers one module (also used by tests to add a module outside res://engine).
func _add_module(stem: String, module: RefCounted) -> void:
	var names: Variant = module.get("C_FUNCTIONS")
	var c_file: Variant = module.get("C_FILE")
	if not names is Array or not c_file is String or (c_file as String).is_empty():
		registration_errors.append("%s lacks C_FILE or C_FUNCTIONS" % stem)
		return
	modules[stem] = module
	for function_name: Variant in names:
		if not function_name is String or not module.has_method(function_name):
			registration_errors.append("%s lists %s, which is not a method" % [stem, function_name])
		elif _functions.has(function_name):
			registration_errors.append("%s and %s both list %s" % [_stem_of(_functions[function_name]), stem, function_name])
		else:
			_functions[function_name] = module

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

# ---- calls

## Calls a C function by name with its arguments (integers; pointers are PS1 addresses). Returns its
## return value at the C width, 0 for void. After a refusal (or a memory fault) it returns 0 at once.
func call_c(function_name: String, args: Array = []) -> int:
	if halted():
		return 0
	if _stubs.has(function_name):
		return _run_stub(function_name, _stubs[function_name], args)
	var module: RefCounted = _functions.get(function_name)
	if module == null:
		refuse(function_name, "C function %s is not ported (no engine module lists it)" % function_name)
		return 0
	if _stack.size() >= MAX_CALL_DEPTH:
		refuse(function_name, "call depth %d exceeded (%s)" % [MAX_CALL_DEPTH, " > ".join(_stack)])
		return 0
	_stack.push_back(function_name)
	var result: Variant = module.callv(function_name, args)
	_stack.pop_back()
	return int(result) if result != null else 0

## A call through a function pointer or code address the port cannot resolve to a C function, for
## example `((s32 (*)()) ovl_entry)()` in game_mode_dispatch: `via` names the C expression ("ovl_entry"),
## `address` is the PS1 code address it holds. Overlay code is ported per overlay, so this refuses
## unless a test double is installed under `via`.
func call_address(via: String, address: int, args: Array = []) -> int:
	if halted():
		return 0
	if _stubs.has(via):
		return _run_stub(via, _stubs[via], args)
	refuse(via, "code at 0x%08X called through %s is not ported (overlay or unmapped code)" % [address, via])
	return 0

func _run_stub(function_name: String, stub: Callable, args: Array) -> int:
	_stack.push_back(function_name)
	var result: Variant = stub.call(args)
	_stack.pop_back()
	return int(result) if result != null else 0

## Latches a refusal (the first one wins). `callee` is the C function (or expression) that cannot run.
func refuse(callee: String, reason: String) -> void:
	if not refusal.is_empty():
		return
	var caller: String = _stack[_stack.size() - 1] if not _stack.is_empty() else "<top level>"
	refusal = "%s -> %s: %s" % [caller, callee, reason]

## True after a refusal or a memory fault: nothing computed from here on is valid.
func halted() -> bool:
	return not refusal.is_empty() or not memory.error.is_empty()

## The first problem, refusal before memory fault, "" when healthy.
func failure() -> String:
	if not refusal.is_empty():
		return refusal
	return "memory fault: " + memory.error if not memory.error.is_empty() else ""

## Clears the latched refusal and memory fault (a test between two calls).
func clear_failure() -> void:
	refusal = ""
	memory.clear_error()
	_stack.clear()

## The C functions currently running (outermost first).
func call_stack() -> PackedStringArray:
	return PackedStringArray(_stack)

# ---- stack frames

## Reserves a C stack frame of `size` bytes and returns the new sp (the frame's address).
func push_frame(size: int) -> int:
	sp -= size
	return sp

func pop_frame(size: int) -> void:
	sp += size

# ---- clock

## The game's frame counter (game_state.frame_count, incremented once per main-loop iteration). Every timer
## the engine models (the CD drive, ...) is a function of this value, never of wall-clock time.
func frame() -> int:
	return memory.game_state().get_field("frame_count")

# ---- test doubles (never used by the game)

## Installs a double for an unported C function (or a pointer slot name given to call_address). The
## Callable receives the arguments array and returns the C return value. Used by the function-
## differential harness to replay a callee's recorded effect; the game never installs one.
func set_stub(function_name: String, stub: Callable) -> void:
	_stubs[function_name] = stub

func clear_stubs() -> void:
	_stubs.clear()
