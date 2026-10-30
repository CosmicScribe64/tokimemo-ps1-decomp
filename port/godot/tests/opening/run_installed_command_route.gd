extends "res://tests/opening/run_opening_frame_timing_route.gd"
## T-39002 actual installed New Game route to the first command-select frame.
## Same scripted entry as the frame timing route (golden input set A: い/い/い,
## 1/1 A, Shiori 1/1 A; TIMED_ENTRY_PAD=2), then the unskipped opening through
## stage 21, the mode 0x34 stage 0xFF reload and the first committed
## 0x34/1/0 frame with command_ready. Nothing is injected: the observer only
## reads committed state after publication.
## REI_TARGET_STAGE must be 22 (never reached in mode 0x15) so the Rei banner
## observer records every stage 8..21 substep frame as a capture.
## Writes /out/transitions-all.json (every mode/stage/step/substep change),
## /out/captures/<mode>-<stage>-<step>-<substep>.png at each stage boundary,
## /out/states/<key>.json (committed state at each change from stage 21 on),
## /out/command-select.json and /out/command-select.png.
## Runner: ROUTE_OBSERVER=command sh tools/port/run_installed_route.sh <out> ...
var all_transitions: Array = []
var _last_key := ""
var _stage_keys: Dictionary = {}

func _target_ok() -> bool:
	return target == 22

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	var key := "%02x/%d/%d/%d" % [state.game_mode,state.mode_stage,state.mode_step,state.substep]
	if key != _last_key:
		_last_key = key
		all_transitions.append([key,int(state.frame_count),tick])
		var stage_key := "%02x-%d-%d" % [state.game_mode,state.mode_stage,state.mode_step]
		if not _stage_keys.has(stage_key) and (state.game_mode == 0x34 or state.mode_stage >= 12):
			_stage_keys[stage_key] = true
			DirAccess.make_dir_recursive_absolute("/out/captures")
			app.last_rendered_image.save_png("/out/captures/%s.png" % key.replace("/","-"))
			_status({"phase":"route_"+key,"frame":state.frame_count})
		# Committed states from stage 21 on, for the golden a2/a4/a6 comparison
		# (tools/port/compare_newgame_golden.py).
		if state.game_mode == 0x34 or state.mode_stage >= 21:
			DirAccess.make_dir_recursive_absolute("/out/states")
			var dump := FileAccess.open("/out/states/%s.json" % key.replace("/","-"),FileAccess.WRITE)
			dump.store_string(JSON.stringify({"tick":tick,"state":state.projection(),"rule_state":state.rule_state,"rng":_rng_record(app)},"\t",false)); dump.close()
	super._frame_published(tick,app)
	if not finishing and state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0 and bool(state.opening.get("command_ready",false)):
		finishing = true; app.set_physics_process(false)
		_finish_command.call_deferred(app)

func _finish_command(app: App) -> void:
	_check_timing()
	check(app.failure.is_empty(),"command route failed: "+app.failure)
	var state = app.consumer.opening.state
	app.last_rendered_image.save_png("/out/command-select.png")
	var file := FileAccess.open("/out/transitions-all.json",FileAccess.WRITE)
	file.store_string(JSON.stringify(all_transitions,"\t")); file.close()
	file = FileAccess.open("/out/command-select.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"checks":checks,"failed":failed,"tick":app.consumer.opening.session.clock.logic_iterations,
		"state":state.projection(),"rule_state":state.rule_state,"rng":_rng_record(app),
		"original_audio":app.bgm.diagnostics()},"\t",false)); file.close()
	_status({"phase":"passed" if not failed else "failed","checks":checks})
	print(("PASS: actual installed New Game reached command select at frame %d" % state.frame_count) if not failed else "FAIL: command route")
	quit(1 if failed else 0)

func _rng_record(app: App) -> Dictionary:
	var rng = app.consumer.opening.session.rng
	var out := {}
	for name: String in ["seed","state","draw_count"]:
		if name in rng: out[name] = rng.get(name)
	return out

func _finish_boundary(app: App) -> void:
	var file := FileAccess.open("/out/transitions-all.json",FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(all_transitions,"\t")); file.close()
	if app.last_rendered_image != null: app.last_rendered_image.save_png("/out/stop.png")
	super._finish_boundary(app)
