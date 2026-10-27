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
const LaunchProfile = preload("res://services/installed_launch_profile.gd")
const Launch = preload("res://app/installed_launch.gd")
signal profile_finished(entry: RefCounted, native_rng: RefCounted, launch_profile: Dictionary, live_context: Dictionary)
# Injected by the installed launch owner after original resource authentication.
var profile_text_store: TextStore
var entry_clock_policy: Dictionary = {}
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
var canvas: Canvas
var presenter: Presenter
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
		if is_instance_valid(_entry_pad): _entry_pad.clear_devices()

func _ensure_native_presenter() -> void:
	if canvas == null:
		canvas = Canvas.new()
		canvas.scale = Vector2(2, 2)
		canvas.z_index = -10
		add_child(canvas)
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
	var hud_manifest: String = str(installed_launch_profile.get("paths", {}).get("hud_manifest", ""))
	if hud_manifest.is_empty() and installed_launch_profile.is_empty():
		hud_manifest = OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	if not installed_launch_profile.is_empty() and hud_manifest.is_empty():
		return
	var cand_presenter := Presenter.new()
	if cand_presenter.init_resources(font_path, hud_manifest).is_empty():
		presenter = cand_presenter

func _ready() -> void:
	_entry_pad = EntryPad.new()
	add_child(_entry_pad)
	# Fresh desktop entry origin; earlier native boot/title duration is unmeasured.
	# These are source initialization operations, never derived from startup RNG.
	_entry_model.frame_count = 0 # game_state_init; desktop origin recorded below.
	_entry_model.system_seed = 0 # sys_init bzero before timer callbacks.
	_entry_model.timer_ticks = 0
	_entry_ui_brightness = 128 # draw_brightness_reset; current entry renderer is full-bright.
	var context_error: String = _entry_runtime.bind(_entry_model, _entry_clock, _entry_pad, entry_clock_policy)
	if not context_error.is_empty(): failure = context_error
	get_window().content_scale_size = Vector2i(1024,768)
	get_window().size = Vector2i(1024,768)
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
	var seed: String = OS.get_environment("TOKIMEMO_NEW_GAME_SEED")
	if not installed_launch_profile.is_empty():
		# The desktop startup clock supplies the initial BIOS LCG state. Record it
		# once; the same live RNG object is retained throughout profile entry.
		seed = str(Time.get_ticks_usec() & 0xffffffff)
	load_diagnostics = {"build":"entry-ui-cleanup-v1","pid":OS.get_process_id(),"executable":OS.get_executable_path(),
		"rules":Diagnostics.inspect(rules_path,Rules._content_valid(tables)),"entry":Diagnostics.inspect(entry_path,Entry.content_valid(content)),"seed_valid":not seed.is_empty() and seed.is_valid_int(),"failed_gate":""}
	load_diagnostics["startup_seed"] = int(seed) if seed.is_valid_int() else -1
	if not Rules._content_valid(tables):
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
		if not rng.configure(Rng.OPENBIOS_PROFILE,int(seed)):
			failure="The startup random source could not be configured.";load_diagnostics.failed_gate="rng_profile"
		else:
			var entry_error: String=controller.begin(tables,content,rng)
			if not entry_error.is_empty(): failure="New Game could not start.";load_diagnostics.failed_gate="entry_begin";load_diagnostics.entry_error=entry_error
			else:
				_entry_residency = {"lba":0x95ea,"source_sha256":ContextOwner.NAME_ENT_SHA,"content_sha256":Entry.CONTENT_SHA,"ready":true}
	if debug_visible: print("T29012_ENTRY_LOAD_DIAGNOSTIC "+JSON.stringify(load_diagnostics))
	_render()

func _send(action: Dictionary) -> void:
	if not focus_paused: queue.append(action.duplicate(true))

func _physics_process(_delta: float) -> void:
	if not failure.is_empty() or focus_paused or _completion_emitted: return
	tick += 1
	var automatic: bool = (controller.stage == 1 and controller.step_id in [0,2,4,6,8]) or (controller.stage == 2 and controller.step_id in [0,2,4])
	if automatic:
		failure = controller.act({"kind":"tick"}); _render()
	elif not queue.is_empty():
		var action: Dictionary = queue.pop_front()
		var diagnostic: String = controller.act(action)
		if not diagnostic.is_empty(): failure = diagnostic
		_render()
	# Snapshot the device owner after the completed entry dispatch/render.
	var current_pad: InputFrame = _entry_pad.sample(_entry_clock.logic_iterations)
	var context_error: String = _entry_runtime.record_completed_iteration(current_pad, _entry_ui_brightness, _entry_residency)
	if not context_error.is_empty():
		failure = context_error
		return
	if profile_complete and not _completion_emitted and not installed_launch_profile.is_empty():
		var live: Dictionary = _entry_runtime.export_profile(controller, rng, profile_text_store)
		if not str(live.get("error", "")).is_empty():
			failure = str(live.error)
			return
		_completion_emitted = true
		profile_handoff = live
		profile_handoff["launch_profile"] = installed_launch_profile.duplicate(true)
		profile_finished.emit(controller, rng, installed_launch_profile.duplicate(true), live)

