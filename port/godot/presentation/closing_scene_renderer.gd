extends RefCounted
## Stages 18-20 frame composition (prologue.c:2565-2706, wiki/spec/opening-stages-18-20.md).
## OT order, back to front (AddPrim head-inserts, so a later add on one tag
## draws first):
##   0xD  dec_bg_draw(1) of P_01_12 (added last by each _run, drawn first),
##        then prologue_white_boxes_draw (opaque 0xFFFFFF over 320x160);
##   0xB  actor96 (O_13, unk_01 0x0C), added by draw_frame after the mode
##        handler, then the flash boxes' ABR1 additive dithered G4s.
## The HUD and message window (smaller tags) are drawn by the caller.
## Everything is read from owned VRAM (uploaded P_01_12 words, O_13 pages in
## the search_tpage slots, CLUT row 0x1FB) and this committed frame's records.
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const AdditiveFlash = preload("res://presentation/additive_flash.gd")
# dec_bg_decode_to_vram(1) rectangles of slot-1 buffers 0 and 1.
const SHOWN_RECTS := [[704,0,320,160],[320,256,320,160]]
const CLUT_ROW := 0x1fb
var _o13 := PackedByteArray()
var _frames: Array = []

func prepare(source: Variant) -> String:
	_o13 = PackedByteArray(); _frames = []
	if not source is PackedByteArray or source.size() != Board.O13_BYTES or Store._sha(source) != Board.O_13_SHA:
		return "stage19 renderer requires the original O_13 bytes"
	var bytes: PackedByteArray = source
	var frames: Array = []
	for index in 7:
		var offset := int(bytes.decode_u32(0x204+index*4))-0x80180000
		if offset < 0 or offset+8 > bytes.size(): return "O_13 frame pointer is outside the resource"
		var count := bytes.decode_s16(offset)
		# Every O_13 frame has origin (0,0); a nonzero origin would need the
		# func_80098680 anchor path, which this renderer does not model.
		if count < 1 or count > 5 or offset+8+count*14 > bytes.size() or bytes.decode_s16(offset+2) != 0 or bytes.decode_s16(offset+4) != 0:
			return "O_13 frame header is outside the modelled source shape"
		var parts: Array = []
		for part_index in count:
			var row := offset+8+part_index*14
			var texture := bytes.decode_u16(row+4)
			var page := texture >> 6
			var u := (texture&7)*16
			var v := ((texture>>3)&7)*16
			var w := bytes.decode_s16(row+10)
			var h := bytes.decode_s16(row+12)
			if page >= 8 or bytes.decode_u16(row+6) != 0x1b00 or bytes.decode_u16(row+8) != 0 or w <= 0 or h <= 0 or u+w > 128 or v+h > 128:
				return "O_13 part needs an unmodelled page, palette, flip or extent"
			parts.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),"u":u,"v":v,"w":w,"h":h,
				"page_offset":int(bytes.decode_u32(0x1e4+page*4))-0x80180000})
		frames.append(parts)
	_o13 = bytes.duplicate()
	_frames = frames
	return ""

