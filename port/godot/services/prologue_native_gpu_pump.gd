extends RefCounted
## Finite actor-96 CSR/CLUT word transfers. No rasterizer or whole-VRAM claim.
## Source: frame_render_audio.c::csr_load_vram/palette_load_vram; prologue.c.
const Store = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const VRAM_BYTES := 1024*512*2
var _receipts: Dictionary = {}
var _board_receipts: Dictionary = {}
var _names_bg_receipts: Dictionary = {}
var _cut_receipts: Dictionary = {}
var _stage_actor_receipts: Dictionary = {}

func execute(store: Store, batch_key: String, owned: Dictionary) -> Dictionary:
	var failure := _validate(store,batch_key,owned)
	if not failure.is_empty(): return _reject(failure)
	if _receipts.has(batch_key): return _reject("native GPU batch already executed; query its receipt instead of replaying")
	var vram: PackedByteArray = owned.vram_words.duplicate()
	var known: PackedByteArray
	if owned.has("vram_known_mask"):
		known = owned.vram_known_mask.duplicate()
	else:
		known.resize(VRAM_BYTES) # Unknown storage, not proof that VRAM was zero.
	var actor: PackedByteArray = owned.actor_runtime_bytes.duplicate()
	var scratch: PackedByteArray = owned.scratch_words.duplicate()
	var regions: Array[Dictionary] = []
	var ordered_ids: Array[String] = []
	for item: Dictionary in owned.pending_gpu_effects:
		var bytes: PackedByteArray = item.source_bytes.duplicate()
		if item.operation == "load_palette":
			bytes = _mode2_stp(bytes)
			if item.id == "actor96.gray_palette":
				scratch = bytes.duplicate()
			else:
				for index in bytes.size(): actor[item.source_offset+index] = bytes[index]
		var rect := Rect2i(item.destination[0],item.destination[1],item.destination[2],item.destination[3])
		var row_bytes := rect.size.x*2
		for row in rect.size.y:
			var destination := ((rect.position.y+row)*1024+rect.position.x)*2
			for column in row_bytes:
				vram[destination+column] = bytes[row*row_bytes+column]
				known[destination+column] = 255
		var actual := _region(vram,rect)
		if actual != bytes: return _reject("native GPU destination readback differs from transferred source")
		regions.append({"id":item.id,"rectangle":item.destination.duplicate(),"sha256":Store._sha(actual),"size":actual.size()})
		ordered_ids.append(item.id)
	# Commit all CPU-visible source STP writebacks and physical word copies only
	# after every queued record and destination has been checked on local copies.
	owned["vram_words"] = vram
	owned["vram_known_mask"] = known
	owned["actor_runtime_bytes"] = actor
	owned["scratch_words"] = scratch
	owned["pending_gpu_effects"] = []
	var receipt := {"batch_key":batch_key,"scratch_owner":owned.scratch_owner,"vram_owner":owned.vram_owner,
		"regions":regions,"gpu_operation_ids":ordered_ids,"gpu_operations":18,
		"actor_runtime_sha256":Store._sha(actor),"scratch_sha256":Store._sha(scratch),
		"original_resource_manifest_sha256":store.identity(),"batch_complete":true,"vram_ready":false}
	_receipts[batch_key] = receipt.duplicate(true)
	return {"error":"","receipt":receipt}

func batch_ready(batch_key: String, owned: Dictionary) -> String:
	if not _receipts.has(batch_key): return "native GPU batch has no executed receipt"
	var receipt: Dictionary = _receipts[batch_key]
	if not _owners(owned) or owned.vram_owner != receipt.vram_owner or owned.scratch_owner != receipt.scratch_owner:
		return "native GPU batch belongs to another runtime buffer owner"
	var scratch_sha := Store._sha(owned.scratch_words) if owned.get("scratch_words") is PackedByteArray else ""
	var scratch_ok: bool = scratch_sha == str(receipt.scratch_sha256)
	if not scratch_ok:
		for cut_rec: Dictionary in _cut_receipts.values():
			if scratch_sha == cut_rec.get("scratch_sha256",""):
				scratch_ok = true
				break
	if not owned.get("actor_runtime_bytes") is PackedByteArray or not owned.get("scratch_words") is PackedByteArray or Store._sha(owned.actor_runtime_bytes) != receipt.actor_runtime_sha256 or not scratch_ok:
		return "native GPU source writeback state changed after execution"
	if not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES: return "native GPU destination ownership mask is unavailable"
	for region: Dictionary in receipt.regions:
		var rect := Rect2i(region.rectangle[0],region.rectangle[1],region.rectangle[2],region.rectangle[3])
		if Store._sha(_region(owned.vram_words,rect)) != region.sha256: return "native GPU batch destination changed after readback"
		var mask := _region(owned.vram_known_mask,rect)
		if mask.count(255) != mask.size(): return "native GPU batch destination is no longer fully known"
	return ""

