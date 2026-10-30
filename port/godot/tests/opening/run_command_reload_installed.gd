extends "res://tests/opening/run_fresh_gameplay_handoff.gd"
## T-39002: the mode 0x34 command reload with the installed app's production
## services (services/command_reload_services.gd) and renderer
## (presentation/command_reload_renderer.gd). Bounded test, not route
## acceptance: the installed route is run separately.
##
## Real data: the installed runtime bundle (NAME_ENT rules/content, prologue
## text, media/board/finish-bank stores, prologue HUD, command HUD manifest),
## the command reload reads (CALROM and the spring room, exported by
## tools/port/command_reload_export.py) and the golden captures
## (newgame-golden-v1 set A: logic.jsonl, a5/a6 decoded globals, a6 frame).
## Every CD read goes through the production media queue and bridge with the
## logic-frame latency model; completions come from the bridge's publication.
##
## Test inputs (not production): the switch state comes from the production
## Entry/Adapter and stage 21 with recording services (as
## run_fresh_gameplay_handoff.gd); the bank poll is a recording owner that
## completes on golden poll 131 (the native bank0 transfer needs the native
## audio library); the prologue HUD checker globals at the switch are the
## golden a5 values D_800B5BCC 133 / D_800B5BD0 189 and its panels the stage 21
## flags (message window on); the media owner's bg_lba_cache buffers at the
## switch are golden a1-a5 [0, 0, 0x4973, 0x496A]. The HUD session is the
## boot-only one the app builds; its menu 0 and cursor differ from golden a5
## (NAME_ENT's file select menu), which the handoff reports as hud_unsourced
## until the build, and the test checks against a5 and a6.
##
## Environment: TOKIMEMO_INSTALLED_ROOT (installed runtime bundle),
## TOKIMEMO_COMMAND_RELOAD_PACK (command reload reads),
## TOKIMEMO_GOLDEN (.asset-cache/emu/newgame-golden-v1).

