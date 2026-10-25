extends RefCounted
## Orchestrates first launch without coupling source import to game rules.

const StoreScript = preload("res://services/installed_pack_store.gd")
const PHASE_VERIFY := "verify"
const PHASE_IMPORT := "import"

var state := "checking"
var progress := 0.0
var status_message := "Checking installed content."
var error := ""
var latest_result: Dictionary = {}
var store: RefCounted
var backend: Object
var _source_path := ""
var _stage_path := ""
var _required_capabilities: Array = []
var _pending_failure := ""

func _init(store_override: RefCounted = null, backend_override: Object = null) -> void:
	store = store_override if store_override != null else StoreScript.new()
	backend = backend_override

func check_existing(required_capabilities: Array) -> Dictionary:
	_required_capabilities = required_capabilities.duplicate()
	var result: Dictionary = store.check_installed(_required_capabilities)
	latest_result = result.duplicate(true)
	state = str(result.get("state", "failed"))
	status_message = str(result.get("message", "Installed content status is unknown."))
	error = str(result.get("message", "")) if state == "failed" else ""
	progress = 1.0 if state == "ready" else 0.0
	return result

func set_storage_location(path: String) -> Dictionary:
	if state in ["verifying", "importing", "cancelling", "validating", "publishing"]:
		return {"state": state, "valid": false, "launch_ready": false, "message": "content location cannot change during import"}
	if not store.set_storage_root(path):
		return {"state": "failed", "valid": false, "launch_ready": false, "message": str(store.error)}
	return check_existing(_required_capabilities)

func choose_source(source_path: String, required_capabilities: Array) -> String:
	if state in ["verifying", "importing", "cancelling", "validating", "publishing"]:
		return "Game setup is already running."
	if backend == null or not backend.has_method("begin_import") or not backend.has_method("poll_event") or not backend.has_method("cancel"):
		state = "importer_unavailable"
		status_message = "This build has no packaged original-game importer. No source files were changed."
		error = status_message
		return error
	if not _safe_source(source_path):
		return _fail("choose an existing absolute source file or folder without symlink components")
	if _source_overlaps_storage(source_path):
		return _fail("selected source overlaps the content location; choose a different source or location")
	_source_path = source_path
	_required_capabilities = required_capabilities.duplicate()
	var operation_id := "import-" + str(Time.get_ticks_usec())
	_stage_path = store.create_stage_path(operation_id)
	if _stage_path.is_empty():
		return _fail(str(store.error))
	var begin_error: Variant = backend.begin_import(_source_path, _stage_path)
	if not begin_error is String or not begin_error.is_empty():
		store.discard_stage(_stage_path)
		return _fail(str(begin_error) if begin_error is String else "packaged importer returned an invalid start response")
	progress = 0.0
	error = ""
	state = "verifying"
	status_message = "Checking the selected PS1 release and required source files."
	return ""

