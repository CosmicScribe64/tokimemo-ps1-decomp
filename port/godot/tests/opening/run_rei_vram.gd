extends SceneTree
## Original-resource transfers for stage7->8; not complete scene/pixel parity.
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var failures := 0

func check(ok: bool, label: String) -> void:
	if not ok:
		failures += 1
		printerr(label)

func _initialize() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"), Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error)
		quit(1)
		return
	var source_root := OS.get_environment("ARRIVAL_MEDIA_PACK")
	var next_scene := board.stage_transfer(Board.O_11_ID)
	check(next_scene.get("error") == "" and next_scene.get("destination") == 0x801e8000 and next_scene.get("lba") == 0xac1e and next_scene.get("sectors") == 33 and next_scene.get("sha256") == Board.O_11_SHA and next_scene.get("bytes",PackedByteArray()).size() == 0x10800,"Source O11 backup read identity differs")
	if not source_root.is_empty():
		var arrival := Store.new()
		check(arrival.open(source_root,FileAccess.get_sha256(source_root.path_join("manifest.json"))).is_empty(),"Installed arrival resource open failed")
		var launch_bridge := Media.new()
		check(launch_bridge.prepare(arrival,board).is_empty(),"Actual launch preparation rejected expanded original transfer inventory")
		for read: Array in [[7,5,49,0x801a4000,0xaab6],[7,6,9,0x801f0000,0xabfc],[8,5,49,0x801a4000,0xaae7],[8,6,25,0x80180000,0xac05],[9,9,49,0x801a4000,0xaa85],[9,10,33,0x801e8000,0xac1e]]:
			var waiting := State.new(); waiting.game_mode = 0x15; waiting.mode_stage = read[0]; waiting.mode_step = 0; waiting.substep = read[1]
			var accepted := Session.FrameResult.new()
			launch_bridge.queue.actor_read(waiting,read[2],read[3],read[4],accepted)
			check(accepted.error.is_empty() and accepted.presentation.size() == 1,"Exact source Rei read was rejected")
			waiting.substep = 0
			var wrong_step := Session.FrameResult.new()
			launch_bridge.queue.actor_read(waiting,read[2],read[3],read[4],wrong_step)
			check(not wrong_step.error.is_empty() and wrong_step.presentation.is_empty(),"Rei read accepted outside original source substep")
	var pixels := PackedByteArray(); pixels.resize(Pump.VRAM_BYTES); pixels.fill(0xa5)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	var scratch := PackedByteArray(); scratch.resize(512)
	var owned := {"vram_owner":"rei-transfer-test", "scratch_owner":"rei-transfer-scratch", "vram_words":pixels, "vram_known_mask":mask, "scratch_words":scratch}
	var pump := Pump.new()
	var bg := pump.execute_names_bg(board, Board.BG_AD00_ID, "rei-bg".sha256_text(), owned)
	check(bg.error.is_empty(), "AD00 decode upload rejected: " + str(bg.error))
	check(Store._sha(Pump._region(owned.vram_words, Rect2i(320,256,320,160))) == Board.BG_AD00_WORD_SHA, "AD00 words differ at original alternate-buffer rectangle")
	var cut := pump.execute_cut_vram(board, Board.CUT_08_ID, "rei-cut".sha256_text(), owned)
	check(cut.error.is_empty(), "CUT09 upload rejected: " + str(cut.error))
	var cut_bytes: PackedByteArray = board.stage_transfer(Board.CUT_08_ID).bytes
	for page in range(6):
		var rect := Rect2i(512 + (page % 3) * 64, (page / 3) * 128, 64, 128)
		check(Pump._region(owned.vram_words, rect) == cut_bytes.slice(page * 0x4000, (page + 1) * 0x4000), "CUT09 source page differs: %d" % page)
	var before: PackedByteArray = owned.vram_words.duplicate()
	var wrong := pump.execute_stage_actor_vram(board, Board.O_08_ID, 12, 0xa0, "wrong-slot".sha256_text(), owned)
	check(not wrong.error.is_empty() and before == owned.vram_words, "O09 accepted wrong source slot or mutated rejected transfer")
	var actor := pump.execute_stage_actor_vram(board, Board.O_08_ID, 5, 0xa0, "rei-actor".sha256_text(), owned)
	check(actor.error.is_empty(), "O09 first free source slot5 rejected: " + str(actor.error))
	var actor_bytes: PackedByteArray = board.stage_transfer(Board.O_08_ID).bytes
	check(Pump._region(owned.vram_words, Rect2i(320,0,64,128)) == actor_bytes.slice(0xa0,0x40a0), "O09 tpage differs from original pointed payload")
	check(Pump._region(owned.vram_known_mask, Rect2i(320,0,64,128)).count(255) == 16384, "O09 upload ownership incomplete")
	check(owned.vram_words[0] == 0xa5 and owned.vram_known_mask[0] == 0, "uploads changed unrelated VRAM")
	var white_result := pump.execute_cut_vram(board, Board.CUT_08_ID, "rei-white".sha256_text(), owned, true)
	check(white_result.error.is_empty(), "Rei white-palette source sequence rejected")
	var white_row := Pump._region(owned.vram_words, Rect2i(0,494,256,1))
	check(white_row.decode_u16(0) == 0 and white_row.decode_u16(478) == 0x7fff and white_row.decode_u16(480) == 0xffff and white_row.decode_u16(510) == 0xffff, "native white palette transparency/STP boundary differs")
	check(white_row == owned.scratch_words, "native white scratch and palette differ")
	check(pump.cut_batch_ready("rei-white".sha256_text(),owned).is_empty(), "white palette receipt describes stale gray pixels")
	check(board.actor_descriptions().get("49:2149203968:43751",{}).get("id") == Board.CUT_09_ID,"CUT10 original transfer identity missing")
	check(board.actor_descriptions().get("25:2149056512:44037",{}).get("id") == Board.O_09_ID,"O10 original transfer identity missing")
	check(pump.execute_cut_vram(board,Board.CUT_09_ID,"rei-entrance-cut".sha256_text(),owned).error.is_empty(),"CUT10 original cut upload rejected")
	for slot: int in [5,6]:
		var offset := 0x42c0 if slot == 5 else 0x82c0
		check(pump.execute_stage_actor_vram(board,Board.O_09_ID,slot,offset,("rei-entrance-%d" % slot).sha256_text(),owned).error.is_empty(),"O10 original tpage rejected")
		check(Pump._region(owned.vram_words,Rect2i(slot*64,0,64,128)) == board.stage_transfer(Board.O_09_ID).bytes.slice(offset,offset+0x4000),"O10 original tpage differs")
	# Receipt ownership checks use original bytes with an explicit synthetic
	# published-buffer setup; they do not establish the CD publication route.
	var state := State.new()
	var session := Session.new()
	session.state = state
	var media := Media.new()
	media._board_store = board
	media._session = weakref(session)
	media._buffers[str(0x801a4000)] = {"resource_id":Board.CUT_08_ID,"sha256":Board.CUT_08_SHA,"bytes":cut_bytes}
	media._buffers[str(0x801f0000)] = {"resource_id":Board.O_08_ID,"sha256":Board.O_08_SHA,"bytes":actor_bytes}
	check(media.completed_cut_buffer(state, Board.CUT_08_ID).error.is_empty(), "completed CUT09 receipt rejected original bytes")
	check(not media.completed_cut_buffer(State.new(), Board.CUT_08_ID).error.is_empty(), "completed CUT09 receipt accepted foreign state")
	check(not media.completed_stage_actor_buffer(state, Board.O_08_ID).error.is_empty(), "O09 backup became primary without copy authorization")
	check(media.completed_stage_actor_buffer(state, Board.O_08_ID, true).error.is_empty(), "O09 exact backup unavailable for authorized copy")
	state.game_mode = 0x15; state.mode_stage = 8; state.mode_step = 0; state.substep = 7
	state.substep = 3; state.frame_count = 100
	state.opening.prologue_text_work = {"line_id":12,"next_frame":100,"pos":0}
	var waiting_voice := Session.FrameResult.new()
	media.rei_banner_effect(state,"white",255,waiting_voice)
	check(waiting_voice.error.is_empty() and state.opening.prologue_text_work.next_frame == 101 and state.opening.prologue_text_work.pos == 0,"Source CD wait advanced ink or lost next text tick")
	var duplicate_wait := Session.FrameResult.new()
	media.rei_banner_effect(state,"white",255,duplicate_wait)
	check(not duplicate_wait.error.is_empty(),"Duplicate CD-wait text tick was accepted")
	state.opening.prologue_text_work.next_frame = 100
	state.opening.prologue_text_work.line_id = 13
	var wrong_line_wait := Session.FrameResult.new()
	media.rei_banner_effect(state,"white",255,wrong_line_wait)
	check(not wrong_line_wait.error.is_empty() and state.opening.prologue_text_work.next_frame == 100,"CD wait accepted another line")
	state.opening.prologue_text_work.line_id = 12; state.mode_stage = 9
	var wrong_stage_wait := Session.FrameResult.new()
	media.rei_banner_effect(state,"white",255,wrong_stage_wait)
	check(not wrong_stage_wait.error.is_empty() and state.opening.prologue_text_work.next_frame == 100,"CD wait accepted another source stage")
	state.mode_stage = 8
	state.substep = 7
	state.opening["media"] = {"epoch":1,"actor_pending":{str(0x801a4000):"cut10",str(0x80180000):"o10"},"requests":{
		"cut10":{"status":"ready","kind":"actors","resource_id":Board.CUT_09_ID,"resource_sha256":Board.CUT_09_SHA},
		"o10":{"status":"ready","kind":"actors","resource_id":Board.O_09_ID,"resource_sha256":Board.O_09_SHA}}}
	var transition := Session.FrameResult.new()
	media.rei_banner_effect(state,"next_scene",0,transition)
	check(transition.error.is_empty() and transition.presentation.size() == 3,"Rei entrance setup did not issue one cut/two actor uploads")
	check(state.opening.actor96.frame_table == 0x80184288 and state.opening.actor96.clut_row == 505 and state.opening.bg_layer_0.enabled == 0,"Rei entrance source pointers/palette/hidden cut differ")
	state.mode_stage = 9; state.substep = 0
	var first_key: String = state.opening.actor96.upload_batch_key
	var second_key: String = state.opening.actor96.upload_batch_key_2
	check(Upload._rei_entrance_owner(state,5,0x42c0,first_key) and Upload._rei_entrance_owner(state,6,0x82c0,second_key),"Original actor96 does not own both O10 texture pages")
	check(not Upload._rei_entrance_owner(state,6,0x82c0,first_key) and not Upload._rei_entrance_owner(state,7,0x82c0,second_key),"O10 accepted another page key or slot")
	var retained_actor: Dictionary = state.opening.actor96.duplicate(true)
	state.opening.actor97 = state.opening.actor96; state.opening.erase("actor96")
	check(not Upload._rei_entrance_owner(state,6,0x82c0,second_key),"O10 accepted actor97 instead of source actor96")
	state.opening.actor96 = retained_actor; state.opening.erase("actor97")
	state.substep = 3
	var crowd := Session.FrameResult.new()
	media.rei_lines_effect(state,"page_transition",0,crowd)
	check(crowd.error.is_empty() and crowd.presentation.size() == 1 and state.opening.bg_layer_0.enabled == 1,"Actual Rei page transition did not initialize original crowd/cut")
	var crowd_key: String = state.opening.actor99.upload_batch_key
	check(Upload._rei_crowd_owner(state,7,0x150,crowd_key),"Original crowd does not own its exact third texture page")
	check(not Upload._rei_crowd_owner(state,6,0x150,crowd_key) and not Upload._rei_crowd_owner(state,7,0x42c0,crowd_key),"Crowd accepted another slot or source pointer")
	state.opening.actor100.upload_batch_key = first_key
	check(not Upload._rei_crowd_owner(state,7,0x150,crowd_key),"Crowd upload accepted mismatched second actor owner")
	state.opening.actor100.upload_batch_key = crowd_key
	var crowd_duplicate := Session.FrameResult.new()
	media.rei_lines_effect(state,"page_transition",0,crowd_duplicate)
	check(not crowd_duplicate.error.is_empty() and crowd_duplicate.presentation.is_empty(),"Rei page transition replayed crowd upload")
	state.substep = 7
	var line14 := Session.FrameResult.new()
	media.rei_lines_effect(state,"line14_setup",0,line14)
	check(line14.error.is_empty() and state.opening.actor98.draw_flags == 0 and state.opening.actor98.seq_step == 3,"No-blink line14 path invented a visible actor")
	state.substep = 8
	media.rei_lines_effect(state,"line14_hold",0,line14)
	check(line14.error.is_empty() and state.opening.actor98.flags == 0,"Line14 did not stop the source sequence index")
	state.mode_stage = 8; state.substep = 7
	state.opening.media.requests.o10.status = "queued"
	var rejected := Session.FrameResult.new()
	media.rei_banner_effect(state,"next_scene",0,rejected)
	check(not rejected.error.is_empty() and rejected.presentation.is_empty(),"Rei entrance accepted an unfinished original read")
	if failures == 0: print("PASS: original Rei background, six cut pages and native actor slot transfer")
	quit(1 if failures else 0)
