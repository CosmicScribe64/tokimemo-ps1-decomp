extends RefCounted
## T-39301: the engine facade (engine/engine.gd): module registry, call by C name, refusals, test doubles,
## stack frames. Invented memory only.

const GameEngine = preload("res://engine/engine.gd")
const Narrow = preload("res://tests/engine/_narrow.gd")

var failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

## A throw-away module defined from source, to test registry errors and recursion without touching the real ones.
static func _module(source: String) -> RefCounted:
	var script := GDScript.new()
	script.source_code = source
	if script.reload() != OK:
		return null
	return script.new()

func run() -> Array[String]:
	failures.clear()
	_test_registry()
	_test_refusals()
	_test_doubles_and_frames()
	_test_frames_and_calling_convention()
	_test_function_table()
	_test_freeze()
	_test_registry_errors()
	return failures

func _test_registry() -> void:
	var engine = GameEngine.new()
	_check(engine.registration_errors.is_empty(), "the engine registers its modules without errors: %s" % [engine.registration_errors])
	for stem: String in ["game_mode", "cd_queue", "reset_iris_wipe"]:
		_check(engine.modules.has(stem), "module %s is registered" % stem)
	_check(engine.is_ported("mode_step_next") and engine.is_ported("cd_read_sectors"), "ported functions are found by C name")
	_check(not engine.is_ported("title_mode_handler") and not engine.is_ported("_cd_drive"), "unported names and helper files are not")
	for stem: String in engine.modules:
		var module: RefCounted = engine.modules[stem]
		var signatures: Dictionary = module.get("C_SIGNATURES")
		for function_name: String in module.get("C_FUNCTIONS"):
			_check(signatures.has(function_name), "%s.%s has a C_SIGNATURES entry" % [stem, function_name])
		var unported_list: Variant = module.get("C_UNPORTED")
		if unported_list is Array:
			for unported: Variant in unported_list:
				_check(not engine.is_ported(String(unported)), "%s lists %s as unported but a module ports it" % [stem, unported])
	_check(engine.memory.game_state().base == 0x800E6280, "the engine owns a GameMemory with the GameState at 0x800E6280")

## These tests need a callee that nobody has ported (the refusal itself is under test). Every package that
## lands ports some of the callees they used, so the facade is told to forget the function (T-39312: the
## HUD package ported message_window_title_show).
static func _unport(engine, names: Array) -> void:
	for function_name: String in names:
		engine._functions.erase(function_name)
		engine._callables.erase(function_name)

func _test_refusals() -> void:
	var engine = GameEngine.new()
	_unport(engine, ["message_window_title_show"])
	var gs = engine.memory.game_state()
	gs.set_field("mode_step", 5)
	_check(engine.call_c("title_mode_handler") == 0 and engine.refusal.contains("title_mode_handler") \
			and engine.refusal.contains("<top level>") and engine.refusal.contains("not ported"), "an unported callee is refused by name: %s" % engine.refusal)
	gs.set_field("mode_step", 5) # (frozen: ignored)
	_check(engine.call_c("mode_step_next") == 0 and gs.get_field("mode_step") == 5, "after a refusal nothing runs (no silent progress)")
	engine.clear_failure()
	# game_mode_set(0x30) reaches message_window_title_show (target mode 0x30 is neither 0x5E nor 0x50)
	engine.call_c("game_mode_set", [0x30])
	_check(engine.refusal.begins_with("game_mode_set -> message_window_title_show"), "a nested refusal names caller and callee: %s" % engine.refusal)
	_check(gs.get_field("game_mode") == 0x30 and gs.get_field("mode_step") == 0, "the state at the refusal is the state before the callee call")
	engine.clear_failure()
	engine.call_address("ovl_entry", 0x80123456)
	_check(engine.refusal.contains("0x80123456") and engine.refusal.contains("ovl_entry"), "an overlay call through a pointer is refused with its address: %s" % engine.refusal)
	engine.clear_failure()
	engine.memory.write_u32(0x00200000, 1)
	_check(engine.halted() and engine.failure().begins_with("memory fault"), "a memory fault halts the engine")
	engine.clear_failure()
	_check(not engine.halted() and engine.failure().is_empty(), "clear_failure resets the latch")

func _test_doubles_and_frames() -> void:
	var engine = GameEngine.new()
	var seen: Array = []
	engine.set_stub("message_window_title_show", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 7)
	engine.call_c("game_mode_set", [0x30])
	_check(engine.refusal.is_empty() and seen == [[0]], "a test double receives the arguments and replaces the refusal: %s" % [seen])
	engine.clear_stubs()
	engine.call_c("game_mode_set", [0x5E])
	_check(engine.refusal.is_empty(), "game_mode_set(0x5E) never calls message_window_title_show (C: t != 0x5E)")
	engine.shutdown()

