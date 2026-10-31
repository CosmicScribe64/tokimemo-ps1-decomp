extends RefCounted
## T-30002 WP6: shared control builders for the settings shell and its pages.
## Every interactive control is registered under a stable focus key so the
## shell can restore focus after a rebuild and tests can address controls.

const CHOICE_NAMES := {"original": "Original (4:3)", "wide": "Widescreen", "auto": "Automatic",
	"windowed": "Windowed", "borderless": "Borderless fullscreen", "exclusive": "Exclusive fullscreen",
	"integer": "Sharp whole pixels", "nearest": "Sharp fill", "linear": "Smooth fill",
	"read": "Read text only", "all": "All text", "slow": "Slow", "normal": "Normal (original)",
	"fast": "Fast", "instant": "Instant", "black": "Black bars", "source": "Continue scene pattern",
	"native": "Original text window", "modern": "Modern text box", "keyboard": "Keyboard",
	"xbox": "Xbox", "playstation": "PlayStation", "nintendo": "Nintendo"}
const TAG_COLOR := Color("e9c77d")
const NOTE_COLOR := Color("a7bbb2")
const PARITY_COLOR := Color("9fd0e8")
const Registry = preload("res://services/feature_registry.gd")
const Parity = preload("res://services/parity_mode.gd")

var focus_items: Dictionary = {}

## T-39230: in Parity mode every row whose registry entries are pinned or
## refused is disabled (it stays focusable so the badge can be read) and
## carries a "Parity" badge. `forced` also locks the play mode row, which a
## launch option owns for the session. Returns the locked focus keys.
func lock_parity_rows(active: bool, forced: bool = false) -> Array[String]:
	var locked: Array[String] = []
	if not active: return locked
	for key: String in focus_items:
		var ids: Array = Registry.row_entries(key)
		if not Registry.row_locked_in_parity(key) and not (forced and "profile.mode" in ids): continue
		var control: Control = focus_items[key]
		if control is BaseButton: (control as BaseButton).disabled = true
		elif control is Range: (control as Range).editable = false
		control.set_meta("parity_locked", true)
		control.tooltip_text = Parity.FORCED_REFUSAL if forced and "profile.mode" in ids else Parity.REFUSAL
		_parity_badge(control)
		locked.append(key)
	return locked

## Buttons get the badge in their text; option and slider rows get a badge
## label in their title row, like the existing "[opening only]" tags.
func _parity_badge(control: Control) -> void:
	var row: Node = null
	if control is OptionButton: row = control.get_parent()
	elif control is Range and control.get_parent() != null and control.get_parent().get_child_count() > 0:
		row = control.get_parent().get_child(0)
	if row is HBoxContainer:
		var badge := Label.new(); badge.text = "[" + Parity.BADGE + "]"
		badge.add_theme_color_override("font_color", PARITY_COLOR); badge.add_theme_font_size_override("font_size", 14)
		badge.set_meta("settings_tag", Parity.BADGE); badge.set_meta("parity_badge", true)
		row.add_child(badge)
		if control is OptionButton: row.move_child(badge, control.get_index())
	elif control is Button:
		(control as Button).text += "  [" + Parity.BADGE + "]"

func register(control: Control, key: String) -> void:
	control.set_meta("settings_focus_key", key)
	control.focus_mode = Control.FOCUS_ALL
	focus_items[key] = control

func heading(parent: Node, title: String) -> Label:
	var label := Label.new(); label.text = title
	label.add_theme_font_size_override("font_size", 21); label.add_theme_color_override("font_color", TAG_COLOR)
	parent.add_child(label); return label

func subheading(parent: Node, title: String) -> Label:
	var label := Label.new(); label.text = title
	label.add_theme_font_size_override("font_size", 17); label.add_theme_color_override("font_color", TAG_COLOR)
	parent.add_child(label); return label

## Plain wrapped text. Used for one-line explanations and read-only rows.
func note(parent: Node, text: String, key: String = "") -> Label:
	var label := Label.new(); label.text = text
	label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	label.add_theme_color_override("font_color", NOTE_COLOR); label.add_theme_font_size_override("font_size", 14)
	if not key.is_empty(): label.set_meta("settings_note_key", key)
	parent.add_child(label); return label

func info(parent: Node, text: String, key: String = "") -> Label:
	var label := Label.new(); label.text = text; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	if not key.is_empty(): label.set_meta("settings_note_key", key)
	parent.add_child(label); return label

