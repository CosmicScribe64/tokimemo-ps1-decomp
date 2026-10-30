extends Control
## Separate development entry scene. Does not replace the captured school-day app.
const ContextOwner = preload("res://flow/entry_runtime_context.gd")
const CoreState = preload("res://core/game_state.gd")
const Clock = preload("res://core/native_clock.gd")
const EntryPad = preload("res://ui/entry_pad.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const Diagnostics = preload("res://services/new_game_load_diagnostics.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Presenter = preload("res://presentation/vn_scene_presenter.gd")
const EntryImages = preload("res://presentation/name_entry_image_presenter.gd")
const NamePointer = preload("res://flow/name_entry_pointer.gd")
const LaunchProfile = preload("res://services/installed_launch_profile.gd")
const Launch = preload("res://app/installed_launch.gd")
const TitleFlow = preload("res://flow/title_file_select.gd")
const DisplayHost = preload("res://presentation/native_display_host.gd")
const Latin = preload("res://flow/name_entry_latin_page.gd")
signal profile_finished(entry: RefCounted, native_rng: RefCounted, launch_profile: Dictionary, live_context: Dictionary)
# Injected by the installed launch owner after original resource authentication.
var profile_text_store: TextStore
var entry_clock_policy: Dictionary = {}
# Boot-path seam (flow/title_file_select.gd handoff(); no installed producer
# yet): the clock, BIOS rand object and scene_work at NAME_ENT stage 1.
var title_handoff: Dictionary = {}
var _entry_runtime := ContextOwner.new()
var _entry_model := CoreState.new()
var _entry_clock := Clock.new()
var _entry_pad: EntryPad
var _entry_residency: Dictionary = {}
var _entry_ui_brightness: int = -1
var installed_launch_profile: Dictionary = {}
var profile_handoff: Dictionary = {}
var _completion_emitted := false
var controller := Entry.new()
var glyphs := Glyphs.new()
var rng := Rng.new()
var queue: Array[Dictionary] = []
var panel: VBoxContainer
## T-30001 WP6: shows last_rendered_image in the window (original or wide).
var display_host: DisplayHost
## Hit layer in native coordinates: the pointer buttons and the keyboard
## outline. It draws no game pixels; it follows the host's aperture.
var canvas: Canvas
## Phase 4 WP-D: English-mode Latin typing over the native Symbols page.
var latin := Latin.new()
## Window-space toggle for English keyboard typing (null until first shown).
var typing_toggle: Button
var presenter: Presenter
var entry_images: EntryImages
var name_pointer := NamePointer.new()
var native_hits: Control
var navigation_cell := Vector2i.ZERO
var navigation_visible := false
var navigation_direction := 0
var navigation_repeat := 0.0
var last_rendered_image: Image
var profile_complete: bool = false
var failure: String = ""
var tick: int = 0
var debug_visible: bool = false
var load_diagnostics: Dictionary = {}
var focus_paused: bool = false
var _window_focused: bool = true
var _application_focused: bool = true

func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_WINDOW_FOCUS_OUT: _window_focused = false
		NOTIFICATION_APPLICATION_FOCUS_OUT: _application_focused = false
		NOTIFICATION_WM_WINDOW_FOCUS_IN: _window_focused = true
		NOTIFICATION_APPLICATION_FOCUS_IN: _application_focused = true
		_: return
	focus_paused = not (_window_focused and _application_focused)
	if focus_paused:
		queue.clear()
		navigation_direction = 0; navigation_repeat = 0.0
		if is_instance_valid(_entry_pad): _entry_pad.clear_devices()

func _ensure_native_presenter() -> void:
	if canvas == null:
		display_host = DisplayHost.new()
		display_host.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
		add_child(display_host)
		move_child(display_host, 0)
		canvas = Canvas.new()
		add_child(canvas)
		display_host.layout_changed.connect(_align_hit_layer)
		_align_hit_layer(display_host.layout)
	if presenter != null:
		return
	var font_path: String = str(installed_launch_profile.get("paths", {}).get("font_root", ""))
	if font_path.is_empty() and installed_launch_profile.is_empty():
		font_path = OS.get_environment("TOKIMEMO_NATIVE_FONT")
	if font_path.is_empty() and installed_launch_profile.is_empty():
		font_path = OS.get_environment("TOKIMEMO_FONT_ROOT")
	if font_path.is_empty() and installed_launch_profile.is_empty():
		for candidate_font: String in [
			ProjectSettings.globalize_path("res://../..").simplify_path().path_join(".asset-cache/port/fonts/native-font-v2"),
			"/workspace/.asset-cache/port/fonts/native-font-v2",
			"/work/.asset-cache/port/fonts/native-font-v2",
		]:
			if DirAccess.dir_exists_absolute(candidate_font):
				font_path = candidate_font
				break
	if font_path.is_empty() or not DirAccess.dir_exists_absolute(font_path):
		return
	var cand_presenter := Presenter.new()
	if cand_presenter.open_font_only(font_path):
		presenter = cand_presenter
		var image_path: String = str(installed_launch_profile.get("paths", {}).get("name_entry_media", ""))
		if not image_path.is_empty():
			var candidate := EntryImages.new()
			if candidate.open_pack(image_path, FileAccess.get_sha256(image_path), presenter):
				entry_images = candidate
			else:
				failure = candidate.media.error
	else:
		failure = "Original entry graphics could not load: " + cand_presenter.error

func _ready() -> void:
	_entry_pad = EntryPad.new()
	# Keys taken by English keyboard typing stay out of the pad sample, so a
	# typed Q is not also a held Square.
	_entry_pad.key_capture = func(event: InputEvent) -> bool:
		return _over_typing_toggle(event) or (_name_input_active() and latin.captures(event, _entry_pad.action_mask(event)))
	add_child(_entry_pad)
	# Fresh desktop entry origin; earlier native boot/title duration is unmeasured.
	# These are source initialization operations, never derived from startup RNG.
	_entry_model.frame_count = 0 # game_state_init; desktop origin recorded below.
	_entry_model.system_seed = 0 # sys_init bzero before timer callbacks.
	_entry_model.timer_ticks = 0
	_entry_ui_brightness = 128 # draw_brightness_reset; current entry renderer is full-bright.
	var title_error: String = title_handoff_error(title_handoff) if not title_handoff.is_empty() else ""
	if not title_handoff.is_empty() and title_error.is_empty():
		# The modelled boot path reaches NAME_ENT stage 1 with these words.
		_entry_model.frame_count = int(title_handoff.frame_count)
		_entry_model.timer_ticks = int(title_handoff.timer_ticks)
		_entry_model.system_seed = int(title_handoff.system_seed)
	var context_error: String = _entry_runtime.bind(_entry_model, _entry_clock, _entry_pad, entry_clock_policy)
	if not context_error.is_empty(): failure = context_error
	# Native art owns its scale; desktop settings retain window-pixel UI space.
	get_window().content_scale_size = Vector2i.ZERO
	_ensure_native_presenter()
	panel = VBoxContainer.new(); panel.position = Vector2(24,16); panel.size = Vector2(950,660); add_child(panel)
	debug_visible = OS.get_environment("TOKIMEMO_ENTRY_DEBUG") == "1"
	get_window().title = "New Game"+(" [%d]"%OS.get_process_id() if debug_visible else "")
	var rules_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_RULES")
	var entry_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_ENTRY")
	var font_path: String = OS.get_environment("TOKIMEMO_NATIVE_FONT")
	if not installed_launch_profile.is_empty():
		var paths: Dictionary = installed_launch_profile.get("paths", {})
		rules_path = str(paths.get("name_entry_rules", ""))
		entry_path = str(paths.get("name_entry_content", ""))
		font_path = str(paths.get("font_root", ""))
	var tables: Dictionary = Rules.load_tables(rules_path)
	var content: Dictionary = Entry.load_content(entry_path)
	latin.configure(content)
	latin.enabled = _english_selected()
	var seed: String = OS.get_environment("TOKIMEMO_NEW_GAME_SEED")
	if not title_handoff.is_empty():
		# The boot path owns the live BIOS rand object (srand(system_data.seed)
		# at file_select.c:735, then the iris draws); the entry keeps drawing it.
		seed = str(title_handoff.rng.state) if title_error.is_empty() else ""
	elif not installed_launch_profile.is_empty():
		# file_select_init_settings: srand(system_data.seed) (file_select.c:735).
		# system_seed was just zeroed by the sys_init reset above and nothing has
		# ticked since, so this is 0. The same live RNG object is retained
		# throughout profile entry.
		seed = str(Entry.file_select_seed(_entry_model))
	load_diagnostics = {"build":"entry-ui-cleanup-v1","pid":OS.get_process_id(),"executable":OS.get_executable_path(),
		"rules":Diagnostics.inspect(rules_path,Rules._content_valid(tables)),"entry":Diagnostics.inspect(entry_path,Entry.content_valid(content)),"seed_valid":not seed.is_empty() and seed.is_valid_int(),"failed_gate":""}
	load_diagnostics["startup_seed"] = int(seed) if seed.is_valid_int() else -1
	load_diagnostics["title_handoff"] = not title_handoff.is_empty()
	if not title_error.is_empty():
		failure = "New Game could not start from the title."; load_diagnostics.failed_gate="title_handoff"; load_diagnostics.title_error=title_error
	elif not Rules._content_valid(tables):
		failure = "Game data could not be loaded. Check the rule pack."; load_diagnostics.failed_gate="rule_pack"
	elif not Entry.content_valid(content):
		failure = "Game data could not be loaded. Check the name-entry pack."; load_diagnostics.failed_gate="entry_pack"
	elif tables.space != content.space:
		failure = "The game-data packs do not match."; load_diagnostics.failed_gate="pack_space_identity"
	elif not load_diagnostics.seed_valid:
		failure = "A startup seed is required."; load_diagnostics.failed_gate="startup_seed"
	if failure.is_empty():
		var font_error: String = glyphs.open_external_font(font_path)
		if not font_error.is_empty():
			failure="The game font could not be loaded. Check the font pack.";load_diagnostics.failed_gate="font_pack";load_diagnostics.font_error=font_error
	if failure.is_empty():
		if not title_handoff.is_empty(): rng = title_handoff.rng
		if not title_handoff.is_empty() and not rng.configuration_error().is_empty():
			failure="The startup random source could not be configured.";load_diagnostics.failed_gate="rng_profile"
		elif title_handoff.is_empty() and not rng.configure(Rng.OPENBIOS_PROFILE,int(seed)):
			failure="The startup random source could not be configured.";load_diagnostics.failed_gate="rng_profile"
		else:
			var entry_error: String=controller.begin(tables,content,rng)
			if not entry_error.is_empty(): failure="New Game could not start.";load_diagnostics.failed_gate="entry_begin";load_diagnostics.entry_error=entry_error
			else:
				_entry_residency = {"lba":0x95ea,"source_sha256":ContextOwner.NAME_ENT_SHA,"content_sha256":Entry.content_identity(content),"ready":true}
	if debug_visible: print("T29012_ENTRY_LOAD_DIAGNOSTIC "+JSON.stringify(load_diagnostics))
	var settings := get_node_or_null("/root/ModernSettings")
	if settings != null:
		settings.preferences_changed.connect(_preferences_changed)
		# English is read from language/en of the pack this launch authenticated.
		if settings.has_method("note_installed_root"):
			settings.note_installed_root(str(installed_launch_profile.get("profile_root", "")))
	_render()

## Shape check of the boot-path handoff (flow/title_file_select.gd handoff()).
static func title_handoff_error(handoff: Dictionary) -> String:
	if handoff.get("schema") != TitleFlow.SCHEMA or not str(handoff.get("error", "x")).is_empty(): return "title handoff is not a completed boot path"
	for key: String in ["frame_count", "timer_ticks", "system_seed", "scene_work"]:
		if not handoff.get(key) is int: return "title handoff lacks " + key
	if handoff.frame_count < 0 or handoff.frame_count > 0xffffffff: return "title handoff frame_count is out of range"
	if handoff.timer_ticks < -0x80000000 or handoff.timer_ticks > 0xffffffff or handoff.system_seed < -0x80000000 or handoff.system_seed > 0xffffffff: return "title handoff timer words are out of range"
	if handoff.scene_work < -0x80000000 or handoff.scene_work > 0x7fffffff: return "title handoff scene_work is out of range"
	var live_rng: Variant = handoff.get("rng")
	if not live_rng is Rng or not live_rng.configuration_error().is_empty(): return "title handoff has no configured BIOS rand object"
	return ""

func title_error_free() -> bool:
	return not title_handoff.is_empty() and title_handoff_error(title_handoff).is_empty()

## Language changes switch Latin typing and redraw with the active art; the
## display host re-frames itself.
func _preferences_changed(_settings: Dictionary) -> void:
	var english := _english_selected()
	if english != latin.enabled:
		latin.enabled = english
		latin.clear()
	_update_typing_toggle()
	if is_instance_valid(panel) and failure.is_empty(): _render_native_canvas()

## English keyboard typing is an explicit mode shown by a window-space
## toggle under the native frame; outside it every key keeps its pad meaning.
func _update_typing_toggle() -> void:
	var wanted := latin.enabled and failure.is_empty() and _name_input_active()
	if not wanted and latin.typing: latin.set_typing(false)
	if typing_toggle == null:
		if not wanted: return
		typing_toggle = Button.new()
		typing_toggle.focus_mode = Control.FOCUS_NONE
		typing_toggle.toggle_mode = true
		typing_toggle.set_anchors_and_offsets_preset(Control.PRESET_BOTTOM_LEFT)
		typing_toggle.position = Vector2(8, get_viewport_rect().size.y - 40)
		typing_toggle.toggled.connect(func(on: bool) -> void:
			latin.set_typing(on)
			_update_typing_toggle())
		add_child(typing_toggle)
	typing_toggle.visible = wanted
	typing_toggle.set_pressed_no_signal(latin.typing)
	var key := OS.get_keycode_string(Latin.TOGGLE_KEY)
	typing_toggle.text = ("Keyboard typing ON - Enter finishes, Esc stops (%s)" % key) if latin.typing \
		else ("Type the name with the keyboard (%s)" % key)
	typing_toggle.reset_size()
	typing_toggle.position = Vector2(8, get_viewport_rect().size.y - typing_toggle.size.y - 8)

func _over_typing_toggle(event: InputEvent) -> bool:
	return event is InputEventMouseButton and typing_toggle != null and typing_toggle.is_visible_in_tree() \
		and typing_toggle.get_global_rect().has_point(event.position)

func _english_selected() -> bool:
	var settings := get_node_or_null("/root/ModernSettings")
	return settings != null and str(settings.preferences.effective_settings().language.get("bundle", "ja")) == "en"

## Keeps the native-coordinate hit layer on the host's 320x240 aperture.
func _align_hit_layer(layout: Dictionary) -> void:
	if canvas == null or not layout.get("valid", false): return
	var aperture: Rect2 = layout.aperture
	canvas.scale = Vector2.ONE*float(layout.factor)
	canvas.position = aperture.position

func _send(action: Dictionary) -> void:
	if not focus_paused: queue.append(action.duplicate(true))

func _source_pointer_ready() -> bool:
	return entry_images != null and entry_images.media.manifest.has("cursors")

func _name_input_active() -> bool:
	return controller.stage == 1 and controller.step_id in [1,3,5]

func differential_snapshot() -> Dictionary:
	# Read-only observer. Desktop clock origin and missing native records remain
	# explicit; this is not a reconstructed PS1 RAM image or a parity verdict.
	var name_bytes: Dictionary = {}
	for part in controller.names:
		name_bytes[part] = PackedByteArray(controller.names[part]).hex_encode()
	var sampled_input: Variant = null
	if is_instance_valid(_entry_pad) and _entry_pad._latest_sample != null:
		sampled_input = _entry_pad._latest_sample.snapshot()
	var composed: Dictionary = {}
	if last_rendered_image != null:
		var digest := HashingContext.new()
		digest.start(HashingContext.HASH_SHA256)
		digest.update(last_rendered_image.get_data())
		composed = {"width":last_rendered_image.get_width(),"height":last_rendered_image.get_height(),
			"format":last_rendered_image.get_format(),"pixel_sha256":digest.finish().hex_encode()}
	return {"schema":"tokimemo.entry-observation.v1","producer":"new_game_entry_boot.differential_snapshot",
		"phase":"observer_call; caller must bind to frame_post_draw","scene":scene_file_path,
		"state":{"stage":controller.stage,"step":controller.step_id,"cursor":controller.cursor,
			"tab":controller.tab,"reading":controller.reading,"page_scroll":controller.page_scroll,
			"names":name_bytes,"player":controller.player.duplicate(true),
			"shiori":controller.shiori.duplicate(true)},
		"clock":{"engine_physics_tick":tick,"native_clock":_entry_clock.snapshot(),
			"model_frame_count":_entry_model.frame_count,"model_timer_ticks":_entry_model.timer_ticks,
			"model_system_seed":_entry_model.system_seed,"policy":entry_clock_policy.duplicate(true)},
		"rng":{"profile":rng.profile,"state":rng.state,"draw_count":rng.draw_count,"error":rng.error},
		"sampled_input":sampled_input,"queued_actions":queue.duplicate(true),
		"composed_image":composed,"failure":failure,
		"native_canvas":{"scale":[canvas.scale.x,canvas.scale.y],"position":[canvas.position.x,canvas.position.y],"viewport_size":[get_viewport_rect().size.x,get_viewport_rect().size.y]},
		"name_marker_draw_state":entry_images.markers.snapshot() if entry_images != null else {},
		"name_pointer":name_pointer.snapshot() if _source_pointer_ready() else {"available":false,"reason":"legacy media lacks source cursor style inventory"},
		"unavailable":["native_text_state","native_actor_memory","native_draw_order"] + ([] if title_error_free() else ["PS1_boot_clock_origin"])}

func _physics_process(delta: float) -> void:
	if not failure.is_empty() or focus_paused or _completion_emitted: return
	tick += 1
	_update_typing_toggle()
	# Desktop events are sampled once before dispatch. The native prior-draw pad
	# latch phase remains a differential-test question, not a parity assumption.
	var current_pad: InputFrame = _entry_pad.sample(_entry_clock.logic_iterations)
	var pointer_changed := false
	if _source_pointer_ready() and _name_input_active(): pointer_changed = name_pointer.advance(current_pad)
	controller.normalize_name_cursor()
	# NAME updates slot actors before handling the character-insertion edge.
	var marker_changed: bool = entry_images != null and entry_images.advance_markers(controller)
	var automatic: bool = (controller.stage == 1 and controller.step_id in [0,2,4,6,8]) or (controller.stage == 2 and controller.step_id in [0,2,4])
	if automatic:
		failure = controller.act({"kind":"tick"}); _render()
	else:
		# English-mode Latin typing feeds one native action per idle tick.
		if queue.is_empty():
			var typed: Dictionary = latin.next_action(controller)
			if not typed.is_empty(): queue.append(typed)
		if not queue.is_empty():
			var action: Dictionary = queue.pop_front()
			if action.get("kind") == "pointer_confirm": action = name_pointer.confirm(controller)
			var diagnostic: String = "" if action.is_empty() else controller.act(action)
			if not diagnostic.is_empty(): failure = diagnostic
			_render()
	if marker_changed: _render_native_canvas()
	elif pointer_changed: _render_native_canvas()
	# Record that exact device sample only after completed dispatch/render.
	var held_direction: int = current_pad.held & 0xf000
	if held_direction == 0 or (_source_pointer_ready() and _name_input_active()): navigation_direction = 0
	elif held_direction == navigation_direction:
		navigation_repeat += delta
		if navigation_repeat >= 0:
			_navigate_entry(held_direction)
			var settings := get_node_or_null("/root/ModernSettings")
			navigation_repeat = -float(settings.preferences.settings.controls.repeat_interval) if settings != null else -0.08
	var context_error: String = _entry_runtime.record_completed_iteration(current_pad, _entry_ui_brightness, _entry_residency)
	if not context_error.is_empty():
		failure = context_error
		return
	if profile_complete and not _completion_emitted and not installed_launch_profile.is_empty():
		var live: Dictionary = _entry_runtime.export_profile(controller, rng, profile_text_store)
		if not str(live.get("error", "")).is_empty():
			failure = str(live.error)
			return
		if not title_handoff.is_empty(): live.context["scene_work"] = int(title_handoff.scene_work)
		_completion_emitted = true
		profile_handoff = live
		profile_handoff["launch_profile"] = installed_launch_profile.duplicate(true)
		profile_finished.emit(controller, rng, installed_launch_profile.duplicate(true), live)

func _input(event: InputEvent) -> void:
	if not is_instance_valid(_entry_pad) or focus_paused: return
	# A click on the typing toggle belongs to the button, not to pointer confirm.
	if _over_typing_toggle(event): return
	if _name_input_active() and latin.push_event(event, _entry_pad.action_mask(event)):
		get_viewport().set_input_as_handled()
		_update_typing_toggle()
		return
	if event is InputEventMouseMotion and _source_pointer_ready():
		if name_pointer.place(Vector2i(canvas.get_local_mouse_position())): _render_native_canvas()
		return
	var mask: int = _entry_pad.action_mask(event)
	if mask == 0: return
	if _source_pointer_ready() and _name_input_active():
		if mask & 0x800: _send({"kind":"start"})
		elif mask & 0x40: _send({"kind":"cross"})
		elif mask & 0x20: _send({"kind":"pointer_confirm"})
		# Direction/Square state is consumed from the single mapped pad sample.
		get_viewport().set_input_as_handled()
		return
	# Primary pointer confirmation belongs to the native coordinate hit regions.
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and mask == 0x20: return
	if mask & 0x800: _send({"kind":"start"})
	elif mask & 0x40: _send({"kind":"cross"})
	elif mask & 0x20:
		if controller.stage == 1 and controller.step_id in [1,3,5]:
			if controller.cell(navigation_cell.x,navigation_cell.y).size() != 2: return
			navigation_visible = true
			_send({"kind":"cell", "column":navigation_cell.x, "row":navigation_cell.y})
		elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1): _send({"kind":"circle"})
		elif (controller.stage == 1 and controller.step_id == 9) or (controller.stage == 2 and controller.step_id == 3): _send({"kind":"yes"})
		else: return
	elif mask & 0xf000:
		var direction := mask & 0xf000
		if navigation_direction != direction:
			navigation_direction = direction
			var settings := get_node_or_null("/root/ModernSettings")
			navigation_repeat = -float(settings.preferences.settings.controls.repeat_delay) if settings != null else -0.4
			_navigate_entry(direction)
	elif mask & 0xc and controller.stage == 1 and controller.step_id in [1,3,5]:
		_send({"kind":"tab", "value":1 - controller.tab if controller.tab in [0,1] else 0})
	else: return
	get_viewport().set_input_as_handled()

