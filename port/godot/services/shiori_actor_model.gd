extends RefCounted
## T-39002 stages 12-14 (wiki/spec/opening-stages-12-14.md): the logical part
## of actor_draw_all (actor_sprites.c:63) for actors 96..159 over the
## byte-addressed 0x80180000 actor area. No pixels: it decides which actors
## draw, which frame, which search_tpage slot each page takes, and advances
## the sequence cursor exactly as the source does after the draw.
##
## The area is opening["actor_area"], a list of memcpy layers in copy order,
## each {"resource_id","sha256","bytes"} copied to 0x80180000. A byte is the
## byte of the newest layer that covers it, so stale tails of a longer older
## copy stay readable (O_13 under O_14, O_15 under O_16). Bytes outside every
## layer are unknown and every read of them is refused.
const Board = preload("res://services/first_arrival_board_store.gd")
const AREA_BASE := 0x80180000
const MAIN_TABLE := -1
const AREA_UNKNOWN := "actor table read falls outside the modelled 0x80180000 actor area"
const SEQUENCE_OUTSIDE := "actor sequence pointer is outside the modelled actor area"
# Stages 12-14 draw the slot-1 buffer dec_bg_show[1] selects. Buffer 0 holds
# P_01_08 (0xAD05) from stage11 case6 (the board pack's names_ad05 decode).

static func visible_scene(state, mode_time: bool = false) -> bool:
	# The frames whose dec_bg_draw(1)/actor_draw_all belong to stages 12-14:
	# every stage 12-14 frame, and stage14's commit frame, whose body draws
	# after mode_stage_next (mode_stage already 15, substep 0).
	# committed_frame is the frame_count draw_frame sees: game_mode_dispatch
	# increments it after the mode handler (core/game_session.gd), so a
	# mode-time caller (dec_bg_draw inside the body) compares one ahead.
	if state == null or state.game_mode != 0x15 or state.mode_step != 0: return false
	var a96: Variant = state.opening.get("actor96")
	# Stage11 case6 (prologue_line_17_run) draws its exit frame after
	# mode_stage_next as 12/0 with the O_13 actor96 it just set up; that frame
	# belongs to the Rei line17 renderer (ReiRenderer.line17_scope). The
	# stage12 body itself never leaves substep 0 (case0 increments it).
	if state.mode_stage == 12 and state.substep == 0 and a96 is Dictionary and a96.get("profile") == "o13_rei_line17_exit": return false
	if state.mode_stage in [12,13,14]: return true
	return state.mode_stage == 15 and state.substep == 0 and a96 is Dictionary and a96.get("profile") == "o16_shiori_line20" and a96.get("committed_frame") == ((state.frame_count + (1 if mode_time else 0)) & 0xffffffff)

static func cleared(index: int) -> Dictionary:
	# actor_clear (actor_sprites.c:672). Its tables point at main-executable
	# defaults (sprite_frame_table, ...), recorded here as MAIN_TABLE.
	return {"profile":"cleared","index":index,"unk_01":0,"flags":0,"draw_flags":0,"brightness":0,"speed":1,
		"sequence_counter":0,"frame_table":MAIN_TABLE,"sequence_table":MAIN_TABLE,"tpage_table":MAIN_TABLE,
		"sequence_slot":0,"frame_index":0,"x":0,"y":0,"quad_flags":0}

static func area_read(bytes_by_id: Dictionary, area: Variant, address: int, size: int) -> PackedByteArray:
	# Empty on any byte no layer covers, or on an unauthenticated layer.
	var out := PackedByteArray()
	if not area is Array or address < AREA_BASE or size < 1: return out
	for layer: Variant in area:
		if not layer is Dictionary or not bytes_by_id.has(layer.get("resource_id")) or not layer.get("bytes") is int or layer.get("sha256") != bytes_by_id[layer.resource_id].sha256 or layer.bytes < 1 or layer.bytes > bytes_by_id[layer.resource_id].bytes.size():
			return out
	var offset := address - AREA_BASE
	for k in size:
		var found := false
		for i in range(area.size()-1,-1,-1):
			if offset+k < area[i].bytes:
				out.append(bytes_by_id[area[i].resource_id].bytes[offset+k]); found = true; break
		if not found: return PackedByteArray()
	return out