const Profile = preload("res://services/installed_launch_profile.gd")
const MediaStore = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const CommandReload = preload("res://services/command_reload_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const ProloguHud = preload("res://presentation/prologue_hud_source_provider.gd")
const Services = preload("res://services/command_reload_services.gd")
const Renderer = preload("res://presentation/command_reload_renderer.gd")

const GOLDEN_A5_SCROLL := [133, 189]
const GOLDEN_A6_SCROLL := [102, 77]
const GOLDEN_A6_CACHE := [0, 0x3fe9, 0x4973, 0x496a]
const GOLDEN_MASCOT := 0
const HUD_READ_FRAME := 132
## actor_pool[0].x / .y (cursor_x / cursor_y) and D_8011F3FF[0] in the golden RAM.
const RAM_CURSOR_X := 0x11ECF6
const RAM_CURSOR_Y := 0x11ECFA
const RAM_MASCOT_FLAGS := 0x11F3FF
const ROOM_READ_FRAME := 206

var pack_root := ""
var golden_root := ""
var profile: Dictionary = {}
var media: Media
var provider: ProloguHud
var renderer: Renderer
var services_owner: Services
var bank_polls: Array[int] = [0]
var bank_starts: Array = []
var hover_texels_open := false


func _initialize() -> void:
	installed_root = OS.get_environment("TOKIMEMO_INSTALLED_ROOT").simplify_path()
	pack_root = OS.get_environment("TOKIMEMO_COMMAND_RELOAD_PACK").simplify_path()
	golden_root = OS.get_environment("TOKIMEMO_GOLDEN").simplify_path()
	var failure: String = _load_packs()
	if failure.is_empty():
		failure = _prepare_owners()
	if not failure.is_empty():
		printerr("FAIL " + failure)
		quit(1)
		return
	_store_negatives()
	_reload_route()
	_service_negatives()
	if profile.paths.has("first_arrival_command_reload"):
		_app_prepare()
	check(sections == (["route", "negatives", "app"] if profile.paths.has("first_arrival_command_reload") else ["route", "negatives"]), "every section ran to its end: %s" % [sections])
	if failures != 0:
		printerr("FAIL: %d of %d command reload checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d command reload checks" % checks)
	quit(0)


func _prepare_owners() -> String:
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(installed_root.path_join("manifest.json")))
	profile = Profile.read_pack(installed_root, manifest if manifest is Dictionary else {})
	if profile.get("valid") != true:
		return "installed runtime: " + str(profile.get("message", ""))
	var roles: Dictionary = profile.role_manifests
	var store := MediaStore.new()
	var error: String = store.open(profile.paths.first_arrival_media, roles.first_arrival_media.sha256)
	var board := BoardStore.new()
	if error.is_empty(): error = board.open_pack(profile.paths.first_arrival_board, roles.first_arrival_board.sha256)
	var finish := FinishBank.new()
	if error.is_empty(): error = finish.open_pack(profile.paths.first_arrival_finish_bank)
	var reload := CommandReload.new()
	# The installed role when the bundle has it, else the exported pack.
	if profile.paths.has("first_arrival_command_reload"):
		pack_root = profile.paths.first_arrival_command_reload
	if error.is_empty(): error = reload.open_pack(pack_root)
	media = Media.new()
	if error.is_empty(): error = media.prepare(store, board, finish, reload)
	provider = ProloguHud.new()
	if error.is_empty(): error = provider.open_installed(installed_root, roles.prologue_hud.relative, roles.prologue_hud.sha256)
	if error.is_empty(): error = provider.snapshot_runtime_assets()
	renderer = Renderer.new()
	var hud_capability := {"manifest":profile.paths.hud_manifest,
		"sha256":profile.file_identities.hud_manifest.sha256,"font_root":profile.paths.font_root}
	# A bundle with the command_hover_texels role draws the reload's hover
	# pulse through the func_80098680 path instead of the flagged approximation.
	if profile.paths.has("command_hover_texels"):
		hud_capability["hover_texels"] = {"path":roles.command_hover_texels.path, "sha256":roles.command_hover_texels.sha256}
		hover_texels_open = true
	if error.is_empty(): error = renderer.prepare(hud_capability, provider)
	services_owner = Services.new()
	if error.is_empty(): error = services_owner.prepare(media, provider, _bank_poll, _bank_start, _projection,
		func() -> Object: return null, renderer.room_original_sha256())
	return error


## The command reload reader refuses tampered packs.
func _store_negatives() -> void:
	var store := CommandReload.new()
	check(store.open_pack(pack_root).is_empty() and store.identity() == CommandReload.TRACK_SHA, "command reload pack opens: CALROM and room reads authenticated")
	check(store.description("hud_calendar").get("lba") == 0x9b94 and store.description("hud_calendar").get("sectors") == 0x95 and store.description("hud_calendar").get("destination") == 0x801a0000, "CALROM read is cd_read_sectors(0x95, 0x801A0000, 0x9B94)")
	check(store.description("room_spring").get("lba") == 0x3fe9 and store.description("room_spring").get("sectors") == 10 and CommandReload.room_lba(0) == 0x3fe9, "spring room read is D_800B5950[0] = 0x3FE9, 10 sectors")
	check(store.open_pack(pack_root) != "", "a reader keeps its first pack")
	var tmp := OS.get_user_data_dir().path_join("command-reload-negative")
	DirAccess.make_dir_recursive_absolute(tmp.path_join("reads"))
	for name: String in ["hud_calendar", "room_spring"]:
		var bytes := FileAccess.get_file_as_bytes(pack_root.path_join("reads/%s.bin" % name))
		if name == "room_spring":
			bytes[100] = bytes[100] ^ 1
		var out := FileAccess.open(tmp.path_join("reads/%s.bin" % name), FileAccess.WRITE)
		out.store_buffer(bytes)
		out.close()
	var manifest := FileAccess.open(tmp.path_join(CommandReload.MANIFEST), FileAccess.WRITE)
	manifest.store_string(FileAccess.get_file_as_string(pack_root.path_join(CommandReload.MANIFEST)))
	manifest.close()
	check(CommandReload.new().open_pack(tmp).begins_with("command reload sector read is missing or changed"), "a changed room sector is refused")
	var text := FileAccess.get_file_as_string(pack_root.path_join(CommandReload.MANIFEST)).replace("16361,", "16362,")
	manifest = FileAccess.open(tmp.path_join(CommandReload.MANIFEST), FileAccess.WRITE)
	manifest.store_string(text)
	manifest.close()
	check(CommandReload.new().open_pack(tmp) != "", "a changed D_800B5950 table or LBA is refused")


