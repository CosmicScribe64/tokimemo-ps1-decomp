extends RefCounted
class_name DialogueTextboxPresenter
## Native glyph textbox and choice presenter at 320x240 PS1 resolution.
## Uses NativeGlyphs with native-font-v2 to render dialogue lines,
## speaker labels, monologue parenthesis wrapping, typewriter progression,
## and cursor-highlighted choice options onto NativeCanvas.

const Glyphs = preload("res://presentation/native_glyphs.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

const NATIVE_SIZE := Vector2i(320, 240)
const TEXTBOX_RECT := Rect2i(27, 164, 266, 56)
const TEXTBOX_INNER := Rect2i(28, 165, 264, 54)

# Native PS1 coordinates from scene_library.c / text_system.c
const LINE_START_Y := 170
const LINE_PITCH_Y := 16
const GLYPH_ADVANCE_X := 14

# Shift-JIS bracket code units
const BRACKET_OPEN_PAREN := "8169"    # （
const BRACKET_CLOSE_PAREN := "816a"   # ）
const QUOTE_OPEN := "8175"            # 「
const QUOTE_CLOSE := "8176"           # 」
const DOUBLE_QUOTE_OPEN := "8177"     # 『
const DOUBLE_QUOTE_CLOSE := "8178"    # 』
const CURSOR_TRIANGLE := "81a0"       # ■ / cursor symbol
const PROMPT_MARKER := "81a5"         # ▼ prompt waiting marker

const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

var font: RefCounted = null
var font_ready: bool = false
var error: String = ""

func _init() -> void:
	font = Glyphs.new()

func open_font(font_root: String) -> String:
	error = ""
	if font_root.is_empty():
		return "font root path must be specified"
	var err: String = font.open_external_font(font_root)
	if not err.is_empty():
		error = err
		font_ready = false
		return err
	font_ready = true
	return ""

func is_font_open() -> bool:
	return font_ready

func render_textbox_background(target: Image) -> void:
	if target == null or target.get_size() != NATIVE_SIZE:
		return

	# Dithered dark gradient matching PS1 hardware style
	for y: int in range(TEXTBOX_INNER.size.y):
		for x: int in range(TEXTBOX_INNER.size.x):
			var factor: float = 10.0 - 2.0 * float(x) / float(TEXTBOX_INNER.size.x) - 4.0 * float(y) / float(TEXTBOX_INNER.size.y)
			var d: int = DITHER[((TEXTBOX_INNER.position.y + y) & 3) * 4 + ((TEXTBOX_INNER.position.x + x) & 3)]
			var red: int = clampi((int(2 * factor) + d) >> 3, 0, 16)
			var green: int = clampi((int(3 * factor) + d) >> 3, 0, 20)
			var blue: int = clampi((int(8 * factor) + d) >> 3, 4, 31)
			target.set_pixelv(TEXTBOX_INNER.position + Vector2i(x, y), Color8(red << 3, green << 3, blue << 3, 230))

	# Outer 1-pixel frame border
	var border_top_left := Color8(140, 160, 200, 255)
	var border_bottom_right := Color8(50, 60, 90, 255)

	for x: int in range(TEXTBOX_RECT.size.x):
		target.set_pixel(TEXTBOX_RECT.position.x + x, TEXTBOX_RECT.position.y, border_top_left)
		target.set_pixel(TEXTBOX_RECT.position.x + x, TEXTBOX_RECT.position.y + TEXTBOX_RECT.size.y - 1, border_bottom_right)

	for y: int in range(TEXTBOX_RECT.size.y):
		target.set_pixel(TEXTBOX_RECT.position.x, TEXTBOX_RECT.position.y + y, border_top_left)
		target.set_pixel(TEXTBOX_RECT.position.x + TEXTBOX_RECT.size.x - 1, TEXTBOX_RECT.position.y + y, border_bottom_right)

func render_dialogue_line(target: Image, line_view: Dictionary, visible_glyph_count: int = -1) -> Dictionary:
	if not font_ready:
		return {"error": "font is not opened"}
	if target == null:
		return {"error": "target image must not be null"}

	var tokens: Array = line_view.get("tokens", [])
	var speaker_id: String = str(line_view.get("speaker_id", ""))
	var wrapper: String = str(line_view.get("wrapper", ""))
	var command: int = int(line_view.get("command", -1))

	var current_x: int = 40
	var current_y: int = LINE_START_Y

	# Render speaker label if available
	var speaker_glyphs_drawn: int = 0
	if speaker_id == "shiori":
		var shiori_bytes: PackedByteArray = "93a18de8".hex_decode() # 藤崎
		speaker_glyphs_drawn = _draw_glyph_sequence(target, shiori_bytes, Vector2i(current_x, current_y), 0)
		current_x += speaker_glyphs_drawn * GLYPH_ADVANCE_X + 4
	elif speaker_id == "yoshio":
		var yoshio_bytes: PackedByteArray = "8d449759".hex_decode() # 好雄
		speaker_glyphs_drawn = _draw_glyph_sequence(target, yoshio_bytes, Vector2i(current_x, current_y), 0)
		current_x += speaker_glyphs_drawn * GLYPH_ADVANCE_X + 4
	elif not speaker_id.is_empty() and speaker_id not in ["player_family", "runtime_current_girl", "inline_or_effect"]:
		if _is_hex(speaker_id):
			var speaker_bytes: PackedByteArray = speaker_id.hex_decode()
			speaker_glyphs_drawn = _draw_glyph_sequence(target, speaker_bytes, Vector2i(current_x, current_y), 0)
			current_x += speaker_glyphs_drawn * GLYPH_ADVANCE_X + 4

	# Opening bracket/quote placement
	var open_bracket_hex: String = ""
	var close_bracket_hex: String = ""
	if wrapper == "parenthesis" or command in [4, 6, 14]:
		open_bracket_hex = BRACKET_OPEN_PAREN
		close_bracket_hex = BRACKET_CLOSE_PAREN
	elif wrapper == "bracket" or command == 0:
		open_bracket_hex = DOUBLE_QUOTE_OPEN
		close_bracket_hex = DOUBLE_QUOTE_CLOSE
	elif wrapper == "quote" or command in [1, 2, 5, 7, 10, 11, 12, 15]:
		open_bracket_hex = QUOTE_OPEN
		close_bracket_hex = QUOTE_CLOSE

	var text_start_x: int = current_x
	if not open_bracket_hex.is_empty():
		text_start_x += GLYPH_ADVANCE_X

	# Split body tokens into logical rows on separator tokens (0x20), matching
	# talk_render() + check_k_scroll() in src/main/scene_library.c:1096-1143.
	var rows: Array[Array] = [[]]
	var total_body_glyphs: int = 0
	for token: Variant in tokens:
		if not token is Dictionary:
			continue
		var kind: String = str(token.get("kind", ""))
		if kind == "glyph":
			rows[rows.size() - 1].append(str(token.get("raw_hex", "")))
			total_body_glyphs += 1
		elif kind == "separator":
			rows.append([])

	var reveal_limit: int = total_body_glyphs if visible_glyph_count < 0 else mini(visible_glyph_count, total_body_glyphs)
	var revealed_glyphs: int = 0
	var revealed_rows: Array[Array] = []
	for row_glyphs: Array in rows:
		var visible_row: Array[String] = []
		for raw_hex: String in row_glyphs:
			if revealed_glyphs >= reveal_limit:
				break
			visible_row.append(raw_hex)
			revealed_glyphs += 1
		if not visible_row.is_empty() or revealed_rows.is_empty():
			revealed_rows.append(visible_row)
		if revealed_glyphs >= reveal_limit:
			break

	# check_k_scroll() keeps at most 3 rows visible in the textbox (y = 170, 186, 202).
	# TextEntry 0 (speaker name) stays at row 0; TextEntry 1 (opening bracket on row 0)
	# scrolls off when first_visible_row > 0.
	const MAX_VISIBLE_ROWS := 3
	var first_visible_row: int = maxi(0, revealed_rows.size() - MAX_VISIBLE_ROWS)
	if first_visible_row == 0 and not open_bracket_hex.is_empty():
		_draw_single_glyph(target, open_bracket_hex.hex_decode(), Vector2i(current_x, LINE_START_Y), 0)

	var last_row_end_x: int = text_start_x
	var last_row_y: int = LINE_START_Y
	for vis_idx: int in range(first_visible_row, revealed_rows.size()):
		var display_row: int = vis_idx - first_visible_row
		var row_y: int = LINE_START_Y + display_row * LINE_PITCH_Y
		var row_x: int = text_start_x
		for raw_hex: String in revealed_rows[vis_idx]:
			_draw_single_glyph(target, raw_hex.hex_decode(), Vector2i(row_x, row_y), 0)
			row_x += GLYPH_ADVANCE_X
		last_row_end_x = row_x
		last_row_y = row_y

	# Closing bracket if typewriter complete
	var is_text_complete: bool = (revealed_glyphs >= total_body_glyphs)
	if is_text_complete and not close_bracket_hex.is_empty() and wrapper != "none":
		_draw_single_glyph(target, close_bracket_hex.hex_decode(), Vector2i(last_row_end_x, last_row_y), 0)

	# Prompt waiting marker if text complete and waiting for input
	if is_text_complete:
		_draw_single_glyph(target, PROMPT_MARKER.hex_decode(), Vector2i(278, 200), 1)

	return {
		"error": "",
		"total_glyphs": total_body_glyphs,
		"revealed_glyphs": revealed_glyphs,
		"text_complete": is_text_complete,
		"first_visible_row": first_visible_row,
		"visible_row_count": revealed_rows.size() - first_visible_row,
	}

func render_choice_menu(target: Image, choice_view: Dictionary, selected_display_index: int = 0,
		visible_glyph_count: int = -1) -> Dictionary:
	if not font_ready:
		return {"error": "font is not opened"}
	if target == null:
		return {"error": "target image must not be null"}

	var options: Array = choice_view.get("options", [])
	if options.is_empty():
		return {"error": "choice menu has no options"}

	var command: int = int(choice_view.get("command", 3))
	var is_parenthesis: bool = (command == 19)
	var open_hex: String = BRACKET_OPEN_PAREN if is_parenthesis else DOUBLE_QUOTE_OPEN
	var close_hex: String = BRACKET_CLOSE_PAREN if is_parenthesis else DOUBLE_QUOTE_CLOSE

	var total_glyphs: int = 0
	for opt: Variant in options:
		if opt is Dictionary:
			for token: Variant in opt.get("tokens", []):
				if token is Dictionary and token.get("kind") == "glyph":
					total_glyphs += 1

	var reveal_limit: int = total_glyphs if visible_glyph_count < 0 else mini(visible_glyph_count, total_glyphs)
	var revealed_glyphs: int = 0

	for display_idx: int in range(options.size()):
		var opt_record: Variant = options[display_idx]
		if not opt_record is Dictionary:
			continue
		var opt_tokens: Array = opt_record.get("tokens", [])
		var opt_y: int = LINE_START_Y + display_idx * LINE_PITCH_Y
		var is_selected: bool = (display_idx == selected_display_index)

		# Cursor indicator for highlighted option
		if is_selected:
			_draw_single_glyph(target, CURSOR_TRIANGLE.hex_decode(), Vector2i(32, opt_y), 1)
			# Highlight row background slightly
			var highlight_rect := Rect2i(44, opt_y - 1, 230, 14)
			_tint_rect(target, highlight_rect, Color8(40, 60, 100, 100))

		# Opening choice bracket
		var opt_x: int = 46
		_draw_single_glyph(target, open_hex.hex_decode(), Vector2i(opt_x, opt_y), 0)
		opt_x += GLYPH_ADVANCE_X

		# Option body text
		for token: Variant in opt_tokens:
			if not token is Dictionary or token.get("kind") != "glyph":
				continue
			if revealed_glyphs < reveal_limit:
				var raw_hex: String = str(token.get("raw_hex", ""))
				_draw_single_glyph(target, raw_hex.hex_decode(), Vector2i(opt_x, opt_y), 0)
				revealed_glyphs += 1
				opt_x += GLYPH_ADVANCE_X

		# Closing choice bracket
		if revealed_glyphs >= total_glyphs or visible_glyph_count < 0:
			_draw_single_glyph(target, close_hex.hex_decode(), Vector2i(opt_x, opt_y), 0)

	return {
		"error": "",
		"total_glyphs": total_glyphs,
		"revealed_glyphs": revealed_glyphs,
		"text_complete": (revealed_glyphs >= total_glyphs or visible_glyph_count < 0),
		"selected_index": selected_display_index,
	}

func render_frame(view: Dictionary, options: Dictionary = {}) -> Image:
	var image := Image.create(NATIVE_SIZE.x, NATIVE_SIZE.y, false, Image.FORMAT_RGBA8)
	var background: Variant = options.get("background")
	if background is Image and background.get_size() == NATIVE_SIZE:
		image.copy_from(background)
	else:
		image.fill(Color8(12, 16, 24, 255))

	render_textbox_background(image)

	var visible_count: int = int(options.get("visible_glyph_count", -1))
	if view.has("options"):
		var selected_idx: int = int(options.get("selected_index", 0))
		render_choice_menu(image, view, selected_idx, visible_count)
	elif view.has("tokens"):
		render_dialogue_line(image, view, visible_count)

	return image

func present_to_canvas(canvas: Node2D, image: Image, frame_id: String = "dialogue_textbox") -> String:
	if canvas == null or not canvas.has_method("present"):
		return "canvas must be a valid NativeCanvas node"
	if image == null:
		return "image must not be null"
	var commands: Array[Dictionary] = [{"id": frame_id, "image": image, "position": Vector2i.ZERO}]
	return canvas.present(commands)

func _draw_single_glyph(target: Image, code_unit: PackedByteArray, pos: Vector2i, palette_row: int = 0) -> void:
	if code_unit.size() != 2:
		return
	var glyph_info: Dictionary = font.get_glyph_image(code_unit, palette_row)
	if glyph_info.get("status") != "glyph":
		return
	var foreground: Image = glyph_info["image"]
	var shadow: Image = foreground.duplicate()
	for y: int in range(12):
		for x: int in range(12):
			if shadow.get_pixel(x, y).a > 0:
				shadow.set_pixel(x, y, Color.BLACK)

	# Shadow blit at pos + Vector2i.ONE
	_blit_alpha(target, shadow, pos + Vector2i.ONE)
	# Foreground blit at pos
	_blit_alpha(target, foreground, pos)

func _draw_glyph_sequence(target: Image, codes: PackedByteArray, pos: Vector2i, palette_row: int = 0) -> int:
	var count: int = 0
	for i: int in range(0, codes.size() - 1, 2):
		_draw_single_glyph(target, codes.slice(i, i + 2), pos + Vector2i(count * GLYPH_ADVANCE_X, 0), palette_row)
		count += 1
	return count

func _blit_alpha(target: Image, source: Image, pos: Vector2i) -> void:
	if source == null or target == null:
		return
	var sw: int = source.get_width()
	var sh: int = source.get_height()
	for y: int in range(sh):
		var py: int = pos.y + y
		if py < 0 or py >= NATIVE_SIZE.y:
			continue
		for x: int in range(sw):
			var px: int = pos.x + x
			if px < 0 or px >= NATIVE_SIZE.x:
				continue
			var src_color := source.get_pixel(x, y)
			if src_color.a > 0:
				target.set_pixel(px, py, src_color)

func _tint_rect(target: Image, rect: Rect2i, tint: Color) -> void:
	for y: int in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= NATIVE_SIZE.y:
			continue
		for x: int in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= NATIVE_SIZE.x:
				continue
			var orig := target.get_pixel(px, py)
			var blended := orig.lerp(tint, 0.4)
			target.set_pixel(px, py, blended)

static func _is_hex(value: String) -> bool:
	if value.is_empty() or value.length() % 2 != 0:
		return false
	for index: int in range(value.length()):
		var code: int = value.unicode_at(index)
		if not ((code >= 48 and code <= 57) or (code >= 97 and code <= 102)
				or (code >= 65 and code <= 70)):
			return false
	return true
