extends SceneTree
## Actual text/font/PCM preparation; invented completion schedule, no route claim.
const Lines = preload("res://services/prologue_line_services.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failures := 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ",label)

func complete(state: State, status: String) -> String:
	var audio: Dictionary = state.opening.audio
	var clip: Dictionary = audio.clips[audio.clip_index]
	var event := Session.ServiceEvent.new(state.frame_count,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION)
	event.phase = 0
	event.service = "opening.audio"
	event.payload = {"epoch":audio.epoch,"line_id":audio.line_id,"clip_index":audio.clip_index,
		"request_key":clip.request_key,"status":status,"completion_index":audio.completion_index+1}
	var result := Session.FrameResult.new()
	AudioRules.complete(event,state,result)
	return result.error

func _initialize() -> void:
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	var audio_root := OS.get_environment("PROLOGUE_AUDIO_ROOT")
	var store := Store.new()
	var painter := Painter.new()
	var audio := Audio.new()
	check(store.open(text_root,FileAccess.get_sha256(text_root.path_join("manifest.json"))).is_empty(),"actual source text opens")
	check(painter.open_font(OS.get_environment("PROLOGUE_FONT_ROOT")).is_empty(),"actual original font opens")
	check(audio.configure_original_opening_audio_pack(audio_root,FileAccess.get_sha256(audio_root.path_join("opening-audio.json"))).is_empty(),"actual original PCM pack opens")
	var lines := Lines.new()
	check(lines.prepare(store,painter,audio).is_empty(),"all source text/font/audio identities prepare")
	check(not lines.prepare(store,painter,audio).is_empty(),"prepared resources cannot be replaced")
	if failures != 0:
		quit(1)
		return
	var identities: Array = range(43)
	identities.append_array([100,101])
	var voiced := 0
	var clips := 0
	for identity: int in identities:
		var state := State.new()
		state.rule_state = {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],
			"girls":[{"birthday":12|(31<<4)|(3<<9)}],"rng_state":12345,"rng_draw_count":7}
		var reset := Session.FrameResult.new()
		lines.scene_reset(state,reset)
		check(reset.error.is_empty(),"source scene reset")
		var start := Session.FrameResult.new()
		lines.line_start(state,identity,start)
		check(start.error.is_empty(),"source line starts: "+str(identity)+" "+start.error)
		if not start.error.is_empty():
			continue
		var clip_count: int = state.opening.audio.clips.size()
		voiced += 1 if clip_count > 0 else 0
		clips += clip_count
		check(start.audio.size() == (1 if clip_count > 0 else 0),"one primary request or silent source line")
		var holds := 0
		var finished := false
		var saw_clear := false
		for frame in range(1,12001):
			state.frame_count = frame
			if clip_count > 0:
				# Synthetic acknowledgements exercise ownership, not playback duration.
				if state.opening.audio.status == "requested":
					check(complete(state,"started").is_empty(),"matching clip start consumed")
				elif state.opening.audio.status == "playing" and frame % 10 == 0:
					check(complete(state,"finished").is_empty(),"matching clip finish consumed")
			var tick := Session.FrameResult.new()
			lines.line_run(state,tick)
			if not tick.error.is_empty():
				check(false,"combined line failed: "+str(identity)+" "+tick.error)
				break
			if state.opening.prologue_text.k_disp_inc_result == 1 and (clip_count == 0 or (state.opening.audio.status == "finished" and state.opening.audio.clip_index+1 == clip_count)):
				holds += 1
				if holds == 2:
					saw_clear = state.opening.prologue_text_work.scroll_state == 0
			if state.substep == 1:
				finished = true
				break
		check(finished and holds == 61 and state.mode_step_timer == 0,"exact audio-owned 61 tick post-text hold: "+str(identity))
		check(saw_clear,"audio scroll clear feeds next numeric text iteration")
		check(state.rule_state.rng_state == 12345 and state.rule_state.rng_draw_count == 7,"line work preserves owned RNG")
		check(lines.resolved_view(state).line_id == identity,"committed numeric work resolves original view")
	check(voiced == 23 and clips == 24,"original 23 voiced lines and 24 clips retained")
	print("PASS: %d combined original text/audio checks" % checks if failures == 0 else "FAIL: %d/%d combined checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
