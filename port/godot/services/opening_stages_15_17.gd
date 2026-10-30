extends RefCounted
## Opening stages 15-17 (prologue.c:2139-2557, wiki/spec/opening-stages-15-17.md).
## Actor and background effects of prologue_lines_20_to_31_run,
## prologue_lines_32_to_36_run and prologue_line_37_run on the original
## O_16 / O_07.NBN bytes of the board pack. Each effect checks its stage,
## substep and actor owner and leaves the state unchanged when it refuses.
## actor_draw records the frames drawn this frame, their search_tpage slots
## and the page uploads a slot change needs; presentation/
## opening_stages_15_17_renderer.gd paints from that record.
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")

const TPAGE_EPOCH_MISSING := "stages 15-17 tpage uploads have no media epoch owner"
const ENTRY_BLINK_UNSUPPORTED := "Stage15 entry actor98 armed by the stage14 switch-frame blink is unsupported"
const O16_PROFILE := "o16_lines20"
const O07_PROFILE := "o07_lines32"
const RESOURCE_BASE := 0x80180000
# Resident table sets: frame table -> [tpage table, frame count, page count].
# O_16 (stage15) and O_07.NBN (BoardStore.O_06_ID) after the 15/34 memcpy.
const O16_TABLES := {0x80180258: [0x8018024c, 16, 3]}
const O07_TABLES := {0x801842c4: [0x801842bc, 11, 2], 0x801800f8: [0x801800f4, 7, 1]}
const O16_SEQUENCE_TABLE := 0x80180298
# Stage15 substeps of each effect (prologue.c:2235-2443).
const STAGE15_ALLOWED := {"slide_in":[0,1],"lip_sync":[1,7,11,15,19,23],"actor99_set":[4],"actor99_done":[5],
	"actor100_set":[10],"actor100_rise":[11],"pulse":[13,17,21],"slide_out":[26],"bg_reset":[34],"next_scene":[34]}
const STAGE16_ALLOWED := {"lip_sync":[1,5,9],"bg_reset":[11],"clear_scene":[11]}
const STAGE17_ALLOWED := {"bg_reset":[3]}
# Effects that follow prologue_line_run on the same frame: line_run may
# already have done substep++ (0x3D hold reached), so these are checked
# against the substep the body dispatched from (passed as value).
const AFTER_LINE_RUN := ["lip_sync","pulse"]
const LIP_SAMPLE_MISSING := "stage%d lip sync needs this frame's getCDlevel sample from the voiced line service"

var _o16 := PackedByteArray()
var _o07 := PackedByteArray()

func prepare(board: BoardStore) -> String:
	_o16 = PackedByteArray(); _o07 = PackedByteArray()
	if board == null: return "stage15-17 effects need the original board pack"
	var o16: Variant = board.stage_transfer(BoardStore.O_16_ID).get("bytes")
	var o07: Variant = board.stage_transfer(BoardStore.O_06_ID).get("bytes")
	if not o16 is PackedByteArray or o16.size() != 0xc800 or Store._sha(o16) != BoardStore.O_16_SHA:
		return "stage15 effects require original O_16 bytes"
	if not o07 is PackedByteArray or o07.size() != 0xc800 or Store._sha(o07) != BoardStore.O_06_SHA:
		return "stage15 effects require original O_07 bytes"
	_o16 = o16; _o07 = o07
	return ""

static func sequence_rows(bytes: PackedByteArray, sequence_table: int, slot: int) -> Array:
	# (frame, duration) rows of sequence_table[slot]; [] on any bad pointer.
	var entry := sequence_table-RESOURCE_BASE+slot*4
	if slot < 0 or entry < 0 or entry+4 > bytes.size(): return []
	var offset := int(bytes.decode_u32(entry))-RESOURCE_BASE
	if offset < 0 or offset+2 > bytes.size(): return []
	var count := bytes.decode_s16(offset)
	if count < 1 or offset+2+count*4 > bytes.size(): return []
	var rows: Array = []
	for step in count:
		rows.append([bytes.decode_s16(offset+2+step*4),bytes.decode_s16(offset+4+step*4)])
	return rows

