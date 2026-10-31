extends RefCounted
## T-39002: frames of the mode 0x34 stage 0xFF command reload on the installed
## fresh New Game (home_screens.c settings_command_screen_mode_handler with
## settings_command_screen_mode_stage_reload), drawn from committed state only.
##
## Every reload frame: hud_screen_frame_draw(0) draws the full checker board
## (D_800B5BCC/D_800B5BD0 scroll continued on the prologue HUD record,
## prologue_hud_source_provider.draw_checker). settings_command_screen_setup_reset
## cleared the prologue text and upper actors and enabled actor drawing, and
## hud_icon_bar_update(0) flags the enabled icon actors 80 + i every frame, so
## the icon bar (actors 64 + i, 80 + i) and the cursor actor 0 are drawn from
## the HUD session from the first reload frame. dec_bg_draw is not called on
## any reload frame: the room is never drawn here.
##
## Before settings_command_screen_build (committed steps 1 and 2) the panels
## are the ones the prologue left visible (hizuke_show / message_window_show
## with the prologue HUD flags; parameter_disp is off). From the build on
## (step 3, and the frame that commits stage 1) the command HUD records are
## initialised (parameter_show_init, hizuke_init, message_window_init,
## parameter_disp_switch(1), sndisp of the ordinary narration) and the frame is
## the command HUD presenter's, without the room.
##
## Unverified, recorded in each frame's provenance: no golden capture exists
## between a5 (switch) and a6 (the last reload frame on screen); the k_disp
## typewriter reveal after the build is not modeled (the narration is drawn
## whole); hud_mascot is not rasterized; the hover pulse of icon 80 + i on
## reload frames 0..31 (rsin scale, func_80098680) is drawn by the
## compositor's scaled path when the installed HUD capability carries the
## hover texels (command_hover_texels role); without them it is a
## nearest-neighbour approximation, flagged in provenance.
##
## Unsourced: while the handoff reports hud_unsourced (the HUD session's menu 0
## and cursor at the switch are not NAME_ENT's, FreshHandoff.HUD_MENU_UNSOURCED),
## the cursor actor 0 is not drawn on the pre-build frames and the frame's
## provenance lists the reason. The icon bar is drawn: its hover index is
## menu_cursor_0 (menu_check_2 with menu_mode 1), which is sourced.

