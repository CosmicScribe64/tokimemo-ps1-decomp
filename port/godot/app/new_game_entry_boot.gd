extends Control
## Separate development entry scene. Does not replace the captured school-day app.
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const Diagnostics = preload("res://services/new_game_load_diagnostics.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
var controller := Entry.new()
var glyphs := Glyphs.new()
var rng := Rng.new()
var queue: Array[Dictionary] = []
var panel: VBoxContainer
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
	if focus_paused: queue.clear()

func _ready() -> void:
	get_window().content_scale_size = Vector2i(1024,768)
	get_window().size = Vector2i(1024,768)
	panel = VBoxContainer.new(); panel.position = Vector2(24,16); panel.size = Vector2(950,660); add_child(panel)
	debug_visible = OS.get_environment("TOKIMEMO_ENTRY_DEBUG") == "1"
	get_window().title = "New Game"+(" [%d]"%OS.get_process_id() if debug_visible else "")
	var rules_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_RULES")
	var entry_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_ENTRY")
	var tables: Dictionary = Rules.load_tables(rules_path)
	var content: Dictionary = Entry.load_content(entry_path)
	var seed: String = OS.get_environment("TOKIMEMO_NEW_GAME_SEED")
	load_diagnostics = {"build":"entry-ui-cleanup-v1","pid":OS.get_process_id(),"executable":OS.get_executable_path(),
		"rules":Diagnostics.inspect(rules_path,Rules._content_valid(tables)),"entry":Diagnostics.inspect(entry_path,Entry.content_valid(content)),"seed_valid":not seed.is_empty() and seed.is_valid_int(),"failed_gate":""}
	if not Rules._content_valid(tables):
		failure = "Game data could not be loaded. Check the rule pack."; load_diagnostics.failed_gate="rule_pack"
	elif not Entry.content_valid(content):
		failure = "Game data could not be loaded. Check the name-entry pack."; load_diagnostics.failed_gate="entry_pack"
	elif tables.space != content.space:
		failure = "The game-data packs do not match."; load_diagnostics.failed_gate="pack_space_identity"
	elif not load_diagnostics.seed_valid:
		failure = "A startup seed is required."; load_diagnostics.failed_gate="startup_seed"
	if failure.is_empty():
		var font_error: String = glyphs.open_external_font(OS.get_environment("TOKIMEMO_NATIVE_FONT"))
		if not font_error.is_empty():
			failure="The game font could not be loaded. Check the font pack.";load_diagnostics.failed_gate="font_pack";load_diagnostics.font_error=font_error
	if failure.is_empty():
		if not rng.configure(Rng.OPENBIOS_PROFILE,int(seed)):
			failure="The startup random source could not be configured.";load_diagnostics.failed_gate="rng_profile"
		else:
			var entry_error: String=controller.begin(tables,content,rng)
			if not entry_error.is_empty(): failure="New Game could not start.";load_diagnostics.failed_gate="entry_begin";load_diagnostics.entry_error=entry_error
	if debug_visible or not failure.is_empty(): print("T29012_ENTRY_LOAD_DIAGNOSTIC "+JSON.stringify(load_diagnostics))
	_render()

func _send(action: Dictionary) -> void:
	if not focus_paused: queue.append(action.duplicate(true))

func _physics_process(_delta: float) -> void:
	if not failure.is_empty() or focus_paused: return
	tick += 1
	var automatic: bool = (controller.stage == 1 and controller.step_id in [0,2,4,6,8]) or (controller.stage == 2 and controller.step_id in [0,2,4])
	if automatic:
		failure = controller.act({"kind":"tick"}); _render()
	elif not queue.is_empty():
		var action: Dictionary = queue.pop_front()
		var diagnostic: String = controller.act(action)
		if not diagnostic.is_empty(): failure = diagnostic
		_render()

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

func _render() -> void:
	for child in panel.get_children(): panel.remove_child(child); child.queue_free()
	_label("New Game")
	if not failure.is_empty(): _label(failure); return
	if debug_visible: _label("Stage %d / step %d · RNG draws %d"%[controller.stage,controller.step_id,rng.draw_count])
	_name_line()
	if not failure.is_empty(): _label(failure); return
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
		_label("The prologue is not available yet.")
		if debug_visible:
			_label(controller.blocked)
			_label("3,823 initialized bytes; 2,973 unowned bytes remain unknown. No command boundary is ready.")

static func profile_text(profile: Dictionary) -> String:
	var month: String = "Not set" if profile.month == 255 else str(int(profile.month)+1)
	var day: String = "Not set" if profile.day == 255 else str(int(profile.day)+1)
	var blood: String = "Not set" if profile.blood == 255 else ["A","B","AB","O"][int(profile.blood)]
	return "Birthday: %s/%s   Blood type: %s"%[month,day,blood]
