extends RefCounted
## Prepared original bytes become software read buffers only after frame commit.
## Completion observations enter the next logical BEFORE_MODE phase.
const Store = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const TextWork = preload("res://rules/prologue_text_work.gd")
const ShioriActors = preload("res://services/shiori_actor_model.gd")
const Stages15To17 = preload("res://services/opening_stages_15_17.gd")
const BOARD_ID := "prologue.board.0x4954"
const APPROVAL_META := &"first_arrival_media_preflight_v1"
# [source buffer, bytes] of each stage's memcpy to 0x80180000.
const SHIORI_COPY_STAGES := {BoardStore.O_13_ID: 12, BoardStore.O_14_ID: 13, BoardStore.O_15_ID: 14, BoardStore.O_16_ID: 15}
const SHIORI_COPY_FRAME_REFUSED := "first-arrival Shiori memcpy is outside its stage's commit frame"
# memcpy(0x80180000, source, n) of each stage's actor resource; O_13 is
# stage11 case6's copy (prologue.c:1805), drawn by stage12 before its commit.
const SHIORI_COPIES := {BoardStore.O_13_ID: [0x801d8000, 0x20800], BoardStore.O_14_ID: [0x801f0000, 0x4800], BoardStore.O_15_ID: [0x801e8000, 0x10800], BoardStore.O_16_ID: [0x801e8000, 0xc800]}
const SHIORI_COMMIT_BLINK_UNSUPPORTED := "stage14 commit-frame blink would arm actor98 with O_15 table offsets over O_16 bytes (sequence pointer 0x1D1D1D1D); the original behaviour is unknown"
# Stages 18-20 (prologue.c:2565-2706). paint_background hands P_01_12, the
# white/flash boxes and the O_13 actor96 to closing_scene_renderer.gd; each
# frame is refused when the renderer or the P_01_12 upload owner is missing.
const CLOSING_RENDERER_MISSING := "stages 18-20 need the prepared closing-scene renderer"
const CLOSING_BG_NOT_UPLOADED := "stages 18-20 dec_bg_draw needs the uploaded P_01_12 buffer from stage17"
const CLOSING_BG_SHOWN_LBA := 0x4973
const CLOSING_BG_OTHER_LBA := 0x496a
const CLOSING_LAYER0_CLEARED := 0x80000000
const O13_DESTINATION := 0x80180000
const O13_CLUT_ROW := 0x1fb
# The 20->21 frame: prologue_closing_run case2 calls mode_stage_next and
# then dec_bg_draw(1) (prologue.c:2697-2705), so stage20's composition is
# drawn at committed 21/0. prologue_finish_run never calls dec_bg_draw.
const CLOSING_EXIT_UNSUPPORTED := "stage21 entry frame is not the stage20 closing exit composition"
const CLOSING_EXIT_LINE := 42
const CLOSING_EXIT_COUNTER := 0x3e
const CLOSING_CASE2_EXIT_DRAW := "stage19 case2 exit frame hides actor96 before actor_draw_all"
const OWN_INTENTS := ["opening_media_request","opening_background_select","opening_arrival_actor_setup",
	"opening_board_decode","opening_media_reset","opening_scene_reset","first_arrival_background_draw",
	"first_arrival_native_upload","first_arrival_board_upload","first_arrival_board_scroll_draw",
	"first_arrival_names_bg_upload","first_arrival_names_crossfade_draw",
	"first_arrival_cut_upload","first_arrival_o12_backup","first_arrival_o12_restore","first_arrival_o06_copy","first_arrival_o07_copy","first_arrival_o08_copy","first_arrival_o11_copy","first_arrival_o13_copy","first_arrival_stage_actor_upload","first_arrival_shiori_copy",
	"opening_cd_command_reset","first_arrival_e8_copy"]
# --- stage 21 (prologue_finish_run) bank reads, T-39002 ---
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const BankLoader = preload("res://services/native_bank_loader.gd")
const FINISH_BANK_UNPREPARED := "original stage21 COM_MIX/SPRING bank reads are not prepared"
## audio_bank_load_poll -> audio_bank_transfer_start -> audio_command(0x11): the
## first point where the source needs the bank in sound RAM. The native audio
## engine has no COM_MIX/SPRING bank0 transfer, so the port stops here.
const FINISH_BANK_TRANSFER_UNSUPPORTED := "normal-gameplay COM_MIX/SPRING bank transfer (audio_command 0x11) is unsupported by the native audio engine"
const Bank0 = preload("res://services/prologue_bank0_audio.gd")
const AudioRulesBank0 = preload("res://services/opening_audio_rules.gd")
const CommandReload = preload("res://services/command_reload_store.gd")
var _finish_bank: FinishBank
## Mode 0x34 command reload reads (installed first_arrival_command_reload role).
var _command_reload: CommandReload
## native-bank0 owner of the stage21 landing and the 0x11 transfer:
## func(state) -> String, "" when a native engine can take them. Without it
## the 0x11 poll is still refused with FINISH_BANK_TRANSFER_UNSUPPORTED.
var bank0_owner: Callable
## The same owner's audio_bgm_bank_ready(1) after the 0x11 frame:
## func(state) -> {"error":String, "ready":bool}, read from the native engine.
var bank0_slot_state: Callable
var queue := Queue.new()
var _store: Store
var _board_store: BoardStore
var _pending: Array[Dictionary] = []
var _buffers: Dictionary = {}
var _actor_upload: Dictionary = {}
var _board_upload: Dictionary = {}
var _names_bg_uploads: Dictionary = {}
var _cut_uploads: Dictionary = {}
var _stage_actor_uploads: Dictionary = {}
var _last_published_tick := -1
var _shiori_bytes: Dictionary = {}
var _session: WeakRef
var stages_15_17 := Stages15To17.new()

func prepare(store: Store, board_store: BoardStore = null, finish_bank: FinishBank = null, command_reload: CommandReload = null) -> String:
	if _store != null: return "first-arrival media bridge already owns immutable resources"
	if store == null or store.identity().is_empty(): return "first-arrival media bridge needs an authenticated native transfer store"
	var resources := store.descriptions()
	var boards: Dictionary = {}
	var shiori: Dictionary = {}
	if board_store != null:
		var read_desc := board_store.native_read_descriptor()
		var upload_desc := board_store.software_upload()
		if not read_desc.get("error") is String or not read_desc.error.is_empty() or not upload_desc.get("error") is String or not upload_desc.error.is_empty():
			return "first-arrival media bridge needs an authenticated board store"
		if read_desc.get("manifest_sha256") != BoardStore.MANIFEST_SHA or read_desc.get("lba") != 0x4954 or read_desc.get("sectors") != 9 or read_desc.get("destination") != 0x80162000 or not read_desc.get("payload") is PackedByteArray or read_desc.payload.size() != 18432 or Store._sha(read_desc.payload) != BoardStore.SOURCE_SHA:
			return "first-arrival board read descriptor differs from source"
		if upload_desc.get("rectangle") != [320,0,640,160] or not upload_desc.get("words") is PackedByteArray or upload_desc.words.size() != 204800 or Store._sha(upload_desc.words) != BoardStore.SOFTWARE_WORD_SHA:
			return "first-arrival board software upload descriptor differs from source"
		for spec: Array in [
			[BoardStore.BG_495D_ID, 0x495d, BoardStore.BG_495D_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_495D_WORD_SHA],
			[BoardStore.BG_4966_ID, 0x4966, BoardStore.BG_4966_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_4966_WORD_SHA],
			[BoardStore.BG_ACDD_ID, 0xacdd, BoardStore.BG_ACDD_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_ACDD_WORD_SHA],
			[BoardStore.BG_ACE6_ID, 0xace6, BoardStore.BG_ACE6_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_ACE6_WORD_SHA],
			[BoardStore.BG_ACF6_ID, 0xacf6, BoardStore.BG_ACF6_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_ACF6_WORD_SHA],
			[BoardStore.BG_ACD4_ID, 0xacd4, BoardStore.BG_ACD4_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_ACD4_WORD_SHA],
			[BoardStore.BG_ACEC_ID, 0xacec, BoardStore.BG_ACEC_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_ACEC_WORD_SHA],
			[BoardStore.BG_AD00_ID, 0xad00, BoardStore.BG_AD00_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_AD00_WORD_SHA],
			[BoardStore.BG_AD0B_ID, 0xad0b, BoardStore.BG_AD0B_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_AD0B_WORD_SHA],
			[BoardStore.BG_AD05_ID, 0xad05, BoardStore.BG_AD05_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_AD05_WORD_SHA],
			[BoardStore.BG_496A_ID, 0x496a, BoardStore.BG_496A_TRANSFER_SHA, [320,256,320,160], 1, BoardStore.BG_496A_WORD_SHA],
			[BoardStore.BG_4973_ID, 0x4973, BoardStore.BG_4973_TRANSFER_SHA, [704,0,320,160], 0, BoardStore.BG_4973_WORD_SHA],
		]:
			var bg_tr := board_store.background_transfer(spec[0])
			var bg_up := board_store.background_software_upload(spec[0])
			if not bg_tr.get("error") is String or not bg_tr.error.is_empty() or bg_tr.get("lba") != spec[1] or bg_tr.get("sectors") != 10 or bg_tr.get("destination") != 0x80162000 or not bg_tr.get("bytes") is PackedByteArray or bg_tr.bytes.size() != 20480 or Store._sha(bg_tr.bytes) != spec[2]:
				return "first-arrival names background transfer descriptor differs from source"
			if not bg_up.get("error") is String or not bg_up.error.is_empty() or bg_up.get("rectangle") != spec[3] or bg_up.get("buffer") != spec[4] or not bg_up.get("words") is PackedByteArray or bg_up.words.size() != 102400 or Store._sha(bg_up.words) != spec[5]:
				return "first-arrival names background software upload descriptor differs from source"
		var bg_descs := board_store.background_descriptions()
		if bg_descs.size() != 12 or not bg_descs.has(str(0xad05)) or not bg_descs.has(str(0x496a)) or not bg_descs.has(str(0x4973)) or not bg_descs.has(str(0xad0b)) or not bg_descs.has(str(0x495d)) or not bg_descs.has(str(0x4966)) or not bg_descs.has(str(0xacdd)) or not bg_descs.has(str(0xace6)) or not bg_descs.has(str(0xacf6)) or not bg_descs.has(str(0xacd4)) or not bg_descs.has(str(0xacec)) or not bg_descs.has(str(0xad00)):
			return "first-arrival names background descriptions are missing"
		resources.backgrounds.merge(bg_descs, true)
		var actor_descs := board_store.actor_descriptions()
		var expected_transfers := ["49:2149203968:43800","17:2149056512:44095","17:2149515264:44095","49:2149203968:43555","9:2149056512:43960","49:2149203968:43604","25:2149515264:43978","49:2149203968:43653","25:2149515264:44003","49:2149203968:43702","9:2149515264:44028","49:2149203968:43751","25:2149056512:44037","33:2149482496:44062","25:2149482496:43978","65:2149056512:44112",
			# Stages 11-14 reads (O_13 to the stage11 staging buffer, CUT_14, O_14, O_15, CUT_16, O_16).
			"65:2149416960:44112","49:2149203968:43849","9:2149515264:44177","33:2149482496:44186","49:2149203968:43898","25:2149482496:44219"]
		if actor_descs.size() != expected_transfers.size() or not actor_descs.has_all(expected_transfers):
			return "first-arrival stage 2..8 actor descriptions are missing"
		resources.actors.merge(actor_descs, true)
		shiori = shiori_area_bytes(board_store)
		if shiori.is_empty(): return "first-arrival stages 12-14 transfer differs from source"
		boards[str(0x4954)] = {"id":BOARD_ID,"sha256":BoardStore.SOURCE_SHA,"lba":0x4954,"sectors":9,"destination":0x80162000,"manifest_sha256":BoardStore.MANIFEST_SHA}
	var banks: Dictionary = {}
	if finish_bank != null:
		banks = finish_bank.descriptions()
		if finish_bank.identity() != FinishBank.TRACK_SHA or banks.size() != FinishBank.READS.size():
			return "first-arrival media bridge needs an authenticated stage21 bank reader"
	# Mode 0x34 command reload reads (CALROM and the slot-0 room); without the
	# installed role hud_read_start and the room read stay refused.
	var hud: Dictionary = {}
	var rooms: Dictionary = {}
	if command_reload != null:
		hud = command_reload.description("hud_calendar")
		var room := command_reload.description("room_spring")
		if command_reload.identity() != CommandReload.TRACK_SHA or hud.is_empty() or room.is_empty():
			return "first-arrival media bridge needs an authenticated command reload reader"
		rooms[str(room.lba)] = room
	var error: String = queue.prepare(resources.backgrounds,resources.actors,hud,boards,banks,rooms)
	if not error.is_empty(): return error
	if board_store != null:
		error = stages_15_17.prepare(board_store)
		if not error.is_empty(): return error
	_store = store
	_board_store = board_store
	_shiori_bytes = shiori
	_finish_bank = finish_bank
	_command_reload = command_reload
	return ""

## Identity of the bytes that landed at a RAM destination ({} when none):
## resource_id, sha256 and, for command reload reads, the primary file sha256.
func buffer_identity(destination: int) -> Dictionary:
	var record: Variant = _buffers.get(str(destination))
	if not record is Dictionary: return {}
	return {"resource_id":str(record.get("resource_id","")),"sha256":str(record.get("sha256","")),
		"primary_sha256":str(record.get("primary_sha256",""))}

static func shiori_area_bytes(board_store: BoardStore) -> Dictionary:
	# Authenticated stages 12-14 resources for ShioriActors' actor area; empty
	# unless the combined board pack is open and every transfer matches its pin.
	var result: Dictionary = {}
	if board_store == null or not board_store.stages_12_14_ready(): return result
	for spec: Array in BoardStore.STAGES_12_14_TRANSFERS:
		var item := board_store.stage_transfer(spec[0])
		if not item.get("error") is String or not item.error.is_empty() or Store._sha(item.bytes) != spec[3]: return {}
		result[spec[0]] = {"bytes":item.bytes,"sha256":spec[3]}
	return result

func bind_session(session: Session) -> String:
	if _store == null or session == null or not session.state is State:
		return "first-arrival media needs its prepared owning Session"
	if _session != null:
		return "" if _session.get_ref() == session else "first-arrival media belongs to another session; reset before rebinding"
	_session = weakref(session)
	return ""

func callbacks() -> Dictionary:
	var services := queue.callbacks()
	services["scene_reset"] = scene_reset
	services["background_decode"] = background_decode
	services["actor_palette_setup"] = actor_palette_setup
	services["board_read"] = board_read
	services["board_decode"] = board_decode
	services["board_scroll_draw"] = board_scroll_draw
	services["prologue_sine"] = Math.rsin
	services["arrival_draw"] = arrival_draw
	services["crossfade_draw"] = crossfade_draw
	services["prologue_palette_adjust"] = prologue_palette_adjust
	services["o12_backup"] = o12_backup
	services["stage_03_setup"] = stage_03_setup
	services["stage_03_to_04_setup"] = stage_03_to_04_setup
	services["stage_04_setup"] = stage_04_setup
	services["stage_04_blink_check"] = stage_04_blink_check
	services["stage_04_to_05_setup"] = stage_04_to_05_setup
	services["stage_05_arm_enter"] = stage_05_arm_enter
	services["stage_05_color_burst"] = stage_05_color_burst
	services["stage_05_cut06_setup"] = stage_05_cut06_setup
	services["stage_05_to_06_setup"] = stage_05_to_06_setup
	services["stage_06_cd_wait"] = stage_06_cd_wait
	services["stage_06_actor98_step"] = stage_06_actor98_step
	services["stage_06_lip_sync_yoshio"] = stage_06_lip_sync_yoshio
	services["stage_06_actor102_set"] = stage_06_actor102_set
	services["stage_06_actor99_turn"] = stage_06_actor99_turn
	services["stage_06_mouth_girl_a"] = stage_06_mouth_girl_a
	services["stage_06_actor103_set"] = stage_06_actor103_set
	services["stage_06_mouth_girl_b"] = stage_06_mouth_girl_b
	services["stage_06_to_07_setup"] = stage_06_to_07_setup
	services["stage_07_cd_wait"] = stage_07_cd_wait
	services["stage_07_lip_sync_line_11"] = stage_07_lip_sync_line_11
	services["stage_07_to_08_setup"] = stage_07_to_08_setup
	services["rei_banner_effect"] = rei_banner_effect
	services["rei_lines_effect"] = rei_lines_effect
	services["rei_lines15_effect"] = rei_lines15_effect
	services["rei_line17_effect"] = rei_line17_effect
	services["shiori_effect"] = shiori_effect
	services["lines20_37_effect"] = stages_15_17.effect
	services["prologue_closing_effect"] = closing_effect
	services["cd_audio_state"] = AudioRules.check_cd
	services["audio_command"] = stage_05_audio_command
	services["finish_cd_reset"] = queue.cd_command_reset
	services["finish_bank_reset"] = finish_bank_reset
	services["finish_bank_load"] = finish_bank_load
	return services

# --- stage 21: prologue_finish_run case 0 and the command-reload bank poll ---
static func _finish_case0(state: State) -> bool:
	return state.game_mode == 0x15 and state.mode_stage == 21 and state.mode_step == 0 and state.substep == 0

## The source bank-loader globals (D_800B3D20..D_800B3D54, audio_bgm_set,
## audio_bgm_sequence) live in one record, state.opening.audio_bank_loader,
## which is a NativeBankLoader checkpoint (T-29012). The reset, load start and
## poll below run NativeBankLoader's frame_render_audio.c rules on it.
## AudioService also holds a NativeBankLoader, but nothing on the opening route
## drives that instance. NativeBankLoader names the a == 0 read destinations
## by role; these are their source addresses.
const FINISH_BANK_ROLES := {"bank0_se_header":0x80010000,"bank0_se_body":0x801b8000,
	"bank0_sequence":0x8001c000,"bank0_music_body":0x801c8000,"bank0_music_header":0x80012000}
const FINISH_BANK_RECORD_ERROR := "stage21 bank loader record is not a NativeBankLoader checkpoint"

static func _finish_loader(state: State, required: bool) -> BankLoader:
	var loader := BankLoader.new()
	var record: Variant = state.opening.get("audio_bank_loader")
	if record == null and not required: return loader
	if not record is Dictionary or not loader.restore(record).is_empty(): return null
	return loader

## frame_render_audio.c::audio_bank_state_reset (NativeBankLoader.reset).
## The opening's role record state.opening.audio_bank is invalidated as well:
## after the reset no bank load is pending or ready (D_800B3D50 = 0,
## audio_bgm_set = 0xFF), so no BGM consumer may treat an earlier role's bank
## as ready. The epoch keeps counting so stale role completions stay stale.
func finish_bank_reset(state: State, result: Session.FrameResult) -> void:
	if not _finish_case0(state):
		result.error = "audio_bank_state_reset is outside prologue_finish_run case 0"; return
	var loader := _finish_loader(state,false)
	if loader == null:
		result.error = FINISH_BANK_RECORD_ERROR; return
	loader.reset()
	state.opening["audio_bank_loader"] = loader.checkpoint()
	var previous: Variant = state.opening.get("audio_bank")
	var epoch: int = int(previous.epoch) if previous is Dictionary and previous.get("epoch") is int else 0
	state.opening["audio_bank"] = {"epoch":epoch+1,"role":"","ready":false}

## audio_bank_load_start(0, b, c, d, e, f) through NativeBankLoader.load_start,
## a == 0 branch with an empty LBA cache: five nowait reads in source order
## (b, c, f, e, d). Cache hits cannot occur after the same-frame reset. The CD
## done count is the queue's completed reads since cd_command_reset.
func finish_bank_load(state: State, args: Array, result: Session.FrameResult) -> void:
	if _finish_bank == null:
		result.error = FINISH_BANK_UNPREPARED; return
	if not _finish_case0(state) or not Store._same(args,FinishBank.SOURCE_ARGS):
		result.error = "stage21 bank load differs from prologue_finish_run case 0"; return
	var loader := _finish_loader(state,true)
	var media: Variant = state.opening.get("media")
	if loader == null or loader.data.load_requested != 0 or loader.data.se_header_0 != 0 or loader.data.music_header_0 != 0 or not media is Dictionary or not media.requests.is_empty() or media.has("bank_pending"):
		result.error = "stage21 bank load requires the same-frame cd_command_reset and audio_bank_state_reset"; return
	var binding := {"bank":args[0],"se_header":args[1],"se_body":args[2],"music_header":args[3],"music_body":args[4],"sequence":args[5]}
	var out := loader.load_start(binding,queue.bank_reads_done(state))
	if not out.error.is_empty():
		result.error = out.error; return
	var reads: Array = out.operations.filter(func(op: Dictionary) -> bool: return op.kind == "cd_read_nowait")
	if out.operations.is_empty() or out.operations[0] != {"kind":"cd_destination_base","role":"bank0_se_body"} or reads.size() != FinishBank.READS.size():
		result.error = "stage21 bank read order differs from audio_bank_load_start"; return
	for i in range(reads.size()):
		var op: Dictionary = reads[i]
		var row: Array = FinishBank.READS[i]
		if op.lba != row[2] or op.sectors != row[3] or FINISH_BANK_ROLES.get(op.destination) != row[4]:
			result.error = "stage21 bank read order differs from audio_bank_load_start"; return
		queue.bank_read(state,op.sectors,row[4],op.lba,result)
		if not result.error.is_empty(): return
	state.opening["audio_bank_loader"] = loader.checkpoint()
	# native-bank0: the same reads land over the live bank0 header/score on the
	# measured schedule (prologue_bank0_audio.gd), owned by the native engine.
	var record: Variant = state.opening.get("bank0_audio")
	if record is Dictionary and record.get("phase") == "stage16" and bank0_owner.is_valid():
		record["phase"] = "loading"; record["load_frame"] = int(state.frame_count)
		result.audio.append({"kind":"native_bank0_load"})

## audio_bank_load_poll (NativeBankLoader.load_poll) for the stage21 bank,
## called by settings_command_screen_reload_calendar substep 0. It returns the
## source -1 until cd_done_count() - D_800B3D20 reaches 5, then queues
## audio_bank_transfer_start's 0x11. With a native bank0 owner (bank0_owner)
## the 0x11 goes to the engine and later polls see slot readiness; without one
## the frame is refused there (FINISH_BANK_TRANSFER_UNSUPPORTED).
func finish_bank_poll(state: State, result: Session.FrameResult) -> int:
	if state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step != 1 or state.substep != 0:
		result.error = "stage21 bank poll is outside the command reload calendar step"; return -1
	var loader := _finish_loader(state,true)
	if loader == null or loader.data.load_requested != 1 or loader.data.bank != 0 or not loader.data.cd_baseline is int:
		result.error = "stage21 bank poll has no owned audio_bank_load_start"; return -1
	var record: Variant = state.opening.get("bank0_audio")
	var native: String = FINISH_BANK_TRANSFER_UNSUPPORTED
	if bank0_owner.is_valid() and record is Dictionary and record.get("phase") in ["loading","transferred"]:
		var owner: Variant = bank0_owner.call(state) if record.phase == "loading" else ""
		native = owner if owner is String else FINISH_BANK_TRANSFER_UNSUPPORTED
	if loader.data.transfer_polls != 0 and (not native.is_empty() or record.get("phase") != "transferred"):
		result.error = native if not native.is_empty() else FINISH_BANK_TRANSFER_UNSUPPORTED; return -1
	# cd_done_count: the queue's completed prefix, bounded by the measured
	# landing (the port queue completes reads the next frame; the drive does not).
	var done: int = queue.bank_reads_done(state)
	if native.is_empty(): done = mini(done, Bank0.reads_done_at(int(state.frame_count) - int(record.load_frame)))
	# audio_bgm_bank_ready(1): the native driver's slot-1 transfer state, which
	# the owner also checks against the measured second pump after the 0x11 frame.
	var ready := false
	if native.is_empty() and record.get("phase") == "transferred":
		var slot: Variant = bank0_slot_state.call(state) if bank0_slot_state.is_valid() else null
		if not slot is Dictionary or not slot.get("error") is String:
			result.error = "bank0 slot poll has no native transfer owner"; return -1
		if not slot.error.is_empty():
			result.error = slot.error; return -1
		if slot.get("ready") != (int(state.frame_count) - int(record.transfer_frame) >= Bank0.TRANSFER_READY_FRAMES):
			result.error = "native slot1 readiness differs from the measured ready frame"; return -1
		ready = slot.ready
	var out := loader.load_poll(done,ready,ready)
	if not out.error.is_empty():
		result.error = out.error; return -1
	if out.operations.has({"kind":"audio_command","command":0x11}):
		if not native.is_empty():
			result.error = native; return -1
		if out.operations != [{"kind":"cd_destination_base","role":"bank0_se_body"},{"kind":"audio_command","command":0x11}]:
			result.error = "stage21 bank poll differs from audio_bank_load_poll"; return -1
		AudioRulesBank0.bank0_transfer(state)
		record["phase"] = "transferred"; record["transfer_frame"] = int(state.frame_count)
		result.audio.append({"kind":"native_audio_command","command":0x11})
	elif out.status == 1:
		# 0x22/0x21/0x2F; settings_command_screen_reload_calendar then calls
		# audio_bank_start(0,0) in this frame, which queues them again with 0x200.
		if out.operations != [{"kind":"audio_command","command":0x22},{"kind":"audio_command","command":0x21},{"kind":"audio_command","command":0x2f}]:
			result.error = "stage21 bank poll differs from audio_bank_load_poll"; return -1
		record["ready_frame"] = int(state.frame_count)
	elif not out.operations.is_empty():
		result.error = "stage21 bank poll differs from audio_bank_load_poll"; return -1
	# D_800B3D4C (transfer_polls) and D_800B3D54 persist in the loader record.
	state.opening["audio_bank_loader"] = loader.checkpoint()
	return out.status

func scene_reset(state: State, result: Session.FrameResult) -> void:
	queue.scene_reset(state,result)
	if not result.error.is_empty(): return
	state.opening["arrival_media"] = {"background_brightness":128,"back_clear":true,"actor_initialized":false,"manifest_sha256":_store.identity()}
	for idx in [96, 97, 98, 99, 100, 101, 102, 103]:
		state.opening.erase("actor%d" % idx)
		state.opening.erase("actor%d_draw_flags" % idx)
		state.opening.erase("actor%d_source_draw" % idx)
	state.opening.erase("o06_batches")
	state.opening.erase("o07_batches")
	state.opening.erase("bg_layer_0")
	state.opening.erase("board_media")
	state.opening.erase("names_media")
	state.opening.erase("bg_fade_amount")
	state.opening.erase("bg_brightness")
	state.opening.erase("D_801220E0")
	state.opening.erase("D_801220E1")
	state.opening.erase("D_801220E2")
	state.opening.erase("petals_source_draw")