func _bank_poll(state: RouteState, _result: Session.FrameResult) -> int:
	check(state.mode_step == 1 and state.substep == 0, "bank poll only in reload_calendar substep 0")
	bank_polls[0] += 1
	return 1 if bank_polls[0] >= GOLDEN_BANK_POLLS else -1


func _bank_start(_state: RouteState, bank: int, sequence: int, _result: Session.FrameResult) -> void:
	bank_starts.append([bank, sequence])


func _projection(state: RouteState) -> Dictionary:
	var flags: int = state.rule_state.player_flags
	var opening_copy: Dictionary = state.opening.duplicate(false)
	if opening_copy.get("prologue_hud_live") is Dictionary:
		opening_copy["prologue_hud_live"] = opening_copy.prologue_hud_live.duplicate(true)
	return {"game_mode":state.game_mode,"mode_stage":state.mode_stage,"mode_step":state.mode_step,"substep":state.substep,
		"frame_count":state.frame_count,"player_flags":PackedByteArray([flags & 255, (flags >> 8) & 255]),
		"pad_held_triangle":false,"opening":opening_copy}


func _golden_timeline() -> Array:
	var rows: Array = []
	var file := FileAccess.open(golden_root.path_join("runs/setA/logic.jsonl"), FileAccess.READ)
	if file == null:
		return rows
	var started := false
	while not file.eof_reached():
		var line := file.get_line()
		if line.is_empty():
			continue
		var row: Variant = JSON.parse_string(line)
		if not row is Dictionary:
			continue
		if int(row.m) == 0x34:
			started = true
		if started:
			rows.append([int(row.s), int(row.st), int(row.ss)])
		if rows.size() > GOLDEN_RELOAD_FRAMES:
			break
	return rows


