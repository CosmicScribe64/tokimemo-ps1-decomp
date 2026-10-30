extends RefCounted
## T-29025: logical CD request/completion records, never host I/O or pixels.
## The owner supplies immutable reader-authenticated resource descriptions.
## This queue cannot establish source authenticity or a gameplay capability.

const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")
const SERVICE := "opening.media"
## CD read completion is a pure function of logic frames (state.frame_count),
## never of host I/O: cd_sync reports bit0 only once the authenticated bytes
## have landed AND the source read latency has elapsed. cd_read_sectors and
## cd_read_sectors_nowait (src/main/cd_queue.c) queue Setloc+ReadN on the
## command ring; cd_read_count/cd_read_status change when the last sector of
## the request arrives (2x speed: 150 sectors/s = 0.4 vsync per sector).
## Source model: floor(14 + 0.4 * sectors) vsyncs from the request frame to
## the first logic frame that sees the read done. It matches 22 of the 25
## opening reads measured in the emulator; the measured table below is
## authoritative (identical in every run that recorded it, shiori-landing-m2:
## five runs r2-start20, r3-start33, r3-start58, r4-start26, r4-start97 for
## stages 0-6, and those five plus r1, which starts from prefix-stage7.sstate,
## for stages 7-11; request = audio_command(6) frame, done = cd_read_count
## write frame; extraction script and table in
## .asset-cache/emu/opening-timing-v2/). Units: state.frame_count counts
## boot_loop iterations, and the original drops vsyncs on frames that also do a
## large memcpy or MDEC decode (.asset-cache/emu/opening-timing-v4/vsync_drops.txt,
## per-substep drops on the same loops.jsonl boundaries as the iteration table,
## fcb-start20/fcb-start33). The table is therefore in iterations: every entry
## equals its vsync measurement except stage2 ACDD. It is requested on the last
## 2/0/7 iteration, which also copies 0x8800 bytes and stays on screen for 2
## vsyncs (repaid by a 0-vsync catch-up iteration at the start of 2/0/8), so
## the request lands one vsync late: 17 vsyncs request-to-done in the v2
## measurement, 18 iterations (2/0/8 lasts 18 iterations in both traces; its 2
## dropped vsyncs are on its last iteration, after the read). The two remaining
## exceptions (stage1 4966 +1, stage3 ABB8 +1) are sub-vsync request phase
## effects and hold in iterations too.
## Key: "mode_stage:lba" in game_mode 0x15; backgrounds are 10 sectors.
const CD_SECTOR_RATE_NUM := 2 ## 0.4 vsync per sector = 2/5
const CD_SECTOR_RATE_DEN := 5
const CD_READ_SETUP_VSYNCS := 14
const BACKGROUND_SECTORS := 10 ## dec_bg_cd_read: cd_read_sectors_nowait(0xA, ...)
const MEASURED_READ_VSYNCS := {
	"0:494b":18, "0:abab":19,
	"1:4954":17, "1:495d":18, "1:4966":19,
	"2:ab18":33, "2:ac3f":20, "2:acdd":18,
	"3:aa23":33, "3:ace6":18, "3:abb8":18,
	"4:ab18":33,
	"5:acf6":18, "5:aa54":33, "5:acd4":18, "5:abca":24,
	"6:acec":18, "6:aa85":33, "6:abe3":24,
	"7:ad00":18, "7:aab6":33, "7:abfc":17,
	"8:aae7":33, "8:ac05":24,
	"9:aa85":33, "9:ac1e":27,
	"10:ab18":33, "10:ac3f":20, "10:acf6":18,
	"11:ab18":33, "11:ad05":18, "11:ac50":40,
}
const MEASURED_READ_SECTORS := {"494b":10,"abab":13,"4954":9,"495d":10,"4966":10,"ab18":49,"ac3f":17,"acdd":10,
	"aa23":49,"ace6":10,"abb8":9,"acf6":10,"aa54":49,"acd4":10,"abca":25,"acec":10,"aa85":49,"abe3":25,
	"ad00":10,"aab6":49,"abfc":9,"aae7":49,"ac05":25,"ac1e":33,"ad05":10,"ac50":65}
var _backgrounds: Dictionary = {}
var _actors: Dictionary = {}
var _boards: Dictionary = {}
var _hud: Dictionary = {}
var _banks: Dictionary = {}
var _rooms: Dictionary = {}
var _prepared := false

