extends "res://tests/opening/run_opening_frame_timing_route.gd"
## T-39002 actual installed New Game playtest past command select. Same
## scripted entry and unskipped opening as run_installed_command_route.gd
## (golden set A, TIMED_ENTRY_PAD=2, REI_TARGET_STAGE=22). On the first
## committed 0x34/1/0 frame with command_ready it records that state
## (/out/states/34-1-0-0.json, the golden a7 pair) and keeps the app running:
## the app hands the session to the school-day controller and the observer
## presses the pad through the app's own input owner (request_button_pulse,
## one sampled frame per press): PLAYTEST_PRESSES ("frames:mask,...", frames
## counted from command select; default RIGHT at 10 and 20, CIRCLE at 30),
## then CIRCLE every PLAYTEST_PRESS_EVERY frames.
## Nothing is injected into the state. The run ends at the app's first
## failure, after PLAYTEST_FRAMES frames past command select, or when
## day_count has advanced PLAYTEST_DAYS days, and writes /out/playtest.json
## (every mode/stage/step/substep change with frame and day, the presses,
## the stop, and the BGM owner's diagnostics as original_audio) plus a capture at every mode change (/out/playtest/).
## A stop on an app failure exits non-zero unless PLAYTEST_EXPECT_STOP names
## that exact stop as "<gate>: <failure>" (the text after "New Game stopped
## at"); when it is set, any other stop, the frame/day limit included, fails.
## PLAYTEST_COMMANDS ("i,j,...", menu cursor indices 0..13) picks one icon at
## each command-select entry in turn: 10 frames after the entry the observer
## moves the cursor the shorter way (RIGHT +1, LEFT -1, wrapping as
## command_screen.select_cursor) one press per 8 frames, then presses CIRCLE.
## While a command select waits for its pick, the periodic CIRCLE is held back.
## When the list is used up, the remaining entries take the hovered icon.
## Runner: ROUTE_OBSERVER=playtest sh tools/port/run_installed_route.sh <out> ...
const GameMemoryDump = preload("res://tests/support/game_memory_dump.gd")
const PAD_CIRCLE := 0x20
var all_transitions: Array = []
var _last_key := ""
var _select_frame := -1
var _select_day := -1
var _presses: Array = []
var _modes_seen: Dictionary = {}
var _limit_frames := int(OS.get_environment("PLAYTEST_FRAMES")) if OS.get_environment("PLAYTEST_FRAMES").is_valid_int() else 6000
var _limit_days := int(OS.get_environment("PLAYTEST_DAYS")) if OS.get_environment("PLAYTEST_DAYS").is_valid_int() else 3
var _first_press := 0
var _scripted: Dictionary = _parse_presses(OS.get_environment("PLAYTEST_PRESSES") if not OS.get_environment("PLAYTEST_PRESSES").is_empty() else "10:8192,20:8192,30:32")
var _press_every := int(OS.get_environment("PLAYTEST_PRESS_EVERY")) if OS.get_environment("PLAYTEST_PRESS_EVERY").is_valid_int() else 30
var _expect_stop := OS.get_environment("PLAYTEST_EXPECT_STOP")
const PAD_RIGHT := 0x2000
const PAD_LEFT := 0x8000
var _commands: Array[int] = _parse_commands(OS.get_environment("PLAYTEST_COMMANDS"))
var _picks: Array = []
var _pick_plan: Dictionary = {}
var _pick_entry := -1

func _target_ok() -> bool:
	return target == 22

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	var key := "%02x/%d/%d/%d" % [state.game_mode,state.mode_stage,state.mode_step,state.substep]
	var day := int(state.rule_state.get("day_count",-1)) if state.rule_state is Dictionary else -1
	if key != _last_key:
		_last_key = key
		all_transitions.append([key,int(state.frame_count),tick,day])
		var stage_key := "%02x-%d-%d" % [state.game_mode,state.mode_stage,state.mode_step]
		if _select_frame < 0 and (state.game_mode == 0x34 or state.mode_stage >= 21):
			DirAccess.make_dir_recursive_absolute("/out/states")
			var dump := FileAccess.open("/out/states/%s.json" % key.replace("/","-"),FileAccess.WRITE)
			dump.store_string(JSON.stringify({"tick":tick,"state":state.projection(),"rule_state":state.rule_state,"rng":_rng_record(app)},"\t",false)); dump.close()
		if _select_frame >= 0 and not _modes_seen.has(stage_key):
			_modes_seen[stage_key] = true
			DirAccess.make_dir_recursive_absolute("/out/playtest")
			if app.last_rendered_image != null: app.last_rendered_image.save_png("/out/playtest/%s.png" % key.replace("/","-"))
	if _select_frame < 0:
		super._frame_published(tick,app)
	if _select_frame < 0 and state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0 and bool(state.opening.get("command_ready",false)):
		_select_frame = int(state.frame_count); _select_day = day
		target = 0x7fffffff
		_check_timing()
		app.last_rendered_image.save_png("/out/command-select.png")
		# T-39300: dev dump of the memory model at this key for tools/port/compare_game_memory.py (read only).
		# Opt in with TOKIMEMO_DUMP_GAME_MEMORY=1: the 2 MiB image holds executable text and stays in ignored data.
		if OS.get_environment("TOKIMEMO_DUMP_GAME_MEMORY") == "1":
			GameMemoryDump.dump(state, "/out", key, int(state.frame_count))
		_status({"phase":"command_select","frame":state.frame_count})
		return
	if _select_frame < 0 or finishing: return
	var since := int(state.frame_count) - _select_frame
	var mask := int(_scripted.get(since,0))
	var at_select: bool = state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0
	if not _commands.is_empty():
		if at_select and _pick_entry < 0:
			_pick_entry = int(state.frame_count)
			_plan_pick(int(state.rule_state.get("menu_cursor",0)))
		elif not at_select:
			_pick_entry = -1
		if at_select:
			mask = int(_pick_plan.get(int(state.frame_count) - _pick_entry,0))
	if mask == 0 and not (at_select and not _commands.is_empty()) and since > _first_press and (since - _first_press) % _press_every == 0: mask = PAD_CIRCLE
	if mask != 0:
		app.consumer.input_owner.request_button_pulse(mask)
		_presses.append([int(state.frame_count),key,mask,int(state.rule_state.get("menu_cursor",-1))])
	if since >= _limit_frames or (day >= 0 and _select_day >= 0 and day - _select_day >= _limit_days):
		finishing = true; app.set_physics_process(false)
		_write_playtest.call_deferred(app,"limit")

