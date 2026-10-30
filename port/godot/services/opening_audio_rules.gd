extends RefCounted
## T-29021 / T-29025: transactional prologue XA ownership, native logical completion gate,
## exact PS1 getCDlevel() 4-sample ring buffer lip-sync engine, and mid-prologue BGM bank
## load & transition contracts (src/ovl/name_entry/prologue.c).

const Math = preload("res://core/native_math.gd")
const COMPLETION_SERVICE: String = "opening.audio"
const FINISH_HOLD: int = 0x3d
## XA voice progression is a pure function of logic frames (state.frame_count);
## AudioStreamPlayer.finished never drives it. A logic frame is one boot_loop
## iteration (game_state.frame_count), not a vsync: the original drops vsyncs
## on some stage-entry frames (see prologue_media_queue.gd), so every offset
## below is counted in main-loop iterations from the frame_count of the handler
## that requested the clip (audio_command(0x300)):
## - playing (cd_audio_state()&4) is first seen XA_START_FRAMES later: the
##   sound driver's audio_xa_play queues Setloc+ReadS and cd_state becomes 4
##   on the ReadS acknowledge (src/main/sound_driver.c audio_xa_start_poll).
##   Every voiced line 5..15 is 6 iterations. Lines 11 and 15 took 7 vsyncs
##   in the vsync-indexed M2 traces only because the original dropped one
##   vsync in that window (7/0/1, 10/0/1); in iterations they are 6 as well
##   (.asset-cache/emu/opening-timing-v4 fcb-start20/fcb-start33 loops.jsonl).
## - finished (cd_audio_state()&2, no &4) is first seen
##   round(0.4 * span) + 13 + ceil(xa_volume_full / 10) after playing,
##   span = recorded request end_lba - start_lba: the drive reads the span at
##   2x (0.4 vsync per sector), audio_xa_check_end detects cd_audio_ticks() >=
##   xa_clip_end (+0x10 sectors read-past) 12 later, audio_xa_fade_start(0xB4)
##   subtracts xa_fade_step 0xA per tick from the speaker's xa_volume_full
##   (audio_speaker_volume_set) until it is <= 0, and cd_state&2 is seen one
##   iteration after audio_xa_fade_finish. Yoshio/Rei (0x6E) give the earlier
##   constant 24; girl A/B (speaker & 0xF = 14, default 0x64) give 23, which
##   was the unexplained +1 on lines 9 and 10.
## Measured in .asset-cache/emu/shiori-landing-m2 (r2-start20, r3-start58,
## r4-start26, r4-start97, r1 for lines 12-15), every voiced line 5..15:
## end - playing = round(0.4*span)+12 exactly (ceil fails lines 9 and 15,
## floor fails 13 and 14); r3-start33 alone ends lines 5 and 10 early (stale
## cd_audio_ticks), treated as an emulator outlier. The fade is visible in the
## audio_state XA volume of shiori-landing-m2/r2-start20 frames.jsonl: 110 -> 0
## in 11 ticks for lines 5/7/11, 100 -> 0 in 10 for lines 9/10, whose hold
## starts end+11 instead of end+12 (opening-timing-v4 fcb-start20 mst).
## Stage 15 line 0x16 (span 448) is 203 iterations in newgame-golden-v1 sets
## A/B but 269 here. The span is right (audio_state clip 279766..280214).
## PCSX-Redux returns short cd_position_lba GetlocL results (the 0x22 stat
## byte, then zero bytes, e.g. 62:22:00 -> 280500) in every run at
## position-dependent points. When one lands inside this clip (s97b,
## request+74), cd_audio_ticks() passes xa_clip_end, the fade starts and the
## line ends after 203; otherwise 269. The source uses the result unchanged.
## Whether real CD hardware returns the same result is unconfirmed, so this
## may be an emulator artifact or genuine behaviour. The port keeps the
## nominal drive model; the golden treatment of 15/0/7 is an open decision
## (.asset-cache/stage15-xa-2026-10-30/emu s97b vs s97/s97c/s41/s20).
const XA_START_FRAMES: int = 6
const XA_END_DETECT_FRAMES: int = 12 ## xa_clip_end + 0x10 sectors read-past
const XA_FADE_STEP: int = 0xA ## audio_xa_fade_start(0xB4): xa_fade_step
const XA_MAX_SPAN_SECTORS: int = 0x10000
const Session = preload("res://core/game_session.gd")
const MediaQueue = preload("res://services/prologue_media_queue.gd")
## Opening bank readiness is a pure function of logic frames too.
## audio_bank_load_start (src/main/frame_render_audio.c) queues its reads with
## cd_read_sectors_nowait in one frame; the command ring lands them serially.
## audio_bank_load_poll sees the last cd_done_count on the frame that read
## lands, calls audio_bank_transfer_start there, and returns 1 once
## audio_bank_transfer_done holds. Read latencies use the media queue's source
## model, floor(14 + 0.4 * sectors) each, which is exact for all eight reads
## below in five M2 runs (stage 0: +15/+41/+69/+115/+131, stage 7:
## +28/+74/+90; .asset-cache/emu/opening-timing-v2/read_latency.txt). The
## transfer frames after the last read are measured, not derived: arrival 2
## (two bank pairs loaded), Rei 1 (a=1, b=c=0 skips the first pair). The poll
## therefore returns 1 at request +133 (arrival) and +91 (Rei) in all runs
## (stage 7 also in r1). Shiori (stage 10) keeps its own measured landing.
## Sectors per read in source call order (b, c, f, e, d; zero LBAs skipped).
const BANK_READ_SECTORS: Dictionary = {"prologue_arrival": [3, 0x20, 0x25, 0x50, 6], "prologue_rei": [0x25, 0x50, 6]}
const BANK_TRANSFER_FRAMES: Dictionary = {"prologue_arrival": 2, "prologue_rei": 1}

