extends RefCounted
## Source-shaped cold audio state and first BGM state plan.
## It never mutates AudioService or claims audible playback.

const COLD_PROFILE: String = "desktop-fresh-new-game-cold-prefix.v1"
const COLD_PROOF_SHA256: String = "f4017d9bafd2f1c09c8d13d6d20a4646c7b80c0076b38ea978672da44b25c095"
const PROLOGUE_ROLE: String = "prologue_arrival"
const SE_HEADER_SHA256: String = "3d38d81c115f91918c318d106fad8e03c0f7ed686eeb4f7bc9bb0efe46eef404"
const SE_BODY_SHA256: String = "23df753d5e39259ce010f716707d151cba7a1a8c29fd2ebfdf5a0bf864bb0b86"
const MUSIC_HEADER_SHA256: String = "05f7477864b4ca41a60fdbf943949b9f9a9cd83dcb74088d6529b3d957081032"
const MUSIC_BODY_SHA256: String = "dc7a06a4d0d90c7a21ac2415cc0f78c2c6bf3ef7f207c6443727fd3d69b588b9"
const SE_MUTATED_HEADER_SHA256: String = "c0098d56c30b6ff9989c38c14f3a5667476f8e299312eb1bada4a4c5b413385f"
const MUSIC_MUTATED_HEADER_SHA256: String = "5e31ed3bee895d8da18b60db67b47632c3b4bf820118b476ef6df0fd9d21c9a4"
const SE_SAMPLE_BYTES: int = 0x8a30
const MUSIC_SAMPLE_BYTES: int = 0x27220
const SE_SOUND_ADDRESS: int = 0x1010
const MUSIC_SOUND_ADDRESS: int = 0x11010
const SE_HEADER_BYTES: int = 3104
const MUSIC_HEADER_BYTES: int = 9760
const PROLOGUE_SEQUENCE_SHA256: String = "16feb604805496721c8797d481fd4244dc6b501c8a5f609d46f51a22c3acc70a"
const PROLOGUE_SEQUENCE_BYTES: int = 27624
const SEQUENCE_DEPTH_OFFSET: int = 0x40


static func _error(message: String) -> Dictionary:
	return {"error": message}


static func _sha256(bytes: PackedByteArray) -> String:
	var hash: HashingContext = HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	return hash.finish().hex_encode()


static func _integer_is(context: Dictionary, key: String, expected: int) -> bool:
	var value: Variant = context.get(key)
	return typeof(value) == TYPE_INT and value == expected


static func _json_integer_is(context: Dictionary, key: String, expected: int) -> bool:
	var value: Variant = context.get(key)
	return (value is int or value is float) and is_finite(float(value)) and float(value) == float(expected)


static func _boolean_is(context: Dictionary, key: String, expected: bool) -> bool:
	var value: Variant = context.get(key)
	return typeof(value) == TYPE_BOOL and value == expected


static func _string_is(context: Dictionary, key: String, expected: String) -> bool:
	var value: Variant = context.get(key)
	return value is String and value == expected


static func _integers_are(value: Variant, expected: Array) -> bool:
	if not value is Array or value.size() != expected.size(): return false
	for index in expected.size():
		if not value[index] is int or value[index] != expected[index]: return false
	return true


static func _base_fields_match(context: Dictionary) -> bool:
	return _string_is(context, "profile", COLD_PROFILE) \
		and _string_is(context, "proof_sha256", COLD_PROOF_SHA256) \
		and _integer_is(context, "generation", 1) \
		and _integer_is(context, "request_flags", 0) \
		and _integer_is(context, "xa_clip_start", 0) \
		and _integer_is(context, "xa_clip_end", 0) \
		and _integer_is(context, "xa_next_start", 0) \
		and _integer_is(context, "xa_next_end", 0) \
		and _integer_is(context, "xa_resume_position", 0) \
		and _integer_is(context, "xa_volume", 100) \
		and _integer_is(context, "xa_volume_full", 100) \
		and _integer_is(context, "bgm_fade_accumulator", 0) \
		and _integer_is(context, "bgm_fade_step", 0x80) \
		and _integer_is(context, "bgm_fade_rate", 0xff) \
		and _integer_is(context, "xa_fade_accumulator", 0) \
		and _integer_is(context, "xa_fade_step", 0x80) \
		and _integer_is(context, "xa_fade_rate", 0xff) \
		and _integer_is(context, "se_slot_a", 0) \
		and _integer_is(context, "bgm_slot", 1) \
		and _integer_is(context, "bgm_auxiliary_slot", 2) \
		and _integer_is(context, "se_slot_b", 3) \
		and _integer_is(context, "source_mode", 2) \
		and _integer_is(context, "output_mode", 2) \
		and _boolean_is(context, "mono", false) \
		and _integer_is(context, "reverb_type", 3) \
		and _integer_is(context, "reverb_mode", 3) \
		and _boolean_is(context, "reverb_enabled", true) \
		and _integers_are(context.get("reverb_depth"), [60, 60])


