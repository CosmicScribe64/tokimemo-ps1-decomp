extends Control
## T-30002 WP8: modern pause menu (Resume, Settings, Quick save, Load, Quit;
## Return to title when the scene supports it). In a game scene, meaning one
## with a registered save host, F10 or L3+R3 open this menu instead of
## Settings and pause the tree. Outside a game scene the keys keep opening
## Settings. Native inputs never leak: devices are cleared on open and close,
## and the tree stays paused while either menu is shown.
const Widgets = preload("res://ui/settings/settings_widgets.gd")
const STICKS := {JOY_BUTTON_LEFT_STICK: 1, JOY_BUTTON_RIGHT_STICK: 2}

var runtime: Node
var focus_items: Dictionary = {}
var notice: Label
var quit_confirm: HBoxContainer
var previous_pause := false
var _held: Dictionary = {}
var _latched: Dictionary = {}
var _return_after_settings := false

## Adds the pause menu to the settings runtime's modern layer. The settings
## menu calls this (deferred) from its _ready; a second call returns the
## existing menu.
static func install(owner: Node) -> Control:
	var existing: Node = owner.layer.get_node_or_null("PauseMenu")
	if existing != null: return existing
	var menu: Control = load("res://ui/pause_menu.gd").new()
	menu.runtime = owner
	menu.name = "PauseMenu"
	owner.layer.add_child(menu)
	menu.hide()
	return menu

func _ready() -> void:
	set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	mouse_filter = Control.MOUSE_FILTER_STOP
	process_mode = Node.PROCESS_MODE_ALWAYS
	theme = Widgets.settings_theme()

func in_game() -> bool:
	return runtime.save_host() != null

func _build() -> void:
	for child: Node in get_children(): remove_child(child); child.queue_free()
	focus_items.clear()
	var widgets := Widgets.new(); widgets.focus_items = focus_items
	var shade := ColorRect.new(); shade.color = Color(0.01, 0.025, 0.025, 0.7); shade.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(shade)
	var center := CenterContainer.new(); center.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(center)
	var card := PanelContainer.new(); center.add_child(card)
	var box := VBoxContainer.new(); box.add_theme_constant_override("separation", 8); box.custom_minimum_size.x = 280; card.add_child(box)
	widgets.heading(box, "Paused")
	widgets.button(box, "Resume", resume)
	widgets.button(box, "Settings", open_settings)
	widgets.button(box, "Quick save", func() -> void: _save(false))
	widgets.button(box, "Load quick save", func() -> void: _save(true))
	var host: Node = runtime.save_host()
	if host != null and host.has_method("request_return_to_title"):
		widgets.button(box, "Return to title", func() -> void: resume(); host.request_return_to_title())
	widgets.button(box, "Quit game", func() -> void: quit_confirm.show(); (focus_items["quit_confirm"] as Control).grab_focus())
	quit_confirm = HBoxContainer.new(); quit_confirm.add_theme_constant_override("separation", 8); box.add_child(quit_confirm)
	widgets.button(quit_confirm, "Quit now", func() -> void: get_tree().quit(0), "quit_confirm")
	widgets.button(quit_confirm, "Keep playing", func() -> void: quit_confirm.hide(); (focus_items["Quit game"] as Control).grab_focus(), "quit_cancel")
	quit_confirm.hide()
	notice = widgets.note(box, "Esc, B or right-click resumes.")
	(focus_items["Resume"] as Control).call_deferred("grab_focus")

func open() -> void:
	if visible: return
	previous_pause = get_tree().paused
	get_tree().paused = true
	_clear_inputs()
	_build(); show()

func resume() -> void:
	if not visible: return
	hide()
	_clear_inputs()
	get_tree().paused = previous_pause

func open_settings() -> void:
	hide(); _return_after_settings = true
	if not runtime.menu.visible: runtime.toggle_menu()

func _save(load_game: bool) -> void:
	var host: Node = runtime.save_host()
	if host == null:
		notice.text = "Save/load is unavailable in this scene."; return
	resume()
	host.request_settings_save(load_game)

func _clear_inputs() -> void:
	for action: String in ["ui_up", "ui_down", "ui_left", "ui_right", "ui_accept", "ui_cancel"]:
		if InputMap.has_action(action): Input.action_release(action)
	get_tree().call_group("native_input_devices", "clear_devices")

func _process(_delta: float) -> void:
	if _return_after_settings and not runtime.menu.visible:
		_return_after_settings = false
		# Settings restored the paused tree it found. A quick save or load
		# chosen there resumes the game; otherwise return to this menu.
		if bool(runtime.menu.get("closing_for_save")):
			runtime.menu.closing_for_save = false
			_clear_inputs(); get_tree().paused = previous_pause
		elif in_game(): _build(); show()
		else: get_tree().paused = previous_pause

func _input(event: InputEvent) -> void:
	if runtime == null: return
	if event is InputEventJoypadButton and STICKS.has(event.button_index):
		_chord(event); return
	if runtime.menu.visible: return
	if event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_F10:
		if visible: resume(); accept_event()
		elif in_game(): open(); accept_event()
		return
	if not visible: return
	if (event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_ESCAPE) \
			or (event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_RIGHT) \
			or (event is InputEventJoypadButton and event.pressed and event.is_action_pressed("ui_cancel")):
		if quit_confirm.visible:
			quit_confirm.hide(); (focus_items["Quit game"] as Control).grab_focus()
		else: resume()
		accept_event()

## L3+R3 opens or closes the pause menu in game. The completing press is
## consumed so the settings runtime does not also open Settings.
func _chord(event: InputEventJoypadButton) -> void:
	var held: int = _held.get(event.device, 0)
	var bit: int = STICKS[event.button_index]
	held = (held | bit) if event.pressed else (held & ~bit)
	_held[event.device] = held
	if held == 0: _latched[event.device] = false
	if held == 3 and not _latched.get(event.device, false) and not runtime.menu.visible and (visible or in_game()):
		_latched[event.device] = true
		if visible: resume()
		else: open()
		accept_event()
