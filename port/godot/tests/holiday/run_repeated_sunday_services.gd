extends "res://tests/holiday/run_real_week2_services.gd"
## T-29026. Continuous real-input spring Week2 through second Sunday/return.
## Any intervening overlay is a separate unresolved route, never an auto-skip oracle.

func _run() -> void:
	_ensure_default_env()
	var harness := Harness.new()
	OS.set_environment("TOKIMEMO_MULTI_WEEK", "1")
	var app: Boot = harness.make_app()
	app.continue_week = true
	var live_hud: Variant = app.hud
	check(harness.failures.is_empty(), "original repeated-week preflight: " + str(harness.failures))
	var menus: Array = []
	var menu_id: String = ""
	var entered: int = -1
	var release: int = -1
	var actors: Dictionary = {}
	var finished: bool = false
	for _iteration: int in range(1600):
		var tick: int = app.session.clock.logic_iterations
		var data: Dictionary = app.state.rule_state
		if tick == release:
			key(app, false)
		if app.state.game_mode in [0x34, 0x46] and app.state.mode_stage == 1 and app.state.mode_step == 0:
			var identity: Array = [data["year"], data["month"], data["day"], app.state.game_mode, data["day_event_index"]]
			if JSON.stringify(identity) != menu_id:
				menu_id = JSON.stringify(identity)
				entered = tick
				menus.append({"tick": tick, "identity": identity, "pixels": app.canvas.get("_commands")[0]["id"]})
			if menus.size() == 5:
				finished = true
				break
			if tick == entered + 32:
				key(app, true)
				release = tick + 2
		app.hud = null if (tick > 0 and app.state.game_mode == 0x40) else live_hud
		app._physics_process(1.0 / 60.0)
		app.hud = live_hud
		if not app.failure.is_empty():
			check(false, "repeated-week app failed at%d: %s" % [tick, app.failure])
			break
		if app.state.game_mode not in [0x30, 0x31, 0x32, 0x34, 0x40, 0x46]:
			check(false, "actual queue requires separately owned overlay0x%x" % app.state.game_mode)
			break
		if data["actor_state"].has("96"):
			var actor: Dictionary = data["actor_state"]["96"]
			var actor_id: String = JSON.stringify([data["month"], data["day"], app.state.game_mode, data["day_event_index"]])
			if not actors.has(actor_id):
				actors[actor_id] = {"slot": actor["sequence_slot"], "frames": 0}
			actors[actor_id]["frames"] += 1
	check(finished and menus[-1]["identity"].slice(0, 3) == [95, 4, 16], "full ordinary Week2 and source Sunday activity return on April16")
	if menus.size() >= 4:
		check(menus[3]["identity"][4] > 9, "second Sunday follows its actual longer queue")
	var file := FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("repeated-sunday-services.json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify({"checks": checks, "failures": failures, "menus": menus, "actors": actors,
			"state": app.state.projection(), "rng": app.session.rng.checkpoint(), "trace": app.trace}, "\t"))
		file.close()
	print(JSON.stringify({"checks": checks, "failures": failures, "menus": menus, "actors": actors}))
	harness.dispose(app)
	harness.free()
	quit(0 if failures.is_empty() else 1)