func background_decode(state: State, slot: int, result: Session.FrameResult) -> int:
	if state.game_mode == 0x15 and state.mode_stage == 0:
		return queue.background_decode(state,slot,result)
	if _board_store == null or slot != 1 or state.game_mode != 0x15:
		result.error = "first-arrival background decode is outside its source stage"
		return -1
	if state.mode_stage == 11 and state.mode_step == 0 and state.substep == 6:
		return _line17_decode(state,result)
	if state.mode_stage == 1 and state.mode_step == 1 and state.substep == 5:
		if not state.opening.get("board_media") is Dictionary or state.opening.board_media.get("decoded") != true or state.opening.get("prologue_scene_counter") != 320:
			result.error = "first-arrival 0x495D decode lacks completed board scroll state"
			return -1
		var buf := queue.background_decode(state,slot,result)
		if not result.error.is_empty() or buf != 0:
			if result.error.is_empty(): result.error = "first-arrival 0x495D decode did not select buffer 0"
			return -1
		var batch_key := JSON.stringify([state.opening.media.epoch,BoardStore.BG_495D_ID,BoardStore.BG_495D_TRANSFER_SHA,BoardStore.BG_495D_WORD_SHA,[704,0,320,160]]).sha256_text()
		state.opening["names_media"] = {
			"bg_show_1":0,
			"active_draw":"board_scroll_transition",
			"buffer_0":{"lba":0x495d,"resource_id":BoardStore.BG_495D_ID,"transfer_sha256":BoardStore.BG_495D_TRANSFER_SHA,"word_sha256":BoardStore.BG_495D_WORD_SHA,"decode_rect":[704,0,320,160],"upload_batch_key":batch_key},
			"manifest_sha256":BoardStore.MANIFEST_SHA,
		}
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_495D_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":BoardStore.BG_495D_WORD_SHA})
		return 0
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 1:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or names_media.has("buffer_1"):
			result.error = "first-arrival 0x4966 decode requires buffer 0 active and buffer 1 free"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_4966_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,BoardStore.BG_4966_ID,BoardStore.BG_4966_TRANSFER_SHA,BoardStore.BG_4966_WORD_SHA,[320,256,320,160]]).sha256_text()
		names_media["buffer_1"] = {"lba":0x4966,"resource_id":BoardStore.BG_4966_ID,"transfer_sha256":BoardStore.BG_4966_TRANSFER_SHA,"word_sha256":BoardStore.BG_4966_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":batch_key}
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_4966_ID,"buffer":1})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_4966_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":BoardStore.BG_4966_WORD_SHA})
		return 1
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 8:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or not names_media.get("buffer_1") is Dictionary:
			result.error = "first-arrival 0xacdd decode requires dual buffers in stage 2 substep 8"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACDD_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACDD_ID,BoardStore.BG_ACDD_TRANSFER_SHA,BoardStore.BG_ACDD_WORD_SHA,[320,256,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACDD_ID,"resource_sha256":BoardStore.BG_ACDD_TRANSFER_SHA,"buffer":1}
		names_media["buffer_1"] = {"lba":0xacdd,"resource_id":BoardStore.BG_ACDD_ID,"transfer_sha256":BoardStore.BG_ACDD_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACDD_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 1
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACDD_ID,"buffer":1})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACDD_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":BoardStore.BG_ACDD_WORD_SHA})
		return 1
	if state.mode_stage == 3 and state.mode_step == 0 and state.substep == 4:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 1 or not names_media.get("buffer_1") is Dictionary or names_media.buffer_1.get("lba") != 0xacdd:
			result.error = "first-arrival 0xace6 decode requires buffer 1 active with 0xacdd in stage 3 substep 4"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACE6_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACE6_ID,BoardStore.BG_ACE6_TRANSFER_SHA,BoardStore.BG_ACE6_WORD_SHA,[704,0,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACE6_ID,"resource_sha256":BoardStore.BG_ACE6_TRANSFER_SHA,"buffer":0}
		names_media["buffer_0"] = {"lba":0xace6,"resource_id":BoardStore.BG_ACE6_ID,"transfer_sha256":BoardStore.BG_ACE6_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACE6_WORD_SHA,"decode_rect":[704,0,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 0
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACE6_ID,"buffer":0})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACE6_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":BoardStore.BG_ACE6_WORD_SHA})
		return 0
	if state.mode_stage == 5 and state.mode_step == 0 and state.substep == 1:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 1 or not names_media.get("buffer_1") is Dictionary or names_media.buffer_1.get("lba") != 0xacdd:
			result.error = "first-arrival 0xacf6 decode requires buffer 1 active with 0xacdd in stage 5 substep 1"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACF6_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACF6_ID,BoardStore.BG_ACF6_TRANSFER_SHA,BoardStore.BG_ACF6_WORD_SHA,[704,0,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACF6_ID,"resource_sha256":BoardStore.BG_ACF6_TRANSFER_SHA,"buffer":0}
		names_media["buffer_0"] = {"lba":0xacf6,"resource_id":BoardStore.BG_ACF6_ID,"transfer_sha256":BoardStore.BG_ACF6_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACF6_WORD_SHA,"decode_rect":[704,0,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 0
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACF6_ID,"buffer":0})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACF6_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":BoardStore.BG_ACF6_WORD_SHA})
		return 0
	if state.mode_stage == 5 and state.mode_step == 0 and state.substep == 5:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or names_media.buffer_0.get("lba") != 0xacf6:
			result.error = "first-arrival 0xacd4 decode requires buffer 0 active with 0xacf6 in stage 5 substep 5"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACD4_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACD4_ID,BoardStore.BG_ACD4_TRANSFER_SHA,BoardStore.BG_ACD4_WORD_SHA,[320,256,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACD4_ID,"resource_sha256":BoardStore.BG_ACD4_TRANSFER_SHA,"buffer":1}
		names_media["buffer_1"] = {"lba":0xacd4,"resource_id":BoardStore.BG_ACD4_ID,"transfer_sha256":BoardStore.BG_ACD4_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACD4_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 1
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACD4_ID,"buffer":1})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACD4_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":BoardStore.BG_ACD4_WORD_SHA})
		return 1
	if state.mode_stage == 6 and state.mode_step == 0 and state.substep == 16:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 1 or not names_media.get("buffer_1") is Dictionary or names_media.buffer_1.get("lba") != 0xacd4:
			result.error = "first-arrival 0xacec decode requires buffer 1 active with 0xacd4 in stage 6 substep 16"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACEC_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed read"
			return -1
		if request.status == "retry":
			return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACEC_ID,BoardStore.BG_ACEC_TRANSFER_SHA,BoardStore.BG_ACEC_WORD_SHA,[704,0,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACEC_ID,"resource_sha256":BoardStore.BG_ACEC_TRANSFER_SHA,"buffer":0}
		names_media["buffer_0"] = {"lba":0xacec,"resource_id":BoardStore.BG_ACEC_ID,"transfer_sha256":BoardStore.BG_ACEC_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACEC_WORD_SHA,"decode_rect":[704,0,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 0
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACEC_ID,"buffer":0})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACEC_ID,"buffer":0,"decode_rect":[704,0,320,160],"word_sha256":BoardStore.BG_ACEC_WORD_SHA})
		return 0
	if state.mode_stage == 7 and state.mode_step == 0 and state.substep == 7:
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or names_media.buffer_0.get("lba") != 0xacec:
			result.error = "first-arrival 0xad00 decode requires buffer 0 active with 0xacec in stage 7 substep 7"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_AD00_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed AD00 read"
			return -1
		if request.status == "retry": return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_AD00_ID,BoardStore.BG_AD00_TRANSFER_SHA,BoardStore.BG_AD00_WORD_SHA,[320,256,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_AD00_ID,"resource_sha256":BoardStore.BG_AD00_TRANSFER_SHA,"buffer":1}
		names_media["buffer_1"] = {"lba":0xad00,"resource_id":BoardStore.BG_AD00_ID,"transfer_sha256":BoardStore.BG_AD00_TRANSFER_SHA,"word_sha256":BoardStore.BG_AD00_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 1
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_AD00_ID,"buffer":1})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_AD00_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":BoardStore.BG_AD00_WORD_SHA})
		return 1
	if state.mode_stage == 10 and state.mode_step == 0 and state.substep == 8:
		# dec_bg_decode_to_vram(1) fills the hidden buffer; the shown buffer
		# is 0 after stage9/substep11's dec_bg_show_switch(1).
		var names_media: Variant = state.opening.get("names_media")
		if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or names_media.buffer_0.get("lba") != 0xacec:
			result.error = "first-arrival stage10 0xacf6 decode requires buffer 0 shown with 0xacec"
			return -1
		var media: Variant = state.opening.get("media")
		var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
		if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_ACF6_ID or request.get("status") not in ["ready","retry"]:
			result.error = "opening background decode has no matching completed ACF6 read"
			return -1
		if request.status == "retry": return -1
		var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_ACF6_ID,BoardStore.BG_ACF6_TRANSFER_SHA,BoardStore.BG_ACF6_WORD_SHA,[320,256,320,160]]).sha256_text()
		media.decoded["1"] = {"resource_id":BoardStore.BG_ACF6_ID,"resource_sha256":BoardStore.BG_ACF6_TRANSFER_SHA,"buffer":1}
		names_media["buffer_1"] = {"lba":0xacf6,"resource_id":BoardStore.BG_ACF6_ID,"transfer_sha256":BoardStore.BG_ACF6_TRANSFER_SHA,"word_sha256":BoardStore.BG_ACF6_WORD_SHA,"decode_rect":[320,256,320,160],"upload_batch_key":batch_key}
		names_media["bg_show_1"] = 1
		state.opening["names_media"] = names_media
		result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_ACF6_ID,"buffer":1})
		result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_ACF6_ID,"buffer":1,"decode_rect":[320,256,320,160],"word_sha256":BoardStore.BG_ACF6_WORD_SHA})
		return 1
	if state.mode_stage in [12,14] and state.mode_step == 0 and state.substep == (3 if state.mode_stage == 12 else 6):
		return _shiori_decode(state,result)
	if state.mode_stage in [15,16,17] and state.mode_step == 0 and STAGES_15_17_DECODES.has(state.mode_stage) and state.substep == STAGES_15_17_DECODES[state.mode_stage][0]:
		return _stages_15_17_decode(state,result)
	result.error = "first-arrival background decode is outside its source stage"
	return -1

# Stage15 case34 (P_01_04), stage16 case11 (P_01_10), stage17 case3 (P_01_12):
# [substep, lba, resource, transfer sha, word sha].
const STAGES_15_17_DECODES := {
	15:[34,0xacd4,BoardStore.BG_ACD4_ID,BoardStore.BG_ACD4_TRANSFER_SHA,BoardStore.BG_ACD4_WORD_SHA],
	16:[11,0x496a,BoardStore.BG_496A_ID,BoardStore.BG_496A_TRANSFER_SHA,BoardStore.BG_496A_WORD_SHA],
	17:[3,0x4973,BoardStore.BG_4973_ID,BoardStore.BG_4973_TRANSFER_SHA,BoardStore.BG_4973_WORD_SHA]}

func _line17_decode(state: State, result: Session.FrameResult) -> int:
	# prologue_line_17_run case6 (prologue.c:1797): dec_bg_decode_to_vram(1)
	# fills buffer 1 - dec_bg_show[1] and dec_bg_show_set shows it. Stage10
	# left ACF6 shown in buffer 1, so P_01_08 lands in buffer 0 (704,0), which
	# stages 12-14 draw until their own decodes. After this substep's
	# dec_bg_reset (a failed decode, prologue.c:1798) both cache LBAs are 0
	# and dec_bg_show[1] is 0, so the retried decode lands in buffer 1
	# (320,256) over ACF6; stage12's decode guard then refuses that state.
	var names_media: Variant = state.opening.get("names_media")
	var reset_here: bool = names_media is Dictionary and names_media.get("dec_bg_reset_at") is Array and names_media.dec_bg_reset_at.slice(0,2) == [state.mode_stage,state.substep]
	var nominal: bool = names_media is Dictionary and not reset_here and names_media.get("bg_show_1") == 1 and names_media.get("buffer_1",{}).get("lba") == 0xacf6
	var after_reset: bool = reset_here and names_media.get("bg_show_1") == 0 and names_media.get("buffer_0",{}).get("lba") == 0 and names_media.get("buffer_1",{}).get("lba") == 0
	if not nominal and not after_reset:
		result.error = "first-arrival stage11 0xad05 decode requires buffer 1 shown with 0xacf6 or this substep's dec_bg_reset"
		return -1
	var media: Variant = state.opening.get("media")
	var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
	if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != BoardStore.BG_AD05_ID or request.get("status") not in ["ready","retry"] or request.get("epoch") != media.get("epoch"):
		result.error = "opening background decode has no matching completed AD05 read"
		return -1
	if request.status == "retry": return -1
	var buffer: int = 1-int(names_media.bg_show_1)
	var rect: Array = BoardStore.SLOT1_BUFFER_RECTS[buffer]
	var batch_key := JSON.stringify([media.epoch,request.request_id,BoardStore.BG_AD05_ID,BoardStore.BG_AD05_TRANSFER_SHA,BoardStore.BG_AD05_WORD_SHA,rect]).sha256_text()
	media.decoded["1"] = {"resource_id":BoardStore.BG_AD05_ID,"resource_sha256":BoardStore.BG_AD05_TRANSFER_SHA,"buffer":buffer}
	names_media["buffer_%d" % buffer] = {"lba":0xad05,"resource_id":BoardStore.BG_AD05_ID,"transfer_sha256":BoardStore.BG_AD05_TRANSFER_SHA,"word_sha256":BoardStore.BG_AD05_WORD_SHA,"decode_rect":rect.duplicate(),"request_id":request.request_id,"upload_batch_key":batch_key}
	names_media["bg_show_1"] = buffer
	names_media.erase("dec_bg_reset_at")
	result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":BoardStore.BG_AD05_ID,"buffer":buffer})
	result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":BoardStore.BG_AD05_ID,"buffer":buffer,"decode_rect":rect.duplicate(),"word_sha256":BoardStore.BG_AD05_WORD_SHA})
	return buffer

func _stages_15_17_decode(state: State, result: Session.FrameResult) -> int:
	# dec_bg_decode_to_vram(1) decodes into buffer 1 - dec_bg_show[1] and
	# returns it; dec_bg_show_set(1, it) shows it. The shown buffer is the
	# live one: stage14 leaves ACEC shown in buffer 1, and a failed decode's
	# dec_bg_reset zeroes it.
	var spec: Array = STAGES_15_17_DECODES[state.mode_stage]
	var names_media: Variant = state.opening.get("names_media")
	if not names_media is Dictionary or names_media.get("bg_show_1") not in [0,1]:
		result.error = "first-arrival stage%d 0x%x decode has no slot1 show state" % [state.mode_stage,spec[1]]
		return -1
	var media: Variant = state.opening.get("media")
	var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
	if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != spec[2] or request.get("status") not in ["ready","retry"] or request.get("epoch") != media.get("epoch"):
		result.error = "opening background decode has no matching completed %X read" % spec[1]
		return -1
	if request.status == "retry": return -1
	var buffer: int = 1-int(names_media.bg_show_1)
	var rect: Array = BoardStore.SLOT1_BUFFER_RECTS[buffer]
	var batch_key := JSON.stringify([media.epoch,request.request_id,spec[2],spec[3],spec[4],rect]).sha256_text()
	media.decoded["1"] = {"resource_id":spec[2],"resource_sha256":spec[3],"buffer":buffer}
	names_media["buffer_%d" % buffer] = {"lba":spec[1],"resource_id":spec[2],"transfer_sha256":spec[3],"word_sha256":spec[4],"decode_rect":rect.duplicate(),"upload_batch_key":batch_key,"request_id":request.request_id}
	names_media["bg_show_1"] = buffer
	result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":spec[2],"buffer":buffer})
	result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":spec[2],"buffer":buffer,"decode_rect":rect.duplicate(),"word_sha256":spec[4]})
	return buffer

func board_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	if _board_store == null:
		result.error = "first-arrival board resource is not prepared"
		return
	if state.game_mode != 0x15 or state.mode_stage != 1 or state.mode_step != 0 or state.substep != 0 or state.opening.get("D_800B3DB0") != 1 or state.opening.get("actor96_draw_flags") != 0xa4 or state.opening.get("prologue_hud_live",{}).get("date_visible") != true:
		result.error = "first-arrival board read is outside its source stage"
		return
	queue.board_read(state,sectors,destination,lba,result)
	if not result.error.is_empty(): return
	state.opening["board_media"] = {"read_requested":true,"lba":0x4954,"sectors":9,"destination":0x80162000,"decoded":false,"manifest_sha256":BoardStore.MANIFEST_SHA}

func board_decode(state: State, result: Session.FrameResult) -> void:
	if _board_store == null:
		result.error = "first-arrival board resource is not prepared"
		return
	if state.game_mode != 0x15 or state.mode_stage != 1 or state.mode_step != 0 or state.substep != 1 or state.opening.get("D_800B3DB0") != 1 or state.opening.get("actor96_draw_flags") != 0 or state.opening.has("actor96") or not state.opening.get("prologue_petals") is Array or state.opening.prologue_petals.size() != 8 or state.opening.get("prologue_hud_live",{}).get("date_visible") != false:
		result.error = "first-arrival board decode is outside its source stage"
		return
	var board_media: Variant = state.opening.get("board_media")
	if not board_media is Dictionary or board_media.get("read_requested") != true or board_media.get("decoded") != false:
		result.error = "first-arrival board decode lacks a pending board read"
		return
	queue.board_decode(state,result)
	if not result.error.is_empty(): return
	var batch_key := JSON.stringify([state.opening.media.epoch,BoardStore.SOURCE_SHA,BoardStore.SOFTWARE_WORD_SHA,[320,0,640,160],[5,14,15]]).sha256_text()
	state.opening["board_media"] = {"read_requested":true,"lba":0x4954,"sectors":9,"destination":0x80162000,"decoded":true,"decode_rect":[320,0,640,160],"move_255_line":[5,14,15],"upload_batch_key":batch_key,"manifest_sha256":BoardStore.MANIFEST_SHA}
	result.presentation.append({"kind":"first_arrival_board_upload","batch_key":batch_key,"decode_rect":[320,0,640,160],"move_call":[5,14,15],"word_sha256":BoardStore.SOFTWARE_WORD_SHA})

func actor_palette_setup(state: State, result: Session.FrameResult) -> void:
	if state.game_mode != 0x15 or state.mode_stage != 0 or state.mode_step != 1 or state.substep != 4:
		result.error = "first-arrival actor initialization is outside its source stage"
		return
	queue.actor_palette_setup(state,result)
	if not result.error.is_empty(): return
	state.opening["actor96"] = _store.initial_actor()
	state.opening["actor96_draw_flags"] = 0x24
	state.opening.arrival_media.actor_initialized = true
	var uploads := _store.upload_requests()
	var batch_key := JSON.stringify([state.opening.media.epoch,Store.ACTOR_SHA,uploads]).sha256_text()
	state.opening.arrival_media["upload_batch_key"] = batch_key
	for index in uploads.size():
		result.presentation.append({"kind":"first_arrival_native_upload","batch_key":batch_key,"ordinal":index,"request":uploads[index]})

func before_line0(state: State, line: int, result: Session.FrameResult) -> void:
	if line == 0 and state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 2 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary:
		state.opening.arrival_media.back_clear = false
		return
	if line == 1 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 1 and state.mode_step == 1 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("decoded") == true and state.opening.get("prologue_scene_counter") in [0, 1]:
		state.opening.erase("prologue_text_cleared")
		return
	if line == 2 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("names_media") is Dictionary and state.opening.names_media.get("buffer_0") is Dictionary and state.opening.get("prologue_scene_counter") == 0:
		state.opening.erase("prologue_text_cleared")
		return
	if line == 3 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 3 and state.mode_step == 0 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("bg_layer_0") is Dictionary and state.opening.get("actor96") is Dictionary:
		state.opening.erase("prologue_text_cleared")
		return
	if line == 4 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 4 and state.mode_step == 0 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("bg_layer_0") is Dictionary and not state.opening.has("actor96"):
		state.opening.erase("prologue_text_cleared")
		return
	if line == 5 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 6 and state.mode_step == 0 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("bg_layer_0") is Dictionary and state.opening.get("actor96") is Dictionary and state.opening.get("actor99") is Dictionary:
		state.opening.erase("prologue_text_cleared")
		return
	if line in [6, 7, 8, 9, 10] and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 6 and state.mode_step == 0 and state.substep == (line - 5) * 2 + 1 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("bg_layer_0") is Dictionary and state.opening.get("actor96") is Dictionary and state.opening.get("actor99") is Dictionary:
		state.opening.erase("prologue_text_cleared")
		return
	if line == 11 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 7 and state.mode_step == 0 and state.substep == 0 and state.opening.get("arrival_media") is Dictionary and state.opening.arrival_media.get("back_clear") == false and state.opening.get("bg_layer_0") is Dictionary and state.opening.get("actor96") is Dictionary and state.opening.actor96.get("profile") == "o07_yoshio_body":
		state.opening.erase("prologue_text_cleared")
		return
	if line == 12 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 8 and state.mode_step == 0 and state.substep == 2 and state.opening.get("actor96",{}).get("profile") == "o08_rei_banner" and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_08_ID:
		state.opening.erase("prologue_text_cleared")
		return
	if line in [13,14] and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 9 and state.mode_step == 0 and state.substep == (0 if line == 13 else 7) and state.opening.get("actor96",{}).get("profile") == "o09_rei_entrance" and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_09_ID:
		state.opening.erase("prologue_text_cleared")
		return
	if line in [15,16] and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 10 and state.mode_step == 0 and state.substep == (0 if line == 15 else 3) and state.opening.get("actor96",{}).get("profile") == "o11_rei_lines15" and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_07_ID:
		state.opening.erase("prologue_text_cleared")
		return
	if line == 17 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == 11 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96",{}).get("profile") == "o12_rei_line17" and state.opening.get("actor96",{}).get("flags") == 0 and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_12_ID:
		state.opening.erase("prologue_text_cleared")
		return
	if line in [18,19] and not _shiori_bytes.is_empty() and state.game_mode == 0x15 and state.mode_stage == line-5 and state.mode_step == 0 and state.substep == 0 and (
		(line == 18 and state.opening.get("actor97",{}).get("profile") == "o14_shiori_mouth" and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_14_ID)
		or (line == 19 and state.opening.get("actor96",{}).get("profile") == "o15_shiori_line19" and state.opening.get("bg_layer_0",{}).get("attribute") == 0x80000000)):
		state.opening.erase("prologue_text_cleared")
		return
	if _stages_15_17_line_allowed(state,line):
		state.opening.erase("prologue_text_cleared")
		return
	# Stage18 cases 0/3/5/7 (lines 38-41) and stage20 case0 (line 42).
	var closing_substep: int = {38:0,39:3,40:5,41:7,42:0}.get(line,-1)
	if closing_substep >= 0 and _board_store != null and state.game_mode == 0x15 and state.mode_stage == (20 if line == 42 else 18) and state.mode_step == 0 and state.substep == closing_substep and _closing_owner_error(state).is_empty():
		state.opening.erase("prologue_text_cleared")
		return
	result.error = "first-arrival line setup is outside the source dispatch"

func _stages_15_17_line_allowed(state: State, line: int) -> bool:
	# prologue_lines_20_to_31_run / 32_to_36 / line_37 start substeps; 100
	# and 101 are the built player answers of cases 12 and 16.
	if _board_store == null or state.game_mode != 0x15 or state.mode_step != 0: return false
	var starts := {15:{0x14:0,0x15:2,0x16:6,0x17:8,0x18:10,100:12,0x19:14,101:16,0x1a:18,0x1b:20,0x1c:22,0x1d:24,0x1e:27,0x1f:29},
		16:{0x20:0,0x21:2,0x22:4,0x23:6,0x24:8},17:{0x25:0}}
	if not starts.get(state.mode_stage,{}).has(line) or starts[state.mode_stage][line] != state.substep: return false
	match state.mode_stage:
		15: return Stages15To17.stage15_entry_owner(state.opening)
		16: return Stages15To17.stage16_owner(state.opening)
		_: return Stages15To17.stage17_owner(state.opening)

func prologue_palette_adjust(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival palette adjust is outside its prepared scope"
		return
	var expected_id := ""
	var expected_sha := ""
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 6:
		expected_id = BoardStore.CUT_12_ID
		expected_sha = BoardStore.CUT_12_SHA
	elif state.mode_stage == 4 and state.mode_step == 0 and state.substep == 0:
		expected_id = BoardStore.CUT_05_ID
		expected_sha = BoardStore.CUT_05_SHA
	elif state.mode_stage == 4 and state.mode_step == 0 and state.substep == 4:
		expected_id = BoardStore.CUT_12_ID
		expected_sha = BoardStore.CUT_12_SHA
	else:
		result.error = "first-arrival palette adjust is outside its source stage"
		return
	var media: Dictionary = state.opening.media
	var req_id: String = str(media.get("actor_pending",{}).get(str(0x801a4000),""))
	var request: Variant = media.get("requests",{}).get(req_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != expected_id or request.get("resource_sha256") != expected_sha:
		result.error = "first-arrival palette adjust has no completed cut NBN read at 0x801a4000"
		return
	var batch_key := JSON.stringify([media.epoch,req_id,expected_id,expected_sha,[512,0,192,256],[0,494,256,2]]).sha256_text()
	state.opening["cut_media"] = {"resource_id":expected_id,"sha256":expected_sha,"request_id":req_id,"upload_batch_key":batch_key}
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":batch_key,"resource_id":expected_id,"sha256":expected_sha})

func o12_backup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 2 or state.mode_step != 0 or state.substep != 8 or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival O_12 backup is outside stage 2 substep 8"
		return
	var media: Dictionary = state.opening.media
	var req_id: String = str(media.get("actor_pending",{}).get(str(0x80180000),""))
	var request: Variant = media.get("requests",{}).get(req_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != BoardStore.O_12_ID or request.get("resource_sha256") != BoardStore.O_12_SHA:
		result.error = "first-arrival O_12 backup has no completed O_12 read at 0x80180000"
		return
	if state.opening.get("names_media") is Dictionary:
		state.opening.names_media["bg_show_1"] = 1
	result.presentation.append({"kind":"first_arrival_o12_backup","resource_id":BoardStore.O_12_ID,"sha256":BoardStore.O_12_SHA,"bytes":34816})

func stage_03_setup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 3 or state.mode_step != 0 or state.substep != 0 or not state.opening.get("cut_media") is Dictionary or state.opening.cut_media.get("resource_id") != BoardStore.CUT_12_ID:
		result.error = "first-arrival stage 3 setup is outside stage 3 substep 0"
		return
	var media: Variant = state.opening.get("media")
	if not media is Dictionary:
		result.error = "first-arrival stage 3 setup lacks media state"
		return
	var batch_key := JSON.stringify([media.epoch,"stage3_o12_96",BoardStore.O_12_ID,5,0x0120]).sha256_text()
	state.opening.erase("prologue_petals")
	state.opening.erase("petals_source_draw")
	state.opening["D_801220E0"] = 0
	state.opening["D_801220E1"] = 0
	state.opening["D_801220E2"] = 0
	state.opening["bg_brightness"] = 0
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":2,
		"clut_row":494,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_12_ID,
		"cut_batch_key":state.opening.cut_media.upload_batch_key,
	}
	state.opening["actor96"] = {
		"profile":"o12_rei_hallway",
		"index":96,"unk_01":10,"flags":1,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x801800d8,"sequence_table":0x801800f0,"tpage_table":0x801800d4,
		"unk_06":2,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-160,"y":-120,"brightness":128,
		"frame_index":0,"sequence_counter":0,"tpage_slot":5,"clut_row":497,
		"resource_id":BoardStore.O_12_ID,"source_sha256":BoardStore.O_12_SHA,"upload_batch_key":batch_key,
	}
	state.opening["actor96_draw_flags"] = 0xa4
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key,"resource_id":BoardStore.O_12_ID,"slot":5,"source_offset":0x0120})

func stage_03_to_04_setup(state: State, decoded: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 4 or state.mode_step != 0 or state.substep != 0 or decoded != 0 or not state.opening.get("cut_media") is Dictionary or state.opening.cut_media.get("resource_id") != BoardStore.CUT_05_ID or not state.opening.get("names_media") is Dictionary:
		result.error = "first-arrival stage 3->4 setup is outside stage 4 substep 0"
		return
	state.opening.names_media["bg_show_1"] = 0
	state.opening["D_801220E0"] = 128
	state.opening["D_801220E1"] = 128
	state.opening["D_801220E2"] = 128
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":1,
		"clut_row":495,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_05_ID,
		"cut_batch_key":state.opening.cut_media.upload_batch_key,
	}
	state.opening.erase("actor96")
	state.opening["actor96_draw_flags"] = 0
	state.opening.erase("actor96_source_draw")

func stage_04_setup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 4 or state.mode_step != 0 or state.substep != 1 or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival stage 4 setup is outside stage 4 substep 1"
		return
	var media: Dictionary = state.opening.media
	var req_id: String = str(media.get("actor_pending",{}).get(str(0x80180000),""))
	var request: Variant = media.get("requests",{}).get(req_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != BoardStore.O_05_ID or request.get("resource_sha256") != BoardStore.O_05_SHA:
		result.error = "first-arrival stage 4 setup has no completed O_05 read at 0x80180000"
		return
	var batch_key := JSON.stringify([media.epoch,"stage4_o05_96",BoardStore.O_05_ID,5,0x00a0]).sha256_text()
	state.opening["actor96"] = {
		"profile":"o05_rei_eyes",
		"index":96,"unk_01":10,"flags":0,"quad_flags":0x41000000,"draw_flags":0xac,
		"frame_table":0x8018007c,"sequence_table":0x8018008c,"tpage_table":0x80180078,
		"unk_06":1,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-160,"y":-120,"brightness":128,
		"frame_index":0,"sequence_counter":0,"seq_step":0,"tpage_slot":5,"clut_row":498,
		"resource_id":BoardStore.O_05_ID,"source_sha256":BoardStore.O_05_SHA,"upload_batch_key":batch_key,
	}
	state.opening["actor96_draw_flags"] = 0xa4
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key,"resource_id":BoardStore.O_05_ID,"slot":5,"source_offset":0x00a0})

func stage_04_blink_check(state: State, rng, result: Session.FrameResult) -> void:
	var actor: Variant = state.opening.get("actor96")
	if not actor is Dictionary or actor.get("profile") != "o05_rei_eyes":
		result.error = "first-arrival stage 4 blink check lacks O_05 actor96"
		return
	var draw_val: int = rng.next("prologue_stage4:blink")
	if (draw_val & 0x7f) == 0x7f and (int(actor.get("flags", 0)) & 1) == 0:
		actor["flags"] = 0x11
		state.opening["actor96"] = actor

