extends RefCounted
## T-29025: source text work feeds the separately owned original XA rules and
## exact PS1 getCDlevel() 4-sample ring buffer lip-sync engine (prologue.c).
## Preparation loads/authenticates resources; callbacks only mutate model/result.
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Work = preload("res://rules/prologue_text_work.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const VoicePlayback = preload("res://services/original_voice_playback.gd")
var _store: Store
var _painter: Painter
var _audio_lines: Dictionary = {}
var _line_clip_levels: Dictionary = {}
var _clip_cd_levels: Dictionary = {}
var _voiced_scope_prepared: bool = false
const FIRST_LINE_SOURCE_SHA := "edc80485d120f5ebd432e9001ae3dba41c5a0d96c33efbaa2564fdd45edce5cd"
const SECOND_LINE_SOURCE_SHA := "4b7c55b053cb7207e17b56c22e9a04df188a6d07e56c0b5b1952109c83008540"
const THIRD_LINE_SOURCE_SHA := "3da19875be828b67b915e68b43a9fe380b508341f9453f9e753ac9fd16bb3739"
const FOURTH_LINE_SOURCE_SHA := "d9b2ceee2cc1367d4b9b33061d18913f2dcdf89b63e7581ef493e4b09463131b"
const FIFTH_LINE_SOURCE_SHA := "613ab0d0b6772a93f07cf1ad72e76452d8c0ebe45f872a416f4a91063c13a5e1"
const SILENT_PLAYER_LINE_SHAS := {
	0: FIRST_LINE_SOURCE_SHA,
	1: SECOND_LINE_SOURCE_SHA,
	2: THIRD_LINE_SOURCE_SHA,
	3: FOURTH_LINE_SOURCE_SHA,
	4: FIFTH_LINE_SOURCE_SHA,
}

func prepare_first_arrival(store: Store, painter: Painter, state: State) -> String:
	# This finite path authenticates genuine player line0..line4 silence after
	# the actual profile handoff. It never substitutes silence for a voiced line.
	if _store != null: return "prologue line resources already own their prepared scope"
	if store == null or painter == null or state == null or store.initial_constants().is_empty() or not state.opening.get("origin") is String or state.opening.origin != "interactive-entry-owned-state-v1":
		return "first-arrival line needs original text/font and the actual interactive profile"
	var audio_lines: Dictionary = {}
	for line_id: int in [0,1,2,3,4]:
		var view := store.resolve(line_id,state.rule_state)
		if not str(view.get("error","unprepared")).is_empty(): return str(view.get("error","first-arrival source text is unavailable"))
		if not view.get("line_id") is int or view.line_id != line_id or not view.get("speaker") is int or view.speaker != 15 or not view.get("audio_request") is bool or view.audio_request or not view.get("source_raw") is PackedByteArray or Store._sha(view.source_raw) != SILENT_PLAYER_LINE_SHAS[line_id]:
			return "first-arrival line must be the authenticated original silent player line%d" % line_id
		var error: String = painter.preflight(view)
		if not error.is_empty(): return error
		audio_lines[line_id] = {"line_id":line_id,"clips":[]}
	_store = store
	_painter = painter
	_audio_lines = audio_lines
	_voiced_scope_prepared = false
	return ""

func prepare(store: Store, painter: Painter, audio: Audio) -> String:
	if _store != null:
		return "prologue line resources are immutable; create a new service for another pack"
	if store == null or painter == null or audio == null or store.initial_constants().is_empty():
		return "prologue line services need prepared original text, font and audio resources"
	var ids: Array = range(43)
	ids.append_array([100,101])
	var lines: Dictionary = {}
	var line_levels: Dictionary = {}
	var clip_levels_by_key: Dictionary = {}
	for line: int in ids:
		var prepared: Dictionary = audio.prepare_original_line(line)
		if not str(prepared.get("error", "")).is_empty():
			return str(prepared.error)
		if prepared.get("line_id") != line or not prepared.get("clips") is Array:
			return "prepared original audio line does not match the source identity"
		var per_clip_levels: Array = []
		for clip: Dictionary in prepared["clips"]:
			var req_key: String = str(clip.get("request_key", ""))
			var levels: Array[int] = []
			if clip_levels_by_key.has(req_key):
				levels = clip_levels_by_key[req_key]
			else:
				var resolved: Dictionary = audio.resolve_original_audio_event(clip)
				if not str(resolved.get("error", "")).is_empty():
					return str(resolved["error"])
				var stream: AudioStreamWAV = resolved.get("stream")
				levels = VoicePlayback.compute_stream_cd_levels(stream)
				if levels.is_empty():
					return "prepared original audio clip produced empty getCDlevel() envelope: " + req_key
				clip_levels_by_key[req_key] = levels
			per_clip_levels.append(levels)
		lines[line] = prepared.duplicate(true)
		line_levels[line] = per_clip_levels
	AudioRules.prepare_mid_prologue_bank_roles(audio)
	_store = store
	_painter = painter
	_audio_lines = lines
	_line_clip_levels = line_levels
	_clip_cd_levels = clip_levels_by_key
	_voiced_scope_prepared = true
	return ""

func callbacks() -> Dictionary:
	return {"line_start":line_start,"line_run":line_run,"text_scene_reset":scene_reset}

func scene_reset(state: State, result: Session.FrameResult) -> void:
	if _store == null:
		result.error = "original prologue line services are not prepared"
		return
	result.error = Work.reset(state)
	if result.error.is_empty() and _voiced_scope_prepared:
		AudioRules.reset_lip_sync(state)

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
	if _voiced_scope_prepared:
		AudioRules.begin_line_lip_sync(state, line, int(view.speaker), _line_clip_levels.get(line, []))
	state.rule_state["current_girl"] = view.speaker
	result.presentation.append({"kind":"opening_text_line","line_id":line,"source_payload_sha256":Store.PAYLOAD_SHA})

func line_run(state: State, result: Session.FrameResult) -> void:
	Work.advance(state,result)
	if not result.error.is_empty():
		return
	if _voiced_scope_prepared:
		AudioRules.step_lip_sync(state)
	AudioRules.run_line(state,result)
	if result.error.is_empty():
		result.presentation.append({"kind":"opening_text_work","work":state.opening.prologue_text_work.duplicate(true)})

func resolved_view(state: State) -> Dictionary:
	if _store == null or not state.opening.get("prologue_text_work") is Dictionary:
		return {"error":"prologue text has no committed line view"}
	return _store.resolve(int(state.opening.prologue_text_work.line_id),state.rule_state)

func clip_cd_levels(line_id: int, clip_index: int = 0) -> Array[int]:
	var clips: Array = _line_clip_levels.get(line_id, [])
	if clip_index < 0 or clip_index >= clips.size() or not clips[clip_index] is Array:
		return []
	var typed: Array[int] = []
	typed.assign(clips[clip_index])
	return typed

func line_speaker_track(line_id: int) -> String:
	var prepared: Dictionary = _audio_lines.get(line_id, {})
	var clips: Array = prepared.get("clips", [])
	var spk: int = int(clips[0]["speaker"]) if not clips.is_empty() else -1
	return AudioRules.speaker_track_for_line(line_id, spk)

func lip_sync_step(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.step_lip_sync(state, cd_level_override)

func lip_sync_yoshio(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_yoshio(state, cd_level_override)

func lip_sync_line_11(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_line_11(state, cd_level_override)

func lip_sync_girl_a(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_girl_a(state, cd_level_override)

func lip_sync_girl_b(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_girl_b(state, cd_level_override)

func lip_sync_rei(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_rei(state, cd_level_override)

func lip_sync_rei_lines_13_and_14(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_rei_lines_13_and_14(state, cd_level_override)

func lip_sync_lines_15_and_16(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_lines_15_and_16(state, cd_level_override)

func lip_sync_shiori(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_shiori(state, cd_level_override)

func lip_sync_shiori_line_19(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_shiori_line_19(state, cd_level_override)

func lip_sync_lines_20_to_31(state: State, cd_level_override: int = -1) -> Dictionary:
	return AudioRules.lip_sync_lines_20_to_31(state, cd_level_override)

func prologue_shiori_birthday_speak(state: State, result: Session.FrameResult) -> void:
	state.opening["prologue_dynamic_line"] = "shiori_birthday"
	line_start(state, 100, result)

func prologue_shiori_blood_type_speak(state: State, result: Session.FrameResult) -> void:
	state.opening["prologue_dynamic_line"] = "shiori_blood"
	line_start(state, 101, result)
