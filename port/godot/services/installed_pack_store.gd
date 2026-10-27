extends RefCounted
## Per-user receipt and immutable content-pack publication.

const ContentStoreScript = preload("res://services/content_store.gd")
const LaunchProfile = preload("res://services/installed_launch_profile.gd")
const RECEIPT_SCHEMA := "tokimemo.installed-content.v1"
const LOCATION_SCHEMA := "tokimemo.content-location.v1"
const CONTENT_SCHEMA := "tokimemo.port-content.v1"
const RELEASE_PROFILE := "original-ja-ps-the-best"
const MAX_RECEIPT_BYTES := 64 * 1024
const MAX_MANIFEST_BYTES := 16 * 1024 * 1024

var storage_root := ""
var error := ""

func _init(root_override: String = "") -> void:
	storage_root = root_override if not root_override.is_empty() else _preferred_storage_root()

func set_storage_root(path: String) -> bool:
	if not _safe_absolute(path):
		error = "install location must be a canonical external absolute directory without symlinks"
		return false
	var previous := storage_root
	storage_root = path
	if not _ensure_root():
		storage_root = previous
		return false
	if not _write_location_preference(path):
		storage_root = previous
		error = "selected content location could not be saved; the previous location remains active"
		return false
	error = ""
	return true

func _preferred_storage_root() -> String:
	var default_root := OS.get_user_data_dir().path_join("content-install")
	var preference := OS.get_user_data_dir().path_join("content-location.json")
	if not _safe_absolute(preference):
		return default_root
	var selected := _read_location_preference(preference)
	if not selected.is_empty():
		return selected
	# A crash after moving the old preference aside but before publishing its
	# replacement leaves the last committed location in `.previous`.
	selected = _read_location_preference(preference + ".previous")
	return selected if not selected.is_empty() else default_root

func _read_location_preference(path: String) -> String:
	if not FileAccess.file_exists(path) or _has_symlink(path):
		return ""
	var size := FileAccess.get_size(path)
	if size <= 0 or size > MAX_RECEIPT_BYTES:
		return ""
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "path"]) or parsed["schema"] != LOCATION_SCHEMA or not parsed["path"] is String or not _safe_absolute(parsed["path"]):
		return ""
	return parsed["path"]

func _write_location_preference(path: String) -> bool:
	var user_root := OS.get_user_data_dir()
	var preference := user_root.path_join("content-location.json")
	if not _safe_absolute(user_root) or _has_symlink(preference):
		return false
	if DirAccess.make_dir_recursive_absolute(user_root) != OK:
		return false
	var temp := preference + ".tmp-" + str(Time.get_ticks_usec())
	if _has_symlink(temp) or FileAccess.file_exists(temp) or DirAccess.dir_exists_absolute(temp):
		return false
	var file := FileAccess.open(temp, FileAccess.WRITE)
	if file == null:
		return false
	file.store_string(JSON.stringify({"schema": LOCATION_SCHEMA, "path": path}, "\t") + "\n")
	file.flush()
	var ok := file.get_error() == OK
	file.close()
	if not ok:
		DirAccess.remove_absolute(temp)
		return false
	var backup := preference + ".previous"
	if FileAccess.file_exists(preference):
		if FileAccess.file_exists(backup) and DirAccess.remove_absolute(backup) != OK:
			DirAccess.remove_absolute(temp)
			return false
		if DirAccess.rename_absolute(preference, backup) != OK:
			DirAccess.remove_absolute(temp)
			return false
	if DirAccess.rename_absolute(temp, preference) != OK:
		if FileAccess.file_exists(backup):
			DirAccess.rename_absolute(backup, preference)
		DirAccess.remove_absolute(temp)
		return false
	if FileAccess.file_exists(backup):
		DirAccess.remove_absolute(backup)
	return true

