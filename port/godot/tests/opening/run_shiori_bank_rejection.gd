extends SceneTree
## T-39002 stage10/substep8 prologue_shiori bank race, bridge and rules level.
## Decided path (2026-10-29, race-emulation): the frame's C1 fade and Shiori
## load reach the native core together; the stage11/substep2 poll is accepted
## only on the measured schedule and starts Shiori through finish_shiori.
## Every unsupported variant is refused before any native call.
## Env: SHIORI_BANK_PACK (opening pack root with prologue_shiori-*.bin).
const Bridge = preload("res://services/first_arrival_bgm_bridge.gd")
const Audio = preload("res://services/audio_service.gd")
const Rules = preload("res://services/opening_audio_rules.gd")
const Shiori = preload("res://services/prologue_shiori_bank_resources.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")

class FakeEngine extends RefCounted:
	var calls: Array = []
	var renders := 0
	var shiori_pump := 0
	var shiori_active := false
	var pump_skew := 0
	func begin_fade(rate: int, decrement: int, per_pump: bool) -> String:
		calls.append(["begin_fade",rate,decrement,per_pump,renders]); return ""
	func begin_shiori_load(inputs: Array, schedule: PackedInt32Array) -> String:
		calls.append(["begin_shiori_load",inputs.size(),schedule.size()]); shiori_active = true; shiori_pump = 0; return ""
	func render_block() -> Dictionary:
		renders += 1
		if shiori_active: shiori_pump += 1
		var pcm := PackedVector2Array(); pcm.resize(735)
		return {"error":"","pcm":pcm,"render_us":0}
	func diagnostics() -> Dictionary: return {"shiori_pump":shiori_pump+pump_skew}
	func finish_shiori() -> String:
		calls.append(["finish_shiori"]); return ""
class OldEngine extends RefCounted:
	var calls: Array = []
class Sink extends RefCounted:
	var generation := 1
	var room := 1000000
	var pushed := 0
	func validate_active(_generation: int) -> String: return ""
	func can_push() -> bool: return room > 0
	func push(_pcm: PackedVector2Array) -> String:
		if room <= 0: return "sink cannot accept exact current block"
		room -= 1; pushed += 1; return ""
class Backend extends RefCounted:
	var _engine: RefCounted
	var _owner: Dictionary = {}
	var sink := Sink.new()
	var _active_sink_generation := 1
	var error := ""
	var rei_calls := 0
	var pumps := 0
	func rei_action(_owner_arg: Dictionary,_action: String,_inputs: Array = [],_xa: bool = false) -> String:
		rei_calls += 1; return ""
	var history: Array = []
	func pump(_current: Dictionary) -> String:
		pumps += 1; return ""
	func reset() -> void: pass
	func diagnostics() -> Dictionary: return {}

var failures := 0
var checks := 0
var inputs: Dictionary

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL: " + label)

func _initialize() -> void:
	var read := Shiori.read_inputs(OS.get_environment("SHIORI_BANK_PACK"))
	if not read.error.is_empty(): printerr(read.error); quit(1); return
	inputs = read.inputs
	_rules_checks()
	_schedule_checks()
	_bridge_checks()
	print("shiori bank bridge/rules: %d checks, %d failures" % [checks,failures])
	quit(1 if failures else 0)

func _schedule_checks() -> void:
	var landed: Array = Shiori.landed_at(2)
	check(landed == [0,0,0],"sector landed before +3")
	for pair: Array in [[3,[1,0,0]],[29,[37,0,0]],[31,[37,1,0]],[75,[37,80,0]],[77,[37,80,1]],[91,[37,80,6]]]:
		check(Shiori.landed_at(pair[0]) == pair[1],"measured landing at +%d differs: %s" % [pair[0],Shiori.landed_at(pair[0])])
	check(Shiori.flat_schedule().size() == Shiori.LANDING_SCHEDULE.size()*3,"flat schedule shape differs")
	var bad := inputs.duplicate(true); bad.music_header = bad.music_header.duplicate(); bad.music_header[0x18] ^= 1
	check(Shiori.validate(bad) == "Shiori native input changed: music_header","changed CBD4 bytes accepted")
	check(Shiori.read_inputs("/nonexistent").error == "Original Shiori bank input is missing: sequence_pack","missing pack not refused")

