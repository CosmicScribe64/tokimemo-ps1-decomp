extends "res://tests/opening/run_installed_command_playtest.gd"
## Route driver, port side: plays an expanded tokimemo.input-script.v2
## (ROUTE_SCRIPT, a res:// or absolute JSON path) on the actual installed
## New Game route, from the first committed 0x34/1/0 command-select frame on.
##
## The entry and the unskipped opening are the T-39002 command route (golden
## set A timed entry, TIMED_ENTRY_PAD=2, REI_TARGET_STAGE=22), unchanged. From
## command select on, every committed iteration is one boundary: the observer
## reads the published state and the pad mask the app's input owner sampled for
## that iteration, feeds tests/route/route_script_player.gd, and sends the
## edges it returns as joypad InputEvents for the next iteration. Nothing is
## injected into the state, the session or the input owner.
##
## Outputs (/out):
##   route.json             script, anchors, skipped segments, delivered edges,
##                          checkpoints, transitions, the stop and its verdict
##   route-trace.jsonl      one row per boundary (the observed state and pad_held;
##                          tools/emu/route_script.py simulate re-runs the rule on it)
##   checkpoints/<id>.png   the frame published at that boundary (native 320x240)
##   states/<id>.json       {tick, state (projection), rule_state, rng} at that
##                          boundary (the compare_newgame_golden.py input shape)
## The run ends at the first app failure, a player failure, a save_relaunch
## segment (the port has no relaunch capability: T-39007), the end of the
## script, or ROUTE_FRAMES iterations past command select. It passes only when
## the script completed or a port "unsupported" row pins the stop: its "stop"
## equals the stop text and its "at_segment" is the segment the player was on
## (Player.pinned_row), so the same text at another week fails.
## Runner: ROUTE_OBSERVER=long ROUTE_SCRIPT=... sh tools/port/run_installed_route.sh <out> ...
const Player = preload("res://tests/route/route_script_player.gd")
const SAVE_RELAUNCH_REFUSAL := "save_relaunch: installed launches have no Continue capability"
var route_player: Player
var route_script: Dictionary = {}
var route_script_path := OS.get_environment("ROUTE_SCRIPT")
var _route_limit := int(OS.get_environment("ROUTE_FRAMES")) if OS.get_environment("ROUTE_FRAMES").is_valid_int() else 4000000
var _trace: FileAccess
var _route_transitions: Array = []
var _route_key := ""
var _captured: Array = []
var _route_done := false

func _initialize() -> void:
	var parsed = JSON.parse_string(FileAccess.get_file_as_string(route_script_path))
	route_script = parsed if parsed is Dictionary else {}
	route_player = Player.new()
	var error := route_player.configure(route_script)
	check(error.is_empty(), "route script %s: %s" % [route_script_path, error])
	super._initialize()

func _observe(app: App) -> Dictionary:
	var state = app.consumer.opening.state
	var rule: Dictionary = state.rule_state if state.rule_state is Dictionary else {}
	var observed := {"frame_count": int(state.frame_count), "game_mode": int(state.game_mode), "mode_stage": int(state.mode_stage),
		"mode_step": int(state.mode_step), "substep": int(state.substep)}
	for key: String in ["year", "month", "day", "weekday", "day_count", "menu_cursor", "icon_count"]:
		if rule.has(key) and rule[key] is int: observed[key] = int(rule[key])
	if not observed.has("icon_count") and state.opening is Dictionary and state.opening.get("icon_count") is int:
		observed["icon_count"] = int(state.opening.icon_count)
	return observed

func _frame_published(tick: int, app: App) -> void:
	if _select_frame < 0:
		super._frame_published(tick, app)
		if _select_frame < 0: return
		DirAccess.make_dir_recursive_absolute("/out/checkpoints")
		DirAccess.make_dir_recursive_absolute("/out/states")
		_trace = FileAccess.open("/out/route-trace.jsonl", FileAccess.WRITE)
	if finishing or _route_done: return
	var observed := _observe(app)
	var pad_held := int(app.consumer.input_owner._previous) if "_previous" in app.consumer.input_owner else -1
	var key := "%02x/%d/%d/%d" % [observed.game_mode, observed.mode_stage, observed.mode_step, observed.substep]
	if key != _route_key:
		_route_key = key
		_route_transitions.append([key, observed.frame_count, observed.get("day_count", -1)])
	var row := observed.duplicate()
	row["pad_held"] = pad_held
	_trace.store_line(JSON.stringify(row))
	var fired_before := route_player.fired.size()
	var edges := route_player.feed(observed, pad_held)
	for index in range(fired_before, route_player.fired.size()):
		_capture(app, route_player.fired[index], tick)
	Player.deliver(edges)
	if route_player.done:
		route_player.release_all()
		_finish_route(app, "player_failure" if not route_player.failure.is_empty() else ("save_relaunch" if not route_player.stop.is_empty() else "script_complete"))
	elif observed.frame_count - _select_frame >= _route_limit:
		_finish_route(app, "limit")

