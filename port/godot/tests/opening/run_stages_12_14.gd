extends SceneTree
## T-39002: opening stages 12-14 (prologue_shiori_load_run, _line_18_run,
## _line_19_run; prologue.c:1831-2129, wiki/spec/opening-stages-12-14.md).
## Part 1 is source control flow with explicit test doubles. Part 2 uses the
## original bytes (the combined stages 12-21 BOARD_PACK) through the real queue,
## media bridge and ShioriActors. Stage11's case6 effects are not reachable
## in the port yet, so the stage12 entry state is built from that case's
## source writes (prologue.c:1803-1818) over the real O_13 bytes, then the
## source's own commit-frame actor_draw_all runs once.
const Flow = preload("res://flow/prologue_arrival.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const Actors = preload("res://services/shiori_actor_model.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
var failures := 0
var checks := 0
# Every part appends its name on its last line. A GDScript runtime error
# aborts only the failing function, so a missing name means skipped checks.
var completed: Array = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

# ---------------------------------------------------------------- part 1
var state := State.new()
var rng := Rng.new()
var events: Array = []
var cd := 1
var cd_audio := 0
var gate: Variant = false
var turn: Variant = false
var decoded: Variant = 1
var complete_line := false
var effect_errors: Dictionary = {}

func effect(s, kind, value, out) -> Variant:
	events.append([kind,s.mode_stage,s.substep,value])
	if effect_errors.has(kind): out.error = effect_errors[kind]
	if kind == "load_gate": return gate
	if kind == "turn_done": return turn
	return null

func doubles() -> Dictionary:
	return {"shiori_effect":effect,
		"line_start":func(s,line,_out): events.append(["line",line,s.substep]),
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["background",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"cd_sync":func(_s,_out): return cd,
		"cd_audio_state":func(_s): return cd_audio,
		"actor_read":func(s,count,destination,lba,_out): events.append(["read",count,destination,lba,s.substep]),
		"background_read":func(s,lba,slot,_out,retry := false): events.append(["bg_read",lba,slot,retry,s.substep]),
		"background_decode":func(_s,slot,_out): events.append(["decode",slot]); return decoded,
		"bank_load_start":func(_s,_role,_out): pass,"scene_reset":func(_s,_out): pass,
		"bank_poll":func(_s,_out): return 0,"bank_start":func(_s,_b,_q,_out): pass,
		"actor_palette_setup":func(_s,_out): pass}

func names() -> Array: return events.map(func(e): return e[0])

func tick(services: Dictionary) -> Session.FrameResult:
	events.clear()
	var out := Session.FrameResult.new()
	match state.mode_stage:
		12: Flow._stage_12_shiori_load(state,services,out)
		13: Flow._stage_13_shiori_line_18(state,rng,services,out)
		14: Flow._stage_14_shiori_line_19(state,rng,services,out)
	return out

static var _seeds: Dictionary = {}

static func seed_hitting(count: int) -> int:
	# A seed whose count-th draw is the first (rand() & 0xFF) == 0xFF.
	if _seeds.has(count): return _seeds[count]
	var probe := Rng.new()
	for seed_word in range(1,1000000):
		probe.configure(Rng.OPENBIOS_PROFILE,seed_word)
		var hits := []
		for i in count: hits.append((probe.next("probe") & 0xff) == 0xff)
		if hits[-1] and not hits.slice(0,count-1).has(true):
			_seeds[count] = seed_word
			return seed_word
	return -1

func _flow_checks() -> void:
	var services := doubles()
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(50))
	state.game_mode = 0x15; state.mode_stage = 12; state.mode_step = 0; state.substep = 0
	var draws := rng.draw_count
	tick(services)
	check(state.substep == 1 and events == [["read",0x31,0x801a4000,0xab49,0],["background",1],["outer",2],["actor_draw",12,1,0]],"Stage12 case0 CUT_14 read/draw order differs: " + str(events))
	cd = 0; tick(services); check(state.substep == 1 and not names().has("read"),"O_14 read issued before CUT_14 completed")
	cd = 1; tick(services); check(state.substep == 2 and events[0] == ["read",9,0x801f0000,0xac91,1],"Stage12 case1 O_14 read differs: " + str(events))
	tick(services); check(state.substep == 3 and events[0] == ["bg_read",0xad0b,1,false,2],"Stage12 case2 AD0B read differs: " + str(events))
	cd = 0; tick(services); check(state.substep == 3 and not names().has("load_gate"),"Case3 tested actor96 before the AD0B read completed")
	cd = 1; gate = false; tick(services)
	check(state.mode_stage == 12 and names() == ["load_gate","background","outer","actor_draw"],"Case3 decoded while actor96 still animated: " + str(events))
	gate = 1; check(tick(services).error == "stage12 requires actual actor96 sequence state" and not names().has("decode"),"Non-bool actor96 gate accepted")
	gate = true; decoded = -1
	# prologue.c:1856-1859: dec_bg_reset() then dec_bg_cd_read(0xAD0B,1); stay.
	var failed := tick(services)
	check(failed.error.is_empty() and state.mode_stage == 12 and state.substep == 3 and events.slice(0,4) == [["load_gate",12,3,0],["decode",1],["dec_bg_reset",12,3,0],["bg_read",0xad0b,1,true,3]] and not names().has("load_setup"),"Failed AD0B decode must reset the cache and reread: " + str(events))
	effect_errors["dec_bg_reset"] = "reset refused"
	check(tick(services).error == "reset refused" and not names().has("bg_read"),"AD0B reread issued after a refused dec_bg_reset")
	effect_errors.clear()
	decoded = 1; effect_errors["load_setup"] = "rejected setup"
	check(tick(services).error == "rejected setup" and state.mode_stage == 12 and state.substep == 3,"Rejected stage12 setup advanced")
	effect_errors.clear(); tick(services)
	check(state.mode_stage == 13 and state.substep == 0 and names() == ["load_gate","decode","load_setup","background","outer","actor_draw"] and events[5] == ["actor_draw",13,0,0],"Stage12 commit order differs: " + str(events))
	check(rng.draw_count == draws,"Stage12 drew RNG")
	# Stage13: one rand() per frame, after the switch, blink before dec_bg_draw.
	draws = rng.draw_count
	tick(services); check(state.substep == 1 and events[0] == ["line",18,0] and state.opening.prologue_line == 18,"Line18 start differs: " + str(events))
	cd_audio = 0; tick(services); check(state.substep == 1 and events[0][0] == "wait_voice" and not names().has("run"),"Case1 ran text before the voice started: " + str(events))
	cd_audio = 4; tick(services); check(state.substep == 2,"Voice start did not advance case1")
	tick(services); check(events.slice(0,2) == [["run",2],["lip_sync",13,2,0]],"Case2 line_run/lip sync order differs: " + str(events))
	complete_line = true; tick(services); complete_line = false
	check(state.substep == 3 and events.slice(0,2) == [["run",2],["lip_sync",13,3,0]],"Case2 completion skipped the same-frame lip sync: " + str(events))
	tick(services); check(state.substep == 4 and events[0] == ["read",0x21,0x801e8000,0xac9a,3],"Case3 O_15 read differs: " + str(events))
	cd = 0; tick(services); check(state.substep == 4 and not names().has("line18_setup"),"O_15 setup before its read completed")
	cd = 1; tick(services)
	check(state.mode_stage == 14 and state.substep == 0 and names() == ["line18_setup","background","outer","actor_draw"],"Stage13 commit differs: " + str(events))
	check(rng.draw_count == draws+8,"Stage13 must draw rand() once per frame (8 frames): %d" % (rng.draw_count-draws))
	# Stage14.
	draws = rng.draw_count
	tick(services); check(state.substep == 1 and events[0] == ["line",19,0],"Line19 start differs")
	tick(services); check(events.slice(0,2) == [["run",1],["lip_sync_line19",14,1,0]],"Case1 lip sync differs (no voice wait in stage14): " + str(events))
	complete_line = true; tick(services); complete_line = false
	check(state.substep == 2 and events[1] == ["lip_sync_line19",14,2,0],"Case1 completion skipped the same-frame lip sync")
	tick(services); check(state.substep == 3 and events[0] == ["actors_reset",14,2,0],"Case2 actor reset differs")
	turn = false; tick(services); check(state.substep == 3 and not names().has("bg_read"),"P_01_06 read before actor99's turn ended")
	turn = "no"; check(tick(services).error == "Shiori line19 requires actual actor99 sequence completion","Non-bool actor99 state accepted")
	turn = true; tick(services); check(state.substep == 4 and events[1] == ["bg_read",0xacec,1,false,4],"Case3 must raise substep before the ACEC read: " + str(events))
	cd = 0; tick(services); check(state.substep == 4 and not names().has("read"),"CUT_16 read before ACEC completed")
	cd = 1; tick(services); check(state.substep == 5 and events[0] == ["read",0x31,0x801a4000,0xab7a,4],"Case4 CUT_16 read differs")
	tick(services); check(state.substep == 6 and events[0] == ["read",0x19,0x801e8000,0xacbb,5],"Case5 O_16 read differs")
	decoded = -1
	var failed14 := tick(services)
	check(failed14.error.is_empty() and state.mode_stage == 14 and state.substep == 6 and events.slice(0,3) == [["decode",1],["dec_bg_reset",14,6,0],["bg_read",0xacec,1,true,6]] and not names().has("line19_setup"),"Failed ACEC decode must reset the cache and reread: " + str(events))
	decoded = 1; tick(services)
	check(state.mode_stage == 15 and state.substep == 0 and names() == ["decode","line19_setup","background","outer","actor_draw"],"Stage14 commit differs: " + str(events))
	check(rng.draw_count == draws+11,"Stage14 must draw rand() once per frame (11 frames with the reset retry): %d" % (rng.draw_count-draws))
	# Dispatch bounds and owner.
	for bad: Array in [[12,0,4],[12,1,0],[13,0,5],[13,0,-1],[14,0,7],[14,2,0]]:
		state.mode_stage = bad[0]; state.mode_step = bad[1]; state.substep = bad[2]
		draws = rng.draw_count
		check(tick(services).error == "prologue stage%d is outside source dispatch" % bad[0] and events.is_empty() and rng.draw_count == draws,"Stage accepted step/substep %s" % str(bad))
	state.mode_step = 0; state.substep = 0; services.erase("shiori_effect")
	for stage in [12,13,14]:
		state.mode_stage = stage
		check(tick(services).error == "source Shiori actor services are unavailable" and events.is_empty(),"Stage%d ran without its effect owner" % stage)
	completed.append("flow")

func _rng_checks() -> void:
	# RNG order through Flow.step: a blink on the 0xFF draw runs before
	# dec_bg_draw; a rejected frame restores both state and RNG.
	var services := doubles()
	state = State.new(); state.game_mode = 0x15; state.mode_stage = 13; state.mode_step = 0; state.substep = 3
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
	events.clear()
	var out := Flow.step(state,rng,0,services)
	check(out.error.is_empty() and names() == ["read","blink_line18","background","outer","actor_draw"],"Stage13 0xFF draw blink order differs: " + str(events))
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(2))
	events.clear(); Flow.step(state,rng,0,services)
	check(not names().has("blink_line18"),"Stage13 blinked on a non-0xFF draw")
	state.mode_stage = 14; state.substep = 6
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
	var before_rng := rng.checkpoint()
	var before: Dictionary = state.projection()
	effect_errors["blink_line19"] = "commit blink rejected"
	events.clear(); out = Flow.step(state,rng,0,services)
	check(out.error == "commit blink rejected" and state.projection() == before and rng.checkpoint() == before_rng and names() == ["decode","line19_setup","blink_line19"],"Rejected post-switch blink did not roll back stage and RNG: " + str(events))
	effect_errors.clear()
	completed.append("rng")