const State = preload("res://rules/new_game_route_state.gd")
const Presenter = preload("res://presentation/command_hud_presenter.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const LiveHud = preload("res://presentation/live_hud_adapter.gd")
const Services = preload("res://services/command_reload_services.gd")
const RoomStore = preload("res://services/command_reload_store.gd")

const NO_GOLDEN := "no golden capture between a5 and a6"
const TYPEWRITER := "k_disp reveal after the build not modeled (narration drawn whole)"
const MASCOT := "hud_mascot not rasterized"
const CHECKER_MODEL := "checker scroll from the frame_count model, not the continued prologue scroll record"
const SCALED := "hover pulse scale: nearest-neighbour approximation of func_80098680"

var presenter: Presenter
var provider: RefCounted
var error := ""


func prepare(hud_manifest: Dictionary, prologue_provider: RefCounted) -> String:
	if presenter != null:
		return "command reload renderer is already open"
	if prologue_provider == null or not prologue_provider.has_method("draw_checker") or not prologue_provider.has_method("draw_front"):
		return "command reload renderer needs the prologue HUD provider"
	for key: String in ["manifest", "sha256", "font_root"]:
		if not hud_manifest.get(key) is String or str(hud_manifest[key]).is_empty():
			return "command reload renderer needs the inventoried HUD manifest and font root"
	var candidate := Presenter.new()
	if not candidate.open_manifest(hud_manifest.manifest, hud_manifest.sha256, hud_manifest.font_root):
		return "command HUD manifest: " + candidate.error
	if hud_manifest.has("hover_texels"):
		var texels: Variant = hud_manifest.hover_texels
		if not texels is Dictionary or not texels.get("path") is String or not texels.get("sha256") is String:
			return "command HUD hover texels need the inventoried path and SHA-256"
		var texel_error: String = candidate.open_hover_texels(texels.path, texels.sha256)
		if not texel_error.is_empty():
			return "command HUD hover texels: " + texel_error
	presenter = candidate
	provider = prologue_provider
	return ""


## Original room file the HUD manifest decoded (provenance inputs.room).
func room_original_sha256() -> String:
	if presenter == null: return ""
	var inputs: Variant = presenter._manifest.get("provenance", {}).get("inputs", {})
	return str(inputs.get("room", {}).get("sha256", "")) if inputs is Dictionary and inputs.get("room") is Dictionary else ""


func icon_sequences() -> Array:
	return presenter._manifest["tables"]["sequences"] if presenter != null else []


static func reload_frame(state: State) -> bool:
	if state.game_mode != 0x34 or not state.opening.get(Services.RECORD) is Dictionary:
		return false
	return (state.mode_stage == 0xFF and state.mode_step in [1, 2, 3]) \
		or (state.mode_stage == 1 and state.mode_step == 0 and state.opening.get("command_ready") == true)


## {"error", "image", "provenance"} for the committed reload frame.
func render(state: State, hud: HudSession, projection: Dictionary, hud_unsourced: String = "") -> Dictionary:
	if presenter == null:
		return {"error":"command reload renderer is not prepared"}
	if not reload_frame(state) or hud == null or not hud.intact():
		return {"error":"command reload renderer needs a committed reload frame and its HUD session"}
	var data: Dictionary = Services.record(state)
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	image.fill(Color.BLACK)
	var failure: String = provider.call("draw_checker", image, projection)
	if not failure.is_empty():
		return {"error":failure}
	var unverified: Array[String] = [NO_GOLDEN, MASCOT]
	if data.get("built") == true:
		if not hud_unsourced.is_empty():
			return {"error":"command reload renderer: the HUD session is still unsourced after the build: " + hud_unsourced}
		var family_hex: String = LiveHud._family_hex(state.rule_state.get("profile_family_bytes"))
		var context: Dictionary = hud.hud_context(0x34, state.mode_stage, state.mode_step, family_hex)
		var view: Dictionary = presenter.create_state(state.rule_state, context)
		if view.is_empty():
			return {"error":presenter.error}
		failure = presenter.set_animation_phase(view, hud.icon_steps, hud.icon_elapsed)
		if not failure.is_empty():
			return {"error":failure}
		var drawn: Image = presenter.render(view, image, false)
		if drawn == null:
			return {"error":presenter.error}
		unverified.append(TYPEWRITER)
		return {"error":"", "image":drawn, "provenance":{"kind":"command_reload_frame","phase":"built","unverified":unverified}}
	failure = provider.call("draw_front", image, projection)
	if not failure.is_empty():
		return {"error":failure}
	var scaled := false
	var actors := _icon_actors(state, hud, hud_unsourced.is_empty())
	if not error.is_empty():
		return {"error":error}
	var compositor: HudCompositor = presenter._compositor
	compositor.error = ""
	compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	var scale: int = HudSession.hover_scale(hud.hover_countdown)
	for actor: Dictionary in actors:
		if actor.get("scale_hover", false) and scale != 0x1000:
			if compositor.hover_texels.has("%d:%s" % [actor["frame"], actor["variant"]]):
				actor["scale"] = scale
				compositor._draw_scaled_actor(image, actor)
			else:
				scaled = true
				_draw_scaled(image, actor, scale)
		else:
			compositor._draw_actor(image, actor)
		if not compositor.error.is_empty():
			return {"error":compositor.error}
	if scaled:
		unverified.append(SCALED)
	var provenance := {"kind":"command_reload_frame","phase":"before_build","unverified":unverified}
	if not hud_unsourced.is_empty():
		provenance["unsourced"] = [hud_unsourced]
		provenance["omitted"] = ["cursor actor 0"]
	return {"error":"", "image":image, "provenance":provenance}


## Gameplay frames after the handoff that still show the command screen:
## schedule_init (0x30, scheduler.c:40-73) and schedule_day_screen_handler
## (0x31, scheduler.c:1530-1541) draw parameter_show, message_window_show,
## hizuke_show, the icon bar (hud_icon_bar_update(0)), hud_mascot_update,
## dec_bg_draw(get_last_gamen_mode()) and hud_screen_frame_draw(1): the
## command select records, with the room while it is the shown background
## (CommandReloadStore.ROOM_TABLE; golden route-golden-smoke-v1 enter-30-001,
## enter-31-001 and enter-40-001 show the room and the narration). Another
## shown background (a later day screen) is not presented here.
func render_schedule(state: State, hud: HudSession) -> Dictionary:
	if presenter == null:
		return {"error":"command reload renderer is not prepared"}
	if hud == null or not hud.intact():
		return {"error":"gameplay command screen needs the intact HUD session"}
	var data: Dictionary = state.rule_state
	var cache: Variant = data.get("background_cache")
	var show: Variant = data.get("background_show")
	if not cache is Array or not show is int or show < 0 or show >= (cache as Array).size() or not int(cache[show]) in RoomStore.ROOM_TABLE:
		return {"error":"gameplay frame with a shown background other than the room is not presented yet"}
	var family_hex: String = LiveHud._family_hex(data.get("profile_family_bytes"))
	var context: Dictionary = hud.hud_context(state.game_mode, state.mode_stage, state.mode_step, family_hex)
	var view: Dictionary = presenter.create_state(data, context)
	if view.is_empty():
		return {"error":presenter.error}
	var failure: String = presenter.set_animation_phase(view, hud.icon_steps, hud.icon_elapsed)
	if not failure.is_empty():
		return {"error":failure}
	# The prologue provider's checker refuses modes other than the opening's;
	# the compositor draws hud_screen_frame_draw's default-mode scroll from
	# frame_count (hud_panels.c:1408-1409), as the legacy live HUD does.
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(int(state.frame_count), (int(data.get("player_flags", 0)) >> 8) & 15))
	var drawn: Image = presenter.render(view, null, true)
	if drawn == null:
		return {"error":presenter.error}
	return {"error":"", "image":drawn, "provenance":{"kind":"gameplay_command_screen_frame",
		"mode":state.game_mode, "unverified":[MASCOT, TYPEWRITER, CHECKER_MODEL]}}


