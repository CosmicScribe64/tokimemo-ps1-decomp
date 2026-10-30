extends SceneTree
## T-39002: XA voice progression on logic frames. Invented clip identities;
## the timing constants are checked against the emulator measurements in
## .asset-cache/emu/opening-timing-v2 (shiori-landing-m2 runs) and the main-loop
## iteration traces in .asset-cache/emu/opening-timing-v4 (fcb-start20/33).
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failures := 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: " + label)

func _clip(span: int, start := 275902) -> Dictionary:
	return {"kind":"voice_play","voice_id":12,"speaker":12,"request_key":"invented-%d" % span,
		"xa_request":{"source_sha256":"ab".repeat(32),"source_lba":0,"start_lba":start,"end_lba":start + span,"file_number":2,"channel":22,"native_evidence":"invented"}}

func _begin(state: State, line: int, clips: Array) -> Session.FrameResult:
	var result := Session.FrameResult.new()
	AudioRules.begin_line(state,{"line_id":line,"clips":clips},result)
	return result

## Session order: handler (frame_count N) -> frame_count N+1 -> CD tick.
func _frame(state: State) -> Session.FrameResult:
	state.frame_count += 1
	var result := Session.FrameResult.new()
	AudioRules.xa_tick(state,result)
	return result

func _initialize() -> void:
	# Durations: round(0.4 * span) + 24, against every voiced line 5..15.
	# span -> (end detect - playing) measured: 352:153 640:268 256:114 224:102
	# 320:140 1024:422 704:294 768:319 (r2-start20, r3-start58, r4-*, r1).
	var measured_end := {352:153, 640:268, 256:114, 224:102, 320:140, 1024:422, 704:294, 768:319}
	for span: int in measured_end:
		check(AudioRules.xa_play_frames(span, 12) == measured_end[span] + 12, "span %d ends round(0.4*span)+12 after playing plus fade/visibility 12 at Rei/Yoshio volume" % span)
	# ceil (the earlier diagnosis) differs on lines 9 and 15, floor on 13 and 14.
	check(ceili(0.4 * 256) + 12 != measured_end[256] and floori(0.4 * 1024) + 12 != measured_end[1024], "neither ceil nor floor fits every measured line")
	# First logic frame seeing finished minus first seeing playing (r3-start33).
	for pair: Array in [[11,13,320,152],[12,12,320,152],[13,12,1024,434],[14,12,704,306],[15,13,768,331]]:
		check(AudioRules.xa_play_frames(pair[2], pair[1]) == pair[3], "line %d playing->finished %d logic frames" % [pair[0], pair[3]])
	# audio_speaker_volume_set: the fade from xa_volume_full in 0xA steps sets the tail.
	# Lines 9/10 (speakers 30/46 -> 14, volume 0x64) fade 10 ticks: one frame shorter
	# than Yoshio/Rei (0x6E, 11 ticks). opening-timing-v4: hold starts end+11 vs end+12.
	for pair: Array in [[13,0x6E,11],[12,0x6E,11],[30,0x64,10],[46,0x64,10],[0,0x64,10],[6,0x7D,13],[10,0x5A,9],[15,0x69,11],[7,0x5F,10]]:
		check(AudioRules.xa_volume_full(pair[0]) == pair[1] and AudioRules.xa_fade_frames(pair[0]) == pair[2], "speaker %d volume 0x%x fades in %d ticks" % pair)
	check(AudioRules.xa_play_frames(256, 30) == 114 + 11 and AudioRules.xa_play_frames(224, 46) == 102 + 11, "lines 9 and 10 see finished end+11 (measured 3915-3904, 4095-4084 less the hold frame)")
	check(AudioRules.xa_play_frames(256, 30) == AudioRules.xa_play_frames(256, 13) - 1, "girl A/B tail is one frame shorter than Yoshio's")
	# In main-loop iterations every voiced line starts 6 after its request; the
	# 7-vsync starts of lines 11 and 15 contain one dropped vsync (7/0/1 and
	# 10/0/1 in opening-timing-v4/vsync_drops.txt).
	for line: int in [5,7,9,10,11,12,13,14,15]:
		check(AudioRules.xa_start_frames(line) == 6, "line %d sees cd_state&4 six iterations after its request" % line)

	# Full schedule: no intent at the request, one intent when playback starts.
	var state := State.new()
	state.frame_count = 1000
	var begun := _begin(state,12,[_clip(320)])
	check(begun.error.is_empty() and begun.audio.is_empty() and state.opening.audio.status == "requested", "request publishes nothing before the drive starts")
	check(state.opening.audio.xa_start_frame == 1006 and state.opening.audio.xa_finish_frame == 1006 + 152, "schedule is anchored to the request handler frame")
	var intents := 0
	var playing_seen := -1
	var finished_seen := -1
	for i: int in range(200):
		# The handler of frame_count C sees the status the previous tick left.
		var c: int = state.frame_count
		if playing_seen < 0 and state.opening.audio.status == "playing": playing_seen = c
		if finished_seen < 0 and state.opening.audio.status == "finished": finished_seen = c
		var r := _frame(state)
		check(r.error.is_empty(), "tick error at %d" % c)
		for e: Dictionary in r.audio:
			if e.kind == "voice_play": intents += 1
	check(playing_seen == 1006, "handler first sees playing at request + 6 (%d)" % playing_seen)
	check(finished_seen == 1006 + 152, "handler first sees finished at playing + 152 (%d)" % finished_seen)
	check(intents == 1, "exactly one voice_play intent, at playback start")
	check(state.opening.audio.voice_started == true and state.opening.audio.xa_started_frame == 1006, "start frame recorded for the bridge preflight")

	# Line 15 (six-iteration start like every line), and a two-clip line rescheduled from run_line.
	var two := State.new()
	two.frame_count = 50
	_begin(two,15,[_clip(768),_clip(256,281470)])
	for i: int in range(6 + 331): _frame(two)
	check(two.opening.audio.status == "finished" and two.frame_count == 50 + 6 + 331, "line 15 finishes 337 frames after its request")
	two.opening["prologue_text"] = {"line_id":15,"iteration":two.frame_count,"k_disp_inc_result":0}
	var run := Session.FrameResult.new()
	AudioRules.run_line(two,run)
	check(run.error.is_empty() and run.audio.is_empty() and two.opening.audio.clip_index == 1 and two.opening.audio.status == "requested", "second clip is requested without an immediate intent")
	check(two.opening.audio.xa_start_frame == 50 + 337 + 6 and two.opening.audio.xa_finish_frame == 50 + 337 + 6 + 126, "second clip schedule restarts from its own request")

	# Wrap-around of the u32 frame counter.
	var wrap := State.new()
	wrap.frame_count = 0xfffffffd
	_begin(wrap,12,[_clip(320)])
	check(wrap.opening.audio.xa_start_frame == 3, "start frame wraps as u32")
	for i: int in range(5): _frame(wrap)
	check(wrap.opening.audio.status == "requested", "wrapped start not reached early")
	_frame(wrap)
	check(wrap.opening.audio.status == "playing", "wrapped start reached on schedule")

	# Negatives.
	for bad: Dictionary in [{"kind":"voice_play"}, _clip(0), _clip(-4), _clip(0x10001)]:
		var s := State.new()
		var r := _begin(s,12,[bad])
		check(not r.error.is_empty() and not s.opening.has("audio"), "clip without an exact recorded span is refused: " + str(bad.get("xa_request",{}).get("end_lba","none")))
	var fractional := _clip(320)
	fractional.xa_request.end_lba = 276222.5
	check(not _begin(State.new(),12,[fractional]).error.is_empty(), "non-integral span is refused")
	for speaker: Variant in [null, -1, 256, 12.0, "12"]:
		var no_speaker := _clip(320)
		if speaker == null: no_speaker.erase("speaker")
		else: no_speaker.speaker = speaker
		var ns := State.new()
		check(not _begin(ns,12,[no_speaker]).error.is_empty() and not ns.opening.has("audio"), "clip without an integral speaker has no fade schedule: " + str(speaker))
	var girl := State.new()
	girl.frame_count = 400
	var girl_clip := _clip(256)
	girl_clip.speaker = 30
	_begin(girl,9,[girl_clip])
	check(girl.opening.audio.xa_start_frame == 406 and girl.opening.audio.xa_finish_frame == 406 + 125, "line 9 schedule uses the girl A volume fade")
	var lost := State.new()
	_begin(lost,12,[_clip(320)])
	lost.opening.audio.erase("xa_finish_frame")
	check(not _frame(lost).error.is_empty(), "scheduled clip without its finish frame fails closed")
	var cancelled := State.new()
	_begin(cancelled,12,[_clip(320)])
	AudioRules.cancel_voice(cancelled,Session.FrameResult.new())
	for i: int in range(20): check(_frame(cancelled).error.is_empty(), "cancelled clip tick is inert")
	check(cancelled.opening.audio.status == "cancelled" and not cancelled.opening.audio.has("xa_start_frame"), "cancel drops the schedule")
	var silent := State.new()
	check(_begin(silent,16,[]).error.is_empty() and _frame(silent).error.is_empty() and silent.opening.audio.status == "silent", "silent line has no schedule")
	# Host observations are refused whatever their content.
	var host := State.new()
	_begin(host,12,[_clip(320)])
	for status: String in ["started","finished"]:
		var event := Session.ServiceEvent.new(0,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,AudioRules.COMPLETION_SERVICE,
			{"epoch":host.opening.audio.epoch,"line_id":12,"clip_index":0,"request_key":"invented-320","status":status,"completion_index":1})
		var r := Session.FrameResult.new()
		AudioRules.complete(event,host,r)
		check(not r.error.is_empty() and host.opening.audio.status == "requested", "host '%s' observation cannot move the clip" % status)

	# Determinism: identical inputs, identical state words every frame.
	var a := State.new()
	var b := State.new()
	for s: State in [a,b]:
		s.frame_count = 9
		_begin(s,13,[_clip(1024)])
	var same := true
	for i: int in range(460):
		_frame(a); _frame(b)
		same = same and a.to_words() == b.to_words()
	check(same and a.opening.audio.status == "finished", "schedule is a pure function of logic frames")

	print("PASS: %d XA logic schedule checks" % checks if failures == 0 else "FAIL: %d of %d XA logic schedule checks" % [failures, checks])
	quit(0 if failures == 0 else 1)
