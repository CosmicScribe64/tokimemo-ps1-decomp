extends SceneTree
const Rei = preload("res://services/rei_bank_resources.gd")
const Memory = preload("res://services/native_vab_memory.gd")
var failures := 0
func check(ok: bool, label: String) -> void:
	if not ok: failures += 1; printerr(label)
func _initialize() -> void:
	var prepared := Rei.prepare(OS.get_environment("REI_BANK_PACK"))
	if not prepared.error.is_empty(): printerr(prepared.error); quit(1); return
	var memory := Memory.new()
	memory.sound_ram.fill(0x5a)
	memory.banks[1] = {"test_owner":"existing arrival playback"}
	var before := memory.checkpoint()
	check(Rei.publish(memory,prepared).is_empty(),"Rei source bank publication failed")
	check(memory.banks[1] == before.banks[1] and memory.sound_ram.slice(0,Rei.ADDRESS) == before.sound_ram.slice(0,Rei.ADDRESS),"Rei publication changed active arrival bank")
	check(memory.banks[2].address == Rei.ADDRESS and memory.banks[2].identity.body_sha256 == Rei.INPUTS.music_body[1],"Rei slot2 identity changed")
	print("Rei mutated header SHA: ",memory.banks[2].header_sha256)
	var retained := memory.checkpoint()
	prepared.memory.banks[2].identity.body_sha256 = "substituted"
	check(not Rei.publish(memory,prepared).is_empty() and memory.checkpoint() == retained,"Tampered Rei identity metadata accepted")
	prepared.memory.banks[2].identity.body_sha256 = Rei.INPUTS.music_body[1]
	prepared.inputs.sequence[0] ^= 1
	check(not Rei.publish(memory,prepared).is_empty() and memory.checkpoint() == retained,"Tampered native sequence input accepted against valid software memory")
	prepared.inputs.sequence[0] ^= 1
	prepared.memory.sound_ram[Rei.ADDRESS] ^= 1
	check(not Rei.publish(memory,prepared).is_empty() and memory.checkpoint() == retained,"Tampered Rei body was published or mutated active memory")
	if failures == 0: print("PASS: original Rei alternate-bank publication preserves active bank")
	quit(1 if failures else 0)