## sound_driver.c::audio_bgm_fade_start: command -> [rate, decrement].
const BGM_FADES: Dictionary = {0xb1: [0x100, 10], 0xc1: [0xc0, 1], 0xd1: [0x80, 1]}
## VAB master volume byte (header offset 0x18) read by audio_bgm_start through
## func_80090270; sequence gain = byte - 25, minus 24 in output mode 1.
## IJYU_VOL.VH sha f5ae3b75... (Rei) and CBD4 sector read sha 622d0390... (Shiori).
const BANK_VAB_VOLUME: Dictionary = {"prologue_rei": 108, "prologue_shiori": 126}

const SPEAKER_SHIORI: int = 0
const SPEAKER_REI: int = 12
const SPEAKER_YOSHIO: int = 13
const SPEAKER_PLAYER: int = 15
const SPEAKER_GIRL_A: int = 30
const SPEAKER_NARRATOR: int = 31
const SPEAKER_GIRL_B: int = 46

const BANK_TRANSITION_CONTRACTS: Dictionary = {
	"prologue_arrival": {
		"role": "prologue_arrival",
		"bank_slot": 0,
		"se_header_lba": 0xc592,
		"se_body_lba": 0xc580,
		"bgm_header_lba": 0xd15f,
		"bgm_body_lba": 0xd110,
		"seq_lba": 0xd102,
		"pre_commands": [],
		"start_bgm_bank": 0,
		"start_bgm_seq": 0,
		"stage": 0,
	},
	"prologue_rei": {
		"role": "prologue_rei",
		"bank_slot": 1,
		"se_header_lba": 0,
		"se_body_lba": 0,
		"bgm_header_lba": 0xcccb,
		"bgm_body_lba": 0xcc86,
		"seq_lba": 0xcc82,
		"pre_commands": [5, 0xb1],
		"start_bgm_bank": 1,
		"start_bgm_seq": 0,
		"stage": 7,
	},
	"prologue_shiori": {
		"role": "prologue_shiori",
		"bank_slot": 1,
		"se_header_lba": 0,
		"se_body_lba": 0,
		"bgm_header_lba": 0xcbd4,
		"bgm_body_lba": 0xcbaa,
		"seq_lba": 0xcba5,
		"alt_bgm_header_lba": 0xccc4,
		"alt_bgm_body_lba": 0xccbf,
		"alt_seq_lba": 0xccba,
		"pre_commands": [5, 0xc1],
		"start_bgm_bank": 1,
		"start_bgm_seq": 0,
		"stage": 10,
	},
	"normal_gameplay": {
		"role": "normal_gameplay",
		"bank_slot": 0,
		"se_header_lba": 0xbf98,
		"se_body_lba": 0xbf79,
		"bgm_header_lba": 0xd3d5,
		"bgm_body_lba": 0xd387,
		"seq_lba": 0xd37d,
		"alt_bgm_header_lba": 0xcca6,
		"alt_bgm_body_lba": 0xcca1,
		"alt_seq_lba": 0xcc9d,
		"pre_commands": [5, 0xd1],
		"start_bgm_bank": 0,
		"start_bgm_seq": 0,
		"requires_cd_reset": true,
		"requires_bank_reset": true,
		"stage": 21,
	},
}


static func bank_transition_contract(role: String) -> Dictionary:
	if not BANK_TRANSITION_CONTRACTS.has(role):
		return {}
	return BANK_TRANSITION_CONTRACTS[role].duplicate(true)


static func matches_bank_load_args(role: String, bank_slot: int, se_h: int, se_b: int, bgm_h: int, bgm_b: int, seq: int) -> bool:
	if not BANK_TRANSITION_CONTRACTS.has(role):
		return false
	var c: Dictionary = BANK_TRANSITION_CONTRACTS[role]
	if bank_slot != int(c["bank_slot"]) or se_h != int(c["se_header_lba"]) or se_b != int(c["se_body_lba"]):
		return false
	var primary_match: bool = bgm_h == int(c["bgm_header_lba"]) and bgm_b == int(c["bgm_body_lba"]) and seq == int(c["seq_lba"])
	var alt_match: bool = c.has("alt_bgm_header_lba") and bgm_h == int(c["alt_bgm_header_lba"]) and bgm_b == int(c["alt_bgm_body_lba"]) and seq == int(c["alt_seq_lba"])
	return primary_match or alt_match


static func prepare_mid_prologue_bank_roles(audio: RefCounted) -> String:
	if audio == null or not audio.get("prepared_bank_roles") is Dictionary:
		return "audio service is unavailable for mid-prologue bank preparation"
	if not audio.prepared_bank_roles.has("prologue_rei"):
		return "Original Rei alternate bank was not prepared from authenticated inputs"
	# stage10/8 prologue_shiori: the BGM owner holds the authenticated sector
	# reads; its role entry mirrors the current software VAB checkpoint because
	# the native transfer only happens at the stage11/substep2 poll.
	var service: Variant = audio.get("original_bgm_service")
	if service != null and service.has_method("shiori_prepared_role"):
		var shiori: Dictionary = service.shiori_prepared_role(audio)
		if shiori.get("error") == "": audio.prepared_bank_roles["prologue_shiori"] = shiori
	return ""


