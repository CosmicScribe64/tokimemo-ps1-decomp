extends SceneTree
## Verification suite for T-29017: Multi-release PS1 disc recognition and validation.
## Tests identification of all 5 known releases, boot target validation,
## volume descriptor disambiguation (Rev 1 vs Shokai), and integration import checks.

const Backend = preload("res://services/original_game_import_backend.gd")
const Store = preload("res://services/installed_pack_store.gd")

var checks := 0
var failures: Array[String] = []
var test_root := ""

func _initialize() -> void:
	test_root = "/tmp/t29017-multi-release-test-%d" % Time.get_ticks_usec()
	DirAccess.make_dir_recursive_absolute(test_root)
	
	print("--- T-29017 Suite 1: Direct Release Profile Identification ---")
	_test_identify_best_release()
	_test_identify_best_with_mismatched_boot()
	_test_identify_rev4_release()
	_test_identify_rev2_release()
	_test_identify_rev1_release_with_pvd()
	_test_identify_shokai_release_with_pvd()
	_test_identify_ambiguous_rev1_or_shokai_without_pvd()
	_test_identify_unknown_executable()
	
	print("--- T-29017 Suite 2: Boot Target Validation ---")
	_test_check_boot_target_variations()
	
	print("--- T-29017 Suite 3: Backend Rejection of Non-Best Releases ---")
	_test_backend_rejects_synthetic_rev4()
	_test_backend_rejects_synthetic_rev2()
	_test_backend_rejects_synthetic_rev1()
	
	print("--- T-29017 Suite 4: Real Disc / Image Verification ---")
	_test_real_disc_files_identified_as_best()
	_test_real_track1_bin_identified_as_best()
	
	_cleanup()
	
	if failures.is_empty():
		print("=======================================================")
		print("T29017 multi-release verification: ALL %d assertions PASS" % checks)
		print("=======================================================")
		quit(0)
	else:
		for failure: String in failures:
			printerr("FAIL: ", failure)
		printerr("T29017 multi-release verification: %d/%d assertions failed" % [failures.size(), checks])
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
			for f in dir.get_files():
				DirAccess.remove_absolute(path.path_join(f))
			for d in dir.get_directories():
				_remove_tree(path.path_join(d))
			DirAccess.remove_absolute(path)

# ---------------------------------------------------------------------------
# Suite 1: Direct Release Profile Identification
# ---------------------------------------------------------------------------

func _test_identify_best_release() -> void:
	var res := Backend.identify_ps1_release(
		"SLPM_86.053",
		Backend.BEST_EXE_SIZE,
		Backend.BEST_EXE_SHA1,
		"BOOT = cdrom:\\SLPM_86.053;1\r\nTCB = 8\r\nEVENT = 32\r\nSTACK = 801FFF00\r\n",
		Backend.BEST_VOLUME_ID,
		Backend.BEST_VOLUME_SET_ID
	)
	_check(res.get("release") == Backend.RELEASE_BEST, "Best identified as 'best'")
	_check(res.get("canonical_profile") == Backend.PROFILE_BEST_CANONICAL, "Canonical profile is best_slpm_86053")
	_check(res.get("supported_for_content_extraction") == true, "Best is supported for content extraction")
	_check(res.get("boot_target_valid") == true, "Best boot target is valid")
	_check(res.get("diagnostic_code") == Backend.CODE_OK_PLAYSTATION_THE_BEST, "Diagnostic code is OK_PLAYSTATION_THE_BEST")

func _test_identify_best_with_mismatched_boot() -> void:
	var res := Backend.identify_ps1_release(
		"SLPM_86.053",
		Backend.BEST_EXE_SIZE,
		Backend.BEST_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		Backend.BEST_VOLUME_ID,
		Backend.BEST_VOLUME_SET_ID
	)
	_check(res.get("release") == Backend.RELEASE_BEST, "Identified as best despite boot mismatch")
	_check(res.get("boot_target_valid") == false, "Boot target marked invalid due to mismatch")
	_check(res.get("supported_for_content_extraction") == false, "Extraction disabled on boot mismatch")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_INVALID_BOOT_TARGET, "Diagnostic code is ERR_INVALID_BOOT_TARGET")

func _test_identify_rev4_release() -> void:
	var res := Backend.identify_ps1_release(
		"PSX.EXE",
		Backend.REV4_EXE_SIZE,
		Backend.REV4_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		"TOKIMEKIMEMORIAL",
		"SLPS00065VER143"
	)
	_check(res.get("release") == Backend.RELEASE_REV4, "Rev 4 identified as 'rev4'")
	_check(res.get("canonical_profile") == Backend.PROFILE_REV4_CANONICAL, "Canonical profile is rev4_slps_00065_v143")
	_check(res.get("supported_for_content_extraction") == false, "Rev 4 is not supported for extraction")
	_check(res.get("boot_target_valid") == true, "Rev 4 boot target is valid")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV4_UNSUPPORTED, "Diagnostic code is ERR_REV4_UNSUPPORTED")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Rev 4") and diag.contains("46 functions") and diag.contains("0x800AF1F0"), "Diagnostic describes Rev 4 rodata and function delta")