func _reload_route() -> void:
	var golden: Array = _golden_timeline()
	check(golden.size() == GOLDEN_RELOAD_FRAMES + 1, "golden logic.jsonl gives the 0x34 timeline (%d rows)" % golden.size())
	var fresh: Array = _fresh_switch()
	var session: Session = fresh[0]
	var state: RouteState = fresh[2]
	check(media.bind_session(session).is_empty(), "media bridge binds the opening session")
	# Test inputs at the switch (see the header).
	state.opening["names_media"] = {"buffer_0":{"lba":0x4973},"buffer_1":{"lba":0x496a}}
	var live := {"year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"title_timer":184,"title_visible":false,
		"date_visible":false,"message_visible":true,"checker_fade":0,"screen_brightness":0x80}
	var imported: Dictionary = provider.import_scroll(live, {"owner":"test:golden-a5","source_globals":["D_800B5BCC","D_800B5BD0"],
		"phase":"after_hud_screen_frame_draw_scroll_update","u":GOLDEN_A5_SCROLL[0],"v":GOLDEN_A5_SCROLL[1]})
	check(imported.get("status") == "scroll_imported", "golden a5 checker globals imported")
	state.opening["prologue_hud_live"] = imported.get("hud", {})
	var hud: HudSession = _boot_hud()
	var controller := Controller.new(rule_tables)
	var handoff := Handoff.new()
	check(handoff.open(state, hud, positions, controller).is_empty(), "handoff opens at the switch: " + handoff.error)
	_check_switch_hud(hud, handoff)
	check(state.rule_state.get("background_cache") == [0, 0, 0x4973, 0x496a] and state.rule_state.get("background_show") == 0, "bg_lba_cache/dec_bg_show[0] at the switch (golden a5)")
	check(handoff.bind(session, services_owner.services()).is_empty(), "handoff binds the production reload services")
	session.draw_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		if Renderer.reload_frame(base):
			result.error = hud.advance_icon_animation(renderer.icon_sequences(), base.rule_state.icon_enabled)
	session.cd_logic = func(base: Variant, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = media.preflight(result, base)
	session.service_completion = func(event: Session.ServiceEvent, base: Variant, rng: Rng, result: Session.FrameResult) -> void:
		media.complete(event, base, rng, result)
	var timeline: Array = []
	var mismatches: Array[String] = []
	var hud_reads: Array[int] = []
	var room_reads: Array[int] = []
	var calendar_frame := -1
	var phases: Dictionary = {}
	var unsourced_frames := 0
	var cursor_omitted := 0
	var approximated_pulse := 0
	var sourced_from := -1
	var images: Array[Image] = []
	var frame := 0
	while handoff.phase == "reload" and frame < 300:
		timeline.append([state.mode_stage, state.mode_step, state.substep])
		var tick: int = session.clock.logic_iterations
		var events: Array[Session.ServiceEvent] = media.pending_events(tick)
		var result: Session.FrameResult = session.step(InputFrame.new(tick, 0, session.latched_input.held), events)
		check(result.committed, "reload frame %d commits: %s" % [frame, result.error])
		if not result.committed:
			break
		var published: String = media.publish(result, tick)
		check(published.is_empty(), "reload frame %d media publication: %s" % [frame, published])
		for intent: Dictionary in result.presentation:
			if intent.get("kind") == "opening_media_request":
				if intent.request.kind == "hud": hud_reads.append(frame)
				if intent.request.kind == "room": room_reads.append(frame)
		if calendar_frame < 0 and not Services.record(state).get("calendar", {}).is_empty():
			calendar_frame = frame
		if sourced_from < 0 and handoff.hud_unsourced.is_empty():
			sourced_from = frame
		var drawn: Dictionary = renderer.render(state, hud, _projection(state), handoff.hud_unsourced)
		if not str(drawn.get("error", "")).is_empty():
			mismatches.append("frame %d render: %s" % [frame, drawn.error])
		else:
			images.append(drawn.image)
			phases[drawn.provenance.phase] = int(phases.get(drawn.provenance.phase, 0)) + 1
			if drawn.provenance.get("unsourced") == [Handoff.HUD_MENU_UNSOURCED]:
				unsourced_frames += 1
			if drawn.provenance.get("omitted") == ["cursor actor 0"]:
				cursor_omitted += 1
			if Renderer.SCALED in drawn.provenance.get("unverified", []):
				approximated_pulse += 1
		frame += 1
	timeline.append([state.mode_stage, state.mode_step, state.substep])
	for index in range(mini(timeline.size(), golden.size())):
		if timeline[index] != golden[index]:
			mismatches.append("frame %d: port %s golden %s" % [index, timeline[index], golden[index]])
	check(mismatches.is_empty(), "every reload frame follows the golden logic.jsonl timeline and renders: %s" % [mismatches.slice(0, 4)])
	if hover_texels_open:
		check(approximated_pulse == 0, "with the hover texels no reload frame approximates the hover pulse (%d did)" % approximated_pulse)
	check(frame == GOLDEN_RELOAD_FRAMES and handoff.phase == "command_select", "command select after the golden 225 frames (got %d, %s)" % [frame, handoff.phase])
	check(hud_reads == [HUD_READ_FRAME] and room_reads == [ROOM_READ_FRAME], "CALROM read on frame 132, room read on frame 206 (got %s, %s)" % [hud_reads, room_reads])
	check(calendar_frame == 205, "calendar_month_load after 73 CD syncs, on frame 205 (got %d)" % calendar_frame)
	check(bank_polls[0] == GOLDEN_BANK_POLLS and bank_starts == [[0, 0]], "131 bank polls, then audio_bank_start(0, 0)")
	var data: Dictionary = Services.record(state)
	check(data.get("calendar", {}).get("year") == 95 and data.calendar.get("month") == 4 and data.calendar.get("month_index") == 0, "calendar_month_load(95, 4, 0)")
	var a6_ram := FileAccess.get_file_as_bytes(golden_root.path_join("runs/setA/a6_mode34_stage1_step0_first_pre.ram.bin"))
	check(a6_ram.size() == 0x200000 and a6_ram[RAM_MASCOT_FLAGS] == 0, "golden a6 D_8011F3FF[0] is 0x00")
	check(data.get("mascot", {}).get("choice") == GOLDEN_MASCOT and data.mascot.get("display") == 0 \
		and data.mascot.get("calls", []).slice(2) == [["hud_mascot_display_switch", 1], ["hud_mascot_update"], ["hud_mascot_set", 0], ["hud_mascot_display_switch", 0]] \
		and int(state.opening.get("hud_mascot_choice", -1)) == GOLDEN_MASCOT,
		"hud_mascot_set(0), display_switch(1), then the handler-tail hud_mascot_update clears the display bit on the same frame (golden a6 D_8011F3FF 0): %s" % [data.get("mascot")])
	check(state.rule_state.background_cache == GOLDEN_A6_CACHE and state.rule_state.background_show == 1 and data.room.get("buffer") == 1, "bg_lba_cache [0, 0x3FE9, 0x4973, 0x496A], dec_bg_show[0] 1 (golden a6)")
	check(media.buffer_identity(0x801a0000).get("sha256") == CommandReload.READS[0][5] and media.buffer_identity(0x80162000).get("sha256") == CommandReload.READS[1][5], "CALROM bytes at 0x801A0000 and the room read at 0x80162000")
	var live_after: Dictionary = state.opening.prologue_hud_live
	check([live_after.checker_scroll_u, live_after.checker_scroll_v] == GOLDEN_A6_SCROLL, "hud_screen_frame_draw(0) scroll after 225 frames %s (golden a6 %s)" % [[live_after.checker_scroll_u, live_after.checker_scroll_v], GOLDEN_A6_SCROLL])
	check(state.game_mode_timer == GOLDEN_A7_GAME_MODE_TIMER and int(state.opening.D_800B5BC8) == 0 and int(state.opening.screen_brightness) == 0x80 and state.opening.command_ready == true, "game_mode_timer 226, D_800B5BC8 0, brightness 0x80 at command select")
	check(phases.get("before_build", 0) == 206 and phases.get("built", 0) == 19, "206 frames before the build, 19 after (got %s)" % [phases])
	check(unsourced_frames == 206 and cursor_omitted == 206 and sourced_from == ROOM_READ_FRAME and handoff.hud_unsourced.is_empty(),
		"the 206 pre-build frames are marked unsourced and omit the cursor; the build frame %d clears the marker (got %d, %d, %d)" % [ROOM_READ_FRAME, unsourced_frames, cursor_omitted, sourced_from])
	_check_a6_hud(hud)
	_compare_a6(images)
	sections.append("route")


static func _ram_s16(ram: PackedByteArray, offset: int) -> int:
	var value: int = ram[offset] | (ram[offset + 1] << 8)
	return value - 0x10000 if value >= 0x8000 else value


static func _golden_menu0(decoded: Dictionary) -> Array:
	# The decoder lists menu_items[i][menu] interleaved: even rows are menu 0.
	var items: Array = decoded.get("game_state", {}).get("fields", {}).get("menu_items", [])
	var menu0: Array = []
	for i: int in range(0, items.size(), 2):
		var item: Dictionary = {}
		for key: String in ["x", "y", "w", "h", "hover", "disabled"]:
			item[key] = int(items[i].get(key, 0))
		menu0.append(item)
	return menu0


func _golden_decoded(name: String) -> Dictionary:
	var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(golden_root.path_join("runs/setA/%s.decoded.json" % name)))
	return value if value is Dictionary else {}


