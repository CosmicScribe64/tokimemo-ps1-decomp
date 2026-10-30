extends SceneTree
## T-39007 / WP-10: save -> relaunch x preferences matrix. Each phase is its own
## `godot` process in its own container; phases share only the mounted
## user-data directory and a proof directory. Driver:
## tools/port/run_save_relaunch_tests.sh --settings-matrix.
##
## A phase boots with the preferences the previous phase wrote, so the
## production settings autoload (ModernSettings) applies them at boot exactly as
## a player's relaunch would. The phase checks that the applied cell is the one
## requested, loads every saved checkpoint and compares its state hash and the
## save-file bytes with what the write phase recorded, then writes the
## preferences for the next phase (MATRIX_NEXT_CELL, "language,aspect,filter").
##
## Scope and labels: the saved checkpoints are invented codec fixtures for the
## two checkpoints boundary_save_controls.gd supports ("command",
## "first_result"), stamped with the build's profile ids. The state hash covers
## the restored snapshot (gameplay state, RNG, clocks, queued service events)
## and its checksum; no gameplay iteration runs. Real captured checkpoints and
## the per-iteration trace need the captured boundary inputs, which are not in
## the ignored data available to this test, so the proof lists them as
## unavailable rather than passed. Aspects 16:10 and 21:9 do not exist in the
## settings model yet and are listed as unavailable too.

