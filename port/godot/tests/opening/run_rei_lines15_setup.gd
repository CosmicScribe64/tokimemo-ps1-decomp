extends SceneTree
## T-39002: stage9/substep11 O11 copy and actor96 setup with original bytes.
## Published buffers are set explicitly; the CD route is covered by the
## installed Rei observer, not by this test.
const Board = preload("res://services/first_arrival_board_store.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Sequences = preload("res://services/rei_actor_sequences.gd")
const ReiRenderer = preload("res://presentation/rei_actor_renderer.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
var failures := 0
var checks := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func _waiting(board: Board) -> State:
	var state := State.new()
	state.game_mode = 0x15; state.mode_stage = 9; state.mode_step = 0; state.substep = 11
	state.opening["actor96"] = {"profile":"o09_rei_entrance","index":96,"resource_id":Board.O_09_ID,"upload_batch_key":"a".sha256_text(),"upload_batch_key_2":"b".sha256_text()}
	state.opening["actor96_draw_flags"] = 0xa4
	state.opening["actor98"] = {"profile":"o10_rei_dialogue","index":98}
	state.opening["actor98_draw_flags"] = 0xac
	state.opening["cut_media"] = {"resource_id":Board.CUT_09_ID,"request_id":"cut10"}
	state.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xad00},"active_draw":"dec_bg_draw"}
	state.opening["media"] = {"epoch":3,"actor_pending":{str(0x801a4000):"cut08",str(0x801e8000):"o11"},"requests":{
		"cut08":{"epoch":3,"status":"ready","kind":"actors","resource_id":Board.CUT_07_ID,"resource_sha256":Board.CUT_07_SHA,"payload":{"lba":0xaa85,"sectors":49,"destination":0x801a4000}},
		"o11":{"epoch":3,"status":"ready","kind":"actors","resource_id":Board.O_11_ID,"resource_sha256":Board.O_11_SHA,"payload":{"lba":0xac1e,"sectors":33,"destination":0x801e8000}}}}
	return state

func _rejects(media: Media, state: State, error: String, label: String) -> void:
	var before: Dictionary = state.opening.duplicate(true)
	var result := Session.FrameResult.new()
	media.rei_lines_effect(state,"next_scene",0,result)
	check(result.error == error and result.presentation.is_empty() and state.opening == before,label + ": " + result.error)