func reset_session() -> void:
	_receipts.clear()
	_board_receipts.clear()
	_names_bg_receipts.clear()
	_cut_receipts.clear()
	_stage_actor_receipts.clear()

func execute_board(board_store: BoardStore, batch_key: String, owned: Dictionary) -> Dictionary:
	if board_store == null or not Store._sha_string(batch_key): return _reject("native GPU board pump needs an authenticated board store and batch key")
	if not _owners(owned) or not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return _reject("native GPU board pump requires prior owned VRAM and known mask")
	if _board_receipts.has(batch_key): return _reject("native GPU board batch already executed; query its receipt instead of replaying")
	var upload := board_store.software_upload()
	if not upload.get("error") is String or not upload.error.is_empty() or upload.get("rectangle") != [320,0,640,160] or not upload.get("words") is PackedByteArray or upload.words.size() != 204800 or Store._sha(upload.words) != BoardStore.SOFTWARE_WORD_SHA:
		return _reject("native GPU board pump received malformed software decode words")
	var words: PackedByteArray = upload.words
	var vram: PackedByteArray = owned.vram_words.duplicate()
	var known: PackedByteArray = owned.vram_known_mask.duplicate()
	var row_bytes := 640*2
	for row in range(160):
		var destination := (row*1024+320)*2
		for column in range(row_bytes):
			vram[destination+column] = words[row*row_bytes+column]
			known[destination+column] = 255
	for i in range(5,15):
		var src_x := i*64+63
		var dst_x := 960+(i-5)
		for row in range(255):
			var src_off := (row*1024+src_x)*2
			var dst_off := (row*1024+dst_x)*2
			vram[dst_off] = vram[src_off]
			vram[dst_off+1] = vram[src_off+1]
			known[dst_off] = known[src_off]
			known[dst_off+1] = known[src_off+1]
	var decoded_actual := _region(vram,Rect2i(320,0,640,160))
	var decoded_known := _region(known,Rect2i(320,0,640,160))
	var left_384 := _region(vram,Rect2i(320,0,384,160))
	var seam_top := _region(vram,Rect2i(960,0,10,160))
	var seam_top_known := _region(known,Rect2i(960,0,10,160))
	var seam_tail_words := _region(vram,Rect2i(960,160,10,95))
	var seam_tail_known := _region(known,Rect2i(960,160,10,95))
	if decoded_actual != words or decoded_known.count(255) != 204800 or seam_top_known.count(255) != 3200:
		return _reject("native GPU board decode or move_255_line readback differs from source")
	owned["vram_words"] = vram
	owned["vram_known_mask"] = known
	var receipt := {"batch_key":batch_key,"vram_owner":owned.vram_owner,
		"decode_rect":[320,0,640,160],"decode_sha256":BoardStore.SOFTWARE_WORD_SHA,
		"left_384_sha256":Store._sha(left_384),
		"move_call":[5,14,15],"seam_top_rect":[960,0,10,160],"seam_top_sha256":Store._sha(seam_top),
		"seam_tail_rect":[960,160,10,95],"seam_tail_words_sha256":Store._sha(seam_tail_words),
		"seam_tail_known_sha256":Store._sha(seam_tail_known),
		"board_manifest_sha256":BoardStore.MANIFEST_SHA,"batch_complete":true,"vram_ready":false}
	_board_receipts[batch_key] = receipt.duplicate(true)
	return {"error":"","receipt":receipt}

