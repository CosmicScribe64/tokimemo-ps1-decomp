extends SceneTree
## Visible draw of opening stages 15-17 from the original board pack
## (BOARD_PACK) and the arrival O_01 palettes (FIRST_ARRIVAL_MEDIA_ROOT/SHA).
## The stage effects emit their upload intents; the native GPU pump executes
## them into owned VRAM; the renderer paints from that VRAM only. Each frame
## is compared with a reference built straight from the resource bytes, and
## every guard is shown to refuse (state left as it was where it matters).
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Stages = preload("res://services/opening_stages_15_17.gd")
const Renderer = preload("res://presentation/opening_stages_15_17_renderer.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")

class Ready:
	# batch_ready over the pump's own receipts, as the upload bridge does.
	var pump: Pump
	var owned: Dictionary
	var kinds: Dictionary = {}
	func batch_ready(key: String) -> String:
		match kinds.get(key,""):
			"bg": return pump.names_bg_batch_ready(key,owned)
			"cut": return pump.cut_batch_ready(key,owned)
			"actor": return pump.stage_actor_batch_ready(key,owned)
		return "batch has not executed"

class Visible:
	# The visible renderer seen by arrival_draw (preflight_visible only).
	var renderer: Renderer
	func preflight_visible(state) -> String:
		return renderer.preflight(state,null,true)

var failures := 0
var checks := 0
var board := Board.new()
var pump := Pump.new()
var ready := Ready.new()
var media := Media.new()
var renderer := Renderer.new()
var owned: Dictionary
var o01 := PackedByteArray()

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func _initialize() -> void:
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if error.is_empty():
		var store := Store.new()
		error = store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA"))
		if error.is_empty(): o01 = store.transfer(Store.ACTOR_ID).get("bytes",PackedByteArray())
	if not error.is_empty() or o01.size() != 26624:
		printerr("inputs: ",error); quit(1); return
	media._board_store = board
	check(media.stages_15_17.prepare(board).is_empty(),"Stage effects refused the board pack")
	check(renderer.prepare(board).is_empty(),"Renderer refused the board pack")
	_board_checks()
	_owned_vram()
	_stage15_checks()
	if failures == 0: print("PASS: %d stage15-17 render checks" % checks)
	quit(1 if failures else 0)

# ---- resources --------------------------------------------------------

func _board_checks() -> void:
	for spec: Array in [[Board.BG_496A_ID,0x496a,Board.BG_496A_TRANSFER_SHA,Board.BG_496A_WORD_SHA],[Board.BG_4973_ID,0x4973,Board.BG_4973_TRANSFER_SHA,Board.BG_4973_WORD_SHA]]:
		var transfer := board.background_transfer(spec[0])
		var words := board.background_software_upload(spec[0])
		check(transfer.get("lba") == spec[1] and transfer.get("sectors") == 10 and transfer.get("destination") == 0x80162000 and Store._sha(transfer.bytes) == spec[2] and transfer.bytes.size() == 20480,"0x%x transfer differs" % spec[1])
		check(Store._sha(words.words) == spec[3] and words.words.size() == 102400,"0x%x decoded words differ" % spec[1])
	# The 10-sector extent carries the next file's first sector (P_01_12, then X_01_13).
	check(board.background_transfer(Board.BG_496A_ID).bytes.slice(18432,20480) == board.background_transfer(Board.BG_4973_ID).bytes.slice(0,2048),"P_01_10 extent does not end in P_01_12's first sector")
	check(renderer.parts(Board.O_16_ID,0x80180258,0).size() == 5 and renderer.parts(Board.O_06_ID,0x801842c4,0).size() == 4 and renderer.parts(Board.O_06_ID,0x801800f8,0).size() == 2,"Original frame part counts differ")
	check(renderer.parts(Board.O_16_ID,0x80180258,16).is_empty() and renderer.parts(Board.O_16_ID,0x801842c4,0).is_empty(),"Frame outside its table resolved")
	check(Renderer.parse_frames(board.stage_transfer(Board.O_16_ID).bytes.slice(0,0x200),0x80180258,0x8018024c,16,3).is_empty(),"Truncated O_16 parsed")

func _owned_vram() -> void:
	var pixels := PackedByteArray(); pixels.resize(Pump.VRAM_BYTES)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	var scratch := PackedByteArray(); scratch.resize(512)
	owned = {"vram_owner":"render1517","scratch_owner":"render1517-scratch","vram_words":pixels,"vram_known_mask":mask,"scratch_words":scratch,"pending_gpu_effects":[]}
	# Stage0 (prologue.c:259) left the O_01 actor CLUTs in rows 496-511.
	for row in 16:
		var clut := Pump._mode2_stp(o01.slice(0x4490+row*512,0x4490+row*512+512))
		for i in 512:
			owned.vram_words[((496+row)*1024)*2+i] = clut[i]
			owned.vram_known_mask[((496+row)*1024)*2+i] = 255
	ready.pump = pump; ready.owned = owned