## HUD state at the switch against golden a5 (the frame before the first
## reload frame). The source's menu 0 is NAME_ENT's file_select_init_menus
## menu (file_select.c:827/836), which the port's entry does not run; the
## session driven from boot holds the icon bar instead. The handoff must say
## so (hud_unsourced); the parts it relies on must equal the golden values.
func _check_switch_hud(hud: HudSession, handoff: Handoff) -> void:
	var a5: Dictionary = _golden_decoded("a5_mode34_first_pre")
	var fields: Dictionary = a5.get("game_state", {}).get("fields", {})
	var ram := FileAccess.get_file_as_bytes(golden_root.path_join("runs/setA/a5_mode34_first_pre.ram.bin"))
	var menu0: Array = _golden_menu0(a5)
	check(not menu0.is_empty() and ram.size() == 0x200000, "golden a5 decoded globals and RAM are available")
	if menu0.is_empty() or ram.size() != 0x200000:
		return
	var golden_cursor := Vector2i(_ram_s16(ram, RAM_CURSOR_X), _ram_s16(ram, RAM_CURSOR_Y))
	check(int(fields.get("menu_count_0", -1)) == 14 and int(fields.get("menu_count_1", -1)) == 4 and golden_cursor == Vector2i(-89, 79), "golden a5: menu counts 14/4, cursor (-89, 79)")
	var file_select_first: bool = [menu0[0].x, menu0[0].y, menu0[0].w, menu0[0].h] == [-144, -18, 32, 16]
	check(file_select_first and [menu0[7].x, menu0[7].y] == [-112, -18], "golden a5 menu 0 is file_select_init_menus' 2x7 grid (x = col*32-144, y = row*16-18, 32x16)")
	var item0: Dictionary = hud.menu_items[0]
	check([item0.x, item0.y, item0.w, item0.h] != [menu0[0].x, menu0[0].y, menu0[0].w, menu0[0].h] and Vector2i(hud.cursor_x, hud.cursor_y) != golden_cursor,
		"the boot-driven session's menu 0 and cursor differ from golden a5 (NAME_ENT menu_set not mirrored)")
	check(handoff.hud_unsourced == Handoff.HUD_MENU_UNSOURCED, "the handoff marks the switch HUD menu 0 and cursor unsourced")
	var globals: Dictionary = a5.get("session_globals", {})
	check(hud.menu_cursor() == int(fields.get("menu_cursor_0", -1)) and hud.menu_mode == int(fields.get("menu_mode", -1)) \
		and hud.hover_last_index == int(globals.get("D_800B5A9C", {}).get("value", 99)) and hud.hover_countdown == int(globals.get("D_800B5AA0", {}).get("value", 99)),
		"menu_cursor_0 0, menu_mode 1 and D_800B5A9C/D_800B5AA0 -1/0 equal golden a5 (hover index and pulse are sourced)")