func board_batch_ready(batch_key: String, owned: Dictionary) -> String:
	if not _board_receipts.has(batch_key): return "native GPU board batch has no executed receipt"
	var receipt: Dictionary = _board_receipts[batch_key]
	if not _owners(owned) or owned.vram_owner != receipt.vram_owner:
		return "native GPU board batch belongs to another runtime buffer owner"
	if not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return "native GPU board destination ownership mask is unavailable"
	var post_495d := Store._sha(_region(owned.vram_words,Rect2i(704,0,320,160))) == BoardStore.BG_495D_WORD_SHA
	if post_495d:
		if Store._sha(_region(owned.vram_words,Rect2i(320,0,384,160))) != receipt.left_384_sha256 or _region(owned.vram_known_mask,Rect2i(320,0,704,160)).count(255) != 225280:
			return "native GPU board left-half or 0x495D overwrite destination changed after readback"
	else:
		if Store._sha(_region(owned.vram_words,Rect2i(320,0,640,160))) != receipt.decode_sha256 or _region(owned.vram_known_mask,Rect2i(320,0,640,160)).count(255) != 204800:
			return "native GPU board decode destination changed after readback"
		if Store._sha(_region(owned.vram_words,Rect2i(960,0,10,160))) != receipt.seam_top_sha256 or _region(owned.vram_known_mask,Rect2i(960,0,10,160)).count(255) != 3200:
			return "native GPU board move_255_line top columns changed after readback"
	if Store._sha(_region(owned.vram_words,Rect2i(960,160,10,95))) != receipt.seam_tail_words_sha256 or Store._sha(_region(owned.vram_known_mask,Rect2i(960,160,10,95))) != receipt.seam_tail_known_sha256:
		return "native GPU board move_255_line tail columns changed after readback"
	return ""

func execute_names_bg(board_store: BoardStore, resource_id: String, batch_key: String, owned: Dictionary) -> Dictionary:
	if board_store == null or not Store._sha_string(batch_key): return _reject("native GPU names background pump needs an authenticated board store and batch key")
	if not _owners(owned) or not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return _reject("native GPU names background pump requires prior owned VRAM and known mask")
	if _names_bg_receipts.has(batch_key): return _reject("native GPU names background batch already executed; query its receipt instead of replaying")
	var expected_rect: Array
	var expected_sha: String
	var expected_buffer: int
	if resource_id == BoardStore.BG_495D_ID:
		expected_rect = [704,0,320,160]
		expected_sha = BoardStore.BG_495D_WORD_SHA
		expected_buffer = 0
	elif resource_id == BoardStore.BG_4966_ID:
		expected_rect = [320,256,320,160]
		expected_sha = BoardStore.BG_4966_WORD_SHA
		expected_buffer = 1
	elif resource_id == BoardStore.BG_ACDD_ID:
		expected_rect = [320,256,320,160]
		expected_sha = BoardStore.BG_ACDD_WORD_SHA
		expected_buffer = 1
	elif resource_id == BoardStore.BG_ACE6_ID:
		expected_rect = [704,0,320,160]
		expected_sha = BoardStore.BG_ACE6_WORD_SHA
		expected_buffer = 0
	elif resource_id == BoardStore.BG_ACF6_ID:
		expected_rect = [704,0,320,160]
		expected_sha = BoardStore.BG_ACF6_WORD_SHA
		expected_buffer = 0
	elif resource_id == BoardStore.BG_ACD4_ID:
		expected_rect = [320,256,320,160]
		expected_sha = BoardStore.BG_ACD4_WORD_SHA
		expected_buffer = 1
	elif resource_id == BoardStore.BG_ACEC_ID:
		expected_rect = [704,0,320,160]
		expected_sha = BoardStore.BG_ACEC_WORD_SHA
		expected_buffer = 0
	else:
		return _reject("native GPU names background pump received an unsupported resource_id")
	var upload := board_store.background_software_upload(resource_id)
	if not upload.get("error") is String or not upload.error.is_empty() or upload.get("rectangle") != expected_rect or upload.get("buffer") != expected_buffer or not upload.get("words") is PackedByteArray or upload.words.size() != 102400 or Store._sha(upload.words) != expected_sha:
		return _reject("native GPU names background pump received malformed software decode words")
	var words: PackedByteArray = upload.words
	var vram: PackedByteArray = owned.vram_words.duplicate()
	var known: PackedByteArray = owned.vram_known_mask.duplicate()
	var rect := Rect2i(expected_rect[0],expected_rect[1],expected_rect[2],expected_rect[3])
	var row_bytes := rect.size.x*2
	for row in range(rect.size.y):
		var destination := ((rect.position.y+row)*1024+rect.position.x)*2
		for column in range(row_bytes):
			vram[destination+column] = words[row*row_bytes+column]
			known[destination+column] = 255
	var decoded_actual := _region(vram,rect)
	var decoded_known := _region(known,rect)
	if decoded_actual != words or decoded_known.count(255) != 102400:
		return _reject("native GPU names background decode readback differs from source")
	owned["vram_words"] = vram
	owned["vram_known_mask"] = known
	var receipt := {"batch_key":batch_key,"vram_owner":owned.vram_owner,
		"resource_id":resource_id,"buffer":expected_buffer,
		"decode_rect":expected_rect,"decode_sha256":expected_sha,
		"board_manifest_sha256":BoardStore.MANIFEST_SHA,"batch_complete":true,"vram_ready":false}
	_names_bg_receipts[batch_key] = receipt.duplicate(true)
	return {"error":"","receipt":receipt}

