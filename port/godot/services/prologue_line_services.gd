extends RefCounted
## T-29025: source text work feeds the separately owned original XA rules.
## Preparation loads/authenticates resources; callbacks only mutate model/result.
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Work = preload("res://rules/prologue_text_work.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
var _store: Store
var _painter: Painter
var _audio_lines: Dictionary = {}
const FIRST_LINE_SOURCE_SHA := "edc80485d120f5ebd432e9001ae3dba41c5a0d96c33efbaa2564fdd45edce5cd"

func prepare_first_arrival(store: Store, painter: Painter, state: State) -> String:
	# This finite path authenticates genuine player line0 silence after the
	# actual profile handoff. It never substitutes silence for a voiced line.
	if _store != null: return "prologue line resources already own their prepared scope"
	if store == null or painter == null or state == null or store.initial_constants().is_empty() or not state.opening.get("origin") is String or state.opening.origin != "interactive-entry-owned-state-v1":
		return "first-arrival line needs original text/font and the actual interactive profile"
	var view := store.resolve(0,state.rule_state)
	if not str(view.get("error","unprepared")).is_empty(): return str(view.get("error","first-arrival source text is unavailable"))
	if not view.get("line_id") is int or view.line_id != 0 or not view.get("speaker") is int or view.speaker != 15 or not view.get("audio_request") is bool or view.audio_request or not view.get("source_raw") is PackedByteArray or Store._sha(view.source_raw) != FIRST_LINE_SOURCE_SHA:
		return "first-arrival line must be the authenticated original silent player line0"
	var error: String = painter.preflight(view)
	if not error.is_empty(): return error
	_store = store
	_painter = painter
	_audio_lines = {0:{"line_id":0,"clips":[]}}
	return ""

func prepare(store: Store, painter: Painter, audio: Audio) -> String:
	if _store != null:
		return "prologue line resources are immutable; create a new service for another pack"
	if store == null or painter == null or audio == null or store.initial_constants().is_empty():
		return "prologue line services need prepared original text, font and audio resources"
	var ids: Array = range(43)
	ids.append_array([100,101])
	var lines: Dictionary = {}
	for line: int in ids:
		var prepared: Dictionary = audio.prepare_original_line(line)
		if not str(prepared.get("error", "")).is_empty():
			return str(prepared.error)
		if prepared.get("line_id") != line or not prepared.get("clips") is Array:
			return "prepared original audio line does not match the source identity"
		lines[line] = prepared.duplicate(true)
	_store = store
	_painter = painter
	_audio_lines = lines
	return ""

func callbacks() -> Dictionary:
	return {"line_start":line_start,"line_run":line_run,"text_scene_reset":scene_reset}

func scene_reset(state: State, result: Session.FrameResult) -> void:
	if _store == null:
		result.error = "original prologue line services are not prepared"
		return
	result.error = Work.reset(state)

func line_start(state: State, line: int, result: Session.FrameResult) -> void:
	if _store == null or not _audio_lines.has(line):
		result.error = "original prologue line is unavailable"
		return
	var view: Dictionary = _store.resolve(line,state.rule_state)
	if not str(view.get("error", "")).is_empty():
		result.error = str(view.error)
		return
	result.error = _painter.preflight(view)
	if not result.error.is_empty():
		return
	var prepared: Dictionary = _audio_lines[line]
	if (not view.audio_request and not prepared.clips.is_empty()) or (view.audio_request and prepared.clips.is_empty()):
		result.error = "original prologue speaker/text and audio ownership differ"
		return
	for clip: Dictionary in prepared.clips:
		if clip.get("speaker") != view.speaker or clip.get("voice_id") != line:
			result.error = "original prologue clip is bound to another speaker/line"
			return
	result.error = Work.begin_line(state,view)
	if not result.error.is_empty():
		return
	AudioRules.begin_line(state,prepared,result)
	state.rule_state["current_girl"] = view.speaker
	result.presentation.append({"kind":"opening_text_line","line_id":line,"source_payload_sha256":Store.PAYLOAD_SHA})

func line_run(state: State, result: Session.FrameResult) -> void:
	Work.advance(state,result)
	if not result.error.is_empty():
		return
	AudioRules.run_line(state,result)
	if result.error.is_empty():
		result.presentation.append({"kind":"opening_text_work","work":state.opening.prologue_text_work.duplicate(true)})

func resolved_view(state: State) -> Dictionary:
	if _store == null or not state.opening.get("prologue_text_work") is Dictionary:
		return {"error":"prologue text has no committed line view"}
	return _store.resolve(int(state.opening.prologue_text_work.line_id),state.rule_state)
