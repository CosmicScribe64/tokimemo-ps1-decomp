extends SceneTree
const Uploads = preload("res://presentation/prologue_actor_upload_provider.gd")
var checks: int = 0
var failures: int = 0

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr(label)

func _initialize() -> void:
	var upload = Uploads.new()
	var actor := FileAccess.get_file_as_bytes("/work/disc/files/CDROM/PROLODIR/O_01.NBN")
	check(actor.decode_u32(0xa8) == 0x801800d0, "original actor tpage table pointer")
	var scratch := PackedByteArray()
	scratch.resize(512)
	for i in range(256): scratch.encode_u16(i*2, 0x8000 if i & 1 else 0x1234)
	var vram := PackedByteArray()
	vram.resize(1024*512*2)
	var owned := {"scratch_owner":"review-scratch","vram_owner":"review-vram","scratch_words":scratch,"vram_words":vram,"pending_gpu_effects":[]}
	var plan: Dictionary = upload.preflight(actor,owned)
	check(plan.status == "preflight_ok", "actual original plan")
	for key in ["scratch_owner","vram_owner","scratch_words","vram_words","pending_gpu_effects"]:
		var missing: Dictionary = owned.duplicate(true)
		missing.erase(key)
		var before: String = JSON.stringify(missing).sha256_text()
		check(upload.preflight(actor,missing).status == "unsupported", "missing owned field " + key)
		check(upload.execute(plan,actor,missing).status == "unsupported" and JSON.stringify(missing).sha256_text() == before, "execute rejects unchanged missing owner " + key)
	var bad := actor.duplicate()
	bad[0xd0] ^= 1
	check(upload.preflight(bad,owned).status == "unsupported" and upload.execute(plan,bad,owned).status == "unsupported", "actual payload tamper rejected")
	var tampered: Dictionary = plan.duplicate(true)
	tampered.effects[1].source_offset += 0x4490
	check(upload.execute(tampered,actor,owned).status == "unsupported", "double-base palette mutation rejected")
	var before_vram: PackedByteArray = owned.vram_words.duplicate()
	var result: Dictionary = upload.execute(plan,actor,owned)
	check(result.status == "effects_enqueued" and result.vram_ready == false and owned.vram_words == before_vram, "no pump means no VRAM write/readiness")
	check(owned.pending_gpu_effects.size() == 18, "exact source GPU queue count")
	for i in range(17):
		var effect: Dictionary = owned.pending_gpu_effects[i]
		check(effect.source_bytes == actor.slice(effect.source_offset,effect.source_offset+effect.size), "raw ordered source bytes " + str(i))
	for i in range(256):
		var word: int = actor.decode_u16(0x4610+i*2)
		var lum: int = (word&31)*30+((word>>5)&31)*59+((word>>10)&31)*11
		if lum > 0 and lum < 100: lum=100
		var gray: int = lum/100
		var expected: int = (scratch.decode_u16(i*2)&0x8000)|gray|(gray<<5)|(gray<<10)
		check(owned.scratch_words.decode_u16(i*2) == expected, "original gray/STP word " + str(i))
	check(owned.pending_gpu_effects[17].source_bytes == owned.scratch_words, "last upload snapshots gray scratch after CPU")
	print("independent_upload checks=%d failures=%d native_gpu_pump=false" % [checks,failures])
	quit(0 if failures == 0 else 1)
