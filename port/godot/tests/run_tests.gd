extends SceneTree

const OPTIONAL_SUITES: Array[String] = [
    "res://tests/core/run_core_tests.gd",
    "res://tests/core/run_state_digest_tests.gd",
    "res://tests/core/run_game_memory_tests.gd",
    "res://tests/core/run_rule_state_bridge_tests.gd",
    "res://tests/core/run_disc_pack_tests.gd",
    "res://tests/engine/run_engine_tests.gd",
    "res://tests/content/run_content_tests.gd",
    "res://tests/rules/run_rule_tests.gd",
    "res://tests/week/run_week_tests.gd",
    "res://tests/rules/run_route_state_parity_tests.gd",
]

static func _ensure_default_env() -> void:
    if OS.get_environment("TOKIMEMO_CONTENT_PACK").is_empty():
        var roots: Array[String] = []
        var env_root: String = OS.get_environment("TOKIMEMO_ROOT")
        if not env_root.is_empty():
            roots.append(env_root)
        roots.append("/repo")
        roots.append("/workspace")
        for root_dir: String in roots:
            var candidate: String = root_dir.path_join(".asset-cache/port/prologue-nbn-preview-v4-schema")
            if FileAccess.file_exists(candidate.path_join("manifest.json")):
                OS.set_environment("TOKIMEMO_CONTENT_PACK", candidate)
                break

func _initialize() -> void:
    call_deferred("_run")

func _run() -> void:
    _ensure_default_env()
    var failures: Array[String] = []
    var suite_count: int = 0
    var project_file: FileAccess = FileAccess.open("res://project.godot", FileAccess.READ)
    if project_file == null:
        failures.append("project.godot is not readable")
    else:
        var project_source: String = project_file.get_as_text()
        project_file.close()
        if not project_source.contains("GL Compatibility"):
            failures.append("project does not declare the GL Compatibility renderer")

    var shell_script: Script = load("res://shell.gd") as Script
    if shell_script == null or not shell_script.can_instantiate():
        failures.append("shell.gd did not compile into an instantiable script")

    var shell: PackedScene = load("res://shell.tscn") as PackedScene
    if shell == null:
        failures.append("original shell scene did not load")
    else:
        suite_count += 1
        print("RUN foundation synthetic shell check")
        var shell_node: Node = shell.instantiate()
        if shell_node.name != "PortFoundation":
            failures.append("shell root name changed")
        shell_node.free()

    for suite_path: String in OPTIONAL_SUITES:
        if not ResourceLoader.exists(suite_path):
            print("SKIP optional suite (not present): ", suite_path)
            continue
        suite_count += 1
        print("RUN optional suite: ", suite_path)
        var suite_script: Script = load(suite_path) as Script
        if suite_script == null or not suite_script.can_instantiate():
            failures.append("could not load suite: " + suite_path)
            continue
        var suite: Object = suite_script.new()
        if not suite.has_method("run"):
            failures.append("suite has no run() method: " + suite_path)
            continue
        var suite_failures: Array[String] = suite.run()
        failures.append_array(suite_failures)

    if failures.is_empty():
        print("PASS: %d test suite(s) ran" % suite_count)
        quit(0)
        return
    for failure: String in failures:
        push_error(failure)
    quit(1)
