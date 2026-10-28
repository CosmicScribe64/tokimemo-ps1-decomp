extends SceneTree
const Player = preload("res://overlays/shooter_player_rules.gd")
const Primary = preload("res://overlays/shooter_primary_rules.gd")
const Collision = preload("res://overlays/shooter_collision_rules.gd")
const Sound = preload("res://overlays/shooter_sound_rules.gd")
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const Rng = preload("res://core/native_rng.gd")
const Tables = preload("res://services/shooter_combat_tables.gd")
var failures: Array[String] = []
var reports: Array[Dictionary] = []
var rng: Rng
var tables: Dictionary

func integers(value: Variant) -> Variant:
	if value is float: return int(value)
	if value is Array:
		for i: int in value.size(): value[i] = integers(value[i])
	elif value is Dictionary:
		for key: Variant in value: value[key] = integers(value[key])
	return value

func sound(state: Dictionary, channel: int, command: int) -> void:
	var result: Dictionary = Sound.play_se(state, channel, command, tables, rng)
	if not result["error"].is_empty(): failures.append(result["error"])

func _initialize() -> void:
	var fixture: Dictionary = integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TACO_NATIVE_FIXTURE"))))
	var loader: Tables = Tables.new()
	var error: String = loader.open_manifest(OS.get_environment("TACO_COMBAT_TABLES"))
	if not error.is_empty():
		push_error(error)
		quit(1)
		return
	tables = loader.tables
	for item: Dictionary in fixture["cases"]:
		var state: Dictionary = item["before"].duplicate(true)
		var expected: Dictionary = item["after"]
		var input: Dictionary = item["case"]
		rng = Rng.new()
		rng.configure(Rng.OPENBIOS_PROFILE, item["rng_before"])
		var result: Dictionary = {"error":"", "value":0}
		var fields: Array[String] = []
		match input["function"]:
			"shooter_update_player":
				result = Player.update_player(state, input["held"], input["pressed"])
				fields = ["actors"]
			"shooter_fire_vulcan", "shooter_fire_wide_shot", "shooter_fire_beam":
				if input["function"] == "shooter_fire_vulcan":
					result = Primary.fire_vulcan(state, input["args"][0], input["args"][1], tables, rng)
				elif input["function"] == "shooter_fire_wide_shot":
					result = Primary.fire_wide(state, input["args"][0], input["args"][1], tables, rng)
				else:
					result = Primary.fire_beam(state, input["args"][0], input["args"][1], tables, rng)
				fields = ["actors", "player_primary", "wingman_primary", "se_cooldowns"]
				if result["value"] != item["return_value"]: failures.append(input["id"] + ": native return differs")
			"shooter_hit_ships_with_bullets":
				result["error"] = Collision.hit_ships_with_bullets(state, sound)
				fields = ["actors", "enemy_bullets", "hit_sound_played", "se_cooldowns"]
			"shooter_check_battle_end":
				result["error"] = Lifecycle.check_battle_end(state, input["pressed"])
				fields = ["time_left", "mode_stage", "mode_step", "mode_stage_timer", "mode_step_timer", "mode_stage_scratch", "substep"]
			"shooter_result_step_compute":
				result["error"] = Lifecycle.compute_result(state)
				fields = ["score", "result_values", "clear_color", "mode_step", "mode_step_timer", "substep"]
			_: result["error"] = "unmapped original function"
		var differences: Array[String] = []
		if not result["error"].is_empty(): differences.append(result["error"])
		for field: String in fields:
			if state[field] != expected[field]: differences.append(field)
		if rng.state != item["rng_after"]: differences.append("rng")
		# Compare actual audio commands, not native C's undefined return values.
		var actual_audio: Array = []
		for event: Dictionary in state["effects"]:
			if event.get("kind") == "audio_command": actual_audio.append(event["command"])
			elif event.get("source") == "audio_command": actual_audio.append(event["args"][0])
		var expected_audio: Array = []
		for event: Dictionary in item["services"]:
			if event["name"] == "audio_command": expected_audio.append(event["a0"])
		if actual_audio != expected_audio: differences.append("audio_command_order")
		if not differences.is_empty(): failures.append(input["id"] + ": " + str(differences))
		reports.append({"id":input["id"], "function":input["function"], "compared_fields":fields, "differences":differences})
	var output: FileAccess = FileAccess.open(OS.get_environment("TACO_NATIVE_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify({"cases":reports.size(), "failures":failures, "reports":reports,
		"scope":"original-instruction function probes; fixture exclusions apply"}, "\t"))
	output.close()
	print(JSON.stringify({"cases":reports.size(), "failures":failures}))
	quit(0 if failures.is_empty() else 1)
