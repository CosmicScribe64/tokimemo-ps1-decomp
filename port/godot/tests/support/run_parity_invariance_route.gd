extends "res://tests/support/run_input_script_route.gd"
## T-39230: one profile of the modern-vs-parity invariance run on the actual
## installed New Game route. PARITY_INVARIANCE_PROFILE (parity, modern or
## modern_state_only, see tests/support/parity_invariance.gd) is applied to
## the ModernSettings preferences before the route starts; the input script is
## played through the active mapping. Every completed game iteration appends
## {frame, phase, state, image} to /out/invariance-frames.jsonl: the entry or
## opening state hash and the RGBA8 hash of the native 320x240 Image.
## /out/invariance-profile.json records the profile, its changes and the
## excluded entries with their invariants. The parity profile passes the
## Parity gate; the modern ones need MODERN_INVARIANCE=1.
## Runner: tools/port/run_parity_invariance.sh <new-out-dir> <script.json>.
const Harness = preload("res://tests/support/parity_invariance.gd")
var invariance_profile := OS.get_environment("PARITY_INVARIANCE_PROFILE")
var frames_file: FileAccess
var last_entry_frame := -1
var last_opening_frame := -1

func _start() -> void:
	var settings: Node = root.get_node_or_null("ModernSettings")
	if settings == null:
		_failed("invariance: no ModernSettings autoload"); return
	var error := Harness.apply(settings.preferences, invariance_profile)
	if not error.is_empty():
		_failed("invariance profile %s: %s" % [invariance_profile, error]); return
	player.remaps = settings.remaps
	var record := Harness.profile_record(invariance_profile, FileAccess.get_sha256(OS.get_environment("INPUT_SCRIPT")))
	var file := FileAccess.open("/out/invariance-profile.json", FileAccess.WRITE)
	if file == null:
		_failed("invariance: cannot write /out/invariance-profile.json"); return
	file.store_string(JSON.stringify(record, "\t", true)); file.close()
	frames_file = FileAccess.open("/out/invariance-frames.jsonl", FileAccess.WRITE)
	if frames_file == null:
		_failed("invariance: cannot write /out/invariance-frames.jsonl"); return
	super._start()

func _store(record: Dictionary) -> void:
	if frames_file != null: frames_file.store_line(JSON.stringify(record, "", true))

## Entry iterations: observed once per physics tick before the next one runs.
func _observe() -> Dictionary:
	var state := super._observe()
	if not state.is_empty() and int(state.game_mode) == 0x14 and is_instance_valid(entry_node) and int(state.frame_count) != last_entry_frame:
		last_entry_frame = int(state.frame_count)
		var snapshot: Dictionary = entry_node.differential_snapshot()
		var record := Harness.frame_record(last_entry_frame, "entry", entry_state_hash(snapshot), entry_node.last_rendered_image)
		record["parts"] = entry_state_parts(snapshot)
		_store(record)
	return state

## Game state of the name entry: the entry state, RNG, model frame count and
## name pointer. The snapshot's window geometry (native_canvas), clock policy
## and composed-image digest are presentation or host data and are left out;
## the native Image is compared separately.
static func entry_state_hash(snapshot: Dictionary) -> String:
	return JSON.stringify(_entry_state(snapshot), "", true).sha256_text()

## Per-component hashes, so a divergence names the part that differs.
static func entry_state_parts(snapshot: Dictionary) -> Dictionary:
	var result := {}
	var state := _entry_state(snapshot)
	for key: String in state: result[key] = JSON.stringify(state[key], "", true).sha256_text().substr(0, 16)
	return result

static func _entry_state(snapshot: Dictionary) -> Dictionary:
	var clock: Dictionary = snapshot.get("clock", {})
	return {"state": snapshot.get("state"), "rng": snapshot.get("rng"),
		"model_frame_count": clock.get("model_frame_count"), "name_pointer": snapshot.get("name_pointer"),
		"sampled_input": snapshot.get("sampled_input")}

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	if state != null and int(state.frame_count) != last_opening_frame:
		last_opening_frame = int(state.frame_count)
		_store(Harness.frame_record(last_opening_frame, "opening", app.consumer.opening.session.state_hash(), app.last_rendered_image))
	super._frame_published(tick, app)

func _write_run(reason: String) -> void:
	if frames_file != null: frames_file.close(); frames_file = null
	super._write_run(reason)