func names_bg_batch_ready(batch_key: String, owned: Dictionary) -> String:
	if not _names_bg_receipts.has(batch_key): return "native GPU names background batch has no executed receipt"
	var receipt: Dictionary = _names_bg_receipts[batch_key]
	if not _owners(owned) or owned.vram_owner != receipt.vram_owner:
		return "native GPU names background batch belongs to another runtime buffer owner"
	if not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return "native GPU names background destination ownership mask is unavailable"
	var rect := Rect2i(receipt.decode_rect[0],receipt.decode_rect[1],receipt.decode_rect[2],receipt.decode_rect[3])
	if Store._sha(_region(owned.vram_words,rect)) != receipt.decode_sha256 or _region(owned.vram_known_mask,rect).count(255) != 102400:
		return "native GPU names background decode destination changed after readback"
	return ""

static func _prologue_palette_adjust(source_clut: PackedByteArray) -> PackedByteArray:
	var out := source_clut.duplicate()
	var w1 := out.decode_u32(4)
	if (w1 & 0x7fff0000) == 0: w1 |= 0x00017fff
	if (w1 & 0x00007fff) == 0: w1 |= 0x7fff0001
	out.encode_u32(4, w1)
	for i in range(1, 0x40):
		for word_idx in [i * 2, i * 2 + 1]:
			var w := out.decode_u32(word_idx * 4)
			if (w & 0x7fff0000) == 0: w |= 0x00017fff
			if (w & 0x00007fff) == 0: w |= 0x7fff0001
			out.encode_u32(word_idx * 4, w)
	return out

static func _gray_clut(source_clut: PackedByteArray) -> PackedByteArray:
	var out := PackedByteArray()
	out.resize(512)
	for i in range(256):
		var c := source_clut.decode_u16(i * 2)
		if c != 0:
			var r := c & 0x1f
			var g := (c >> 5) & 0x1f
			var b := (c >> 10) & 0x1f
			@warning_ignore("integer_division")
			var gray := mini(0x1f, (r + g + b) / 3)
			c = (c & 0x8000) | (gray << 10) | (gray << 5) | gray
		out.encode_u16(i * 2, c)
	return out