## HUD state after the reload against golden a6: the build rewrote menu 0 and
## placed the cursor, so the session equals the source again.
func _check_a6_hud(hud: HudSession) -> void:
	var a6: Dictionary = _golden_decoded("a6_mode34_stage1_step0_first_pre")
	var fields: Dictionary = a6.get("game_state", {}).get("fields", {})
	var ram := FileAccess.get_file_as_bytes(golden_root.path_join("runs/setA/a6_mode34_stage1_step0_first_pre.ram.bin"))
	var menu0: Array = _golden_menu0(a6)
	if menu0.size() < 14 or ram.size() != 0x200000:
		check(false, "golden a6 decoded globals and RAM are available")
		return
	var differing: Array = []
	for i: int in range(14):
		var port: Dictionary = hud.menu_items[i]
		var golden: Dictionary = menu0[i]
		for key: String in ["x", "y", "w", "h", "hover", "disabled"]:
			if int(port[key]) != int(golden[key]):
				differing.append("%d.%s %d/%d" % [i, key, port[key], golden[key]])
	check(differing.is_empty() and hud.menu_count == int(fields.get("menu_count_0", -1)) and hud.menu_cursor() == int(fields.get("menu_cursor_0", -1)),
		"menu 0 after the reload equals golden a6 (icon bar, hover on item 0): %s" % [differing.slice(0, 4)])
	var golden_cursor := Vector2i(_ram_s16(ram, RAM_CURSOR_X), _ram_s16(ram, RAM_CURSOR_Y))
	check(Vector2i(hud.cursor_x, hud.cursor_y) == golden_cursor, "cursor after the reload %s equals golden a6 %s" % [Vector2i(hud.cursor_x, hud.cursor_y), golden_cursor])


