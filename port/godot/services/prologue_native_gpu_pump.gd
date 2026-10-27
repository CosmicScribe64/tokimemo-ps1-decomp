extends RefCounted
## Finite actor-96 CSR/CLUT word transfers. No rasterizer or whole-VRAM claim.
## Source: frame_render_audio.c::csr_load_vram/palette_load_vram; prologue.c.
const Store = preload("res://services/first_arrival_media_store.gd")
const VRAM_BYTES := 1024*512*2
var _receipts: Dictionary = {}

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
	if not owned.get("actor_runtime_bytes") is PackedByteArray or not owned.get("scratch_words") is PackedByteArray or Store._sha(owned.actor_runtime_bytes) != receipt.actor_runtime_sha256 or Store._sha(owned.scratch_words) != receipt.scratch_sha256:
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
	bytes.resize(rect.size.x*rect.size.y*2)
	var row_bytes := rect.size.x*2
	for row in rect.size.y:
		var offset := ((rect.position.y+row)*1024+rect.position.x)*2
		for column in row_bytes: bytes[row*row_bytes+column] = vram[offset+column]
	return bytes

static func _reject(message: String) -> Dictionary:
	return {"error":message,"batch_complete":false,"vram_ready":false}