func execute_cut_vram(board_store: BoardStore, resource_id: String, batch_key: String, owned: Dictionary) -> Dictionary:
	if board_store == null or not Store._sha_string(batch_key): return _reject("native GPU cut pump needs an authenticated board store and batch key")
	if not _owners(owned) or not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return _reject("native GPU cut pump requires prior owned VRAM and known mask")
	if _cut_receipts.has(batch_key): return _reject("native GPU cut batch already executed; query its receipt instead of replaying")
	var expected_sha := BoardStore.CUT_12_SHA if resource_id == BoardStore.CUT_12_ID else (BoardStore.CUT_05_SHA if resource_id == BoardStore.CUT_05_ID else (BoardStore.CUT_06_SHA if resource_id == BoardStore.CUT_06_ID else (BoardStore.CUT_07_SHA if resource_id == BoardStore.CUT_07_ID else "")))
	if expected_sha.is_empty(): return _reject("native GPU cut pump received an unsupported resource_id")
	var transfer := board_store.stage_transfer(resource_id)
	if not transfer.get("error") is String or not transfer.error.is_empty() or not transfer.get("bytes") is PackedByteArray or transfer.bytes.size() != 100352 or Store._sha(transfer.bytes) != expected_sha:
		return _reject("native GPU cut pump received malformed cut payload")
	var nbn: PackedByteArray = transfer.bytes
	var vram: PackedByteArray = owned.vram_words.duplicate()
	var known: PackedByteArray = owned.vram_known_mask.duplicate()
	var scratch: PackedByteArray = owned.scratch_words.duplicate()
	for row_idx in range(2):
		for col_idx in range(3):
			var page_idx := row_idx * 3 + col_idx
			var page_bytes := nbn.slice(page_idx * 0x4000, (page_idx + 1) * 0x4000)
			var rect := Rect2i(512 + col_idx * 64, row_idx * 128, 64, 128)
			var row_bytes := 128
			for r in range(128):
				var dst := ((rect.position.y + r) * 1024 + rect.position.x) * 2
				for c in range(row_bytes):
					vram[dst + c] = page_bytes[r * row_bytes + c]
					known[dst + c] = 255
	var raw_pal := nbn.slice(0x18000, 0x18200)
	if resource_id == BoardStore.CUT_12_ID:
		raw_pal = _prologue_palette_adjust(raw_pal)
	var color_pal := _mode2_stp(raw_pal)
	var gray_raw := _gray_clut(color_pal)
	var gray_pal := _mode2_stp(gray_raw)
	scratch = gray_pal.duplicate()
	var row495_dst := (495 * 1024) * 2
	var row494_dst := (494 * 1024) * 2
	for i in range(512):
		vram[row495_dst + i] = color_pal[i]
		known[row495_dst + i] = 255
		vram[row494_dst + i] = gray_pal[i]
		known[row494_dst + i] = 255
	var tex_rect := Rect2i(512, 0, 192, 256)
	var clut_rect := Rect2i(0, 494, 256, 2)
	var tex_sha := Store._sha(_region(vram, tex_rect))
	var clut_sha := Store._sha(_region(vram, clut_rect))
	owned["vram_words"] = vram
	owned["vram_known_mask"] = known
	owned["scratch_words"] = scratch
	var receipt := {"batch_key":batch_key,"vram_owner":owned.vram_owner,"scratch_owner":owned.scratch_owner,
		"resource_id":resource_id,"tex_rect":[512,0,192,256],"tex_sha256":tex_sha,
		"clut_rect":[0,494,256,2],"clut_sha256":clut_sha,"scratch_sha256":Store._sha(scratch),
		"board_manifest_sha256":BoardStore.MANIFEST_SHA,"batch_complete":true,"vram_ready":false}
	_cut_receipts[batch_key] = receipt.duplicate(true)
	return {"error":"","receipt":receipt}

func cut_batch_ready(batch_key: String, owned: Dictionary) -> String:
	if not _cut_receipts.has(batch_key): return "native GPU cut batch has no executed receipt"
	var receipt: Dictionary = _cut_receipts[batch_key]
	if not _owners(owned) or owned.vram_owner != receipt.vram_owner or owned.scratch_owner != receipt.scratch_owner:
		return "native GPU cut batch belongs to another runtime buffer owner"
	if not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return "native GPU cut destination ownership mask is unavailable"
	var tex_rect := Rect2i(512, 0, 192, 256)
	var clut_rect := Rect2i(0, 494, 256, 2)
	if Store._sha(_region(owned.vram_words, tex_rect)) != receipt.tex_sha256 or _region(owned.vram_known_mask, tex_rect).count(255) != 98304:
		return "native GPU cut texture destination changed after readback"
	if Store._sha(_region(owned.vram_words, clut_rect)) != receipt.clut_sha256 or _region(owned.vram_known_mask, clut_rect).count(255) != 1024:
		return "native GPU cut CLUT destination changed after readback"
	return ""

