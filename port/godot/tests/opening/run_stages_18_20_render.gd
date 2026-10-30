extends SceneTree
## Stages 18-20 rendering (wiki/spec/opening-stages-18-20.md) on original
## bytes. BOARD_PACK gives P_01_12's decoded words and O_13;
## FIRST_ARRIVAL_MEDIA_ROOT/SHA gives the store the stage0 O_01 batch is
## validated against; O01_FILE (disc/files/CDROM/PROLODIR/O_01.NBN,
## read-only) gives the raw palette bytes the reference CLUT is built from.
## VRAM is filled only by the native GPU pump: P_01_12, the O_13 pages and
## the whole stage0 O_01 batch (producer CPU queue, then pump.execute, which
## writes CLUT row 0x1FB from load_palette row 11 and keeps a receipt).
## References are built from the source bytes with the source rules written
## out here, never read back from VRAM or from the pump's helpers.
## _bridge_chain drives the production path for stage19: Flow.step through a
## Session, the real media bridge, the upload bridge's preflight/publish of
## the O_13 page uploads, and first_arrival_actor_renderer.paint_closing.
## The stage-17 exit state is built from its source-defined records, as
## run_stages_18_20.gd does.
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Closing = preload("res://presentation/closing_scene_renderer.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const AdditiveFlash = preload("res://presentation/additive_flash.gd")
const Producer = preload("res://presentation/prologue_actor_upload_provider.gd")
const Cold = preload("res://flow/cold_no_card_scratch.gd")
const Flow = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const TextPainter = preload("res://presentation/prologue_text_presenter.gd")
const Presenter = preload("res://presentation/first_arrival_scene_presenter.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const BG_KEY := "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
const O01_KEY := "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
const BOARD_KEY := "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
const BACK := Color8(10,20,30)
var failures := 0
var checks := 0
var board := Board.new()
var store := Store.new()
var pump := Pump.new()
var owned: Dictionary
var o13 := PackedByteArray()
var clut := PackedByteArray()
var bg_words := PackedByteArray()

class Uploads:
	var pump
	var owned: Dictionary
	var o01_key := ""
	var refuse := ""
	func batch_ready(key: String) -> String:
		if not refuse.is_empty(): return refuse
		if key == o01_key: return pump.batch_ready(key,owned)
		if pump._names_bg_receipts.has(key): return pump.names_bg_batch_ready(key,owned)
		return pump.stage_actor_batch_ready(key,owned)

class Painter:
	var calls := 0
	func paint_closing(_target: Image, _state) -> String:
		calls += 1
		return ""

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func _entry() -> State:
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 18; s.mode_step = 0; s.substep = 1; s.frame_count = 3000
	s.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0x496a},"active_draw":"dec_bg_draw",
		"buffer_1":{"lba":0x4973,"resource_id":Board.BG_4973_ID,"transfer_sha256":Board.BG_4973_TRANSFER_SHA,"word_sha256":Board.BG_4973_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":BG_KEY}}
	s.opening["bg_layer_0"] = {"attribute":0x80000000,"enabled":0}
	s.opening["arrival_media"] = {"actor_initialized":true,"upload_batch_key":O01_KEY}
	s.opening["bg_brightness"] = 128
	s.opening["screen_brightness"] = 128
	s.opening["media"] = {"epoch":5,"actor_pending":{str(0x80180000):"o13"},"requests":{
		"o13":{"request_id":"o13","epoch":5,"status":"ready","kind":"actors","resource_id":Board.O_13_ID,"resource_sha256":Board.O_13_SHA,"payload":{"lba":0xac50,"sectors":0x41,"destination":0x80180000}}}}
	return s

static func _copy(s: State) -> State:
	var c := State.new()
	c.game_mode = s.game_mode; c.mode_stage = s.mode_stage; c.mode_step = s.mode_step; c.substep = s.substep
	c.frame_count = s.frame_count; c.rule_state = s.rule_state; c.opening = s.opening.duplicate(true)
	return c

func _effect(media: Media, s: State, kind: String, value: int = 0) -> Session.FrameResult:
	var result := Session.FrameResult.new()
	media.closing_effect(s,kind,value,result)
	return result

func _pump_uploads(result: Session.FrameResult) -> int:
	var count := 0
	for intent: Dictionary in result.presentation:
		if intent.kind != "first_arrival_stage_actor_upload": continue
		var done := pump.execute_stage_actor_vram(board,intent.resource_id,intent.slot,intent.source_offset,intent.batch_key,owned)
		check(done.error.is_empty(),"O_13 page upload rejected: " + str(done.error))
		count += 1
	return count

static func _cold_scratch() -> PackedByteArray:
	var route := {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false}
	return Cold.initialize(route).scratch_words

func _o01_batch(o01: PackedByteArray, runtime: Dictionary, bridge: Upload, gpu, key: String) -> void:
	# Stage0 prologue_actor_palette_setup (prologue.c:250-262), in the upload
	# bridge's publish order: the producer queues load_tpage_buf_lock,
	# sixteen load_palette rows (mode 2) and the gray row, the bridge checks
	# the queue extents, then the pump runs palette_load_vram's STP pass and
	# the copies.
	runtime["actor_runtime_bytes"] = o01.duplicate()
	var producer := Producer.new()
	var plan := producer.preflight(o01,runtime)
	check(plan.get("status") == "preflight_ok","O_01 producer preflight failed: " + str(plan.get("diagnostic")))
	var produced := producer.execute(plan,o01,runtime)
	check(produced.get("status") == "effects_enqueued","O_01 producer queue failed: " + str(produced.get("diagnostic")))
	var extents := bridge._pump_extents(runtime)
	check(extents.is_empty(),"O_01 queue extents rejected: " + extents)
	var done: Dictionary = gpu.execute(store,key,runtime)
	check(done.error.is_empty(),"O_01 batch rejected by the pump: " + str(done.error))

static func _clut_reference(o01: PackedByteArray) -> PackedByteArray:
	# load_palette(0x80184490, 16+11, 1, 2, 11) (prologue.c:258-260) queues
	# row 11: y = (27 & 31) + 0x1E0 = 0x1FB, first = 11 << 7 words. Then
	# palette_load_vram case 2 (frame_render_audio.c:296-313) on u32 words:
	# words 0..119 clear both STP bits; words 120..127 set the STP bit of
	# each half whose RGB is nonzero.
	var row := o01.slice(0x4490+11*512,0x4490+12*512)
	for j in 128:
		var w := row.decode_u32(j*4)
		if j < 120:
			w &= 0x7fff7fff
		else:
			if (w & 0x7fff0000) != 0: w |= 0x80000000
			if (w & 0x7fff) != 0: w |= 0x8000
		row.encode_u32(j*4,w)
	return row

static func _rgb(r: int, g: int, b: int) -> Color:
	return Color8((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2))

func _background(target: Image, brightness: int) -> void:
	for y in 160:
		for x in 320:
			var c := bg_words.decode_u16((y*320+x)*2)
			target.set_pixel(x,y,_rgb(mini(31,((c&31)*brightness)>>7),mini(31,(((c>>5)&31)*brightness)>>7),mini(31,(((c>>10)&31)*brightness)>>7)))

func _actor(target: Image, frame: int, tint: int = 128) -> void:
	# Reference from the O_13 resource itself: frame table +0x204, page table
	# +0x1E4, parts drawn 0..count-1, 8-bit texels through CLUT row 11, each
	# texel modulated by draw_brightness_get() (draw_flags 0x20), then the
	# semi-transparent texels averaged with the background.
	var offset := int(o13.decode_u32(0x204+frame*4))-0x80180000
	for part in o13.decode_s16(offset):
		var row := offset+8+part*14
		var texture := o13.decode_u16(row+4)
		var page := int(o13.decode_u32(0x1e4+(texture>>6)*4))-0x80180000
		var u := (texture&7)*16; var v := ((texture>>3)&7)*16
		for y in o13.decode_s16(row+12):
			for x in o13.decode_s16(row+10):
				var px := o13.decode_s16(row)+x; var py := o13.decode_s16(row+2)+y
				if px < 0 or px >= 320 or py < 0 or py >= 240: continue
				var color := clut.decode_u16(o13[page+(v+y)*128+u+x]*2)
				if color == 0: continue
				var r := mini(31,((color&31)*tint)>>7); var g := mini(31,(((color>>5)&31)*tint)>>7); var b := mini(31,(((color>>10)&31)*tint)>>7)
				if (color&0x8000) != 0:
					var back := target.get_pixel(px,py)
					r = ((back.r8>>3)+r)>>1; g = ((back.g8>>3)+g)>>1; b = ((back.b8>>3)+b)>>1
				target.set_pixel(px,py,_rgb(r,g,b))

func _want(white: bool, actor_frame: int, flash: int, tint: int = 128) -> Image:
	var want := Image.create(320,240,false,Image.FORMAT_RGBA8); want.fill(BACK); _background(want,128)
	if white: want.fill_rect(Rect2i(0,0,320,160),Color8(255,255,255))
	if actor_frame >= 0: _actor(want,actor_frame,tint)
	if flash >= 0: AdditiveFlash.paint(want,flash)
	return want

func _frame(state: State, renderer: Closing, upload: Uploads) -> Dictionary:
	var image := Image.create(320,240,false,Image.FORMAT_RGBA8)
	image.fill(BACK)
	var error := renderer.paint(image,state,owned,upload)
	return {"error":error,"image":image}

static func _flash_level(s: State) -> int:
	# The level the renderer draws for this frame's flash record.
	var color: int = s.opening.prologue_closing_draws[-1].color
	return 255 if color == 0xffffff else color & 0xff

func _initialize() -> void:
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if error.is_empty(): error = store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA"))
	if not error.is_empty():
		printerr(error); quit(1); return
	var o01 := FileAccess.get_file_as_bytes(OS.get_environment("O01_FILE"))
	if Store._sha(o01) != Store.ACTOR_SHA:
		printerr("O01_FILE is not the original O_01.NBN"); quit(1); return
	check(store.transfer(Store.ACTOR_ID).get("bytes") == o01,"Media store O_01 differs from the disc file")
	o13 = board.stage_transfer(Board.O_13_ID).bytes
	var upload_desc := board.background_software_upload(Board.BG_4973_ID)
	var transfer := board.background_transfer(Board.BG_4973_ID)
	bg_words = upload_desc.words
	check(transfer.error.is_empty() and transfer.lba == 0x4973 and transfer.sectors == 10 and transfer.destination == 0x80162000 and Store._sha(transfer.bytes) == Board.BG_4973_TRANSFER_SHA,"P_01_12 10-sector transfer differs")
	check(upload_desc.error.is_empty() and upload_desc.buffer == 0 and upload_desc.rectangle == [704,0,320,160] and Store._sha(bg_words) == Board.BG_4973_WORD_SHA and bg_words.size() == 102400,"P_01_12 decode descriptor differs")
	check(board.background_descriptions().get(str(0x4973),{}).get("id") == Board.BG_4973_ID,"P_01_12 read is not described for the media queue")
	var pages: Array = []
	for k in 8: pages.append(int(o13.decode_u32(0x1e4+k*4))-0x80180000)
	check(pages == Board.O13_PAGE_OFFSETS,"O_13 page table differs from O13_PAGE_OFFSETS: " + str(pages))

	# --- Pump: the stage0 O_01 batch (CLUT row 0x1FB), P_01_12 into buffer 1, O_13 pages.
	var pixels := PackedByteArray(); pixels.resize(Pump.VRAM_BYTES); pixels.fill(0xa5)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	owned = {"vram_owner":"closing-test","scratch_owner":"closing-scratch","vram_words":pixels,"vram_known_mask":mask,"scratch_words":_cold_scratch(),"pending_gpu_effects":[]}
	var extent_media := Media.new()
	var extent_bridge := Upload.new()
	check(extent_media.prepare(store,board).is_empty() and extent_bridge.prepare(extent_media,store,owned,board).is_empty(),"Upload bridge for the O_01 extents did not prepare")
	_o01_batch(o01,owned,extent_bridge,pump,O01_KEY)
	clut = _clut_reference(o01)
	check(Pump._region(owned.vram_words,Rect2i(0,0x1fb,256,1)) == clut and Pump._region(owned.vram_known_mask,Rect2i(0,0x1fb,256,1)).count(255) == 512,"Pump CLUT row 0x1FB differs from load_palette row 11 after the mode-2 STP pass")
	check(clut != o01.slice(0x4490+11*512,0x4490+12*512),"Mode-2 STP pass left row 11 unchanged; the CLUT check proves nothing")
	check(pump.batch_ready(O01_KEY,owned).is_empty(),"O_01 batch has no receipt")
	# The decode buffer is 1 - dec_bg_show[1] at runtime (buffer 0 on the
	# uninterrupted path, 1 here as after a stage16/17 dec_bg_reset); no third.
	check(not pump.execute_names_bg(board,Board.BG_4973_ID,"wrong-buffer".sha256_text(),owned,2).error.is_empty(),"P_01_12 accepted into buffer 2")
	check(pump.execute_names_bg(board,Board.BG_4973_ID,BG_KEY,owned,1).error.is_empty(),"P_01_12 upload rejected")
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(320,256,320,160))) == Board.BG_4973_WORD_SHA,"P_01_12 words differ at buffer 1")
	const BAD := "native GPU stage actor pump received an unsupported resource/slot/offset combination"
	check(pump.execute_stage_actor_vram(board,Board.O_13_ID,7,0x230,"o13-slot7".sha256_text(),owned).error == BAD,"O_13 page accepted in slot 7")
	check(pump.execute_stage_actor_vram(board,Board.O_13_ID,5,0x231,"o13-off".sha256_text(),owned).error == BAD,"O_13 accepted an offset outside its page table")
	var upload := Uploads.new(); upload.pump = pump; upload.owned = owned; upload.o01_key = O01_KEY

	var renderer := Closing.new()
	check(renderer.prepare(o13.slice(0,0x20000)) == "stage19 renderer requires the original O_13 bytes","Closing renderer accepted truncated O_13")
	var tampered := o13.duplicate(); tampered.encode_u16(0x8+6,0x1a00)
	check(renderer.prepare(tampered) == "stage19 renderer requires the original O_13 bytes","Closing renderer accepted modified O_13")
	check(renderer.prepare(o13).is_empty(),"Closing renderer rejected original O_13")

	var media := Media.new()
	media._board_store = board
	media._names_bg_uploads[BG_KEY] = {"batch_key":BG_KEY}

	# --- Stage 18: P_01_12 only, tinted by bg_brightness; HUD rows untouched.
	var s := _entry()
	var drawn := _frame(s,renderer,upload)
	var want := _want(false,-1,-1)
	check(drawn.error.is_empty() and drawn.image.get_data() == want.get_data(),"Stage18 P_01_12 frame differs: " + drawn.error)
	var preview := board.background_preview(Board.BG_4973_ID); preview.convert(Image.FORMAT_RGBA8)
	check(drawn.image.get_region(Rect2i(0,0,320,160)).get_data() == preview.get_data(),"Stage18 frame differs from the exported P_01_12 decode")
	s.opening.bg_brightness = 0x40
	drawn = _frame(s,renderer,upload); want.fill(BACK); _background(want,0x40)
	check(drawn.error.is_empty() and drawn.image.get_data() == want.get_data(),"Stage18 bg_brightness tint differs")
	s.opening.bg_brightness = 128
	# The uninterrupted route decodes P_01_12 into buffer 0 (stage16 showed
	# P_01_10 in buffer 1); dec_bg_draw(1) reads whichever buffer is shown.
	var bg_key0 := "b0".sha256_text()
	check(pump.execute_names_bg(board,Board.BG_4973_ID,bg_key0,owned,0).error.is_empty(),"P_01_12 upload into buffer 0 rejected")
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(704,0,320,160))) == Board.BG_4973_WORD_SHA,"P_01_12 words differ at buffer 0")
	var s0 := _entry()
	s0.opening.names_media = {"bg_show_1":0,"buffer_1":{"lba":0x496a},"active_draw":"dec_bg_draw",
		"buffer_0":{"lba":0x4973,"resource_id":Board.BG_4973_ID,"transfer_sha256":Board.BG_4973_TRANSFER_SHA,"word_sha256":Board.BG_4973_WORD_SHA,"decode_rect":[704,0,320,160],"upload_batch_key":bg_key0}}
	var drawn0 := _frame(s0,renderer,upload)
	check(drawn0.error.is_empty() and drawn0.image.get_data() == _frame(s,renderer,upload).image.get_data(),"Stage18 P_01_12 from buffer 0 differs from buffer 1: " + drawn0.error)
	var s0_rect := _copy(s0); s0_rect.opening.names_media.buffer_0.decode_rect = [320,256,320,160]
	check(_frame(s0_rect,renderer,upload).error == "closing background is not the uploaded P_01_12 buffer","Renderer accepted buffer 0 at buffer 1's rectangle")
	# Owners and VRAM: each guard is exercised once.
	var neg := _entry(); neg.opening.names_media.bg_show_1 = 0
	check(_frame(neg,renderer,upload).error == "closing background is not the uploaded P_01_12 buffer","Renderer drew the hidden buffer")
	neg = _entry(); neg.opening.names_media.buffer_1.upload_batch_key = "x".sha256_text()
	check(_frame(neg,renderer,upload).error == "native GPU stage actor batch has no executed receipt","Renderer drew without the P_01_12 receipt")
	upload.refuse = "refused"; check(_frame(_entry(),renderer,upload).error == "refused","Renderer ignored upload readiness"); upload.refuse = ""
	neg = _entry(); neg.opening.bg_brightness = 256
	check(_frame(neg,renderer,upload).error == "closing background brightness is not a native byte","Renderer accepted bg_brightness 256")
	owned.vram_known_mask[((256+5)*1024+320+7)*2] = 0
	check(_frame(_entry(),renderer,upload).error == "native GPU names background decode destination changed after readback","Renderer drew partly unknown P_01_12 VRAM")
	owned.vram_known_mask[((256+5)*1024+320+7)*2] = 255
	neg = _entry(); neg.opening["prologue_closing_draws"] = [media._closing_primitive(0x808080,0xb,0x81)]
	neg.opening.prologue_closing_draws[0]["draw_mode"] = {"tag":0xb,"abr":1,"tp":1}; neg.opening["prologue_scene_counter"] = 0x84
	check(_frame(neg,renderer,upload).error == "closing boxes are outside stage19","Renderer drew flash boxes in stage18")
	neg.mode_stage = 20
	check(_frame(neg,renderer,upload).error == "closing boxes are outside stage19","Renderer drew flash boxes in stage20")

	# --- Stage 19 substep 0: additive dithered flash over P_01_12.
	s = _entry(); s.mode_stage = 19; s.substep = 0; s.opening["prologue_scene_counter"] = 0
	drawn = _frame(s,renderer,upload)
	check(drawn.error.is_empty() and drawn.image.get_data() == _want(false,-1,-1).get_data(),"Stage19 entry frame (no boxes yet) differs: " + drawn.error)
	s.opening.prologue_scene_counter = 0x80
	var flash := _effect(media,s,"flash",0x80)
	check(flash.error.is_empty(),"Flash record rejected")
	s.opening.prologue_scene_counter = 0x84
	drawn = _frame(s,renderer,upload)
	check(drawn.error.is_empty() and drawn.image.get_data() == _want(false,-1,0x80).get_data(),"Stage19 additive flash differs: " + drawn.error)
	check(drawn.image.get_pixel(5,200) == BACK,"Flash reached the HUD rows")
	s.opening.prologue_scene_counter = 0x80
	check(_frame(s,renderer,upload).error == "closing flash boxes are not owned by this committed frame","Stale flash record drawn")
	s.opening.prologue_scene_counter = 0x84; s.opening.prologue_closing_draws[0].color = 0x808081
	check(_frame(s,renderer,upload).error == "closing flash colour is not counter * 0x10101","Flash colour outside counter*0x10101 drawn")
	s.opening.prologue_closing_draws[0].color = 0x808080; s.opening.prologue_closing_draws[0].draw_mode.abr = 0
	check(_frame(s,renderer,upload).error == "closing flash boxes differ from prologue_flash_boxes_draw","Flash without ABR1 drawn")
	s.opening.erase("prologue_closing_draws")
	check(_frame(s,renderer,upload).error == "stage19 frame lacks its flash boxes","Stage19 frame without boxes drawn")

	# --- Stage 19 case0 exit (the peak): flash at 252, counter 0x100,
	# actor_setup, substep 1, then draw_frame's actor_draw_all draws actor96
	# in the same frame (prologue.c:2624-2645): flash plus actor, no white.
	var a := _entry(); a.mode_stage = 19; a.substep = 0; a.opening["prologue_scene_counter"] = 252
	a.frame_count = 4000 - 1
	check(_effect(media,a,"flash",252).error.is_empty(),"Case0 peak flash rejected")
	a.opening.prologue_scene_counter = 0x100
	var setup := _effect(media,a,"actor_setup")
	check(setup.error.is_empty(),"O_13 actor96 setup rejected: " + setup.error)
	a.substep = 1
	var peak := _effect(media,a,"actor_draw")
	check(peak.error.is_empty(),"Peak-frame actor_draw rejected: " + peak.error)
	var uploads := _pump_uploads(peak)
	a.frame_count += 1
	var frames_seen: Dictionary = {a.opening.o13_draw.frame:true}
	drawn = _frame(a,renderer,upload)
	check(a.opening.o13_draw.frame == 0 and a.opening.prologue_closing_draws.size() == 1,"Peak frame records differ")
	check(drawn.error.is_empty() and drawn.image.get_data() == _want(false,0,252).get_data(),"Peak frame (actor plus flash, no white) differs: " + drawn.error)
	# At level 252 every dithered flash term is (252+d)>>3 = 31, so rows
	# 0-159 saturate to white over any actor or background; the pixels
	# cannot show actor96 here. Its draw is proved by the records above and
	# by the uploads it owns; the composition is checked at lower levels.
	var saturated := Image.create(320,160,false,Image.FORMAT_RGBA8); saturated.fill(Color8(255,255,255))
	check(drawn.image.get_region(Rect2i(0,0,320,160)).get_data() == saturated.get_data() and drawn.image.get_pixel(5,200) == BACK and uploads == 2,"Peak frame is not the saturated flash with two O_13 page uploads")

	# --- Cases 1 and 2: per frame, as prologue_flash_run then draw_frame:
	# white, flash, counter step, the substep change (and on case2's exit
	# the hide), then actor_draw_all.
	var last_visible: State = null
	var low_flash: State = null
	for i in 128:
		a.opening.erase("prologue_closing_draws")
		var counter: int = a.opening.prologue_scene_counter
		check(_effect(media,a,"white").error.is_empty() and _effect(media,a,"flash",counter).error.is_empty(),"Case1/2 boxes rejected")
		var exit := false
		if a.substep == 1:
			a.opening.prologue_scene_counter = counter-4
			if counter-4 < 3: a.substep = 2
		else:
			a.opening.prologue_scene_counter = counter+4
			if counter+4 >= 0x100:
				a.substep = 3; exit = true
				check(_effect(media,a,"actor_hide").error.is_empty(),"Case2 exit hide rejected")
		var step := _effect(media,a,"actor_draw")
		check(step.error.is_empty(),"actor_draw rejected: " + step.error)
		var emitted := _pump_uploads(step)
		uploads += emitted
		a.frame_count += 1
		if exit:
			check(i == 127 and step.presentation.is_empty() and not a.opening.has("o13_draw"),"Case2 exit frame drew actor96 or came early (%d)" % i)
			drawn = _frame(a,renderer,upload)
			check(drawn.error.is_empty() and drawn.image.get_data() == _want(true,-1,counter).get_data(),"Case2 exit frame (white plus flash, no actor) differs: " + drawn.error)
			break
		var frame: int = a.opening.o13_draw.frame
		last_visible = _copy(a)
		if emitted > 0 or not frames_seen.has(frame) or i == 60:
			frames_seen[frame] = true
			drawn = _frame(a,renderer,upload)
			check(drawn.error.is_empty() and drawn.image.get_data() == _want(true,frame,counter if counter < 0x100 else 255).get_data(),"Stage19 actor frame %d (draw %d) differs: %s" % [frame,i,drawn.error])
			if i == 60:
				# Tint: draw_flags 0x20 modulates by draw_brightness_get() =
				# screen_brightness; checked at flash level 16, where the
				# actor shows through, while this frame's pages are resident.
				low_flash = _copy(a)
				for tint: int in [0x60, 0xc0]:
					low_flash.opening.screen_brightness = tint
					var tinted := _frame(low_flash,renderer,upload)
					check(tinted.error.is_empty() and tinted.image.get_data() == _want(true,frame,16,tint).get_data(),"Actor96 tint %d differs: %s" % [tint,tinted.error])
					check(tinted.image.get_data() != drawn.image.get_data(),"Actor96 tint %d did not change the frame" % tint)
				# White boxes plus a low flash would hide P_01_12; actor96 must show.
				var plain := Image.create(320,240,false,Image.FORMAT_RGBA8); plain.fill(BACK)
				plain.fill_rect(Rect2i(0,0,320,160),Color8(255,255,255)); AdditiveFlash.paint(plain,counter)
				check(counter == 16 and drawn.image.get_data() != plain.get_data(),"Actor96 did not change the white frame")
	check(a.mode_stage == 19 and a.substep == 3 and a.opening.actor96.draw_flags == 0 and a.opening.actor96_draw_flags == 0,"Loop did not end on the case2 exit")
	check(frames_seen.size() == 7,"Not every O_13 frame was drawn: " + str(frames_seen.keys()))
	# Pages change residency: f0 2, f1 2, f2 2, f3 2, f4 2, f5 1, f6 1.
	check(uploads == 12,"O_13 tpage uploads differ from residency changes: %d" % uploads)

	# The last reachable visible frame (19/2, counter 252) for the guards.
	var v := last_visible
	check(v != null and v.substep == 2 and v.opening.prologue_scene_counter == 252 and v.opening.actor96_draw_flags == 0xa4,"Last visible frame differs")
	check(low_flash != null and _flash_level(low_flash) == 16,"Low-flash tint frame was not reached")
	# The state the source cannot produce: 19/2 whose flash ran at 252 and
	# whose counter reached 0x100 without case2's substep 3 and hide.
	var bad := _copy(v); bad.opening.prologue_scene_counter = 0x100
	bad.opening.prologue_closing_draws[-1].color = 252*0x10101
	check(_frame(bad,renderer,upload).error == "closing actor96 is visible on the stage19 case2 exit frame","Renderer drew actor96 on the case2 exit frame")
	bad.opening.erase("o13_draw")
	check(_effect(media,bad,"actor_draw").error == Media.CLOSING_CASE2_EXIT_DRAW,"actor_draw ran on the case2 exit frame")
	# Actor guards.
	var keep: Dictionary = v.opening.o13_draw.duplicate(true)
	v.opening.o13_draw.frame_count -= 1
	check(_frame(v,renderer,upload).error == "closing actor96 needs this committed frame's draw record","Stale actor draw drawn")
	v.opening.o13_draw = keep.duplicate(true)
	var tp_keep: Dictionary = v.opening.o13_tpages.duplicate(true)
	var any_slot: String = v.opening.o13_tpages.resident.keys()[0]
	v.opening.o13_tpages.resident[any_slot].key = "y".sha256_text()
	check(_frame(v,renderer,upload).error == "native GPU stage actor batch has no executed receipt","Actor drawn from a page with no upload receipt")
	v.opening.o13_tpages = tp_keep.duplicate(true)
	var swapped: Dictionary = {}
	for page: String in keep.slots: swapped[page] = 11 - int(keep.slots[page])
	v.opening.o13_draw.slots = swapped
	check(_frame(v,renderer,upload).error == "closing actor96 tpage allocation differs from search_tpage","Actor drawn with swapped slots")
	v.opening.o13_draw = keep.duplicate(true)
	v.opening.erase("screen_brightness")
	check(_frame(v,renderer,upload).error == "closing actor96 tint has no owned brightness","Actor drawn without draw_brightness_get")
	v.opening.screen_brightness = 128
	v.opening.actor96.clut_row = 0x1fa
	check(_frame(v,renderer,upload).error == "closing actor96 is not the stage19 O_13 setup","Actor drawn with another CLUT row")
	v.opening.actor96.clut_row = 0x1fb
	var clut_at := (0x1fb*1024)*2+2
	owned.vram_known_mask[clut_at] = 0
	check(_frame(v,renderer,upload).error == "closing actor96 references unknown palette memory","Actor drawn through an unknown CLUT row")
	owned.vram_known_mask[clut_at] = 255
	check(_frame(v,renderer,upload).error.is_empty(),"Actor frame failed after restoring its guards")
	# Upload owner: only this frame's resident record in a visible 19/1..2 frame.
	var res: Dictionary = v.opening.o13_tpages.resident[any_slot]
	var slot_n := int(any_slot)
	check(Upload._closing_o13_owner(v,slot_n,res.offset,res.key),"Upload owner rejected the resident O_13 page")
	check(not Upload._closing_o13_owner(v,11-slot_n,res.offset,res.key) and not Upload._closing_o13_owner(v,slot_n,res.offset,"z".sha256_text()),"Upload owner accepted another slot or key")
	v.substep = 3; check(not Upload._closing_o13_owner(v,slot_n,res.offset,res.key),"Upload owner accepted a substep without actor draws"); v.substep = 2

	# --- Case3 after the hide: flash only, no actor, no draw record.
	a.opening.erase("prologue_closing_draws")
	check(_effect(media,a,"flash",0x100).error.is_empty(),"Case3 flash rejected")
	a.opening.prologue_scene_counter = 0xfc
	var hidden := _effect(media,a,"actor_draw")
	check(hidden.error.is_empty() and hidden.presentation.is_empty() and not a.opening.has("o13_draw"),"Hidden actor96 drew or uploaded")
	a.frame_count += 1
	drawn = _frame(a,renderer,upload)
	check(drawn.error.is_empty() and drawn.image.get_data() == _want(false,-1,255).get_data(),"Case3 flash over P_01_12 differs: " + drawn.error)
	a.opening["o13_draw"] = keep
	check(_frame(a,renderer,upload).error == "hidden actor96 has a draw record","Hidden actor96 drawn")
	a.opening.erase("o13_draw")

	# --- The 20->21 frame: committed 21/0 with line 42 draws P_01_12 only.
	var e := _copy(a); e.mode_stage = 21; e.substep = 0
	e.opening.erase("prologue_closing_draws"); e.opening["prologue_line"] = 42; e.opening.prologue_scene_counter = 0x3e
	drawn = _frame(e,renderer,upload)
	check(drawn.error.is_empty() and drawn.image.get_data() == _want(false,-1,-1).get_data(),"20->21 frame differs: " + drawn.error)
	e.opening.prologue_line = 41
	check(_frame(e,renderer,upload).error == "closing renderer is outside stages 18-20","Renderer drew a stage21 frame without line 42")
	e.opening.prologue_line = 42; e.substep = 1
	check(_frame(e,renderer,upload).error == "closing renderer is outside stages 18-20","Renderer drew stage21 after its entry frame")
	e.substep = 0; e.opening["prologue_closing_draws"] = [media._closing_primitive(0xffffff,0xd,1)]
	check(_frame(e,renderer,upload).error == "closing boxes are outside stage19","Renderer drew boxes on the 20->21 frame")
	e.opening.erase("prologue_closing_draws")

	# --- Media bridge: schema, paint_background routing and the stage17 decode.
	check(media._intent_schema({"kind":"first_arrival_stage_actor_upload","batch_key":BG_KEY,"resource_id":Board.O_13_ID,"slot":5,"source_offset":0x1c230}).is_empty(),"O_13 page upload intent rejected")
	for bad_intent: Array in [[7,0x230],[5,0x231],[37,0x230]]:
		check(media._intent_schema({"kind":"first_arrival_stage_actor_upload","batch_key":BG_KEY,"resource_id":Board.O_13_ID,"slot":bad_intent[0],"source_offset":bad_intent[1]}) == "first-arrival stage actor upload descriptor is invalid","O_13 upload intent accepted %s" % str(bad_intent))
	check(media._intent_schema({"kind":"first_arrival_names_bg_upload","batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":Board.BG_4973_WORD_SHA}).is_empty(),"P_01_12 upload intent rejected")
	check(media._intent_schema({"kind":"first_arrival_names_bg_upload","batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":Board.BG_4973_WORD_SHA}).is_empty(),"P_01_12 upload intent rejected buffer 0")
	check(media._intent_schema({"kind":"first_arrival_names_bg_upload","batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":0,"decode_rect":[320,256,320,160],"word_sha256":Board.BG_4973_WORD_SHA}) == "first-arrival names background upload descriptor is invalid","P_01_12 upload intent accepted buffer 0 at buffer 1's rectangle")
	var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var painter := Painter.new()
	s = _entry()
	check(media.paint_background(target,s,null) == Media.CLOSING_RENDERER_MISSING,"paint_background drew stages 18-20 without a renderer")
	check(media.paint_background(target,s,painter).is_empty() and painter.calls == 1,"paint_background did not route to paint_closing")
	check(media.paint_background(target,e,painter).is_empty() and painter.calls == 2,"paint_background did not route the 20->21 frame to paint_closing")
	e.opening["o13_draw"] = keep
	check(media.paint_background(target,e,painter) == Media.CLOSING_EXIT_UNSUPPORTED and painter.calls == 2,"paint_background accepted a 20->21 frame with an actor draw")
	e.opening.erase("o13_draw")
	media._names_bg_uploads.clear()
	check(media.paint_background(target,s,painter) == Media.CLOSING_BG_NOT_UPLOADED and painter.calls == 2,"paint_background drew an unpublished P_01_12")
	media._names_bg_uploads[BG_KEY] = {"batch_key":BG_KEY}
	s.opening.bg_layer_0.attribute = 0x41000000
	check(media.paint_background(target,s,painter) == "stages 18-20 require the bg layer 0 cleared by stage16" and painter.calls == 2,"paint_background drew over a live layer 0")
	var d := _entry(); d.mode_stage = 17; d.substep = 3
	d.opening.names_media = {"bg_show_1":0,"buffer_0":{"lba":0x496a},"active_draw":"dec_bg_draw"}
	d.opening.media["decoded"] = {}
	d.opening.media["background_pending"] = {"1":"bg"}
	d.opening.media.requests["bg"] = {"request_id":"bg","epoch":5,"status":"ready","kind":"background","resource_id":Board.BG_4973_ID}
	var before: Dictionary = d.opening.duplicate(true)
	var decode := Session.FrameResult.new()
	# dec_bg_decode_to_vram(1) follows dec_bg_show[1]: with buffer 1 shown
	# (after a dec_bg_reset retry, for example) P_01_12 lands in buffer 0.
	var d1 := State.new(); d1.restore(d.projection()); d1.opening.names_media.bg_show_1 = 1
	check(media.background_decode(d1,1,decode) == 0 and decode.error.is_empty() and d1.opening.names_media.bg_show_1 == 0 and d1.opening.names_media.buffer_0.decode_rect == [704,0,320,160],"Stage17 decode did not follow dec_bg_show[1]")
	d.opening.names_media.bg_show_1 = 2
	decode = Session.FrameResult.new()
	check(media.background_decode(d,1,decode) == -1 and decode.error == "first-arrival stage17 0x4973 decode has no slot1 show state","Stage17 decode accepted a malformed show state")
	d.opening.names_media.bg_show_1 = 0
	d.opening.media.requests.bg.resource_id = Board.BG_AD0B_ID
	decode = Session.FrameResult.new()
	check(media.background_decode(d,1,decode) == -1 and decode.error == "opening background decode has no matching completed 4973 read" and d.opening.names_media == before.names_media and d.opening.media.decoded.is_empty(),"Stage17 decode accepted another read")
	d.opening.media.requests.bg.resource_id = Board.BG_4973_ID; d.opening.media.requests.bg.status = "retry"
	decode = Session.FrameResult.new()
	check(media.background_decode(d,1,decode) == -1 and decode.error.is_empty() and d.opening.names_media == before.names_media,"Stage17 retry changed the shown buffer")
	d.opening.media.requests.bg.status = "ready"
	decode = Session.FrameResult.new()
	check(media.background_decode(d,1,decode) == 1 and decode.error.is_empty(),"Stage17 P_01_12 decode rejected: " + decode.error)
	var buf: Dictionary = d.opening.names_media.buffer_1
	check(d.opening.names_media.bg_show_1 == 1 and buf.lba == 0x4973 and buf.decode_rect == [320,256,320,160] and buf.word_sha256 == Board.BG_4973_WORD_SHA and decode.presentation.size() == 2 and decode.presentation[1].kind == "first_arrival_names_bg_upload" and decode.presentation[1].batch_key == buf.upload_batch_key,"Stage17 decode records differ")
	check(media._intent_schema(decode.presentation[1]).is_empty(),"Stage17 decode emitted an invalid upload intent")
	_bridge_chain(o01)
	if failures == 0: print("PASS: %d original stages 18-20 render checks" % checks)
	quit(1 if failures else 0)