func _playing_state() -> State:
	var state := State.new(); state.game_mode = 0x15; state.mode_stage = 10; state.substep = 8
	state.opening.native_audio_context = {"bgm_volume":93,"bgm_volume_full":93}
	state.opening.audio_bank = {"role":"prologue_rei","epoch":2,"ready":true}
	state.opening.prologue_bgm_state = {"playing":true,"role":"prologue_rei","bank":1,"sequence":0,"bank_epoch":2,"gain":83}
	return state

func _rules_checks() -> void:
	# B1 keeps its reviewed shape (event-driven ticks every frame).
	var b1 := _playing_state(); var r := Session.FrameResult.new()
	Rules.audio_command(b1,0xb1,r)
	check(b1.opening.bgm_fade == {"active":true,"gain":93,"ticks":0} and r.audio == [{"kind":"native_bgm_fade","command":0xb1}],"B1 fade shape changed")
	# C1: rate 0xC0, decrement 1 from Rei's gain 83; not a stop.
	var state := _playing_state(); r = Session.FrameResult.new()
	Rules.audio_command(state,5,r); Rules.audio_command(state,0xc1,r)
	check(r.error.is_empty() and r.audio == [{"kind":"native_audio_control","command":5},{"kind":"native_bgm_fade","command":0xc1}],"C1 did not emit a source fade: " + str(r.audio))
	check(state.opening.bgm_fade.rate == 0xc0 and state.opening.bgm_fade.decrement == 1 and state.opening.bgm_fade.gain == 83 and state.opening.prologue_bgm_state.playing,"C1 fade fields differ or BGM marked stopped")
	var gains := {}
	for frame: int in range(0,114):
		var tick := Session.FrameResult.new()
		Rules.bgm_fade_tick(state,tick)
		check(tick.audio.is_empty(),"C1 tick emitted a per-frame native event")
		gains[frame] = [state.opening.bgm_fade.gain,state.opening.bgm_fade.active]
		state.frame_count += 1
	# Frame 0 is the command frame; +1 first accumulates, +2 first carries.
	check(gains[1] == [83,true] and gains[2] == [82,true] and gains[4] == [80,true] and gains[5] == [80,true],"C1 accumulator pattern differs: " + str([gains[1],gains[2],gains[4],gains[5]]))
	check(gains[111] == [0,true] and gains[112] == [-1,false] and not state.opening.prologue_bgm_state.playing,"C1 gain0 at +111 / finish at +112 differs: " + str([gains[111],gains[112]]))
	# D1 from the same dispatch: rate 0x80, decrement 1.
	var d1 := _playing_state(); r = Session.FrameResult.new()
	Rules.audio_command(d1,0xd1,r)
	check(r.error.is_empty() and d1.opening.bgm_fade.rate == 0x80 and d1.opening.bgm_fade.decrement == 1,"D1 rate/decrement differs")
	r = Session.FrameResult.new(); Rules.audio_command(d1,0xc1,r)
	check(r.error == "BGM fade restart while a source fade request is outstanding is unsupported","fade restart over an outstanding request accepted")
	var silent := _playing_state(); silent.opening.prologue_bgm_state.erase("gain"); r = Session.FrameResult.new()
	Rules.audio_command(silent,0xc1,r)
	check(r.error == "BGM fade requires the actual playing sequence gain","C1 without a playing gain accepted")
	r = Session.FrameResult.new(); Rules.audio_command(_playing_state(),0x71,r)
	check(r.audio == [{"kind":"bgm_stop","command":0x71}],"0x71 is no longer the stop")
	# audio_bgm_start gain: VAB byte 0x18 - 25 (- 24 in output mode 1).
	var start := _playing_state(); start.opening.audio_bank = {"role":"prologue_shiori","epoch":3,"ready":true}
	start.opening.bgm_fade = {"active":false,"request":true}
	r = Session.FrameResult.new(); Rules.bank_start(start,1,0,r)
	check(r.error.is_empty() and start.opening.prologue_bgm_state.gain == 101 and not start.opening.bgm_fade.has("request"),"Shiori start gain/request differs")
	start.opening.native_audio_context.bgm_volume_full = 69
	r = Session.FrameResult.new(); Rules.bank_start(start,1,0,r)
	check(start.opening.prologue_bgm_state.gain == 77,"mode-1 Shiori gain differs")
	start.opening.native_audio_context.erase("bgm_volume_full")
	r = Session.FrameResult.new(); Rules.bank_start(start,1,0,r)
	check(r.error == "prologue BGM start requires the actual output-mode gain","start without output-mode gain accepted")

