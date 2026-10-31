extends "res://tests/services/settings_test_tree.gd"
## T-39230: launch options and the test gate on the real ModernSettings
## autoload. PARITY_LAUNCH_EXPECT names what this launch must show:
##   forced-env   TOKIMEMO_PARITY=1           -> forced, gate passes
##   forced-arg   "-- --parity"               -> forced, gate passes
##   modern       no option                   -> not forced, gate fails
##   invariance   MODERN_INVARIANCE=1 only    -> not forced, gate passes as invariance
## tools/port/run_parity_mode_tests.sh runs all four.
const Parity = preload("res://services/parity_mode.gd")

func _initialize() -> void: call_deferred("run")

func run() -> void:
	var expect := OS.get_environment("PARITY_LAUNCH_EXPECT")
	var settings: Node = root.get_node("ModernSettings")
	var forced: String = settings.preferences.parity_forced_by
	var out := "user://parity-launch-%s" % expect
	DirAccess.make_dir_recursive_absolute(out)
	var gate := Parity.gate(self, out)
	var written = JSON.parse_string(FileAccess.get_file_as_string(out.path_join(Parity.IDENTITY_FILE)))
	check(written is Dictionary and written.gate == ("passed" if gate.ok else "failed"))
	match expect:
		"forced-env":
			check(forced == Parity.ENV_FORCE and gate.ok and gate.identity.play_mode == "parity")
		"forced-arg":
			check(forced == Parity.ARG_FORCE and gate.ok and gate.identity.play_mode == "parity")
		"modern":
			check(forced.is_empty() and not gate.ok and gate.error.contains("not active"))
			# A QA route (not evidence) runs in Modern, and its record says so.
			var qa_out := out + "-qa"; DirAccess.make_dir_recursive_absolute(qa_out)
			var qa := Parity.gate(self, qa_out, false)
			var qa_written = JSON.parse_string(FileAccess.get_file_as_string(qa_out.path_join(Parity.IDENTITY_FILE)))
			check(qa.ok and qa_written is Dictionary and qa_written.gate == Parity.GATE_NOT_EVIDENCE)
		"invariance":
			check(forced.is_empty() and gate.ok and gate.identity.play_mode == "modern" and gate.identity.invariance_run)
		_:
			printerr("PARITY_LAUNCH_EXPECT must be forced-env, forced-arg, modern or invariance"); check(false)
	if expect.begins_with("forced"):
		# The forced session refuses enhancements and never writes the saved mode.
		check(not settings.preferences.set_value("accessibility", "text_speed", "fast"))
		check(not settings.preferences.set_mode("modern"))
		# A violation slipped in by hand fails the gate even though Parity is on.
		settings.preferences.settings.display.aspect = "wide"
		var dirty := Parity.gate(self)
		check(not dirty.ok and dirty.error.contains("display.aspect"))
		settings.preferences.settings.display.aspect = "original"
	print("Parity launch %s: forced_by '%s', gate %s passed" % [expect, forced, "ok" if gate.ok else "refused"])
	finish()