# ---------------------------------------------------------------- part 2
var board := Board.new()
var media := Media.new()
var queue := Queue.new()
var bytes: Dictionary = {}
var text_frame := 0

func _complete_reads(s: State) -> void:
	for request: Dictionary in s.opening.media.requests.values():
		if request.status != "pending": continue
		var out := Session.FrameResult.new()
		var event := Session.ServiceEvent.new(s.frame_count,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,Queue.SERVICE,
			{"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,"status":"ready","resource_id":request.resource_id})
		queue.complete(event,s,null,out)
		check(out.error.is_empty(),"Completion of %s refused: %s" % [request.resource_id,out.error])

func real_services() -> Dictionary:
	var services := queue.callbacks()
	services["background_decode"] = media.background_decode
	services["shiori_effect"] = media.shiori_effect
	services["line_start"] = func(s,line,_out):
		s.opening["prologue_text_work"] = {"line_id":line,"next_frame":Math_u32(s.frame_count+1)}
		s.opening["audio"] = {"status":"requested","clip_index":0,"line_id":line}
		s.opening["prologue_lip_sync"] = {"clip_levels":[[40,40,40,40,40,40,40,40,40,40,40,40]],"clip_index":0,"clip_frame":0}
	services["line_run"] = func(s,_out):
		text_frame += 1
		if text_frame >= 6: s.substep += 1; text_frame = 0
	services["arrival_draw"] = func(_s,_slot,_out): pass
	services["outer_draw"] = func(_s,_arg,_out): pass
	services["cd_audio_state"] = AudioRules.check_cd
	return services

static func Math_u32(v: int) -> int: return v & 0xffffffff

func real_tick(s: State, services: Dictionary) -> Session.FrameResult:
	var out := Session.FrameResult.new()
	match s.mode_stage:
		12: Flow._stage_12_shiori_load(s,services,out)
		13: Flow._stage_13_shiori_line_18(s,rng,services,out)
		14: Flow._stage_14_shiori_line_19(s,rng,services,out)
	# game_mode_dispatch increments frame_count after the mode handler.
	s.frame_count += 1
	if out.error.is_empty(): _complete_reads(s)
	return out

func _stage12_entry() -> State:
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 12; s.mode_step = 0; s.substep = 0; s.frame_count = 1000
	s.opening["media"] = {"epoch":5,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	s.opening["names_media"] = {"bg_show_1":0,"buffer_0":{"lba":0xad05},"buffer_1":{"lba":0xacf6},"active_draw":"dec_bg_draw"}
	s.opening["cut_media"] = {"resource_id":Board.CUT_12_ID}
	s.opening["bg_layer_0"] = {"enabled":0,"attribute":0x80000000,"resource_id":Board.CUT_12_ID,"clut_row":495}
	s.opening["actor_area"] = [{"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":0x20800}]
	# prologue_line_17_run case6: actor_clear_upper, then these writes.
	var a96 := Actors.cleared(96)
	a96.merge({"profile":"o13_shiori_line17","unk_01":0xa,"flags":5,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180204,
		"sequence_table":0x80180220,"tpage_table":0x801801e4,"sequence_slot":0,"brightness":0x80,"x":-0xa0,"y":-0x78},true)
	s.opening["actor96"] = a96; s.opening["actor96_draw_flags"] = 0xa4
	var drawn := Actors.draw_all(bytes,s.opening,s.frame_count)
	check(drawn.error.is_empty() and drawn.drawn.size() == 1,"Stage11 commit-frame actor96 draw over O_13 failed: " + str(drawn))
	return s

func _rejects(s: State, kind: String, error: String, label: String) -> void:
	var before: Dictionary = s.opening.duplicate(true)
	var out := Session.FrameResult.new()
	media.shiori_effect(s,kind,0,out)
	check(out.error == error and s.opening == before,label + ": " + out.error)

func _board_checks() -> void:
	check(not board.stages_12_14_ready() and Media.shiori_area_bytes(board).is_empty(),"Stages 12-14 resources available before the board pack opened")
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),"0".repeat(64))
	check(error == "board role requires a canonical external directory and reviewed manifest pin","Board accepted another manifest pin: " + error)
	error = board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error); failures += 1; return
	var actor_descs := board.actor_descriptions()
	var read_keys := ["65:%d:%d" % [0x801d8000,0xac50],"49:%d:%d" % [0x801a4000,0xab49],"9:%d:%d" % [0x801f0000,0xac91],"33:%d:%d" % [0x801e8000,0xac9a],"49:%d:%d" % [0x801a4000,0xab7a],"25:%d:%d" % [0x801e8000,0xacbb],"65:%d:%d" % [0x80180000,0xac50]]
	check(board.stages_12_14_ready() and actor_descs.size() == 22 and actor_descs.has_all(read_keys) and board.background_descriptions().size() == 12 and board.background_descriptions().has(str(0xad05)) and board.background_descriptions().has(str(0xad0b)) and board.background_descriptions().has(str(0x496a)) and board.background_descriptions().has(str(0x4973)),"Combined board descriptions differ")
	# O_13 is read to two destinations: its transfer record names neither.
	check(board.stage_transfer(Board.O_13_ID).destination == -1 and board.stage_transfer(Board.O_14_ID).destination == 0x801f0000,"O_13 stage transfer names a destination")
	bytes = Media.shiori_area_bytes(board)
	check(bytes.size() == 6,"Stages 12-14 area bytes missing")
	media._board_store = board
	media._shiori_bytes = bytes
	var backgrounds := board.background_descriptions()
	var actors := actor_descs
	check(queue.prepare(backgrounds,actors,{}).is_empty(),"Queue refused the merged descriptions")
	# Disc facts the RNG edges depend on.
	var o15: PackedByteArray = bytes[Board.O_15_ID].bytes
	var o16: PackedByteArray = bytes[Board.O_16_ID].bytes
	check(o15.decode_u32(0xf0) == 0x00580088 and o16.decode_u32(0x284) == 0x8018017c and o16.decode_u32(0x294) == 0x801801dc and o16.decode_u32(0x2c8) == 0x1d1d1d1d,"O_15/O_16 words under the stale tables differ")
	completed.append("board")

