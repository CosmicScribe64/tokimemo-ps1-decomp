extends "res://tests/run_installed_next_arrival_boundary.gd"
## Actual installed entry progression; no injected source state or fake services.
var rei_frames: Dictionary = {}
var dialogue := OS.get_environment("REI_DIALOGUE") == "1"
func _frame_published(tick: int, app: App) -> void:
	super._frame_published(tick,app)
	var state = app.consumer.opening.state
	if state.mode_stage == 8 or (dialogue and state.mode_stage == 9):
		var key := "stage%d-%d" % [state.mode_stage,state.substep]
		if not rei_frames.has(key):
			rei_frames[key] = {"frame":state.frame_count,"counter":state.opening.get("prologue_scene_counter"),"opening":state.opening.duplicate(true)}
			app.last_rendered_image.save_png("/out/"+key+".png")
			_status({"phase":key,"frame":state.frame_count})
	if state.mode_stage == (10 if dialogue else 9) and not finishing:
		finishing = true; app.set_physics_process(false)
		_finish_rei.call_deferred(app)
func _finish_rei(app: App) -> void:
	for step: int in 8: check(rei_frames.has("stage8-%d" % step),"missing actual stage8 substep%d" % step)
	if dialogue:
		for step: int in 12: check(rei_frames.has("stage9-%d" % step),"missing actual stage9 substep%d" % step)
	check(app.failure.is_empty(),"Rei transition failed")
	var audio := app.bgm.diagnostics()
	check(audio.get("engine",{}).get("rei_started") == true and audio.get("engine",{}).get("sequence_gain") == 83,"Actual Rei route did not publish original alternate sequence")
	check(app.entry_frame_provenance.get("source") == "actual-displayed-native-entry-image","Native entry image handoff lost its source provenance")
	app.last_rendered_image.save_png("/out/stage9-entrance.png")
	var file := FileAccess.open("/out/rei-proof.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"checks":checks,"failed":failed,"states":rei_frames,"final":app.consumer.opening.state.projection(),"original_audio":audio,"entry_image":app.entry_frame_provenance},"\t")); file.close()
	_status({"phase":"passed" if not failed else "failed","checks":checks})
	print("PASS: actual unskipped installed Rei stage7-to9 progression" if not failed else "FAIL: Rei progression")
	quit(1 if failed else 0)
func _finish_boundary(app: App) -> void:
	var state = app.consumer.opening.state
	var message := "Rei observer stopped at %d/%d/%d: %s: %s" % [state.mode_stage,state.mode_step,state.substep,app.failure_gate,app.failure]
	var file := FileAccess.open("/out/rei-failure.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"message":message,"state":state.projection(),"observed":rei_frames},"\t")); file.close()
	_status({"phase":"failed","message":message}); printerr(message); quit(1)