static func _completion_schema(event: RefCounted, service: String, fields: Array[String]) -> String:
	if event.phase != Session.Phase.BEFORE_MODE or event.kind != Session.ServiceKind.COMPLETION or event.service != service:
		return "opening completion arrived outside its declared service/phase/kind"
	var payload: Dictionary = event.payload
	if payload.size() != fields.size() or not payload.has_all(fields):
		return "opening completion fields differ from its exact schema"
	for key: String in ["epoch", "line_id", "clip_index", "completion_index", "bank_epoch"]:
		if payload.has(key) and not payload[key] is int:
			return "opening completion requires an exact integer: " + key
	for key: String in ["epoch", "completion_index", "bank_epoch"]:
		if payload.has(key) and payload[key] < 1:
			return "opening completion has invalid positive ownership: " + key
	if payload.has("line_id") and (payload["line_id"] < 0 or payload["line_id"] > 42):
		return "opening completion line is outside original voiced lines"
	if payload.has("clip_index") and (payload["clip_index"] < 0 or payload["clip_index"] > 1):
		return "opening completion clip index is invalid"
	for key: String in ["role", "request_key"]:
		if payload.has(key) and (not payload[key] is String or not RegEx.create_from_string("^[A-Za-z0-9_.-]{1,96}$").search(payload[key])):
			return "opening completion identity is invalid: " + key
	if payload.has("status") and (not payload["status"] is String or payload["status"] not in ["started", "finished"]):
		return "opening completion status is invalid"
	if payload.has("ready") and (not payload["ready"] is bool or not payload["ready"]):
		return "opening bank completion must explicitly report ready true"
	return ""


static func begin_bank(state: RefCounted, role: String, result: RefCounted) -> void:
	var epoch: int = int(state.opening.get("audio_bank", {}).get("epoch", 0)) + 1
	var slots: Dictionary = state.opening.get("audio_banks",{}).duplicate(true)
	var previous: Dictionary = state.opening.get("audio_bank",{})
	if previous.get("ready") == true: slots[previous.role] = previous.duplicate(true)
	state.opening["audio_banks"] = slots
	state.opening["audio_bank"] = {"epoch": epoch, "role": role, "ready": false}
	if BANK_READ_SECTORS.has(role):
		state.opening.audio_bank["ready_frame"] = bank_ready_frame(state, role)
	state.opening["audio_profile"] = role
	if role != "prologue_arrival" and BANK_TRANSITION_CONTRACTS.has(role):
		var contract: Dictionary = bank_transition_contract(role)
		state.opening["prologue_bgm_transition"] = {
			"role": role,
			"bank_epoch": epoch,
			"ready": false,
			"bgm_started": false,
			"contract": contract,
		}
	result.audio.append({"kind": "bank_load", "role": role, "bank_epoch": epoch})


static func bank_complete(event: RefCounted, state: RefCounted, result: RefCounted) -> void:
	result.error = _completion_schema(event, "opening.audio.bank", ["bank_epoch", "role", "ready"])
	if not result.error.is_empty():
		return
	var bank: Dictionary = state.opening.get("audio_bank", {})
	if event.phase != 0 or event.service != "opening.audio.bank" or bank.is_empty() or bank.get("ready") != false or event.payload.get("role") != bank.get("role") or event.payload.get("bank_epoch") != bank.get("epoch") or not event.payload.get("ready") is bool or not event.payload["ready"]:
		result.error = "stale, duplicate or mismatched opening bank completion"
		return
	bank["ready"] = true
	state.opening.get_or_add("audio_banks",{})[bank.role] = bank.duplicate(true)
	if state.opening.get("prologue_bgm_transition") is Dictionary:
		var trans: Dictionary = state.opening["prologue_bgm_transition"]
		if trans.get("role") == bank.get("role") and trans.get("bank_epoch") == bank.get("epoch"):
			trans["ready"] = true


static func bank_poll(state: RefCounted, _result: RefCounted = null) -> int:
	var bank: Dictionary = state.opening.get("audio_bank", {})
	if not bool(bank.get("ready", false)):
		return 0
	if bank.get("ready_frame") is int and Math.s32(int(bank.ready_frame) - int(state.frame_count)) > 0:
		return 0
	return 1


## First logic frame on which audio_bank_load_poll returns 1 for a bank load
## requested on this frame. The reads queue behind any read still in flight.
static func bank_ready_frame(state: RefCounted, role: String) -> int:
	var frames: int = MediaQueue.cd_wait_frames(state) + int(BANK_TRANSFER_FRAMES[role])
	for sectors: int in BANK_READ_SECTORS[role]:
		frames += MediaQueue.model_read_vsyncs(sectors)
	return Math.u32(int(state.frame_count) + frames)


static func bank_start(state: RefCounted, bank_slot: int, sequence: int, result: RefCounted) -> void:
	var bank: Dictionary = state.opening.get("audio_bank", {})
	if bank.is_empty() or not bool(bank.get("ready", false)):
		result.error = "prologue BGM start requires a ready transactional sound bank"
		return
	if bank_slot < 0 or bank_slot > 1 or sequence < 0 or sequence > 3:
		result.error = "prologue BGM start bank or sequence is outside native range"
		return
	var role: String = str(bank.get("role", ""))
	var epoch: int = int(bank.get("epoch", 0))
	state.opening["prologue_bgm_state"] = {
		"playing": true,
		"role": role,
		"bank": bank_slot,
		"sequence": sequence,
		"bank_epoch": epoch,
	}
	if role in ["prologue_rei", "prologue_shiori"]:
		# audio_bgm_start: gain = VAB master volume - 25 (- 24 in mode 1).
		var full: Variant = state.opening.get("native_audio_context", {}).get("bgm_volume_full")
		if not full in [69, 93]:
			result.error = "prologue BGM start requires the actual output-mode gain"
			return
		state.opening.prologue_bgm_state["gain"] = int(BANK_VAB_VOLUME[role]) - 25 - (24 if full == 69 else 0)
	if role in ["prologue_rei", "prologue_shiori"] and state.opening.get("bgm_fade") is Dictionary:
		# Source audio_command_bgm finishes an outstanding fade when the new
		# sequence request arrives (request bit1), and audio_bgm_start clears
		# that request; it does not keep fading the new sequence.
		state.opening.bgm_fade.active = false
		state.opening.bgm_fade.erase("request")
	if state.opening.get("prologue_bgm_transition") is Dictionary:
		var trans: Dictionary = state.opening["prologue_bgm_transition"]
		if trans.get("role") == role and trans.get("bank_epoch") == epoch:
			trans["bgm_started"] = true
			trans["started_bank"] = bank_slot
			trans["started_sequence"] = sequence
	result.audio.append({
		"kind": "bgm_play",
		"bank": bank_slot,
		"sequence": sequence,
		"role": role,
		"bank_epoch": epoch,
	})


