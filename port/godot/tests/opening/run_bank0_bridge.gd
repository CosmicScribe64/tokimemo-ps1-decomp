extends SceneTree
## native-bank0, BGM bridge and audio rules with a recording engine: stage16
## audio_bank_start(0,1), stage20/21 commands 5 + D1, the stage21 load, the
## +131 0x11 and the spring audio_bank_start(0,0) reach the native owner on
## their source frames with the logic-locked clock (one native block per logic
## frame), and every unsupported variant is refused before any native call.
## Env: ORIGINAL_BGM_PACK (PRO_VOL.SEQ/VH), FINISH_BANK_PACK (stage21 reads).
const Bridge = preload("res://services/first_arrival_bgm_bridge.gd")
const Audio = preload("res://services/audio_service.gd")
const Rules = preload("res://services/opening_audio_rules.gd")
const Bank0 = preload("res://services/prologue_bank0_audio.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")

class FakeEngine extends RefCounted:
	var calls: Array = []
	var renders := 0
	var load_at := -1
	var pump_skew := 0
	var start_status := 1
	var transfer_at := -1
	var transfer_skew := 0
	func render_block() -> Dictionary:
		renders += 1
		var pcm := PackedVector2Array(); pcm.resize(735)
		return {"error":"","pcm":pcm,"render_us":0}
	func submit_command(command: int) -> String:
		if command == 0x11: transfer_at = renders
		calls.append(["submit",command,renders]); return ""
	func bank0_start(sequence: int, tick_skip: int) -> Dictionary:
		calls.append(["bank0_start",sequence,tick_skip,renders]); return {"error":"","status":start_status}
	func begin_bank0_load(inputs: Array, schedule: PackedInt32Array) -> String:
		calls.append(["begin_bank0_load",inputs.size(),schedule.size(),renders]); load_at = renders; return ""
	## FAKE ONLY: these answers stand in for the native engine; the real
	## landing/transfer state is checked in run_bank0_candidate_bridge.gd.
	## Completion follows the last row of the measured landing schedule, and
	## the 0x11 moves slot0 in the next pump and slot1 in the one after.
	func bank0_diagnostics() -> Dictionary:
		var pump := renders - load_at + pump_skew
		var moved := renders - transfer_at - transfer_skew if transfer_at >= 0 else 0
		var requests := 0 if moved <= 0 else (2 if moved == 1 else 0)
		return {"bank0_load_pump":pump,"bank0_complete":load_at >= 0 and pump >= int(Bank0.LANDING_SCHEDULE.back()[0]),
			"bank0_loaded":moved >= 1,"transfer_requests":requests,"transfer_done":moved >= 1 and requests == 0}
class OldEngine extends RefCounted:
	var calls: Array = []
	func render_block() -> Dictionary:
		var pcm := PackedVector2Array(); pcm.resize(735)
		return {"error":"","pcm":pcm,"render_us":0}
class Sink extends RefCounted:
	var generation := 1
	func validate_active(_generation: int) -> String: return ""
	func can_push() -> bool: return true
	func push(_pcm: PackedVector2Array) -> String: return ""
class Backend extends RefCounted:
	var _engine: RefCounted
	var _owner: Dictionary = {}
	var sink := Sink.new()
	var _active_sink_generation := 1
	var error := ""
	func rei_action(_o: Dictionary, _a: String, _i: Array = [], _x: bool = false) -> String: return "unexpected Rei action"
	func pump(_current: Dictionary) -> String: return "unexpected wall-clock pump"
	func reset() -> void: pass
	func diagnostics() -> Dictionary: return {}

var failures := 0
var checks := 0
var files: Array = []
var store := FinishBank.new()

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL: " + label)