## The a6 checkpoint (first stage 1 dispatch) shows the scan-out buffer. The
## checker in a HUD-free area must equal one of the last two reload frames
## exactly; whole-frame and 16x16 tile differences are reported (the test's
## NAME_ENT name differs from the golden one).
const CHECKER_AREA := Rect2i(100, 80, 180, 80)

func _compare_a6(images: Array[Image]) -> void:
	var golden := Image.load_from_file(golden_root.path_join("runs/setA/a6_mode34_stage1_step0_first_pre.png"))
	check(images.size() == GOLDEN_RELOAD_FRAMES and golden != null and golden.get_size() == Vector2i(320, 240), "every reload frame rendered and the golden a6 capture is available")
	if images.size() < 2 or golden == null:
		return
	golden.convert(Image.FORMAT_RGB8)
	var exact: Array[int] = []
	for back: int in [1, 2]:
		var port: Image = images[images.size() - back].duplicate()
		port.convert(Image.FORMAT_RGB8)
		var bad_pixels := 0
		var bad_checker := 0
		var bad_tiles: Dictionary = {}
		for y in range(240):
			for x in range(320):
				var a: Color = port.get_pixel(x, y)
				var b: Color = golden.get_pixel(x, y)
				if (a.r8 >> 3) != (b.r8 >> 3) or (a.g8 >> 3) != (b.g8 >> 3) or (a.b8 >> 3) != (b.b8 >> 3):
					bad_pixels += 1
					bad_tiles[Vector2i(x >> 4, y >> 4)] = true
					if CHECKER_AREA.has_point(Vector2i(x, y)):
						bad_checker += 1
		print("a6 vs reload frame %d: %d of 76800 pixels, %d of 300 16x16 tiles, %d checker-area pixels differ (RGB555)" % [
			GOLDEN_RELOAD_FRAMES - back, bad_pixels, bad_tiles.size(), bad_checker])
		if bad_checker == 0:
			exact.append(GOLDEN_RELOAD_FRAMES - back)
		var out := OS.get_environment("TOKIMEMO_RELOAD_OUT")
		if not out.is_empty():
			port.save_png(out.path_join("reload-frame-%d.png" % (GOLDEN_RELOAD_FRAMES - back)))
	check(not exact.is_empty(), "the a6 checker equals a rendered reload frame exactly (frames %s)" % [exact])


## The installed app's reload preparation (app/first_arrival_boot.gd
## _prepare_reload) on the inventoried pack: launch resources open the
## command reload role and the HUD manifest identity, the renderer and the
## services prepare, and the HUD session runs process_image, game_state_init(1)
## and boot_hud_init's hud_icon_bar_init.
func _app_prepare() -> void:
	var Boot: GDScript = load("res://app/first_arrival_boot.gd")
	var boot: Object = Boot.new()
	var failure: String = boot.resources.prepare(profile)
	check(failure.is_empty(), "installed launch resources prepare: " + failure)
	check(boot.resources.command_reload != null and boot.resources.command_reload.identity() == CommandReload.TRACK_SHA, "launch resources open the first_arrival_command_reload role")
	check(boot.resources.hud_manifest.get("sha256") == profile.file_identities.hud_manifest.sha256, "the HUD manifest identity comes from the installed inventory")
	if failure.is_empty():
		failure = boot.media.prepare(boot.resources.media_store, boot.resources.board_store, boot.resources.finish_bank, boot.resources.command_reload)
		check(failure.is_empty(), "the app's media bridge prepares with the command reload reads: " + failure)
		failure = boot._prepare_reload()
		check(failure.is_empty(), "the app prepares the reload: " + failure)
		var session: HudSession = boot.hud_session
		check(session != null and session.trace().slice(0, 2) == ["process_image", "game_state_init(1)"] and not session.trace().has("command_screen_build"), "the app's HUD session starts at boot: %s" % [session.trace() if session != null else []])
		check(boot._icon_positions == positions, "the app's icon positions are the HUD manifest's")
		var refused := Session.FrameResult.new()
		boot._command_reload_unbound(null, refused)
		check(refused.error == boot.COMMAND_RELOAD_UNBOUND, "the opening's own 0x34 handler refuses the reload")
	boot.free()
	sections.append("app")