## hud: the command reload's CALROM read (cd_read_sectors(0x95, 0x801A0000,
## 0x9B94)); rooms: its slot-0 room reads keyed by str(lba)
## (dec_bg_cd_read(D_800B5950[season], 0): 10 sectors to 0x80162000).
func prepare(backgrounds: Dictionary, actors: Dictionary, hud: Dictionary, boards: Dictionary = {}, banks: Dictionary = {}, rooms: Dictionary = {}) -> String:
	if _prepared:
		return "opening media descriptions are immutable; create a new queue for another pack"
	for resource: Variant in backgrounds.values() + actors.values() + boards.values() + banks.values() + rooms.values() + ([hud] if not hud.is_empty() else []):
		if not resource is Dictionary or not resource.get("id") is String or resource.id.is_empty() or not resource.get("sha256") is String or resource.sha256.length() != 64:
			return "opening media needs immutable authenticated resource identities"
	_backgrounds = backgrounds.duplicate(true)
	_actors = actors.duplicate(true)
	_boards = boards.duplicate(true)
	_hud = hud.duplicate(true)
	_banks = banks.duplicate(true)
	_rooms = rooms.duplicate(true)
	_prepared = true
	return ""

func callbacks() -> Dictionary:
	return {"scene_reset":scene_reset,"background_read":background_read,"cd_sync":cd_sync,
		"background_decode":background_decode,"actor_read":actor_read,
		"actor_palette_setup":actor_palette_setup,"board_read":board_read,
		"board_decode":board_decode,"hud_read_start":hud_read_start,
		"finish_cd_reset":finish_cd_reset}

## Source model latency (vsyncs from request to completion seen by logic).
static func model_read_vsyncs(sectors: int) -> int:
	if sectors < 1 or sectors > 0xff: return -1
	return CD_READ_SETUP_VSYNCS + (sectors * CD_SECTOR_RATE_NUM) / CD_SECTOR_RATE_DEN

## Measured opening latency for this request when the emulator recorded it,
## otherwise the source model. Retries are never in the table.
static func read_vsyncs(game_mode: int, mode_stage: int, lba: int, sectors: int, retry: bool = false) -> int:
	var key := "%d:%x" % [mode_stage, lba]
	if game_mode == 0x15 and not retry and MEASURED_READ_VSYNCS.has(key) and MEASURED_READ_SECTORS.get("%x" % lba) == sectors:
		return int(MEASURED_READ_VSYNCS[key])
	return model_read_vsyncs(sectors)

## Frames still to wait (0 when the last scheduled read has landed).
static func cd_wait_frames(state: State) -> int:
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("cd_ready_frame") is int: return 0
	return maxi(0, Math.s32(int(media.cd_ready_frame) - int(state.frame_count)))

func _schedule(state: State, lba: int, sectors: int, retry: bool, result: Session.FrameResult) -> void:
	var vsyncs := read_vsyncs(state.game_mode, state.mode_stage, lba, sectors, retry)
	if vsyncs < 1:
		result.error = "opening CD read has no source latency for %d sectors" % sectors
		return
	var media: Dictionary = _record(state)
	# The command ring serializes reads: a read queued behind one still in
	# flight starts when that one completes.
	var start := int(state.frame_count) + cd_wait_frames(state)
	media["cd_ready_frame"] = Math.u32(start + vsyncs)