func _initialize() -> void:
	var root := OS.get_environment("ORIGINAL_BGM_PACK")
	for name: String in ["executable.bin","SYS_MIX.VH","SYS_MIX.VB","PRO_VOL.VH","PRO_VOL.VB","PRO_VOL.SEQ"]:
		files.append(FileAccess.get_file_as_bytes(root.path_join(name)) if name.begins_with("PRO_VOL") else PackedByteArray())
	var opened := store.open_pack(OS.get_environment("FINISH_BANK_PACK"))
	if not opened.is_empty() or files[5].size() != 27624 or files[3].size() != 9760: printerr("inputs missing: " + opened); quit(1); return
	check(Bank0.pack_word0(files[5]) == 0x40 and Bank0.vab_volume(files[3]) == 118, "PRO_VOL pack word/VAB volume differ from the source decision")
	var spring := Bank0.native_inputs(store)
	check(spring.error.is_empty() and Bank0.pack_word0(spring.inputs[2]) == 0x40 and Bank0.vab_volume(spring.inputs[4]) == 119, "SPRING pack word/VAB volume differ")
	_route()
	_refusals()
	print("bank0 bridge/rules: %d checks, %d failures" % [checks,failures])
	if failures == 0: print("PASS: bank0 native ownership on source frames")
	quit(1 if failures else 0)

## Bridge after the Shiori start (Rei start tick 200, Shiori load at 500).
func _bridge(engine: RefCounted) -> Array:
	var audio := Audio.new()
	var state := State.new()
	state.game_mode = 0x15; state.mode_stage = 16; state.mode_step = 0; state.substep = 4
	var session := Session.new(); session.state = state
	var bridge := Bridge.new(); bridge._audio = weakref(audio); bridge._session = weakref(session)
	var backend := Backend.new(); backend._engine = engine
	bridge._backend = backend
	bridge._committed_after = {"test":true,"bgm_volume_full":93}
	bridge._owner = {"role":"arrival","epoch":1}; backend._owner = bridge._owner
	bridge._rei_epoch = 2; bridge._rei_started = true
	bridge._shiori = {"epoch":3,"tick":500,"started":true,"phase":300}
	bridge._original = {"files":files}
	check(bridge.bind_finish_bank(store).is_empty(), "finish reads not bound")
	session.clock.logic_iterations = 5000
	bridge._race = {"rei_tick":200,"rendered":4800,"fifo":[],"max_fifo":0}
	if engine is FakeEngine: engine.renders = 4800
	audio.native_audio_context = {"test":true,"bgm_volume_full":93}
	audio.active_bank_role = "prologue_shiori"; audio.bank_ready = {"arrival":true}
	state.opening.native_audio_context = {"test":true,"bgm_volume_full":93}
	state.opening.audio_banks = {"arrival":{"role":"arrival","epoch":1,"ready":true}}
	state.opening.audio_bank = {"role":"prologue_shiori","epoch":3,"ready":true}
	state.opening.prologue_bgm_state = {"playing":true,"role":"prologue_shiori","bank":1,"sequence":0,"bank_epoch":3,"gain":101}
	state.opening.bgm_fade = {"active":false,"gain":0,"ticks":112,"command":0xc1,"rate":0xc0,"decrement":1,"accumulator":0}
	return [bridge,state,session,audio,backend]

## final_precommit + publish of one logic frame at `tick`.
func _commit(bridge: Bridge, state: State, session: Session, frame: Session.FrameResult, tick: int) -> String:
	session.clock.logic_iterations = tick
	bridge.final_precommit(state,session.rng,frame)
	if not frame.error.is_empty(): return frame.error
	frame.committed = true; session.clock.logic_iterations = tick+1; frame.state_hash = session.state_hash()
	return bridge.publish(frame,tick)

func _logic_frames(bridge: Bridge, state: State, session: Session, from: int, to: int) -> String:
	for tick in range(from,to):
		session.clock.logic_iterations = tick
		var pumped := bridge.pump(state)
		if not pumped.is_empty(): return pumped
		var tickr := Session.FrameResult.new()
		Rules.bgm_fade_tick(state,tickr)
		state.frame_count += 1
	return ""

