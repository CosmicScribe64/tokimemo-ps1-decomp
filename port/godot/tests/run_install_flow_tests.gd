extends SceneTree

const Flow = preload("res://app/first_launch_import_flow.gd")
const Store = preload("res://services/installed_pack_store.gd")
const SyntheticStore = preload("res://tests/fixtures/synthetic_install_store.gd")
const Backend = preload("res://tests/fixtures/fake_import_backend.gd")
const SetupScene = preload("res://app/first_launch_setup.tscn")
const GAMEPLAY_CAP := "test.synthetic.route"

var checks := 0
var failures: Array[String] = []
var test_root := ""
var source := ""
var source_hash := ""

func _initialize() -> void:
	test_root = "/tmp/t29016-install-test-%d" % Time.get_ticks_usec()
	source = test_root.path_join("original-disc.img")
	DirAccess.make_dir_recursive_absolute(test_root)
	var source_file := FileAccess.open(source, FileAccess.WRITE)
	source_file.store_buffer("invented PS1 source fixture".to_utf8_buffer())
	source_file.close()
	source_hash = FileAccess.get_sha256(source)
	_test_interrupted_location_preference_recovers()
	_test_first_launch_import_and_restart()
	_test_reentrant_import_preserves_active_operation()
	_test_cancel_failure_and_incomplete_reimport()
	_test_source_symlink_and_default_backend_fail_closed()
	_test_source_must_not_overlap_install_root()
	_test_storage_location_boundary()
	_test_actual_partial_pack_never_claims_gameplay()
	_test_setup_scene_builds()
	_check(FileAccess.get_sha256(source) == source_hash, "source bytes remain unchanged through every synthetic operation")
	_cleanup()
	if failures.is_empty():
		print("T29016 install flow: %d synthetic assertions PASS" % checks)
		quit(0)
	else:
		for failure: String in failures:
			push_error(failure)
		printerr("T29016 install flow: %d/%d synthetic assertions failed" % [failures.size(), checks])
		quit(1)

func _test_first_launch_import_and_restart() -> void:
	var store = SyntheticStore.new(test_root.path_join("user-data"))
	var backend = Backend.new()
	var flow = Flow.new(store, backend)
	var first: Dictionary = flow.check_existing([GAMEPLAY_CAP])
	_check(first.get("state") == "needs_source", "missing receipt asks for original source on first launch")
	backend.configure([
		_running("verify", 0.2, "identify release"),
		_running("import", 0.72, "read-only import"),
		_complete(backend)
	], [GAMEPLAY_CAP])
	_check(flow.choose_source(source, [GAMEPLAY_CAP]).is_empty(), "selected source begins verification")
	_check(flow.state == "verifying" and backend.started, "flow enters source verification before import")
	flow.tick()
	_check(flow.state == "verifying" and is_equal_approx(flow.progress, 0.2), "verification progress is surfaced")
	flow.tick()
	_check(flow.state == "importing" and is_equal_approx(flow.progress, 0.72), "import progress is surfaced")
	flow.tick()
	_check(flow.state == "ready" and is_equal_approx(flow.progress, 1.0), "validated complete package publishes as launch ready")
	var receipt_path: String = store.storage_root.path_join("installed-content.json")
	_check(FileAccess.file_exists(receipt_path), "published pack has a persistent receipt")
	var receipt_hash := FileAccess.get_sha256(receipt_path)
	_check(source_hash == FileAccess.get_sha256(source), "importer did not modify the selected original source")
	DirAccess.remove_absolute(source)
	var location_store = SyntheticStore.new(store.storage_root)
	_check(location_store.set_storage_root(store.storage_root), "content location can be selected and remembered per user")
	var alternate_root := test_root.path_join("alternate-location")
	var alternate_location = SyntheticStore.new()
	var alternate_flow = Flow.new(alternate_location)
	alternate_flow.check_existing([GAMEPLAY_CAP])
	var alternate_result: Dictionary = alternate_flow.set_storage_location(alternate_root)
	_check(alternate_result.get("state") == "needs_source", "new location does not claim an install or copy old data")
	_check(store.check_installed([GAMEPLAY_CAP]).get("state") == "ready", "changing location leaves prior installation intact")
	var restore_location: Dictionary = alternate_flow.set_storage_location(store.storage_root)
	_check(restore_location.get("state") == "ready", "previous content location can be selected again: " + str(restore_location.get("message", "")))
	var restarted = Flow.new(SyntheticStore.new())
	var restored: Dictionary = restarted.check_existing([GAMEPLAY_CAP])
	_check(restored.get("state") == "ready" and restored.get("manifest_sha256") == backend.manifest_sha256,
		"restart reuses the validated receipt without original source availability")
	_check(FileAccess.get_sha256(receipt_path) == receipt_hash, "restart does not rewrite the installed receipt")
	DirAccess.make_dir_recursive_absolute(test_root)
	var replacement := FileAccess.open(source, FileAccess.WRITE)
	replacement.store_buffer("replacement synthetic source".to_utf8_buffer())
	replacement.close()
	source_hash = FileAccess.get_sha256(source)

