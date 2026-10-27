extends SceneTree
## Source-state tests. No audio device or synthesized playback is involved.

const Context = preload("res://services/first_arrival_audio_context.gd")
const AudioService = preload("res://services/audio_service.gd")

var checks: int = 0
var failures: int = 0


func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ", label)


func _initialize() -> void:
	var profile: String = Context.COLD_PROFILE
	var rejected: Dictionary = Context.cold_context("options-visited.v1", {})
	check(not str(rejected.get("error", "")).is_empty(), "options route cannot initialize the cold context")
	for prior: Dictionary in [{"history": 1}, {"load": true}, {"retained_session": true}]:
		rejected = Context.cold_context(profile, prior)
		check(not str(rejected.get("error", "")).is_empty(), "retained or loaded state cannot be replaced by cold initialization")
	var initialized: Dictionary = Context.cold_context(profile, {})
	check(initialized.get("error") == "", "fresh profile initializes source audio state")
	if not initialized.get("error", "").is_empty():
		_finish()
		return
	var cold: Dictionary = initialized["context"]
	check(cold.state_flags == 0x10 and cold.request_flags == 0 and cold.bgm_volume == 80 \
		and cold.bgm_volume_full == 80 and cold.se_volume == 90 and cold.se_volume_default == 90,
		"cold state uses source flags and decimal volume values")
	check(cold.source_mode == 2 and cold.output_mode == 2 and cold.mono == false \
		and cold.reverb_type == 3 and cold.reverb_mode == 3 and cold.reverb_enabled \
		and cold.reverb_depth == [60, 60], "cold output and reverb state is source initialized")
	check(cold.bgm_sequence_handle == 0 and cold.bgm_sequence_id == -1 \
		and cold.bgm_next_sequence_id == -1 and cold.se_slot_a == 0 \
		and cold.bgm_slot == 1 and cold.bgm_auxiliary_slot == 2 \
		and cold.se_slot_b == 3 and cold.sequence_pack_set == 0,
		"cold sequence handles and slot ownership match source initialization")
	check(cold.published_slots.is_empty() and not cold.bank_ready \
		and not cold.planned_start, "private setup does not mark a bank or playback ready")

	var pack_root: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	check(not pack_root.is_empty() and pack_root.is_absolute_path(), "actual original opening pack is mounted read-only")
	if pack_root.is_empty():
		_finish()
		return
	var audio := AudioService.new()
	var error: String = audio.configure_opening_pack(pack_root)
	check(error.is_empty(), "actual original audio pack configures: " + error)
	if not error.is_empty():
		_finish()
		return
	audio.native_audio_context = cold.duplicate(true)
	check(audio.prepare_opening_bank_resources().is_empty(), "actual VAB bank resources prepare transactionally")
	var unavailable: Dictionary = Context.bank_publication(cold, audio, "prologue_arrival", 1)
	check(not str(unavailable.get("error", "")).is_empty(), "private bank preparation cannot publish source-ready flags")
	check(audio.publish_prepared_bank("prologue_arrival").is_empty(), "actual prologue VAB memories publish")
	var missing_flag: Dictionary = cold.duplicate(true)
	missing_flag.erase("state_flags")
	check(not Context.bank_publication(missing_flag, audio, "prologue_arrival", 1).get("error", "").is_empty(),
		"missing source state flags reject publication")
	var wrong_flag_type: Dictionary = cold.duplicate(true)
	wrong_flag_type["state_flags"] = 16.0
	check(not Context.bank_publication(wrong_flag_type, audio, "prologue_arrival", 1).get("error", "").is_empty(),
		"non-integer source flags reject publication")
	var wrong_role: Dictionary = Context.bank_publication(cold, audio, "normal_gameplay", 1)
	check(not str(wrong_role.get("error", "")).is_empty(), "unrelated bank role cannot claim cold-prefix readiness")
	var wrong_epoch: Dictionary = Context.bank_publication(cold, audio, "prologue_arrival", 2)
	check(not str(wrong_epoch.get("error", "")).is_empty(), "bank publication requires the next exact epoch")
	var publication: Dictionary = Context.bank_publication(cold, audio, "prologue_arrival", 1)
	check(publication.get("error") == "", "actual original and mutated VAB identities authorize logical bank completion")
	if not publication.get("error", "").is_empty():
		_finish()
		return
	var published: Dictionary = publication["context"]
	check(cold.state_flags == 0x10 and cold.published_slots.is_empty(), "publication plan does not mutate prior context")
	check(published.state_flags == 0x30010 and published.published_slots == [0, 1] \
		and published.bank_ready and published.bank_epoch == 1,
		"only verified actual VAB publication adds both native slot-ready flags")
	check(audio.native_audio_context == cold, "publication helper leaves AudioService context for the caller to commit")

	var memory_snapshot: Dictionary = audio.vab_memory.checkpoint()
	var banks: Dictionary = audio.vab_memory.banks.duplicate(true)
	var slot_one: Dictionary = banks[1]
	var music_header: PackedByteArray = slot_one["header"]
	music_header[0] = music_header[0] ^ 1
	slot_one["header"] = music_header
	banks[1] = slot_one
	audio.vab_memory.banks = banks
	check(not Context.bank_publication(cold, audio, "prologue_arrival", 1).get("error", "").is_empty(),
		"mutated actual VAB header bytes reject publication")
	check(audio.vab_memory.restore(memory_snapshot).is_empty(), "header mutation fixture restores actual VAB memory")
	var ram: PackedByteArray = audio.vab_memory.sound_ram.duplicate()
	ram[0x1010] = ram[0x1010] ^ 1
	audio.vab_memory.sound_ram = ram
	check(not Context.bank_publication(cold, audio, "prologue_arrival", 1).get("error", "").is_empty(),
		"mutated actual VAB body bytes reject publication")
	check(audio.vab_memory.restore(memory_snapshot).is_empty(), "body mutation fixture restores actual VAB memory")

	var sequence: PackedByteArray = FileAccess.get_file_as_bytes(pack_root.path_join("reads/prologue_arrival-sequence.bin"))
	var plan: Dictionary = Context.start_plan(published, sequence, 0, 0)
	check(plan.get("error") == "", "original prologue sequence authorizes a pure first-start plan")
	if plan.get("error", "").is_empty():
		var after: Dictionary = plan["after"]
		check(after.state_flags == 0x30110 and after.request_flags == 0,
			"first-start state applies source BGM flags and request clears")
		check(after.bgm_volume == 93 and after.bgm_volume_full == 93 \
			and after.bgm_sequence_id == 0 and after.bgm_next_sequence_id == -1 \
			and after.bgm_sequence_handle == 0,
			"first-start plan applies sourced sequence and gain state")
		check(after.reverb_depth == [60, 60] and plan.physical_playback_claimed == false,
			"sequence byte at 0x40 sets logical reverb depth without claiming physical playback")
		check(published.state_flags == 0x30010 and not published.planned_start \
			and audio.bgm_state.playing == false,
			"start plan leaves published context and AudioService playback unchanged")
	var bad_sequence: PackedByteArray = sequence.duplicate()
	bad_sequence[Context.SEQUENCE_DEPTH_OFFSET] = bad_sequence[Context.SEQUENCE_DEPTH_OFFSET] ^ 1
	check(not Context.start_plan(published, bad_sequence, 0, 0).get("error", "").is_empty(),
		"altered original sequence byte rejects first-start plan")
	check(not Context.start_plan(published, sequence, 1, 0).get("error", "").is_empty()
		and not Context.start_plan(published, sequence, 0, 1).get("error", "").is_empty(),
		"unsupported bank or sequence cannot use the first-start source contract")
	var malformed: Dictionary = published.duplicate(true)
	malformed.erase("request_flags")
	check(not Context.start_plan(malformed, sequence, 0, 0).get("error", "").is_empty(),
		"missing request flags reject first-start planning")
	malformed = published.duplicate(true)
	malformed["reverb_depth"] = [60.0, 60.0]
	check(not Context.start_plan(malformed, sequence, 0, 0).get("error", "").is_empty(),
		"non-integer reverb depth rejects first-start planning")
	_finish()


func _finish() -> void:
	print("FIRST_ARRIVAL_AUDIO_CONTEXT checks=%d failures=%d" % [checks, failures])
	quit(0 if failures == 0 else 1)