func _route() -> void:
	var parts := _bridge(FakeEngine.new())
	var bridge: Bridge = parts[0]; var state: State = parts[1]; var session: Session = parts[2]; var engine: FakeEngine = parts[4]._engine
	state.frame_count = 5000
	# Stage16 case4 (tick 5000): audio_bank_start(0,1), then line 0x22 bumps substep.
	var f16 := Session.FrameResult.new()
	bridge.start(state,0,1,f16); state.substep = 5
	check(f16.error.is_empty() and f16.audio == [{"kind":"native_bank0_start","sequence":1,"tick_skip":Bank0.STAGE16_TICK_SKIP}], "stage16 start intent differs: %s %s" % [f16.error,f16.audio])
	check(state.opening.bgm_fade.command == 0xb1 and state.opening.bgm_fade.next_sequence == 1 and state.opening.bgm_fade.next_gain == 93, "stage16 B1/next sequence differs: " + str(state.opening.bgm_fade))
	check(_commit(bridge,state,session,f16,5000).is_empty() and engine.calls == [["bank0_start",1,1,4800]], "stage16 native start differs: " + str(engine.calls))
	# Logic mirror: B1 from 101 by 10 per pump; sequence 1 at gain 93 after the 11th step.
	check(_logic_frames(bridge,state,session,5001,5013).is_empty(), "stage16 logic frames failed")
	check(state.opening.prologue_bgm_state == {"playing":true,"role":"bank0_pro_vol","bank":0,"sequence":1,"gain":93} and not state.opening.bgm_fade.has("request"), "logic B1 chain differs: " + str(state.opening.prologue_bgm_state))
	check(engine.renders == 4812, "locked clock rendered %d, logic 4812" % engine.renders)
	# Stage20 case2 (tick 7309): audio_command(5), (0xD1), mode_stage_next.
	state.mode_stage = 20; state.substep = 2
	var f20 := Session.FrameResult.new()
	Rules.audio_command(state,5,f20); Rules.audio_command(state,0xd1,f20)
	state.mode_stage = 21; state.substep = 0
	var xa := f20.audio.duplicate(true)
	state.opening.cd_audio_state_override = 4
	var refused := Session.FrameResult.new(); refused.audio = xa
	check(_commit(bridge,state,session,refused,7309) == Bank0.XA_UNSUPPORTED and engine.calls.size() == 1, "D1 with XA playing accepted")
	state.opening.erase("cd_audio_state_override")
	bridge.abort_frame()
	check(_commit(bridge,state,session,f20,7309).is_empty() and engine.calls.slice(1) == [["submit",5,7109],["submit",0xd1,7109]], "stage20 commands differ: " + str(engine.calls))
	# Stage21 case0 (tick 7310): 5, D1 (ignored by audio_bgm_fade_start), the reads.
	var f21 := Session.FrameResult.new()
	Rules.audio_command(state,5,f21); Rules.audio_command(state,0xd1,f21)
	check(f21.error.is_empty() and state.opening.bgm_fade.ignored == 1 and state.opening.bgm_fade.started_frame == 5012, "stage21 D1 was not ignored: " + f21.error)
	state.opening.bank0_audio.phase = "loading"; state.opening.bank0_audio.load_frame = state.frame_count
	f21.audio.append({"kind":"native_bank0_load"})
	state.substep = 1
	state.frame_count += 1
	check(_commit(bridge,state,session,f21,7310).is_empty() and engine.calls.slice(3) == [["submit",5,7110],["submit",0xd1,7110],["begin_bank0_load",5,Bank0.LANDING_SCHEDULE.size()*3,7110]], "stage21 native calls differ: " + str(engine.calls))
	# Mode 0x34 stage 0xFF poll at +131: 0x11, after exactly 131 locked blocks.
	state.game_mode = 0x34; state.mode_stage = 0xff; state.mode_step = 1; state.substep = 0
	var skew := Session.FrameResult.new(); skew.audio = [{"kind":"native_audio_command","command":0x11}]
	state.opening.bank0_audio.phase = "transferred"; state.opening.bank0_audio.transfer_frame = state.opening.bank0_audio.load_frame + 131
	engine.pump_skew = 2
	var skewed := _commit(bridge,state,session,skew,7441)
	check(skewed == "native bank0 landing +133 differs from logic +131 at the 0x11 poll" and engine.calls.size() == 6, "skewed native landing accepted: " + skewed)
	engine.pump_skew = 0
	var early := Session.FrameResult.new(); early.audio = [{"kind":"native_audio_command","command":0x11}]
	check(_commit(bridge,state,session,early,7440) == "0x11 poll at +130 differs from the measured reads-complete frame +131", "0x11 off the measured frame accepted")
	bridge.abort_frame()
	var f11 := Session.FrameResult.new(); f11.audio = [{"kind":"native_audio_command","command":0x11}]
	check(_commit(bridge,state,session,f11,7441).is_empty() and engine.calls.back() == ["submit",0x11,7241], "0x11 not submitted at +131: " + str(engine.calls.back()))
	# +132: the reload poll asks the engine for slot1 (audio_bgm_bank_ready).
	state.frame_count = state.opening.bank0_audio.transfer_frame + 1; session.clock.logic_iterations = 7442
	engine.transfer_skew = -1
	var early_slot := bridge.bank0_slot_state(state)
	check(early_slot.get("error") == "native slot1 transfer (ready true, requests 0) at +1 differs from the measured ready frame +2", "native slot1 ready one pump early accepted: " + str(early_slot))
	engine.transfer_skew = 0
	check(bridge.bank0_slot_state(state) == {"error":"","ready":false} and engine.renders == 7242, "slot poll at +1 differs from the native slot0-only state")
	# +133: the transfer-ready poll and audio_bank_start(0,0) in one frame.
	var nostart := Session.FrameResult.new()
	bridge.start(state,0,0,nostart)
	check(nostart.error == "bank0 sequence 0 start lacks the same-frame transfer-ready poll", "spring start without the ready poll accepted")
	state.frame_count = state.opening.bank0_audio.transfer_frame + 2; session.clock.logic_iterations = 7443
	engine.transfer_skew = 1
	var late_slot := bridge.bank0_slot_state(state)
	check(late_slot.get("error") == "native slot1 transfer (ready false, requests 2) at +2 differs from the measured ready frame +2", "native slot1 still pending at the ready frame accepted: " + str(late_slot))
	state.opening.bank0_audio.ready_frame = state.frame_count
	var pending := Session.FrameResult.new()
	bridge.start(state,0,0,pending)
	check(pending.error.is_empty() and _commit(bridge,state,session,pending,7443) == late_slot.error and engine.calls.back() == ["submit",0x11,7241], "spring start over a pending native slot1 accepted")
	bridge.abort_frame()
	engine.transfer_skew = 0
	state.opening.bank0_audio.phase = "transferred"
	session.clock.logic_iterations = 7443
	check(bridge.bank0_slot_state(state) == {"error":"","ready":true}, "slot poll at +2 differs from the native transfer-done state")
	var spring := Session.FrameResult.new()
	bridge.start(state,0,0,spring)
	check(spring.error.is_empty() and spring.audio == [{"kind":"native_bank0_start","sequence":0,"tick_skip":Bank0.SPRING_TICK_SKIP}] and state.opening.prologue_bgm_state.gain == 94, "spring start differs: %s %s" % [spring.error,spring.audio])
	check(_commit(bridge,state,session,spring,7443).is_empty() and engine.calls.back() == ["bank0_start",0,0,7243], "spring native start differs: " + str(engine.calls.back()))
	_gameplay_effects(bridge,state,session,engine)
	# The opening's later pumps keep one block per logic frame after the reset record.
	state.opening.audio_bank = {"epoch":4,"role":"","ready":false}
	session.clock.logic_iterations = 7450
	check(bridge.pump(state).is_empty() and engine.renders == 7250, "bank0 pump after the stage21 reset failed")
	state.opening.audio_bank = {"epoch":9,"role":"","ready":false}
	check(bridge.pump(state) == "active original BGM actual bank changed", "foreign bank record accepted by the bank0 pump")