func _test_cancel_failure_and_incomplete_reimport() -> void:
	var user_root := test_root.path_join("user-data")
	var store = SyntheticStore.new(user_root)
	var backend = Backend.new()
	var receipt_path := user_root.path_join("installed-content.json")
	var old_receipt_hash := FileAccess.get_sha256(receipt_path)
	var flow = Flow.new(store, backend)
	backend.configure([_running("verify", 0.1, "check replacement"), _running("import", 0.5, "stage replacement"), {"status": "cancelled"}], [GAMEPLAY_CAP])
	_check(flow.choose_source(source, [GAMEPLAY_CAP]).is_empty(), "repair/import can start after receipt exists")
	flow.tick()
	flow.tick()
	_check(flow.cancel().is_empty() and flow.state == "cancelling", "cancel enters a wait-for-stop state")
	flow.tick()
	_check(flow.state == "cancelled", "cancellation completes only after helper confirms stop")
	_check(FileAccess.get_sha256(receipt_path) == old_receipt_hash, "cancel preserves last working receipt")
	_check(store.check_installed([GAMEPLAY_CAP]).get("state") == "ready", "cancel preserves last working installed pack")

	backend.configure([_running("verify", 0.1, "check replacement"), {"status": "failed", "message": "source checksum mismatch"}], [GAMEPLAY_CAP])
	flow = Flow.new(store, backend)
	flow.choose_source(source, [GAMEPLAY_CAP])
	flow.tick()
	flow.tick()
	_check(flow.state == "failed" and flow.error == "source checksum mismatch", "source validation failure is shown explicitly")
	_check(FileAccess.get_sha256(receipt_path) == old_receipt_hash, "failed verification leaves previous receipt unchanged")

	backend.configure([_complete(backend)], ["prologue.nbn_native_cells.static_preview"])
	flow = Flow.new(store, backend)
	flow.choose_source(source, [GAMEPLAY_CAP])
	flow.tick()
	_check(flow.state == "ready", "incomplete re-import keeps the prior ready result active")
	_check(flow.latest_result.get("launch_ready") == true and flow.latest_result.get("candidate_missing_capabilities") == [GAMEPLAY_CAP],
		"missing required capability is reported while the prior pack remains selected")
	_check(FileAccess.get_sha256(receipt_path) == old_receipt_hash, "incomplete re-import preserves the last launch-ready receipt")
	_check(store.check_installed([GAMEPLAY_CAP]).get("state") == "ready", "previous launch-ready pack remains selected after incomplete re-import")