func _input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed and not event.echo:
		if event.keycode == KEY_ENTER:
			_send({"kind":"start"}); get_viewport().set_input_as_handled()
		elif event.keycode == KEY_ESCAPE:
			_send({"kind":"cross"}); get_viewport().set_input_as_handled()

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
	var img := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	presenter.recorded_glyphs.clear()
	presenter.bounding_box_violations.clear()

	# 1. Authentic PS1 32x32 green checkerboard background (hud_screen_frame_draw)
	presenter.draw_checkerboard_frame(img, 0, Vector2i((tick / 2) & 31, (tick / 2) & 31), 0)

	# 2. Authentic PS1 POLY_G4 case 2 gradient base panel (cal_base_show) & bottom message window
	presenter.draw_cal_base(img, Rect2i(16, 36, 288, 120), 224)
	presenter.draw_message_window(img)

	# 3. Top-left wooden title badge (message_window_title_update)
	var badge_hex: String = _valid_glyph_hex(controller.names.get("family", PackedByteArray()), 3)
	if badge_hex.is_empty() and presenter._manifest.get("content", {}).get("strings", {}).has("narration_prefix"):
		badge_hex = str(presenter._manifest["content"]["strings"]["narration_prefix"].get("raw_hex", "")).substr(0, 8)
	presenter.draw_title_badge(img, badge_hex, Vector2i.ZERO, maxi(2, badge_hex.length() / 4))

	# 4. Entered player/Shiori name glyphs on the cal_base panel
	var x_positions: Dictionary = {"family": 28, "given": 112, "nickname": 196}
	for part: String in ["family", "given", "nickname"]:
		if not controller.names.has(part):
			continue
		var part_hex: String = _valid_glyph_hex(controller.names[part], 5)
		if not part_hex.is_empty():
			presenter._draw_text_hex_checked(img, part_hex, Vector2i(int(x_positions[part]), 48), 0, "name_entry")

	# Draw kana grid sample or birthday/blood-type selection rows inside cal_base
	if controller.stage == 1 and controller.step_id in [1, 3, 5]:
		for row_idx: int in range(4):
			var row_bytes := PackedByteArray()
			for col_idx: int in range(8):
				var cell_pair: PackedByteArray = controller.cell(col_idx, row_idx)
				if cell_pair.size() == 2:
					row_bytes.append_array(cell_pair)
			var row_hex: String = _valid_glyph_hex(row_bytes, 8)
			if not row_hex.is_empty():
				presenter._draw_text_hex_checked(img, row_hex, Vector2i(28, 72 + row_idx * 18), 0, "name_grid")

	# 5. Birthday/blood-type and stage prompt inside the message window
	var prompt_hex: String = ""
	if presenter._manifest.get("content", {}).get("strings", {}).has("ordinary_command"):
		prompt_hex = str(presenter._manifest["content"]["strings"]["ordinary_command"].get("raw_hex", "")).substr(0, 32)
	if not prompt_hex.is_empty():
		presenter.draw_vn_dialogue(img, "", [prompt_hex])

	# 6. Authentic PS1 hand cursor (Frame 156) at controller.cursor
	var cursor_pos := Vector2i(36 + clampi(controller.cursor, 0, 11) * 18, 62)
	if (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
		cursor_pos = Vector2i(48 + clampi(controller.cursor, 0, 2) * 80, 104)
	elif (controller.stage == 1 and controller.step_id == 9) or (controller.stage == 2 and controller.step_id in [3, 5]):
		cursor_pos = Vector2i(72 + clampi(controller.cursor, 0, 1) * 80, 124)
	presenter.draw_hand_cursor(img, cursor_pos)

	last_rendered_image = img
	canvas.set("_textures", {})
	canvas.present([{
		"id": "new_game_entry:%d:%d:%d" % [controller.stage, controller.step_id, tick],
		"image": img,
		"position": Vector2i.ZERO,
		"blend_mode": 0,
	}])

func _render() -> void:
	profile_complete = (controller.stage == 2 and controller.step_id == 5)
	for child in panel.get_children(): panel.remove_child(child); child.queue_free()
	_label("New Game")
	if not failure.is_empty():
		_label(failure)
		_recovery_actions()
		return
	_render_native_canvas()
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