func _parse_commands(text: String) -> Array[int]:
	var out: Array[int] = []
	for item: String in text.split(",",false):
		check(item.is_valid_int() and int(item) >= 0 and int(item) < 14,"PLAYTEST_COMMANDS item is a menu index 0..13: "+item)
		if item.is_valid_int(): out.append(int(item))
	return out

## Cursor moves from `cursor` to the next listed pick, then CIRCLE.
func _plan_pick(cursor: int) -> void:
	_pick_plan = {}
	var target_icon: int = _commands[_picks.size()] if _picks.size() < _commands.size() else cursor
	var forward: int = (target_icon - cursor + 14) % 14
	var steps: int = forward if forward <= 7 else 14 - forward
	var direction: int = PAD_RIGHT if forward <= 7 else PAD_LEFT
	for index: int in range(steps):
		_pick_plan[10 + 8 * index] = direction
	_pick_plan[10 + 8 * steps] = PAD_CIRCLE
	_picks.append({"entry_frame":_pick_entry,"from":cursor,"icon":target_icon})

func _parse_presses(text: String) -> Dictionary:
	var out := {}
	for item: String in text.split(",",false):
		var parts := item.split(":")
		check(parts.size() == 2 and parts[0].is_valid_int() and parts[1].is_valid_int(),"PLAYTEST_PRESSES item is frames:mask: "+item)
		if parts.size() == 2:
			out[int(parts[0])] = int(parts[1]); _first_press = maxi(_first_press,int(parts[0]))
	return out

func _rng_record(app: App) -> Dictionary:
	var rng = app.consumer.opening.session.rng
	var out := {}
	for name: String in ["seed","state","draw_count"]:
		if name in rng: out[name] = rng.get(name)
	return out

func _write_playtest(app: App, reason: String) -> void:
	var state = app.consumer.opening.state
	var file := FileAccess.open("/out/playtest.json",FileAccess.WRITE)
	# BGM receipt for tools/port/compare_route_modes.py (FAST pacing); the app's
	# _fail snapshots it into failure_audio_diagnostics before resetting the owner.
	var audio: Dictionary = app.failure_audio_diagnostics if not app.failure_audio_diagnostics.is_empty() else (app.bgm.diagnostics() if app.bgm != null else {})
	file.store_string(JSON.stringify({"original_audio":audio,"reason":reason,"select_frame":_select_frame,"select_day":_select_day,
		"failure_gate":app.failure_gate,"failure":app.failure,"final_key":_last_key,"final_frame":int(state.frame_count),
		"final_day":state.rule_state.get("day_count",-1),"presses":_presses,"picks":_picks,"transitions":all_transitions,
		"final_state":state.projection(),"rule_state":state.rule_state},"\t",false)); file.close()
	if app.last_rendered_image != null: app.last_rendered_image.save_png("/out/playtest-stop.png")
	var stop := "%s: %s" % [app.failure_gate,app.failure] if reason == "app_failure" else ""
	_require(_select_frame >= 0,"playtest never reached command select")
	if _expect_stop.is_empty():
		_require(reason != "app_failure","playtest stopped on an app failure: "+stop)
	else:
		_require(stop == _expect_stop,"playtest stop %s (%s) is not the expected %s" % [reason,stop,_expect_stop])
	print("PLAYTEST stop=%s key=%s frame=%d (select %d) gate=%s failure=%s" % [reason,_last_key,int(state.frame_count),_select_frame,app.failure_gate,app.failure])
	quit(1 if failed else 0)

## Records a failed requirement even when the boundary is already finishing,
## where check() no longer records one.
func _require(ok: bool, message: String) -> void:
	checks += 1
	if ok: return
	failed = true
	printerr(message)

func _finish_boundary(app: App) -> void:
	if _select_frame >= 0:
		_write_playtest(app,"app_failure")
		return
	var file := FileAccess.open("/out/transitions-all.json",FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(all_transitions,"\t")); file.close()
	super._finish_boundary(app)
