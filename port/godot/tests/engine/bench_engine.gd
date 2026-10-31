extends SceneTree
## T-39301: cost of call_c and of the accessors (not part of the suite). docker ... --script res://tests/engine/bench_engine.gd
const GameEngine = preload("res://engine/engine.gd")
const N: int = 100000

func _initialize() -> void:
	call_deferred("_run")

func _time(label: String, body: Callable) -> void:
	var start: int = Time.get_ticks_usec()
	body.call()
	var per: float = float(Time.get_ticks_usec() - start) / N
	print("%-46s %7.2f us/op" % [label, per])

func _run() -> void:
	var engine = GameEngine.new()
	var module: RefCounted = engine.modules.game_mode
	var gs = engine.memory.game_state()
	var address: int = engine.gs_address("mode_step")
	_time("empty loop", func() -> void:
		for i: int in N:
			pass)
	_time("call_c(mode_step_next)", func() -> void:
		for i: int in N:
			engine.call_c("mode_step_next"))
	_time("module.mode_step_next() direct", func() -> void:
		for i: int in N:
			module.mode_step_next())
	_time("call_c(cd_read_sync_status) (global read)", func() -> void:
		for i: int in N:
			engine.call_c("cd_read_sync_status"))
	_time("View: game_state().get_field(\"mode_step\")", func() -> void:
		for i: int in N:
			engine.memory.game_state().get_field("mode_step"))
	_time("View reused: gs.get_field(\"mode_step\")", func() -> void:
		for i: int in N:
			gs.get_field("mode_step"))
	_time("engine.gs_get(\"mode_step\") (cached row)", func() -> void:
		for i: int in N:
			engine.gs_get("mode_step"))
	_time("memory.u8(cached address)", func() -> void:
		for i: int in N:
			engine.memory.u8(address))
	_time("memory.read_global(\"cd_state\")", func() -> void:
		for i: int in N:
			engine.memory.read_global("cd_state"))
	engine.shutdown()
	quit(0)