static func cold_context(initialization_profile: String, prior: Dictionary) -> Dictionary:
	if initialization_profile != COLD_PROFILE:
		return _error("audio context requires the accepted fresh cold New Game profile")
	if not prior.is_empty():
		return _error("audio context cannot replace retained or previously initialized state")
	return {
		"error": "",
		"context": {
			"profile": COLD_PROFILE,
			"proof_sha256": COLD_PROOF_SHA256,
			"generation": 1,
			"phase": "cold_initialized",
			"state_flags": 0x10,
			"request_flags": 0,
			"xa_clip_start": 0,
			"xa_clip_end": 0,
			"xa_next_start": 0,
			"xa_next_end": 0,
			"xa_resume_position": 0,
			"xa_volume": 100,
			"xa_volume_full": 100,
			"bgm_volume": 80,
			"bgm_volume_full": 80,
			"se_volume": 90,
			"se_volume_default": 90,
			"bgm_sequence_handle": 0,
			"bgm_sequence_id": -1,
			"bgm_next_sequence_id": -1,
			"bgm_fade_accumulator": 0,
			"bgm_fade_step": 0x80,
			"bgm_fade_rate": 0xff,
			"xa_fade_accumulator": 0,
			"xa_fade_step": 0x80,
			"xa_fade_rate": 0xff,
			"se_slot_a": 0,
			"bgm_slot": 1,
			"bgm_auxiliary_slot": 2,
			"se_slot_b": 3,
			"sequence_pack_set": 0,
			"source_mode": 2,
			"output_mode": 2,
			"mono": false,
			"reverb_type": 3,
			"reverb_mode": 3,
			"reverb_enabled": true,
			"reverb_depth": [60, 60],
			"bank_epoch": 0,
			"active_bank_role": "",
			"bank_ready": false,
			"published_slots": [],
			"planned_start": false,
		}
	}


static func _valid_cold_context(context: Dictionary) -> bool:
	return _base_fields_match(context) \
		and _string_is(context, "phase", "cold_initialized") \
		and _integer_is(context, "state_flags", 0x10) \
		and _integer_is(context, "bgm_volume", 80) \
		and _integer_is(context, "bgm_volume_full", 80) \
		and _integer_is(context, "se_volume", 90) \
		and _integer_is(context, "se_volume_default", 90) \
		and _integer_is(context, "bgm_sequence_handle", 0) \
		and _integer_is(context, "bgm_sequence_id", -1) \
		and _integer_is(context, "bgm_next_sequence_id", -1) \
		and _integer_is(context, "sequence_pack_set", 0) \
		and _integer_is(context, "bank_epoch", 0) \
		and _string_is(context, "active_bank_role", "") \
		and _boolean_is(context, "bank_ready", false) \
		and context.get("published_slots") is Array and context.get("published_slots").is_empty() \
		and _boolean_is(context, "planned_start", false)


static func _asset_matches(assets: Dictionary, key: String, expected_sha: String,
			expected_size: int) -> bool:
	var record: Variant = assets.get(key)
	return record is Dictionary and _string_is(record, "sha256", expected_sha) \
		and _json_integer_is(record, "size", expected_size)


static func _bank_matches(memory: Object, slot: int, address: int, sample_bytes: int,
		header_bytes: int, source_header_sha: String, source_body_sha: String,
		mutated_header_sha: String) -> bool:
	var banks: Variant = memory.get("banks")
	var ram: Variant = memory.get("sound_ram")
	if not banks is Dictionary or not banks.has(slot) or not ram is PackedByteArray:
		return false
	var bank: Variant = banks[slot]
	if not bank is Dictionary:
		return false
	var identity: Variant = bank.get("identity")
	var header: Variant = bank.get("header")
	if not identity is Dictionary or not header is PackedByteArray:
		return false
	if not _integer_is(bank, "address", address) or not _integer_is(bank, "sample_bytes", sample_bytes):
		return false
	if not _string_is(identity, "header_sha256", source_header_sha) \
			or not _string_is(identity, "body_sha256", source_body_sha):
		return false
	if header.size() != header_bytes or _sha256(header) != mutated_header_sha \
			or not _string_is(bank, "header_sha256", mutated_header_sha):
		return false
	if address < 0 or address + sample_bytes > ram.size():
		return false
	var installed_body: PackedByteArray = ram.slice(address, address + sample_bytes)
	return installed_body.size() == sample_bytes and _sha256(installed_body) == source_body_sha


