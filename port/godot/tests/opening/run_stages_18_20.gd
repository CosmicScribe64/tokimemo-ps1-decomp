extends SceneTree
## Opening stages 18-20 (prologue.c:2565-2706, wiki/spec/opening-stages-18-20.md).
## Part 1 drives the flow with explicit test doubles: source substep order,
## the line38 voice wait, the 257-frame flash, the post-increment hold and
## the 5/0xD1 order. Part 2 drives Flow.step for RNG order, rollback and the
## START test. Part 3 uses the original board pack (BOARD_PACK) for the
## bridge: line admissions, the O_13 actor96 built from real bytes, its
## sequence and tpage slots, flash/white records and the P_01_12 draw owner.
## Earlier stages do not reach stage 18 on the actual route, so part 3 builds
## the stage-17 exit state from its source-defined records, as
## run_rei_line17.gd does for stage 10.
const Flow = preload("res://flow/prologue_arrival.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
var state := State.new()
var services: Dictionary
var events: Array = []
var cd: Variant = 0
var complete_line := false
var reject_kind := ""
var reject_line := false
var audio_error := ""
var failures := 0
const BG_KEY := "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
const BG_DRAW := [{"kind":"first_arrival_background_draw","slot":1}]
var checks := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func effect(s, kind, value, result) -> Variant:
	events.append([kind,value,s.mode_stage,s.substep])
	if kind == reject_kind: result.error = "rejected " + kind
	return null

func _services() -> Dictionary:
	return {"prologue_closing_effect":effect,
		"line_start":func(s,line,out):
			events.append(["line",line,s.opening.get("prologue_scene_counter")])
			if reject_line: out.error = "rejected line",
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(s,slot,_out): events.append(["background",slot,s.mode_stage]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"cd_audio_state":func(_s): return cd,
		"audio_command":func(_s,command,out):
			events.append(["audio",command])
			if not audio_error.is_empty(): out.error = audio_error}

func tick(stage: int = -1) -> Session.FrameResult:
	events.clear()
	var result := Session.FrameResult.new()
	match state.mode_stage if stage < 0 else stage:
		18: Flow._stage_18_lines_38_to_41(state,services,result)
		19: Flow._stage_19_flash(state,services,result)
		20: Flow._stage_20_closing(state,services,result)
	return result

func names() -> Array: return events.map(func(e): return e[0])

func _at(stage: int, substep: int) -> void:
	state.game_mode = 0x15; state.mode_stage = stage; state.mode_step = 0; state.substep = substep

func _stage18_checks() -> void:
	services = _services()
	_at(18,0)
	check(tick().error.is_empty() and state.substep == 1 and state.opening.prologue_line == 38,"Line38 start differs")
	check(events == [["line",38,null],["background",1,18],["outer",2],["actor_draw",0,18,1]],"Case0 order differs: " + str(events))
	# Case1: XA wait only; no prologue_line_run while waiting.
	cd = 0; tick()
	check(state.substep == 1 and names() == ["wait_voice","background","outer","actor_draw"],"Case1 ran the line or advanced without XA: " + str(events))
	cd = 2; tick(); check(state.substep == 1,"Case1 advanced on bit1 only")
	cd = "4"; check(tick().error == "line38 CD state is not an integer" and state.substep == 1,"Non-integer CD state accepted")
	services.erase("cd_audio_state"); cd = 4
	check(tick().error == "line38 requires original CD playback state","Case1 ran without the CD playback owner")
	services = _services()
	reject_kind = "wait_voice"; check(tick().error == "rejected wait_voice" and state.substep == 1 and names() == ["wait_voice"],"Rejected text-tick wait still polled CD"); reject_kind = ""
	tick(); check(state.substep == 2 and events[0][0] == "wait_voice","Case1 did not advance on bit2")
	tick(); check(state.substep == 2 and events[0] == ["run",2],"Case2 must only run line38")
	var order: Array = []
	complete_line = true
	for expected: Array in [[2,3],[3,4],[4,5],[5,6],[6,7],[7,8],[8,9]]:
		check(state.substep == expected[0],"Stage18 substep sequence differs at %d" % expected[0])
		tick(); order.append(events[0].slice(0,2))
		check(state.substep == expected[1],"Stage18 substep %d did not advance" % expected[0])
	complete_line = false
	check(order == [["run",2],["line",39],["run",4],["line",40],["run",6],["line",41],["run",8]],"Lines 39-41 order differs: " + str(order))
	check(state.opening.prologue_line == 41,"Line41 not recorded")
	# Case9 is its own frame: counter = 0, mode_stage_next, then dec_bg_draw.
	state.opening["prologue_scene_counter"] = 77
	tick(18)
	check(state.mode_stage == 19 and state.substep == 0 and state.opening.prologue_scene_counter == 0,"Case9 reset/stage switch differs")
	check(events == [["background",1,19],["outer",2],["actor_draw",0,19,0]],"Case9 frame differs: " + str(events))
	reject_line = true; _at(18,3)
	check(tick().error == "rejected line" and state.substep == 3 and names() == ["line"],"Rejected line39 start advanced")
	reject_line = false
	for bad: Array in [[0,10],[0,-1],[1,0]]:
		_at(18,bad[1]); state.mode_step = bad[0]
		check(tick().error == "prologue stage18 is outside source dispatch" and events.is_empty(),"Stage18 accepted step/substep %s" % str(bad))
	_at(18,0); services.erase("prologue_closing_effect")
	check(tick().error == "source closing-scene services are unavailable" and events.is_empty(),"Stage18 ran without the closing service")
	services = _services()

func _stage19_checks() -> void:
	services = _services()
	_at(19,0); state.opening["prologue_scene_counter"] = 0
	var per_substep := [0,0,0,0,0]
	var flashes: Array = [[],[],[],[]]
	var frames := 0
	var setup_frame := -1
	var hide_frame := -1
	var order_ok := true
	while state.mode_stage == 19 and frames < 400:
		var substep := state.substep
		per_substep[substep] += 1
		check(tick().error.is_empty(),"Stage19 frame rejected")
		var kinds := names()
		if substep in [1,2]: order_ok = order_ok and kinds.slice(0,2) == ["white","flash"]
		elif substep in [0,3]: order_ok = order_ok and kinds[0] == "flash" and not kinds.has("white")
		else: order_ok = order_ok and kinds == ["background","outer","actor_draw"]
		for e: Array in events:
			if e[0] == "flash": flashes[substep].append(e[1])
			if e[0] == "actor_setup": setup_frame = frames
			if e[0] == "actor_hide":
				hide_frame = frames
				order_ok = order_ok and e[3] == 3
		order_ok = order_ok and kinds.slice(kinds.size()-3) == ["background","outer","actor_draw"]
		frames += 1
	check(frames == 257 and per_substep == [64,64,64,64,1],"Stage19 frame counts differ: %d %s" % [frames,str(per_substep)])
	check(order_ok,"Stage19 per-frame draw order differs")
	check(flashes[0] == range(0,256,4) and flashes[2] == range(0,256,4),"Rising flash values differ")
	check(flashes[1] == range(256,0,-4) and flashes[3] == range(256,0,-4),"Falling flash values differ")
	check(setup_frame == 63 and hide_frame == 191,"Actor96 setup/hide frames differ: %d %d" % [setup_frame,hide_frame])
	check(state.mode_stage == 20 and state.substep == 0 and state.opening.prologue_scene_counter == 0,"Stage19 exit differs")
	_at(19,0); state.opening.erase("prologue_scene_counter")
	check(tick().error == "stage19 requires the scene counter reset by stage18" and events.is_empty(),"Stage19 invented a missing counter")
	state.opening["prologue_scene_counter"] = "0"
	check(tick().error == "stage19 requires the scene counter reset by stage18","Stage19 accepted a non-integer counter")
	state.opening["prologue_scene_counter"] = 252; reject_kind = "actor_setup"
	check(tick().error == "rejected actor_setup" and state.substep == 0,"Rejected actor setup advanced substep0")
	reject_kind = ""
	for bad: Array in [[0,5],[0,-1],[2,0]]:
		_at(19,bad[1]); state.mode_step = bad[0]; state.opening["prologue_scene_counter"] = 0
		check(tick().error == "prologue stage19 is outside source dispatch" and events.is_empty(),"Stage19 accepted step/substep %s" % str(bad))

func _stage20_checks() -> void:
	services = _services()
	_at(20,0); state.opening["prologue_scene_counter"] = 999
	tick()
	check(state.substep == 1 and state.opening.prologue_line == 42 and events[0] == ["line",42,0],"Case0 must reset the counter before line42: " + str(events))
	tick(); check(state.substep == 1 and events[0] == ["run",1],"Case1 must run line42")
	complete_line = true; tick(); complete_line = false
	check(state.substep == 2 and state.opening.prologue_scene_counter == 0,"Line42 completion differs")
	var frames := 0
	var audio_frames: Array = []
	while state.mode_stage == 20 and frames < 100:
		tick(); frames += 1
		if names().has("audio"): audio_frames.append(frames)
	check(frames == 62 and audio_frames == [62],"Post-increment hold differs: %d %s" % [frames,str(audio_frames)])
	check(events == [["audio",5],["audio",0xd1],["background",1,21],["outer",2]],"Fade order/switch frame differs: " + str(events))
	check(state.mode_stage == 21 and state.substep == 0 and state.opening.prologue_scene_counter == 0x3e,"Stage20 exit differs")
	_at(20,2); state.opening["prologue_scene_counter"] = 0x3c; services.erase("audio_command")
	check(tick().error.is_empty() and state.mode_stage == 20 and state.opening.prologue_scene_counter == 0x3d,"Non-firing frame needed the audio owner")
	check(tick().error == "stage20 BGM fade requires the source audio command owner" and state.mode_stage == 20,"Fade without audio owner advanced")
	services = _services(); _at(20,2); state.opening["prologue_scene_counter"] = 0x3d; audio_error = "fade refused"
	check(tick().error == "fade refused" and state.mode_stage == 20 and events == [["audio",5]],"Refused fade continued: " + str(events))
	audio_error = ""
	state.opening["prologue_scene_counter"] = null
	check(tick().error == "stage20 requires the scene counter reset by case0","Stage20 accepted a missing counter")
	for bad: Array in [[0,3],[1,1]]:
		_at(20,bad[1]); state.mode_step = bad[0]
		check(tick().error == "prologue stage20 is outside source dispatch" and events.is_empty(),"Stage20 accepted step/substep %s" % str(bad))
	# The production owner: FirstArrivalBoot._services() merges
	# AudioRules.audio_command over the bridge's stage-scoped audio_command.
	services = _services(); services["audio_command"] = AudioRules.audio_command
	_at(20,2); state.opening["prologue_scene_counter"] = 0x3d
	state.opening.erase("prologue_bgm_state"); state.opening.erase("bgm_fade")
	var refused := tick()
	check(refused.error == "BGM fade requires the actual playing sequence gain" and state.mode_stage == 20 and refused.audio == [{"kind":"native_audio_control","command":5}],"Stage20 D1 fade without a playing BGM gain was not refused by AudioRules: " + refused.error)
	_at(20,2); state.opening["prologue_scene_counter"] = 0x3d
	state.opening["prologue_bgm_state"] = {"playing":true,"gain":83}
	var faded := tick()
	check(faded.error.is_empty() and state.mode_stage == 21 and state.opening.prologue_scene_counter == 0x3e and faded.audio == [{"kind":"native_audio_control","command":5},{"kind":"native_bgm_fade","command":0xd1}],"Stage20 AudioRules 5/D1 differs: " + str(faded.audio) + faded.error)
	check(state.opening.bgm_fade.command == 0xd1 and state.opening.bgm_fade.gain == 83 and state.opening.bgm_fade.rate == 0x80 and state.opening.bgm_fade.decrement == 1 and state.opening.bgm_fade.request == true,"Stage20 D1 fade state differs: " + str(state.opening.bgm_fade))
	_at(20,2); state.opening["prologue_scene_counter"] = 0x3d
	check(tick().error == "BGM fade restart while a source fade request is outstanding is unsupported" and state.mode_stage == 20,"Stage20 D1 restarted over an outstanding fade request")
	state.opening.erase("prologue_bgm_state"); state.opening.erase("bgm_fade"); state.opening.erase("audio_state_flags")
	services = _services()

func _step_checks() -> void:
	# Flow.step: no RNG draw in stages 18-20 (spec section 0), rollback of a
	# refused frame, and the dispatcher-tail START test.
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,19950404)
	var full := _services()
	for key: String in Flow.REQUIRED:
		if not full.has(key): full[key] = func(_a = null,_b = null,_c = null,_d = null,_e = null): return 0
	services = full
	state = State.new(); _at(18,0)
	complete_line = true; cd = 4
	var frames := 0
	while state.mode_stage in [18,19,20] and frames < 600:
		events.clear()
		var out := Flow.step(state,rng,0,full)
		check(out.error.is_empty(),"Flow.step rejected %d/%d: %s" % [state.mode_stage,state.substep,out.error])
		if not out.error.is_empty(): break
		state.frame_count += 1; frames += 1
	complete_line = false
	check(state.mode_stage == 21 and rng.draw_count == 0,"Stages 18-20 drew RNG or did not finish: %d" % rng.draw_count)
	state = State.new(); _at(19,0); state.opening["prologue_scene_counter"] = 252
	var before := state.projection()
	reject_kind = "actor_setup"
	var refused := Flow.step(state,rng,0,full)
	reject_kind = ""
	check(refused.error == "rejected actor_setup" and state.projection() == before and rng.draw_count == 0,"Refused stage19 frame was not rolled back")
	var started := Flow.step(state,rng,0x0800,full)
	check(started.error.is_empty() and state.mode_stage == 21,"START did not reach stage21 from stage19")

# --- Part 3: bridge with original board bytes.

func _entry(board: Board) -> State:
	# Stage17 exit: P_01_12 shown, P_01_10 in the other buffer (stages 16-17
	# decode), bg layer 0 cleared (prologue.c:2514), actors 96-159 cleared
	# (:2513), stage0 palette rows uploaded, and the unwaited O_13 read.
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 18; s.mode_step = 0; s.substep = 0; s.frame_count = 1000
	# Stage16 decodes P_01_10 into buffer 0 (shown); stage17 decodes P_01_12
	# into buffer 1 and shows it, with its names-background upload owner.
	s.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0x496a},"active_draw":"dec_bg_draw",
		"buffer_1":{"lba":0x4973,"resource_id":Board.BG_4973_ID,"transfer_sha256":Board.BG_4973_TRANSFER_SHA,"word_sha256":Board.BG_4973_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":BG_KEY}}
	s.opening["bg_layer_0"] = {"attribute":0x80000000,"enabled":0}
	s.opening["arrival_media"] = {"actor_initialized":true,"upload_batch_key":"k".sha256_text()}
	s.opening["media"] = {"epoch":5,"actor_pending":{str(0x80180000):"o13"},"requests":{
		"o13":{"request_id":"o13","epoch":5,"status":"pending","kind":"actors","resource_id":Board.O_13_ID,"resource_sha256":Board.O_13_SHA,"payload":{"lba":0xac50,"sectors":0x41,"destination":0x80180000}}}}
	return s

func _call(media: Media, s: State, kind: String, value: int = 0) -> String:
	var result := Session.FrameResult.new()
	media.closing_effect(s,kind,value,result)
	return result.error

func _line(media: Media, s: State, line: int) -> String:
	var result := Session.FrameResult.new()
	s.opening["prologue_text_cleared"] = true
	media.before_line0(s,line,result)
	return result.error

func _setup_state(board: Board, media: Media) -> State:
	var s := _entry(board)
	s.mode_stage = 19; s.substep = 0; s.opening["prologue_scene_counter"] = 0x100
	s.opening.media.requests.o13.status = "ready"
	check(_call(media,s,"actor_setup").is_empty(),"Exact O_13 actor96 setup rejected")
	s.substep = 1
	return s

func _board_checks() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error); failures += 1; return
	var o13: PackedByteArray = board.stage_transfer(Board.O_13_ID).bytes
	check(o13.size() == 0x20800 and board.stage_transfer(Board.O_13_ID).lba == 0xac50 and board.stage_transfer(Board.O_13_ID).sectors == 0x41,"O_13 transfer extent differs")
	check(Board.o13_sequence(o13) == [[0,6],[1,5],[2,5],[3,4],[4,7],[5,7],[6,0x50]],"O_13 sequence 0 differs: " + str(Board.o13_sequence(o13)))
	check(Board.o13_tpage_slots(o13,0) == {0x230:5,0x8230:6} and Board.o13_tpage_slots(o13,1) == {0x8230:5,0x4230:6},"O_13 frame0/1 slots differ")
	check(Board.o13_tpage_slots(o13,3) == {0x4230:5,0x10230:6} and Board.o13_tpage_slots(o13,6) == {0x8230:5,0x1c230:6},"O_13 frame3/6 slots differ")
	check(Board.o13_tpage_slots(o13.slice(0,0x20000),0).is_empty() and Board.o13_sequence(o13.slice(0,0x20000)).is_empty(),"Truncated O_13 resolved tables")
	var broken := o13.duplicate(); broken.encode_u32(0x204,0x801a0800)
	check(Board.o13_tpage_slots(broken,0).is_empty(),"Out-of-range O_13 frame pointer resolved slots")
	broken = o13.duplicate(); broken.encode_s16(0x1c4+2,7)
	check(Board.o13_sequence(broken).is_empty(),"O_13 sequence with a frame outside the table accepted")
	var media := Media.new()
	media._board_store = board
	media._names_bg_uploads[BG_KEY] = {"batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":1}

	# Line admissions: stage18 cases 0/3/5/7 and stage20 case0 only.
	var s := _entry(board)
	check(_line(media,s,38).is_empty() and not s.opening.has("prologue_text_cleared"),"Line38 start rejected at 18/0")
	for pair: Array in [[39,3],[40,5],[41,7]]:
		s.substep = pair[1]; check(_line(media,s,pair[0]).is_empty(),"Line%d start rejected" % pair[0])
	const OUTSIDE := "first-arrival line setup is outside the source dispatch"
	for case: Array in [["substep",1],["line",39],["line",42],["stage",17],["actor97",null],["flags",0xa4],["swap",null],["layer",0x41000000],["palette",false],["no_read",null],["read_lba",0xac51],["read_sectors",33],["read_dest",0x8018c800],["read_sha",Board.O_11_SHA],["read_epoch",4],["read_retry",null]]:
		var other := _entry(board)
		var line := 38
		match case[0]:
			"substep": other.substep = case[1]
			"line": line = case[1]
			"stage": other.mode_stage = case[1]
			"actor97": other.opening["actor97"] = {"profile":"o12_rei_line17"}
			"flags": other.opening["actor96_draw_flags"] = case[1]
			"swap": other.opening.names_media.bg_show_1 = 0
			"layer": other.opening.bg_layer_0.attribute = case[1]
			"palette": other.opening.arrival_media.actor_initialized = case[1]
			"no_read": other.opening.media.actor_pending.clear()
			"read_lba": other.opening.media.requests.o13.payload.lba = case[1]
			"read_sectors": other.opening.media.requests.o13.payload.sectors = case[1]
			"read_dest": other.opening.media.requests.o13.payload.destination = case[1]
			"read_sha": other.opening.media.requests.o13.resource_sha256 = case[1]
			"read_epoch": other.opening.media.requests.o13.epoch = case[1]
			"read_retry": other.opening.media.requests.o13.status = "retry"
		check(_line(media,other,line) == OUTSIDE and other.opening.has("prologue_text_cleared"),"Line start accepted wrong " + case[0])

	# Case1 text tick: only line38, only on the expected frame.
	s = _entry(board); s.substep = 1
	s.opening["prologue_text_work"] = {"line_id":38,"next_frame":1000}
	check(_call(media,s,"wait_voice").is_empty() and s.opening.prologue_text_work.next_frame == 1001,"Line38 wait did not carry the text tick")
	check(_call(media,s,"wait_voice") == "line38 voice wait lost its expected text tick","Duplicate wait tick accepted")
	s.opening.prologue_text_work = {"line_id":39,"next_frame":1000}
	check(_call(media,s,"wait_voice") == "line38 voice wait lost its expected text tick","Wait accepted another line")
	s.substep = 2; check(_call(media,s,"wait_voice") == "closing effect is outside its source substep","Wait accepted case2")

	# dec_bg_draw(1): owner and the published P_01_12 upload checked.
	s = _entry(board)
	var drawn := Session.FrameResult.new(); media.arrival_draw(s,1,drawn)
	check(drawn.error.is_empty() and drawn.presentation == BG_DRAW,"Stage18 background draw rejected: " + drawn.error)
	s.opening.names_media.buffer_0.lba = 0xad05
	drawn = Session.FrameResult.new(); media.arrival_draw(s,1,drawn)
	check(drawn.error == "stages 18-20 require P_01_12 shown with P_01_10 in the other slot 1 buffer","Stage18 draw accepted another background")
	for bad: Array in [["upload_batch_key","b".sha256_text()],["word_sha256",Board.BG_AD0B_WORD_SHA],["resource_id",Board.BG_AD0B_ID],["decode_rect",[704,0,320,160]]]:
		s = _entry(board); s.opening.names_media.buffer_1[bad[0]] = bad[1]
		drawn = Session.FrameResult.new(); media.arrival_draw(s,1,drawn)
		check(drawn.error == Media.CLOSING_BG_NOT_UPLOADED and drawn.presentation.is_empty(),"Stage18 draw accepted an unowned P_01_12 buffer: " + bad[0])
	# Same-frame upload: the stage17->18 frame queues the upload, then draws.
	media._names_bg_uploads.clear()
	s = _entry(board); drawn = Session.FrameResult.new(); media.arrival_draw(s,1,drawn)
	check(drawn.error == Media.CLOSING_BG_NOT_UPLOADED,"Stage18 draw accepted an unpublished P_01_12 upload")
	drawn = Session.FrameResult.new()
	drawn.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":Board.BG_4973_WORD_SHA})
	media.arrival_draw(s,1,drawn)
	check(drawn.error.is_empty() and drawn.presentation.size() == 2 and drawn.presentation[1] == BG_DRAW[0],"Stage18 draw rejected this frame's P_01_12 upload: " + drawn.error)
	media._names_bg_uploads[BG_KEY] = {"batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":1}
	s = _entry(board); drawn = Session.FrameResult.new(); media.arrival_draw(s,0,drawn)
	check(drawn.error == "stages 18-20 dec_bg_draw requires slot 1 and the board media","Stage18 draw accepted slot 0")

	# Flash and white records (prologue.c:1257-1275).
	s = _entry(board); s.mode_stage = 19; s.substep = 0; s.opening["prologue_scene_counter"] = 0x80
	check(_call(media,s,"flash",0x80).is_empty(),"Flash at 19/0 rejected")
	var flash: Dictionary = s.opening.prologue_closing_draws[0]
	check(flash.color == 0x808080 and flash.tag == 0xb and flash.mode == 0x81 and flash.rects == [[-160,-120,160,160],[0,-120,160,160]] and flash.draw_mode == {"tag":0xb,"abr":1,"tp":1},"Flash record differs: " + str(flash))
	check(_call(media,s,"flash",0x80) == "closing boxes are outside their source draw order","Second flash in one frame accepted")
	s.opening.erase("prologue_closing_draws")
	check(_call(media,s,"flash",0x84) == "flash boxes require the source scene counter","Flash accepted another counter")
	check(_call(media,s,"white") == "closing effect is outside its source substep","White boxes accepted at 19/0")
	s.substep = 1; s.opening["prologue_scene_counter"] = 0x100
	check(_call(media,s,"flash",0x100) == "closing boxes are outside their source draw order","Case1 flash without white boxes accepted")
	check(_call(media,s,"white").is_empty() and _call(media,s,"flash",0x100).is_empty(),"Case1 white+flash rejected")
	check(s.opening.prologue_closing_draws[0] == {"tag":0xd,"color":0xffffff,"mode":1,"rects":[[-160,-120,160,160],[0,-120,160,160]]} and s.opening.prologue_closing_draws[1].color == 0xffffff,"Case1 records differ")
	s.substep = 3; s.opening.erase("prologue_closing_draws")
	check(_call(media,s,"white") == "closing effect is outside its source substep","White boxes accepted at 19/3")

	# Actor96 setup from the completed O_13 read only.
	s = _entry(board); s.mode_stage = 19; s.opening["prologue_scene_counter"] = 0x100
	check(_call(media,s,"actor_setup") == "stage19 actor96 setup lacks the completed O_13 read" and not s.opening.has("actor96"),"Setup accepted a pending O_13 read")
	s.opening.media.requests.o13.status = "ready"
	s.opening.prologue_scene_counter = 0xfc
	check(_call(media,s,"actor_setup") == "stage19 actor96 setup is outside the flash peak","Setup accepted before the peak")
	s.opening.prologue_scene_counter = 0x100; s.opening.arrival_media.actor_initialized = false
	check(_call(media,s,"actor_setup") == "stages 18-20 require the stage0 O_01 palette rows including CLUT row 0x1FB","Setup accepted without CLUT row 0x1FB")
	for field: Array in [["epoch",6],["resource_id",Board.O_11_ID],["kind","background"]]:
		var bad := _entry(board); bad.mode_stage = 19; bad.opening["prologue_scene_counter"] = 0x100
		bad.opening.media.requests.o13.status = "ready"; bad.opening.media.requests.o13[field[0]] = field[1]
		check(_call(media,bad,"actor_setup") == "stage19 actor96 setup lacks the completed O_13 read","Setup accepted O_13 read with wrong " + field[0])
	var a := _setup_state(board,media)
	var actor: Dictionary = a.opening.actor96
	check(actor.profile == "o13_flash" and actor.unk_01 == 0xc and actor.flags == 5 and actor.quad_flags == 0x41000000 and actor.draw_flags == 0xa4 and actor.frame_table == 0x80180204 and actor.sequence_table == 0x80180220 and actor.tpage_table == 0x801801e4,"Actor96 table fields differ")
	check(actor.sequence_slot == 0 and actor.blink_period == 2 and actor.unk_05 == 0x80 and actor.x == -160 and actor.y == -120 and actor.brightness == 0x80 and actor.scale == 0x1000 and actor.clut_row == 0x1fb and actor.read_request == "o13" and a.opening.actor96_draw_flags == 0xa4,"Actor96 defaults/fields differ")
	a.substep = 0
	check(_call(media,a,"actor_setup") == "stage19 actor96 setup cannot replay over a live actor","Actor96 setup replayed")
	a.substep = 2
	check(_call(media,a,"actor_setup") == "closing effect is outside its source substep","Setup accepted 19/2")

	# actor_draw_all: draw the current row, then step at frame_count % 2.
	for parity: int in [0,1]:
		var v := _setup_state(board,media)
		v.frame_count = 2000 + parity - 1  # first draw sees frame_count+1
		var counts: Dictionary = {}
		var first_six := -1
		var slots0: Variant = null
		for i in 128:
			check(_call(media,v,"actor_draw").is_empty(),"Actor draw rejected")
			var frame: int = v.opening.o13_draw.frame
			if i == 0: slots0 = v.opening.o13_draw.slots
			counts[frame] = counts.get(frame,0)+1
			if frame == 6 and first_six < 0: first_six = i
			v.frame_count += 1
		# Even start: row k ends on the tick that reaches its duration, so
		# rows 0-5 draw 11,10,10,8,14,14 frames and row 6 starts at draw 67.
		var want: Dictionary = {0:11,1:10,2:10,3:8,4:14,5:14,6:61} if parity == 0 else {0:12,1:10,2:10,3:8,4:14,5:14,6:60}
		check(counts == want and first_six == (67 if parity == 0 else 68),"O_13 sequence timing differs (parity %d): %s %d" % [parity,str(counts),first_six])
		check(slots0 == {str(0x230):5,str(0x8230):6} and v.opening.actor96.flags == 5,"First drawn frame slots/flags differ")
		if parity == 0:
			for i in 200:
				_call(media,v,"actor_draw"); v.frame_count += 1
			check(v.opening.actor96.flags == 0 and v.opening.actor96.seq_step == 6 and v.opening.o13_draw.frame == 6,"Flags 5 did not hold the last row")
	# Hide at case2 exit; a hidden actor neither draws nor steps.
	var h := _setup_state(board,media)
	_call(media,h,"actor_draw")
	h.substep = 3
	check(_call(media,h,"actor_hide").is_empty() and h.opening.actor96.draw_flags == 0 and h.opening.actor96_draw_flags == 0 and not h.opening.has("o13_draw"),"Actor96 hide differs")
	var seq_before: Array = [h.opening.actor96.seq_step,h.opening.actor96.sequence_counter]
	h.frame_count += 1
	check(_call(media,h,"actor_draw").is_empty() and not h.opening.has("o13_draw") and [h.opening.actor96.seq_step,h.opening.actor96.sequence_counter] == seq_before,"Hidden actor96 drew or stepped")
	check(_call(media,h,"actor_hide") == "stage19 actor96 hide lacks the visible O_13 actor","Actor96 hide replayed")
	h.substep = 2; check(_call(media,h,"actor_hide") == "closing effect is outside its source substep","Hide accepted before case2's substep increment")
	# dec_bg_draw after the hide (19/3, 19/4) and on the visible substeps
	# (19/1, 19/2): with the owner exact the background draw is queued, so
	# a looser or stricter visibility set changes the result.
	h.substep = 3; drawn = Session.FrameResult.new(); media.arrival_draw(h,1,drawn)
	check(drawn.error.is_empty() and drawn.presentation == BG_DRAW,"Stage19/3 draw after hide differs: " + drawn.error)
	for sub: int in [1,2]:
		var vis := _setup_state(board,media); vis.substep = sub
		drawn = Session.FrameResult.new(); media.arrival_draw(vis,1,drawn)
		check(drawn.error.is_empty() and drawn.presentation == BG_DRAW,"Stage19/%d draw with the visible actor differs: %s" % [sub,drawn.error])
		vis.opening.actor96.draw_flags = 0; vis.opening.actor96_draw_flags = 0
		drawn = Session.FrameResult.new(); media.arrival_draw(vis,1,drawn)
		check(drawn.error == "stages 19-20 require the source O_13 actor96 state","Stage19/%d draw accepted a hidden actor" % sub)
	h.substep = 4; drawn = Session.FrameResult.new(); media.arrival_draw(h,1,drawn)
	check(drawn.error.is_empty() and drawn.presentation == BG_DRAW,"Stage19/4 draw after hide differs: " + drawn.error)
	h.opening.actor96.draw_flags = 0xa4; h.opening.actor96_draw_flags = 0xa4
	drawn = Session.FrameResult.new(); media.arrival_draw(h,1,drawn)
	check(drawn.error == "stages 19-20 require the source O_13 actor96 state","Stage19/4 draw accepted a visible actor")
	# Stage20: the actor96_draw_flags mirror must match the actor record.
	var d := _setup_state(board,media); d.mode_stage = 20; d.substep = 1
	d.opening.actor96.draw_flags = 0; d.opening.actor96_draw_flags = 0
	drawn = Session.FrameResult.new(); media.arrival_draw(d,1,drawn)
	check(drawn.error.is_empty() and drawn.presentation == BG_DRAW,"Stage20 draw with the hidden actor differs: " + drawn.error)
	d.opening.actor96_draw_flags = 0xa4
	drawn = Session.FrameResult.new(); media.arrival_draw(d,1,drawn)
	check(drawn.error == "stages 19-20 require the source O_13 actor96 state","Stage20 draw accepted desynced actor96_draw_flags: " + drawn.error)
	d.opening.actor96_draw_flags = 0; d.opening.actor96.draw_flags = 0xa4
	drawn = Session.FrameResult.new(); media.arrival_draw(d,1,drawn)
	check(drawn.error == "stages 19-20 require the source O_13 actor96 state","Stage20 draw accepted a visible actor record: " + drawn.error)
	_exit_frame_checks(board,media)
	# Owners and states outside the source.
	var o := _setup_state(board,media)
	o.opening.actor96.seq_step = 7
	check(_call(media,o,"actor_draw") == "O_13 actor96 sequence state is outside the source states","Actor draw accepted a step past the table")
	o.opening.actor96.seq_step = 0; o.opening.actor96.flags = 3
	check(_call(media,o,"actor_draw") == "O_13 actor96 sequence state is outside the source states","Actor draw accepted flags 3")
	o.opening.actor96.flags = 5; o.opening.actor96.blink_period = 1
	check(_call(media,o,"actor_draw") == "O_13 actor96 sequence state is outside the source states","Actor draw accepted blink_period 1")
	o.opening.actor96.blink_period = 2
	o.opening.actor96.profile = "o12_rei_line17"; o.opening.actor96.flags = 5
	check(_call(media,o,"actor_draw") == "closing actor draw requires the O_13 actor96 owner","Actor draw accepted another owner")
	o = _entry(board); o.opening["actor98"] = {"profile":"x"}
	check(_call(media,o,"actor_draw") == "closing actor draw found an unowned upper actor","Stage18 actor draw accepted a live upper actor")
	o = _entry(board); o.mode_stage = 17
	check(_call(media,o,"actor_draw") == "closing effect requires source stages 18-20","Closing effect accepted stage17")
	o.mode_stage = 18; o.mode_step = 1
	check(_call(media,o,"actor_draw") == "closing effect requires source stages 18-20","Closing effect accepted mode_step 1")
	o.mode_step = 0
	check(_call(media,o,"lip_sync") == "closing effect is outside its source substep","Closing effect accepted lip sync")
	# Line42 start needs the hidden O_13 actor left by stage19.
	var t := _setup_state(board,media)
	t.mode_stage = 20; t.substep = 0
	check(_line(media,t,42) == OUTSIDE,"Line42 accepted a visible actor96")
	t.opening.actor96.draw_flags = 0; t.opening.actor96_draw_flags = 0
	check(_line(media,t,42).is_empty(),"Line42 start rejected at 20/0")
	t.substep = 1; check(_line(media,t,42) == OUTSIDE,"Line42 accepted 20/1")
	t.substep = 0; t.opening.erase("actor96"); t.opening.erase("actor96_draw_flags")
	check(_line(media,t,42) == OUTSIDE,"Line42 accepted without actor96")
	# Production services expose the closing owner.
	check(media.callbacks().get("prologue_closing_effect") is Callable,"Bridge callbacks lack the closing effect")