static func audio_command(state: RefCounted, command: int, result: RefCounted) -> void:
	if command == 5:
		state.opening["audio_state_flags"] = int(state.opening.get("audio_state_flags", 0)) | 0x100000
		result.audio.append({"kind": "native_audio_control", "command": 5})
	elif command == 0xb1:
		var context: Dictionary = state.opening.get("native_audio_context",{})
		if not context.get("bgm_volume") is int:
			result.error = "BGM fade requires the actual current sequence gain"; return
		state.opening["bgm_fade"] = {"active":true,"gain":int(context.bgm_volume),"ticks":0}
		result.audio.append({"kind":"native_bgm_fade","command":command})
	elif command in [0xc1, 0xd1]:
		# audio_control_dispatch 0xC0/0xD0 -> audio_bgm_fade_start: a fade of
		# the playing sequence, not a stop. While request bit1 from an earlier
		# fade is still set the source leaves accumulator/rate unchanged.
		var previous: Variant = state.opening.get("bgm_fade")
		if previous is Dictionary and previous.get("request") == true:
			# native-bank0: prologue_finish_run's D1 one frame after the stage20
			# D1 is a no-op in audio_bgm_fade_start; the native owner ignores it
			# the same way. Any other restart stays refused.
			if command == 0xd1 and previous.get("command") == 0xd1 and state.opening.get("bank0_audio") is Dictionary:
				previous["ignored"] = int(previous.get("ignored",0)) + 1
				result.audio.append({"kind":"native_bgm_fade","command":command})
				return
			result.error = "BGM fade restart while a source fade request is outstanding is unsupported"; return
		var playing: Dictionary = state.opening.get("prologue_bgm_state", {})
		if playing.get("playing") != true or not playing.get("gain") is int:
			result.error = "BGM fade requires the actual playing sequence gain"; return
		var fade: Array = BGM_FADES[command]
		state.opening["bgm_fade"] = {"active":true,"gain":int(playing.gain),"ticks":0,"command":command,
			"rate":fade[0],"decrement":fade[1],"accumulator":0,"request":true,"started_frame":int(state.frame_count)}
		result.audio.append({"kind":"native_bgm_fade","command":command})
	elif command == 0x71:
		var bgm_state: Dictionary = state.opening.get("prologue_bgm_state", {}).duplicate(true)
		bgm_state["playing"] = false
		bgm_state["last_stop_command"] = command
		state.opening["prologue_bgm_state"] = bgm_state
		result.audio.append({"kind": "bgm_stop", "command": command})
	elif command in [0x503, 0x604, 0x605]:
		var se_log: Array = state.opening.get("prologue_se_log", []).duplicate(true)
		se_log.append({"frame": int(state.frame_count), "command": command})
		state.opening["prologue_se_log"] = se_log
		result.audio.append({"kind": "se_play", "command": command})
	else:
		result.error = "unsupported prologue audio command: " + str(command)


static func bgm_fade_tick(state: RefCounted, result: RefCounted) -> void:
	var fade: Dictionary = state.opening.get("bgm_fade",{})
	if fade.get("active") != true: return
	if fade.has("command"):
		# C1/D1: audio_bgm_update arithmetic, first applied in the pump after the
		# command frame. The native engine runs the same update once per audio
		# pump itself, so no per-frame native event is emitted.
		if int(fade.started_frame) == int(state.frame_count): return
		fade.accumulator = int(fade.accumulator) + int(fade.rate)
		if (int(fade.accumulator) & 0xff00) == 0: return
		fade.accumulator = int(fade.accumulator) & 0xff
		fade.gain -= int(fade.decrement)
		fade.ticks += 1
		if fade.gain < 0:
			# audio_bgm_fade_finish -> audio_bgm_control(0x71)
			fade.active = false
			state.opening.get_or_add("prologue_bgm_state",{})["playing"] = false
			if fade.has("next_sequence"):
				# native-bank0 stage16: bgm_sequence_id = next, audio_bgm_request_start
				# on the ready slot 1 -> audio_bgm_start, which clears request bit1.
				fade.erase("request")
				state.opening["prologue_bgm_state"] = {"playing":true,"role":fade.next_role,"bank":0,"sequence":fade.next_sequence,"gain":fade.next_gain}
		return
	fade.gain -= 10
	fade.ticks += 1
	if fade.gain < 0: fade.active = false
	result.audio.append({"kind":"native_bgm_fade_tick","xa_active":(check_cd(state)&4)!=0,"tick":fade.ticks,"gain":fade.gain})

static func finish_cd_reset(state: RefCounted, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {})
	if audio.get("status") in ["requested", "playing"]:
		cancel_voice(state, result)
	state.opening["cd_command_reset"] = true


static func finish_bank_reset(state: RefCounted, _result: RefCounted) -> void:
	state.opening["audio_bank_state_reset"] = true


static func cancel_voice(state: RefCounted, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {}).duplicate(true)
	audio.merge({"epoch": int(audio.get("epoch", 0)) + 1, "clips": [], "voice_started": false,
		"status": "cancelled"}, true)
	for key: String in ["xa_request_frame", "xa_start_frame", "xa_finish_frame", "xa_started_frame"]:
		audio.erase(key)
	state.opening["audio"] = audio
	result.audio.append({"kind": "native_stop_xa_if_active", "epoch": audio["epoch"]})