func execute_stage_actor_vram(board_store: BoardStore, resource_id: String, slot: int, source_offset: int, batch_key: String, owned: Dictionary) -> Dictionary:
	if board_store == null or not Store._sha_string(batch_key) or slot not in [5, 6, 7]:
		return _reject("native GPU stage actor pump needs an authenticated board store, slot 5..7, and batch key")
	if not _owners(owned) or not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return _reject("native GPU stage actor pump requires prior owned VRAM and known mask")
	if _stage_actor_receipts.has(batch_key): return _reject("native GPU stage actor batch already executed; query its receipt instead of replaying")
	var valid_o12_96 := resource_id == BoardStore.O_12_ID and slot == 5 and source_offset == 0x0120
	var valid_o12_97 := resource_id == BoardStore.O_12_ID and slot == 6 and source_offset == 0x41a0
	var valid_o05_96 := resource_id == BoardStore.O_05_ID and slot == 5 and source_offset == 0x00a0
	var valid_o06_96_0 := resource_id == BoardStore.O_06_ID and slot == 5 and source_offset == 0x4310
	var valid_o06_96_1 := resource_id == BoardStore.O_06_ID and slot == 6 and source_offset == 0x8310
	var valid_o06_99 := resource_id == BoardStore.O_06_ID and slot == 7 and source_offset == 0x0130
	var valid_o07_96_0 := resource_id == BoardStore.O_07_ID and slot == 5 and source_offset == 0x0190
	var valid_o07_96_1 := resource_id == BoardStore.O_07_ID and slot == 6 and source_offset == 0x4190
	var valid_o07_96_2 := resource_id == BoardStore.O_07_ID and slot == 7 and source_offset == 0x8190
	if not valid_o12_96 and not valid_o12_97 and not valid_o05_96 and not valid_o06_96_0 and not valid_o06_96_1 and not valid_o06_99 and not valid_o07_96_0 and not valid_o07_96_1 and not valid_o07_96_2:
		return _reject("native GPU stage actor pump received an unsupported resource/slot/offset combination")
	var expected_sha := BoardStore.O_12_SHA if resource_id == BoardStore.O_12_ID else (BoardStore.O_05_SHA if resource_id == BoardStore.O_05_ID else (BoardStore.O_06_SHA if resource_id == BoardStore.O_06_ID else BoardStore.O_07_SHA))
	var transfer := board_store.stage_transfer(resource_id)
	if not transfer.get("error") is String or not transfer.error.is_empty() or not transfer.get("bytes") is PackedByteArray or Store._sha(transfer.bytes) != expected_sha:
		return _reject("native GPU stage actor pump received malformed actor NBN payload")
	var page_bytes: PackedByteArray = transfer.bytes.slice(source_offset, source_offset + 0x4000)
	if page_bytes.size() != 16384:
		return _reject("native GPU stage actor tpage slice is incomplete")
	var vram: PackedByteArray = owned.vram_words.duplicate()
	var known: PackedByteArray = owned.vram_known_mask.duplicate()
	var rect := Rect2i((slot & 0x0f) * 64, 0, 64, 128)
	for r in range(128):
		var dst := ((rect.position.y + r) * 1024 + rect.position.x) * 2
		for c in range(128):
			vram[dst + c] = page_bytes[r * 128 + c]
			known[dst + c] = 255
	var actual := _region(vram, rect)
	if actual != page_bytes or _region(known, rect).count(255) != 16384:
		return _reject("native GPU stage actor tpage readback differs from source")
	owned["vram_words"] = vram
	owned["vram_known_mask"] = known
	var receipt := {"batch_key":batch_key,"vram_owner":owned.vram_owner,
		"resource_id":resource_id,"slot":slot,"source_offset":source_offset,
		"rect":[rect.position.x,rect.position.y,rect.size.x,rect.size.y],
		"sha256":Store._sha(actual),"board_manifest_sha256":BoardStore.MANIFEST_SHA,"batch_complete":true,"vram_ready":false}
	_stage_actor_receipts[batch_key] = receipt.duplicate(true)
	return {"error":"","receipt":receipt}

func stage_actor_batch_ready(batch_key: String, owned: Dictionary) -> String:
	if not _stage_actor_receipts.has(batch_key): return "native GPU stage actor batch has no executed receipt"
	var receipt: Dictionary = _stage_actor_receipts[batch_key]
	if not _owners(owned) or owned.vram_owner != receipt.vram_owner:
		return "native GPU stage actor batch belongs to another runtime buffer owner"
	if not owned.get("vram_known_mask") is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES:
		return "native GPU stage actor destination ownership mask is unavailable"
	var rect := Rect2i(receipt.rect[0],receipt.rect[1],receipt.rect[2],receipt.rect[3])
	if Store._sha(_region(owned.vram_words, rect)) != receipt.sha256 or _region(owned.vram_known_mask, rect).count(255) != 16384:
		return "native GPU stage actor tpage destination changed after readback"
	return ""

