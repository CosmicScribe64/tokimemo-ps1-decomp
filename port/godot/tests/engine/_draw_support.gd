extends RefCounted
## T-39311: shared helpers of the draw-core hand checks (gpu_primitives_test.gd, ...): an engine on invented
## memory, recording doubles for the unported callees, and small memory readers. Not a test (leading "_").

const GameEngine = preload("res://engine/engine.gd")

var engines: Array = []
var failures: Array[String] = []

func new_engine():
	var engine = GameEngine.new()
	engines.append(engine)
	return engine

func shutdown_all() -> void:
	for engine in engines:
		engine.shutdown()
	engines.clear()

func check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

## Installs a recording double: every call appends [name, args...] to `log`; `results` maps a name to the value
## it returns (default 0); `effects` maps a name to a Callable(args) run before returning (to write memory).
func record(engine, names: Array, log: Array, results: Dictionary = {}, effects: Dictionary = {}) -> void:
	for n: String in names:
		engine.set_stub(n, func(args: Array) -> int:
			log.append([n] + args.map(func(a): return a & 0xFFFFFFFF))
			if effects.has(n):
				effects[n].call(args)
			return int(results.get(n, 0)))

func bytes(engine, address: int, count: int) -> Array:
	var out: Array = []
	for i: int in count:
		out.append(engine.memory.u8(address + i))
	return out

func set_gs_s16(engine, field: String, index: int, value: int) -> void:
	engine.memory.write_s16(engine.gs_address(field) + index * 2, value)