## Recorded XA request span in sectors, or -1 when the clip has no exact one.
static func xa_span_sectors(clip: Dictionary) -> int:
	var request: Variant = clip.get("xa_request")
	if not request is Dictionary: return -1
	for key: String in ["start_lba", "end_lba"]:
		var v: Variant = request.get(key)
		if not (v is int or v is float) or float(v) != floor(float(v)): return -1
	var span: int = int(request.end_lba) - int(request.start_lba)
	return span if span > 0 and span <= XA_MAX_SPAN_SECTORS else -1

static func xa_start_frames(_line_id: int) -> int:
	return XA_START_FRAMES

## sound_driver.c audio_speaker_volume_set: XA volume for speaker & 0xF.
static func xa_volume_full(speaker: int) -> int:
	match speaker & 0xF:
		6: return 0x7D
		7, 9, 11: return 0x5F
		10: return 0x5A
		12, 13: return 0x6E
		15: return 0x69
		_: return 0x64

## audio_xa_fade_tick calls (one per iteration) until the volume reaches <= 0.
static func xa_fade_frames(speaker: int) -> int:
	return (xa_volume_full(speaker) + XA_FADE_STEP - 1) / XA_FADE_STEP

## Iterations from cd_state&4 to cd_state&2 for a span and speaker:
## round(0.4*span) + 12 (end detect) + fade ticks + 1 (fade finish -> &2 seen).
static func xa_play_frames(span: int, speaker: int) -> int:
	return (4 * span + 5) / 10 + XA_END_DETECT_FRAMES + xa_fade_frames(speaker) + 1

## Schedules the current clip from this handler's frame_count.
static func _schedule_xa(state: RefCounted, audio: Dictionary, result: RefCounted) -> void:
	var span: int = xa_span_sectors(audio["clips"][audio["clip_index"]])
	if span < 0:
		result.error = "opening XA clip has no recorded request span for its logic-frame schedule"
		return
	var speaker: Variant = audio["clips"][audio["clip_index"]].get("speaker")
	if not speaker is int or speaker < 0 or speaker > 0xff:
		result.error = "opening XA clip has no speaker for its volume fade schedule"
		return
	# The Setloc+ReadS of audio_xa_play goes through the same command ring as
	# the data reads: a clip requested while a read is still in flight starts
	# XA_START_FRAMES after that read lands. Golden set A stage 18: line 0x26
	# is requested one frame after stage17 case3's O_13 read (0x41 sectors,
	# 40 frames) and is first seen playing at that landing + 6 (18/0/1 lasts
	# 44 frames, .asset-cache/emu/newgame-golden-v1/runs/setA/logic.jsonl).
	var start: int = Math.u32(int(state.frame_count) + MediaQueue.cd_wait_frames(state) + xa_start_frames(int(audio["line_id"])))
	audio["xa_request_frame"] = int(state.frame_count)
	audio["xa_start_frame"] = start
	audio["xa_finish_frame"] = Math.u32(start + xa_play_frames(span, speaker))

## CD phase, once per committed frame after the mode handler (frame_count is
## already the next handler's value): advances the scheduled clip so that the
## next handler sees playing/finished on exactly the scheduled frame. The
## voice_play intent is published when playback starts.
static func xa_tick(state: RefCounted, result: RefCounted) -> void:
	var audio: Variant = state.opening.get("audio")
	if not audio is Dictionary or not audio.get("clips") is Array or audio.clips.is_empty(): return
	var status: Variant = audio.get("status")
	if status != "requested" and status != "playing": return
	for key: String in ["xa_start_frame", "xa_finish_frame"]:
		if not audio.get(key) is int:
			result.error = "scheduled opening XA clip lacks its logic-frame schedule"
			return
	var now: int = int(state.frame_count)
	if status == "requested" and Math.s32(now - int(audio.xa_start_frame)) >= 0:
		audio["status"] = "playing"
		audio["voice_started"] = true
		audio["xa_started_frame"] = now
		if state.opening.get("prologue_lip_sync") is Dictionary:
			state.opening["prologue_lip_sync"]["clip_index"] = int(audio["clip_index"])
			state.opening["prologue_lip_sync"]["clip_frame"] = 0
		result.audio.append(_intent(audio))
	elif status == "playing" and Math.s32(now - int(audio.xa_finish_frame)) >= 0:
		audio["status"] = "finished"


static func begin_line(state: RefCounted, prepared: Dictionary, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {}).duplicate(true)
	var epoch: int = int(audio.get("epoch", 0)) + 1
	var clips: Array = prepared.get("clips", [])
	audio.merge({"epoch": epoch, "line_id": int(prepared["line_id"]), "clips": clips.duplicate(true),
		"clip_index": 0, "voice_started": false, "status": "silent" if clips.is_empty() else "requested",
		"completion_index": int(audio.get("completion_index", 0))}, true)
	for key: String in ["xa_request_frame", "xa_start_frame", "xa_finish_frame", "xa_started_frame"]:
		audio.erase(key)
	if not clips.is_empty():
		_schedule_xa(state, audio, result)
		if not result.error.is_empty(): return
	state.opening["audio"] = audio
	result.trace.append({"source": "prologue_line_start", "line_id": audio["line_id"], "epoch": epoch})


static func _intent(audio: Dictionary) -> Dictionary:
	var event: Dictionary = audio["clips"][audio["clip_index"]].duplicate(true)
	event.merge({"epoch": audio["epoch"], "line_id": audio["line_id"], "clip_index": audio["clip_index"]}, true)
	return event