func _bridge_chain(o01: PackedByteArray) -> void:
	# Production path for stage19's first frames. Stand-ins, named:
	#  - The stage0 O_01 batch, stage1 board batch and stage17 P_01_12 batch
	#    run through the upload bridge's own pump and are recorded as its
	#    executed keys; their Session publications are not replayed here.
	#  - The O_13 read's published buffer is placed in the media bridge (as
	#    run_rei_vram.gd does for O_09); the request record is in _entry().
	#  - The P_01_10 and stage15 cut keys are ready stand-ins for the presenter.
	# Everything else is production: Flow.step in a Session, the media
	# bridge's closing effects and arrival_draw, upload preflight/publish
	# (which owns and executes the O_13 page uploads), and the real
	# first_arrival_actor_renderer.paint_closing on the bridge's VRAM.
	var media := Media.new()
	check(media.prepare(store,board).is_empty(),"Media bridge rejected the render board pack")
	var session := Session.new()
	var st := _entry(); st.mode_stage = 19; st.substep = 0; st.opening["prologue_scene_counter"] = 252; st.frame_count = 5000 - 1
	session.state = st
	session.rng.configure(Rng.OPENBIOS_PROFILE,1)
	var pixels := PackedByteArray(); pixels.resize(Pump.VRAM_BYTES); pixels.fill(0xa5)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	var runtime := {"vram_owner":"closing-bridge","scratch_owner":"closing-bridge-scratch","vram_words":pixels,"vram_known_mask":mask,"scratch_words":_cold_scratch(),"pending_gpu_effects":[]}
	var upload := Upload.new()
	check(upload.prepare(media,store,runtime,board).is_empty(),"Upload bridge rejected the render board pack")
	check(upload.bind_session(session).is_empty(),"Upload bridge did not bind the Session")
	_o01_batch(o01,runtime,upload,upload._pump,O01_KEY); upload._batch_key = O01_KEY
	check(upload._pump.execute_board(board,BOARD_KEY,runtime).error.is_empty(),"Board batch rejected"); upload._board_batch_key = BOARD_KEY
	check(upload._pump.execute_names_bg(board,Board.BG_4973_ID,BG_KEY,runtime,1).error.is_empty(),"P_01_12 batch rejected")
	upload._names_bg_batch_keys[BG_KEY] = Board.BG_4973_ID
	media._names_bg_uploads[BG_KEY] = {"batch_key":BG_KEY,"resource_id":Board.BG_4973_ID,"buffer":1}
	media._buffers[str(0x80180000)] = {"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":o13.duplicate()}
	check(Pump._region(runtime.vram_words,Rect2i(0,0x1fb,256,1)) == clut,"Bridge pump CLUT row 0x1FB differs from the reference")
	var actor := ActorRenderer.new()
	check(actor.prepare(store,upload,board).is_empty(),"Actor renderer did not prepare on the render pack")
	var services: Dictionary = {}
	for key: String in Flow.REQUIRED:
		services[key] = func(_a = null,_b = null,_c = null,_d = null,_e = null): return 0
	services["prologue_closing_effect"] = media.closing_effect
	services["arrival_draw"] = func(s, slot, out): media.arrival_draw(s,slot,out,actor)
	services["outer_draw"] = func(_s, _arg, _out): pass
	session.router.bind_mode(0x15,func(current, input, rng, result):
		var inner: Session.FrameResult = Flow.step(current,rng,input.pressed,services)
		result.error = inner.error
		result.presentation.append_array(inner.presentation))
	session.service_completion = media.complete
	session.draw_logic = func(_s, _r, _result): pass
	session.cd_logic = func(current, _r, result): result.error = upload.preflight(result,current)
	session.audio_logic = func(_s, _r, _result): pass
	var frames := 0
	var o13_uploads := 0
	var compared := 0
	var presenter_frame: State = null
	while frames < 16:
		var tick := session.clock.logic_iterations
		var result := session.step(InputFrame.new(tick),media.pending_events(tick))
		check(result.committed and result.error.is_empty(),"Session frame %d rejected: %s" % [frames,result.error])
		if not result.committed: return
		check(media.publish(result,tick).is_empty(),"Media publication failed at frame %d" % frames)
		var published := upload.publish(result,tick)
		check(published.is_empty(),"Upload publication failed at frame %d: %s" % [frames,published])
		var emitted := 0
		for intent: Dictionary in result.presentation:
			if intent.kind == "first_arrival_stage_actor_upload":
				emitted += 1
				check(upload.batch_ready(intent.batch_key).is_empty(),"O_13 page %s has no bridge receipt" % str(intent.source_offset))
		o13_uploads += emitted
		check(result.presentation.any(func(i): return i.kind == "first_arrival_background_draw"),"Frame %d queued no dec_bg_draw" % frames)
		var frame: int = st.opening.o13_draw.frame
		if frames == 0 or emitted > 0:
			var image := Image.create(320,240,false,Image.FORMAT_RGBA8); image.fill(BACK)
			var err := actor.paint_closing(image,st)
			var white := frames > 0
			check(err.is_empty() and image.get_data() == _want(white,frame,_flash_level(st)).get_data(),"Bridge frame %d (O_13 frame %d) differs: %s" % [frames,frame,err])
			compared += 1
			if frames == 0:
				check(st.substep == 1 and emitted == 2 and not white,"Bridge peak frame differs")
			presenter_frame = _copy(st)
		frames += 1
		if frame == 1 and emitted > 0: break
	check(compared == 2 and o13_uploads == 4 and st.opening.o13_draw.frame == 1,"Bridge chain did not reach the O_13 frame-1 page change: %d %d" % [compared,o13_uploads])
	# A page key the bridge never executed is refused by paint_closing.
	var forged := _copy(st)
	var any_slot: String = forged.opening.o13_tpages.resident.keys()[0]
	forged.opening.o13_tpages.resident[any_slot].key = "q".sha256_text()
	var probe := Image.create(320,240,false,Image.FORMAT_RGBA8)
	check(actor.paint_closing(probe,forged) == "actual first-arrival upload batch has not executed","paint_closing accepted a page the bridge never uploaded")
	_presenter_checks(media,actor,upload,presenter_frame)