func stage_04_to_05_setup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 4 or state.mode_step != 0 or state.substep != 4 or not state.opening.get("cut_media") is Dictionary or state.opening.cut_media.get("resource_id") != BoardStore.CUT_12_ID or not state.opening.get("names_media") is Dictionary or not state.opening.names_media.get("buffer_1") is Dictionary or state.opening.names_media.buffer_1.get("lba") != 0xacdd:
		result.error = "first-arrival stage 4->5 setup is outside stage 4 substep 4"
		return
	var media: Dictionary = state.opening.media
	var batch_key_96 := JSON.stringify([media.epoch,"stage5_o12_96",BoardStore.O_12_ID,5,0x0120]).sha256_text()
	var batch_key_97 := JSON.stringify([media.epoch,"stage5_o12_97",BoardStore.O_12_ID,6,0x41a0]).sha256_text()
	state.opening.names_media["bg_show_1"] = 1
	state.opening["D_801220E0"] = 128
	state.opening["D_801220E1"] = 128
	state.opening["D_801220E2"] = 128
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":2,
		"clut_row":494,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_12_ID,
		"cut_batch_key":state.opening.cut_media.upload_batch_key,
	}
	state.opening["actor96"] = {
		"profile":"o12_rei_hallway",
		"index":96,"unk_01":10,"flags":0,"quad_flags":0x41000000,"draw_flags":0xac,
		"frame_table":0x801800d8,"sequence_table":0x801800f0,"tpage_table":0x801800d4,
		"unk_06":1,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-160,"y":-120,"brightness":128,
		"frame_index":0,"sequence_counter":0,"tpage_slot":5,"clut_row":497,
		"resource_id":BoardStore.O_12_ID,"source_sha256":BoardStore.O_12_SHA,"upload_batch_key":batch_key_96,
	}
	state.opening["actor96_draw_flags"] = 0xa4
	state.opening["actor97"] = {
		"profile":"o12_yoshio_arm",
		"index":97,"unk_01":10,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xac,
		"frame_table":0x80184184,"sequence_table":0x8018418c,"tpage_table":0x80184180,
		"unk_06":1,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-32,"y":-108,"brightness":128,
		"frame_index":0,"sequence_counter":0,"tpage_slot":6,"clut_row":499,
		"resource_id":BoardStore.O_12_ID,"source_sha256":BoardStore.O_12_SHA,"upload_batch_key":batch_key_97,
	}
	state.opening["actor97_draw_flags"] = 0xa4
	state.opening["prologue_scene_counter"] = 0
	state.opening["prologue_text_cleared"] = true
	result.presentation.append({"kind":"first_arrival_o12_restore","resource_id":BoardStore.O_12_ID,"sha256":BoardStore.O_12_SHA,"bytes":34816})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96,"resource_id":BoardStore.O_12_ID,"slot":5,"source_offset":0x0120})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_97,"resource_id":BoardStore.O_12_ID,"slot":6,"source_offset":0x41a0})

func stage_05_arm_enter(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 5 or state.mode_step != 0 or state.substep != 0 or state.opening.get("prologue_scene_counter") != 0x81 or not state.opening.get("actor97") is Dictionary:
		result.error = "first-arrival stage 5 arm enter is outside stage 5 substep 0 counter 0x81"
		return
	state.opening.actor97["flags"] = 5
	state.opening.actor97["draw_flags"] = 0xa4
	state.opening.actor97["sequence_slot"] = 0
	state.opening.actor97["seq_step"] = 1
	state.opening.actor97["frame_index"] = 1
	state.opening.actor97["sequence_counter"] = 0
	state.opening.actor97["unk_05"] = 128

func stage_05_audio_command(state: State, command: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_step != 0:
		result.error = "source service is outside the prepared first-arrival scope"
		return
	if state.mode_stage == 5 and command == 0x0605:
		var sub0_ok: bool = state.substep == 0 and state.opening.get("prologue_scene_counter") == 0x81
		var a97: Variant = state.opening.get("actor97")
		var sub1_ok: bool = state.substep == 1 and a97 is Dictionary and a97.get("seq_step") == 3 and a97.get("sequence_counter") == 0
		if not sub0_ok and not sub1_ok:
			result.error = "source service is outside the prepared first-arrival scope"
			return
		state.opening["stage_05_se_605_count"] = int(state.opening.get("stage_05_se_605_count", 0)) + 1
		return
	if state.mode_stage == 6 and state.substep == 10 and command == 0x0503:
		state.opening["stage_06_se_503_count"] = int(state.opening.get("stage_06_se_503_count", 0)) + 1
		AudioRules.audio_command(state, command, result)
		return
	result.error = "source service is outside the prepared first-arrival scope"

func stage_05_color_burst(state: State, decoded: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 5 or state.mode_step != 0 or state.substep != 1 or decoded != 0 or not state.opening.get("cut_media") is Dictionary or state.opening.cut_media.get("resource_id") != BoardStore.CUT_12_ID or not state.opening.get("names_media") is Dictionary or not state.opening.names_media.get("buffer_0") is Dictionary or state.opening.names_media.buffer_0.get("lba") != 0xacf6 or not state.opening.get("actor97") is Dictionary:
		result.error = "first-arrival stage 5 color burst is outside stage 5 substep 1"
		return
	state.opening.names_media["bg_show_1"] = 0
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":1,
		"clut_row":495,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_12_ID,
		"cut_batch_key":state.opening.cut_media.upload_batch_key,
	}
	state.opening.actor97["unk_43"] = 0x1d
	state.opening.actor97["clut_row"] = 509

func stage_05_cut06_setup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 5 or state.mode_step != 0 or state.substep != 5 or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival stage 5 cut_06 setup is outside stage 5 substep 5"
		return
	var media: Dictionary = state.opening.media
	var req_id: String = str(media.get("actor_pending",{}).get(str(0x801a4000),""))
	var request: Variant = media.get("requests",{}).get(req_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != BoardStore.CUT_06_ID or request.get("resource_sha256") != BoardStore.CUT_06_SHA:
		result.error = "first-arrival stage 5 cut_06 setup has no completed CUT_06 read at 0x801a4000"
		return
	var batch_key := JSON.stringify([media.epoch,req_id,BoardStore.CUT_06_ID,BoardStore.CUT_06_SHA,[512,0,192,256],[0,494,256,2]]).sha256_text()
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_06_ID,"sha256":BoardStore.CUT_06_SHA,"request_id":req_id,"upload_batch_key":batch_key}
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":1,
		"clut_row":495,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_06_ID,
		"cut_batch_key":batch_key,
	}
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":batch_key,"resource_id":BoardStore.CUT_06_ID,"sha256":BoardStore.CUT_06_SHA})

func stage_05_to_06_setup(state: State, decoded: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 5 or state.mode_step != 0 or state.substep != 5 or decoded != 1 or not state.opening.get("cut_media") is Dictionary or state.opening.cut_media.get("resource_id") != BoardStore.CUT_06_ID or not state.opening.get("names_media") is Dictionary or not state.opening.names_media.get("buffer_1") is Dictionary or state.opening.names_media.buffer_1.get("lba") != 0xacd4 or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival stage 5->6 setup is outside stage 5 substep 5"
		return
	var media: Dictionary = state.opening.media
	var req_id: String = str(media.get("actor_pending",{}).get(str(0x801f0000),""))
	var request: Variant = media.get("requests",{}).get(req_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != BoardStore.O_06_ID or request.get("resource_sha256") != BoardStore.O_06_SHA:
		result.error = "first-arrival stage 5->6 setup has no completed O_06 read at 0x801f0000"
		return
	var batch_key_96_5 := JSON.stringify([media.epoch,"stage6_o06_96_s5",BoardStore.O_06_ID,5,0x4310]).sha256_text()
	var batch_key_96_6 := JSON.stringify([media.epoch,"stage6_o06_96_s6",BoardStore.O_06_ID,6,0x8310]).sha256_text()
	var batch_key_99_7 := JSON.stringify([media.epoch,"stage6_o06_99_s7",BoardStore.O_06_ID,7,0x0130]).sha256_text()
	state.opening.names_media["bg_show_1"] = 1
	state.opening.erase("actor97")
	state.opening.erase("actor97_draw_flags")
	state.opening.erase("actor97_source_draw")
	state.opening["actor96"] = {
		"profile":"o06_yoshio_body",
		"index":96,"flags":0x40,"draw_flags":0xa4,
		"x":-160,"y":-120,"frame_index":0,"sequence_counter":0,"clut_row":500,
		"resource_id":BoardStore.O_06_ID,
		"upload_batch_key":batch_key_96_5,"upload_batch_key_2":batch_key_96_6,
	}
	state.opening["actor96_draw_flags"] = 0xa4
	state.opening["actor99"] = {
		"profile":"o06_crowd_girls",
		"index":99,"flags":0,"draw_flags":0xa4,
		"x":-160,"y":-120,"frame_index":0,"sequence_counter":0,"clut_row":501,
		"resource_id":BoardStore.O_06_ID,
		"upload_batch_key":batch_key_99_7,
	}
	state.opening["actor99_draw_flags"] = 0xa4
	state.opening["o06_batches"] = {"slot_5":batch_key_96_5,"slot_6":batch_key_96_6,"slot_7":batch_key_99_7}
	result.presentation.append({"kind":"first_arrival_o06_copy","resource_id":BoardStore.O_06_ID,"sha256":BoardStore.O_06_SHA,"bytes":51200})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96_5,"resource_id":BoardStore.O_06_ID,"slot":5,"source_offset":0x4310})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96_6,"resource_id":BoardStore.O_06_ID,"slot":6,"source_offset":0x8310})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_99_7,"resource_id":BoardStore.O_06_ID,"slot":7,"source_offset":0x0130})

func stage_06_cd_wait(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 1:
		result.error = "first-arrival stage 6 CD wait is outside stage 6 substep 1"
		return
	var ready: bool = (not state.opening.has("prologue_lip_sync") and not state.opening.has("cd_audio_state_override")) or (AudioRules.check_cd(state) & 4) == 4
	if ready:
		state.substep += 1
	if state.opening.get("prologue_text_work") is Dictionary and state.opening.prologue_text_work.has("next_frame"):
		state.opening.prologue_text_work["next_frame"] = Math.u32(state.frame_count + 1)

func stage_06_actor98_step(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 2 or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 actor98 step is outside stage 6 substep 2"
		return
	var prev_unk08: int = int(state.opening.get("actor98",{}).get("sequence_counter",0))
	var next_unk08: int = prev_unk08 + 1
	var frame_idx: int = 8 if next_unk08 < 5 else 10
	state.opening["actor98"] = {
		"profile":"o06_yoshio_collar",
		"index":98,"flags":0x40,"draw_flags":0xa4,
		"x":-160,"y":-120,"frame_index":frame_idx,"sequence_counter":next_unk08,"clut_row":500,
		"upload_batch_key":state.opening.o06_batches.slot_5,
	}
	state.opening["actor98_draw_flags"] = 0xa4

func stage_06_lip_sync_yoshio(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep not in [2, 3, 4, 5, 6, 7] or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 Yoshio lip-sync is outside stage 6 substeps 2..7"
		return
	var lip: Dictionary = {}
	if state.opening.get("prologue_lip_sync") is Dictionary and state.opening.get("audio") is Dictionary:
		lip = state.opening.prologue_lip_sync
	else:
		lip = AudioRules.lip_sync_yoshio(state)
	state.opening.erase("actor97")
	state.opening.erase("actor97_draw_flags")
	state.opening.erase("actor97_source_draw")
	if bool(lip.get("cd_active",false)):
		var ring_frame: int = clampi(int(lip.get("ring_frame",0)),0,3)
		state.opening["actor97"] = {
			"profile":"o06_yoshio_mouth",
			"index":97,"flags":0x40,"draw_flags":0xa4,
			"x":-160,"y":-120,"frame_index":6 - ring_frame,"sequence_counter":0,"clut_row":500,
			"upload_batch_key":state.opening.o06_batches.slot_6,
		}
		state.opening["actor97_draw_flags"] = 0xa4

func stage_06_actor102_set(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 2 or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 actor102 setup is outside stage 6 substep 2"
		return
	state.opening["actor102"] = {
		"profile":"o06_yoshio_blink",
		"index":102,"flags":3,"draw_flags":0xa4,
		"x":-160,"y":-120,"seq_step":0,"frame_index":1,"sequence_counter":0,"clut_row":500,
		"upload_batch_key":state.opening.o06_batches.slot_5,"upload_batch_key_2":state.opening.o06_batches.slot_6,
	}
	state.opening["actor102_draw_flags"] = 0xa4

func stage_06_actor99_turn(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 10 or not state.opening.get("actor99") is Dictionary:
		result.error = "first-arrival stage 6 actor99 turn is outside stage 6 substep 9->10"
		return
	state.opening.actor99["flags"] = 5
	state.opening.actor99["seq_step"] = int(state.opening.actor99.get("seq_step",0))

func stage_06_mouth_girl_a(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep not in [10, 11] or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 Girl A mouth is outside stage 6 substep 10..11"
		return
	var lip: Dictionary = {}
	if state.opening.get("prologue_lip_sync") is Dictionary and state.opening.get("audio") is Dictionary:
		lip = state.opening.prologue_lip_sync
	else:
		lip = AudioRules.lip_sync_girl_a(state)
	var existing: Variant = state.opening.get("actor100")
	if bool(lip.get("cd_active",false)):
		var step_val: int = int(existing.get("seq_step",0)) if existing is Dictionary else 0
		var cnt_val: int = int(existing.get("sequence_counter",0)) if existing is Dictionary else 0
		var f_idx: int = 3 if step_val == 0 else 4
		state.opening["actor100"] = {
			"profile":"o06_mouth_girl_a",
			"index":100,"flags":1,"draw_flags":0xa4,
			"x":-160,"y":-120,"seq_step":step_val,"frame_index":f_idx,"sequence_counter":cnt_val,"clut_row":501,
			"upload_batch_key":state.opening.o06_batches.slot_7,
		}
		state.opening["actor100_draw_flags"] = 0xa4
	elif existing is Dictionary:
		existing["flags"] = 0
		state.opening["actor100"] = existing

func stage_06_actor103_set(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 12 or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 actor103 setup is outside stage 6 substep 11->12"
		return
	state.opening["actor103"] = {
		"profile":"o06_yoshio_sweat",
		"index":103,"flags":0x40,"draw_flags":0xa4,
		"x":-160,"y":-120,"frame_index":7,"sequence_counter":0,"clut_row":500,
		"upload_batch_key":state.opening.o06_batches.slot_6,
	}
	state.opening["actor103_draw_flags"] = 0xa4

func stage_06_mouth_girl_b(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep not in [12, 13] or not state.opening.get("o06_batches") is Dictionary:
		result.error = "first-arrival stage 6 Girl B mouth is outside stage 6 substep 12..13"
		return
	var lip: Dictionary = {}
	if state.opening.get("prologue_lip_sync") is Dictionary and state.opening.get("audio") is Dictionary:
		lip = state.opening.prologue_lip_sync
	else:
		lip = AudioRules.lip_sync_girl_b(state)
	var existing: Variant = state.opening.get("actor101")
	if bool(lip.get("cd_active",false)):
		var step_val: int = int(existing.get("seq_step",0)) if existing is Dictionary else 0
		var cnt_val: int = int(existing.get("sequence_counter",0)) if existing is Dictionary else 0
		var f_idx: int = 5 if step_val == 0 else 6
		state.opening["actor101"] = {
			"profile":"o06_mouth_girl_b",
			"index":101,"flags":1,"draw_flags":0xa4,
			"x":-160,"y":-120,"seq_step":step_val,"frame_index":f_idx,"sequence_counter":cnt_val,"clut_row":501,
			"upload_batch_key":state.opening.o06_batches.slot_7,
		}
		state.opening["actor101_draw_flags"] = 0xa4
	elif existing is Dictionary:
		existing["flags"] = 0
		state.opening["actor101"] = existing

func stage_06_to_07_setup(state: State, decoded: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 6 or state.mode_step != 0 or state.substep != 16 or decoded != 0 or not state.opening.get("names_media") is Dictionary or not state.opening.names_media.get("buffer_0") is Dictionary or state.opening.names_media.buffer_0.get("lba") != 0xacec or not state.opening.get("media") is Dictionary:
		result.error = "first-arrival stage 6->7 setup is outside stage 6 substep 16"
		return
	var media: Dictionary = state.opening.media
	var cut_req_id: String = str(media.get("actor_pending",{}).get(str(0x801a4000),""))
	var cut_req: Variant = media.get("requests",{}).get(cut_req_id)
	if not cut_req is Dictionary or cut_req.get("kind") != "actors" or cut_req.get("status") != "ready" or cut_req.get("resource_id") != BoardStore.CUT_07_ID or cut_req.get("resource_sha256") != BoardStore.CUT_07_SHA:
		result.error = "first-arrival stage 6->7 setup has no completed CUT_07 read at 0x801a4000"
		return
	var o07_req_id: String = str(media.get("actor_pending",{}).get(str(0x801f0000),""))
	var o07_req: Variant = media.get("requests",{}).get(o07_req_id)
	if not o07_req is Dictionary or o07_req.get("kind") != "actors" or o07_req.get("status") != "ready" or o07_req.get("resource_id") != BoardStore.O_07_ID or o07_req.get("resource_sha256") != BoardStore.O_07_SHA:
		result.error = "first-arrival stage 6->7 setup has no completed O_07 read at 0x801f0000"
		return
	var cut_batch_key := JSON.stringify([media.epoch,cut_req_id,BoardStore.CUT_07_ID,BoardStore.CUT_07_SHA,[512,0,192,256],[0,494,256,2]]).sha256_text()
	var batch_key_96_5 := JSON.stringify([media.epoch,"stage7_o07_96_s5",BoardStore.O_07_ID,5,0x0190]).sha256_text()
	var batch_key_96_6 := JSON.stringify([media.epoch,"stage7_o07_96_s6",BoardStore.O_07_ID,6,0x4190]).sha256_text()
	var batch_key_96_7 := JSON.stringify([media.epoch,"stage7_o07_96_s7",BoardStore.O_07_ID,7,0x8190]).sha256_text()
	for idx in [96, 97, 98, 99, 100, 101, 102, 103]:
		state.opening.erase("actor%d" % idx)
		state.opening.erase("actor%d_draw_flags" % idx)
		state.opening.erase("actor%d_source_draw" % idx)
	state.opening.erase("o06_batches")
	state.opening.names_media["bg_show_1"] = 0
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_07_ID,"sha256":BoardStore.CUT_07_SHA,"request_id":cut_req_id,"upload_batch_key":cut_batch_key}
	state.opening["bg_layer_0"] = {
		"enabled":1,
		"palette_flag":1,
		"clut_row":495,
		"attribute":0x41000000,
		"resource_id":BoardStore.CUT_07_ID,
		"cut_batch_key":cut_batch_key,
	}
	state.opening["actor96"] = {
		"profile":"o07_yoshio_body",
		"index":96,"flags":0x40,"draw_flags":0xa4,
		"x":-224,"y":-120,"frame_index":0,"sequence_counter":0,"clut_row":502,
		"resource_id":BoardStore.O_07_ID,
		"upload_batch_key":batch_key_96_5,"upload_batch_key_2":batch_key_96_6,"upload_batch_key_3":batch_key_96_7,
	}
	state.opening["actor96_draw_flags"] = 0xa4
	state.opening["o07_batches"] = {"slot_5":batch_key_96_5,"slot_6":batch_key_96_6,"slot_7":batch_key_96_7}
	result.presentation.append({"kind":"first_arrival_o07_copy","resource_id":BoardStore.O_07_ID,"sha256":BoardStore.O_07_SHA,"bytes":51200})
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_batch_key,"resource_id":BoardStore.CUT_07_ID,"sha256":BoardStore.CUT_07_SHA})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96_5,"resource_id":BoardStore.O_07_ID,"slot":5,"source_offset":0x0190})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96_6,"resource_id":BoardStore.O_07_ID,"slot":6,"source_offset":0x4190})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":batch_key_96_7,"resource_id":BoardStore.O_07_ID,"slot":7,"source_offset":0x8190})

func stage_07_to_08_setup(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 7 or state.mode_step != 0 or state.substep != 7 or not state.opening.get("media") is Dictionary or not state.opening.get("arrival_media") is Dictionary:
		result.error = "Rei transition requires source stage7/substep7 and prepared media"
		return
	var media: Dictionary = state.opening.media
	var requests: Dictionary = media.get("requests",{})
	var pending: Dictionary = media.get("actor_pending",{})
	var cut_id := str(pending.get(str(0x801a4000),""))
	var actor_id := str(pending.get(str(0x801f0000),""))
	var cut: Variant = requests.get(cut_id)
	var actor: Variant = requests.get(actor_id)
	if not cut is Dictionary or cut.get("status") != "ready" or cut.get("kind") != "actors" or cut.get("resource_id") != BoardStore.CUT_08_ID or cut.get("resource_sha256") != BoardStore.CUT_08_SHA:
		result.error = "Rei transition lacks completed original CUT09 read"
		return
	if not actor is Dictionary or actor.get("status") != "ready" or actor.get("kind") != "actors" or actor.get("resource_id") != BoardStore.O_08_ID or actor.get("resource_sha256") != BoardStore.O_08_SHA:
		result.error = "Rei transition lacks completed original O09 backup read"
		return
	# Source memcpy precedes decode; publication applies these only after the
	# complete frame passes validation and commits.
	result.presentation.append({"kind":"first_arrival_o08_copy","resource_id":BoardStore.O_08_ID,"sha256":BoardStore.O_08_SHA,"bytes":18432})
	var decoded := background_decode(state, 1, result)
	if not result.error.is_empty() or decoded != 1:
		if result.error.is_empty(): result.error = "Rei transition did not select original alternate background"
		return
	for index in range(96,160):
		state.opening.erase("actor%d" % index)
		state.opening.erase("actor%d_draw_flags" % index)
		state.opening.erase("actor%d_source_draw" % index)
	state.opening.erase("o07_batches")
	state.opening["bg_brightness"] = 0
	state.opening["prologue_text_cleared"] = true
	state.opening["prologue_scene_counter"] = 0
	for component in ["D_801220E0","D_801220E1","D_801220E2"]: state.opening[component] = 0
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_08_ID,"white_palette"]).sha256_text()
	var actor_key := JSON.stringify([media.epoch,actor_id,BoardStore.O_08_ID,5,0xa0]).sha256_text()
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_08_ID,"sha256":BoardStore.CUT_08_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":true}
	state.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":494,"attribute":0x41000000,"resource_id":BoardStore.CUT_08_ID,"cut_batch_key":cut_key}
	state.opening["actor96"] = {"profile":"o08_rei_banner","index":96,"unk_01":12,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0x84,"frame_table":0x80180078,"sequence_table":0x80180088,"tpage_table":0x80180074,"frame_index":0,"sequence_counter":0,"x":-160,"y":-120,"brightness":0,"clut_row":494,"tpage_slot":5,"resource_id":BoardStore.O_08_ID,"source_sha256":BoardStore.O_08_SHA,"upload_batch_key":actor_key}
	state.opening["actor96_draw_flags"] = 0x84
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_08_ID,"sha256":BoardStore.CUT_08_SHA})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":actor_key,"resource_id":BoardStore.O_08_ID,"slot":5,"source_offset":0xa0})

func rei_banner_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 8 or state.mode_step != 0:
		result.error = "Rei banner effect requires original stage8 media"
		return
	match kind:
		"flash", "white":
			if kind == "white" and state.substep == 3:
				# Source case3 waits for CD playback and does not call line_run.
				# Account for that exact skipped text tick without revealing ink.
				var work: Variant = state.opening.get("prologue_text_work")
				if not work is Dictionary or work.get("line_id") != 12 or work.get("next_frame") != state.frame_count:
					result.error = "Rei voice wait lost its expected text frame"; return
				work.next_frame = Math.u32(state.frame_count+1)
			state.opening["rei_banner_primitive"] = {"kind":kind,"level":clampi(value,0,255),"frame_count":Math.u32(state.frame_count+1)}
		"color_cut":
			if state.substep != 1 or not state.opening.get("actor96") is Dictionary or state.opening.get("cut_media",{}).get("resource_id") != BoardStore.CUT_08_ID:
				result.error = "Rei color cut has no source white-palette owner"; return
			state.opening.actor96["clut_row"] = 504
			state.opening.actor96["brightness"] = 128
			state.opening.actor96["scale"] = 128
			var key := JSON.stringify([state.opening.media.epoch,BoardStore.CUT_08_ID,"stage8_color"]).sha256_text()
			state.opening.cut_media["upload_batch_key"] = key
			state.opening.cut_media["white_palette"] = false
			state.opening.bg_layer_0["cut_batch_key"] = key
			state.opening.bg_layer_0["clut_row"] = 495
			result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":key,"resource_id":BoardStore.CUT_08_ID,"sha256":BoardStore.CUT_08_SHA})
		"lip_sync":
			if not state.opening.get("actor96") is Dictionary:
				result.error = "Rei lip synchronization lost actor96"; return
			var lip: Dictionary = state.opening.get("prologue_lip_sync",{})
			if lip.get("variant") != "rei": lip = AudioRules.lip_sync_rei(state)
			if (AudioRules.check_cd(state) & 4) != 0:
				state.opening.actor96["unk_01"] = 9
				state.opening.actor96["draw_flags"] = 0xa4
				state.opening["actor96_draw_flags"] = 0xa4
				state.opening.actor96["clut_row"] = 504
				state.opening.actor96["frame_index"] = int(lip.get("ring_frame",0))
			else: state.opening.actor96["frame_index"] = 0
		"clear_for_read":
			state.opening.bg_layer_0["enabled"] = 0
			state.opening.erase("actor96"); state.opening.erase("actor96_source_draw")
			state.opening["actor96_draw_flags"] = 0
		"next_scene":
			_rei_entrance_setup(state,result)
		_: result.error = "Unknown source Rei banner effect"

func _rei_entrance_setup(state: State, result: Session.FrameResult) -> void:
	# prologue.c::prologue_rei_banner_run case7: these are direct primary
	# reads. Unlike O09, O10 must never be copied from the backup buffer.
	if state.substep != 7 or not state.opening.get("media") is Dictionary:
		result.error = "Rei entrance requires source stage8/substep7"; return
	var media: Dictionary = state.opening.media
	var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_09_ID,BoardStore.CUT_09_SHA)
	var actor_id := _ready_read(media,0x80180000,BoardStore.O_09_ID,BoardStore.O_09_SHA)
	if cut_id.is_empty() or actor_id.is_empty():
		result.error = "Rei entrance lacks completed original CUT10/O10 read"; return
	_actor_clear_upper(state)
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_09_ID]).sha256_text()
	var key5 := JSON.stringify([media.epoch,actor_id,BoardStore.O_09_ID,5,0x82c0]).sha256_text()
	var key6 := JSON.stringify([media.epoch,actor_id,BoardStore.O_09_ID,6,0x42c0]).sha256_text()
	state.opening["bg_brightness"] = 128
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_09_ID,"sha256":BoardStore.CUT_09_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":false}
	state.opening["bg_layer_0"] = {"enabled":0,"palette_flag":1,"clut_row":495,"attribute":0x41000000,"resource_id":BoardStore.CUT_09_ID,"cut_batch_key":cut_key}
	state.opening["actor96"] = {"profile":"o09_rei_entrance","index":96,"unk_01":10,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80184288,"sequence_table":0x801842ac,"tpage_table":0x80184280,"frame_index":0,"sequence_counter":0,"x":-160,"y":-120,"brightness":128,"clut_row":505,"resource_id":BoardStore.O_09_ID,"source_sha256":BoardStore.O_09_SHA,"upload_batch_key":key5,"upload_batch_key_2":key6}
	state.opening["actor96_draw_flags"] = 0xa4
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_09_ID,"sha256":BoardStore.CUT_09_SHA})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":key5,"resource_id":BoardStore.O_09_ID,"slot":5,"source_offset":0x82c0})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":key6,"resource_id":BoardStore.O_09_ID,"slot":6,"source_offset":0x42c0})

