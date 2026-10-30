extends RefCounted
## T-29025. Bounded O_01 actor96. Source actor_sprites.c::actor_draw_all;
## prologue.c::prologue_arrival_scene_init/line. No whole-VRAM readiness claim.
const Store = preload("res://services/first_arrival_media_store.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const ReiRenderer = preload("res://presentation/rei_actor_renderer.gd")
var _rei := ReiRenderer.new()
const VRAM_BYTES := 1024*512*2
const DRAW_KEY := "actor96_source_draw"
const PETALS_DRAW_KEY := "petals_source_draw"
var _source := PackedByteArray()
var _runtime: Dictionary
var _upload: Upload
var _store: Store
var _frames: Array[Dictionary] = []
var _petal_frames: Array[Dictionary] = []
var _board_vram_strip: Image
var _board_strip_batch_key: String = ""
var _names_bg_cache_image: Image
var _names_bg_cache_key: String = ""
var _last_names_bg_target_id: int = 0
var _last_names_bg_key: String = ""
var _stage35_actor_cache: Dictionary = {}
var _stage35_actor_cache_bg_key: String = ""
var _stage35_bg_a96_cache_image: Image
var _stage35_bg_a96_cache_key: String = ""

func prepare(store: Store, upload_owner: Upload, board_store = null) -> String:
	if not _source.is_empty(): return "O_01 renderer already owns its source"
	if store == null or store.identity().is_empty() or upload_owner == null: return "O_01 needs authentic store and upload owner"
	var context := upload_owner.actor_render_context(store)
	if not context.get("error") is String or not context.error.is_empty() or not context.get("runtime") is Dictionary: return "O_01 upload owner has no matching runtime"
	var owned_runtime: Dictionary = context.runtime
	var transfer := store.transfer(Store.ACTOR_ID)
	if not transfer.get("error") is String or not transfer.error.is_empty() or not transfer.get("bytes") is PackedByteArray or Store._sha(transfer.bytes) != Store.ACTOR_SHA: return "O_01 source identity differs"
	var failure := _runtime_error(owned_runtime)
	if not failure.is_empty(): return failure
	var bytes: PackedByteArray = transfer.bytes
	var frames: Array[Dictionary] = []
	for index in 5:
		var offset: int = bytes.decode_u32(0xac+index*4)-0x80180000
		if offset != [0,24,48,72,108][index]: return "O_01 frame pointers differ"
		var count := bytes.decode_s16(offset)
		if count not in [1,2] or bytes.decode_s16(offset+2) != 0 or bytes.decode_s16(offset+4) != 0: return "O_01 frame header differs"
		var parts: Array[Dictionary] = []
		for part_index in count:
			var row := offset+8+part_index*14
			var texture := bytes.decode_u16(row+4)
			if bytes.decode_u16(row+6) != 0x1000 or bytes.decode_u16(row+8) != 0 or texture >> 6 != 0: return "O_01 part requires an unsupported page/palette/flip"
			parts.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),"u":(texture&7)*16,"v":((texture>>3)&7)*16,"flip":0,"w":bytes.decode_s16(row+10),"h":bytes.decode_s16(row+12)})
		frames.append({"parts":parts})
	if bytes.decode_u32(0x4224) != 0x801800d0 or bytes.decode_u32(0x4258) != 0x801841f0 or bytes.decode_s16(0x41f0) != 12:
		return "O_01 petal tpage/sequence pointers differ"
	var petal_frames: Array[Dictionary] = []
	for index in 12:
		var offset: int = bytes.decode_u32(0x4228+index*4)-0x80180000
		if offset != 0x40d0+index*24: return "O_01 petal frame pointers differ"
		var count := bytes.decode_s16(offset)
		var row := offset+8
		var w := bytes.decode_s16(row+10)
		var h := bytes.decode_s16(row+12)
		if count != 1 or bytes.decode_s16(offset+2) != 7 or bytes.decode_s16(offset+4) != 7 or w != 16 or h != 16: return "O_01 petal frame header differs"
		var texture := bytes.decode_u16(row+4)
		var flip := bytes.decode_u16(row+8)
		if bytes.decode_u16(row+6) != 0x1000 or flip not in [0,3] or texture >> 6 != 0: return "O_01 petal part requires an unsupported page/palette/flip"
		var parts: Array[Dictionary] = [{"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),"ox":7,"oy":7,"u":(texture&7)*16,"v":((texture>>3)&7)*16,"flip":flip,"w":w,"h":h}]
		petal_frames.append({"parts":parts})
	_source = bytes.duplicate()
	_frames = frames
	_petal_frames = petal_frames
	_runtime = owned_runtime
	_upload = upload_owner
	_store = store
	if board_store != null:
		var rei_transfer: Dictionary = board_store.stage_transfer("prologue.o_09.0xac05")
		if not rei_transfer.get("bytes") is PackedByteArray: return "Rei renderer has no original O10 resource"
		return _rei.source.prepare(rei_transfer.bytes)
	return ""

func paint_rei_front(target: Image, state: State) -> String:
	return _rei.paint(target,state,_runtime,_upload,true)

static func _runtime_error(runtime: Dictionary) -> String:
	if not runtime.get("vram_owner") is String or runtime.vram_owner.is_empty() or not runtime.get("vram_words") is PackedByteArray or runtime.vram_words.size() != VRAM_BYTES: return "O_01 requires explicitly owned VRAM"
	# The mask may be absent before real uploads; visible rendering requires it.
	if runtime.has("vram_known_mask") and (not runtime.vram_known_mask is PackedByteArray or runtime.vram_known_mask.size() != VRAM_BYTES): return "O_01 VRAM mask is malformed"
	return ""

func _actor_error(state: State) -> String:
	if _source.is_empty() or state == null or state.game_mode != 0x15: return "O_01 renderer is outside its prepared source mode"
	var actor: Variant = state.opening.get("actor96")
	if not actor is Dictionary: return "O_01 actor has no source initialization"
	for key: String in ["index","unk_01","flags","quad_flags","draw_flags","frame_table","sequence_table","tpage_table","unk_06","unk_07","sequence_slot","unk_05","x","y","brightness","frame_index","sequence_counter"]:
		if not actor.get(key) is int: return "O_01 actor field requires native integer: "+key
	var expected := {"index":96,"unk_01":10,"flags":1,"quad_flags":0x41000000,"draw_flags":0x24,"frame_table":0x801800ac,"sequence_table":0x801800c0,"tpage_table":0x801800a8,"unk_06":2,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-160,"y":-120,"brightness":128}
	for key: String in expected:
		if actor[key] != expected[key]: return "O_01 actor source setup changed: "+key
	if not actor.get("resource_id") is String or actor.resource_id != Store.ACTOR_ID or not actor.get("source_sha256") is String or actor.source_sha256 != Store.ACTOR_SHA: return "O_01 actor resource changed"
	if actor.frame_index < 0 or actor.frame_index >= 5 or actor.sequence_counter < 0 or actor.sequence_counter >= [3,3,3,1,1][actor.frame_index]: return "O_01 sequence is outside native rows"
	if not state.opening.get("actor96_draw_flags") is int or state.opening.actor96_draw_flags not in [0x24,0xa4]: return "O_01 visibility flags are unsupported"
	return ""

func _petals_error(state: State) -> String:
	var stage1_ok: bool = state != null and state.mode_stage == 1 and state.mode_step == 1 and state.substep in [0, 1, 2, 3, 4, 5]
	var stage2_ok: bool = state != null and state.mode_stage == 2 and state.mode_step == 0 and state.substep in [0, 1, 2, 3, 4, 5, 6, 7, 8]
	var stage2_to_3_ok: bool = state != null and state.mode_stage == 3 and state.mode_step == 0 and state.substep == 0 and not state.opening.has("bg_layer_0")
	if _source.is_empty() or state == null or state.game_mode != 0x15 or (not stage1_ok and not stage2_ok and not stage2_to_3_ok):
		return "O_01 petals are outside the board-load/scroll/names boundary"
	var counter: Variant = state.opening.get("prologue_scene_counter")
	if state.opening.has("actor96") or state.opening.get("actor96_draw_flags") != 0 or state.opening.get("D_800B3DB0") != 1 or not counter is int or counter < 0 or counter > 320 or (stage1_ok and state.substep == 0 and counter != 0) or state.opening.get("prologue_hud_live",{}).get("date_visible") != false:
		return "O_01 petals require actor96 cleared, D_800B3DB0=1 and date pillar hidden"
	var board_media: Variant = state.opening.get("board_media")
	if not board_media is Dictionary or board_media.get("decoded") != true or not board_media.get("upload_batch_key") is String or board_media.upload_batch_key.is_empty():
		return "O_01 petals require a completed board decode batch"
	var petals: Variant = state.opening.get("prologue_petals")
	if not petals is Array or petals.size() != 8:
		return "O_01 petals require 8 initialized actor records"
	var faded_out_petals: bool = (stage2_ok and state.substep in [5, 6, 7, 8]) or stage2_to_3_ok
	for index in 8:
		var petal: Variant = petals[index]
		if not petal is Dictionary:
			return "O_01 petal record is malformed at index "+str(index)
		var dflags: Variant = petal.get("draw_flags")
		var unk18: Variant = petal.get("unk_18")
		var unk07: Variant = petal.get("unk_07")
		var unk08: Variant = petal.get("unk_08", 0)
		if not dflags is int or dflags not in [0, 0xa4] or (not faded_out_petals and index < 4 and dflags != 0xa4) or not unk18 is int or unk18 < 0 or unk18 >= 12 or not unk07 is int or unk07 < 0 or unk07 > 255 or not unk08 is int or unk08 < 0 or unk08 >= 5 or not petal.get("x_word") is int or not petal.get("y_word") is int or not petal.get("brightness") is int:
			return "O_01 petal record is malformed at index "+str(index)
	return ""

func _stage35_error(state: State, require_uploaded: bool = false) -> String:
	if state != null and state.mode_stage == 9:
		return _rei.preflight(state,_upload if require_uploaded else null)
	if _source.is_empty() or state == null or state.game_mode != 0x15 or state.mode_stage not in [3, 4, 5, 6, 7, 8, 9] or state.mode_step != 0:
		return "stage 3..7 renderer is outside its prepared source mode"
	var layer0: Variant = state.opening.get("bg_layer_0")
	if not layer0 is Dictionary or (layer0.get("enabled") != 1 and not (((state.mode_stage == 8 and state.substep == 7) or (state.mode_stage == 9 and state.substep == 0)) and layer0.get("enabled") == 0)) or layer0.get("clut_row") not in [494, 495] or not layer0.get("cut_batch_key") is String or str(layer0.cut_batch_key).is_empty():
		return "stage 3..7 renderer lacks bg_layer_0 state"
	if require_uploaded:
		var cut_ready: Variant = _upload.batch_ready(str(layer0.cut_batch_key))
		if not cut_ready is String or not cut_ready.is_empty():
			return cut_ready if cut_ready is String else "stage 3..7 cut upload owner returned malformed readiness"
	if state.opening.has("actor96"):
		var a96: Variant = state.opening.get("actor96")
		if not a96 is Dictionary or a96.get("profile") not in ["o12_rei_hallway", "o05_rei_eyes", "o06_yoshio_body", "o07_yoshio_body", "o08_rei_banner", "o09_rei_entrance"] or not a96.get("upload_batch_key") is String or str(a96.upload_batch_key).is_empty():
			return "stage 3..7 actor96 state is malformed"
		if a96.get("profile") == "o08_rei_banner":
			if state.mode_stage != 8 or state.opening.get("actor96_draw_flags") not in [0x84,0xa4] or a96.get("clut_row") not in [494,504] or a96.get("frame_index") not in [0,1,2,3] or not a96.get("brightness") is int or a96.brightness < 0 or a96.brightness > 128:
				return "Rei banner actor differs from its source draw state"
		elif a96.get("profile") == "o09_rei_entrance":
			if state.mode_stage != 9 or state.substep != 0 or state.opening.get("actor96_draw_flags") != 0xa4 or a96.get("frame_index") != 0 or a96.get("clut_row") != 505:
				return "Rei entrance is outside its original transition frame"
		elif state.opening.get("actor96_draw_flags") != 0xa4:
			return "stage 3..7 actor96 draw flags differ from source"
		if a96.get("profile") in ["o06_yoshio_body", "o07_yoshio_body", "o09_rei_entrance"] and (not a96.get("upload_batch_key_2") is String or str(a96.upload_batch_key_2).is_empty()):
			return "stage 6..7 actor96 lacks second tpage upload batch"
		if a96.get("profile") == "o07_yoshio_body" and (not a96.get("upload_batch_key_3") is String or str(a96.upload_batch_key_3).is_empty()):
			return "stage 7 actor96 lacks third tpage upload batch"
		if require_uploaded:
			var a96_ready: Variant = _upload.batch_ready(str(a96.upload_batch_key))
			if not a96_ready is String or not a96_ready.is_empty():
				return a96_ready if a96_ready is String else "stage 3..7 actor96 upload owner returned malformed readiness"
			if a96.get("profile") in ["o06_yoshio_body", "o07_yoshio_body", "o09_rei_entrance"]:
				var a96_ready2: Variant = _upload.batch_ready(str(a96.upload_batch_key_2))
				if not a96_ready2 is String or not a96_ready2.is_empty():
					return a96_ready2 if a96_ready2 is String else "stage 6..7 actor96 second upload owner returned malformed readiness"
			if a96.get("profile") == "o07_yoshio_body":
				var a96_ready3: Variant = _upload.batch_ready(str(a96.upload_batch_key_3))
				if not a96_ready3 is String or not a96_ready3.is_empty():
					return a96_ready3 if a96_ready3 is String else "stage 7 actor96 third upload owner returned malformed readiness"
	elif state.opening.get("actor96_draw_flags") != 0:
		return "stage 3..7 cleared actor96 requires draw_flags=0"
	if state.opening.has("actor97"):
		var a97: Variant = state.opening.get("actor97")
		if not a97 is Dictionary or a97.get("profile") not in ["o12_yoshio_arm", "o06_yoshio_mouth", "o07_yoshio_mouth"] or a97.get("clut_row") not in [499, 500, 502, 509] or not a97.get("upload_batch_key") is String or str(a97.upload_batch_key).is_empty() or state.opening.get("actor97_draw_flags") != 0xa4:
			return "stage 3..7 actor97 state is malformed"
		if a97.get("profile") == "o07_yoshio_mouth" and (not a97.get("upload_batch_key_2") is String or str(a97.upload_batch_key_2).is_empty()):
			return "stage 7 actor97 lacks second tpage upload batch"
		if require_uploaded:
			var a97_ready: Variant = _upload.batch_ready(str(a97.upload_batch_key))
			if not a97_ready is String or not a97_ready.is_empty():
				return a97_ready if a97_ready is String else "stage 3..7 actor97 upload owner returned malformed readiness"
			if a97.get("profile") == "o07_yoshio_mouth":
				var a97_ready2: Variant = _upload.batch_ready(str(a97.upload_batch_key_2))
				if not a97_ready2 is String or not a97_ready2.is_empty():
					return a97_ready2 if a97_ready2 is String else "stage 7 actor97 second upload owner returned malformed readiness"
	if state.opening.has("actor98"):
		var a98: Variant = state.opening.get("actor98")
		if not a98 is Dictionary or a98.get("profile") not in ["o06_yoshio_collar", "o07_yoshio_hand"] or a98.get("clut_row") not in [500, 502] or not a98.get("upload_batch_key") is String or str(a98.upload_batch_key).is_empty() or state.opening.get("actor98_draw_flags") != 0xa4:
			return "stage 6..7 actor98 state is malformed"
		if require_uploaded:
			var a98_ready: Variant = _upload.batch_ready(str(a98.upload_batch_key))
			if not a98_ready is String or not a98_ready.is_empty():
				return a98_ready if a98_ready is String else "stage 6..7 actor98 upload owner returned malformed readiness"
	if state.opening.has("actor99"):
		var a99: Variant = state.opening.get("actor99")
		if not a99 is Dictionary or a99.get("profile") != "o06_crowd_girls" or a99.get("clut_row") != 501 or not a99.get("upload_batch_key") is String or str(a99.upload_batch_key).is_empty() or state.opening.get("actor99_draw_flags") != 0xa4:
			return "stage 6 actor99 state is malformed"
		if require_uploaded:
			var a99_ready: Variant = _upload.batch_ready(str(a99.upload_batch_key))
			if not a99_ready is String or not a99_ready.is_empty():
				return a99_ready if a99_ready is String else "stage 6 actor99 upload owner returned malformed readiness"
	for extra_idx: int in [100, 101, 102, 103]:
		var extra_key := "actor%d" % extra_idx
		if state.opening.has(extra_key):
			var ex: Variant = state.opening.get(extra_key)
			if not ex is Dictionary or ex.get("profile") not in ["o06_mouth_girl_a", "o06_mouth_girl_b", "o06_yoshio_blink", "o06_yoshio_sweat"] or ex.get("clut_row") not in [500, 501] or not ex.get("upload_batch_key") is String or str(ex.upload_batch_key).is_empty() or state.opening.get("%s_draw_flags" % extra_key) != 0xa4:
				return "stage 6 %s state is malformed" % extra_key
			if require_uploaded:
				var ex_ready: Variant = _upload.batch_ready(str(ex.upload_batch_key))
				if not ex_ready is String or not ex_ready.is_empty():
					return ex_ready if ex_ready is String else "stage 6 %s upload owner returned malformed readiness" % extra_key
				if ex.has("upload_batch_key_2"):
					var ex_ready2: Variant = _upload.batch_ready(str(ex.upload_batch_key_2))
					if not ex_ready2 is String or not ex_ready2.is_empty():
						return ex_ready2 if ex_ready2 is String else "stage 6 %s second upload owner returned malformed readiness" % extra_key
	return _visible_error(state, require_uploaded)

static func _s16(value: int) -> int:
	var masked := value & 0xffff
	return masked - 0x10000 if (masked & 0x8000) != 0 else masked

func preflight_visible(state: State) -> String:
	# Pure source/owner validation for the mode callback before draw_frame.
	# The actual sequence counter changes only in logical_draw below.
	if state != null and state.mode_stage in [3, 4, 5, 6, 7, 8, 9] and state.opening.get("bg_layer_0") is Dictionary:
		return _stage35_error(state)
	if not state.opening.has("actor96"):
		var petal_failure := _petals_error(state)
		if not petal_failure.is_empty(): return petal_failure
		return _visible_error(state)
	var failure := _actor_error(state)
	if not failure.is_empty(): return failure
	var arrival_boundary: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0
	var board_read_pending: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("read_requested") == true and state.opening.board_media.get("decoded") == false
	if (not arrival_boundary and not board_read_pending) or state.opening.actor96_draw_flags != 0xa4:
		return "O_01 preflight is outside the arrival-to-board visible boundary"
	return _visible_error(state)

func logical_draw(state: State, _rng, result: Session.FrameResult) -> void:
	if state != null and state.mode_stage == 9:
		result.error = _rei.logical_draw(state)
		return
	# draw_frame runs after game_mode_dispatch's frame_count increment.
	if state != null and state.mode_stage in [3, 4, 5, 6, 7, 8, 9] and state.opening.get("bg_layer_0") is Dictionary:
		result.error = _stage35_error(state)
		if not result.error.is_empty(): return
		if state.opening.has("actor96"):
			var a96: Dictionary = state.opening.actor96
			var d96_rec := {"frame_index":int(a96.frame_index),"x":int(a96.x),"y":int(a96.y),"source_frame_count":state.frame_count,"batch_key":str(a96.upload_batch_key)}
			if a96.has("upload_batch_key_2"):
				d96_rec["batch_key_2"] = str(a96.upload_batch_key_2)
			if a96.has("upload_batch_key_3"):
				d96_rec["batch_key_3"] = str(a96.upload_batch_key_3)
			state.opening[DRAW_KEY] = d96_rec
			if a96.get("profile") == "o05_rei_eyes" and (int(a96.get("flags", 0)) & 1) != 0:
				var durations := [2, 2, 1, 2, 2]
				var frames := [0, 1, 2, 3, 2]
				var step: int = int(a96.get("seq_step", 0))
				a96["sequence_counter"] = int(a96.get("sequence_counter", 0)) + 1
				if int(a96.sequence_counter) >= durations[step]:
					a96["sequence_counter"] = 0
					var flg: int = int(a96.flags)
					if (flg & 0x10) != 0:
						step += 1
						flg &= ~0x20
						if step >= 5:
							flg = (flg & ~0x10) | 0x20
							step = 2
					elif (flg & 0x20) != 0:
						step -= 1
						if step <= 0:
							step = 0
							flg &= ~0x21
					a96["flags"] = flg
					a96["seq_step"] = step
					a96["frame_index"] = frames[step]
				state.opening["actor96"] = a96
		else:
			state.opening.erase(DRAW_KEY)
		if state.opening.has("actor97"):
			var a97: Dictionary = state.opening.actor97
			var d97_rec := {"profile":str(a97.get("profile","")),"frame_index":int(a97.frame_index),"x":int(a97.x),"y":int(a97.y),"clut_row":int(a97.get("clut_row",499)),"source_frame_count":state.frame_count,"batch_key":str(a97.upload_batch_key)}
			if a97.has("upload_batch_key_2"):
				d97_rec["batch_key_2"] = str(a97.upload_batch_key_2)
			state.opening["actor97_source_draw"] = d97_rec
			if a97.get("profile") == "o12_yoshio_arm" and (int(a97.get("flags", 0)) & 0x40) == 0 and (int(a97.get("flags", 0)) & 1) == 1:
				var durations97 := [18, 18, 18, 18, 72]
				var frames97 := [0, 1, 0, 1, 0]
				var step97: int = int(a97.get("seq_step", 0))
				a97["sequence_counter"] = int(a97.get("sequence_counter", 0)) + 1
				if int(a97.sequence_counter) >= durations97[step97]:
					step97 += 1
					a97["sequence_counter"] = 0
				var flg97: int = int(a97.flags)
				if step97 >= 5:
					if (flg97 & 4) != 0:
						flg97 &= 0xfffa
						step97 -= 1
				a97["flags"] = flg97
				a97["seq_step"] = step97
				a97["frame_index"] = frames97[step97]
				state.opening["actor97"] = a97
		else:
			state.opening.erase("actor97_source_draw")
		if state.opening.has("actor98"):
			var a98: Dictionary = state.opening.actor98
			state.opening["actor98_source_draw"] = {"profile":str(a98.get("profile","")),"frame_index":int(a98.frame_index),"x":int(a98.x),"y":int(a98.y),"clut_row":int(a98.get("clut_row",500)),"source_frame_count":state.frame_count,"batch_key":str(a98.upload_batch_key)}
		else:
			state.opening.erase("actor98_source_draw")
		if state.opening.has("actor99"):
			var a99: Dictionary = state.opening.actor99
			state.opening["actor99_source_draw"] = {"frame_index":int(a99.frame_index),"source_frame_count":state.frame_count,"batch_key":str(a99.upload_batch_key)}
			if (int(a99.get("flags", 0)) & 0x40) == 0 and (int(a99.get("flags", 0)) & 1) == 1:
				var step99: int = int(a99.get("seq_step", 0))
				a99["sequence_counter"] = int(a99.get("sequence_counter", 0)) + 1
				if int(a99.sequence_counter) >= 4:
					step99 += 1
					a99["sequence_counter"] = 0
				var flg99: int = int(a99.flags)
				if step99 >= 3:
					if (flg99 & 4) != 0:
						flg99 &= 0xfffa
						step99 -= 1
					else:
						step99 = 0
				a99["flags"] = flg99
				a99["seq_step"] = step99
				a99["frame_index"] = step99
				state.opening["actor99"] = a99
		else:
			state.opening.erase("actor99_source_draw")
		if state.opening.has("actor100"):
			var a100: Dictionary = state.opening.actor100
			state.opening["actor100_source_draw"] = {"frame_index":int(a100.frame_index),"source_frame_count":state.frame_count,"batch_key":str(a100.upload_batch_key)}
			if (int(a100.get("flags", 0)) & 0x40) == 0 and (int(a100.get("flags", 0)) & 1) == 1:
				var step100: int = int(a100.get("seq_step", 0))
				a100["sequence_counter"] = int(a100.get("sequence_counter", 0)) + 1
				if int(a100.sequence_counter) >= 8:
					step100 = (step100 + 1) % 2
					a100["sequence_counter"] = 0
				a100["seq_step"] = step100
				a100["frame_index"] = 3 if step100 == 0 else 4
				state.opening["actor100"] = a100
		else:
			state.opening.erase("actor100_source_draw")
		if state.opening.has("actor101"):
			var a101: Dictionary = state.opening.actor101
			state.opening["actor101_source_draw"] = {"frame_index":int(a101.frame_index),"source_frame_count":state.frame_count,"batch_key":str(a101.upload_batch_key)}
			if (int(a101.get("flags", 0)) & 0x40) == 0 and (int(a101.get("flags", 0)) & 1) == 1:
				var step101: int = int(a101.get("seq_step", 0))
				a101["sequence_counter"] = int(a101.get("sequence_counter", 0)) + 1
				if int(a101.sequence_counter) >= 8:
					step101 = (step101 + 1) % 2
					a101["sequence_counter"] = 0
				a101["seq_step"] = step101
				a101["frame_index"] = 5 if step101 == 0 else 6
				state.opening["actor101"] = a101
		else:
			state.opening.erase("actor101_source_draw")
		if state.opening.has("actor102"):
			var a102: Dictionary = state.opening.actor102
			state.opening["actor102_source_draw"] = {"frame_index":int(a102.frame_index),"source_frame_count":state.frame_count,"batch_key":str(a102.upload_batch_key),"batch_key_2":str(a102.get("upload_batch_key_2",""))}
			if (int(a102.get("flags", 0)) & 0x40) == 0 and (int(a102.get("flags", 0)) & 1) == 1:
				var step102: int = int(a102.get("seq_step", 0))
				a102["sequence_counter"] = int(a102.get("sequence_counter", 0)) + 1
				if int(a102.sequence_counter) >= 22:
					step102 += 1
					a102["sequence_counter"] = 0
				var flg102: int = int(a102.flags)
				if step102 >= 2:
					step102 = 0
					if (flg102 & 2) != 0:
						flg102 &= ~3
				a102["flags"] = flg102
				a102["seq_step"] = step102
				a102["frame_index"] = 1 if step102 == 0 else 2
				state.opening["actor102"] = a102
		else:
			state.opening.erase("actor102_source_draw")
		if state.opening.has("actor103"):
			var a103: Dictionary = state.opening.actor103
			state.opening["actor103_source_draw"] = {"frame_index":int(a103.frame_index),"source_frame_count":state.frame_count,"batch_key":str(a103.upload_batch_key)}
		else:
			state.opening.erase("actor103_source_draw")
		return
	if not state.opening.has("actor96"):
		state.opening.erase(DRAW_KEY)
		if not state.opening.has("prologue_petals"): return
		result.error = _petals_error(state)
		if not result.error.is_empty(): return
		result.error = _visible_error(state)
		if not result.error.is_empty(): return
		state.opening[PETALS_DRAW_KEY] = {"source_frame_count":state.frame_count,"batch_key":state.opening.arrival_media.upload_batch_key,"board_batch_key":state.opening.board_media.upload_batch_key,"petals":state.opening.prologue_petals.duplicate(true)}
		var petals: Array = state.opening.prologue_petals
		for index in 8:
			var petal: Dictionary = petals[index]
			if (int(petal.draw_flags) & 0x80) == 0: continue
			var seq_cnt: int = int(petal.get("unk_08", 0)) + 1
			if seq_cnt >= 5:
				seq_cnt = 0
				petal["unk_18"] = (int(petal.unk_18) + 1) % 12
			petal["unk_08"] = seq_cnt
			petals[index] = petal
		state.opening["prologue_petals"] = petals
		return
	result.error = _actor_error(state)
	if not result.error.is_empty(): return
	if state.opening.actor96_draw_flags == 0x24:
		state.opening.erase(DRAW_KEY)
		return
	var arrival_boundary: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0
	var board_read_pending: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("read_requested") == true and state.opening.board_media.get("decoded") == false
	if not arrival_boundary and not board_read_pending:
		result.error = "O_01 visible adapter supports only the arrival-to-board boundary"
		return
	var failure := _visible_error(state)
	if not failure.is_empty(): result.error = failure; return
	var actor: Dictionary = state.opening.actor96
	state.opening[DRAW_KEY] = {"frame_index":actor.frame_index,"source_frame_count":state.frame_count,"batch_key":state.opening.arrival_media.upload_batch_key}
	if state.frame_count % 2 == 0: actor.sequence_counter += 1
	if actor.sequence_counter >= [3,3,3,1,1][actor.frame_index]:
		actor.sequence_counter = 0
		actor.frame_index = (actor.frame_index+1)%5

func _visible_error(state: State, require_uploaded: bool = true) -> String:
	if _upload == null or _store == null: return "O_01 lost its upload owner"
	var context := _upload.actor_render_context(_store)
	if not context.get("error") is String or not context.error.is_empty() or not context.get("runtime") is Dictionary or not is_same(context.runtime,_runtime): return "O_01 sampled runtime is foreign to its upload owner"
	var error := _runtime_error(_runtime)
	if not error.is_empty(): return error
	if not _runtime.get("vram_known_mask") is PackedByteArray: return "O_01 texture/CLUT have not been uploaded"
	var media: Variant = state.opening.get("arrival_media")
	if not media is Dictionary or not media.get("upload_batch_key") is String: return "O_01 visible draw lost its upload batch"
	if require_uploaded:
		var ready: Variant = _upload.batch_ready(media.upload_batch_key)
		if not ready is String or not ready.is_empty(): return ready if ready is String else "O_01 upload owner returned malformed readiness"
	if not state.opening.get("screen_brightness") is int or state.opening.screen_brightness != 128: return "O_01 boundary requires actual source full brightness"
	return ""

func paint(target: Image, state: State, normalized_draw_origin: Vector2i) -> String:
	# Caller owns the draw-buffer offset normalization. Source initializes native
	# origins (160,360)/(160,120); normalized viewport origin is (160,120).
	if target == null or target.get_size() != Vector2i(320,240) or normalized_draw_origin != Vector2i(160,120): return "O_01 target/origin is unsupported"
	if state != null and state.mode_stage == 9: return _rei.paint(target,state,_runtime,_upload,false)
	if state != null and state.mode_stage in [3, 4, 5, 6, 7, 8, 9] and state.opening.get("bg_layer_0") is Dictionary:
		var err35 := _stage35_error(state, true)
		if not err35.is_empty(): return err35
		var actor_sig: Array = [_last_names_bg_key]
		for a_key: String in ["actor96", "actor97", "actor98", "actor99", "actor100", "actor101", "actor102", "actor103"]:
			if state.opening.has(a_key):
				var a_val: Dictionary = state.opening[a_key]
				var d_key: String = DRAW_KEY if a_key == "actor96" else ("%s_source_draw" % a_key)
				var d_val: Variant = state.opening.get(d_key)
				if not d_val is Dictionary or d_val.get("source_frame_count") != state.frame_count or d_val.get("batch_key") != a_val.get("upload_batch_key"):
					return "stage 3..7 %s needs this committed frame's original draw record" % a_key
				if a_val.has("upload_batch_key_2") and d_val.get("batch_key_2") != a_val.get("upload_batch_key_2"):
					return "stage 3..7 %s needs this committed frame's second draw record" % a_key
				if a_val.has("upload_batch_key_3") and d_val.get("batch_key_3") != a_val.get("upload_batch_key_3"):
					return "stage 3..7 %s needs this committed frame's third draw record" % a_key
				actor_sig.append([a_key, a_val.get("profile"), d_val.get("frame_index"), d_val.get("x", a_val.get("x")), d_val.get("y", a_val.get("y")), d_val.get("clut_row", a_val.get("clut_row")), d_val.get("batch_key"), d_val.get("batch_key_2", ""), d_val.get("batch_key_3", "")])
		var can_cache_stage35: bool = state.mode_stage != 8 and target.get_instance_id() == _last_names_bg_target_id and not _last_names_bg_key.is_empty()
		if can_cache_stage35 and _stage35_actor_cache_bg_key != _last_names_bg_key:
			_stage35_actor_cache.clear()
			_stage35_actor_cache_bg_key = _last_names_bg_key
		var stage_actor_cache_key := JSON.stringify(actor_sig) if can_cache_stage35 else ""
		if can_cache_stage35 and _stage35_actor_cache.has(stage_actor_cache_key):
			var cached_stage_img: Image = _stage35_actor_cache[stage_actor_cache_key]
			target.blit_rect(cached_stage_img, Rect2i(0, 0, 320, 160), Vector2i.ZERO)
			return ""
		var stage_img := target.duplicate()
		var s_vram: PackedByteArray = _runtime.vram_words
		var s_known: PackedByteArray = _runtime.vram_known_mask
		if state.opening.has("actor96"):
			var a96: Dictionary = state.opening.actor96
			var d96: Variant = state.opening.get(DRAW_KEY)
			if not d96 is Dictionary or d96.get("source_frame_count") != state.frame_count or d96.get("batch_key") != a96.get("upload_batch_key"):
				return "stage 3..7 actor96 needs this committed frame's original draw record"
			var profile96: String = str(a96.get("profile",""))
			var f_idx: int = int(d96.get("frame_index", 0))
			if profile96 in ["o06_yoshio_body","o09_rei_entrance"] and d96.get("batch_key_2") != a96.get("upload_batch_key_2"):
				return "stage 6 actor96 needs this committed frame's second draw record"
			if profile96 == "o07_yoshio_body" and (d96.get("batch_key_2") != a96.get("upload_batch_key_2") or d96.get("batch_key_3") != a96.get("upload_batch_key_3")):
				return "stage 7 actor96 needs this committed frame's second and third draw records"
			var bg_a96_key := JSON.stringify([_last_names_bg_key, profile96, f_idx, d96.get("x"), d96.get("y"), d96.get("batch_key"), d96.get("batch_key_2", ""), d96.get("batch_key_3", "")]) if can_cache_stage35 else ""
			if can_cache_stage35 and _stage35_bg_a96_cache_image != null and _stage35_bg_a96_cache_key == bg_a96_key:
				stage_img.blit_rect(_stage35_bg_a96_cache_image, Rect2i(0, 0, 320, 160), Vector2i.ZERO)
			else:
				var parts96: Array = []
				var clut_row96 := 497
				if profile96 == "o12_rei_hallway":
					clut_row96 = 497
					parts96 = [{"x": 152, "y": 32, "u": 0, "v": 0, "w": 48, "h": 64, "slot_x": 320}]
				elif profile96 == "o05_rei_eyes":
					clut_row96 = 498
					parts96 = [{"x": 136, "y": 40, "u": f_idx * 32, "v": 0, "w": 32, "h": 32, "slot_x": 320}]
				elif profile96 == "o08_rei_banner":
					clut_row96 = int(a96.clut_row)
					parts96 = [{"x":176,"y":80,"u":f_idx * 32,"v":0,"w":32,"h":32,"slot_x":320}]
				elif profile96 == "o09_rei_entrance":
					clut_row96 = 505
					parts96 = [{"x":128,"y":0,"u":0,"v":0,"w":80,"h":80,"slot_x":320},{"x":88,"y":80,"u":0,"v":0,"w":80,"h":80,"slot_x":384},{"x":168,"y":80,"u":0,"v":80,"w":64,"h":48,"slot_x":384},{"x":168,"y":128,"u":64,"v":96,"w":64,"h":32,"slot_x":384}]
				elif profile96 == "o06_yoshio_body":
					clut_row96 = 500
					parts96 = [
						{"x": 104, "y": 0, "u": 0, "v": 96, "w": 112, "h": 32, "slot_x": 384},
						{"x": 104, "y": 32, "u": 112, "v": 32, "w": 16, "h": 16, "slot_x": 320},
						{"x": 120, "y": 32, "u": 0, "v": 0, "w": 96, "h": 48, "slot_x": 320},
						{"x": 120, "y": 80, "u": 0, "v": 48, "w": 128, "h": 80, "slot_x": 320},
					]
				elif profile96 == "o07_yoshio_body":
					clut_row96 = 502
					parts96 = [
						{"x": 32, "y": 0, "u": 32, "v": 0, "w": 96, "h": 32, "slot_x": 320},
						{"x": 0, "y": 32, "u": 0, "v": 32, "w": 128, "h": 64, "slot_x": 320},
						{"x": 0, "y": 96, "u": 0, "v": 96, "w": 80, "h": 32, "slot_x": 320},
						{"x": 128, "y": 0, "u": 0, "v": 80, "w": 32, "h": 48, "slot_x": 384},
						{"x": 0, "y": 128, "u": 0, "v": 0, "w": 80, "h": 32, "slot_x": 384},
					]
				for p96: Dictionary in parts96:
					var base_px96: int = normalized_draw_origin.x + int(d96.x) + int(p96.x)
					var base_py96: int = normalized_draw_origin.y + int(d96.y) + int(p96.y)
					var slot_x96: int = int(p96.slot_x)
					for y in int(p96.h):
						for x in int(p96.w):
							var u: int = int(p96.u) + x
							var v: int = int(p96.v) + y
							var at: int = (v * 1024 + slot_x96) * 2 + u
							if s_known[at] != 255: return "stage 3..7 actor96 texture references unknown VRAM"
							var idx: int = s_vram[at]
							var clut: int = (clut_row96 * 1024 + idx) * 2
							if s_known[clut] != 255 or s_known[clut + 1] != 255: return "stage 3..7 actor96 palette references unknown VRAM"
							var color: int = s_vram.decode_u16(clut)
							if color == 0: continue
							var px: int = base_px96 + x
							var py: int = base_py96 + y
							if px < 0 or px >= 320 or py < 0 or py >= 160: continue
							var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
							if profile96 == "o08_rei_banner":
								var tint: int = int(state.opening.get("screen_brightness",128)) if (int(a96.draw_flags) & 0x20) != 0 else int(a96.brightness)
								r = mini(31,(r * tint) >> 7)
								g = mini(31,(g * tint) >> 7)
								b = mini(31,(b * tint) >> 7)
							if (color & 0x8000) != 0:
								var back: Color = stage_img.get_pixel(px, py)
								r = mini(31, ((back.r8 >> 3) + r) >> 1)
								g = mini(31, ((back.g8 >> 3) + g) >> 1)
								b = mini(31, ((back.b8 >> 3) + b) >> 1)
							stage_img.set_pixel(px, py, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
				if can_cache_stage35:
					_stage35_bg_a96_cache_image = stage_img.get_region(Rect2i(0, 0, 320, 160))
					_stage35_bg_a96_cache_key = bg_a96_key
		if state.opening.has("actor97"):
			var a97: Dictionary = state.opening.actor97
			var d97: Variant = state.opening.get("actor97_source_draw")
			if not d97 is Dictionary or d97.get("source_frame_count") != state.frame_count or d97.get("batch_key") != a97.get("upload_batch_key"):
				return "stage 3..7 actor97 needs this committed frame's original draw record"
			var profile97: String = str(d97.get("profile", a97.get("profile", "")))
			var f_idx97: int = int(d97.get("frame_index", 0))
			var clut_row97: int = int(d97.get("clut_row", 499))
			var parts97: Array = []
			if profile97 == "o12_yoshio_arm":
				parts97 = [
					{"x": 160, "y": 96, "u": 64 if f_idx97 == 1 else 0, "v": 64, "w": 64, "h": 64, "slot_x": 384},
					{"x": 224, "y": 96, "u": 0, "v": 0, "w": 96, "h": 64, "slot_x": 384},
				]
			elif profile97 == "o06_yoshio_mouth":
				parts97 = [
					{"x": 144, "y": 64, "u": (f_idx97 - 3) * 32, "v": 0, "w": 32, "h": 32, "slot_x": 384},
				]
			elif profile97 == "o07_yoshio_mouth":
				if d97.get("batch_key_2") != a97.get("upload_batch_key_2"):
					return "stage 7 actor97 needs this committed frame's second draw record"
				if f_idx97 == 1:
					parts97 = [{"x": 64, "y": 64, "u": 0, "v": 0, "w": 32, "h": 32, "slot_x": 320}]
				else:
					parts97 = [{"x": 64, "y": 64, "u": 96, "v": (4 - f_idx97) * 32, "w": 32, "h": 32, "slot_x": 384}]
			for p97: Dictionary in parts97:
				var base_px97: int = normalized_draw_origin.x + int(d97.x) + int(p97.x)
				var base_py97: int = normalized_draw_origin.y + int(d97.y) + int(p97.y)
				var slot_x97: int = int(p97.get("slot_x", 384))
				for y in int(p97.h):
					for x in int(p97.w):
						var u: int = int(p97.u) + x
						var v: int = int(p97.v) + y
						var at: int = (v * 1024 + slot_x97) * 2 + u
						if s_known[at] != 255: return "stage 3..7 actor97 texture references unknown VRAM"
						var idx: int = s_vram[at]
						var clut: int = (clut_row97 * 1024 + idx) * 2
						if s_known[clut] != 255 or s_known[clut + 1] != 255: return "stage 3..7 actor97 palette references unknown VRAM"
						var color: int = s_vram.decode_u16(clut)
						if color == 0: continue
						var px: int = base_px97 + x
						var py: int = base_py97 + y
						if px < 0 or px >= 320 or py < 0 or py >= 160: continue
						var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
						if (color & 0x8000) != 0:
							var back: Color = stage_img.get_pixel(px, py)
							r = mini(31, ((back.r8 >> 3) + r) >> 1)
							g = mini(31, ((back.g8 >> 3) + g) >> 1)
							b = mini(31, ((back.b8 >> 3) + b) >> 1)
						stage_img.set_pixel(px, py, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
		if state.opening.has("actor98"):
			var a98: Dictionary = state.opening.actor98
			var d98: Variant = state.opening.get("actor98_source_draw")
			if not d98 is Dictionary or d98.get("source_frame_count") != state.frame_count or d98.get("batch_key") != a98.get("upload_batch_key"):
				return "stage 6..7 actor98 needs this committed frame's original draw record"
			var profile98: String = str(d98.get("profile", a98.get("profile", "")))
			var f_idx98: int = int(d98.get("frame_index", 0))
			var clut_row98: int = int(d98.get("clut_row", 500))
			var parts98: Array = []
			if profile98 == "o06_yoshio_collar":
				if f_idx98 == 8:
					parts98 = [{"x": 112, "y": 152, "u": 96, "v": 0, "w": 32, "h": 16, "slot_x": 320}]
				elif f_idx98 == 9:
					parts98 = [{"x": 107, "y": 146, "u": 96, "v": 0, "w": 32, "h": 16, "slot_x": 320}]
				else:
					parts98 = [{"x": 102, "y": 144, "u": 96, "v": 0, "w": 32, "h": 16, "slot_x": 320}]
			elif profile98 == "o07_yoshio_hand":
				parts98 = [{"x": 28, "y": 88, "u": 96, "v": (8 - f_idx98) * 16, "w": 16, "h": 16, "slot_x": 448}]
			for p98: Dictionary in parts98:
				var base_px98: int = normalized_draw_origin.x + int(d98.x) + int(p98.x)
				var base_py98: int = normalized_draw_origin.y + int(d98.y) + int(p98.y)
				var slot_x98: int = int(p98.slot_x)
				for y in int(p98.h):
					for x in int(p98.w):
						var u: int = int(p98.u) + x
						var v: int = int(p98.v) + y
						var at: int = (v * 1024 + slot_x98) * 2 + u
						if s_known[at] != 255: return "stage 6..7 actor98 texture references unknown VRAM"
						var idx: int = s_vram[at]
						var clut: int = (clut_row98 * 1024 + idx) * 2
						if s_known[clut] != 255 or s_known[clut + 1] != 255: return "stage 6..7 actor98 palette references unknown VRAM"
						var color: int = s_vram.decode_u16(clut)
						if color == 0: continue
						var px: int = base_px98 + x
						var py: int = base_py98 + y
						if px < 0 or px >= 320 or py < 0 or py >= 160: continue
						var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
						if (color & 0x8000) != 0:
							var back: Color = stage_img.get_pixel(px, py)
							r = mini(31, ((back.r8 >> 3) + r) >> 1)
							g = mini(31, ((back.g8 >> 3) + g) >> 1)
							b = mini(31, ((back.b8 >> 3) + b) >> 1)
						stage_img.set_pixel(px, py, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
		if state.opening.has("actor99"):
			var a99: Dictionary = state.opening.actor99
			var d99: Variant = state.opening.get("actor99_source_draw")
			if not d99 is Dictionary or d99.get("source_frame_count") != state.frame_count or d99.get("batch_key") != a99.get("upload_batch_key"):
				return "stage 6 actor99 needs this committed frame's original draw record"
			var f_idx99: int = int(d99.get("frame_index", 0))
			var parts99: Array = [
				{"x": 240, "y": 32, "u": 0, "v": 0, "w": 48, "h": 64, "slot_x": 448},
				{"x": 288, "y": 32, "u": 48, "v": 0, "w": 32, "h": 64, "slot_x": 448},
			]
			if f_idx99 == 1:
				parts99 = [
					{"x": 240, "y": 32, "u": 0, "v": 64, "w": 48, "h": 64, "slot_x": 448},
					{"x": 288, "y": 32, "u": 48, "v": 64, "w": 32, "h": 64, "slot_x": 448},
				]
			elif f_idx99 == 2:
				parts99 = [
					{"x": 240, "y": 32, "u": 80, "v": 0, "w": 48, "h": 64, "slot_x": 448},
					{"x": 288, "y": 32, "u": 80, "v": 64, "w": 32, "h": 64, "slot_x": 448},
				]
			for p99: Dictionary in parts99:
				var base_px99: int = normalized_draw_origin.x + int(a99.x) + int(p99.x)
				var base_py99: int = normalized_draw_origin.y + int(a99.y) + int(p99.y)
				var slot_x99: int = int(p99.slot_x)
				for y in int(p99.h):
					for x in int(p99.w):
						var u: int = int(p99.u) + x
						var v: int = int(p99.v) + y
						var at: int = (v * 1024 + slot_x99) * 2 + u
						if s_known[at] != 255: return "stage 6 actor99 texture references unknown VRAM"
						var idx: int = s_vram[at]
						var clut: int = (501 * 1024 + idx) * 2
						if s_known[clut] != 255 or s_known[clut + 1] != 255: return "stage 6 actor99 palette references unknown VRAM"
						var color: int = s_vram.decode_u16(clut)
						if color == 0: continue
						var px: int = base_px99 + x
						var py: int = base_py99 + y
						if px < 0 or px >= 320 or py < 0 or py >= 160: continue
						var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
						if (color & 0x8000) != 0:
							var back: Color = stage_img.get_pixel(px, py)
							r = mini(31, ((back.r8 >> 3) + r) >> 1)
							g = mini(31, ((back.g8 >> 3) + g) >> 1)
							b = mini(31, ((back.b8 >> 3) + b) >> 1)
						stage_img.set_pixel(px, py, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
		for extra_idx: int in [102, 100, 101, 103]:
			var extra_key := "actor%d" % extra_idx
			if state.opening.has(extra_key):
				var a_ex: Dictionary = state.opening[extra_key]
				var d_ex: Variant = state.opening.get("%s_source_draw" % extra_key)
				if not d_ex is Dictionary or d_ex.get("source_frame_count") != state.frame_count or d_ex.get("batch_key") != a_ex.get("upload_batch_key"):
					return "stage 6 %s needs this committed frame's original draw record" % extra_key
				if a_ex.has("upload_batch_key_2") and d_ex.get("batch_key_2") != a_ex.get("upload_batch_key_2"):
					return "stage 6 %s needs this committed frame's second draw record" % extra_key
				var f_ex: int = int(d_ex.get("frame_index", 0))
				var clut_ex: int = int(a_ex.get("clut_row", 500))
				var parts_ex: Array = []
				if extra_idx == 102:
					if f_ex == 1:
						parts_ex = [{"x": 152, "y": 32, "u": 32, "v": 0, "w": 32, "h": 32, "slot_x": 320}]
					else:
						parts_ex = [
							{"x": 152, "y": 32, "u": 0, "v": 32, "w": 32, "h": 32, "slot_x": 384},
							{"x": 152, "y": 32, "u": 0, "v": 32, "w": 32, "h": 32, "slot_x": 384},
						]
				elif extra_idx == 100:
					parts_ex = [{"x": 248, "y": 56, "u": 112, "v": 64 if f_ex == 3 else 80, "w": 16, "h": 16, "slot_x": 448}]
				elif extra_idx == 101:
					parts_ex = [{"x": 272, "y": 64, "u": 112, "v": 96 if f_ex == 5 else 112, "w": 16, "h": 16, "slot_x": 448}]
				elif extra_idx == 103:
					parts_ex = [
						{"x": 120, "y": 32, "u": 64, "v": 32, "w": 32, "h": 32, "slot_x": 384},
						{"x": 152, "y": 32, "u": 32, "v": 32, "w": 32, "h": 32, "slot_x": 384},
						{"x": 144, "y": 64, "u": 96, "v": 32, "w": 32, "h": 32, "slot_x": 384},
					]
				for p_ex: Dictionary in parts_ex:
					var base_px_ex: int = normalized_draw_origin.x + int(a_ex.x) + int(p_ex.x)
					var base_py_ex: int = normalized_draw_origin.y + int(a_ex.y) + int(p_ex.y)
					var slot_x_ex: int = int(p_ex.slot_x)
					for y in int(p_ex.h):
						for x in int(p_ex.w):
							var u: int = int(p_ex.u) + x
							var v: int = int(p_ex.v) + y
							var at: int = (v * 1024 + slot_x_ex) * 2 + u
							if s_known[at] != 255: return "stage 6 %s texture references unknown VRAM" % extra_key
							var idx: int = s_vram[at]
							var clut: int = (clut_ex * 1024 + idx) * 2
							if s_known[clut] != 255 or s_known[clut + 1] != 255: return "stage 6 %s palette references unknown VRAM" % extra_key
							var color: int = s_vram.decode_u16(clut)
							if color == 0: continue
							var px: int = base_px_ex + x
							var py: int = base_py_ex + y
							if px < 0 or px >= 320 or py < 0 or py >= 160: continue
							var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
							if (color & 0x8000) != 0:
								var back: Color = stage_img.get_pixel(px, py)
								r = mini(31, ((back.r8 >> 3) + r) >> 1)
								g = mini(31, ((back.g8 >> 3) + g) >> 1)
								b = mini(31, ((back.b8 >> 3) + b) >> 1)
							stage_img.set_pixel(px, py, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
		if can_cache_stage35:
			if _stage35_actor_cache.size() >= 48:
				_stage35_actor_cache.clear()
			_stage35_actor_cache[stage_actor_cache_key] = stage_img.get_region(Rect2i(0, 0, 320, 160))
		target.copy_from(stage_img)
		return ""
	if not state.opening.has("actor96"):
		if not state.opening.has("prologue_petals"): return ""
		var petal_err := _petals_error(state)
		if not petal_err.is_empty(): return petal_err
		petal_err = _visible_error(state)
		if not petal_err.is_empty(): return petal_err
		var board_key: String = state.opening.board_media.upload_batch_key
		var pre_cut_board: bool = state.mode_stage == 1 or (state.mode_stage == 2 and state.substep <= 5)
		if pre_cut_board and _board_strip_batch_key != board_key:
			var board_ready: Variant = _upload.batch_ready(board_key)
			if not board_ready is String or not board_ready.is_empty():
				return board_ready if board_ready is String else "O_01 board upload owner returned malformed readiness"
		var petal_draw: Variant = state.opening.get(PETALS_DRAW_KEY)
		if not petal_draw is Dictionary or petal_draw.size() != 4 or petal_draw.get("source_frame_count") != state.frame_count or petal_draw.get("batch_key") != state.opening.arrival_media.upload_batch_key or petal_draw.get("board_batch_key") != board_key or not petal_draw.get("petals") is Array or petal_draw.petals.size() != 8:
			return "O_01 petals need this committed frame's original draw record"
		var any_active_petal := false
		for p_chk: Dictionary in petal_draw.petals:
			if (int(p_chk.draw_flags) & 0x80) != 0:
				any_active_petal = true
				break
		if not any_active_petal: return ""
		var petal_image := target.duplicate()
		var petal_vram: PackedByteArray = _runtime.vram_words
		var petal_known: PackedByteArray = _runtime.vram_known_mask
		for petal: Dictionary in petal_draw.petals:
			if (int(petal.draw_flags) & 0x80) == 0: continue
			var actor_x := _s16((int(petal.x_word) >> 16) & 0xffff)
			var actor_y := _s16((int(petal.y_word) >> 16) & 0xffff)
			for part: Dictionary in _petal_frames[int(petal.unk_18)].parts:
				var base_px: int = normalized_draw_origin.x + actor_x + int(part.x) - int(part.get("ox", 7))
				var base_py: int = normalized_draw_origin.y + actor_y + int(part.y) - int(part.get("oy", 7))
				for y in part.h:
					var sy: int = part.h - 1 - y if (int(part.flip) & 1) != 0 else y
					for x in part.w:
						var sx: int = part.w - 1 - x if (int(part.flip) & 2) != 0 else x
						var u: int = part.u + sx
						var v: int = part.v + sy
						var at: int = ((256+v)*1024+640)*2+u
						if petal_known[at] != 255: return "O_01 texture references unknown VRAM"
						var index: int = petal_vram[at]
						var clut: int = (496*1024+index)*2
						if petal_known[clut] != 255 or petal_known[clut+1] != 255: return "O_01 palette references unknown VRAM"
						var color: int = petal_vram.decode_u16(clut)
						if color&0x8000: return "O_01 mode2 CLUT has unexpected STP"
						if color == 0: continue
						var px: int = base_px + x
						var py: int = base_py + y
						if px < 0 or px >= 320 or py < 0 or py >= 160: continue
						var r: int = color&31; var g: int = (color>>5)&31; var b: int = (color>>10)&31
						petal_image.set_pixel(px,py,Color8((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2),255))
		target.copy_from(petal_image)
		return ""
	var error := _actor_error(state)
	if not error.is_empty(): return error
	if state.opening.actor96_draw_flags == 0x24: return ""
	var arrival_boundary: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0
	var board_read_pending: bool = state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("read_requested") == true and state.opening.board_media.get("decoded") == false
	if not arrival_boundary and not board_read_pending: return "O_01 paint is outside the arrival-to-board boundary"
	error = _visible_error(state)
	if not error.is_empty(): return error
	var draw: Variant = state.opening.get(DRAW_KEY)
	if not draw is Dictionary or draw.size() != 3 or not draw.get("frame_index") is int or draw.frame_index < 0 or draw.frame_index >= 5 or not draw.get("source_frame_count") is int or draw.source_frame_count != state.frame_count or not draw.get("batch_key") is String or draw.batch_key != state.opening.arrival_media.upload_batch_key: return "O_01 needs this committed frame's original draw record"
	var image := target.duplicate()
	var vram: PackedByteArray = _runtime.vram_words
	var known: PackedByteArray = _runtime.vram_known_mask
	# AddPrim head insertion reverses queue order: source j=count-1..0 means
	# GPU part0 is drawn first. These source parts have no transforms.
	for part: Dictionary in _frames[draw.frame_index].parts:
		for y in part.h:
			for x in part.w:
				var u: int = part.u+x
				var v: int = part.v+y
				var at: int = ((256+v)*1024+640)*2+(u>>1)
				if known[at] != 255: return "O_01 texture references unknown VRAM"
				var index: int = (vram[at] >> ((u&1)*4))&15
				var clut: int = (496*1024+index)*2
				if known[clut] != 255 or known[clut+1] != 255: return "O_01 palette references unknown VRAM"
				var color: int = vram.decode_u16(clut)
				if color&0x8000: return "O_01 mode2 CLUT has unexpected STP"
				if color == 0: continue
				var px: int = normalized_draw_origin.x+state.opening.actor96.x+part.x+x
				var py: int = normalized_draw_origin.y+state.opening.actor96.y+part.y+y
				if px < 0 or px >= 320 or py < 0 or py >= 240: continue
				# PS1 five-bit framebuffer channels: no filtered/rescaled atlas.
				var r: int = color&31; var g: int = (color>>5)&31; var b: int = (color>>10)&31
				image.set_pixel(px,py,Color8((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2),255))
	target.copy_from(image)
	return ""

@warning_ignore("integer_division")
func paint_board_scroll(target: Image, state: State, board_preview: Image) -> String:
	# src/main/screen_layers.c::srn_tpage_show(5, 2, 0, &rect, prologue_scene_counter, 0, 0, 0, 0xD)
	if target == null or target.get_size() != Vector2i(320,240) or board_preview == null or board_preview.get_size() != Vector2i(640,160):
		return "O_01 board scroll target/preview is unsupported"
	var error := _petals_error(state)
	if not error.is_empty(): return error
	error = _visible_error(state)
	if not error.is_empty(): return error
	var stage1_scroll: bool = state.mode_stage == 1 and state.mode_step == 1 and state.substep in [1, 2, 3, 4, 5]
	var stage2_transition: bool = state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("active_draw") == "board_scroll_transition"
	if not stage1_scroll and not stage2_transition:
		return "O_01 board scroll is outside its active substep"
	var board_media: Dictionary = state.opening.board_media
	var board_key: String = board_media.upload_batch_key
	var tpage: Variant = board_media.get("tpage_show")
	var scroll_x: int = int(state.opening.prologue_scene_counter)
	if not tpage is Dictionary or board_media.get("scroll_x") != scroll_x or tpage.get("scroll_x") != scroll_x or tpage.get("tpage_x") != 5 or tpage.get("tpage_y") != 2 or tpage.get("ot_slot") != 13:
		return "O_01 board scroll lacks this frame's srn_tpage_show record"
	if _board_vram_strip == null or _board_strip_batch_key != board_key:
		var ready: Variant = _upload.batch_ready(board_key)
		if not ready is String or not ready.is_empty():
			return ready if ready is String else "O_01 board upload owner returned malformed readiness"
		var strip := Image.create(650,160,false,Image.FORMAT_RGBA8)
		strip.blit_rect(board_preview,Rect2i(0,0,640,160),Vector2i.ZERO)
		_board_vram_strip = strip
		_board_strip_batch_key = board_key
	var vram: PackedByteArray = _runtime.vram_words
	var known: PackedByteArray = _runtime.vram_known_mask
	var active_strip: Image = _board_vram_strip
	if stage2_transition:
		var b0: Variant = state.opening.get("names_media",{}).get("buffer_0")
		if not b0 is Dictionary or not b0.get("upload_batch_key") is String:
			return "O_01 board scroll transition lacks buffer_0 upload batch"
		var b0_ready: Variant = _upload.batch_ready(str(b0.upload_batch_key))
		if not b0_ready is String or not b0_ready.is_empty():
			return b0_ready if b0_ready is String else "O_01 names buffer_0 upload owner returned malformed readiness"
		active_strip = _board_vram_strip.duplicate()
		for y in 160:
			var row_base: int = (y * 1024 + 704) * 2
			for col in 266:
				var at: int = row_base + col * 2
				if known[at] != 255 or known[at + 1] != 255:
					return "O_01 board scroll transition references unknown VRAM"
				var color: int = vram.decode_u16(at)
				var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
				active_strip.set_pixel(384 + col, y, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
	else:
		for y in 160:
			var row_base: int = (y * 1024 + 960) * 2
			for col in 10:
				var at: int = row_base + col * 2
				var src_at: int = (y * 1024 + (320 + col * 64 + 63)) * 2
				if known[at] != 255 or known[at + 1] != 255 or vram[at] != vram[src_at] or vram[at + 1] != vram[src_at + 1]:
					return "O_01 board seam references unknown or mutated VRAM"
				var color: int = vram.decode_u16(at)
				var r: int = color & 31; var g: int = (color >> 5) & 31; var b: int = (color >> 10) & 31
				active_strip.set_pixel(640 + col, y, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
	var page_shift: int = scroll_x & 0x7f00
	var frac_x: int = scroll_x & 0xff
	var seam_base: int = (scroll_x / 256) * 4
	var quads: Array = [
		[-frac_x, 255, page_shift],
		[255 - frac_x, 1, 640 + seam_base + 3],
		[256 - frac_x, 255, 256 + page_shift],
		[511 - frac_x, 1, 640 + seam_base + 7],
		[512 - frac_x, 255, 512 + page_shift],
	]
	for quad: Array in quads:
		var q_dst_x: int = quad[0]
		var q_w: int = quad[1]
		var q_src_x: int = quad[2]
		var left_clip: int = maxi(0, -q_dst_x)
		var right_clip: int = mini(q_w, 320 - q_dst_x)
		var vis_w: int = right_clip - left_clip
		if vis_w <= 0:
			continue
		var src_x: int = q_src_x + left_clip
		var dst_x: int = q_dst_x + left_clip
		if src_x < 0 or src_x + vis_w > active_strip.get_width():
			return "O_01 board scroll quad references out-of-bounds strip"
		target.blit_rect(active_strip, Rect2i(src_x, 0, vis_w, 160), Vector2i(dst_x, 0))
	return ""

@warning_ignore("integer_division")
func paint_names_background(target: Image, state: State) -> String:
	if target == null or target.get_size() != Vector2i(320,240):
		return "O_01 names background target is unsupported"
	if state != null and state.mode_stage in [3, 4, 5, 6, 7, 8, 9] and state.opening.get("bg_layer_0") is Dictionary:
		var err35 := _stage35_error(state, true)
		if not err35.is_empty(): return err35
		var nm35: Variant = state.opening.get("names_media")
		if not nm35 is Dictionary or nm35.get("bg_show_1") not in [0, 1] or nm35.get("active_draw") != "dec_bg_draw":
			return "stage 3..5 names background lacks active dec_bg_draw state"
		var show35: int = int(nm35.bg_show_1)
		var buf_name35: String = "buffer_0" if show35 == 0 else "buffer_1"
		var buf35: Variant = nm35.get(buf_name35)
		if not buf35 is Dictionary or not buf35.get("upload_batch_key") is String:
			return "stage 3..5 names background lacks active buffer state"
		var b_key35: String = str(buf35.upload_batch_key)
		var b_ready35: Variant = _upload.batch_ready(b_key35)
		if not b_ready35 is String or not b_ready35.is_empty():
			return b_ready35 if b_ready35 is String else "stage 3..5 names buffer upload owner returned malformed readiness"
		var layer0: Dictionary = state.opening.bg_layer_0
		var cut_key: String = str(layer0.cut_batch_key)
		var clut_row: int = int(layer0.clut_row)
		var bg_bri: int = int(state.opening.get("bg_brightness", 128))
		var d_bri: int = int(state.opening.get("D_801220E0", 128))
		var primitive: Dictionary = state.opening.get("rei_banner_primitive",{})
		var white_back: bool = primitive.get("kind") == "white"
		var cache_key35 := "%s:%d:%s:%d:%d:%s:%s" % [b_key35, bg_bri, cut_key, clut_row, d_bri, white_back, layer0.enabled]
		if _names_bg_cache_image == null or _names_bg_cache_key != cache_key35:
			var vram35: PackedByteArray = _runtime.vram_words
			var known35: PackedByteArray = _runtime.vram_known_mask
			var img35 := Image.create(320, 160, false, Image.FORMAT_RGBA8)
			var base_x_words: int = 704 if show35 == 0 else 320
			var base_y_rows: int = 0 if show35 == 0 else 256
			for y in 160:
				var row_base: int = ((base_y_rows + y) * 1024 + base_x_words) * 2
				for x in 320:
					var at: int = row_base + x * 2
					if known35[at] != 255 or known35[at + 1] != 255:
						return "stage 3..5 names background references unknown VRAM"
					var color: int = vram35.decode_u16(at)
					var r: int = mini(31, ((color & 31) * bg_bri) >> 7)
					var g: int = mini(31, (((color >> 5) & 31) * bg_bri) >> 7)
					var b: int = mini(31, (((color >> 10) & 31) * bg_bri) >> 7)
					img35.set_pixel(x, y, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
			# White POLY_G4 is OT13, behind the cut at OT12.
			if white_back: img35.fill(Color8(255,255,255))
			for y in 160:
				if layer0.enabled == 0: continue
				var row_page: int = 0 if y < 128 else 1
				var py: int = y if y < 128 else y - 128
				for x in 320:
					var col_page: int = x / 128
					var px: int = x % 128
					var at_cut: int = ((row_page * 128 + py) * 1024 + (512 + col_page * 64)) * 2 + px
					if known35[at_cut] != 255:
						return "stage 3..5 cut layer references unknown VRAM"
					var idx_cut: int = vram35[at_cut]
					var clut_cut: int = (clut_row * 1024 + idx_cut) * 2
					if known35[clut_cut] != 255 or known35[clut_cut + 1] != 255:
						return "stage 3..5 cut CLUT references unknown VRAM"
					var c_cut: int = vram35.decode_u16(clut_cut)
					if c_cut == 0: continue
					var rc: int = mini(31, ((c_cut & 31) * d_bri) >> 7)
					var gc: int = mini(31, (((c_cut >> 5) & 31) * d_bri) >> 7)
					var bc: int = mini(31, (((c_cut >> 10) & 31) * d_bri) >> 7)
					if (c_cut & 0x8000) != 0:
						var back_c: Color = img35.get_pixel(x, y)
						rc = mini(31, ((back_c.r8 >> 3) + rc) >> 1)
						gc = mini(31, ((back_c.g8 >> 3) + gc) >> 1)
						bc = mini(31, ((back_c.b8 >> 3) + bc) >> 1)
					img35.set_pixel(x, y, Color8((rc << 3) | (rc >> 2), (gc << 3) | (gc >> 2), (bc << 3) | (bc >> 2), 255))
			_names_bg_cache_image = img35
			_names_bg_cache_key = cache_key35
		target.blit_rect(_names_bg_cache_image, Rect2i(0, 0, 320, 160), Vector2i.ZERO)
		_last_names_bg_target_id = target.get_instance_id()
		_last_names_bg_key = cache_key35
		return ""
	var error := _petals_error(state)
	if not error.is_empty(): return error
	error = _visible_error(state)
	if not error.is_empty(): return error
	var stage2_sub_ok: bool = state.mode_stage == 2 and state.mode_step == 0 and state.substep in [0, 1, 2, 3, 4, 5, 6, 7, 8]
	var stage2_to_3_ok: bool = state.mode_stage == 3 and state.mode_step == 0 and state.substep == 0 and not state.opening.has("bg_layer_0")
	if not stage2_sub_ok and not stage2_to_3_ok:
		return "O_01 names background is outside stage 2 substep 0..8"
	var names_media: Variant = state.opening.get("names_media")
	if not names_media is Dictionary or names_media.get("bg_show_1") not in [0, 1] or not names_media.get("buffer_0") is Dictionary:
		return "O_01 names background lacks buffer_0 state"
	var b0: Dictionary = names_media.buffer_0
	var b0_key: String = str(b0.get("upload_batch_key",""))
	var b0_ready: Variant = _upload.batch_ready(b0_key)
	if not b0_ready is String or not b0_ready.is_empty():
		return b0_ready if b0_ready is String else "O_01 names buffer_0 upload owner returned malformed readiness"
	var active_draw: String = str(names_media.get("active_draw",""))
	var vram: PackedByteArray = _runtime.vram_words
	var known: PackedByteArray = _runtime.vram_known_mask
	if active_draw == "dec_bg_draw":
		if not stage2_to_3_ok and state.substep not in [0, 1, 2, 5, 6, 7, 8]:
			return "O_01 dec_bg_draw is outside substep 0..2, 5..8"
		var show_buf: int = int(names_media.bg_show_1)
		var active_key: String = b0_key
		if show_buf == 1:
			if not names_media.get("buffer_1") is Dictionary:
				return "O_01 dec_bg_draw lacks buffer_1 state"
			active_key = str(names_media.buffer_1.get("upload_batch_key",""))
			var b1_r: Variant = _upload.batch_ready(active_key)
			if not b1_r is String or not b1_r.is_empty():
				return b1_r if b1_r is String else "O_01 names buffer_1 upload owner returned malformed readiness"
		var bg_bri2: int = int(state.opening.get("bg_brightness", 128))
		var cache_key := "%s:dec_bg_draw:%d:%d" % [active_key, show_buf, bg_bri2]
		if _names_bg_cache_image == null or _names_bg_cache_key != cache_key:
			var img := Image.create(320,160,false,Image.FORMAT_RGBA8)
			var bx: int = 704 if show_buf == 0 else 320
			var by: int = 0 if show_buf == 0 else 256
			for y in 160:
				var row_base: int = ((by + y) * 1024 + bx) * 2
				for x in 320:
					var at: int = row_base + x * 2
					if known[at] != 255 or known[at + 1] != 255:
						return "O_01 names buffer references unknown VRAM"
					var color: int = vram.decode_u16(at)
					var r: int = mini(31, ((color & 31) * bg_bri2) >> 7)
					var g: int = mini(31, (((color >> 5) & 31) * bg_bri2) >> 7)
					var b: int = mini(31, (((color >> 10) & 31) * bg_bri2) >> 7)
					img.set_pixel(x, y, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
			_names_bg_cache_image = img
			_names_bg_cache_key = cache_key
		target.blit_rect(_names_bg_cache_image, Rect2i(0, 0, 320, 160), Vector2i.ZERO)
		return ""
	if active_draw == "bg_crossfade_draw":
		if state.substep not in [2, 3, 4, 5] or not names_media.get("buffer_1") is Dictionary or not names_media.get("drawn_fade_amount") is int:
			return "O_01 bg_crossfade_draw lacks buffer_1 or drawn_fade_amount"
		var b1: Dictionary = names_media.buffer_1
		var b1_key: String = str(b1.get("upload_batch_key",""))
		var b1_ready: Variant = _upload.batch_ready(b1_key)
		if not b1_ready is String or not b1_ready.is_empty():
			return b1_ready if b1_ready is String else "O_01 names buffer_1 upload owner returned malformed readiness"
		var fade: int = int(names_media.drawn_fade_amount)
		if fade < 14 or fade > 128:
			return "O_01 bg_crossfade_draw fade_amount is outside 14..128"
		var inv_fade: int = (128 - fade) & 0xff
		var cache_key := b0_key + ":" + b1_key + ":" + str(fade)
		if _names_bg_cache_image == null or _names_bg_cache_key != cache_key:
			var img := Image.create(320,160,false,Image.FORMAT_RGBA8)
			for y in 160:
				var row0: int = (y * 1024 + 704) * 2
				var row1: int = ((256 + y) * 1024 + 320) * 2
				for x in 320:
					var at0: int = row0 + x * 2
					var at1: int = row1 + x * 2
					if known[at0] != 255 or known[at0 + 1] != 255 or known[at1] != 255 or known[at1 + 1] != 255:
						return "O_01 bg_crossfade_draw references unknown VRAM"
					var c0: int = vram.decode_u16(at0)
					var c1: int = vram.decode_u16(at1)
					var r0: int = mini(31, ((c0 & 31) * fade) >> 7)
					var g0: int = mini(31, (((c0 >> 5) & 31) * fade) >> 7)
					var b0_c: int = mini(31, (((c0 >> 10) & 31) * fade) >> 7)
					var r1: int = mini(31, ((c1 & 31) * inv_fade) >> 7)
					var g1: int = mini(31, (((c1 >> 5) & 31) * inv_fade) >> 7)
					var b1_c: int = mini(31, (((c1 >> 10) & 31) * inv_fade) >> 7)
					var r: int = mini(31, r0 + r1)
					var g: int = mini(31, g0 + g1)
					var b: int = mini(31, b0_c + b1_c)
					img.set_pixel(x, y, Color8((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255))
			_names_bg_cache_image = img
			_names_bg_cache_key = cache_key
		target.blit_rect(_names_bg_cache_image, Rect2i(0, 0, 320, 160), Vector2i.ZERO)
		return ""
	return "O_01 names background active_draw is unsupported"
