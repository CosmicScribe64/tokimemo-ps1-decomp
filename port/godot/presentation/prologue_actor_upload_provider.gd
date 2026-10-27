class_name PrologueActorUploadProvider
extends RefCounted
## Ordered actor-96 upload plan. Preflight needs the caller's actual runtime
## scratch bytes; execution records enqueue/CPU order without claiming a VRAM pump.

const ACTOR_SHA := "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"
const ACTOR_SIZE := 26624
const GRAY_SOURCE_OFFSET := 0x4610
const GRAY_DESTINATION := 0x800e78bc

func preflight(actor_bytes: PackedByteArray, owned_runtime_state: Dictionary) -> Dictionary:
	if actor_bytes.size() != ACTOR_SIZE or _sha(actor_bytes) != ACTOR_SHA:
		return _reject("actor bytes do not match the authenticated O_01.NBN transfer")
	if not _owned_scratch(owned_runtime_state):
		return _reject("actual runtime owner and 512-byte scratch_words are required")
	if not owned_runtime_state.get("vram_words") is PackedByteArray or owned_runtime_state["vram_words"].size() != 1024 * 512 * 2:
		return _reject("an explicitly owned 1024x512x16-bit VRAM word buffer is required")
	if not owned_runtime_state.get("pending_gpu_effects") is Array:
		return _reject("the caller-owned pending GPU queue is required")
	var queue: Array[Dictionary] = []
	queue.append({"id":"actor96.tpage","phase":"enqueue","operation":"load_tpage_buf_lock",
		"slot":0x1a,"source_pointer":0x801800d0,"source_offset":0xd0,"size":16384,
		"destination":[640,256,64,128]})
	for row in range(16):
		var effective_offset: int = 0x4490 + row * 512
		queue.append({"id":"actor96.palette."+str(row),"phase":"enqueue","operation":"load_palette",
			"palette_index":16+row,"stp_mode":2,"buffer_lock":1,"source_row":row,
			"native_source_pointer":0x80184490,"payload_row":0,"source_pointer":0x80184490+row*512,
			"source_offset":effective_offset,"size":512,"destination":[0,496+row,256,1]})
	queue.append({"id":"actor96.gray","phase":"cpu_then_enqueue","operation":"color_palette_to_gray",
		"source_pointer":0x80184610,"source_offset":GRAY_SOURCE_OFFSET,"size":512,
		"destination_runtime_address":GRAY_DESTINATION,"preserve_destination_stp":true})
	queue.append({"id":"actor96.gray_palette","phase":"enqueue_after_gray_cpu","operation":"load_palette",
		"palette_index":13,"stp_mode":2,"buffer_lock":1,"source_row":0,"payload_row":0,
		"native_source_pointer":GRAY_DESTINATION,"source":{"after_operation":"actor96.gray","runtime_address":GRAY_DESTINATION,"size":512},
		"destination":[0,493,256,1]})
	return {"status":"preflight_ok","effect_count":queue.size(),"effects":queue,
		"actor_sha256":ACTOR_SHA,"scratch_owner":owned_runtime_state["scratch_owner"],
		"vram_owner":owned_runtime_state["vram_owner"],"vram_ready":false,
		"plan_sha256":_plan_hash(queue, str(owned_runtime_state["scratch_owner"]),
			str(owned_runtime_state["vram_owner"]), ACTOR_SHA),
		"limitation":"effects are ordered and validated; GPU/CSR pump completion is not implemented"}

func execute(plan: Dictionary, actor_bytes: PackedByteArray,
		owned_runtime_state: Dictionary) -> Dictionary:
	if plan.get("status") != "preflight_ok" or typeof(plan.get("effect_count")) != TYPE_INT or plan["effect_count"] != 19:
		return _reject("a successful 19-effect preflight plan is required")
	# Re-derive the complete plan before any writes. Hashes detect accidental
	# mutation; only exact source-derived fields establish executable semantics.
	var expected: Dictionary = preflight(actor_bytes, owned_runtime_state)
	if expected.get("status") != "preflight_ok" or plan != expected:
		return _reject("runtime ownership or source upload plan changed after preflight")
	var scratch: PackedByteArray = owned_runtime_state["scratch_words"]
	var pending: Array = owned_runtime_state["pending_gpu_effects"].duplicate(true)
	var trace: Array[Dictionary] = []
	for index in range(17):
		var queued_effect: Dictionary = plan["effects"][index].duplicate(true)
		queued_effect["source_bytes"] = actor_bytes.slice(int(queued_effect["source_offset"]),
			int(queued_effect["source_offset"]) + int(queued_effect["size"]))
		queued_effect["queue_sequence"] = pending.size()
		pending.append(queued_effect)
		trace.append({"order":index,"id":queued_effect["id"],"action":"enqueue_gpu_effect"})
	var output := _gray_palette(actor_bytes.slice(GRAY_SOURCE_OFFSET, GRAY_SOURCE_OFFSET + 512), scratch)
	owned_runtime_state["scratch_words"] = output
	trace.append({"order":17,"id":"actor96.gray","action":"read_original_nbn_and_write_owned_scratch"})
	var final_effect: Dictionary = plan["effects"][18].duplicate(true)
	final_effect["source_bytes"] = output.duplicate()
	final_effect["queue_sequence"] = pending.size()
	pending.append(final_effect)
	owned_runtime_state["pending_gpu_effects"] = pending
	trace.append({"order":18,"id":"actor96.gray_palette","action":"enqueue_after_gray_cpu"})
	return {"status":"effects_enqueued","trace":trace,"effect_count":19,
		"scratch_sha256":_sha(output),"scratch_owner":owned_runtime_state["scratch_owner"],
		"queued_gpu_effect_count":18,
		"vram_ready":false,"gpu_completion":"pending_external_native_pump"}

static func _gray_palette(source: PackedByteArray, destination: PackedByteArray) -> PackedByteArray:
	var result := destination.duplicate()
	for i in range(0, 512, 2):
		var src: int = source.decode_u16(i)
		var dst: int = destination.decode_u16(i)
		var r: int = src & 31
		var g: int = (src >> 5) & 31
		var b: int = (src >> 10) & 31
		var lum: int = r * 0x1e + g * 0x3b + b * 0xb
		if lum != 0 and lum < 100: lum = 100
		var gray: int = clampi(lum / 100, 0, 31)
		result.encode_u16(i, (dst & 0x8000) | gray | (gray << 5) | (gray << 10))
	return result

static func _owned_scratch(value: Dictionary) -> bool:
	return value.get("scratch_owner") is String and not str(value.get("scratch_owner", "")).is_empty() \
		and value.get("vram_owner") is String and not str(value.get("vram_owner", "")).is_empty() \
		and value.get("scratch_words") is PackedByteArray and value["scratch_words"].size() == 512 \
		and value.get("vram_words") is PackedByteArray and value["vram_words"].size() == 1024 * 512 * 2 \
		and value.get("pending_gpu_effects") is Array

static func _sha(bytes: PackedByteArray) -> String:
	var context := HashingContext.new()
	if context.start(HashingContext.HASH_SHA256) != OK or context.update(bytes) != OK:
		return ""
	return context.finish().hex_encode()

static func _plan_hash(effects: Variant, scratch_owner: String, vram_owner: String,
		actor_sha: String) -> String:
	return JSON.stringify([effects, scratch_owner, vram_owner, actor_sha]).sha256_text()

static func _reject(message: String) -> Dictionary:
	return {"status":"unsupported","diagnostic":message,"vram_ready":false}