func rei_lines_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	if kind == "blink" and state.mode_stage == 10 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96",{}).get("profile") == "o11_rei_lines15":
		# Case11's mode_stage_next resets substep before the <6 RNG test, so
		# prologue_actor98_animation_start_rei can point actor98 at O10 table
		# addresses over copied O11 bytes. Those words lack bit31, so
		# actor_draw_all skips it (no draw, no sequence step) until case2's
		# actor_clear(0x62).
		if state.opening.has("actor98"):
			result.error = "Rei post-switch blink found a live actor98 after actor_clear_upper"; return null
		state.opening["actor98"] = {"profile":"o10_tables_over_o11","index":98,"unk_01":9,"flags":3,"quad_flags":0x41000000,"draw_flags":0xac,"frame_table":0x80184288,"sequence_table":0x801842ac,"tpage_table":0x80184280,"seq_step":0,"brightness":128,"x":-160,"y":-120}
		state.opening["actor98_draw_flags"] = 0xac
		return null
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 9 or state.mode_step != 0 or state.opening.get("actor96",{}).get("profile") != "o09_rei_entrance":
		result.error = "Rei dialogue effect requires its original O10 actor owner"; return null
	var allowed := {"flash":[1,2,3],"wait_voice":[1],"lip_sync":[2,3,4,8,9],"page_boundary":[2,3],"page_transition":[3],"animate_exit":[4],"exit_done":[5],"line14_setup":[7],"line14_hold":[8],"blink":[1,2,3,4,5],"next_scene":[11]}
	if not allowed.has(kind) or state.substep not in allowed[kind]:
		result.error = "Rei dialogue effect is outside its source substep"; return null
	match kind:
		"flash":
			if value < 0 or value > 255:
				result.error = "Rei dialogue flash exceeds its source byte"; return null
			state.opening["rei_banner_primitive"] = {"kind":"flash","level":value,"frame_count":Math.u32(state.frame_count+1)}
		"wait_voice":
			var work: Variant = state.opening.get("prologue_text_work")
			if not work is Dictionary or work.get("line_id") != 13 or work.get("next_frame") != state.frame_count:
				result.error = "Rei line13 voice wait lost its expected text tick"; return null
			work.next_frame = Math.u32(state.frame_count+1)
		"page_boundary":
			var work: Variant = state.opening.get("prologue_text_work")
			if not work is Dictionary or work.get("line_id") != 13 or not TextWork._work_is_valid(work):
				result.error = "Rei page boundary requires original line13 text state"; return null
			return TextWork._get_now_k(work) == int(work.first_line)+3
		"lip_sync":
			var lip := AudioRules.lip_sync_rei_lines_13_and_14(state)
			state.opening.erase("actor97"); state.opening.erase("actor97_source_draw")
			state.opening["actor97_draw_flags"] = 0
			if lip.cd_active:
				var actor := _rei_dialogue_actor(state,97)
				actor["flags"] = 0x40; actor["frame_index"] = int(lip.ring_frame)+5
				state.opening["actor97"] = actor; state.opening["actor97_draw_flags"] = 0xa4
		"blink":
			var actor: Dictionary = state.opening.get("actor98",{})
			if (int(actor.get("flags",0)) & 1) == 0:
				if actor.is_empty(): actor = _rei_dialogue_actor(state,98)
				actor["flags"] = 3; actor["draw_flags"] = 0xac
				state.opening["actor98"] = actor; state.opening["actor98_draw_flags"] = 0xac
		"line14_setup":
			var actor: Dictionary = state.opening.get("actor98",{})
			# If RNG never initialized98, actor_clear's draw_flags remains0:
			# the source changes its sequence cursor but draws no sprite.
			if actor.is_empty():
				actor = {"profile":"cleared","index":98,"draw_flags":0}
				state.opening["actor98"] = actor
				state.opening["actor98_draw_flags"] = 0
			actor["flags"] = 1; actor["seq_step"] = 3; actor["frame_index"] = 4; actor["sequence_counter"] = 0
		"line14_hold":
			var actor: Variant = state.opening.get("actor98")
			if not actor is Dictionary:
				result.error = "Rei line14 lost its initialized actor98"; return null
			if actor.get("seq_step") == 3: actor["flags"] = 0
		"page_transition":
			if state.opening.has("actor99") or state.opening.has("actor100"):
				result.error = "Rei crowd setup cannot replay"; return null
			var key := JSON.stringify([state.opening.media.epoch,BoardStore.O_09_ID,7,0x150]).sha256_text()
			for index in [99,100]:
				var actor := _rei_dialogue_actor(state,index)
				actor["upload_batch_key"] = key
				actor.erase("upload_batch_key_2")
				state.opening["actor%d" % index] = actor; state.opening["actor%d_draw_flags" % index] = 0xa4
			state.opening.bg_layer_0["enabled"] = 1
			result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":key,"resource_id":BoardStore.O_09_ID,"slot":7,"source_offset":0x150})
		"animate_exit", "exit_done":
			if not state.opening.get("actor99") is Dictionary or not state.opening.get("actor100") is Dictionary:
				result.error = "Rei crowd animation lacks both source actors"; return null
			if kind == "exit_done": return state.opening.actor99.flags == 0
			state.opening.actor100["flags"] = 5; state.opening.actor99["flags"] = 5
		"next_scene":
			_rei_lines_15_setup(state,result)
	return null

static func _ready_read(media: Dictionary, destination: int, resource_id: String, sha256: String, payload: Variant = null) -> String:
	# Request id of a completed actor read at destination, or "". A payload
	# also pins the source extent and the current media epoch.
	var request_id := str(media.get("actor_pending",{}).get(str(destination),""))
	var request: Variant = media.get("requests",{}).get(request_id)
	if not request is Dictionary or request.get("status") != "ready" or request.get("kind") != "actors" or request.get("resource_id") != resource_id or request.get("resource_sha256") != sha256:
		return ""
	if payload != null and (request.get("epoch") != media.get("epoch") or not Store._same(request.get("payload"),payload)): return ""
	return request_id

static func _actor_clear_upper(state: State) -> void:
	for index in range(96,160):
		state.opening.erase("actor%d" % index)
		state.opening.erase("actor%d_draw_flags" % index)
		state.opening.erase("actor%d_source_draw" % index)

func _rei_lines_15_setup(state: State, result: Session.FrameResult) -> void:
	# prologue_rei_lines_13_and_14_run case11: actor_clear_upper, O11 memcpy
	# 0x801e8000->0x80180000 (0x10800), actor96 setup, dec_bg_show_switch(1),
	# prologue_background_slot_set(0x801a4000,0xf) for the substep9 cut read.
	var media: Variant = state.opening.get("media")
	var names_media: Variant = state.opening.get("names_media")
	if not media is Dictionary or not names_media is Dictionary or names_media.get("bg_show_1") not in [0,1] or not names_media.get("buffer_0" if names_media.bg_show_1 == 1 else "buffer_1") is Dictionary:
		result.error = "Rei line15 setup lacks its source media and background buffers"; return
	var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_07_ID,BoardStore.CUT_07_SHA,{"lba":0xaa85,"sectors":49,"destination":0x801a4000})
	var actor_id := _ready_read(media,0x801e8000,BoardStore.O_11_ID,BoardStore.O_11_SHA,{"lba":0xac1e,"sectors":33,"destination":0x801e8000})
	if cut_id.is_empty() or actor_id.is_empty():
		result.error = "Rei line15 setup lacks completed original CUT08/O11 reads"; return
	_actor_clear_upper(state)
	state.opening.erase("o11_tpages")
	result.presentation.append({"kind":"first_arrival_o11_copy","resource_id":BoardStore.O_11_ID,"sha256":BoardStore.O_11_SHA,"bytes":0x10800})
	state.opening["actor96"] = {"profile":"o11_rei_lines15","index":96,"unk_01":10,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180150,"sequence_table":0x80180174,"tpage_table":0x80180140,"frame_index":0,"sequence_counter":0,"x":-160,"y":-120,"brightness":128,"clut_row":502,"resource_id":BoardStore.O_11_ID,"source_sha256":BoardStore.O_11_SHA,"read_request":actor_id}
	state.opening["actor96_draw_flags"] = 0xa4
	names_media["bg_show_1"] = 1-int(names_media.bg_show_1)
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_07_ID,"line15"]).sha256_text()
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_07_ID,"sha256":BoardStore.CUT_07_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":false}
	state.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"attribute":0x41000000,"resource_id":BoardStore.CUT_07_ID,"cut_batch_key":cut_key}
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_07_ID,"sha256":BoardStore.CUT_07_SHA})
	_o11_tpage_update(state,result)

func _o11_tpage_update(state: State, result: Session.FrameResult) -> void:
	# search_tpage reloads each frame; an upload is needed only where the
	# source slot's resident page changes.
	var slots := BoardStore.o11_tpage_slots(_board_store.stage_transfer(BoardStore.O_11_ID).get("bytes",PackedByteArray()),BoardStore.o11_drawn_frames(state.opening))
	if slots.is_empty():
		result.error = "Rei O11 frames exceed the source free tpage slots"; return
	var previous: Dictionary = state.opening.get("o11_tpages",{})
	var resident: Dictionary = previous.get("resident",{}).duplicate(true)
	var mapped: Dictionary = {}
	for page: int in slots:
		var slot: int = slots[page]
		mapped[str(page)] = slot
		if resident.get(str(slot),{}).get("offset") == page: continue
		var key := JSON.stringify([state.opening.media.epoch,state.opening.actor96.read_request,BoardStore.O_11_ID,slot,page,state.frame_count]).sha256_text()
		resident[str(slot)] = {"offset":page,"key":key}
		result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":key,"resource_id":BoardStore.O_11_ID,"slot":slot,"source_offset":page})
	state.opening["o11_tpages"] = {"slots":mapped,"resident":resident}

func rei_lines15_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	# prologue_lines_15_and_16_run (stage10) on the O11 body set up by case11.
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 10 or state.mode_step != 0 or state.opening.get("actor96",{}).get("profile") != "o11_rei_lines15":
		result.error = "Rei line15 effect requires its original O11 actor owner"; return null
	var allowed := {"wait_voice":[1],"lip_sync":[2],"advance_x":[2,3,4,5,6,7,8],"palette_adjust":[6],"next_scene":[8]}
	if not allowed.has(kind) or state.substep not in allowed[kind]:
		result.error = "Rei line15 effect is outside its source substep"; return null
	var body: Dictionary = state.opening.actor96
	match kind:
		"wait_voice":
			var work: Variant = state.opening.get("prologue_text_work")
			if not work is Dictionary or work.get("line_id") != 15 or work.get("next_frame") != state.frame_count:
				result.error = "Rei line15 voice wait lost its expected text tick"; return null
			work.next_frame = Math.u32(state.frame_count+1)
		"lip_sync":
			var lip := AudioRules.lip_sync_lines_15_and_16(state)
			for index in [97,98]:
				state.opening.erase("actor%d" % index); state.opening.erase("actor%d_source_draw" % index)
				state.opening["actor%d_draw_flags" % index] = 0
			if lip.cd_active:
				for index in [97,98]:
					var frame: int = int(lip.ring_frame)+(1 if index == 97 else 5)
					state.opening["actor%d" % index] = {"profile":"o11_rei_lip","index":index,"unk_01":9,"flags":0x40,"quad_flags":0x41000000,"draw_flags":0xa4,"frame_table":0x80180150,"sequence_table":0x80180174,"tpage_table":0x80180140,"frame_index":frame,"brightness":128,"x":-160,"y":-120,"resource_id":BoardStore.O_11_ID,"source_sha256":BoardStore.O_11_SHA}
					state.opening["actor%d_draw_flags" % index] = 0xa4
			_o11_tpage_update(state,result)
		"advance_x":
			# actors[96].x.w -= 0x8000; 97/98 x.h[1] follow 96 x.h[1].
			var word: int = Math.u32(((int(body.x)&0xffff)<<16)|int(body.get("x_low",0)))
			word = Math.u32(word-0x8000)
			body["x"] = (word>>16)-0x10000 if (word&0x80000000) != 0 else word>>16
			body["x_low"] = word&0xffff
			for index in [97,98]:
				if state.opening.get("actor%d" % index) is Dictionary: state.opening["actor%d" % index]["x"] = body.x
		"palette_adjust":
			var request_id := _ready_read(state.opening.media,0x801a4000,BoardStore.CUT_12_ID,BoardStore.CUT_12_SHA,{"lba":0xab18,"sectors":49,"destination":0x801a4000})
			if request_id.is_empty():
				result.error = "Rei line16 palette adjust lacks the completed CUT12 read"; return null
			# Source edits the resident CUT12 palette words; the cut pump applies
			# the same adjustment when prim_bg_slot_setup uploads it at substep8.
			state.opening["cut12_palette_adjusted"] = request_id
		"next_scene":
			_rei_line17_setup(state,result)
	return null

func _rei_line17_setup(state: State, result: Session.FrameResult) -> void:
	# prologue_lines_15_and_16_run case8 after a successful ACF6 decode.
	var media: Dictionary = state.opening.media
	var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_12_ID,BoardStore.CUT_12_SHA,{"lba":0xab18,"sectors":49,"destination":0x801a4000})
	var actor_id := _ready_read(media,0x801f0000,BoardStore.O_12_ID,BoardStore.O_12_SHA,{"lba":0xac3f,"sectors":17,"destination":0x801f0000})
	if cut_id.is_empty() or actor_id.is_empty():
		result.error = "Rei line17 setup lacks completed original CUT12/O12 reads"; return
	if state.opening.get("cut12_palette_adjusted") != cut_id:
		result.error = "Rei line17 setup lacks its source CUT12 palette adjustment"; return
	if state.opening.get("names_media",{}).get("bg_show_1") != 1 or state.opening.names_media.get("buffer_1",{}).get("lba") != 0xacf6:
		result.error = "Rei line17 setup requires the decoded ACF6 background"; return
	var slots := BoardStore.o12_line17_tpage_slots(_board_store.stage_transfer(BoardStore.O_12_ID).get("bytes",PackedByteArray()))
	if slots.size() != 1:
		result.error = "Rei line17 O12 frame0 does not resolve to one source tpage slot"; return
	var page: int = slots.keys()[0]
	var slot: int = slots[page]
	_actor_clear_upper(state)
	state.opening.erase("o11_tpages"); state.opening.erase("cut12_palette_adjusted")
	state.opening["bg_brightness"] = 128
	result.presentation.append({"kind":"first_arrival_o12_restore","resource_id":BoardStore.O_12_ID,"sha256":BoardStore.O_12_SHA,"bytes":0x8800})
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_12_ID,"line17"]).sha256_text()
	var key5 := JSON.stringify([media.epoch,actor_id,BoardStore.O_12_ID,slot,page]).sha256_text()
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_12_ID,"sha256":BoardStore.CUT_12_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":false}
	state.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"attribute":0x41000000,"resource_id":BoardStore.CUT_12_ID,"cut_batch_key":cut_key}
	state.opening["actor96"] = {"profile":"o12_rei_line17","index":96,"unk_01":10,"flags":0,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x801800d8,"sequence_table":0x801800f0,"tpage_table":0x801800d4,"sequence_slot":0,"frame_index":0,"sequence_counter":0,"seq_step":0,"speed":1,
		"x":-160,"y":-120,"brightness":128,"tpage_slot":slot,"clut_row":497,"resource_id":BoardStore.O_12_ID,"source_sha256":BoardStore.O_12_SHA,"upload_batch_key":key5,
		"tpages":{str(slot):{"offset":page,"key":key5}}}
	state.opening["actor96_draw_flags"] = 0xa4
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_12_ID,"sha256":BoardStore.CUT_12_SHA})
	result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":key5,"resource_id":BoardStore.O_12_ID,"slot":slot,"source_offset":page})

func rei_line17_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	# prologue_line_17_run (stage11) case2..6 on the O12 actor of case8.
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 11 or state.mode_step != 0 or state.opening.get("actor96",{}).get("profile") != "o12_rei_line17":
		result.error = "Rei line17 effect requires its original O12 actor owner"; return null
	var allowed := {"sequence_start":[3],"sequence_done":[3],"palette_adjust":[4],"dec_bg_reset":[6],"next_scene":[6]}
	if not allowed.has(kind) or state.substep not in allowed[kind] or (kind == "sequence_start" and value != 5) or (kind == "dec_bg_reset" and value != 0):
		result.error = "Rei line17 effect is outside its source substep"; return null
	if kind == "dec_bg_reset":
		# Case6 after a failed AD05 decode (prologue.c:1798). The reset only
		# touches slot-1 state, so it is modelled as at stages 12 and 14.
		_dec_bg_reset_slot1(state,result); return null
	var body: Dictionary = state.opening.actor96
	var rows := BoardStore.line17_sequence("o12_rei_line17",_board_store.stage_transfer(BoardStore.O_12_ID).get("bytes",PackedByteArray()))
	if rows.is_empty() or not body.get("seq_step") is int or not body.get("sequence_counter") is int or body.get("speed") != 1 or not body.get("flags") is int:
		result.error = "Rei line17 actor lacks its source O12 sequence state"; return null
	match kind:
		"sequence_start":
			# Case2 D_80120652 = 5 on the stopped case8 record (step0, counter0).
			if body.flags != 0 or body.seq_step != 0 or body.sequence_counter != 0:
				result.error = "Rei line17 sequence start found a running O12 sequence"; return null
			body["flags"] = 5
		"sequence_done":
			# Case3 tests rec->unk_18 == 1 && rec->unk_08 == 1.
			return body.seq_step == 1 and body.sequence_counter == 1
		"palette_adjust":
			# Case4 adjusts the CUT12 palette words of the case2 reread.
			var request_id := _ready_read(state.opening.media,0x801a4000,BoardStore.CUT_12_ID,BoardStore.CUT_12_SHA,{"lba":0xab18,"sectors":49,"destination":0x801a4000})
			if request_id.is_empty() or request_id == str(state.opening.get("cut_media",{}).get("request_id","")):
				result.error = "Rei line17 palette adjust lacks the completed case2 CUT12 reread"; return null
			state.opening["cut12_palette_adjusted"] = request_id
		"next_scene":
			_rei_line17_exit_setup(state,result)
	return null

func _rei_line17_exit_setup(state: State, result: Session.FrameResult) -> void:
	# prologue_line_17_run case6 after a successful AD05 decode:
	# actor_clear_upper, memcpy 0x801D8000 -> 0x80180000 (0x20800),
	# prologue_background_slot_set(0x801A4000,0xF), bg_layers = 0x80000000,
	# actors[96] O_13 tables with flags 5, then mode_stage_next.
	var media: Dictionary = state.opening.media
	var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_12_ID,BoardStore.CUT_12_SHA,{"lba":0xab18,"sectors":49,"destination":0x801a4000})
	var actor_id := _ready_read(media,0x801d8000,BoardStore.O_13_ID,BoardStore.O_13_SHA,{"lba":0xac50,"sectors":65,"destination":0x801d8000})
	if cut_id.is_empty() or actor_id.is_empty():
		result.error = "Rei line17 exit lacks completed original CUT12/O_13 reads"; return
	if state.opening.get("cut12_palette_adjusted") != cut_id:
		result.error = "Rei line17 exit lacks its source CUT12 palette adjustment"; return
	if state.opening.get("names_media",{}).get("bg_show_1") != 0 or state.opening.names_media.get("buffer_0",{}).get("lba") != 0xad05:
		result.error = "Rei line17 exit requires the decoded AD05 background"; return
	var bytes: PackedByteArray = _board_store.stage_transfer(BoardStore.O_13_ID).get("bytes",PackedByteArray())
	var rows := BoardStore.line17_sequence("o13_rei_line17_exit",bytes)
	var slots := BoardStore.line17_tpage_slots("o13_rei_line17_exit",bytes,rows[0].frame if not rows.is_empty() else -1)
	if rows.is_empty() or slots.is_empty():
		result.error = "Rei line17 exit O_13 tables do not resolve to source tpage slots"; return
	_actor_clear_upper(state)
	state.opening.erase("cut12_palette_adjusted")
	result.presentation.append({"kind":"first_arrival_o13_copy","resource_id":BoardStore.O_13_ID,"sha256":BoardStore.O_13_SHA,"bytes":0x20800})
	# The memcpy leaves O_13 as the whole modelled 0x80180000 actor area: the
	# base layer stage12 draws actor96 from and its O_14 copy lands on.
	state.opening["actor_area"] = [{"resource_id":BoardStore.O_13_ID,"sha256":BoardStore.O_13_SHA,"bytes":0x20800}]
	var cut_key := JSON.stringify([media.epoch,cut_id,BoardStore.CUT_12_ID,"line17_exit"]).sha256_text()
	state.opening["cut_media"] = {"resource_id":BoardStore.CUT_12_ID,"sha256":BoardStore.CUT_12_SHA,"request_id":cut_id,"upload_batch_key":cut_key,"white_palette":false}
	# prologue_background_slot_set(CUT12) then bg_layers = 0x80000000: the
	# whole attribute word is the display-off value stage 12 starts from
	# (ShioriScene, wiki/spec/opening-stages-12-14.md).
	state.opening["bg_layer_0"] = {"enabled":0,"palette_flag":1,"clut_row":495,"attribute":0x80000000,"resource_id":BoardStore.CUT_12_ID,"cut_batch_key":cut_key}
	var resident: Dictionary = {}
	for page: int in slots:
		var key := JSON.stringify([media.epoch,actor_id,BoardStore.O_13_ID,slots[page],page]).sha256_text()
		resident[str(slots[page])] = {"offset":page,"key":key}
	state.opening["actor96"] = {"profile":"o13_rei_line17_exit","index":96,"unk_01":10,"flags":5,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x80180204,"sequence_table":0x80180220,"tpage_table":0x801801e4,"sequence_slot":0,"seq_step":0,"sequence_counter":0,"speed":1,
		"frame_index":int(rows[0].frame),"x":-160,"y":-120,"brightness":128,"resource_id":BoardStore.O_13_ID,"source_sha256":BoardStore.O_13_SHA,
		"read_request":actor_id,"tpages":resident}
	state.opening["actor96_draw_flags"] = 0xa4
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":cut_key,"resource_id":BoardStore.CUT_12_ID,"sha256":BoardStore.CUT_12_SHA})
	for slot_key: String in resident:
		result.presentation.append({"kind":"first_arrival_stage_actor_upload","batch_key":resident[slot_key].key,"resource_id":BoardStore.O_13_ID,"slot":int(slot_key),"source_offset":resident[slot_key].offset})

# ---- Stages 12-14: prologue_shiori_load_run, _line_18_run, _line_19_run ----
# Logical owners only (wiki/spec/opening-stages-12-14.md). Actor records keep
# the source fields; ShioriActors.draw_all is the per-frame actor_draw_all.
# Uploads: the CUT pages/CLUTs at each prologue_background_slot_set, the
# AD0B/ACEC decode into buffer 1, and each newly resident search_tpage page.
# arrival_draw/paint_background hand the visible scene to the renderer's
# ShioriScene, which refuses frames that show the unexported P_01_08 buffer.
const O13_ACTOR96_TABLES := [0x80180204, 0x80180220, 0x801801e4]
const O14_TABLES := [0x801800f4, 0x80180114, 0x801800f0]
const O15_TABLES := [0x80180294, 0x801802c8, 0x80180284]
const O16_TABLES := [0x80180258, 0x80180298, 0x8018024c]

static func _shiori_actor(index: int, profile: String, unk_01: int, flags: int, draw_flags: int, tables: Array, x: int, base: Dictionary = {}) -> Dictionary:
	# The fields every Shiori setup writes; frame, timer and slot come from
	# base (the record before the write, cleared when absent).
	var actor := base.duplicate(true) if not base.is_empty() else ShioriActors.cleared(index)
	actor.merge({"profile":profile,"index":index,"unk_01":unk_01,"flags":flags,"quad_flags":0x41000000,"draw_flags":draw_flags,
		"frame_table":tables[0],"sequence_table":tables[1],"tpage_table":tables[2],"brightness":0x80,"x":x,"y":-0x78}, true)
	return actor

static func _record_actor(state: State, actor: Dictionary) -> void:
	state.opening["actor%d" % actor.index] = actor
	state.opening["actor%d_draw_flags" % actor.index] = actor.draw_flags

static func _actor_clear(state: State, index: int) -> void:
	state.opening.erase("actor%d" % index)
	state.opening.erase("actor%d_draw_flags" % index)
	state.opening.erase("actor%d_source_draw" % index)

static func _current(state: State, index: int) -> Dictionary:
	var actor: Variant = state.opening.get("actor%d" % index)
	return actor if actor is Dictionary else ShioriActors.cleared(index)

static func _just_committed(state: State, profile: String) -> bool:
	# Mode-time check: committed_frame names the draw frame (frame_count + 1).
	return state.opening.get("actor96",{}).get("profile") == profile and state.opening.actor96.get("committed_frame") == Math.u32(state.frame_count+1)

func _area_push(state: State, resource_id: String, sha256: String, length: int) -> String:
	var area: Variant = state.opening.get("actor_area")
	if not area is Array or area.is_empty(): return "Shiori memcpy needs the modelled 0x80180000 actor area"
	area = area.duplicate(true)
	area.append({"resource_id":resource_id,"sha256":sha256,"bytes":length})
	# Layers a newer copy covers completely can never be read again.
	var newest: Dictionary = area[-1]
	area = area.filter(func(layer): return layer.bytes > length or layer == newest)
	state.opening["actor_area"] = area
	return ""

static func _holds(buffer: Variant, lba: int) -> bool:
	# A slot-1 buffer holds `lba` when its cache entry names it, or when
	# dec_bg_reset cleared the entry but left the decoded pixels in VRAM.
	return buffer is Dictionary and (buffer.get("lba") == lba or (buffer.get("lba") == 0 and buffer.get("vram_lba") == lba))

func _shiori_decode(state: State, result: Session.FrameResult) -> int:
	# dec_bg_decode_to_vram(1) into the hidden buffer 1 - dec_bg_show[1], then
	# dec_bg_show_set. Stage12: shown AD05 (buffer0), ACF6 hidden -> AD0B into
	# buffer1. Stage14: stage13's show_switch showed AD05 again -> ACEC over
	# AD0B. After this substep's dec_bg_reset both cache LBAs are 0 and
	# dec_bg_show[1] is 0, so the retried decode also lands in buffer 1.
	var stage12 := state.mode_stage == 12
	var names_media: Variant = state.opening.get("names_media")
	if _shiori_bytes.is_empty() or not names_media is Dictionary or names_media.get("bg_show_1") != 0:
		result.error = "Shiori background decode requires the source slot-1 buffers"
		return -1
	var reset_here: bool = names_media.get("dec_bg_reset_at") is Array and names_media.dec_bg_reset_at.slice(0,2) == [state.mode_stage,state.substep]
	var cache_ok: bool
	if reset_here:
		cache_ok = names_media.get("buffer_0",{}).get("lba") == 0 and names_media.get("buffer_1",{}).get("lba") == 0
	elif stage12:
		cache_ok = names_media.get("buffer_0",{}).get("lba") == 0xad05 and names_media.get("buffer_1",{}).get("lba") == 0xacf6
	else:
		cache_ok = _holds(names_media.get("buffer_0"),0xad05) and names_media.get("buffer_1",{}).get("lba") == 0xad0b
	if not cache_ok:
		result.error = "Shiori background decode requires the source slot-1 buffers"
		return -1
	var resource_id: String = BoardStore.BG_AD0B_ID if stage12 else BoardStore.BG_ACEC_ID
	var media: Variant = state.opening.get("media")
	var request: Variant = media.get("requests",{}).get(media.get("background_pending",{}).get("1","")) if media is Dictionary else null
	if not request is Dictionary or request.get("kind") != "background" or request.get("resource_id") != resource_id or request.get("status") not in ["ready","retry"] or request.get("epoch") != media.get("epoch"):
		result.error = "opening background decode has no matching completed Shiori read"
		return -1
	if request.status == "retry": return -1
	var transfer_sha: String = BoardStore.BG_AD0B_TRANSFER_SHA if stage12 else BoardStore.BG_ACEC_TRANSFER_SHA
	var word_sha: String = BoardStore.BG_AD0B_WORD_SHA if stage12 else BoardStore.BG_ACEC_WORD_SHA
	var rect := [320,256,320,160]
	var batch_key := JSON.stringify([media.epoch,request.request_id,resource_id,transfer_sha,word_sha,rect]).sha256_text()
	media.decoded["1"] = {"resource_id":resource_id,"resource_sha256":transfer_sha,"buffer":1}
	names_media["buffer_1"] = {"lba":0xad0b if stage12 else 0xacec,"resource_id":resource_id,"transfer_sha256":transfer_sha,"word_sha256":word_sha,"decode_rect":rect,"request_id":request.request_id,"upload_batch_key":batch_key}
	names_media["bg_show_1"] = 1
	names_media.erase("dec_bg_reset_at")
	result.presentation.append({"kind":"opening_background_select","slot":1,"resource_id":resource_id,"buffer":1})
	result.presentation.append({"kind":"first_arrival_names_bg_upload","batch_key":batch_key,"resource_id":resource_id,"buffer":1,"decode_rect":rect,"word_sha256":word_sha})
	return 1

func _dec_bg_reset_slot1(state: State, result: Session.FrameResult) -> void:
	# dec_bg_reset (background_images.c:37-48) zeroes bg_lba_cache[0..3],
	# dec_bg_show[0..2], sets bg_brightness 0x80, dec_bg_last_screen_mode -1
	# and D_800B5934 0. Modelled here: the slot-1 cache entries
	# bg_lba_cache[2]/[3] (buffer_0/buffer_1, the caches dec_bg_cd_read(lba,1)
	# consults), dec_bg_show[1] and bg_brightness. VRAM is untouched, so each
	# buffer keeps its pixels; only the cache entry is cleared (vram_lba
	# records what the pixels are). Not modelled: the slot-0 caches
	# bg_lba_cache[0]/[1], dec_bg_show[0]/[2], the last screen mode and
	# D_800B5934. Stages 11-14 read none of them (every background read there
	# is dec_bg_cd_read(lba,1), and dec_bg_draw(1) rewrites the screen mode the
	# same frame); no slot-0 cache is modelled anywhere a later hit could use.
	var names_media: Variant = state.opening.get("names_media")
	if not names_media is Dictionary or not names_media.get("buffer_0") is Dictionary or not names_media.get("buffer_1") is Dictionary:
		result.error = "dec_bg_reset requires the slot-1 buffer records"; return
	for key: String in ["buffer_0","buffer_1"]:
		var buffer: Dictionary = names_media[key]
		if buffer.get("lba",0) != 0: buffer["vram_lba"] = buffer.lba
		buffer["lba"] = 0
	names_media["bg_show_1"] = 0
	names_media["dec_bg_reset_at"] = [state.mode_stage,state.substep,state.frame_count]
	state.opening["bg_brightness"] = 0x80