func paint(target: Image, state, runtime: Dictionary, upload) -> String:
	if _o13.is_empty(): return "stage19 renderer has no prepared O_13 source"
	# Committed 21/0 with line 42 is the 20->21 frame: prologue_closing_run
	# case2 switches stage and then runs dec_bg_draw(1) (prologue.c:2697-2705).
	if target == null or target.get_size() != Vector2i(320,240) or state == null or state.game_mode != 0x15 or state.mode_step != 0 or not (state.mode_stage in [18,19,20] or (state.mode_stage == 21 and state.substep == 0 and state.opening.get("prologue_line") == 42)):
		return "closing renderer is outside stages 18-20"
	var vram: Variant = runtime.get("vram_words")
	var known: Variant = runtime.get("vram_known_mask")
	if not vram is PackedByteArray or not known is PackedByteArray or vram.size() != 1024*512*2 or known.size() != vram.size() or upload == null or not upload.has_method("batch_ready"):
		return "closing renderer requires owned VRAM and its upload owner"
	var o: Dictionary = state.opening
	# 1. dec_bg_draw(1): P_01_12 in the buffer stage17 case3 decoded into
	# (1 - dec_bg_show[1], buffer 0 on the uninterrupted path after stage16
	# showed P_01_10 in buffer 1), tinted by bg_brightness.
	var names: Variant = o.get("names_media")
	var show: Variant = names.get("bg_show_1") if names is Dictionary else null
	var shown: Variant = names.get("buffer_%d" % show) if show is int and show in [0,1] else null
	if not shown is Dictionary or shown.get("lba") != 0x4973 or shown.get("resource_id") != Board.BG_4973_ID or shown.get("word_sha256") != Board.BG_4973_WORD_SHA or not Store._same(shown.get("decode_rect"),SHOWN_RECTS[show]) or not shown.get("upload_batch_key") is String:
		return "closing background is not the uploaded P_01_12 buffer"
	var ready: Variant = upload.batch_ready(shown.upload_batch_key)
	if not ready is String or not ready.is_empty(): return ready if ready is String else "closing background upload readiness is unknown"
	var bg_bri: Variant = o.get("bg_brightness",128)
	if not bg_bri is int or bg_bri < 0 or bg_bri > 255: return "closing background brightness is not a native byte"
	var rect: Array = SHOWN_RECTS[show]
	for y in 160:
		var row_base: int = ((int(rect[1])+y)*1024+int(rect[0]))*2
		for x in 320:
			var at: int = row_base+x*2
			if known[at] != 255 or known[at+1] != 255: return "closing background references unknown VRAM"
			var color: int = vram.decode_u16(at)
			target.set_pixel(x,y,_rgb(mini(31,((color&31)*bg_bri)>>7),mini(31,(((color>>5)&31)*bg_bri)>>7),mini(31,(((color>>10)&31)*bg_bri)>>7)))
	# 2. This frame's box records (closing_effect validated them when queued).
	var boxes := _boxes(state)
	if boxes.has("error"): return boxes.error
	if boxes.white:
		target.fill_rect(Rect2i(0,0,320,160),Color8(255,255,255))
	# 3. actor96 when actor_draw_all drew it this frame.
	var error := _paint_actor(target,state,vram,known,upload)
	if not error.is_empty(): return error
	# 4. Additive flash on top of the actor (same OT tag, queued earlier).
	if boxes.flash >= 0: return AdditiveFlash.paint(target,boxes.flash)
	return ""

func _boxes(state) -> Dictionary:
	# Returns {white, flash level or -1} or {error}. Records exist only in
	# stage 19 frames; the drawn counter is the committed one -/+ 4.
	var draws: Variant = state.opening.get("prologue_closing_draws",[])
	if not draws is Array or draws.size() > 2: return {"error":"closing box records are malformed"}
	if draws.is_empty():
		# Only stage 18/20 frames (and the 20->21 frame) and stage 19's entry
		# frame (stage18 case9) draw no boxes.
		if state.mode_stage == 19 and not (state.substep == 0 and state.opening.get("prologue_scene_counter") == 0):
			return {"error":"stage19 frame lacks its flash boxes"}
		return {"white":false,"flash":-1}
	if state.mode_stage != 19:
		return {"error":"closing boxes are outside stage19"}
	var white: bool = draws.size() == 2
	if white and not Store._same(draws[0],{"tag":0xd,"color":0xffffff,"mode":1,"rects":[[-160,-120,160,160],[0,-120,160,160]]}):
		return {"error":"closing white boxes differ from prologue_white_boxes_draw"}
	var flash: Variant = draws[-1]
	if not flash is Dictionary or flash.get("tag") != 0xb or flash.get("mode") != 0x81 or not Store._same(flash.get("draw_mode"),{"tag":0xb,"abr":1,"tp":1}) or not Store._same(flash.get("rects"),[[-160,-120,160,160],[0,-120,160,160]]) or not flash.get("color") is int:
		return {"error":"closing flash boxes differ from prologue_flash_boxes_draw"}
	var color: int = flash.color
	var level := 255 if color == 0xffffff else color & 0xff
	var counter := 0x100 if color == 0xffffff else level
	if color != 0xffffff and (color != level*0x10101 or level % 4 != 0):
		return {"error":"closing flash colour is not counter * 0x10101"}
	var committed: Variant = state.opening.get("prologue_scene_counter")
	if not committed is int or absi(committed-counter) != 4:
		return {"error":"closing flash boxes are not owned by this committed frame"}
	return {"white":white,"flash":level}

