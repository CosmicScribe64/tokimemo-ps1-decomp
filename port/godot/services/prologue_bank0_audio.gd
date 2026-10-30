extends RefCounted
## native-bank0: the bank0 sequence pack (0x8001C000) after the Shiori start.
##  - stage16 prologue_lines_32_to_36_run case4: audio_bank_start(0,1). The pack
##    word is PRO_VOL.SEQ's 0x40, so 0x22/0x21/0x2F/0x201 are queued: the Shiori
##    sequence fades out by B1 (sequence-volume mode, 0x100000 is clear), then
##    PRO_VOL sequence 1 starts on slot 1 at VAB volume 118-25.
##  - stage20/21 audio_command(5)+(0xD1): a master D1 fade of that sequence; the
##    stage21 D1 is ignored by audio_bgm_fade_start (request bit1 is set).
##  - stage21 audio_bank_load_start(0,...): five nowait reads that land, sector
##    by sector, over the live bank0 header/score (COM_MIX.VH, SPRING.SEQ, SPRING.VH).
##  - mode 0x34 stage 0xFF: audio_bank_load_poll queues 0x11 (dispose/transfer of
##    slots 0 and 1, one per pump), then 0x22/0x21/0x2F and audio_bank_start(0,0).
## Source semantics are in sound_driver.c / frame_render_audio.c; the values below
## that are not source-defined are MEASURED on PCSX-Redux, run r1 of
## .asset-cache/emu/finish-landing-m3 (frames.jsonl 42b4e3ed..., events.jsonl
## d7d41d20...; schedule file bc3196bc...). Not a hardware capture. M3 r2 is a
## deterministic replay of r1 from r1's own stage15 save, not a second
## measurement. The independent check is .asset-cache/emu/finish-landing-m3-indep:
## s41 and s97 boot from the name-entry save. s97 reaches stage16 66 frames
## earlier after R than r1, and the schedule to L+131, the +131 read completion,
## the +132/+133 slot transfers and every PRO_VOL/SPRING key-on (which depend on
## STAGE16_TICK_SKIP) recur there. Both runs use the same emulator timing model.
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")

## Sector landings after the stage21 frame L: [pump, region, cumulative sectors
## resident before that pump's libsnd tick]; regions in issue order
## 0=BF98 (0x80010000), 1=BF79 (0x801B8000), 2=D37D (0x8001C000), 3=D387
## (0x801C8000), 4=D3D5 (0x80012000). MEASURED (M3 r1 rows L+n, L = vsync 9316).
const LANDING_SCHEDULE := [
	[2,0,1],[14,0,2],[15,0,3],[17,1,1],[29,1,2],[30,1,4],[31,1,7],[32,1,9],[33,1,12],[34,1,14],[35,1,17],[36,1,19],
	[37,1,22],[38,1,24],[39,1,27],[40,1,29],[41,1,32],[43,2,1],[55,2,2],[56,2,4],[57,2,7],[58,2,9],[59,2,12],[60,2,14],
	[61,2,17],[62,2,19],[63,2,22],[64,2,24],[65,2,27],[66,2,29],[67,2,32],[68,2,34],[69,2,37],[71,3,1],[83,3,2],
	[84,3,4],[85,3,7],[86,3,9],[87,3,12],[88,3,14],[89,3,17],[90,3,19],[91,3,22],[92,3,24],[93,3,27],[94,3,29],
	[95,3,32],[96,3,34],[97,3,37],[98,3,39],[99,3,42],[100,3,44],[101,3,47],[102,3,49],[103,3,52],[104,3,54],
	[105,3,57],[106,3,59],[107,3,62],[108,3,64],[109,3,67],[110,3,69],[111,3,72],[112,3,74],[113,3,77],[114,3,79],
	[115,3,80],[117,4,1],[129,4,2],[130,4,4],[131,4,6],
]
## Logic frame (after L) whose cd_done_count first includes read k (0..4): the
## frame its last sector lands. MEASURED: the L+131 poll queued 0x11 (event
## 9447) while the snapshot of that vsync still showed four reads done.
const READ_DONE_FRAMES := [15, 41, 69, 115, 131]
## MEASURED: SsSeqCalledTbyT ticks lost to audio_bgm_start's SpuClearReverbWorkArea
## wait at each bank0 start (stage16 started after the next vsync tick; spring did not).
const STAGE16_TICK_SKIP := 1
const SPRING_TICK_SKIP := 0
## audio_bank_transfer_schedule moves one requested slot per pump: slot0 in the
## pump after the 0x11 frame T, slot1 in the next. audio_bgm_bank_ready tests
## only the BGM slot (1), so the T+2 poll is the first to see both ready.
const TRANSFER_READY_FRAMES := 2
const STAGE16_ROLE := "bank0_pro_vol"
const SPRING_ROLE := "bank0_spring"
const XA_UNSUPPORTED := "XA playback during the bank0 master D1 fade is unsupported (state_flags 0x400 not modelled)"

static func flat_schedule() -> PackedInt32Array:
	var flat := PackedInt32Array()
	for row: Array in LANDING_SCHEDULE: flat.append_array(PackedInt32Array(row))
	return flat

## cd_done_count() reads complete by logic frame `elapsed` after L.
static func reads_done_at(elapsed: int) -> int:
	var done := 0
	for frame: int in READ_DONE_FRAMES:
		if elapsed >= frame: done += 1
	return done

## The five authenticated sector reads, in issue order, from the finish store.
static func native_inputs(store: FinishBank) -> Dictionary:
	if store == null or store.identity() != FinishBank.TRACK_SHA: return {"error":"bank0 load has no authenticated stage21 reads"}
	var inputs: Array = []
	for row: Array in FinishBank.READS:
		var item := store.transfer(FinishBank.resource_id(row[0]))
		if not str(item.get("error","")).is_empty() or item.get("sha256") != row[6]: return {"error":"bank0 stage21 read changed: " + row[0]}
		inputs.append(item.bytes)
	return {"error":"","inputs":inputs}

## audio_sequence_pack_index word 0 and VAB master volume (header byte 0x18)
## of authenticated bytes: PRO_VOL.SEQ/VH before stage21, the landed
## SPRING.SEQ/VH reads after it.
static func pack_word0(bytes: PackedByteArray) -> int:
	return -1 if bytes.size() < 4 else bytes.decode_u32(0)

static func vab_volume(header: PackedByteArray) -> int:
	return -1 if header.size() < 0x19 else int(header[0x18])