## Gameplay audio_command calls once SPRING plays (bank0 phase "spring"): the
## school day's class-5 effects 0x501/0x502 (school_day.c:460-463) reach the
## native owner like the confirm's 0x500; another class (0x600) and the
## gameplay effects inside the prologue mode (0x15) are refused unsent.
func _gameplay_effects(bridge: Bridge, state: State, session: Session, engine: FakeEngine) -> void:
	var mode: int = state.game_mode
	state.game_mode = 0x40
	for command: int in [0x501, 0x502]:
		var effect := Session.FrameResult.new(); effect.audio = [{"kind":"native_audio_command","command":command}]
		var sent: int = engine.calls.size()
		check(_commit(bridge,state,session,effect,7443).is_empty() and engine.calls.size() == sent+1 and engine.calls.back().slice(0,2) == ["submit",command], "school-day effect 0x%x not sent to the native owner: %s" % [command,engine.calls.back()])
	var other := Session.FrameResult.new(); other.audio = [{"kind":"native_audio_command","command":0x600}]
	var before: int = engine.calls.size()
	check(not _commit(bridge,state,session,other,7443).is_empty() and engine.calls.size() == before, "class-6 effect 0x600 accepted")
	state.game_mode = 0x15
	var prologue := Session.FrameResult.new(); prologue.audio = [{"kind":"native_audio_command","command":0x501}]
	check(_commit(bridge,state,session,prologue,7443) == "gameplay audio_command(0x501) has no playing bank0 owner" and engine.calls.size() == before, "gameplay effect inside the prologue mode accepted")
	state.game_mode = mode