func _navigate_entry(mask: int) -> void:
	navigation_visible = true
	if controller.stage == 1 and controller.step_id in [1,3,5]:
		var direction := Vector2i(1,0) if mask & 0x2000 else (Vector2i(-1,0) if mask & 0x8000 else (Vector2i(0,1) if mask & 0x4000 else Vector2i(0,-1)))
		for attempt: int in 102:
			navigation_cell = Vector2i(posmod(navigation_cell.x + direction.x, 17), posmod(navigation_cell.y + direction.y, 6))
			if controller.cell(navigation_cell.x, navigation_cell.y).size() == 2: break
	elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
		var field: int = controller.cursor
		var selected: int = controller.profile()[["month","day","blood"][field]]
		if selected == 255: selected = 0
		var columns: int = [6,11,4][field]
		var amount := 1 if mask & 0x2000 else (-1 if mask & 0x8000 else (columns if mask & 0x4000 else -columns))
		_send({"kind":"select", "value":posmod(selected + amount, [12,31,4][field])})
	if is_instance_valid(canvas): _native_controls()

func _native_button(rect: Rect2i, action: Dictionary, hint: String) -> void:
	var button := Button.new()
	button.position = rect.position
	button.size = rect.size
	button.tooltip_text = hint
	button.focus_mode = Control.FOCUS_NONE
	for state: String in ["normal","hover","pressed","focus","disabled"]:
		button.add_theme_stylebox_override(state,StyleBoxEmpty.new())
	button.pressed.connect(_send.bind(action))
	native_hits.add_child(button)