func _test_identify_rev2_release() -> void:
	var res := Backend.identify_ps1_release(
		"PSX.EXE",
		Backend.REV2_EXE_SIZE,
		Backend.REV2_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		"TOKIMEKIMEMORIAL",
		"SLPS00065VER125"
	)
	_check(res.get("release") == Backend.RELEASE_REV2, "Rev 2 identified as 'rev2'")
	_check(res.get("canonical_profile") == Backend.PROFILE_REV2_CANONICAL, "Canonical profile is rev2_slps_00065_v125")
	_check(res.get("supported_for_content_extraction") == false, "Rev 2 is not supported for extraction")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV2_UNSUPPORTED, "Diagnostic code is ERR_REV2_UNSUPPORTED")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Rev 2") and diag.contains("475 functions"), "Diagnostic describes Rev 2 475 changed functions")

func _test_identify_rev1_release_with_pvd() -> void:
	var res := Backend.identify_ps1_release(
		"PSX.EXE",
		Backend.REV1_EXE_SIZE,
		Backend.REV1_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		"TOKIMEKIMEMORIAL",
		"SLPS00065VER110"
	)
	_check(res.get("release") == Backend.RELEASE_REV1, "Rev 1 identified as 'rev1' with volume set ID SLPS00065")
	_check(res.get("canonical_profile") == Backend.PROFILE_REV1_CANONICAL, "Canonical profile is rev1_slps_00065_v110")
	_check(res.get("supported_for_content_extraction") == false, "Rev 1 is not supported for extraction")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV1_UNSUPPORTED, "Diagnostic code is ERR_REV1_UNSUPPORTED")

func _test_identify_shokai_release_with_pvd() -> void:
	var res := Backend.identify_ps1_release(
		"PSX.EXE",
		Backend.REV1_EXE_SIZE,
		Backend.REV1_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		"TOKIMEKIMEMORIAL",
		"SLPS00064VER110"
	)
	_check(res.get("release") == Backend.RELEASE_SHOKAI, "Shokai identified as 'shokai' with volume set ID SLPS00064")
	_check(res.get("canonical_profile") == Backend.PROFILE_SHOKAI_CANONICAL, "Canonical profile is shokai_slps_00064_v110")
	_check(res.get("supported_for_content_extraction") == false, "Shokai is not supported for extraction")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_SHOKAI_UNSUPPORTED, "Diagnostic code is ERR_SHOKAI_UNSUPPORTED")
	var diag: String = res.get("diagnostic", "")
	_check(diag.contains("Shokai Genteiban"), "Diagnostic mentions Shokai Genteiban")

func _test_identify_ambiguous_rev1_or_shokai_without_pvd() -> void:
	var res := Backend.identify_ps1_release(
		"PSX.EXE",
		Backend.REV1_EXE_SIZE,
		Backend.REV1_EXE_SHA1,
		"BOOT = cdrom:\\PSX.EXE;1\r\n",
		"",
		""
	)
	_check(res.get("canonical_profile") == Backend.PROFILE_REV1_OR_SHOKAI_CANONICAL, "Without PVD, falls back to rev1_or_shokai_v110 profile")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV1_OR_SHOKAI_UNSUPPORTED, "Diagnostic code is ERR_REV1_OR_SHOKAI_UNSUPPORTED")
	_check(res.get("supported_for_content_extraction") == false, "Ambiguous profile is not supported for extraction")

func _test_identify_unknown_executable() -> void:
	var res := Backend.identify_ps1_release(
		"UNKNOWN.EXE",
		123456,
		"0123456789abcdef0123456789abcdef01234567",
		"BOOT = cdrom:\\UNKNOWN.EXE;1\r\n"
	)
	_check(res.get("release") == Backend.RELEASE_UNKNOWN, "Unknown executable identified as 'unknown'")
	_check(res.get("canonical_profile") == "unknown", "Canonical profile is unknown")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_UNKNOWN_EXECUTABLE, "Diagnostic code is ERR_UNKNOWN_EXECUTABLE")

# ---------------------------------------------------------------------------
# Suite 2: Boot Target Validation
# ---------------------------------------------------------------------------

