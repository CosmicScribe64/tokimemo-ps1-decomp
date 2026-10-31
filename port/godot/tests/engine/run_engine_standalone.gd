extends SceneTree
## Runs the engine suite alone (faster than the whole core suite): prints failures, exit code 1 on any.

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var suite = load("res://tests/engine/run_engine_tests.gd").new()
	var failures: Array[String] = suite.run()
	for failure: String in failures:
		printerr("FAIL: ", failure)
	print("ENGINE SUITE: ", "FAIL (%d)" % failures.size() if not failures.is_empty() else "PASS")
	quit(1 if not failures.is_empty() else 0)
