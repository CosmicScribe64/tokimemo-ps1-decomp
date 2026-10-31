extends SceneTree
## T-39303: times InstalledLaunchProfile.read_pack on PROFILE_ROOT (best of 3, ms).
## Development measurement, not part of the suite:
##   PROFILE_ROOT=/bundle godot --headless --path /workspace --script res://tests/core/time_profile_open.gd

const Profile = preload("res://services/installed_launch_profile.gd")

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var root: String = OS.get_environment("PROFILE_ROOT")
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(root.path_join("manifest.json")))
	var best: int = -1
	var result: Dictionary = {}
	for attempt: int in 3:
		var start: int = Time.get_ticks_msec()
		result = Profile.read_pack(root, manifest if manifest is Dictionary else {})
		var elapsed: int = Time.get_ticks_msec() - start
		best = elapsed if best < 0 else mini(best, elapsed)
	print("read_pack valid=%s message=%s paths=%d gaps=%s best_ms=%d" % [result.get("valid"), result.get("message", ""), result.get("paths", {}).size(), result.get("optional_role_gaps", {}), best])
	quit(0 if result.get("valid", false) else 1)