func _shiori_cut(state: State, cut_id: String, cut_sha: String, request_id: String, result: Session.FrameResult) -> void:
	# prologue_background_slot_set(0x801A4000, 0xF): prim_bg_slot_setup locks
	# the six pages in tpage slots 8-10/40-42 and resets the layer colour to
	# 0x80 (gpu_effects.c:67-69, D_801220E0..E2); load_palette rows 0x1EF and
	# the gray 0x1EE; order 0xD (OT12); attribute 0x41000000.
	var key := JSON.stringify([state.opening.media.epoch,request_id,cut_id,cut_sha,state.mode_stage,state.frame_count]).sha256_text()
	state.opening["cut_media"] = {"resource_id":cut_id,"sha256":cut_sha,"request_id":request_id,"upload_batch_key":key,"white_palette":false}
	state.opening["bg_layer_0"] = {"enabled":1,"palette_flag":1,"clut_row":495,"gray_clut_row":494,"attribute":0x41000000,"resource_id":cut_id,"cut_batch_key":key}
	for component in ["D_801220E0","D_801220E1","D_801220E2"]: state.opening[component] = 0x80
	result.presentation.append({"kind":"first_arrival_cut_upload","batch_key":key,"resource_id":cut_id,"sha256":cut_sha})

func _shiori_copy(state: State, resource_id: String, result: Session.FrameResult) -> String:
	# memcpy(0x80180000, source, n): the logical layer and the RAM transfer.
	var copy: Array = SHIORI_COPIES[resource_id]
	var sha: String = _shiori_bytes[resource_id].sha256
	var error := _area_push(state,resource_id,sha,int(copy[1]))
	if error.is_empty(): result.presentation.append({"kind":"first_arrival_shiori_copy","resource_id":resource_id,"sha256":sha,"bytes":int(copy[1]),"source":int(copy[0])})
	return error

func _shiori_tpage_uploads(state: State, page_slots: Dictionary, result: Session.FrameResult) -> Variant:
	# search_tpage leaves free slots unlocked (tpage_buf stays -1), so the
	# source queues load_csr_tp for every drawn page on every frame. VRAM
	# changes only when a slot receives another page or the page's RAM bytes
	# change (a newer memcpy layer), so an upload is emitted exactly then.
	var previous: Variant = state.opening.get("shiori_tpages")
	var resident: Dictionary = previous.resident.duplicate(true) if previous is Dictionary and previous.get("resident") is Dictionary else {}
	var uploads: Array[Dictionary] = []
	for page: int in page_slots:
		var slot: int = page_slots[page]
		var layer := ShioriActors.page_layer(_shiori_bytes,state.opening.get("actor_area"),page)
		if not layer.error.is_empty(): return layer.error
		var held: Variant = resident.get(str(slot))
		if held is Dictionary and held.get("page") == page and held.get("resource_id") == layer.resource_id: continue
		var key := ""
		# Every layer is a memcpy of SHIORI_COPIES (O_13 is stage11's copy
		# that stage12 draws before its commit), so each page has an owner.
		if SHIORI_COPIES.has(layer.resource_id):
			key = JSON.stringify([state.opening.get("media",{}).get("epoch",-1),layer.resource_id,slot,page,state.frame_count]).sha256_text()
			uploads.append({"kind":"first_arrival_stage_actor_upload","batch_key":key,"resource_id":layer.resource_id,"slot":slot,"source_offset":int(layer.offset)})
		resident[str(slot)] = {"page":page,"resource_id":layer.resource_id,"key":key}
	if uploads.size() > 3: return "Shiori tpage uploads exceed the three slots one frame can load"
	result.presentation.append_array(uploads)
	return resident

func shiori_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	if _shiori_bytes.is_empty() or state.game_mode != 0x15 or state.mode_step != 0 or state.mode_stage not in [12,13,14,15]:
		result.error = "Shiori stage effect requires the authenticated stages 12-14 group"; return null
	var at := [state.mode_stage,state.substep]
	var ok := false
	match kind:
		"load_gate", "load_setup": ok = at == [12,3]
		"dec_bg_reset": ok = at in [[12,3],[14,6]]
		"wait_voice": ok = at == [13,1]
		"lip_sync": ok = at in [[13,2],[13,3]]
		"line18_setup": ok = at == [13,4]
		"blink_line18": ok = (state.mode_stage == 13 and state.substep in [0,1,2,3,4]) or (at == [14,0] and _just_committed(state,"o15_shiori_line19"))
		"lip_sync_line19": ok = at in [[14,1],[14,2]]
		"actors_reset": ok = at == [14,2]
		"turn_done": ok = at == [14,3]
		"line19_setup": ok = at == [14,6]
		"blink_line19": ok = (state.mode_stage == 14 and state.substep in [0,1,2,3,4,5,6]) or (at == [15,0] and _just_committed(state,"o16_shiori_line20"))
		"actor_draw": ok = state.mode_stage in [12,13,14] or (at == [15,0] and _just_committed(state,"o16_shiori_line20"))
	if not ok or value != 0:
		result.error = "Shiori stage effect is outside its source substep"; return null
	match kind:
		"load_gate":
			# case3: (cd_read_sync_status() & 1) && !(D_80120652 & 1); stage11
			# case6 left actor96 on O_13 with flags 5 (prologue.c:1803-1818).
			var a96: Variant = state.opening.get("actor96")
			if not a96 is Dictionary or [a96.get("frame_table"),a96.get("sequence_table"),a96.get("tpage_table")] != O13_ACTOR96_TABLES or not a96.get("flags") is int:
				result.error = "stage12 gate requires stage11's O_13 actor96"; return null
			return (a96.flags & 1) == 0
		"load_setup":
			var media: Dictionary = state.opening.get("media",{})
			var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_14_ID,BoardStore.CUT_14_SHA,{"lba":0xab49,"sectors":49,"destination":0x801a4000})
			var actor_id := _ready_read(media,0x801f0000,BoardStore.O_14_ID,BoardStore.O_14_SHA,{"lba":0xac91,"sectors":9,"destination":0x801f0000})
			if cut_id.is_empty() or actor_id.is_empty():
				result.error = "stage12 setup lacks completed original CUT_14/O_14 reads"; return null
			if state.opening.get("names_media",{}).get("bg_show_1") != 1 or state.opening.names_media.get("buffer_1",{}).get("lba") != 0xad0b:
				result.error = "stage12 setup requires the decoded AD0B background"; return null
			var area: Variant = state.opening.get("actor_area")
			if not area is Array or area.is_empty() or area[-1].get("resource_id") != BoardStore.O_13_ID or area[-1].get("bytes") != 0x20800:
				result.error = "stage12 setup requires stage11's O_13 actor area"; return null
			_actor_clear_upper(state)
			_shiori_cut(state,BoardStore.CUT_14_ID,BoardStore.CUT_14_SHA,cut_id,result)
			var error := _shiori_copy(state,BoardStore.O_14_ID,result)
			if not error.is_empty(): result.error = error; return null
			var a97 := _shiori_actor(97,"o14_shiori_mouth",9,0x40,0xa4,O14_TABLES,-0xa0)
			a97["frame_index"] = 4
			_record_actor(state,a97)
		"wait_voice":
			# case1 polls cd_audio_state only; prologue_line_run is not called.
			var work: Variant = state.opening.get("prologue_text_work")
			if not work is Dictionary or work.get("line_id") != 18 or work.get("next_frame") != state.frame_count:
				result.error = "Shiori line18 voice wait lost its expected text tick"; return null
			work.next_frame = Math.u32(state.frame_count+1)
		"lip_sync":
			# prologue_lip_synchronization_shiori (prologue.c:1884).
			var lip := AudioRules.lip_sync_shiori(state)
			if lip.cd_active:
				var a97 := _shiori_actor(97,"o14_shiori_mouth",9,0x40,0xa4,O14_TABLES,-0xa0,_current(state,97))
				a97["frame_index"] = int(lip.ring_frame)+4
				_record_actor(state,a97)
			else:
				var a97 := _current(state,97)
				a97["frame_index"] = 4
				_record_actor(state,a97)
		"blink_line18":
			# prologue_actor98_animation_start_shiori: frame and timer kept.
			var a98 := _current(state,98)
			if (int(a98.flags) & 1) != 0: return null
			a98 = _shiori_actor(98,"o14_shiori_blink" if state.mode_stage == 13 else "o14_tables_over_o15",9,0x11,0xac,O14_TABLES,-0xa0,a98)
			a98["sequence_slot"] = 0
			_record_actor(state,a98)
		"line18_setup":
			var media: Dictionary = state.opening.get("media",{})
			var actor_id := _ready_read(media,0x801e8000,BoardStore.O_15_ID,BoardStore.O_15_SHA,{"lba":0xac9a,"sectors":33,"destination":0x801e8000})
			if actor_id.is_empty():
				result.error = "stage13 setup lacks the completed original O_15 read"; return null
			var names_media: Variant = state.opening.get("names_media")
			if not names_media is Dictionary or names_media.get("bg_show_1") != 1 or not _holds(names_media.get("buffer_0"),0xad05) or names_media.get("buffer_1",{}).get("lba") != 0xad0b or state.opening.get("bg_layer_0",{}).get("resource_id") != BoardStore.CUT_14_ID:
				result.error = "stage13 setup requires the stage12 AD0B/CUT_14 scene"; return null
			_actor_clear_upper(state)
			names_media["bg_show_1"] = 0
			state.opening.bg_layer_0["attribute"] = 0x80000000
			state.opening.bg_layer_0["enabled"] = 0
			var error := _shiori_copy(state,BoardStore.O_15_ID,result)
			if not error.is_empty(): result.error = error; return null
			var a96 := _shiori_actor(96,"o15_shiori_line19",9,0x40,0xa4,O15_TABLES,-0xa0)
			a96["committed_frame"] = Math.u32(state.frame_count+1)
			_record_actor(state,a96)
		"lip_sync_line19":
			# prologue_lip_synchronization_shiori_line_19: actor_clear(0x61)
			# every call, then the O_15 mouth only while the voice plays.
			var lip := AudioRules.lip_sync_shiori_line_19(state)
			_actor_clear(state,97)
			if lip.cd_active:
				var a97 := _shiori_actor(97,"o15_shiori_mouth",8,0x40,0xa4,O15_TABLES,-0xa0)
				a97["frame_index"] = int(lip.ring_frame)+5
				_record_actor(state,a97)
		"actors_reset":
			if state.opening.get("actor96",{}).get("profile") != "o15_shiori_line19":
				result.error = "Shiori line19 actor reset requires the O_15 actor96"; return null
			for index in [96,97,98,99]: _actor_clear(state,index)
			var a99 := _shiori_actor(99,"o15_shiori_turn",8,5,0xa4,O15_TABLES,-0xa0)
			a99["sequence_slot"] = 2
			_record_actor(state,a99)
		"turn_done":
			var a99: Variant = state.opening.get("actor99")
			if not a99 is Dictionary or a99.get("profile") != "o15_shiori_turn" or not a99.get("flags") is int:
				result.error = "Shiori line19 turn wait lost its actor99"; return null
			return (a99.flags & 1) == 0
		"blink_line19":
			if (int(_current(state,98).flags) & 1) != 0: return null
			# prologue_actor98_animation_start_shiori_line_19 tests actor99 itself.
			if (int(_current(state,99).flags) & 1) != 0: return null
			var sequence := ShioriActors.word(_shiori_bytes,state.opening.get("actor_area"),O15_TABLES[1])
			if state.mode_stage == 15 or sequence < 0 or not ShioriActors.in_area(_shiori_bytes,state.opening.get("actor_area"),sequence):
				result.error = SHIORI_COMMIT_BLINK_UNSUPPORTED; return null
			_actor_clear(state,98)
			var a98 := _shiori_actor(98,"o15_shiori_blink",8,3,0xac,O15_TABLES,-0xa0)
			a98["sequence_slot"] = 0
			_record_actor(state,a98)
		"line19_setup":
			var media: Dictionary = state.opening.get("media",{})
			var cut_id := _ready_read(media,0x801a4000,BoardStore.CUT_16_ID,BoardStore.CUT_16_SHA,{"lba":0xab7a,"sectors":49,"destination":0x801a4000})
			var actor_id := _ready_read(media,0x801e8000,BoardStore.O_16_ID,BoardStore.O_16_SHA,{"lba":0xacbb,"sectors":25,"destination":0x801e8000})
			if cut_id.is_empty() or actor_id.is_empty():
				result.error = "stage14 setup lacks completed original CUT_16/O_16 reads"; return null
			if state.opening.get("names_media",{}).get("bg_show_1") != 1 or state.opening.names_media.get("buffer_1",{}).get("lba") != 0xacec:
				result.error = "stage14 setup requires the decoded ACEC background"; return null
			_actor_clear_upper(state)
			_shiori_cut(state,BoardStore.CUT_16_ID,BoardStore.CUT_16_SHA,cut_id,result)
			var error := _shiori_copy(state,BoardStore.O_16_ID,result)
			if not error.is_empty(): result.error = error; return null
			var a96 := _shiori_actor(96,"o16_shiori_line20",0xb,0x40,0xa4,O16_TABLES,-0xf0)
			a96["resource_id"] = BoardStore.O_16_ID
			a96["committed_frame"] = Math.u32(state.frame_count+1)
			_record_actor(state,a96)
		"dec_bg_reset":
			_dec_bg_reset_slot1(state,result)
		"actor_draw":
			# The records as actor_draw_all reads them, before its post-draw
			# sequence advance; the renderer draws exactly these.
			var before: Dictionary = {}
			for index in range(96,160):
				var record: Variant = state.opening.get("actor%d" % index)
				if record is Dictionary: before[index] = record.duplicate(true)
			# actor_draw_all runs in draw_frame, after game_mode_dispatch's
			# frame_count increment; its records are owned by that value.
			var draw_frame: int = Math.u32(state.frame_count+1)
			var drawn := ShioriActors.draw_all(_shiori_bytes,state.opening,draw_frame)
			if not drawn.error.is_empty(): result.error = drawn.error; return null
			var resident: Variant = _shiori_tpage_uploads(state,drawn.slots,result)
			if resident is String: result.error = resident; return null
			var slots: Dictionary = {}
			for page: int in drawn.slots: slots["0x%x" % page] = drawn.slots[page]
			state.opening["shiori_tpages"] = {"slots":slots,"drawn":drawn.drawn,"resident":resident,"frame_count":draw_frame}
			var records: Array = []
			for item: Array in drawn.drawn:
				var a: Dictionary = before[item[0]]
				records.append({"index":item[0],"frame":item[1],"unk_01":int(a.get("unk_01",0)),"draw_flags":int(a.get("draw_flags",0)),"unk_05":int(a.get("brightness",0)),
					"quad_flags":int(a.get("quad_flags",0)),"frame_table":int(a.frame_table),"tpage_table":int(a.tpage_table),"x":int(a.get("x",0)),"y":int(a.get("y",0))})
			state.opening["shiori_draw"] = {"frame_count":draw_frame,"actors":records}
	return null

# --- Stages 18-20: prologue_lines_38_to_41_run, prologue_flash_run and
# prologue_closing_run (prologue.c:2565-2706). Preconditions come from the
# real stages 16-17 and are checked here, never re-established.

func _closing_background_error(state: State) -> String:
	# Stage16 decodes P_01_10 into the hidden slot-1 buffer and shows it, then
	# clears layer 0 (bg_layers = 0x80000000, prologue.c:2514); stage17 decodes
	# P_01_12 into the other buffer and shows it (prologue.c:2540-2549). The
	# stage0 load_palette loop (prologue.c:258-260) is the only load of CLUT
	# row 0x1FB that the O_13 actor uses.
	var names: Variant = state.opening.get("names_media")
	if not state.opening.get("media") is Dictionary or not names is Dictionary or names.get("bg_show_1") not in [0, 1]:
		return "stages 18-20 lack the slot 1 background owner"
	var shown: Variant = names.get("buffer_%d" % int(names.bg_show_1))
	var hidden: Variant = names.get("buffer_%d" % (1-int(names.bg_show_1)))
	if not shown is Dictionary or shown.get("lba") != CLOSING_BG_SHOWN_LBA or not hidden is Dictionary or hidden.get("lba") != CLOSING_BG_OTHER_LBA:
		return "stages 18-20 require P_01_12 shown with P_01_10 in the other slot 1 buffer"
	if not state.opening.get("bg_layer_0") is Dictionary or state.opening.bg_layer_0.get("attribute") != CLOSING_LAYER0_CLEARED:
		return "stages 18-20 require the bg layer 0 cleared by stage16"
	var arrival: Variant = state.opening.get("arrival_media")
	if not arrival is Dictionary or arrival.get("actor_initialized") != true or not arrival.get("upload_batch_key") is String:
		return "stages 18-20 require the stage0 O_01 palette rows including CLUT row 0x1FB"
	return ""

static func _upper_actors_cleared(state: State) -> bool:
	for index in range(96,160):
		if state.opening.has("actor%d" % index) or state.opening.get("actor%d_draw_flags" % index,0) != 0: return false
	return true

func _o13_request(state: State, statuses: Array) -> String:
	# Stage17's unwaited cd_read_sectors(0x41,0x80180000,0xAC50).
	var media: Dictionary = state.opening.media
	var request_id := str(media.get("actor_pending",{}).get(str(O13_DESTINATION),""))
	var request: Variant = media.get("requests",{}).get(request_id)
	if not request is Dictionary or request.get("kind") != "actors" or request.get("resource_id") != BoardStore.O_13_ID or request.get("resource_sha256") != BoardStore.O_13_SHA or request.get("epoch") != media.get("epoch") or request.get("status") not in statuses:
		return ""
	if not Store._same(request.get("payload"),{"lba":0xac50,"sectors":0x41,"destination":O13_DESTINATION}): return ""
	return request_id

func _closing_owner_error(state: State) -> String:
	# Background owner plus the actor96 state each source substep implies.
	var error := _closing_background_error(state)
	if not error.is_empty(): return error
	var before_setup: bool = state.mode_stage == 18 or (state.mode_stage == 19 and state.substep == 0)
	if before_setup:
		if not _upper_actors_cleared(state): return "stages 18-19 require actors 96-159 cleared by stage16"
		if _o13_request(state,["pending","ready"]).is_empty(): return "stages 18-19 require stage17's O_13 read to 0x80180000"
		return ""
	var actor: Variant = state.opening.get("actor96")
	var visible: bool = state.mode_stage == 19 and state.substep in [1, 2]
	if not actor is Dictionary or actor.get("profile") != "o13_flash" or actor.get("draw_flags") != (0xa4 if visible else 0) or state.opening.get("actor96_draw_flags") != actor.draw_flags:
		return "stages 19-20 require the source O_13 actor96 state"
	return ""

func _closing_upload_error(state: State, result: Session.FrameResult = null) -> String:
	# Stage17 case3 decoded P_01_12 into the hidden buffer 1 - dec_bg_show[1]
	# with its upload owner (background_decode) and showed it through stages
	# 18-20. The upload is published, or queued by this frame (the
	# stage17->18 frame decodes and then runs stage18's dec_bg_draw).
	var names: Dictionary = state.opening.names_media
	var show: Variant = names.get("bg_show_1")
	var shown: Variant = names.get("buffer_%d" % show) if show is int and show in [0,1] else null
	if not shown is Dictionary or shown.get("resource_id") != BoardStore.BG_4973_ID or shown.get("word_sha256") != BoardStore.BG_4973_WORD_SHA or not Store._same(shown.get("decode_rect"),BoardStore.SLOT1_BUFFER_RECTS[show]) or not shown.get("upload_batch_key") is String:
		return CLOSING_BG_NOT_UPLOADED
	if _names_bg_uploads.has(shown.upload_batch_key): return ""
	if result != null:
		for intent: Dictionary in result.presentation:
			if intent.get("kind") == "first_arrival_names_bg_upload" and intent.get("batch_key") == shown.upload_batch_key and intent.get("resource_id") == BoardStore.BG_4973_ID: return ""
	return CLOSING_BG_NOT_UPLOADED

static func closing_exit_frame(state: State) -> bool:
	# Committed 21/0 that still carries stage20's line 42 (the case2 exit
	# frame, or a START that left stage 20 after its draw). Only stage20
	# starts line 42, and stage21 case0 leaves 21/0 on its first frame.
	return state != null and state.game_mode == 0x15 and state.mode_stage == 21 and state.mode_step == 0 and state.substep == 0 and state.opening.get("prologue_line") == CLOSING_EXIT_LINE

## prologue_finish_run's own frames: stage 21 after the 20->21 exit frame,
## and case1's mode switch committed as 0x34/0xFF/0/0. Neither case calls
## dec_bg_draw (prologue.c:2714-2741), so nothing is drawn below the HUD.
static func finish_frame(state: State) -> bool:
	if state == null or state.mode_step != 0: return false
	if state.game_mode == 0x34: return state.mode_stage == 0xff and state.substep == 0
	return state.game_mode == 0x15 and state.mode_stage == 21 and not closing_exit_frame(state)

static func closing_frame(state: State) -> bool:
	return state != null and state.game_mode == 0x15 and (state.mode_stage in [18, 19, 20] or closing_exit_frame(state))

func _closing_exit_error(state: State, hold_exit: bool) -> String:
	# The 21/0 composition is stage20's: P_01_12 shown, actor96 hidden since
	# stage19 case2, no box records (prologue_closing_run draws none).
	# hold_exit pins the counter that case2's post-increment leaves (0x3D+1);
	# only that exit calls dec_bg_draw at stage 21.
	if not closing_exit_frame(state): return CLOSING_EXIT_UNSUPPORTED
	if hold_exit and state.opening.get("prologue_scene_counter") != CLOSING_EXIT_COUNTER: return CLOSING_EXIT_UNSUPPORTED
	var draws: Variant = state.opening.get("prologue_closing_draws",[])
	if not draws is Array or not draws.is_empty() or state.opening.has("o13_draw"): return CLOSING_EXIT_UNSUPPORTED
	return ""

func _closing_primitive(color: int, tag: int, mode: int) -> Dictionary:
	# prim_gradient_rect(-0xA0,-0x78,0xA0,0xA0,...) and (0,-0x78,0xA0,0xA0,...)
	return {"tag":tag,"color":color,"mode":mode,"rects":[[-160,-120,160,160],[0,-120,160,160]]}

func closing_effect(state: State, kind: String, value: int, result: Session.FrameResult) -> Variant:
	if _board_store == null or state.game_mode != 0x15 or state.mode_step != 0 or state.mode_stage not in [18, 19, 20]:
		result.error = "closing effect requires source stages 18-20"; return null
	# Substeps are the values when the flow calls the effect: actor_hide
	# follows case2's substep += 1, actor_draw follows the whole dispatch.
	var allowed: Dictionary = {
		18:{"wait_voice":[1],"actor_draw":range(10)},
		19:{"flash":[0,1,2,3],"white":[1,2],"actor_setup":[0],"actor_hide":[3],"actor_draw":range(5)},
		20:{"actor_draw":[0,1,2]}}[state.mode_stage]
	if not allowed.has(kind) or state.substep not in allowed[kind]:
		result.error = "closing effect is outside its source substep"; return null
	var error := _closing_background_error(state)
	if not error.is_empty():
		result.error = error; return null
	match kind:
		"wait_voice":
			# Case1 waits for XA playback without prologue_line_run.
			var work: Variant = state.opening.get("prologue_text_work")
			if not work is Dictionary or work.get("line_id") != 38 or work.get("next_frame") != state.frame_count:
				result.error = "line38 voice wait lost its expected text tick"; return null
			work.next_frame = Math.u32(state.frame_count+1)
		"white", "flash":
			# prologue_white_boxes_draw / prologue_flash_boxes_draw (prologue.c:1257-1275).
			var draws: Array = state.opening.get("prologue_closing_draws",[])
			var expected_before: int = 1 if kind == "flash" and state.substep in [1, 2] else 0
			if draws.size() != expected_before:
				result.error = "closing boxes are outside their source draw order"; return null
			if kind == "white":
				draws.append(_closing_primitive(0xffffff,0xd,1))
			else:
				var counter: Variant = state.opening.get("prologue_scene_counter")
				if not counter is int or counter != value or value < 0 or value > 0x100 or value % 4 != 0:
					result.error = "flash boxes require the source scene counter"; return null
				var flash := _closing_primitive(value*0x10101 if value < 0x100 else 0xffffff,0xb,0x81)
				flash["draw_mode"] = {"tag":0xb,"abr":1,"tp":1}
				draws.append(flash)
			state.opening["prologue_closing_draws"] = draws
		"actor_setup":
			# Case0 exit: actor_clear(0x60) and the O_13 fields (prologue.c:2629-2643).
			if state.opening.has("actor96") or not _upper_actors_cleared(state):
				result.error = "stage19 actor96 setup cannot replay over a live actor"; return null
			if state.opening.get("prologue_scene_counter") != 0x100:
				result.error = "stage19 actor96 setup is outside the flash peak"; return null
			var request_id := _o13_request(state,["ready"])
			if request_id.is_empty():
				result.error = "stage19 actor96 setup lacks the completed O_13 read"; return null
			var transfer := _board_store.stage_transfer(BoardStore.O_13_ID)
			if not str(transfer.get("error","missing")).is_empty() or transfer.get("sha256") != BoardStore.O_13_SHA or BoardStore.o13_sequence(transfer.bytes).is_empty():
				result.error = "stage19 O_13 tables are unavailable"; return null
			state.opening["actor96"] = {"profile":"o13_flash","index":96,"unk_01":0xc,"flags":5,"quad_flags":0x41000000,"draw_flags":0xa4,
				"frame_table":0x80180204,"sequence_table":0x80180220,"tpage_table":0x801801e4,"sequence_slot":0,"seq_step":0,"sequence_counter":0,
				"blink_period":2,"unk_05":0x80,"unk_43":0,"brightness":0x80,"scale":0x1000,"x":-0xa0,"y":-0x78,"frame_index":0,
				"clut_row":O13_CLUT_ROW,"resource_id":BoardStore.O_13_ID,"source_sha256":BoardStore.O_13_SHA,"read_request":request_id}
			state.opening["actor96_draw_flags"] = 0xa4
		"actor_hide":
			# Case2 exit: D_80120653 = 0.
			var actor: Variant = state.opening.get("actor96")
			if not actor is Dictionary or actor.get("profile") != "o13_flash" or actor.get("draw_flags") != 0xa4 or state.opening.get("prologue_scene_counter") != 0x100:
				result.error = "stage19 actor96 hide lacks the visible O_13 actor"; return null
			actor.draw_flags = 0
			state.opening["actor96_draw_flags"] = 0
			state.opening.erase("o13_draw")
		"actor_draw":
			_o13_actor_draw(state,result)
	return null

static func _closing_case2_exit(state: State) -> bool:
	# prologue_flash_run case2 (prologue.c:2655-2663): the frame whose counter
	# reaches 0x100 also sets substep 3 and D_80120653 = 0 before draw_frame,
	# so 19/2 with the counter at 0x100 and a visible actor96 never exists.
	var counter: Variant = state.opening.get("prologue_scene_counter")
	return state.mode_stage == 19 and state.substep == 2 and counter is int and counter >= 0x100

func _o13_actor_draw(state: State, result: Session.FrameResult) -> void:
	# actor_draw_all for actor96 (actor_sprites.c:84-346), run by draw_frame
	# after game_mode_dispatch and frame_count += 1 (main_loop.c:50-70): draw
	# the current sequence row, then step it. Hidden actors neither draw nor step.
	state.opening.erase("o13_draw")
	var actor: Variant = state.opening.get("actor96")
	if not actor is Dictionary:
		if not _upper_actors_cleared(state): result.error = "closing actor draw found an unowned upper actor"
		return
	if actor.get("profile") != "o13_flash":
		result.error = "closing actor draw requires the O_13 actor96 owner"; return
	if (int(actor.get("draw_flags",0)) & 0x80) == 0: return
	if _closing_case2_exit(state):
		result.error = CLOSING_CASE2_EXIT_DRAW; return
	var bytes: PackedByteArray = _board_store.stage_transfer(BoardStore.O_13_ID).get("bytes",PackedByteArray())
	var rows := BoardStore.o13_sequence(bytes)
	var step: Variant = actor.get("seq_step")
	var counter: Variant = actor.get("sequence_counter")
	var flags: Variant = actor.get("flags")
	if rows.is_empty() or not step is int or not counter is int or flags not in [0, 5] or step < 0 or step >= rows.size() or actor.get("blink_period") != 2:
		result.error = "O_13 actor96 sequence state is outside the source states"; return
	var drawn: int = rows[step][0]
	var slots := BoardStore.o13_tpage_slots(bytes,drawn)
	if slots.is_empty():
		result.error = "O_13 frame exceeds the source free tpage slots"; return
	var frame_count: int = Math.u32(state.frame_count+1)
	var mapped: Dictionary = {}
	# search_tpage reloads every frame; an upload is emitted only where a
	# slot's resident page changes (as _o11_tpage_update does for stage10).
	var resident: Dictionary = state.opening.get("o13_tpages",{}).get("resident",{}).duplicate(true)
	var uploads: Array[Dictionary] = []
	for page: int in slots:
		var slot: int = slots[page]
		mapped[str(page)] = slot
		if resident.get(str(slot),{}).get("offset") == page: continue
		var key := JSON.stringify([state.opening.media.epoch,actor.get("read_request"),BoardStore.O_13_ID,slot,page,state.frame_count]).sha256_text()
		resident[str(slot)] = {"offset":page,"key":key}
		uploads.append({"kind":"first_arrival_stage_actor_upload","batch_key":key,"resource_id":BoardStore.O_13_ID,"slot":slot,"source_offset":page})
	result.presentation.append_array(uploads)
	state.opening["o13_tpages"] = {"slots":mapped.duplicate(),"resident":resident}
	state.opening["o13_draw"] = {"frame":drawn,"slots":mapped,"frame_count":frame_count}
	if (flags & 1) == 1:
		if frame_count % 2 == 0: counter += 1
		if counter >= int(rows[step][1]):
			step += 1
			counter = 0
		if step >= rows.size():
			# flags 5: bit2 clears bits 0 and 2 and holds the last row.
			flags = flags & 0xfffa
			step -= 1
	actor.seq_step = step
	actor.sequence_counter = counter
	actor.flags = flags
	actor.frame_index = int(rows[step][0])

