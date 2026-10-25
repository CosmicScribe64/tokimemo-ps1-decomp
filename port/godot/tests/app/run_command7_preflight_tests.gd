extends SceneTree
## T-29008: a complete rest route must be preflighted before confirmation.

const Harness = preload("res://tests/app/run_command7_app_tests.gd")
const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")

func _initialize() -> void:
	var app: Boot = Boot.new()
	var error: String = app._load_inputs()
	var checks: Dictionary = {}
	if OS.get_environment("TOKIMEMO_COMMAND7_NEGATIVE_KIND") == "broken":
		checks = {"preflight_rejected": not error.is_empty(), "no_session": app.session == null,
			"no_capabilities": app.command_resources.is_empty(), "no_renderer_staging": app.staged_commands.is_empty()}
	else:
		app.canvas = Canvas.new()
		app.pad = Pad.new()
		app.status_label = Label.new()
		app.detail_label = Label.new()
		if error.is_empty():
			error = app._reset()
		checks["v1_still_loads"] = error.is_empty()
		checks["only_command0_enabled"] = app.controller.command_capabilities == [0] and not app.command_resources.has(7)
		for tick: int in range(98):
			Harness.device_recipe(app, 7, tick)
			app._physics_process(1.0 / 60.0)
		var before: String = app.session.state_hash()
		app._physics_process(1.0 / 60.0)
		checks["command7_rejected"] = app.stopped and app.failure.contains("content capability is unavailable")
		checks["confirmation_rolls_back"] = app.session.state_hash() == before and app.state.game_mode == 0x34
		checks["no_resource_completion"] = app.pending_services.is_empty() and app.staged_services.is_empty()
		checks["no_render_fallback"] = app.canvas._commands.is_empty() and app.canvas._textures.is_empty()
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
	var ok: bool = true
	for value: bool in checks.values():
		ok = ok and value
	print(JSON.stringify({"kind": OS.get_environment("TOKIMEMO_COMMAND7_NEGATIVE_KIND"), "checks": checks, "pass": ok, "diagnostic": error}))
	app.free()
	quit(0 if ok else 1)