static func word(bytes_by_id: Dictionary, area: Variant, address: int) -> int:
	var b := area_read(bytes_by_id,area,address,4)
	return -1 if b.size() != 4 else b.decode_u32(0)

static func half(bytes_by_id: Dictionary, area: Variant, address: int) -> Variant:
	var b := area_read(bytes_by_id,area,address,2)
	return null if b.size() != 2 else b.decode_s16(0)

static func in_area(bytes_by_id: Dictionary, area: Variant, address: int) -> bool:
	return area_read(bytes_by_id,area,address,1).size() == 1

const PAGE_BYTES := 0x4000
const PAGE_SPANS_LAYERS := "actor tpage spans more than one memcpy layer of the actor area"

static func page_layer(bytes_by_id: Dictionary, area: Variant, page: int) -> Dictionary:
	# The memcpy layer whose bytes load_csr_tp reads for a 64x128 8-bit page
	# at `page` (0x4000 bytes). The newest layer covering the first byte must
	# also cover the last one; otherwise the page mixes two copies.
	if not area is Array or area.is_empty() or page < AREA_BASE: return {"error":AREA_UNKNOWN}
	if area_read(bytes_by_id,area,page,1).size() != 1: return {"error":AREA_UNKNOWN}
	var offset := page - AREA_BASE
	for i in range(area.size()-1,-1,-1):
		var layer: Dictionary = area[i]
		if offset >= int(layer.bytes): continue
		if offset + PAGE_BYTES > int(layer.bytes): return {"error":PAGE_SPANS_LAYERS}
		return {"error":"","resource_id":layer.resource_id,"offset":offset}
	return {"error":AREA_UNKNOWN}

static func frame_parts(bytes_by_id: Dictionary, area: Variant, frame_table: int, frame: int) -> Dictionary:
	# SpriteFrame (count, origin_x, origin_y, pad) and its 14-byte parts, read
	# through the actor area exactly where actor_draw_all reads them.
	var anim := word(bytes_by_id,area,frame_table+frame*4)
	if anim < 0: return {"error":AREA_UNKNOWN}
	var head := area_read(bytes_by_id,area,anim,8)
	if head.size() != 8: return {"error":AREA_UNKNOWN}
	var count := head.decode_s16(0)
	if count < 0 or count > 32: return {"error":"actor frame part count is outside the source table"}
	var parts: Array = []
	for j in count:
		var row := area_read(bytes_by_id,area,anim+8+j*14,14)
		if row.size() != 14: return {"error":AREA_UNKNOWN}
		parts.append({"x":row.decode_s16(0),"y":row.decode_s16(2),"texture":row.decode_u16(4),"palette":row.decode_u16(6),
			"flip":row.decode_u16(8),"w":row.decode_s16(10),"h":row.decode_s16(12)})
	return {"error":"","origin_x":head.decode_s16(2),"origin_y":head.decode_s16(4),"parts":parts}

