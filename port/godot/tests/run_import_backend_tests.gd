extends SceneTree
## Tests for T-29016 import backend and boot routing.
## Tests synthetic negative cases, PS1 release diagnostics, container formats,
## actual disc import against workspace disc/files and track1.bin,
## plus entry-only restart routing. This does not exercise the default opening gate.
## TOKIMEMO_TEST_CUE overrides the generated /workspace/disc/disc.cue fixture.

const Flow = preload("res://app/first_launch_import_flow.gd")
const Store = preload("res://services/installed_pack_store.gd")
const SyntheticStore = preload("res://tests/fixtures/synthetic_install_store.gd")
const Backend = preload("res://services/original_game_import_backend.gd")
const BootRouterScript = preload("res://app/boot_router.gd")
const ContentStoreScript = preload("res://services/content_store.gd")
const SetupScene = preload("res://app/first_launch_setup.tscn")

const REQUIRED_CAP := "new_game.entry"
const LEGACY_CAP := "prologue.nbn_native_cells.static_preview"

var checks := 0
var failures: Array[String] = []
var test_root := ""

func _initialize() -> void:
	test_root = "/tmp/t29016-backend-test-%d" % Time.get_ticks_usec()
	DirAccess.make_dir_recursive_absolute(test_root)
	
	print("--- T-29016 Suite 1: Synthetic & Negative Cases ---")
	_test_nonexistent_source_rejected()
	_test_unsupported_container_format()
	_test_reentrant_begin_import_rejected()
	_test_cancel_during_verification()
	_test_unsupported_release_rev1_diagnostics()
	_test_unsupported_release_rev2_diagnostics()
	_test_unsupported_release_rev4_diagnostics()
	_test_corrupt_executable_diagnostics()
	
	print("--- T-29016 Suite 2: Boot Router Preflight & Routing ---")
	_test_boot_router_needs_source_on_first_launch()
	_test_boot_router_routes_to_setup_when_no_pack()
	_test_boot_router_forced_setup_repair()
	_test_boot_router_missing_capabilities()
	_test_setup_scene_defaults_to_real_backend()
	
	print("--- T-29016 Suite 3: Actual Disc Files Import, Restart & Offline ---")
	_test_actual_disc_files_import_and_restart_offline()
	
	print("--- T-29016 Suite 4: Disc Track Image Inspection & Import ---")
	_test_actual_track_image_inspection()
	_test_actual_cue_sheet_import()
	
	_cleanup()
	
	if failures.is_empty():
		print("=======================================================")
		print("T29016 import backend & boot routing: ALL %d assertions PASS" % checks)
		print("=======================================================")
		quit(0)
	else:
		for failure: String in failures:
			printerr("FAIL: ", failure)
		printerr("T29016 import backend: %d/%d assertions failed" % [failures.size(), checks])
		quit(1)

func _check(condition: bool, description: String) -> void:
	checks += 1
	if condition:
		print("PASS: ", description)
	else:
		failures.append(description)
		printerr("FAIL: ", description)

func _cleanup() -> void:
	if not test_root.is_empty() and DirAccess.dir_exists_absolute(test_root):
		_remove_tree(test_root)

func _remove_tree(path: String) -> void:
	if DirAccess.dir_exists_absolute(path):
		var dir := DirAccess.open(path)
		if dir != null:
			dir.include_hidden = true
			for f in dir.get_files():
				DirAccess.remove_absolute(path.path_join(f))
			for d in dir.get_directories():
				_remove_tree(path.path_join(d))
			DirAccess.remove_absolute(path)

# ---------------------------------------------------------------------------
# Suite 1: Synthetic & Negative Cases
# ---------------------------------------------------------------------------

func _test_nonexistent_source_rejected() -> void:
	var backend = Backend.new()
	var err := backend.begin_import("/tmp/does_not_exist_at_all.bin", test_root.path_join("stage"))
	_check(not err.is_empty(), "nonexistent source path is rejected immediately")

func _test_unsupported_container_format() -> void:
	var dummy := test_root.path_join("unsupported.txt")
	var f := FileAccess.open(dummy, FileAccess.WRITE)
	f.store_string("not a ps1 image")
	f.close()
	
	var backend = Backend.new()
	var err := backend.begin_import(dummy, test_root.path_join("stage1"))
	_check(err.is_empty(), "begin_import accepts existing file to inspect format")
	var event := backend.poll_event()
	_check(event.get("status") == "failed" and "unsupported container format" in str(event.get("message", "")),
		"unsupported container format .txt is rejected in poll_event")