static func tpage_slots(bytes: PackedByteArray, tables: Dictionary, drawn: Array) -> Dictionary:
	# search_tpage(0x7FFF) then actor_draw_all: drawn is [[frame_table, frame]]
	# in ascending actor index; each page takes the first free stage10 slot
	# unless this frame already holds it. {} when a frame does not resolve.
	var slots: Dictionary = {}
	for item: Array in drawn:
		var table: Variant = tables.get(item[0])
		if not table is Array: return {}
		var pages := BoardStore.actor_frame_pages(bytes,int(item[0])-RESOURCE_BASE,int(table[0])-RESOURCE_BASE,int(table[1]),int(table[2]),int(item[1]))
		if pages.is_empty(): return {}
		for page: int in pages:
			if slots.has(page): continue
			if slots.size() >= BoardStore.STAGE10_FREE_TPAGE_SLOTS.size(): return {}
			slots[page] = BoardStore.STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

static func stage15_entry_owner(opening: Dictionary) -> bool:
	# Actor96 as stage14 case6 leaves it (prologue.c:2100-2121) or as stage15 keeps it.
	var a: Variant = opening.get("actor96")
	return a is Dictionary and a.get("resource_id") == BoardStore.O_16_ID and a.get("unk_01") == 0xb and a.get("flags") == 0x40 and a.get("draw_flags") == 0xa4 \
		and a.get("frame_table") == 0x80180258 and a.get("sequence_table") == 0x80180298 and a.get("tpage_table") == 0x8018024c and a.get("frame_index") == 0 and a.get("x") is int

static func stage16_owner(opening: Dictionary) -> bool:
	var a: Variant = opening.get("actor96")
	return a is Dictionary and a.get("profile") == O07_PROFILE and a.get("resource_id") == BoardStore.O_06_ID

static func stage17_owner(opening: Dictionary) -> bool:
	for index in range(96,160):
		if opening.has("actor%d" % index): return false
	return opening.get("bg_layer_0",{}).get("attribute") == 0x80000000

func effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	if _o16.is_empty() or state.game_mode != 0x15 or state.mode_step != 0:
		result.error = "stage15-17 effects are outside their prepared source scope"; return null
	var before: Dictionary = state.opening.duplicate(true)
	var outcome: Variant = null
	match state.mode_stage:
		15: outcome = _stage15(state,kind,value,result)
		16: outcome = _stage16(state,kind,value,result)
		17: outcome = _stage17(state,kind,value,result)
		18: _stage18_entry_draw(state,kind,result)
		_: result.error = "stage15-17 effect is outside stages 15..17"
	if not result.error.is_empty():
		state.opening = before
		return null
	return outcome

func _allowed(state: State, allowed: Dictionary, kind: String, value: int, result: Session.FrameResult) -> bool:
	if kind == "actor_draw": return true
	var dispatched: int = state.substep
	if kind in AFTER_LINE_RUN:
		# Same substep, or the one run_line's completion just entered (it
		# zeroes mode_step_timer when it does substep++).
		if state.substep != value and (state.substep != value+1 or state.mode_step_timer != 0):
			result.error = "stage%d effect is outside its source substep" % state.mode_stage
			return false
		dispatched = value
	if not allowed.has(kind) or dispatched not in allowed[kind]:
		result.error = "stage%d effect is outside its source substep" % state.mode_stage
		return false
	return true

static func _line_run_lip_sample(state: State, variant: String, result: Session.FrameResult) -> Dictionary:
	# prologue_line_services.line_run already sampled getCDlevel into the
	# 4-entry ring for this frame (opening_audio_rules step_lip_sync -> the
	# variant's lip_sync_*). The source samples once per frame, so the effect reads
	# that sample and never steps the ring again. sampled_clip_frame exists
	# only after a step of the current line's ring (begin_line_lip_sync
	# rebuilds the record without it).
	var lip: Variant = state.opening.get("prologue_lip_sync")
	if not lip is Dictionary or lip.get("variant") != variant or lip.get("line_id") != state.opening.get("prologue_line") \
			or not lip.has("sampled_clip_frame") or not lip.get("cd_active") is bool or not lip.get("ring_frame") is int:
		result.error = LIP_SAMPLE_MISSING % state.mode_stage
		return {}
	return lip

