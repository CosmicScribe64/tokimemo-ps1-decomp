extends SceneTree
## T-39002: stage11 prologue_line_17_run (prologue.c:1756). Part 1 is source
## control flow with explicit test doubles; part 2 uses the original board
## pack (BOARD_PACK, with O_13 and P_01_08) for the case8 O12 actor, line17
## start, the flags-5 O12 sequence, palette adjust, the case6 O_13 setup and
## its uploads, the stage11 media reads, renderer, HUD and bank poll.
const Flow = preload("res://flow/prologue_arrival.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Bgm = preload("res://services/first_arrival_bgm_bridge.gd")
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const ReiRenderer = preload("res://presentation/rei_actor_renderer.gd")
const Sequences = preload("res://services/rei_actor_sequences.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var state := State.new()
var services: Dictionary
var events: Array = []
var poll: Variant = 0
var poll_error := ""
var cd := 0
var decoded: Variant = 1
var sequence_done: Variant = false
var complete_line := false
var reject_setup := false
var failures := 0
var checks := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func effect(_state, kind, value, result) -> Variant:
	events.append([kind,value])
	if kind == "next_scene" and reject_setup: result.error = "rejected actual AC50 setup"
	return sequence_done if kind == "sequence_done" else null

func tick() -> Session.FrameResult:
	events.clear()
	var result := Session.FrameResult.new()
	Flow._stage_11_line_17(state,services,result)
	return result

func names() -> Array: return events.map(func(e): return e[0])

func _flow_checks() -> void:
	state.game_mode = 0x15; state.mode_stage = 11; state.mode_step = 0; state.substep = 0
	services = {"rei_line17_effect":effect,
		"line_start":func(_s,line,_out): events.append(["line",line]),
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["background",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"bank_poll":func(_s,out):
			events.append(["poll"])
			if not poll_error.is_empty(): out.error = poll_error
			return poll,
		"bank_start":func(_s,bank,sequence,_out): events.append(["bank_start",bank,sequence]),
		"cd_sync":func(_s,_out): return cd,
		"actor_read":func(_s,count,destination,lba,_out): events.append(["read",count,destination,lba]),
		"background_read":func(_s,lba,slot,_out,retry := false): events.append(["bg_read",lba,slot,retry]),
		"background_decode":func(_s,slot,_out): events.append(["decode",slot]); return decoded,
		"audio_command":func(_s,command,_out): events.append(["audio",command])}
	check(tick().error.is_empty() and state.substep == 1 and state.opening.prologue_line == 17,"Line17 start differs")
	check(events == [["line",17],["background",1],["outer",2]],"Substep0 must start line17 then dec_bg_draw(1): " + str(events))
	tick(); check(state.substep == 1 and events == [["run",1],["background",1],["outer",2]],"Substep1 must only run line17: " + str(events))
	complete_line = true; tick(); complete_line = false
	check(state.substep == 2,"Line17 completion did not advance")
	# Poll refusal (e.g. no native race support): no bank start, read, draw or substep change.
	poll_error = Bgm.SHIORI_NATIVE_UNAVAILABLE
	var rejected := tick()
	check(rejected.error == Bgm.SHIORI_NATIVE_UNAVAILABLE and state.substep == 2 and events == [["poll"]],"Shiori poll refusal did not stop substep2: " + str(events))
	poll_error = ""
	poll = 0; tick(); check(state.substep == 2 and names() == ["poll","background","outer"],"Pending bank poll advanced substep2")
	poll = "1"; check(tick().error == "stage11 bank poll status is unknown" and state.substep == 2,"Non-integer poll accepted")
	poll = 2; tick(); check(state.substep == 2 and not names().has("bank_start"),"Poll without bit0 started the bank")
	# audio_bank_load_poll returns -1 on its first call; (-1 & 1) advances.
	poll = -1; tick()
	check(state.substep == 3 and events.slice(0,4) == [["poll"],["bank_start",1,0],["read",49,0x801a4000,0xab18],["sequence_start",5]],"Case2 bank start/CUT12 read/flags5 order differs: " + str(events))
	tick(); check(state.substep == 3 and names() == ["sequence_done","background","outer"],"Unfinished actor96 sequence advanced")
	sequence_done = 1; check(tick().error == "Rei line17 requires actual actor96 sequence completion","Non-bool sequence completion accepted")
	sequence_done = true; tick()
	check(state.substep == 4 and events.slice(0,2) == [["sequence_done",0],["audio",0x604]],"Case3 0x604 differs: " + str(events))
	cd = 0; tick(); check(state.substep == 4 and not names().has("palette_adjust"),"Pending CUT12 read was palette-adjusted")
	cd = 1; tick(); check(state.substep == 5 and events.slice(0,2) == [["palette_adjust",0],["bg_read",0xad05,1,false]],"Case4 palette/AD05 read differs: " + str(events))
	cd = 0; tick(); check(state.substep == 5 and not names().has("read"),"AC50 read started before AD05 completed")
	cd = 1; tick(); check(state.substep == 6 and events[0] == ["read",0x41,0x801d8000,0xac50],"Case5 AC50 read differs from source")
	cd = 0; tick(); check(state.substep == 6 and not names().has("decode"),"AD05 decoded before AC50 completed")
	cd = 1; decoded = -1; tick()
	# prologue.c:1797-1800: dec_bg_reset() then dec_bg_cd_read(0xAD05,1); stay.
	check(state.mode_stage == 11 and state.substep == 6 and events.slice(0,3) == [["decode",1],["dec_bg_reset",0],["bg_read",0xad05,1,true]] and not names().has("next_scene"),"Failed AD05 decode must reset and reread: " + str(events))
	decoded = 1; reject_setup = true
	check(not tick().error.is_empty() and state.mode_stage == 11 and state.substep == 6,"Rejected case6 setup advanced the stage")
	reject_setup = false; tick()
	check(state.mode_stage == 12 and state.substep == 0 and names() == ["decode","next_scene","background","outer"],"Case6 setup/stage switch differs: " + str(names()))
	for bad: Array in [[11,0,7],[11,0,-1],[11,1,0]]:
		state.mode_stage = bad[0]; state.mode_step = bad[1]; state.substep = bad[2]
		check(tick().error == "prologue stage11 is outside source dispatch" and events.is_empty(),"Stage11 accepted step/substep %s" % str(bad))
	state.mode_step = 0; state.substep = 0; services.erase("rei_line17_effect")
	check(tick().error == "source Rei line17 actor services are unavailable" and events.is_empty(),"Stage11 ran without its O12 actor service")

func _stage10_case8(board: Board) -> State:
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 10; s.mode_step = 0; s.substep = 8
	s.opening["actor96"] = {"profile":"o11_rei_lines15","index":96,"x":-160,"upload_batch_key":"a".sha256_text(),"upload_batch_key_2":"b".sha256_text()}
	s.opening["actor96_draw_flags"] = 0xa4
	s.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xacf6},"active_draw":"dec_bg_draw"}
	s.opening["cut12_palette_adjusted"] = "cut12"
	s.opening["media"] = {"epoch":3,"actor_pending":{str(0x801a4000):"cut12",str(0x801f0000):"o12"},"requests":{
		"cut12":{"request_id":"cut12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.CUT_12_ID,"resource_sha256":Board.CUT_12_SHA,"payload":{"lba":0xab18,"sectors":49,"destination":0x801a4000}},
		"o12":{"request_id":"o12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.O_12_ID,"resource_sha256":Board.O_12_SHA,"payload":{"lba":0xac3f,"sectors":17,"destination":0x801f0000}}}}
	return s

func _line17_state(board: Board, media: Media) -> State:
	# Case8 setup, then mode_stage_next as the flow does it.
	var s := _stage10_case8(board)
	var setup := Session.FrameResult.new()
	media.rei_lines15_effect(s,"next_scene",0,setup)
	check(setup.error.is_empty(),"Exact case8 setup rejected: " + setup.error)
	s.next_stage()
	return s

func _effect_rejects(media: Media, s: State, kind: String, value: int, error: String, label: String) -> void:
	var before: Dictionary = s.opening.duplicate(true)
	var result := Session.FrameResult.new()
	media.rei_line17_effect(s,kind,value,result)
	check(result.error == error and result.presentation.is_empty() and s.opening == before,label + ": " + result.error)

func _hud(substep: int, line: int, pos: int, profile: String = "o12_rei_line17") -> int:
	return Hud.source_frame_argument({"game_mode":21,"mode_stage":11,"mode_step":0,"substep":substep,"opening":{
		"actor96_draw_flags":0xa4,"prologue_line":line,"actor96":{"profile":profile},
		"arrival_media":{"actor_initialized":true,"upload_batch_key":"k"},"prologue_text_work":{"line_id":line,"pos":pos,"total":10},
		"D_800B3DB0":1,"prologue_hud_live":{"date_visible":false},"board_media":{"read_requested":true},
		"names_media":{"bg_show_1":1,"active_draw":"dec_bg_draw"},"bg_layer_0":{"enabled":1}}})

func _board_checks() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error); failures += 1; return
	_o13_resource_checks(board)
	var o12: PackedByteArray = board.stage_transfer(Board.O_12_ID).bytes
	check(Board.o12_line17_tpage_slots(o12) == {0x0120:5},"O12 frame0 search_tpage slot differs: " + str(Board.o12_line17_tpage_slots(o12)))
	check(Board.o12_line17_tpage_slots(o12.slice(0,0x8000)).is_empty(),"Truncated O12 bytes resolved a tpage slot")
	var broken := o12.duplicate(); broken.encode_u32(0xd8,0x80190000)
	check(Board.o12_line17_tpage_slots(broken).is_empty(),"Out-of-range O12 frame pointer resolved a tpage slot")
	var media := Media.new()
	media._board_store = board
	var s := _line17_state(board,media)
	check(s.mode_stage == 11 and s.substep == 0 and s.opening.actor96.profile == "o12_rei_line17" and s.opening.actor96.tpage_slot == 5 and s.opening.actor96.flags == 0 and s.opening.actor96.frame_index == 0,"Case8 actor96 differs at the stage11 entry")
	# Replay: the case8 owner is gone once the O12 actor owns actor96.
	var replay := Session.FrameResult.new()
	s.mode_stage = 10; s.substep = 8
	media.rei_lines15_effect(s,"next_scene",0,replay)
	check(replay.error == "Rei line15 effect requires its original O11 actor owner" and replay.presentation.is_empty(),"Case8 setup replayed over the O12 actor: " + replay.error)
	s.mode_stage = 11; s.substep = 0
	# Unfinished and wrong reads leave the stage10 owner in place.
	for broken_read: Array in [["status","pending"],["epoch",4],["lba",0xac50],["destination",0x801d8000]]:
		var bad := _stage10_case8(board)
		match broken_read[0]:
			"status": bad.opening.media.requests.o12.status = broken_read[1]
			"epoch": bad.opening.media.requests.o12.epoch = broken_read[1]
			"lba": bad.opening.media.requests.o12.payload.lba = broken_read[1]
			"destination": bad.opening.media.requests.o12.payload.destination = broken_read[1]
		var rejected := Session.FrameResult.new()
		media.rei_lines15_effect(bad,"next_scene",0,rejected)
		check(rejected.error == "Rei line17 setup lacks completed original CUT12/O12 reads" and rejected.presentation.is_empty() and bad.opening.actor96.profile == "o11_rei_lines15","Case8 accepted O12 read with wrong " + broken_read[0])
	# Line17 start: only stage11 substep0 on the case8 O12 actor and CUT12.
	var started := Session.FrameResult.new()
	s.opening["prologue_text_cleared"] = true
	media.before_line0(s,17,started)
	check(started.error.is_empty() and not s.opening.has("prologue_text_cleared"),"Line17 start rejected at 11/0/0: " + started.error)
	for case: Array in [["substep",1],["line",16],["profile","o11_rei_lines15"],["flags",5],["cut",Board.CUT_07_ID],["stage",10]]:
		var other := _line17_state(board,media)
		var line := 17
		match case[0]:
			"substep": other.substep = case[1]
			"line": line = case[1]
			"profile": other.opening.actor96.profile = case[1]
			"flags": other.opening.actor96.flags = case[1]
			"cut": other.opening.cut_media.resource_id = case[1]
			"stage": other.mode_stage = case[1]
		var result := Session.FrameResult.new()
		media.before_line0(other,line,result)
		check(result.error == "first-arrival line setup is outside the source dispatch","Line17 start accepted wrong " + case[0])
	_effect_checks(board,media)
	_queue_checks(board)
	_renderer_checks(board,media)
	_hud_checks()

func _ready_request(s: State, id: String, resource: String, sha: String, lba: int, sectors: int, destination: int, status := "ready") -> void:
	s.opening.media.requests[id] = {"request_id":id,"epoch":s.opening.media.epoch,"status":status,"kind":"actors","resource_id":resource,"resource_sha256":sha,"payload":{"lba":lba,"sectors":sectors,"destination":destination}}
	s.opening.media.actor_pending[str(destination)] = id

var _o12_rows: Array = []

func _exit_state(board: Board, media: Media) -> State:
	# Stage11 state at case6: case2 CUT12 reread, case4 palette adjust,
	# case4 AD05 decoded into buffer0, case5 O_13 read at 0x801D8000.
	var s := _line17_state(board,media)
	_ready_request(s,"cut12b",Board.CUT_12_ID,Board.CUT_12_SHA,0xab18,49,0x801a4000)
	_ready_request(s,"o13",Board.O_13_ID,Board.O_13_SHA,0xac50,65,0x801d8000)
	s.opening["cut12_palette_adjusted"] = "cut12b"
	s.opening.names_media["bg_show_1"] = 0
	s.opening.names_media["buffer_0"] = {"lba":0xad05}
	s.opening.actor96["flags"] = 0; s.opening.actor96["seq_step"] = 4
	s.substep = 6
	return s

func _o13_resource_checks(board: Board) -> void:
	# O_13 is read at stage11 (0x801D8000) and stage17 (0x80180000): the
	# manifest record pins only the disc extent, each read is a composite.
	var m: Variant = JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("BOARD_PACK").path_join("manifest.json")))
	var record: Dictionary = m.stage_02_04_transfers["0xac50"]
	check(not record.read.has("destination") and Board._check_stage_resource_manifest(record,"o_13.nbn","CDROM/PROLODIR/O_13.NBN",Board.O_13_SHA,133120,0xac50,65),"O_13 manifest record is not destination-free")
	var with_dest: Dictionary = record.duplicate(true); with_dest.read["destination"] = 0x801d8000
	check(not Board._check_stage_resource_manifest(with_dest,"o_13.nbn","CDROM/PROLODIR/O_13.NBN",Board.O_13_SHA,133120,0xac50,65),"O_13 record with a destination accepted")
	var other_dest: Dictionary = record.duplicate(true); other_dest.read.erase("transfer_bytes"); other_dest.read["destination"] = 0x80180000
	check(not Board._check_stage_resource_manifest(other_dest,"o_13.nbn","CDROM/PROLODIR/O_13.NBN",Board.O_13_SHA,133120,0xac50,65),"O_13 record with a swapped destination field accepted")
	var wrong_sectors: Dictionary = record.duplicate(true); wrong_sectors.read["sectors"] = 64
	check(not Board._check_stage_resource_manifest(wrong_sectors,"o_13.nbn","CDROM/PROLODIR/O_13.NBN",Board.O_13_SHA,133120,0xac50,65),"O_13 record with wrong sectors accepted")
	check(board.stage_transfer(Board.O_13_ID).destination == -1,"O_13 transfer claims a single destination")
	var o13_reads: Array = board.actor_descriptions().values().filter(func(d: Dictionary) -> bool: return d.id == Board.O_13_ID)
	# Two source reads of the one extent: stage11 case5 to 0x801D8000 (copied
	# by case6) and stage17 case3 to 0x80180000 (drawn by stage19).
	var o13_destinations: Array = o13_reads.map(func(d: Dictionary) -> int: return int(d.destination))
	o13_destinations.sort()
	check(o13_reads.size() == 2 and o13_destinations == [0x80180000,0x801d8000] and o13_reads.all(func(d: Dictionary) -> bool: return d.sectors == 65 and d.lba == 0xac50) and board.actor_descriptions().has("65:%d:%d" % [0x801d8000,0xac50]) and board.actor_descriptions().has("65:%d:%d" % [0x80180000,0xac50]),"O_13 stage11/stage17 composite reads differ: " + str(o13_reads))

func _effect_checks(board: Board, media: Media) -> void:
	var o12: PackedByteArray = board.stage_transfer(Board.O_12_ID).bytes
	var o13: PackedByteArray = board.stage_transfer(Board.O_13_ID).bytes
	# Source tables: O12 seq0 at 0x8C, O_13 seq0 at 0x1C4.
	_o12_rows = Board.line17_sequence("o12_rei_line17",o12)
	check(_o12_rows == [{"frame":0,"duration":51},{"frame":1,"duration":6},{"frame":2,"duration":6},{"frame":3,"duration":6},{"frame":4,"duration":1}],"O12 line17 sequence differs: " + str(_o12_rows))
	var o13_rows := Board.line17_sequence("o13_rei_line17_exit",o13)
	check(o13_rows.map(func(r): return [r.frame,r.duration]) == [[0,6],[1,5],[2,5],[3,4],[4,7],[5,7],[6,80]],"O_13 sequence differs: " + str(o13_rows))
	for frame in 5: check(Board.line17_tpage_slots("o12_rei_line17",o12,frame) == {0x120:5},"O12 frame%d leaves slot5" % frame)
	check(Board.line17_tpage_slots("o13_rei_line17_exit",o13,0) == {0x230:5,0x8230:6},"O_13 frame0 search_tpage differs: " + str(Board.line17_tpage_slots("o13_rei_line17_exit",o13,0)))
	check(Board.line17_tpage_slots("o13_rei_line17_exit",o13,3) == {0x4230:5,0x10230:6},"O_13 frame3 search_tpage differs")
	check(Board.line17_frame_parts("o13_rei_line17_exit",o13,0).size() == 4 and Board.line17_frame_parts("o12_rei_line17",o12,4).size() == 2,"Line17 part counts differ")
	check(Board.line17_sequence("o13_rei_line17_exit",o13.slice(0,0x20000)).is_empty() and Board.line17_tpage_slots("o13_rei_line17_exit",o13.slice(0,0x20000),0).is_empty(),"Truncated O_13 resolved tables")
	var bad13 := o13.duplicate(); bad13.encode_u32(0x204,0x00180000)
	check(Board.line17_tpage_slots("o13_rei_line17_exit",bad13,0).is_empty(),"O_13 frame pointer without bit31 resolved a slot")
	bad13 = o13.duplicate(); bad13.encode_u16(0x1c4+4,0)
	check(Board.line17_sequence("o13_rei_line17_exit",bad13).is_empty(),"Zero-duration O_13 row accepted")
	bad13 = o13.duplicate(); bad13.encode_u16(0x8+4,8<<6)
	check(Board.line17_tpage_slots("o13_rei_line17_exit",bad13,0).is_empty(),"O_13 part page outside the 8-entry table accepted")
	check(Board.line17_tpage_slots("o12_rei_line17",o13,0).is_empty(),"O_13 bytes accepted as O12")
	var seq := Sequences.new()
	check(seq.prepare_line17(o12,o13).is_empty() and seq.prepare_line17(o12,bad13) != "","Line17 table preparation differs")
	seq.prepare_line17(o12,o13)
	# Owner and substep gates.
	var effects := _line17_state(board,media)
	_effect_rejects(media,effects,"sequence_start",5,"Rei line17 effect is outside its source substep","Sequence start accepted substep0")
	effects.substep = 3
	_effect_rejects(media,effects,"sequence_start",4,"Rei line17 effect is outside its source substep","Sequence start accepted flags 4")
	effects.substep = 5
	_effect_rejects(media,effects,"next_scene",0,"Rei line17 effect is outside its source substep","Case6 setup accepted substep5")
	_effect_rejects(media,effects,"lip_sync",0,"Rei line17 effect is outside its source substep","Line17 accepted a lip-sync effect")
	effects.substep = 6; effects.opening.actor96.profile = "o11_rei_lines15"
	_effect_rejects(media,effects,"next_scene",0,"Rei line17 effect requires its original O12 actor owner","Line17 effect accepted another actor owner")
	effects.opening.actor96.profile = "o12_rei_line17"; effects.mode_stage = 10
	_effect_rejects(media,effects,"next_scene",0,"Rei line17 effect requires its original O12 actor owner","Line17 effect accepted stage10")
	_retry_checks(board,media)
	effects.mode_stage = 11; effects.substep = 3; effects.opening.actor96.erase("seq_step")
	_effect_rejects(media,effects,"sequence_start",5,"Rei line17 actor lacks its source O12 sequence state","Sequence start accepted an actor without sequence state")
	# Case2 flags 5, then case3 waits for unk_18 == 1 && unk_08 == 1.
	var run := _line17_state(board,media); run.substep = 3
	var started := Session.FrameResult.new()
	media.rei_line17_effect(run,"sequence_start",5,started)
	check(started.error.is_empty() and run.opening.actor96.flags == 5 and started.presentation.is_empty(),"Case2 flags 5 differs: " + started.error)
	_effect_rejects(media,run,"sequence_start",5,"Rei line17 sequence start found a running O12 sequence","Sequence start replayed")
	var done_at := -1
	for frame in 80:
		var probe := Session.FrameResult.new()
		var done: Variant = media.rei_line17_effect(run,"sequence_done",0,probe)
		if done == true and done_at < 0: done_at = frame
		check(probe.error.is_empty() and done is bool,"Sequence completion probe failed")
		seq.draw_and_advance_rows(run.opening.actor96,_o12_rows,frame)
	# Draw 51 leaves step1/counter0; the draw after it gives step1/counter1.
	check(done_at == 52,"Case3 completion frame differs from step1/counter1: %d" % done_at)
	check(run.opening.actor96.flags == 0 and run.opening.actor96.seq_step == 4 and run.opening.actor96.frame_index == 4,"O12 sequence end differs from flags&4 hold: " + str(run.opening.actor96))
	# Case4 palette adjust needs the case2 reread, not the stage10 read.
	var pal := _line17_state(board,media); pal.substep = 4
	_effect_rejects(media,pal,"palette_adjust",0,"Rei line17 palette adjust lacks the completed case2 CUT12 reread","Palette adjust reused the stage10 CUT12 read")
	_ready_request(pal,"cut12b",Board.CUT_12_ID,Board.CUT_12_SHA,0xab18,49,0x801a4000,"pending")
	_effect_rejects(media,pal,"palette_adjust",0,"Rei line17 palette adjust lacks the completed case2 CUT12 reread","Palette adjust accepted a pending reread")
	pal.opening.media.requests.cut12b.status = "ready"
	var adjusted := Session.FrameResult.new()
	media.rei_line17_effect(pal,"palette_adjust",0,adjusted)
	check(adjusted.error.is_empty() and pal.opening.cut12_palette_adjusted == "cut12b","Case4 palette adjust differs: " + adjusted.error)
	# Case6 setup.
	var exit := _exit_state(board,media)
	exit.opening["actor97"] = {"profile":"stale"}; exit.opening["actor97_draw_flags"] = 0xa4
	var setup := Session.FrameResult.new()
	media.rei_line17_effect(exit,"next_scene",0,setup)
	check(setup.error.is_empty(),"Case6 setup rejected: " + setup.error)
	var a96: Dictionary = exit.opening.get("actor96",{})
	check(a96.get("profile") == "o13_rei_line17_exit" and a96.flags == 5 and a96.seq_step == 0 and a96.sequence_counter == 0 and a96.speed == 1 and a96.frame_table == 0x80180204 and a96.sequence_table == 0x80180220 and a96.tpage_table == 0x801801e4 and a96.x == -160 and a96.y == -120 and a96.resource_id == Board.O_13_ID,"Case6 actor96 differs: " + str(a96))
	check(not exit.opening.has("actor97") and exit.opening.get("actor97_draw_flags",0) == 0 and not exit.opening.has("cut12_palette_adjusted"),"Case6 did not clear the upper actors")
	check(exit.opening.bg_layer_0.enabled == 0 and exit.opening.bg_layer_0.attribute == 0x80000000 and exit.opening.bg_layer_0.resource_id == Board.CUT_12_ID and exit.opening.cut_media.request_id == "cut12b","Case6 cut layer differs from slot_set + bg_layers 0x80000000")
	var kinds: Array = setup.presentation.map(func(i): return i.kind)
	check(kinds == ["first_arrival_o13_copy","first_arrival_cut_upload","first_arrival_stage_actor_upload","first_arrival_stage_actor_upload"],"Case6 intents differ: " + str(kinds))
	var uploads: Dictionary = {}
	for intent: Dictionary in setup.presentation:
		if intent.kind == "first_arrival_stage_actor_upload": uploads[intent.slot] = intent.source_offset
	check(uploads == {5:0x230,6:0x8230} and setup.presentation[0].bytes == 0x20800,"Case6 O_13 uploads differ: " + str(uploads))
	for intent: Dictionary in setup.presentation:
		var schema: String = media._intent_schema(intent)
		check(schema.is_empty(),"Case6 intent %s fails the publication schema: %s" % [intent.kind,schema])
	var foreign: Dictionary = setup.presentation[2].duplicate(); foreign.source_offset = 0x231
	check(media._intent_schema(foreign) == "first-arrival stage actor upload descriptor is invalid","Schema accepted a misaligned O_13 page")
	var foreign_copy: Dictionary = setup.presentation[0].duplicate(); foreign_copy.bytes = 0x20000
	check(media._intent_schema(foreign_copy) == "first-arrival O_13 copy descriptor is invalid","Schema accepted a short O_13 copy")
	for broken: String in ["o13_pending","o13_destination","o13_missing","palette","ad05","cut_reread"]:
		var bad := _exit_state(board,media)
		match broken:
			"o13_pending": bad.opening.media.requests.o13.status = "pending"
			"o13_destination": bad.opening.media.requests.o13.payload.destination = 0x80180000
			"o13_missing": bad.opening.media.actor_pending.erase(str(0x801d8000))
			"palette": bad.opening.erase("cut12_palette_adjusted")
			"ad05": bad.opening.names_media.buffer_0.lba = 0xacec
			"cut_reread": bad.opening.media.actor_pending[str(0x801a4000)] = "o12"
		var expected := "Rei line17 exit lacks completed original CUT12/O_13 reads"
		if broken == "palette": expected = "Rei line17 exit lacks its source CUT12 palette adjustment"
		elif broken == "ad05": expected = "Rei line17 exit requires the decoded AD05 background"
		_effect_rejects(media,bad,"next_scene",0,expected,"Case6 accepted " + broken)

func _queue_checks(board: Board) -> void:
	var queue := Queue.new()
	check(queue.prepare(board.background_descriptions(),board.actor_descriptions(),{}).is_empty(),"Line17 queue preparation failed")
	for case: Array in [[2,0x31,0x801a4000,0xab18,true],[3,0x31,0x801a4000,0xab18,false],[5,0x41,0x801d8000,0xac50,true],[4,0x41,0x801d8000,0xac50,false],[5,0x41,0x80180000,0xac50,false],[5,0x40,0x801d8000,0xac50,false]]:
		var s := State.new(); s.game_mode = 0x15; s.mode_stage = 11; s.mode_step = 0; s.substep = case[0]
		var result := Session.FrameResult.new()
		queue.actor_read(s,case[1],case[2],case[3],result)
		check(result.error.is_empty() == case[4],"Stage11 actor read %s differs: %s" % [str(case),result.error])
	var s := State.new(); s.game_mode = 0x15; s.mode_stage = 11; s.mode_step = 0; s.substep = 4
	var bg := Session.FrameResult.new()
	queue.background_read(s,0xad05,1,bg)
	check(bg.error.is_empty() and s.opening.media.background_pending.has("1"),"Stage11 AD05 read rejected: " + bg.error)
	s.substep = 6
	var reread := Session.FrameResult.new()
	queue.background_read(s,0xad05,1,reread,true)
	check(reread.error.is_empty(),"Stage11 AD05 decode-failure reread rejected: " + reread.error)

func _renderer_state(board: Board, media: Media, substep: int) -> State:
	var s := _line17_state(board,media); s.substep = substep
	s.opening.bg_layer_0["enabled"] = 1
	if substep >= 3: s.opening.actor96["flags"] = 5
	return s

func _renderer_checks(board: Board, media: Media) -> void:
	var rei := ReiRenderer.new()
	check(rei.source.prepare_line17(board.stage_transfer(Board.O_12_ID).bytes,board.stage_transfer(Board.O_13_ID).bytes).is_empty(),"Rei renderer line17 tables rejected")
	for substep: int in [0,1,2,3,4,5,6]:
		var view := _renderer_state(board,media,substep)
		check(ReiRenderer.line17_scope(view) and rei.preflight(view).is_empty(),"Line17 renderer rejected substep%d: %s" % [substep,rei.preflight(view)])
	var cases := {"flags_early":"Rei line17 sequence flags differ from source","stopped_mid":"Rei line17 sequence flags differ from source","counter":"Rei line17 sequence cursor differs from source",
		"slot":"Rei line17 tpage allocation differs from search_tpage","actor97":"Rei line17 has an actor without a source owner","layer":"Rei line17 renderer lost its original cut owner",
		"x":"Rei line17 body source identity or geometry differs","substep":"Rei line17 renderer is outside stage11"}
	for name: String in cases:
		var view := _renderer_state(board,media,3)
		match name:
			"flags_early": view.substep = 2
			"stopped_mid": view.opening.actor96.flags = 0; view.opening.actor96.seq_step = 1
			"counter": view.opening.actor96.sequence_counter = 51
			"slot": view.opening.actor96.tpages = {"6":{"offset":0x120,"key":"k"}}
			"actor97": view.opening["actor97_draw_flags"] = 0xa4
			"layer": view.opening.bg_layer_0.enabled = 0
			"x": view.opening.actor96.x = -150
			"substep": view.substep = 7
		check(rei.preflight(view) == cases[name],"Line17 renderer accepted %s: %s" % [name,rei.preflight(view)])
	# Logical draw is actor_draw_all: draw the row, then step the sequence.
	var run := _renderer_state(board,media,3)
	var drawn: Array = []
	for frame in 75:
		run.frame_count = frame
		var error := rei.logical_draw(run)
		if not error.is_empty(): check(false,"Line17 logical draw failed: " + error); break
		drawn.append(run.opening.actor96_source_draw.frame_index)
	check(drawn.slice(0,51).count(0) == 51 and drawn.slice(51,57).count(1) == 6 and drawn[69] == 4 and drawn[74] == 4 and run.opening.actor96.flags == 0,"O12 drawn frames differ: " + str(drawn.slice(48,75)))
	# Case6 frame: stage12/substep0 with O_13 and the cut layer disabled.
	var exit := _exit_state(board,media)
	var setup := Session.FrameResult.new()
	media.rei_line17_effect(exit,"next_scene",0,setup)
	exit.next_stage()
	check(ReiRenderer.line17_scope(exit) and rei.preflight(exit).is_empty(),"Line17 exit frame rejected: " + rei.preflight(exit))
	check(rei.logical_draw(exit).is_empty() and exit.opening.actor96_source_draw.frame_index == 0 and exit.opening.actor96.sequence_counter == 1 and exit.opening.actor96_source_draw.slots == {str(0x230):5,str(0x8230):6},"Line17 exit draw differs: " + str(exit.opening.get("actor96_source_draw")))
	exit.opening.bg_layer_0.enabled = 1
	check(rei.preflight(exit) == "Rei line17 renderer lost its original cut owner","Exit frame accepted the enabled CUT12 layer")
	exit.opening.bg_layer_0.enabled = 0; exit.substep = 1
	check(not ReiRenderer.line17_scope(exit),"Stage12 beyond its entry frame entered the line17 scope")
	exit.substep = 0; exit.opening.actor96.profile = "o12_rei_line17"
	check(not ReiRenderer.line17_scope(exit),"Stage12 entry accepted the O12 actor")
	# Upload owner: O_13 pages only on the case6 frame from actor96's record.
	var owned := _exit_state(board,media)
	var owned_setup := Session.FrameResult.new()
	media.rei_line17_effect(owned,"next_scene",0,owned_setup)
	owned.next_stage()
	var key5: String = owned.opening.actor96.tpages["5"].key
	check(Upload._rei_line17_exit_owner(owned,5,0x230,key5),"O_13 slot5 upload has no owner")
	check(not Upload._rei_line17_exit_owner(owned,6,0x230,key5) and not Upload._rei_line17_exit_owner(owned,5,0x4230,key5) and not Upload._rei_line17_exit_owner(owned,5,0x230,"x".sha256_text()),"O_13 upload owner accepted a foreign slot/page/key")
	owned.mode_stage = 11
	check(not Upload._rei_line17_exit_owner(owned,5,0x230,key5),"O_13 upload owner accepted stage11")

func _hud_checks() -> void:
	check(_hud(0,16,10) == 2 and _hud(0,16,9) == -1 and _hud(0,17,10) == -1,"Stage11 handoff HUD frame differs")
	check(_hud(1,17,3) == 2 and _hud(1,16,10) == -1,"Stage11 line17 HUD frame differs")
	for substep in [2,3,4,5,6]: check(_hud(substep,17,10) == 2 and _hud(substep,17,9) == -1,"Stage11 completed line17 HUD differs at substep%d" % substep)
	check(_hud(7,17,10) == -1 and _hud(1,17,3,"o11_rei_lines15") == -1,"Stage11 HUD accepted substep7 or another actor")
	check(_hud_exit(0,"o13_rei_line17_exit",0) == 2,"Line17 exit HUD frame rejected")
	check(_hud_exit(0,"o13_rei_line17_exit",1) == -1,"Line17 exit HUD accepted an enabled CUT12 layer")
	# Past the exit frame the stage12 body owns the frame; its outer HUD is
	# hud_screen_frame_draw(2) like every stage but 0 (prologue.c:2812-2817).
	check(_hud_exit(1,"o13_rei_line17_exit",0) == 2 and _hud_exit(0,"o12_rei_line17",0) == 2,"Stage12 body frame lost its outer HUD argument 2")

func _hud_exit(substep: int, profile: String, enabled: int) -> int:
	return Hud.source_frame_argument({"game_mode":21,"mode_stage":12,"mode_step":0,"substep":substep,"opening":{
		"actor96_draw_flags":0xa4,"prologue_line":17,"actor96":{"profile":profile},
		"arrival_media":{"actor_initialized":true,"upload_batch_key":"k"},"prologue_text_work":{"line_id":17,"pos":10,"total":10},
		"D_800B3DB0":1,"prologue_hud_live":{"date_visible":false},"board_media":{"read_requested":true},
		"names_media":{"bg_show_1":0,"active_draw":"dec_bg_draw"},"bg_layer_0":{"enabled":enabled}}})

func _ad05_request(s: State, request_id: String, status: String) -> void:
	s.opening.media.requests[request_id] = {"request_id":request_id,"epoch":3,"status":status,"kind":"background","resource_id":Board.BG_AD05_ID}
	s.opening.media["background_pending"] = {"1":request_id}
	s.opening.media["decoded"] = {}

func _retry_checks(board: Board, media: Media) -> void:
	# Case6 retry through the production decode and reset: a failed AD05
	# decode runs dec_bg_reset (background_images.c:37-48), so the reread
	# decodes into buffer 1 - dec_bg_show[1] = 1 (320,256) over ACF6.
	var s := _line17_state(board,media)
	s.substep = 6; s.frame_count = 500
	s.opening["bg_brightness"] = 0x40
	_ad05_request(s,"ad05-a","retry")
	var reads: Array = []
	var svc := {"rei_line17_effect":media.rei_line17_effect,"cd_sync":func(_s,_out): return 1,
		"background_decode":media.background_decode,
		"background_read":func(_s,lba,slot,_out,retry := false): reads.append([lba,slot,retry]),
		"arrival_draw":func(_s,_slot,_out): pass,"outer_draw":func(_s,_arg,_out): pass}
	var out := Session.FrameResult.new()
	Flow._stage_11_line_17(s,svc,out)
	var nm: Dictionary = s.opening.names_media
	check(out.error.is_empty() and s.mode_stage == 11 and s.substep == 6 and reads == [[0xad05,1,true]],"Stage11 AD05 retry did not reread after the reset: %s %s" % [out.error,str(reads)])
	check(nm.bg_show_1 == 0 and nm.buffer_0 == {"lba":0,"vram_lba":0xacec} and nm.buffer_1 == {"lba":0,"vram_lba":0xacf6} and nm.dec_bg_reset_at == [11,6,500] and s.opening.bg_brightness == 0x80,"Stage11 dec_bg_reset state differs: " + str(nm))
	# Without this substep's reset marker the cleared caches are refused.
	var stale := State.new(); stale.restore(s.projection()); stale.opening.names_media.erase("dec_bg_reset_at")
	_ad05_request(stale,"ad05-b","ready")
	out = Session.FrameResult.new()
	check(media.background_decode(stale,1,out) == -1 and out.error == "first-arrival stage11 0xad05 decode requires buffer 1 shown with 0xacf6 or this substep's dec_bg_reset","Stage11 decode accepted cleared caches without the reset")
	# The reread completes: AD05 lands in buffer 1, not the nominal buffer 0.
	_ad05_request(s,"ad05-b","ready")
	out = Session.FrameResult.new()
	check(media.background_decode(s,1,out) == 1 and out.error.is_empty(),"Stage11 retried AD05 decode did not select buffer 1: " + out.error)
	nm = s.opening.names_media
	check(nm.bg_show_1 == 1 and nm.buffer_1.lba == 0xad05 and nm.buffer_1.decode_rect == [320,256,320,160] and nm.buffer_1.request_id == "ad05-b" and nm.buffer_0 == {"lba":0,"vram_lba":0xacec} and not nm.has("dec_bg_reset_at") and s.opening.media.decoded["1"].buffer == 1,"Stage11 retried AD05 buffer state differs: " + str(nm))
	check(out.presentation.map(func(i): return [i.kind,i.resource_id,i.buffer]) == [["opening_background_select",Board.BG_AD05_ID,1],["first_arrival_names_bg_upload",Board.BG_AD05_ID,1]] and out.presentation[1].decode_rect == [320,256,320,160],"Stage11 retried AD05 intents differ: " + str(out.presentation))
	# The nominal path still fills buffer 0 at (704,0).
	var nominal := _line17_state(board,media)
	nominal.substep = 6
	_ad05_request(nominal,"ad05-c","ready")
	out = Session.FrameResult.new()
	check(media.background_decode(nominal,1,out) == 0 and nominal.opening.names_media.buffer_0.decode_rect == [704,0,320,160] and nominal.opening.names_media.bg_show_1 == 0,"Stage11 nominal AD05 decode differs: " + out.error)
	# Stage12's decode guard refuses the flipped buffers instead of
	# following the nominal chain.
	s.mode_stage = 12; s.substep = 3
	out = Session.FrameResult.new()
	check(media.background_decode(s,1,out) == -1 and out.error == "Shiori background decode requires the source slot-1 buffers","Stage12 accepted the buffers of a stage11 retry")
	# The reset belongs to case6 only, with value 0.
	var other := _line17_state(board,media)
	other.substep = 5
	_effect_rejects(media,other,"dec_bg_reset",0,"Rei line17 effect is outside its source substep","Stage11 dec_bg_reset accepted substep5")
	other.substep = 6
	_effect_rejects(media,other,"dec_bg_reset",1,"Rei line17 effect is outside its source substep","Stage11 dec_bg_reset accepted value 1")

func _poll_checks() -> void:
	var boot: Variant = load("res://app/first_arrival_boot.gd").new()
	var s := State.new()
	s.opening["audio_bank"] = {"role":"prologue_shiori","epoch":3,"ready":true}
	var result := Session.FrameResult.new()
	check(boot._bank_poll(s,result) == 0 and result.error == "original opening BGM backend is unavailable","Bank poll without the BGM owner accepted the Shiori bank: " + result.error)
	# With the owner, the Shiori poll is the bridge's measured-schedule poll;
	# without an in-flight load it is refused, never a ready-flag wait.
	boot._bgm_prepare_error = ""
	s.opening.audio_bank.ready = false
	result = Session.FrameResult.new()
	check(boot._bank_poll(s,result) == 0 and result.error == "Shiori bank poll has no owned in-flight load","Bank poll did not delegate the Shiori bank to the BGM owner: " + result.error)
	s.opening["audio_bank"] = {"role":"prologue_rei","epoch":2,"ready":true}
	result = Session.FrameResult.new()
	check(boot._bank_poll(s,result) == 1 and result.error.is_empty(),"Rei bank poll control differs: " + result.error)
	boot.free()

func _initialize() -> void:
	_flow_checks()
	_board_checks()
	_poll_checks()
	if failures == 0: print("PASS: %d original stage11 line17 checks" % checks)
	quit(1 if failures else 0)
