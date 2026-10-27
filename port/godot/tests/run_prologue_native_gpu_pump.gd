extends SceneTree
## Actual original bytes + software word-transfer checks. Queue/route fixture,
## not normal-start, physical GPU completion or native pixel-parity evidence.
const Store = preload("res://services/first_arrival_media_store.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Cold = preload("res://flow/cold_no_card_scratch.gd")
var checks := 0
var failures := 0
func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ",label)

func pending_fixture(store: Store, scratch: PackedByteArray) -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	var operations := store.upload_requests()
	for index in range(18):
		var operation: Dictionary = operations[index if index < 17 else 18]
		var rect: Dictionary = operation.destination
		var item := {"id":operation.id,"operation":operation.operation,"size":rect.width_words*rect.height*2,
			"destination":[rect.x_words,rect.y,rect.width_words,rect.height],"queue_sequence":index}
		if index < 17:
			item["source_bytes"] = store.upload_source(operation.id).bytes
			item["source_offset"] = operation.source.offset
			item["source_pointer"] = operation.source.address
		else:
			item["source_bytes"] = scratch.duplicate()
			item["source"] = operation.source.duplicate(true)
		if index == 0: item["slot"] = 26
		else:
			item["palette_index"] = operation.palette_index
			item["mode"] = operation.mode # Explicit source fact, never a pump default.
		result.append(item)
	return result

func digest(owned: Dictionary) -> String:
	return JSON.stringify([Store._sha(owned.vram_words),Store._sha(owned.scratch_words),Store._sha(owned.actor_runtime_bytes),owned.pending_gpu_effects,owned.get("vram_known_mask")]).sha256_text()

func _initialize() -> void:
	var store := Store.new()
	check(store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")).is_empty(),"original native media authenticates")
	var route := {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false}
	check(not Cold.initialize({}).error.is_empty(),"no generic scratch default")
	for key: String in ["options_visited","retained_game","memory_card_load"]:
		var other := route.duplicate(true); other[key] = true
		check(not Cold.initialize(other).error.is_empty(),"cold initializer rejects excluded route: "+key)
	var cold := Cold.initialize(route)
	check(cold.error.is_empty() and cold.scratch_words.size() == 512 and cold.known_mask.count(255) == 512,"explicit source cold-route scratch initialized as known")
	var initial: PackedByteArray = cold.scratch_words
	check(initial.decode_u16(0) == 0 and initial.decode_u16(2) == 0x65fb and initial.decode_u16(30) == 0x65fb and initial.slice(32).count(0) == 480,"source 16-color bar affects exactly its first32 bytes")
	if failures != 0:
		quit(1)
		return
	# An explicit post-CPU scratch snapshot is supplied by this queue fixture.
	# The GPU pump does not invent or recompute the producer's grayscale step.
	var scratch: PackedByteArray = initial.duplicate()
	for index in range(256): scratch.encode_u16(index*2,0x8001 if index%2 == 0 else 0x0000)
	var vram := PackedByteArray(); vram.resize(Pump.VRAM_BYTES); vram.fill(0xa5)
	var owned := {"scratch_owner":"explicit-pump-fixture-scratch","vram_owner":"explicit-pump-fixture-vram",
		"scratch_words":scratch,"vram_words":vram,"actor_runtime_bytes":store.transfer(Store.ACTOR_ID).bytes,
		"pending_gpu_effects":pending_fixture(store,scratch)}
	var pump := Pump.new()
	for defect: String in ["missing_mode","wrong_mode_type","wrong_destination","wrong_source_offset","swapped_order","incomplete_queue","wrong_gray_source"]:
		var bad := owned.duplicate(true)
		match defect:
			"missing_mode": bad.pending_gpu_effects[1].erase("mode")
			"wrong_mode_type": bad.pending_gpu_effects[1].mode = {}
			"wrong_destination": bad.pending_gpu_effects[1].destination[1] = 0
			"wrong_source_offset": bad.pending_gpu_effects[2].source_offset += 512
			"swapped_order":
				var first: Dictionary = bad.pending_gpu_effects[0]
				bad.pending_gpu_effects[0] = bad.pending_gpu_effects[1]
				bad.pending_gpu_effects[1] = first
			"incomplete_queue": bad.pending_gpu_effects.pop_back()
			"wrong_gray_source": bad.pending_gpu_effects[17].source_bytes[0] ^= 1
		var before := digest(bad)
		var rejected := pump.execute(store,"1".repeat(64),bad)
		check(not rejected.error.is_empty() and digest(bad) == before,"invalid pump input rejects without source/VRAM/queue mutation: "+defect)
	var result := pump.execute(store,"1".repeat(64),owned)
	check(result.error.is_empty(),"actual word-copy/STP pump executes")
	if not result.error.is_empty():
		quit(1)
		return
	check(result.receipt.batch_complete and result.receipt.gpu_operations == 18 and not result.receipt.vram_ready and owned.pending_gpu_effects.is_empty(),"receipt proves18 executed transfers without claiming whole VRAM readiness")
	check(owned.vram_known_mask.count(255) == 25088 and owned.vram_words[0] == 0xa5,"only exact actor/tpage/CLUT byte regions become known or change")
	check(Pump._region(owned.vram_words,Rect2i(640,256,64,128)) == store.upload_source("actor96.tpage").bytes,"tpage source appears at native slot26 rectangle")
	var original_palette: PackedByteArray = store.upload_source("actor96.palette.0").bytes
	var expected_palette := original_palette.duplicate()
	for word in range(256):
		var value := original_palette.decode_u16(word*2)
		if word < 240: value &= 0x7fff
		elif (value & 0x7fff) != 0: value |= 0x8000
		expected_palette.encode_u16(word*2,value)
	check(Pump._region(owned.vram_words,Rect2i(0,496,256,1)) == expected_palette and owned.actor_runtime_bytes.slice(0x4490,0x4690) == expected_palette,"mode2 palette pixels and source-RAM STP writeback match source")
	check(owned.scratch_words.decode_u16(0) == 1 and owned.scratch_words.decode_u16(480) == 0x8001 and Pump._region(owned.vram_words,Rect2i(0,493,256,1)) == owned.scratch_words,"gray CLUT applies mode2 to owned scratch and uploads that exact result")
	check(pump.batch_ready("1".repeat(64),owned).is_empty(),"batch readiness verifies actual destination/source readback")
	var before_repeat := digest(owned)
	check(not pump.execute(store,"1".repeat(64),owned).error.is_empty() and digest(owned) == before_repeat,"executed batch cannot replay effects")
	owned.vram_words[(256*1024+640)*2] ^= 1
	check(not pump.batch_ready("1".repeat(64),owned).is_empty(),"changed destination invalidates executed receipt")
	owned.vram_words[(256*1024+640)*2] ^= 1
	owned.vram_known_mask[(256*1024+640)*2] = 0
	check(not pump.batch_ready("1".repeat(64),owned).is_empty(),"lost destination ownership invalidates receipt")
	pump.reset_session()
	check(not pump.batch_ready("1".repeat(64),owned).is_empty(),"session reset clears native completion receipts")
	print("PASS: %d native GPU word-pump checks" % checks if failures == 0 else "FAIL: %d/%d GPU pump checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