func _initialize() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error); quit(1); return
	var o11: PackedByteArray = board.stage_transfer(Board.O_11_ID).bytes
	var media := Media.new()
	media._board_store = board
	var session := Session.new()

	var state := _waiting(board)
	var done := Session.FrameResult.new()
	media.rei_lines_effect(state,"next_scene",0,done)
	check(done.error.is_empty(),"Exact case11 setup rejected: " + done.error)
	var kinds: Array = done.presentation.map(func(item): return item.kind)
	check(kinds == ["first_arrival_o11_copy","first_arrival_cut_upload","first_arrival_stage_actor_upload","first_arrival_stage_actor_upload"],"Case11 effects differ: " + str(kinds))
	check(done.presentation[0].bytes == 0x10800 and done.presentation[0].sha256 == Board.O_11_SHA,"O11 memcpy extent differs from source 0x10800")
	var a96: Dictionary = state.opening.get("actor96",{})
	var expected := {"profile":"o11_rei_lines15","unk_01":10,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180150,"sequence_table":0x80180174,"tpage_table":0x80180140,"frame_index":0,"brightness":128,"x":-160,"y":-120,"resource_id":Board.O_11_ID}
	for key: String in expected: check(a96.get(key) == expected[key],"actor96 source field differs: " + key)
	check(not state.opening.has("actor98") and state.opening.get("actor98_draw_flags",0) == 0,"actor_clear_upper kept actor98")
	check(state.opening.names_media.bg_show_1 == 0,"dec_bg_show_switch(1) did not toggle the shown buffer")
	check(state.opening.bg_layer_0.enabled == 1 and state.opening.bg_layer_0.clut_row == 495 and state.opening.bg_layer_0.resource_id == Board.CUT_07_ID,"prologue_background_slot_set(0x801a4000,0xf) state differs")
	# actor_draw_all walks frame0 parts count-1..0: page2 (0x8190) is met
	# first and gets free slot5, page1 (0x4190) slot6.
	check(done.presentation[2].slot == 5 and done.presentation[2].source_offset == 0x8190 and done.presentation[3].slot == 6 and done.presentation[3].source_offset == 0x4190,"O11 frame0 search_tpage allocation differs")
	check(Board.o11_tpage_slots(o11,[0]) == {0x8190:5,0x4190:6},"BoardStore frame0 allocation differs")
	check(Board.o11_tpage_slots(o11,[0,1,5]) == {0x8190:5,0x4190:6,0x0190:7,0xc190:37} and Board.o11_tpage_slots(o11,[0,2,5]) == {0x8190:5,0x4190:6,0xc190:7},"Lip pages do not take the next source-free slots (7, then 37 past cut locks 8-10)")
	for intent: Dictionary in done.presentation: check(media._intent_schema(intent).is_empty(),"Case11 intent fails schema: " + intent.kind)
	var replay := Session.FrameResult.new()
	media.rei_lines_effect(state,"next_scene",0,replay)
	check(replay.error == "Rei dialogue effect requires its original O10 actor owner" and replay.presentation.is_empty(),"Case11 setup replayed after its owner changed: " + replay.error)

	const READS := "Rei line15 setup lacks completed original CUT08/O11 reads"
	var wrong_step := _waiting(board); wrong_step.substep = 10
	_rejects(media,wrong_step,"Rei dialogue effect is outside its source substep","Case11 setup accepted substep10")
	var unfinished := _waiting(board); unfinished.opening.media.requests.o11.status = "pending"
	_rejects(media,unfinished,READS,"Case11 setup accepted an unfinished O11 read")
	var cut_pending := _waiting(board); cut_pending.opening.media.requests.cut08.status = "queued"
	_rejects(media,cut_pending,READS,"Case11 setup accepted an unfinished CUT08 read")
	var wrong_sha := _waiting(board); wrong_sha.opening.media.requests.o11.resource_sha256 = Board.O_09_SHA
	_rejects(media,wrong_sha,READS,"Case11 setup accepted another O11 SHA")
	var wrong_cut := _waiting(board); wrong_cut.opening.media.requests.cut08.resource_id = Board.CUT_08_ID
	_rejects(media,wrong_cut,READS,"Case11 setup accepted another cut resource")
	var wrong_extent := _waiting(board); wrong_extent.opening.media.requests.o11.payload.sectors = 25
	_rejects(media,wrong_extent,READS,"Case11 setup accepted the former 25-sector O11 extent")
	var foreign_epoch := _waiting(board); foreign_epoch.opening.media.requests.o11.epoch = 2
	_rejects(media,foreign_epoch,READS,"Case11 setup accepted a read from another media epoch")
	var other_owner := _waiting(board); other_owner.opening.actor96.profile = "o10_rei_dialogue"
	_rejects(media,other_owner,"Rei dialogue effect requires its original O10 actor owner","Case11 setup accepted a non-O10 actor owner")
	var bad_copy: Dictionary = done.presentation[0].duplicate(); bad_copy.bytes = 0x8800
	check(not media._intent_schema(bad_copy).is_empty(),"O11 copy accepted another byte count")
	var bad_page: Dictionary = done.presentation[2].duplicate(); bad_page.slot = 8
	check(not media._intent_schema(bad_page).is_empty(),"O11 upload accepted the cut-locked slot8")

	# Page ownership on the published stage10/substep0 frame.
	state.mode_stage = 10; state.substep = 0
	var key5: String = done.presentation[2].batch_key
	var key6: String = done.presentation[3].batch_key
	check(Upload._rei_lines15_owner(state,5,0x8190,key5) and Upload._rei_lines15_owner(state,6,0x4190,key6),"Recorded residency does not own both O11 frame0 pages")
	check(not Upload._rei_lines15_owner(state,5,0x4190,key5) and not Upload._rei_lines15_owner(state,6,0x4190,key5) and not Upload._rei_lines15_owner(state,7,0x0190,key5),"O11 accepted another page, slot or key")
	state.substep = 1
	check(not Upload._rei_lines15_owner(state,5,0x8190,key5),"O11 page owner accepted substep1")
	state.substep = 0
	state.opening["actor97"] = state.opening.actor96; state.opening.erase("actor96")
	check(not Upload._rei_lines15_owner(state,5,0x8190,key5),"O11 page owner accepted actor97")
	state.opening["actor96"] = state.opening.actor97; state.opening.erase("actor97")
	# Post-switch blink: modelled as the source's skipped actor98.
	var blink := Session.FrameResult.new()
	media.rei_lines_effect(state,"blink",0,blink)
	var a98: Dictionary = state.opening.get("actor98",{})
	check(blink.error.is_empty() and a98.get("profile") == "o10_tables_over_o11" and a98.get("frame_table") == 0x80184288 and a98.get("tpage_table") == 0x80184280 and a98.get("flags") == 3 and state.opening.actor98_draw_flags == 0xac,"Post-switch blink differs from prologue_actor98_animation_start_rei: " + blink.error)
	check((o11.decode_u32(0x4280) & 0x80000000) == 0 and (o11.decode_u32(0x4288) & 0x80000000) == 0,"O11 words at the O10 table addresses gained bit31; actor_draw_all would no longer skip")
	check(Board.o11_drawn_frames(state.opening) == [0],"Skipped actor98 took a tpage slot")
	var second_blink := Session.FrameResult.new()
	media.rei_lines_effect(state,"blink",0,second_blink)
	check(second_blink.error == "Rei post-switch blink found a live actor98 after actor_clear_upper","Second post-switch blink accepted: " + second_blink.error)
	var tables_check := Sequences.new(); tables_check.prepare_lines15(o11)
	var renderer := ReiRenderer.new(); renderer.source = tables_check
	check(renderer.preflight(state).is_empty(),"Renderer rejected the source-skipped actor98: " + renderer.preflight(state))
	state.substep = 3
	check(renderer.preflight(state) == "Rei O11 post-switch actor98 is not the source-skipped record","Renderer kept the skipped actor98 past case2's actor_clear")
	state.substep = 0

	# Published-buffer authority for the same-frame copy.
	media._session = weakref(session); session.state = state
	check(not media.completed_stage_actor_buffer(state,Board.O_11_ID).error.is_empty(),"O11 became primary without the case11 copy")
	media._buffers[str(0x801e8000)] = {"resource_id":Board.O_11_ID,"sha256":Board.O_11_SHA,"bytes":o11}
	check(media.completed_stage_actor_buffer(state,Board.O_11_ID,true).error.is_empty(),"Authorized O11 copy source unavailable")
	var altered := o11.duplicate(); altered[0x4190] ^= 1
	media._buffers[str(0x801e8000)] = {"resource_id":Board.O_11_ID,"sha256":Board.O_11_SHA,"bytes":altered}
	check(not media.completed_stage_actor_buffer(state,Board.O_11_ID,true).error.is_empty(),"Altered O11 bytes accepted")

	var pixels := PackedByteArray(); pixels.resize(Pump.VRAM_BYTES)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	var scratch := PackedByteArray(); scratch.resize(512)
	var owned := {"vram_owner":"lines15","scratch_owner":"lines15-scratch","vram_words":pixels,"vram_known_mask":mask,"scratch_words":scratch}
	var pump := Pump.new()
	check(not pump.execute_stage_actor_vram(board,Board.O_11_ID,26,0x0190,"o11-page0".sha256_text(),owned).error.is_empty(),"Pump accepted O11 over the O01-locked slot26")
	check(not pump.execute_stage_actor_vram(board,Board.O_11_ID,8,0xc190,"o11-page3".sha256_text(),owned).error.is_empty(),"Pump accepted O11 page3 over the cut slot")
	for spec: Array in [[5,0x8190,Vector2i(320,0)],[6,0x4190,Vector2i(384,0)],[7,0x0190,Vector2i(448,0)],[37,0xc190,Vector2i(320,128)],[58,0xc190,Vector2i(640,384)]]:
		check(pump.execute_stage_actor_vram(board,Board.O_11_ID,spec[0],spec[1],("o11-%d" % spec[0]).sha256_text(),owned).error.is_empty(),"O11 original page rejected")
		check(Pump._region(owned.vram_words,Rect2i(spec[2],Vector2i(64,128))) == o11.slice(spec[1],spec[1]+0x4000),"O11 page differs from original bytes at its tpage_buf slot")

	var tables := Sequences.new()
	check(tables.prepare_lines15(board.stage_transfer(Board.O_09_ID).bytes) != "","O11 tables accepted O10 bytes")
	check(tables.prepare_lines15(o11).is_empty(),"Original O11 tables rejected")
	var parts := tables.parts("o11",0)
	check(parts.size() == 5 and parts.all(func(part): return part.page_offset in [0x4190,0x8190]),"O11 frame0 parts or pages differ")
	_stage10_checks(board,media,pump,owned)
	_lip_checks(board,media)
	if failures == 0: print("PASS: %d stage9/11 O11 and stage10 O12 copy, setup and page-ownership checks" % checks)
	quit(1 if failures else 0)