func _test_reentrant_import_preserves_active_operation() -> void:
	var store = SyntheticStore.new(test_root.path_join("reentrant-user-data"))
	var backend = Backend.new()
	backend.configure([_running("verify", 0.1, "checking"), _running("import", 0.7, "reading"), _complete(backend)], [GAMEPLAY_CAP])
	var flow = Flow.new(store, backend)
	_check(flow.choose_source(source, [GAMEPLAY_CAP]).is_empty(), "initial import starts")
	var second_attempt: String = flow.choose_source(source, [GAMEPLAY_CAP])
	_check(not second_attempt.is_empty(), "second import request while active is rejected")
	_check(flow.state == "verifying" and backend.started and not backend.cancel_requested,
		"rejected second request leaves the first backend operation active")
	flow.tick()
	_check(flow.state == "verifying", "first operation still accepts verification progress after reentrant request")
	flow.tick()
	flow.tick()
	_check(flow.state == "ready" and store.check_installed([GAMEPLAY_CAP]).get("state") == "ready",
		"first operation completes and publishes after reentrant request is rejected")

func _test_source_must_not_overlap_install_root() -> void:
	var equal_root := test_root.path_join("source-equals-content-root")
	DirAccess.make_dir_recursive_absolute(equal_root)
	var source_file := FileAccess.open(equal_root.path_join("original.img"), FileAccess.WRITE)
	source_file.store_string("invented source in selected root")
	source_file.close()
	var same_store = SyntheticStore.new(equal_root)
	var same_backend = Backend.new()
	var same_flow = Flow.new(same_store, same_backend)
	_check(not same_flow.choose_source(equal_root, [GAMEPLAY_CAP]).is_empty(), "source folder equal to install root is rejected")
	_check(not same_backend.started and not DirAccess.dir_exists_absolute(equal_root.path_join(".staging")),
		"equal-root rejection occurs before stage creation or backend access")
	_check(FileAccess.file_exists(equal_root.path_join("original.img")), "equal-root rejection preserves original source")

	var parent_source := test_root.path_join("source-parent-of-install")
	DirAccess.make_dir_recursive_absolute(parent_source)
	var nested_store = SyntheticStore.new(parent_source.path_join("content-install"))
	var nested_backend = Backend.new()
	var nested_flow = Flow.new(nested_store, nested_backend)
	_check(not nested_flow.choose_source(parent_source, [GAMEPLAY_CAP]).is_empty(), "source folder containing the install root is rejected")
	_check(not nested_backend.started and not DirAccess.dir_exists_absolute(parent_source.path_join("content-install")),
		"ancestor-root rejection does not create an install directory inside the source")

	var file_store_root := test_root.path_join("file-source-content-root")
	DirAccess.make_dir_recursive_absolute(file_store_root)
	var file_source_path := file_store_root.path_join("original.img")
	var file_source := FileAccess.open(file_source_path, FileAccess.WRITE)
	file_source.store_string("invented original source")
	file_source.close()
	var file_store = SyntheticStore.new(file_store_root)
	var file_backend = Backend.new()
	var file_flow = Flow.new(file_store, file_backend)
	_check(not file_flow.choose_source(file_source_path, [GAMEPLAY_CAP]).is_empty(), "source file inside install root is rejected")
	_check(not file_backend.started and not DirAccess.dir_exists_absolute(file_store_root.path_join(".staging")),
		"source-file overlap rejection precedes stage creation")

func _test_interrupted_location_preference_recovers() -> void:
	var preferences_root := OS.get_user_data_dir()
	DirAccess.make_dir_recursive_absolute(preferences_root)
	var preference := preferences_root.path_join("content-location.json")
	var backup := preference + ".previous"
	_check(not FileAccess.file_exists(preference) and not FileAccess.file_exists(backup), "isolated test app starts without a saved location preference")
	var chosen_root := test_root.path_join("interrupted-preference-install")
	var writer = Store.new()
	_check(writer.set_storage_root(chosen_root), "custom content location preference commits")
	_check(DirAccess.rename_absolute(preference, backup) == OK, "test simulates interruption after old preference moves aside")
	var restarted = Store.new()
	_check(restarted.storage_root == chosen_root, "restart recovers selected location from previous preference")

