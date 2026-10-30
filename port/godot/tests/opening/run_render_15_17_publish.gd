extends SceneTree
## Stages 14-16 media through a bound Session: every frame goes through
## media.preflight (cd_logic, via the upload bridge), media.publish and the
## FirstArrivalUploadBridge preview/commit into its own owned VRAM.
## Stage 0/1 run the real arrival bodies (actor and board VRAM ownership,
## as run_first_arrival_board_load). Stages 14-16 are a mode fixture that
## makes the source calls of the frames under test (reads, decode, slot
## sets, memcpy, actor_draw); it is not a route and skips stages 2-13.
## Inputs: BOARD_PACK, FIRST_ARRIVAL_MEDIA_ROOT/SHA, PROLOGUE_HUD_ROOT.
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Cold = preload("res://flow/cold_no_card_scratch.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const Renderer = preload("res://presentation/opening_stages_15_17_renderer.gd")
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

const E8_REFUSED := "first-arrival 0x801E8000 copy lacks its completed original read"
const NO_RESIDENT := "first-arrival stage actor upload lacks resident NBN buffer at 0x80180000"

var checks := 0
var failures := 0
var media := Media.new()
var upload := Upload.new()
var actor_renderer := ActorRenderer.new()
var hud := Hud.new()
var session := Session.new()
var state := State.new()
var board_store := BoardStore.new()
var media_store := Store.new()
var owned: Dictionary
var services: Dictionary = {}
var fixture: Callable

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func tick(include_pending: bool = true) -> Session.FrameResult:
	var events: Array[Session.ServiceEvent] = []
	if include_pending: events = media.pending_events(session.clock.logic_iterations)
	return session.step(InputFrame.new(session.clock.logic_iterations),events)

func commit(label: String, include_pending: bool = true) -> Session.FrameResult:
	# One committed frame, then the media and upload publishers in order.
	var r := tick(include_pending)
	var error := r.error if not r.committed else ""
	if r.committed:
		var at: int = session.clock.logic_iterations-1
		error = media.publish(r,at)
		if error.is_empty(): error = upload.publish(r,at)
	check(r.committed and error.is_empty(),"%s: %s" % [label,error])
	return r

## Commits frames until done() holds. Reads complete on the media queue's
## logic-frame CD schedule (MediaQueue.cd_wait_frames: serialized reads,
## floor(14 + 0.4 * sectors) frames each), so a cd_sync poll holds its
## substep until then; every hold frame is a clean committed frame.
func commit_until(label: String, done: Callable, limit: int = 120) -> Session.FrameResult:
	var r: Session.FrameResult = null
	for i in limit:
		r = commit("%s (frame %d)" % [label,i])
		if not r.committed or done.call(): return r
	check(false,"%s did not complete within %d frames" % [label,limit])
	return r

func _hud_projection(current: State) -> Dictionary:
	var opening_copy: Dictionary = current.opening.duplicate(false)
	if opening_copy.get("prologue_hud_live") is Dictionary:
		opening_copy["prologue_hud_live"] = opening_copy.prologue_hud_live.duplicate(true)
	return {"game_mode":current.game_mode,"mode_stage":current.mode_stage,"mode_step":current.mode_step,"substep":current.substep,
		"frame_count":current.frame_count,"player_flags":PackedByteArray([0,0]),"pad_held_triangle":false,"opening":opening_copy}

func buffer_sha(destination: int) -> String:
	return str(media.diagnostics().buffers.get(str(destination),{}).get("sha256",""))

func _initialize() -> void:
	check(media_store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")).is_empty(),"media store opens")
	check(board_store.open_pack(OS.get_environment("BOARD_PACK"),BoardStore.MANIFEST_SHA).is_empty(),"board store opens")
	check(hud.open_installed(OS.get_environment("PROLOGUE_HUD_ROOT"),"packs/prologue_hud/manifest.json",Hud.INSTALLED_PROFILE_SHA256).is_empty(),"HUD opens")
	if failures != 0:
		quit(1); return
	check(media.prepare(media_store,board_store).is_empty(),"media bridge prepares with board store")
	var initial := Cold.initialize({"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false})
	var vram := PackedByteArray(); vram.resize(Pump.VRAM_BYTES); vram.fill(0xa5)
	owned = {"scratch_owner":"render1517-publish-scratch","vram_owner":"render1517-publish-vram","scratch_words":initial.scratch_words,"vram_words":vram,"pending_gpu_effects":[]}
	check(upload.prepare(media,media_store,owned,board_store).is_empty(),"upload bridge prepares with board store")
	check(actor_renderer.prepare(media_store,upload,board_store).is_empty(),"actor renderer prepares the stages 15-17 renderer")
	if failures != 0:
		quit(1); return
	_stage0_and_1()
	if failures == 0: _stage14_commit()
	if failures == 0: _stage15_frames()
	if failures == 0: _switch_frame()
	if failures == 0: print("PASS: %d stage14-16 publish/upload checks" % checks)
	quit(1 if failures else 0)

# ---- actor and board VRAM ownership (run_first_arrival_board_load) ------

func _stage0_and_1() -> void:
	state.game_mode = 0x15; state.mode_stage = 0; state.mode_step = 1; state.substep = 1
	state.sound_active = true
	state.opening["screen_brightness"] = 128
	state.opening["prologue_line"] = 0
	state.opening["prologue_text_work"] = {"line_id":0,"pos":1,"total":1}
	state.opening["prologue_hud_live"] = {"year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"title_timer":184,
		"checker_scroll_u":31,"checker_scroll_v":255,"checker_scroll_owner":"render1517-publish",
		"screen_brightness":128,"checker_fade":255,"title_visible":false,"date_visible":true,"message_visible":true,
		"checker_scroll_phase":"before_outer_scroll_update"}
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE,0x13572468)
	check(upload.bind_session(session).is_empty(),"upload and media bind the Session")
	services = media.callbacks()
	services["arrival_draw"] = func(s: State, slot: int, out: Session.FrameResult) -> void:
		media.arrival_draw(s,slot,out,actor_renderer)
	services["outer_draw"] = func(s: State, arg: int, out: Session.FrameResult) -> void:
		var proj := _hud_projection(s)
		out.error = hud.advance_outer(proj,session.rng,{},arg)
		if out.error.is_empty(): s.opening.prologue_hud_live = proj.opening.prologue_hud_live
	session.service_completion = media.complete
	session.draw_logic = func(current: State, rng: Rng, result: Session.FrameResult) -> void:
		actor_renderer.logical_draw(current,rng,result)
	session.cd_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = upload.preflight(result,current)
	session.audio_logic = func(_state: State, _rng: Rng, _result: Session.FrameResult) -> void: pass
	session.router.bind_mode(0x15,func(current: State, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
		if fixture.is_valid():
			fixture.call(current,result)
			return
		match current.opening.get("setup_phase",0):
			0:
				media.scene_reset(current,result)
				current.opening["screen_brightness"] = 128
				media.queue.background_read(current,0x494b,1,result)
				current.opening.setup_phase = 1
			1:
				if media.queue.cd_sync(current,result) == 1:
					media.queue.background_decode(current,1,result)
					current.substep = 3
					media.queue.actor_read(current,13,0x80180000,0xabab,result)
					current.opening.setup_phase = 2
			2:
				if media.queue.cd_sync(current,result) == 1:
					current.substep = 4
					media.actor_palette_setup(current,result)
					current.opening.setup_phase = 3
			3:
				current.mode_stage = 0; current.mode_step = 2; current.substep = 3; current.mode_step_timer = 61
				current.opening["prologue_line"] = 0
				current.opening["prologue_text_work"] = {"line_id":0,"pos":1,"total":1}
				Arrival._stage_00_arrival(current,session.rng,services,result)
				current.opening.setup_phase = 4
			_:
				Arrival._stage_01_board(current,rng,services,result)
	)
	commit_until("stage0 setup",func() -> bool: return state.opening.get("setup_phase") == 4)
	commit("stage1 board read")
	commit("stage1 hold",false)
	commit_until("stage1 board decode",func() -> bool: return state.opening.get("board_media") is Dictionary and state.opening.board_media.has("upload_batch_key"))
	check(upload.batch_ready(state.opening.arrival_media.upload_batch_key).is_empty() and upload.batch_ready(state.opening.board_media.upload_batch_key).is_empty(),"Actor and board VRAM batches own the runtime")
	# Stages 14-17 draws are checked per frame below, not by logical_draw.
	session.draw_logic = func(_s: State, _rng: Rng, _result: Session.FrameResult) -> void: pass

# ---- 14/3..14/6: reads, then the case6 commit frame ---------------------

func _stage14_commit() -> void:
	fixture = func(current: State, result: Session.FrameResult) -> void:
		# Stage13 left AD05 shown in buffer 0 and AD0B in buffer 1, the O_13
		# and O_15 copies in the actor area (fixture of stages 2-13).
		for key: String in ["prologue_petals","actor96","actor96_draw_flags","actor96_source_draw"]: current.opening.erase(key)
		current.mode_stage = 14; current.mode_step = 0; current.substep = 4
		current.opening["names_media"] = {"bg_show_1":0,"active_draw":"dec_bg_draw","buffer_0":{"lba":0xad05},"buffer_1":{"lba":0xad0b}}
		current.opening["bg_brightness"] = 0x80
		current.opening["actor_area"] = [{"resource_id":BoardStore.O_13_ID,"sha256":BoardStore.O_13_SHA,"bytes":0x20800},{"resource_id":BoardStore.O_15_ID,"sha256":BoardStore.O_15_SHA,"bytes":0x10800}]
		media.queue.background_read(current,0xacec,1,result)
	commit("14/3 ACEC read")
	fixture = func(current: State, result: Session.FrameResult) -> void:
		if media.queue.cd_sync(current,result) == 1:
			media.queue.actor_read(current,0x31,0x801a4000,0xab7a,result)
			current.substep = 5
	commit_until("14/4 CUT_16 read",func() -> bool: return state.substep == 5)
	fixture = func(current: State, result: Session.FrameResult) -> void:
		if media.queue.cd_sync(current,result) == 1:
			media.queue.actor_read(current,0x19,0x801e8000,0xacbb,result)
			current.substep = 6
	commit_until("14/5 O_16 read",func() -> bool: return state.substep == 6)
	check(buffer_sha(0x801e8000) == BoardStore.O_16_SHA and buffer_sha(0x801a4000) == BoardStore.CUT_16_SHA and buffer_sha(0x80180000) != BoardStore.O_16_SHA,"O_16/CUT_16 reads did not publish to 0x801E8000/0x801A4000")
	fixture = func(current: State, result: Session.FrameResult) -> void:
		# A tint left by an earlier stage; the slot set writes 0x80.
		current.opening["D_801220E0"] = 0x40; current.opening["D_801220E1"] = 0xa0; current.opening["D_801220E2"] = 0xff
		# Case6 polls cd_read_sync_status until the O_16 read lands.
		if media.queue.cd_sync(current,result) != 1: return
		if media.background_decode(current,1,result) != 1: return
		media.shiori_effect(current,"line19_setup",0,result)
		if not result.error.is_empty(): return
		current.next_stage()
		media.shiori_effect(current,"actor_draw",0,result)
	var r := commit_until("14/6 commit frame",func() -> bool: return state.mode_stage == 15)
	var kinds: Array = r.presentation.map(func(i): return i.kind).filter(func(k): return k.begins_with("first_arrival_"))
	# Stage14 case6: the ACEC decode into buffer 1 (dec_bg_show[1] was 0),
	# the CUT_16 slot set, the O_16 memcpy and this frame's search_tpage pages.
	check(kinds == ["first_arrival_names_bg_upload","first_arrival_cut_upload","first_arrival_shiori_copy","first_arrival_stage_actor_upload","first_arrival_stage_actor_upload"],"14/6 upload intents differ: " + str(kinds))
	check(state.mode_stage == 15 and state.substep == 0,"14/6 did not switch to 15/0")
	check(buffer_sha(0x80180000) == BoardStore.O_16_SHA,"14/6 publish did not copy O_16 from 0x801E8000 to 0x80180000")
	check(upload.batch_ready(state.opening.cut_media.upload_batch_key).is_empty() and state.opening.bg_layer_0.cut_batch_key == state.opening.cut_media.upload_batch_key,"CUT_16 slot-set upload is not ready")
	check(state.opening.names_media.bg_show_1 == 1 and state.opening.names_media.buffer_1.lba == 0xacec and upload.batch_ready(state.opening.names_media.buffer_1.upload_batch_key).is_empty(),"ACEC buffer-1 upload is not ready")
	check([state.opening.D_801220E0,state.opening.D_801220E1,state.opening.D_801220E2] == [128,128,128],"14/6 slot set kept a stale layer tint")
	# Same frame without the copy's read, or at another substep: refused.
	var negative := Session.FrameResult.new()
	negative.presentation = [{"kind":"first_arrival_shiori_copy","resource_id":BoardStore.O_16_ID,"sha256":BoardStore.O_16_SHA,"bytes":0xc800,"source":0x801e8000}]
	check(media.preflight(negative,state).is_empty(),"O_16 memcpy control at 15/0 refused")
	_with_state(func() -> void: state.substep = 1,func() -> void:
		check(media.preflight(negative,state) == Media.SHIORI_COPY_FRAME_REFUSED,"O_16 memcpy accepted at 15/1"))
	_with_state(func() -> void: state.mode_stage = 14; state.substep = 6,func() -> void:
		check(media.preflight(negative,state) == Media.SHIORI_COPY_FRAME_REFUSED,"O_16 memcpy accepted before stage14's next_stage"))
	var o16_e8 := {"kind":"first_arrival_e8_copy","resource_id":BoardStore.O_16_ID,"sha256":BoardStore.O_16_SHA,"bytes":0xc800}
	check(not media._intent_schema(o16_e8).is_empty(),"O_16 accepted as a stage15 e8 copy; stage14's memcpy has one owner")
	var o07 := Session.FrameResult.new()
	o07.presentation = [{"kind":"first_arrival_e8_copy","resource_id":BoardStore.O_06_ID,"sha256":BoardStore.O_06_SHA,"bytes":0xc800}]
	check(media.preflight(o07,state) == E8_REFUSED,"O_07 e8 copy accepted at 15/0 with O_16 at 0x801E8000")
	# The commit frame draws through stage14's ShioriScene from the owned
	# ACEC, CUT_16 and O_16 uploads.
	var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var painted := actor_renderer.paint_shiori_scene(target,state)
	check(painted.is_empty(),"Stage14 commit frame did not draw through the real path: " + painted)

static func _put(img: Image, x: int, y: int, c: int) -> void:
	var v := [c&31,(c>>5)&31,(c>>10)&31]
	if (c&0x8000) != 0 and img.get_pixel(x,y).a8 != 0:
		var back := img.get_pixel(x,y)
		v = [((back.r8>>3)+v[0])>>1,((back.g8>>3)+v[1])>>1,((back.b8>>3)+v[2])>>1]
	img.set_pixel(x,y,Color8((v[0]<<3)|(v[0]>>2),(v[1]<<3)|(v[1]>>2),(v[2]<<3)|(v[2]>>2),255))

func _reference(s: State) -> Image:
	# 16/0 from resource bytes only: ACD4 words, CUT_07.NBN cells and CLUT,
	# O_07.NBN frames of the actor_draw record with the O_01 CLUT rows.
	var img := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var words: PackedByteArray = board_store.background_software_upload(BoardStore.BG_ACD4_ID).words
	for y in 160:
		for x in 320:
			var c := words.decode_u16((y*320+x)*2)
			img.set_pixel(x,y,Color8(((c&31)<<3)|((c&31)>>2),(((c>>5)&31)<<3)|(((c>>5)&31)>>2),(((c>>10)&31)<<3)|(((c>>10)&31)>>2),255))
	var cut: PackedByteArray = board_store.stage_transfer(BoardStore.CUT_06_ID).bytes
	var pal := Pump._mode2_stp(cut.slice(0x18000,0x18200))
	for y in 160:
		for x in 320:
			@warning_ignore("integer_division")
			var page := (x/128)+(3 if y >= 128 else 0)
			var c := pal.decode_u16(cut[page*0x4000+(y%128)*128+(x%128)]*2)
			if c != 0: _put(img,x,y,c)
	var o01: PackedByteArray = media_store.transfer(Store.ACTOR_ID).bytes
	var bytes: PackedByteArray = board_store.stage_transfer(BoardStore.O_06_ID).bytes
	var order: Array = s.opening.o07_tpages.drawn.duplicate()
	order.sort_custom(func(a, b): return [int(a.unk_01),int(a.index)] > [int(b.unk_01),int(b.index)])
	for a: Dictionary in order:
		var tpage_table: int = s.opening["actor%d" % a.index].tpage_table
		var offset := int(bytes.decode_u32(int(a.frame_table)-0x80180000+int(a.frame)*4))-0x80180000
		for part in bytes.decode_s16(offset):
			var row := offset+8+part*14
			var texture := bytes.decode_u16(row+4)
			var page := int(bytes.decode_u32(tpage_table-0x80180000+(texture>>6)*4))-0x80180000
			var clut_at := 0x4490+((bytes.decode_u16(row+6)>>8)&15)*512
			var clut := Pump._mode2_stp(o01.slice(clut_at,clut_at+512))
			for y in bytes.decode_s16(row+12):
				for x in bytes.decode_s16(row+10):
					var px: int = 160+int(a.x)+bytes.decode_s16(row)+x
					var py: int = 120+int(a.y)+bytes.decode_s16(row+2)+y
					if px < 0 or px >= 320 or py < 0 or py >= 240: continue
					var c := clut.decode_u16(bytes[page+(((texture>>3)&7)*16+y)*128+(texture&7)*16+x]*2)
					if c != 0: _put(img,px,py,c)
	return img

func _with_state(change: Callable, body: Callable) -> void:
	# Test-only edit of the owned state around a read-only preflight call.
	var saved := state.projection()
	var before := session.state_hash()
	change.call()
	body.call()
	state.restore(saved)
	check(session.state_hash() == before,"state was not restored after a negative")

# ---- stage15 page uploads on the published O_16 -----------------------

func _stage15_frames() -> void:
	fixture = func(current: State, result: Session.FrameResult) -> void:
		media.stages_15_17.effect(current,"slide_in",0,result)
		if result.error.is_empty(): current.substep = 1
		if result.error.is_empty(): media.stages_15_17.effect(current,"actor_draw",0,result)
	var r := commit("15/0 first stage15 actor_draw")
	var pages: Array = r.presentation.filter(func(i): return i.kind == "first_arrival_stage_actor_upload").map(func(i): return [i.resource_id,i.slot,i.source_offset])
	check(pages == [[BoardStore.O_16_ID,5,0x82c0],[BoardStore.O_16_ID,6,0x42c0]],"15/0 page uploads differ: " + str(pages))
	for intent: Dictionary in r.presentation.filter(func(i): return i.kind == "first_arrival_stage_actor_upload"):
		check(upload.batch_ready(intent.batch_key).is_empty(),"O_16 page 0x%x is not ready" % intent.source_offset)
	check(buffer_sha(0x80180000) == BoardStore.O_16_SHA,"0x80180000 lost O_16")
	# An O_07 page with no same-frame memcpy and O_16 resident: refused.
	var foreign := Session.FrameResult.new()
	foreign.presentation = [{"kind":"first_arrival_stage_actor_upload","batch_key":"foreign".sha256_text(),"resource_id":BoardStore.O_06_ID,"slot":5,"source_offset":0x4310}]
	check(media.preflight(foreign,state) == NO_RESIDENT,"O_07 page upload without its copy accepted: " + media.preflight(foreign,state))
	check(upload.preflight(foreign,state) == NO_RESIDENT,"Upload bridge accepted an O_07 page without its copy")
	# Lip sync on line 0x14 puts the mouth page 0x2C0 into slot 7.
	fixture = func(current: State, result: Session.FrameResult) -> void:
		current.opening["prologue_line"] = 0x14
		AudioRules.begin_line_lip_sync(current,0x14,0,[[30,30,30,30]])
		current.opening["cd_audio_state_override"] = 4
		AudioRules.step_lip_sync(current)
		media.stages_15_17.effect(current,"lip_sync",1,result)
		current.opening.erase("cd_audio_state_override")
		if result.error.is_empty(): media.stages_15_17.effect(current,"actor_draw",0,result)
	r = commit("15/1 lip frame")
	var lip: Array = r.presentation.filter(func(i): return i.kind == "first_arrival_stage_actor_upload")
	check(lip.map(func(i): return [i.resource_id,i.slot,i.source_offset]) == [[BoardStore.O_16_ID,7,0x2c0]],"Lip frame pages differ: " + str(lip.map(func(i): return [i.slot,i.source_offset])))
	check(lip.size() == 1 and upload.batch_ready(lip[0].batch_key).is_empty(),"Mouth page upload is not ready")
	# The same page key again is an executed batch.
	if lip.size() == 1:
		var replay := Session.FrameResult.new(); replay.presentation = [lip[0]]
		check(not upload.preflight(replay,state).is_empty(),"Executed page batch accepted again")
	# 15/10: actor100 at y -0x38 (rows 160-223) with the checker at its OT 9.
	# Through the bound Session its pages are owned, and the production
	# renderer splits its message-area rows into the under-checker layer.
	fixture = func(current: State, result: Session.FrameResult) -> void:
		current.substep = 4
		media.stages_15_17.effect(current,"actor99_set",0,result)
		current.substep = 10
		if result.error.is_empty(): media.stages_15_17.effect(current,"actor100_set",0,result)
		if result.error.is_empty(): media.stages_15_17.effect(current,"actor_draw",0,result)
	r = commit("15/10 actor100 rise start")
	check(state.opening.get("D_800B5BD4") == 9 and state.opening.actor100.y == -0x38,"15/10 actor100/checker setup differs")
	var under := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var under_error := actor_renderer.paint_under_checker(under,state)
	var scene := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var scene_error := actor_renderer.paint(scene,state,Vector2i(160,120))
	check(under_error.is_empty() and scene_error.is_empty(),"15/10 message-area actor rows were refused: %s / %s" % [under_error,scene_error])
	var under_rows := [999,-1]
	for y in 240:
		for x in 320:
			if under.get_pixel(x,y).a8 != 0: under_rows = [mini(under_rows[0],y),maxi(under_rows[1],y)]
	check(under_rows[0] >= 160 and under_rows[1] <= 239,"Under-checker layer drew outside the message area: " + str(under_rows))

# ---- 15/31..15/34 and the switch frame -----------------------------------

func _switch_frame() -> void:
	fixture = func(current: State, result: Session.FrameResult) -> void:
		current.substep = 31
		media.queue.background_read(current,0xacd4,1,result)
		current.substep = 32
	commit("15/31 ACD4 read")
	fixture = func(current: State, result: Session.FrameResult) -> void:
		if media.queue.cd_sync(current,result) == 1:
			media.queue.actor_read(current,0x31,0x801a4000,0xaa54,result)
			current.substep = 33
	commit_until("15/32 CUT_07 read",func() -> bool: return state.substep == 33)
	fixture = func(current: State, result: Session.FrameResult) -> void:
		if media.queue.cd_sync(current,result) == 1:
			media.queue.actor_read(current,0x19,0x801e8000,0xabca,result)
			current.substep = 34
	commit_until("15/33 O_07 read",func() -> bool: return state.substep == 34)
	check(buffer_sha(0x801e8000) == BoardStore.O_06_SHA and buffer_sha(0x80180000) == BoardStore.O_16_SHA,"O_07 read did not publish beside the resident O_16")
	var built: Array = []
	fixture = func(current: State, result: Session.FrameResult) -> void:
		current.opening["D_801220E0"] = 0x40; current.opening["D_801220E1"] = 0xa0; current.opening["D_801220E2"] = 0xff
		# Case34 polls cd_read_sync_status until the O_07 read lands.
		if media.queue.cd_sync(current,result) != 1: return
		if media.background_decode(current,1,result) != 0: return
		media.stages_15_17.effect(current,"next_scene",0,result)
		if not result.error.is_empty(): return
		current.next_stage()
		media.arrival_draw(current,1,result,actor_renderer)
		if not result.error.is_empty(): return
		media.stages_15_17.effect(current,"actor_draw",0,result)
		# The frame as the cd_logic preflight will see it.
		built.assign(result.presentation.duplicate(true))
	# The switch frame without its e8 copy is refused before commit.
	var held := session.state_hash()
	session.draw_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void:
		if not built.is_empty():
			var stripped := Session.FrameResult.new()
			stripped.presentation.assign(result.presentation.filter(func(i): return i.kind != "first_arrival_e8_copy"))
			var error := media.preflight(stripped,current)
			if error != NO_RESIDENT: result.error = "switch frame without its e8 copy was not refused: " + error
	var r := commit_until("15/34 -> 16/0 switch frame",func() -> bool: return state.mode_stage == 16)
	session.draw_logic = func(_s: State, _rng: Rng, _result: Session.FrameResult) -> void: pass
	check(held != session.state_hash(),"Switch frame did not commit")
	var uploads: Array = r.presentation.filter(func(i): return str(i.kind).begins_with("first_arrival_") and i.kind != "first_arrival_background_draw").map(func(i): return [i.kind,i.get("resource_id"),i.get("slot",i.get("buffer"))])
	check(uploads == [["first_arrival_names_bg_upload",BoardStore.BG_ACD4_ID,0],["first_arrival_cut_upload",BoardStore.CUT_06_ID,null],["first_arrival_e8_copy",BoardStore.O_06_ID,null],
		["first_arrival_stage_actor_upload",BoardStore.O_06_ID,5],["first_arrival_stage_actor_upload",BoardStore.O_06_ID,6],["first_arrival_stage_actor_upload",BoardStore.O_06_ID,7]],"Switch-frame intents differ: " + str(uploads))
	check(state.mode_stage == 16 and state.substep == 0,"Switch frame did not reach 16/0")
	check(buffer_sha(0x80180000) == BoardStore.O_06_SHA,"Switch publish did not copy O_07 from 0x801E8000 to 0x80180000")
	check([state.opening.D_801220E0,state.opening.D_801220E1,state.opening.D_801220E2] == [128,128,128],"15/34 slot set kept a stale layer tint")
	for intent: Dictionary in r.presentation:
		if intent.kind in ["first_arrival_names_bg_upload","first_arrival_cut_upload","first_arrival_stage_actor_upload"]:
			check(upload.batch_ready(intent.batch_key).is_empty(),"Switch-frame %s batch is not ready" % intent.kind)
	# The published frame paints from the upload owner's VRAM.
	var img := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var error := actor_renderer.paint_names_background(img,state)
	if error.is_empty(): error = actor_renderer.paint(img,state,Vector2i(160,120))
	check(error.is_empty(),"Switch frame paint refused: " + error)
	var bare := Image.create(320,240,false,Image.FORMAT_RGBA8)
	check(actor_renderer.paint_names_background(bare,state).is_empty() and img.get_data() != bare.get_data(),"Switch-frame actors left no pixels")
	check(img.get_data() == _reference(state).get_data(),"Switch frame from the upload owner's VRAM differs from the resource-byte reference")
	# e8 copy of O_07 at 16/1 (after its frame) is refused.
	var late := Session.FrameResult.new()
	late.presentation = [{"kind":"first_arrival_e8_copy","resource_id":BoardStore.O_06_ID,"sha256":BoardStore.O_06_SHA,"bytes":0xc800}]
	_with_state(func() -> void: state.substep = 1,func() -> void:
		check(media.preflight(late,state) == E8_REFUSED,"O_07 e8 copy accepted at 16/1"))