static func _rei_dialogue_actor(state: State, index: int) -> Dictionary:
	var crowd := index in [99,100]
	return {"profile":"o10_rei_dialogue","index":index,"unk_01":12 if crowd else 9,"flags":0,"quad_flags":0x41000000,"draw_flags":0xa4,
		"frame_table":0x80180110 if crowd else 0x80184288,"sequence_table":0x80180130 if crowd else 0x801842ac,"tpage_table":0x8018010c if crowd else 0x80184280,
		"sequence":"crowd_left" if index == 99 else ("crowd_right" if index == 100 else "blink"),"sequence_slot":1 if index == 100 else 0,
		"seq_step":0,"frame_index":4 if index == 100 else (0 if index == 99 else 1),"sequence_counter":0,"speed":1,"x":-160,"y":-120,"brightness":128,"clut_row":506 if crowd else 505,
		"resource_id":BoardStore.O_09_ID,"source_sha256":BoardStore.O_09_SHA,"upload_batch_key":state.opening.actor96.upload_batch_key,"upload_batch_key_2":state.opening.actor96.upload_batch_key_2}

func stage_07_cd_wait(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 7 or state.mode_step != 0 or state.substep != 1:
		result.error = "first-arrival stage 7 CD wait is outside stage 7 substep 1"
		return
	var ready: bool = (not state.opening.has("prologue_lip_sync") and not state.opening.has("cd_audio_state_override")) or (AudioRules.check_cd(state) & 4) == 4
	if ready:
		state.substep += 1
	if state.opening.get("prologue_text_work") is Dictionary and state.opening.prologue_text_work.has("next_frame"):
		state.opening.prologue_text_work["next_frame"] = Math.u32(state.frame_count + 1)

func stage_07_lip_sync_line_11(state: State, result: Session.FrameResult) -> void:
	if _board_store == null or state.game_mode != 0x15 or state.mode_stage != 7 or state.mode_step != 0 or state.substep not in [2, 3] or not state.opening.get("o07_batches") is Dictionary:
		result.error = "first-arrival stage 7 line 11 lip-sync is outside stage 7 substep 2..3"
		return
	var lip: Dictionary = {}
	if state.opening.get("prologue_lip_sync") is Dictionary and state.opening.get("audio") is Dictionary:
		lip = state.opening.prologue_lip_sync
	else:
		lip = AudioRules.lip_sync_line_11(state)
	for idx in [97, 98]:
		state.opening.erase("actor%d" % idx)
		state.opening.erase("actor%d_draw_flags" % idx)
		state.opening.erase("actor%d_source_draw" % idx)
	if bool(lip.get("cd_active",false)):
		var ring_frame: int = clampi(int(lip.get("ring_frame",0)),0,3)
		state.opening["actor97"] = {
			"profile":"o07_yoshio_mouth",
			"index":97,"flags":0x40,"draw_flags":0xa4,
			"x":-160,"y":-120,"frame_index":ring_frame + 1,"sequence_counter":0,"clut_row":502,
			"upload_batch_key":state.opening.o07_batches.slot_5,"upload_batch_key_2":state.opening.o07_batches.slot_6,
		}
		state.opening["actor97_draw_flags"] = 0xa4
		state.opening["actor98"] = {
			"profile":"o07_yoshio_hand",
			"index":98,"flags":0x40,"draw_flags":0xa4,
			"x":-160,"y":-120,"frame_index":ring_frame + 5,"sequence_counter":0,"clut_row":502,
			"upload_batch_key":state.opening.o07_batches.slot_7,
		}
		state.opening["actor98_draw_flags"] = 0xa4

func board_scroll_draw(state: State, counter: int, result: Session.FrameResult) -> void:
	var stage1_scroll: bool = state.mode_stage == 1 and state.mode_step == 1 and state.substep in [1, 2, 3, 4, 5]
	var stage2_transition: bool = state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and counter == 320 and state.opening.get("names_media") is Dictionary and state.opening.names_media.get("active_draw") == "board_scroll_transition"
	if _board_store == null or state.game_mode != 0x15 or (not stage1_scroll and not stage2_transition) or not counter is int or counter < 0 or counter > 320 or state.opening.get("prologue_scene_counter") != counter or not state.opening.get("board_media") is Dictionary or state.opening.board_media.get("decoded") != true:
		result.error = "first-arrival board scroll draw is outside its source stage"
		return
	state.opening.board_media["scroll_x"] = counter
	state.opening.board_media["tpage_show"] = {"tpage_x":5,"tpage_y":2,"semi_trans":0,"rect":[-160,-120,320,160],"scroll_x":counter,"scroll_y":0,"src_x":0,"src_y":0,"ot_slot":13}
	result.presentation.append({"kind":"first_arrival_board_scroll_draw","scroll_x":counter,"rect":[-160,-120,320,160],"tpage_args":[5,2,0,0,0,0,13]})

func arrival_draw(state: State, slot: int, result: Session.FrameResult, visible_renderer: RefCounted = null) -> void:
	if ShioriActors.visible_scene(state,true):
		# dec_bg_draw(1) of stages 12-14 and of stage14's commit frame (the
		# stage14 body draws after mode_stage_next). Only the source owners are
		# checked here; actor_draw_all runs after this call in the same frame.
		if _board_store == null or _shiori_bytes.is_empty() or slot != 1:
			result.error = "stages 12-14 dec_bg_draw requires slot 1 and the board media"; return
		if visible_renderer == null or not visible_renderer.has_method("preflight_shiori"):
			result.error = "stages 12-14 have no prepared Shiori scene renderer"; return
		var scene: Variant = visible_renderer.call("preflight_shiori",state)
		if not scene is String or not scene.is_empty():
			result.error = scene if scene is String else "Shiori scene preflight returned an unknown result"; return
		state.opening.names_media["active_draw"] = "dec_bg_draw"
		result.presentation.append({"kind":"first_arrival_background_draw","slot":1})
		return
	if state.game_mode == 0x15 and state.mode_stage in [15, 16, 17]:
		# dec_bg_draw(1) at the end of each stage15-17 body. The 15/0 frame
		# that stage14 case6 switched into belongs to stage14's draw
		# (ShioriActors.visible_scene, handled above).
		if _store == null or _board_store == null or slot != 1 or state.mode_step != 0 or not state.opening.get("names_media") is Dictionary:
			result.error = "stages 15-17 dec_bg_draw requires slot 1 and the board media"
			return
		state.opening.names_media["active_draw"] = "dec_bg_draw"
		var stages_boundary := _visible_boundary_error(state,visible_renderer)
		if not stages_boundary.is_empty():
			result.error = stages_boundary
			return
		result.presentation.append({"kind":"first_arrival_background_draw","slot":1})
		return
	if state.game_mode == 0x15 and state.mode_stage in [18, 19, 20, 21]:
		# dec_bg_draw(1) of P_01_12: the source owner must be intact and the
		# shown buffer must be stage17's uploaded decode. At stage 21 only the
		# stage20 case2 exit frame reaches here (CLOSING_EXIT_UNSUPPORTED).
		var owner := _closing_owner_error(state) if _board_store != null and slot == 1 else "stages 18-20 dec_bg_draw requires slot 1 and the board media"
		if owner.is_empty() and state.mode_stage == 21: owner = _closing_exit_error(state,true)
		if owner.is_empty(): owner = _closing_upload_error(state,result)
		if not owner.is_empty():
			result.error = owner; return
		result.presentation.append({"kind":"first_arrival_background_draw","slot":1})
		return
	if state.game_mode == 0x15 and (state.mode_stage in [2, 3, 4, 5, 6, 7, 8, 9, 10, 11] or line17_exit_frame(state)) and state.mode_step == 0 and state.opening.get("names_media") is Dictionary:
		if state.mode_stage == 2 and state.substep in [0, 1, 2, 5, 6, 7, 8]:
			state.opening.names_media["active_draw"] = "dec_bg_draw"
		elif state.mode_stage in [3, 4, 5, 6, 7, 8, 9, 10, 11, 12]:
			state.opening.names_media["active_draw"] = "dec_bg_draw"
	var visible_boundary := _visible_boundary_error(state,visible_renderer)
	var flags: Variant = state.opening.get("actor96_draw_flags",0)
	if _store == null or slot != 1 or not flags is int or (state.mode_stage != 0 and not visible_boundary.is_empty()) or ((flags & 0x80) != 0 and not visible_boundary.is_empty()):
		result.error = "first-arrival provider needs a visible actor renderer before the stage transition"
		return
	result.presentation.append({"kind":"first_arrival_background_draw","slot":1})

func crossfade_draw(state: State, slot: int, result: Session.FrameResult) -> void:
	if _board_store == null or slot != 1 or state.game_mode != 0x15 or state.mode_stage != 2 or state.mode_step != 0 or state.substep not in [2, 3, 4]:
		result.error = "first-arrival crossfade draw is outside its source stage"
		return
	var names_media: Variant = state.opening.get("names_media")
	var fade: Variant = state.opening.get("bg_fade_amount")
	if not names_media is Dictionary or names_media.get("bg_show_1") != 0 or not names_media.get("buffer_0") is Dictionary or not names_media.get("buffer_1") is Dictionary or not fade is int or fade < 14 or fade > 128:
		result.error = "first-arrival crossfade draw lacks prepared dual buffers or fade amount"
		return
	names_media["active_draw"] = "bg_crossfade_draw"
	names_media["drawn_fade_amount"] = fade
	state.opening["names_media"] = names_media
	result.presentation.append({"kind":"first_arrival_names_crossfade_draw","slot":1,"fade_amount":fade})

static func line17_exit_frame(state: State) -> bool:
	# The prologue_line_17_run case6 frame: mode_stage_next has run, so the
	# frame draws stage12/substep0 state with the case6 O_13 actor. This frame
	# belongs to the Rei line17 path; stage12's own frames (substep 1 on) are
	# the Shiori scene (ShioriActors.visible_scene excludes this one).
	return state.game_mode == 0x15 and state.mode_stage == 12 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96") is Dictionary and state.opening.actor96.get("profile") == "o13_rei_line17_exit"

func _visible_boundary_error(state: State, renderer: RefCounted) -> String:
	if state.game_mode == 0x15 and state.mode_stage in [15, 16, 17]:
		if renderer == null or not renderer.has_method("preflight_visible"): return "visible stages 15-17 have no prepared renderer"
		var checked: Variant = renderer.call("preflight_visible",state)
		return checked if checked is String else "visible actor preflight returned an unknown result"
	if state.game_mode != 0x15 or (state.mode_stage not in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11] and not line17_exit_frame(state)) or not state.opening.get("actor96_draw_flags") is int:
		return "arrival background is outside its exact visible boundary"
	var flags: int = state.opening.actor96_draw_flags
	if state.mode_stage == 1:
		if flags == 0xa4:
			var arrival_boundary: bool = state.mode_step == 0 and state.substep == 0
			var board_read_pending: bool = state.mode_step == 0 and state.substep == 1 and _board_store != null and state.opening.get("D_800B3DB0") == 1 and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("read_requested") == true and state.opening.board_media.get("decoded") == false
			if not arrival_boundary and not board_read_pending:
				return "arrival background is outside its exact visible boundary"
		elif flags == 0:
			var counter: Variant = state.opening.get("prologue_scene_counter")
			var board_decoded: bool = state.mode_step == 1 and state.substep in [0, 1, 2, 3, 4, 5] and _board_store != null and not state.opening.has("actor96") and state.opening.get("D_800B3DB0") == 1 and counter is int and counter >= 0 and counter <= 320 and state.opening.get("prologue_petals") is Array and state.opening.prologue_petals.size() == 8 and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("decoded") == true and state.opening.get("prologue_hud_live",{}).get("date_visible") == false
			if not board_decoded:
				return "arrival background is outside its exact visible boundary"
		else:
			return "arrival background is outside its exact visible boundary"
	elif state.mode_stage == 2:
		var counter2: Variant = state.opening.get("prologue_scene_counter")
		var names_media: Variant = state.opening.get("names_media")
		var petals_ok: bool = state.opening.get("prologue_petals") is Array and state.opening.prologue_petals.size() == 8
		var stage2_ok: bool = flags == 0 and state.mode_step == 0 and state.substep in [0, 1, 2, 3, 4, 5, 6, 7, 8] and _board_store != null and not state.opening.has("actor96") and state.opening.get("D_800B3DB0") == 1 and counter2 is int and counter2 >= 0 and counter2 <= 320 and petals_ok and state.opening.get("board_media") is Dictionary and state.opening.board_media.get("decoded") == true and state.opening.get("prologue_hud_live",{}).get("date_visible") == false and names_media is Dictionary and names_media.get("bg_show_1") in [0, 1] and names_media.get("buffer_0") is Dictionary and names_media.get("active_draw") in ["board_scroll_transition","dec_bg_draw","bg_crossfade_draw"]
		if not stage2_ok:
			return "arrival background is outside its exact visible boundary"
		if names_media.get("active_draw") == "bg_crossfade_draw" and (not names_media.get("buffer_1") is Dictionary or not names_media.get("drawn_fade_amount") is int):
			return "arrival background crossfade lacks buffer_1 or drawn_fade_amount"
	else:
		var names_media3: Variant = state.opening.get("names_media")
		var layer0: Variant = state.opening.get("bg_layer_0")
		var stage2_to_3_transition: bool = state.mode_stage == 3 and state.mode_step == 0 and state.substep == 0 and not state.opening.has("bg_layer_0") and state.opening.get("prologue_petals") is Array and state.opening.prologue_petals.size() == 8 and flags == 0
		var stage35_active: bool = not state.opening.has("prologue_petals") and layer0 is Dictionary and (layer0.get("enabled") == 1 or (((state.mode_stage == 8 and state.substep == 7) or (state.mode_stage == 9 and state.substep in [0,1,2]) or line17_exit_frame(state)) and layer0.get("enabled") == 0))
		var stage35_ok: bool = _board_store != null and state.mode_step == 0 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("prologue_hud_live",{}).get("date_visible") == false and names_media3 is Dictionary and names_media3.get("bg_show_1") in [0, 1] and names_media3.get("active_draw") == "dec_bg_draw" and (stage2_to_3_transition or stage35_active)
		if not stage35_ok:
			return "arrival background is outside its exact visible boundary"
	if renderer == null or not renderer.has_method("preflight_visible"): return "visible arrival has no prepared actor renderer"
	var validation: Variant = renderer.call("preflight_visible",state)
	return validation if validation is String else "visible actor preflight returned an unknown result"

func complete(event: Session.ServiceEvent, state: State, rng, result: Session.FrameResult) -> void:
	queue.complete(event,state,rng,result)

func pending_events(tick: int) -> Array[Session.ServiceEvent]:
	var events: Array[Session.ServiceEvent] = []
	for observation: Dictionary in _pending:
		events.append(Session.ServiceEvent.new(tick,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,Queue.SERVICE,observation))
	return events

func _observations(result: Session.FrameResult) -> String:
	var consumed := 0
	for entry: Dictionary in result.trace:
		if not entry.get("kind") is int or entry.kind != Session.ServiceKind.COMPLETION: continue
		if not entry.get("service") is String or entry.service != Queue.SERVICE: continue
		if consumed >= _pending.size() or not entry.get("payload") is Dictionary or not Store._same(entry.payload,_pending[consumed]):
			return "first-arrival media observation differs from the actual queued producer"
		consumed += 1
	return ""

func preflight(result: Session.FrameResult, state: State) -> String:
	if _store == null: return "first-arrival media bridge is not prepared"
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if owner == null or owner.state != state: return "first-arrival media intent belongs to another or unbound logical Session"
	var error := _observations(result)
	if not error.is_empty(): return error
	var setup_count := 0
	var upload_count := 0
	var board_decode_count := 0
	var board_upload_count := 0
	var board_scroll_draw_count := 0
	var names_bg_upload_count := 0
	var names_crossfade_draw_count := 0
	var cut_upload_count := 0
	var o12_backup_count := 0
	var o12_restore_count := 0
	var o06_copy_count := 0
	var o07_copy_count := 0
	var o08_copy_count := 0
	var o11_copy_count := 0
	var o13_copy_count := 0
	var shiori_copy_count := 0
	var shiori_copy_id := ""
	var e8_copy_count := 0
	var stage_actor_upload_count := 0
	var reset_seen := false
	for intent: Dictionary in result.presentation:
		error = _intent_schema(intent)
		if not error.is_empty(): return error
		match intent.get("kind"):
			"opening_media_request":
				error = _request(intent,state)
				if not error.is_empty(): return error
			"opening_background_select":
				var allowed_bg: bool = intent.get("resource_id") == Store.BG_ID or (_board_store != null and intent.get("resource_id") in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID, BoardStore.BG_AD00_ID, BoardStore.BG_AD05_ID, BoardStore.BG_AD0B_ID, BoardStore.BG_496A_ID, BoardStore.BG_4973_ID])
				if intent.get("slot") != 1 or not allowed_bg:
					return "first-arrival background selection is outside its prepared source"
			"opening_arrival_actor_setup":
				setup_count += 1
				if intent.get("resource_id") != Store.ACTOR_ID or not _buffers.has(str(0x80180000)) or state.opening.get("actor96_draw_flags") != 0x24:
					return "first-arrival palette setup has no completed original actor read"
			"first_arrival_native_upload":
				var source: Variant = state.opening.get("arrival_media")
				if not source is Dictionary or not source.get("upload_batch_key") is String or intent.batch_key != source.upload_batch_key or intent.ordinal != upload_count or intent.request.id != _store.upload_requests()[upload_count].id:
					return "first-arrival upload differs from its source batch/operation order"
				upload_count += 1
			"opening_board_decode":
				board_decode_count += 1
				if _board_store == null or intent.get("resource_id") != BOARD_ID or not _buffers.has(str(0x80162000)) or _buffers[str(0x80162000)].sha256 != BoardStore.SOURCE_SHA or state.opening.get("actor96_draw_flags") != 0:
					return "first-arrival board decode has no completed original board read"
			"first_arrival_board_upload":
				var board_source: Variant = state.opening.get("board_media")
				if _board_store == null or not board_source is Dictionary or not board_source.get("upload_batch_key") is String or intent.batch_key != board_source.upload_batch_key:
					return "first-arrival board upload differs from its source batch"
				board_upload_count += 1
			"first_arrival_board_scroll_draw":
				board_scroll_draw_count += 1
				var scroll_board: Variant = state.opening.get("board_media")
				var valid_scroll_stage: bool = (state.mode_stage == 1 and state.mode_step == 1 and state.substep in [1,2,3,4,5]) or (state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("active_draw") == "board_scroll_transition")
				if _board_store == null or not valid_scroll_stage or not scroll_board is Dictionary or scroll_board.get("decoded") != true or intent.scroll_x != state.opening.get("prologue_scene_counter"):
					return "first-arrival board scroll draw is outside its decoded board state"
			"first_arrival_names_bg_upload":
				names_bg_upload_count += 1
				var names_src: Variant = state.opening.get("names_media")
				var buf_key: String = "buffer_0" if intent.get("buffer") == 0 else "buffer_1"
				var expected_tr_sha := ""
				match intent.get("resource_id"):
					BoardStore.BG_495D_ID: expected_tr_sha = BoardStore.BG_495D_TRANSFER_SHA
					BoardStore.BG_4966_ID: expected_tr_sha = BoardStore.BG_4966_TRANSFER_SHA
					BoardStore.BG_ACDD_ID: expected_tr_sha = BoardStore.BG_ACDD_TRANSFER_SHA
					BoardStore.BG_ACE6_ID: expected_tr_sha = BoardStore.BG_ACE6_TRANSFER_SHA
					BoardStore.BG_ACF6_ID: expected_tr_sha = BoardStore.BG_ACF6_TRANSFER_SHA
					BoardStore.BG_ACD4_ID: expected_tr_sha = BoardStore.BG_ACD4_TRANSFER_SHA
					BoardStore.BG_ACEC_ID: expected_tr_sha = BoardStore.BG_ACEC_TRANSFER_SHA
					BoardStore.BG_AD00_ID: expected_tr_sha = BoardStore.BG_AD00_TRANSFER_SHA
					BoardStore.BG_AD05_ID: expected_tr_sha = BoardStore.BG_AD05_TRANSFER_SHA
					BoardStore.BG_AD0B_ID: expected_tr_sha = BoardStore.BG_AD0B_TRANSFER_SHA
					BoardStore.BG_496A_ID: expected_tr_sha = BoardStore.BG_496A_TRANSFER_SHA
					BoardStore.BG_4973_ID: expected_tr_sha = BoardStore.BG_4973_TRANSFER_SHA
				if _board_store == null or expected_tr_sha.is_empty() or not names_src is Dictionary or not names_src.get(buf_key) is Dictionary or names_src[buf_key].get("upload_batch_key") != intent.batch_key or not _buffers.has(str(0x80162000)) or _buffers[str(0x80162000)].get("resource_id") != intent.resource_id or _buffers[str(0x80162000)].get("sha256") != expected_tr_sha:
					return "first-arrival names background upload lacks completed read or matching batch"
			"first_arrival_names_crossfade_draw":
				names_crossfade_draw_count += 1
				var cf_src: Variant = state.opening.get("names_media")
				if _board_store == null or state.mode_stage != 2 or state.mode_step != 0 or state.substep not in [2,3,4,5] or not cf_src is Dictionary or cf_src.get("active_draw") != "bg_crossfade_draw" or cf_src.get("drawn_fade_amount") != intent.fade_amount:
					return "first-arrival names crossfade draw is outside its prepared state"
			"first_arrival_cut_upload":
				cut_upload_count += 1
				var cut_src: Variant = state.opening.get("cut_media")
				if _board_store == null or not cut_src is Dictionary or cut_src.get("upload_batch_key") != intent.batch_key or not _buffers.has(str(0x801a4000)) or _buffers[str(0x801a4000)].get("resource_id") != intent.resource_id or _buffers[str(0x801a4000)].get("sha256") != intent.sha256:
					return "first-arrival cut upload lacks completed read or matching batch"
			"first_arrival_o12_backup":
				o12_backup_count += 1
				if _board_store == null or not _buffers.has(str(0x80180000)) or _buffers[str(0x80180000)].get("resource_id") != BoardStore.O_12_ID or _buffers[str(0x80180000)].get("sha256") != BoardStore.O_12_SHA:
					return "first-arrival O_12 backup lacks completed O_12 read at 0x80180000"
			"first_arrival_o12_restore":
				o12_restore_count += 1
				if _board_store == null or not _buffers.has(str(0x801f0000)) or _buffers[str(0x801f0000)].get("resource_id") != BoardStore.O_12_ID or _buffers[str(0x801f0000)].get("sha256") != BoardStore.O_12_SHA:
					return "first-arrival O_12 restore lacks backed-up O_12 payload at 0x801f0000"
			"first_arrival_o06_copy":
				o06_copy_count += 1
				if _board_store == null or not _buffers.has(str(0x801f0000)) or _buffers[str(0x801f0000)].get("resource_id") != BoardStore.O_06_ID or _buffers[str(0x801f0000)].get("sha256") != BoardStore.O_06_SHA:
					return "first-arrival O_06 copy lacks completed O_06 read at 0x801f0000"
			"first_arrival_o07_copy":
				o07_copy_count += 1
				if _board_store == null or not _buffers.has(str(0x801f0000)) or _buffers[str(0x801f0000)].get("resource_id") != BoardStore.O_07_ID or _buffers[str(0x801f0000)].get("sha256") != BoardStore.O_07_SHA:
					return "first-arrival O_07 copy lacks completed O_07 read at 0x801f0000"
			"first_arrival_o08_copy":
				o08_copy_count += 1
				if _board_store == null or not _buffers.has(str(0x801f0000)) or _buffers[str(0x801f0000)].get("resource_id") != BoardStore.O_08_ID or _buffers[str(0x801f0000)].get("sha256") != BoardStore.O_08_SHA:
					return "first-arrival O_08 copy lacks completed original read at 0x801f0000"
			"first_arrival_o11_copy":
				o11_copy_count += 1
				if _board_store == null or not _buffers.has(str(0x801e8000)) or _buffers[str(0x801e8000)].get("resource_id") != BoardStore.O_11_ID or _buffers[str(0x801e8000)].get("sha256") != BoardStore.O_11_SHA:
					return "first-arrival O_11 copy lacks completed original read at 0x801e8000"
			"first_arrival_o13_copy":
				o13_copy_count += 1
				if _board_store == null or not _buffers.has(str(0x801d8000)) or _buffers[str(0x801d8000)].get("resource_id") != BoardStore.O_13_ID or _buffers[str(0x801d8000)].get("sha256") != BoardStore.O_13_SHA:
					return "first-arrival O_13 copy lacks completed original read at 0x801d8000"
			"first_arrival_shiori_copy":
				shiori_copy_count += 1
				shiori_copy_id = str(intent.resource_id)
				var copy_source: Variant = _buffers.get(str(intent.source))
				if _board_store == null or not copy_source is Dictionary or copy_source.get("resource_id") != intent.resource_id or copy_source.get("sha256") != intent.sha256:
					return "first-arrival Shiori memcpy lacks its completed original read"
				# Each memcpy runs on its stage's commit frame, after
				# mode_stage_next: O_14 at 13/0, O_15 at 14/0, O_16 at 15/0.
				if state.game_mode != 0x15 or state.mode_step != 0 or state.substep != 0 or state.mode_stage != SHIORI_COPY_STAGES.get(intent.resource_id,-1):
					return SHIORI_COPY_FRAME_REFUSED
			"first_arrival_e8_copy":
				# memcpy(0x80180000, 0x801E8000, 0xC800) at stage15 case34
				# (O_07.NBN, board id O_06). Stage14 case6's O_16 copy is a
				# first_arrival_shiori_copy (SHIORI_COPIES).
				e8_copy_count += 1
				if _board_store == null or intent.resource_id != BoardStore.O_06_ID or state.game_mode != 0x15 or state.mode_stage != 16 or state.mode_step != 0 or state.substep != 0 or not _buffers.has(str(0x801e8000)) or _buffers[str(0x801e8000)].get("resource_id") != intent.resource_id or _buffers[str(0x801e8000)].get("sha256") != intent.sha256:
					return "first-arrival 0x801E8000 copy lacks its completed original read"
			"first_arrival_stage_actor_upload":
				stage_actor_upload_count += 1
				if SHIORI_COPIES.has(intent.resource_id) and ShioriActors.visible_scene(state):
					# Same frame as the memcpy: the source read; later: the copy.
					var shiori_page := completed_shiori_page(state,intent.resource_id,intent.source_offset,shiori_copy_id == intent.resource_id)
					if not shiori_page.error.is_empty(): return shiori_page.error
					continue
				var has_actor_buf: bool = (_buffers.has(str(0x80180000)) and _buffers[str(0x80180000)].get("resource_id") == intent.resource_id) or (o12_restore_count == 1 and intent.resource_id == BoardStore.O_12_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_12_ID) or (o06_copy_count == 1 and intent.resource_id == BoardStore.O_06_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_06_ID) or (o07_copy_count == 1 and intent.resource_id == BoardStore.O_07_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_07_ID)
				if o08_copy_count == 1 and intent.resource_id == BoardStore.O_08_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_08_ID:
					has_actor_buf = true
				if o11_copy_count == 1 and intent.resource_id == BoardStore.O_11_ID and _buffers.has(str(0x801e8000)) and _buffers[str(0x801e8000)].get("resource_id") == BoardStore.O_11_ID:
					has_actor_buf = true
				if o13_copy_count == 1 and intent.resource_id == BoardStore.O_13_ID and _buffers.has(str(0x801d8000)) and _buffers[str(0x801d8000)].get("resource_id") == BoardStore.O_13_ID:
					has_actor_buf = true
				if e8_copy_count == 1 and intent.resource_id == BoardStore.O_06_ID and _buffers.has(str(0x801e8000)) and _buffers[str(0x801e8000)].get("resource_id") == intent.resource_id:
					has_actor_buf = true
				if _board_store == null or not has_actor_buf:
					return "first-arrival stage actor upload lacks resident NBN buffer at 0x80180000"
			"opening_media_reset":
				reset_seen = true
				if intent.get("service") != Queue.SERVICE or intent.get("epoch") != state.opening.get("media",{}).get("epoch"):
					return "first-arrival reset differs from transactional ownership"
			"opening_cd_command_reset":
				if intent.get("service") != Queue.SERVICE or intent.get("epoch") != state.opening.get("media",{}).get("epoch") or state.game_mode != 0x15 or state.mode_stage != 21 or state.mode_step != 0:
					return "first-arrival cd_command_reset differs from prologue_finish_run ownership"
	if setup_count > 1 or (setup_count == 1 and (upload_count != 19 or reset_seen)) or (setup_count == 0 and upload_count != 0):
		return "first-arrival source setup requires one complete ordered native upload batch"
	if board_decode_count > 1 or board_decode_count != board_upload_count or (board_decode_count == 1 and (reset_seen or setup_count != 0)):
		return "first-arrival board decode requires one complete board upload batch"
	if names_bg_upload_count > 1 or (names_bg_upload_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0)):
		return "first-arrival names background decode requires one isolated upload batch"
	if cut_upload_count > 1 or (cut_upload_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0)):
		return "first-arrival cut upload requires one isolated upload batch"
	if stage_actor_upload_count > 3 or o12_backup_count > 1 or o12_restore_count > 1 or o06_copy_count > 1 or o07_copy_count > 1 or o08_copy_count > 1 or o11_copy_count > 1 or o13_copy_count > 1 or shiori_copy_count > 1 or e8_copy_count > 1 or o13_copy_count + shiori_copy_count + e8_copy_count > 1:
		return "first-arrival stage actor upload or backup/restore cannot repeat in one frame"
	if board_scroll_draw_count > 1 or (board_scroll_draw_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0 or names_crossfade_draw_count != 0)):
		return "first-arrival board scroll draw cannot combine with setup or reset"
	if names_crossfade_draw_count > 1 or (names_crossfade_draw_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0 or names_bg_upload_count != 0)):
		return "first-arrival names crossfade draw cannot combine with setup or decode"
	if not result.committed:
		# FrameResult metadata is callback-owned output, never external peer or
		# model mutation. Keep approval when later Core phases append their trace.
		result.set_meta(APPROVAL_META,{"session_id":owner.get_instance_id(),"tick":owner.clock.logic_iterations,"media_hash":_effect_hash(result)})
	return ""