func check_installed(required_capabilities: Array = []) -> Dictionary:
	error = ""
	if not _ensure_root():
		return _failure(error)
	var receipt_path := storage_root.path_join("installed-content.json")
	var backup_path := receipt_path + ".previous"
	if not FileAccess.file_exists(receipt_path) and FileAccess.file_exists(backup_path):
		if DirAccess.rename_absolute(backup_path, receipt_path) != OK:
			return _failure("previous install receipt could not be recovered")
	if not FileAccess.file_exists(receipt_path):
		return {"state": "needs_source", "launch_ready": false, "message": "Choose the original game to import."}
	var receipt := _read_receipt(receipt_path)
	if not receipt.get("error", "").is_empty() and FileAccess.file_exists(backup_path):
		var quarantine := receipt_path + ".invalid"
		if FileAccess.file_exists(quarantine):
			DirAccess.remove_absolute(quarantine)
		if DirAccess.rename_absolute(receipt_path, quarantine) == OK and DirAccess.rename_absolute(backup_path, receipt_path) == OK:
			DirAccess.remove_absolute(quarantine)
			receipt = _read_receipt(receipt_path)
	if not receipt.get("error", "").is_empty():
		return _failure("installed content receipt is invalid: " + receipt["error"])
	receipt = receipt["receipt"]
	var pack_root := storage_root.path_join("packs").path_join(receipt["pack_id"])
	var validated := validate_pack(pack_root, required_capabilities)
	if not validated.get("valid", false):
		return _failure("installed content pack failed validation: " + validated.get("message", "unknown error"))
	if validated["manifest_sha256"] != receipt["manifest_sha256"] or validated["release_profile"] != receipt["release_profile"]:
		return _failure("installed content no longer matches its receipt")
	validated["pack_id"] = receipt["pack_id"]
	validated["pack_root"] = pack_root
	validated["state"] = "ready" if validated["launch_ready"] else "installed_incomplete"
	validated["message"] = "Installed content is valid." if validated["launch_ready"] else _missing_message(validated["missing_capabilities"])
	return validated

func create_stage_path(operation_id: String) -> String:
	if not _safe_id(operation_id) or not _ensure_root():
		error = "staging operation id or per-user storage root is invalid"
		return ""
	var parent := storage_root.path_join(".staging")
	if _has_symlink(parent):
		error = "per-user staging directory is linked"
		return ""
	if DirAccess.make_dir_recursive_absolute(parent) != OK or _has_symlink(parent):
		error = "per-user staging directory is unavailable or linked"
		return ""
	var stage := parent.path_join(operation_id)
	if DirAccess.dir_exists_absolute(stage) or FileAccess.file_exists(stage) or _has_symlink(stage):
		error = "staging directory already exists"
		return ""
	error = ""
	return stage

func validate_pack(pack_root: String, required_capabilities: Array = []) -> Dictionary:
	error = ""
	if not _safe_absolute(pack_root) or not DirAccess.dir_exists_absolute(pack_root):
		return _failure("pack path must be an existing external absolute directory without symlinks")
	var manifest_path := pack_root.path_join("manifest.json")
	if _has_symlink(manifest_path) or not FileAccess.file_exists(manifest_path):
		return _failure("pack manifest is missing or linked")
	var size := FileAccess.get_size(manifest_path)
	if size <= 0 or size > MAX_MANIFEST_BYTES:
		return _failure("pack manifest size is outside bounds")
	var manifest_sha := FileAccess.get_sha256(manifest_path)
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary:
		return _failure("installed manifest is not a dictionary")
	if parsed.get("schema") == LaunchProfile.SCHEMA:
		return _validate_runtime(pack_root, parsed, manifest_sha, required_capabilities)
	if parsed.get("schema") != CONTENT_SCHEMA or parsed.get("schema_version") != 1:
		return _failure("unsupported installed content schema")
	if parsed.get("release_profile") != RELEASE_PROFILE:
		return _failure("this build does not support the content release profile")
	var capabilities: Variant = parsed.get("route_capabilities")
	if not capabilities is Dictionary or not capabilities.get("available") is Array or not capabilities.get("unavailable") is Array:
		return _failure("content manifest route capabilities are malformed")
	var validation := _validate_content_pack(pack_root)
	if not validation.get("valid", false):
		return _failure(validation.get("message", "content validation failed"))
	var available: Array = capabilities["available"]
	var unavailable: Array = capabilities["unavailable"]
	if available.has(LaunchProfile.ENTRY_CAP) or available.has(LaunchProfile.GAMEPLAY_CAP):
		return _failure("a legacy static-preview pack cannot authorize New Game")
	var required := _clean_capabilities(required_capabilities)
	if required.is_empty() and not required_capabilities.is_empty():
		return _failure("required launch capability list is malformed")
	var missing: Array[String] = []
	for capability: String in required:
		if not available.has(capability):
			missing.append(capability)
	return {"valid": true, "launch_ready": not required.is_empty() and missing.is_empty(),
		"missing_capabilities": missing, "available_capabilities": available.duplicate(),
		"unavailable_capabilities": unavailable.duplicate(), "manifest_sha256": manifest_sha,
		"release_profile": RELEASE_PROFILE, "message": ""}

