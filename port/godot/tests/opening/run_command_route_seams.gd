extends SceneTree
## T-39002 bounded regressions for the stops the actual installed New Game
## route (tests/opening/run_installed_command_route.gd) hit between stage 12
## and command select. Each check pins one source rule; no packs needed.
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const Handoff = preload("res://flow/fresh_gameplay_handoff.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failed := 0

func check(value: bool, message: String) -> void:
	checks += 1
	if not value:
		failed += 1
		printerr("FAIL: " + message)

func _initialize() -> void:
	_hud_late_stages()
	_finish_bank_role()
	_command_build()
	_closing_draw_scope()
	_handoff_background_cache()
	_xa_behind_read()
	print(("PASS: %d command route seam checks" % checks) if failed == 0 else "FAIL: %d of %d command route seam checks" % [failed, checks])
	quit(1 if failed else 0)

func _late(stage: int, step: int, opening: Dictionary) -> Dictionary:
	return {"game_mode":21,"mode_stage":stage,"mode_step":step,"substep":0,"opening":opening}

## prologue.c:2812-2817: every stage but 0 ends with hud_screen_frame_draw(2).
func _hud_late_stages() -> void:
	var live := {"D_800B3DB0":1,"prologue_hud_live":{},"board_media":{"read_requested":true}}
	for stage in range(12,22):
		check(Hud.source_frame_argument(_late(stage,0,live)) == 2,"stage %d outer HUD argument is not 2" % stage)
		check(Hud.source_frame_argument(_late(stage,1,live)) == -1,"stage %d accepted a mode_step its stage guard never runs" % stage)
	check(Hud.source_frame_argument(_late(13,0,{"D_800B3DB0":0,"prologue_hud_live":{},"board_media":{"read_requested":true}})) == -1,"late stage accepted a HUD the board stage never started")
	check(Hud.source_frame_argument(_late(14,0,{"D_800B3DB0":1,"board_media":{"read_requested":true}})) == -1,"late stage accepted a missing live HUD")
	check(Hud.source_frame_argument(_late(22,0,live)) == -1,"stage 22 is outside prologue_mode_handler")
	# prologue_finish_run case1: the switch frame's tail is still the prologue's.
	var switched := {"D_800B3DB0":0,"D_800B5BD4":0x0F,"prologue_hud_live":{},"board_media":{"read_requested":true}}
	var frame := {"game_mode":0x34,"mode_stage":0xFF,"mode_step":0,"substep":0,"opening":switched}
	check(Hud.source_frame_argument(frame) == 2,"stage21 switch frame lost its hud_screen_frame_draw(2)")
	frame.mode_step = 1
	check(Hud.source_frame_argument(frame) == -1,"a command reload frame was given the prologue HUD")
	frame.mode_step = 0; switched.D_800B5BD4 = 0x10
	check(Hud.source_frame_argument(frame) == -1,"switch frame accepted a checker OT slot stage21 never set")

## prologue_finish_run case 0's five reads are an installed directory role
## whose manifest is the store's finish_bank.json.
func _finish_bank_role() -> void:
	check("first_arrival_finish_bank" in Profile.DIR_ROLES,"finish bank is not an installed directory role")
	check(Profile.ROLE_MANIFESTS.get("first_arrival_finish_bank") == "finish_bank.json","finish bank role manifest is not finish_bank.json")
	check(not "first_arrival_finish_bank" in Profile.OPENING_RESOURCE_ROLES,"finish bank must stay optional for the opening resources")

## settings_command_screen_build ordinary branch on the fresh 1995-04-04 state:
## golden a7 icon_enabled 01010101 00010101 00000001 01000000 and every
## menu_items[i].disabled 0 (.asset-cache/emu/newgame-golden-v1).
func _command_build() -> void:
	var s := RouteState.new()
	s.game_mode = 0x34; s.mode_stage = 0xFF; s.mode_step = 2
	s.rule_state = Handoff.boot_records()
	s.rule_state["icon_enabled"] = Handoff.boot_icon_enabled()
	s.rule_state["girls"] = [{"flags":1}]
	s.opening = {"icon_mode":0,"icon_count":14}
	var error := Reload.build_command_screen(s)
	check(error.is_empty(),"ordinary build refused: "+error)
	check(s.rule_state.icon_enabled == [1,1,1,1,0,1,1,1,0,0,0,1,1,0,0],"build icons differ from golden a7: %s" % [s.rule_state.icon_enabled])
	check(s.rule_state.menu_disabled == Handoff._zeros(14),"build wrote menu_disabled; source menu_items[].disabled stay 0")
	check(s.rule_state.menu_cursor == 0 and s.mode_step == 3,"build cursor/step differ")
	var expected: Dictionary = Handoff.source_ordinary_build(Handoff.boot_records(),0,14,0)
	check(s.rule_state.icon_enabled == expected.icon_enabled,"Reload build differs from the handoff's source_ordinary_build")
	var sick := RouteState.new()
	sick.rule_state = Handoff.boot_records()
	sick.rule_state["player_flags"] = (0xB << 12) | (3 << 8)
	sick.rule_state["icon_enabled"] = Handoff.boot_icon_enabled()
	sick.opening = {"icon_mode":0}
	check(not Reload.build_command_screen(sick).is_empty(),"sick branch was built as the ordinary one")
	# hud_icon_bar_init: ICON_MODE_FREE sets icon_count 0xF (fifteen icons and
	# menu records), which is refused before any field is written.
	var free := RouteState.new()
	free.game_mode = 0x34; free.mode_stage = 0xFF; free.mode_step = 2
	free.rule_state = Handoff.boot_records()
	free.rule_state["icon_enabled"] = Handoff.boot_icon_enabled()
	free.rule_state["girls"] = [{"flags":1}]
	free.opening = {"icon_mode":2,"icon_count":14}
	var free_before := str([free.rule_state.get("icon_enabled"), free.rule_state.get("menu_disabled"), free.rule_state.get("menu_cursor")])
	var free_error := Reload.build_command_screen(free)
	check(free_error == Reload.ICON_MODE_FREE_UNSUPPORTED,"icon mode 2 was not refused explicitly: "+free_error)
	var free_after := str([free.rule_state.get("icon_enabled"), free.rule_state.get("menu_disabled"), free.rule_state.get("menu_cursor")])
	check(free.opening.icon_count == 14 and free.mode_step == 2 and free_after == free_before,"refused icon mode 2 build changed state")
	# TypeB keeps the earlier icon_count; only fourteen is modeled.
	var type_b := RouteState.new()
	type_b.game_mode = 0x34; type_b.mode_stage = 0xFF; type_b.mode_step = 2
	type_b.rule_state = Handoff.boot_records()
	type_b.rule_state["icon_enabled"] = Handoff.boot_icon_enabled()
	type_b.rule_state["girls"] = [{"flags":1}]
	type_b.opening = {"icon_mode":1,"icon_count":14}
	check(Reload.build_command_screen(type_b).is_empty() and type_b.rule_state.icon_enabled == s.rule_state.icon_enabled,"TypeB fourteen-icon build differs from TypeA")
	type_b.mode_step = 2
	type_b.opening = {"icon_mode":1,"icon_count":15}
	check(not Reload.build_command_screen(type_b).is_empty(),"kept icon_count 15 was built as fourteen")

## draw_frame owners for stages 18-21: the closing effect owns 18-20 and the
## 21/0 exit frame (line 42); stage 21's own body frames draw nothing.
func _closing_draw_scope() -> void:
	var s := RouteState.new()
	s.game_mode = 0x15; s.mode_step = 0
	for stage in [18,19,20]:
		s.mode_stage = stage
		check(ActorRenderer._closing_scope(s) and not ActorRenderer._finish_scope(s),"stage %d draw is not the closing owner's" % stage)
	s.mode_stage = 21; s.substep = 0; s.opening = {"prologue_line":42}
	check(ActorRenderer._closing_scope(s),"20->21 exit frame lost its closing owner")
	s.substep = 1
	check(not ActorRenderer._closing_scope(s) and ActorRenderer._finish_scope(s),"stage21 body frame is not the finish scope")
	s.mode_stage = 17; s.substep = 0
	check(not ActorRenderer._closing_scope(s) and not ActorRenderer._finish_scope(s),"stage17 was claimed by the closing owners")
	s.mode_stage = 19; s.mode_step = 1
	check(not ActorRenderer._closing_scope(s),"closing owner accepted mode_step 1")
	# prologue_finish_run draws no background (no dec_bg_draw in either case).
	var f := RouteState.new()
	f.game_mode = 0x15; f.mode_stage = 21; f.mode_step = 0; f.substep = 1; f.opening = {"prologue_line":42}
	check(Media.finish_frame(f) and not Media.closing_frame(f),"stage21 substep1 frame is not a finish frame")
	f.substep = 0
	check(not Media.finish_frame(f) and Media.closing_frame(f),"20->21 exit frame was taken from the closing composition")
	f.game_mode = 0x34; f.mode_stage = 0xFF
	check(Media.finish_frame(f) and ActorRenderer._finish_scope(f),"mode-switch frame lost its finish scope")
	f.mode_step = 1
	check(not Media.finish_frame(f) and not ActorRenderer._finish_scope(f),"a command reload frame was given the prologue finish scope")

## bg_lba_cache at the stage 21 switch comes from the media owner's layer-1
## buffers (golden a4: [0, 0, 0x4973, 0x496A], dec_bg_show[0] 0).
func _handoff_background_cache() -> void:
	var h := Handoff.new()
	var s := RouteState.new()
	s.rule_state = {"background_cache":[0,0,0,0],"background_show":0}
	s.opening = {"names_media":{"bg_show_1":0,"buffer_0":{"lba":0x4973},"buffer_1":{"lba":0x496a}}}
	h.state = s
	h.unsourced = {"background_cache":"x","background_show":"x","scene_work":"x"}
	h._take_background_cache()
	check(s.rule_state.background_cache == [0,0,0x4973,0x496a] and s.rule_state.background_show == 0,"handoff bg cache differs from the media buffers")
	check(not h.unsourced.has("background_cache") and not h.unsourced.has("background_show") and h.unsourced.has("scene_work"),"handoff kept the sourced cache unsourced or dropped scene_work")
	var partial := RouteState.new()
	partial.rule_state = {"background_cache":[0,0,0,0]}
	partial.opening = {"names_media":{"buffer_0":{"lba":0x4973}}}
	h.state = partial
	h.unsourced = {"background_cache":"x"}
	h._take_background_cache()
	check(partial.rule_state.background_cache == [0,0,0,0] and h.unsourced.has("background_cache"),"handoff sourced a cache without both owned buffers")

## audio_xa_play's Setloc+ReadS waits for an in-flight data read: golden
## stage 18 substep 1 lasts 44 frames (O_13 read 40 frames + 6), not 6.
func _xa_behind_read() -> void:
	var s := RouteState.new()
	s.frame_count = 1000
	var audio := {"clips":[{"xa_request":{"start_lba":279742,"end_lba":280190},"speaker":13}],"clip_index":0,"line_id":0x26}
	var r := Session.FrameResult.new()
	AudioRules._schedule_xa(s,audio,r)
	check(r.error.is_empty() and audio.xa_start_frame == 1006,"idle drive XA start differs from request + 6")
	s.opening["media"] = {"cd_ready_frame":1039}
	AudioRules._schedule_xa(s,audio,r)
	check(r.error.is_empty() and audio.xa_start_frame == 1045 and audio.xa_request_frame == 1000,"XA start did not wait for the in-flight read (got %s)" % str(audio.get("xa_start_frame")))
	s.opening["media"] = {"cd_ready_frame":990}
	AudioRules._schedule_xa(s,audio,r)
	check(audio.xa_start_frame == 1006,"a landed read delayed the XA start")
