extends SceneTree
## T-29025 (voiced-prologue-lines-v1): Headless verification of the exact PS1
## getCDlevel() 4-sample ring buffer lip-sync engine across all 4 voiced prologue
## speaker tracks (Yoshio, Girl A & Girl B, Rei Ijuin, Shiori Fujisaki + dynamic
## birthday/blood-type lines 100/101) and the 3 mid-prologue BGM bank load &
## transition contracts (prologue_rei, prologue_shiori, normal_gameplay).

const Lines = preload("res://services/prologue_line_services.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const Bridge = preload("res://services/opening_audio_bridge.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const VoicePlayback = preload("res://services/original_voice_playback.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")

var checks: int = 0
var failures: int = 0


func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: " + label)


func _resolve_path(env_key: String, fallbacks: Array[String]) -> String:
	var from_env: String = OS.get_environment(env_key)
	if not from_env.is_empty() and (DirAccess.dir_exists_absolute(from_env) or FileAccess.file_exists(from_env)):
		return from_env
	for candidate: String in fallbacks:
		if DirAccess.dir_exists_absolute(candidate) or FileAccess.file_exists(candidate):
			return candidate
	return from_env


func _complete_voice(state: State, status: String) -> String:
	var audio: Dictionary = state.opening["audio"]
	var clip: Dictionary = audio["clips"][audio["clip_index"]]
	var event := Session.ServiceEvent.new(state.frame_count, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION)
	event.phase = 0
	event.service = "opening.audio"
	event.payload = {
		"epoch": audio["epoch"],
		"line_id": audio["line_id"],
		"clip_index": audio["clip_index"],
		"request_key": clip["request_key"],
		"status": status,
		"completion_index": int(audio["completion_index"]) + 1,
	}
	var result := Session.FrameResult.new()
	AudioRules.complete(event, state, result)
	return result.error


func _initialize() -> void:
	call_deferred("_run")


func _run() -> void:
	print("--- Running Prologue Voiced Dialogue & Lip-Sync Test Suite ---")

	var text_root: String = _resolve_path("PROLOGUE_TEXT_ROOT", [
		"/workspace/.asset-cache/port/installed-clean-v1/packs/prologue_text",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.asset-cache/port/installed-clean-v1/packs/prologue_text",
	])
	var font_root: String = _resolve_path("PROLOGUE_FONT_ROOT", [
		"/workspace/.asset-cache/port/fonts/native-font-v2",
		"/workspace/.asset-cache/port/installed-clean-v1/packs/font_root",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.asset-cache/port/fonts/native-font-v2",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.asset-cache/port/installed-clean-v1/packs/font_root",
	])
	var audio_root: String = _resolve_path("PROLOGUE_AUDIO_ROOT", [
		"/workspace/.claude/orchestrator/state/T-29021/candidate-v4/.asset-cache/opening-audio-v2",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.claude/orchestrator/state/T-29021/candidate-v4/.asset-cache/opening-audio-v2",
	])
	var opening_pack_root: String = _resolve_path("TOKIMEMO_OPENING_PACK", [
		"/workspace/.asset-cache/port/installed-clean-v1/packs/opening_pack",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.asset-cache/port/installed-clean-v1/packs/opening_pack",
	])
	var witness_receipt: String = _resolve_path("TOKIMEMO_WITNESS_RECEIPT", [
		"/workspace/.asset-cache/port/installed-clean-v1/packs/opening_pack/witness.json",
		"/Users/lila/Documents/Projects/tokimemo-decomp/.asset-cache/port/installed-clean-v1/packs/opening_pack/witness.json",
	])

	var store := Store.new()
	var painter := Painter.new()
	var audio := Audio.new()
	check(store.open(text_root, FileAccess.get_sha256(text_root.path_join("manifest.json"))).is_empty(), "original prologue text pack opens")
	check(painter.open_font(font_root).is_empty(), "original prologue font pack opens")
	check(audio.configure_original_opening_audio_pack(audio_root, FileAccess.get_sha256(audio_root.path_join("opening-audio.json"))).is_empty(), "original opening XA PCM pack opens")
	if not opening_pack_root.is_empty():
		var cfg_err: String = audio.configure_opening_pack(opening_pack_root, witness_receipt if FileAccess.file_exists(witness_receipt) else "")
		check(cfg_err.is_empty(), "opening VAB pack configures cleanly: " + cfg_err)
		check(audio.prepare_opening_bank_resources().is_empty(), "opening VAB bank resources prepare cleanly")

	var lines := Lines.new()
	var prep_err: String = lines.prepare(store, painter, audio)
	check(prep_err.is_empty(), "prologue line services prepare all 45 lines + getCDlevel envelopes: " + prep_err)
	if failures != 0:
		quit(1)
		return

	# =========================================================================
	# 1. Exact Ring Buffer Math & Threshold Verification (prologue.c)
	# =========================================================================
	var math_state := State.new()
	AudioRules.reset_lip_sync(math_state)
	check(math_state.opening["prologue_cd_level_history"] == [0, 0, 0, 0] and math_state.opening["prologue_cd_level_last"] == 0, "reset_lip_sync zeroes 4-sample ring buffer and last level")

	math_state.opening["cd_audio_state_override"] = 4
	# Step 4 frames with synthetic levels [8, 12, 16, 20] -> average = 56 / 4 = 14 -> frame = 1
	var synthetic_levels: Array[int] = [8, 12, 16, 20]
	for idx: int in range(4):
		math_state.frame_count = idx
		var step_out: Dictionary = lines.lip_sync_yoshio(math_state, synthetic_levels[idx])
		check(math_state.opening["prologue_cd_level_last"] == synthetic_levels[idx], "prologue_cd_level_last updated on frame %d" % idx)
		if idx == 3:
			check(step_out["history"] == [8, 12, 16, 20] and step_out["average"] == 14 and step_out["mouth_index"] == 1, "4-sample ring buffer average (8+12+16+20)/4 = 14 -> mouth_index 1")
			check(step_out["actors"][97]["flags"] == 1 and step_out["actors"][97]["unk_18"] == 5, "Yoshio actor 97 unk_18 == 6 - 1 == 5")

	# Push high levels [60, 60, 60, 60] -> average = 60 -> clamp(60/10, 0, 3) == 3
	for idx: int in range(4, 8):
		math_state.frame_count = idx
		var step_out: Dictionary = lines.lip_sync_yoshio(math_state, 60)
		if idx == 7:
			check(step_out["average"] == 60 and step_out["mouth_index"] == 3, "average >= 40 clamps mouth_index to 3")
			check(step_out["actors"][97]["unk_18"] == 3, "Yoshio actor 97 unk_18 == 6 - 3 == 3 at max mouth opening")

	# When checkCD() == 0 (cd_audio_state() & 4 == 0), level is 0 and mouth_index is immediately 0
	math_state.opening["cd_audio_state_override"] = 0
	math_state.frame_count = 8
	var closed_out: Dictionary = lines.lip_sync_yoshio(math_state, 60)
	check(closed_out["cd_level"] == 0 and closed_out["mouth_index"] == 0 and closed_out["actors"][97]["flags"] == 0, "when checkCD() == 0, cd_level is 0, mouth_index is 0, and actor 97 is cleared")
	math_state.opening.erase("cd_audio_state_override")

	# =========================================================================
	# 2. Speaker-Specific Lip-Sync Variants Verification (prologue.c)
	# =========================================================================
	var var_state := State.new()
	AudioRules.reset_lip_sync(var_state)
	var_state.opening["cd_audio_state_override"] = 4
	for f: int in range(4):
		var_state.frame_count = f
		lines.lip_sync_line_11(var_state, 25) # average after 4 frames = 25 -> frame = 2
	var l11: Dictionary = var_state.opening["prologue_lip_sync"]
	check(l11["mouth_index"] == 2 and l11["actors"][97]["unk_18"] == 3 and l11["actors"][98]["unk_18"] == 7, "line 11 sets actor 97 (frame+1=3) and actor 98 (frame+5=7)")

	var_state.frame_count = 4
	var ga_on: Dictionary = lines.lip_sync_girl_a(var_state, 30)
	check(ga_on["actors"][100]["flags"] == 1 and ga_on["actors"][100]["sequence_slot"] == 1 and ga_on["binary_mouth_index"] == 1, "Girl A line 9 active sets actor 100 flags=1 sequence_slot=1")
	var_state.frame_count = 5
	var gb_on: Dictionary = lines.lip_sync_girl_b(var_state, 30)
	check(gb_on["actors"][101]["flags"] == 1 and gb_on["actors"][101]["sequence_slot"] == 2 and gb_on["binary_mouth_index"] == 1, "Girl B line 10 active sets actor 101 flags=1 sequence_slot=2")

	var_state.frame_count = 6
	var rei_on: Dictionary = lines.lip_sync_rei(var_state, 35)
	check(rei_on["actors"][96]["sequence_slot"] == 0 and rei_on["actors"][96]["unk_18"] == rei_on["mouth_index"], "Rei line 12 sets actor 96 unk_18 = frame")

	var_state.frame_count = 7
	var rei_13_14: Dictionary = lines.lip_sync_rei_lines_13_and_14(var_state, 35)
	check(rei_13_14["actors"][97]["flags"] == 1 and rei_13_14["actors"][97]["unk_18"] == int(rei_13_14["mouth_index"]) + 5, "Rei lines 13..14 sets actor 97 unk_18 = frame + 5")

	var_state.frame_count = 8
	var l15_16: Dictionary = lines.lip_sync_lines_15_and_16(var_state, 35)
	check(l15_16["actors"][97]["unk_18"] == int(l15_16["mouth_index"]) + 1 and l15_16["actors"][98]["unk_18"] == int(l15_16["mouth_index"]) + 5, "Yoshio lines 15..16 sets actor 97 (frame+1) and actor 98 (frame+5)")

	var_state.frame_count = 9
	var shiori_18: Dictionary = lines.lip_sync_shiori(var_state, 35)
	check(shiori_18["actors"][97]["sequence_slot"] == 0 and shiori_18["actors"][97]["unk_18"] == int(shiori_18["mouth_index"]) + 4, "Shiori line 18 sets actor 97 unk_18 = frame + 4")

	var_state.frame_count = 10
	var shiori_19: Dictionary = lines.lip_sync_shiori_line_19(var_state, 35)
	check(shiori_19["actors"][97]["flags"] == 1 and shiori_19["actors"][97]["unk_18"] == int(shiori_19["mouth_index"]) + 5, "Shiori line 19 sets actor 97 unk_18 = frame + 5")

	var_state.frame_count = 11
	var l20_31: Dictionary = lines.lip_sync_lines_20_to_31(var_state, 35)
	check(l20_31["actors"][97]["unk_18"] == int(l20_31["mouth_index"]) + 6 and l20_31["actors"][98]["unk_18"] == int(l20_31["mouth_index"]) + 10 and l20_31["actors"][100]["flags"] == 0, "Yoshio lines 20..31 sets actor 97 (frame+6), actor 98 (frame+10), actor 100 flags=0")

	var_state.opening["cd_audio_state_override"] = 0
	var_state.frame_count = 12
	var shiori_18_off: Dictionary = lines.lip_sync_shiori(var_state, 35)
	check(shiori_18_off["mouth_index"] == 0 and shiori_18_off["actors"][97]["unk_18"] == 4, "Shiori line 18 when checkCD() == 0 resets actor 97 unk_18 to 4 (closed mouth)")
	var rei_12_off: Dictionary = lines.lip_sync_rei(var_state, 35)
	check(rei_12_off["mouth_index"] == 0 and rei_12_off["actors"][96]["unk_18"] == 0, "Rei line 12 when checkCD() == 0 resets actor 96 unk_18 to 0 (closed mouth)")
	var ga_off: Dictionary = lines.lip_sync_girl_a(var_state, 35)
	var gb_off: Dictionary = lines.lip_sync_girl_b(var_state, 35)
	check(ga_off["actors"][100]["flags"] == 0 and gb_off["actors"][101]["flags"] == 0, "Girl A and Girl B when checkCD() == 0 clear actor 100 and 101 flags to 0")

	# =========================================================================
	# 3. Full 45-Line (0..42, 100, 101) Real WAV Envelope & Lip-Sync Execution
	# =========================================================================
	var all_ids: Array = range(43)
	all_ids.append_array([100, 101])
	var voiced_lines_seen: int = 0
	var total_clips_seen: int = 0
	var speaker_tracks_seen: Dictionary = {"yoshio": 0, "girl_a": 0, "girl_b": 0, "rei": 0, "shiori": 0, "player": 0}

	for line_id: int in all_ids:
		var st := State.new()
		st.rule_state = {
			"profile_family_bytes": [130, 160, 0, 0, 0, 0, 0, 0],
			"profile_given_bytes": [130, 162, 0, 0, 0, 0, 0, 0],
			"girls": [{"birthday": 12 | (31 << 4) | (3 << 9)}],
			"rng_state": 12345,
			"rng_draw_count": 7,
		}
		var reset_res := Session.FrameResult.new()
		lines.scene_reset(st, reset_res)
		check(reset_res.error.is_empty(), "scene_reset succeeded for line %d" % line_id)

		var start_res := Session.FrameResult.new()
		if line_id == 100:
			lines.prologue_shiori_birthday_speak(st, start_res)
		elif line_id == 101:
			lines.prologue_shiori_blood_type_speak(st, start_res)
		else:
			lines.line_start(st, line_id, start_res)
		check(start_res.error.is_empty(), "line_start succeeded for line %d: %s" % [line_id, start_res.error])

		var clip_count: int = st.opening["audio"]["clips"].size()
		if clip_count > 0:
			voiced_lines_seen += 1
		total_clips_seen += clip_count
		var track: String = lines.line_speaker_track(line_id)
		speaker_tracks_seen[track] = int(speaker_tracks_seen.get(track, 0)) + 1

		var max_mouth_while_playing: int = 0
		var mouth_while_finished: int = 0
		var clips_completed: int = 0
		var line_done: bool = false

		for frame: int in range(1, 16000):
			st.frame_count = frame
			if clip_count > 0:
				var cur_clip_idx: int = int(st.opening["audio"]["clip_index"])
				var cur_levels: Array[int] = lines.clip_cd_levels(line_id, cur_clip_idx)
				var play_frames: int = mini(60, maxi(12, cur_levels.size()))
				if st.opening["audio"]["status"] == "requested":
					check(_complete_voice(st, "started").is_empty(), "voice start consumed for line %d clip %d" % [line_id, cur_clip_idx])
				elif st.opening["audio"]["status"] == "playing" and int(st.opening["prologue_lip_sync"].get("clip_frame", 0)) >= play_frames:
					check(_complete_voice(st, "finished").is_empty(), "voice finish consumed for line %d clip %d" % [line_id, cur_clip_idx])
					clips_completed += 1

			var run_res := Session.FrameResult.new()
			lines.line_run(st, run_res)
			if not run_res.error.is_empty():
				check(false, "line_run failed on line %d: %s" % [line_id, run_res.error])
				break

			var lip_state: Dictionary = st.opening.get("prologue_lip_sync", {})
			if st.opening["audio"]["status"] == "playing":
				max_mouth_while_playing = maxi(max_mouth_while_playing, int(lip_state.get("mouth_index", 0)))
			elif st.opening["audio"]["status"] in ["finished", "silent"]:
				mouth_while_finished = maxi(mouth_while_finished, int(lip_state.get("mouth_index", 0)))

			if st.substep == 1:
				line_done = true
				break

		check(line_done, "line %d completed text + voice + 61-tick hold" % line_id)
		if clip_count > 0:
			check(clips_completed == clip_count, "line %d completed all %d XA clip(s)" % [line_id, clip_count])
			check(max_mouth_while_playing > 0, "voiced line %d (%s) animated mouth_index > 0 from real WAV getCDlevel()" % [line_id, track])
			check(mouth_while_finished == 0, "voiced line %d closed mouth_index == 0 when checkCD() == 0" % line_id)
		else:
			check(max_mouth_while_playing == 0 and mouth_while_finished == 0, "silent line %d kept mouth_index == 0 throughout" % line_id)

	check(voiced_lines_seen == 23 and total_clips_seen == 24, "verified all 23 voiced lines and 24 XA clips across 45 prologue lines")
	check(speaker_tracks_seen["yoshio"] == 16 and speaker_tracks_seen["girl_a"] == 1 and speaker_tracks_seen["girl_b"] == 1 and speaker_tracks_seen["rei"] == 3 and speaker_tracks_seen["shiori"] == 4, "all 4 speaker tracks (Yoshio=16, Girl A=1, Girl B=1, Rei=3, Shiori=2 voiced + 2 dynamic=4) verified")

	# =========================================================================
	# 4. Mid-Prologue BGM Bank Load & Transition Contracts (prologue.c)
	# =========================================================================
	check(AudioRules.matches_bank_load_args("prologue_rei", 1, 0, 0, 0xcccb, 0xcc86, 0xcc82), "prologue_rei matches audio_bank_load_start(1, 0, 0, 0xCCCB, 0xCC86, 0xCC82)")
	check(AudioRules.matches_bank_load_args("prologue_shiori", 1, 0, 0, 0xcbd4, 0xcbaa, 0xcba5), "prologue_shiori matches audio_bank_load_start(1, 0, 0, 0xCBD4, 0xCBAA, 0xCBA5)")
	check(AudioRules.matches_bank_load_args("prologue_shiori", 1, 0, 0, 0xccc4, 0xccbf, 0xccba), "prologue_shiori also matches prologue.c LBA tuple (1, 0, 0, 0xCCC4, 0xCCBF, 0xCCBA)")
	check(AudioRules.matches_bank_load_args("normal_gameplay", 0, 0xbf98, 0xbf79, 0xd3d5, 0xd387, 0xd37d), "normal_gameplay matches audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD3D5, 0xD387, 0xD37D)")
	check(AudioRules.matches_bank_load_args("normal_gameplay", 0, 0xbf98, 0xbf79, 0xcca6, 0xcca1, 0xcc9d), "normal_gameplay also matches prologue.c LBA tuple (0, 0xBF98, 0xBF79, 0xCCA6, 0xCCA1, 0xCC9D)")

	var bridge := Bridge.new()
	var voice_player := AudioStreamPlayer.new()
	var bgm_player := AudioStreamPlayer.new()
	root.add_child(voice_player)
	root.add_child(bgm_player)
	check(bridge.bind(audio, voice_player, bgm_player).is_empty(), "OpeningAudioBridge binds AudioService and players")

	var bgm_state := State.new()
	bgm_state.game_mode = 0x15
	var tick_cursor: int = 100

	for transition_spec: Dictionary in [
		{"role": "prologue_rei", "fade_cmd": 0xb1, "start_bank": 1, "start_seq": 0},
		{"role": "prologue_shiori", "fade_cmd": 0xc1, "start_bank": 1, "start_seq": 0},
		{"role": "normal_gameplay", "fade_cmd": 0xd1, "start_bank": 0, "start_seq": 0},
	]:
		var role_name: String = transition_spec["role"]
		var fade_cmd: int = transition_spec["fade_cmd"]
		var start_bank: int = transition_spec["start_bank"]
		var start_seq: int = transition_spec["start_seq"]

		var load_res := Session.FrameResult.new()
		if role_name == "normal_gameplay":
			AudioRules.finish_cd_reset(bgm_state, load_res)
			AudioRules.finish_bank_reset(bgm_state, load_res)
		AudioRules.audio_command(bgm_state, 5, load_res)
		AudioRules.audio_command(bgm_state, fade_cmd, load_res)
		AudioRules.begin_bank(bgm_state, role_name, load_res)
		check(load_res.error.is_empty(), "%s bank load + fade commands staged cleanly" % role_name)
		check(AudioRules.bank_poll(bgm_state) == 0, "%s bank_poll is 0 before completion" % role_name)
		var pre_err: String = bridge.preflight(load_res, bgm_state)
		check(pre_err.is_empty(), "%s bank load + fade preflight succeeded: %s" % [role_name, pre_err])
		load_res.committed = true
		var pub_err: String = bridge.publish(load_res, tick_cursor)
		check(pub_err.is_empty(), "%s bank load + fade published cleanly: %s" % [role_name, pub_err])
		tick_cursor += 1

		var pending: Array[Session.ServiceEvent] = bridge.pending_events(tick_cursor)
		check(pending.size() == 1 and pending[0].service == "opening.audio.bank", "%s bank publication queued completion observation" % role_name)
		var comp_res := Session.FrameResult.new()
		AudioRules.bank_complete(pending[0], bgm_state, comp_res)
		comp_res.trace.append({"kind": Session.ServiceKind.COMPLETION, "service": pending[0].service, "payload": pending[0].payload})
		if AudioRules.BANK_READ_SECTORS.has(role_name):
			var ready_frame: int = int(bgm_state.opening.audio_bank.ready_frame)
			bgm_state.frame_count = ready_frame - 1
			check(comp_res.error.is_empty() and AudioRules.bank_poll(bgm_state) == 0, "%s bank_poll stays 0 before its CD read + transfer frames elapse" % role_name)
			bgm_state.frame_count = ready_frame
		check(comp_res.error.is_empty() and AudioRules.bank_poll(bgm_state) == 1, "%s bank completion sets bank_poll == 1" % role_name)
		AudioRules.bank_start(bgm_state, start_bank, start_seq, comp_res)
		check(comp_res.error.is_empty(), "%s bank_start(%d, %d) staged cleanly" % [role_name, start_bank, start_seq])
		check(bridge.preflight(comp_res, bgm_state).is_empty(), "%s bank_start preflight succeeded" % role_name)
		comp_res.committed = true
		check(bridge.publish(comp_res, tick_cursor).is_empty(), "%s bank_start published cleanly" % role_name)
		check(audio.bgm_state["playing"] == true and audio.bgm_state["role"] == role_name and audio.bgm_state["bank"] == start_bank and audio.bgm_state["sequence"] == start_seq, "%s active BGM state matches startBGM(%d) on bank %d" % [role_name, start_seq, start_bank])
		tick_cursor += 1

	bridge.reset_session()
	voice_player.queue_free()
	bgm_player.queue_free()
	await process_frame

	print("------------------------------------------------------------------")
	print(("PASS: %d prologue voiced dialogue & lip-sync checks" % checks) if failures == 0 else ("FAIL: %d/%d checks failed" % [failures, checks]))
	quit(0 if failures == 0 else 1)
