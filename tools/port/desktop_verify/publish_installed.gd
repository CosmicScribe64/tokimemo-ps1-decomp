extends SceneTree
## Run by the pinned Godot binary against the staged export project (export
## templates ignore --script). Publishes a developer content bundle that
## verify_desktop.sh copied into the per-user staging directory, through the same
## InstalledPackStore.publish_stage call the first-launch import flow uses, into
## the per-user root the exported player resolves (same project name).
## No pack, capability or readiness field is written here.
const Store = preload("res://services/installed_pack_store.gd")
const Profile = preload("res://services/installed_launch_profile.gd")

func _initialize() -> void:
	var store := Store.new()
	var expected := OS.get_environment("TOKIMEMO_VERIFY_STORE_ROOT")
	var stage := store.storage_root.path_join(".staging/prepared-original")
	var result := {"storage_root": store.storage_root, "user_data_dir": OS.get_user_data_dir()}
	if expected.is_empty() or store.storage_root != expected:
		result.error = "exported player resolved a different per-user content root"
	elif not FileAccess.file_exists(stage.path_join("manifest.json")):
		result.error = "staged bundle manifest is missing"
	else:
		var published := store.publish_stage(stage, FileAccess.get_sha256(stage.path_join("manifest.json")),
			Profile.RELEASE, [Profile.ENTRY_CAP, Profile.OPENING_RESOURCES_CAP])
		for key in ["valid", "launch_ready", "state", "message", "manifest_sha256"]:
			result[key] = published.get(key)
	print("TOKIMEMO_DESKTOP_PUBLISH " + JSON.stringify(result))
	quit(0 if result.get("valid", false) and result.get("launch_ready", false) else 1)