static func _effect_hash(result: Session.FrameResult) -> String:
	var intents: Array[Dictionary] = []
	var observations: Array[Dictionary] = []
	for intent: Dictionary in result.presentation:
		if intent.get("kind") is String and intent.kind in OWN_INTENTS: intents.append(intent)
	for entry: Dictionary in result.trace:
		if entry.get("kind") is int and entry.kind == Session.ServiceKind.COMPLETION and entry.get("service") is String and entry.service == Queue.SERVICE: observations.append(entry)
	return JSON.stringify([intents,observations]).sha256_text()

func _request(intent: Dictionary, state: State) -> String:
	var request: Dictionary = intent.request
	if not request.get("request_id") is String or not state.opening.get("media") is Dictionary:
		return "first-arrival native transfer lacks transactional ownership"
	var owned: Variant = state.opening.media.requests.get(request.request_id)
	if not owned is Dictionary or owned != request or request.get("status") != "pending":
		return "first-arrival native transfer differs from its transactional request"
	return ""

func _intent_schema(intent: Dictionary) -> String:
	var kind: Variant = intent.get("kind")
	if not kind is String: return "first-arrival presentation kind requires a string"
	var fields: Array
	match kind:
		"opening_media_request": fields = ["kind","service","request"]
		"opening_background_select": fields = ["kind","slot","resource_id","buffer"]
		"opening_arrival_actor_setup": fields = ["kind","resource_id"]
		"opening_board_decode": fields = ["kind","resource_id"]
		"opening_media_reset": fields = ["kind","service","epoch"]
		"opening_cd_command_reset": fields = ["kind","service","epoch"]
		"opening_scene_reset": fields = ["kind"]
		"first_arrival_background_draw": fields = ["kind","slot"]
		"first_arrival_native_upload": fields = ["kind","batch_key","ordinal","request"]
		"first_arrival_board_upload": fields = ["kind","batch_key","decode_rect","move_call","word_sha256"]
		"first_arrival_board_scroll_draw": fields = ["kind","scroll_x","rect","tpage_args"]
		"first_arrival_names_bg_upload": fields = ["kind","batch_key","resource_id","buffer","decode_rect","word_sha256"]
		"first_arrival_names_crossfade_draw": fields = ["kind","slot","fade_amount"]
		"first_arrival_cut_upload": fields = ["kind","batch_key","resource_id","sha256"]
		"first_arrival_shiori_copy": fields = ["kind","resource_id","sha256","bytes","source"]
		"first_arrival_o12_backup": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o12_restore": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o06_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o07_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o08_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o11_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o13_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_e8_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_stage_actor_upload": fields = ["kind","batch_key","resource_id","slot","source_offset"]
		_:
			return "unsupported first-arrival media intent" if kind.begins_with("opening_media_") else ""
	if intent.size() != fields.size() or not intent.has_all(fields): return "first-arrival intent fields differ from the supported schema"
	if kind == "first_arrival_native_upload":
		if not Store._sha_string(intent.batch_key) or not intent.ordinal is int or intent.ordinal < 0 or intent.ordinal >= 19: return "first-arrival upload batch identity is invalid"
		return _store.validate_upload(intent.request)
	if kind == "first_arrival_board_upload":
		if _board_store == null or not Store._sha_string(intent.batch_key) or not Store._same(intent.decode_rect,[320,0,640,160]) or not Store._same(intent.move_call,[5,14,15]) or intent.word_sha256 != BoardStore.SOFTWARE_WORD_SHA:
			return "first-arrival board upload batch identity is invalid"
		return ""
	if kind == "first_arrival_board_scroll_draw":
		if _board_store == null or not intent.scroll_x is int or intent.scroll_x < 0 or intent.scroll_x > 320 or not Store._same(intent.rect,[-160,-120,320,160]) or not Store._same(intent.tpage_args,[5,2,0,0,0,0,13]):
			return "first-arrival board scroll draw intent is invalid"
		return ""
	if kind == "first_arrival_names_bg_upload":
		if _board_store == null or not Store._sha_string(intent.batch_key) or not intent.resource_id is String or not intent.buffer is int:
			return "first-arrival names background upload batch identity is invalid"
		var valid_495d: bool = intent.resource_id == BoardStore.BG_495D_ID and intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160]) and intent.word_sha256 == BoardStore.BG_495D_WORD_SHA
		var valid_4966: bool = intent.resource_id == BoardStore.BG_4966_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_4966_WORD_SHA
		var valid_acdd: bool = intent.resource_id == BoardStore.BG_ACDD_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_ACDD_WORD_SHA
		var valid_ace6: bool = intent.resource_id == BoardStore.BG_ACE6_ID and intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160]) and intent.word_sha256 == BoardStore.BG_ACE6_WORD_SHA
		var valid_acf6: bool = intent.resource_id == BoardStore.BG_ACF6_ID and ((intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160])) or (intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]))) and intent.word_sha256 == BoardStore.BG_ACF6_WORD_SHA
		var valid_acd4: bool = intent.resource_id == BoardStore.BG_ACD4_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_ACD4_WORD_SHA
		var valid_acec: bool = intent.resource_id == BoardStore.BG_ACEC_ID and intent.buffer in [0,1] and Store._same(intent.decode_rect,BoardStore.SLOT1_BUFFER_RECTS[intent.buffer]) and intent.word_sha256 == BoardStore.BG_ACEC_WORD_SHA
		var valid_ad00: bool = intent.resource_id == BoardStore.BG_AD00_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_AD00_WORD_SHA
		# Stage12 decodes AD0B and stage14 ACEC into buffer 1 (prologue.c:1860, :2103).
		# Stage11 decodes AD05 into buffer 0 (prologue.c:1797).
		var valid_ad05: bool = intent.resource_id == BoardStore.BG_AD05_ID and intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160]) and intent.word_sha256 == BoardStore.BG_AD05_WORD_SHA
		var valid_ad0b: bool = intent.resource_id == BoardStore.BG_AD0B_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_AD0B_WORD_SHA
		valid_acec = valid_acec or (intent.resource_id == BoardStore.BG_ACEC_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_ACEC_WORD_SHA)
		# Stages 15-17 decode into whichever buffer dec_bg_show[1] leaves hidden.
		var stages_word: String = {BoardStore.BG_ACD4_ID:BoardStore.BG_ACD4_WORD_SHA,BoardStore.BG_496A_ID:BoardStore.BG_496A_WORD_SHA,BoardStore.BG_4973_ID:BoardStore.BG_4973_WORD_SHA}.get(intent.resource_id,"")
		var valid_15_17: bool = not stages_word.is_empty() and intent.buffer in [0,1] and Store._same(intent.decode_rect,BoardStore.SLOT1_BUFFER_RECTS[intent.buffer]) and intent.word_sha256 == stages_word
		if not valid_495d and not valid_4966 and not valid_acdd and not valid_ace6 and not valid_acf6 and not valid_acd4 and not valid_acec and not valid_ad00 and not valid_ad05 and not valid_ad0b and not valid_15_17:
			return "first-arrival names background upload descriptor is invalid"
		return ""
	if kind == "first_arrival_cut_upload":
		var valid_cut12: bool = intent.resource_id == BoardStore.CUT_12_ID and intent.sha256 == BoardStore.CUT_12_SHA
		var valid_cut05: bool = intent.resource_id == BoardStore.CUT_05_ID and intent.sha256 == BoardStore.CUT_05_SHA
		var valid_cut06: bool = intent.resource_id == BoardStore.CUT_06_ID and intent.sha256 == BoardStore.CUT_06_SHA
		var valid_cut07: bool = intent.resource_id == BoardStore.CUT_07_ID and intent.sha256 == BoardStore.CUT_07_SHA
		var valid_cut09: bool = intent.resource_id == BoardStore.CUT_09_ID and intent.sha256 == BoardStore.CUT_09_SHA
		var valid_cut08: bool = intent.resource_id == BoardStore.CUT_08_ID and intent.sha256 == BoardStore.CUT_08_SHA
		var valid_shiori_cut: bool = (intent.resource_id == BoardStore.CUT_14_ID and intent.sha256 == BoardStore.CUT_14_SHA) or (intent.resource_id == BoardStore.CUT_16_ID and intent.sha256 == BoardStore.CUT_16_SHA)
		if _board_store == null or not Store._sha_string(intent.batch_key) or (not valid_cut12 and not valid_cut05 and not valid_cut06 and not valid_cut07 and not valid_cut08 and not valid_cut09 and not valid_shiori_cut):
			return "first-arrival cut upload descriptor is invalid"
		return ""
	if kind == "first_arrival_shiori_copy":
		var copy: Variant = SHIORI_COPIES.get(intent.resource_id)
		if _board_store == null or _shiori_bytes.is_empty() or not copy is Array or intent.sha256 != _shiori_bytes[intent.resource_id].sha256 or intent.bytes != copy[1] or intent.source != copy[0]:
			return "first-arrival Shiori memcpy descriptor is invalid"
		return ""
	if kind in ["first_arrival_o12_backup", "first_arrival_o12_restore"]:
		if _board_store == null or intent.resource_id != BoardStore.O_12_ID or intent.sha256 != BoardStore.O_12_SHA or intent.bytes != 34816:
			return "first-arrival O_12 backup/restore descriptor is invalid"
		return ""
	if kind == "first_arrival_o06_copy":
		if _board_store == null or intent.resource_id != BoardStore.O_06_ID or intent.sha256 != BoardStore.O_06_SHA or intent.bytes != 51200:
			return "first-arrival O_06 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_o07_copy":
		if _board_store == null or intent.resource_id != BoardStore.O_07_ID or intent.sha256 != BoardStore.O_07_SHA or intent.bytes != 51200:
			return "first-arrival O_07 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_o08_copy":
		if _board_store == null or intent.resource_id != BoardStore.O_08_ID or intent.sha256 != BoardStore.O_08_SHA or intent.bytes != 18432:
			return "first-arrival O_08 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_o13_copy":
		if _board_store == null or intent.resource_id != BoardStore.O_13_ID or intent.sha256 != BoardStore.O_13_SHA or intent.bytes != 0x20800:
			return "first-arrival O_13 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_o11_copy":
		if _board_store == null or intent.resource_id != BoardStore.O_11_ID or intent.sha256 != BoardStore.O_11_SHA or intent.bytes != 0x10800:
			return "first-arrival O_11 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_e8_copy":
		# Only stage15 case34's O_07.NBN copy; stage14's O_16 copy is a
		# first_arrival_shiori_copy.
		var valid_e8: bool = intent.resource_id == BoardStore.O_06_ID and intent.sha256 == BoardStore.O_06_SHA
		if _board_store == null or not valid_e8 or not intent.bytes is int or intent.bytes != 0xc800:
			return "first-arrival 0x801E8000 copy descriptor is invalid"
		return ""
	if kind == "first_arrival_stage_actor_upload":
		var valid_o12_96: bool = intent.resource_id == BoardStore.O_12_ID and intent.slot == 5 and intent.source_offset == 0x0120
		var valid_o12_97: bool = intent.resource_id == BoardStore.O_12_ID and intent.slot == 6 and intent.source_offset == 0x41a0
		var valid_o05_96: bool = intent.resource_id == BoardStore.O_05_ID and intent.slot == 5 and intent.source_offset == 0x00a0
		var valid_o06_96_s5: bool = intent.resource_id == BoardStore.O_06_ID and intent.slot == 5 and intent.source_offset == 0x4310
		var valid_o06_96_s6: bool = intent.resource_id == BoardStore.O_06_ID and intent.slot == 6 and intent.source_offset == 0x8310
		var valid_o06_99_s7: bool = intent.resource_id == BoardStore.O_06_ID and intent.slot == 7 and intent.source_offset == 0x0130
		var valid_o07_96_s5: bool = intent.resource_id == BoardStore.O_07_ID and intent.slot == 5 and intent.source_offset == 0x0190
		var valid_o07_96_s6: bool = intent.resource_id == BoardStore.O_07_ID and intent.slot == 6 and intent.source_offset == 0x4190
		var valid_o07_96_s7: bool = intent.resource_id == BoardStore.O_07_ID and intent.slot == 7 and intent.source_offset == 0x8190
		var valid_o09_96: bool = intent.resource_id == BoardStore.O_09_ID and BoardStore.O10_ENTRANCE_TPAGE_SLOTS.get(intent.source_offset) == intent.slot
		var valid_o08_96: bool = intent.resource_id == BoardStore.O_08_ID and intent.slot == 5 and intent.source_offset == 0xa0
		var valid_o11_96: bool = intent.resource_id == BoardStore.O_11_ID and intent.source_offset in [0x0190,0x4190,0x8190,0xc190] and intent.slot in BoardStore.STAGE10_FREE_TPAGE_SLOTS
		# Stages 12-14: a page of the resource's own tpage table, in a free slot.
		var valid_shiori: bool = intent.resource_id is String and intent.resource_id != BoardStore.O_13_ID and SHIORI_COPIES.has(intent.resource_id) and _shiori_bytes.has(intent.resource_id) and intent.source_offset in BoardStore.shiori_page_offsets(intent.resource_id,_shiori_bytes[intent.resource_id].bytes) and intent.slot in BoardStore.STAGE10_FREE_TPAGE_SLOTS
		# Stage15 O_16 pages (tpage table 0x24C) in the stage10 free slots.
		var valid_o16: bool = intent.resource_id == BoardStore.O_16_ID and intent.source_offset in [0x02c0,0x42c0,0x82c0] and intent.slot in BoardStore.STAGE10_FREE_TPAGE_SLOTS
		# O_13 actor96 (stage11 case6 exit frame, stage12, stage19): two pages in slots 5/6.
		var valid_o13_96: bool = intent.resource_id == BoardStore.O_13_ID and intent.source_offset in BoardStore.O13_PAGE_OFFSETS and intent.slot in [5, 6]
		if _board_store == null or not Store._sha_string(intent.batch_key) or (not valid_shiori and not valid_o11_96 and not valid_o16 and not valid_o13_96 and not valid_o12_96 and not valid_o12_97 and not valid_o05_96 and not valid_o06_96_s5 and not valid_o06_96_s6 and not valid_o06_99_s7 and not valid_o07_96_s5 and not valid_o07_96_s6 and not valid_o07_96_s7 and not valid_o08_96 and not valid_o09_96):
			return "first-arrival stage actor upload descriptor is invalid"
		return ""
	if kind == "first_arrival_names_crossfade_draw":
		if _board_store == null or not intent.slot is int or intent.slot != 1 or not intent.fade_amount is int or intent.fade_amount < 14 or intent.fade_amount > 128:
			return "first-arrival names crossfade draw intent is invalid"
		return ""
	if intent.has("service") and (not intent.service is String or intent.service != Queue.SERVICE): return "first-arrival media intent belongs to another service"
	if intent.has("slot") and (not intent.slot is int or intent.slot != 1): return "first-arrival background slot differs from source"
	if intent.has("buffer") and (not intent.buffer is int or intent.buffer not in [0,1]): return "first-arrival background buffer is invalid"
	if intent.has("epoch") and (not intent.epoch is int or intent.epoch < 0 or intent.epoch > 0xffffffff): return "first-arrival reset epoch is invalid"
	if intent.has("resource_id"):
		if not intent.resource_id is String: return "first-arrival intent selects another prepared resource"
		if kind == "opening_arrival_actor_setup":
			if intent.resource_id != Store.ACTOR_ID: return "first-arrival intent selects another prepared resource"
		elif kind == "opening_board_decode":
			if intent.resource_id != BOARD_ID: return "first-arrival intent selects another prepared resource"
		elif kind == "opening_background_select":
			var valid_bg_sel: bool = intent.resource_id == Store.BG_ID or (_board_store != null and ((intent.resource_id in [BoardStore.BG_495D_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACEC_ID] and intent.buffer == 0) or (intent.resource_id in [BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_AD00_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_AD0B_ID, BoardStore.BG_ACEC_ID] and intent.buffer == 1) or (intent.resource_id == BoardStore.BG_AD05_ID and intent.buffer == 0) or intent.resource_id in [BoardStore.BG_ACD4_ID, BoardStore.BG_496A_ID, BoardStore.BG_4973_ID]))
			if not valid_bg_sel: return "first-arrival intent selects another prepared resource"
		elif intent.resource_id != Store.BG_ID:
			return "first-arrival intent selects another prepared resource"
	if kind != "opening_media_request": return ""
	var request: Variant = intent.request
	var request_fields: Array[String] = ["epoch","request_id","kind","payload","resource_id","resource_sha256","status","request_key"]
	if not request is Dictionary or request.size() != 8 or not request.has_all(request_fields): return "first-arrival native request fields are malformed"
	if not request.epoch is int or request.epoch < 0 or request.epoch > 0xffffffff: return "first-arrival request epoch is invalid"
	for key: String in ["request_id","kind","resource_id","resource_sha256","status","request_key"]:
		if not request[key] is String: return "first-arrival request identity requires exact strings"
	if request.request_id.is_empty() or request.request_id.length() > 16 or not request.request_id.is_valid_int() or request.request_id.begins_with("-"): return "first-arrival request identifier is invalid"
	if request.status != "pending" or not Store._sha_string(request.request_key): return "first-arrival request status/key is invalid"
	if not request.payload is Dictionary: return "first-arrival request arguments are malformed"
	var args: Dictionary = request.payload
	if request.kind == "background":
		if args.size() != 3 or not args.has_all(["lba","slot","retry"]) or not args.lba is int or not args.slot is int or args.slot != 1 or not args.retry is bool:
			return "first-arrival background arguments differ from source"
		var bg_494b: bool = request.resource_id == Store.BG_ID and request.resource_sha256 == Store.BG_TRANSFER_SHA and args.lba == 0x494b
		var bg_495d: bool = _board_store != null and request.resource_id == BoardStore.BG_495D_ID and request.resource_sha256 == BoardStore.BG_495D_TRANSFER_SHA and args.lba == 0x495d
		var bg_4966: bool = _board_store != null and request.resource_id == BoardStore.BG_4966_ID and request.resource_sha256 == BoardStore.BG_4966_TRANSFER_SHA and args.lba == 0x4966
		var bg_acdd: bool = _board_store != null and request.resource_id == BoardStore.BG_ACDD_ID and request.resource_sha256 == BoardStore.BG_ACDD_TRANSFER_SHA and args.lba == 0xacdd
		var bg_ace6: bool = _board_store != null and request.resource_id == BoardStore.BG_ACE6_ID and request.resource_sha256 == BoardStore.BG_ACE6_TRANSFER_SHA and args.lba == 0xace6
		var bg_acf6: bool = _board_store != null and request.resource_id == BoardStore.BG_ACF6_ID and request.resource_sha256 == BoardStore.BG_ACF6_TRANSFER_SHA and args.lba == 0xacf6
		var bg_acd4: bool = _board_store != null and request.resource_id == BoardStore.BG_ACD4_ID and request.resource_sha256 == BoardStore.BG_ACD4_TRANSFER_SHA and args.lba == 0xacd4
		var bg_acec: bool = _board_store != null and request.resource_id == BoardStore.BG_ACEC_ID and request.resource_sha256 == BoardStore.BG_ACEC_TRANSFER_SHA and args.lba == 0xacec
		var bg_ad00: bool = _board_store != null and request.resource_id == BoardStore.BG_AD00_ID and request.resource_sha256 == BoardStore.BG_AD00_TRANSFER_SHA and args.lba == 0xad00
		var bg_ad05: bool = _board_store != null and request.resource_id == BoardStore.BG_AD05_ID and request.resource_sha256 == BoardStore.BG_AD05_TRANSFER_SHA and args.lba == 0xad05
		var bg_ad0b: bool = _board_store != null and request.resource_id == BoardStore.BG_AD0B_ID and request.resource_sha256 == BoardStore.BG_AD0B_TRANSFER_SHA and args.lba == 0xad0b
		var bg_16_17: bool = _board_store != null and ((request.resource_id == BoardStore.BG_496A_ID and request.resource_sha256 == BoardStore.BG_496A_TRANSFER_SHA and args.lba == 0x496a) or (request.resource_id == BoardStore.BG_4973_ID and request.resource_sha256 == BoardStore.BG_4973_TRANSFER_SHA and args.lba == 0x4973))
		if not bg_16_17 and not bg_494b and not bg_495d and not bg_4966 and not bg_acdd and not bg_ace6 and not bg_acf6 and not bg_acd4 and not bg_acec and not bg_ad00 and not bg_ad05 and not bg_ad0b:
			return "first-arrival background arguments differ from source"
	elif request.kind == "actors":
		if args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival actor arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival actor arguments require exact integers"
		var act_abab: bool = request.resource_id == Store.ACTOR_ID and request.resource_sha256 == Store.ACTOR_SHA and args.lba == 0xabab and args.sectors == 13 and args.destination == 0x80180000
		var act_ab18: bool = _board_store != null and request.resource_id == BoardStore.CUT_12_ID and request.resource_sha256 == BoardStore.CUT_12_SHA and args.lba == 0xab18 and args.sectors == 49 and args.destination == 0x801a4000
		var act_ac3f: bool = _board_store != null and request.resource_id == BoardStore.O_12_ID and request.resource_sha256 == BoardStore.O_12_SHA and args.lba == 0xac3f and args.sectors == 17 and args.destination in [0x80180000,0x801f0000]
		var act_aa23: bool = _board_store != null and request.resource_id == BoardStore.CUT_05_ID and request.resource_sha256 == BoardStore.CUT_05_SHA and args.lba == 0xaa23 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abb8: bool = _board_store != null and request.resource_id == BoardStore.O_05_ID and request.resource_sha256 == BoardStore.O_05_SHA and args.lba == 0xabb8 and args.sectors == 9 and args.destination == 0x80180000
		var act_aa54: bool = _board_store != null and request.resource_id == BoardStore.CUT_06_ID and request.resource_sha256 == BoardStore.CUT_06_SHA and args.lba == 0xaa54 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abca: bool = _board_store != null and request.resource_id == BoardStore.O_06_ID and request.resource_sha256 == BoardStore.O_06_SHA and args.lba == 0xabca and args.sectors == 25 and args.destination == 0x801f0000
		var act_aa85: bool = _board_store != null and request.resource_id == BoardStore.CUT_07_ID and request.resource_sha256 == BoardStore.CUT_07_SHA and args.lba == 0xaa85 and args.sectors == 49 and args.destination == 0x801a4000
		var act_aab6: bool = _board_store != null and request.resource_id == BoardStore.CUT_08_ID and request.resource_sha256 == BoardStore.CUT_08_SHA and args.lba == 0xaab6 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abe3: bool = _board_store != null and request.resource_id == BoardStore.O_07_ID and request.resource_sha256 == BoardStore.O_07_SHA and args.lba == 0xabe3 and args.sectors == 25 and args.destination == 0x801f0000
		var act_aae7: bool = _board_store != null and request.resource_id == BoardStore.CUT_09_ID and request.resource_sha256 == BoardStore.CUT_09_SHA and args.lba == 0xaae7 and args.sectors == 49 and args.destination == 0x801a4000
		var act_ac05: bool = _board_store != null and request.resource_id == BoardStore.O_09_ID and request.resource_sha256 == BoardStore.O_09_SHA and args.lba == 0xac05 and args.sectors == 25 and args.destination == 0x80180000
		var act_ac1e: bool = _board_store != null and request.resource_id == BoardStore.O_11_ID and request.resource_sha256 == BoardStore.O_11_SHA and args.lba == 0xac1e and args.sectors == 33 and args.destination == 0x801e8000
		var act_abfc: bool = _board_store != null and request.resource_id == BoardStore.O_08_ID and request.resource_sha256 == BoardStore.O_08_SHA and args.lba == 0xabfc and args.sectors == 9 and args.destination == 0x801f0000
		var act_ac50: bool = _board_store != null and request.resource_id == BoardStore.O_13_ID and request.resource_sha256 == BoardStore.O_13_SHA and args.lba == 0xac50 and args.sectors == 65 and args.destination == 0x801d8000
		# Stages 12-14 checked reads (prologue.c:1838-2096), one extent each.
		var act_shiori := false
		for read: Array in BoardStore.STAGES_11_17_READS.slice(1,6):
			if _board_store != null and request.resource_id == read[0] and request.resource_sha256 == read[1] and args.sectors == read[2] and args.destination == read[3] and args.lba == read[4]: act_shiori = true
		# Stage14 case4/5 (CUT_16, O_16), stage15 case33 (O_07.NBN to
		# 0x801E8000) and stage17 case3 (O_13 to 0x80180000).
		var act_15_17: bool = _board_store != null and ((request.resource_id == BoardStore.CUT_16_ID and request.resource_sha256 == BoardStore.CUT_16_SHA and args.lba == 0xab7a and args.sectors == 49 and args.destination == 0x801a4000)
			or (request.resource_id == BoardStore.O_16_ID and request.resource_sha256 == BoardStore.O_16_SHA and args.lba == 0xacbb and args.sectors == 25 and args.destination == 0x801e8000)
			or (request.resource_id == BoardStore.O_06_ID and request.resource_sha256 == BoardStore.O_06_SHA and args.lba == 0xabca and args.sectors == 25 and args.destination == 0x801e8000)
			or (request.resource_id == BoardStore.O_13_ID and request.resource_sha256 == BoardStore.O_13_SHA and args.lba == 0xac50 and args.sectors == 65 and args.destination == 0x80180000))
		if not act_15_17 and not act_shiori and not act_abab and not act_ab18 and not act_ac3f and not act_aa23 and not act_abb8 and not act_aa54 and not act_abca and not act_aa85 and not act_abe3 and not act_aab6 and not act_abfc and not act_aae7 and not act_ac05 and not act_ac1e and not act_ac50:
			return "first-arrival actor read extent differs from source"
	elif request.kind == "bank":
		if _finish_bank == null or args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival bank read arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival bank read arguments require exact integers"
		var described: Variant = _finish_bank.descriptions().get(FinishBank.descriptor_key(args.sectors,args.destination,args.lba))
		if not described is Dictionary or described.id != request.resource_id or described.sha256 != request.resource_sha256: return "first-arrival bank read extent differs from source"
	elif request.kind == "board":
		if _board_store == null or request.resource_id != BOARD_ID or request.resource_sha256 != BoardStore.SOURCE_SHA or args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival board arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival board arguments require exact integers"
		if args.lba != 0x4954 or args.sectors != 9 or args.destination != 0x80162000: return "first-arrival board read extent differs from source"
	elif request.kind in ["hud","room"]:
		# Mode 0x34 reload: the CALROM read and the slot-0 room read.
		if _command_reload == null: return "command reload read has no authenticated reader"
		var wanted: Array = ["lba","sectors","destination"] if request.kind == "hud" else ["lba","sectors","destination","slot"]
		if args.size() != wanted.size() or not args.has_all(wanted): return "command reload read arguments differ from source"
		for key: String in wanted:
			if not args[key] is int: return "command reload read arguments require exact integers"
		var reload_desc := _command_reload.description("hud_calendar" if request.kind == "hud" else "room_spring")
		if reload_desc.is_empty() or reload_desc.id != request.resource_id or reload_desc.sha256 != request.resource_sha256 or args.lba != reload_desc.lba or args.sectors != reload_desc.sectors or args.destination != reload_desc.destination or (request.kind == "room" and args.slot != 0):
			return "command reload read extent differs from source"
	else: return "unsupported first-arrival native request kind"
	if JSON.stringify([request.epoch,request.request_id,request.kind,args,request.resource_id,request.resource_sha256]).sha256_text() != request.request_key: return "first-arrival request key differs from its authenticated identity"
	return ""