func _stage10(board: Board, substep: int) -> State:
	var state := State.new()
	state.game_mode = 0x15; state.mode_stage = 10; state.mode_step = 0; state.substep = substep
	state.opening["actor96"] = {"profile":"o11_rei_lines15","index":96,"x":-160,"upload_batch_key":"a".sha256_text(),"upload_batch_key_2":"b".sha256_text()}
	state.opening["actor96_draw_flags"] = 0xa4
	state.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xacf6},"active_draw":"dec_bg_draw"}
	state.opening["cut12_palette_adjusted"] = "cut12"
	state.opening["media"] = {"epoch":3,"actor_pending":{str(0x801a4000):"cut12",str(0x801f0000):"o12"},"requests":{
		"cut12":{"request_id":"cut12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.CUT_12_ID,"resource_sha256":Board.CUT_12_SHA,"payload":{"lba":0xab18,"sectors":49,"destination":0x801a4000}},
		"o12":{"request_id":"o12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.O_12_ID,"resource_sha256":Board.O_12_SHA,"payload":{"lba":0xac3f,"sectors":17,"destination":0x801f0000}}}}
	return state

func _stage10_checks(board: Board, media: Media, pump: Pump, owned: Dictionary) -> void:
	var moving := _stage10(board,2)
	moving.opening["actor97"] = {"x":-160}
	for frame in 3: media.rei_lines15_effect(moving,"advance_x",0,Session.FrameResult.new())
	check(moving.opening.actor96.x == -162 and moving.opening.actor96.x_low == 0x8000 and moving.opening.actor97.x == -162,"x.w -= 0x8000 or the 97/98 x.h[1] copy differs")
	var early := _stage10(board,1)
	var early_move := Session.FrameResult.new()
	media.rei_lines15_effect(early,"advance_x",0,early_move)
	check(not early_move.error.is_empty() and early.opening.actor96.x == -160,"x moved before source substep2")
	var palette := _stage10(board,6); palette.opening.erase("cut12_palette_adjusted")
	var adjusted := Session.FrameResult.new()
	media.rei_lines15_effect(palette,"palette_adjust",0,adjusted)
	check(adjusted.error.is_empty() and palette.opening.cut12_palette_adjusted == "cut12","CUT12 palette adjust did not bind its completed read")
	palette.opening.media.requests.cut12.status = "pending"
	var pending_adjust := Session.FrameResult.new()
	media.rei_lines15_effect(palette,"palette_adjust",0,pending_adjust)
	check(not pending_adjust.error.is_empty(),"Palette adjust accepted an unfinished CUT12 read")
	var setup := _stage10(board,8)
	var done := Session.FrameResult.new()
	media.rei_lines15_effect(setup,"next_scene",0,done)
	check(done.error.is_empty(),"Exact case8 setup rejected: " + done.error)
	check(done.presentation.map(func(i): return i.kind) == ["first_arrival_o12_restore","first_arrival_cut_upload","first_arrival_stage_actor_upload"],"Case8 memcpy/cut/actor effects differ")
	check(done.presentation[0].bytes == 0x8800 and setup.opening.actor96.frame_table == 0x801800d8 and setup.opening.actor96.sequence_table == 0x801800f0 and setup.opening.actor96.tpage_table == 0x801800d4 and setup.opening.actor96.flags == 0 and setup.opening.bg_layer_0.clut_row == 495,"Case8 O12 actor/cut source fields differ")
	for intent: Dictionary in done.presentation: check(media._intent_schema(intent).is_empty(),"Case8 intent fails schema: " + intent.kind)
	for broken: Array in [["cut12_palette_adjusted",null],["bg_show_1",0],["o12_dest",0x80180000],["o12_status","pending"],["cut_sha",Board.CUT_07_SHA]]:
		var state := _stage10(board,8)
		match broken[0]:
			"cut12_palette_adjusted": state.opening.erase("cut12_palette_adjusted")
			"bg_show_1": state.opening.names_media.bg_show_1 = 0
			"o12_dest": state.opening.media.requests.o12.payload.destination = broken[1]
			"o12_status": state.opening.media.requests.o12.status = broken[1]
			"cut_sha": state.opening.media.requests.cut12.resource_sha256 = broken[1]
		var rejected := Session.FrameResult.new()
		media.rei_lines15_effect(state,"next_scene",0,rejected)
		check(not rejected.error.is_empty() and rejected.presentation.is_empty() and state.opening.actor96.profile == "o11_rei_lines15","Case8 setup accepted " + broken[0])
	var wrong_step := _stage10(board,7)
	var step_result := Session.FrameResult.new()
	media.rei_lines15_effect(wrong_step,"next_scene",0,step_result)
	check(not step_result.error.is_empty(),"Case8 setup accepted substep7")
	var decode := _stage10(board,8); decode.opening.names_media = {"bg_show_1":0,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xad00}}
	decode.opening.media["background_pending"] = {"1":"acf6"}; decode.opening.media["decoded"] = {}
	decode.opening.media.requests["acf6"] = {"request_id":"acf6","status":"ready","kind":"background","resource_id":Board.BG_ACF6_ID}
	var decoded := Session.FrameResult.new()
	check(media.background_decode(decode,1,decoded) == 1 and decoded.error.is_empty() and decode.opening.names_media.bg_show_1 == 1 and decode.opening.names_media.buffer_1.decode_rect == [320,256,320,160],"ACF6 did not decode into the hidden buffer1")
	for intent: Dictionary in decoded.presentation: check(media._intent_schema(intent).is_empty(),"ACF6 buffer1 intent fails schema")
	check(pump.execute_names_bg(board,Board.BG_ACF6_ID,"acf6-1".sha256_text(),owned,1).error.is_empty() and Store._sha(Pump._region(owned.vram_words,Rect2i(320,256,320,160))) == Board.BG_ACF6_WORD_SHA,"ACF6 words differ at buffer1")
	# ACEC also decodes into buffer 1 at stage14 case6 (prologue.c:2103); ACE6 never does.
	check(not pump.execute_names_bg(board,Board.BG_ACE6_ID,"ace6-1".sha256_text(),owned,1).error.is_empty(),"Another background accepted the stage10 buffer override")

func _lip_checks(board: Board, media: Media) -> void:
	var state := _waiting(board)
	media.rei_lines_effect(state,"next_scene",0,Session.FrameResult.new())
	state.mode_stage = 10; state.substep = 2; state.frame_count = 40
	state.opening["audio"] = {"status":"playing","clip_index":0}
	state.opening["prologue_lip_sync"] = {"clip_levels":[[40]],"clip_frame":0}
	var first := Session.FrameResult.new()
	media.rei_lines15_effect(state,"lip_sync",0,first)
	check(first.error.is_empty() and state.opening.actor97.frame_index == 2 and state.opening.actor98.frame_index == 6,"Lip sync frames differ from ring+1/ring+5: " + first.error)
	check(first.presentation.size() == 1 and first.presentation[0].slot == 7 and first.presentation[0].source_offset == 0xc190,"Mouth on resident page1 must leave eyes page3 the next free slot7: " + str(first.presentation))
	var eyes_key: String = first.presentation[0].batch_key
	check(Upload._rei_lines15_owner(state,7,0xc190,eyes_key),"Lip-sync eye page has no source owner")
	check(not Upload._rei_lines15_owner(state,37,0xc190,eyes_key) and not Upload._rei_lines15_owner(state,7,0x0190,eyes_key),"Lip page owner accepted another slot or page")
	var stale := Session.FrameResult.new()
	state.frame_count = 41
	media.rei_lines15_effect(state,"lip_sync",0,stale)
	check(stale.error.is_empty() and stale.presentation.is_empty(),"Unchanged lip pages were uploaded again")
	state.opening.prologue_lip_sync.clip_levels = [[0]]; state.opening.prologue_cd_level_history = [0,0,0,0]; state.frame_count = 42
	var second := Session.FrameResult.new()
	media.rei_lines15_effect(state,"lip_sync",0,second)
	var placed: Array = second.presentation.map(func(i): return [i.slot,i.source_offset])
	check(second.error.is_empty() and state.opening.actor97.frame_index == 1 and placed == [[7,0x0190],[37,0xc190]],"Mouth page0 must take slot7 and push eyes page3 to slot37: " + str(placed))
	check(Upload._rei_lines15_owner(state,37,0xc190,second.presentation[1].batch_key) and not Upload._rei_lines15_owner(state,7,0xc190,eyes_key),"Displaced lip page kept its old owner")
	state.substep = 4
	check(not Upload._rei_lines15_owner(state,37,0xc190,second.presentation[1].batch_key),"Lip page owner accepted substep4")
	state.substep = 2; state.opening.audio.status = "stopped"
	var silent := Session.FrameResult.new()
	media.rei_lines15_effect(state,"lip_sync",0,silent)
	check(silent.error.is_empty() and not state.opening.has("actor97") and not state.opening.has("actor98") and state.opening.o11_tpages.slots.size() == 2,"Inactive CD must actor_clear 97/98 and keep only body pages")
	state.substep = 3
	var late := Session.FrameResult.new()
	media.rei_lines15_effect(state,"lip_sync",0,late)
	check(late.error == "Rei line15 effect is outside its source substep","Lip sync accepted substep3: " + late.error)
