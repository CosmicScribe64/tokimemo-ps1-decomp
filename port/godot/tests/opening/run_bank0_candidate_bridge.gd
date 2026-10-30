extends SceneTree
## native-bank0: the pinned candidate BGM bridge driving the real candidate
## library (no fake engine). Run with the candidate .so mounted over
## res://bin/liboriginal_audio... and the candidate bridge (the tracked bridge
## with only LIBRARY_SHA changed) mounted over
## res://services/first_arrival_bgm_bridge.gd; the test refuses any other pair.
##
## The bridge's prepare() checks the library/descriptor pins and registers the
## extension; the Rei prefix runs on the engine directly (as in
## run_bank0_native_audio.gd). From the Rei start the bridge's logic-locked
## clock renders every block. The bridge's preflight/publish then carry, each on
## its source frame: the stage10/8 C1 + Shiori load, the stage11 Shiori start,
## the stage16 audio_bank_start(0,1), stage20/21 commands 5 + D1, the stage21
## load, the +131 0x11 and the +133 spring audio_bank_start(0,0). The engine's
## own bank0_diagnostics() is checked on those frames and its whole pump/key-on
## log is compared with M3 r1 (pumps R+4759..R+7255).
## Env: ORIGINAL_BGM_PACK, REI_BANK_PACK, SHIORI_BANK_PACK, FINISH_BANK_PACK, BANK0_M3.
const Bridge = preload("res://services/first_arrival_bgm_bridge.gd")
const Audio = preload("res://services/audio_service.gd")
const Rules = preload("res://services/opening_audio_rules.gd")
const Shiori = preload("res://services/prologue_shiori_bank_resources.gd")
const Bank0 = preload("res://services/prologue_bank0_audio.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
## .asset-cache/port/native-audio-bank0-v2 (receipt.json).
const CANDIDATE_SHA := "ab2d3cbcb3837edb4c69e3b493f231646cb6abada355e4c9a65d93e84ed89b2e"
## compare_bank0.py --emit of M3 r1 (same table as run_bank0_native_audio.gd).
const EXPECTATIONS_SHA := "3f61a6ba10cbf120312b7bdfc4fbb4da206193d2835a6ba16696a1e3485e5e58"
## Pumps after R, the C1 call frame (M3 r1 events).
const PREFIX := 1949
const SHIORI_POLL := 181
const STAGE16 := 4758
const STAGE20 := 7109
const STAGE21 := 7110
const LAST := 7262
## Logic ticks: Rei start, then C1 (R) PREFIX frames later.
const REI_TICK := 1000
const C1 := REI_TICK + PREFIX
## frame_count = tick - FRAME_OFFSET during a logic frame.
const FRAME_OFFSET := 500

class Sink extends RefCounted:
	var generation := 1
	var pushed := 0
	func validate_active(_generation: int) -> String: return ""
	func can_push() -> bool: return true
	func push(_pcm: PackedVector2Array) -> String: pushed += 1; return ""
class Backend extends RefCounted:
	var _engine: RefCounted
	var _owner: Dictionary = {}
	var sink := Sink.new()
	var _active_sink_generation := 1
	var error := ""
	var history: Array = []
	func rei_action(_o: Dictionary, _a: String, _i: Array = [], _x: bool = false) -> String: return "unexpected Rei action"
	func pump(_current: Dictionary) -> String: return "unexpected wall-clock pump"
	func reset() -> void: pass
	func diagnostics() -> Dictionary: return _engine.diagnostics()

var failures := 0
var checks := 0
var bridge: Bridge
var state: State
var session: Session
var audio: Audio
var engine: RefCounted
var expect: Dictionary

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL: " + label)

func _initialize() -> void:
	var pinned := FileAccess.get_sha256(Bridge.LIBRARY)
	if pinned != CANDIDATE_SHA or Bridge.LIBRARY_SHA != CANDIDATE_SHA:
		printerr("Test requires the native-bank0 candidate library %s with its pinned candidate bridge (library %s, bridge pin %s)" % [CANDIDATE_SHA, pinned, Bridge.LIBRARY_SHA]); quit(1); return
	var path := OS.get_environment("BANK0_M3").path_join("native-expectations.json")
	if FileAccess.get_sha256(path) != EXPECTATIONS_SHA: printerr("M3 expectations differ from the pinned table"); quit(1); return
	expect = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not _setup(): quit(1); return
	_route()
	_negatives()
	print("bank0 candidate bridge: %d checks, %d failures" % [checks,failures])
	if failures == 0: print("PASS: candidate bridge routes bank0 to the real candidate engine on source frames; M3 gain/MVOL/key-ons R+4759..R+7255")
	quit(1 if failures else 0)

func _setup() -> bool:
	audio = Audio.new()
	audio.opening_pack_root = OS.get_environment("SHIORI_BANK_PACK")
	bridge = Bridge.new()
	var prepared := bridge.prepare(audio, root, OS.get_environment("ORIGINAL_BGM_PACK"))
	check(prepared.is_empty() and bridge._shiori_error.is_empty(), "candidate bridge prepare refused the pinned candidate: " + prepared + bridge._shiori_error)
	var store := FinishBank.new()
	var opened := store.open_pack(OS.get_environment("FINISH_BANK_PACK"))
	check(opened.is_empty() and bridge.bind_finish_bank(store).is_empty(), "stage21 reads not bound: " + opened)
	var rei = load("res://services/rei_bank_resources.gd").read_inputs(OS.get_environment("REI_BANK_PACK"))
	if not prepared.is_empty() or not opened.is_empty() or not rei.error.is_empty(): printerr("inputs missing"); return false
	# Rei prefix on the engine itself: arrival BGM, Rei load, B1, Rei start.
	engine = ClassDB.instantiate("OriginalAudioEngine")
	var ok: bool = engine.initialize_inputs(bridge._original.files,93).is_empty() and engine.load_rei_bank([rei.inputs.music_header,rei.inputs.music_body,rei.inputs.sequence]).is_empty() and engine.begin_rei_fade().is_empty()
	for _tick in range(10): ok = ok and engine.tick_rei_fade(false).is_empty() and engine.render_block().error.is_empty()
	ok = ok and engine.start_rei(false).is_empty()
	check(ok, "Rei prefix on the candidate engine failed")
	if not ok: return false
	state = State.new()
	session = Session.new(); session.state = state
	bridge._session = weakref(session)
	var backend := Backend.new(); backend._engine = engine
	bridge._backend = backend
	bridge._committed_after = {"test":true,"bgm_volume_full":93}
	bridge._owner = {"role":"arrival","epoch":1}; backend._owner = bridge._owner
	bridge._rei_epoch = 2; bridge._rei_started = true
	bridge._race = {"rei_tick":REI_TICK,"rendered":0,"fifo":[],"max_fifo":0}
	audio.native_audio_context = {"test":true,"bgm_volume_full":93}
	audio.active_bank_role = "prologue_shiori"; audio.bank_ready = {"arrival":true}
	state.game_mode = 0x15; state.mode_step = 0
	state.opening.native_audio_context = {"test":true,"bgm_volume_full":93}
	state.opening.audio_banks = {"arrival":{"role":"arrival","epoch":1,"ready":true}}
	state.opening.audio_bank = {"role":"prologue_shiori","epoch":3,"ready":false}
	state.opening.prologue_bgm_transition = {"bank_epoch":3}
	state.opening.prologue_bgm_state = {"playing":true,"role":"prologue_rei","bank":1,"sequence":0,"bank_epoch":2,"gain":101}
	return true

func _at(tick: int, stage: int, substep: int) -> void:
	session.clock.logic_iterations = tick
	state.frame_count = tick - FRAME_OFFSET
	state.mode_stage = stage; state.substep = substep

## final_precommit + publish of one logic frame at `tick` (the Session's order).
func _commit(frame: Session.FrameResult, tick: int) -> String:
	session.clock.logic_iterations = tick
	bridge.final_precommit(state,session.rng,frame)
	if not frame.error.is_empty(): return frame.error
	frame.committed = true; session.clock.logic_iterations = tick+1; frame.state_hash = session.state_hash()
	return bridge.publish(frame,tick)

## Logic frames [from, to): the boot's pre-step pump, then the fade mirror.
func _frames(from: int, to: int) -> String:
	for tick in range(from,to):
		session.clock.logic_iterations = tick
		state.frame_count = tick - FRAME_OFFSET
		var pumped := bridge.pump(state)
		if not pumped.is_empty(): return "frame %d: %s" % [tick, pumped]
		Rules.bgm_fade_tick(state,Session.FrameResult.new())
	return ""

func _d() -> Dictionary: return engine.bank0_diagnostics()

## From the C1 frame R through the stage21 load and the reload polls up to
## L+130. Returns the engine pump count at R.
func _to_reload() -> int:
	# Stage10/8 -> stage11/0 (C1 frame R): audio_command(5), (0xC1), Shiori load.
	_at(C1, 11, 0)
	var fc1 := Session.FrameResult.new()
	state.opening.bgm_fade = {}
	Rules.audio_command(state,5,fc1); Rules.audio_command(state,0xc1,fc1)
	fc1.audio.append({"kind":"bank_load","role":"prologue_shiori","bank_epoch":3})
	check(fc1.error.is_empty() and _commit(fc1,C1).is_empty(), "C1 + Shiori load refused: " + fc1.error)
	check(bridge._race.rendered == PREFIX and bridge._shiori.get("tick") == C1, "C1 not at Rei start +%d: %s" % [PREFIX, bridge._race.rendered])
	var r_abs := int(_d().pumps)
	state.opening.bgm_fade = {"active":false,"gain":0,"ticks":112,"command":0xc1,"rate":0xc0,"decrement":1,"accumulator":0}
	state.opening.audio_bank.ready = true
	check(_frames(C1+1, C1+SHIORI_POLL).is_empty(), "Shiori race frames failed")
	# Stage11/2 poll (+181): bgm_play of the Shiori bank on the native engine.
	_at(C1+SHIORI_POLL, 11, 2)
	var fpoll := Session.FrameResult.new()
	check(bridge.shiori_poll(state,fpoll) == -1 and fpoll.error.is_empty(), "Shiori poll refused: " + fpoll.error)
	fpoll.audio.append({"kind":"bgm_play","role":"prologue_shiori","bank_epoch":3,"bank":1,"sequence":0})
	var started := _commit(fpoll,C1+SHIORI_POLL)
	check(started.is_empty() and bridge._shiori.started == true, "Shiori start through the bridge failed: " + started)
	state.opening.prologue_bgm_state = {"playing":true,"role":"prologue_shiori","bank":1,"sequence":0,"bank_epoch":3,"gain":101}
	check(_frames(C1+SHIORI_POLL+1, C1+STAGE16).is_empty(), "Shiori play frames failed")
	# Stage16 case4: audio_bank_start(0,1); line 0x22 then bumps substep.
	_at(C1+STAGE16, 16, 4)
	var f16 := Session.FrameResult.new()
	bridge.start(state,0,1,f16); state.substep = 5
	var s16 := _commit(f16,C1+STAGE16)
	check(s16.is_empty() and _d().queued_commands == 4 and int(_d().pumps) - r_abs == STAGE16, "stage16 start not queued on the engine at R+%d: %s" % [STAGE16, s16])
	var mirrored := _frames(C1+STAGE16+1, C1+STAGE20)
	var d16 := _d()
	check(mirrored.is_empty() and d16.bgm_sequence == 1 and d16.sequence_gain == 93 and not d16.stopped and state.opening.prologue_bgm_state.get("gain") == 93, "PRO_VOL sequence 1 differs between engine and logic: %s %s %s" % [mirrored, d16.sequence_gain, state.opening.prologue_bgm_state])
	# Stage20 case2: audio_command(5), (0xD1).
	_at(C1+STAGE20, 20, 2)
	var f20 := Session.FrameResult.new()
	Rules.audio_command(state,5,f20); Rules.audio_command(state,0xd1,f20)
	state.mode_stage = 21; state.substep = 0
	check(_commit(f20,C1+STAGE20).is_empty() and _d().queued_commands == 2, "stage20 commands not queued on the engine")
	# Stage21 case0: 5, D1 (ignored), the five reads.
	_at(C1+STAGE21, 21, 0)
	var f21 := Session.FrameResult.new()
	Rules.audio_command(state,5,f21); Rules.audio_command(state,0xd1,f21)
	state.opening.bank0_audio.phase = "loading"; state.opening.bank0_audio.load_frame = state.frame_count
	f21.audio.append({"kind":"native_bank0_load"})
	state.substep = 1; state.frame_count += 1
	var loaded := _commit(f21,C1+STAGE21)
	check(loaded.is_empty() and _d().bank0_loading and _d().bank0_load_pump == 0, "stage21 load not on the engine: " + loaded)
	# Reload poll frames until the measured reads-complete frame.
	state.game_mode = 0x34; state.mode_stage = 0xff; state.mode_step = 1; state.substep = 0
	var last_read: int = Bank0.READ_DONE_FRAMES[4]
	var landed_ok := true
	for k in range(1,last_read):
		var pumped := _frames(C1+STAGE21+k, C1+STAGE21+k+1)
		var d := _d()
		var expected := [0,0,0,0,0]
		for row: Array in Bank0.LANDING_SCHEDULE: if row[0] <= k: expected[row[1]] = row[2]
		if not pumped.is_empty() or d.bank0_load_pump != k or d.bank0_landed != expected or d.bank0_complete:
			landed_ok = false; printerr("landing at +%d: %s %s %s" % [k, pumped, d.bank0_landed, expected]); break
	check(landed_ok, "engine landing through the bridge clock differs from the measured schedule")
	return r_abs

func _route() -> void:
	var r_abs := _to_reload()
	var last_read: int = Bank0.READ_DONE_FRAMES[4]
	# A 0x11 one frame early is refused by the owner before reaching the engine.
	var t11 := C1+STAGE21+last_read
	_at(t11-1, 0xff, 0)
	state.opening.bank0_audio.phase = "transferred"; state.opening.bank0_audio.transfer_frame = state.frame_count
	var early := Session.FrameResult.new(); early.audio = [{"kind":"native_audio_command","command":0x11}]
	check(_commit(early,t11-1) == "0x11 poll at +130 differs from the measured reads-complete frame +131" and not _d().bank0_loaded, "0x11 at +130 accepted")
	bridge.abort_frame()
	# +131: the engine itself reports the landing complete on this pump.
	_at(t11, 0xff, 0)
	check(bridge.pump(state).is_empty(), "0x11 frame pump failed")
	var before := _d()
	check(before.bank0_load_pump == last_read and before.bank0_complete and before.bank0_landed == [3,0x20,0x25,0x50,6], "engine landing at the 0x11 frame differs: " + str([before.bank0_load_pump, before.bank0_landed]))
	Rules.bank0_transfer(state)
	state.opening.bank0_audio.transfer_frame = state.frame_count
	var f11 := Session.FrameResult.new(); f11.audio = [{"kind":"native_audio_command","command":0x11}]
	check(_commit(f11,t11).is_empty() and _d().queued_commands == 1 and not _d().bank0_loaded, "0x11 not queued on the engine at +131")
	# +132: slot0 moved, slot1 still requested; the owner reports not ready.
	_at(t11+1, 0xff, 0)
	check(bridge.pump(state).is_empty(), "+132 pump failed")
	var d132 := _d()
	var slot132 := bridge.bank0_slot_state(state)
	check(d132.bank0_loaded and d132.transfer_requests == 2 and not d132.transfer_done and slot132 == {"error":"","ready":false}, "engine slot state at +132 differs: %s %s" % [[d132.transfer_requests, d132.transfer_done], slot132])
	# +133: slot1 moved; ready poll and audio_bank_start(0,0) in one frame.
	_at(t11+2, 0xff, 0)
	check(bridge.pump(state).is_empty(), "+133 pump failed")
	var d133 := _d()
	var slot133 := bridge.bank0_slot_state(state)
	check(d133.transfer_requests == 0 and d133.transfer_done and slot133 == {"error":"","ready":true}, "engine slot state at +133 differs: %s %s" % [[d133.transfer_requests, d133.transfer_done], slot133])
	state.opening.bank0_audio.ready_frame = state.frame_count
	var spring := Session.FrameResult.new()
	bridge.start(state,0,0,spring)
	var sp := _commit(spring,t11+2)
	check(spring.error.is_empty() and sp.is_empty() and _d().queued_commands == 4, "spring start not queued on the engine: %s %s" % [spring.error, sp])
	check(_frames(t11+3, C1+LAST+1).is_empty(), "spring frames failed")
	var d := _d()
	check(d.bgm_sequence == 0 and d.sequence_gain == 94 and not d.stopped and state.opening.prologue_bgm_state.get("gain") == 94 and state.opening.prologue_bgm_state.get("role") == "bank0_spring", "SPRING sequence 0 differs between engine and logic: " + str([d.bgm_sequence, d.sequence_gain]))
	check(int(d.pumps) - r_abs == LAST and bridge._race.rendered == PREFIX + LAST and bridge._race.max_fifo == 0 and bridge._backend.sink.pushed == PREFIX + LAST, "locked clock rendered %d blocks after R, logic %d" % [int(d.pumps) - r_abs, LAST])
	# Source event pumps after R (same as the direct-engine test).
	var events := []
	for row: PackedInt64Array in d.bank0_log:
		if int(row[1]) in [2,3,4,6,7,8] and int(row[0]) - r_abs >= STAGE16: events.append([int(row[0]) - r_abs,int(row[1]),int(row[2])])
	check(events == [[4759,2,0x100],[4769,3,-9],[4769,4,1],[7110,2,0x80],[7111,8,0x80],[7242,6,0],[7242,6,1],[7242,3,27],[7242,7,0],[7243,7,1],[7244,4,0]],"bank0 event pumps through the bridge differ: " + str(events))
	_compare(d, r_abs)

## The bridge's own checks against the real engine: an engine one pump away
## from the logic clock (an out-of-band render_block, which the locked clock
## never does) is refused at the 0x11 frame and at the slot-1 poll.
func _negatives() -> void:
	var t11 := C1+STAGE21+int(Bank0.READ_DONE_FRAMES[4])
	if not _setup(): return
	_to_reload()
	_at(t11, 0xff, 0)
	check(bridge.pump(state).is_empty() and engine.render_block().error.is_empty(), "skew route pump failed")
	Rules.bank0_transfer(state)
	state.opening.bank0_audio.phase = "transferred"; state.opening.bank0_audio.transfer_frame = state.frame_count
	var skewed := Session.FrameResult.new(); skewed.audio = [{"kind":"native_audio_command","command":0x11}]
	var refused := _commit(skewed,t11)
	check(refused == "native bank0 landing +132 differs from logic +131 at the 0x11 poll" and _d().queued_commands == 0 and not _d().bank0_loaded, "real engine one pump ahead at the 0x11 accepted: " + refused)
	if not _setup(): return
	_to_reload()
	_at(t11, 0xff, 0)
	check(bridge.pump(state).is_empty(), "slot route pump failed")
	Rules.bank0_transfer(state)
	state.opening.bank0_audio.phase = "transferred"; state.opening.bank0_audio.transfer_frame = state.frame_count
	var f11 := Session.FrameResult.new(); f11.audio = [{"kind":"native_audio_command","command":0x11}]
	check(_commit(f11,t11).is_empty(), "slot route 0x11 refused")
	_at(t11+1, 0xff, 0)
	check(bridge.pump(state).is_empty() and engine.render_block().error.is_empty(), "slot route +132 pump failed")
	var slot := bridge.bank0_slot_state(state)
	check(slot.get("error") == "native slot1 transfer (ready true, requests 0) at +1 differs from the measured ready frame +2", "real engine slot1 done one pump early accepted: " + str(slot))

func _compare(d: Dictionary, r_abs: int) -> void:
	var log := {}
	var flat: PackedInt64Array = d.bank0_pump_log
	for i in range(0,flat.size(),4): log[int(flat[i]) - r_abs] = [int(flat[i+1]),int(flat[i+2])]
	var kon := {}
	var flat_kon: PackedInt64Array = d.bank0_kon_log
	for i in range(0,flat_kon.size(),4): kon.get_or_add(int(flat_kon[i]) - r_abs,[]).append(int(flat_kon[i+2]))
	var gain_mismatch := []
	var kon_mismatch := []
	var kons := 0
	for key: String in expect.pumps:
		var pump := int(key)
		var row: Array = expect.pumps[key]
		if not expect.skip_gain.has(float(pump)) and log.get(pump) != [int(row[0]),int(row[1])]: gain_mismatch.append(pump)
		var ours: Array = kon.get(pump,[]); ours.sort()
		var theirs: Array = row[2].map(func(x): return int(x))
		kons += theirs.size()
		if ours != theirs: kon_mismatch.append(pump)
	check(expect.pumps.size() == 2497 and kons == 1257,"M3 table coverage differs: %d pumps, %d key-ons" % [expect.pumps.size(),kons])
	check(gain_mismatch.is_empty(),"MVOL/gain through the bridge differs from M3 at pumps " + str(gain_mismatch.slice(0,20)))
	check(kon_mismatch.is_empty(),"key-on samples through the bridge differ from M3 at pumps " + str(kon_mismatch.slice(0,20)))