func _real_route() -> void:
	var services := real_services()
	rng.configure(Rng.OPENBIOS_PROFILE,0x2468ace)
	var s := _stage12_entry()
	# O_13 actor96 sequence slot0: total ticks until flags5 holds the last row.
	var area: Array = s.opening.actor_area
	var seq_address := Actors.word(bytes,area,0x80180220)
	var rows: int = Actors.half(bytes,area,seq_address)
	var ticks := 0
	for i in rows: ticks += int(Actors.half(bytes,area,seq_address+4+i*4))
	print("O_13 actor96 sequence: %d rows, %d ticks" % [rows,ticks])
	var draws_before := rng.draw_count
	var frame := 0
	var commit_frame := -1
	var gate_closed_seen := false
	while s.mode_stage == 12 and frame < 400:
		var a96_flags: int = s.opening.actor96.flags
		var out := real_tick(s,services)
		if not out.error.is_empty(): check(false,"Stage12 frame %d failed: %s" % [frame,out.error]); return
		if s.mode_stage == 12 and s.substep == 3 and (a96_flags & 1) != 0: gate_closed_seen = true
		if s.mode_stage == 13: commit_frame = frame
		frame += 1
	check(rng.draw_count == draws_before,"Stage12 drew RNG on the real route")
	# Stage11's commit-frame draw is tick1; frames 0..2 add three more.
	check(commit_frame == maxi(3,ticks-1),"Stage12 commit frame %d differs from the O_13 sequence end (%d ticks)" % [commit_frame,ticks])
	check(ticks <= 4 or gate_closed_seen,"Case3 never waited on actor96")
	check(s.opening.actor_area == [{"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":0x20800},{"resource_id":Board.O_14_ID,"sha256":Board.O_14_SHA,"bytes":0x4800}],"Stage12 memcpy layers differ: " + str(s.opening.actor_area))
	check(s.opening.names_media.bg_show_1 == 1 and s.opening.names_media.buffer_1.lba == 0xad0b and s.opening.names_media.buffer_0.lba == 0xad05,"AD0B was not decoded over the ACF6 buffer")
	check(s.opening.bg_layer_0.resource_id == Board.CUT_14_ID and s.opening.bg_layer_0.attribute == 0x41000000 and s.opening.bg_layer_0.clut_row == 495,"CUT_14 slot set differs")
	var a97: Dictionary = s.opening.get("actor97",{})
	check(a97.get("profile") == "o14_shiori_mouth" and a97.frame_index == 4 and a97.flags == 0x40 and a97.unk_01 == 9 and a97.x == -0xa0 and not s.opening.has("actor96"),"Stage12 actor97 setup differs: " + str(a97))
	check(s.opening.shiori_tpages.slots == {"0x80180130":5} and s.opening.shiori_tpages.drawn == [[97,4]],"Stage12 commit-frame tpage differs: " + str(s.opening.shiori_tpages))
	_rejects(s,"load_setup","Shiori stage effect is outside its source substep","Stage12 setup replayed in stage13")
	# Unfinished/wrong reads and ownership at 12/3.
	for broken: String in ["pending","lba","epoch"]:
		var bad := _stage12_entry()
		bad.substep = 3
		bad.opening.names_media = {"bg_show_1":1,"buffer_0":{"lba":0xad05},"buffer_1":{"lba":0xad0b}}
		bad.opening.media.actor_pending = {str(0x801a4000):"c",str(0x801f0000):"o"}
		bad.opening.media.requests = {"c":{"epoch":5,"status":"ready","kind":"actors","resource_id":Board.CUT_14_ID,"resource_sha256":Board.CUT_14_SHA,"payload":{"lba":0xab49,"sectors":49,"destination":0x801a4000}},
			"o":{"epoch":5,"status":"ready","kind":"actors","resource_id":Board.O_14_ID,"resource_sha256":Board.O_14_SHA,"payload":{"lba":0xac91,"sectors":9,"destination":0x801f0000}}}
		match broken:
			"pending": bad.opening.media.requests.o.status = "pending"
			"lba": bad.opening.media.requests.c.payload.lba = 0xab7a
			"epoch": bad.opening.media.requests.o.epoch = 4
		_rejects(bad,"load_setup","stage12 setup lacks completed original CUT_14/O_14 reads","Stage12 setup accepted a %s read" % broken)
	var owner := _stage12_entry(); owner.substep = 3; owner.opening.actor96.frame_table = 0x80180294
	_rejects(owner,"load_gate","stage12 gate requires stage11's O_13 actor96","Gate accepted another actor96 owner")
	owner.substep = 2
	_rejects(owner,"load_gate","Shiori stage effect is outside its source substep","Gate accepted case2")
	var r := Session.FrameResult.new()
	media.arrival_draw(s,1,r)
	check(r.error == "stages 12-14 have no prepared Shiori scene renderer" and r.presentation.is_empty(),"Production dec_bg_draw accepted a Shiori stage without its renderer")
	r = Session.FrameResult.new(); s.opening["prologue_text_cleared"] = true
	media.before_line0(s,18,r)
	check(r.error.is_empty() and not s.opening.has("prologue_text_cleared"),"Line18 start refused at 13/0: " + r.error)

	_line_gate_checks(s,18)

	# Stage13 on the real owner; one blink run, then the post-switch RNG edge.
	draws_before = rng.draw_count
	var frames13 := 0
	var blink_frames: Array = []
	var blink_flags_after: Array = []
	var mouths: Array = []
	var busy98: Dictionary = {}
	while s.mode_stage == 13 and frames13 < 300:
		# Frames where actor98 is still mid-blink; prologue.c:1981 must skip
		# the restart on a 0xFF draw (checked after the loop).
		if s.substep in [1,2] and s.opening.get("actor98") is Dictionary and int(s.opening.actor98.flags) in [0x11,0x21] and not busy98.has(int(s.opening.actor98.flags)):
			busy98[int(s.opening.actor98.flags)] = s.projection()
		if s.substep == 1 and frames13 == 12: s.opening.audio.status = "playing"
		if frames13 == 0:
			# Force one 0xFF draw on the line18 start frame; the voice wait
			# then leaves room for the whole 14-draw ping-pong.
			rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
		if s.substep == 4:
			# Commit frame draw hits 0xFF: actor98 armed with O_14 offsets over O_15.
			rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
		var out := real_tick(s,services)
		if not out.error.is_empty(): check(false,"Stage13 frame %d failed: %s" % [frames13,out.error]); return
		if s.mode_stage == 13 and s.opening.get("actor98") is Dictionary:
			var drawn98: Array = s.opening.shiori_tpages.drawn.filter(func(d): return d[0] == 98)
			if not drawn98.is_empty(): blink_frames.append(drawn98[0][1]); blink_flags_after.append(s.opening.actor98.flags)
		if s.mode_stage == 13 and s.substep in [2,3] and s.opening.get("actor97") is Dictionary: mouths.append(s.opening.actor97.frame_index)
		frames13 += 1
	check(s.mode_stage == 14 and rng.draw_count >= 0,"Stage13 did not commit")
	check(busy98.has(0x11) and busy98.has(0x21),"Stage13 route never had actor98 mid-blink with flags 0x11 and 0x21: " + str(busy98.keys()))
	for flags: int in busy98: _busy_blink_check(busy98[flags],services)
	check(blink_frames.slice(0,15) == [0,0,1,1,2,2,3,3,3,3,2,2,1,1,0] and blink_flags_after[13] == 0 and blink_flags_after[12] == 0x21,"O_14 ping-pong blink differs (flags must reach 0, not 0x10): %s %s" % [str(blink_frames),str(blink_flags_after)])
	check(not mouths.is_empty() and mouths.all(func(m): return m >= 4 and m <= 7) and mouths.has(7),"Line18 lip sync frames differ: " + str(mouths))
	var a98: Dictionary = s.opening.get("actor98",{})
	check(a98.get("profile") == "o14_tables_over_o15" and a98.flags == 0x11 and not s.opening.shiori_tpages.drawn.any(func(d): return d[0] == 98),"Commit-frame blink must arm actor98 over O_15 bytes and be skipped: " + str(a98))
	check(s.opening.shiori_tpages.slots == {"0x801882e0":5,"0x801842e0":6} and s.opening.shiori_tpages.drawn == [[96,0]],"Stage13 commit tpage slots differ: " + str(s.opening.shiori_tpages))
	check(s.opening.names_media.bg_show_1 == 0 and s.opening.bg_layer_0.attribute == 0x80000000 and s.opening.bg_layer_0.enabled == 0,"Stage13 show switch/CUT hide differs")
	check(s.opening.actor_area.map(func(l): return l.resource_id) == [Board.O_13_ID,Board.O_15_ID],"Stage13 memcpy layers differ: " + str(s.opening.actor_area))
	check(s.opening.actor96.profile == "o15_shiori_line19" and s.opening.actor96.frame_index == 0 and s.opening.actor96.unk_01 == 9 and not s.opening.has("actor97"),"Stage13 actor96 setup differs")
	var replay := State.new(); replay.restore(s.projection()); replay.mode_stage = 13; replay.substep = 4
	_rejects(replay,"line18_setup","stage13 setup requires the stage12 AD0B/CUT_14 scene","Stage13 setup replayed over its own result")
	_line_gate_checks(s,19)
	r = Session.FrameResult.new(); s.opening["prologue_text_cleared"] = true
	media.before_line0(s,19,r)
	check(r.error.is_empty(),"Line19 start refused at 14/0: " + r.error)

	# Stage14: the armed actor98 keeps the blink closed until case2's reset.
	var turn_frames: Array = []
	var waits := 0
	var frames14 := 0
	var queued_acec := false
	var at14: Dictionary = {}
	while s.mode_stage == 14 and frames14 < 300:
		if s.substep in [1,6] and not at14.has(s.substep): at14[s.substep] = s.projection()
		if s.substep <= 1:
			rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
		elif s.substep == 3 and turn_frames.size() == 5:
			rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(1))
		var substep_before := s.substep
		var out := real_tick(s,services)
		if not out.error.is_empty(): check(false,"Stage14 frame %d failed: %s" % [frames14,out.error]); return
		if substep_before <= 1: check(s.opening.get("actor98",{}).get("profile") == "o14_tables_over_o15" and s.opening.actor98.flags == 0x11,"Closed blink gate re-armed actor98 at substep %d" % substep_before)
		if substep_before == 1: check(not s.opening.has("actor97"),"Silent line19 frame left actor97 drawn")
		if substep_before in [2,3]:
			var drawn99: Array = s.opening.shiori_tpages.drawn.filter(func(d): return d[0] == 99)
			if not drawn99.is_empty(): turn_frames.append(drawn99[0][1])
			if substep_before == 3 and s.substep == 3: waits += 1
			if substep_before == 3 and s.substep == 3: check(not s.opening.has("actor98"),"Blink armed while actor99 turned")
			if drawn99.size() == 1 and drawn99[0][1] == 10:
				check(s.opening.shiori_tpages.slots == {"0x801882e0":5,"0x801802e0":6,"0x801842e0":7},"Frame10 tpage slots differ: " + str(s.opening.shiori_tpages.slots))
			if drawn99.size() == 1 and drawn99[0][1] == 12:
				check(s.opening.shiori_tpages.slots == {"0x801802e0":5,"0x8018c2e0":6},"Frame12 tpage slots differ: " + str(s.opening.shiori_tpages.slots))
		if substep_before == 3 and s.substep == 4:
			queued_acec = s.opening.media.requests.values().any(func(q): return q.kind == "background" and q.payload.lba == 0xacec and q.status != "pending")
			# Replay of the same P_01_06 read outside case3 is a cache reuse.
			var again := Session.FrameResult.new()
			var copy := State.new(); copy.restore(s.projection()); copy.substep = 5
			queue.background_read(copy,0xacec,1,again)
			check(again.error == "opening background cache reuse is not implemented for this resource","ACEC re-read accepted outside case3: " + again.error)
			copy.substep = 4; copy.opening.names_media.buffer_1.lba = 0xacec
			again = Session.FrameResult.new(); queue.background_read(copy,0xacec,1,again)
			check(again.error == "opening background cache reuse is not implemented for this resource","ACEC re-read accepted on a cache hit")
		frames14 += 1
	var expected_turn: Array = [9,10,10,11]
	for i in 20: expected_turn.append(12)
	expected_turn.append_array([10,9,9])
	check(turn_frames.slice(0,27) == expected_turn,"Actor99 26-tick turn differs: " + str(turn_frames))
	check(waits == 25,"Case3 must wait 25 frames for the 26-tick turn: %d" % waits)
	check(queued_acec,"Case3 did not re-read P_01_06")
	check(at14.has(1) and at14.has(6),"Stage14 route skipped substep 1 or 6")
	if at14.has(1): _line19_mouth_clear_check(at14[1],services)
	if at14.has(6): _decode_shown_buffer_check(at14[6])
	check(s.mode_stage == 15 and s.opening.actor96.profile == "o16_shiori_line20" and s.opening.actor96.x == -0xf0 and s.opening.actor96.unk_01 == 0xb,"Stage14 actor96 setup differs")
	check(s.opening.names_media.bg_show_1 == 1 and s.opening.names_media.buffer_1.lba == 0xacec and s.opening.bg_layer_0.resource_id == Board.CUT_16_ID and s.opening.bg_layer_0.attribute == 0x41000000,"Stage14 background/CUT_16 differs")
	check(s.opening.actor_area.map(func(l): return [l.resource_id,l.bytes]) == [[Board.O_13_ID,0x20800],[Board.O_15_ID,0x10800],[Board.O_16_ID,0xc800]],"Stage14 memcpy layers differ")
	check(s.opening.shiori_tpages.slots == {"0x801882c0":5,"0x801842c0":6},"Stage14 commit tpage slots differ: " + str(s.opening.shiori_tpages))

	# Stage14 one-shot blink after the turn: rests on O_15 frame1.
	var blink := _stage14_after_turn(s)
	check(blink == [1,2,2,3,4,4,3,3,1,1],"O_15 one-shot blink differs: " + str(blink))

	# Commit-frame blink of stage14: explicit rejection with rollback.
	var edge := State.new(); edge.restore(s.projection())
	edge.mode_stage = 15; edge.substep = 0
	# Mode time of the commit frame: frame_count before the post-handler increment.
	edge.frame_count -= 1
	_rejects(edge,"blink_line19",Media.SHIORI_COMMIT_BLINK_UNSUPPORTED,"Stage14 commit-frame blink was not refused")
	edge.opening.actor96.committed_frame = edge.frame_count
	_rejects(edge,"blink_line19","Shiori stage effect is outside its source substep","Blink accepted stage15 after its commit frame")
	r = Session.FrameResult.new(); var empty := Media.new()
	empty.shiori_effect(edge,"actor_draw",0,r)
	check(r.error == "Shiori stage effect requires the authenticated stages 12-14 group","Effect ran without the stages 12-14 board resources")
	completed.append("real_route")

