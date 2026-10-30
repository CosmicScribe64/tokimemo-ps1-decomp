extends RefCounted
## Visible draw of opening stages 15-17 (prologue.c:2227-2557,
## wiki/spec/opening-stages-15-17.md): dec_bg_draw(1) of the shown slot-1
## buffer, layer 0 (the cut locked by prologue_background_slot_set) while
## its unk_00 is not negative, then actor_draw_all's sprites. Every pixel is
## read from VRAM the native GPU pump filled from original board bytes; the
## frames, pages and positions are the ones this frame's actor_draw effect
## recorded (opening_stages_15_17.gd). Anything outside that is refused.
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Stages = preload("res://services/opening_stages_15_17.gd")

const RESOURCE_BASE := 0x80180000
# Frame tables of the resident resources: frame table -> [tpage table, frames, pages].
const TABLES := {BoardStore.O_16_ID: Stages.O16_TABLES, BoardStore.O_06_ID: Stages.O07_TABLES}
const TPAGE_KEYS := {BoardStore.O_16_ID: "o16_tpages", BoardStore.O_06_ID: "o07_tpages"}
const LAYER_LIVE := 0x41000000
const LAYER_CLEARED := 0x80000000
# Source scene: 320x160 draw area at the (160,120) origin (dec_bg_draw).
const SCENE_H := 160
const SCREEN_H := 240
# Rows 160-239 (the message area) layer by OT index; ClearOTagR walks the
# table from the highest entry down, each entry from its head (AddPrim
# head-inserts, so a later AddPrim in one entry draws first):
# - hud_screen_frame_draw(2) adds the checker frame sprites (rows 160-191 at
#   the edge columns, rows 192-239 everywhere) at OT D_800B5BD4
#   (hud_panels.c:1457/1461) from the mode handler, before draw_frame's
#   actor_draw_all, so in the same entry an actor part draws first and the
#   checker covers it. prologue_actor100_set_lines_20_to_31 sets 9 (actor100's
#   OT, unk_01 0xA) while actor100 rises from y -0x38; 0xF otherwise.
# - message_window_show's prim_gradient_rect is opaque at OT 8
#   (hud_panels.c:771) and the text at OT >= 1 (text_system.c:296): both in
#   front of every actor at OT >= 9.
# The renderer paints message-area parts with OT >= D_800B5BD4 before the
# checker (UNDER_CHECKER) and the rest with the scene; the HUD's front layer
# then draws the window and text.
const UNDER_CHECKER := "under_checker"
const SCENE_LAYER := "scene"
const MESSAGE_WINDOW_OT := 8
const MESSAGE_ORDER_UNSUPPORTED := "stages 15-17 actor%d reaches the message area at OT %d, in front of the message window (OT 8), which the HUD layer does not model"

var _frames: Dictionary = {}
var _bytes: Dictionary = {}

func prepare(board: BoardStore) -> String:
	_frames.clear(); _bytes.clear()
	if board == null: return "stages 15-17 renderer needs the original board pack"
	for spec: Array in [[BoardStore.O_16_ID,BoardStore.O_16_SHA],[BoardStore.O_06_ID,BoardStore.O_06_SHA]]:
		var bytes: Variant = board.stage_transfer(spec[0]).get("bytes")
		if not bytes is PackedByteArray or bytes.size() != 0xc800 or Store._sha(bytes) != spec[1]:
			return "stages 15-17 renderer requires original %s bytes" % spec[0]
		var tables: Dictionary = TABLES[spec[0]]
		var parsed: Dictionary = {}
		for frame_table: int in tables:
			var decoded := parse_frames(bytes,frame_table,int(tables[frame_table][0]),int(tables[frame_table][1]),int(tables[frame_table][2]))
			if decoded.is_empty(): return "stages 15-17 frame table 0x%x is outside its original resource" % frame_table
			parsed[frame_table] = decoded
		_frames[spec[0]] = parsed
		_bytes[spec[0]] = bytes
	return ""