func _validate_runtime(pack_root: String, manifest: Dictionary, manifest_sha: String, required_capabilities: Array) -> Dictionary:
	var profile: Dictionary = LaunchProfile.read_pack(pack_root, manifest)
	if not profile.get("valid", false):
		return _failure(str(profile.get("message", "installed runtime validation failed")))
	var required := _clean_capabilities(required_capabilities)
	if required.is_empty() and not required_capabilities.is_empty():
		return _failure("required launch capability list is malformed")
	var missing: Array[String] = []
	for capability: String in required:
		if capability not in profile.available_capabilities:
			missing.append(capability)
	return {"valid": true, "launch_ready": not required.is_empty() and missing.is_empty(),
		"missing_capabilities": missing, "available_capabilities": profile.available_capabilities.duplicate(),
		"unavailable_capabilities": profile.unavailable_capabilities.duplicate(), "manifest_sha256": manifest_sha,
		"release_profile": RELEASE_PROFILE, "message": "", "launch_profile": profile}

func publish_stage(stage_path: String, expected_manifest_sha256: String,
		release_profile: String, required_capabilities: Array = []) -> Dictionary:
	error = ""
	var staging_root := storage_root.path_join(".staging")
	if not _safe_absolute(stage_path) or not stage_path.begins_with(staging_root + "/") or stage_path.trim_prefix(staging_root + "/").contains("/"):
		return _failure("import output must be one direct child of the per-user staging directory")
	if release_profile != RELEASE_PROFILE:
		return _failure("imported release profile is not supported by this build")
	var checked := validate_pack(stage_path, required_capabilities)
	if not checked.get("valid", false):
		return checked
	if checked["manifest_sha256"] != expected_manifest_sha256:
		return _failure("import completion manifest identity differs from validated bytes")
	var pack_id := RELEASE_PROFILE + "-" + expected_manifest_sha256.substr(0, 16)
	var packs_root := storage_root.path_join("packs")
	if _has_symlink(packs_root):
		return _failure("per-user pack directory is linked")
	if DirAccess.make_dir_recursive_absolute(packs_root) != OK or _has_symlink(packs_root):
		return _failure("per-user pack directory is unavailable or linked")
	var destination := packs_root.path_join(pack_id)
	if DirAccess.dir_exists_absolute(destination):
		var existing := validate_pack(destination, required_capabilities)
		if not existing.get("valid", false) or existing["manifest_sha256"] != expected_manifest_sha256:
			return _failure("versioned pack destination exists with different or invalid content")
		if not _remove_tree(stage_path):
			return _failure("duplicate staging output could not be removed")
		checked = existing
	else:
		if FileAccess.file_exists(destination) or _has_symlink(destination):
			return _failure("versioned pack destination is occupied or linked")
		if DirAccess.rename_absolute(stage_path, destination) != OK:
			return _failure("validated pack could not be atomically published")
	# Absolute launch locators must point at the immutable published location,
	# never the staging directory that was just renamed.
	checked = validate_pack(destination, required_capabilities)
	if not checked.get("valid", false) or checked.get("manifest_sha256") != expected_manifest_sha256:
		return _failure("published content changed before receipt commit")
	checked["pack_id"] = pack_id
	checked["pack_root"] = destination
	var previous := check_installed(required_capabilities)
	if previous.get("state") == "ready" and not checked["launch_ready"]:
		previous["message"] = "New content lacked required capabilities; the previous launch-ready pack remains selected."
		previous["candidate_pack_id"] = pack_id
		previous["candidate_missing_capabilities"] = checked["missing_capabilities"].duplicate()
		return previous
	var receipt := {"schema": RECEIPT_SCHEMA, "version": 1, "release_profile": RELEASE_PROFILE,
		"pack_id": pack_id, "manifest_sha256": expected_manifest_sha256}
	if not _write_receipt(receipt):
		return _failure("new pack is published but its install receipt could not be committed; the previous receipt remains available")
	checked["state"] = "ready" if checked["launch_ready"] else "installed_incomplete"
	checked["message"] = "Original data is installed and validated." if checked["launch_ready"] else _missing_message(checked["missing_capabilities"])
	return checked