## A committed Rei owner at stage10/8, with the source frame applied.
func _bridge(engine: RefCounted) -> Array:
	var audio := Audio.new()
	var state := _playing_state()
	var session := Session.new(); session.state = state
	var bridge := Bridge.new(); bridge._audio = weakref(audio); bridge._session = weakref(session)
	var backend := Backend.new(); backend._engine = engine
	bridge._backend = backend
	bridge._committed_after = {"test":true,"bgm_volume_full":93}
	bridge._owner = {"role":"arrival","epoch":1}; backend._owner = bridge._owner
	bridge._rei_epoch = 2; bridge._rei_started = true
	# Rei started at logic tick 200 and the locked clock is caught up to 300.
	bridge._race = {"rei_tick":200,"rendered":300,"fifo":[],"max_fifo":0}
	if engine is FakeEngine: engine.renders = 300
	bridge._shiori_inputs = inputs; bridge._shiori_error = ""
	audio.native_audio_context = {"test":true,"bgm_volume_full":93}
	audio.active_bank_role = "prologue_rei"; audio.bank_ready = {"arrival":true}
	state.opening.native_audio_context = {"test":true,"bgm_volume_full":93}
	state.opening.audio_banks = {"arrival":{"role":"arrival","epoch":1,"ready":true}}
	return [bridge,state,session,audio,backend]

## The refused publication consumed the validated poll frame; re-validate it.
func _rearm_poll(bridge: Bridge, state: State, session: Session, poll: Session.FrameResult) -> void:
	session.clock.logic_iterations = 681; bridge._shiori_poll_tick = -1
	poll.audio.clear(); poll.error = ""
	bridge.shiori_poll(state,poll); bridge.start(state,1,0,poll)
	bridge.final_precommit(state,session.rng,poll)
	poll.committed = true; session.clock.logic_iterations = 682; poll.state_hash = session.state_hash()

func _source_frame(state: State) -> Session.FrameResult:
	var r := Session.FrameResult.new()
	Rules.audio_command(state,5,r); Rules.audio_command(state,0xc1,r)
	Rules.begin_bank(state,"prologue_shiori",r)
	state.mode_stage = 11; state.substep = 0 # next_stage() in the same frame
	return r