func _line_gate_checks(at: State, line: int) -> void:
	# before_line0 accepts line 18/19 only at stage line-5, step 0, substep 0
	# (the case-0 prologue_line_start of prologue.c:1931 and :2053).
	var cases: Array = [["substep",1],["mode_step",1],["mode_stage",13 if line == 19 else 14]]
	if line == 18: cases.append(["substep",4])
	for c: Array in cases:
		var bad := State.new(); bad.restore(at.projection())
		bad.set(c[0],c[1])
		bad.opening["prologue_text_cleared"] = true
		var before: Dictionary = bad.opening.duplicate(true)
		var out := Session.FrameResult.new()
		media.before_line0(bad,line,out)
		check(out.error == "first-arrival line setup is outside the source dispatch" and bad.opening == before,"Line%d start accepted %s=%d: %s" % [line,c[0],c[1],out.error])

func _busy_blink_check(snapshot: Dictionary, services: Dictionary) -> void:
	# prologue.c:1981: a 0xFF draw while actor98 still animates (flags & 1)
	# must not call prologue_actor98_animation_start_shiori. The same frame
	# with and without the 0xFF draw must leave actor98 identical.
	var saved_text := text_frame
	var runs: Array = []
	for draw in [1,2]:
		var c := State.new(); c.restore(snapshot)
		rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(draw))
		var out := real_tick(c,services)
		text_frame = saved_text
		runs.append([out.error,c.opening.get("actor98"),c.mode_stage,c.substep])
	var a98: Dictionary = snapshot.opening.actor98
	check(runs[0][0].is_empty() and runs[0] == runs[1],"0xFF draw restarted the busy actor98 (flags 0x%x): %s vs %s" % [a98.flags,str(runs[0]),str(runs[1])])
	var direct := State.new(); direct.restore(snapshot)
	direct.frame_count += 1
	var out := Session.FrameResult.new()
	media.shiori_effect(direct,"blink_line18",0,out)
	check(out.error.is_empty() and direct.opening.actor98 == a98 and [a98.flags,a98.frame_index,a98.sequence_counter] == [direct.opening.actor98.flags,direct.opening.actor98.frame_index,direct.opening.actor98.sequence_counter],"blink_line18 rewrote the busy actor98: " + str(direct.opening.actor98))