## Refusals of the production services: each leaves the frame to Reload.step,
## which restores the state.
func _service_negatives() -> void:
	var fresh: Array = _fresh_switch()
	var state: RouteState = fresh[2]
	var result := Session.FrameResult.new()
	services_owner.setup_reset(state, result)
	check(result.error.is_empty() and state.mode_step == 1 and Services.record(state).get("setup", {}).get("k_disp_start") == 2, "setup_reset records its source calls and steps")
	result = Session.FrameResult.new()
	services_owner.setup_reset(state, result)
	check(not result.error.is_empty(), "setup_reset runs once, at step 0")
	result = Session.FrameResult.new()
	services_owner.calendar_upload(state, 95, 4, 0, result)
	check(result.error.begins_with("calendar_month_load has no completed CALROM read"), "calendar_month_load needs the landed CALROM read: " + result.error)
	result = Session.FrameResult.new()
	services_owner.calendar_upload(state, 98, 3, 0, result)
	check(result.error == Services.CALENDAR_INDEX_UNSUPPORTED, "calendar_month_load(98, 3) (month_data_index 35, skipped by the source's < 35 guard) is refused")
	result = Session.FrameResult.new()
	services_owner.mascot_set(state, 1, 1, result)
	check(result.error == Services.MASCOT_UPDATE_UNSUPPORTED and Services.record(state).get("mascot") == {}, "a visible mascot's per-frame hud_mascot_update is refused")
	result = Session.FrameResult.new()
	services_owner.hud_read_start(state, 0x95, 0x801a0000, 0x9b94, result)
	check(result.error.begins_with("command HUD read is outside"), "the CALROM read is refused outside reload_calendar substep 1")
	state.substep = 1
	result = Session.FrameResult.new()
	services_owner.hud_read_start(state, 0x95, 0x801a0000, 0x9b95, result)
	check(result.error == "source-backed opening command HUD is unavailable", "a CALROM read at another LBA is refused")
	result = Session.FrameResult.new()
	services_owner.room_finish(state, result)
	check(not result.error.is_empty(), "room_finish needs the build's room read")
	state.substep = 0
	state.next_step()
	state.rule_state["background_cache"] = [0x3fe9, 0, 0x4973, 0x496a]
	state.rule_state["background_show"] = 0
	result = Session.FrameResult.new()
	services_owner.command_build(state, result)
	check(result.error == Services.CACHE_HIT_UNSUPPORTED and state.mode_step == 2, "a slot-0 cache hit is refused before the build")
	state.rule_state.erase("background_cache")
	result = Session.FrameResult.new()
	services_owner.command_build(state, result)
	check(result.error.begins_with("dec_bg_cd_read needs the owned bg_lba_cache"), "the build needs the owned bg_lba_cache")
	var other := Services.new()
	check(other.prepare(media, provider, _bank_poll, _bank_start, _projection, func() -> Object: return null, "0".repeat(64)) != "", "services refuse a HUD room image decoded from another file")
	state.mode_step = 0 # the switch frame, before setup_reset
	var drawn: Dictionary = renderer.render(state, _boot_hud(), _projection(state))
	check(str(drawn.get("error", "")).begins_with("command reload renderer needs a committed reload frame"), "the renderer refuses a frame outside the reload")
	sections.append("negatives")