func _refusals() -> void:
	# Library without the bank0 methods: refused before any native call.
	var old := _bridge(OldEngine.new())
	var r := Session.FrameResult.new()
	old[0].start(old[1],0,1,r)
	check(r.error == Bridge.BANK0_NATIVE_UNAVAILABLE and r.audio.is_empty(), "library without bank0 support accepted: " + r.error)
	# Outside stage16 case4.
	var parts := _bridge(FakeEngine.new())
	parts[1].substep = 3
	r = Session.FrameResult.new(); parts[0].start(parts[1],0,1,r)
	check(r.error == "bank0 sequence 1 start is outside stage16 case4" and parts[4]._engine.calls.is_empty(), "stage16 start outside case4 accepted")
	# No logic-locked clock.
	parts = _bridge(FakeEngine.new()); parts[0]._race = {}
	r = Session.FrameResult.new(); parts[0].start(parts[1],0,1,r)
	check(r.error == "bank0 operation requires the logic-locked clock after the Shiori start", "bank0 start without the locked clock accepted")
	# Pack word not 0x40: the source returns -1 and queues nothing; D1 is then refused.
	parts = _bridge(FakeEngine.new())
	var tampered: PackedByteArray = files[5].duplicate(); tampered[0] = 0x41
	parts[0]._original = {"files":[files[0],files[1],files[2],files[3],files[4],tampered]}
	r = Session.FrameResult.new(); parts[0].start(parts[1],0,1,r)
	check(r.error.is_empty() and r.audio.is_empty() and parts[1].opening.bank0_audio == {"phase":"stage16_ignored"} and parts[1].opening.prologue_bgm_state.role == "prologue_shiori", "non-0x40 pack word queued a start")
	parts[1].mode_stage = 21; parts[1].substep = 0
	var d1 := Session.FrameResult.new()
	Rules.audio_command(parts[1],5,d1); Rules.audio_command(parts[1],0xd1,d1)
	check(_commit(parts[0],parts[1],parts[2],d1,7309) == "D1 fade is outside its source stage20/21 commands" and parts[4]._engine.calls.is_empty(), "D1 over the Shiori sequence accepted")
	# A second ignored D1 without the stage20 D1 is refused by the rules.
	var bare := State.new()
	bare.opening.bgm_fade = {"active":true,"command":0xc1,"request":true}
	bare.opening.bank0_audio = {"phase":"stage16"}
	var restart := Session.FrameResult.new(); Rules.audio_command(bare,0xd1,restart)
	check(restart.error == "BGM fade restart while a source fade request is outstanding is unsupported", "D1 over an outstanding C1 accepted")
	# Engine disagreeing with the source pack decision.
	parts = _bridge(FakeEngine.new()); parts[4]._engine.start_status = -1
	r = Session.FrameResult.new(); parts[0].start(parts[1],0,1,r); parts[1].substep = 5
	check(_commit(parts[0],parts[1],parts[2],r,5000) == "native pack word at 0x8001C000 differs from the source start decision", "native -1 accepted for a queued start")
