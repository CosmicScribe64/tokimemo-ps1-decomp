extends SceneTree
## T-39007 (reviews R10-R12): save -> quit -> relaunch -> Continue, recovery
## and clear-save persistence across separate engine processes. Each phase is
## its own `godot` process; phases share only the mounted user-data directory
## and a proof directory. Driver: tools/port/run_save_relaunch_tests.sh.
##
## The session snapshot is an invented codec fixture stamped with this build's
## profile identity, because the captured development boundary that produces
## real session saves is not available here. The clear-save counters and
## album records are written by the production transaction code.

const Save = preload("res://services/save_service.gd")
const SystemRecords = preload("res://services/system_record_service.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const TitleContinue = preload("res://app/title_continue.gd")
const Launch = preload("res://app/installed_launch.gd")
const Router = preload("res://app/boot_router.gd")
const Store = preload("res://services/installed_pack_store.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")

var failures: Array[String] = []
var checks: int = 0
var proof_root: String = ""
var save_root: String = ""

func _initialize() -> void:
	_run.call_deferred()

func _run() -> void:
	proof_root = OS.get_environment("T39007_PROOF")
	save_root = TitleContinue.default_root()
	_check(proof_root.is_absolute_path() and OS.get_user_data_dir().begins_with("/userdata/"), "persistent user-data mount and proof directory are active")
	var phase: String = OS.get_environment("T39007_PHASE")
	match phase:
		"write": _write_phase()
		"continue": await _continue_phase()
		"corrupt": _corrupt_phase()
		"recover": await _recover_phase()
		"keyboard": await _keyboard_phase()
		"readonly": _readonly_phase()
		"retry": _retry_phase()
		_: _check(false, "known phase: " + phase)
	var report: Dictionary = {"phase": phase, "checks": checks, "failures": failures}
	var file: FileAccess = FileAccess.open(proof_root.path_join("phase-" + phase + ".json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "  "))
		file.close()
	if failures.is_empty():
		print("PASS: save relaunch phase %s; %d checks" % [phase, checks])
		quit(0)
	else:
		for failure: String in failures:
			printerr("FAIL: ", failure)
		quit(1)

func _write_phase() -> void:
	_check(not TitleContinue.has_save_files(save_root), "first launch starts without save files")
	var versions: Dictionary = Controls.profile_versions()
	_check(versions["error"].is_empty(), "profile identity is configured")
	var store: Save = Save.new(save_root)
	_check(store.save_slot(Controls.SLOT, _snapshot(versions["versions"], 1))["error"].is_empty(), "first session save")
	_check(store.save_slot(Controls.SLOT, _snapshot(versions["versions"], 2))["error"].is_empty(), "second session save keeps a backup")
	# Clear save through the overlay state machine and the production host.
	var host: ClearSave = ClearSave.new(save_root)
	_check(host.load_error.is_empty() and host.record["girl_clear_counters"] == SystemRecords.default_record()["girl_clear_counters"], "fresh system record")
	var seq: Dictionary = _clear_save_prompt(0)
	seq = EndingOverlay.step_graduation_sequence(seq, EndingOverlay.PAD_CIRCLE)
	_check(seq["clear_save_status"] == "requested" and not seq["clear_save_completed"], "yes requests storage without claiming completion")
	# A directory at the staging path is a real write failure.
	DirAccess.make_dir_recursive_absolute(save_root.path_join("system.json.tmp"))
	var failed: Dictionary = host.service_request(seq)
	_check(failed["payload"]["ok"] == false and not FileAccess.file_exists(save_root.path_join("system.json")), "blocked write fails and publishes nothing")
	var applied: Dictionary = EndingOverlay.apply_clear_save_completion(seq, failed["payload"])
	seq = applied["state"]
	_check(applied["error"].is_empty() and seq["clear_save_status"] == "failed" and not seq["clear_save_completed"]
		and seq["clear_save_error"] == failed["payload"]["error"], "failure keeps its reason and stays unsaved")
	seq = EndingOverlay.step_graduation_sequence(seq, 0)
	_check(not seq["completed"] and seq["clear_save_status"] == "failed", "failed clear save waits for acknowledgement")
	DirAccess.remove_absolute(save_root.path_join("system.json.tmp"))
	seq = EndingOverlay.step_graduation_sequence(seq, EndingOverlay.PAD_CIRCLE)
	_check(seq["clear_save_status"] == "prompt" and not seq.has("clear_save_request"), "acknowledging the failure returns to the ask without writing")
	seq = EndingOverlay.step_graduation_sequence(seq, EndingOverlay.PAD_CIRCLE)
	var retried: Dictionary = host.service_request(seq)
	_check(retried["payload"]["ok"] == true, "retry publishes the system record")
	var saved: Dictionary = EndingOverlay.apply_clear_save_completion(seq, retried["payload"])["state"]
	_check(saved["completed"] and saved["clear_save_completed"] and saved["album_clear_mask"] == 1, "saved receipt finishes the ending in the completion step")
	var restepped: Dictionary = EndingOverlay.step_graduation_sequence(saved, 0)
	_check(not restepped["completed"] and not restepped["clear_save_completed"] and restepped["album_clear_mask"] == 0,
		"a stored copy of the finished state is not trusted when stepped again")
	# Source clear_save_write_step increments before each write attempt.
	_check(host.record["girl_clear_counters"][0] == 2, "failed attempt plus retry count twice, as the source does")
	_check(host.verify_receipt(retried["payload"]["receipt"]).is_empty(), "receipt matches the stored record")
	var log: Array = []
	for index: int in range(SystemRecords.ALBUM_CAPACITY):
		log.append([0 if index % 3 == 0 else 5, index & 0xFF, 1, 0, 96, 4, 1 + index % 28, index % 7])
	var album: Dictionary = host.album_save(0, log, [98, 3, 1])
	_check(album["error"].is_empty() and album["count"] == 86, "album save packs only the girl's records")
	_check(not host.album_save(0, [], [98, 3, 1])["error"].is_empty(), "album save without a session event log is rejected")
	_save_proof({"receipt": retried["payload"]["receipt"], "session_checksum": store.load_slot(Controls.SLOT)["checksum_sha256"],
		"backup_checksum": store.load_slot(Controls.SLOT, true)["checksum_sha256"]})

func _continue_phase() -> void:
	var proof: Dictionary = _load_proof()
	_check(TitleContinue.has_save_files(save_root), "relaunch finds save files")
	var store: Store = Store.new("/content")
	var router: Router = Router.new(store)
	var decision: Dictionary = router.check_boot_state()
	_check(decision["action"] == "playable" and decision["target"] == Launch.TITLE_SCENE, "normal launch routes to the title Continue owner: " + str(decision.get("reason")))
	var host_node := Node.new()
	root.add_child(host_node)
	current_scene = host_node
	router.route(host_node)
	await process_frame
	var title: Node = current_scene
	_check(title != null and title.get_script() == TitleContinue, "title owner is the current scene")
	if title == null or title.get_script() != TitleContinue:
		return
	# The captured boundary and rule tables a restored session needs are not
	# installed by any launcher, so the title shows Continue disabled with the
	# reason instead of offering it and failing in the playable app.
	var continue_action: Array = title.actions.filter(func(action: Dictionary) -> bool: return action["id"] == "continue")
	_check(not continue_action.is_empty() and not continue_action[0]["enabled"]
		and continue_action[0]["reason"].contains("captured game state is missing"), "Continue is disabled without the captured inputs: " + str(continue_action))
	_check(title.perform("continue").contains("not available"), "a disabled Continue is refused")
	_continue_preflight_checks()
	_check(title.actions.filter(func(action: Dictionary) -> bool: return action["id"].ends_with("_recover")).is_empty(), "no recovery offered for valid files")
	# Clear state persisted across processes.
	var host: ClearSave = ClearSave.new(save_root)
	_check(host.record["girl_clear_counters"][0] == 2 and host.record["album_state"] == [1, 86], "clear counters and album survive relaunch")
	_check(host.verify_receipt(proof["receipt"]).contains("does not match"), "receipt superseded by the album save is stale")
	var current: Dictionary = ClearSave.receipt_for(host.record, 0, host.records.storage.load_slot(SystemRecords.SLOT)["checksum_sha256"])
	_check(host.verify_receipt(current).is_empty() and current["album_clear_mask"] == 1, "current record yields a valid receipt after relaunch")
	for forged: Dictionary in _forged_receipts(current):
		_check(not host.verify_receipt(forged).is_empty(), "forged receipt is rejected: " + JSON.stringify(forged))
	# A forged saved state with a complete, well-formed receipt and a random
	# checksum must not finish the ending.
	var fabricated: Dictionary = _clear_save_prompt(0)
	fabricated["clear_save_completed"] = true
	fabricated["clear_save_status"] = "saved"
	fabricated["album_clear_mask"] = 0x1FFF
	var random_hex: String = ""
	for index: int in range(8):
		random_hex += "%08x" % (randi() & 0xFFFFFFFF)
	fabricated["clear_save_receipt"] = {"service": "ending.clear_save", "girl": 0, "counter": 1, "album_clear_mask": 0x1FFF, "checksum_sha256": random_hex}
	_check(EndingOverlay.clear_save_receipt_error(fabricated["clear_save_receipt"], 0).is_empty(), "forged receipt is structurally well formed")
	for pad: int in [0, 0x0800, 0x1000, 0x4000]:
		var waited: Dictionary = EndingOverlay.step_graduation_sequence(fabricated, pad)
		_check(not waited["completed"] and not waited["clear_save_completed"] and waited["album_clear_mask"] == 0
			and waited["clear_save_status"] == "prompt" and not waited.has("clear_save_receipt"),
			"forged saved state is discarded; pad 0x%04x is not a clear-save answer" % pad)
	var stepped: Dictionary = EndingOverlay.step_graduation_sequence(fabricated, EndingOverlay.PAD_CIRCLE)
	_check(not stepped["clear_save_completed"] and stepped["album_clear_mask"] == 0 and stepped["clear_save_status"] == "requested", "circle asks for a real save")
	var mismatched: Dictionary = EndingOverlay.apply_clear_save_completion(stepped, {"ok": true, "girl": 0, "attempt": stepped["clear_save_attempts"], "receipt": {}})
	_check(not mismatched["error"].is_empty(), "completion without a receipt is rejected")
	var wrong_girl: Dictionary = proof["receipt"].duplicate()
	wrong_girl["girl"] = 3
	_check(not EndingOverlay.apply_clear_save_completion(stepped, {"ok": true, "girl": 0, "attempt": stepped["clear_save_attempts"], "receipt": wrong_girl})["error"].is_empty(), "receipt for another girl is rejected")
	# Hand-off mechanics only (not production Continue evidence): the playable
	# owner receives the selected save, skips the new-game entry and stops at
	# the same preflight the title reported.
	var error: String = Launch.change_scene(title, Launch.PLAYABLE_SCENE, {}, null, null,
		{"slot": Controls.SLOT, "checksum_sha256": proof["session_checksum"]})
	await process_frame
	var playable: Node = current_scene
	_check(error.is_empty() and playable != null and playable.scene_file_path == Launch.PLAYABLE_SCENE, "hand-off opens the playable app")
	if playable != null and playable.scene_file_path == Launch.PLAYABLE_SCENE:
		_check(playable.continue_request == {"slot": Controls.SLOT, "checksum_sha256": proof["session_checksum"]}, "playable app received the selected save")
		_check(not playable.new_game_boot, "Continue does not run the new-game entry")
		_check(String(playable.failure).contains("development boundary missing"), "playable app refuses for the same missing input: " + String(playable.failure))
		_save_proof_field("continue_playable_failure", playable.failure)

# continue_content_error checks the declared identities and the files behind
# them. Temporary files stand in for the inputs; their own hashes are passed
# as the expected identities, so this tests the check, not real content.
func _continue_preflight_checks() -> void:
	var dir: String = "/tmp/t39007-preflight-" + str(Time.get_ticks_usec())
	DirAccess.make_dir_recursive_absolute(dir)
	_write(dir.path_join("boundary.json"), "boundary".to_utf8_buffer())
	_write(dir.path_join("tables.json"), "tables".to_utf8_buffer())
	var boundary_sha: String = FileAccess.get_sha256(dir.path_join("boundary.json"))
	var tables_sha: String = FileAccess.get_sha256(dir.path_join("tables.json"))
	var saved_env: Dictionary = {}
	for key: String in ["TOKIMEMO_BOUNDARY", "TOKIMEMO_TABLES", "TOKIMEMO_BOUNDARY_SHA", "TOKIMEMO_TABLES_SHA"]:
		saved_env[key] = OS.get_environment(key)
	OS.set_environment("TOKIMEMO_BOUNDARY", dir.path_join("boundary.json"))
	OS.set_environment("TOKIMEMO_TABLES", dir.path_join("tables.json"))
	OS.set_environment("TOKIMEMO_BOUNDARY_SHA", boundary_sha)
	OS.set_environment("TOKIMEMO_TABLES_SHA", tables_sha)
	_check(Controls.continue_content_error(boundary_sha, tables_sha).is_empty(), "preflight passes when both files match their identities")
	_check(not Controls.continue_content_error("0".repeat(64), tables_sha).is_empty(), "preflight rejects a declared identity other than the reviewed one")
	OS.set_environment("TOKIMEMO_TABLES_SHA", "f".repeat(64))
	_check(not Controls.continue_content_error(boundary_sha, "f".repeat(64)).is_empty(), "preflight rejects a 64-character identity the file does not hash to")
	OS.set_environment("TOKIMEMO_TABLES_SHA", tables_sha)
	OS.set_environment("TOKIMEMO_BOUNDARY", dir.path_join("absent.json"))
	_check(not Controls.continue_content_error(boundary_sha, tables_sha).is_empty(), "preflight rejects a missing boundary file")
	for key: String in saved_env:
		if saved_env[key].is_empty():
			OS.unset_environment(key)
		else:
			OS.set_environment(key, saved_env[key])

func _corrupt_phase() -> void:
	var path: String = save_root.path_join("quick.json")
	var bytes: PackedByteArray = FileAccess.get_file_as_bytes(path)
	_write(path, bytes.slice(0, bytes.size() / 2))
	_write(save_root.path_join("quick.json.tmp"), "interrupted".to_utf8_buffer())
	_check(not Save.new(save_root).inspect_slot(Controls.SLOT)["primary"]["state"] == "valid", "session save is damaged on disk")

func _recover_phase() -> void:
	var proof: Dictionary = _load_proof()
	var title: Node = TitleContinue.new()
	root.add_child(title)
	current_scene = title
	await process_frame
	var ids: Array = title.actions.filter(func(action: Dictionary) -> bool: return action["enabled"]).map(func(action: Dictionary) -> String: return action["id"])
	_check("continue" not in ids, "Continue is withheld for a damaged save")
	_check("session_staging" in ids and "session_recover" not in ids, "interrupted files must be set aside before recovery: " + str(ids))
	_check(title.perform("continue") != "", "Continue request is refused while damaged")
	_check(title.perform("session_staging").is_empty(), "interrupted file set aside")
	ids = title.actions.filter(func(action: Dictionary) -> bool: return action["enabled"]).map(func(action: Dictionary) -> String: return action["id"])
	_check("session_recover" in ids, "backup recovery is offered: " + str(ids))
	_check(title.perform("session_recover").is_empty(), "backup recovery succeeds")
	_check(Save.new(save_root).load_slot(Controls.SLOT).get("checksum_sha256") == proof["backup_checksum"], "the recovered save is the backup")
	_check(title.continue_check()["error"].contains("captured game state is missing"), "after recovery only the missing captured inputs withhold Continue")
	_check(FileAccess.file_exists(save_root.path_join("quick.json.invalid-1")) and FileAccess.file_exists(save_root.path_join("quick.json.tmp.interrupted-1")), "damaged and interrupted files are retained")

# Rendered title under X11: Continue is visible but disabled with its reason,
# and keyboard focus starts on the first available action.
func _keyboard_phase() -> void:
	var title: Node = TitleContinue.new()
	root.add_child(title)
	current_scene = title
	for frame: int in range(3):
		await process_frame
	await RenderingServer.frame_post_draw
	var image: Image = root.get_viewport().get_texture().get_image()
	_check(image != null and image.save_png(proof_root.path_join("title-continue.png")) == OK, "title screen rendered and captured")
	var buttons: Array = title.find_children("*", "Button", true, false)
	var continue_button: Array = buttons.filter(func(button: Button) -> bool: return button.text.begins_with("Continue"))
	_check(continue_button.size() == 1 and continue_button[0].disabled and continue_button[0].text.contains("missing"),
		"Continue is shown disabled with its reason: " + (continue_button[0].text if continue_button.size() == 1 else "none"))
	var focused: Control = root.gui_get_focus_owner()
	_check(focused is Button and focused.text == "New Game", "focus starts on New Game: " + (focused.text if focused is Button else "none"))

func _readonly_phase() -> void:
	var host: ClearSave = ClearSave.new(save_root)
	_check(host.load_error.is_empty(), "system record readable on a read-only volume")
	var before: PackedByteArray = FileAccess.get_file_as_bytes(save_root.path_join("system.json"))
	var seq: Dictionary = EndingOverlay.step_graduation_sequence(_clear_save_prompt(0), EndingOverlay.PAD_CIRCLE)
	var service: Dictionary = host.service_request(seq)
	_check(service["payload"]["ok"] == false, "write to a read-only volume fails: " + str(service["payload"].get("error")))
	seq = EndingOverlay.apply_clear_save_completion(seq, service["payload"])["state"]
	_check(seq["clear_save_status"] == "failed" and not EndingOverlay.step_graduation_sequence(seq, 0)["completed"], "ending stays at the clear-save prompt")
	_check(FileAccess.get_file_as_bytes(save_root.path_join("system.json")) == before, "stored record is unchanged")
	seq = EndingOverlay.step_graduation_sequence(seq, EndingOverlay.PAD_CROSS)
	_check(not seq["completed"] and seq["clear_save_status"] == "prompt", "cross acknowledges the failure notice and returns to the ask")
	seq = EndingOverlay.step_graduation_sequence(seq, EndingOverlay.PAD_CROSS)
	_check(seq["completed"] and not seq["clear_save_completed"], "player can decline after a failure")

func _retry_phase() -> void:
	var host: ClearSave = ClearSave.new(save_root)
	_check(host.record["girl_clear_counters"][0] == 2, "failed read-only attempts did not persist")
	var outcome: Dictionary = host.clear_save(0, 1)
	_check(outcome["ok"] and ClearSave.new(save_root).record["girl_clear_counters"][0] == 3, "a later launch saves the clear")

func _clear_save_prompt(girl: int) -> Dictionary:
	return {"phases": ["confession", "staff_roll", "last_photo", "clear_save"], "phase_index": 3, "phase": "clear_save",
		"history": ["confession", "staff_roll", "last_photo", "clear_save"], "current_girl": girl, "confessed_girl": girl,
		"eligible": true, "ending_type": "confession", "stage": 6, "choice_cursor": 0, "clear_save_status": "prompt",
		"clear_save_attempts": 0, "clear_save_completed": false, "album_clear_mask": 0, "completed": false}

func _forged_receipts(receipt: Dictionary) -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	for change: Array in [["counter", 99], ["album_clear_mask", 0x1FFF], ["checksum_sha256", "0".repeat(64)], ["girl", 1], ["service", "other"]]:
		var forged: Dictionary = receipt.duplicate()
		forged[change[0]] = change[1]
		result.append(forged)
	return result

func _snapshot(versions: Dictionary, marker: int) -> Dictionary:
	var snapshot: Dictionary = {"boundary": "stable", "gameplay_state": {"checkpoint_id": "command", "fixture_marker": marker},
		"native_rng": {"state": marker}, "native_clocks": {"logic_iterations": marker}, "queued_service_events": []}
	snapshot.merge(versions)
	return snapshot

func _save_proof(data: Dictionary) -> void:
	var file: FileAccess = FileAccess.open(proof_root.path_join("proof.json"), FileAccess.WRITE)
	file.store_string(JSON.stringify(_integers_to_strings(data)))
	file.close()

func _save_proof_field(key: String, value: Variant) -> void:
	var data: Dictionary = _load_proof()
	data[key] = value
	_save_proof(data)

func _load_proof() -> Dictionary:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(proof_root.path_join("proof.json")))
	_check(parsed is Dictionary, "previous process proof is readable")
	return _strings_to_integers(parsed) if parsed is Dictionary else {}

# JSON numbers come back as floats; integers travel as tagged strings.
func _integers_to_strings(value: Variant) -> Variant:
	if value is int:
		return "i:" + str(value)
	if value is Dictionary:
		var out: Dictionary = {}
		for key: String in value:
			out[key] = _integers_to_strings(value[key])
		return out
	return value

func _strings_to_integers(value: Variant) -> Variant:
	if value is String and value.begins_with("i:"):
		return int(value.substr(2))
	if value is Dictionary:
		var out: Dictionary = {}
		for key: String in value:
			out[key] = _strings_to_integers(value[key])
		return out
	return value

func _write(path: String, bytes: PackedByteArray) -> void:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	file.store_buffer(bytes)
	file.close()

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)
