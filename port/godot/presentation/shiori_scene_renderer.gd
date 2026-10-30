extends RefCounted
## Stages 12-14 visible scene (wiki/spec/opening-stages-12-14.md). One frame is
## dec_bg_draw(1) of the shown slot-1 buffer (OT13, background_images.c:187),
## the CUT layer of bg_layers[0] (order 0xD, OT12, prim_bg_slot_setup cells)
## and the actors actor_draw_all drew this frame (OT unk_01-1). Every texel
## and CLUT entry is read back from owned VRAM that the native GPU pump filled
## from the original transfers; nothing is drawn from a decoded preview.
const Board = preload("res://services/first_arrival_board_store.gd")
const ShioriActors = preload("res://services/shiori_actor_model.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const VRAM_BYTES := 1024*512*2
# dec_bg_decode_to_vram buffers: 0 at VRAM (0x2C0,0), 1 at (0x140,0x100).
const BUFFER_ORIGIN := {0: Vector2i(704,0), 1: Vector2i(320,256)}
const SCREEN := Vector2i(320,160)
const MESSAGE_AREA_UNSUPPORTED := "stages 12-14 actor%d reaches the message area (row >= 160), which no stage 12-14 frame draws"
# mode_stage of the drawn frame -> [shown resource, lba, slot-1 buffer]
# (stage 15 is stage14's commit frame).
const SHOWN_BACKGROUNDS := {
	12: [Board.BG_AD05_ID, 0xad05, 0],
	13: [Board.BG_AD0B_ID, 0xad0b, 1],
	14: [Board.BG_AD05_ID, 0xad05, 0],
	15: [Board.BG_ACEC_ID, 0xacec, 1],
}
var bytes_by_id: Dictionary = {}
var _base: Image
var _base_key := ""
var _painted: Array = []

func prepare(board_store: Board) -> String:
	if not bytes_by_id.is_empty(): return "Shiori scene renderer already owns its resources"
	if board_store == null or not board_store.stages_12_14_ready(): return "Shiori scene renderer needs the combined board pack"
	var out: Dictionary = {}
	for spec: Array in Board.STAGES_12_14_TRANSFERS:
		var item := board_store.stage_transfer(spec[0])
		if not item.get("error") is String or not item.error.is_empty() or not item.get("bytes") is PackedByteArray or Store._sha(item.bytes) != spec[3]:
			return "Shiori scene renderer resource differs from source"
		out[spec[0]] = {"bytes":item.bytes,"sha256":spec[3]}
	bytes_by_id = out
	return ""

static func _cut_for(state) -> String:
	# CUT_14 is visible from stage12's commit until stage13's commit hides
	# it; CUT_16 from stage14's commit (prologue.c:1862, :1960, :2122).
	if state.mode_stage == 13: return Board.CUT_14_ID
	if state.mode_stage == 15: return Board.CUT_16_ID
	return ""

func scene_error(state, mode_time: bool = false) -> String:
	# Source owners only (no VRAM): what dec_bg_draw(1) and the bg layer show.
	# mode_time: dec_bg_draw inside the stage body, before frame_count's
	# post-handler increment (ShioriActors.visible_scene).
	if bytes_by_id.is_empty(): return "Shiori scene renderer is not prepared"
	if not ShioriActors.visible_scene(state,mode_time): return "Shiori scene renderer is outside stages 12-14"
	var names_media: Variant = state.opening.get("names_media")
	if not names_media is Dictionary or names_media.get("bg_show_1") not in [0,1]:
		return "Shiori scene lacks its slot-1 show state"
	var shown: Variant = names_media.get("buffer_%d" % names_media.bg_show_1)
	if not shown is Dictionary: return "Shiori scene lacks the shown slot-1 buffer"
	# [resource, lba, buffer] dec_bg_draw(1) shows: stage11's P_01_08 in
	# buffer 0 until stage12's commit and again from stage13's
	# dec_bg_show_switch (prologue.c:1962) to stage14's commit, AD0B and
	# ACEC in buffer 1 from their decodes. A dec_bg_reset clears only the
	# cache LBA (lba 0); the decoded pixels stay in VRAM (vram_lba).
	var expected: Variant = SHOWN_BACKGROUNDS.get(state.mode_stage)
	if not expected is Array or names_media.bg_show_1 != expected[2] or shown.get("resource_id") != expected[0] or not (shown.get("lba") == expected[1] or (shown.get("lba") == 0 and shown.get("vram_lba") == expected[1])) or shown.get("decode_rect") != Board.SLOT1_BUFFER_RECTS[expected[2]] or not shown.get("upload_batch_key") is String or shown.upload_batch_key.is_empty():
		return "Shiori scene shows a slot-1 buffer outside its source sequence"
	var brightness: Variant = state.opening.get("bg_brightness")
	if not brightness is int or brightness < 0 or brightness > 255: return "Shiori scene has no owned bg_brightness"
	var layer: Variant = state.opening.get("bg_layer_0")
	if not layer is Dictionary: return "Shiori scene lacks its bg layer 0"
	var cut := _cut_for(state)
	if layer.get("attribute") == 0x41000000:
		if cut.is_empty() or layer.get("enabled") != 1 or layer.get("resource_id") != cut or layer.get("clut_row") != 495 or not layer.get("cut_batch_key") is String or layer.cut_batch_key.is_empty():
			return "Shiori scene CUT layer differs from its source slot set"
		for component in ["D_801220E0","D_801220E1","D_801220E2"]:
			var tint: Variant = state.opening.get(component)
			if not tint is int or tint < 0 or tint > 255: return "Shiori scene CUT layer colour is not owned"
	elif layer.get("attribute") == 0x80000000:
		# bg_layers = 0x80000000 (stage13 substep4): display-off bit 31.
		if not cut.is_empty() or layer.get("enabled") != 0: return "Shiori scene hid a CUT layer the source shows"
	else:
		return "Shiori scene bg layer attribute is outside the source"
	var col: Variant = state.opening.get("screen_brightness")
	if not col is int or col < 0 or col > 255: return "Shiori scene has no owned draw brightness"
	return ""

func logical_error(state) -> String:
	# After the mode handler: this frame's actor_draw_all must have run.
	var error := scene_error(state)
	if not error.is_empty(): return error
	var drawn: Variant = state.opening.get("shiori_draw")
	var tpages: Variant = state.opening.get("shiori_tpages")
	if not drawn is Dictionary or drawn.get("frame_count") != state.frame_count or not tpages is Dictionary or tpages.get("frame_count") != state.frame_count:
		return "Shiori scene has no actor_draw_all record for this frame"
	return ""

static func _ready(upload, key: Variant) -> String:
	if upload == null or not upload.has_method("batch_ready"): return "Shiori scene has no upload owner"
	if not key is String or key.is_empty(): return "Shiori scene texture has no upload owner"
	var ready: Variant = upload.batch_ready(key)
	return ready if ready is String else "Shiori scene upload owner returned malformed readiness"

func _actor_sprites(state, upload) -> Variant:
	# The sprites actor_draw_all queued, in GPU draw order. Within one OT
	# slot AddPrim head-inserts, so later actors (higher index) and earlier
	# parts draw first; a higher OT index is farther back.
	var area: Variant = state.opening.get("actor_area")
	var tpages: Dictionary = state.opening.shiori_tpages
	var sprites: Array = []
	for record: Variant in state.opening.shiori_draw.get("actors",[]):
		if not record is Dictionary: return "Shiori scene actor record is malformed"
		var flags: int = int(record.get("draw_flags",0))
		if flags not in [0xa4,0xac] or record.get("quad_flags") != 0x41000000 or record.get("unk_05") != 0x80:
			return "Shiori scene actor uses a draw mode outside the 8-bit sprite path"
		var frame := ShioriActors.frame_parts(bytes_by_id,area,int(record.frame_table),int(record.frame))
		if not frame.error.is_empty(): return frame.error
		var count: int = frame.parts.size()
		if (flags & 8) != 0 and count > 4: count = 4
		var ot: int = (int(record.unk_01)-1) & 0xffff if int(record.unk_01) != 0 else 0
		for j in count:
			var part: Dictionary = frame.parts[j]
			if part.flip != 0: return "Shiori scene part requires an unmodelled flip"
			var page := ShioriActors.word(bytes_by_id,area,int(record.tpage_table)+((part.texture & 0xffff) >> 6)*4)
			if page < 0: return ShioriActors.AREA_UNKNOWN
			var slot: Variant = tpages.get("slots",{}).get("0x%x" % page)
			if not slot is int: return "Shiori scene page has no search_tpage slot this frame"
			var resident: Variant = tpages.get("resident",{}).get(str(slot))
			if not resident is Dictionary or resident.get("page") != page:
				return "Shiori scene slot does not hold the drawn page"
			var ready := _ready(upload,resident.get("key"))
			if not ready.is_empty(): return ready
			var uv: int = part.texture & 0x3f
			var u: int = (uv & 7)*16
			var v: int = (uv >> 3)*16 + (slot & 0x20)*4
			if u + int(part.w) > 128 or v + int(part.h) > 256: return "Shiori scene part wraps its texture page"
			# GsSPRITE at (x,y) anchored at (ox-part.x, oy-part.y); draw_flags
			# bit 2 subtracts the frame origin (actor_sprites.c:168-171).
			var shift_x: int = int(frame.origin_x) if (flags & 4) != 0 else 0
			var shift_y: int = int(frame.origin_y) if (flags & 4) != 0 else 0
			# Rows 160-239 layer with the checker frame and the message
			# window by OT (opening_stages_15_17_renderer.gd); no stage 12-14
			# part reaches them, so one that does is refused, never clipped.
			var left: int = 160+int(record.x)-shift_x+int(part.x)
			if 120+int(record.y)-shift_y+int(part.y)+int(part.h) > SCREEN.y and left < 320 and left+int(part.w) > 0:
				return MESSAGE_AREA_UNSUPPORTED % int(record.index)
			sprites.append({"ot":ot,"index":int(record.index),"part":j,
				"x":160+int(record.x)-shift_x+int(part.x),"y":120+int(record.y)-shift_y+int(part.y),"w":int(part.w),"h":int(part.h),
				"vram_x":(slot & 15)*64,"vram_y":((slot >> 4) & 1)*256+v,"u":u,"clut_row":((part.palette & 0xf00) >> 8)+0x1f0})
	sprites.sort_custom(func(a,b):
		if a.ot != b.ot: return a.ot > b.ot
		if a.index != b.index: return a.index > b.index
		return a.part < b.part)
	return sprites

static func _channel(value: int, scale: int) -> int:
	return mini(31,(value*scale) >> 7)

static func _rgb(red: int, green: int, blue: int) -> Color:
	return Color8((red<<3)|(red>>2),(green<<3)|(green>>2),(blue<<3)|(blue>>2))

func _paint_base(state, vram: PackedByteArray, known: PackedByteArray) -> Variant:
	var image := Image.create(SCREEN.x,SCREEN.y,false,Image.FORMAT_RGBA8)
	var names_media: Dictionary = state.opening.names_media
	var origin: Vector2i = BUFFER_ORIGIN[int(names_media.bg_show_1)]
	var bri: int = int(state.opening.bg_brightness)
	for y in SCREEN.y:
		for x in SCREEN.x:
			var at: int = ((origin.y+y)*1024+origin.x+x)*2
			if known[at] != 255 or known[at+1] != 255: return "Shiori scene background references unknown VRAM"
			var c: int = vram.decode_u16(at)
			image.set_pixel(x,y,_rgb(_channel(c & 31,bri),_channel((c >> 5) & 31,bri),_channel((c >> 10) & 31,bri)))
	var layer: Dictionary = state.opening.bg_layer_0
	if layer.attribute != 0x41000000: return image
	var tint := [int(state.opening.D_801220E0),int(state.opening.D_801220E1),int(state.opening.D_801220E2)]
	# Cells: 3x2 of 128x120 at (-160,-120), u 0, v row*120, tpages 8-10 over
	# VRAM x 512..703 and rows 0..255 (slots 8-10 and 40-42), CLUT row 0x1EF.
	for y in SCREEN.y:
		for x in SCREEN.x:
			var at: int = (y*1024+512+(x >> 7)*64)*2+(x & 127)
			if known[at] != 255: return "Shiori scene CUT layer references unknown VRAM"
			var clut: int = (int(layer.clut_row)*1024+vram[at])*2
			if known[clut] != 255 or known[clut+1] != 255: return "Shiori scene CUT CLUT references unknown VRAM"
			var c: int = vram.decode_u16(clut)
			if c == 0: continue
			var red := _channel(c & 31,tint[0]); var green := _channel((c >> 5) & 31,tint[1]); var blue := _channel((c >> 10) & 31,tint[2])
			if (c & 0x8000) != 0:
				var back := image.get_pixel(x,y)
				red = ((back.r8 >> 3)+red) >> 1; green = ((back.g8 >> 3)+green) >> 1; blue = ((back.b8 >> 3)+blue) >> 1
			image.set_pixel(x,y,_rgb(red,green,blue))
	return image

static func draw_sprites(frame: Image, sprites: Array, vram: PackedByteArray, known: PackedByteArray, col: int) -> String:
	# The queued actor sprites over `frame`, in the order _actor_sprites gives.
	for sprite: Dictionary in sprites:
		for y in int(sprite.h):
			var py: int = int(sprite.y)+y
			if py < 0 or py >= SCREEN.y: continue
			for x in int(sprite.w):
				var px: int = int(sprite.x)+x
				if px < 0 or px >= SCREEN.x: continue
				var at: int = ((int(sprite.vram_y)+y)*1024+int(sprite.vram_x))*2+int(sprite.u)+x
				if known[at] != 255: return "Shiori scene actor references unknown texture memory"
				var clut: int = (int(sprite.clut_row)*1024+vram[at])*2
				if known[clut] != 255 or known[clut+1] != 255: return "Shiori scene actor references unknown palette memory"
				var c: int = vram.decode_u16(clut)
				if c == 0: continue
				# draw_flags 0x20: tint = draw_brightness_get() (screen_brightness).
				var red := _channel(c & 31,col); var green := _channel((c >> 5) & 31,col); var blue := _channel((c >> 10) & 31,col)
				if (c & 0x8000) != 0:
					var back := frame.get_pixel(px,py)
					red = ((back.r8 >> 3)+red) >> 1; green = ((back.g8 >> 3)+green) >> 1; blue = ((back.b8 >> 3)+blue) >> 1
				frame.set_pixel(px,py,_rgb(red,green,blue))
	return ""

func paint(target: Image, state, runtime: Dictionary, upload) -> String:
	if target == null or target.get_size() != Vector2i(320,240): return "Shiori scene target is unsupported"
	var error := logical_error(state)
	if not error.is_empty(): return error
	if not runtime.get("vram_words") is PackedByteArray or runtime.vram_words.size() != VRAM_BYTES or not runtime.get("vram_known_mask") is PackedByteArray or runtime.vram_known_mask.size() != VRAM_BYTES:
		return "Shiori scene requires owned VRAM and its known mask"
	var vram: PackedByteArray = runtime.vram_words
	var known: PackedByteArray = runtime.vram_known_mask
	var names_media: Dictionary = state.opening.names_media
	var shown: Dictionary = names_media["buffer_%d" % names_media.bg_show_1]
	error = _ready(upload,shown.upload_batch_key)
	if not error.is_empty(): return error
	var layer: Dictionary = state.opening.bg_layer_0
	if layer.attribute == 0x41000000:
		error = _ready(upload,layer.cut_batch_key)
		if not error.is_empty(): return error
	var sprites: Variant = _actor_sprites(state,upload)
	if sprites is String: return sprites
	var base_key := JSON.stringify([shown.upload_batch_key,state.opening.bg_brightness,layer.attribute,layer.get("cut_batch_key"),state.opening.get("D_801220E0"),state.opening.get("D_801220E1"),state.opening.get("D_801220E2")])
	if _base == null or _base_key != base_key:
		var base: Variant = _paint_base(state,vram,known)
		if base is String: return base
		_base = base
		_base_key = base_key
	var frame := _base.duplicate() as Image
	error = draw_sprites(frame,sprites,vram,known,int(state.opening.screen_brightness))
	if not error.is_empty(): return error
	target.blit_rect(frame,Rect2i(Vector2i.ZERO,SCREEN),Vector2i.ZERO)
	_painted = [target.get_instance_id(),state.frame_count]
	return ""

func painted_this_frame(target: Image, state) -> bool:
	return target != null and _painted == [target.get_instance_id(),state.frame_count]