func discard_stage(stage_path: String) -> bool:
	var staging_root := storage_root.path_join(".staging")
	if not stage_path.begins_with(staging_root + "/") or stage_path.trim_prefix(staging_root + "/").contains("/") or _has_symlink(stage_path):
		return false
	return _remove_tree(stage_path)

func _validate_content_pack(pack_root: String) -> Dictionary:
	var validator = ContentStoreScript.new()
	if not validator.open_pack(pack_root):
		return {"valid": false, "message": validator.error_message()}
	return {"valid": true, "message": ""}

func _write_receipt(receipt: Dictionary) -> bool:
	var receipt_path := storage_root.path_join("installed-content.json")
	var backup_path := receipt_path + ".previous"
	var temp_path := receipt_path + ".tmp-" + str(Time.get_ticks_usec())
	if _has_symlink(temp_path) or FileAccess.file_exists(temp_path) or DirAccess.dir_exists_absolute(temp_path):
		return false
	var file := FileAccess.open(temp_path, FileAccess.WRITE)
	if file == null:
		return false
	file.store_string(JSON.stringify(receipt, "\t") + "\n")
	file.flush()
	var write_ok := file.get_error() == OK
	file.close()
	if not write_ok:
		DirAccess.remove_absolute(temp_path)
		return false
	var had_receipt := FileAccess.file_exists(receipt_path)
	if had_receipt:
		if FileAccess.file_exists(backup_path) and DirAccess.remove_absolute(backup_path) != OK:
			DirAccess.remove_absolute(temp_path)
			return false
		if DirAccess.rename_absolute(receipt_path, backup_path) != OK:
			DirAccess.remove_absolute(temp_path)
			return false
	if DirAccess.rename_absolute(temp_path, receipt_path) != OK:
		if had_receipt:
			DirAccess.rename_absolute(backup_path, receipt_path)
		DirAccess.remove_absolute(temp_path)
		return false
	if had_receipt:
		DirAccess.remove_absolute(backup_path)
	return true

func _read_receipt(path: String) -> Dictionary:
	if _has_symlink(path) or FileAccess.get_size(path) <= 0 or FileAccess.get_size(path) > MAX_RECEIPT_BYTES:
		return {"error": "receipt is linked, empty, or oversized"}
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "version", "release_profile", "pack_id", "manifest_sha256"]):
		return {"error": "receipt fields are malformed"}
	if parsed["schema"] != RECEIPT_SCHEMA or parsed["version"] != 1 or parsed["release_profile"] != RELEASE_PROFILE or not _safe_id(str(parsed["pack_id"])) or not _sha(parsed["manifest_sha256"]):
		return {"error": "receipt identity is unsupported"}
	if parsed["pack_id"] != RELEASE_PROFILE + "-" + str(parsed["manifest_sha256"]).substr(0, 16):
		return {"error": "receipt pack identity does not match its manifest hash"}
	return {"error": "", "receipt": parsed}