func _capture(app: App, checkpoint: Dictionary, tick: int) -> void:
	var state = app.consumer.opening.state
	var id := str(checkpoint.id)
	var png := "/out/checkpoints/%s.png" % id
	var sha := ""
	if app.last_rendered_image != null:
		app.last_rendered_image.save_png(png)
		sha = FileAccess.get_sha256(png)
	var dump := FileAccess.open("/out/states/%s.json" % id, FileAccess.WRITE)
	dump.store_string(JSON.stringify({"tick": tick, "checkpoint": checkpoint, "state": state.projection(), "rule_state": state.rule_state,
		"rng": _rng_record(app)}, "\t", false))
	dump.close()
	_captured.append({"id": id, "frame_count": int(checkpoint.frame_count), "png": png.trim_prefix("/out/"), "png_sha256": sha,
		"size": [app.last_rendered_image.get_width(), app.last_rendered_image.get_height()] if app.last_rendered_image != null else []})

func _finish_route(app: App, reason: String) -> void:
	if _route_done: return
	_route_done = true
	finishing = true
	app.set_physics_process(false)
	_write_route.call_deferred(app, reason)

## App failures arrive here (the base observer's boundary hook).
func _finish_boundary(app: App) -> void:
	if _select_frame >= 0:
		_route_done = true
		route_player.release_all()
		_write_route(app, "app_failure")
		return
	super._finish_boundary(app)

func _write_route(app: App, reason: String) -> void:
	var state = app.consumer.opening.state
	var stop := ""
	match reason:
		"app_failure": stop = "%s: %s" % [app.failure_gate, app.failure]
		"player_failure": stop = "input script: " + route_player.failure
		"save_relaunch": stop = SAVE_RELAUNCH_REFUSAL
		"limit": stop = "route frame limit %d" % _route_limit
	var segment_at_stop := route_player.current_segment_id()
	var pin := Player.pinned_row(route_script, "port", stop, segment_at_stop)
	var passed := reason == "script_complete" or not pin.is_empty()
	if app.last_rendered_image != null: app.last_rendered_image.save_png("/out/route-stop.png")
	if _trace != null: _trace.close()
	var file := FileAccess.open("/out/route.json", FileAccess.WRITE)
	file.store_string(JSON.stringify({"schema": "tokimemo.route-run.v1", "side": "port", "script": route_script.get("name"),
		"script_path": route_script_path, "script_sha256": FileAccess.get_sha256(route_script_path),
		"reason": reason, "stop": stop, "stop_pinned": not pin.is_empty(), "passed": passed, "segment_at_stop": segment_at_stop,
		"save_relaunch_segment": route_player.stop, "player_failure": route_player.failure,
		"select_frame": _select_frame, "final_key": _route_key, "final_frame": int(state.frame_count),
		"final_calendar": _observe(app), "segment_index": route_player._index,
		"segments_total": route_player.segments.size(), "anchors": route_player.anchors, "skipped": route_player.skipped, "unsampled": route_player.unsampled,
		"delivered": route_player.delivered, "checkpoints": _captured, "transitions": _route_transitions,
		"final_state": state.projection(), "rule_state": state.rule_state}, "\t", false))
	file.close()
	_require(_select_frame >= 0, "route never reached command select")
	_require(passed, "route stop %s (%s) at segment %s is not a pinned port stop of the script" % [reason, stop, segment_at_stop])
	print("ROUTE stop=%s key=%s frame=%d (select %d) segment=%d/%d (%s) checkpoints=%d stop_text=%s" % [reason, _route_key,
		int(state.frame_count), _select_frame, route_player._index, route_player.segments.size(), segment_at_stop, _captured.size(), stop])
	quit(1 if failed else 0)