static func run_line(state: RefCounted, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {})
	var text: Dictionary = state.opening.get("prologue_text", {})
	if audio.is_empty() or text.get("line_id", -1) != audio.get("line_id") or text.get("iteration", -1) != state.frame_count or not text.get("k_disp_inc_result") is int or text["k_disp_inc_result"] not in [0, 1]:
		result.error = "prologue line requires matching sourced text result for this logical iteration"
		return
	var text_complete: bool = text["k_disp_inc_result"] == 1
	if audio["clips"].is_empty():
		if text_complete:
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			text["scroll_state"] = 0
		else:
			state.mode_step_timer = 0
	elif bool(audio["voice_started"]):
		if audio["status"] == "finished":
			if int(audio["clip_index"]) + 1 < audio["clips"].size():
				audio["clip_index"] += 1
				audio["status"] = "requested"
				audio.erase("xa_started_frame")
				if state.opening.get("prologue_lip_sync") is Dictionary:
					state.opening["prologue_lip_sync"]["clip_index"] = int(audio["clip_index"])
					state.opening["prologue_lip_sync"]["clip_frame"] = 0
				_schedule_xa(state, audio, result)
			elif text_complete:
				state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
				text["scroll_state"] = 0
		else:
			state.mode_step_timer = 0
	# Native voiced-finished/text-incomplete preserves the accumulated timer;
	# narrator text-incomplete and active voiced playback reset it (prologue.c).
	if Math.u32(state.mode_step_timer) >= FINISH_HOLD:
		state.mode_step_timer = 0
		state.substep += 1


## Host playback observations (AudioStreamPlayer start/finish) never drive
## the opening: xa_tick advances voice status on logic frames. Any such
## observation is refused so a wall-clock signal cannot reach the logic.
static func complete(event: RefCounted, _state: RefCounted, result: RefCounted) -> void:
	result.error = _completion_schema(event, COMPLETION_SERVICE, ["epoch", "line_id", "clip_index", "request_key", "status", "completion_index"])
	if not result.error.is_empty():
		return
	result.error = "opening voice progression is scheduled on logic frames; host playback observations are refused"


# =============================================================================
# Exact PS1 getCDlevel() 4-Sample Ring Buffer Lip-Sync Engine (prologue.c)
# =============================================================================

static func speaker_track_for_line(line_id: int, speaker: int = -1) -> String:
	if line_id in [100, 101]:
		return "shiori"
	match speaker:
		SPEAKER_YOSHIO:
			return "yoshio"
		SPEAKER_GIRL_A:
			return "girl_a"
		SPEAKER_GIRL_B:
			return "girl_b"
		SPEAKER_REI:
			return "rei"
		SPEAKER_SHIORI:
			return "shiori"
	if line_id in [5, 7, 11, 15, 20, 22, 24, 25, 26, 28, 30, 32, 34, 36, 38, 40]:
		return "yoshio"
	if line_id == 9:
		return "girl_a"
	if line_id == 10:
		return "girl_b"
	if line_id in [12, 13, 14]:
		return "rei"
	if line_id in [18, 19]:
		return "shiori"
	return "player"


static func lip_sync_variant_for_line(line_id: int) -> String:
	if line_id in [5, 6, 7, 32, 33, 34, 35, 36]:
		return "yoshio"
	if line_id == 9:
		return "girl_a"
	if line_id == 10:
		return "girl_b"
	if line_id == 11:
		return "line_11"
	if line_id == 12:
		return "rei"
	if line_id in [13, 14]:
		return "rei_lines_13_and_14"
	if line_id in [15, 16]:
		return "lines_15_and_16"
	if line_id == 18:
		return "shiori"
	if line_id == 19:
		return "shiori_line_19"
	if line_id >= 20 and line_id <= 31:
		return "lines_20_to_31"
	if line_id == 100:
		return "shiori_birthday_speak"
	if line_id == 101:
		return "shiori_blood_type_speak"
	if line_id in [38, 40]:
		return "yoshio_stage18"
	return "player_silent"


static func reset_lip_sync(state: RefCounted) -> void:
	# Matches prologue_reset_scene_objects (prologue.c:236-260):
	# prologue_cd_level_last = 0; prologue_cd_level_history[0..3] = {0, 0, 0, 0}.
	state.opening["prologue_cd_level_last"] = 0
	state.opening["prologue_cd_level_history"] = [0, 0, 0, 0]
	state.opening["prologue_lip_sync"] = {
		"line_id": -1,
		"speaker": -1,
		"speaker_track": "none",
		"variant": "none",
		"check_cd": 0,
		"cd_active": false,
		"cd_level": 0,
		"history": [0, 0, 0, 0],
		"average": 0,
		"ring_frame": 0,
		"mouth_index": 0,
		"clip_index": 0,
		"clip_frame": 0,
		"clip_levels": [],
		"actors": {},
	}


static func begin_line_lip_sync(state: RefCounted, line_id: int, speaker: int, clip_levels: Array = []) -> void:
	if not state.opening.get("prologue_cd_level_history") is Array or state.opening["prologue_cd_level_history"].size() != 4:
		state.opening["prologue_cd_level_history"] = [0, 0, 0, 0]
	if not state.opening.has("prologue_cd_level_last"):
		state.opening["prologue_cd_level_last"] = 0
	var history: Array = state.opening["prologue_cd_level_history"].duplicate()
	var sum_val: int = int(history[0]) + int(history[1]) + int(history[2]) + int(history[3])
	var average: int = int(sum_val / 4)
	var ring_frame: int = clampi(int(average / 10), 0, 3)
	state.opening["prologue_lip_sync"] = {
		"line_id": line_id,
		"speaker": speaker,
		"speaker_track": speaker_track_for_line(line_id, speaker),
		"variant": lip_sync_variant_for_line(line_id),
		"check_cd": 0,
		"cd_active": false,
		"cd_level": 0,
		"history": history,
		"average": average,
		"ring_frame": ring_frame,
		"mouth_index": 0,
		"clip_index": 0,
		"clip_frame": 0,
		"clip_levels": clip_levels.duplicate(true),
		"actors": {},
	}


static func check_cd(state: RefCounted) -> int:
	if state.opening.has("cd_audio_state_override"):
		return int(state.opening["cd_audio_state_override"]) & 4
	var audio: Dictionary = state.opening.get("audio", {})
	if audio.get("status") == "playing":
		return 4
	return 0