static func parse_frames(bytes: PackedByteArray, frame_table: int, tpage_table: int, frames: int, pages: int) -> Array:
	# SpriteFrame {count, origin_x, origin_y, pad} then 14-byte parts
	# {x, y, texture, palette, flip, w, h} (actor_sprites.c:110-160).
	var result: Array = []
	var ft := frame_table-RESOURCE_BASE; var tt := tpage_table-RESOURCE_BASE
	if ft < 0 or tt < 0 or ft+frames*4 > bytes.size() or tt+pages*4 > bytes.size(): return []
	for index in frames:
		var offset := int(bytes.decode_u32(ft+index*4))-RESOURCE_BASE
		if offset < 0 or offset+8 > bytes.size(): return []
		var count := bytes.decode_s16(offset)
		if count < 1 or count > 8 or offset+8+count*14 > bytes.size(): return []
		var parts: Array = []
		for part_index in count:
			var row := offset+8+part_index*14
			var texture := bytes.decode_u16(row+4)
			var page := texture >> 6
			if page >= pages: return []
			var page_offset := int(bytes.decode_u32(tt+page*4))-RESOURCE_BASE
			if page_offset < 0 or page_offset+0x4000 > bytes.size(): return []
			parts.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),
				"origin_x":bytes.decode_s16(offset+2),"origin_y":bytes.decode_s16(offset+4),
				"u":(texture&7)*16,"v":((texture>>3)&7)*16,"page_offset":page_offset,
				"palette_flags":bytes.decode_u16(row+6),"flip":bytes.decode_u16(row+8),
				"w":bytes.decode_s16(row+10),"h":bytes.decode_s16(row+12)})
		result.append(parts)
	return result

func parts(resource_id: String, frame_table: int, frame: int) -> Array:
	var tables: Variant = _frames.get(resource_id,{}).get(frame_table)
	if not tables is Array or frame < 0 or frame >= tables.size(): return []
	return tables[frame].duplicate(true)

# ---- preflight (before draw_frame) ------------------------------------

func preflight(state, upload = null, before_actor_draw: bool = false) -> String:
	# before_actor_draw: dec_bg_draw's check inside the stage body, which runs
	# before this frame's actor_draw; otherwise the frame's draw has run.
	if _frames.is_empty(): return "stages 15-17 renderer is not prepared"
	if state == null or state.game_mode != 0x15 or state.mode_stage not in [15,16,17] or state.mode_step != 0:
		return "stages 15-17 renderer is outside its source stages"
	var o: Dictionary = state.opening
	var names: Variant = o.get("names_media")
	if not names is Dictionary or names.get("bg_show_1") not in [0,1] or names.get("active_draw") != "dec_bg_draw":
		return "stages 15-17 dec_bg_draw has no slot-1 show state"
	var shown: Variant = names.get("buffer_%d" % int(names.bg_show_1))
	if not shown is Dictionary or not shown.get("upload_batch_key") is String or shown.upload_batch_key.is_empty() or shown.get("decode_rect") != BoardStore.SLOT1_BUFFER_RECTS[int(names.bg_show_1)]:
		return "stages 15-17 shown background buffer has no decoded upload"
	if not o.get("bg_brightness",128) is int or int(o.get("bg_brightness",128)) < 0 or int(o.get("bg_brightness",128)) > 255:
		return "stages 15-17 background brightness is malformed"
	if upload != null:
		var ready: Variant = upload.batch_ready(shown.upload_batch_key)
		if not ready is String or not ready.is_empty(): return "stages 15-17 shown background upload is incomplete"
	var layer_error := _layer_error(state,upload)
	if not layer_error.is_empty(): return layer_error
	var owner_error := _actor_owner_error(state)
	if not owner_error.is_empty(): return owner_error
	return _message_area_error(state,before_actor_draw)