func _stage15(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	var o: Dictionary = state.opening
	if not stage15_entry_owner(o):
		result.error = "stage15 effect requires its original O_16 actor96 owner"; return null
	var a98: Variant = o.get("actor98")
	if a98 is Dictionary and a98.get("profile") not in [O16_PROFILE + "_lip","cleared"]:
		result.error = ENTRY_BLINK_UNSUPPORTED; return null
	if not _allowed(state,STAGE15_ALLOWED,kind,value,result): return null
	match kind:
		"slide_in":
			var x: int = o.actor96.x
			if x < -0xa0:
				o.actor96.x = x+1
				_actor_or_cleared(o,97)["x"] = x+1
		"lip_sync":
			# prologue_lip_synchronization_lines_20_to_31 (2139-2190).
			var lip := _line_run_lip_sample(state,"lines_20_to_31",result)
			if lip.is_empty(): return null
			_clear(o,97)
			if lip.get("cd_active") == true:
				var frame: int = int(lip.ring_frame)
				for index in [97,98]:
					o["actor%d" % index] = _o16_actor(index,9,0x40,frame+(6 if index == 97 else 10),int(o.actor96.x),-0x78)
					o["actor%d" % index]["profile"] = O16_PROFILE + "_lip"
					o["actor%d_draw_flags" % index] = 0xa4
				if o.get("actor100") is Dictionary: o.actor100["flags"] = 0
		"actor99_set":
			# prologue_actor99_set_lines_20_to_31: sequence_slot and unk_18 keep actor_clear's 0.
			var a99 := _actor_or_cleared(o,99)
			var step := int(a99.get("seq_step",0)); var counter := int(a99.get("sequence_counter",0)); var slot := int(a99.get("sequence_slot",0))
			o["actor99"] = _o16_actor(99,9,5,0,-0xa0,-0x78)
			o.actor99.erase("frame_index")
			o.actor99.merge({"sequence_slot":slot,"seq_step":step,"sequence_counter":counter},true)
			o["actor99_draw_flags"] = 0xa4
		"actor99_done":
			var a: Variant = o.get("actor99")
			if not a is Dictionary or a.get("profile") != O16_PROFILE:
				result.error = "stage15 actor99 wait has no source actor99"; return null
			return (int(a.flags) & 1) == 0
		"actor100_set":
			var a100 := _actor_or_cleared(o,100)
			var step := int(a100.get("seq_step",0)); var counter := int(a100.get("sequence_counter",0))
			o["actor100"] = _o16_actor(100,0xa,0,0,-0xa0,-0x38)
			o.actor100.erase("frame_index")
			o.actor100.merge({"sequence_slot":3,"seq_step":step,"sequence_counter":counter},true)
			o["actor100_draw_flags"] = 0xa4
			o["D_800B5BD4"] = 9
		"actor100_rise":
			var a: Variant = o.get("actor100")
			if not a is Dictionary or a.get("profile") != O16_PROFILE:
				result.error = "stage15 actor100 rise has no source actor100"; return null
			if int(a.y) >= -0x77: a.y = int(a.y)-1
		"pulse":
			var a: Variant = o.get("actor100")
			if not a is Dictionary or a.get("profile") != O16_PROFILE:
				result.error = "stage15 pulse has no source actor100"; return null
			if (int(a.flags) & 1) == 0: a.flags = 1
			o["D_800B5BD4"] = 0xf
		"slide_out":
			var x: int = o.actor96.x
			if x < -0x18f: return false
			x = ((x-2+0x8000) & 0xffff)-0x8000
			for index in [101,100,99,98,97,96]:
				_actor_or_cleared(o,index)["x"] = x
			return true
		"bg_reset":
			_bg_reset(o,result)
		"next_scene":
			_stage15_next_scene(state,result)
		"actor_draw":
			_actor_draw(state,_o16,O16_TABLES,"o16_tpages",BoardStore.O_16_ID,result)
	return null

func _stage15_next_scene(state: State, result: Session.FrameResult) -> void:
	# Case34 success after dec_bg_show_set(1, decode): actor_clear_upper,
	# prologue_background_slot_set(0x801A4000,0xF), memcpy(0x80180000,
	# 0x801E8000,0xC800), actor96/99 scene_06 setups (829-841, 919-930).
	var o: Dictionary = state.opening
	var media: Variant = o.get("media")
	if not media is Dictionary:
		result.error = "stage15 next scene has no media owner"; return
	var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_06_ID,BoardStore.CUT_06_SHA,{"lba":0xaa54,"sectors":0x31,"destination":0x801a4000})
	var actor_id := _ready_read(media,0x801e8000,BoardStore.O_06_ID,BoardStore.O_06_SHA,{"lba":0xabca,"sectors":0x19,"destination":0x801e8000})
	if cut_id.is_empty() or actor_id.is_empty():
		result.error = "stage15 next scene lacks completed original CUT_07/O_07 reads"; return
	var names_media: Variant = o.get("names_media")
	if not names_media is Dictionary or names_media.get("bg_show_1") not in [0,1] or names_media.get("buffer_%d" % int(names_media.bg_show_1),{}).get("lba") != 0xacd4:
		result.error = "stage15 next scene requires the decoded P_01_04 background"; return
	for index in range(96,160):
		_clear(o,index)
	o.erase("o16_tpages")
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_06_ID,"stage15"]).sha256_text()
	o["cut_media"] = {"resource_id":BoardStore.CUT_06_ID,"sha256":BoardStore.CUT_06_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":false}
	o["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"attribute":0x41000000,"resource_id":BoardStore.CUT_06_ID,"cut_batch_key":cut_key}
	# prim_bg_slot_setup writes bg_layers[0].unk_10/11/12 (D_801220E0..E2) = 0x80.
	for channel: String in ["D_801220E0","D_801220E1","D_801220E2"]: o[channel] = 128
	o["o07_copy"] = {"resource_id":BoardStore.O_06_ID,"sha256":BoardStore.O_06_SHA,"request_id":actor_id,"source":0x801e8000,"destination":0x80180000,"bytes":0xc800}
	# The slot set uploads the cut pages and CLUT rows 495/494; the memcpy
	# moves the published O_07 bytes from 0x801E8000 to 0x80180000.
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_06_ID,"sha256":BoardStore.CUT_06_SHA})
	result.presentation.append({"kind":"first_arrival_e8_copy","resource_id":BoardStore.O_06_ID,"sha256":BoardStore.O_06_SHA,"bytes":0xc800})
	o["actor96"] = _actor(96,0xa,0x40,0,-0xa0,-0x78,0x801842c4,0x801842f0,0x801842bc,BoardStore.O_06_ID)
	o["actor99"] = _actor(99,9,0,0,-0xa0,-0x78,0x801800f8,0x80180114,0x801800f4,BoardStore.O_06_ID)
	o.actor99.erase("frame_index")
	o.actor99.merge({"sequence_slot":0,"seq_step":0,"sequence_counter":0},true)
	o["actor96_draw_flags"] = 0xa4
	o["actor99_draw_flags"] = 0xa4

