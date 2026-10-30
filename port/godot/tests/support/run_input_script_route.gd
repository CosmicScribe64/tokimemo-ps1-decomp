extends "res://tests/opening/run_installed_rei_banner.gd"
## T-39023 (T-39018 WP-06): plays an input-script v1 (INPUT_SCRIPT, a JSON path)
## on the actual installed New Game route. The entry and the opening receive only
## joypad InputEvents from tests/support/input_script_player.gd at game-update
## boundaries; nothing is queued into the entry or injected into the session.
## Writes v2 receipts (checkpoint-receipts.json) for each name-entry step and,
## through the base observer, each Rei stage key, plus transitions.json and
## script-run.json (anchors, delivered edges, failure, expected stops).
## The run ends at REI_TARGET_STAGE (the declared port stop) or on failure.
## Runner: tools/port/run_input_script_route.sh <new-out-dir> <script.json>.
const Player = preload("res://tests/support/input_script_player.gd")
var player: Node
var script_data: Dictionary = {}
var entry_node: Node
var name_steps: Dictionary = {}
var transitions: Dictionary = {}

func _initialize() -> void:
	var path := OS.get_environment("INPUT_SCRIPT")
	var parsed = JSON.parse_string(FileAccess.get_file_as_string(path))
	script_data = parsed if parsed is Dictionary else {}
	player = Player.new()
	var error: String = player.configure(script_data, _observe)
	check(error.is_empty(), "input script %s: %s" % [path, error])
	player.finished.connect(_player_finished)
	node_added.connect(_node_added)
	super._initialize()

func _node_added(node: Node) -> void:
	if entry_node != null or node.get_script() == null or node.get_script().resource_path != "res://app/new_game_entry_boot.gd": return
	entry_node = node
	if player.get_parent() == null: root.add_child.call_deferred(player)

## Post-update state of the last completed iteration, from whichever scene owns the game now.
func _observe() -> Dictionary:
	var app := current_scene as App
	if app != null and app.consumer != null and app.consumer.opening.state != null and not is_instance_valid(app._entry):
		var state = app.consumer.opening.state
		return {"frame_count": int(state.frame_count), "game_mode": int(state.game_mode), "mode_stage": int(state.mode_stage),
			"mode_step": int(state.mode_step), "substep": int(state.substep)}
	if is_instance_valid(entry_node) and entry_node.controller != null:
		var c = entry_node.controller
		_entry_checkpoint(c)
		var names := {}
		for part: String in c.names: names[part] = PackedByteArray(c.names[part]).hex_encode()
		# names/player/shiori feed the script's expect_state (same encoding as the native buffers).
		return {"frame_count": int(entry_node._entry_model.frame_count), "game_mode": 0x14, "mode_stage": int(c.stage),
			"mode_step": int(c.step_id), "substep": 0, "names": names, "player": c.player.duplicate(),
			"shiori": c.shiori.duplicate()}
	return {}

func _entry_checkpoint(c) -> void:
	var key := "name-%d-%d" % [c.stage, c.step_id]
	if name_steps.has(key) or entry_node.last_rendered_image == null: return
	name_steps[key] = int(entry_node._entry_model.frame_count)
	var clocks := {"frame_count":int(entry_node._entry_model.frame_count),"game_mode":0x14,"mode_stage":int(c.stage),"mode_step":int(c.step_id),"substep":0}
	var receipt := checkpoint_observer.record(key,entry_node.last_rendered_image,"cpu_composed_image",clocks,
		{"draw_count":int(entry_node.rng.draw_count),"state":int(entry_node.rng.state)},CheckpointObserver.settings_identity(self),
		{"pack_manifest_sha256":pack_manifest_sha},{"names":entry_node.differential_snapshot().get("state",{}).get("names",{}),"name_pointer":entry_node.differential_snapshot().get("name_pointer",{})})
	checkpoint_observer.write()
	check(not receipt.is_empty(), "entry checkpoint %s not recorded: %s" % [key, checkpoint_observer.errors])

func _player_finished(failure: String) -> void:
	if failure.is_empty() or finishing: return
	finishing = true
	_write_run("input script failure: " + failure)
	print("FAIL: input script stopped: " + failure)
	quit(1)

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	if state != null and state.game_mode == 0x15:
		var key := "%d/%d/%d" % [state.mode_stage,state.mode_step,state.substep]
		if not transitions.has(key): transitions[key] = int(state.frame_count)
	super._frame_published(tick,app)

func _finish_rei(app: App) -> void:
	_write_run("stopped at REI_TARGET_STAGE %d (declared port stop)" % target)
	super._finish_rei(app)

func _finish_boundary(app: App) -> void:
	_write_run("route failure: %s" % app.failure)
	super._finish_boundary(app)

func _write_run(reason: String) -> void:
	var file := FileAccess.open("/out/transitions.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"transitions":transitions},"\t")); file.close()
	var stops: Array = script_data.get("unsupported",[]).filter(func(row): return row.get("side") == "port")
	file = FileAccess.open("/out/script-run.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"schema":"tokimemo.input-script-run.v1","script":script_data.get("name"),"end":reason,
		"anchors":player.anchors,"delivered":player.delivered,"player_failure":player.failure,"player_done":player.done,
		"name_steps":name_steps,"expected_port_stops":stops},"\t"))
	file.close()
	check(player.failure.is_empty(),"input script player failed: " + player.failure)