func _line19_mouth_clear_check(snapshot: Dictionary, services: Dictionary) -> void:
	# prologue_lip_synchronization_shiori_line_19 (prologue.c:1992-2028):
	# actor_clear(0x61) every call; actor 97 is rewritten only while
	# cd_audio_state() & 4. A silent frame after a voiced one removes it.
	var saved_text := text_frame
	var c := State.new(); c.restore(snapshot)
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(2))
	c.opening["cd_audio_state_override"] = 4
	var out := real_tick(c,services)
	var voiced: Variant = c.opening.get("actor97")
	check(out.error.is_empty() and voiced is Dictionary and voiced.get("profile") == "o15_shiori_mouth" and c.opening.shiori_tpages.drawn.any(func(d): return d[0] == 97),"Voiced line19 frame did not draw the O_15 mouth: %s %s" % [out.error,str(voiced)])
	c.opening["cd_audio_state_override"] = 0
	out = real_tick(c,services)
	text_frame = saved_text
	check(out.error.is_empty() and c.mode_stage == 14 and c.substep == 1 and not c.opening.has("actor97") and not c.opening.shiori_tpages.drawn.any(func(d): return d[0] == 97),"Silent line19 frame kept the voiced mouth: %s %s" % [out.error,str(c.opening.get("actor97"))])