func _layer_error(state, upload) -> String:
	# Stage15 shows CUT_16 (stage14 slot set) until 15/34, then CUT_07;
	# 16/11 writes bg_layers = 0x80000000 and func_80098D00 skips it.
	var o: Dictionary = state.opening
	var layer: Variant = o.get("bg_layer_0")
	if not layer is Dictionary: return "stages 15-17 layer 0 has no slot-set owner"
	if layer.get("attribute") == LAYER_CLEARED:
		# Only 16/11's success clears it, and that frame already draws as stage17.
		if layer.get("enabled") != 0 or state.mode_stage != 17:
			return "stages 15-17 layer 0 was cleared outside 16/11"
		return ""
	if layer.get("attribute") != LAYER_LIVE or layer.get("enabled") != 1 or layer.get("clut_row") != 495 or layer.get("palette_flag") != 1:
		return "stages 15-17 layer 0 differs from prologue_background_slot_set(0x801A4000,0xF)"
	var expected: String = BoardStore.CUT_16_ID if state.mode_stage == 15 else BoardStore.CUT_06_ID
	var cut: Variant = o.get("cut_media")
	if state.mode_stage == 17 or layer.get("resource_id") != expected or not cut is Dictionary or cut.get("resource_id") != expected:
		return "stages 15-17 layer 0 is not the cut its slot set locked"
	if not layer.get("cut_batch_key") is String or layer.cut_batch_key.is_empty() or cut.get("upload_batch_key") != layer.cut_batch_key:
		return "stages 15-17 layer 0 cut has no upload owner"
	if upload != null:
		var ready: Variant = upload.batch_ready(layer.cut_batch_key)
		if not ready is String or not ready.is_empty(): return "stages 15-17 layer 0 cut upload is incomplete"
	return ""

func _actor_owner_error(state) -> String:
	var o: Dictionary = state.opening
	var resource := _resource(state)
	for index in range(96,160):
		var actor: Variant = o.get("actor%d" % index)
		if actor == null: continue
		if not actor is Dictionary: return "stages 15-17 actor%d record is malformed" % index
		if actor.get("profile") == "cleared" and int(actor.get("draw_flags",0)) == 0: continue
		if resource.is_empty() or actor.get("resource_id") != resource or index > 101:
			return "stages 15-17 actor%d has no source owner in this stage" % index
		if not TABLES[resource].has(actor.get("frame_table")) or TABLES[resource][actor.frame_table][0] != actor.get("tpage_table"):
			return "stages 15-17 actor%d tables are outside the resident resource" % index
		if actor.get("draw_flags") != 0xa4 or o.get("actor%d_draw_flags" % index) != 0xa4 or actor.get("quad_flags") != LAYER_LIVE or actor.get("brightness") != 0x80:
			return "stages 15-17 actor%d draw fields differ from its source setup" % index
		if not actor.get("x") is int or not actor.get("y") is int:
			return "stages 15-17 actor%d position is malformed" % index
	return ""

static func checker_ot(state) -> int:
	# D_800B5BD4, the checker frame's OT entry; -1 when not a native byte.
	var value: Variant = state.opening.get("D_800B5BD4")
	return value if value is int and value >= 0 and value <= 255 else -1

static func actor_ot(unk_01: int) -> int:
	# actor_draw_all sorts each part into OT unk_01-1 (0 when unk_01 is 0).
	return (unk_01-1) & 0xff if unk_01 != 0 else 0

