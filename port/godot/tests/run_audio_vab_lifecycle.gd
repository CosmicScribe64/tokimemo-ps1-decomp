extends SceneTree

const AudioService = preload("res://services/audio_service.gd")
const VabMemory = preload("res://services/native_vab_memory.gd")

const EXPECTED_PROLOGUE_SE_HEADER_SHA: String = "c0098d56c30b6ff9989c38c14f3a5667476f8e299312eb1bada4a4c5b413385f"
const EXPECTED_PROLOGUE_MUSIC_HEADER_SHA: String = "5e31ed3bee895d8da18b60db67b47632c3b4bf820118b476ef6df0fd9d21c9a4"
const EXPECTED_NORMAL_SE_HEADER_SHA: String = "f9f160aeadac495ca667e1e504e2652d08afb96c5cc211c22ba2f35c9d308d8e"
const EXPECTED_NORMAL_MUSIC_HEADER_SHA: String = "1113b8ff7c9719c84c316b6f73e4863d1f26e316d32dc750c40c23207e7e33e8"
const PROLOGUE_SE_SAMPLE_BYTES: int = 0x8a30
const PROLOGUE_MUSIC_SAMPLE_BYTES: int = 0x27220
const NORMAL_SE_SAMPLE_BYTES: int = 0xf3c0
const NORMAL_MUSIC_SAMPLE_BYTES: int = 0x26d10

var checks: int = 0
var failures: int = 0


func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL " + label)