func _select_profile_field(field: int) -> void:
	# Use the existing controller actions so each field transition is recorded.
	for index: int in range(absi(field-controller.cursor)):
		_send({"kind":"circle" if field > controller.cursor else "cross"})

func _native_controls() -> void:
	if native_hits != null:
		canvas.remove_child(native_hits)
		native_hits.queue_free()
	native_hits = Control.new()
	native_hits.mouse_filter = Control.MOUSE_FILTER_IGNORE
	canvas.add_child(native_hits)
	if controller.stage == 1 and controller.step_id in [1,3,5]:
		_native_button(Rect2i(3,72,27,15),{"kind":"tab","value":0},"Hiragana")
		_native_button(Rect2i(3,104,27,15),{"kind":"tab","value":1},"Katakana")
		_native_button(Rect2i(3,136,27,15),{"kind":"tab","value":2},"Symbols")
		_native_button(Rect2i(3,168,27,15),{"kind":"tab","value":3},"Kanji")
		_native_button(Rect2i(3,200,27,15),{"kind":"done"},"Finish this name")
		var part: int = (controller.step_id-1)/2
		for index: int in (5 if part == 2 else 3):
			_native_button(Rect2i([57,113,233][part]+index*16,35,15,15),{"kind":"slot","value":[0,3,6][part]+index},"Edit name character")
		for row: int in range(6):
			for column: int in range(17):
				if controller.cell(column,row).size() == 2:
					_native_button(Rect2i(40+column*16,72+row*16,16,16),{"kind":"cell","column":column,"row":row},"")
		if controller.tab in [2,3]:
			_native_button(Rect2i(297,73,15,15),{"kind":"scroll","delta":-1},"Previous character row")
			_native_button(Rect2i(297,137,15,15),{"kind":"scroll","delta":1},"Next character row")
		if controller.tab == 3:
			for row: int in range(3):
				for column: int in range(17):
					_native_button(Rect2i(40+column*16,176+row*16,16,16),{"kind":"reading","column":column,"row":row},"")
	elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
		for field: int in range(3):
			var label := Button.new()
			label.position = [Vector2(40,64),Vector2(40,112),Vector2(40,176)][field]
			label.size = Vector2(64,16)
			label.focus_mode = Control.FOCUS_NONE
			for state: String in ["normal","hover","pressed","focus"]: label.add_theme_stylebox_override(state,StyleBoxEmpty.new())
			label.pressed.connect(_select_profile_field.bind(field))
			native_hits.add_child(label)
		var field: int = controller.cursor
		for index: int in [12,31,4][field]:
			var position: Vector2i
			if field == 0: position = Vector2i(56+(index%6)*40,80+(index/6)*16)
			elif field == 1: position = Vector2i(48+(index%11)*24,128+(index/11)*16)
			else: position = Vector2i(64+index*64,192)
			_native_button(Rect2i(position,Vector2i(24 if field == 1 else 32,16)),{"kind":"select","value":index},"")
		_native_button(Rect2i(3,200,27,15),{"kind":"start"},"Finish birthday and blood type")
	else:
		_native_button(Rect2i(112,200,64,16),{"kind":"yes"},"Confirm")
		_native_button(Rect2i(176,200,64,16),{"kind":"no"},"Edit")
	if navigation_visible:
		var outline := ReferenceRect.new(); outline.editor_only = false
		outline.border_color = Color(1,0.9,0.2); outline.border_width = 1.0; outline.mouse_filter = Control.MOUSE_FILTER_IGNORE
		if controller.stage == 1 and controller.step_id in [1,3,5]:
			outline.position = Vector2(40 + navigation_cell.x * 16,72 + navigation_cell.y * 16); outline.size = Vector2(16,16)
		elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
			outline.position = [Vector2(40,64),Vector2(40,112),Vector2(40,176)][controller.cursor]; outline.size = Vector2(64,16)
		else: outline.position = Vector2(112,200); outline.size = Vector2(64,16)
		native_hits.add_child(outline)