func _exit_frame_checks(board: Board, media: Media) -> void:
	# Stage20 case2 through the real bridge: mode_stage_next, then
	# dec_bg_draw(1) in the same frame (prologue.c:2697-2705), so the 20->21
	# frame queues the P_01_12 draw at committed 21/0. Only line/HUD/audio
	# owners are doubles here.
	var real := {"prologue_closing_effect":media.closing_effect,"arrival_draw":media.arrival_draw,
		"line_start":func(_s,_line,_out): pass,"line_run":func(_s,_out): pass,
		"outer_draw":func(_s,_arg,_out): pass,"audio_command":func(_s,_command,_out): pass}
	var e := _setup_state(board,media)
	e.opening.actor96.draw_flags = 0; e.opening.actor96_draw_flags = 0
	e.mode_stage = 20; e.substep = 2; e.opening["prologue_line"] = 42; e.opening["prologue_scene_counter"] = 0x3c
	var hold := Session.FrameResult.new()
	Flow._stage_20_closing(e,real,hold)
	check(hold.error.is_empty() and e.mode_stage == 20 and hold.presentation == BG_DRAW,"Stage20/2 hold frame through the bridge differs: " + hold.error)
	var exit := Session.FrameResult.new()
	Flow._stage_20_closing(e,real,exit)
	check(exit.error.is_empty() and e.mode_stage == 21 and e.substep == 0 and e.opening.prologue_scene_counter == Media.CLOSING_EXIT_COUNTER and exit.presentation == BG_DRAW,"Stage20->21 frame through the bridge differs: " + exit.error)
	check(Media.closing_exit_frame(e) and Media.closing_frame(e),"Stage21/0 exit frame is not a closing frame")
	# Only that exit draws at stage 21: each departure is refused by name.
	for case: Array in [["counter",0x3d],["line",41],["boxes",null],["o13_draw",null],["substep",1],["visible",null],["upload",null]]:
		var x := State.new(); x.game_mode = e.game_mode; x.mode_stage = 21; x.mode_step = 0; x.substep = 0; x.frame_count = e.frame_count
		x.opening = e.opening.duplicate(true)
		var want := Media.CLOSING_EXIT_UNSUPPORTED
		match case[0]:
			"counter": x.opening.prologue_scene_counter = case[1]
			"line": x.opening.prologue_line = case[1]
			"boxes": x.opening["prologue_closing_draws"] = [media._closing_primitive(0xffffff,0xd,1)]
			"o13_draw": x.opening["o13_draw"] = {"frame":6,"slots":{},"frame_count":x.frame_count}
			"substep": x.substep = case[1]
			"visible":
				x.opening.actor96.draw_flags = 0xa4; x.opening.actor96_draw_flags = 0xa4
				want = "stages 19-20 require the source O_13 actor96 state"
			"upload": x.opening.names_media.buffer_1.upload_batch_key = "b".sha256_text(); want = Media.CLOSING_BG_NOT_UPLOADED
		var refused := Session.FrameResult.new(); media.arrival_draw(x,1,refused)
		check(refused.error == want and refused.presentation.is_empty(),"Stage21 draw accepted a non-exit state (%s): %s" % [case[0],refused.error])

func _initialize() -> void:
	_stage18_checks()
	_stage19_checks()
	_stage20_checks()
	_step_checks()
	_board_checks()
	if failures == 0: print("PASS: %d original stages 18-20 checks" % checks)
	quit(1 if failures else 0)