func _record(state: State) -> Dictionary:
	if not state.opening.has("media"):
		state.opening["media"] = {"epoch":0,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	return state.opening.media

func _request(state: State, kind: String, payload: Dictionary, resource: Dictionary, result: Session.FrameResult) -> String:
	var media: Dictionary = _record(state)
	if state.game_mode == 0x15 and state.mode_stage >= 2 and media.requests.size() > 3:
		var keep: Dictionary = {}
		for v: Variant in media.background_pending.values(): keep[str(v)] = true
		for v: Variant in media.actor_pending.values(): keep[str(v)] = true
		if media.get("board_pending") is Dictionary:
			for v: Variant in media.board_pending.values(): keep[str(v)] = true
		if media.get("bank_pending") is Array:
			for v: Variant in media.bank_pending: keep[str(v)] = true
		if not str(media.get("hud_pending", "")).is_empty():
			keep[str(media.hud_pending)] = true
		for req_key: String in media.requests.keys():
			if not keep.has(req_key) and media.requests[req_key].get("status") != "pending":
				media.requests.erase(req_key)
	if media.requests.size() >= 512:
		result.error = "opening media request bound reached"
		return ""
	var identity: String = str(media.next_id)
	media.next_id += 1
	var request := {"epoch":media.epoch,"request_id":identity,"kind":kind,"payload":payload.duplicate(true),
		"resource_id":resource.id,"resource_sha256":resource.sha256,"status":"pending"}
	request["request_key"] = JSON.stringify([request.epoch,identity,kind,payload,resource.id,resource.sha256]).sha256_text()
	media.requests[identity] = request
	result.presentation.append({"kind":"opening_media_request","service":SERVICE,"request":request.duplicate(true)})
	return identity

func scene_reset(state: State, result: Session.FrameResult) -> void:
	finish_cd_reset(state, result)
	if not result.error.is_empty():
		return
	var media: Dictionary = _record(state)
	media.decoded.clear()
	media.actor_pending.clear()
	media.erase("board_pending")
	result.presentation.append({"kind":"opening_scene_reset"})

func finish_cd_reset(state: State, result: Session.FrameResult) -> void:
	if not _prepared:
		result.error = "opening media queue is not prepared"
		return
	var media: Dictionary = _record(state)
	media.epoch += 1
	media.next_id = 0
	media.requests.clear()
	media.background_pending.clear()
	media.actor_pending.clear()
	media.erase("board_pending")
	media.erase("cd_ready_frame")
	media.erase("bank_pending")
	media.erase("room_pending")
	media.hud_pending = ""
	result.presentation.append({"kind":"opening_media_reset","service":SERVICE,"epoch":media.epoch})

## cd_queue.c::cd_command_reset (prologue_finish_run case 0): forget every
## queued and in-flight request (head/tail, read count, phase). It sends no
## CD-ROM command and touches no RAM or VRAM, so completed buffers and actor
## uploads stay owned; only the request records and pending completions go.
func cd_command_reset(state: State, result: Session.FrameResult) -> void:
	if not _prepared:
		result.error = "opening media queue is not prepared"
		return
	var media: Dictionary = _record(state)
	media.epoch += 1
	media.next_id = 0
	media.requests.clear()
	media.background_pending.clear()
	media.actor_pending.clear()
	media.erase("board_pending")
	media.erase("bank_pending")
	media.erase("room_pending")
	media.hud_pending = ""
	result.presentation.append({"kind":"opening_cd_command_reset","service":SERVICE,"epoch":media.epoch})

## One audio_bank_load_start cd_read_sectors_nowait. On the opening route only
## prologue_finish_run case 0 issues these reads, in the source order.
func bank_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	var resource: Variant = _banks.get("%d:%d:%d" % [sectors, destination, lba])
	if not resource is Dictionary or resource.get("lba") != lba or resource.get("sectors") != sectors or resource.get("destination") != destination:
		result.error = "source-backed opening bank read is unavailable for LBA %x" % lba
		return
	if state.game_mode != 0x15 or state.mode_stage != 21 or state.mode_step != 0 or state.substep != 0:
		result.error = "opening bank read is outside prologue_finish_run case 0"
		return
	var media: Dictionary = _record(state)
	if not media.get("bank_pending") is Array:
		media["bank_pending"] = []
	if media.bank_pending.size() >= 5:
		result.error = "opening bank reads exceed the five audio_bank_load_start reads"
		return
	var identity := _request(state,"bank",{"lba":lba,"sectors":sectors,"destination":destination},resource,result)
	if not identity.is_empty():
		media.bank_pending.append(identity)

## cd_done_count() since the reset. The CD queue completes reads in issue
## order, so only the completed prefix of the bank reads counts.
func bank_reads_done(state: State) -> int:
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("bank_pending") is Array: return 0
	var done := 0
	for identity: Variant in media.bank_pending:
		var request: Variant = media.requests.get(identity)
		if not request is Dictionary or request.kind != "bank" or request.status != "ready" or request.epoch != media.epoch: break
		done += 1
	return done

func background_read(state: State, lba: int, slot: int, result: Session.FrameResult, retry: bool = false) -> void:
	var resource: Variant = _backgrounds.get(str(lba))
	if not resource is Dictionary or resource.get("slot") != slot or resource.get("decoded") != true:
		result.error = "source-backed opening background/decode is unavailable for LBA %x slot %d" % [lba,slot]
		return
	var allow_re_read := false
	# Stage10/substep7: slot1 buffers hold ACEC/AD00, so dec_bg_cd_read misses
	# its two-entry LBA cache and rereads ACF6.
	if state.game_mode == 0x15 and ((lba == 0xacdd and state.mode_stage == 2 and state.substep == 7) or (lba == 0xace6 and state.mode_stage == 3 and state.substep == 3) or (lba == 0xacf6 and state.mode_stage == 10 and state.mode_step == 0 and state.substep == 7)
			# Stage15/31: the slot1 cache holds AD0B and ACEC, so ACD4 misses.
			or (lba == 0xacd4 and state.mode_stage == 15 and state.mode_step == 0 and state.substep == 31)):
		allow_re_read = true
	# Stage14 case3 (prologue.c:2101): substep is already 4 when
	# dec_bg_cd_read(0xACEC,1) runs. Its LBA cache (the slot-1 buffers) holds
	# AD05 and AD0B, so the source misses and reads P_01_06 again.
	if state.game_mode == 0x15 and lba == 0xacec and state.mode_stage == 14 and state.mode_step == 0 and state.substep == 4 and _slot1_cache_misses(state,lba):
		allow_re_read = true
	if not retry and not allow_re_read:
		for previous: Dictionary in _record(state).requests.values():
			if previous.kind == "background" and previous.payload.lba == lba:
				result.error = "opening background cache reuse is not implemented for this resource"
				return
	var identity := _request(state,"background",{"lba":lba,"slot":slot,"retry":retry},resource,result)
	if not identity.is_empty():
		_record(state).background_pending[str(slot)] = identity
		_schedule(state,lba,BACKGROUND_SECTORS,retry,result)

static func _slot1_cache_misses(state: State, lba: int) -> bool:
	var names_media: Variant = state.opening.get("names_media")
	if not names_media is Dictionary: return false
	for key: String in ["buffer_0","buffer_1"]:
		if not names_media.get(key) is Dictionary or names_media[key].get("lba") == lba: return false
	return true

func actor_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	var composite_key := "%d:%d:%d" % [sectors, destination, lba]
	var resource: Variant = _actors.get(composite_key, _actors.get(str(lba)))
	if not resource is Dictionary or resource.get("sectors") != sectors or resource.get("destination") != destination:
		result.error = "source-backed opening actor composition is unavailable for LBA %x" % lba
		return
	var hidden_arrival: bool = resource.get("hidden_stage0_profile") is String and resource.hidden_stage0_profile == "o01_hidden.v1" and lba == 0xabab and sectors == 13 and destination == 0x80180000 and state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 1 and state.substep == 3
	var stage_2_to_6_nbn: bool = state.game_mode == 0x15 and ((lba == 0xab18 and sectors == 49 and destination == 0x801a4000 and ((state.mode_stage == 2 and state.substep == 4) or (state.mode_stage == 4 and state.substep == 3))) or (lba == 0xac3f and sectors == 17 and destination == 0x80180000 and state.mode_stage == 2 and state.substep == 6) or (lba == 0xaa23 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 3 and state.substep == 2) or (lba == 0xabb8 and sectors == 9 and destination == 0x80180000 and state.mode_stage == 4 and state.substep == 0) or (lba == 0xaa54 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 5 and state.substep == 1) or (lba == 0xabca and sectors == 25 and destination == 0x801f0000 and state.mode_stage == 5 and state.substep == 4) or (lba == 0xaa85 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 6 and state.substep == 14) or (lba == 0xabe3 and sectors == 25 and destination == 0x801f0000 and state.mode_stage == 6 and state.substep == 15))
	var rei_nbn: bool = state.game_mode == 0x15 and state.mode_step == 0 and (
		(lba == 0xaab6 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 7 and state.substep == 5)
		or (lba == 0xabfc and sectors == 9 and destination == 0x801f0000 and state.mode_stage == 7 and state.substep == 6)
		or (lba == 0xaae7 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 8 and state.substep == 5)
		or (lba == 0xac05 and sectors == 25 and destination == 0x80180000 and state.mode_stage == 8 and state.substep == 6)
		or (lba == 0xaa85 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 9 and state.substep == 9)
		or (lba == 0xac1e and sectors == 33 and destination == 0x801e8000 and state.mode_stage == 9 and state.substep == 10)
		or (lba == 0xab18 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 10 and state.substep == 5)
		or (lba == 0xac3f and sectors == 17 and destination == 0x801f0000 and state.mode_stage == 10 and state.substep == 6)
		# prologue_line_17_run: case2 rereads CUT12 (the Shiori VAB body
		# staging at 0x801B8000 overwrote its tail), case5 reads O_13.
		or (lba == 0xab18 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 11 and state.substep == 2)
		or (lba == 0xac50 and sectors == 65 and destination == 0x801d8000 and state.mode_stage == 11 and state.substep == 5)
		# Stages 12-14 (prologue.c:1831-2129), checked reads at the substep
		# that issues them.
		or (lba == 0xab49 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 12 and state.substep == 0)
		or (lba == 0xac91 and sectors == 9 and destination == 0x801f0000 and state.mode_stage == 12 and state.substep == 1)
		or (lba == 0xac9a and sectors == 33 and destination == 0x801e8000 and state.mode_stage == 13 and state.substep == 3)
		or (lba == 0xab7a and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 14 and state.substep == 4)
		or (lba == 0xacbb and sectors == 25 and destination == 0x801e8000 and state.mode_stage == 14 and state.substep == 5)
		# prologue_lines_20_to_31_run 32/33 and the 34 retry; prologue_line_37_run 3.
		or (lba == 0xaa54 and sectors == 49 and destination == 0x801a4000 and state.mode_stage == 15 and state.substep in [32,34])
		or (lba == 0xabca and sectors == 25 and destination == 0x801e8000 and state.mode_stage == 15 and state.substep == 33)
		or (lba == 0xac50 and sectors == 65 and destination == 0x80180000 and state.mode_stage == 17 and state.substep == 3))
	if not stage_2_to_6_nbn and not rei_nbn and (not resource.get("native_composition") is bool or (not resource.native_composition and not hidden_arrival)):
		result.error = "source-backed visible actor composition is unavailable for LBA %x" % lba
		return
	var identity := _request(state,"actors",{"lba":lba,"sectors":sectors,"destination":destination},resource,result)
	if not identity.is_empty():
		_record(state).actor_pending[str(destination)] = identity
		_schedule(state,lba,sectors,false,result)

func hud_read_start(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	if _hud.get("lba") != lba or _hud.get("sectors") != sectors or _hud.get("destination") != destination or _hud.get("native_composition") != true:
		result.error = "source-backed opening command HUD is unavailable"
		return
	var identity := _request(state,"hud",{"lba":lba,"sectors":sectors,"destination":destination},_hud,result)
	if not identity.is_empty():
		_record(state).hud_pending = identity
		_schedule(state,lba,sectors,false,result)

## settings_command_screen_reload_calendar substep 1:
## cd_read_sectors(0x95, 0x801A0000, 0x9B94). On the fresh route only mode
## 0x34 stage 0xFF step 1 substep 1 issues it.
func command_hud_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	if state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step != 1 or state.substep != 1:
		result.error = "command HUD read is outside settings_command_screen_reload_calendar substep 1"
		return
	hud_read_start(state, sectors, destination, lba, result)

## background_images.c::dec_bg_cd_read(lba, 0) on a slot-0 cache miss:
## cd_read_sectors_nowait(0xA, 0x80162000, lba). The caller owns the LBA
## cache; this only queues the read of an authenticated room resource.
func room_read(state: State, lba: int, result: Session.FrameResult) -> void:
	var resource: Variant = _rooms.get(str(lba))
	if not resource is Dictionary or resource.get("lba") != lba or resource.get("sectors") != BACKGROUND_SECTORS or resource.get("destination") != 0x80162000:
		result.error = "source-backed command room read is unavailable for LBA %x" % lba
		return
	if state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step != 2:
		result.error = "command room read is outside settings_command_screen_build"
		return
	var identity := _request(state,"room",{"lba":lba,"sectors":BACKGROUND_SECTORS,"destination":0x80162000,"slot":0},resource,result)
	if not identity.is_empty():
		_record(state)["room_pending"] = identity
		_schedule(state,lba,BACKGROUND_SECTORS,false,result)

## The completed request named by media[key] of the given kind, or {} while it
## is pending, stale or absent.
static func completed_request(state: State, key: String, kind: String) -> Dictionary:
	var media: Variant = state.opening.get("media")
	if not media is Dictionary: return {}
	var request: Variant = media.requests.get(str(media.get(key, "")))
	if not request is Dictionary or request.kind != kind or request.status != "ready" or request.epoch != media.epoch: return {}
	return request

func cd_sync(state: State, _result: Session.FrameResult) -> int:
	if not _prepared:
		_result.error = "opening media queue is not prepared"
		return 0
	for request: Dictionary in _record(state).requests.values():
		if request.status == "pending":
			return 0
	return 0 if cd_wait_frames(state) > 0 else 1

func background_decode(state: State, slot: int, result: Session.FrameResult) -> int:
	var media: Dictionary = _record(state)
	var request: Variant = media.requests.get(media.background_pending.get(str(slot), ""))
	if not request is Dictionary or request.kind != "background" or request.status not in ["ready","retry"]:
		result.error = "opening background decode has no matching completed read"
		return -1
	if request.status == "retry":
		return -1
	var previous: Dictionary = media.decoded.get(str(slot), {"buffer":0})
	var buffer: int = 1 - int(previous.buffer)
	media.decoded[str(slot)] = {"resource_id":request.resource_id,"resource_sha256":request.resource_sha256,"buffer":buffer}
	result.presentation.append({"kind":"opening_background_select","slot":slot,"resource_id":request.resource_id,"buffer":buffer})
	return buffer

func actor_palette_setup(state: State, result: Session.FrameResult) -> void:
	var media: Dictionary = _record(state)
	var request: Variant = media.requests.get(media.actor_pending.get(str(0x80180000), ""))
	if not request is Dictionary or request.kind != "actors" or request.status != "ready":
		result.error = "arrival actor initialization has no completed native actor resource"
		return
	result.presentation.append({"kind":"opening_arrival_actor_setup","resource_id":request.resource_id})

func board_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	var resource: Variant = _boards.get(str(lba))
	if not resource is Dictionary or resource.get("lba") != lba or resource.get("sectors") != sectors or resource.get("destination") != destination:
		result.error = "source-backed opening board read is unavailable for LBA %x" % lba
		return
	if lba != 0x4954 or sectors != 9 or destination != 0x80162000 or state.game_mode != 0x15 or state.mode_stage != 1 or state.mode_step != 0 or state.substep != 0:
		result.error = "opening board read is outside its source stage/extent"
		return
	var identity := _request(state,"board",{"lba":lba,"sectors":sectors,"destination":destination},resource,result)
	if not identity.is_empty():
		var media: Dictionary = _record(state)
		if not media.get("board_pending") is Dictionary:
			media["board_pending"] = {}
		media.board_pending[str(destination)] = identity
		_schedule(state,lba,sectors,false,result)

func board_decode(state: State, result: Session.FrameResult) -> void:
	var media: Dictionary = _record(state)
	var pending: Variant = media.get("board_pending", {})
	var request: Variant = media.requests.get(pending.get(str(0x80162000), "") if pending is Dictionary else "")
	if not request is Dictionary or request.kind != "board" or request.status != "ready":
		result.error = "opening board decode has no matching completed read"
		return
	result.presentation.append({"kind":"opening_board_decode","resource_id":request.resource_id})

func complete(event: Session.ServiceEvent, state: State, _rng, result: Session.FrameResult) -> void:
	if event == null or event.kind != Session.ServiceKind.COMPLETION or event.service != SERVICE or event.phase != Session.Phase.BEFORE_MODE:
		result.error = "opening media completion belongs to the wrong service phase"
		return
	var payload: Dictionary = event.payload
	if payload.size() != 5 or not payload.has_all(["epoch","request_id","request_key","status","resource_id"]):
		result.error = "opening media completion identity is malformed"
		return
	if not payload.epoch is int or payload.epoch < 0 or payload.epoch > 0xffffffff:
		result.error = "opening media completion epoch must be an exact native-range integer"
		return
	for key: String in ["request_id","request_key","status","resource_id"]:
		if not payload[key] is String or payload[key].is_empty() or payload[key].length() > 128:
			result.error = "opening media completion identity requires bounded strings"
			return
	if not state.opening.get("media") is Dictionary:
		result.error = "opening media completion has no transactional request owner"
		return
	var media: Dictionary = state.opening.media
	var request: Variant = media.requests.get(payload.request_id)
	if not request is Dictionary or request.status != "pending" or payload.epoch != media.epoch or payload.request_key != request.request_key or payload.resource_id != request.resource_id:
		result.error = "opening media completion is stale, duplicate or mismatched"
		return
	if payload.status != "ready" and not (payload.status == "retry" and request.kind == "background"):
		result.error = "opening media completion status is unsupported"
		return
	request.status = payload.status
