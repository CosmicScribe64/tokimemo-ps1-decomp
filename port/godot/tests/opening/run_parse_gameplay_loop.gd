extends SceneTree
## Parse check for the scripts changed by the gameplay-loop round (load only).

func _init() -> void:
	var failed := 0
	for path: String in ["res://app/first_arrival_boot.gd", "res://scenes/school_day_controller.gd",
			"res://presentation/command_reload_renderer.gd", "res://services/opening_audio_bridge.gd",
			"res://services/first_arrival_bgm_bridge.gd", "res://scenes/holiday_controller.gd",
			"res://services/holiday_scene_adapter.gd", "res://services/school_day_load_service.gd", "res://presentation/command_hud_presenter.gd",
			"res://services/first_arrival_launch_resources.gd", "res://tests/opening/run_installed_command_playtest.gd"]:
		var script: Script = load(path)
		if script == null or not script.can_instantiate():
			printerr("FAIL parse ", path)
			failed += 1
	print("PARSE ", "FAIL" if failed else "PASS")
	quit(1 if failed else 0)