func _presenter_checks(media: Media, actor: ActorRenderer, upload: Upload, visible: State) -> void:
	# The presenter composes stages 18-20 and the 20->21 frame through
	# paint_background with the line lifetimes, on the real actor renderer.
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	if text_root.is_empty() or visible == null: return
	var text := TextStore.new()
	check(text.open(text_root,"5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc").is_empty(),"Prologue text did not open")
	var painter := TextPainter.new()
	check(painter.open_font(OS.get_environment("PROLOGUE_FONT_ROOT")).is_empty(),"Prologue font did not open")
	var s := _copy(visible)
	# A visible actor96 would send stages 0-11 to the O_01 actor painter.
	check(s.opening.get("actor96_draw_flags") == 0xa4 and s.opening.has("o13_draw"),"Presenter check frame lacks the visible actor96")
	s.rule_state = {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0]}
	s.opening.arrival_media["back_clear"] = true
	s.opening.bg_layer_0["cut_batch_key"] = "cut".sha256_text()
	s.opening.names_media.buffer_0["upload_batch_key"] = "p0110".sha256_text()
	var presenter := Presenter.new()
	check(presenter.prepare(media,text,painter,s,actor).is_empty(),"Presenter did not prepare with lines 37-42")
	var stand_ins := ["cut".sha256_text(),"p0110".sha256_text()]
	var ready := func(key: String) -> String: return "" if key in stand_ins else upload.batch_ready(key)
	var checker := func(target: Image, _state) -> String:
		target.fill(Color.BLACK); return ""
	var front := func(target: Image, _state) -> String:
		target.set_pixel(300,200,Color.GREEN); return ""
	s.opening["prologue_text_work"] = {"line_id":41,"pos":0,"total":0,"entries":[]}
	var direct := Image.create(320,240,false,Image.FORMAT_RGBA8); direct.fill(Color.BLACK)
	check(actor.paint_closing(direct,s).is_empty(),"Direct closing paint failed")
	var out: Dictionary = presenter.render(s,checker,front,ready)
	check(out.error.is_empty() and out.image.get_region(Rect2i(0,0,320,160)).get_data() == direct.get_region(Rect2i(0,0,320,160)).get_data() and out.image.get_pixel(300,200) == Color.GREEN,"Presenter stage19 frame differs: " + str(out.error))
	s.opening.prologue_text_work.line_id = 42
	check(presenter.render(s,checker,front,ready).error == "first-arrival presenter line is outside its original scene lifetime","Presenter accepted line 42 in stage19")
	s.opening.prologue_text_work.line_id = 37
	check(presenter.render(s,checker,front,ready).error == "first-arrival presenter line is outside its original scene lifetime","Presenter accepted line 37 in stage19")
	s.opening.prologue_text_work.line_id = 41
	var bad_ready := func(key: String) -> String: return "P_01_10 not ready" if key == "p0110".sha256_text() else ready.call(key)
	check(presenter.render(s,checker,front,bad_ready).error == "P_01_10 not ready","Presenter ignored the hidden buffer's upload owner")
	# The 20->21 frame: actor96 hidden since case2, no boxes, line 42 still shown.
	var e := _copy(s); e.mode_stage = 21; e.substep = 0
	e.opening.actor96.draw_flags = 0; e.opening.actor96_draw_flags = 0
	e.opening.erase("o13_draw"); e.opening.erase("prologue_closing_draws")
	e.opening["prologue_line"] = 42; e.opening.prologue_scene_counter = 0x3e
	e.opening.prologue_text_work.line_id = 42
	direct.fill(Color.BLACK)
	check(actor.paint_closing(direct,e).is_empty(),"Direct 20->21 paint failed")
	out = presenter.render(e,checker,front,ready)
	check(out.error.is_empty() and out.image.get_region(Rect2i(0,0,320,160)).get_data() == direct.get_region(Rect2i(0,0,320,160)).get_data() and out.image.get_region(Rect2i(0,0,320,160)).get_data() == _want(false,-1,-1).get_region(Rect2i(0,0,320,160)).get_data(),"Presenter 20->21 frame differs: " + str(out.get("error")))
	e.opening.prologue_text_work.line_id = 41
	check(presenter.render(e,checker,front,ready).error == "first-arrival presenter line is outside its original scene lifetime","Presenter accepted line 41 on the 20->21 frame")
