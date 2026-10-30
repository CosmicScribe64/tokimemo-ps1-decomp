extends RefCounted
## T-39002: O10 stage9 actor layers, separated at native OT11 flash boundary;
## O11 stage10 layers after the case11 memcpy (all actors in front).
const Sequences = preload("res://services/rei_actor_sequences.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
var source := Sequences.new()

static func line17_scope(state) -> bool:
	# Stage11 (prologue_line_17_run) and its case6 frame, which is drawn
	# after mode_stage_next as stage12/substep0 with the O_13 actor.
	if state == null or state.game_mode != 0x15 or state.mode_step != 0: return false
	if state.mode_stage == 11: return true
	return state.mode_stage == 12 and state.substep == 0 and state.opening.get("actor96") is Dictionary and state.opening.actor96.get("profile") == "o13_rei_line17_exit"

func preflight(state, upload = null) -> String:
	if line17_scope(state): return _line17_preflight(state,upload)
	if state.mode_stage == 10: return _lines15_preflight(state,upload)
	if state.game_mode != 0x15 or state.mode_stage != 9 or state.mode_step != 0 or state.substep < 0 or state.substep > 11:
		return "Rei actor renderer is outside stage9"
	var layer: Variant = state.opening.get("bg_layer_0")
	if not layer is Dictionary or layer.get("enabled") not in [0,1] or layer.get("resource_id") != Board.CUT_09_ID:
		return "Rei actor renderer lost its original cut owner"
	for index in range(96,101):
		var actor: Variant = state.opening.get("actor%d" % index)
		if actor == null:
			if index == 96: return "Rei body actor is missing"
			continue
		if not actor is Dictionary: return "Rei actor record is malformed"
		if actor.get("profile") == "cleared" and index == 98 and state.opening.get("actor98_draw_flags") == 0: continue
		if actor.get("profile") != ("o09_rei_entrance" if index == 96 else "o10_rei_dialogue") or actor.get("resource_id") != Board.O_09_ID or actor.get("source_sha256") != Board.O_09_SHA or actor.get("x") != -160 or actor.get("y") != -120:
			return "Rei actor source identity or geometry differs"
		if not actor.get("frame_index") is int or source.parts("crowd" if index >= 99 else "body",actor.frame_index).is_empty():
			return "Rei actor has no authenticated frame"
		if actor.get("draw_flags") not in [0xa4,0xac] or state.opening.get("actor%d_draw_flags" % index) != actor.draw_flags:
			return "Rei actor visibility differs"
		for key in ["upload_batch_key"] if index >= 99 else ["upload_batch_key","upload_batch_key_2"]:
			if not actor.get(key) is String or actor[key].is_empty(): return "Rei actor upload ownership is missing"
			if upload != null:
				var ready: Variant = upload.batch_ready(actor[key])
				if not ready is String or not ready.is_empty(): return "Rei actor upload is incomplete"
	return ""

func _lines15_preflight(state, upload) -> String:
	if state.game_mode != 0x15 or state.mode_step != 0 or state.substep < 0 or state.substep > 8:
		return "Rei O11 renderer is outside stage10"
	var layer: Variant = state.opening.get("bg_layer_0")
	if not layer is Dictionary or layer.get("enabled") != 1 or layer.get("clut_row") != 495 or layer.get("resource_id") != Board.CUT_07_ID:
		return "Rei O11 renderer lost its original cut owner"
	var body: Variant = state.opening.get("actor96")
	if not body is Dictionary or body.get("profile") != "o11_rei_lines15" or body.get("resource_id") != Board.O_11_ID or body.get("source_sha256") != Board.O_11_SHA or not body.get("x") is int or body.x > -160 or body.x < -0x8000 or body.get("y") != -120 or body.get("frame_index") != 0:
		return "Rei O11 body source identity or geometry differs"
	if body.get("draw_flags") != 0xa4 or state.opening.get("actor96_draw_flags") != 0xa4:
		return "Rei O11 body visibility differs"
	for index in range(97,160):
		var actor: Variant = state.opening.get("actor%d" % index)
		if actor == null:
			if state.opening.get("actor%d_draw_flags" % index,0) != 0: return "Rei O11 cleared actor is still visible"
			continue
		if index == 98 and actor is Dictionary and actor.get("profile") == "o10_tables_over_o11":
			# Post-switch blink record: actor_draw_all skips it because neither
			# table word in the copied O11 bytes has bit31.
			var bytes := source.o11_bytes
			if state.substep > 2 or bytes.size() != 0x10800 or actor.get("tpage_table") != 0x80184280 or actor.get("frame_table") != 0x80184288 or (bytes.decode_u32(0x4280) & 0x80000000) != 0 or (bytes.decode_u32(0x4288) & 0x80000000) != 0:
				return "Rei O11 post-switch actor98 is not the source-skipped record"
			continue
		if index > 98 or not actor is Dictionary or actor.get("profile") != "o11_rei_lip" or actor.get("resource_id") != Board.O_11_ID or actor.get("x") != body.x or actor.get("y") != -120:
			return "Rei O11 stage10 has an actor without a source owner"
		var lowest := 1 if index == 97 else 5
		if not actor.get("frame_index") is int or actor.frame_index < lowest or actor.frame_index > lowest+3 or actor.get("draw_flags") != 0xa4 or state.opening.get("actor%d_draw_flags" % index) != 0xa4:
			return "Rei O11 lip actor frame or visibility differs"
	var expected := Board.o11_tpage_slots(source.o11_bytes,Board.o11_drawn_frames(state.opening))
	var tpages: Variant = state.opening.get("o11_tpages")
	if expected.is_empty() or not tpages is Dictionary or not tpages.get("slots") is Dictionary or tpages.slots.size() != expected.size():
		return "Rei O11 tpage allocation differs from search_tpage"
	for page: int in expected:
		var resident: Variant = tpages.get("resident",{}).get(str(expected[page]))
		if tpages.slots.get(str(page)) != expected[page] or not resident is Dictionary or resident.get("offset") != page or not resident.get("key") is String:
			return "Rei O11 tpage allocation differs from search_tpage"
		if upload != null:
			var ready: Variant = upload.batch_ready(resident.key)
			if not ready is String or not ready.is_empty(): return "Rei O11 upload is incomplete"
	return ""

func _line17_preflight(state, upload) -> String:
	var exit_frame: bool = state.mode_stage == 12
	if state.mode_stage == 11 and (state.substep < 0 or state.substep > 6):
		return "Rei line17 renderer is outside stage11"
	var layer: Variant = state.opening.get("bg_layer_0")
	# Stage10 case8 enables the CUT12 layer; case6 relocks it with
	# bg_layers = 0x80000000, so the exit frame shows only AD05.
	if not layer is Dictionary or layer.get("enabled") != (0 if exit_frame else 1) or layer.get("clut_row") != 495 or layer.get("resource_id") != Board.CUT_12_ID:
		return "Rei line17 renderer lost its original cut owner"
	if upload != null:
		var cut_ready: Variant = upload.batch_ready(str(layer.get("cut_batch_key","")))
		if not cut_ready is String or not cut_ready.is_empty(): return "Rei line17 cut upload is incomplete"
	var profile := "o13_rei_line17_exit" if exit_frame else "o12_rei_line17"
	var body: Variant = state.opening.get("actor96")
	if not body is Dictionary or body.get("profile") != profile or body.get("resource_id") != (Board.O_13_ID if exit_frame else Board.O_12_ID) or body.get("source_sha256") != (Board.O_13_SHA if exit_frame else Board.O_12_SHA) or body.get("x") != -160 or body.get("y") != -120 or body.get("speed") != 1:
		return "Rei line17 body source identity or geometry differs"
	if body.get("draw_flags") != 0xa4 or state.opening.get("actor96_draw_flags") != 0xa4:
		return "Rei line17 body visibility differs"
	# Case2 sets flags 5; until then the case8 record is stopped (flags 0).
	var rows: Array = source.line17_rows.get(profile,[])
	var running: bool = exit_frame or state.substep >= 3
	if rows.is_empty() or not body.get("seq_step") is int or body.seq_step < 0 or body.seq_step >= rows.size() or not body.get("sequence_counter") is int or body.sequence_counter < 0 or body.sequence_counter >= rows[body.seq_step].duration:
		return "Rei line17 sequence cursor differs from source"
	if (not running and (body.get("flags") != 0 or body.seq_step != 0 or body.sequence_counter != 0)) or (running and body.get("flags") not in [0,5]) or (body.get("flags") == 0 and running and body.seq_step != rows.size()-1):
		return "Rei line17 sequence flags differ from source"
	for index in range(97,160):
		if state.opening.has("actor%d" % index) or state.opening.get("actor%d_draw_flags" % index,0) != 0:
			return "Rei line17 has an actor without a source owner"
	var slots := source.line17_slots(profile,int(rows[body.seq_step].frame))
	var tpages: Variant = body.get("tpages")
	if slots.is_empty() or not tpages is Dictionary:
		return "Rei line17 tpage allocation differs from search_tpage"
	for page: int in slots:
		var resident: Variant = tpages.get(str(slots[page]))
		if not resident is Dictionary or resident.get("offset") != page or not resident.get("key") is String:
			return "Rei line17 tpage allocation differs from search_tpage"
		if upload != null:
			var ready: Variant = upload.batch_ready(resident.key)
			if not ready is String or not ready.is_empty(): return "Rei line17 upload is incomplete"
	return ""

func logical_draw(state) -> String:
	var error := preflight(state)
	if not error.is_empty(): return error
	if line17_scope(state):
		var body: Dictionary = state.opening.actor96
		var draw := source.draw_and_advance_rows(body,source.line17_rows.get(body.profile,[]),state.frame_count)
		if not draw.error.is_empty(): return draw.error
		state.opening["actor96_source_draw"] = {"profile":body.profile,"frame_index":int(draw.frame_index),"source_frame_count":state.frame_count,"slots":_string_keys(source.line17_slots(body.profile,int(draw.frame_index))),"tpages":body.tpages.duplicate(true)}
		return ""
	if state.mode_stage == 10:
		for index in [96,97,98]:
			var actor: Variant = state.opening.get("actor%d" % index)
			if actor == null or actor.get("profile") == "o10_tables_over_o11": state.opening.erase("actor%d_source_draw" % index); continue
			state.opening["actor%d_source_draw" % index] = {"frame_index":int(actor.frame_index),"x":int(actor.x),"source_frame_count":state.frame_count,"tpages":state.opening.o11_tpages.duplicate(true)}
		return ""
	for index in range(96,101):
		var key := "actor%d" % index
		var actor: Variant = state.opening.get(key)
		if actor == null or state.opening.get(key+"_draw_flags",0) == 0:
			state.opening.erase(key+"_source_draw"); continue
		var frame: int = actor.frame_index
		if index >= 98:
			var draw := source.draw_and_advance(actor,state.frame_count)
			if not draw.error.is_empty(): return draw.error
			frame = draw.frame_index
		state.opening[key+"_source_draw"] = {"frame_index":frame,"source_frame_count":state.frame_count,"batch_key":actor.upload_batch_key,"batch_key_2":actor.get("upload_batch_key_2","")}
	return ""

func paint(target: Image, state, runtime: Dictionary, upload, front: bool) -> String:
	var error := preflight(state,upload)
	if not error.is_empty(): return error
	var vram: PackedByteArray = runtime.vram_words
	var known: PackedByteArray = runtime.vram_known_mask
	if line17_scope(state):
		if not front: return ""
		var body: Dictionary = state.opening.actor96
		var drawn: Variant = state.opening.get("actor96_source_draw")
		if not drawn is Dictionary or drawn.get("source_frame_count") != state.frame_count or drawn.get("profile") != body.profile or not Store._same(drawn.get("tpages"),body.tpages):
			return "Rei actor draw is not owned by this committed frame"
		for part: Dictionary in source.line17_parts(body.profile,int(drawn.frame_index)):
			if part.flip != 0 or part.origin_x != 0 or part.origin_y != 0: return "Rei actor requires unsupported native flip/origin"
			var slot: int = int(drawn.slots.get(str(part.page_offset),-1))
			if slot < 0 or body.tpages.get(str(slot),{}).get("offset") != part.page_offset: return "Rei actor texture pointer has no exact upload slot"
			var error_part := _paint_part(target,part,slot,int(body.x)+160,vram,known)
			if not error_part.is_empty(): return error_part
		return ""
	# actor_draw_all traverses ascending actor indices and head-inserts OT.
	# Crowd priority12=>OT11 is behind the mode's later same-slot flash;
	# body priority10=>OT9, mouth/eyes priority9=>OT8 are in front.
	var lines15: bool = state.mode_stage == 10
	for index in ([96,98,97] if front else [100,99]) if not lines15 else ([96,98,97] if front else []):
		var key := "actor%d" % index
		var actor: Variant = state.opening.get(key)
		if actor == null or state.opening.get(key+"_draw_flags",0) == 0 or (lines15 and actor.get("profile") == "o10_tables_over_o11"): continue
		var draw: Variant = state.opening.get(key+"_source_draw")
		if not draw is Dictionary or draw.get("source_frame_count") != state.frame_count or (lines15 and (draw.get("x") != actor.x or not Store._same(draw.get("tpages"),state.opening.get("o11_tpages")))) or (not lines15 and (draw.get("batch_key") != actor.upload_batch_key or draw.get("batch_key_2","") != actor.get("upload_batch_key_2",""))):
			return "Rei actor draw is not owned by this committed frame"
		# Screen origin (160,120); stage10 moves x by actors[96].x.h[1].
		var origin_x: int = int(actor.x)+160 if lines15 else 0
		var parts := source.parts("o11" if lines15 else ("crowd" if index >= 99 else "body"),int(draw.frame_index))
		for part: Dictionary in parts:
			if part.flip != 0 or part.origin_x != 0 or part.origin_y != 0: return "Rei actor requires unsupported native flip/origin"
			# Stage10 uses the recomputed search_tpage allocation; stage9 uses
			# the source O10 entrance allocation (BoardStore).
			var slot: int = int(state.opening.o11_tpages.slots.get(str(part.page_offset),-1)) if lines15 else int(Board.O10_ENTRANCE_TPAGE_SLOTS.get(part.page_offset,-1))
			if slot < 0: return "Rei actor texture pointer has no exact upload slot"
			var slot_x: int = (slot&15)*64; var slot_y: int = ((slot>>4)&1)*256+(slot>>5)*128
			var clut_y: int = ((int(part.palette_flags)&0xf00)>>8)+496
			for y in int(part.h):
				for x in int(part.w):
					var px: int = part.x+x+origin_x; var py: int = part.y+y
					if px < 0 or px >= 320 or py < 0 or py >= 160: continue
					var address: int = ((slot_y+int(part.v)+y)*1024+slot_x)*2+int(part.u)+x
					if address < 0 or address >= known.size() or known[address] != 255: return "Rei actor references unknown texture memory"
					var clut: int = (clut_y*1024+vram[address])*2
					if known[clut] != 255 or known[clut+1] != 255: return "Rei actor references unknown palette memory"
					var color := vram.decode_u16(clut)
					if color == 0: continue
					var red := color&31; var green := (color>>5)&31; var blue := (color>>10)&31
					if (color&0x8000) != 0:
						var back := target.get_pixel(px,py)
						red = ((back.r8>>3)+red)>>1; green = ((back.g8>>3)+green)>>1; blue = ((back.b8>>3)+blue)>>1
					target.set_pixel(px,py,Color8((red<<3)|(red>>2),(green<<3)|(green>>2),(blue<<3)|(blue>>2)))
	return ""

static func _paint_part(target: Image, part: Dictionary, slot: int, origin_x: int, vram: PackedByteArray, known: PackedByteArray) -> String:
	var slot_x: int = (slot&15)*64; var slot_y: int = ((slot>>4)&1)*256+(slot>>5)*128
	var clut_y: int = ((int(part.palette_flags)&0xf00)>>8)+496
	for y in int(part.h):
		for x in int(part.w):
			var px: int = part.x+x+origin_x; var py: int = part.y+y
			if px < 0 or px >= 320 or py < 0 or py >= 160: continue
			var address: int = ((slot_y+int(part.v)+y)*1024+slot_x)*2+int(part.u)+x
			if address < 0 or address >= known.size() or known[address] != 255: return "Rei actor references unknown texture memory"
			var clut: int = (clut_y*1024+vram[address])*2
			if known[clut] != 255 or known[clut+1] != 255: return "Rei actor references unknown palette memory"
			var color := vram.decode_u16(clut)
			if color == 0: continue
			var red := color&31; var green := (color>>5)&31; var blue := (color>>10)&31
			if (color&0x8000) != 0:
				var back := target.get_pixel(px,py)
				red = ((back.r8>>3)+red)>>1; green = ((back.g8>>3)+green)>>1; blue = ((back.b8>>3)+blue)>>1
			target.set_pixel(px,py,Color8((red<<3)|(red>>2),(green<<3)|(green>>2),(blue<<3)|(blue>>2)))
	return ""

static func _string_keys(value: Dictionary) -> Dictionary:
	var out: Dictionary = {}
	for key: Variant in value: out[str(key)] = value[key]
	return out