func _run(result: Session.FrameResult) -> String:
	# Execute this frame's upload intents in order, as publish would.
	for intent: Dictionary in result.presentation:
		var schema := media._intent_schema(intent)
		if not schema.is_empty(): return "schema: " + schema
		var out: Dictionary = {"error":""}
		match intent.kind:
			"first_arrival_names_bg_upload":
				out = pump.execute_names_bg(board,intent.resource_id,intent.batch_key,owned,intent.buffer); ready.kinds[intent.batch_key] = "bg"
			"first_arrival_cut_upload":
				out = pump.execute_cut_vram(board,intent.resource_id,intent.batch_key,owned); ready.kinds[intent.batch_key] = "cut"
			"first_arrival_stage_actor_upload":
				out = pump.execute_stage_actor_vram(board,intent.resource_id,intent.slot,intent.source_offset,intent.batch_key,owned); ready.kinds[intent.batch_key] = "actor"
		if not str(out.error).is_empty(): return str(out.error)
	return ""

func _fx(s: State, kind: String, value: int = 0) -> Array:
	var result := Session.FrameResult.new()
	var outcome: Variant = media.stages_15_17.effect(s,kind,value,result)
	return [result,outcome]

func _draw(s: State) -> Array:
	# actor_draw at the end of the body, then game_mode_dispatch's
	# frame_count increment: the paint below sees that frame's value.
	var out := _fx(s,"actor_draw")
	s.frame_count += 1
	return out

# ---- reference built from the resource bytes, not VRAM ----------------

static func _rgb(c: int, tint: int, tint_g: int = -1, tint_b: int = -1) -> Array:
	var tg: int = tint if tint_g < 0 else tint_g
	var tb: int = tint if tint_b < 0 else tint_b
	return [mini(31,((c&31)*tint)>>7),mini(31,(((c>>5)&31)*tg)>>7),mini(31,(((c>>10)&31)*tb)>>7)]

static func _put(img: Image, x: int, y: int, c: int, tint: int, tint_g: int = -1, tint_b: int = -1) -> void:
	var v := _rgb(c,tint,tint_g,tint_b)
	if (c&0x8000) != 0:
		var back := img.get_pixel(x,y)
		v = [((back.r8>>3)+v[0])>>1,((back.g8>>3)+v[1])>>1,((back.b8>>3)+v[2])>>1]
	img.set_pixel(x,y,Color8((v[0]<<3)|(v[0]>>2),(v[1]<<3)|(v[1]>>2),(v[2]<<3)|(v[2]>>2),255))

const CHECKER_MARK := Color8(255,0,255,255)

static func _checker_cell(x: int, y: int) -> bool:
	# hud_screen_frame_draw(2) cells (hud_panels.c:1418-1428): rows 6-7
	# (y 192-255) everywhere, row 5 (y 160-191) at columns 0 and 9.
	return y >= 192 or (y >= 160 and (x < 32 or x >= 288))

static func _mark_checker(img: Image) -> void:
	# Stand-in for the HUD's checker sprites: an opaque marker per cell.
	for y in range(160,240):
		for x in 320:
			if _checker_cell(x,y): img.set_pixel(x,y,CHECKER_MARK)

func _reference(bg_id: String, cut_id: String, resource_id: String, actors: Array, cut_tint: Array = [128,128,128], checker_ot: int = -1) -> Image:
	# actors: [unk_01, index, frame_table, tpage_table, frame, x, y] drawn back to front.
	# Actor pixels are not clipped at row 160. checker_ot >= 0 adds the
	# checker frame at that OT entry: ClearOTagR draws higher entries first,
	# and within one entry the checker (added by the mode handler, before
	# actor_draw_all) draws after the actors, so it covers OT >= checker_ot.
	var img := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var words: PackedByteArray = board.background_software_upload(bg_id).words
	for y in 160:
		for x in 320:
			var c := words.decode_u16((y*320+x)*2)
			var v := _rgb(c,128)
			img.set_pixel(x,y,Color8((v[0]<<3)|(v[0]>>2),(v[1]<<3)|(v[1]>>2),(v[2]<<3)|(v[2]>>2),255))
	if not cut_id.is_empty():
		var cut: PackedByteArray = board.stage_transfer(cut_id).bytes
		var pal := Pump._mode2_stp(cut.slice(0x18000,0x18200))
		for y in 160:
			for x in 320:
				@warning_ignore("integer_division")
				var page := (x/128)+(3 if y >= 128 else 0)
				var index := cut[page*0x4000+(y%128)*128+(x%128)]
				var c := pal.decode_u16(index*2)
				if c != 0: _put(img,x,y,c,cut_tint[0],cut_tint[1],cut_tint[2])
	if resource_id.is_empty():
		if checker_ot >= 0: _mark_checker(img)
		return img
	var bytes: PackedByteArray = board.stage_transfer(resource_id).bytes
	var order := actors.duplicate()
	order.sort_custom(func(a, b): return [a[0],a[1]] > [b[0],b[1]])
	var checker_done := checker_ot < 0
	for a: Array in order:
		var ot: int = (int(a[0])-1) & 0xff if int(a[0]) != 0 else 0
		if not checker_done and ot < checker_ot:
			_mark_checker(img); checker_done = true
		var offset := int(bytes.decode_u32(int(a[2])-0x80180000+int(a[4])*4))-0x80180000
		for part in bytes.decode_s16(offset):
			var row := offset+8+part*14
			var texture := bytes.decode_u16(row+4)
			var page := int(bytes.decode_u32(int(a[3])-0x80180000+(texture>>6)*4))-0x80180000
			var clut := Pump._mode2_stp(o01.slice(0x4490+((bytes.decode_u16(row+6)>>8)&15)*512,0x4490+((bytes.decode_u16(row+6)>>8)&15)*512+512))
			for y in bytes.decode_s16(row+12):
				for x in bytes.decode_s16(row+10):
					var px: int = 160+int(a[5])+bytes.decode_s16(row)+x
					var py: int = 120+int(a[6])+bytes.decode_s16(row+2)+y
					if px < 0 or px >= 320 or py < 0 or py >= 240: continue
					var index := bytes[page+(((texture>>3)&7)*16+y)*128+(texture&7)*16+x]
					var c := clut.decode_u16(index*2)
					if c != 0: _put(img,px,py,c,128)
	if not checker_done: _mark_checker(img)
	return img

