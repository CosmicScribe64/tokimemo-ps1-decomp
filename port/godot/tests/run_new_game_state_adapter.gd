extends "res://tests/run_new_game_entry.gd"

const Adapter = preload("res://flow/new_game_state_adapter.gd")


func _initialize() -> void:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 123)

	var entry := Entry.new()
	entry.begin(invented_tables(), invented_content(), rng)
	finish_names(entry, rng, true)

	for kind in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)

	var before: PackedByteArray = entry.state_bytes.duplicate()
	var before_known: PackedByteArray = entry.known_bytes.duplicate()
	var result = Adapter.from_entry(entry, 1234, 5678, {"initial_mascot": 255})

	check(result.error.is_empty(), "committed entry materializes consumed fresh-route state: " + result.error)
	check(entry.state_bytes == before and entry.known_bytes == before_known, "adapter preserves independent raw bytes and ownership")

	if result.state != null:
		check(result.state.frame_count == 1234 and result.state.system_seed == 5678 and result.state.game_mode == 0x15, "explicit logical clock and source entry mode")
		check(not result.state.opening.command_ready and result.state.opening.audio_profile == "unset", "adapter cannot invent command readiness or audio")
		var checkpoint: Dictionary = result.state.projection()
		result.state.opening.screen_brightness = 88
		result.state.restore(checkpoint)
		check(result.state.projection() == checkpoint, "opening service state participates in transaction rollback")

	check(not Adapter.from_entry(entry, 0, 0, {}).error.is_empty(), "missing authenticated original mascot blocks route")

	entry.known_bytes[0xfc] = 0
	check(not Adapter.from_entry(entry, 0, 0, {"initial_mascot": 255}).error.is_empty(), "consumed unknown byte blocks route")

	print(("PASS" if failures == 0 else "FAIL") + ": %d fresh entry adapter checks" % checks)
	quit(0 if failures == 0 else 1)