## week_day frames of stage 0 (school_day_screen_init, school_day_sd_load):
## parameter_show, message_window_show, hizuke_show, hud_icon_bar_update(1),
## dec_bg_draw(0) and hud_screen_frame_draw(1) (school_day.c:67-75) over the
## shown background: today's command background from the installed media
## once dec_bg_show[0] selects it. Before that the room slot is shown; the
## room is not drawn on these frames (recorded in provenance). The message
## window was reset by message_window_init/text_system_reset, so no text runs.
func render_school_day(state: State, hud: HudSession, background_lba: int, background_commands: Array[Dictionary], actor_commands: Array[Dictionary] = []) -> Dictionary:
	if presenter == null:
		return {"error":"command reload renderer is not prepared"}
	if hud == null or not hud.intact():
		return {"error":"school-day frame needs the intact HUD session"}
	var data: Dictionary = state.rule_state
	var family_hex: String = LiveHud._family_hex(data.get("profile_family_bytes"))
	var context: Dictionary = hud.hud_context(state.game_mode, state.mode_stage, state.mode_step, family_hex)
	var view: Dictionary = presenter.create_state(data, context)
	if view.is_empty():
		return {"error":presenter.error}
	var failure: String = presenter.set_animation_phase(view, hud.icon_steps, hud.icon_elapsed)
	if not failure.is_empty():
		return {"error":failure}
	var shown: int = int(data.background_cache[int(data.background_show)])
	var scene: Array[Dictionary] = background_commands.duplicate() if shown == background_lba else ([] as Array[Dictionary])
	scene.append_array(actor_commands)
	var unverified: Array[String] = [MASCOT, CHECKER_MODEL]
	if shown != background_lba:
		unverified.append("shown room slot not drawn during school_day_screen_init")
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(int(state.frame_count), (int(data.get("player_flags", 0)) >> 8) & 15))
	var command: Dictionary = presenter.activity_command(view, scene, [0, 0, 0, 0, 0, 0, 0, 0, 0])
	if command.is_empty():
		return {"error":presenter.error}
	return {"error":"", "image":command.image, "provenance":{"kind":"school_day_setup_frame",
		"mode":state.game_mode, "background_lba":shown, "unverified":unverified}}


## T-39002: frames of the Sunday holiday (0x46) and of the next week's command
## setup (0x34 stage 0) after it. scene holds the shown background (already at
## its dec_bg brightness) and the holiday SD actor draws; narration_hex is the
## message window run after the family name, empty while the text is reset.
func render_holiday(state: State, hud: HudSession, scene: Array[Dictionary], narration_hex: String, unverified_extra: Array[String] = []) -> Dictionary:
	if presenter == null:
		return {"error":"command reload renderer is not prepared"}
	if hud == null or not hud.intact():
		return {"error":"holiday frame needs the intact HUD session"}
	var data: Dictionary = state.rule_state
	var family_hex: String = LiveHud._family_hex(data.get("profile_family_bytes"))
	var context: Dictionary = hud.hud_context(state.game_mode, state.mode_stage, state.mode_step, family_hex)
	var view: Dictionary = presenter.create_state(data, context)
	if view.is_empty():
		return {"error":presenter.error}
	var failure: String = presenter.set_animation_phase(view, hud.icon_steps, hud.icon_elapsed)
	if not failure.is_empty():
		return {"error":failure}
	presenter._compositor.set_checker_uv(HudCompositor.compute_checker_uv(int(state.frame_count), (int(data.get("player_flags", 0)) >> 8) & 15))
	var command: Dictionary = presenter.scene_command(view, scene, narration_hex)
	if command.is_empty():
		return {"error":presenter.error}
	var unverified: Array[String] = [MASCOT, CHECKER_MODEL]
	if not narration_hex.is_empty():
		unverified.append(TYPEWRITER)
	unverified.append_array(unverified_extra)
	return {"error":"", "image":command.image, "provenance":{"kind":"holiday_frame",
		"mode":state.game_mode, "stage":state.mode_stage, "unverified":unverified}}