func _paint_with_checker(s: State, layer_split: bool = true) -> Array:
	# The presenter's order: message-area parts under the checker, the
	# checker (marker), then background, cut and the scene actor layer.
	var img := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var error := ""
	if layer_split: error = renderer.paint_actors(img,s,owned,ready,Renderer.UNDER_CHECKER)
	if error.is_empty(): _mark_checker(img)
	if error.is_empty(): error = renderer.paint_background(img,s,owned,ready)
	if error.is_empty(): error = renderer.paint_actors(img,s,owned,ready)
	return [error,img]

func _paint(s: State) -> Array:
	var img := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var error := renderer.paint_background(img,s,owned,ready)
	if error.is_empty(): error = renderer.paint_actors(img,s,owned,ready)
	return [error,img]

static func _same_scene(a: Image, b: Image) -> bool:
	# All 240 rows: rows 160-239 must stay untouched in both images.
	return a.get_data() == b.get_data()

func _drawn(s: State, key: String) -> Array:
	# The reference's actor list from this frame's actor_draw record.
	var out: Array = []
	for item: Dictionary in s.opening[key].drawn:
		var actor: Dictionary = s.opening["actor%d" % item.index]
		out.append([item.unk_01,item.index,item.frame_table,actor.tpage_table,item.frame,item.x,item.y])
	return out

# ---- stage 15 ---------------------------------------------------------