func _test_frames_and_calling_convention() -> void:
	var engine = GameEngine.new()
	var top: int = engine.sp
	_check(engine.call_c("mode_step_next") == 0 and engine.sp == top, "sp is restored after a call")
	_check(engine.call_c("mode_step_next", [1]) == 0 and engine.refusal.contains("takes 0 argument(s), called with 1"), "an argument count mismatch is refused: %s" % engine.refusal)
	engine.clear_failure()
	# cd_read_sectors has a 0x48-byte frame; its buffer is at frame + 0x28 = entry sp - 0x20, and the 12 bytes of
	# the 16 the ring copies that it never writes are the stale bytes found there
	engine.set_stub("audio_command", func(args: Array) -> int:
		return 0)
	for i: int in 16:
		engine.memory.write_u8(top - 0x20 + i, 0xA0 + i)
	engine.call_c("cd_read_sectors", [3, 0x80100000, 100])
	var ring: int = engine.memory.global_address("cd_command_ring")
	_check(engine.memory.read_bytes(ring + 8 + 4, 11) == PackedByteArray([0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE]) and engine.sp == top,
		"call_c puts the original frame under the callee: the ring copies the stale stack bytes of buf (%s)" % engine.memory.read_bytes(ring + 12, 11).hex_encode())
	engine.clear_stubs()
	# return values are narrowed to the C return type
	_check(Narrow.apply(0x1FF, "u8") == 0xFF and Narrow.apply(0xFF, "s8") == -1 and Narrow.apply(0x18000, "s16") == -0x8000 \
			and Narrow.apply(-1, "u16") == 0xFFFF and Narrow.apply(0x100000001, "s32") == 1 and Narrow.apply(-1, "ptr") == 0xFFFFFFFF and Narrow.apply(5, "void") == 0, "return narrowing by C type")
	engine.shutdown()

func _test_function_table() -> void:
	var engine = GameEngine.new()
	_check(engine.function_address("mode_step_next") == 0x8004284C and engine.function_at(0x8004284C) == "mode_step_next", "name <-> address table (config/symbol_addrs*.txt)")
	_check(engine.function_address("no_such_function") == -1 and engine.function_at(0x80132000) == "", "unknown names and overlay addresses have no entry")
	var gs = engine.memory.game_state()
	gs.set_field("mode_step", 4)
	engine.call_address("some_pointer", 0x8004284C)
	_check(gs.get_field("mode_step") == 5 and engine.refusal.is_empty(), "call_address of a ported main-exe function dispatches to it")
	engine.call_address("some_pointer", 0x8005A06C)
	_check(engine.refusal.contains("boot_mode_handler") and engine.refusal.contains("not ported"), "call_address of an unported main-exe function refuses by name: %s" % engine.refusal)
	engine.shutdown()

func _test_freeze() -> void:
	var engine = GameEngine.new()
	_unport(engine, ["message_window_title_show"])
	var gs = engine.memory.game_state()
	gs.set_field("game_mode", 0x30)
	gs.set_field("mode_step", 3)
	var before_all: PackedByteArray = engine.memory.snapshot()
	engine.call_c("game_mode_set", [0x30]) # refuses at message_window_title_show
	var at_refusal: PackedByteArray = engine.memory.snapshot()
	_check(engine.halted() and engine.memory.frozen, "a refusal freezes the memory")
	# everything after it, in any form, leaves RAM exactly as it was at the call
	engine.memory.write_u8(0x80100000, 1)
	engine.memory.write_u32(0x80100004, 1)
	engine.memory.write_bytes(0x80100010, PackedByteArray([1, 2, 3]))
	engine.memory.fill(0x80100020, 8, 0xFF)
	gs.set_field("mode_step", 9)
	engine.call_c("mode_step_next")
	engine.call_c("cd_command_push", [9, 1, 0])
	engine.cd_drive.note_read_request(5, 0, 0)
	_check(engine.memory.snapshot() == at_refusal, "no write lands after a refusal")
	_check(engine.cd_drive.cd_control(9, 0, 0) == 0 and engine.cd_drive.cd_sync(1, 0) == 0 and engine.cd_drive.cd_read(1, 0x80100000, 0) == 0, "the CD drive does not act after a refusal")
	engine.clear_failure()
	_check(not engine.memory.frozen and not engine.halted(), "clear_failure unfreezes")
	engine.memory.write_u8(0x80100000, 1)
	_check(engine.memory.u8(0x80100000) == 1, "writes work again")
	# a memory fault freezes too
	engine.memory.u32(0x00200000)
	engine.memory.write_u8(0x80100001, 7)
	_check(engine.memory.frozen and engine.memory.u8(0x80100001) == 0, "a memory fault freezes the memory")
	_check(before_all != at_refusal, "(the refusing call did change RAM before the callee call, as the C does)")
	engine.shutdown()

func _test_registry_errors() -> void:
	var engine = GameEngine.new()
	engine.registration_errors.clear()
	var module: RefCounted = _module("extends RefCounted\nconst C_FILE := 'x.c'\nconst C_FUNCTIONS := ['mode_step_next', 'no_such_method']\nvar engine\nfunc mode_step_next(): return 1\n")
	_check(module != null, "test module compiles")
	engine._add_module("dup", module, false)
	_check(engine.registration_errors.size() == 2 and "".join(engine.registration_errors).contains("both list mode_step_next") \
			and "".join(engine.registration_errors).contains("no_such_method"), "duplicate and missing methods are reported: %s" % [engine.registration_errors])
	var loop: RefCounted = _module("extends RefCounted\nconst C_FILE := 'y.c'\nconst C_FUNCTIONS := ['spin']\nvar engine\nfunc spin(): return engine.call_c('spin')\n")
	engine._add_module("loop", loop, false)
	loop.set("engine", engine)
	engine.call_c("spin")
	_check(engine.refusal.contains("call depth"), "runaway recursion is refused: %s" % engine.refusal.left(80))
