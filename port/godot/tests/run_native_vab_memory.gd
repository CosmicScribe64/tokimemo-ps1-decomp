extends SceneTree

const Memory = preload("res://services/native_vab_memory.gd")

var checks: int = 0
var failures: int = 0


func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL " + label)


func fixture() -> Dictionary:
	var header := PackedByteArray()
	header.resize(0xc20)
	for i in 4:
		header[i] = "pBAV".to_ascii_buffer()[i]
	header.encode_u32(4, 7)
	header.encode_u32(12, header.size() + 16)
	header.encode_u16(0x12, 1)
	header.encode_u16(0x14, 1)
	header.encode_u16(0x16, 1)
	header[0x20 + 127 * 16] = 1
	header.encode_u16(0x820 + 20, 127)
	header.encode_u16(0x820 + 22, 1)
	header.encode_u16(0xa20 + 2, 2)

	var body := PackedByteArray()
	body.resize(16)
	body.fill(42)

	return {
		"header": header,
		"body": body,
		"identity": {
			"header_sha256": Memory._sha(header),
			"body_sha256": Memory._sha(body),
		}
	}


func _initialize() -> void:
	var memory := Memory.new()
	var f: Dictionary = fixture()
	var out = memory.install(0, 0x1000, 64, f.header, f.body, f.identity)

	check(out.error.is_empty() and out.installed and out.sample_bytes == 16, "bank raw image actually installed")
	check(memory.sound_ram.slice(0x1000, 0x1010) == f.body, "sound RAM has exact compressed source bytes")
	check(memory.banks[0].programs[127].ordinal == 0 and memory.banks[0].header.decode_u32(0x28 + 127 * 16) == 0, "sparse logical program maps first packed block")
	check(not memory.banks[0].playback_capability, "memory install cannot invent sequence synthesis")

	var before: Dictionary = memory.checkpoint()
	f.body[0] = 7
	check(not memory.install(0, 0x1000, 64, f.header, f.body, f.identity).error.is_empty() and memory.checkpoint() == before, "identity failure preserves installed bank and RAM")

	f = fixture()
	check(not memory.install(0, 0x1000, 8, f.header, f.body, f.identity).error.is_empty() and memory.checkpoint() == before, "slot overflow preserves prior bank")

	memory.sound_ram[0] = 123
	memory.restore(before)
	check(memory.checkpoint() == before, "RAM and bank object rollback")

	var pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	if not pack_path.is_empty():
		var pack: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(pack_path + "/opening.json"))
		for role in ["prologue_arrival", "normal_gameplay"]:
			for half in ["se", "music"]:
				var assets: Dictionary = pack.bank_roles[role].assets
				var h: Dictionary = assets[half + "_header"]
				var b: Dictionary = assets[half + "_body"]
				var header: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-" + half + "_header.bin").slice(0, int(h.size))
				var body: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-" + half + "_body.bin").slice(0, int(b.size))
				var slot: int = 0 if half == "se" else 1
				var result = memory.install(
					slot,
					0x1010 if slot == 0 else 0x11010,
					0x10000 if slot == 0 else 0x28000,
					header,
					body,
					{"header_sha256": h.sha256, "body_sha256": b.sha256}
				)
				check(result.error.is_empty() and result.installed, "actual original bank installation " + role + ":" + half + " " + result.error)

	print(("PASS" if failures == 0 else "FAIL") + ": %d VAB memory checks" % checks)
	quit(0 if failures == 0 else 1)