func _test_check_boot_target_variations() -> void:
	_check(Backend.check_boot_target("SLPM_86.053", "BOOT = cdrom:SLPM_86.053;1"), "Valid standard boot format")
	_check(Backend.check_boot_target("SLPM_86.053", "BOOT = cdrom:\\SLPM_86.053;1"), "Valid backslash boot format")
	_check(Backend.check_boot_target("slpm_86.053", "boot = cdrom:SLPM_86.053;1"), "Case-insensitive boot target matching")
	_check(Backend.check_boot_target("PSX.EXE", "BOOT=cdrom:PSX.EXE;1"), "Valid tight syntax format")
	_check(not Backend.check_boot_target("SLPM_86.053", "BOOT = cdrom:PSX.EXE;1"), "Mismatch detected for different executable")
	_check(not Backend.check_boot_target("SLPM_86.053", "TCB = 4\r\nEVENT = 16\r\n"), "Missing BOOT entry detected as invalid")
	_check(Backend.check_boot_target("SLPM_86.053", ""), "Empty CNF treated gracefully")

# ---------------------------------------------------------------------------
# Suite 3: Backend Rejection of Non-Best Releases
# ---------------------------------------------------------------------------

func _test_backend_rejects_synthetic_rev4() -> void:
	var rev4_dir := test_root.path_join("synthetic_rev4_backend")
	DirAccess.make_dir_recursive_absolute(rev4_dir)
	var cnf := FileAccess.open(rev4_dir.path_join("SYSTEM.CNF"), FileAccess.WRITE)
	cnf.store_string("BOOT = cdrom:PSX.EXE;1\r\n")
	cnf.close()
	
	# Create synthetic PSX.EXE with Rev 4 size and payload (we can test backend's rejection logic)
	var backend = Backend.new()
	var res := backend._evaluate_release("PSX.EXE", Backend.REV4_EXE_SIZE, Backend.REV4_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1")
	_check(res.get("release") == Backend.RELEASE_REV4, "Backend evaluator flags Rev 4")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV4_UNSUPPORTED, "Diagnostic code matches ERR_REV4_UNSUPPORTED")

func _test_backend_rejects_synthetic_rev2() -> void:
	var backend = Backend.new()
	var res := backend._evaluate_release("PSX.EXE", Backend.REV2_EXE_SIZE, Backend.REV2_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1")
	_check(res.get("release") == Backend.RELEASE_REV2, "Backend evaluator flags Rev 2")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV2_UNSUPPORTED, "Diagnostic code matches ERR_REV2_UNSUPPORTED")

func _test_backend_rejects_synthetic_rev1() -> void:
	var backend = Backend.new()
	var res := backend._evaluate_release("PSX.EXE", Backend.REV1_EXE_SIZE, Backend.REV1_EXE_SHA1, "BOOT = cdrom:PSX.EXE;1", "TOKIMEKIMEMORIAL", "SLPS00065VER110")
	_check(res.get("release") == Backend.RELEASE_REV1, "Backend evaluator flags Rev 1")
	_check(res.get("diagnostic_code") == Backend.CODE_ERR_REV1_UNSUPPORTED, "Diagnostic code matches ERR_REV1_UNSUPPORTED")

# ---------------------------------------------------------------------------
# Suite 4: Real Disc / Image Verification
# ---------------------------------------------------------------------------

func _test_real_disc_files_identified_as_best() -> void:
	var disc_dir := "/workspace/disc/files"
	if not DirAccess.dir_exists_absolute(disc_dir):
		print("SKIP: disc/files not found")
		return
	
	var backend = Backend.new()
	var stage := test_root.path_join("stage_real_disc")
	var err := backend.begin_import(disc_dir, stage)
	_check(err.is_empty(), "begin_import succeeds for disc/files")
	
	# Poll through step 1 (identify_release)
	var ev1 := backend.poll_event()
	_check(ev1.get("status") == "running", "step 0 container check running")
	var ev2 := backend.poll_event()
	_check(ev2.get("status") == "running", "step 1 release identification running")
	var msg: String = ev2.get("message", "")
	_check(msg.contains("PlayStation the Best"), "Step 1 progress reports PlayStation the Best identified")
	backend.cancel()

func _test_real_track1_bin_identified_as_best() -> void:
	var track1 := "/workspace/disc/track1.bin"
	if not FileAccess.file_exists(track1):
		print("SKIP: disc/track1.bin not found")
		return
	
	var backend = Backend.new()
	var stage := test_root.path_join("stage_real_track1")
	var err := backend.begin_import(track1, stage)
	_check(err.is_empty(), "begin_import succeeds for disc/track1.bin")
	
	var ev1 := backend.poll_event()
	_check(ev1.get("status") == "running", "step 0 track container check running")
	var ev2 := backend.poll_event()
	_check(ev2.get("status") == "running", "step 1 track release identification running")
	var msg: String = ev2.get("message", "")
	_check(msg.contains("PlayStation the Best"), "Track 1 identified as PlayStation the Best")
	backend.cancel()