func _entry() -> State:
	# Stage14 case6 (prologue.c:2100-2121): ACEC decoded into buffer 1 and
	# shown, CUT_16 slot set, O_16 copied, actor96 at x -0xF0.
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 15; s.mode_step = 0; s.substep = 1; s.frame_count = 1000
	var acec_key := "acec-1".sha256_text()
	var cut_key := "cut16".sha256_text()
	s.opening["names_media"] = {"bg_show_1":1,"active_draw":"dec_bg_draw","buffer_0":{"lba":0xad05},
		"buffer_1":{"lba":0xacec,"resource_id":Board.BG_ACEC_ID,"decode_rect":[320,256,320,160],"upload_batch_key":acec_key}}
	s.opening["cut_media"] = {"resource_id":Board.CUT_16_ID,"sha256":Board.CUT_16_SHA,"request_id":"cut16","upload_batch_key":cut_key,"white_palette":false}
	s.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"gray_clut_row":494,"attribute":0x41000000,"resource_id":Board.CUT_16_ID,"cut_batch_key":cut_key}
	s.opening["actor96"] = {"profile":"o16_shiori_line20","index":96,"unk_01":0xb,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x80180258,"sequence_table":0x80180298,"tpage_table":0x8018024c,"frame_index":0,"sequence_counter":0,"speed":1,
		"brightness":0x80,"x":-0xf0,"y":-0x78,"resource_id":Board.O_16_ID}
	s.opening["actor96_draw_flags"] = 0xa4
	s.opening["media"] = {"epoch":7,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	s.opening["screen_brightness"] = 128
	if not ready.kinds.has(acec_key):
		var bg := pump.execute_names_bg(board,Board.BG_ACEC_ID,acec_key,owned,1)
		check(bg.error.is_empty(),"ACEC buffer1 upload refused: " + str(bg.error))
		ready.kinds[acec_key] = "bg"
		var cut := pump.execute_cut_vram(board,Board.CUT_16_ID,cut_key,owned)
		check(cut.error.is_empty(),"CUT_16 slot-set upload refused: " + str(cut.error))
		ready.kinds[cut_key] = "cut"
	return s

func _stage15_checks() -> void:
	var s := _entry()
	# Before this frame's actor_draw there is no draw record: refused.
	var early := _paint(s)
	check(early[0] == "stages 15-17 actors need this committed frame's actor_draw record","Paint without this frame's actor_draw accepted: " + str(early[0]))
	check(renderer.preflight(s).is_empty(),"Stage15 preflight refused the stage14 entry: " + renderer.preflight(s))
	# Frame 1: slide_in then actor_draw; the frame's uploads are O_16 pages.
	_fx(s,"slide_in")
	var drawn: Array = _draw(s)
	var intents: Array = drawn[0].presentation.map(func(i): return [i.kind,i.resource_id,i.slot,i.source_offset])
	check(drawn[0].error.is_empty() and intents == [["first_arrival_stage_actor_upload",Board.O_16_ID,5,0x82c0],["first_arrival_stage_actor_upload",Board.O_16_ID,6,0x42c0]],"Frame0 page uploads differ from search_tpage: " + str(intents))
	# Not uploaded yet: the record names keys whose batches never ran.
	var unready := _paint(s)
	check(unready[0] == "stages 15-17 actor page upload is incomplete","Paint accepted pages that were never uploaded: " + str(unready[0]))
	check(_run(drawn[0]).is_empty(),"Pump refused the O_16 page uploads")
	var frame := _paint(s)
	check(frame[0].is_empty(),"Stage15 frame refused: " + str(frame[0]))
	var expected := _reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages"))
	check(frame[0].is_empty() and _same_scene(frame[1],expected),"Stage15 frame differs from the original resource reference")
	# Actor96 really drew: the frame is not the bare bg + cut.
	check(not _same_scene(frame[1],_reference(Board.BG_ACEC_ID,Board.CUT_16_ID,"",[])),"Stage15 actor96 left no pixels")
	# Next frame: resident pages are not uploaded again.
	s.frame_count += 1
	var again: Array = _draw(s)
	check(again[0].error.is_empty() and again[0].presentation.is_empty(),"Unchanged resident pages were uploaded again")
	# Guards: stale record, altered slot map, VRAM overwritten, missing epoch.
	s.frame_count += 1
	check(_paint(s)[0] == "stages 15-17 actors need this committed frame's actor_draw record","Stale actor_draw record accepted")
	_draw(s)
	var bad_slot := State.new(); bad_slot.restore(s.projection()); bad_slot.frame_count = s.frame_count
	bad_slot.opening.o16_tpages.slots[str(0x82c0)] = 6
	check(_paint(bad_slot)[0] == "stages 15-17 tpage allocation differs from search_tpage","Altered tpage slot map accepted")
	var no_epoch := State.new(); no_epoch.restore(s.projection()); no_epoch.frame_count = s.frame_count; no_epoch.opening.erase("media")
	var before: Dictionary = no_epoch.opening.duplicate(true)
	var refused: Array = _fx(no_epoch,"actor_draw")
	check(refused[0].error == Stages.TPAGE_EPOCH_MISSING and no_epoch.opening == before,"actor_draw without a media epoch was accepted")
	# Layer guards: a cleared layer in stage15, a foreign cut, an unexecuted cut.
	for broken: String in ["cleared","foreign","unexecuted","bg_unexecuted","bg_rect"]:
		var b := State.new(); b.restore(s.projection()); b.frame_count = s.frame_count
		match broken:
			"cleared": b.opening.bg_layer_0.attribute = 0x80000000; b.opening.bg_layer_0.enabled = 0
			"foreign": b.opening.bg_layer_0.resource_id = Board.CUT_06_ID
			"unexecuted": b.opening.bg_layer_0.cut_batch_key = "never".sha256_text(); b.opening.cut_media.upload_batch_key = "never".sha256_text()
			"bg_unexecuted": b.opening.names_media.buffer_1.upload_batch_key = "never-bg".sha256_text()
			"bg_rect": b.opening.names_media.buffer_1.decode_rect = [704,0,320,160]
		check(not _paint(b)[0].is_empty(),"Stage15 paint accepted a %s layer/background" % broken)
	# An actor without a stage15 owner (O_07 table in stage15) is refused.
	var foreign := State.new(); foreign.restore(s.projection()); foreign.frame_count = s.frame_count
	foreign.opening.actor96.resource_id = Board.O_06_ID
	check(renderer.preflight(foreign) == "stages 15-17 actor96 has no source owner in this stage","Foreign stage15 actor accepted")
	# VRAM overwritten after the pump readback: the receipt no longer holds.
	var saved: PackedByteArray = owned.vram_words.duplicate()
	owned.vram_words[(0*1024+5*64)*2] ^= 0xff
	check(_paint(s)[0] == "stages 15-17 actor page upload is incomplete","Overwritten actor page accepted")
	owned.vram_words = saved; ready.owned = owned
	# Lip sync on line 0x14: mouth frames 6..9 and 10..13 at actor96.x.
	s.substep = 1
	s.opening["prologue_line"] = 0x14
	AudioRules.begin_line_lip_sync(s,0x14,0,[[30,30,30,30]])
	s.opening["cd_audio_state_override"] = 4
	AudioRules.step_lip_sync(s)
	s.frame_count += 1
	check(_fx(s,"lip_sync",1)[0].error.is_empty(),"Lip sync refused")
	s.opening.erase("cd_audio_state_override")
	var lip: Array = _draw(s)
	check(lip[0].error.is_empty() and _run(lip[0]).is_empty(),"Lip page upload refused")
	check(lip[0].presentation.map(func(i): return [i.slot,i.source_offset]) == [[7,0x2c0]],"Mouth page did not take slot7")
	var lip_frame := _paint(s)
	check(lip_frame[0].is_empty() and _same_scene(lip_frame[1],_reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages"))),"Stage15 lip-sync frame differs: " + str(lip_frame[0]))
	_rise_checks(s)
	# Exit slide: every actor reaches x=-400 and leaves only bg + cut.
	s.substep = 26
	while _fx(s,"slide_out")[1] == true: pass
	s.frame_count += 1
	var gone: Array = _draw(s)
	check(gone[0].error.is_empty() and _run(gone[0]).is_empty(),"Exit-slide draw refused")
	var gone_frame := _paint(s)
	check(gone_frame[0].is_empty() and _same_scene(gone_frame[1],_reference(Board.BG_ACEC_ID,Board.CUT_16_ID,"",[])),"Actors at x=-400 still drew: " + str(gone_frame[0]))
	_switch_checks(s)

func _rise_checks(s: State) -> void:
	# 15/10 sets actor100 at y -0x38 (one 64x64 part at part y 96: rows
	# 160-223) and moves the checker frame to OT 9 (D_800B5BD4), actor100's
	# OT; 15/11 raises it one row a frame to -0x78 (prologue.c:2205-2218,
	# 2295-2297). Its message-area rows draw under the checker cells and
	# show between them (rows 160-191 inside the edge columns); the window
	# and text (OT 8 and below) are the HUD's front layer.
	s.substep = 4; _fx(s,"actor99_set")
	s.substep = 10; _fx(s,"actor100_set")
	check(s.opening.actor100.y == -0x38 and s.opening.D_800B5BD4 == 9,"actor100 setup differs from prologue_actor100_set_lines_20_to_31")
	check(renderer.preflight(s,null,true).is_empty(),"dec_bg_draw preflight refused actor100 in the message area: " + renderer.preflight(s,null,true))
	var front_of_window := State.new(); front_of_window.restore(s.projection()); front_of_window.opening.actor100.unk_01 = 9
	check(renderer.preflight(front_of_window,null,true) == Renderer.MESSAGE_ORDER_UNSUPPORTED % [100,8],"An actor in front of the message window was not refused: " + renderer.preflight(front_of_window,null,true))
	var no_ot := State.new(); no_ot.restore(s.projection()); no_ot.opening.erase("D_800B5BD4")
	check(renderer.preflight(no_ot,null,true) == "stages 15-17 checker OT (D_800B5BD4) is not a native byte","A missing checker OT was accepted")
	# Before this frame's draw the previous record still owns frame_count: the
	# after-draw view of it (no actor100 yet) is not this frame's.
	check(renderer.preflight(s).is_empty(),"Previous frame's record was refused: " + renderer.preflight(s))
	var rise: Array = _draw(s)
	check(rise[0].error.is_empty() and _run(rise[0]).is_empty(),"Actor99/100 uploads refused")
	var first := _paint_with_checker(s)
	var first_ref := _reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages"),[128,128,128],9)
	check(first[0].is_empty() and _same_scene(first[1],first_ref),"15/10 frame with actor100 at rows 160-223 differs from the OT model: " + str(first[0]))
	var area_frames := _area_frame_checks(s,first_ref,0)
	var behind := State.new(); behind.restore(s.projection()); behind.opening.D_800B5BD4 = 0xf
	var behind_frame := _paint_with_checker(behind)
	check(behind_frame[0].is_empty() and _same_scene(behind_frame[1],_reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages"),[128,128,128],0xf)),"Checker OT 0xF frame differs from the OT model: " + str(behind_frame[0]))
	var behind_differs := not _same_scene(behind_frame[1],first_ref)
	var compared := 1
	var final_y := 0
	s.substep = 11
	for i in 80:
		var before_y: int = s.opening.actor100.y
		_fx(s,"actor100_rise")
		var step: Array = _draw(s)
		if not step[0].error.is_empty() or not _run(step[0]).is_empty():
			check(false,"Rise frame %d draw refused: %s" % [i,step[0].error]); return
		var y: int = s.opening.actor100.y
		final_y = y
		# Every eighth row and the end of the rise are compared pixel for pixel.
		if (y & 7) == 0 or y == -0x78:
			var painted := _paint_with_checker(s)
			var rise_ref := _reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages"),[128,128,128],int(s.opening.D_800B5BD4))
			check(painted[0].is_empty() and _same_scene(painted[1],rise_ref),"Stage15 rising actor100 frame at y %d differs: %s" % [y,str(painted[0])])
			area_frames += _area_frame_checks(s,rise_ref,y)
			if not behind_differs and int(s.opening.D_800B5BD4) == 9:
				var behind_rise := State.new(); behind_rise.restore(s.projection()); behind_rise.opening.D_800B5BD4 = 0xf
				behind_differs = not _same_scene(_paint_with_checker(behind_rise)[1],rise_ref)
			compared += 1
		if y == -0x78:
			var drawn_without_100: Array = _drawn(s,"o16_tpages").filter(func(a): return a[1] != 100)
			check(not _same_scene(_paint(s)[1],_reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,drawn_without_100)),"Risen actor100 left no pixels")
		if before_y == y: break
	check(final_y == -0x78 and compared >= 9,"Rise did not reach y -0x78 with every eighth row compared: y %d, %d frames" % [final_y,compared])
	# Some compared frame shows actor100 through the checker opening (and
	# only there), and moving the checker behind it (OT 0xF) changes a frame.
	check(area_frames > 0 and behind_differs,"No rise frame shows actor100 in the message area (%d) or separates checker OT 9 from 0xF (%s)" % [area_frames,str(behind_differs)])
	var copy := State.new(); copy.restore(s.projection()); copy.opening.actor100.y = -0x77
	check(renderer.preflight(copy,null,true).is_empty(),"Live preflight refused actor100 at y -0x77: " + renderer.preflight(copy,null,true))

func _area_frame_checks(s: State, ref: Image, y_label: int) -> int:
	# 1 when actor100 shows in the message area of this frame. Its visible
	# message-area pixels must lie in the checker opening (rows 160-191,
	# x 32-287), and one layer for every row (no split) must change the frame.
	var without_100 := _reference(Board.BG_ACEC_ID,Board.CUT_16_ID,Board.O_16_ID,_drawn(s,"o16_tpages").filter(func(a): return a[1] != 100),[128,128,128],int(s.opening.D_800B5BD4))
	var in_area := 0
	var outside := 0
	for y in range(160,240):
		for x in 320:
			if ref.get_pixel(x,y) != without_100.get_pixel(x,y):
				if _checker_cell(x,y): outside += 1
				else: in_area += 1
	check(outside == 0,"actor100 drew over %d checker-cell pixels at y %d" % [outside,y_label])
	if in_area == 0: return 0
	check(not _same_scene(_paint_with_checker(s,false)[1],ref),"Drawing actor100's message-area rows in the scene layer did not change the frame at y %d" % y_label)
	return 1

func _ready_reads(s: State) -> Dictionary:
	# Completed 15/31-33 reads (ACD4, CUT_07, O_07) in the live epoch.
	var epoch: int = s.opening.media.epoch
	s.opening.media.requests = {
		"bg":{"request_id":"bg","epoch":epoch,"kind":"background","status":"ready","resource_id":Board.BG_ACD4_ID,"resource_sha256":Board.BG_ACD4_TRANSFER_SHA,"payload":{"lba":0xacd4,"slot":1,"retry":false}},
		"cut":{"request_id":"cut","epoch":epoch,"kind":"actors","status":"ready","resource_id":Board.CUT_06_ID,"resource_sha256":Board.CUT_06_SHA,"payload":{"lba":0xaa54,"sectors":49,"destination":0x801a4000}},
		"o07":{"request_id":"o07","epoch":epoch,"kind":"actors","status":"ready","resource_id":Board.O_06_ID,"resource_sha256":Board.O_06_SHA,"payload":{"lba":0xabca,"sectors":25,"destination":0x801e8000}}}
	s.opening.media.background_pending = {"1":"bg"}
	s.opening.media.actor_pending = {str(0x801a4000):"cut",str(0x801e8000):"o07"}
	return s.opening.media

func _switch_checks(s: State) -> void:
	# 15/34: decode follows the shown buffer (1 shown -> buffer 0).
	s.substep = 34
	_ready_reads(s)
	var decode := Session.FrameResult.new()
	var buffer := media.background_decode(s,1,decode)
	check(buffer == 0 and decode.error.is_empty() and s.opening.names_media.bg_show_1 == 0 and s.opening.names_media.buffer_0.decode_rect == [704,0,320,160] and s.opening.names_media.buffer_1.lba == 0xacec,"ACD4 did not decode into the hidden buffer 0: " + decode.error)
	var retry_state := State.new(); retry_state.restore(s.projection())
	retry_state.opening.names_media.bg_show_1 = 0
	_ready_reads(retry_state)
	var retry := Session.FrameResult.new()
	check(media.background_decode(retry_state,1,retry) == 1 and retry_state.opening.names_media.buffer_1.decode_rect == [320,256,320,160],"After dec_bg_reset ACD4 did not decode into buffer 1")
	# A tint left by an earlier stage: the slot set's prim_bg_slot_setup
	# writes 0x80 to bg_layers[0].unk_10/11/12 (gpu_effects.c:63-65).
	s.opening["D_801220E0"] = 0x40; s.opening["D_801220E1"] = 0xa0; s.opening["D_801220E2"] = 0xff
	var next: Array = _fx(s,"next_scene")
	check([s.opening.D_801220E0,s.opening.D_801220E1,s.opening.D_801220E2] == [128,128,128],"15/34 slot set kept a stale layer tint")
	var kinds: Array = next[0].presentation.map(func(i): return i.kind)
	check(next[0].error.is_empty() and kinds == ["first_arrival_cut_upload","first_arrival_e8_copy"],"15/34 setup intents differ: " + str(kinds))
	s.next_stage()
	var draw16: Array = _draw(s)
	var pages: Array = draw16[0].presentation.map(func(i): return [i.resource_id,i.slot,i.source_offset])
	check(draw16[0].error.is_empty() and pages == [[Board.O_06_ID,5,0x4310],[Board.O_06_ID,6,0x8310],[Board.O_06_ID,7,0x130]],"Switch-frame O_07 pages differ: " + str(pages))
	var switch := Session.FrameResult.new()
	switch.presentation = decode.presentation + next[0].presentation + draw16[0].presentation
	check(_run(switch).is_empty(),"Switch-frame uploads refused")
	# Upload ownership of the switch frame's pages.
	for intent: Dictionary in draw16[0].presentation:
		check(Upload._stages_15_17_owner(s,intent.resource_id,intent.slot,intent.source_offset,intent.batch_key),"Switch-frame page has no owner")
		check(not Upload._stages_15_17_owner(s,intent.resource_id,intent.slot,intent.source_offset,"x".sha256_text()),"Foreign key owned a switch-frame page")
		check(not Upload._stages_15_17_owner(s,Board.O_16_ID,intent.slot,intent.source_offset,intent.batch_key),"O_16 owned an O_07 page")
	var late := State.new(); late.restore(s.projection()); late.frame_count = s.frame_count+1
	check(not Upload._stages_15_17_owner(late,Board.O_06_ID,5,0x4310,draw16[0].presentation[0].batch_key),"Page owner outlived its frame")
	var switch_frame := _paint(s)
	check(switch_frame[0].is_empty() and _same_scene(switch_frame[1],_reference(Board.BG_ACD4_ID,Board.CUT_06_ID,Board.O_06_ID,_drawn(s,"o07_tpages"))),"Stage16 switch frame differs: " + str(switch_frame[0]))
	# The renderer applies D_801220E0..E2 per channel to the cut cells.
	var tinted := State.new(); tinted.restore(s.projection())
	tinted.opening["D_801220E0"] = 0x40; tinted.opening["D_801220E1"] = 0xa0; tinted.opening["D_801220E2"] = 0xff
	var tinted_frame := _paint(tinted)
	check(tinted_frame[0].is_empty() and _same_scene(tinted_frame[1],_reference(Board.BG_ACD4_ID,Board.CUT_06_ID,Board.O_06_ID,_drawn(s,"o07_tpages"),[0x40,0xa0,0xff])) and not _same_scene(tinted_frame[1],switch_frame[1]),"Per-channel cut tint differs: " + str(tinted_frame[0]))
	tinted.opening["D_801220E1"] = 0x100
	check(_paint(tinted)[0] == "stages 15-17 layer 0 tint is malformed","Out-of-range layer tint accepted")
	# Yoshio lip sync (actor97 frame 6-ring).
	s.substep = 1; s.frame_count += 1
	s.opening["prologue_line"] = 0x20
	AudioRules.begin_line_lip_sync(s,0x20,0,[[60,60,60,60]])
	s.opening["cd_audio_state_override"] = 4
	AudioRules.step_lip_sync(s)
	check(_fx(s,"lip_sync",1)[0].error.is_empty(),"Yoshio lip sync refused")
	s.opening.erase("cd_audio_state_override")
	var yd: Array = _draw(s)
	check(yd[0].error.is_empty() and yd[0].presentation.is_empty(),"Yoshio mouth on resident 0x8310 uploaded a page")
	var y_frame := _paint(s)
	check(y_frame[0].is_empty() and _same_scene(y_frame[1],_reference(Board.BG_ACD4_ID,Board.CUT_06_ID,Board.O_06_ID,_drawn(s,"o07_tpages"))),"Stage16 lip frame differs: " + str(y_frame[0]))
	_stage16_17_checks(s)

func _stage16_17_checks(s: State) -> void:
	# 16/11: P_01_10 into hidden buffer 1, clear_scene, stage17.
	s.substep = 11
	var epoch: int = s.opening.media.epoch
	s.opening.media.requests["p10"] = {"request_id":"p10","epoch":epoch,"kind":"background","status":"ready","resource_id":Board.BG_496A_ID,"resource_sha256":Board.BG_496A_TRANSFER_SHA,"payload":{"lba":0x496a,"slot":1,"retry":false}}
	s.opening.media.background_pending = {"1":"p10"}
	var failed := State.new(); failed.restore(s.projection())
	failed.opening.media.requests.p10.status = "retry"
	check(media.background_decode(failed,1,Session.FrameResult.new()) == -1 and failed.opening.names_media.bg_show_1 == 0,"Failed P_01_10 read decoded")
	var decode := Session.FrameResult.new()
	check(media.background_decode(s,1,decode) == 1 and s.opening.names_media.buffer_1.lba == 0x496a,"P_01_10 did not decode into buffer 1")
	var cleared: Array = _fx(s,"clear_scene")
	check(cleared[0].error.is_empty(),"16/11 clear refused: " + cleared[0].error)
	s.next_stage()
	var tail: Array = _draw(s)
	check(tail[0].error.is_empty() and tail[0].presentation.is_empty() and _run(decode).is_empty(),"Stage17 entry uploads differ")
	var f17 := _paint(s)
	check(f17[0].is_empty() and _same_scene(f17[1],_reference(Board.BG_496A_ID,"","",[])),"Stage17 entry differs from P_01_10 alone: " + str(f17[0]))
	# A layer still live in stage17 is refused.
	var live := State.new(); live.restore(s.projection()); live.opening.bg_layer_0.attribute = 0x41000000; live.opening.bg_layer_0.enabled = 1
	check(not renderer.preflight(live).is_empty(),"Stage17 accepted a live layer 0")
	var leftover := State.new(); leftover.restore(s.projection())
	leftover.opening["actor97"] = {"profile":Stages.O07_PROFILE,"index":97,"resource_id":Board.O_06_ID}
	check(not renderer.preflight(leftover).is_empty(),"Stage17 accepted a leftover actor")
	# 17/3: P_01_12 into the hidden buffer 0.
	s.substep = 3
	s.opening.media.requests["p12"] = {"request_id":"p12","epoch":epoch,"kind":"background","status":"ready","resource_id":Board.BG_4973_ID,"resource_sha256":Board.BG_4973_TRANSFER_SHA,"payload":{"lba":0x4973,"slot":1,"retry":false}}
	s.opening.media.background_pending = {"1":"p12"}
	var d12 := Session.FrameResult.new()
	check(media.background_decode(s,1,d12) == 0 and _run(d12).is_empty() and s.opening.names_media.buffer_0.decode_rect == [704,0,320,160],"P_01_12 did not decode into buffer 0")
	var f17b := _paint(s)
	check(f17b[0].is_empty() and _same_scene(f17b[1],_reference(Board.BG_4973_ID,"","",[])),"Stage17 P_01_12 frame differs: " + str(f17b[0]))
	# A foreign-epoch read does not decode.
	var stale := State.new(); stale.restore(s.projection()); stale.opening.media.epoch += 1
	var stale_out := Session.FrameResult.new()
	check(media.background_decode(stale,1,stale_out) == -1 and stale_out.error == "opening background decode has no matching completed 4973 read","Foreign-epoch P_01_12 read decoded")
	_schema_and_pump_checks(s)

func _schema_and_pump_checks(s: State) -> void:
	var intent := {"kind":"first_arrival_names_bg_upload","batch_key":"a".sha256_text(),"resource_id":Board.BG_496A_ID,"buffer":0,"decode_rect":[320,256,320,160],"word_sha256":Board.BG_496A_WORD_SHA}
	check(not media._intent_schema(intent).is_empty(),"Names upload with the other buffer's rectangle accepted")
	intent.decode_rect = [704,0,320,160]
	check(media._intent_schema(intent).is_empty(),"P_01_10 buffer0 upload refused")
	check(not media._intent_schema({"kind":"first_arrival_e8_copy","resource_id":Board.O_16_ID,"sha256":Board.O_16_SHA,"bytes":0x10800}).is_empty(),"e8 copy accepted another extent")
	check(not media._intent_schema({"kind":"first_arrival_e8_copy","resource_id":Board.O_11_ID,"sha256":Board.O_11_SHA,"bytes":0xc800}).is_empty(),"e8 copy accepted O_11 under the stage14/15 kind")
	check(not media._intent_schema({"kind":"first_arrival_stage_actor_upload","batch_key":"b".sha256_text(),"resource_id":Board.O_16_ID,"slot":8,"source_offset":0x2c0}).is_empty(),"O_16 page accepted in cut-locked slot 8")
	var before: PackedByteArray = owned.vram_words.duplicate()
	check(not pump.execute_stage_actor_vram(board,Board.O_16_ID,9,0x2c0,"c".sha256_text(),owned).error.is_empty() and owned.vram_words == before,"Pump wrote an O_16 page over the cut slot")
	check(not pump.execute_stage_actor_vram(board,Board.O_16_ID,5,0x4310,"d".sha256_text(),owned).error.is_empty(),"Pump accepted an O_07 offset as an O_16 page")
	check(not pump.execute_names_bg(board,Board.BG_AD00_ID,"e".sha256_text(),owned,0).error.is_empty(),"Pump accepted AD00 into buffer 0")
	# arrival_draw: the stage14->15 commit frame stays stage14's draw.
	var commit := _entry(); commit.substep = 0
	# dec_bg_draw runs in the mode handler, before frame_count's increment;
	# committed_frame names the draw frame (frame_count + 1).
	commit.opening.actor96["committed_frame"] = commit.frame_count + 1
	var visible := Visible.new(); visible.renderer = renderer
	var result := Session.FrameResult.new()
	media._store = Store.new()
	media.arrival_draw(commit,1,result,visible)
	check(result.error.begins_with("stages 12-14 "),"Commit frame was drawn as stage15: " + result.error)
	var view := _entry()
	result = Session.FrameResult.new()
	media.arrival_draw(view,1,result,visible)
	check(result.error.is_empty() and result.presentation == [{"kind":"first_arrival_background_draw","slot":1}],"Stage15 dec_bg_draw refused: " + result.error)
	result = Session.FrameResult.new()
	media.arrival_draw(view,1,result,null)
	check(result.error == "visible stages 15-17 have no prepared renderer","Stage15 dec_bg_draw accepted without a renderer")
	result = Session.FrameResult.new()
	media.arrival_draw(view,0,result,visible)
	check(result.error == "stages 15-17 dec_bg_draw requires slot 1 and the board media","Stage15 dec_bg_draw accepted slot 0")
