extends SceneTree
## T-39007 / WP-10: settings file recovery (schema v2 since T-30002 WP1) across separate
## engine processes. Each phase is its own `godot` process in its own
## container; phases share only the mounted user-data directory and a proof
## directory. Driver: tools/port/run_save_relaunch_tests.sh --settings-matrix.
##
## Every phase first checks what the production settings autoload
## (ModernSettings, services/settings_runtime.gd) loaded at this boot, then
## damages the settings file for the next boot. The memory-card saves written
## in the seed phase are an invented codec fixture (the captured boundary that
## produces real session saves is not available to this test); they exist to
## prove that settings recovery never touches native save bytes.
##
## Order (SETTINGS_PHASE): seed, corrupt, missing, newer, newer-with-backup,
## pending-kill (the process kills itself with a display change pending),
## pending-boot (the next boot rolls that change back).
##
## Expected failures (XFAIL) are checks for a known, ticketed product defect.
## They are reported apart from failures, the phase exits 3 and the driver
## exits non-zero, so a known defect never reads as a pass. They are strict:
## once the defect is fixed the check passes, which is reported as a failure
## (XPASS) until the expectation is removed.

const Preferences = preload("res://services/user_preferences_service.gd")
const Save = preload("res://services/save_service.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
const TitleContinue = preload("res://app/title_continue.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const SETTINGS := "user://desktop-settings.json"
## A schema newer than the current preferences service (v2 since T-30002 WP1).
const NEWER := "{\"schema\": \"tokimemo.port-settings.v3\", \"settings\": {\"future\": true}}"
## Settings-owner defect: user_preferences_service.gd load_preferences saves after
## recovering from the current-version backup, which replaces a newer-version settings file.
const NEWER_BACKUP_DEFECT := "T-39045"
const FIXTURE_VERSIONS := {"source_version": Controls.SOURCE_PROFILE, "content_version": "p4qa-settings-fixture",
	"behavior_version": Controls.BEHAVIOR_PROFILE}

var failures: Array[String] = []
var xfail: Array[String] = []
var checks := 0
var proof_root := ""
var save_root := ""
var runtime: Node

func _initialize() -> void:
	_run.call_deferred()

func _run() -> void:
	proof_root = OS.get_environment("SETTINGS_PROOF")
	save_root = TitleContinue.default_root()
	runtime = root.get_node_or_null("ModernSettings")
	_check(proof_root.is_absolute_path() and OS.get_user_data_dir().begins_with("/userdata/"), "persistent user-data mount and proof directory are active")
	_check(runtime != null, "production settings autoload booted")
	var phase := OS.get_environment("SETTINGS_PHASE")
	if runtime != null:
		match phase:
			"seed": _seed()
			"corrupt": _corrupt()
			"missing": _missing()
			"newer": _newer()
			"newer-with-backup": _newer_with_backup()
			"pending-kill": _pending_kill()
			"pending-boot": _pending_boot()
			_: _check(false, "known phase: " + phase)
	_finish(phase)

func _finish(phase: String, killed: bool = false) -> void:
	var report := {"phase": phase, "checks": checks, "failures": failures, "xfail": xfail, "killed": killed}
	var file := FileAccess.open(proof_root.path_join("settings-" + phase + ".json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "  "))
		file.close()
	if killed: return
	for expected: String in xfail: print("XFAIL: ", expected)
	if failures.is_empty() and not xfail.is_empty():
		print("XFAIL: settings recovery phase %s; %d checks, %d expected failure(s)" % [phase, checks, xfail.size()])
		quit(3)
	elif failures.is_empty():
		print("PASS: settings recovery phase %s; %d checks" % [phase, checks])
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: ", failure)
		quit(1)

func _seed() -> void:
	_check(not FileAccess.file_exists(SETTINGS) and runtime.first_launch, "first launch has no settings file")
	var store := Save.new(save_root)
	_check(store.save_slot(Controls.SLOT, _snapshot(1))["error"].is_empty(), "fixture session save")
	_check(store.save_slot(Controls.SLOT, _snapshot(2))["error"].is_empty(), "second save keeps a session backup")
	_check(ClearSave.new(save_root).clear_save(0, 1).get("ok") == true, "system record (clear counters) saved")
	_check(runtime.preferences.set_value("language", "text", "en"), "language preference saved")
	_check(runtime.preferences.set_value("audio", "mono", true), "second write leaves a settings backup")
	_check(FileAccess.file_exists(SETTINGS + ".backup"), "settings backup exists")
	var hashes := _save_hashes()
	_check(hashes.size() >= 3, "session, session backup and system record files exist")
	_write_text(proof_root.path_join("save-hashes.json"), JSON.stringify(hashes))
	_damage("{ not json", false)

func _corrupt() -> void:
	_check_boot_defaults("corrupt")
	for suffix: String in ["", ".backup", ".tmp", ".backup.tmp"]:
		DirAccess.remove_absolute(ProjectSettings.globalize_path(SETTINGS + suffix))

func _missing() -> void:
	var preferences: Preferences = runtime.preferences
	_check(runtime.first_launch and preferences.error.is_empty() and not preferences.recovered, "missing settings: clean first-launch boot, no recovery notice")
	_check(preferences.effective_settings() == Preferences.defaults(), "missing settings: defaults")
	_check(not FileAccess.file_exists(SETTINGS), "missing settings: nothing written until the player changes a setting")
	_check_saves_unchanged("missing")
	_damage(NEWER, false)

func _newer() -> void:
	_check_boot_defaults("newer")
	# A newer file next to a valid current-version backup that holds the player's settings.
	var settings := Preferences.defaults()
	settings.language.text = "en"; settings.audio.mono = true
	_write_text(SETTINGS + ".backup", JSON.stringify({"schema": Preferences.SCHEMA, "settings": settings, "pending_display_boot_check": {}}))

func _newer_with_backup() -> void:
	var effective: Dictionary = runtime.preferences.effective_settings()
	_check(runtime.preferences.recovered and effective.language.text == "en" and effective.audio.mono, "newer-with-backup: backup settings are active")
	var kept := FileAccess.get_file_as_string(SETTINGS + ".backup") == NEWER
	_check_expected_failure(FileAccess.get_file_as_string(SETTINGS) == NEWER, NEWER_BACKUP_DEFECT,
		"newer-with-backup: the newer-version settings file is left in place, not overwritten at boot (data loss: "
		+ ("it survives only as .backup until the next settings save)" if kept else "it is gone)"))
	_check_saves_unchanged("newer-with-backup")

func _pending_kill() -> void:
	var preferences: Preferences = runtime.preferences
	_check(preferences.error.is_empty(), "valid settings boot")
	var display: Dictionary = preferences.settings.display.duplicate(true)
	display.width = 1280; display.height = 720; display.aspect = "wide"; display.filter = "linear"
	_check(preferences.begin_display(display, 0.0), "display change is pending")
	_check(preferences.effective_settings().display.aspect == "wide", "pending display is the effective one")
	var stored: Variant = JSON.parse_string(FileAccess.get_file_as_string(SETTINGS))
	_check(stored is Dictionary and not stored.get("pending_display_boot_check", {}).is_empty(), "durable rollback marker written before the change applies")
	_finish("pending-kill", true)
	# No confirm, no revert, no clean exit: the process dies here.
	OS.kill(OS.get_process_id())

func _pending_boot() -> void:
	var effective: Dictionary = runtime.preferences.effective_settings()
	_check(runtime.preferences.recovered, "boot detected the interrupted display change")
	_check(effective.display.aspect == "original" and effective.display.filter == "integer" and effective.display.width == 960,
		"display rolled back to the previous configuration")
	_check(effective.language.text == "en" and effective.audio.mono, "other preferences survive the rollback")
	var stored: Variant = JSON.parse_string(FileAccess.get_file_as_string(SETTINGS))
	_check(stored is Dictionary and stored.get("pending_display_boot_check", {"x": 1}).is_empty(), "rollback marker cleared on disk")
	_check_saves_unchanged("pending-boot")

## The damaged file this boot found: defaults, a notice, the file left as is.
func _check_boot_defaults(label: String) -> void:
	var preferences: Preferences = runtime.preferences
	_check(preferences.effective_settings() == Preferences.defaults(), label + ": safe defaults are active")
	_check(not preferences.error.is_empty(), label + ": the player is told settings were reset")
	var expected: String = FileAccess.get_file_as_string(proof_root.path_join("damaged-sha256.txt"))
	_check(FileAccess.get_sha256(SETTINGS) == expected, label + ": the unreadable file is left for inspection, not overwritten at boot")
	_check_saves_unchanged(label)

func _damage(text: String, keep_backup: bool) -> void:
	_write_text(SETTINGS, text)
	if not keep_backup: DirAccess.remove_absolute(ProjectSettings.globalize_path(SETTINGS + ".backup"))
	_write_text(proof_root.path_join("damaged-sha256.txt"), FileAccess.get_sha256(SETTINGS))

func _check_saves_unchanged(label: String) -> void:
	var expected: Variant = JSON.parse_string(FileAccess.get_file_as_string(proof_root.path_join("save-hashes.json")))
	_check(expected is Dictionary and not expected.is_empty() and _save_hashes() == expected,
		label + ": memory-card saves are byte-identical")

func _save_hashes() -> Dictionary:
	var hashes := {}
	var directory := DirAccess.open(save_root)
	if directory == null: return hashes
	for name: String in directory.get_files():
		hashes[name] = FileAccess.get_sha256(save_root.path_join(name))
	return hashes

func _snapshot(marker: int) -> Dictionary:
	var snapshot := {"boundary": "stable", "gameplay_state": {"checkpoint_id": "command", "fixture_marker": marker},
		"native_rng": {"state": marker}, "native_clocks": {"logic_iterations": marker}, "queued_service_events": []}
	snapshot.merge(FIXTURE_VERSIONS)
	return snapshot

func _write_text(path: String, text: String) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(text)
	file.close()

## Strict expected failure for a ticketed defect: a failing check is XFAIL, a passing one is an XPASS failure.
func _check_expected_failure(condition: bool, ticket: String, message: String) -> void:
	checks += 1
	if condition:
		failures.append("XPASS %s: %s; remove the expected failure" % [ticket, message])
	else:
		xfail.append("%s: %s" % [ticket, message])

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition: failures.append(message)
