extends SceneTree
## Original line0/text/font with explicit profile fixture. No normal-route or
## voiced-line readiness, fake player completion or BGM readiness is claimed.
const Lines = preload("res://services/prologue_line_services.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failures := 0

class ChangedStore extends Store:
	var original: Store
	var defect := ""
	func initial_constants() -> Dictionary:
		return original.initial_constants()
	func resolve(line_id: int, state: Dictionary) -> Dictionary:
		var view := original.resolve(1 if defect == "another_line" else line_id,state)
		if not view.get("error","").is_empty(): return view
		match defect:
			"voiced":
				view.audio_request = true
				view.speaker = 0
			"audio_type": view.audio_request = {}
			"another_line": view.line_id = 0
			"source_control": view.source_raw[0] ^= 1
			"unrenderable": view.rows[0].raw = PackedByteArray([0xff,0xff])
		return view

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func _initialize() -> void:
	var store := Store.new()
	var painter := Painter.new()
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	check(store.open(text_root,"5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc").is_empty(),"original text payload authenticates")
	check(painter.open_font(OS.get_environment("PROLOGUE_FONT_ROOT")).is_empty(),"actual original font opens")
	if failures != 0:
		quit(1)
		return
	var state := State.new()
	state.opening = {"origin":"interactive-entry-owned-state-v1"}
	state.rule_state = {"profile_family_bytes":[130,160,0,0,0,0,0,0],
		"profile_given_bytes":[130,162,0,0,0,0,0,0]}
	var original := state.projection()
	check(not Lines.new().prepare_first_arrival(Store.new(),painter,state).is_empty() and state.projection() == original,"missing original text rejects without profile mutation")
	check(not Lines.new().prepare_first_arrival(store,Painter.new(),state).is_empty() and state.projection() == original,"unprepared font rejects without profile mutation")
	for wrong_origin: Variant in [null,{},"save-load","return-to-title"]:
		var wrong := State.new(); wrong.restore(original); wrong.opening.origin = wrong_origin
		var before := wrong.projection()
		check(not Lines.new().prepare_first_arrival(store,painter,wrong).is_empty() and wrong.projection() == before,"non-entry profile cannot prepare first-arrival line")
	var missing_names := State.new(); missing_names.restore(original); missing_names.rule_state.erase("profile_family_bytes")
	check(not Lines.new().prepare_first_arrival(store,painter,missing_names).is_empty(),"unresolved actual-name substitution rejects")
	for defect: String in ["voiced","audio_type","another_line","source_control","unrenderable"]:
		var changed := ChangedStore.new(); changed.original = store; changed.defect = defect
		var service := Lines.new()
		check(not service.prepare_first_arrival(changed,painter,state).is_empty() and state.projection() == original,"changed source view rejects without model mutation: "+defect)
		check(service.prepare_first_arrival(store,painter,state).is_empty(),"rejected preparation did not claim resources: "+defect)
	var lines := Lines.new()
	check(lines.prepare_first_arrival(store,painter,state).is_empty() and state.projection() == original,"authentic silent line0 resolves actual fixture names without model mutation")
	check(not lines.prepare_first_arrival(store,painter,state).is_empty(),"prepared first-arrival scope cannot be replaced")
	var result := Session.FrameResult.new()
	lines.scene_reset(state,result)
	lines.line_start(state,0,result)
	check(result.error.is_empty() and state.opening.audio.status == "silent" and state.opening.audio.clips.is_empty() and result.audio.is_empty(),"genuine source silence starts text without manufacturing an audio request or completion")
	check(state.opening.prologue_text_work.line_id == 0,"original first line owns numeric text work")
	var later := Session.FrameResult.new()
	var before := state.projection()
	lines.line_start(state,1,later)
	check(not later.error.is_empty() and state.projection() == before and later.audio.is_empty(),"future lines remain explicitly unavailable without their prepared original audio")
	print("PASS: %d first-arrival silent-line checks" % checks if failures == 0 else "FAIL: %d/%d silent-line checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
