extends RefCounted
## T-39002: production services of the mode 0x34 stage 0xFF command reload on
## the installed fresh New Game (flow/new_game_command_reload.gd REQUIRED).
## Source: src/main/home_screens.c settings_command_screen_mode_handler,
## settings_command_screen_mode_stage_reload, settings_command_screen_setup_reset,
## settings_command_screen_reload_calendar, settings_command_screen_build ->
## settings_room_bg_load_start, settings_room_bg_load_finish;
## src/main/background_images.c dec_bg_cd_read / dec_bg_decode_to_vram /
## dec_bg_show_set.
##
## Every CD read goes through the opening media queue with the logic-frame
## latency model (floor(14 + 0.4 * sectors) frames: 73 for the 0x95-sector
## CALROM read, 18 for the 10-sector room read), and its bytes come from the
## installed first_arrival_command_reload role through the media bridge's
## publication. The bank poll and bank start are the opening's own owners
## (media.finish_bank_poll, the BGM bridge). Nothing here completes a read,
## a poll or a decode by itself.
##
## state.opening.command_reload records the source calls with no other port
## representation: setup (setup_reset's draw2d3d/actor_draw_enable/set_dec_bri/
## text_system_reset/k_disp_start), calendar (calendar_month_load), mascot
## (hud_mascot_set/hud_mascot_display_switch and the same frame's handler-tail
## hud_mascot_update), room (the slot-0 read and its
## decode) and built (settings_command_screen_build ran). The renderer reads it.
##
## Not modeled, refused explicitly: calendar_month_load with month_data_index
## 35 or more (the source skips both LoadSquare uploads), a visible mascot's
## per-frame hud_mascot_update (choice 1..13), a slot-0 LBA cache hit in
## settings_room_bg_load_start (r 0/1 -> dec_bg_show_set + mode_stage_next) and
## a failed decode (bg_decode_result -1 -> dec_bg_reset + reread). The decoded
## room pixels are the installed HUD manifest's room image, whose provenance
## names the same C_02_02H.BS file; this owner checks that identity.

