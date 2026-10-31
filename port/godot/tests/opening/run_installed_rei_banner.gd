extends "res://tests/run_installed_next_arrival_boundary.gd"
## Actual installed entry progression; no injected source state or fake services.
## Each stageS-K frame is written through the v2 checkpoint observer (T-39019):
## /out/checkpoint-receipts.json binds every PNG hash, clock and setting at write time.
const CheckpointObserver = preload("res://tests/support/checkpoint_observer.gd")
var checkpoint_observer := CheckpointObserver.new("/out","res://tests/opening/run_installed_rei_banner.gd")
var rei_frames: Dictionary = {}
var dialogue := OS.get_environment("REI_DIALOGUE") == "1"
## Source substep counts per stage; REI_TARGET_STAGE stops on its first frame.
const SUBSTEPS := {8:8,9:12,10:9,11:7}
var target := int(OS.get_environment("REI_TARGET_STAGE")) if OS.get_environment("REI_TARGET_STAGE").is_valid_int() else (10 if dialogue else 9)
## T-39230: every route built on this one (frame timing, opening trace,
## presentation pacing, input script) is differential or playtest evidence, so
## it fails fast unless Parity mode is active and clean, or MODERN_INVARIANCE=1
## asks for an explicit modern-invariance run. /out/parity-mode.json records
## the result for tools/port/parity_gate.py.
const ParityMode = preload("res://services/parity_mode.gd")
var parity_identity: Dictionary = {}
## A QA observer that applies modern preferences on purpose overrides this to
## return false. It then runs in any mode, and its parity-mode.json says
## gate "not_evidence", which tools/port/parity_gate.py refuses.
func parity_evidence_route() -> bool:
	return true
func _start() -> void:
	var parity := ParityMode.gate(self, "/out", parity_evidence_route())
	parity_identity = parity.identity
	if not parity.ok:
		print("FAIL: parity gate: " + parity.error)
		_failed("parity gate: " + parity.error)
		return
	super._start()
func _frame_published(tick: int, app: App) -> void:
	super._frame_published(tick,app)
	var state = app.consumer.opening.state
	if state.mode_stage >= 8 and state.mode_stage < target:
		var key := "stage%d-%d" % [state.mode_stage,state.substep]
		if not rei_frames.has(key):
			rei_frames[key] = {"frame":state.frame_count,"counter":state.opening.get("prologue_scene_counter"),"opening":state.opening.duplicate(true)}
			_record_checkpoint(key,app,state)
			_status({"phase":key,"frame":state.frame_count})
	if state.mode_stage == target and not finishing:
		finishing = true; app.set_physics_process(false)
		_finish_rei.call_deferred(app)
func _record_checkpoint(key: String, app: App, state) -> void:
	var clocks := {"frame_count":int(state.frame_count),"game_mode":int(state.game_mode),"mode_stage":int(state.mode_stage),
		"mode_step":int(state.mode_step),"substep":int(state.substep)}
	var rng = app.consumer.opening.session.rng
	var receipt := checkpoint_observer.record(key,app.last_rendered_image,"cpu_composed_image",clocks,
		{"draw_count":int(rng.draw_count),"state":int(rng.state)},CheckpointObserver.settings_identity(self),
		{"pack_manifest_sha256":pack_manifest_sha},{"prologue_scene_counter":state.opening.get("prologue_scene_counter"),"opening":state.opening.duplicate(true)})
	check(not receipt.is_empty(),"checkpoint observer refused %s: %s" % [key,checkpoint_observer.errors])
	checkpoint_observer.write()

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
	if app.consumer == null or app.consumer.opening.state == null:
		# The app failed before the opening owned a state (for example a bundle
		# this runtime rejects): no state or race clock to record.
		var early := "Rei observer stopped before the opening: %s: %s" % [app.failure_gate,app.failure]
		var receipt := FileAccess.open("/out/rei-failure.json",FileAccess.WRITE)
		receipt.store_string(JSON.stringify({"message":early,"audio_clock":{"race_clock":{},"sink":app.failure_audio_diagnostics.get("sink",{})}},"\t")); receipt.close()
		_status({"phase":"failed","message":early}); printerr(early); quit(1)
		return
	var state = app.consumer.opening.state
	var message := "Rei observer stopped at %d/%d/%d: %s: %s" % [state.mode_stage,state.mode_step,state.substep,app.failure_gate,app.failure]
	var file := FileAccess.open("/out/rei-failure.json",FileAccess.WRITE)
	# T-39002: the logic-locked Rei clock and native race counters, so the
	# receipt states the Rei-start-to-C1 phase the route actually ran.
	# _fail snapshots the BGM owner into failure_audio_diagnostics, then resets it.
	var bgm: Dictionary = app.failure_audio_diagnostics if not app.failure_audio_diagnostics.is_empty() else (app.bgm.diagnostics() if app.bgm != null else {})
	var engine: Dictionary = bgm.get("engine",{})
	var audio_clock := {"race_clock":bgm.get("race_clock",{}),"shiori":bgm.get("shiori",{}),"active_skips":bgm.get("active_skips",0),
		"sink":bgm.get("sink",{}),"native":{"shiori_pump":engine.get("shiori_pump"),"shiori_landed":engine.get("shiori_landed"),"stopped":engine.get("stopped"),"sequence_gain":engine.get("sequence_gain"),"frames":engine.get("frames"),"kon_log":engine.get("kon_log",[]).filter(func(row): return int(row[1]) > 0)}}
	file.store_string(JSON.stringify({"message":message,"state":state.projection(),"observed":rei_frames,"audio_clock":audio_clock},"\t")); file.close()
	_status({"phase":"failed","message":message}); printerr(message); quit(1)
