extends RefCounted
## Derive playback ownership from the committed bank and real service memory.
const Backend = preload("res://bgm_backend.gd")
const SourcePack = preload("res://bgm_source_pack.gd")
const AudioService = preload("res://services/audio_service.gd")
const State = preload("res://rules/new_game_route_state.gd")

static func derive(state: State, audio: AudioService, original: Dictionary, flags: Variant, reverb: Dictionary) -> Dictionary:
	if not original.get("error") is String or not original["error"].is_empty() or not original.get("identities") is Dictionary or original["identities"] != Backend.ORIGINAL_IDENTITIES:
		return {"error": "validated original BGM source role required"}
	var raw_bank: Variant = state.opening.get("audio_bank", {})
	if not raw_bank is Dictionary:
		return {"error": "live bank record requires a dictionary"}
	var bank: Dictionary = raw_bank
	if not bank.get("epoch") is int or bank["epoch"] < 1 or not bank.get("ready") is bool or not bank["ready"] or not bank.get("role") is String or bank["role"] != "prologue_arrival":
		return {"error": "live first-arrival bank epoch is not ready"}
	if audio.active_bank_role != "prologue_arrival" or not audio.bank_ready.get("prologue_arrival") is bool or not audio.bank_ready["prologue_arrival"]:
		return {"error": "AudioService has not published the first-arrival bank"}
	for slot: int in range(2):
		var raw_installed: Variant = audio.vab_memory.banks.get(slot, {})
		if not raw_installed is Dictionary:
			return {"error": "live installed bank record requires a dictionary"}
		var installed: Dictionary = raw_installed
		if not installed.get("identity") is Dictionary:
			return {"error": "live installed bank identity requires a dictionary"}
		var identity: Dictionary = installed["identity"]
		var prefix: String = "SE" if slot == 0 else "music"
		var address: int = 0x1010 if slot == 0 else 0x11010
		var size: int = SourcePack.SIZES[2 if slot == 0 else 4]
		var mutated: String = "c0098d56c30b6ff9989c38c14f3a5667476f8e299312eb1bada4a4c5b413385f" if slot == 0 else "5e31ed3bee895d8da18b60db67b47632c3b4bf820118b476ef6df0fd9d21c9a4"
		if not installed.get("address") is int or not installed.get("sample_bytes") is int or not installed.get("header_sha256") is String or not identity.get("header_sha256") is String or not identity.get("body_sha256") is String or not installed.get("header") is PackedByteArray:
			return {"error": "live installed bank fields have invalid types"}
		if installed["address"] != address or installed["sample_bytes"] != size or installed["header_sha256"] != mutated or identity["header_sha256"] != Backend.ORIGINAL_IDENTITIES[prefix + "_header_sha256"] or identity["body_sha256"] != Backend.ORIGINAL_IDENTITIES[prefix + "_body_sha256"]:
			return {"error": "live installed bank metadata differs from original source"}
		if SourcePack.sha(installed.get("header", PackedByteArray())) != mutated or SourcePack.sha(audio.vab_memory.sound_ram.slice(address, address + size)) != Backend.ORIGINAL_IDENTITIES[prefix + "_body_sha256"]:
			return {"error": "live installed bank bytes differ from original source"}
	var owner: Dictionary = Backend.ORIGINAL_IDENTITIES.duplicate(true)
	owner.merge({"role": bank["role"], "epoch": bank["epoch"], "ready": bank["ready"], "load_set": 0, "sequence_pack_set": 0, "sequence": 0, "music_slot": 1, "SE_slot": 0, "state_flags": flags, "reverb_preset": reverb.get("preset"), "reverb_depth": reverb.get("depth"), "reverb_enabled": reverb.get("enabled")})
	var error: String = Backend.validate_owner(owner, bank["epoch"])
	return {"error": error, "owner": owner} if error.is_empty() else {"error": error}
