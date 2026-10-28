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
const BOARD_ID := "prologue.board.0x4954"
const APPROVAL_META := &"first_arrival_media_preflight_v1"
const OWN_INTENTS := ["opening_media_request","opening_background_select","opening_arrival_actor_setup",
	"opening_board_decode","opening_media_reset","opening_scene_reset","first_arrival_background_draw",
	"first_arrival_native_upload","first_arrival_board_upload","first_arrival_board_scroll_draw",
	"first_arrival_names_bg_upload","first_arrival_names_crossfade_draw",
	"first_arrival_cut_upload","first_arrival_o12_backup","first_arrival_o12_restore","first_arrival_o06_copy","first_arrival_o07_copy","first_arrival_stage_actor_upload"]
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
var _session: WeakRef

func prepare(store: Store, board_store: BoardStore = null) -> String:
	if _store != null: return "first-arrival media bridge already owns immutable resources"
	if store == null or store.identity().is_empty(): return "first-arrival media bridge needs an authenticated native transfer store"
	var resources := store.descriptions()
	var boards: Dictionary = {}
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
		]:
			var bg_tr := board_store.background_transfer(spec[0])
			var bg_up := board_store.background_software_upload(spec[0])
			if not bg_tr.get("error") is String or not bg_tr.error.is_empty() or bg_tr.get("lba") != spec[1] or bg_tr.get("sectors") != 10 or bg_tr.get("destination") != 0x80162000 or not bg_tr.get("bytes") is PackedByteArray or bg_tr.bytes.size() != 20480 or Store._sha(bg_tr.bytes) != spec[2]:
				return "first-arrival names background transfer descriptor differs from source"
			if not bg_up.get("error") is String or not bg_up.error.is_empty() or bg_up.get("rectangle") != spec[3] or bg_up.get("buffer") != spec[4] or not bg_up.get("words") is PackedByteArray or bg_up.words.size() != 102400 or Store._sha(bg_up.words) != spec[5]:
				return "first-arrival names background software upload descriptor differs from source"
		var bg_descs := board_store.background_descriptions()
		if bg_descs.size() != 7 or not bg_descs.has(str(0x495d)) or not bg_descs.has(str(0x4966)) or not bg_descs.has(str(0xacdd)) or not bg_descs.has(str(0xace6)) or not bg_descs.has(str(0xacf6)) or not bg_descs.has(str(0xacd4)) or not bg_descs.has(str(0xacec)):
			return "first-arrival names background descriptions are missing"
		resources.backgrounds.merge(bg_descs, true)
		var actor_descs := board_store.actor_descriptions()
		if actor_descs.size() != 8:
			return "first-arrival stage 2..6 actor descriptions are missing"
		resources.actors.merge(actor_descs, true)
		boards[str(0x4954)] = {"id":BOARD_ID,"sha256":BoardStore.SOURCE_SHA,"lba":0x4954,"sectors":9,"destination":0x80162000,"manifest_sha256":BoardStore.MANIFEST_SHA}
	var error: String = queue.prepare(resources.backgrounds,resources.actors,{},boards)
	if not error.is_empty(): return error
	_store = store
	_board_store = board_store
	return ""

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
	services["audio_command"] = stage_05_audio_command
	return services

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
	state.opening.erase("stage_06_timer")
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
	result.error = "first-arrival background decode is outside its source stage"
	return -1

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
	result.error = "first-arrival line0 setup is outside the source dispatch"

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
		state.mode_step_timer = 0
		state.opening["stage_06_timer"] = 0
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
	if state.game_mode == 0x15 and state.mode_stage in [2, 3, 4, 5, 6, 7] and state.mode_step == 0 and state.opening.get("names_media") is Dictionary:
		if state.mode_stage == 2 and state.substep in [0, 1, 2, 5, 6, 7, 8]:
			state.opening.names_media["active_draw"] = "dec_bg_draw"
		elif state.mode_stage in [3, 4, 5, 6, 7]:
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