func button(parent: Node, title: String, callback: Callable, key: String = "") -> Button:
	var item := Button.new(); item.text = title; item.custom_minimum_size.y = 34
	parent.add_child(item); item.pressed.connect(callback)
	register(item, title if key.is_empty() else key); return item

## Label + OptionButton row. `tag` is appended to the label, such as "opening only".
func option(parent: Node, title: String, choices: Array, selected: Variant, callback: Callable, tag: String = "", names: Dictionary = {}) -> OptionButton:
	var row := HBoxContainer.new(); row.add_theme_constant_override("separation", 10); parent.add_child(row)
	_row_label(row, title, tag)
	var item := OptionButton.new(); item.custom_minimum_size = Vector2(200, 34); row.add_child(item)
	var available := choices.duplicate()
	if selected not in available: available.append(selected)
	for choice: Variant in available:
		item.add_item(str(names.get(choice, CHOICE_NAMES.get(choice, str(choice).capitalize()))))
	item.select(available.find(selected))
	item.item_selected.connect(func(i: int) -> void: callback.call(available[i]))
	register(item, title); return item

func toggle(parent: Node, title: String, value: bool, callback: Callable, tag: String = "") -> CheckButton:
	var item := CheckButton.new(); item.text = title + ("" if tag.is_empty() else "  [" + tag + "]")
	item.button_pressed = value; item.custom_minimum_size.y = 34
	parent.add_child(item); item.toggled.connect(callback); register(item, title); return item

func slider(parent: Node, title: String, value: float, low: float, high: float, step: float, callback: Callable, tag: String = "", key: String = "") -> HSlider:
	var block := VBoxContainer.new(); block.add_theme_constant_override("separation", 3); parent.add_child(block)
	var row := HBoxContainer.new(); block.add_child(row)
	_row_label(row, title, tag)
	var number := Label.new(); number.add_theme_color_override("font_color", TAG_COLOR)
	number.text = value_text(title, value); number.horizontal_alignment = HORIZONTAL_ALIGNMENT_RIGHT
	number.custom_minimum_size.x = 60; row.add_child(number)
	var item := HSlider.new(); item.custom_minimum_size.y = 22
	item.min_value = low; item.max_value = high; item.step = step; item.value = value; block.add_child(item)
	item.value_changed.connect(func(v: float) -> void: number.text = value_text(title, v); callback.call(v))
	register(item, title if key.is_empty() else key); return item

static func value_text(title: String, value: float) -> String:
	if title.contains("repeat") or title.contains("reading") or title.contains("seconds"): return "%.2f s" % value
	return "%d%%" % roundi(value * 100.0)

func _row_label(row: HBoxContainer, title: String, tag: String) -> void:
	var label := Label.new(); label.text = title; label.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; row.add_child(label)
	if not tag.is_empty():
		var badge := Label.new(); badge.text = "[" + tag + "]"
		badge.add_theme_color_override("font_color", TAG_COLOR); badge.add_theme_font_size_override("font_size", 14)
		badge.set_meta("settings_tag", tag); row.add_child(badge)

static func settings_theme() -> Theme:
	var result := Theme.new(); result.default_font_size = 16
	result.set_color("font_color", "Label", Color("e4eee3"))
	var panel := style(Color("102b29"), Color("698c6b"))
	panel.content_margin_left = 18; panel.content_margin_right = 18; panel.content_margin_top = 14; panel.content_margin_bottom = 14
	result.set_stylebox("panel", "PanelContainer", panel)
	result.set_stylebox("panel", "TabContainer", StyleBoxEmpty.new())
	for type: String in ["Button", "OptionButton", "CheckButton"]:
		result.set_stylebox("normal", type, style(Color("1b3c36"), Color("456452")))
		result.set_stylebox("hover", type, style(Color("285348"), Color("8aa478")))
		result.set_stylebox("pressed", type, style(Color("315b43"), Color("d7b565")))
		result.set_stylebox("focus", type, style(Color(0, 0, 0, 0), Color("f2cb75")))
		result.set_color("font_color", type, Color("e4eee3")); result.set_color("font_pressed_color", type, Color("ffe3a0"))
		result.set_color("font_hover_color", type, Color.WHITE)
	result.set_stylebox("focus", "HSlider", style(Color(0, 0, 0, 0), Color("f2cb75")))
	return result

static func style(background: Color, border: Color) -> StyleBoxFlat:
	var result := StyleBoxFlat.new(); result.bg_color = background; result.border_color = border
	result.set_border_width_all(1); result.set_corner_radius_all(5)
	result.content_margin_left = 10; result.content_margin_right = 10; result.content_margin_top = 6; result.content_margin_bottom = 6
	return result