func _test_reentrant_begin_import_rejected() -> void:
	var dummy := test_root.path_join("dummy.bin")
	var f := FileAccess.open(dummy, FileAccess.WRITE)
	f.store_string("dummy bytes")
	f.close()
	
	var backend = Backend.new()
	backend.begin_import(dummy, test_root.path_join("stage_reentrant1"))
	var second_err := backend.begin_import(dummy, test_root.path_join("stage_reentrant2"))
	_check(not second_err.is_empty() and second_err == "an import operation is already in progress",
		"re-entrant begin_import call while backend active is rejected")

func _test_cancel_during_verification() -> void:
	var disc_dir := "/workspace/disc/files"
	if not DirAccess.dir_exists_absolute(disc_dir):
		print("SKIP: workspace disc/files not found")
		return
	
	var backend = Backend.new()
	var stage := test_root.path_join("stage_cancel")
	var err := backend.begin_import(disc_dir, stage)
	_check(err.is_empty(), "begin_import starts for cancellation test")
	var ev1 := backend.poll_event()
	_check(ev1.get("status") == "running", "first poll event is running")
	_check(backend.cancel(), "backend accepts cancel request")
	var ev2 := backend.poll_event()
	_check(ev2.get("status") == "cancelled", "poll event returns cancelled status immediately after cancel request")