static func _has_publish_work(result: Session.FrameResult) -> bool:
	for intent: Dictionary in result.presentation:
		var k: Variant = intent.get("kind")
		if k is String and k not in ["first_arrival_background_draw", "first_arrival_board_scroll_draw", "first_arrival_names_crossfade_draw"]:
			return true
	for entry: Dictionary in result.trace:
		if entry.get("kind") is int and entry.kind == Session.ServiceKind.COMPLETION and entry.get("service") is String and entry.service == Queue.SERVICE:
			return true
	return false

func publish(result: Session.FrameResult, tick: int) -> String:
	if not result.committed: return "aborted frames cannot publish native media transfers"
	if _store == null: return "first-arrival media bridge is not prepared"
	if tick < 0 or tick <= _last_published_tick: return "first-arrival media frame was already published or is out of order"
	var owner: Session = _session.get_ref() as Session if _session != null else null
	var has_work := _has_publish_work(result)
	if owner == null or tick != owner.clock.logic_iterations-1 or result.state_hash.is_empty() or (has_work and result.state_hash != owner.state_hash()): return "first-arrival publication is not the current committed Session frame"
	var approval: Variant = result.get_meta(APPROVAL_META,null)
	if not approval is Dictionary or approval.size() != 3 or not approval.has_all(["session_id","tick","media_hash"]) or not approval.session_id is int or approval.session_id != owner.get_instance_id() or not approval.tick is int or approval.tick != tick or not approval.media_hash is String or approval.media_hash != _effect_hash(result):
		return "first-arrival media effects changed or lost their frame preflight approval"
	var error := preflight(result,owner.state as State)
	if not error.is_empty(): return error
	var consumed := 0
	for entry: Dictionary in result.trace:
		if entry.get("kind") is int and entry.kind == Session.ServiceKind.COMPLETION and entry.get("service") is String and entry.service == Queue.SERVICE: consumed += 1
	var next_pending: Array[Dictionary] = _pending.slice(consumed)
	var next_buffers: Dictionary = _buffers.duplicate()
	var next_upload: Dictionary = _actor_upload.duplicate()
	var next_board_upload: Dictionary = _board_upload.duplicate()
	var next_names_uploads: Dictionary = _names_bg_uploads.duplicate()
	var next_cut_uploads: Dictionary = _cut_uploads.duplicate()
	var next_stage_actor_uploads: Dictionary = _stage_actor_uploads.duplicate()
	var setup_seen := false
	var board_decode_seen := false
	var names_bg_upload_seen := false
	for intent: Dictionary in result.presentation:
		error = _intent_schema(intent)
		if not error.is_empty(): return error
		match intent.get("kind"):
			"opening_media_reset":
				next_pending.clear()
				next_buffers.clear()
				next_upload.clear()
				next_board_upload.clear()
				next_names_uploads.clear()
				next_cut_uploads.clear()
				next_stage_actor_uploads.clear()
			"opening_cd_command_reset":
				# Queued completions are forgotten; RAM buffers and VRAM uploads stay.
				next_pending.clear()
			"opening_media_request":
				var request: Dictionary = intent.request
				if request.kind == "bank":
					var bank_transfer := _finish_bank.transfer(request.resource_id)
					if not str(bank_transfer.get("error","")).is_empty(): return str(bank_transfer.error)
					next_buffers[str(bank_transfer.destination)] = {"bytes":bank_transfer.bytes,"sha256":bank_transfer.sha256,"resource_id":request.resource_id}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind in ["hud","room"]:
					# Mode 0x34 reload: CALROM to 0x801A0000, the room to 0x80162000.
					if _command_reload == null: return "command reload read has no authenticated reader"
					var reload_transfer := _command_reload.transfer(request.resource_id)
					if not str(reload_transfer.get("error","")).is_empty(): return str(reload_transfer.error)
					if reload_transfer.destination != request.payload.destination or reload_transfer.lba != request.payload.lba or reload_transfer.sectors != request.payload.sectors:
						return "command reload read differs from its queued request"
					next_buffers[str(reload_transfer.destination)] = {"bytes":reload_transfer.bytes,"sha256":reload_transfer.sha256,"resource_id":request.resource_id,"primary_sha256":reload_transfer.primary_sha256}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind == "board":
					var desc := _board_store.native_read_descriptor()
					if not str(desc.get("error","")).is_empty(): return str(desc.error)
					next_buffers[str(desc.destination)] = {"bytes":desc.payload,"sha256":BoardStore.SOURCE_SHA,"resource_id":BOARD_ID}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind == "background" and request.resource_id in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID, BoardStore.BG_AD00_ID, BoardStore.BG_AD05_ID, BoardStore.BG_AD0B_ID, BoardStore.BG_496A_ID, BoardStore.BG_4973_ID]:
					var bg_transfer := _board_store.background_transfer(request.resource_id)
					if not str(bg_transfer.get("error","")).is_empty(): return str(bg_transfer.error)
					next_buffers[str(bg_transfer.destination)] = {"bytes":bg_transfer.bytes,"sha256":bg_transfer.sha256,"resource_id":request.resource_id}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind == "actors" and request.resource_id in [BoardStore.CUT_12_ID, BoardStore.O_12_ID, BoardStore.CUT_05_ID, BoardStore.O_05_ID, BoardStore.CUT_06_ID, BoardStore.O_06_ID, BoardStore.CUT_07_ID, BoardStore.O_07_ID, BoardStore.CUT_08_ID, BoardStore.O_08_ID, BoardStore.CUT_09_ID, BoardStore.O_09_ID, BoardStore.O_11_ID, BoardStore.CUT_14_ID, BoardStore.O_14_ID, BoardStore.O_15_ID, BoardStore.CUT_16_ID, BoardStore.O_16_ID, BoardStore.O_13_ID]:
					var st_transfer := _board_store.stage_transfer(request.resource_id)
					if not str(st_transfer.get("error","")).is_empty(): return str(st_transfer.error)
					# Schema-pinned request destination; O12 is also read to 0x801f0000.
					next_buffers[str(request.payload.destination)] = {"bytes":st_transfer.bytes,"sha256":st_transfer.sha256,"resource_id":request.resource_id}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				else:
					var transfer := _store.transfer(request.resource_id)
					if not str(transfer.get("error","")).is_empty(): return str(transfer.error)
					# These are actual authenticated native-size bytes, not a ready flag.
					next_buffers[str(transfer.destination)] = {"bytes":transfer.bytes,"sha256":transfer.sha256,"resource_id":request.resource_id}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
			"opening_arrival_actor_setup":
				if setup_seen: return "first-arrival actor setup cannot publish twice in one frame"
				setup_seen = true
				if not next_buffers.has(str(0x80180000)) or next_buffers[str(0x80180000)].sha256 != Store.ACTOR_SHA: return "first-arrival source texture setup has no completed actor buffer"
				next_upload = {"source_sha256":Store.ACTOR_SHA,"operation_ids":[],"vram_ready":false,"visible_composition_supported":false}
			"first_arrival_native_upload":
				if next_upload.is_empty(): return "first-arrival native upload has no source setup batch"
				if intent.ordinal != next_upload.operation_ids.size() or intent.request.id != _store.upload_requests()[intent.ordinal].id: return "first-arrival native uploads are missing, repeated or out of source order"
				if not next_upload.has("batch_key"): next_upload["batch_key"] = intent.batch_key
				if next_upload.batch_key != intent.batch_key: return "first-arrival native uploads belong to different batches"
				next_upload.operation_ids.append(intent.request.id)
			"opening_board_decode":
				if board_decode_seen: return "first-arrival board decode cannot publish twice in one frame"
				board_decode_seen = true
				if not next_buffers.has(str(0x80162000)) or next_buffers[str(0x80162000)].sha256 != BoardStore.SOURCE_SHA:
					return "first-arrival board decode has no completed board buffer"
				next_board_upload = {"source_sha256":BoardStore.SOURCE_SHA,"word_sha256":BoardStore.SOFTWARE_WORD_SHA,"decode_rect":[320,0,640,160],"move_call":[5,14,15]}
			"first_arrival_board_upload":
				if not board_decode_seen or next_board_upload.is_empty() or next_board_upload.has("batch_key"):
					return "first-arrival board upload has no matching board decode"
				next_board_upload["batch_key"] = intent.batch_key
			"first_arrival_names_bg_upload":
				if names_bg_upload_seen or next_names_uploads.has(intent.batch_key):
					return "first-arrival names background upload cannot publish twice"
				names_bg_upload_seen = true
				next_names_uploads[intent.batch_key] = {"batch_key":intent.batch_key,"resource_id":intent.resource_id,"buffer":intent.buffer,"decode_rect":intent.decode_rect.duplicate(),"word_sha256":intent.word_sha256}
			"first_arrival_cut_upload":
				next_cut_uploads[intent.batch_key] = {"batch_key":intent.batch_key,"resource_id":intent.resource_id,"sha256":intent.sha256}
			"first_arrival_o12_backup":
				next_buffers[str(0x801f0000)] = next_buffers[str(0x80180000)].duplicate(true)
			"first_arrival_o12_restore", "first_arrival_o06_copy", "first_arrival_o07_copy", "first_arrival_o08_copy":
				next_buffers[str(0x80180000)] = next_buffers[str(0x801f0000)].duplicate(true)
			"first_arrival_o11_copy", "first_arrival_e8_copy":
				next_buffers[str(0x80180000)] = next_buffers[str(0x801e8000)].duplicate(true)
			"first_arrival_o13_copy":
				# prologue_line_17_run case6 memcpy(0x80180000, 0x801D8000, 0x20800): the
				# whole O_13 extent, also the stage12 actor area base layer (SHIORI_COPIES).
				var o13_area: Dictionary = next_buffers[str(0x801d8000)].duplicate(true)
				o13_area["source_sha256"] = intent.sha256
				o13_area["copy_bytes"] = intent.bytes
				next_buffers[str(0x80180000)] = o13_area
			"first_arrival_shiori_copy":
				# memcpy(0x80180000, source, n): only the copied prefix is RAM
				# the port knows; bytes past it are not modelled here.
				var copied: PackedByteArray = next_buffers[str(intent.source)].bytes.slice(0,intent.bytes)
				next_buffers[str(0x80180000)] = {"bytes":copied,"sha256":Store._sha(copied),"resource_id":intent.resource_id,"source_sha256":intent.sha256,"copy_bytes":intent.bytes}
			"first_arrival_stage_actor_upload":
				next_stage_actor_uploads[intent.batch_key] = {"batch_key":intent.batch_key,"resource_id":intent.resource_id,"slot":intent.slot,"source_offset":intent.source_offset}
	if setup_seen and (not next_upload.get("operation_ids") is Array or next_upload.operation_ids.size() != 19): return "first-arrival native upload batch is incomplete"
	if board_decode_seen and not next_board_upload.has("batch_key"): return "first-arrival board upload batch is incomplete"
	_pending = next_pending
	_buffers = next_buffers
	_actor_upload = next_upload
	_board_upload = next_board_upload
	_names_bg_uploads = next_names_uploads
	_cut_uploads = next_cut_uploads
	_stage_actor_uploads = next_stage_actor_uploads
	_last_published_tick = tick
	return ""

func resource_identity() -> String:
	return _store.identity() if _store != null else ""

func board_resource_identity() -> String:
	if _board_store == null: return ""
	var desc := _board_store.native_read_descriptor()
	return str(desc.get("manifest_sha256","")) if desc.get("error","") == "" else ""

func completed_names_bg_buffer(state: State, resource_id: String) -> Dictionary:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID, BoardStore.BG_AD00_ID, BoardStore.BG_AD05_ID, BoardStore.BG_AD0B_ID, BoardStore.BG_496A_ID, BoardStore.BG_4973_ID]:
		return {"error":"completed names background buffer belongs to another or unbound Session"}
	var expected_sha := ""
	match resource_id:
		BoardStore.BG_495D_ID: expected_sha = BoardStore.BG_495D_TRANSFER_SHA
		BoardStore.BG_4966_ID: expected_sha = BoardStore.BG_4966_TRANSFER_SHA
		BoardStore.BG_ACDD_ID: expected_sha = BoardStore.BG_ACDD_TRANSFER_SHA
		BoardStore.BG_ACE6_ID: expected_sha = BoardStore.BG_ACE6_TRANSFER_SHA
		BoardStore.BG_ACF6_ID: expected_sha = BoardStore.BG_ACF6_TRANSFER_SHA
		BoardStore.BG_ACD4_ID: expected_sha = BoardStore.BG_ACD4_TRANSFER_SHA
		BoardStore.BG_ACEC_ID: expected_sha = BoardStore.BG_ACEC_TRANSFER_SHA
		BoardStore.BG_AD00_ID: expected_sha = BoardStore.BG_AD00_TRANSFER_SHA
		BoardStore.BG_AD05_ID: expected_sha = BoardStore.BG_AD05_TRANSFER_SHA
		BoardStore.BG_AD0B_ID: expected_sha = BoardStore.BG_AD0B_TRANSFER_SHA
		BoardStore.BG_496A_ID: expected_sha = BoardStore.BG_496A_TRANSFER_SHA
		BoardStore.BG_4973_ID: expected_sha = BoardStore.BG_4973_TRANSFER_SHA
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("epoch") is int or not media.get("requests") is Dictionary or not media.get("background_pending") is Dictionary:
		return {"error":"completed names background transfer has no owned request"}
	var request: Variant = media.requests.get(media.background_pending.get("1",""))
	if not request is Dictionary or request.get("kind") != "background" or request.get("status") != "ready" or request.get("resource_id") != resource_id or request.get("resource_sha256") != expected_sha:
		return {"error":"names background transfer has not completed in this logical Session"}
	var pending_request: Dictionary = request.duplicate(true)
	pending_request.status = "pending"
	var error := _intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":pending_request})
	if not error.is_empty() or request.epoch != media.epoch:
		return {"error":"completed names background request has malformed or foreign ownership"}
	var buffer: Variant = _buffers.get(str(0x80162000))
	if not buffer is Dictionary or buffer.get("resource_id") != resource_id or buffer.get("sha256") != expected_sha or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 20480 or Store._sha(buffer.bytes) != expected_sha:
		return {"error":"completed names background transfer has no published authentic bytes"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":expected_sha,"resource_id":resource_id,
		"epoch":media.epoch,"request_key":request.request_key,"publication_tick":_last_published_tick}

func completed_cut_buffer(state: State, resource_id: String) -> Dictionary:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.CUT_12_ID, BoardStore.CUT_05_ID, BoardStore.CUT_06_ID, BoardStore.CUT_07_ID, BoardStore.CUT_08_ID, BoardStore.CUT_09_ID, BoardStore.CUT_14_ID, BoardStore.CUT_16_ID]:
		return {"error":"completed cut buffer belongs to another or unbound Session"}
	var expected_sha := BoardStore.CUT_12_SHA
	if resource_id == BoardStore.CUT_05_ID: expected_sha = BoardStore.CUT_05_SHA
	elif resource_id == BoardStore.CUT_06_ID: expected_sha = BoardStore.CUT_06_SHA
	elif resource_id == BoardStore.CUT_07_ID: expected_sha = BoardStore.CUT_07_SHA
	elif resource_id == BoardStore.CUT_08_ID: expected_sha = BoardStore.CUT_08_SHA
	elif resource_id == BoardStore.CUT_09_ID: expected_sha = BoardStore.CUT_09_SHA
	elif resource_id == BoardStore.CUT_14_ID: expected_sha = BoardStore.CUT_14_SHA
	elif resource_id == BoardStore.CUT_16_ID: expected_sha = BoardStore.CUT_16_SHA
	var buffer: Variant = _buffers.get(str(0x801a4000))
	if not buffer is Dictionary or buffer.get("resource_id") != resource_id or buffer.get("sha256") != expected_sha or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 100352 or Store._sha(buffer.bytes) != expected_sha:
		return {"error":"completed cut transfer has no published authentic bytes at 0x801a4000"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":expected_sha,"resource_id":resource_id,"publication_tick":_last_published_tick}

func completed_shiori_page(state: State, resource_id: String, offset: int, copied_this_frame: bool) -> Dictionary:
	# The RAM page load_csr_tp reads at 0x80180000+offset: the published
	# memcpy prefix of resource_id, or (on the copy frame, before this
	# frame's publication) its completed source buffer.
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state or not SHIORI_COPIES.has(resource_id):
		return {"error":"completed Shiori page belongs to another or unbound Session"}
	var copy: Array = SHIORI_COPIES[resource_id]
	var sha: String = _shiori_bytes[resource_id].sha256
	if offset < 0 or offset + ShioriActors.PAGE_BYTES > int(copy[1]):
		return {"error":"Shiori page lies outside the bytes its memcpy copied"}
	var buffer: Variant = _buffers.get(str(int(copy[0]) if copied_this_frame else 0x80180000))
	if not buffer is Dictionary or buffer.get("resource_id") != resource_id or not buffer.get("bytes") is PackedByteArray:
		return {"error":"Shiori page has no published RAM copy of its resource"}
	var expected: PackedByteArray = _shiori_bytes[resource_id].bytes
	if copied_this_frame:
		if buffer.get("sha256") != sha or Store._sha(buffer.bytes) != sha: return {"error":"Shiori page source read differs from its resource"}
	elif buffer.get("copy_bytes") != int(copy[1]) or buffer.get("source_sha256") != sha or buffer.bytes != expected.slice(0,int(copy[1])):
		return {"error":"Shiori page RAM copy differs from its memcpy"}
	var page: PackedByteArray = buffer.bytes.slice(offset,offset+ShioriActors.PAGE_BYTES)
	if page != expected.slice(offset,offset+ShioriActors.PAGE_BYTES): return {"error":"Shiori page bytes differ from the resource"}
	return {"error":"","bytes":page,"publication_tick":_last_published_tick}

func completed_stage_actor_buffer(state: State, resource_id: String, allow_backup_restore: bool = false) -> Dictionary:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.O_12_ID, BoardStore.O_05_ID, BoardStore.O_06_ID, BoardStore.O_07_ID, BoardStore.O_08_ID, BoardStore.O_09_ID, BoardStore.O_11_ID, BoardStore.O_16_ID, BoardStore.O_13_ID]:
		return {"error":"completed stage actor buffer belongs to another or unbound Session"}
	var expected_sha := BoardStore.O_12_SHA
	var expected_size := 34816
	if resource_id == BoardStore.O_05_ID:
		expected_sha = BoardStore.O_05_SHA
		expected_size = 18432
	elif resource_id == BoardStore.O_06_ID:
		expected_sha = BoardStore.O_06_SHA
		expected_size = 51200
	elif resource_id == BoardStore.O_07_ID:
		expected_sha = BoardStore.O_07_SHA
		expected_size = 51200
	elif resource_id == BoardStore.O_08_ID:
		expected_sha = BoardStore.O_08_SHA
		expected_size = 18432
	if resource_id == BoardStore.O_09_ID:
		expected_sha = BoardStore.O_09_SHA
		expected_size = 51200
	if resource_id == BoardStore.O_11_ID:
		expected_sha = BoardStore.O_11_SHA
		expected_size = 0x10800
	if resource_id == BoardStore.O_16_ID:
		expected_sha = BoardStore.O_16_SHA
		expected_size = 0xc800
	if resource_id == BoardStore.O_13_ID:
		# Stage17 case3 cd_read_sectors(0x41, 0x80180000, 0xAC50): no memcpy.
		expected_sha = BoardStore.O_13_SHA
		expected_size = BoardStore.O13_BYTES
	# The same-frame memcpy source: 0x801E8000 for O_11 and the stage15
	# case34 O_07.NBN copy; 0x801F0000 otherwise. O_16's copy frame (stage14
	# case6) is a Shiori memcpy read through completed_shiori_page; later
	# stage15 frames read its published copy at 0x80180000 here.
	var e8_source: bool = resource_id == BoardStore.O_11_ID or (resource_id == BoardStore.O_06_ID and state.mode_stage in [15, 16])
	var buffer: Variant = _buffers.get(str(0x80180000))
	if allow_backup_restore and (not buffer is Dictionary or buffer.get("resource_id") != resource_id):
		# Stage11 case6 copies O_13 from its 0x801D8000 staging read.
		buffer = _buffers.get(str(0x801e8000 if e8_source else (0x801d8000 if resource_id == BoardStore.O_13_ID else 0x801f0000)))
	if not buffer is Dictionary or buffer.get("resource_id") != resource_id or buffer.get("sha256") != expected_sha or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != expected_size or Store._sha(buffer.bytes) != expected_sha:
		return {"error":"completed stage actor transfer has no published authentic bytes"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":expected_sha,"resource_id":resource_id,"publication_tick":_last_published_tick}

func completed_board_buffer(state: State) -> Dictionary:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state:
		return {"error":"completed board buffer belongs to another or unbound Session"}
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("epoch") is int or not media.get("requests") is Dictionary or not media.get("board_pending") is Dictionary:
		return {"error":"completed board transfer has no owned request"}
	var request: Variant = media.requests.get(media.board_pending.get(str(0x80162000),""))
	if not request is Dictionary or request.get("kind") != "board" or request.get("status") != "ready" or request.get("resource_id") != BOARD_ID or request.get("resource_sha256") != BoardStore.SOURCE_SHA:
		return {"error":"board transfer has not completed in this logical Session"}
	var pending_request: Dictionary = request.duplicate(true)
	pending_request.status = "pending"
	var error := _intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":pending_request})
	if not error.is_empty() or request.epoch != media.epoch:
		return {"error":"completed board request has malformed or foreign ownership"}
	var buffer: Variant = _buffers.get(str(0x80162000))
	if not buffer is Dictionary or buffer.get("resource_id") != BOARD_ID or buffer.get("sha256") != BoardStore.SOURCE_SHA or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 18432 or Store._sha(buffer.bytes) != BoardStore.SOURCE_SHA:
		return {"error":"completed board transfer has no published authentic bytes"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":BoardStore.SOURCE_SHA,
		"epoch":media.epoch,"request_key":request.request_key,"publication_tick":_last_published_tick}

func completed_actor_buffer(state: State) -> Dictionary:
	# Read the published buffer only through this Session's actual completion.
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _store == null or owner == null or owner.state != state:
		return {"error":"completed actor buffer belongs to another or unbound Session"}
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("epoch") is int or not media.get("requests") is Dictionary or not media.get("actor_pending") is Dictionary:
		return {"error":"completed actor transfer has no owned request"}
	var request: Variant = media.requests.get(media.actor_pending.get(str(0x80180000),""))
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != Store.ACTOR_ID or request.get("resource_sha256") != Store.ACTOR_SHA:
		return {"error":"actor transfer has not completed in this logical Session"}
	var pending_request: Dictionary = request.duplicate(true)
	pending_request.status = "pending" # Reuse the native request schema/hash guard.
	var error := _intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":pending_request})
	if not error.is_empty() or request.epoch != media.epoch:
		return {"error":"completed actor request has malformed or foreign ownership"}
	var buffer: Variant = _buffers.get(str(0x80180000))
	if not buffer is Dictionary or buffer.get("resource_id") != Store.ACTOR_ID or buffer.get("sha256") != Store.ACTOR_SHA or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 26624 or Store._sha(buffer.bytes) != Store.ACTOR_SHA:
		return {"error":"completed actor transfer has no published authentic bytes"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":Store.ACTOR_SHA,
		"epoch":media.epoch,"request_key":request.request_key,"publication_tick":_last_published_tick}

func publication_tick() -> int:
	return _last_published_tick

func paint_background(target: Image, state: State, visible_renderer: RefCounted = null) -> String:
	if ShioriActors.visible_scene(state):
		if _store == null or _shiori_bytes.is_empty() or target == null or target.get_size() != Vector2i(320,240):
			return "Shiori scene draw is outside its prepared source scope"
		if visible_renderer == null or not visible_renderer.has_method("paint_shiori_scene"):
			return "stages 12-14 have no prepared Shiori scene renderer"
		var painted: Variant = visible_renderer.call("paint_shiori_scene",target,state)
		return painted if painted is String else "Shiori scene renderer returned an unknown result"
	if closing_frame(state):
		# Stages 18-20 and the 20->21 frame: P_01_12, white/flash boxes and
		# the O_13 actor96 are one OT composition; the HUD stays with the caller.
		if _board_store == null or target == null or target.get_size() != Vector2i(320,240): return "stages 18-20 draw is outside its prepared source scope"
		var owner := _closing_owner_error(state)
		if owner.is_empty() and state.mode_stage == 21: owner = _closing_exit_error(state,false)
		if owner.is_empty(): owner = _closing_upload_error(state)
		if not owner.is_empty(): return owner
		if visible_renderer == null or not visible_renderer.has_method("paint_closing"): return CLOSING_RENDERER_MISSING
		var closing: Variant = visible_renderer.call("paint_closing",target,state)
		return closing if closing is String else "closing renderer returned an unknown result"
	if finish_frame(state):
		return "" if _board_store != null and target != null and target.get_size() == Vector2i(320,240) else "stage21 frame is outside its prepared source scope"
	var visible_boundary := _visible_boundary_error(state,visible_renderer)
	if _store == null or target == null or target.get_size() != Vector2i(320,240) or state.game_mode != 0x15 or (state.mode_stage != 0 and not visible_boundary.is_empty()):
		return "first-arrival background draw is outside its prepared source scope"
	if state.mode_stage in [15, 16, 17]:
		if _board_store == null or visible_renderer == null or not visible_renderer.has_method("paint_names_background"):
			return "first-arrival names background requires a prepared names renderer"
		var stages_err: Variant = visible_renderer.call("paint_names_background",target,state)
		return stages_err if stages_err is String else "names background renderer returned an unknown result"
	var media: Variant = state.opening.get("arrival_media")
	if not media is Dictionary or not media.get("background_brightness") is int or media.background_brightness != 128 or not media.get("actor_initialized") is bool:
		return "first-arrival background needs its source reset/brightness state"
	var flags: Variant = state.opening.get("actor96_draw_flags",0)
	if not flags is int or (flags != (0x24 if media.actor_initialized else 0) and not visible_boundary.is_empty()):
		return "first-arrival requires a source actor compositor before visibility"
	var stage1_scroll: bool = state.mode_stage == 1 and state.mode_step == 1 and state.substep in [1, 2, 3, 4, 5]
	var stage2_transition: bool = state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("active_draw") == "board_scroll_transition"
	if stage1_scroll or stage2_transition:
		if _board_store == null or visible_renderer == null or not visible_renderer.has_method("paint_board_scroll"):
			return "first-arrival board scroll requires a prepared board renderer"
		var scroll_err: Variant = visible_renderer.call("paint_board_scroll",target,state,_board_store.preview())
		return scroll_err if scroll_err is String else "board scroll renderer returned an unknown result"
	if (state.mode_stage in [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17] or line17_exit_frame(state)) and state.mode_step == 0:
		if _board_store == null or visible_renderer == null or not visible_renderer.has_method("paint_names_background"):
			return "first-arrival names background requires a prepared names renderer"
		var names_err: Variant = visible_renderer.call("paint_names_background",target,state)
		return names_err if names_err is String else "names background renderer returned an unknown result"
	var work: Variant = state.opening.get("media")
	if not work is Dictionary or not work.get("decoded") is Dictionary: return "first-arrival background cache is not initialized"
	var decoded: Variant = work.decoded.get("1")
	if decoded == null: return ""
	if not decoded is Dictionary or decoded.get("resource_id") is not String or decoded.resource_id != Store.BG_ID or decoded.get("resource_sha256") is not String or decoded.resource_sha256 != Store.BG_TRANSFER_SHA or not decoded.get("buffer") is int or decoded.buffer not in [0,1]:
		return "first-arrival draw has no authenticated selected background"
	# dec_bg_reset gives bg_brightness128 independently of UI brightness.
	target.blend_rect(_store.background_image(),Rect2i(0,0,320,160),Vector2i.ZERO)
	return ""

func diagnostics() -> Dictionary:
	var buffers: Dictionary = {}
	for destination: String in _buffers:
		buffers[destination] = {"size":_buffers[destination].bytes.size(),"sha256":_buffers[destination].sha256}
	return {"buffers":buffers,"pending_observations":_pending.size(),"actor_source_setup_requested":not _actor_upload.is_empty(),
		"native_upload_operation_ids":_actor_upload.get("operation_ids",[]).duplicate(),"vram_ready":false,"visible_actor_supported":false}

func reset_session() -> void:
	_pending.clear()
	_buffers.clear()
	_actor_upload.clear()
	_board_upload.clear()
	_names_bg_uploads.clear()
	_cut_uploads.clear()
	_stage_actor_uploads.clear()
	_last_published_tick = -1
	_session = null