func _message_area_error(state, before_actor_draw: bool) -> String:
	# The frames drawn this frame: this frame's actor_draw record once it
	# ran, otherwise what actor_draw_all will pick from the live actors.
	# A message-area part must sit behind the message window (OT >= 9) and
	# the checker's OT must be a native byte.
	var resource := _resource(state)
	if resource.is_empty(): return ""
	var o: Dictionary = state.opening
	var record: Variant = o.get(TPAGE_KEYS[resource])
	var items: Array = []
	if not before_actor_draw and record is Dictionary and record.get("frame_count") == state.frame_count and record.get("drawn") is Array:
		items = record.drawn
	else:
		for index in range(96,102):
			var actor: Variant = o.get("actor%d" % index)
			if not actor is Dictionary or actor.get("profile") == "cleared" or (int(actor.get("draw_flags",0)) & 0x80) == 0: continue
			var frame := -1
			if (int(actor.get("flags",0)) & 0x40) != 0:
				frame = int(actor.get("frame_index",-1))
			else:
				var rows := Stages.sequence_rows(_bytes[resource],int(actor.get("sequence_table",0)),int(actor.get("sequence_slot",-1)))
				var step := int(actor.get("seq_step",-1))
				if step >= 0 and step < rows.size(): frame = int(rows[step][0])
			items.append({"index":index,"frame_table":actor.get("frame_table"),"frame":frame,"x":actor.x,"y":actor.y,"unk_01":int(actor.get("unk_01",0))})
	for item: Dictionary in items:
		var frame_parts := parts(resource,int(item.frame_table),int(item.frame))
		if frame_parts.is_empty(): return "stages 15-17 actor%d frame has no original parts" % item.index
		for part: Dictionary in frame_parts:
			# A part wholly left or right of the screen puts no pixel there.
			var left: int = 160+int(item.x)+int(part.x)
			if left >= 320 or left+int(part.w) <= 0: continue
			if 120+int(item.y)+int(part.y)+int(part.h) > SCENE_H:
				var ot := actor_ot(int(item.get("unk_01",0)))
				if checker_ot(state) < 0: return "stages 15-17 checker OT (D_800B5BD4) is not a native byte"
				if ot <= MESSAGE_WINDOW_OT: return MESSAGE_ORDER_UNSUPPORTED % [item.index,ot]
	return ""

static func _resource(state) -> String:
	# O_16 until the 15/34 memcpy; O_07 (board id O_06) through stage16.
	var a96: Variant = state.opening.get("actor96")
	if state.mode_stage == 15: return BoardStore.O_16_ID
	if state.mode_stage == 16 and (not a96 is Dictionary or a96.get("resource_id") == BoardStore.O_06_ID): return BoardStore.O_06_ID
	return ""

# ---- paint (after the frame committed and published) ------------------

func paint_background(target: Image, state, runtime: Dictionary, upload) -> String:
	var error := preflight(state,upload)
	if not error.is_empty(): return error
	if target == null or target.get_size() != Vector2i(320,240): return "stages 15-17 target is unsupported"
	if not runtime.get("vram_words") is PackedByteArray or not runtime.get("vram_known_mask") is PackedByteArray or runtime.vram_words.size() != 1024*512*2 or runtime.vram_known_mask.size() != runtime.vram_words.size():
		return "stages 15-17 renderer has no owned VRAM"
	var vram: PackedByteArray = runtime.vram_words; var known: PackedByteArray = runtime.vram_known_mask
	var o: Dictionary = state.opening
	var rect: Array = BoardStore.SLOT1_BUFFER_RECTS[int(o.names_media.bg_show_1)]
	var bright: int = int(o.get("bg_brightness",128))
	for y in SCENE_H:
		var row := ((int(rect[1])+y)*1024+int(rect[0]))*2
		for x in 320:
			var at := row+x*2
			if known[at] != 255 or known[at+1] != 255: return "stages 15-17 background references unknown VRAM"
			var color: int = vram.decode_u16(at)
			target.set_pixel(x,y,_rgb(mini(31,((color&31)*bright)>>7),mini(31,(((color>>5)&31)*bright)>>7),mini(31,(((color>>10)&31)*bright)>>7)))
	var layer: Dictionary = o.bg_layer_0
	if layer.attribute != LAYER_LIVE: return ""
	# prim_bg_slot_setup(addr,0,6,3,2,0x1EF,-0xA0,-0x78,8,8): 3x2 cells of
	# 128x128 8-bit texels in tpages 8-10/40-42 at (512+64i, 128j). The
	# cells take bg_layers[0].unk_10/11/12 (D_801220E0..E2) as r/g/b.
	var tint: Array = []
	for name: String in ["D_801220E0","D_801220E1","D_801220E2"]:
		var channel: Variant = o.get(name,128)
		if not channel is int or channel < 0 or channel > 255: return "stages 15-17 layer 0 tint is malformed"
		tint.append(channel)
	for y in SCENE_H:
		var page_y := 0 if y < 128 else 1
		var v := y if y < 128 else y-128
		for x in 320:
			@warning_ignore("integer_division")
			var at := ((page_y*128+v)*1024+(512+(x/128)*64))*2+(x%128)
			if known[at] != 255: return "stages 15-17 cut layer references unknown VRAM"
			var clut := (int(layer.clut_row)*1024+vram[at])*2
			if known[clut] != 255 or known[clut+1] != 255: return "stages 15-17 cut CLUT references unknown VRAM"
			var c: int = vram.decode_u16(clut)
			if c == 0: continue
			_plot(target,x,y,c,tint[0],tint[1],tint[2])
	return ""