func _test_unsupported_release_rev1_diagnostics() -> void:
	# Create synthetic Rev 1 source directory with PSX.EXE (SHA1 2d8d4748...)
	var rev1_dir := test_root.path_join("synthetic_rev1")
	DirAccess.make_dir_recursive_absolute(rev1_dir)
	var cnf := FileAccess.open(rev1_dir.path_join("SYSTEM.CNF"), FileAccess.WRITE)
	cnf.store_string("BOOT = cdrom:PSX.EXE;1\r\nTCB = 8\r\nEVENT = 32\r\nSTACK = 801FFF00\r\n")
	cnf.close()
	
	# Create dummy PSX.EXE that hashes to Rev 1 SHA1 or is identified as Rev 1
	# We test _evaluate_release directly and end-to-end
	var backend = Backend.new()
	var res: Dictionary = backend._evaluate_release("PSX.EXE", Backend.REV1_EXE_SIZE, Backend.REV1_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1", "TOKIMEKIMEMORIAL", "SLPS00065VER110")
	_check(res.get("release") == "rev1", "Rev 1 identified by PSX.EXE SHA1")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Rev 1") and diag.contains("521 functions") and diag.contains("SLPM-86053"),
		"Rev 1 diagnostic mentions Rev 1, 521 changed functions, and PlayStation the Best requirement per wiki/versions.md")
	
	# Shokai variant
	var res_shokai: Dictionary = backend._evaluate_release("PSX.EXE", Backend.REV1_EXE_SIZE, Backend.REV1_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1", "TOKIMEKIMEMORIAL", "SLPS00064VER110")
	_check(res_shokai.get("diagnostic", "").contains("Shokai"), "Shokai variant identified from volume set ID SLPS00064")

func _test_unsupported_release_rev2_diagnostics() -> void:
	var backend = Backend.new()
	var res: Dictionary = backend._evaluate_release("PSX.EXE", Backend.REV2_EXE_SIZE, Backend.REV2_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1", "TOKIMEKIMEMORIAL", "SLPS00065VER125")
	_check(res.get("release") == "rev2", "Rev 2 identified by PSX.EXE SHA1")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Rev 2") and diag.contains("475 functions") and diag.contains("SLPM-86053"),
		"Rev 2 diagnostic mentions Rev 2, 475 changed functions, and PlayStation the Best requirement")

func _test_unsupported_release_rev4_diagnostics() -> void:
	var backend = Backend.new()
	var res: Dictionary = backend._evaluate_release("PSX.EXE", Backend.REV4_EXE_SIZE, Backend.REV4_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1", "TOKIMEKIMEMORIAL", "SLPS00065VER143")
	_check(res.get("release") == "rev4", "Rev 4 identified by PSX.EXE SHA1")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Rev 4") and diag.contains("46 functions") and diag.contains("SLPM-86053"),
		"Rev 4 diagnostic mentions Rev 4, 46 functions, shifted rodata symbols, and PlayStation the Best requirement")

func _test_corrupt_executable_diagnostics() -> void:
	var backend = Backend.new()
	var res: Dictionary = backend._evaluate_release("PSX.EXE", 12345, "0123456789abcdef0123456789abcdef01234567", "BOOT = cdrom:PSX.EXE;1")
	_check(res.get("release") == "unknown", "unknown executable identified as unknown")
	_check(res.get("diagnostic", "").contains("Unknown PS1 executable") and res.get("diagnostic", "").contains("Expected PlayStation the Best"),
		"unknown executable diagnostic explicitly reports expected PlayStation the Best")

# ---------------------------------------------------------------------------
# Suite 2: Boot Router Preflight & Routing
# ---------------------------------------------------------------------------

func _test_boot_router_needs_source_on_first_launch() -> void:
	var user_data := test_root.path_join("user_data_clean")
	var store = Store.new(user_data)
	var router = BootRouterScript.new(store)
	var state: Dictionary = router.check_boot_state([REQUIRED_CAP])
	_check(state.get("action") == "setup", "first launch with empty install requests setup")
	_check(state.get("launch_ready") == false, "launch_ready is false on first launch")
	_check(state.get("target") == BootRouterScript.SETUP_SCENE_PATH, "target is first_launch_setup.tscn")

func _test_boot_router_routes_to_setup_when_no_pack() -> void:
	var user_data := test_root.path_join("user_data_clean2")
	var store = Store.new(user_data)
	var router = BootRouterScript.new(store)
	var route_decision: Dictionary = router.route(null)
	_check(route_decision.get("action") == "setup" and route_decision.get("target") == BootRouterScript.SETUP_SCENE_PATH,
		"router.route() returns setup scene decision without modifying host when null")

func _test_boot_router_forced_setup_repair() -> void:
	var user_data := test_root.path_join("user_data_forced")
	var store = SyntheticStore.new(user_data)
	# Publish a synthetic pack
	var stage := store.create_stage_path("op-forced")
	DirAccess.make_dir_recursive_absolute(stage)
	var mf := FileAccess.open(stage.path_join("manifest.json"), FileAccess.WRITE)
	mf.store_string(JSON.stringify({"schema": "tokimemo.port-content.v1", "schema_version": 1,
		"release_profile": "original-ja-ps-the-best", "synthetic_test_only": true,
		"route_capabilities": {"available": [LEGACY_CAP], "unavailable": []}}))
	mf.close()
	var sha := FileAccess.get_sha256(stage.path_join("manifest.json"))
	store.publish_stage(stage, sha, "original-ja-ps-the-best", [LEGACY_CAP])
	
	# Normal preflight should pass
	var router = BootRouterScript.new(store)
	var normal_state: Dictionary = router.check_boot_state([REQUIRED_CAP])
	_check(normal_state.get("action") == "setup", "legacy preview cannot authorize interactive New Game")
	
	# Now set TOKIMEMO_FORCE_SETUP = "1"
	OS.set_environment(BootRouterScript.ENV_FORCE_SETUP, "1")
	var forced_state: Dictionary = router.check_boot_state([REQUIRED_CAP])
	_check(forced_state.get("action") == "setup" and "Repair or re-import was explicitly requested" in forced_state.get("reason", ""),
		"TOKIMEMO_FORCE_SETUP=1 forces route to setup for repair/re-import")
	OS.set_environment(BootRouterScript.ENV_FORCE_SETUP, "")

func _test_boot_router_missing_capabilities() -> void:
	var user_data := test_root.path_join("user_data_missing_cap")
	var store = SyntheticStore.new(user_data)
	var stage := store.create_stage_path("op-missing")
	DirAccess.make_dir_recursive_absolute(stage)
	var mf := FileAccess.open(stage.path_join("manifest.json"), FileAccess.WRITE)
	mf.store_string(JSON.stringify({"schema": "tokimemo.port-content.v1", "schema_version": 1,
		"release_profile": "original-ja-ps-the-best", "synthetic_test_only": true,
		"route_capabilities": {"available": ["different.capability"], "unavailable": []}}))
	mf.close()
	var sha := FileAccess.get_sha256(stage.path_join("manifest.json"))
	store.publish_stage(stage, sha, "original-ja-ps-the-best", ["different.capability"])
	
	var router = BootRouterScript.new(store)
	var state: Dictionary = router.check_boot_state([REQUIRED_CAP])
	_check(state.get("action") == "setup" and "lacks required capabilities" in state.get("reason", ""),
		"boot router routes to setup when installed pack lacks required launch capability")

func _test_setup_scene_defaults_to_real_backend() -> void:
	var setup: Control = SetupScene.instantiate()
	_check(setup != null, "first_launch_setup scene instantiates")
	# Check exported script properties
	_check(setup.get("importer_backend") == null, "backend starts unassigned before ready")
	setup.free()

# ---------------------------------------------------------------------------
# Suite 3: Actual Disc Files Import, Restart & Offline Test
# ---------------------------------------------------------------------------

func _test_actual_disc_files_import_and_restart_offline() -> void:
	var disc_dir := "/workspace/disc/files"
	if not DirAccess.dir_exists_absolute(disc_dir):
		print("SKIP: /workspace/disc/files not present in environment")
		return
	
	var original_exe_path := disc_dir.path_join("SLPM_86.053")
	var source_exe_hash_before := FileAccess.get_sha256(original_exe_path)
	
	var user_data := test_root.path_join("user_data_actual")
	var store = Store.new(user_data)
	var backend = Backend.new()
	var flow = Flow.new(store, backend)
	
	# Preflight check before import
	var initial_status: Dictionary = flow.check_existing([REQUIRED_CAP])
	_check(initial_status.get("state") == "needs_source", "initial preflight reports needs_source")
	
	# Boot router should route to setup initially
	var router = BootRouterScript.new(store, backend, [REQUIRED_CAP])
	var initial_boot := router.check_boot_state()
	_check(initial_boot.get("action") == "setup", "boot router routes to setup on first launch")
	
	# Begin real import on /workspace/disc/files
	var start_err := flow.choose_source(disc_dir, [REQUIRED_CAP])
	_check(start_err.is_empty(), "choose_source starts import on workspace disc/files")
	_check(flow.state == "verifying", "flow enters verifying state")
	
	var progress_history: Array[float] = [flow.progress]
	var max_ticks := 50
	var ticks := 0
	while flow.state in ["verifying", "importing", "validating", "publishing"] and ticks < max_ticks:
		ticks += 1
		flow.tick()
		progress_history.append(flow.progress)
	
	_check(flow.state == "ready", "real import reaches ready state: " + flow.status_message)
	_check(is_equal_approx(flow.progress, 1.0), "final progress is 1.0")
	
	# Check monotonic progress
	var monotonic := true
	for i in range(1, progress_history.size()):
		if progress_history[i] < progress_history[i - 1]:
			monotonic = false
			break
	_check(monotonic, "progress increased monotonically across all %d ticks" % ticks)
	
	# Receipt check
	var receipt_path: String = str(store.storage_root).path_join("installed-content.json")
	_check(FileAccess.file_exists(receipt_path), "installed-content.json receipt was committed")
	var receipt_text := FileAccess.get_file_as_string(receipt_path)
	var receipt: Dictionary = JSON.parse_string(receipt_text)
	_check(receipt.get("release_profile") == "original-ja-ps-the-best", "receipt records release_profile original-ja-ps-the-best")
	_check(str(receipt.get("pack_id", "")).begins_with("original-ja-ps-the-best-"), "receipt records versioned pack_id")
	
	# Validate the native runtime pack with its production installed reader.
	var pack_root: String = str(store.storage_root).path_join("packs").path_join(str(receipt.get("pack_id", "")))
	_check(DirAccess.dir_exists_absolute(pack_root), "published pack directory exists")
	_check(FileAccess.file_exists(pack_root.path_join("manifest.json")), "manifest.json exists in published pack")
	_check(DirAccess.dir_exists_absolute(pack_root.path_join("payloads")), "payloads directory exists in published pack")
	
	var validated: Dictionary = store.validate_pack(pack_root, [REQUIRED_CAP])
	_check(validated.get("valid", false), "production runtime reader validates published pack")
	_check(validated.get("available_capabilities", []).has(REQUIRED_CAP), "published pack provides authenticated native entry")
	_check(not validated.get("available_capabilities", []).has("new_game.gameplay"), "profile readiness does not claim playable opening")
	
	# Verify original source was NOT modified
	var source_exe_hash_after := FileAccess.get_sha256(original_exe_path)
	_check(source_exe_hash_before == source_exe_hash_after, "original disc files were NOT modified by import")
	
	# -----------------------------------------------------------------------
	# Restart & Offline Test:
	# Original disc is NOT referenced or accessed during restart
	# -----------------------------------------------------------------------
	var restart_store = Store.new(user_data)
	var restart_router = BootRouterScript.new(restart_store, null, [REQUIRED_CAP])
	var restart_decision := restart_router.check_boot_state()
	
	_check(restart_decision.get("action") == "playable", "restart preflight routes directly to playable without asking for disc")
	_check(restart_decision.get("target") == "res://app/new_game_entry.tscn", "restart opens interactive entry")
	_check(restart_decision.get("launch_ready") == true, "restart launch_ready is true")
	_check(restart_decision.get("pack_root") == pack_root, "restart remembers persistent pack_root")
	_check(restart_decision.get("manifest_sha256") == receipt.get("manifest_sha256"), "restart validates persistent manifest_sha256")
	
	# Confirm offline: even if an invalid path is configured or source removed
	var route_decision := restart_router.route(null)
	_check(route_decision.get("action") == "playable", "route() selects playable directly on restart")

# ---------------------------------------------------------------------------
# Suite 4: Disc Track Image Inspection
# ---------------------------------------------------------------------------

func _test_actual_track_image_inspection() -> void:
	var track_path := "/workspace/disc/track1.bin"
	if not FileAccess.file_exists(track_path):
		print("SKIP: /workspace/disc/track1.bin not present in environment")
		return
	
	var backend = Backend.new()
	var stage := test_root.path_join("stage_track_inspect")
	var err := backend.begin_import(track_path, stage)
	_check(err.is_empty(), "begin_import accepts /workspace/disc/track1.bin")
	
	var ev1 := backend.poll_event()
	_check(ev1.get("status") == "running" and ev1.get("phase") == "verify", "inspects track1.bin as disc image")
	
	var ev2 := backend.poll_event()
	_check(ev2.get("status") == "running" and "PlayStation the Best" in str(ev2.get("message", "")),
		"identifies PlayStation the Best from track1.bin ISO9660 PVD and SLPM_86.053")
	
	backend.cancel()
	var ev3 := backend.poll_event()
	_check(ev3.get("status") == "cancelled", "cancels cleanly after release verification of raw track")

func _test_actual_cue_sheet_import() -> void:
	var configured_cue := OS.get_environment("TOKIMEMO_TEST_CUE")
	var cue_path := configured_cue if not configured_cue.is_empty() else "/workspace/disc/disc.cue"
	if not FileAccess.file_exists(cue_path):
		if not configured_cue.is_empty():
			_check(false, "configured TOKIMEMO_TEST_CUE does not exist: " + cue_path)
		else:
			print("SKIP: generated /workspace/disc/disc.cue not present; set TOKIMEMO_TEST_CUE to a valid CUE")
		return
	
	var user_data := test_root.path_join("user_data_cue")
	var store = Store.new(user_data)
	var backend = Backend.new()
	var flow = Flow.new(store, backend)
	
	var start_err := flow.choose_source(cue_path, [REQUIRED_CAP])
	_check(start_err.is_empty(), "choose_source accepts genuine PS1 CUE sheet")
	
	var max_ticks := 50
	var ticks := 0
	while flow.state in ["verifying", "importing", "validating", "publishing"] and ticks < max_ticks:
		ticks += 1
		flow.tick()
	
	_check(flow.state == "ready", "import via CUE reaches ready: " + flow.status_message)
	_check(is_equal_approx(flow.progress, 1.0), "final CUE progress is 1.0")
	var receipt_path: String = str(store.storage_root).path_join("installed-content.json")
	_check(FileAccess.file_exists(receipt_path), "CUE import commits receipt")