static func _validate(store: Store, batch_key: String, owned: Dictionary) -> String:
	if store == null or store.identity().is_empty() or not Store._sha_string(batch_key): return "native GPU pump needs an authenticated resource and bounded batch identity"
	if not _owners(owned): return "native GPU pump requires explicit scratch and VRAM owners/buffers"
	if not owned.get("scratch_words") is PackedByteArray or owned.scratch_words.size() != 512: return "native GPU pump requires the live 512-byte scratch destination"
	if not owned.get("actor_runtime_bytes") is PackedByteArray or owned.actor_runtime_bytes.size() != 26624 or Store._sha(owned.actor_runtime_bytes) != Store.ACTOR_SHA:
		return "native GPU pump requires the original completed actor read before STP writeback"
	if owned.has("vram_known_mask") and (not owned.vram_known_mask is PackedByteArray or owned.vram_known_mask.size() != VRAM_BYTES): return "native VRAM known mask has an invalid extent"
	var pending: Variant = owned.get("pending_gpu_effects")
	if not pending is Array or pending.size() != 18: return "native actor batch requires exactly 18 queued GPU effects"
	var source := store.upload_requests()
	for index in range(18):
		var effect: Variant = pending[index]
		var expected: Dictionary = source[index if index < 17 else 18]
		if not effect is Dictionary or not effect.has_all(["id","operation","source_bytes","destination","size"]) or not effect.id is String or effect.id != expected.id or not effect.operation is String or effect.operation != expected.operation:
			return "native GPU effect identity/order differs from the original queue"
		var rect: Dictionary = expected.destination
		if not Store._same(effect.destination,[rect.x_words,rect.y,rect.width_words,rect.height]) or not effect.size is int or effect.size != rect.width_words*rect.height*2:
			return "native GPU transfer rectangle/byte count differs from source"
		if not effect.source_bytes is PackedByteArray or effect.source_bytes.size() != effect.size: return "native GPU transfer payload is incomplete"
		if index < 17:
			if not effect.get("source_offset") is int or effect.source_offset != expected.source.offset or not effect.get("source_pointer") is int or effect.source_pointer != expected.source.address or effect.source_bytes != owned.actor_runtime_bytes.slice(expected.source.offset,expected.source.offset+expected.source.size):
				return "native GPU transfer source extent differs from the completed original read"
		else:
			if not Store._same(effect.get("source"),expected.source) or effect.source_bytes != owned.scratch_words: return "native gray CLUT does not reference the actual post-CPU scratch snapshot"
		if effect.operation == "load_palette":
			var mode: Variant = effect.get("stp_mode",effect.get("mode"))
			if not mode is int or mode != 2: return "native palette upload needs the explicit source STP mode 2"
			if effect.has("stp_mode") and effect.has("mode") and (not effect.mode is int or effect.mode != mode): return "conflicting native palette STP modes"
			if not effect.get("palette_index") is int or effect.palette_index != expected.palette_index: return "native CLUT index differs from its source queue slot"
		elif not effect.get("slot") is int or effect.slot != 26: return "native tpage upload does not target slot 0x1A"
	return ""

static func _owners(value: Dictionary) -> bool:
	return value.get("scratch_owner") is String and not value.scratch_owner.is_empty() and value.scratch_owner.length() <= 256 and value.get("vram_owner") is String and not value.vram_owner.is_empty() and value.vram_owner.length() <= 256 and value.get("vram_words") is PackedByteArray and value.vram_words.size() == VRAM_BYTES

static func _mode2_stp(source: PackedByteArray) -> PackedByteArray:
	# palette_load_vram mode2: first120 pairs clear STP; last8 pairs set it
	# on non-black RGB, preserving the existing STP bit of a black texel.
	var bytes := source.duplicate()
	for index in range(256):
		var word := bytes.decode_u16(index*2)
		if index < 240: word &= 0x7fff
		elif (word & 0x7fff) != 0: word |= 0x8000
		bytes.encode_u16(index*2,word)
	return bytes

static func _region(vram: PackedByteArray, rect: Rect2i) -> PackedByteArray:
	var bytes := PackedByteArray()
	var row_bytes := rect.size.x*2
	for row in rect.size.y:
		var offset := ((rect.position.y+row)*1024+rect.position.x)*2
		bytes.append_array(vram.slice(offset,offset+row_bytes))
	return bytes

static func _reject(message: String) -> Dictionary:
	return {"error":message,"batch_complete":false,"vram_ready":false}