static func draw_all(bytes_by_id: Dictionary, opening: Dictionary, frame_count: int) -> Dictionary:
	# Returns {"error","slots":{page word -> tpage slot},"drawn":[[actor,frame]]}
	# and mutates the actor records like the source's post-draw advance.
	var area: Variant = opening.get("actor_area")
	var slots: Dictionary = {}
	var drawn: Array = []
	for index in range(96,160):
		var actor: Variant = opening.get("actor%d" % index)
		if not actor is Dictionary: continue
		var df: int = int(actor.get("draw_flags",0))
		var flags: int = int(actor.get("flags",0))
		var speed: int = int(actor.get("speed",1))
		if (df & 0x80) == 0 or ((df & 0x40) == 0x40 and frame_count % (speed+1) == 0) or (flags & 0x80) != 0: continue
		if actor.get("frame_table") == MAIN_TABLE or actor.get("tpage_table") == MAIN_TABLE or actor.get("sequence_table") == MAIN_TABLE:
			return {"error":"actor%d draws through main-executable default tables, which are not modelled" % index}
		if actor.get("quad_flags") != 0x41000000 or (df & 0x10) != 0:
			return {"error":"actor%d uses a quad or motion mode outside search_tpage 8-bit draws" % index}
		var tpage_word := word(bytes_by_id,area,int(actor.tpage_table))
		var frame_word := word(bytes_by_id,area,int(actor.frame_table))
		if tpage_word < 0 or frame_word < 0: return {"error":AREA_UNKNOWN}
		# actor_sprites.c:93: bit31 clear on either table word skips the actor
		# entirely (printf only): no draw, no tpage, no sequence advance.
		if (tpage_word & 0x80000000) == 0 or (frame_word & 0x80000000) == 0: continue
		var step: int = int(actor.get("frame_index",0))
		var seq := -1
		var frame := step
		if (flags & 0x40) == 0:
			seq = word(bytes_by_id,area,int(actor.sequence_table)+int(actor.get("sequence_slot",0))*4)
			if seq < 0: return {"error":AREA_UNKNOWN}
			if not in_area(bytes_by_id,area,seq): return {"error":SEQUENCE_OUTSIDE}
			var row: Variant = half(bytes_by_id,area,seq+(step*2+1)*2)
			if row == null: return {"error":AREA_UNKNOWN}
			frame = row
		var anim := word(bytes_by_id,area,int(actor.frame_table)+frame*4)
		var count: Variant = half(bytes_by_id,area,anim) if anim >= 0 else null
		if count == null: return {"error":AREA_UNKNOWN}
		var j: int = count-1
		while j >= 0:
			if (df & 8) != 0 and j >= 4: j = 3
			var texture: Variant = half(bytes_by_id,area,anim+8+j*14+4)
			if texture == null: return {"error":AREA_UNKNOWN}
			var page := word(bytes_by_id,area,int(actor.tpage_table)+((int(texture) & 0xffff) >> 6)*4)
			if page < 0: return {"error":AREA_UNKNOWN}
			if not slots.has(page):
				if slots.size() >= Board.STAGE10_FREE_TPAGE_SLOTS.size(): return {"error":"actor pages exceed the source free tpage slots"}
				slots[page] = Board.STAGE10_FREE_TPAGE_SLOTS[slots.size()]
			j -= 1
		drawn.append([index,frame])
		if (flags & 0x40) != 0 or (flags & 1) != 1: continue
		var counter: int = int(actor.get("sequence_counter",0))
		if (speed & 0x80) != 0 and (speed & 0x7f) != 0: counter += speed & 0x7f
		elif speed != 0 and frame_count % speed == 0: counter += 1
		var duration: Variant = half(bytes_by_id,area,seq+(step*2+2)*2)
		var rows: Variant = half(bytes_by_id,area,seq)
		if duration == null or rows == null: return {"error":AREA_UNKNOWN}
		if counter >= duration:
			if (flags & 0x10) == 0x10:
				step += 1; counter = 0; flags &= ~0x20
			elif (flags & 0x20) == 0x20:
				step -= 1; counter = 0
				if step <= 0: flags &= 0xffde
			else:
				step += 1; counter = 0
		if step >= rows:
			if (flags & 2) == 2:
				step = 0; flags &= ~3
			elif (flags & 4) == 4:
				flags &= 0xfffa; step -= 1
			elif (flags & 8) == 8:
				step = 0; flags &= 0xfffc; df &= ~0x80
			elif (flags & 0x10) == 0x10:
				flags &= ~0x10; flags |= 0x20
				if (df & 8) != 0: step = step-3 if rows >= 3 else 0
				elif rows >= 2: step -= 1
				else: step = 0
			else: step = 0
		actor["frame_index"] = step
		actor["sequence_counter"] = counter
		actor["flags"] = flags
		actor["draw_flags"] = df
		opening["actor%d_draw_flags" % index] = df
	return {"error":"","slots":slots,"drawn":drawn}
