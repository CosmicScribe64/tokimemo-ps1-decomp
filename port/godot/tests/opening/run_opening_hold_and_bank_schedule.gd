extends SceneTree
## Review regressions for the opening's logic-frame timing:
## - stage 6 (prologue_yoshio_greeting_run, prologue.c) adds no per-frame
##   mode_step_timer increment of its own, so FINISH_HOLD lasts 0x3d frames
##   and actor102 is set when the hold counter is 4 after line_run;
## - the arrival (stage 0) and Rei (stage 7) bank polls return 1 only after
##   the serialized CD reads and the SPU transfer frames, +133 and +91 frames
##   after the request, as in every M2 emulator run.
## Line services are stubs; these tests establish no media or audio parity.

const Arrival = preload("res://flow/prologue_arrival.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const MediaQueue = preload("res://services/prologue_media_queue.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")

var checks: int = 0
var failures: int = 0

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	_test_stage6_timer_untouched()
	_test_stage6_hold_and_actor102()
	_test_bank_schedule()
	_test_bank_serialization_and_wrap()
	print("PASS: %d opening hold and bank schedule checks" % checks if failures == 0 else "FAIL: %d/%d opening hold and bank schedule checks" % [failures, checks])
	quit(0 if failures == 0 else 1)

func check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: " + message)

func _stage6_state(substep: int) -> State:
	var state := State.new()
	state.game_mode = Arrival.MODE_PROLOGUE
	state.mode_stage = 6
	state.mode_step = 0
	state.substep = substep
	return state

## Source line_run hold for a voiced line: voice finished after `voice_frames`
## line_run calls, text already complete.
func _services(log: Array, voice_frames: int) -> Dictionary:
	var calls := [0]
	var services := {}
	services["line_start"] = func(_s: State, _line: int, _out: Session.FrameResult) -> void: log.append("start")
	services["line_run"] = func(s: State, _out: Session.FrameResult) -> void:
		calls[0] += 1
		log.append("run")
		if calls[0] <= voice_frames:
			s.mode_step_timer = 0
		else:
			s.mode_step_timer = Math.add32(s.mode_step_timer, 1)
		if Math.u32(s.mode_step_timer) >= AudioRules.FINISH_HOLD:
			s.mode_step_timer = 0
			s.substep += 1
	services["arrival_draw"] = func(_s: State, _slot: int, _out: Session.FrameResult) -> void: pass
	services["outer_draw"] = func(_s: State, _slot: int, _out: Session.FrameResult) -> void: pass
	services["stage_06_actor102_set"] = func(s: State, _out: Session.FrameResult) -> void:
		log.append("actor102:%d" % s.mode_step_timer)
	return services

func _test_stage6_timer_untouched() -> void:
	# With a line_run that never touches the timer, no stage 6 substep may move it.
	for substep: int in [0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12]:
		var state := _stage6_state(substep)
		state.mode_step_timer = 7
		var log: Array = []
		var services := _services(log, 0)
		services["line_run"] = func(_s: State, _out: Session.FrameResult) -> void: pass
		services["stage_06_cd_wait"] = func(_s: State, _out: Session.FrameResult) -> void: pass
		services["stage_06_actor99_turn"] = func(_s: State, _out: Session.FrameResult) -> void: pass
		services["audio_command"] = func(_s: State, _c: int, _out: Session.FrameResult) -> void: pass
		var out := Session.FrameResult.new()
		Arrival._stage_06_yoshio_greeting(state, services, out)
		check(state.mode_step_timer == 7, "stage 6 substep %d leaves mode_step_timer to line_run (got %d): %s" % [substep, state.mode_step_timer, out.error])
	var cd_state := _stage6_state(1)
	cd_state.mode_step_timer = 9
	var cd_out := Session.FrameResult.new()
	Arrival._stage_06_yoshio_greeting(cd_state, _services([], 0), cd_out)
	check(cd_out.error.is_empty() and cd_state.substep == 2 and cd_state.mode_step_timer == 9, "stage 6 substep 1 without a CD wait service advances without resetting mode_step_timer")
	check(not cd_state.opening.has("stage_06_timer"), "stage 6 keeps no separate per-frame timer")

func _test_stage6_hold_and_actor102() -> void:
	for voice_frames: int in [1, 140, 141]:
		var state := _stage6_state(2)
		var log: Array = []
		var services := _services(log, voice_frames)
		var frames := 0
		while state.substep == 2 and frames < 1000:
			var out := Session.FrameResult.new()
			Arrival._stage_06_yoshio_greeting(state, services, out)
			frames += 1
			if not out.error.is_empty():
				check(false, "stage 6 substep 2 frame failed: " + out.error)
				break
		check(frames == voice_frames + AudioRules.FINISH_HOLD, "line 5 lasts voice + 0x3d hold frames (%d voice -> %d, got %d)" % [voice_frames, voice_frames + AudioRules.FINISH_HOLD, frames])
		var actor := log.filter(func(e: String) -> bool: return e.begins_with("actor102"))
		check(actor == ["actor102:4"], "actor102 is set exactly once, with the hold counter at 4: %s" % [actor])
		var at: int = log.find("actor102:4")
		var runs_before := log.slice(0, at).count("run")
		check(at > 0 and log[at - 1] == "run" and runs_before == voice_frames + 4, "actor102 follows line_run on the fourth hold frame (source order)")
		check(state.substep == 3 and state.mode_step_timer == 0, "hold end advances to substep 3 with the timer cleared")

func _bank_state(frame: int) -> State:
	var state := State.new()
	state.game_mode = 0x15
	state.frame_count = frame
	return state

func _complete(state: State) -> void:
	var bank: Dictionary = state.opening.audio_bank
	var event := Session.ServiceEvent.new(0, 0, Session.ServiceKind.COMPLETION, "opening.audio.bank",
		{"bank_epoch": bank.epoch, "role": bank.role, "ready": true})
	var result := Session.FrameResult.new()
	AudioRules.bank_complete(event, state, result)
	check(result.error.is_empty(), "bank completion accepted: " + result.error)

func _test_bank_schedule() -> void:
	check(MediaQueue.model_read_vsyncs(3) == 15 and MediaQueue.model_read_vsyncs(0x20) == 26 and MediaQueue.model_read_vsyncs(0x25) == 28 and MediaQueue.model_read_vsyncs(0x50) == 46 and MediaQueue.model_read_vsyncs(6) == 16,
		"bank read model latencies give the measured serialized landings 15/41/69/115/131 and 28/74/90")
	for spec: Array in [["prologue_arrival", 133], ["prologue_rei", 91]]:
		var role: String = spec[0]
		var delay: int = spec[1]
		var state := _bank_state(1000)
		var result := Session.FrameResult.new()
		AudioRules.begin_bank(state, role, result)
		check(result.error.is_empty() and state.opening.audio_bank.get("ready_frame") == 1000 + delay, "%s bank is ready at request +%d" % [role, delay])
		check(AudioRules.bank_poll(state) == 0, "%s poll is 0 before the producer completion" % role)
		_complete(state)
		for frame: int in [1000, 1001, 1000 + delay - 1]:
			state.frame_count = frame
			check(AudioRules.bank_poll(state) == 0, "%s producer completion alone does not release the poll at +%d" % [role, frame - 1000])
		state.frame_count = 1000 + delay
		check(AudioRules.bank_poll(state) == 1, "%s poll returns 1 on request +%d" % [role, delay])
		state.frame_count = 1000 + delay + 50
		check(AudioRules.bank_poll(state) == 1, "%s poll stays 1 after its deadline" % role)
		var late := _bank_state(1000)
		AudioRules.begin_bank(late, role, Session.FrameResult.new())
		late.frame_count = 1000 + delay + 10
		check(AudioRules.bank_poll(late) == 0, "%s elapsed frames alone never release the poll" % role)
	for role: String in ["prologue_shiori", "normal_gameplay"]:
		var other := _bank_state(50)
		AudioRules.begin_bank(other, role, Session.FrameResult.new())
		check(not other.opening.audio_bank.has("ready_frame"), "%s keeps its own readiness model" % role)
		_complete(other)
		check(AudioRules.bank_poll(other) == 1, "%s poll follows its producer completion only" % role)
	var a := _bank_state(321)
	var b := _bank_state(321)
	AudioRules.begin_bank(a, "prologue_arrival", Session.FrameResult.new())
	AudioRules.begin_bank(b, "prologue_arrival", Session.FrameResult.new())
	check(a.opening.audio_bank == b.opening.audio_bank, "bank schedule is deterministic")

func _test_bank_serialization_and_wrap() -> void:
	var state := _bank_state(500)
	state.opening["media"] = {"cd_ready_frame": 510}
	AudioRules.begin_bank(state, "prologue_rei", Session.FrameResult.new())
	check(state.opening.audio_bank.ready_frame == 510 + 91, "bank reads queue behind a CD read still in flight")
	var wrap := _bank_state(0xffffffff - 10)
	AudioRules.begin_bank(wrap, "prologue_arrival", Session.FrameResult.new())
	check(wrap.opening.audio_bank.ready_frame == 133 - 11, "bank deadline wraps as a u32 frame counter")
	_complete(wrap)
	wrap.frame_count = 0xffffffff
	check(AudioRules.bank_poll(wrap) == 0, "wrapped deadline is still in the future before the wrap")
	wrap.frame_count = 133 - 11
	check(AudioRules.bank_poll(wrap) == 1, "wrapped deadline releases on its frame")
