extends RefCounted
## T-39002: O10 stage9 actor layers, separated at native OT11 flash boundary.
const Sequences = preload("res://services/rei_actor_sequences.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
var source := Sequences.new()

func preflight(state, upload = null) -> String:
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

func logical_draw(state) -> String:
	var error := preflight(state)
	if not error.is_empty(): return error
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
	# actor_draw_all traverses ascending actor indices and head-inserts OT.
	# Crowd priority12=>OT11 is behind the mode's later same-slot flash;
	# body priority10=>OT9, mouth/eyes priority9=>OT8 are in front.
	for index in [96,98,97] if front else [100,99]:
		var key := "actor%d" % index
		var actor: Variant = state.opening.get(key)
		if actor == null or state.opening.get(key+"_draw_flags",0) == 0: continue
		var draw: Variant = state.opening.get(key+"_source_draw")
		if not draw is Dictionary or draw.get("source_frame_count") != state.frame_count or draw.get("batch_key") != actor.upload_batch_key or draw.get("batch_key_2","") != actor.get("upload_batch_key_2",""):
			return "Rei actor draw is not owned by this committed frame"
		var parts := source.parts("crowd" if index >= 99 else "body",int(draw.frame_index))
		for part: Dictionary in parts:
			if part.flip != 0 or part.origin_x != 0 or part.origin_y != 0: return "Rei actor requires unsupported native flip/origin"
			var slot: int = {0x42c0:5,0x82c0:6,0x150:7}.get(part.page_offset,-1)
			if slot < 0: return "Rei actor texture pointer has no exact upload slot"
			var clut_y: int = ((int(part.palette_flags)&0xf00)>>8)+496
			for y in int(part.h):
				for x in int(part.w):
					var px: int = part.x+x; var py: int = part.y+y
					if px < 0 or px >= 320 or py < 0 or py >= 160: continue
					var address: int = ((int(part.v)+y)*1024+slot*64)*2+int(part.u)+x
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
