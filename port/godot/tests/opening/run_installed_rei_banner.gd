extends "res://tests/run_installed_next_arrival_boundary.gd"
## Actual installed entry progression; no injected source state or fake services.
var rei_frames: Dictionary = {}
var dialogue := OS.get_environment("REI_DIALOGUE") == "1"
## Source substep counts per stage; REI_TARGET_STAGE stops on its first frame.
const SUBSTEPS := {8:8,9:12,10:9,11:7}
var target := int(OS.get_environment("REI_TARGET_STAGE")) if OS.get_environment("REI_TARGET_STAGE").is_valid_int() else (10 if dialogue else 9)
func _frame_published(tick: int, app: App) -> void:
	super._frame_published(tick,app)
	var state = app.consumer.opening.state
	if state.mode_stage >= 8 and state.mode_stage < target:
		var key := "stage%d-%d" % [state.mode_stage,state.substep]
		if not rei_frames.has(key):
			rei_frames[key] = {"frame":state.frame_count,"counter":state.opening.get("prologue_scene_counter"),"opening":state.opening.duplicate(true)}
			app.last_rendered_image.save_png("/out/"+key+".png")
			_status({"phase":key,"frame":state.frame_count})
	if state.mode_stage == target and not finishing:
		finishing = true; app.set_physics_process(false)
		_finish_rei.call_deferred(app)
func _finish_rei(app: App) -> void:
	for stage: int in range(8,target):
		for step: int in SUBSTEPS.get(stage,0): check(rei_frames.has("stage%d-%d" % [stage,step]),"missing actual stage%d substep%d" % [stage,step])
	check(SUBSTEPS.has(target-1),"observer has no source substep count before stage%d" % target)
	check(app.failure.is_empty(),"Rei transition failed")
	var audio := app.bgm.diagnostics()
	# Stage11/substep2 starts the Shiori bank (VAB byte 0x18 126 - 25).
	var gain := 101 if target >= 12 else 83
	check(audio.get("engine",{}).get("rei_started") == true and audio.get("engine",{}).get("sequence_gain") == gain,"Actual Rei route did not publish original alternate sequence: "+str(audio.get("engine",{}).get("sequence_gain")))
	check(app.entry_frame_provenance.get("source") == "actual-displayed-native-entry-image","Native entry image handoff lost its source provenance")
	app.last_rendered_image.save_png("/out/stage%d-entrance.png" % target)
	var file := FileAccess.open("/out/rei-proof.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"checks":checks,"failed":failed,"states":rei_frames,"final":app.consumer.opening.state.projection(),"original_audio":audio,"entry_image":app.entry_frame_provenance},"\t")); file.close()
	_status({"phase":"passed" if not failed else "failed","checks":checks})
	print(("PASS: actual unskipped installed Rei progression to stage%d" % target) if not failed else "FAIL: Rei progression")
	quit(1 if failed else 0)
func _finish_boundary(app: App) -> void:
	var state = app.consumer.opening.state
	var message := "Rei observer stopped at %d/%d/%d: %s: %s" % [state.mode_stage,state.mode_step,state.substep,app.failure_gate,app.failure]
	var file := FileAccess.open("/out/rei-failure.json",FileAccess.WRITE)
	# T-39002: the logic-locked Rei clock and native race counters, so the
	# receipt states the Rei-start-to-C1 phase the route actually ran.
	# _fail snapshots the BGM owner into failure_audio_diagnostics, then resets it.
	var bgm: Dictionary = app.failure_audio_diagnostics if not app.failure_audio_diagnostics.is_empty() else (app.bgm.diagnostics() if app.bgm != null else {})
	var engine: Dictionary = bgm.get("engine",{})
	var audio_clock := {"race_clock":bgm.get("race_clock",{}),"shiori":bgm.get("shiori",{}),"active_skips":bgm.get("active_skips",0),
		"native":{"shiori_pump":engine.get("shiori_pump"),"shiori_landed":engine.get("shiori_landed"),"stopped":engine.get("stopped"),"sequence_gain":engine.get("sequence_gain"),"frames":engine.get("frames"),"kon_log":engine.get("kon_log",[]).filter(func(row): return int(row[1]) > 0)}}
	file.store_string(JSON.stringify({"message":message,"state":state.projection(),"observed":rei_frames,"audio_clock":audio_clock},"\t")); file.close()
	_status({"phase":"failed","message":message}); printerr(message); quit(1)