func _label(text: String) -> void:
	var label := Label.new(); label.text = text; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; panel.add_child(label)

func _button(parent: Node, text: String, action: Dictionary) -> Button:
	var button := Button.new(); button.text = text; button.custom_minimum_size = Vector2(44,34)
	button.pressed.connect(_send.bind(action)); parent.add_child(button)
	return button

func _glyph_texture(pair: PackedByteArray) -> Texture2D:
	var rendered: Dictionary = glyphs.get_glyph_image(pair,5)
	if rendered.get("status") != "glyph":
		failure = "Some game characters could not be loaded."
		print("T29012_ENTRY_GLYPH_ERROR "+str(rendered.get("diagnostic",rendered.get("status"))))
		return null
	return ImageTexture.create_from_image(rendered.image)

func _name_line() -> void:
	var row := HBoxContainer.new(); panel.add_child(row)
	for part in ["family","given","nickname"]:
		var label := Label.new(); label.text = {"family":"Family","given":"Given","nickname":"Nickname"}[part]+": "; row.add_child(label)
		var data: PackedByteArray = controller.names[part]
		for i in range(0,data.size(),2):
			if data[i] == 0: break
			var glyph := TextureRect.new(); glyph.texture = _glyph_texture(data.slice(i,i+2)); glyph.custom_minimum_size = Vector2(24,24); row.add_child(glyph)