## The command room as the HUD manifest decoded it (C_02_02H, 320x88), or null.
func command_room_image() -> Image:
	if presenter == null:
		return null
	var compositor: HudCompositor = presenter._compositor
	return compositor._image(compositor.descriptor.get("room", {}))


## Icon actors 64 + i / 80 + i and the cursor actor 0, in OT order (higher
## OT first, higher actor index first), as hud_compositor.build_actors lays
## them out, from the HUD session (icon phase, hover record, cursor actor).
func _icon_actors(state: State, hud: HudSession, draw_cursor: bool) -> Array[Dictionary]:
	error = ""
	var descriptor: Dictionary = presenter._compositor.descriptor
	var icons: Variant = state.rule_state.get("icon_enabled")
	if not icons is Array or (icons as Array).size() < 14:
		error = "reload icon bar needs icon_enabled"
		return []
	var steps: Array = hud.icon_steps
	var hover: int = hud.hover_index()
	var actors: Array[Dictionary] = []
	for i: int in range(14):
		var position := Vector2i(descriptor["icon_positions"][i][0], descriptor["icon_positions"][i][1])
		var enabled: bool = int(icons[i]) == 1
		var hovering: bool = enabled and i == hover
		var variant := "icon-52" if hovering else ("icon-53" if enabled else "icon-51")
		actors.append(HudCompositor._actor(64 + i, 3 if not enabled or hovering else 4, 2, position, variant))
		var sequence: Array = descriptor["sequences"][i]["steps"]
		var step: int = int(steps[i])
		if step < 0 or step >= sequence.size():
			error = "reload icon sequence step is outside the original sequence"
			return []
		var icon := HudCompositor._actor(80 + i, 2 if hovering else 3, sequence[step]["frame_index"], position)
		if hovering:
			icon["scale_hover"] = true
		actors.append(icon)
	actors.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		return a["priority"] > b["priority"] if a["priority"] != b["priority"] else a["id"] > b["id"])
	if draw_cursor:
		actors.append(HudCompositor._actor(0, 1, Presenter.resolve_cursor_actor_frame(hud.cursor_style),
			Vector2i(hud.cursor_x, hud.cursor_y), "base", false))
	return actors


func _draw_scaled(target: Image, actor: Dictionary, scale: int) -> void:
	var compositor: HudCompositor = presenter._compositor
	var key := "%d:%s" % [actor["frame"], actor["variant"]]
	if not compositor.descriptor["frames"].has(key):
		compositor.error = "source HUD frame/palette is unavailable: " + key
		return
	var record: Dictionary = compositor.descriptor["frames"][key]
	var anchor: Vector2i = Vector2i(160, 120) + actor["position"]
	for part: Dictionary in record["parts"]:
		var source: Image = compositor._image(part["png"])
		var stp: Image = compositor._image(part["stp_png"]) if part.get("stp_png") is Dictionary else null
		var offset := Vector2i(part["x"], part["y"]) - Vector2i(record["origin"][0], record["origin"][1])
		var size := Vector2i((source.get_width() * scale) >> 12, (source.get_height() * scale) >> 12)
		if size.x < 1 or size.y < 1:
			continue
		var scaled: Image = source.duplicate()
		scaled.resize(size.x, size.y, Image.INTERPOLATE_NEAREST)
		var scaled_stp: Image = null
		if stp != null:
			scaled_stp = stp.duplicate()
			scaled_stp.resize(size.x, size.y, Image.INTERPOLATE_NEAREST)
		compositor._blit(target, scaled, scaled_stp, anchor + Vector2i((offset.x * scale) >> 12, (offset.y * scale) >> 12), actor["semi"])