func _test_source_symlink_and_default_backend_fail_closed() -> void:
	var user_root := test_root.path_join("user-data")
	var store = SyntheticStore.new(user_root)
	var folder := DirAccess.open(test_root)
	var link := test_root.path_join("source-link.img")
	_check(folder.create_link(source, link) == OK, "test creates source symlink")
	var backend = Backend.new()
	var flow = Flow.new(store, backend)
	_check(not flow.choose_source(link, [GAMEPLAY_CAP]).is_empty() and flow.state == "failed",
		"source symlink is rejected before the importer starts")
	_check(not backend.started, "rejected source never reaches the importer")
	flow = Flow.new(store)
	_check(not flow.choose_source(source, [GAMEPLAY_CAP]).is_empty() and flow.state == "importer_unavailable",
		"missing packaged helper fails closed and never claims an import")
	DirAccess.remove_absolute(link)

func _test_actual_partial_pack_never_claims_gameplay() -> void:
	var store = Store.new(test_root.path_join("actual-validator-user-data"))
	var pack := "/workspace/.asset-cache/port/prologue-nbn-preview-v3-final"
	if not DirAccess.dir_exists_absolute(pack):
		_check(false, "actual ignored T29003 partial pack is mounted")
		return
	var result := store.validate_pack(pack, ["school_day.route"])
	_check(result.get("valid", false), "production ContentStore validates actual partial T29003 pack: " + str(result.get("message", "")))
	_check(not result.get("launch_ready", true) and result.get("missing_capabilities", []).has("school_day.route"),
		"actual partial pack is explicitly not ready for a gameplay route")

func _test_storage_location_boundary() -> void:
	var store = SyntheticStore.new(test_root.path_join("boundary-user-data"))
	var project_root := ProjectSettings.globalize_path("res://").simplify_path()
	_check(not store.set_storage_root("res://content-install"), "project resource URI is rejected as persistent content")
	_check(not store.set_storage_root(project_root), "project directory is rejected as persistent content")
	var external := test_root.path_join("external-storage")
	DirAccess.make_dir_recursive_absolute(external)
	var link := test_root.path_join("linked-storage")
	var dir := DirAccess.open(test_root)
	_check(dir.create_link(external, link) == OK, "test creates storage symlink")
	_check(not store.set_storage_root(link), "symlinked content location is rejected")
	_check(not DirAccess.dir_exists_absolute(project_root.path_join("content-install")), "rejected project path is not created")
	DirAccess.remove_absolute(link)

func _test_setup_scene_builds() -> void:
	var setup := SetupScene.instantiate()
	setup._ready()
	_check(setup.get_child_count() >= 2, "first-launch scene constructs file/folder picker and status controls (children=" + str(setup.get_child_count()) + ")")
	var unavailable: String = setup._friendly_state("importer_unavailable").to_lower()
	var incomplete: String = setup._friendly_state("installed_incomplete").to_lower()
	_check(not unavailable.contains("backend") and not unavailable.contains("importer") and not incomplete.contains("capability"),
		"user-facing setup copy hides implementation and capability jargon")
	setup.free()

func _complete(backend: Object) -> Dictionary:
	return {"status": "complete", "release_profile": "original-ja-ps-the-best", "manifest_sha256": backend.manifest_sha256}

func _running(phase: String, fraction: float, message: String) -> Dictionary:
	return {"status": "running", "phase": phase, "fraction": fraction, "message": message}

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)
		printerr("FAIL: " + message)
	else:
		print("PASS: " + message)

func _cleanup() -> void:
	var store = SyntheticStore.new(test_root.path_join("user-data"))
	store._remove_tree(test_root)
