extends RefCounted
## T-39301: the engine test suite. Runs every tests/engine/*_test.gd (each extends RefCounted and has
## `run() -> Array[String]` returning its failures), so a worker adds <c_file>_test.gd without editing a
## shared file. Fixtures are invented memory and an invented disc pack; the checks are written from the C
## source. Captures from the emulator are replayed by engine_diff_runner.gd (tools/port/run_engine_diff.sh).

func run() -> Array[String]:
	var failures: Array[String] = []
	var dir: DirAccess = DirAccess.open("res://tests/engine")
	if dir == null:
		failures.append("cannot open res://tests/engine")
		return failures
	var files: PackedStringArray = dir.get_files()
	files.sort()
	var suites: int = 0
	for file_name: String in files:
		var source: String = file_name.trim_suffix(".remap")
		if not source.ends_with("_test.gd"):
			continue
		var script: Script = load("res://tests/engine/" + source) as Script
		if script == null or not script.can_instantiate():
			failures.append("engine test does not compile: " + source)
			continue
		suites += 1
		print("  engine test: ", source)
		var test: Object = script.new()
		if not test.has_method("run"):
			failures.append("engine test has no run(): " + source)
			continue
		for failure: String in test.run():
			failures.append("%s: %s" % [source, failure])
	if suites == 0:
		failures.append("no engine tests found")
	return failures