func _decode_shown_buffer_check(snapshot: Dictionary) -> void:
	# Stage14 case6 decodes into buffer 1 only because stage13's
	# dec_bg_show_switch(1) showed AD05 again (prologue.c:1958); with buffer 1
	# shown the decode would land in the visible buffer.
	var good := State.new(); good.restore(snapshot)
	var out := Session.FrameResult.new()
	check(media.background_decode(good,1,out) == 1 and out.error.is_empty(),"Stage14 case6 decode control failed: " + out.error)
	var bad := State.new(); bad.restore(snapshot)
	bad.opening.names_media.bg_show_1 = 1
	var before: Dictionary = bad.opening.duplicate(true)
	out = Session.FrameResult.new()
	var decoded_bad: Variant = media.background_decode(bad,1,out)
	check(decoded_bad == -1 and out.error == "Shiori background decode requires the source slot-1 buffers" and bad.opening == before,"Decode accepted a shown buffer 1: %s %s" % [str(decoded_bad),out.error])

func _stage14_after_turn(done: State) -> Array:
	# Rebuild 14/3 after the turn (actor99 at rest) and arm one blink there.
	var s := State.new(); s.restore(done.projection())
	s.mode_stage = 14; s.substep = 4
	s.opening.actor_area = [{"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":0x20800},{"resource_id":Board.O_15_ID,"sha256":Board.O_15_SHA,"bytes":0x10800}]
	for i in [96,97,98,99]: s.opening.erase("actor%d" % i)
	var a99 := Actors.cleared(99)
	a99.merge({"profile":"o15_shiori_turn","unk_01":8,"flags":0,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180294,"sequence_table":0x801802c8,"tpage_table":0x80180284,"sequence_slot":2,"frame_index":5,"brightness":0x80,"x":-0xa0,"y":-0x78},true)
	s.opening["actor99"] = a99
	var out := Session.FrameResult.new()
	media.shiori_effect(s,"blink_line19",0,out)
	var frames: Array = []
	for i in 10:
		var d := Session.FrameResult.new()
		media.shiori_effect(s,"actor_draw",0,d)
		if not d.error.is_empty(): return [d.error]
		var hit: Array = s.opening.shiori_tpages.drawn.filter(func(x): return x[0] == 98)
		frames.append(hit[0][1] if not hit.is_empty() else -1)
	check(out.error.is_empty() and s.opening.actor98.flags == 0 and s.opening.actor98.frame_index == 0,"One-shot blink did not stop at step0")
	return frames

func _initialize() -> void:
	_flow_checks()
	_rng_checks()
	_board_checks()
	if not bytes.is_empty(): _real_route()
	for part in ["flow","rng","board","real_route"]:
		if not completed.has(part):
			failures += 1
			printerr("Part %s did not run to its end (runtime error or missing pack bytes)" % part)
	if failures == 0: print("PASS: %d stage12-14 source checks" % checks)
	quit(1 if failures else 0)