static func get_cd_level(state: RefCounted, cd_level_override: int = -1) -> int:
	if (check_cd(state) & 4) == 0:
		return 0
	if cd_level_override >= 0:
		return cd_level_override
	var lip: Dictionary = state.opening.get("prologue_lip_sync", {})
	var clip_levels: Array = lip.get("clip_levels", [])
	var audio: Dictionary = state.opening.get("audio", {})
	var clip_idx: int = int(audio.get("clip_index", lip.get("clip_index", 0)))
	var frame_cursor: int = int(lip.get("clip_frame", 0))
	if clip_idx >= 0 and clip_idx < clip_levels.size() and clip_levels[clip_idx] is Array:
		var levels: Array = clip_levels[clip_idx]
		if not levels.is_empty():
			return int(levels[clampi(frame_cursor, 0, levels.size() - 1)])
	return 0


static func _step_ring_buffer(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	var cd_state: int = check_cd(state)
	var cd_active: bool = (cd_state & 4) != 0
	var level: int = get_cd_level(state, cd_level_override) if cd_active else 0
	var history: Array = [0, 0, 0, 0]
	if state.opening.get("prologue_cd_level_history") is Array and state.opening["prologue_cd_level_history"].size() == 4:
		history = state.opening["prologue_cd_level_history"].duplicate()
	var slot: int = int(state.frame_count) & 3
	history[slot] = level
	state.opening["prologue_cd_level_history"] = history
	state.opening["prologue_cd_level_last"] = level
	var sum_val: int = int(history[0]) + int(history[1]) + int(history[2]) + int(history[3])
	var average: int = int(sum_val / 4)
	var ring_frame: int = int(average / 10)
	if ring_frame >= 4:
		ring_frame = 3
	elif ring_frame < 0:
		ring_frame = 0
	var mouth_index: int = ring_frame if cd_active else 0
	var lip: Dictionary = state.opening.get("prologue_lip_sync", {}).duplicate(true)
	var audio: Dictionary = state.opening.get("audio", {})
	var clip_idx: int = int(audio.get("clip_index", lip.get("clip_index", 0)))
	var clip_frame: int = int(lip.get("clip_frame", 0))
	lip["check_cd"] = cd_state
	lip["cd_active"] = cd_active
	lip["cd_level"] = level
	lip["history"] = history.duplicate()
	lip["average"] = average
	lip["ring_frame"] = ring_frame
	lip["mouth_index"] = mouth_index
	lip["clip_index"] = clip_idx
	lip["sampled_clip_frame"] = clip_frame
	if cd_active:
		lip["clip_frame"] = clip_frame + 1
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_yoshio(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_yoshio (prologue.c:843-879):
	# actor_clear_slot(0x61); if (cd_audio_state() & 4) { actor_init_slot(0x61, 1, 1, 0, 0x38, 0xFFF0, 0xFFDC); D_801206AC = 6 - frame; }
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": 6 - frame, "z": 0x38, "x": -16, "y": -36}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "yoshio"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_line_11(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_line_11 (prologue.c:1108-1156):
	# clears 97 and 98; if (cd_audio_state() & 4) sets 97 unk_18 = frame + 1, 98 unk_18 = frame + 5.
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 1, "z": 0x3c, "x": -14, "y": -39}
		actors[98] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 5, "z": 0x3c, "x": -23, "y": -80}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
		actors[98] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "line_11"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_girl_a(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_actor100_mouth_girl_a (prologue.c:945-961):
	# if (cd_audio_state() & 4) { D_80120710 = 1; D_80120712 = 1; } else { D_80120710 = 0; }
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[100] = {"flags": 1, "sequence_slot": 1}
	else:
		actors[100] = {"flags": 0, "sequence_slot": 0}
	lip["variant"] = "girl_a"
	lip["binary_mouth_index"] = 1 if cd_active else 0
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_girl_b(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_actor101_mouth_girl_b (prologue.c:963-979):
	# if (cd_audio_state() & 4) { D_80120734 = 1; D_80120736 = 2; } else { D_80120734 = 0; }
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[101] = {"flags": 1, "sequence_slot": 2}
	else:
		actors[101] = {"flags": 0, "sequence_slot": 0}
	lip["variant"] = "girl_b"
	lip["binary_mouth_index"] = 1 if cd_active else 0
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_rei(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_rei (prologue.c:1277-1317):
	# if (cd_audio_state() & 4) { D_80120668 = 0; D_80120688 = frame; } else { D_80120668 = 0; D_80120688 = 0; }
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	actors[96] = {"flags": 1, "sequence_slot": 0, "unk_18": frame if cd_active else 0}
	lip["variant"] = "rei"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_rei_lines_13_and_14(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_rei_lines_13_and_14 (prologue.c:1418-1454):
	# clears 97; if (cd_audio_state() & 4) sets 97 unk_18 = frame + 5.
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 5, "z": 0x3c, "x": -2, "y": -59}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "rei_lines_13_and_14"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_lines_15_and_16(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_lines_15_and_16 (prologue.c:1618-1666):
	# clears 97 and 98; if (cd_audio_state() & 4) sets 97 unk_18 = frame + 1, 98 unk_18 = frame + 5.
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 1, "z": 0x3c, "x": -14, "y": -39}
		actors[98] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 5, "z": 0x3c, "x": -23, "y": -80}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
		actors[98] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "lines_15_and_16"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_shiori(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_shiori (prologue.c:1884-1923):
	# if (cd_audio_state() & 4) { D_8012068C = 0; D_801206AC = frame + 4; } else { D_8012068C = 0; D_801206AC = 4; }
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	actors[97] = {"flags": 1, "sequence_slot": 0, "unk_18": (frame + 4) if cd_active else 4}
	lip["variant"] = "shiori"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_shiori_line_19(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_shiori_line_19 (prologue.c:1992-2028):
	# clears 97; if (cd_audio_state() & 4) sets 97 unk_18 = frame + 5.
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 5, "z": 0x3c, "x": 0, "y": -59}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "shiori_line_19"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func lip_sync_lines_20_to_31(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	# prologue_lip_synchronization_lines_20_to_31 (prologue.c:2139-2190):
	# clears 97; if (cd_audio_state() & 4) sets 97 unk_18 = frame + 6, 98 unk_18 = frame + 10, 100 flags = 0.
	var lip: Dictionary = _step_ring_buffer(state, cd_level_override)
	var cd_active: bool = bool(lip["cd_active"])
	var frame: int = int(lip["ring_frame"])
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if cd_active:
		actors[97] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 6, "z": 0x3c, "x": -52, "y": -39}
		actors[98] = {"flags": 1, "sequence_slot": 1, "unk_18": frame + 10, "z": 0x3c, "x": -61, "y": -80}
		actors[100] = {"flags": 0, "sequence_slot": 0}
	else:
		actors[97] = {"flags": 0, "sequence_slot": 0, "unk_18": 0}
	lip["variant"] = "lines_20_to_31"
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func actor100_pulse(state: RefCounted) -> Dictionary:
	# prologue_actor100_pulse (prologue.c:2192-2214):
	# v = ((u32)(game_state.frame_count >> 2) % 3) + 1; if (v == 1) flags = 0 else flags = 1, sequence_slot = v + 0xD.
	var lip: Dictionary = _step_ring_buffer(state, 0)
	var v: int = (((int(state.frame_count) & 0xffffffff) >> 2) % 3) + 1
	var actors: Dictionary = lip.get("actors", {}).duplicate(true)
	if v == 1:
		actors[100] = {"flags": 0, "sequence_slot": 0}
	else:
		actors[100] = {"flags": 1, "sequence_slot": v + 0x0d}
	lip["actors"] = actors
	state.opening["prologue_lip_sync"] = lip
	return lip


static func step_lip_sync(state: RefCounted, cd_level_override: int = -1) -> Dictionary:
	var audio: Dictionary = state.opening.get("audio", {})
	var lip: Dictionary = state.opening.get("prologue_lip_sync", {})
	var line_id: int = int(audio.get("line_id", lip.get("line_id", -1)))
	var variant: String = lip_sync_variant_for_line(line_id)
	match variant:
		"yoshio":
			return lip_sync_yoshio(state, cd_level_override)
		"line_11":
			return lip_sync_line_11(state, cd_level_override)
		"girl_a":
			return lip_sync_girl_a(state, cd_level_override)
		"girl_b":
			return lip_sync_girl_b(state, cd_level_override)
		"rei":
			return lip_sync_rei(state, cd_level_override)
		"rei_lines_13_and_14":
			return lip_sync_rei_lines_13_and_14(state, cd_level_override)
		"lines_15_and_16":
			return lip_sync_lines_15_and_16(state, cd_level_override)
		"shiori":
			return lip_sync_shiori(state, cd_level_override)
		"shiori_line_19":
			return lip_sync_shiori_line_19(state, cd_level_override)
		"lines_20_to_31":
			return lip_sync_lines_20_to_31(state, cd_level_override)
		"shiori_birthday_speak", "shiori_blood_type_speak":
			var pulsed: Dictionary = actor100_pulse(state)
			pulsed["variant"] = variant
			state.opening["prologue_lip_sync"] = pulsed
			return pulsed
		_:
			var res: Dictionary = _step_ring_buffer(state, cd_level_override)
			res["variant"] = variant
			state.opening["prologue_lip_sync"] = res
			return res


## native-bank0 stage16 audio_bank_start(0,1) with the 0x40 pack word: 0x201
## arrives while the Shiori sequence plays (state 0x100, no request bit1), so
## audio_command_bgm arms bgm_next_sequence_id = 1 and audio_bgm_fade_start(0xB1).
## The fade steps once per pump from the next frame; at its finish sequence 1
## starts with gain `next_gain` (bgm_fade_tick).
static func bank0_stage16_start(state: RefCounted, next_gain: int, result: RefCounted) -> void:
	var playing: Dictionary = state.opening.get("prologue_bgm_state", {})
	var previous: Variant = state.opening.get("bgm_fade")
	if playing.get("playing") != true or playing.get("role") != "prologue_shiori" or not playing.get("gain") is int:
		result.error = "stage16 bank0 start requires the playing Shiori sequence"; return
	if previous is Dictionary and (previous.get("active") == true or previous.get("request") == true):
		result.error = "stage16 bank0 start during an outstanding fade is not modelled"; return
	state.opening["bgm_fade"] = {"active":true,"gain":int(playing.gain),"ticks":0,"command":0xb1,"rate":0x100,"decrement":10,
		"accumulator":0,"request":true,"started_frame":int(state.frame_count),"next_sequence":1,"next_gain":next_gain,"next_role":"bank0_pro_vol"}

## native-bank0 0x11 (audio_command_bank_transfer 17) on the BGM slot:
## audio_bank_dispose(1) + audio_bank_request_transfer(1) -> audio_bgm_fade_finish
## when state 0x100 is set: the fade request and the sequence end here.
static func bank0_transfer(state: RefCounted) -> void:
	var fade: Variant = state.opening.get("bgm_fade")
	if fade is Dictionary:
		fade.active = false
		fade.erase("request")
	state.opening.get_or_add("prologue_bgm_state",{})["playing"] = false

## native-bank0 spring audio_bank_start(0,0): nothing plays, so 0x200 starts
## SPRING.SEQ sequence 0 on slot 1 in the next pump (audio_bgm_request_start).
static func bank0_spring_start(state: RefCounted, gain: int) -> void:
	state.opening["prologue_bgm_state"] = {"playing":true,"role":"bank0_spring","bank":0,"sequence":0,"gain":gain}