func _paint_actor(target: Image, state, vram: PackedByteArray, known: PackedByteArray, upload) -> String:
	var o: Dictionary = state.opening
	var actor: Variant = o.get("actor96")
	var drawn: Variant = o.get("o13_draw")
	if not actor is Dictionary or (int(actor.get("draw_flags",0)) & 0x80) == 0:
		return "" if drawn == null else "hidden actor96 has a draw record"
	if actor.get("profile") != "o13_flash" or actor.get("draw_flags") != 0xa4 or actor.get("x") != -0xa0 or actor.get("y") != -0x78 or actor.get("unk_05") != 0x80 or actor.get("clut_row") != CLUT_ROW:
		return "closing actor96 is not the stage19 O_13 setup"
	if not drawn is Dictionary or drawn.get("frame_count") != state.frame_count or not drawn.get("frame") is int or drawn.frame < 0 or drawn.frame >= _frames.size():
		return "closing actor96 needs this committed frame's draw record"
	# Case2's exit frame sets substep 3 and hides actor96 before draw_frame
	# (prologue.c:2655-2663): a visible actor at 19/2 with the counter at
	# 0x100 is not a source state.
	var counter: Variant = o.get("prologue_scene_counter")
	if state.mode_stage == 19 and state.substep == 2 and counter is int and counter >= 0x100:
		return "closing actor96 is visible on the stage19 case2 exit frame"
	# draw_flags 0x20: tint by draw_brightness_get() = game_state.screen_brightness.
	var tint: Variant = o.get("screen_brightness")
	if not tint is int or tint < 0 or tint > 255: return "closing actor96 tint has no owned brightness"
	var slots := Board.o13_tpage_slots(_o13,int(drawn.frame))
	var tpages: Variant = o.get("o13_tpages")
	if slots.is_empty() or not tpages is Dictionary: return "closing actor96 tpage allocation is missing"
	for page: int in slots:
		var resident: Variant = tpages.get("resident",{}).get(str(slots[page]))
		if drawn.get("slots",{}).get(str(page)) != slots[page] or not resident is Dictionary or resident.get("offset") != page or not resident.get("key") is String:
			return "closing actor96 tpage allocation differs from search_tpage"
		var ready: Variant = upload.batch_ready(resident.key)
		if not ready is String or not ready.is_empty(): return ready if ready is String else "closing actor96 upload readiness is unknown"
	for part: Dictionary in _frames[int(drawn.frame)]:
		var slot: int = slots[part.page_offset]
		var slot_x: int = (slot&15)*64
		var slot_y: int = ((slot>>4)&1)*256+(slot>>5)*128
		for y in int(part.h):
			for x in int(part.w):
				# actor x/y (-160,-120) plus the screen origin (160,120) cancel.
				var px: int = int(part.x)+x
				var py: int = int(part.y)+y
				if px < 0 or px >= 320 or py < 0 or py >= 240: continue
				var address: int = ((slot_y+int(part.v)+y)*1024+slot_x)*2+int(part.u)+x
				if known[address] != 255: return "closing actor96 references unknown texture memory"
				var clut: int = (CLUT_ROW*1024+vram[address])*2
				if known[clut] != 255 or known[clut+1] != 255: return "closing actor96 references unknown palette memory"
				var color := vram.decode_u16(clut)
				if color == 0: continue
				var red: int = mini(31,((color&31)*tint)>>7)
				var green: int = mini(31,(((color>>5)&31)*tint)>>7)
				var blue: int = mini(31,(((color>>10)&31)*tint)>>7)
				if (color&0x8000) != 0:
					var back := target.get_pixel(px,py)
					red = ((back.r8>>3)+red)>>1; green = ((back.g8>>3)+green)>>1; blue = ((back.b8>>3)+blue)>>1
				target.set_pixel(px,py,_rgb(red,green,blue))
	return ""

static func _rgb(red: int, green: int, blue: int) -> Color:
	return Color8((red<<3)|(red>>2),(green<<3)|(green>>2),(blue<<3)|(blue>>2))