func _stage16(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	var o: Dictionary = state.opening
	if not stage16_owner(o):
		result.error = "stage16 effect requires its original O_07 actor96 owner"; return null
	if not _allowed(state,STAGE16_ALLOWED,kind,value,result): return null
	match kind:
		"lip_sync":
			# prologue_lip_synchronization_yoshio (843-879).
			var lip := _line_run_lip_sample(state,"yoshio",result)
			if lip.is_empty(): return null
			_clear(o,97)
			if lip.get("cd_active") == true:
				o["actor97"] = _actor(97,9,0x40,6-int(lip.ring_frame),-0xa0,-0x78,0x801842c4,0x801842f0,0x801842bc,BoardStore.O_06_ID)
				o["actor97_draw_flags"] = 0xa4
		"bg_reset":
			_bg_reset(o,result)
		"clear_scene":
			# Case11 success: actor_clear_upper; bg_layers = 0x80000000. Layer
			# order 0xD stays, but func_80098D00 returns at once for a negative
			# unk_00 (asm/libspu.s 80098D3C bltz), so layer 0 draws nothing.
			var names_media: Variant = o.get("names_media")
			if not names_media is Dictionary or names_media.get("buffer_%d" % int(names_media.get("bg_show_1",-1)),{}).get("lba") != 0x496a:
				result.error = "stage16 clear requires the decoded P_01_10 background"; return null
			for index in range(96,160):
				_clear(o,index)
			o.erase("o07_tpages")
			var layer: Dictionary = o.get("bg_layer_0",{}).duplicate(true)
			layer["attribute"] = 0x80000000
			layer["enabled"] = 0
			o["bg_layer_0"] = layer
		"actor_draw":
			_actor_draw(state,_o07,O07_TABLES,"o07_tpages",BoardStore.O_06_ID,result)
	return null

func _stage17(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	var o: Dictionary = state.opening
	if not stage17_owner(o):
		result.error = "stage17 effect requires the cleared stage16 scene"; return null
	if not _allowed(state,STAGE17_ALLOWED,kind,value,result): return null
	if kind == "bg_reset": _bg_reset(o,result)
	return null

func _stage18_entry_draw(state: State, kind: String, result: Session.FrameResult) -> void:
	# prologue_line_37_run case3 success: mode_stage_next(), then dec_bg_draw,
	# the HUD and draw_frame's actor_draw_all on the same frame (2545-2556).
	# Stage16 case11 cleared every upper actor, so actor_draw_all draws none.
	if kind != "actor_draw" or state.substep != 0 or not stage17_owner(state.opening):
		result.error = "stage15-17 effect is outside stages 15..17"

static func _bg_reset(o: Dictionary, result: Session.FrameResult) -> void:
	# dec_bg_reset (background_images.c:36-48): show flags 0, brightness 0x80.
	# The failed decode's dec_bg_show_set is overwritten by this reset.
	var names_media: Variant = o.get("names_media")
	if not names_media is Dictionary:
		result.error = "background reset has no slot1 owner"; return
	names_media["bg_show_1"] = 0
	o["bg_brightness"] = 128

func _actor_draw(state: State, bytes: PackedByteArray, tables: Dictionary, key: String, resource_id: String, result: Session.FrameResult) -> void:
	# actor_draw_all (actor_sprites.c:87-367): ascending actor index; draw
	# the current frame, then step a sequence actor whose flags have bit0.
	# search_tpage reloads every frame; a slot whose resident page changes
	# gets a load_csr_tp upload of that original page.
	var o: Dictionary = state.opening
	var drawn: Array = []
	var records: Array = []
	var steps: Array = []
	for index in range(96,102):
		var a: Variant = o.get("actor%d" % index)
		if not a is Dictionary or a.get("profile") == "cleared" or (int(a.get("draw_flags",0)) & 0x80) == 0: continue
		if not tables.has(a.get("frame_table")) or tables[a.frame_table][0] != a.get("tpage_table"):
			result.error = "actor%d tables are outside the resident resource" % index; return
		var frame: int
		if (int(a.flags) & 0x40) != 0:
			frame = int(a.frame_index)
		else:
			var rows := sequence_rows(bytes,int(a.sequence_table),int(a.sequence_slot))
			var step := int(a.seq_step)
			if rows.is_empty() or step < 0 or step >= rows.size():
				result.error = "actor%d sequence cursor is outside its source rows" % index; return
			frame = int(rows[step][0])
			if (int(a.flags) & 1) == 1: steps.append([a,rows])
		drawn.append([a.frame_table,frame])
		records.append({"index":index,"frame_table":a.frame_table,"frame":frame,"x":int(a.x),"y":int(a.y),"unk_01":int(a.get("unk_01",0))})
	var slots := tpage_slots(bytes,tables,drawn)
	if slots.is_empty() and not drawn.is_empty():
		result.error = "stage%d frames exceed the source free tpage slots" % state.mode_stage; return
	if drawn.is_empty():
		o.erase(key)
	else:
		var media: Variant = o.get("media")
		if not media is Dictionary or not media.get("epoch") is int:
			result.error = TPAGE_EPOCH_MISSING; return
		var resident: Dictionary = o.get(key,{}).get("resident",{}).duplicate(true)
		var mapped: Dictionary = {}
		var uploads: Array = []
		for page: int in slots:
			var slot: int = slots[page]
			mapped[str(page)] = slot
			var held: Variant = resident.get(str(slot))
			if held is Dictionary and held.get("offset") == page and held.get("resource_id") == resource_id: continue
			var upload_key := JSON.stringify([media.epoch,resource_id,slot,page,state.frame_count]).sha256_text()
			resident[str(slot)] = {"offset":page,"resource_id":resource_id,"key":upload_key}
			uploads.append({"kind":"first_arrival_stage_actor_upload","batch_key":upload_key,"resource_id":resource_id,"slot":slot,"source_offset":page})
		# game_mode_dispatch increments frame_count after the handler, so the
		# draw, the CD/upload preflight and the published paint of this
		# frame all see frame_count + 1: the record is owned by that value.
		o[key] = {"slots":mapped,"resident":resident,"frame_count":(state.frame_count+1) & 0xffffffff,"drawn":records}
		result.presentation.append_array(uploads)
	for item: Array in steps:
		_sequence_step(item[0],item[1],state.frame_count,result)
		if not result.error.is_empty(): return

static func _sequence_step(a: Dictionary, rows: Array, frame_count: int, result: Session.FrameResult) -> void:
	# actor_sprites.c:312-367 for the flag sets stage15 uses (1 loop, 5 hold).
	var flags := int(a.flags)
	if flags & 0x3a:
		result.error = "actor sequence flags outside stage15 source states"; return
	var speed := int(a.get("speed",1))
	var counter := int(a.sequence_counter)
	var step := int(a.seq_step)
	if (speed & 0x80) != 0 and (speed & 0x7f) != 0: counter += speed & 0x7f
	elif speed != 0 and frame_count % speed == 0: counter += 1
	if counter >= int(rows[step][1]):
		step += 1
		counter = 0
	if step >= rows.size():
		if (flags & 4) == 4:
			flags &= 0xfffa
			step -= 1
		else: step = 0
	a.flags = flags; a.seq_step = step; a.sequence_counter = counter

static func _ready_read(media: Dictionary, destination: int, resource_id: String, sha256: String, payload: Dictionary) -> String:
	var request_id := str(media.get("actor_pending",{}).get(str(destination),""))
	var request: Variant = media.get("requests",{}).get(request_id)
	if not request is Dictionary or request.get("status") != "ready" or request.get("kind") != "actors" or request.get("resource_id") != resource_id or request.get("resource_sha256") != sha256:
		return ""
	if request.get("epoch") != media.get("epoch") or not Store._same(request.get("payload"),payload): return ""
	return request_id

static func _actor(index: int, unk_01: int, flags: int, frame: int, x: int, y: int, frame_table: int, sequence_table: int, tpage_table: int, resource_id: String) -> Dictionary:
	# Source fields of an actor setup; others keep actor_clear's values
	# (unk_06 speed 1, unk_08 counter 0).
	return {"profile":O16_PROFILE if resource_id == BoardStore.O_16_ID else O07_PROFILE,"index":index,"unk_01":unk_01,"flags":flags,
		"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":frame_table,"sequence_table":sequence_table,"tpage_table":tpage_table,
		"frame_index":frame,"sequence_counter":0,"speed":1,"brightness":0x80,"x":x,"y":y,"resource_id":resource_id}

static func _o16_actor(index: int, unk_01: int, flags: int, frame: int, x: int, y: int) -> Dictionary:
	return _actor(index,unk_01,flags,frame,x,y,0x80180258,O16_SEQUENCE_TABLE,0x8018024c,BoardStore.O_16_ID)

static func _clear(o: Dictionary, index: int) -> void:
	o.erase("actor%d" % index)
	o.erase("actor%d_source_draw" % index)
	o.erase("actor%d_draw_flags" % index)

static func _actor_or_cleared(o: Dictionary, index: int) -> Dictionary:
	# A store into a cleared record (actor_clear: draw_flags 0, x 0).
	if not o.get("actor%d" % index) is Dictionary:
		o["actor%d" % index] = {"profile":"cleared","index":index,"draw_flags":0,"flags":0,"x":0}
	return o["actor%d" % index]
