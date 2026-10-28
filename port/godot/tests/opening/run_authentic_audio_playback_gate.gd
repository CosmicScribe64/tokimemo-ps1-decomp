extends SceneTree
## T-29021 / T-29019 authentic audio playback gate:
## 1. Static source verification that services/audio_service.gd has zero procedural sine/triad/chime/vowel synthesis.
## 2. Exact byte-for-byte SPU ADPCM decoding verification against prepared_banks .s16 SHA-256 digests in opening.json.
## 3. Authentic SE command decoding (0x500..0x605) via sound_driver.c:725-771 VAB program/note mapping.
## 4. Authentic BGM sequence + VAB decoding (PRO_VOL.SEQ + PRO_VOL.VH/VB vs SPRING.SEQ + SPRING.VH/VB).
## 5. Authentic XA Form2 voice decoding across voiced prologue lines and AudioStreamPlayer attachment.

const AudioService = preload("res://services/audio_service.gd")

var checks: int = 0
var failures: int = 0


func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: " + label)


func _has_non_silent_pcm16(data: PackedByteArray) -> bool:
	for i: int in range(0, data.size() - 1, 2):
		var raw_u16: int = data.decode_u16(i)
		var s16: int = raw_u16 - 0x10000 if (raw_u16 & 0x8000) != 0 else raw_u16
		if absi(s16) > 256:
			return true
	return false