func _visible_boundary_error(state: State, renderer: RefCounted) -> String:
	if state.game_mode != 0x15 or state.mode_stage not in [1, 2, 3, 4, 5, 6, 7] or not state.opening.get("actor96_draw_flags") is int:
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
		var stage35_active: bool = not state.opening.has("prologue_petals") and layer0 is Dictionary and layer0.get("enabled") == 1
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
				var allowed_bg: bool = intent.get("resource_id") == Store.BG_ID or (_board_store != null and intent.get("resource_id") in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID])
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
			"first_arrival_stage_actor_upload":
				stage_actor_upload_count += 1
				var has_actor_buf: bool = (_buffers.has(str(0x80180000)) and _buffers[str(0x80180000)].get("resource_id") == intent.resource_id) or (o12_restore_count == 1 and intent.resource_id == BoardStore.O_12_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_12_ID) or (o06_copy_count == 1 and intent.resource_id == BoardStore.O_06_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_06_ID) or (o07_copy_count == 1 and intent.resource_id == BoardStore.O_07_ID and _buffers.has(str(0x801f0000)) and _buffers[str(0x801f0000)].get("resource_id") == BoardStore.O_07_ID)
				if _board_store == null or not has_actor_buf:
					return "first-arrival stage actor upload lacks resident NBN buffer at 0x80180000"
			"opening_media_reset":
				reset_seen = true
				if intent.get("service") != Queue.SERVICE or intent.get("epoch") != state.opening.get("media",{}).get("epoch"):
					return "first-arrival reset differs from transactional ownership"
	if setup_count > 1 or (setup_count == 1 and (upload_count != 19 or reset_seen)) or (setup_count == 0 and upload_count != 0):
		return "first-arrival source setup requires one complete ordered native upload batch"
	if board_decode_count > 1 or board_decode_count != board_upload_count or (board_decode_count == 1 and (reset_seen or setup_count != 0)):
		return "first-arrival board decode requires one complete board upload batch"
	if names_bg_upload_count > 1 or (names_bg_upload_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0)):
		return "first-arrival names background decode requires one isolated upload batch"
	if cut_upload_count > 1 or (cut_upload_count == 1 and (reset_seen or setup_count != 0 or board_decode_count != 0)):
		return "first-arrival cut upload requires one isolated upload batch"
	if stage_actor_upload_count > 3 or o12_backup_count > 1 or o12_restore_count > 1 or o06_copy_count > 1 or o07_copy_count > 1:
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
		"opening_scene_reset": fields = ["kind"]
		"first_arrival_background_draw": fields = ["kind","slot"]
		"first_arrival_native_upload": fields = ["kind","batch_key","ordinal","request"]
		"first_arrival_board_upload": fields = ["kind","batch_key","decode_rect","move_call","word_sha256"]
		"first_arrival_board_scroll_draw": fields = ["kind","scroll_x","rect","tpage_args"]
		"first_arrival_names_bg_upload": fields = ["kind","batch_key","resource_id","buffer","decode_rect","word_sha256"]
		"first_arrival_names_crossfade_draw": fields = ["kind","slot","fade_amount"]
		"first_arrival_cut_upload": fields = ["kind","batch_key","resource_id","sha256"]
		"first_arrival_o12_backup": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o12_restore": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o06_copy": fields = ["kind","resource_id","sha256","bytes"]
		"first_arrival_o07_copy": fields = ["kind","resource_id","sha256","bytes"]
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
		var valid_acf6: bool = intent.resource_id == BoardStore.BG_ACF6_ID and intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160]) and intent.word_sha256 == BoardStore.BG_ACF6_WORD_SHA
		var valid_acd4: bool = intent.resource_id == BoardStore.BG_ACD4_ID and intent.buffer == 1 and Store._same(intent.decode_rect,[320,256,320,160]) and intent.word_sha256 == BoardStore.BG_ACD4_WORD_SHA
		var valid_acec: bool = intent.resource_id == BoardStore.BG_ACEC_ID and intent.buffer == 0 and Store._same(intent.decode_rect,[704,0,320,160]) and intent.word_sha256 == BoardStore.BG_ACEC_WORD_SHA
		if not valid_495d and not valid_4966 and not valid_acdd and not valid_ace6 and not valid_acf6 and not valid_acd4 and not valid_acec:
			return "first-arrival names background upload descriptor is invalid"
		return ""
	if kind == "first_arrival_cut_upload":
		var valid_cut12: bool = intent.resource_id == BoardStore.CUT_12_ID and intent.sha256 == BoardStore.CUT_12_SHA
		var valid_cut05: bool = intent.resource_id == BoardStore.CUT_05_ID and intent.sha256 == BoardStore.CUT_05_SHA
		var valid_cut06: bool = intent.resource_id == BoardStore.CUT_06_ID and intent.sha256 == BoardStore.CUT_06_SHA
		var valid_cut07: bool = intent.resource_id == BoardStore.CUT_07_ID and intent.sha256 == BoardStore.CUT_07_SHA
		if _board_store == null or not Store._sha_string(intent.batch_key) or (not valid_cut12 and not valid_cut05 and not valid_cut06 and not valid_cut07):
			return "first-arrival cut upload descriptor is invalid"
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
		if _board_store == null or not Store._sha_string(intent.batch_key) or (not valid_o12_96 and not valid_o12_97 and not valid_o05_96 and not valid_o06_96_s5 and not valid_o06_96_s6 and not valid_o06_99_s7 and not valid_o07_96_s5 and not valid_o07_96_s6 and not valid_o07_96_s7):
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
			var valid_bg_sel: bool = intent.resource_id == Store.BG_ID or (_board_store != null and ((intent.resource_id in [BoardStore.BG_495D_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACEC_ID] and intent.buffer == 0) or (intent.resource_id in [BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACD4_ID] and intent.buffer == 1)))
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
		if not bg_494b and not bg_495d and not bg_4966 and not bg_acdd and not bg_ace6 and not bg_acf6 and not bg_acd4 and not bg_acec:
			return "first-arrival background arguments differ from source"
	elif request.kind == "actors":
		if args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival actor arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival actor arguments require exact integers"
		var act_abab: bool = request.resource_id == Store.ACTOR_ID and request.resource_sha256 == Store.ACTOR_SHA and args.lba == 0xabab and args.sectors == 13 and args.destination == 0x80180000
		var act_ab18: bool = _board_store != null and request.resource_id == BoardStore.CUT_12_ID and request.resource_sha256 == BoardStore.CUT_12_SHA and args.lba == 0xab18 and args.sectors == 49 and args.destination == 0x801a4000
		var act_ac3f: bool = _board_store != null and request.resource_id == BoardStore.O_12_ID and request.resource_sha256 == BoardStore.O_12_SHA and args.lba == 0xac3f and args.sectors == 17 and args.destination == 0x80180000
		var act_aa23: bool = _board_store != null and request.resource_id == BoardStore.CUT_05_ID and request.resource_sha256 == BoardStore.CUT_05_SHA and args.lba == 0xaa23 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abb8: bool = _board_store != null and request.resource_id == BoardStore.O_05_ID and request.resource_sha256 == BoardStore.O_05_SHA and args.lba == 0xabb8 and args.sectors == 9 and args.destination == 0x80180000
		var act_aa54: bool = _board_store != null and request.resource_id == BoardStore.CUT_06_ID and request.resource_sha256 == BoardStore.CUT_06_SHA and args.lba == 0xaa54 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abca: bool = _board_store != null and request.resource_id == BoardStore.O_06_ID and request.resource_sha256 == BoardStore.O_06_SHA and args.lba == 0xabca and args.sectors == 25 and args.destination == 0x801f0000
		var act_aa85: bool = _board_store != null and request.resource_id == BoardStore.CUT_07_ID and request.resource_sha256 == BoardStore.CUT_07_SHA and args.lba == 0xaa85 and args.sectors == 49 and args.destination == 0x801a4000
		var act_abe3: bool = _board_store != null and request.resource_id == BoardStore.O_07_ID and request.resource_sha256 == BoardStore.O_07_SHA and args.lba == 0xabe3 and args.sectors == 25 and args.destination == 0x801f0000
		if not act_abab and not act_ab18 and not act_ac3f and not act_aa23 and not act_abb8 and not act_aa54 and not act_abca and not act_aa85 and not act_abe3:
			return "first-arrival actor read extent differs from source"
	elif request.kind == "board":
		if _board_store == null or request.resource_id != BOARD_ID or request.resource_sha256 != BoardStore.SOURCE_SHA or args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival board arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival board arguments require exact integers"
		if args.lba != 0x4954 or args.sectors != 9 or args.destination != 0x80162000: return "first-arrival board read extent differs from source"
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
			"opening_media_request":
				var request: Dictionary = intent.request
				if request.kind == "board":
					var desc := _board_store.native_read_descriptor()
					if not str(desc.get("error","")).is_empty(): return str(desc.error)
					next_buffers[str(desc.destination)] = {"bytes":desc.payload,"sha256":BoardStore.SOURCE_SHA,"resource_id":BOARD_ID}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind == "background" and request.resource_id in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID]:
					var bg_transfer := _board_store.background_transfer(request.resource_id)
					if not str(bg_transfer.get("error","")).is_empty(): return str(bg_transfer.error)
					next_buffers[str(bg_transfer.destination)] = {"bytes":bg_transfer.bytes,"sha256":bg_transfer.sha256,"resource_id":request.resource_id}
					next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
						"status":"ready","resource_id":request.resource_id})
				elif request.kind == "actors" and request.resource_id in [BoardStore.CUT_12_ID, BoardStore.O_12_ID, BoardStore.CUT_05_ID, BoardStore.O_05_ID, BoardStore.CUT_06_ID, BoardStore.O_06_ID, BoardStore.CUT_07_ID, BoardStore.O_07_ID]:
					var st_transfer := _board_store.stage_transfer(request.resource_id)
					if not str(st_transfer.get("error","")).is_empty(): return str(st_transfer.error)
					next_buffers[str(st_transfer.destination)] = {"bytes":st_transfer.bytes,"sha256":st_transfer.sha256,"resource_id":request.resource_id}
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
			"first_arrival_o12_restore", "first_arrival_o06_copy", "first_arrival_o07_copy":
				next_buffers[str(0x80180000)] = next_buffers[str(0x801f0000)].duplicate(true)
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
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.BG_495D_ID, BoardStore.BG_4966_ID, BoardStore.BG_ACDD_ID, BoardStore.BG_ACE6_ID, BoardStore.BG_ACF6_ID, BoardStore.BG_ACD4_ID, BoardStore.BG_ACEC_ID]:
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
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.CUT_12_ID, BoardStore.CUT_05_ID, BoardStore.CUT_06_ID, BoardStore.CUT_07_ID]:
		return {"error":"completed cut buffer belongs to another or unbound Session"}
	var expected_sha := BoardStore.CUT_12_SHA
	if resource_id == BoardStore.CUT_05_ID: expected_sha = BoardStore.CUT_05_SHA
	elif resource_id == BoardStore.CUT_06_ID: expected_sha = BoardStore.CUT_06_SHA
	elif resource_id == BoardStore.CUT_07_ID: expected_sha = BoardStore.CUT_07_SHA
	var buffer: Variant = _buffers.get(str(0x801a4000))
	if not buffer is Dictionary or buffer.get("resource_id") != resource_id or buffer.get("sha256") != expected_sha or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 100352 or Store._sha(buffer.bytes) != expected_sha:
		return {"error":"completed cut transfer has no published authentic bytes at 0x801a4000"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":expected_sha,"resource_id":resource_id,"publication_tick":_last_published_tick}

func completed_stage_actor_buffer(state: State, resource_id: String, allow_backup_restore: bool = false) -> Dictionary:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _board_store == null or owner == null or owner.state != state or resource_id not in [BoardStore.O_12_ID, BoardStore.O_05_ID, BoardStore.O_06_ID, BoardStore.O_07_ID]:
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
	var buffer: Variant = _buffers.get(str(0x80180000))
	if allow_backup_restore and (not buffer is Dictionary or buffer.get("resource_id") != resource_id):
		buffer = _buffers.get(str(0x801f0000))
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
	var visible_boundary := _visible_boundary_error(state,visible_renderer)
	if _store == null or target == null or target.get_size() != Vector2i(320,240) or state.game_mode != 0x15 or (state.mode_stage != 0 and not visible_boundary.is_empty()):
		return "first-arrival background draw is outside its prepared source scope"
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
	if state.mode_stage in [2, 3, 4, 5, 6, 7] and state.mode_step == 0:
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
