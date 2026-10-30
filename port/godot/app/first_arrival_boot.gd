extends Control
## T-29025: normal installed desktop New Game -> the same owned Session.
## Fresh source-prefix initialization is explicit; native startup timing is not
## observed. Missing BGM stops at the actual source bank_start before BG read.
signal frame_published(logical_tick: int)

const Resources = preload("res://services/first_arrival_launch_resources.gd")
const Consumer = preload("res://app/first_arrival_entry_consumer.gd")
const EntryApp = preload("res://app/new_game_entry_boot.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Cold = preload("res://flow/cold_no_card_checker.gd")
const Scratch = preload("res://flow/cold_no_card_scratch.gd")
const Presenter = preload("res://presentation/first_arrival_scene_presenter.gd")
const AudioBridge = preload("res://services/opening_audio_bridge.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Live = preload("res://flow/interactive_prologue.gd")
const Launch = preload("res://app/installed_launch.gd")
const Bgm = preload("res://services/first_arrival_bgm_bridge.gd")
const AudioContext = preload("res://services/first_arrival_audio_context.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const FreshHandoff = preload("res://flow/fresh_gameplay_handoff.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const ReloadServices = preload("res://services/command_reload_services.gd")
const ReloadRenderer = preload("res://presentation/command_reload_renderer.gd")
const SchoolDay = preload("res://scenes/school_day_controller.gd")
const LiveHud = preload("res://presentation/live_hud_adapter.gd")
const OpeningHandoff = preload("res://flow/opening_command_handoff.gd")
const INITIALIZATION_PROFILE := "desktop-fresh-new-game-cold-prefix.v1"
const HUD_PREFIX_SOURCE_SHA := "440bdf2172fb3dae871aa30b96072f81110825ac8d0f875ebce64d288b48f889"
var installed_launch_profile: Dictionary = {}
var initialization_profile := ""
# Boot-path seam: NAME_ENT stage 1 context from flow/title_file_select.gd
# handoff(). Empty for the installed launch (no installed producer yet).
var title_handoff: Dictionary = {}
var resources := Resources.new()
var consumer: Consumer
var media := Media.new()
var upload := Upload.new()
var checker := Cold.new()
var presenter := Presenter.new()
var actor_renderer := ActorRenderer.new()
var audio_bridge := AudioBridge.new()
var bgm := Bgm.new()
var _bgm_prepare_error := "original opening BGM backend is unavailable"
var failure := ""
var failure_gate := ""
var failure_audio_diagnostics: Dictionary = {}
var last_rendered_image: Image
var entry_frame_provenance: Dictionary = {}
var _entry: EntryApp
var _display: TextureRect
var _message: Label
var _voice_player: AudioStreamPlayer
var _bgm_player: AudioStreamPlayer
var _owned_runtime: Dictionary = {}
var _entry_image: Image
var _window_focused := true
var _application_focused := true
var _active := false
## T-39002 mode 0x34 reload: the HUD session driven from this app's boot
## (process_image, game_state_init(1), boot_hud_init's hud_icon_bar_init), the
## handoff opened and bound at the stage 21 switch, its services and renderer.
var hud_session: HudSession
var handoff: FreshHandoff
var reload_services := ReloadServices.new()
var reload_renderer := ReloadRenderer.new()
var reload_frame_provenance: Dictionary = {}
var _reload_unavailable := ""
var _icon_positions: Array = []
var _school_day: SchoolDay

func _ready() -> void:
	get_window().content_scale_size = Vector2i.ZERO
	set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	_display = TextureRect.new()
	_display.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	_display.stretch_mode = TextureRect.STRETCH_SCALE
	_display.texture_filter = CanvasItem.TEXTURE_FILTER_PARENT_NODE
	add_child(_display)
	resized.connect(_layout_display)
	var modern_settings := get_node_or_null("/root/ModernSettings")
	if modern_settings != null: modern_settings.preferences_changed.connect(_layout_display)
	_layout_display()
	_message = Label.new()
	_message.position = Vector2(24,24)
	_message.size = Vector2(900,120)
	_message.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	_message.visible = false
	add_child(_message)
	if initialization_profile != INITIALIZATION_PROFILE:
		_fail("fresh_initialization_profile","New Game requires an explicit fresh-start initialization profile")
		return
	var error := resources.prepare(installed_launch_profile)
	if not error.is_empty():
		_fail("installed_resources",error)
		return
	installed_launch_profile = resources.launch_profile.duplicate(true)
	var audio_cold := AudioContext.cold_context(initialization_profile,resources.audio.native_audio_context)
	if not audio_cold.error.is_empty():
		_fail("audio_context",audio_cold.error)
		return
	resources.audio.native_audio_context = audio_cold.context
	if installed_launch_profile.paths.has("original_first_arrival_bgm"):
		_bgm_prepare_error = bgm.prepare(resources.audio,self,installed_launch_profile.paths.original_first_arrival_bgm)
		if _bgm_prepare_error.is_empty(): resources.audio.original_bgm_service = bgm

	error = media.prepare(resources.media_store,resources.board_store,resources.finish_bank,resources.command_reload)
	# native-bank0: the BGM owner takes the stage21 landing and the 0x11 transfer
	# only with authenticated stage21 reads; otherwise 0x11 stays refused.
	if error.is_empty() and _bgm_prepare_error.is_empty() and resources.finish_bank != null and bgm.bind_finish_bank(resources.finish_bank).is_empty():
		media.bank0_owner = bgm.bank0_transfer_owner
		media.bank0_slot_state = bgm.bank0_slot_state
	var cold := Scratch.initialize({"source_flow_sha256":Scratch.FLOW_SHA,"cold_boot":true,
		"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false})
	if error.is_empty(): error = cold.error
	if error.is_empty():
		var vram := PackedByteArray()
		vram.resize(1024*512*2) # Storage only; all bytes remain unknown until written.
		var owner := "installed-first-arrival:"+str(get_instance_id())
		_owned_runtime = {"scratch_owner":owner+":scratch","vram_owner":owner+":vram",
			"scratch_words":cold.scratch_words,"vram_words":vram,"pending_gpu_effects":[]}
		error = upload.prepare(media,resources.media_store,_owned_runtime,resources.board_store)
		if error.is_empty(): error = actor_renderer.prepare(resources.media_store,upload,resources.board_store)
	if not error.is_empty():
		_fail("runtime_buffers",error)
		return
	_reload_unavailable = _prepare_reload()
	_voice_player = AudioStreamPlayer.new()
	_bgm_player = AudioStreamPlayer.new()
	_voice_player.bus = "Voice"
	_bgm_player.bus = "BGM"
	add_child(_voice_player)
	add_child(_bgm_player)
	error = audio_bridge.bind(resources.audio,_voice_player,_bgm_player)
	if not error.is_empty():
		_fail("audio_owner",error)
		return
	consumer = Consumer.new()
	add_child(consumer)
	consumer.entry_owned.connect(_entry_owned)
	var packed: PackedScene = load(Launch.ENTRY_SCENE)
	_entry = packed.instantiate() as EntryApp
	# Original text/constants and clock policy are injected before Entry._ready.
	_entry.set("installed_launch_profile",installed_launch_profile.duplicate(true))
	_entry.set("profile_text_store",resources.text)
	_entry.set("entry_clock_policy",{"clock_origin":"desktop-fresh-entry-local; native boot/title prefix unmeasured",
		"timer_observation_source":"desktop-one-timer-per-logic-v1","native_clock_parity":false})
	if not title_handoff.is_empty():
		_entry.set("title_handoff",title_handoff)
		_entry.set("entry_clock_policy",{"clock_origin":"title-file-select boot path (flow/title_file_select.gd)",
			"timer_observation_source":"desktop-one-timer-per-logic-v1","native_clock_parity":false})
	add_child(_entry)
	error = consumer.watch_entry(_entry)
	if not error.is_empty(): _fail("entry_consumer",error)

func _entry_owned() -> void:
	if initialization_profile != INITIALIZATION_PROFILE:
		_fail("fresh_initialization_profile","A retained or loaded game cannot use the fresh New Game prefix")
		return
	var error := consumer.begin_opening(_services(),[],[bgm.publish,media.publish,upload.publish,audio_bridge.publish,_publish_frame])
	if error.is_empty():
		consumer.opening.state.opening["native_audio_context"] = resources.audio.native_audio_context.duplicate(true)
		if _bgm_prepare_error.is_empty(): error = bgm.bind_session(consumer.opening.session)
	if error.is_empty(): error = checker.prepare(consumer.opening.session,_checker_route())
	if error.is_empty(): error = upload.bind_session(consumer.opening.session)
	if error.is_empty() and not resources.launch_profile.paths.has("opening_audio"):
		error = resources.lines.prepare_first_arrival(resources.text,resources.painter,consumer.opening.state)
	if error.is_empty(): error = presenter.prepare(media,resources.text,resources.painter,consumer.opening.state,actor_renderer)
	if not error.is_empty():
		_fail("opening_handoff",error)
		return
	# Retain the native image actually submitted by entry after its final frame.
	# Window letterboxing must not become part of the game's retained image.
	# The retained pad has already moved synchronously; no logic ticks elapse
	# while this platform presentation boundary waits for its framebuffer.
	await RenderingServer.frame_post_draw
	if not is_inside_tree() or not is_instance_valid(_entry): return
	var viewport_image: Variant = _entry.get("last_rendered_image")
	if not viewport_image is Image or viewport_image.is_empty() or viewport_image.get_size() != Vector2i(320,240):
		_fail("entry_framebuffer","The live native entry image is unavailable")
		return
	entry_frame_provenance = {"source":"actual-displayed-native-entry-image",
		"original_size":[viewport_image.get_width(),viewport_image.get_height()],
		"logical_size":[320,240],"render_frame":Engine.get_process_frames(),
		"native_entry_pixel_parity":false,"initialization_profile":initialization_profile}
	_entry_image = viewport_image.duplicate()
	last_rendered_image = _entry_image.duplicate()
	error = consumer.release_entry_scene()
	_entry = null
	if not error.is_empty(): _fail("entry_release",error)
	else: _active = true

func _layout_display(_settings: Dictionary = {}) -> void:
	if not is_instance_valid(_display): return
	var area := get_viewport_rect().size
	var factor := minf(area.x/320.0,area.y/240.0)
	var settings := get_node_or_null("/root/ModernSettings")
	if settings != null and settings.preferences.effective_settings().display.filter == "integer" and factor >= 1.0:
		factor = floorf(factor)
	_display.size = Vector2(320,240)*factor
	_display.position = (area-_display.size)*0.5

func _services() -> Dictionary:
	var result := media.callbacks()
	for key: String in Live.Arrival.REQUIRED+Live.Reload.REQUIRED:
		if not result.has(key): result[key] = _unsupported
	# The 0x34 reload belongs to the fresh gameplay handoff, bound at the
	# stage 21 switch (_open_reload); the opening's own 0x34 handler refuses it.
	result["setup_reset"] = _command_reload_unbound
	result.merge({"bank_load_start":_bank_load,"bank_poll":_bank_poll,"bank_start":_bank_start,
		"audio_command":AudioRules.audio_command,
		"scene_reset":_scene_reset,"arrival_draw":_arrival_draw,"line_start":_line_start,"line_run":resources.lines.line_run,
		"outer_draw":_outer_draw,"name_entry_exit_draw":_exit_draw,"overlay_draw":checker.overlay_draw,
		"logical_draw":_logical_draw,"cd_tick":_cd_tick,"audio_tick":_audio_tick,"completion":_completion},true)
	return result

func _physics_process(delta: float) -> void:
	if not _active or not is_physics_processing() or not failure.is_empty() or consumer == null or consumer.opening.session == null or not _window_focused or not _application_focused: return
	var settings := get_node_or_null("/root/ModernSettings")
	if settings != null:
		audio_bridge.voice.custom_voice_pack = settings.voice_pack
		presenter.text_translations = settings.text_pack
		var skipping := false
		if Input.is_action_just_pressed("port_skip"): skipping = settings.request_prologue_skip(consumer.input_owner)
		var hold: bool = settings.pacing.step(consumer.opening.state.opening, delta)
		settings.observe_prologue_read(consumer.opening.state.opening)
		presenter.presentation_visible_count = settings.pacing.visible_count
		if hold and not skipping:
			var refreshed: Dictionary = presenter.render(consumer.opening.state, _draw_checker, _draw_front, upload.batch_ready, last_rendered_image)
			if not refreshed.error.is_empty(): _fail("accessible_presentation", refreshed.error)
			else:
				last_rendered_image = refreshed.image
				if _display.texture is ImageTexture: (_display.texture as ImageTexture).update(last_rendered_image)
			return
	var pre_pump := bgm.pump(consumer.opening.state)
	if not pre_pump.is_empty():
		_fail("bgm_pump",pre_pump)
		return
	var tick: int = consumer.opening.session.clock.logic_iterations
	var events := media.pending_events(tick)
	events.append_array(audio_bridge.pending_events(tick))
	var preparation := bgm.prepare_pending(consumer.opening.state,events) if _bgm_prepare_error.is_empty() else ""
	if not preparation.is_empty():
		bgm.abort_frame()
		_fail("bgm_preparation",preparation)
		return
	var result := consumer.step(events)
	if not result.committed:
		bgm.abort_frame()
		_fail("bgm_backend" if result.error == "original opening BGM backend is unavailable" else "source_transaction",result.error)
	elif not consumer.opening.publish_error.is_empty(): _fail("publication",consumer.opening.publish_error)
	elif failure.is_empty():
		var pumping := bgm.pump(consumer.opening.state)
		if not pumping.is_empty(): _fail("bgm_pump",pumping)
		elif handoff == null and consumer.opening.state.game_mode == 0x34: _open_reload()
		elif handoff != null and handoff.phase == "command_select" and consumer.opening.command_ready: _connect_gameplay()

func _process(_delta: float) -> void:
	# Service the wall-clock mixer between rendered/physics frames too. This
	# never advances source simulation or changes its committed audio owner.
	if not _active or not failure.is_empty() or not _window_focused or not _application_focused or consumer == null: return
	var outcome := bgm.pump(consumer.opening.state)
	if not outcome.is_empty(): _fail("bgm_pump",outcome)

func _bank_load(state: State, role: String, result: Session.FrameResult) -> void:
	AudioRules.begin_bank(state,role,result)

func _bank_poll(state: State, result: Session.FrameResult) -> int:
	# Mode 0x34 stage 0xFF polls the stage21 COM_MIX/SPRING reads, which the
	# media queue owns (prologue_finish_run case 0), not an opening audio role.
	if state.game_mode == 0x34: return media.finish_bank_poll(state,result)
	var bank: Variant = state.opening.get("audio_bank")
	if not bank is Dictionary or not bank.get("ready") is bool:
		result.error = "opening bank has no owned producer completion state"
		return 0
	# Stage11 substep2 polls the three CD reads that overwrite the live Rei
	# score and header; the BGM owner holds their measured landing schedule.
	if bank.get("role") == "prologue_shiori":
		if not _bgm_prepare_error.is_empty():
			result.error = _bgm_prepare_error
			return 0
		return bgm.shiori_poll(state,result)
	# The arrival and Rei banks also need their CD reads and SPU transfer
	# frames to have elapsed in logic frames (AudioRules.BANK_READ_SECTORS).
	return AudioRules.bank_poll(state,result)

func _bank_start(state: State, bank: int, sequence: int, result: Session.FrameResult) -> void:
	if not _bgm_prepare_error.is_empty(): result.error = _bgm_prepare_error
	else: bgm.start(state,bank,sequence,result)

func _arrival_draw(state: State, slot: int, result: Session.FrameResult) -> void:
	media.arrival_draw(state,slot,result,actor_renderer)

func _scene_reset(state: State, result: Session.FrameResult) -> void:
	media.scene_reset(state,result)
	if not result.error.is_empty(): return
	resources.lines.scene_reset(state,result)
	if not result.error.is_empty(): return
	var projection := _hud_projection(state)
	var output: Dictionary = {}
	result.error = resources.hud.scene_reset(projection,output)
	if result.error.is_empty():
		state.opening.prologue_hud_live = projection.opening.prologue_hud_live
		state.opening["prologue_hud_scene_initialized"] = true

func _line_start(state: State, line: int, result: Session.FrameResult) -> void:
	media.before_line0(state,line,result)
	if result.error.is_empty(): resources.lines.line_start(state,line,result)

func _outer_draw(state: State, argument: int, result: Session.FrameResult) -> void:
	if resources.hud.source_frame_argument(_hud_projection(state)) != argument:
		result.error = "first-arrival HUD cannot render another source stage"
		return
	if not state.opening.has("prologue_hud_live"):
		result.error = _initial_hud(state)
		if not result.error.is_empty(): return
	if state.opening.get("prologue_hud_scene_initialized",false):
		var projection := _hud_projection(state)
		result.error = resources.hud.advance_outer(projection,consumer.opening.session.rng,{},argument)
		if result.error.is_empty(): state.opening.prologue_hud_live = projection.opening.prologue_hud_live
	else:
		# The first bank-load frame precedes scene init. The hidden title update
		# returns without advancing its184 timer; only the actual checker moves.
		var live: Dictionary = state.opening.prologue_hud_live.duplicate(true)
		var updated := Cold._updated(live.checker_scroll_u,live.checker_scroll_v,state.frame_count)
		live.checker_scroll_u = updated.u
		live.checker_scroll_v = updated.v
		live.checker_scroll_phase = "after_hud_screen_frame_draw_scroll_update"
		state.opening.prologue_hud_live = live
	# Every hud_screen_frame_draw call also steps the checker palette fade
	# D_800B5BC8 (hud_panels.c:1462-1466): 0xFE -> 0xFF -> 0 after stage 0's
	# store, on frame_count & 3 == 3 before the post-handler increment.
	if result.error.is_empty(): FreshHandoff.hud_screen_fade(state.opening,state.frame_count)

func _initial_hud(state: State) -> String:
	var route: Dictionary = state.rule_state
	for key: String in ["year","month","day","weekday"]:
		if not route.get(key) is int: return "fresh calendar field has no original profile owner"
	if not route.get("calendar") is Array or route.calendar.is_empty() or not route.calendar[0] is Dictionary or not route.calendar[0].get("day_kind") is Array or route.day < 0 or route.day >= route.calendar[0].day_kind.size() or not route.calendar[0].day_kind[route.day] is int:
		return "fresh calendar has no owned indexed day kind"
	if [route.year,route.month,route.day,route.weekday,route.calendar[0].day_kind[route.day]] != [95,4,4,1,0]: return "first-arrival initialization excludes a retained or loaded calendar"
	var transfer := checker.handoff(state)
	if not transfer.error.is_empty(): return transfer.error
	var live := {"year":route.year,"month":route.month,"day":route.day,"weekday":route.weekday,
		"day_kind":route.calendar[0].day_kind[route.day],"title_timer":184,"title_visible":false,
		"date_visible":false,"message_visible":false,"checker_fade":0,"screen_brightness":state.opening.screen_brightness}
	var imported := resources.hud.import_scroll(live,transfer.transport)
	if imported.get("status") != "scroll_imported": return str(imported.get("diagnostic","owned checker prefix could not be imported"))
	state.opening["prologue_hud_live"] = imported.hud
	state.opening["hud_prefix_source_sha256"] = HUD_PREFIX_SOURCE_SHA
	return ""

func _hud_projection(state: State) -> Dictionary:
	var flags: int = state.rule_state.player_flags
	var opening_copy: Dictionary = state.opening.duplicate(false)
	if opening_copy.get("prologue_hud_live") is Dictionary:
		opening_copy["prologue_hud_live"] = opening_copy.prologue_hud_live.duplicate(true)
	return {"game_mode":state.game_mode,"mode_stage":state.mode_stage,"mode_step":state.mode_step,"substep":state.substep,"frame_count":state.frame_count,
		"player_flags":PackedByteArray([flags&255,(flags>>8)&255]),
		"pad_held_triangle":(consumer.opening.session.latched_input.held&0x1000) != 0,
		"opening":opening_copy}

func _exit_draw(state: State, result: Session.FrameResult) -> void:
	# Source draw publication is deferred until the owning Session commits.
	result.presentation.append({"kind":"first_arrival_entry_exit_draw","brightness":state.opening.screen_brightness})

func _logical_draw(state: State, _rng, result: Session.FrameResult) -> void:
	# Reload frames: actor_draw_all advances the sequence of every flagged
	# icon actor 80 + i (hud_icon_bar_update(0) flags the enabled ones).
	if ReloadRenderer.reload_frame(state):
		if handoff == null or handoff.hud == null: result.error = "reload frame has no HUD session"
		else: result.error = handoff.hud.advance_icon_animation(reload_renderer.icon_sequences(),state.rule_state.icon_enabled)
		return
	# The stage21 switch frame (committed 0x34/0xFF/0/0) is still drawn by the
	# prologue's draw_frame; later 0x34 frames belong to the command reload.
	var switch_frame: bool = state.game_mode == 0x34 and resources.hud.source_frame_argument(_hud_projection(state)) == 2
	if (state.game_mode not in [0x14,2,0x15] and not switch_frame) or (state.game_mode == 0x15 and state.mode_stage != 0 and resources.hud.source_frame_argument(_hud_projection(state)) != 2):
		result.error = "only the original first-arrival route is prepared"
		return
	if state.game_mode == 0x15 or switch_frame: actor_renderer.logical_draw(state,_rng,result)

func _cd_tick(state: State, _rng, result: Session.FrameResult) -> void:
	# XA cd_state advances on logic frames before the audio phase preflights
	# the voice intent it may publish.
	AudioRules.xa_tick(state,result)
	if not result.error.is_empty(): return
	result.error = upload.preflight(result,state)
	if result.error.is_empty():
		result.error = bgm.pump(state)

func _audio_tick(state: State, _rng, result: Session.FrameResult) -> void:
	AudioRules.bgm_fade_tick(state,result)
	result.error = audio_bridge.preflight(result,state)
	if result.error.is_empty():
		result.error = bgm.pump(state)

func _completion(event: Session.ServiceEvent, state: State, rng, result: Session.FrameResult) -> void:
	if event.service == Media.Queue.SERVICE: media.complete(event,state,rng,result)
	elif event.service == "opening.audio.bank":
		AudioRules.bank_complete(event,state,result)
		if result.error.is_empty() and _bgm_prepare_error.is_empty(): result.error = bgm.bank_completed(state)
	elif event.service == AudioRules.COMPLETION_SERVICE: AudioRules.complete(event,state,result)
	else: result.error = "opening completion belongs to an unprepared service"

func _publish_frame(result: Session.FrameResult, tick: int) -> String:
	if not result.committed or consumer.opening.session.clock.logic_iterations-1 != tick: return "frame publication lost its committed Session owner"
	var state: State = consumer.opening.state
	var image: Image
	# ovl_load_enter changes game_mode before its own loader draw. The first
	# prologue HUD pass occurs on the following actual mode dispatch.
	var loader_enter := state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 0 and state.substep == 0 and not state.opening.has("prologue_hud_live")
	if loader_enter:
		var prefix: Variant = state.opening.get(Cold.RECORD)
		if not prefix is Dictionary or prefix.get("loader_calls") != 2 or prefix.get("handed_off") != false or state.opening.screen_brightness != 0: return "loader-enter framebuffer has no exact completed checker prefix"
	if ReloadRenderer.reload_frame(state):
		var drawn := reload_renderer.render(state,handoff.hud if handoff != null else null,_hud_projection(state),handoff.hud_unsourced if handoff != null else "")
		if not str(drawn.get("error","")).is_empty(): return str(drawn.error)
		image = drawn.image
		reload_frame_provenance = drawn.provenance
	elif state.game_mode in [0x14,2] or loader_enter:
		image = _entry_image.duplicate()
		var brightness: int = state.opening.screen_brightness
		for y in range(image.get_height()):
			for x in range(image.get_width()):
				var color := image.get_pixel(x,y)
				image.set_pixel(x,y,Color8((color.r8*brightness)>>7,(color.g8*brightness)>>7,(color.b8*brightness)>>7,color.a8))
	elif state.opening.has("arrival_media"):
		var modern_settings := get_node_or_null("/root/ModernSettings")
		if modern_settings != null:
			presenter.configure_accessibility(modern_settings.preferences.settings.accessibility)
		var composed := presenter.render(state,_draw_checker,_draw_front,upload.batch_ready,last_rendered_image)
		if not composed.error.is_empty(): return composed.error
		image = composed.image
	else:
		image = last_rendered_image.duplicate()
		var error := _draw_checker(image,state)
		if error.is_empty(): error = _draw_front(image,state)
		if not error.is_empty(): return error
	last_rendered_image = image
	if _display.texture is ImageTexture:
		(_display.texture as ImageTexture).update(image)
	else:
		_display.texture = ImageTexture.create_from_image(image)
	frame_published.emit(tick)
	return ""

func _draw_checker(target: Image, state: State) -> String:
	var err := resources.hud.draw_checker(target,_hud_projection(state))
	if not err.is_empty(): return err
	return bgm.pump(state)

func _draw_front(target: Image, state: State) -> String:
	var err := resources.hud.draw_front(target,_hud_projection(state))
	if not err.is_empty(): return err
	return bgm.pump(state)

## T-39002: the mode 0x34 stage 0xFF reload. The HUD session starts at this
## app's boot; the handoff opens on the committed switch frame (0x34/0xFF/0/0)
## and binds its 0x34 handler with the reload services, so every reload frame
## runs FreshHandoff.mode_handler -> Reload.step inside GameSession.step.
const COMMAND_RELOAD_UNBOUND := "mode 0x34 reload runs only through the fresh gameplay handoff bound at the stage 21 switch"

func _command_reload_unbound(_state: State, result: Session.FrameResult) -> void:
	result.error = COMMAND_RELOAD_UNBOUND

## Prepares what the reload needs from the installed pack. An empty result
## means ready; otherwise the reason is reported at the switch.
func _prepare_reload() -> String:
	if resources.command_reload == null: return "installed pack lacks the first_arrival_command_reload role (CALROM and room reads)"
	if resources.hud_manifest.is_empty(): return "installed pack lacks the command HUD manifest, font root or rule tables"
	var error := reload_renderer.prepare(resources.hud_manifest,resources.hud)
	if error.is_empty():
		error = reload_services.prepare(media,resources.hud,_bank_poll,_bank_start,_hud_projection,
			func() -> Object: return consumer.opening.session.rng if consumer != null and consumer.opening.session != null else null,
			reload_renderer.room_original_sha256())
	if not error.is_empty(): return error
	var tables: Variant = reload_renderer.presenter._manifest.get("tables",{}).get("icon_positions")
	if not tables is Array or (tables as Array).size() != 14: return "command HUD manifest lacks the fourteen icon positions"
	_icon_positions = LiveHud._integers((tables as Array).duplicate(true))
	if FileAccess.get_sha256(resources.hud_manifest.rule_tables) != resources.hud_manifest.rule_tables_sha256: return "rule tables differ from the installed inventory"
	var rules: Variant = LiveHud._integers(JSON.parse_string(FileAccess.get_file_as_string(resources.hud_manifest.rule_tables)))
	if not rules is Dictionary or rules.get("schema") != "tokimemo.port-rule-tables.v1" or not rules.get("tables") is Dictionary: return "installed rule tables are unreadable"
	_school_day = SchoolDay.new(rules.tables)
	if not _school_day.error.is_empty(): return "school-day controller: " + _school_day.error
	# process_image, then the main boot: game_state_init(1) (game_init.c:142)
	# and boot_hud_init's hud_icon_bar_init (boot_assets.c:136).
	var session := HudSession.new()
	error = session.game_state_init(true)
	if error.is_empty(): error = session.hud_icon_bar_init(_icon_positions)
	if not error.is_empty(): return "boot HUD session: " + error
	hud_session = session
	return ""

## Opens and binds the handoff on the committed stage 21 switch frame.
func _open_reload() -> void:
	if not _reload_unavailable.is_empty():
		_fail("command_reload",_reload_unavailable)
		return
	var next := FreshHandoff.new()
	var error := next.open(consumer.opening.state,hud_session,_icon_positions,_school_day)
	if error.is_empty(): error = next.bind(consumer.opening.session,reload_services.services())
	if not error.is_empty():
		_fail("command_reload",error)
		return
	handoff = next

## On the committed first command-select frame (0x34/1/0, command_ready) the
## opening session is handed, unchanged, to the school-day controller through
## the handoff (flow/opening_command_handoff.gd); later frames step the same
## session under the controller's handlers.
func _connect_gameplay() -> void:
	var connected := OpeningHandoff.connect_gameplay(consumer.opening,handoff,hud_session)
	if not str(connected.get("error","")).is_empty():
		_fail("gameplay_handoff",str(connected.error))

func _unsupported(a = null,b = null,c = null,d = null,e = null,f = null) -> int:
	for value: Variant in [a,b,c,d,e,f]:
		if value is Session.FrameResult: value.error = "source service is outside the prepared first-arrival scope"
	return 0

func _checker_route() -> Dictionary:
	return {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"title_new_game":true,"no_card_answer":true,
		"options_visited":false,"retained_game":false,"memory_card_load":false,"attract_visited":false,"debug_intervention":false}

func _fail(gate: String, detail: String) -> void:
	failure_audio_diagnostics = bgm.diagnostics()
	bgm.reset()
	failure_gate = gate
	failure = detail
	if is_instance_valid(_message):
		_message.text = "Opening music is not available in this build." if gate == "bgm_backend" else "New Game could not continue. Check the original game data in setup."
		_message.visible = true
		move_child(_message,get_child_count()-1)
	if OS.get_environment("TOKIMEMO_ENTRY_DEBUG") == "1": printerr("New Game stopped at ",gate,": ",detail)

func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_WINDOW_FOCUS_OUT: _window_focused = false
		NOTIFICATION_APPLICATION_FOCUS_OUT: _application_focused = false
		NOTIFICATION_WM_WINDOW_FOCUS_IN: _window_focused = true
		NOTIFICATION_APPLICATION_FOCUS_IN: _application_focused = true
		_: return
	bgm.pause(not _window_focused or not _application_focused)
	if (not _window_focused or not _application_focused) and is_instance_valid(consumer) and is_instance_valid(consumer.input_owner): consumer.input_owner.clear_devices()

func _exit_tree() -> void:
	bgm.reset()
	audio_bridge.reset_session()
