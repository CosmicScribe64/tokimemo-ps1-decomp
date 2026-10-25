extends "res://services/installed_pack_store.gd"
## Test-only validator for invented manifests; never used by the launcher.

func _validate_content_pack(pack_root: String) -> Dictionary:
	var manifest_path := pack_root.path_join("manifest.json")
	if not FileAccess.file_exists(manifest_path):
		return {"valid": false, "message": "synthetic fixture manifest is missing"}
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not manifest is Dictionary or manifest.get("synthetic_test_only") != true:
		return {"valid": false, "message": "test-only synthetic marker is missing"}
	return {"valid": true, "message": "synthetic workflow fixture only"}