static func _actual_prologue_bank_is_published(audio_service: Object) -> bool:
	var pack: Variant = audio_service.get("opening_pack")
	if not pack is Dictionary or not pack.get("bank_roles") is Dictionary:
		return false
	var roles: Dictionary = pack["bank_roles"]
	if not roles.has(PROLOGUE_ROLE):
		return false
	var role_record: Variant = roles[PROLOGUE_ROLE]
	if not role_record is Dictionary or not _json_integer_is(role_record, "bank", 0) \
			or not _boolean_is(role_record, "playback_capability", false) \
			or not _string_is(role_record, "source_request", "prologue_arrival_bank_start"):
		return false
	var assets: Variant = role_record.get("assets")
	if not assets is Dictionary or not _asset_matches(assets, "se_header", SE_HEADER_SHA256, SE_HEADER_BYTES) \
			or not _asset_matches(assets, "se_body", SE_BODY_SHA256, SE_SAMPLE_BYTES) \
			or not _asset_matches(assets, "music_header", MUSIC_HEADER_SHA256, MUSIC_HEADER_BYTES) \
			or not _asset_matches(assets, "music_body", MUSIC_BODY_SHA256, MUSIC_SAMPLE_BYTES):
		return false
	if not audio_service.get("active_bank_role") is String or audio_service.get("active_bank_role") != PROLOGUE_ROLE:
		return false
	var ready: Variant = audio_service.get("bank_ready")
	if not ready is Dictionary or not _boolean_is(ready, PROLOGUE_ROLE, true):
		return false
	var memory: Variant = audio_service.get("vab_memory")
	if not memory is Object:
		return false
	return _bank_matches(memory, 0, 0x1010, SE_SAMPLE_BYTES, SE_HEADER_BYTES,
		SE_HEADER_SHA256, SE_BODY_SHA256, SE_MUTATED_HEADER_SHA256) \
		and _bank_matches(memory, 1, 0x11010, MUSIC_SAMPLE_BYTES, MUSIC_HEADER_BYTES,
		MUSIC_HEADER_SHA256, MUSIC_BODY_SHA256, MUSIC_MUTATED_HEADER_SHA256)


static func bank_publication(before: Dictionary, actual_audio_service: Object,
		role: String, epoch: int) -> Dictionary:
	if not _valid_cold_context(before) or actual_audio_service == null \
			or role != PROLOGUE_ROLE or epoch != int(before.get("bank_epoch", -1)) + 1:
		return _error("bank publication requires the current cold context, prologue role and next epoch")
	if actual_audio_service.get("native_audio_context") != before:
		return _error("AudioService does not own the supplied cold context")
	if not _actual_prologue_bank_is_published(actual_audio_service):
		return _error("prologue bank bytes are not the verified actual VAB publication")
	var after: Dictionary = before.duplicate(true)
	after["state_flags"] = int(after["state_flags"]) | (0x10000 << 0) | (0x10000 << 1)
	after["phase"] = "prologue_bank_published"
	after["bank_epoch"] = epoch
	after["active_bank_role"] = role
	after["bank_ready"] = true
	after["published_slots"] = [0, 1]
	return {"error": "", "context": after}


static func _valid_published_context(context: Dictionary) -> bool:
	return _base_fields_match(context) \
		and _string_is(context, "phase", "prologue_bank_published") \
		and _integer_is(context, "state_flags", 0x30010) \
		and _integer_is(context, "bank_epoch", 1) \
		and _integer_is(context, "bgm_volume", 80) \
		and _integer_is(context, "bgm_volume_full", 80) \
		and _integer_is(context, "se_volume", 90) \
		and _integer_is(context, "se_volume_default", 90) \
		and _integer_is(context, "bgm_sequence_handle", 0) \
		and _integer_is(context, "bgm_sequence_id", -1) \
		and _integer_is(context, "bgm_next_sequence_id", -1) \
		and _integer_is(context, "sequence_pack_set", 0) \
		and _string_is(context, "active_bank_role", PROLOGUE_ROLE) \
		and _boolean_is(context, "bank_ready", true) \
		and _integers_are(context.get("published_slots"), [0, 1]) \
		and _boolean_is(context, "planned_start", false)


static func start_plan(before: Dictionary, verified_original_sequence: PackedByteArray,
		bank: int, sequence: int) -> Dictionary:
	if not _valid_published_context(before) or bank != 0 or sequence != 0:
		return _error("first BGM plan requires the published prologue bank and sequence zero")
	if verified_original_sequence.size() != PROLOGUE_SEQUENCE_BYTES \
			or _sha256(verified_original_sequence) != PROLOGUE_SEQUENCE_SHA256 \
			or verified_original_sequence.size() <= SEQUENCE_DEPTH_OFFSET:
		return _error("sequence bytes differ from the verified original PRO_VOL pack")
	var depth: int = int(verified_original_sequence[SEQUENCE_DEPTH_OFFSET + sequence])
	if depth != 60:
		return _error("first prologue sequence reverb byte differs from the cold source profile")
	var after: Dictionary = before.duplicate(true)
	after["state_flags"] = (int(after["state_flags"]) | 0x100) & ~0x1000 & ~0x100000
	after["request_flags"] = int(after["request_flags"]) & ~0x3
	after["bgm_volume"] = 93
	after["bgm_volume_full"] = 93
	after["bgm_sequence_handle"] = 0
	after["bgm_sequence_id"] = sequence
	after["bgm_next_sequence_id"] = -1
	after["sequence_pack_set"] = bank
	after["reverb_depth"] = [depth, depth]
	after["phase"] = "first_bgm_start_plan"
	after["planned_start"] = true
	after["planned_bank"] = bank
	after["planned_sequence"] = sequence
	return {"error": "", "before": before.duplicate(true), "after": after,
		"physical_playback_claimed": false}