const Preferences = preload("res://services/user_preferences_service.gd")
const Save = preload("res://services/save_service.gd")
const SystemRecords = preload("res://services/system_record_service.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
const TitleContinue = preload("res://app/title_continue.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const CHECKPOINTS := {"command": "quick", "first_result": "matrix-first-result"}
const FIXTURE_VERSIONS := {"source_version": Controls.SOURCE_PROFILE, "content_version": "p4qa-matrix-fixture",
	"behavior_version": Controls.BEHAVIOR_PROFILE}
## T-39060: aspect and filter reach the picture through NativeDisplayHost
## (NativeFrameLayout); the window scale itself stays pinned to neutral values.
## Each relaunch mounts a live host under the booted ModernSettings and checks
## what it actually presents, not the layout recomputed from the preferences.
const DisplayHost = preload("res://presentation/native_display_host.gd")
const HOST_WINDOW := Vector2(1920, 1080)
## Presented frame size at 1920x1080 per "aspect,filter" (auto is wide at 16:9):
## original 320 columns, wide 2*floor(120*16/9) = 426 columns; integer floors
## the 4.5 factor to 4, nearest/linear keep 4.5.
const EXPECTED_FRAME := {"original,integer": Vector2(1280, 960), "original,nearest": Vector2(1440, 1080), "original,linear": Vector2(1440, 1080),
	"wide,integer": Vector2(1704, 960), "wide,nearest": Vector2(1917, 1080), "wide,linear": Vector2(1917, 1080),
	"auto,integer": Vector2(1704, 960), "auto,nearest": Vector2(1917, 1080), "auto,linear": Vector2(1917, 1080)}
const UNAVAILABLE := ["captured command boundary (TOKIMEMO_BOUNDARY inputs outside .asset-cache)",
	"captured first_result boundary", "per-iteration state/RNG trace after continue", "aspect 16:10", "aspect 21:9"]

var failures: Array[String] = []
var checks := 0
var proof_root := ""
var save_root := ""
var runtime: Node

func _initialize() -> void:
	_run.call_deferred()

func _run() -> void:
	proof_root = OS.get_environment("MATRIX_PROOF")
	save_root = TitleContinue.default_root()
	runtime = root.get_node_or_null("ModernSettings")
	_check(proof_root.is_absolute_path() and OS.get_user_data_dir().begins_with("/userdata/"), "persistent user-data mount and proof directory are active")
	_check(runtime != null, "production settings autoload booted")
	var phase := OS.get_environment("MATRIX_PHASE")
	var cell := OS.get_environment("MATRIX_CELL")
	if runtime != null:
		if phase != "write": await _check_applied(cell)
		match phase:
			"write": _write()
			"relaunch": _relaunch(cell)
			"continue-resave": _continue_resave(cell)
			"clear-write": _clear_write()
			"clear-verify": _clear_verify()
			_: _check(false, "known phase: " + phase)
		var next := OS.get_environment("MATRIX_NEXT_CELL")
		if not next.is_empty(): _set_cell(next)
	var label := phase + ("-" + cell.replace(",", "-") if not cell.is_empty() else "")
	var report := {"phase": phase, "cell": cell, "checks": checks, "failures": failures, "unavailable": UNAVAILABLE,
		"evidence": "T0 synthetic: fixture snapshots; production save/settings services in separate processes"}
	var file := FileAccess.open(proof_root.path_join("matrix-" + label + ".json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "  "))
		file.close()
	if failures.is_empty():
		print("PASS: settings matrix phase %s; %d checks" % [label, checks])
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: ", failure)
		quit(1)

func _write() -> void:
	_check(runtime.first_launch, "matrix starts from a first launch")
	_set_cell("ja,original,integer")
	_check_applied("ja,original,integer")
	var store := Save.new(save_root)
	var states := {}
	for checkpoint: String in CHECKPOINTS:
		var saved: Dictionary = store.save_slot(CHECKPOINTS[checkpoint], _snapshot(checkpoint))
		_check(saved["error"].is_empty(), "fixture %s checkpoint saves" % checkpoint)
		states[checkpoint] = _state_hash(store, CHECKPOINTS[checkpoint])
	_check(states.values().all(func(value: String) -> bool: return value.length() == 64), "every checkpoint has a state hash")
	_negative_controls(store, states)
	_write_proof("states.json", states)
	_write_proof("save-files.json", _save_hashes())

## The comparisons this matrix relies on must be able to fail.
func _negative_controls(store: Save, states: Dictionary) -> void:
	var scratch := Save.new(save_root.path_join("negative-controls"))
	var changed := _snapshot("command")
	changed.native_rng.draw_count += 1
	_check(scratch.save_slot("probe", changed)["error"].is_empty() and _state_hash(scratch, "probe") != states.command,
		"negative control: one extra RNG draw changes the state hash")
	var path := save_root.path_join("negative-controls").path_join("probe.json")
	var bytes := FileAccess.get_file_as_bytes(path)
	bytes[bytes.size() >> 1] ^= 0x01
	var file := FileAccess.open(path, FileAccess.WRITE); file.store_buffer(bytes); file.close()
	_check(not scratch.load_slot("probe")["error"].is_empty(), "negative control: a flipped save byte is rejected, not loaded")
	_check(_cell_matches({"language": {"text": "en"}, "display": {"aspect": "wide", "filter": "linear"}}, "ja,wide,linear") == false,
		"negative control: a language mismatch is detected")
	for name: String in ["probe.json", "probe.json.bak"]:
		DirAccess.remove_absolute(save_root.path_join("negative-controls").path_join(name))
	DirAccess.remove_absolute(save_root.path_join("negative-controls"))
	_check(store.load_slot(CHECKPOINTS.command)["error"].is_empty(), "negative controls left the real saves intact")

static func _cell_matches(effective: Dictionary, cell: String) -> bool:
	var parts := cell.split(",")
	return parts.size() == 3 and effective.language.text == parts[0] and effective.display.aspect == parts[1] and effective.display.filter == parts[2]

func _relaunch(cell: String) -> void:
	_check_states(cell)
	_check(_save_hashes() == _read_proof("save-files.json"), cell + ": loading changed no save file")
	_check_no_preferences_in_saves(cell)

## Continue in this language and save again at the same checkpoint: the saved
## record must be byte-identical, so a ja save continued in en and back is unchanged.
func _continue_resave(cell: String) -> void:
	_check_states(cell)
	var store := Save.new(save_root)
	var before := FileAccess.get_file_as_bytes(save_root.path_join(CHECKPOINTS.command + ".json"))
	var loaded: Dictionary = store.load_slot(CHECKPOINTS.command)
	_check(loaded["error"].is_empty() and store.save_slot(CHECKPOINTS.command, loaded["snapshot"])["error"].is_empty(), cell + ": resave succeeds")
	_check(FileAccess.get_file_as_bytes(save_root.path_join(CHECKPOINTS.command + ".json")) == before, cell + ": resaved record is byte-identical")
	_check(_state_hash(store, CHECKPOINTS.command) == _read_proof("states.json").command, cell + ": state hash unchanged after resave")
	_write_proof("save-files.json", _save_hashes())

func _clear_write() -> void:
	var host := ClearSave.new(save_root)
	_check(host.load_error.is_empty(), "system record loads")
	_check(host.clear_save(0, 1).get("ok") == true, "clear save with English active")
	var log: Array = []
	for index: int in range(SystemRecords.ALBUM_CAPACITY):
		log.append([0 if index % 3 == 0 else 5, index & 0xFF, 1, 0, 96, 4, 1 + index % 28, index % 7])
	var album: Dictionary = host.album_save(0, log, [98, 3, 1])
	_check(album["error"].is_empty() and album["count"] == 86, "album save with English active")
	# JSON numbers come back as floats, so the records travel as their JSON text.
	_write_proof("system.json", {"sha256": FileAccess.get_sha256(save_root.path_join(SystemRecords.SLOT + ".json")),
		"counters": JSON.stringify(host.record["girl_clear_counters"]), "album_state": JSON.stringify(host.record["album_state"])})

func _clear_verify() -> void:
	var expected: Dictionary = _read_proof("system.json")
	var host := ClearSave.new(save_root)
	_check(host.load_error.is_empty(), "system record loads after restart")
	_check(JSON.stringify(host.record["girl_clear_counters"]) == expected.counters and JSON.stringify(host.record["album_state"]) == expected.album_state,
		"clear counters and album state persist across the restart")
	_check(FileAccess.get_sha256(save_root.path_join(SystemRecords.SLOT + ".json")) == expected.sha256, "system record bytes unchanged by the relaunch")
	_check_states("clear-verify")

func _check_applied(cell: String) -> void:
	var parts := cell.split(",")
	_check(parts.size() == 3, "cell is language,aspect,filter: " + cell)
	if parts.size() != 3: return
	var effective: Dictionary = runtime.preferences.effective_settings()
	_check(runtime.preferences.error.is_empty() and runtime.preferences.pending.is_empty(), cell + ": settings booted cleanly")
	_check(_cell_matches(effective, cell), cell + ": the relaunch booted with the requested preferences")
	var window := root.get_window()
	_check(window.content_scale_aspect == Window.CONTENT_SCALE_ASPECT_KEEP and window.content_scale_stretch == Window.CONTENT_SCALE_STRETCH_FRACTIONAL,
		cell + ": the window scale stays neutral")
	var host := DisplayHost.new()
	root.add_child(host)
	host.position = Vector2.ZERO
	host.size = HOST_WINDOW
	var native := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	native.fill(Color8(40, 80, 120))
	_check(host.present(native, 0x34), cell + ": the live host accepts a native frame")
	await process_frame
	await process_frame
	var layout: Dictionary = host.layout
	_check(layout.get("valid", false) and layout.aspect == parts[1] and layout.filter == parts[2], cell + ": the live host applied the relaunched aspect and filter: " + str([layout.get("aspect"), layout.get("filter")]))
	var wide := parts[1] != "original"
	_check(layout.mode == ("wide" if wide else "original"), cell + ": the live host frames the requested aspect at 16:9")
	var expected: Vector2 = EXPECTED_FRAME.get(parts[1] + "," + parts[2], Vector2.ZERO)
	_check(host.display.size == expected and host.display.position == ((HOST_WINDOW - expected) * 0.5).round(),
		cell + ": the live display rect is %s at %s (expected %s)" % [str(host.display.size), str(host.display.position), str(expected)])
	_check(host.presented_image != null and host.presented_image.get_width() == (426 if wide else 320) and host.presented_image.get_height() == 240,
		cell + ": the live host presents the requested frame width")
	_check(host.display.texture_filter == CanvasItem.TEXTURE_FILTER_PARENT_NODE and window.canvas_item_default_texture_filter == (Viewport.DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_LINEAR if parts[2] == "linear" else Viewport.DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_NEAREST),
		cell + ": the live display samples with the relaunched filter")
	root.remove_child(host)
	host.free()

func _set_cell(cell: String) -> void:
	var parts := cell.split(",")
	if parts.size() != 3:
		_check(false, "next cell is language,aspect,filter: " + cell)
		return
	var preferences: Preferences = runtime.preferences
	if preferences.settings.language.text != parts[0]:
		_check(preferences.set_value("language", "text", parts[0]), "language preference for the next boot")
	var display: Dictionary = preferences.settings.display.duplicate(true)
	display.aspect = parts[1]; display.filter = parts[2]
	if display != preferences.settings.display:
		_check(preferences.begin_display(display, 0.0) and preferences.confirm_display(), "display preference for the next boot")

func _check_states(label: String) -> void:
	var expected: Dictionary = _read_proof("states.json")
	var store := Save.new(save_root)
	for checkpoint: String in CHECKPOINTS:
		_check(_state_hash(store, CHECKPOINTS[checkpoint]) == expected.get(checkpoint, ""), "%s: %s restores the identical state hash" % [label, checkpoint])

func _check_no_preferences_in_saves(label: String) -> void:
	var directory := DirAccess.open(save_root)
	for name: String in directory.get_files() if directory != null else PackedStringArray():
		var text := FileAccess.get_file_as_string(save_root.path_join(name))
		_check(not text.contains("\"aspect\"") and not text.contains("\"filter\"") and not text.contains("\"text_manifest\""),
			"%s: %s carries no desktop preference" % [label, name])

func _state_hash(store: Save, slot: String) -> String:
	var loaded: Dictionary = store.load_slot(slot)
	if not loaded["error"].is_empty(): return "load failed: " + loaded["error"]
	return (JSON.stringify(loaded["snapshot"], "", true) + "|" + str(loaded["checksum_sha256"])).sha256_text()

func _snapshot(checkpoint: String) -> Dictionary:
	var snapshot := {"boundary": "stable", "gameplay_state": {"checkpoint_id": checkpoint, "fixture_marker": checkpoint.length()},
		"native_rng": {"state": 0x1234 + checkpoint.length(), "draw_count": 77}, "native_clocks": {"logic_iterations": 163},
		"queued_service_events": []}
	snapshot.merge(FIXTURE_VERSIONS)
	return snapshot

func _save_hashes() -> Dictionary:
	var hashes := {}
	var directory := DirAccess.open(save_root)
	if directory == null: return hashes
	for name: String in directory.get_files():
		hashes[name] = FileAccess.get_sha256(save_root.path_join(name))
	return hashes

func _write_proof(name: String, data: Variant) -> void:
	var file := FileAccess.open(proof_root.path_join(name), FileAccess.WRITE)
	file.store_string(JSON.stringify(data))
	file.close()

func _read_proof(name: String) -> Variant:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(proof_root.path_join(name)))
	_check(parsed != null, "proof %s is readable" % name)
	return parsed if parsed != null else {}

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition: failures.append(message)
