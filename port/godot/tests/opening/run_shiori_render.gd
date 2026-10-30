extends SceneTree
## Stages 12-14 rendering (wiki/spec/opening-stages-12-14.md) from the real
## combined board pack: the stage bodies run through the real queue, media
## bridge and ShioriActors; every upload intent they emit runs through the
## native GPU pump into owned VRAM; ShioriScene draws from that VRAM. Each
## drawn frame is compared, pixel by pixel, with a reference composed
## directly from the disc bytes (AD0B/ACEC decode words, CUT_14/CUT_16 pages
## and CLUT, O_14/O_15/O_16 frames, O_01 actor palettes), never from VRAM.
## The O_01 actor CLUT rows 0x1F0-0x1FF are loaded in stage 0
## (prologue.c:259); the fixture loads them the same way from the installed
## first-arrival media pack (FIRST_ARRIVAL_MEDIA_ROOT).
const Flow = preload("res://flow/prologue_arrival.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Actors = preload("res://services/shiori_actor_model.gd")
const Scene = preload("res://presentation/shiori_scene_renderer.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
var failures := 0
var checks := 0
var completed: Array = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

class UploadOwner extends RefCounted:
	# batch_ready over the pump's own receipts, as the upload bridge answers.
	var pump: Pump
	var owned: Dictionary
	func batch_ready(key: String) -> String:
		if pump._cut_receipts.has(key): return pump.cut_batch_ready(key,owned)
		if pump._names_bg_receipts.has(key): return pump.names_bg_batch_ready(key,owned)
		if pump._stage_actor_receipts.has(key): return pump.stage_actor_batch_ready(key,owned)
		return "no executed receipt for " + key

class SceneAdapter extends RefCounted:
	# The visible_renderer arrival_draw receives (first_arrival_actor_renderer).
	var scene: Scene
	func preflight_shiori(state) -> String: return scene.scene_error(state,true)

var board := Board.new()
var media := Media.new()
var scene := Scene.new()
var adapter := SceneAdapter.new()
var rng := Rng.new()
var owner := UploadOwner.new()
var bytes: Dictionary = {}
var o01 := PackedByteArray()
var text_frame := 0
var draws: Array = []
var order_overlap := {"actors":0}
var uploads_seen: Array = []
var ad05_reads := 0

func _fixture() -> String:
	var error := board.open_pack(OS.get_environment("BOARD_PACK"),Board.MANIFEST_SHA)
	if not error.is_empty(): return error
	var root := OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT")
	var arrival := Store.new()
	error = arrival.open(root,FileAccess.get_sha256(root.path_join("manifest.json")))
	if not error.is_empty(): return "first-arrival media pack: " + error
	var transfer := arrival.transfer(Store.ACTOR_ID)
	if not transfer.get("bytes") is PackedByteArray or Store._sha(transfer.bytes) != Store.ACTOR_SHA: return "O_01 transfer differs"
	o01 = transfer.bytes
	bytes = Media.shiori_area_bytes(board)
	media._board_store = board
	media._shiori_bytes = bytes
	error = media.queue.prepare(board.background_descriptions(),board.actor_descriptions(),{})
	if not error.is_empty(): return error
	error = scene.prepare(board)
	if not error.is_empty(): return error
	adapter.scene = scene
	var vram := PackedByteArray(); vram.resize(Pump.VRAM_BYTES)
	var mask := PackedByteArray(); mask.resize(Pump.VRAM_BYTES)
	var scratch := PackedByteArray(); scratch.resize(512)
	# load_palette(0x80184490, 16+i, 1, 2, i) for i in 0..15 (prologue.c:259).
	for row in 16:
		var clut := Pump._mode2_stp(o01.slice(0x4490+row*512,0x4490+row*512+512))
		var at := ((0x1f0+row)*1024)*2
		for i in 512:
			vram[at+i] = clut[i]; mask[at+i] = 255
	owner.pump = Pump.new()
	owner.owned = {"vram_owner":"shiori-render-test","scratch_owner":"shiori-render-scratch","vram_words":vram,"vram_known_mask":mask,"scratch_words":scratch}
	return ""

static func seed_hitting(count: int) -> int:
	var probe := Rng.new()
	for seed_word in range(1,1000000):
		probe.configure(Rng.OPENBIOS_PROFILE,seed_word)
		var hits := []
		for i in count: hits.append((probe.next("probe") & 0xff) == 0xff)
		if hits[-1] and not hits.slice(0,count-1).has(true): return seed_word
	return -1

# ------------------------------------------------------------------ route
func services() -> Dictionary:
	var out := media.queue.callbacks()
	out["background_decode"] = media.background_decode
	out["shiori_effect"] = media.shiori_effect
	out["line_start"] = func(s,line,_out):
		s.opening["prologue_text_work"] = {"line_id":line,"next_frame":(s.frame_count+1) & 0xffffffff}
		s.opening["audio"] = {"status":"requested","clip_index":0,"line_id":line}
		s.opening["prologue_lip_sync"] = {"clip_levels":[[40,40,40,40,40,40,40,40,40,40,40,40]],"clip_index":0,"clip_frame":0}
	out["line_run"] = func(s,_out):
		text_frame += 1
		if text_frame >= 6: s.substep += 1; text_frame = 0
	# The production dec_bg_draw, recorded per frame; the route goes on
	# when it refuses so the later stages stay reachable for the checks.
	out["arrival_draw"] = func(s,slot,_out):
		var r := Session.FrameResult.new()
		media.arrival_draw(s,slot,r,adapter)
		draws.append([s.mode_stage,s.substep,s.frame_count,r.error,r.presentation.duplicate(true)])
	out["outer_draw"] = func(_s,_arg,_out): pass
	out["cd_audio_state"] = AudioRules.check_cd
	return out

func complete_reads(s: State, retry_resource: String = "") -> void:
	for request: Dictionary in s.opening.media.requests.values():
		if request.status != "pending": continue
		var status := "retry" if request.resource_id == retry_resource and request.kind == "background" else "ready"
		var out := Session.FrameResult.new()
		var event := Session.ServiceEvent.new(s.frame_count,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,Queue.SERVICE,
			{"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,"status":status,"resource_id":request.resource_id})
		media.queue.complete(event,s,null,out)
		check(out.error.is_empty(),"Completion of %s refused: %s" % [request.resource_id,out.error])

func run_uploads(s: State, out: Session.FrameResult) -> void:
	# The upload bridge's per-intent work, in presentation order.
	for intent: Dictionary in out.presentation:
		if not intent.get("kind") in ["first_arrival_names_bg_upload","first_arrival_cut_upload","first_arrival_stage_actor_upload","first_arrival_shiori_copy","opening_background_select"]: continue
		check(media._intent_schema(intent).is_empty(),"Schema refused %s: %s" % [str(intent),media._intent_schema(intent)])
		var done: Dictionary = {"error":""}
		match intent.kind:
			"first_arrival_names_bg_upload": done = owner.pump.execute_names_bg(board,intent.resource_id,intent.batch_key,owner.owned,intent.buffer)
			"first_arrival_cut_upload": done = owner.pump.execute_cut_vram(board,intent.resource_id,intent.batch_key,owner.owned,false)
			"first_arrival_stage_actor_upload":
				check(Upload._shiori_owner(s,intent.resource_id,intent.slot,intent.source_offset,intent.batch_key),"Upload owner refused this frame's page " + str(intent))
				done = owner.pump.execute_stage_actor_vram(board,intent.resource_id,intent.slot,intent.source_offset,intent.batch_key,owner.owned)
		check(done.error.is_empty(),"Pump refused %s: %s" % [str(intent),done.error])
		uploads_seen.append([s.mode_stage,s.substep,intent.kind,intent.get("resource_id"),intent.get("slot",intent.get("buffer",-1)),intent.get("source_offset",intent.get("bytes",-1))])

func tick(s: State, svc: Dictionary, retry_resource: String = "") -> Session.FrameResult:
	var out := Session.FrameResult.new()
	match s.mode_stage:
		12: Flow._stage_12_shiori_load(s,svc,out)
		13: Flow._stage_13_shiori_line_18(s,rng,svc,out)
		14: Flow._stage_14_shiori_line_19(s,rng,svc,out)
	# game_mode_dispatch increments frame_count after the mode handler; the
	# upload owner, logical draw and paint see the incremented value.
	s.frame_count += 1
	if out.error.is_empty():
		run_uploads(s,out)
		complete_reads(s,retry_resource)
	return out

func stage11_ad05_decode(s: State) -> void:
	# prologue_line_17_run case6 (prologue.c:1797) through the production
	# decode: stage10 showed ACF6 in buffer 1, so P_01_08 lands in buffer 0,
	# and its upload runs through the pump into the owned VRAM.
	s.mode_stage = 11; s.substep = 6
	s.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0},"buffer_1":{"lba":0xacf6},"active_draw":"dec_bg_draw"}
	ad05_reads += 1
	var request_id := "ad05-%d" % ad05_reads
	s.opening.media.requests[request_id] = {"request_id":request_id,"epoch":5,"status":"ready","kind":"background","resource_id":Board.BG_AD05_ID}
	s.opening.media.background_pending["1"] = request_id
	var out := Session.FrameResult.new()
	check(media.background_decode(s,1,out) == 0 and out.error.is_empty(),"Stage11 AD05 decode refused: " + out.error)
	check(out.presentation.map(func(i): return [i.kind,i.resource_id,i.buffer]) == [["opening_background_select",Board.BG_AD05_ID,0],["first_arrival_names_bg_upload",Board.BG_AD05_ID,0]],"Stage11 AD05 decode intents differ: " + str(out.presentation))
	run_uploads(s,out)
	check(owner.pump.names_bg_batch_ready(s.opening.names_media.buffer_0.upload_batch_key,owner.owned).is_empty(),"AD05 buffer-0 upload has no receipt")
	s.opening.media.requests.erase(request_id)
	s.opening.media.background_pending.erase("1")
	s.opening.media.decoded = {}
	s.mode_stage = 12; s.substep = 0

func stage12_entry(bg_brightness: int = 128) -> State:
	# prologue_line_17_run case6 (prologue.c:1797-1818) over the real O_13,
	# as in run_stages_12_14; stage10/11 left bg_brightness and the colours
	# at 0x80 and full screen brightness. The route and the reset checks pass
	# a lower bg_brightness, which the P_01_08 frames before the reset draw
	# at: it makes dec_bg_reset's 0x80 write visible in the 13/0 pixels.
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 12; s.mode_step = 0; s.substep = 0; s.frame_count = 1000
	s.opening["media"] = {"epoch":5,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	stage11_ad05_decode(s)
	s.opening["cut_media"] = {"resource_id":Board.CUT_12_ID}
	s.opening["bg_layer_0"] = {"enabled":0,"attribute":0x80000000,"resource_id":Board.CUT_12_ID,"clut_row":495}
	s.opening["actor_area"] = [{"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":0x20800}]
	s.opening["bg_brightness"] = bg_brightness; s.opening["screen_brightness"] = 128
	for component in ["D_801220E0","D_801220E1","D_801220E2"]: s.opening[component] = 128
	var a96 := Actors.cleared(96)
	a96.merge({"profile":"o13_shiori_line17","unk_01":0xa,"flags":5,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180204,
		"sequence_table":0x80180220,"tpage_table":0x801801e4,"sequence_slot":0,"brightness":0x80,"x":-0xa0,"y":-0x78},true)
	s.opening["actor96"] = a96; s.opening["actor96_draw_flags"] = 0xa4
	return s

# -------------------------------------------------------------- reference
static func _stp(c: int, index: int) -> int:
	# palette_load_vram mode 2 (frame_render_audio.c): colours 0..239 lose
	# STP, colours 240..255 gain it when not black.
	if index < 240: return c & 0x7fff
	return c | 0x8000 if (c & 0x7fff) != 0 else c

static func _tinted(c: int, tint: Array) -> int:
	# Texture modulation: each 5-bit channel (c*tint)>>7, saturated at 31.
	var out := 0
	for k in 3: out |= mini(31,(((c >> (k*5)) & 31)*int(tint[k])) >> 7) << (k*5)
	return out

static func _blend(back: int, c: int, tint: Array) -> int:
	# c is the raw texel colour (STP in bit 15). Semi-transparency mode 0,
	# (B+F)/2, applies to the tinted colour when STP is set.
	var front := _tinted(c,tint)
	if (c & 0x8000) == 0: return front
	var out := 0
	for shift in [0,5,10]: out |= ((((back >> shift) & 31)+((front >> shift) & 31)) >> 1) << shift
	return out

# The values these stages leave in place: dec_bg_reset writes bg_brightness
# 0x80 (background_images.c:44), prim_bg_slot_setup writes D_801220E0..E2
# 0x80 (gpu_effects.c:67-69), and the screen brightness stays full.
const SOURCE_TINTS := {"bg":0x80,"cut":[0x80,0x80,0x80],"col":0x80}

func ot_order(s: State, src: Dictionary, reverse_actors := false, reverse_parts := false) -> Array:
	# The GPU draw order, modelled from the source loops rather than copied
	# from the renderer's sort. dec_bg_draw(1) adds the background at OT13,
	# and the CUT layer sits at OT12. actor_draw_all (actor_sprites.c:87,
	# :121) visits actors i ascending and parts j = cnt-1 down to 0, with j
	# capped to 3 under draw_flags 8. Each part is GsSortSprite'd into OT
	# unk_01-1 (0 when unk_01 is 0). AddPrim inserts at the head of its OT
	# entry, and the ClearOTagR list draws from the highest entry down to 0,
	# each entry from its head. The reverse flags flip one source loop, so a
	# test can show that its frame tells the two orders apart.
	var ot: Dictionary = {13:[["bg"]],12:[["cut"]]}
	var records: Array = s.opening.shiori_draw.actors
	var indices: Array = records.map(func(r): return int(r.index))
	indices.sort()
	if reverse_actors: indices.reverse()
	for index: int in indices:
		var record: Dictionary = records.filter(func(r): return int(r.index) == index)[0]
		var resource: PackedByteArray = src[_newest(s,record.frame_table)].bytes
		var anim: int = resource.decode_u32(record.frame_table - 0x80180000 + record.frame*4) - 0x80180000
		var js: Array = []
		var j: int = resource.decode_s16(anim) - 1
		while j >= 0:
			if (int(record.draw_flags) & 8) != 0 and j >= 4: j = 3
			js.append(j)
			j -= 1
		if reverse_parts: js.reverse()
		var slot: int = (int(record.unk_01)-1) & 0xffff if int(record.unk_01) != 0 else 0
		for part: int in js:
			if not ot.has(slot): ot[slot] = []
			ot[slot].push_front([record,part])
	var keys: Array = ot.keys()
	keys.sort()
	keys.reverse()
	var out: Array = []
	for key: int in keys: out.append_array(ot[key])
	return out

func reference(s: State, src: Dictionary = bytes, tints: Dictionary = SOURCE_TINTS, actors_only := false, reverse_actors := false, reverse_parts := false) -> PackedInt32Array:
	# The frame from disc bytes, never VRAM, drawn in ot_order: the decode
	# words at bg_brightness, the CUT cells tinted D_801220E0..E2, then the
	# drawn actor parts tinted by the screen brightness (draw_flags 0x20).
	var image := PackedInt32Array(); image.resize(320*160)
	var bg_tint := [tints.bg,tints.bg,tints.bg]
	var col := [tints.col,tints.col,tints.col]
	for entry: Array in ot_order(s,src,reverse_actors,reverse_parts):
		if entry[0] is String:
			if actors_only: continue
			if entry[0] == "bg":
				var shown: Dictionary = s.opening.names_media["buffer_%d" % s.opening.names_media.bg_show_1]
				var words: PackedByteArray = board.background_software_upload(shown.resource_id).words
				for i in 320*160: image[i] = _tinted(words.decode_u16(i*2) & 0x7fff,bg_tint)
				continue
			var layer: Dictionary = s.opening.bg_layer_0
			if layer.attribute != 0x41000000: continue
			var cut: PackedByteArray = src[layer.resource_id].bytes
			for y in 160:
				for x in 320:
					var index: int = cut[((y >> 7)*3+(x >> 7))*0x4000+(y & 127)*128+(x & 127)]
					var c := _stp(cut.decode_u16(0x18000+index*2),index)
					if c != 0: image[y*320+x] = _blend(image[y*320+x],c,tints.cut)
			continue
		var record: Dictionary = entry[0]
		var resource: PackedByteArray = src[_newest(s,record.frame_table)].bytes
		var anim: int = resource.decode_u32(record.frame_table - 0x80180000 + record.frame*4) - 0x80180000
		# draw_flags bit 2 subtracts the frame origin (actor_sprites.c:169-171).
		var shifted: bool = (int(record.draw_flags) & 4) != 0
		var ox: int = resource.decode_s16(anim+2) if shifted else 0
		var oy: int = resource.decode_s16(anim+4) if shifted else 0
		var row: int = anim+8+int(entry[1])*14
		var texture := resource.decode_u16(row+4)
		var page: int = resource.decode_u32(record.tpage_table-0x80180000+(texture >> 6)*4) - 0x80180000
		var clut_row: int = (resource.decode_u16(row+6) & 0xf00) >> 8
		var u: int = (texture & 7)*16; var v: int = ((texture >> 3) & 7)*16
		for y in resource.decode_s16(row+12):
			for x in resource.decode_s16(row+10):
				var px: int = 160+record.x-ox+resource.decode_s16(row)+x
				var py: int = 120+record.y-oy+resource.decode_s16(row+2)+y
				if px < 0 or px >= 320 or py < 0 or py >= 160: continue
				var index: int = resource[page+(v+y)*128+u+x]
				var c := _stp(o01.decode_u16(0x4490+clut_row*512+index*2),index)
				if c != 0: image[py*320+px] = _blend(image[py*320+px],c,col)
	return image

func sprite_order_matches(s: State, sprites: Array) -> bool:
	# The renderer's queued (actor, part) sequence equals the OT model's.
	var expected: Array = ot_order(s,bytes).filter(func(e): return e[0] is Dictionary).map(func(e): return [int(e[0].index),int(e[1])])
	return sprites.map(func(sp): return [int(sp.index),int(sp.part)]) == expected

static func differing(a: PackedInt32Array, b: PackedInt32Array) -> int:
	var n := 0
	for i in a.size(): if a[i] != b[i]: n += 1
	return n

func _newest(s: State, address: int) -> String:
	var area: Array = s.opening.actor_area
	for i in range(area.size()-1,-1,-1):
		if address-0x80180000 < area[i].bytes: return area[i].resource_id
	return ""

func pixel_mismatches(target: Image, expected: PackedInt32Array, label: String) -> int:
	var mismatches := 0
	var first := ""
	for y in 160:
		for x in 320:
			var p := target.get_pixel(x,y)
			var got: int = (p.r8 >> 3) | ((p.g8 >> 3) << 5) | ((p.b8 >> 3) << 10)
			if got != expected[y*320+x]:
				mismatches += 1
				if first.is_empty(): first = "(%d,%d) got %04x want %04x" % [x,y,got,expected[y*320+x]]
	check(mismatches == 0,"%s differs from the disc-byte reference at %d pixels, first %s" % [label,mismatches,first])
	return mismatches

func compare(s: State, label: String, tints: Dictionary = SOURCE_TINTS, renderer: Scene = scene, src: Dictionary = bytes) -> bool:
	var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)
	var error := renderer.paint(target,s,owner.owned,owner)
	check(error.is_empty(),label + " paint failed: " + error)
	if not error.is_empty(): return false
	var sprites: Variant = renderer._actor_sprites(s,owner)
	check(sprites is Array and sprite_order_matches(s,sprites),label + " queues actor parts outside the source OT order")
	var mismatches := pixel_mismatches(target,reference(s,src,tints),label)
	check(target.get_pixel(5,200) == Color.BLACK,label + " drew below the dec_bg/cut area")
	return mismatches == 0

func compare_actors(s: State, label: String) -> Dictionary:
	# The actor sprites alone over black, for frames whose background the
	# scene refuses (stage 14 shows P_01_08): the same queue, VRAM pages and
	# CLUT rows as paint, against the disc-byte reference. Returns how many
	# pixels the reversed actor loop and the reversed part loop would change.
	var sprites: Variant = scene._actor_sprites(s,owner)
	check(sprites is Array,label + " actor queue refused: " + str(sprites))
	if not sprites is Array: return {}
	check(sprite_order_matches(s,sprites),label + " queues actor parts outside the source OT order")
	var frame := Image.create(320,160,false,Image.FORMAT_RGBA8)
	frame.fill(Color.BLACK)
	var error := Scene.draw_sprites(frame,sprites,owner.owned.vram_words,owner.owned.vram_known_mask,int(s.opening.screen_brightness))
	check(error.is_empty(),label + " actor draw failed: " + error)
	var expected := reference(s,bytes,SOURCE_TINTS,true)
	pixel_mismatches(frame,expected,label)
	return {"actors":differing(expected,reference(s,bytes,SOURCE_TINTS,true,true)),"parts":differing(expected,reference(s,bytes,SOURCE_TINTS,true,false,true))}

# ------------------------------------------------------------------ parts
func _route() -> void:
	var svc := services()
	rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(3))
	var s := stage12_entry(0x40)
	var frames := 0
	var compared: Dictionary = {}
	var stage13_painted := 0
	var stage12_painted := 0
	var p0108_compared := 0
	var reset_done := false
	var actor_compared: Dictionary = {}
	var order_seen := {"actors":0,"parts":0}
	while frames < 900 and not (s.mode_stage == 15):
		# Keep the source's 1/256 commit-frame blink rejection out of the
		# drawn route (run_stages_12_14 covers it).
		if s.mode_stage == 14 and s.substep == 6: rng.configure(Rng.OPENBIOS_PROFILE,seed_hitting(3))
		if s.mode_stage == 13 and s.substep == 1: s.opening.audio.status = "playing"
		# One failed AD0B decode: the read completes with retry status.
		var retry := ""
		if s.mode_stage == 12 and s.substep == 2 and not reset_done: retry = Board.BG_AD0B_ID
		var before := [s.mode_stage,s.substep]
		var out := tick(s,svc,retry)
		if not out.error.is_empty(): check(false,"Frame %d at %s failed: %s" % [frames,str(before),out.error]); return
		if before == [12,3] and s.mode_stage == 12 and s.opening.names_media.has("dec_bg_reset_at"): reset_done = true
		var drawn: Array = draws[-1]
		if s.mode_stage == 12 or (s.mode_stage == 14):
			# P_01_08 (buffer 0, stage11's decode) under O_13 (stage 12) or
			# O_15 (stage 14), CUT layer hidden (bg_layers = 0x80000000).
			check(drawn[3].is_empty() and drawn[4] == [{"kind":"first_arrival_background_draw","slot":1}],"P_01_08 frame %s refused: %s" % [str([s.mode_stage,s.substep]),drawn[3]])
			check(scene.logical_error(s).is_empty(),"Logical draw record missing at " + str([s.mode_stage,s.substep]))
			var bri: int = int(s.opening.bg_brightness)
			var p_key := "%d/%d/bg%x/%s" % [s.mode_stage,s.substep,bri,str(s.opening.shiori_draw.actors.map(func(r): return [r.index,r.frame]))]
			if not compared.has(p_key) and p0108_compared < 24:
				# Before stage12's dec_bg_reset the planted entry brightness is
				# what dec_bg_draw applies; the reset writes the source 0x80.
				var tints: Dictionary = SOURCE_TINTS if bri == 0x80 else {"bg":bri,"cut":SOURCE_TINTS.cut,"col":SOURCE_TINTS.col}
				compared[p_key] = compare(s,"P_01_08 frame " + p_key,tints)
				p0108_compared += 1
			else:
				var p_target := Image.create(320,240,false,Image.FORMAT_RGBA8)
				var p_error := scene.paint(p_target,s,owner.owned,owner)
				check(p_error.is_empty(),"P_01_08 frame paint failed: " + p_error)
			if s.mode_stage == 12: stage12_painted += 1
			# Stage 14's overlapping parts and actors (actor 99's turn frames
			# 10/11 over actors 96-98) are also checked alone over black.
			if s.mode_stage == 14 and s.opening.get("shiori_draw",{}).get("frame_count") == s.frame_count:
				var key: String = "14:" + str(s.opening.shiori_draw.actors.map(func(r): return [r.index,r.frame,r.x,r.y]))
				if not actor_compared.has(key):
					actor_compared[key] = compare_actors(s,"Stage14 actors " + key)
					order_seen.actors = maxi(order_seen.actors,int(actor_compared[key].get("actors",0)))
					order_seen.parts = maxi(order_seen.parts,int(actor_compared[key].get("parts",0)))
		else:
			check(drawn[3].is_empty() and drawn[4] == [{"kind":"first_arrival_background_draw","slot":1}],"Visible frame %s refused: %s" % [str([s.mode_stage,s.substep]),drawn[3]])
			check(scene.logical_error(s).is_empty(),"Logical draw record missing at " + str([s.mode_stage,s.substep]))
			var both: bool = s.opening.shiori_draw.actors.size() == 2
			var key := "%d/%d%s" % [s.mode_stage,s.substep,"/blink" if both else ""]
			if not compared.has(key):
				compared[key] = compare(s,"Frame " + key)
				if s.mode_stage == 13 and s.substep == 0:
					_negatives(s)
					_variants(s)
				if key.ends_with("/blink"): _overlap(s)
			else:
				var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
				var error := scene.paint(target,s,owner.owned,owner)
				check(error.is_empty(),"Stage13 frame paint failed: " + error)
			if s.mode_stage == 13: stage13_painted += 1
		frames += 1
	check(reset_done,"The AD0B retry never reached dec_bg_reset")
	check(s.mode_stage == 15 and s.substep == 0,"Route did not reach stage14's commit frame")
	check(compared.has("13/0") and compared.has("15/0") and compared.keys().any(func(k): return k.ends_with("/blink")),"Route skipped a compared frame: " + str(compared.keys()))
	check(stage13_painted >= 10,"Too few stage13 frames drawn: %d" % stage13_painted)
	check(stage12_painted >= 3 and compared.keys().any(func(k): return k.begins_with("12/") and k.contains("/bg40/")) and compared.keys().any(func(k): return k.begins_with("12/") and k.contains("/bg80/")) and compared.keys().any(func(k): return k.begins_with("14/")),"P_01_08 frames before and after the reset, and stage 14, were not all compared: " + str(compared.keys()))
	# The compared frames must tell the source draw order from its reversals:
	# reversing the part loop changes stage 14 pixels (actor 99's turn frames
	# overlap their own parts), reversing the actor loop changes the 97/98
	# overlap. The real stage 14 actors overlap only where their texels agree.
	check(order_seen.parts > 0 and order_overlap.actors > 0,"No compared frame separates the OT orders: stage14 %s, synthetic %s" % [str(order_seen),str(order_overlap)])
	print("order discrimination: stage14 actors %d px, parts %d px (%d actor frames); synthetic 97/98 overlap %d px" % [order_seen.actors,order_seen.parts,actor_compared.size(),order_overlap.actors])
	# Upload sequence: the source's slot sets, decodes and newly resident pages.
	var at13: Array = uploads_seen.filter(func(u): return u[0] == 13 and u[1] == 0)
	check(at13 == [[13,0,"opening_background_select",Board.BG_AD0B_ID,1,-1],[13,0,"first_arrival_names_bg_upload",Board.BG_AD0B_ID,1,-1],[13,0,"first_arrival_cut_upload",Board.CUT_14_ID,-1,-1],[13,0,"first_arrival_shiori_copy",Board.O_14_ID,-1,0x4800],[13,0,"first_arrival_stage_actor_upload",Board.O_14_ID,5,0x130]],"Stage12 commit uploads differ: " + str(at13))
	var at14: Array = uploads_seen.filter(func(u): return u[0] == 14 and u[1] == 0)
	check(at14 == [[14,0,"first_arrival_shiori_copy",Board.O_15_ID,-1,0x10800],[14,0,"first_arrival_stage_actor_upload",Board.O_15_ID,5,0x82e0],[14,0,"first_arrival_stage_actor_upload",Board.O_15_ID,6,0x42e0]],"Stage13 commit uploads differ: " + str(at14))
	var at15: Array = uploads_seen.filter(func(u): return u[0] == 15)
	check(at15 == [[15,0,"opening_background_select",Board.BG_ACEC_ID,1,-1],[15,0,"first_arrival_names_bg_upload",Board.BG_ACEC_ID,1,-1],[15,0,"first_arrival_cut_upload",Board.CUT_16_ID,-1,-1],[15,0,"first_arrival_shiori_copy",Board.O_16_ID,-1,0xc800],[15,0,"first_arrival_stage_actor_upload",Board.O_16_ID,5,0x82c0],[15,0,"first_arrival_stage_actor_upload",Board.O_16_ID,6,0x42c0]],"Stage14 commit uploads differ: " + str(at15))
	# Stage11's O_13 copy is what stage12 draws before its commit; its pages
	# are uploaded only there (each one passed _shiori_owner and the pump).
	var o13_pages: Array = uploads_seen.filter(func(u): return u[3] == Board.O_13_ID)
	check(not o13_pages.is_empty() and o13_pages.all(func(u): return u[0] == 12 and u[2] == "first_arrival_stage_actor_upload"),"O_13 pages were not uploaded only at stage 12: " + str(o13_pages))
	var turn: Array = uploads_seen.filter(func(u): return u[0] == 14 and u[1] in [2,3] and u[2] == "first_arrival_stage_actor_upload").map(func(u): return [u[4],u[5]])
	check(turn == [[6,0x2e0],[7,0x42e0],[5,0x2e0],[6,0xc2e0],[5,0x82e0],[6,0x2e0],[6,0x42e0]],"Actor99 turn page residency differs from search_tpage (frames 9,10,10,11,12x20,10,9): " + str(turn))
	check(s.opening.names_media.buffer_0.get("vram_lba") == 0xad05 and s.opening.names_media.buffer_0.lba == 0,"dec_bg_reset did not clear buffer0's cache entry: " + str(s.opening.names_media.buffer_0))
	_post_commit_negatives(s)
	completed.append("route")

func _negatives(s: State) -> void:
	# Each guard on the 13/0 frame (stage12 commit), on copies of its state.
	var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
	var hidden := State.new(); hidden.restore(s.projection())
	hidden.opening.bg_layer_0.attribute = 0x80000000; hidden.opening.bg_layer_0.enabled = 0
	check(scene.paint(target,hidden,owner.owned,owner) == "Shiori scene hid a CUT layer the source shows","Stage13 accepted a hidden CUT_14")
	var stale := State.new(); stale.restore(s.projection()); stale.frame_count += 1
	check(scene.paint(target,stale,owner.owned,owner) == "Shiori scene has no actor_draw_all record for this frame","Paint accepted another frame's actor draw")
	var moved := State.new(); moved.restore(s.projection())
	moved.opening.shiori_tpages.resident["5"].page = 0x801842e0
	check(scene.paint(target,moved,owner.owned,owner) == "Shiori scene slot does not hold the drawn page","Paint accepted a slot holding another page")
	var unowned := State.new(); unowned.restore(s.projection())
	unowned.opening.shiori_tpages.resident["5"].key = ""
	check(scene.paint(target,unowned,owner.owned,owner) == "Shiori scene texture has no upload owner","Paint accepted a page without an upload")
	var wrong_bg := State.new(); wrong_bg.restore(s.projection())
	wrong_bg.opening.names_media.buffer_1.resource_id = Board.BG_ACEC_ID
	check(scene.paint(target,wrong_bg,owner.owned,owner) == "Shiori scene shows a slot-1 buffer outside its source sequence","Stage13 accepted ACEC in the shown buffer")
	var other_cut := State.new(); other_cut.restore(s.projection())
	other_cut.opening.bg_layer_0.resource_id = Board.CUT_16_ID
	check(scene.paint(target,other_cut,owner.owned,owner) == "Shiori scene CUT layer differs from its source slot set","Stage13 accepted CUT_16")
	# A missing upload: a fresh pump has no receipts.
	var empty := UploadOwner.new(); empty.pump = Pump.new(); empty.owned = owner.owned
	check(scene.paint(target,s,owner.owned,empty).begins_with("no executed receipt"),"Paint accepted VRAM without upload receipts")
	# VRAM changed after the page readback (slot 5 at x 320).
	var dirty: Dictionary = owner.owned.duplicate()
	dirty["vram_words"] = owner.owned.vram_words.duplicate()
	dirty.vram_words[(10*1024+320)*2] ^= 0xff
	var dirty_owner := UploadOwner.new(); dirty_owner.pump = owner.pump; dirty_owner.owned = dirty
	check(scene.paint(target,s,dirty,dirty_owner) == "native GPU stage actor tpage destination changed after readback","Paint accepted a page overwritten after its upload")
	# Unknown actor palette memory.
	var no_clut: Dictionary = owner.owned.duplicate()
	no_clut["vram_known_mask"] = owner.owned.vram_known_mask.duplicate()
	for i in 512: no_clut.vram_known_mask[(0x1fc*1024)*2+i] = 0
	var no_clut_owner := UploadOwner.new(); no_clut_owner.pump = owner.pump; no_clut_owner.owned = no_clut
	check(scene.paint(target,s,no_clut,no_clut_owner) == "Shiori scene actor references unknown palette memory","Paint accepted an actor CLUT row that was never loaded")
	# Upload ownership of this frame's page.
	var page: Dictionary = s.opening.shiori_tpages.resident["5"]
	check(Upload._shiori_owner(s,Board.O_14_ID,5,0x130,page.key),"Upload owner refused the drawn O_14 page")
	check(not Upload._shiori_owner(s,Board.O_14_ID,6,0x130,page.key) and not Upload._shiori_owner(s,Board.O_15_ID,5,0x130,page.key) and not Upload._shiori_owner(s,Board.O_14_ID,5,0x130,"0".repeat(64)),"Upload owner accepted another slot/resource/key")
	var later := State.new(); later.restore(s.projection()); later.frame_count += 1
	check(not Upload._shiori_owner(later,Board.O_14_ID,5,0x130,page.key),"Upload owner accepted a page outside its draw frame")
	completed.append("negatives")

static func _patch_origin(src: Dictionary, s: State, ox: int, oy: int) -> Dictionary:
	# A copy of the resource bytes with each drawn frame's origin set to
	# (ox, oy). No O_14/O_15/O_16 frame has a non-zero origin, so this is the
	# only way to exercise the draw_flags bit 2 shift; the copy is test-only.
	var out: Dictionary = {}
	for key: String in src: out[key] = {"bytes":src[key].bytes.duplicate(),"sha256":src[key].sha256}
	for record: Dictionary in s.opening.shiori_draw.actors:
		var id := ""
		for i in range(s.opening.actor_area.size()-1,-1,-1):
			if record.frame_table-0x80180000 < s.opening.actor_area[i].bytes: id = s.opening.actor_area[i].resource_id; break
		var resource: PackedByteArray = out[id].bytes
		var anim: int = resource.decode_u32(record.frame_table-0x80180000+record.frame*4) - 0x80180000
		resource.encode_s16(anim+2,ox); resource.encode_s16(anim+4,oy)
	return out

func _variants(s: State) -> void:
	# 13/0 at colours other than 0x80: the texel modulation (c*tint)>>7,
	# saturated at 31, of the background, the CUT layer and the actors.
	var tinted := State.new(); tinted.restore(s.projection())
	tinted.opening.bg_brightness = 0x50; tinted.opening.screen_brightness = 0x60
	tinted.opening.D_801220E0 = 0x40; tinted.opening.D_801220E1 = 0xa0; tinted.opening.D_801220E2 = 0xff
	var tints := {"bg":0x50,"cut":[0x40,0xa0,0xff],"col":0x60}
	check(differing(reference(tinted,bytes,tints),reference(s)) > 0,"The tinted 13/0 reference equals the 0x80 one")
	compare(tinted,"Frame 13/0 tinted",tints)
	# 13/0 with non-zero frame origins: draw_flags bit 2 (set on every drawn
	# record) moves each sprite by -origin.
	check(s.opening.shiori_draw.actors.all(func(r): return (int(r.draw_flags) & 4) != 0),"13/0 draws a record without draw_flags bit 2")
	var shifted := Scene.new()
	shifted.bytes_by_id = _patch_origin(scene.bytes_by_id,s,6,-5)
	var src := _patch_origin(bytes,s,6,-5)
	check(differing(reference(s,src),reference(s)) > 0,"The shifted-origin 13/0 reference equals the unshifted one")
	compare(s,"Frame 13/0 shifted origin",SOURCE_TINTS,shifted,src)
	completed.append("variants")

func _overlap(s: State) -> void:
	# Actors 97 and 98 (both OT 8) moved onto each other: the blink part over
	# the mouth part, real O_14 frames from the resident page. The reversed
	# actor loop must change pixels, so the source order is what is checked.
	var moved := State.new(); moved.restore(s.projection())
	var a97: Dictionary = moved.opening.shiori_draw.actors.filter(func(r): return r.index == 97)[0]
	var a98: Dictionary = moved.opening.shiori_draw.actors.filter(func(r): return r.index == 98)[0]
	check(a97.unk_01 == a98.unk_01,"Actors 97/98 no longer share an OT entry")
	# O_14 frames 0-3 (actor 98 here) sit at (176,40), frames 4-7 (actor 97)
	# at (168,72). The first shift of actor 98, row-major over a fixed grid,
	# whose opaque texels differ under the two actor orders is compared.
	var x98: int = int(a98.x); var y98: int = int(a98.y)
	var found := 0
	for dy in range(-48,49,4):
		for dx in range(-16,17,4):
			a98.x = x98 + dx; a98.y = y98 + dy
			found = differing(reference(moved,bytes,SOURCE_TINTS,true),reference(moved,bytes,SOURCE_TINTS,true,true))
			if found > 0: break
		if found > 0: break
	var label := "Frame %d/%d overlap 97/98 at (%d,%d)" % [s.mode_stage,s.substep,int(a98.x)-x98,int(a98.y)-y98]
	order_overlap.actors = maxi(order_overlap.actors,differing(reference(moved),reference(moved,bytes,SOURCE_TINTS,false,true)))
	compare(moved,label)
	completed.append("overlap")

func _post_commit_negatives(s: State) -> void:
	# Pump, schema and memcpy guards on the real resources.
	var pump := Pump.new()
	var owned: Dictionary = owner.owned.duplicate(true)
	const BAD := "native GPU stage actor pump received an unsupported resource/slot/offset combination"
	check(pump.execute_stage_actor_vram(board,Board.O_14_ID,5,0x0,"o14-bad-offset".sha256_text(),owned).error == BAD,"Pump accepted an O_14 offset its tpage table does not name")
	check(pump.execute_stage_actor_vram(board,Board.O_15_ID,8,0x2e0,"o15-cut-slot".sha256_text(),owned).error.begins_with("native GPU stage actor pump needs"),"Pump accepted a page in the CUT's locked slot 8")
	check(pump.execute_stage_actor_vram(board,Board.O_16_ID,5,0xc2c0,"o16-past-table".sha256_text(),owned).error == BAD,"Pump accepted an O_16 page past its three-entry table")
	check(pump.execute_names_bg(board,Board.BG_AD0B_ID,"ad0b-buffer0".sha256_text(),owned,0).error == "native GPU names background buffer differs from source","Pump accepted AD0B into buffer 0")
	check(Board.shiori_page_offsets(Board.O_15_ID,bytes[Board.O_15_ID].bytes) == ([0x2e0,0x42e0,0x82e0,0xc2e0] as Array[int]) and Board.shiori_page_offsets(Board.O_14_ID,bytes[Board.O_14_ID].bytes) == ([0x130] as Array[int]) and Board.shiori_page_offsets(Board.O_16_ID,bytes[Board.O_16_ID].bytes) == ([0x2c0,0x42c0,0x82c0] as Array[int]),"Shiori tpage tables differ from the disc bytes")
	check(not media._intent_schema({"kind":"first_arrival_shiori_copy","resource_id":Board.O_14_ID,"sha256":Board.O_14_SHA,"bytes":0x4800,"source":0x801e8000}).is_empty() and not media._intent_schema({"kind":"first_arrival_shiori_copy","resource_id":Board.O_16_ID,"sha256":Board.O_16_SHA,"bytes":0x10800,"source":0x801e8000}).is_empty(),"Schema accepted a memcpy with another source or length")
	check(not media._intent_schema({"kind":"first_arrival_names_bg_upload","batch_key":"k".sha256_text(),"resource_id":Board.BG_AD0B_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":Board.BG_AD0B_WORD_SHA}).is_empty(),"Schema accepted AD0B into buffer 0")
	check(not media._intent_schema({"kind":"first_arrival_cut_upload","batch_key":"k".sha256_text(),"resource_id":Board.CUT_14_ID,"sha256":Board.CUT_16_SHA}).is_empty(),"Schema accepted CUT_14 with another payload")
	check(not media._intent_schema({"kind":"first_arrival_stage_actor_upload","batch_key":"k".sha256_text(),"resource_id":Board.O_15_ID,"slot":5,"source_offset":0x130}).is_empty(),"Schema accepted an O_15 page at an O_14 offset")
	# A page whose 0x4000 bytes straddle two memcpy layers is refused.
	var area := [{"resource_id":Board.O_13_ID,"sha256":Board.O_13_SHA,"bytes":0x20800},{"resource_id":Board.O_14_ID,"sha256":Board.O_14_SHA,"bytes":0x4800}]
	check(Actors.page_layer(bytes,area,0x80184000).error == Actors.PAGE_SPANS_LAYERS and Actors.page_layer(bytes,area,0x80180130).resource_id == Board.O_14_ID and Actors.page_layer(bytes,area,0x80188000).resource_id == Board.O_13_ID,"Page layer resolution differs")
	completed.append("post_commit")

func _reset_checks() -> void:
	# dec_bg_reset on the real owner: the decode guard follows the reset
	# marker, never a stale cache state.
	var s := stage12_entry(0x40)
	s.substep = 3
	s.opening.actor96.flags = 4
	var out := Session.FrameResult.new()
	media.shiori_effect(s,"dec_bg_reset",0,out)
	check(out.error.is_empty() and s.opening.names_media.bg_show_1 == 0 and s.opening.names_media.buffer_0.lba == 0 and s.opening.names_media.buffer_0.vram_lba == 0xad05 and s.opening.names_media.buffer_0.resource_id == Board.BG_AD05_ID and s.opening.names_media.buffer_1 == {"lba":0,"vram_lba":0xacf6} and s.opening.bg_brightness == 0x80 and s.opening.names_media.dec_bg_reset_at == [12,3,1000],"dec_bg_reset state differs: " + str(s.opening.names_media))
	var stale := State.new(); stale.restore(s.projection()); stale.opening.names_media.erase("dec_bg_reset_at")
	out = Session.FrameResult.new()
	check(media.background_decode(stale,1,out) == -1 and out.error == "Shiori background decode requires the source slot-1 buffers","Decode accepted cleared caches without this substep's reset")
	var moved := State.new(); moved.restore(s.projection()); moved.substep = 2
	out = Session.FrameResult.new()
	media.shiori_effect(moved,"dec_bg_reset",0,out)
	check(out.error == "Shiori stage effect is outside its source substep","dec_bg_reset accepted outside cases 3/6")
	var shown := State.new(); shown.restore(s.projection()); shown.opening.names_media.bg_show_1 = 1
	out = Session.FrameResult.new()
	check(media.background_decode(shown,1,out) == -1 and not out.error.is_empty(),"Decode after reset accepted a shown buffer 1")
	completed.append("reset")

func _initialize() -> void:
	var error := _fixture()
	if not error.is_empty():
		printerr(error)
		quit(1)
		return
	_reset_checks()
	_route()
	for part in ["reset","route","negatives","variants","overlap","post_commit"]:
		if not completed.has(part):
			failures += 1
			printerr("Part %s did not run to its end" % part)
	if failures == 0: print("PASS: %d stage12-14 render checks" % checks)
	quit(1 if failures else 0)
