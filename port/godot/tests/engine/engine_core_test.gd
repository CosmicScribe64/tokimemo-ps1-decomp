extends RefCounted
## T-39301: the engine facade (engine/engine.gd): module registry, call by C name, refusals, test doubles,
## stack frames. Invented memory only.

const GameEngine = preload("res://engine/engine.gd")

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

func _test_refusals() -> void:
	var engine = GameEngine.new()
	_check(engine.call_c("title_mode_handler") == 0 and engine.refusal.contains("title_mode_handler") \
			and engine.refusal.contains("<top level>") and engine.refusal.contains("not ported"), "an unported callee is refused by name: %s" % engine.refusal)
	var gs = engine.memory.game_state()
	gs.set_field("mode_step", 5)
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
	var top: int = engine.sp
	var frame: int = engine.push_frame(0x48)
	_check(frame == top - 0x48 and engine.sp == frame, "push_frame moves sp down by the C frame size")
	engine.pop_frame(0x48)
	_check(engine.sp == top, "pop_frame restores sp")

func _test_registry_errors() -> void:
	var engine = GameEngine.new()
	engine.registration_errors.clear()
	var module: RefCounted = _module("extends RefCounted\nconst C_FILE := 'x.c'\nconst C_FUNCTIONS := ['mode_step_next', 'no_such_method']\nvar engine\nfunc mode_step_next(): return 1\n")
	_check(module != null, "test module compiles")
	engine._add_module("dup", module)
	_check(engine.registration_errors.size() == 2 and "".join(engine.registration_errors).contains("both list mode_step_next") \
			and "".join(engine.registration_errors).contains("no_such_method"), "duplicate and missing methods are reported: %s" % [engine.registration_errors])
	var loop: RefCounted = _module("extends RefCounted\nconst C_FILE := 'y.c'\nconst C_FUNCTIONS := ['spin']\nvar engine\nfunc spin(): return engine.call_c('spin')\n")
	engine._add_module("loop", loop)
	loop.set("engine", engine)
	engine.call_c("spin")
	_check(engine.refusal.contains("call depth"), "runaway recursion is refused: %s" % engine.refusal.left(80))
