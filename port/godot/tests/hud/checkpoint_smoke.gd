extends SceneTree
## Bounded development checkpoint, not joined gameplay or acceptance evidence.
const Hud = preload("res://presentation/holiday_hud_adapter.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

func _initialize() -> void:
	var hud: Hud = Hud.new()
	var report: Dictionary = {"accepted": false, "checks": {}, "samples": {}}
	var opened: bool = hud.open(OS.get_environment("HUD_MANIFEST"), Hud.HUD_SHA,
		OS.get_environment("HUD_FONT_ROOT"), OS.get_environment("HOLIDAY_MEDIA"), Hud.MEDIA_MANIFEST_SHA,
		OS.get_environment("HOLIDAY_TEXT"), Hud.TEXT_SHA)
	report["checks"]["verified content opens"] = opened
	report["open_error"] = hud.error
	if opened:
		var raw_profile: Dictionary = integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("HUD_PROFILE"))))
		var profile: Dictionary = {}
		for key: String in ["family_name_hex", "icon_mode", "icon_count", "cursor_style"]:
			profile[key] = raw_profile[key]
		report["checks"]["genuine profile validates"] = hud.validate_profile(profile).is_empty()
		var room: Dictionary = hud.get_background("next_command", 16361)
		report["checks"]["public returned-room getter"] = not room.is_empty() and room["image_data"].get_size() == Vector2i(320, 88)
		var existing: Dictionary = integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("HOLIDAY_LIVE_PROOF"))))
		var run: Dictionary = existing["runs"][0]
		for point: Dictionary in run["points"]:
			var core: Dictionary = point["after"]
			var rule: Dictionary = core["rule_state"]
			var active: int = rule["background_cache"][rule["background_show"]]
			var name: String = ""
			if core["game_mode"] == 0x46 and core["mode_stage"] == 1 and active == 16318:
				name = "holiday"
			elif core["game_mode"] == 0x46 and core["mode_stage"] == 5 and active == 16379 and rule["actor_state"].is_empty():
				name = "activity"
			elif core["game_mode"] == 0x34 and core["mode_stage"] == 1 and active == 16361:
				name = "next_command"
			if name.is_empty() or report["samples"].has(name):
				continue
			var history: Array[Dictionary] = []
			for row: Dictionary in run["trace"]:
				if row["frame_count"] <= core["frame_count"]:
					history.append(row)
			var background: Dictionary = hud.get_background("room" if name == "holiday" else name, active)
			var commands: Array[Dictionary] = Canvas.background_commands(background["image_data"], background["id"])
			var composed: Dictionary = hud.compose(rule, core, profile, history, commands, active)
			report["samples"][name] = {"frame_count": core["frame_count"], "error": composed["error"], "retain": composed["retain"]}
			if composed["error"].is_empty() and not composed["retain"]:
				var image: Image = composed["commands"][0]["image"]
				report["samples"][name]["size"] = [image.get_width(), image.get_height()]
				report["samples"][name]["saved"] = image.save_png(OS.get_environment("HOLIDAY_CHECKPOINT_OUT").path_join(name + ".png")) == OK
	var output: FileAccess = FileAccess.open(OS.get_environment("HOLIDAY_CHECKPOINT_OUT").path_join("checkpoint-smoke.json"), FileAccess.WRITE)
	output.store_string(JSON.stringify(report, "\t"))
	output.close()
	print(JSON.stringify(report))
	quit(0 if opened else 1)

static func integers(value: Variant) -> Variant:
	if value is Dictionary:
		var result: Dictionary = {}
		for key: String in value:
			result[key] = integers(value[key])
		return result
	if value is Array:
		var result: Array = []
		for item: Variant in value:
			result.append(integers(item))
		return result
	return int(value) if value is float and value == floor(value) else value
