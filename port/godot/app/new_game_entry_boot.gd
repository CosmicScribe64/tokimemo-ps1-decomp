extends Control
## Separate development entry scene. Does not replace the captured school-day app.
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
var controller := Entry.new()
var glyphs := Glyphs.new()
var rng := Rng.new()
var queue: Array[Dictionary] = []
var panel: VBoxContainer
var failure: String = ""
var tick: int = 0
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
	var tables: Dictionary = Rules.load_tables(OS.get_environment("TOKIMEMO_NEW_GAME_RULES"))
	var content: Dictionary = Entry.load_content(OS.get_environment("TOKIMEMO_NEW_GAME_ENTRY"))
	var seed: String = OS.get_environment("TOKIMEMO_NEW_GAME_SEED")
	if seed.is_empty() or not seed.is_valid_int(): failure = "Provide an explicit development RNG seed; original file-select system-data seeding is not implemented here."
	if failure.is_empty(): failure = glyphs.open_external_font(OS.get_environment("TOKIMEMO_NATIVE_FONT"))
	if failure.is_empty():
		rng.configure(Rng.OPENBIOS_PROFILE,int(seed)); failure = controller.begin(tables,content,rng)
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

func _unhandled_key_input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed and not event.echo:
		if event.keycode == KEY_ENTER: _send({"kind":"start"})
		elif event.keycode == KEY_ESCAPE: _send({"kind":"cross"})

func _label(text: String) -> void:
	var label := Label.new(); label.text = text; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; panel.add_child(label)

func _button(parent: Node, text: String, action: Dictionary) -> Button:
	var button := Button.new(); button.text = text; button.custom_minimum_size = Vector2(44,34)
	button.pressed.connect(_send.bind(action)); parent.add_child(button)
	return button

func _glyph_texture(pair: PackedByteArray) -> Texture2D:
	var rendered: Dictionary = glyphs.get_glyph_image(pair,5)
	if rendered.get("status") != "glyph":
		failure = "Required entry glyph is unavailable: "+str(rendered.get("diagnostic",rendered.get("status")))
		return null
	return ImageTexture.create_from_image(rendered.image)

func _name_line() -> void:
	var row := HBoxContainer.new(); panel.add_child(row)
	for part in ["family","given","nickname"]:
		var label := Label.new(); label.text = part+": "; row.add_child(label)
		var data: PackedByteArray = controller.names[part]
		for i in range(0,data.size(),2):
			if data[i] == 0: break
			var glyph := TextureRect.new(); glyph.texture = _glyph_texture(data.slice(i,i+2)); glyph.custom_minimum_size = Vector2(24,24); row.add_child(glyph)

func _render() -> void:
	for child in panel.get_children(): panel.remove_child(child); child.queue_free()
	_label("New Game entry — development action flow")
	_label("Original Japanese kana/presets; Enter = START, Escape = CROSS. Native timing/art/audio is not reproduced by this scene.")
	if not failure.is_empty(): _label("Stopped: "+failure); return
	_label("Native stage %d / step %d • RNG draws %d"%[controller.stage,controller.step_id,rng.draw_count])
	_name_line()
	if not failure.is_empty(): _label("Stopped: "+failure); return
	if controller.stage == 1 and controller.step_id in [1,3,5]:
		_label("Enter "+controller.active_part()+"; native slot "+str(controller.cursor))
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
		_button(actions,"Back one slot",{"kind":"cross"}); _button(actions,"Done / START",{"kind":"done"})
		_label("Symbols, kanji readings, preset chords and native pointer grid are pending original-route coverage.")
	elif (controller.stage == 1 and controller.step_id == 7) or (controller.stage == 2 and controller.step_id == 1):
		var who: String = "Player" if controller.stage == 1 else "Shiori"
		_label(who+" profile — "+["month","day","blood type"][controller.cursor])
		var grid := GridContainer.new(); grid.columns = 11; panel.add_child(grid)
		for i in [12,31,4][controller.cursor]:
			_button(grid,["A","B","AB","O"][i] if controller.cursor == 2 else str(i+1),{"kind":"select","value":i})
		_label("Selections (native zero-based indices): "+str(controller.profile()))
		var actions := HBoxContainer.new(); panel.add_child(actions)
		_button(actions,"Back field",{"kind":"cross"}); _button(actions,"Next field / CIRCLE",{"kind":"circle"}); _button(actions,"Finish / START",{"kind":"start"})
	elif (controller.stage == 1 and controller.step_id == 9) or (controller.stage == 2 and controller.step_id == 3):
		_label(("Player" if controller.stage == 1 else "Shiori")+" confirmation: "+str(controller.profile()))
		var actions := HBoxContainer.new(); panel.add_child(actions)
		_button(actions,"Yes / START",{"kind":"yes"}); _button(actions,"No / CROSS",{"kind":"no"})
	elif controller.stage == 2 and controller.step_id == 5:
		_label("Native profile setup committed independently from entered data.")
		_label("Logical projection: 3,823 initialized bytes; 2,973 unowned bytes remain unknown. This is not native boot or a ready session.")
		_label(controller.blocked)
		_label("No captured gameplay snapshot was loaded. Title/file-select seed, native load/fade, voiced prologue and session adapter remain required.")