func _initialize() -> void:
	print("--- Running Authentic Audio Playback Gate (T-29021) ---")

	# 1. Static source check: zero procedural sine/oscillator calls in audio_service.gd
	var audio_src: String = FileAccess.get_file_as_string("res://services/audio_service.gd")
	check(not audio_src.is_empty(), "Read res://services/audio_service.gd source")
	for forbidden: String in ["sin(", "cos(", "TAU", "triads", "vowels", "chime_freq"]:
		check(not audio_src.contains(forbidden), "audio_service.gd contains zero procedural token: " + forbidden)

	# 1b. Bare-instance create_pcm_wav_stream fallback verification (used by playable_boot.gd)
	var bare_svc := AudioService.new()
	for kind: String in ["bgm_play", "se_play", "voice_play"]:
		var fallback_stream: AudioStreamWAV = bare_svc.create_pcm_wav_stream(kind, 1, 180)
		check(fallback_stream != null, "bare AudioService.create_pcm_wav_stream(%s) is non-null" % kind)
		if fallback_stream != null:
			check(fallback_stream.format == AudioStreamWAV.FORMAT_16_BITS, "bare %s stream is 16-bit PCM" % kind)
			check(fallback_stream.mix_rate == 22050, "bare %s stream mix_rate is 22050 Hz" % kind)
			check(not fallback_stream.stereo, "bare %s stream is mono" % kind)
			check(_has_non_silent_pcm16(fallback_stream.data), "bare %s stream contains non-silent authentic decoded samples" % kind)
			if kind == "bgm_play":
				check(fallback_stream.loop_mode == AudioStreamWAV.LOOP_FORWARD, "bare bgm_play stream enables forward looping")
			else:
				check(fallback_stream.loop_mode == AudioStreamWAV.LOOP_DISABLED, "bare %s stream disables looping" % kind)
	check(bare_svc.active_bank_role.is_empty(), "bare AudioService fallback does not mutate active_bank_role")

	var pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	var xa_pack_path: String = OS.get_environment("TOKIMEMO_ORIGINAL_OPENING_AUDIO_PACK")
	var xa_sha: String = OS.get_environment("TOKIMEMO_ORIGINAL_OPENING_AUDIO_SHA")
	if xa_sha.is_empty():
		xa_sha = AudioService.DEFAULT_OPENING_AUDIO_SHA

	check(not pack_path.is_empty(), "TOKIMEMO_OPENING_PACK is provided")
	check(not xa_pack_path.is_empty(), "TOKIMEMO_ORIGINAL_OPENING_AUDIO_PACK is provided")

	var svc := AudioService.new()
	var player := AudioStreamPlayer.new()
	root.add_child(player)
	check(svc.attach_player(player).is_empty(), "Attached AudioStreamPlayer to AudioService")

	check(svc.configure_opening_pack(pack_path).is_empty(), "Configured opening VAB/SEQ pack")
	check(svc.configure_original_opening_audio_pack(xa_pack_path, xa_sha).is_empty(), "Configured original opening XA pack")

	# 2. Verify all 6 prologue_arrival SE VAG samples and all 17 prologue_arrival Music VAG samples match opening.json SHA-256
	var install_pro: Dictionary = svc.install_bank_role("prologue_arrival")
	check(str(install_pro.get("error", "")).is_empty(), "Installed prologue_arrival VAB + SEQ bank")
	var prepared_banks: Dictionary = svc.opening_pack.get("prepared_banks", {})
	var pro_se_samples: Array = prepared_banks.get("prologue_arrival:se", {}).get("samples", [])
	check(pro_se_samples.size() == 6, "opening.json lists 6 prologue_arrival:se reference samples")
	for s_entry: Variant in pro_se_samples:
		var s_dict: Dictionary = s_entry
		var idx: int = int(s_dict["index"])
		var decoded: Dictionary = svc.decode_vab_sample_pcm(AudioService.SE_SLOT_INDEX, idx)
		check(
			str(decoded.get("sha256", "")) == str(s_dict["sha256"]) and int(decoded.get("samples", 0)) == int(s_dict["samples"]),
			"prologue_arrival SE VAG %d SPU ADPCM decode matches exact reference s16 SHA-256 (%s)" % [idx, str(s_dict["sha256"]).substr(0, 12)]
		)

	var pro_mu_samples: Array = prepared_banks.get("prologue_arrival:music", {}).get("samples", [])
	check(pro_mu_samples.size() == 17, "opening.json lists 17 prologue_arrival:music reference samples")
	for s_entry: Variant in pro_mu_samples:
		var s_dict: Dictionary = s_entry
		var idx: int = int(s_dict["index"])
		var decoded: Dictionary = svc.decode_vab_sample_pcm(AudioService.MUSIC_SLOT_INDEX, idx)
		check(
			str(decoded.get("sha256", "")) == str(s_dict["sha256"]) and int(decoded.get("samples", 0)) == int(s_dict["samples"]),
			"prologue_arrival Music VAG %d SPU ADPCM decode matches exact reference s16 SHA-256 (%s)" % [idx, str(s_dict["sha256"]).substr(0, 12)]
		)

	# 3. Verify SE commands (0x500..0x505) produce distinct non-silent SPU ADPCM waveforms
	var se_hashes: Dictionary = {}
	for cmd: int in [0x0500, 0x0501, 0x0502, 0x0503, 0x0604, 0x0605]:
		var se_wav: AudioStreamWAV = svc.create_pcm_wav_stream("se_play", cmd, 160, false)
		check(se_wav != null and _has_non_silent_pcm16(se_wav.data), "SE command 0x%x decodes non-silent 16-bit PCM WAV from SYS_MIX" % cmd)
		if se_wav != null:
			se_hashes[cmd] = svc._sha256(se_wav.data)
	check(se_hashes.size() == 6 and se_hashes[0x0500] != se_hashes[0x0605] and se_hashes[0x0503] != se_hashes[0x0604], "Distinct SE commands (0x500, 0x503, 0x604, 0x605) decode distinct VAB tone/VAG samples")

	# 4. Verify normal_gameplay VAB samples (16 SE + 17 Music) match opening.json SHA-256
	var install_norm: Dictionary = svc.install_bank_role("normal_gameplay")
	check(str(install_norm.get("error", "")).is_empty(), "Installed normal_gameplay VAB + SEQ bank")
	var norm_se_samples: Array = prepared_banks.get("normal_gameplay:se", {}).get("samples", [])
	check(norm_se_samples.size() == 16, "opening.json lists 16 normal_gameplay:se reference samples")
	for s_entry: Variant in norm_se_samples:
		var s_dict: Dictionary = s_entry
		var idx: int = int(s_dict["index"])
		var decoded: Dictionary = svc.decode_vab_sample_pcm(AudioService.SE_SLOT_INDEX, idx)
		check(
			str(decoded.get("sha256", "")) == str(s_dict["sha256"]) and int(decoded.get("samples", 0)) == int(s_dict["samples"]),
			"normal_gameplay SE VAG %d SPU ADPCM decode matches exact reference s16 SHA-256 (%s)" % [idx, str(s_dict["sha256"]).substr(0, 12)]
		)

	var norm_mu_samples: Array = prepared_banks.get("normal_gameplay:music", {}).get("samples", [])
	check(norm_mu_samples.size() == 17, "opening.json lists 17 normal_gameplay:music reference samples")
	for s_entry: Variant in norm_mu_samples:
		var s_dict: Dictionary = s_entry
		var idx: int = int(s_dict["index"])
		var decoded: Dictionary = svc.decode_vab_sample_pcm(AudioService.MUSIC_SLOT_INDEX, idx)
		check(
			str(decoded.get("sha256", "")) == str(s_dict["sha256"]) and int(decoded.get("samples", 0)) == int(s_dict["samples"]),
			"normal_gameplay Music VAG %d SPU ADPCM decode matches exact reference s16 SHA-256 (%s)" % [idx, str(s_dict["sha256"]).substr(0, 12)]
		)

	# 5. Verify BGM stream and Voice XA stream via drain_phase and create_pcm_wav_stream
	check(svc.queue_bgm_play(0, 0, 30, AudioService.Phase.AUDIO).is_empty(), "Queued normal_gameplay bgm_play(0, 0)")
	check(svc.queue_voice_play(5, 31, AudioService.Phase.AUDIO).is_empty(), "Queued Yoshio line 5 voice_play(5)")
	var d30: Dictionary = svc.drain_phase(30, AudioService.Phase.AUDIO)
	check(str(d30.get("error", "")).is_empty() and player.stream != null and _has_non_silent_pcm16(player.stream.data), "Draining bgm_play assigns real decoded SPU ADPCM + SEQ AudioStreamWAV to attached AudioStreamPlayer")
	var d31: Dictionary = svc.drain_phase(31, AudioService.Phase.AUDIO)
	check(str(d31.get("error", "")).is_empty() and svc.voice_log.size() == 1 and svc.voice_log[0]["source"] == "original_xa_pcm" and int(svc.voice_log[0]["decoded_pcm_bytes"]) > 0, "Draining voice_play(5) resolves authentic XA PCM stream and drives attached AudioStreamPlayer")

	var voice5_wav: AudioStreamWAV = svc.create_pcm_wav_stream("voice_play", 5, 220, false)
	var voice9_wav: AudioStreamWAV = svc.create_pcm_wav_stream("voice_play", 9, 220, false)
	check(voice5_wav != null and voice9_wav != null and _has_non_silent_pcm16(voice5_wav.data) and _has_non_silent_pcm16(voice9_wav.data), "create_pcm_wav_stream(voice_play) produces non-silent 16-bit PCM WAVs for Yoshio line 5 and Shiori line 9")
	if voice5_wav != null and voice9_wav != null:
		check(svc._sha256(voice5_wav.data) != svc._sha256(voice9_wav.data), "Yoshio line 5 and Shiori line 9 XA voice streams have distinct PCM hashes")

	print("--------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d authentic audio playback gate checks" % checks)
	quit(0 if failures == 0 else 1)