const Reload = preload("res://flow/new_game_command_reload.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const CommandReload = preload("res://services/command_reload_store.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")

const RECORD := "command_reload"
const MODE := 0x34
const STAGE_RELOAD := 0xFF
const ROOM_SLOT := 0
const ROOM_DESTINATION := 0x80162000
const CALENDAR_SIDE := 0
const FULL_BRIGHTNESS := 0x80
const CACHE_HIT_UNSUPPORTED := "settings_room_bg_load_start slot-0 cache hit (dec_bg_show_set + mode_stage_next) is not modeled"
const MASCOT_UPDATE_UNSUPPORTED := "hud_mascot_update for a visible mascot (choice 1..13) is not modeled"
## calendar_month_load's `month_data_index < 35` LoadSquare guard.
const CALENDAR_MONTH_LIMIT := 35
const CALENDAR_INDEX_UNSUPPORTED := "calendar_month_load with month_data_index outside 0..34 (the source's `< 35` LoadSquare guard) is not modeled"
const DECODE_FAILURE_UNSUPPORTED := "settings_room_bg_load_finish decode failure (dec_bg_reset + reread) is not modeled"

var media: Media
var hud_provider: RefCounted
## func(state) -> int: the stage21 COM_MIX/SPRING bank poll owner.
var bank_poll_owner: Callable
## func(state, bank, sequence, result): audio_bank_start's owner.
var bank_start_owner: Callable
## func(state) -> Dictionary: the prologue HUD projection of the state.
var projection_owner: Callable
## func() -> Object: the session RNG (hud_screen_frame_draw mode 4 only).
var rng_owner: Callable
## Original file identity of the room the installed HUD manifest decoded.
var room_original_sha256 := ""
var error := ""


func prepare(bridge: Media, provider: RefCounted, bank_poll: Callable, bank_start: Callable,
		projection: Callable, rng: Callable, hud_room_original_sha256: String) -> String:
	if media != null:
		return "command reload services already own their media bridge"
	if bridge == null or bridge._command_reload == null:
		return "command reload needs the media bridge with the installed command reload reads"
	if provider == null or not provider.has_method("advance_outer"):
		return "command reload needs the prologue HUD owner of the checker globals"
	for callable: Callable in [bank_poll, bank_start, projection, rng]:
		if not callable.is_valid():
			return "command reload owner callbacks are unavailable"
	var room: Dictionary = bridge._command_reload.description("room_spring")
	if room.is_empty() or room.get("primary_sha256") != hud_room_original_sha256:
		return "installed HUD room image is not decoded from the command reload room read"
	media = bridge
	hud_provider = provider
	bank_poll_owner = bank_poll
	bank_start_owner = bank_start
	projection_owner = projection
	rng_owner = rng
	room_original_sha256 = hud_room_original_sha256
	return ""


## The Reload.REQUIRED services, bound to this owner.
func services() -> Dictionary:
	return {"setup_reset":setup_reset,"bank_poll":bank_poll,"bank_start":bank_start,
		"hud_read_start":hud_read_start,"cd_sync":cd_sync,"calendar_upload":calendar_upload,
		"mascot_set":mascot_set,"command_build":command_build,"room_finish":room_finish,
		"frame_draw":frame_draw,"outer_draw":outer_draw}


static func _reload_frame(state: State, step: int) -> bool:
	return state.game_mode == MODE and state.mode_stage == STAGE_RELOAD and state.mode_step == step


static func record(state: State) -> Dictionary:
	var value: Variant = state.opening.get(RECORD)
	return value if value is Dictionary else {}


## settings_command_screen_setup_reset: draw2d3d(1, 0), actor_draw_enable(1),
## sprite_tables_clear, actor_clear_upper, tile_layer_enabled = 0,
## boot_hold = 0, set_dec_bri(0), text_system_reset, k_disp_start(2),
## mode_step_next.
func setup_reset(state: State, result: Session.FrameResult) -> void:
	if not _reload_frame(state, 0) or state.opening.has(RECORD):
		result.error = "settings_command_screen_setup_reset runs once, at 0x34 stage 0xFF step 0"
		return
	state.opening[RECORD] = {"setup":{"draw2d3d":[1, 0],"actor_draw_enable":1,"sprite_tables_clear":true,
		"actor_clear_upper":true,"tile_layer_enabled":0,"boot_hold":0,"dec_bg_brightness":0,
		"text_system_reset":true,"k_disp_start":2},"calendar":{},"mascot":{},"room":{},"built":false}
	state.next_step()


## audio_bank_load_poll for the stage21 bank (media.finish_bank_poll).
func bank_poll(state: State, result: Session.FrameResult) -> int:
	if record(state).is_empty():
		result.error = "bank poll precedes settings_command_screen_setup_reset"
		return -1
	return int(bank_poll_owner.call(state, result))


func bank_start(state: State, bank: int, sequence: int, result: Session.FrameResult) -> void:
	bank_start_owner.call(state, bank, sequence, result)


## cd_read_sectors(0x95, 0x801A0000, 0x9B94).
func hud_read_start(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	media.queue.command_hud_read(state, sectors, destination, lba, result)


## cd_read_sync_status: bit0 once every queued read has landed and the
## logic-frame latency of the last one has elapsed.
func cd_sync(state: State, result: Session.FrameResult) -> int:
	return media.queue.cd_sync(state, result)


## calendar_month_load(year, month, 0): the month calendar from the CALROM
## bytes at 0x801A0000. Requires the completed read whose bytes landed there.
func calendar_upload(state: State, year: int, month: int, side: int, result: Session.FrameResult) -> void:
	var month_index: int = (year - 95) * 12 + month - 4
	# hud_screens.c calendar_month_load: LoadSquare only when month_data_index < 35
	# (it uploads months index and index + 1); index 35 loads nothing.
	if side != CALENDAR_SIDE or month < 1 or month > 12 or month_index < 0 or month_index >= CALENDAR_MONTH_LIMIT:
		result.error = CALENDAR_INDEX_UNSUPPORTED
		return
	var data := record(state)
	var request := Queue.completed_request(state, "hud_pending", "hud")
	var landed := media.buffer_identity(Reload.HUD_READ_DEST)
	var expected: Dictionary = media._command_reload.description("hud_calendar")
	if data.is_empty() or request.is_empty() or landed.get("resource_id") != expected.get("id") or landed.get("sha256") != expected.get("sha256"):
		result.error = "calendar_month_load has no completed CALROM read at 0x801A0000"
		return
	data["calendar"] = {"year":year,"month":month,"side":side,"month_index":month_index,"source_sha256":landed.sha256}


## hud_mascot_set(choice) then hud_mascot_display_switch(display), plus the
## same frame's handler tail (home_screens.c settings_command_screen_mode_handler:
## `if (*D_8011F3FF & 0x80) hud_mascot_update();`). display_switch(1) sets that
## bit, so hud_mascot_update runs on this frame; with choice 0 it calls
## hud_mascot_set(0) -> hud_mascot_display_switch(0) and clears the bit again
## (hud_screens.c). End state: display 0, as golden a6 D_8011F3FF = 0x00.
## A visible mascot (choice 1..13) is animated by hud_mascot_update every
## frame; that per-frame actor update is not modeled and is refused.
func mascot_set(state: State, choice: int, display: int, result: Session.FrameResult) -> void:
	var data := record(state)
	if data.is_empty() or choice < 0 or choice >= Reload.MASCOT_COUNT or display != 1:
		result.error = "hud_mascot_set is outside the reload calendar step"
		return
	if choice != 0:
		result.error = MASCOT_UPDATE_UNSUPPORTED
		return
	data["mascot"] = {"choice":choice,"display":0,"calls":[["hud_mascot_set", 0], ["hud_mascot_display_switch", 0],
		["hud_mascot_display_switch", display], ["hud_mascot_update"], ["hud_mascot_set", 0], ["hud_mascot_display_switch", 0]]}


## settings_command_screen_build, ordinary branch (Reload.build_command_screen),
## ending in settings_room_bg_load_start: dec_bg_cd_read(D_800B5950[season], 0).
## The slot-0 lookup and read run first: they read only the month and the LBA
## cache, which the build does not write, and a refused build restores the
## whole frame. On a miss the read is queued, bg_lba_cache[1 - dec_bg_show[0]]
## takes the LBA and r -1 gives the build's mode_step_next.
func command_build(state: State, result: Session.FrameResult) -> void:
	var data := record(state)
	if data.is_empty() or not _reload_frame(state, 2):
		result.error = "settings_command_screen_build runs at 0x34 stage 0xFF step 2"
		return
	var cache: Variant = state.rule_state.get("background_cache")
	var show: Variant = state.rule_state.get("background_show")
	if not cache is Array or (cache as Array).size() != 4 or not show is int or int(show) not in [0, 1]:
		result.error = "dec_bg_cd_read needs the owned bg_lba_cache and dec_bg_show[0]"
		return
	var lba: int = CommandReload.room_lba(HudCompositor.hud_season_index(int(state.rule_state.get("month", 0))))
	if lba < 0:
		result.error = "D_800B5950 has no room for this month"
		return
	if lba == int(cache[0]) or lba == int(cache[1]):
		result.error = CACHE_HIT_UNSUPPORTED
		return
	media.queue.room_read(state, lba, result)
	if not result.error.is_empty():
		return
	var next_cache: Array = (cache as Array).duplicate()
	next_cache[1 - int(show)] = lba
	state.rule_state["background_cache"] = next_cache
	data["room"] = {"lba":lba,"slot":ROOM_SLOT,"read_frame":state.frame_count}
	result.error = Reload.build_command_screen(state)
	if result.error.is_empty():
		data["built"] = true


## settings_room_bg_load_finish: on cd_read_sync_status & 1,
## dec_bg_show_set(0, dec_bg_decode_to_vram(0)); decode writes buffer
## 1 - dec_bg_show[0]; then set_dec_bri(0x80), screen_brightness 0x80 and
## mode_stage_set(1).
func room_finish(state: State, result: Session.FrameResult) -> void:
	var data := record(state)
	if data.is_empty() or not _reload_frame(state, 3) or not data.get("room") is Dictionary or data.room.is_empty():
		result.error = "settings_room_bg_load_finish has no room read from the build"
		return
	var status: int = media.queue.cd_sync(state, result)
	if not result.error.is_empty() or (status & 1) == 0:
		return
	var request := Queue.completed_request(state, "room_pending", "room")
	var landed := media.buffer_identity(ROOM_DESTINATION)
	var expected: Dictionary = media._command_reload.description("room_spring")
	if request.is_empty() or int(request.payload.lba) != int(data.room.lba) or landed.get("resource_id") != expected.get("id") \
			or landed.get("sha256") != expected.get("sha256") or landed.get("primary_sha256") != room_original_sha256:
		result.error = DECODE_FAILURE_UNSUPPORTED
		return
	var show: int = int(state.rule_state["background_show"])
	var buffer: int = 1 - show
	state.rule_state["background_show"] = buffer
	data.room["buffer"] = buffer
	data.room["decoded_sha256"] = landed.sha256
	data.room["dec_bg_brightness"] = FULL_BRIGHTNESS
	Reload.finish_room_load(state)


## hizuke_show, message_window_show, parameter_show (and from step 2
## message_window_title_update): draw-only on this route; the renderer reads
## the committed state.
func frame_draw(state: State, _title_update: bool, result: Session.FrameResult) -> void:
	if record(state).is_empty():
		result.error = "reload frame draw precedes settings_command_screen_setup_reset"


## settings_command_screen_mode_handler tail hud_screen_frame_draw(0): the
## checker scroll D_800B5BCC/D_800B5BD0 and the season fade continue on the
## prologue HUD record. D_800B5BC8 is opening.D_800B5BC8 (stored 0xFE by the
## calendar step); the prologue record's checker_fade mirrors the same global
## and is synchronised before the step. The handoff steps opening.D_800B5BC8
## itself after the handler (FreshHandoff.hud_screen_fade).
func outer_draw(state: State, result: Session.FrameResult) -> void:
	if record(state).is_empty():
		result.error = "reload outer draw precedes settings_command_screen_setup_reset"
		return
	var live: Variant = state.opening.get("prologue_hud_live")
	if not live is Dictionary:
		result.error = "hud_screen_frame_draw(0) needs the prologue HUD's checker globals"
		return
	var projection: Dictionary = projection_owner.call(state)
	if state.opening.get("D_800B5BC8") is int:
		projection.opening.prologue_hud_live["checker_fade"] = int(state.opening.D_800B5BC8)
	var out: Dictionary = {}
	result.error = hud_provider.call("advance_outer", projection, rng_owner.call(), out, 0)
	if result.error.is_empty():
		state.opening["prologue_hud_live"] = projection.opening.prologue_hud_live
