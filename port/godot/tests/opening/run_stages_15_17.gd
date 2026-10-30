extends SceneTree
## Opening stages 15-17 (prologue.c:2227-2557). Part 1 drives the flow with
## explicit test doubles (source order, CD gates, retry quirks, built answers,
## same-frame stage switches, RNG). Part 2 uses the original board pack
## (BOARD_PACK) for the O_16/O_07 tables, the effect owner, the queue
## admissions and the line/decode/draw gates. Earlier stages do not reach
## stage15 on the route yet, so entry states are built from the stage14 case6
## source fields with board-pack identities, as run_rei_line17.gd does.
const Flow = preload("res://flow/prologue_arrival.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const Stages = preload("res://services/opening_stages_15_17.gd")
const AudioService = preload("res://services/audio_service.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
var state := State.new()
var services: Dictionary
var events: Array = []
var cd := 1
var decoded: Variant = 1
var done: Variant = false
var moved: Variant = true
var complete_line := false
var bank_error := ""
var failures := 0
var checks := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func effect(_s, kind, value, _out) -> Variant:
	events.append([kind,value])
	if kind == "actor99_done": return done
	if kind == "slide_out": return moved
	return null

func names() -> Array: return events.map(func(e): return e[0])

func tick(stage: int) -> Session.FrameResult:
	events.clear()
	var result := Session.FrameResult.new()
	match stage:
		15: Flow._stage_15_lines_20_to_31(state,services,result)
		16: Flow._stage_16_lines_32_to_36(state,services,result)
		17: Flow._stage_17_line_37(state,services,result)
	return result

func _doubles() -> void:
	services = {"lines20_37_effect":effect,
		"line_start":func(_s,line,_out): events.append(["line",line]),
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["background",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"bank_start":func(_s,bank,sequence,out):
			events.append(["bank_start",bank,sequence])
			if not bank_error.is_empty(): out.error = bank_error,
		"cd_sync":func(_s,_out): return cd,
		"actor_read":func(_s,count,destination,lba,_out): events.append(["read",count,destination,lba]),
		"background_read":func(_s,lba,slot,_out,retry := false): events.append(["bg_read",lba,slot,retry]),
		"background_decode":func(_s,slot,_out): events.append(["decode",slot]); return decoded,
		"audio_command":func(_s,command,_out): events.append(["audio",command])}

const TAIL := [["background",1],["outer",2],["actor_draw",0]]

func _stage15_flow() -> void:
	state = State.new()
	state.game_mode = 0x15; state.mode_stage = 15; state.mode_step = 0; state.substep = 0
	state.opening["prologue_line"] = 0x13
	var stage := 15
	check(tick(stage).error.is_empty() and state.substep == 1 and state.opening.prologue_line == 0x14,"Stage15 line 0x14 start differs")
	check(events == [["slide_in",0],["line",0x14]] + TAIL,"Case0 must slide, then start line 0x14: " + str(events))
	tick(stage); check(state.substep == 1 and events == [["slide_in",0],["run",1],["lip_sync",1]] + TAIL,"Case1 slide/run/lip-sync order differs: " + str(events))
	# Walk every line pair; record which effects follow each run substep.
	var expected_runs := {1:["lip_sync"],3:[],7:["lip_sync"],9:[],11:["lip_sync"],13:["pulse"],15:["lip_sync"],17:["pulse"],19:["lip_sync"],21:["pulse"],23:["lip_sync"],25:[],28:[],30:[]}
	var lines := {2:0x15,6:0x16,8:0x17,10:0x18,14:0x19,18:0x1a,20:0x1b,22:0x1c,24:0x1d,27:0x1e,29:0x1f}
	complete_line = true; tick(stage); complete_line = false
	while state.substep < 31:
		var sub: int = state.substep
		if lines.has(sub):
			var result := tick(stage)
			var head: Array = [["actor100_set",0]] if sub == 10 else []
			check(result.error.is_empty() and events == head + [["line",lines[sub]]] + TAIL and state.opening.prologue_line == lines[sub] and state.substep == sub+1,"Case%d line start differs: %s" % [sub,str(events)])
		elif sub in [12,16]:
			var before_line: int = state.opening.prologue_line
			tick(stage)
			check(events == [["line",100 if sub == 12 else 101]] + TAIL and state.opening.prologue_line == before_line and before_line == (0x18 if sub == 12 else 0x19) and state.substep == sub+1,"Built answer %d must start without touching prologue_line: %s" % [sub,str(events)])
			check(not names().has("audio") and not names().has("lip_sync"),"Built answer issued audio or lip-sync")
		elif sub == 4:
			tick(stage); check(events == [["actor99_set",0]] + TAIL and state.substep == 5,"Case4 actor99 setup differs: " + str(events))
		elif sub == 5:
			done = false; tick(stage); check(state.substep == 5 and events == [["actor99_done",0]] + TAIL,"Case5 advanced with actor99 still animating")
			done = 1; check(tick(stage).error == "stage15 actor99 wait requires an actual sequence state" and state.substep == 5,"Non-bool actor99 completion accepted")
			done = true; tick(stage); check(state.substep == 6,"Case5 did not advance on actor99 completion")
		elif sub == 26:
			moved = true; tick(stage); check(state.substep == 26 and events == [["slide_out",0]] + TAIL,"Case26 slide differs: " + str(events))
			moved = "x"; check(tick(stage).error == "stage15 exit slide requires actual actor positions","Non-bool slide accepted")
			moved = false; tick(stage); check(state.substep == 27 and events == [["slide_out",0]] + TAIL,"Case26 end did not advance: " + str(events))
		elif expected_runs.has(sub):
			tick(stage)
			var want: Array = [["run",sub]]
			if sub == 11: want = [["actor100_rise",0],["run",sub]]
			for kind: String in expected_runs[sub]: want.append([kind,sub])
			check(events == want + TAIL,"Case%d run differs: %s" % [sub,str(events)])
			complete_line = true; tick(stage); complete_line = false
		else:
			check(false,"Unexpected stage15 substep %d" % sub); return
	check(state.substep == 31,"Stage15 line walk ended at %d" % state.substep)
	tick(stage); check(state.substep == 32 and events == [["bg_read",0xacd4,1,false]] + TAIL,"Case31 ACD4 read differs: " + str(events))
	cd = 0; tick(stage); check(state.substep == 32 and events == TAIL,"Case32 read CUT_07 before ACD4 completed")
	cd = 1; tick(stage); check(state.substep == 33 and events == [["read",49,0x801a4000,0xaa54]] + TAIL,"Case32 CUT_07 read differs: " + str(events))
	cd = 0; tick(stage); check(state.substep == 33 and events == TAIL,"Case33 read O_07 before CUT_07 completed")
	cd = 1; tick(stage); check(state.substep == 34 and events == [["read",25,0x801e8000,0xabca]] + TAIL,"Case33 O_07 read differs: " + str(events))
	cd = 0; tick(stage); check(state.substep == 34 and not names().has("decode"),"Case34 decoded before O_07 completed")
	cd = 1; decoded = "1"; check(tick(stage).error == "stage15 P_01_04 background decode result is unknown","Non-integer decode accepted")
	decoded = -1; tick(stage)
	check(state.mode_stage == 15 and state.substep == 34 and events == [["decode",1],["bg_reset",0],["read",49,0x801a4000,0xaa54]] + TAIL,"Case34 failure must reset and re-read CUT_07, not the background: " + str(events))
	decoded = 1; tick(stage)
	check(state.mode_stage == 16 and state.substep == 0 and events == [["decode",1],["next_scene",0]] + TAIL,"Case34 success/stage switch differs: " + str(events))
	for bad: Array in [[15,0,35],[15,0,-1],[15,1,0]]:
		state.mode_stage = bad[0]; state.mode_step = bad[1]; state.substep = bad[2]
		check(tick(stage).error == "prologue stage15 is outside source dispatch" and events.is_empty(),"Stage15 accepted step/substep %s" % str(bad))
	state.mode_step = 0; state.substep = 0; services.erase("lines20_37_effect")
	check(tick(stage).error == "source stage15-17 actor services are unavailable" and events.is_empty(),"Stage15 ran without its effect service")
	services["lines20_37_effect"] = effect

func _stage16_flow() -> void:
	state = State.new()
	state.game_mode = 0x15; state.mode_stage = 16; state.mode_step = 0; state.substep = 0
	var stage := 16
	var lines := {0:0x20,2:0x21,4:0x22,6:0x23,8:0x24}
	while state.substep < 10:
		var sub: int = state.substep
		if lines.has(sub):
			if sub == 4:
				bank_error = "bank refused"
				check(tick(stage).error == "bank refused" and state.substep == 4 and events == [["bank_start",0,1]],"Bank refusal must stop case4 before line 0x22")
				bank_error = ""
			tick(stage)
			var head: Array = [["bank_start",0,1]] if sub == 4 else []
			check(events == head + [["line",lines[sub]]] + TAIL and state.substep == sub+1,"Stage16 case%d differs: %s" % [sub,str(events)])
		else:
			tick(stage)
			var want: Array = [["run",sub]]
			if sub in [1,5,9]: want.append(["lip_sync",sub])
			check(events == want + TAIL,"Stage16 case%d run differs: %s" % [sub,str(events)])
			complete_line = true; tick(stage); complete_line = false
	tick(stage); check(state.substep == 11 and events == [["bg_read",0x496a,1,false]] + TAIL,"Stage16 P_01_10 read must use slot1: " + str(events))
	cd = 0; tick(stage); check(state.substep == 11 and events == TAIL,"Stage16 decoded before its read completed")
	cd = 1; decoded = -1; tick(stage)
	check(state.substep == 11 and events == [["decode",1],["bg_reset",0],["bg_read",0x496a,1,true]] + TAIL,"Stage16 failure must reset and re-read P_01_10: " + str(events))
	decoded = 1; tick(stage)
	check(state.mode_stage == 17 and state.substep == 0 and events == [["decode",1],["clear_scene",0]] + TAIL,"Stage16 success differs: " + str(events))
	state.mode_stage = 16; state.substep = 12
	check(tick(stage).error == "prologue stage16 is outside source dispatch","Stage16 accepted substep 12")

func _stage17_flow() -> void:
	state = State.new()
	state.game_mode = 0x15; state.mode_stage = 17; state.mode_step = 0; state.substep = 0
	var stage := 17
	tick(stage); check(state.substep == 1 and events == [["line",0x25]] + TAIL and state.opening.prologue_line == 0x25,"Stage17 line 0x25 differs")
	tick(stage); check(events == [["run",1]] + TAIL,"Stage17 run differs")
	complete_line = true; tick(stage); complete_line = false
	tick(stage); check(state.substep == 3 and events == [["bg_read",0x4973,1,false]] + TAIL,"Stage17 P_01_12 read differs: " + str(events))
	cd = 0; tick(stage); check(state.substep == 3 and events == TAIL,"Stage17 decoded before its read completed")
	cd = 1; decoded = -1; tick(stage)
	check(state.substep == 3 and events == [["decode",1],["bg_reset",0],["bg_read",0x4973,1,true]] + TAIL,"Stage17 failure differs: " + str(events))
	decoded = 0; tick(stage)
	check(state.mode_stage == 18 and state.substep == 0 and events == [["decode",1],["read",0x41,0x80180000,0xac50]] + TAIL,"Stage17 must read O_13 and switch in the same frame: " + str(events))
	state.mode_stage = 17; state.substep = 4
	check(tick(stage).error == "prologue stage17 is outside source dispatch","Stage17 accepted the old substep 4")

func _rng_checks() -> void:
	# Stage15-17 bodies call no rand(); an error frame restores state and RNG.
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,19950404)
	var all: Dictionary = services.duplicate()
	all.merge({"bank_load_start":func(_s,_r,_o): pass,"scene_reset":func(_s,_o): pass,"bank_poll":func(_s,_o): return 1,
		"actor_palette_setup":func(_s,_o): pass},true)
	for stage: int in [15,16,17]:
		var s := State.new()
		s.game_mode = 0x15; s.mode_stage = stage; s.mode_step = 0; s.substep = 1
		var before: Dictionary = rng.checkpoint()
		var result := Flow.step(s,rng,0,all)
		check(result.error.is_empty() and rng.checkpoint() == before,"Stage%d consumed RNG draws" % stage)
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 16; s.mode_step = 0; s.substep = 4
	var projection: Dictionary = s.projection()
	bank_error = "bank refused"
	var refused := Flow.step(s,rng,0,all)
	bank_error = ""
	check(refused.error == "bank refused" and s.projection() == projection,"Refused stage16 frame did not restore its state")

# ---- part 2: original board bytes --------------------------------------

func _entry(board: Board) -> State:
	# Stage14 case6 (prologue.c:2100-2121) as it leaves the O_16 actor96.
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 15; s.mode_step = 0; s.substep = 0
	s.opening["actor96"] = {"profile":"o16_shiori_line19","index":96,"unk_01":0xb,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x80180258,"sequence_table":0x80180298,"tpage_table":0x8018024c,"frame_index":0,"sequence_counter":0,"speed":1,
		"brightness":0x80,"x":-0xf0,"y":-0x78,"resource_id":Board.O_16_ID}
	s.opening["actor96_draw_flags"] = 0xa4
	s.opening["names_media"] = {"bg_show_1":0,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xad0b},"active_draw":"dec_bg_draw"}
	s.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"attribute":0x41000000,"resource_id":Board.CUT_16_ID}
	s.opening["media"] = {"epoch":5,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	return s

func _fx(media: Media, s: State, kind: String) -> Array:
	# Effects after line_run carry the dispatched substep, as the flow does.
	var result := Session.FrameResult.new()
	var value: Variant = media.stages_15_17.effect(s,kind,s.substep if kind in Stages.AFTER_LINE_RUN else 0,result)
	return [result.error,value]

func _rejects(media: Media, s: State, kind: String, error: String, label: String) -> void:
	var before: Dictionary = s.opening.duplicate(true)
	var outcome := _fx(media,s,kind)
	check(outcome[0] == error and s.opening == before,label + ": " + str(outcome[0]))

func _sample(s: State, line: int, cd: int, levels: Array = [30,30,30,30]) -> void:
	# What the voiced line_run does before the effect: begin the line's ring,
	# then one getCDlevel sample (AudioRules.step_lip_sync).
	s.opening["prologue_line"] = line
	AudioRules.begin_line_lip_sync(s,line,0,[levels])
	s.opening["cd_audio_state_override"] = cd
	AudioRules.step_lip_sync(s)

func _complete(queue: Queue, s: State, request_id: String, status: String = "ready") -> void:
	var request: Dictionary = s.opening.media.requests[request_id]
	var result := Session.FrameResult.new()
	queue.complete(Session.ServiceEvent.new(0,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,Queue.SERVICE,
		{"epoch":request.epoch,"request_id":request_id,"request_key":request.request_key,"status":status,"resource_id":request.resource_id}),s,null,result)
	check(result.error.is_empty(),"Completion of %s refused: %s" % [request_id,result.error])

func _board_checks() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error); failures += 1; return
	var o16: PackedByteArray = board.stage_transfer(Board.O_16_ID).bytes
	var o07: PackedByteArray = board.stage_transfer(Board.O_06_ID).bytes
	check(Stages.sequence_rows(o16,0x80180298,0) == [[1,2],[2,2],[3,2],[4,2],[3,2],[5,1]],"O_16 sequence 0 differs from the spec")
	check(Stages.sequence_rows(o16,0x80180298,3) == [[14,10],[15,10]],"O_16 sequence 3 differs from the spec")
	check(Stages.sequence_rows(o16.slice(0,0x200),0x80180298,0).is_empty(),"Truncated O_16 resolved a sequence")
	check(Stages.tpage_slots(o16,Stages.O16_TABLES,[[0x80180258,0]]) == {0x82c0:5,0x42c0:6},"O_16 frame0 tpage slots differ")
	check(Stages.tpage_slots(o16,Stages.O16_TABLES,[[0x80180258,0],[0x80180258,7],[0x80180258,11],[0x80180258,1],[0x80180258,14]]) == {0x82c0:5,0x42c0:6,0x2c0:7},"O_16 lip/actor99/actor100 tpage slots differ")
	check(Stages.tpage_slots(o07,Stages.O07_TABLES,[[0x801842c4,0],[0x801800f8,0]]) == {0x4310:5,0x8310:6,0x130:7},"O_07 actor96/99 tpage slots differ")
	check(Stages.tpage_slots(o16,Stages.O16_TABLES,[[0x80180258,16]]).is_empty(),"Out-of-range O_16 frame resolved")
	var media := Media.new()
	media._board_store = board
	check(media.stages_15_17.prepare(board).is_empty(),"Stage15-17 effects refused the board pack")
	var unprepared := Stages.new()
	var probe := Session.FrameResult.new()
	unprepared.effect(_entry(board),"slide_in",0,probe)
	check(probe.error == "stage15-17 effects are outside their prepared source scope","Unprepared effects ran")
	var s := _entry(board)
	# Entry slide: 80 frames over cases 0-1, actor97 follows.
	for frame in 85: _fx(media,s,"slide_in")
	check(s.opening.actor96.x == -0xa0 and s.opening.actor97.x == -0xa0 and s.opening.actor97.profile == "cleared","Entry slide differs: " + str(s.opening.actor96.x))
	s.substep = 1
	var unsampled := s.projection()
	s.opening["prologue_line"] = 0x14; AudioRules.begin_line_lip_sync(s,0x14,0,[[30]])
	_rejects(media,s,"lip_sync",Stages.LIP_SAMPLE_MISSING % 15,"Lip sync stepped the ring itself without line_run's sample")
	s.restore(unsampled)
	_sample(s,0x14,0)
	_fx(media,s,"lip_sync")
	check(not s.opening.has("actor97") and not s.opening.has("actor98"),"Idle lip-sync must clear actor97 and leave actor98 alone")
	_sample(s,0x14,4)
	var lipped := _fx(media,s,"lip_sync")
	check(lipped[0].is_empty() and s.opening.actor97.frame_index in [6,7,8,9] and s.opening.actor98.frame_index == s.opening.actor97.frame_index+4 and s.opening.actor98.x == -0xa0 and s.opening.actor97.y == -0x78,"Voiced lip-sync fields differ")
	s.opening.erase("cd_audio_state_override")
	# Case4/5: the one-shot actor99 sequence clears flags after 11 draws.
	s.substep = 4; check(_fx(media,s,"actor99_set")[0].is_empty() and s.opening.actor99.flags == 5 and s.opening.actor99.sequence_slot == 0,"Actor99 setup differs")
	s.substep = 5
	var draws := 0
	while _fx(media,s,"actor99_done")[1] == false and draws < 40:
		check(_fx(media,s,"actor_draw")[0].is_empty(),"Actor draw refused during the actor99 sequence")
		draws += 1
	check(draws == 11 and s.opening.actor99.seq_step == 5 and s.opening.actor99.flags == 0,"Actor99 one-shot length differs: %d" % draws)
	check(s.opening.o16_tpages.slots.has(str(0x2c0)),"Actor99 frames did not take the 0x2C0 page")
	# Case10/11/13: actor100 rise, pulse loop, lip-sync stop.
	s.substep = 10; _fx(media,s,"actor100_set")
	check(s.opening.actor100.y == -0x38 and s.opening.actor100.sequence_slot == 3 and s.opening.D_800B5BD4 == 9,"Actor100 setup differs")
	s.substep = 11
	for frame in 70: _fx(media,s,"actor100_rise")
	check(s.opening.actor100.y == -0x78,"Actor100 rise differs: " + str(s.opening.actor100.y))
	s.substep = 13; _fx(media,s,"pulse")
	check(s.opening.actor100.flags == 1 and s.opening.D_800B5BD4 == 0xf,"Pulse differs")
	for frame in 25: _fx(media,s,"actor_draw")
	check(s.opening.actor100.seq_step == 0 and s.opening.actor100.flags == 1,"Actor100 loop must wrap to step 0")
	s.substep = 15; _sample(s,0x19,4); _fx(media,s,"lip_sync"); s.opening.erase("cd_audio_state_override")
	check(s.opening.actor100.flags == 0,"Voiced lip-sync must stop actor100")
	# Case26: 120 frames of -2 to -400, then the advance frame.
	s.substep = 26
	var steps := 0
	while _fx(media,s,"slide_out")[1] == true and steps < 200: steps += 1
	check(steps == 120 and s.opening.actor96.x == -400 and s.opening.actor101.x == -400 and s.opening.actor101.profile == "cleared" and s.opening.actor99.x == -400,"Exit slide differs: %d" % steps)
	# Wrong substep, owner and the stage14 switch-frame blink.
	s.substep = 6
	_rejects(media,s,"pulse","stage15 effect is outside its source substep","Pulse accepted case6")
	_rejects(media,s,"next_scene","stage15 effect is outside its source substep","Next scene accepted case6")
	var armed := _entry(board)
	armed.opening["actor98"] = {"profile":"o15_blink_over_o16","index":98,"flags":3,"draw_flags":0xac}
	_rejects(media,armed,"slide_in",Stages.ENTRY_BLINK_UNSUPPORTED,"Armed stage14 blink accepted")
	var foreign := _entry(board); foreign.opening.actor96.resource_id = Board.O_09_ID
	_rejects(media,foreign,"slide_in","stage15 effect requires its original O_16 actor96 owner","Foreign actor96 accepted")
	# Case31-34 through the real queue and decode branch.
	var queue := media.queue
	check(queue.prepare(board.background_descriptions(),board.actor_descriptions(),{}).is_empty(),"Queue refused the board descriptors")
	var t := _entry(board)
	t.opening["media"] = {"epoch":5,"next_id":0,"requests":{"old":{"request_id":"old","kind":"background","status":"ready","payload":{"lba":0xacd4,"slot":1,"retry":false},"resource_id":Board.BG_ACD4_ID}},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	var result := Session.FrameResult.new()
	t.substep = 30; queue.background_read(t,0xacd4,1,result)
	check(result.error == "opening background cache reuse is not implemented for this resource","ACD4 cache refusal outside case31 changed")
	for wrong: Array in [[31,0x31,0x801a4000,0xaa54],[32,0x19,0x801f0000,0xabca],[33,0x19,0x801f0000,0xabca],[32,0x41,0x80180000,0xac50]]:
		t.substep = wrong[0]; result = Session.FrameResult.new()
		queue.actor_read(t,wrong[1],wrong[2],wrong[3],result)
		check(not result.error.is_empty(),"Queue admitted %s" % str(wrong))
	t.substep = 31; result = Session.FrameResult.new(); queue.background_read(t,0xacd4,1,result)
	check(result.error.is_empty(),"Case31 ACD4 read refused despite the source cache miss: " + result.error)
	t.opening.media.requests.erase("old")
	var bg_id: String = t.opening.media.background_pending["1"]
	t.substep = 32; result = Session.FrameResult.new(); queue.actor_read(t,0x31,0x801a4000,0xaa54,result)
	check(result.error.is_empty(),"Case32 CUT_07 read refused: " + result.error)
	var cut_id: String = t.opening.media.actor_pending[str(0x801a4000)]
	t.substep = 33; result = Session.FrameResult.new(); queue.actor_read(t,0x19,0x801e8000,0xabca,result)
	check(result.error.is_empty() and t.opening.media.requests[t.opening.media.actor_pending[str(0x801e8000)]].resource_id == Board.O_06_ID,"Case33 O_07 read refused: " + result.error)
	var o07_id: String = t.opening.media.actor_pending[str(0x801e8000)]
	t.substep = 34
	var unfinished := Session.FrameResult.new()
	media.background_decode(t,1,unfinished)
	check(unfinished.error == "opening background decode has no matching completed ACD4 read","Pending ACD4 decoded")
	_rejects(media,t,"next_scene","stage15 next scene lacks completed original CUT_07/O_07 reads","Next scene accepted unfinished reads")
	_complete(queue,t,bg_id); _complete(queue,t,cut_id); _complete(queue,t,o07_id)
	_rejects(media,t,"next_scene","stage15 next scene requires the decoded P_01_04 background","Next scene accepted an undecoded background")
	for broken: String in ["epoch","destination"]:
		var bad := State.new(); bad.restore(t.projection())
		if broken == "epoch": bad.opening.media.requests[o07_id].epoch = 4
		else: bad.opening.media.requests[o07_id].payload.destination = 0x801f0000
		bad.opening.names_media["bg_show_1"] = 1; bad.opening.names_media.buffer_1.lba = 0xacd4
		_rejects(media,bad,"next_scene","stage15 next scene lacks completed original CUT_07/O_07 reads","Next scene accepted O_07 with wrong " + broken)
	# The hidden buffer follows the live dec_bg_show[1]: shown 1 decodes into 0.
	var shown := State.new(); shown.restore(t.projection()); shown.opening.names_media.bg_show_1 = 1
	result = Session.FrameResult.new()
	check(media.background_decode(shown,1,result) == 0 and result.error.is_empty() and shown.opening.names_media.bg_show_1 == 0 and shown.opening.names_media.buffer_0.decode_rect == [704,0,320,160] and shown.opening.names_media.buffer_1.lba == 0xad0b,"Decode with buffer 1 shown did not fill buffer 0: " + result.error)
	result = Session.FrameResult.new()
	check(media.background_decode(t,1,result) == 1 and result.error.is_empty() and t.opening.names_media.bg_show_1 == 1 and t.opening.names_media.buffer_1.lba == 0xacd4,"Case34 ACD4 decode differs: " + result.error)
	var next := _fx(media,t,"next_scene")
	check(next[0].is_empty() and t.opening.actor96.profile == Stages.O07_PROFILE and t.opening.actor96.frame_table == 0x801842c4 and t.opening.actor99.sequence_table == 0x80180114 and not t.opening.has("actor100") and t.opening.bg_layer_0.resource_id == Board.CUT_06_ID and t.opening.o07_copy.bytes == 0xc800,"Case34 setup differs: " + str(next[0]))
	_rejects(media,t,"next_scene","stage15 effect requires its original O_16 actor96 owner","Case34 setup replayed over the O_07 actors")
	t.next_stage()
	var stage16 := State.new(); stage16.restore(t.projection())
	check(_fx(media,t,"actor_draw")[0].is_empty() and t.opening.o07_tpages.slots == {str(0x4310):5,str(0x8310):6,str(0x130):7},"Stage16 O_07 tpage slots differ: " + str(t.opening.get("o07_tpages")))
	# Line-start gates: substep, owner and stage.
	var gate := _entry(board)
	result = Session.FrameResult.new(); media.before_line0(gate,0x14,result)
	check(result.error.is_empty(),"Line 0x14 start refused at 15/0: " + result.error)
	for case: Array in [[12,100,""],[13,100,"x"],[12,101,"x"],[16,101,""],[0,0x15,"x"]]:
		gate.substep = case[0]; result = Session.FrameResult.new(); media.before_line0(gate,case[1],result)
		check(result.error.is_empty() == (case[2] == ""),"Line %d at 15/%d gate differs" % [case[1],case[0]])
	t.substep = 4; result = Session.FrameResult.new(); media.before_line0(t,0x22,result)
	check(result.error.is_empty(),"Line 0x22 start refused at 16/4")
	gate.mode_stage = 16; gate.substep = 4; result = Session.FrameResult.new(); media.before_line0(gate,0x22,result)
	check(not result.error.is_empty(),"Line 0x22 accepted the O_16 owner")
	# Stage16: Yoshio lip-sync and the clear; stage17: O_13 admission.
	t.substep = 1; _sample(t,0x20,4); _fx(media,t,"lip_sync"); t.opening.erase("cd_audio_state_override")
	check(t.opening.actor97.frame_table == 0x801842c4 and t.opening.actor97.frame_index in [3,4,5,6] and t.opening.actor97.x == -0xa0,"Yoshio lip-sync differs")
	t.substep = 11
	_rejects(media,t,"clear_scene","stage16 clear requires the decoded P_01_10 background","Clear accepted without P_01_10")
	t.substep = 10
	_rejects(media,t,"clear_scene","stage16 effect is outside its source substep","Clear accepted case10")
	t.substep = 11; t.opening.names_media.buffer_0 = {"lba":0x496a}; t.opening.names_media.bg_show_1 = 0
	check(_fx(media,t,"clear_scene")[0].is_empty() and t.opening.bg_layer_0.attribute == 0x80000000 and t.opening.bg_layer_0.enabled == 0 and not t.opening.has("actor96"),"Stage16 clear differs")
	t.next_stage()
	var stage17 := State.new(); stage17.restore(t.projection())
	result = Session.FrameResult.new(); media.before_line0(t,0x25,result)
	check(result.error.is_empty(),"Line 0x25 start refused at 17/0: " + result.error)
	for lba: int in [0x496a,0x4973]:
		var probe_state := State.new(); probe_state.restore(t.projection())
		result = Session.FrameResult.new(); queue.background_read(probe_state,lba,1,result)
		check(result.error.is_empty() and probe_state.opening.media.requests[probe_state.opening.media.background_pending["1"]].resource_id == (Board.BG_496A_ID if lba == 0x496a else Board.BG_4973_ID),"Exported background %x was refused: %s" % [lba,result.error])
	t.substep = 2; result = Session.FrameResult.new(); queue.actor_read(t,0x41,0x80180000,0xac50,result)
	check(not result.error.is_empty(),"O_13 admitted at 17/2")
	t.substep = 3; result = Session.FrameResult.new(); queue.actor_read(t,0x41,0x80180000,0xac50,result)
	check(result.error.is_empty() and t.opening.media.requests[t.opening.media.actor_pending[str(0x80180000)]].resource_id == Board.O_13_ID,"O_13 read refused at 17/3: " + result.error)
	# dec_bg_draw needs the prepared media owner and a visible renderer; the
	# rendered scenes are covered by tests/opening/run_render_15_17.gd.
	for stage: int in [15,16,17]:
		var view := _entry(board); view.mode_stage = stage
		result = Session.FrameResult.new(); media._store = null
		media.arrival_draw(view,1,result)
		check(result.error == "stages 15-17 dec_bg_draw requires slot 1 and the board media","Stage%d draw without its owner was not refused: %s" % [stage,result.error])
	_real_effect_frames(media,board,stage16,stage17)
	_effect_guards(media,board,stage16,stage17)
	# Built answers stay silent player lines.
	var audio := AudioService.new()
	for line: int in [100,101]:
		var event: Dictionary = audio.original_opening_line_event(line)
		check(event.get("kind") == "voice_silent" and event.get("speaker") == 15 and event.get("xa_requests") == [],"Built answer %d has a voice" % line)

# ---- part 3: flow bodies with the real effect -------------------------

func _real_services(media: Media) -> Dictionary:
	# line_run as the voiced line service does it: one getCDlevel sample
	# (step_lip_sync), then run_line's substep++ when the hold completes.
	return {"lines20_37_effect":media.stages_15_17.effect,
		"line_run":func(s,_out):
			AudioRules.step_lip_sync(s)
			if complete_line:
				s.mode_step_timer = 0; s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["bg",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"cd_sync":func(_s,_out): return 1,
		"background_decode":func(_s,slot,_out): events.append(["decode",slot]); return 0,
		"actor_read":func(_s,count,destination,lba,_out): events.append(["read",count,destination,lba]),
		"background_read":func(_s,lba,slot,_out,retry := false): events.append(["bg_read",lba,slot,retry]),
		"bank_start":func(_s,_bank,_sequence,_out): pass,
		"line_start":func(_s,_line,_out): pass}

func _lip_frames(s: State, stage: int, sub: int, line: int, real: Dictionary) -> void:
	# Three holding frames, then the frame run_line completes: each frame
	# takes exactly one ring sample and the effect still runs on the last.
	var levels := [0,10,20,30,40,50,60,70,80,90]
	s.substep = sub; s.mode_step_timer = 0
	s.opening["prologue_line"] = line
	AudioRules.begin_line_lip_sync(s,line,0,[levels])
	s.opening["cd_audio_state_override"] = 4
	for frame in 4:
		complete_line = frame == 3
		s.frame_count += 1
		var result := Session.FrameResult.new()
		if stage == 15: Flow._stage_15_lines_20_to_31(s,real,result)
		else: Flow._stage_16_lines_32_to_36(s,real,result)
		complete_line = false
		var lip: Dictionary = s.opening.prologue_lip_sync
		var mouth: int = (int(lip.ring_frame)+6) if stage == 15 else 6-int(lip.ring_frame)
		check(result.error.is_empty() and lip.clip_frame == frame+1 and lip.cd_level == levels[frame] and s.opening.get("actor97",{}).get("frame_index") == mouth,
			"Stage%d sub%d frame%d: error='%s' clip_frame=%s" % [stage,sub,frame,result.error,str(lip.clip_frame)])
		if stage == 15: check(s.opening.get("actor98",{}).get("frame_index") == int(lip.ring_frame)+10,"Stage15 sub%d actor98 mouth differs" % sub)
	check(s.substep == sub+1,"Stage%d sub%d line did not complete: substep=%d" % [stage,sub,s.substep])
	s.opening.erase("cd_audio_state_override")

func _real_effect_frames(media: Media, board: Board, stage16: State, stage17: State) -> void:
	var real := _real_services(media)
	var s := _entry(board)
	s.opening.actor96.x = -0xa0
	s.substep = 10; check(_fx(media,s,"actor100_set")[0].is_empty(),"Actor100 setup refused")
	for item: Array in [[1,0x14],[7,0x16],[11,0x18],[15,0x19],[19,0x1a],[23,0x1c]]:
		_lip_frames(s,15,item[0],item[1],real)
	for item: Array in [[13,100],[17,101],[21,0x1b]]:
		s.substep = item[0]; s.mode_step_timer = 0
		if item[1] < 100: s.opening["prologue_line"] = item[1]
		AudioRules.begin_line_lip_sync(s,item[1],0,[])
		complete_line = true
		var result := Session.FrameResult.new()
		Flow._stage_15_lines_20_to_31(s,real,result)
		complete_line = false
		check(result.error.is_empty() and s.substep == item[0]+1 and s.opening.actor100.flags == 1 and s.opening.D_800B5BD4 == 0xf,"Stage15 pulse sub%d line did not complete: %s" % [item[0],result.error])
	var y := State.new(); y.restore(stage16.projection())
	for item: Array in [[1,0x20],[5,0x22],[9,0x24]]:
		_lip_frames(y,16,item[0],item[1],real)
	# Stage17 case3 success: O_13 read, mode_stage_next, then the shared frame end.
	var z := State.new(); z.restore(stage17.projection())
	z.substep = 3
	var all: Dictionary = real.duplicate()
	all.merge({"bank_load_start":func(_s,_r,_o): pass,"scene_reset":func(_s,_o): pass,"bank_poll":func(_s,_o): return 1,
		"actor_palette_setup":func(_s,_o): pass},true)
	var rng := Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE,19950404)
	events.clear()
	var switched := Flow.step(z,rng,0,all)
	check(switched.error.is_empty() and z.mode_stage == 18 and z.substep == 0 and events == [["decode",1],["read",0x41,0x80180000,0xac50],["bg",1],["outer",2]],
		"Stage17->18 switch frame differs: error='%s' stage=%d events=%s" % [switched.error,z.mode_stage,str(events)])

func _effect_guards(media: Media, board: Board, stage16: State, stage17: State) -> void:
	const SCOPE := "stage15-17 effects are outside their prepared source scope"
	var s := _entry(board)
	var y := State.new(); y.restore(stage16.projection())
	var z := State.new(); z.restore(stage17.projection())
	for item: Array in [[s,"slide_in"],[y,"actor_draw"],[z,"actor_draw"]]:
		item[0].mode_step = 1
		_rejects(media,item[0],item[1],SCOPE,"Stage%d effect ran with mode_step 1" % item[0].mode_stage)
		item[0].mode_step = 0
	# After-line-run effects: only the dispatched substep or the one run_line entered.
	_sample(s,0x14,4); s.substep = 3
	var before: Dictionary = s.opening.duplicate(true)
	var result := Session.FrameResult.new()
	media.stages_15_17.effect(s,"lip_sync",1,result)
	check(result.error == "stage15 effect is outside its source substep" and s.opening == before,"Lip sync accepted two substeps past its dispatch")
	s.substep = 2; s.mode_step_timer = 5; result = Session.FrameResult.new()
	media.stages_15_17.effect(s,"lip_sync",1,result)
	check(result.error == "stage15 effect is outside its source substep" and s.opening == before,"Lip sync accepted substep+1 without run_line's completion")
	s.substep = 3; s.mode_step_timer = 0; result = Session.FrameResult.new()
	media.stages_15_17.effect(s,"lip_sync",3,result)
	check(result.error == "stage15 effect is outside its source substep" and s.opening == before,"Lip sync accepted dispatch from case3")
	# Stage16 owners: the O_16 entry actor96 and a foreign actor96.
	var o16 := _entry(board); o16.mode_stage = 16; o16.substep = 11
	_rejects(media,o16,"bg_reset","stage16 effect requires its original O_07 actor96 owner","Stage16 accepted the O_16 actor96")
	var foreign := State.new(); foreign.restore(stage16.projection()); foreign.opening.actor96.resource_id = Board.O_09_ID; foreign.substep = 11
	_rejects(media,foreign,"bg_reset","stage16 effect requires its original O_07 actor96 owner","Stage16 accepted a foreign actor96")
	# Stage17 owner: a leftover upper actor or a live layer 0.
	var leftover := State.new(); leftover.restore(stage17.projection()); leftover.substep = 3
	leftover.opening["actor97"] = {"profile":Stages.O07_PROFILE,"index":97}
	_rejects(media,leftover,"bg_reset","stage17 effect requires the cleared stage16 scene","Stage17 accepted a leftover actor97")
	var layer := State.new(); layer.restore(stage17.projection()); layer.substep = 3
	layer.opening.bg_layer_0.attribute = 0x41000000
	_rejects(media,layer,"bg_reset","stage17 effect requires the cleared stage16 scene","Stage17 accepted a live layer 0")
	# Stage18 entry frame: only actor_draw at 18/0 over the cleared scene.
	var entry18 := State.new(); entry18.restore(stage17.projection()); entry18.next_stage()
	check(_fx(media,entry18,"actor_draw")[0].is_empty(),"Stage18 entry actor_draw refused")
	for bad: Array in [["bg_reset",0,false],["actor_draw",1,false],["actor_draw",0,true]]:
		var b := State.new(); b.restore(entry18.projection()); b.substep = bad[1]
		if bad[2]: b.opening["actor96"] = {"profile":Stages.O07_PROFILE,"index":96}
		_rejects(media,b,bad[0],"stage15-17 effect is outside stages 15..17","Stage18 accepted %s" % str(bad))

func _initialize() -> void:
	_doubles()
	_stage15_flow()
	_stage16_flow()
	_stage17_flow()
	_rng_checks()
	_board_checks()
	if failures == 0: print("PASS: %d stage15-17 checks" % checks)
	quit(1 if failures else 0)