func _initialize() -> void:
	print("--- Running Audio Service & Native VAB Lifecycle Test Suite ---")

	var svc: AudioService = AudioService.new()
	var pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	var receipt_path: String = OS.get_environment("TOKIMEMO_WITNESS_RECEIPT")

	check(not pack_path.is_empty(), "TOKIMEMO_OPENING_PACK environment variable is set")
	if not pack_path.is_empty():
		var cfg_err: String = svc.configure_opening_pack(pack_path, receipt_path)
		check(cfg_err.is_empty(), "AudioService configured opening pack and witness receipt: " + cfg_err)

	# 1. Reject bgm_play and se_play before any bank is loaded
	check(svc.queue_bgm_play(0, 0, 0, AudioService.Phase.AUDIO).is_empty(), "bgm_play event can be queued")
	var unready_drain: Dictionary = svc.drain_phase(0, AudioService.Phase.AUDIO)
	check(not str(unready_drain.get("error", "")).is_empty(), "bgm_play fails drain when no bank is installed")
	svc.queue.clear()

	# 2. Queue and drain prologue_arrival bank_load, bgm_play, voice_play, se_play, bgm_stop
	check(svc.queue_bank_load("prologue_arrival", 10, AudioService.Phase.CD).is_empty(), "Queue prologue_arrival bank_load in CD phase")
	check(svc.queue_bgm_play(0, 0, 10, AudioService.Phase.AUDIO).is_empty(), "Queue prologue BGM play (0, 0) in AUDIO phase")
	check(svc.queue_voice_play(0, 11, AudioService.Phase.AUDIO).is_empty(), "Queue prologue line 0 voice_play in AUDIO phase")
	check(svc.queue_se_play(0x0605, 12, AudioService.Phase.AUDIO).is_empty(), "Queue prologue SE 0x605 in AUDIO phase")
	check(svc.queue_bgm_stop(5, 13, AudioService.Phase.AUDIO).is_empty(), "Queue prologue BGM stop in AUDIO phase")

	var cd_drain: Dictionary = svc.drain_phase(10, AudioService.Phase.CD)
	check(str(cd_drain.get("error", "")).is_empty() and cd_drain["events"].size() == 1, "Drained prologue_arrival bank_load in CD phase: " + str(cd_drain.get("error", "")))
	check(svc.active_bank_role == "prologue_arrival", "Active bank role is prologue_arrival")

	if not pack_path.is_empty():
		check(svc.vab_memory.banks[0]["sample_bytes"] == PROLOGUE_SE_SAMPLE_BYTES, "Prologue SE bank (SYS_MIX) sample bytes == 0xc070")
		check(svc.vab_memory.banks[1]["sample_bytes"] == PROLOGUE_MUSIC_SAMPLE_BYTES, "Prologue music bank (PRO_VOL) sample bytes == 0x22ee0")
		check(svc.vab_memory.banks[0]["header_sha256"] == EXPECTED_PROLOGUE_SE_HEADER_SHA, "Prologue SE mutated VAB header matches PS1 hardware witness SHA-256")
		check(svc.vab_memory.banks[1]["header_sha256"] == EXPECTED_PROLOGUE_MUSIC_HEADER_SHA, "Prologue music mutated VAB header matches PS1 hardware witness SHA-256")

	var audio_drain_10: Dictionary = svc.drain_phase(10, AudioService.Phase.AUDIO)
	check(str(audio_drain_10.get("error", "")).is_empty() and svc.bgm_state["playing"] == true and svc.bgm_state["role"] == "prologue_arrival", "Drained prologue bgm_play (0, 0)")

	var audio_drain_11: Dictionary = svc.drain_phase(11, AudioService.Phase.AUDIO)
	check(str(audio_drain_11.get("error", "")).is_empty() and svc.voice_log.size() == 1 and svc.voice_log[0]["voice_id"] == 0, "Drained prologue voice_play (line 0)")

	var audio_drain_12: Dictionary = svc.drain_phase(12, AudioService.Phase.AUDIO)
	check(str(audio_drain_12.get("error", "")).is_empty() and svc.se_log.size() == 1 and svc.se_log[0]["command"] == 0x0605, "Drained prologue se_play (0x605)")

	var audio_drain_13: Dictionary = svc.drain_phase(13, AudioService.Phase.AUDIO)
	check(str(audio_drain_13.get("error", "")).is_empty() and svc.bgm_state["playing"] == false and svc.bgm_state.get("last_stop_command") == 5, "Drained prologue bgm_stop (command 5)")

	# 3. Checkpoint before normal_gameplay bank load and verify rollback
	var snap: Dictionary = svc.checkpoint()

	check(svc.queue_bank_load("normal_gameplay", 20, AudioService.Phase.CD).is_empty(), "Queue normal_gameplay bank_load in CD phase")
	check(svc.queue_bgm_play(0, 0, 20, AudioService.Phase.AUDIO).is_empty(), "Queue normal_gameplay BGM play (0, 0) in AUDIO phase")

	var normal_cd_drain: Dictionary = svc.drain_phase(20, AudioService.Phase.CD)
	check(str(normal_cd_drain.get("error", "")).is_empty(), "Drained normal_gameplay bank_load: " + str(normal_cd_drain.get("error", "")))
	check(svc.active_bank_role == "normal_gameplay", "Active bank role updated to normal_gameplay")

	if not pack_path.is_empty():
		check(svc.vab_memory.banks[0]["sample_bytes"] == NORMAL_SE_SAMPLE_BYTES, "Normal gameplay SE bank (COM_MIX) sample bytes == 0xf3c0")
		check(svc.vab_memory.banks[1]["sample_bytes"] == NORMAL_MUSIC_SAMPLE_BYTES, "Normal gameplay music bank (SPRING) sample bytes == 0x26d10")
		check(svc.vab_memory.banks[0]["header_sha256"] == EXPECTED_NORMAL_SE_HEADER_SHA, "Normal gameplay SE mutated VAB header matches PS1 hardware witness SHA-256")
		check(svc.vab_memory.banks[1]["header_sha256"] == EXPECTED_NORMAL_MUSIC_HEADER_SHA, "Normal gameplay music mutated VAB header matches PS1 hardware witness SHA-256")

	var normal_audio_drain: Dictionary = svc.drain_phase(20, AudioService.Phase.AUDIO)
	check(str(normal_audio_drain.get("error", "")).is_empty() and svc.bgm_state["playing"] == true and svc.bgm_state["role"] == "normal_gameplay", "Drained normal_gameplay bgm_play (0, 0)")

	# 4. Restore checkpoint and verify sound RAM and service state roll back to prologue_arrival
	var restore_err: String = svc.restore(snap)
	check(restore_err.is_empty() and svc.active_bank_role == "prologue_arrival" and svc.bgm_state["playing"] == false, "Restoring AudioService checkpoint rolls back VAB sound RAM and BGM state")
	if not pack_path.is_empty():
		check(svc.vab_memory.banks[0]["header_sha256"] == EXPECTED_PROLOGUE_SE_HEADER_SHA, "Rolled-back VAB slot 0 header matches prologue_arrival witness")

	print("--------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d audio VAB lifecycle checks" % checks)
	quit(0 if failures == 0 else 1)