func paint_actors(target: Image, state, runtime: Dictionary, upload, layer: String = SCENE_LAYER) -> String:
	# layer SCENE_LAYER: rows 0-159 of every part and message-area rows of
	# parts in front of the checker (OT < D_800B5BD4); UNDER_CHECKER: the
	# message-area rows of the parts the checker frame covers.
	if layer not in [SCENE_LAYER,UNDER_CHECKER]: return "stages 15-17 actor layer is unknown"
	var error := preflight(state,upload)
	if not error.is_empty(): return error
	# The checker's OT matters only for a part that reaches the message area.
	var checker := checker_ot(state)
	var resource := _resource(state)
	var o: Dictionary = state.opening
	if resource.is_empty():
		for index in range(96,160):
			if o.get("actor%d" % index) is Dictionary and o["actor%d" % index].get("profile") != "cleared": return "stages 15-17 actors have no resident resource"
		return ""
	var key: String = TPAGE_KEYS[resource]
	var record: Variant = o.get(key)
	var visible := []
	for index in range(96,102):
		var actor: Variant = o.get("actor%d" % index)
		if actor is Dictionary and actor.get("profile") != "cleared": visible.append(index)
	if visible.is_empty():
		return "" if record == null else "stages 15-17 tpage record outlived its actors"
	if not record is Dictionary or record.get("frame_count") != state.frame_count or not record.get("drawn") is Array or not record.get("slots") is Dictionary or not record.get("resident") is Dictionary:
		return "stages 15-17 actors need this committed frame's actor_draw record"
	var drawn: Array = record.drawn
	if drawn.map(func(item): return item.get("index")) != visible:
		return "stages 15-17 drawn actors differ from the live actor pool"
	# Recompute search_tpage for the recorded frames and require every page's
	# slot upload, owned by the record, to still hold the original bytes.
	var expected := Stages.tpage_slots(_bytes[resource],TABLES[resource],drawn.map(func(item): return [item.frame_table,item.frame]))
	if expected.is_empty() or expected.size() != record.slots.size():
		return "stages 15-17 tpage allocation differs from search_tpage"
	for page: int in expected:
		var resident: Variant = record.resident.get(str(expected[page]))
		if record.slots.get(str(page)) != expected[page] or not resident is Dictionary or resident.get("offset") != page or resident.get("resource_id") != resource or not resident.get("key") is String:
			return "stages 15-17 tpage allocation differs from search_tpage"
		if upload != null:
			var ready: Variant = upload.batch_ready(resident.key)
			if not ready is String or not ready.is_empty(): return "stages 15-17 actor page upload is incomplete"
	var vram: PackedByteArray = runtime.vram_words; var known: PackedByteArray = runtime.vram_known_mask
	var tint: int = int(o.get("screen_brightness",128))
	# actor_draw_all head-inserts each part at OT unk_01-1: a lower OT is in
	# front; within one OT the lower actor index is in front (inserted first).
	var order: Array = drawn.duplicate()
	order.sort_custom(func(a, b): return [int(a.unk_01),int(a.index)] > [int(b.unk_01),int(b.index)])
	for item: Dictionary in order:
		var frame_parts := parts(resource,int(item.frame_table),int(item.frame))
		if frame_parts.is_empty(): return "stages 15-17 actor%d frame has no original parts" % item.index
		var under_checker: bool = checker >= 0 and actor_ot(int(item.unk_01)) >= checker
		# Parts are inserted count-1..0 at the list head, so part 0 is drawn
		# first and part count-1 ends in front.
		for part: Dictionary in frame_parts:
			if part.flip != 0 or part.origin_x != 0 or part.origin_y != 0: return "stages 15-17 actor part needs unsupported native flip/origin"
			var slot: int = int(record.slots.get(str(part.page_offset),-1))
			if slot < 0: return "stages 15-17 actor texture page has no source slot"
			var slot_x: int = (slot&15)*64; var slot_y: int = ((slot>>4)&1)*256+(slot>>5)*128
			# unk_05 0x80: CLUT row ((palette>>8)&0xF)+0x1F0, column 0.
			var clut_y: int = ((int(part.palette_flags)&0xf00)>>8)+496
			for y in int(part.h):
				var py: int = 120+int(item.y)+int(part.y)+y
				if py < 0 or py >= SCREEN_H: continue
				if py >= SCENE_H and checker < 0: return "stages 15-17 checker OT (D_800B5BD4) is not a native byte"
				# Rows 0-159 always draw with the scene; a message-area row
				# draws in the layer its OT puts it in relative to the checker.
				var in_layer: bool = (layer == SCENE_LAYER and (py < SCENE_H or not under_checker)) or (layer == UNDER_CHECKER and py >= SCENE_H and under_checker)
				if not in_layer: continue
				if py >= SCENE_H and actor_ot(int(item.unk_01)) <= MESSAGE_WINDOW_OT: return MESSAGE_ORDER_UNSUPPORTED % [item.index,actor_ot(int(item.unk_01))]
				for x in int(part.w):
					var px: int = 160+int(item.x)+int(part.x)+x
					if px < 0 or px >= 320: continue
					var at: int = ((slot_y+int(part.v)+y)*1024+slot_x)*2+int(part.u)+x
					if known[at] != 255: return "stages 15-17 actor references unknown texture memory"
					var clut: int = (clut_y*1024+vram[at])*2
					if known[clut] != 255 or known[clut+1] != 255: return "stages 15-17 actor references unknown palette memory"
					var c: int = vram.decode_u16(clut)
					if c == 0: continue
					_plot(target,px,py,c,tint,tint,tint)
	return ""

static func _plot(target: Image, x: int, y: int, c: int, tint_r: int, tint_g: int, tint_b: int) -> void:
	# Modulated texel; stp texels average with the frame (semi mode 0).
	var r: int = mini(31,((c&31)*tint_r)>>7); var g: int = mini(31,(((c>>5)&31)*tint_g)>>7); var b: int = mini(31,(((c>>10)&31)*tint_b)>>7)
	if (c&0x8000) != 0:
		var back := target.get_pixel(x,y)
		r = ((back.r8>>3)+r)>>1; g = ((back.g8>>3)+g)>>1; b = ((back.b8>>3)+b)>>1
	target.set_pixel(x,y,_rgb(r,g,b))

static func _rgb(r: int, g: int, b: int) -> Color:
	return Color8((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2),255)