func tick() -> void:
	if state not in ["verifying", "importing", "cancelling"]:
		return
	var event: Variant = backend.poll_event()
	if not event is Dictionary or not event.has("status"):
		_request_abort("packaged importer emitted a malformed progress event")
		return
	var event_status: String = str(event["status"])
	if state == "cancelling":
		if event_status == "running":
			status_message = "Cancelling import; the previous installed pack remains available."
			return
		if event_status not in ["cancelled", "failed", "complete"]:
			status_message = "Waiting for the importer to stop; staging output is retained."
			return
		store.discard_stage(_stage_path)
		if event_status == "cancelled":
			if _pending_failure.is_empty():
				state = "cancelled"
				status_message = "Import cancelled. The previous installed pack was preserved."
				error = ""
			else:
				state = "failed"
				error = _pending_failure
				status_message = error + ". The previous installed pack was preserved."
		else:
			state = "failed"
			status_message = "Import stopped during cancellation; the previous installed pack was preserved."
			error = _pending_failure if not _pending_failure.is_empty() else str(event.get("message", status_message))
			if not _pending_failure.is_empty():
				status_message = _pending_failure + ". The previous installed pack was preserved."
		_stage_path = ""
		_pending_failure = ""
		return
	if event_status == "running":
		if not _keys(event, ["status", "phase", "fraction", "message"]):
			_request_abort("packaged importer progress fields differ from the protocol")
			return
		var fraction: Variant = event["fraction"]
		if not fraction is float and not fraction is int:
			_request_abort("packaged importer progress must be numeric")
			return
		if not is_finite(float(fraction)) or float(fraction) < progress or float(fraction) < 0.0 or float(fraction) > 1.0:
			_request_abort("packaged importer progress moved outside the monotonic 0–1 range")
			return
		if not event["phase"] is String or not event["message"] is String or str(event["message"]).length() > 512:
			_request_abort("packaged importer progress text fields are invalid")
			return
		var phase: String = event["phase"]
		if phase == PHASE_VERIFY:
			if state == "importing":
				_request_abort("packaged importer regressed from import to verification")
				return
			state = "verifying"
			status_message = "Verifying original release: " + event["message"]
		elif phase == PHASE_IMPORT:
			state = "importing"
			status_message = "Importing and validating content: " + event["message"]
		else:
			_request_abort("packaged importer emitted an unknown stage")
			return
		progress = min(float(fraction), 0.99)
		return
	if event_status == "cancelled":
		store.discard_stage(_stage_path)
		_stage_path = ""
		state = "failed"
		status_message = "Importer stopped unexpectedly; the previous installed pack was preserved."
		error = "importer reported cancellation without a user request"
		return
	if event_status == "failed":
		store.discard_stage(_stage_path)
		_stage_path = ""
		var failure_message: String = _pending_failure if not _pending_failure.is_empty() else str(event.get("message", "source verification or import failed"))
		if failure_message.is_empty() or failure_message.length() > 1024:
			failure_message = "source verification or import failed"
		_fail(failure_message)
		_pending_failure = ""
		return
	if event_status == "complete" and not _keys(event, ["status", "release_profile", "manifest_sha256"]):
		store.discard_stage(_stage_path)
		_stage_path = ""
		_fail("packaged importer completion record is malformed")
		return
	if event_status != "complete":
		_request_abort("packaged importer emitted an unknown event state")
		return
	state = "validating"
	status_message = "Validating imported components before publication."
	state = "publishing"
	var published: Dictionary = store.publish_stage(_stage_path, str(event["manifest_sha256"]),
		str(event["release_profile"]), _required_capabilities)
	latest_result = published.duplicate(true)
	if not published.get("valid", false):
		store.discard_stage(_stage_path)
		_stage_path = ""
		state = "failed"
		error = str(published.get("message", "installed pack validation failed"))
		status_message = error + ". The previous installed pack was preserved."
		return
	_stage_path = ""
	progress = 1.0
	state = str(published.get("state", "installed_incomplete"))
	status_message = str(published.get("message", "Content was installed."))
	error = ""

func cancel() -> String:
	if state not in ["verifying", "importing"]:
		return _fail("there is no active import to cancel")
	var cancel_result: Variant = backend.cancel()
	if not cancel_result is bool or not cancel_result:
		return "packaged importer could not accept cancellation"
	_pending_failure = ""
	state = "cancelling"
	status_message = "Cancelling import; the previous installed pack remains available."
	return ""

func _safe_source(path: String) -> bool:
	if not path.is_absolute_path() or path.begins_with("//") or path.contains("\\") or path != path.simplify_path() or path.contains("res://"):
		return false
	if not FileAccess.file_exists(path) and not DirAccess.dir_exists_absolute(path):
		return false
	return not _has_symlink(path)

func _source_overlaps_storage(source_path: String) -> bool:
	var content_root: String = str(store.storage_root).trim_suffix("/")
	if FileAccess.file_exists(source_path):
		return _contains_path(content_root, source_path)
	var source_root := source_path.trim_suffix("/")
	return _contains_path(source_root, content_root) or _contains_path(content_root, source_root)

func _contains_path(parent: String, child: String) -> bool:
	return parent == child or child.begins_with(parent + "/")

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

func _keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key: String in expected:
		if not value.has(key):
			return false
	return true

func _fail(message: String) -> String:
	state = "failed"
	error = message
	status_message = message
	return message

func _request_abort(message: String) -> void:
	_pending_failure = message
	var cancel_result: Variant = false
	if backend != null and backend.has_method("cancel"):
		cancel_result = backend.cancel()
	state = "cancelling"
	status_message = message + ("; waiting for the importer to stop safely." if cancel_result == true else "; importer stop was not confirmed, so staging output is retained.")