func _valid_glyph_hex(raw: PackedByteArray, max_glyphs: int = 8) -> String:
	if presenter == null or presenter._font == null:
		return ""
	var out_hex: String = ""
	var count: int = 0
	for i: int in range(0, raw.size(), 2):
		if i + 1 >= raw.size() or raw[i] == 0 or count >= max_glyphs:
			break
		var pair: PackedByteArray = raw.slice(i, i + 2)
		if presenter._font.get_glyph_image(pair, 0, 0).get("status") == "glyph":
			out_hex += pair.hex_encode()
			count += 1
	return out_hex

func _render_native_canvas() -> void:
	_ensure_native_presenter()
	if presenter == null or canvas == null:
		return
	if entry_images == null:
		return
	presenter.recorded_glyphs.clear()
	presenter.bounding_box_violations.clear()
	var settings := get_node_or_null("/root/ModernSettings")
	entry_images.translations = settings.translated_images if settings != null else null
	entry_images.pointer = name_pointer if _source_pointer_ready() else null
	var img := entry_images.render(controller)
	if img == null: return

	last_rendered_image = img
	display_host.present(img, "name_entry")

func _render() -> void:
	profile_complete = (controller.stage == 2 and controller.step_id == 5)
	panel.visible = true
	for child in panel.get_children(): panel.remove_child(child); child.queue_free()
	_label("New Game")
	if not failure.is_empty():
		_label(failure)
		_recovery_actions()
		return
	_render_native_canvas()
	if entry_images != null and not debug_visible:
		panel.hide()
		_native_controls()
		return
	if debug_visible: _label("Stage %d / step %d · RNG draws %d"%[controller.stage,controller.step_id,rng.draw_count])
	_name_line()
	if not failure.is_empty():
		_label(failure)
		_recovery_actions()
		return
	if controller.stage == 1 and controller.step_id in [1,3,5]:
		_label("Enter your "+{"family":"family name","given":"given name","nickname":"nickname"}[controller.active_part()])
		var tabs := HBoxContainer.new(); panel.add_child(tabs)
		_button(tabs,"Hiragana",{"kind":"tab","value":0}); _button(tabs,"Katakana",{"kind":"tab","value":1})
		var grid := GridContainer.new(); grid.columns = 17; panel.add_child(grid)
		for y in 6:
			for x in 17:
				var pair: PackedByteArray = controller.cell(x,y)
				var button: Button = _button(grid,"",{"kind":"cell","column":x,"row":y})
				button.custom_minimum_size = Vector2(42,36)
				if pair.size() == 2: button.icon = _glyph_texture(pair); button.expand_icon = true
				else: button.disabled = true
		var actions := HBoxContainer.new(); panel.add_child(actions)
		_button(actions,"Back",{"kind":"cross"}); _button(actions,"Done",{"kind":"done"})

	elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
		var who: String = "Player" if controller.stage == 1 else "Shiori"
		_label(who+" · "+["Month","Day","Blood type"][controller.cursor])
		var grid := GridContainer.new(); grid.columns = 11; panel.add_child(grid)
		for i in [12,31,4][controller.cursor]:
			_button(grid,["A","B","AB","O"][i] if controller.cursor == 2 else str(i+1),{"kind":"select","value":i})
		_label(profile_text(controller.profile()))
		var actions := HBoxContainer.new(); panel.add_child(actions)
		_button(actions,"Back",{"kind":"cross"}); _button(actions,"Next",{"kind":"circle"}); _button(actions,"Finish",{"kind":"start"})
	elif (controller.stage == 1 and controller.step_id == 9) or (controller.stage == 2 and controller.step_id == 3):
		_label(("Player" if controller.stage == 1 else "Shiori")+" · "+profile_text(controller.profile()))
		var actions := HBoxContainer.new(); panel.add_child(actions)
		_button(actions,"Confirm",{"kind":"yes"}); _button(actions,"Edit",{"kind":"no"})
	elif controller.stage == 2 and controller.step_id == 5:
		_label("Profile complete.")
		_label("The playable opening is not available in this build yet.")
		_recovery_actions()
		if debug_visible:
			_label(controller.blocked)
			_label("3,823 initialized bytes; 2,973 unowned bytes remain unknown. No command boundary is ready.")

func _recovery_actions() -> void:
	var actions := HBoxContainer.new(); panel.add_child(actions)
	var setup := Button.new(); setup.text = "Return to setup"; actions.add_child(setup)
	setup.pressed.connect(func() -> void: Launch.change_scene(self, Launch.SETUP_SCENE, {}))
	var exit := Button.new(); exit.text = "Exit"; actions.add_child(exit)
	exit.pressed.connect(func() -> void: get_tree().quit())

static func profile_text(profile: Dictionary) -> String:
	var month: String = "Not set" if profile.month == 255 else str(int(profile.month)+1)
	var day: String = "Not set" if profile.day == 255 else str(int(profile.day)+1)
	var blood: String = "Not set" if profile.blood == 255 else ["A","B","AB","O"][int(profile.blood)]
	return "Birthday: %s/%s   Blood type: %s"%[month,day,blood]
