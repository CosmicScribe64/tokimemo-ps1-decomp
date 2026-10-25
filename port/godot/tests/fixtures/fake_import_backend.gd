extends RefCounted
## Synthetic event source and package writer for coordinator tests only.

const PROFILE := "original-ja-ps-the-best"

var events: Array[Dictionary] = []
var available: Array = []
var started := false
var cancel_requested := false
var manifest_sha256 := ""
var source_path := ""

func configure(next_events: Array[Dictionary], capabilities: Array) -> void:
	events = next_events.duplicate(true)
	available = capabilities.duplicate()
	started = false
	cancel_requested = false
	manifest_sha256 = ""

func begin_import(source: String, stage_path: String) -> String:
	if not FileAccess.file_exists(source):
		return "synthetic source file is missing"
	source_path = source
	if DirAccess.make_dir_recursive_absolute(stage_path) != OK:
		return "synthetic staging directory could not be created"
	var manifest := {"schema": "tokimemo.port-content.v1", "schema_version": 1,
		"release_profile": PROFILE, "synthetic_test_only": true,
		"route_capabilities": {"available": available.duplicate(), "unavailable": []}}
	var file := FileAccess.open(stage_path.path_join("manifest.json"), FileAccess.WRITE)
	if file == null:
		return "synthetic manifest could not be written"
	file.store_string(JSON.stringify(manifest) + "\n")
	file.flush()
	manifest_sha256 = FileAccess.get_sha256(stage_path.path_join("manifest.json"))
	for index: int in range(events.size()):
		if events[index].get("status") == "complete":
			events[index]["manifest_sha256"] = manifest_sha256
			events[index]["release_profile"] = PROFILE
	started = true
	return ""

func poll_event() -> Dictionary:
	return events.pop_front() if not events.is_empty() else {"status": "running", "phase": "import", "fraction": 0.7, "message": "synthetic wait"}

func cancel() -> bool:
	cancel_requested = true
	return true