func _recover_receipt() -> void:
	var receipt := storage_root.path_join("installed-content.json")
	var backup := receipt + ".previous"
	if not FileAccess.file_exists(receipt) and FileAccess.file_exists(backup):
		DirAccess.rename_absolute(backup, receipt)

func _ensure_root() -> bool:
	if not _safe_absolute(storage_root):
		error = "application data root must be an external canonical absolute path"
		return false
	var raw_project := ProjectSettings.globalize_path("res://")
	if not raw_project.is_empty():
		var project_root := raw_project.simplify_path()
		if project_root != "/" and DirAccess.dir_exists_absolute(project_root):
			if storage_root == project_root or storage_root.begins_with(project_root.trim_suffix("/") + "/"):
				error = "persistent content must live outside the application project"
				return false
	if DirAccess.make_dir_recursive_absolute(storage_root) != OK or _has_symlink(storage_root):
		error = "persistent per-user content directory could not be created safely"
		return false
	return true

func _safe_absolute(path: String) -> bool:
	if not path.is_absolute_path() or path.begins_with("//") or path.contains("\\") or path != path.simplify_path() or path.contains("res://"):
		return false
	var path_parts := path.trim_prefix("/")
	if path.length() >= 3 and path.substr(1, 1) == ":" and path.substr(2, 1) == "/":
		path_parts = path.substr(3)
	for part: String in path_parts.split("/"):
		if part.is_empty() or part == "." or part == "..":
			return false
	return not _has_symlink(path)

func _has_symlink(path: String) -> bool:
	var current := "/"
	var path_parts := path.trim_prefix("/")
	if path.length() >= 3 and path.substr(1, 1) == ":" and path.substr(2, 1) == "/":
		current = path.substr(0, 3)
		path_parts = path.substr(3)
	for part: String in path_parts.split("/"):
		if part.is_empty():
			continue
		current = current.path_join(part)
		var dir := DirAccess.open(current.get_base_dir())
		if dir != null and dir.is_link(current.get_file()):
			return true
	return false

func _remove_tree(path: String) -> bool:
	if _has_symlink(path):
		return DirAccess.remove_absolute(path) == OK
	if DirAccess.dir_exists_absolute(path):
		var directory := DirAccess.open(path)
		if directory == null:
			return false
		directory.include_hidden = true
		for name: String in directory.get_files():
			if DirAccess.remove_absolute(path.path_join(name)) != OK:
				return false
		for name: String in directory.get_directories():
			if not _remove_tree(path.path_join(name)):
				return false
		return DirAccess.remove_absolute(path) == OK
	if FileAccess.file_exists(path):
		return DirAccess.remove_absolute(path) == OK
	return true

func _clean_capabilities(value: Array) -> Array[String]:
	var result: Array[String] = []
	for item: Variant in value:
		if not item is String or item.is_empty() or result.has(item):
			return []
		result.append(item)
	return result

func _missing_message(missing: Array) -> String:
	if missing.is_empty():
		return "Content is validated, but this application has not declared the capabilities required to launch."
	return "Content is installed, but gameplay is unavailable until required content is imported: " + ", ".join(missing)

func _safe_id(value: String) -> bool:
	if value.is_empty() or value.length() > 128:
		return false
	for character: String in value:
		if not "abcdefghijklmnopqrstuvwxyz0123456789-".contains(character):
			return false
	return true

func _sha(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if not "0123456789abcdef".contains(character):
			return false
	return true

func _keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key: String in expected:
		if not value.has(key):
			return false
	return true

func _failure(message: String) -> Dictionary:
	error = message
	return {"state": "failed", "valid": false, "launch_ready": false, "message": message}