func _bridge_checks() -> void:
	# Negatives: each refused at the final barrier, with no native call.
	var cases := {
		"wrong stage":func(_b,s,_r): s.mode_stage = 10; s.substep = 8,
		"Rei not started":func(b,_s,_r): b._rei_started = false,
		"inputs missing":func(b,_s,_r): b._shiori_error = "Original Shiori bank input is missing: music_body",
		"inputs changed":func(b,_s,_r): b._shiori_inputs = inputs.duplicate(); b._shiori_inputs.music_body = PackedByteArray([1]),
		"load without C1":func(_b,_s,r): r.audio.remove_at(1),
		"C1 without load":func(_b,_s,r): r.audio.remove_at(2),
		"XA during C1":func(_b,s,_r): s.opening.cd_audio_state_override = 4,
		"wrong epoch":func(_b,_s,r): r.audio[2].bank_epoch = 9,
		"no locked clock":func(b,_s,_r): b._race = {},
	}
	var expected := {
		"wrong stage":"C1 fade is outside its source stage10/substep8 command",
		"Rei not started":"C1 fade is outside its source stage10/substep8 command",
		"inputs missing":"Original Shiori bank input is missing: music_body",
		"inputs changed":"Shiori native input changed: music_body",
		"load without C1":"Shiori load lacks the preceding source C1 fade",
		"C1 without load":"C1 fade and Shiori load must arrive together in the source stage10/substep8 frame",
		"XA during C1":"XA playback during the in-flight Shiori bank race is unsupported",
		"wrong epoch":"Shiori load differs from original stage10/substep8 transition",
		"no locked clock":"Shiori race requires the logic-locked Rei audio clock",
	}
	for name: String in cases:
		var parts := _bridge(FakeEngine.new())
		var r := _source_frame(parts[1])
		cases[name].call(parts[0],parts[1],r)
		parts[0].final_precommit(parts[1],parts[2].rng,r)
		check(r.error == expected[name] and parts[4]._engine.calls.is_empty() and parts[0]._rei_result_id == 0,"%s: %s" % [name,r.error])
	# Logic far ahead of a stalled sink: bounded FIFO, then an explicit stop.
	var stalled := _bridge(FakeEngine.new())
	stalled[4].sink.room = 0
	stalled[2].clock.logic_iterations = 200+300+Bridge.RACE_FIFO_LIMIT+1
	var stall_error: String = stalled[0]._race_pump(stalled[2].clock.logic_iterations-200)
	check(stall_error == "logic ran more than %d frames ahead of the audio sink during the logic-locked Rei clock" % Bridge.RACE_FIFO_LIMIT and stalled[4]._engine.renders == 300+Bridge.RACE_FIFO_LIMIT+1,"unbounded locked FIFO accepted")
	check(stalled[0]._race_pump(stalled[2].clock.logic_iterations-200+5) == stalled[4].error and stalled[4]._engine.renders == 300+Bridge.RACE_FIFO_LIMIT+1,"locked clock rendered after its sticky error")
	var old := _bridge(OldEngine.new())
	var refused := _source_frame(old[1])
	old[0].final_precommit(old[1],old[2].rng,refused)
	check(refused.error == Bridge.SHIORI_NATIVE_UNAVAILABLE,"library without race support accepted: " + refused.error)
	# Positive path.
	var parts := _bridge(FakeEngine.new())
	var bridge: Bridge = parts[0]; var state: State = parts[1]; var session: Session = parts[2]; var audio: Audio = parts[3]; var backend: Backend = parts[4]
	var frame := _source_frame(state)
	session.clock.logic_iterations = 500
	bridge.final_precommit(state,session.rng,frame)
	check(frame.error.is_empty() and bridge._rei_result_id == frame.get_instance_id(),"source stage10/8 frame refused: " + frame.error)
	frame.committed = true; session.clock.logic_iterations = 501; frame.state_hash = session.state_hash()
	check(bridge.publish(frame,500).is_empty(),"stage10/8 publication failed")
	var engine: FakeEngine = backend._engine
	# Rei start tick 200, C1 tick 500: the native race starts after exactly 300
	# locked pumps (the publication caught up from 300 rendered; none extra).
	check(engine.calls == [["begin_fade",0xc0,1,true,300],["begin_shiori_load",3,Shiori.LANDING_SCHEDULE.size()*3]],"native race calls differ: " + str(engine.calls))
	check(bridge._shiori == {"epoch":3,"tick":500,"started":false,"phase":300},"in-flight record differs: " + str(bridge._shiori))
	check(backend.rei_calls == 0,"C1 used the hard-coded B1 Rei fade")
	# Pending/acknowledged Shiori bank pumps one locked block per logic frame,
	# never through the wall-clock look-ahead refill.
	audio.active_bank_role = "prologue_shiori"
	check(bridge.pump(state).is_empty() and backend.pumps == 0 and engine.renders == 301 and engine.shiori_pump == 1,"pending Shiori bank did not pump one locked block")
	check(bridge.pump(state).is_empty() and bridge.pump(state).is_empty() and engine.renders == 301,"repeated wall-clock pumps advanced the locked clock")
	# Sink full: the block is still rendered on its logic frame and queued.
	backend.sink.room = 0; session.clock.logic_iterations = 503
	check(bridge.pump(state).is_empty() and engine.renders == 303 and bridge._race.fifo.size() == 2,"full sink stalled the locked clock")
	backend.sink.room = 1
	check(bridge.pump(state).is_empty() and bridge._race.fifo.size() == 1 and engine.renders == 303,"queued locked block was not pushed first")
	backend.sink.room = 1000000
	check(bridge.pump(state).is_empty() and bridge._race.fifo.is_empty() and bridge.diagnostics().race_clock == {"rei_tick":200,"rendered":303,"fifo":0,"max_fifo":2},"locked clock diagnostics differ: " + str(bridge.diagnostics().race_clock))
	session.clock.logic_iterations = 501
	check(bridge.pump(state) == "logic-locked Rei clock rendered 303 blocks ahead of logic 301","locked clock ahead of logic accepted")
	session.clock.logic_iterations = 503
	state.opening.cd_audio_state_override = 4
	check(bridge.pump(state) == "XA playback during the in-flight Shiori bank race is unsupported","XA during the race pumped")
	state.opening.erase("cd_audio_state_override")
	state.opening.audio_bank.ready = true
	check(bridge.bank_completed(state).is_empty(),"Shiori acknowledgement refused")
	state.opening.audio_bank.epoch = 9
	check(bridge.bank_completed(state) == "Shiori completion differs from the actual in-flight load","stale Shiori acknowledgement accepted")
	state.opening.audio_bank.epoch = 3
	# Stage11/substep2 poll on the measured schedule.
	state.mode_stage = 11; state.substep = 2
	for pair: Array in [[550,"Shiori CD reads are incomplete at the bank poll (+50, measured complete at +91)"],[600,"Shiori dispose before the C1 fade finished is outside the measured path (+100)"]]:
		session.clock.logic_iterations = pair[0]
		var early := Session.FrameResult.new()
		check(bridge.shiori_poll(state,early) == 0 and early.error == pair[1],"early poll not refused: " + early.error)
	var wrong := Session.FrameResult.new(); state.substep = 1; session.clock.logic_iterations = 681
	check(bridge.shiori_poll(state,wrong) == 0 and wrong.error == "Shiori bank poll is outside stage11/substep2","poll outside substep2 accepted")
	state.substep = 2
	var nopoll := Session.FrameResult.new(); bridge.start(state,1,0,nopoll)
	check(nopoll.error == "Shiori start has no same-frame bank poll of its in-flight load","start without poll accepted")
	var poll := Session.FrameResult.new()
	session.clock.logic_iterations = 681
	check(bridge.shiori_poll(state,poll) == -1 and poll.error.is_empty(),"poll at +181 not accepted: " + poll.error)
	var again := Session.FrameResult.new()
	check(bridge.shiori_poll(state,again) == 0 and again.error == "Shiori bank poll has no owned in-flight load","second poll in one frame accepted")
	bridge.abort_frame()
	check(bridge._shiori_poll_tick == -1,"aborted frame kept the poll")
	poll = Session.FrameResult.new()
	bridge.shiori_poll(state,poll); bridge.start(state,1,0,poll)
	check(poll.error.is_empty() and poll.audio == [{"kind":"bgm_play","bank":1,"sequence":0,"role":"prologue_shiori","bank_epoch":3}],"Shiori start differs: " + str(poll.audio))
	bridge.final_precommit(state,session.rng,poll)
	check(poll.error.is_empty(),"poll frame refused: " + poll.error)
	poll.committed = true; session.clock.logic_iterations = 682; poll.state_hash = session.state_hash()
	# Skewed native pump count is refused before finish_shiori.
	engine.pump_skew = 3
	var skew := bridge.publish(poll,681)
	check(skew == "native Shiori pump +184 differs from logic +181 at the bank poll" and engine.calls.back()[0] != "finish_shiori","skewed native pump accepted: " + skew)
	engine.pump_skew = 0
	check(engine.renders == 481 and engine.shiori_pump == 181,"publication did not catch the locked clock up to +181: %d/%d" % [engine.renders,engine.shiori_pump])
	_rearm_poll(bridge,state,session,poll)
	check(bridge.publish(poll,681).is_empty() and engine.calls.back() == ["finish_shiori"] and bridge._shiori.started,"finish_shiori not published")
	var replay := Session.FrameResult.new(); session.clock.logic_iterations = 700
	check(bridge.shiori_poll(state,replay) == 0,"poll after the started Shiori bank accepted")
	bridge.reset()
	check(bridge._shiori.is_empty() and bridge._shiori_poll_tick == -1,"reset kept Shiori ownership")
