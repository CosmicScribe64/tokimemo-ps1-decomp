extends RefCounted
class_name DialogueTextboxPresenter
## Native glyph textbox and choice presenter at 320x240 PS1 resolution.
## Uses NativeGlyphs with native-font-v2 to render dialogue lines,
## speaker labels, monologue parenthesis wrapping, typewriter progression,
## and authentic PS1 HUD actor sprites (actor2/frame1 downward hand continuation
## prompt marker with native bob 72..79 and text/voice/phase gating, and actor0
## menu selection cursor frames 0 / 155..173 from the reviewed HUD manifest).

const Glyphs = preload("res://presentation/native_glyphs.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Flow = preload("res://presentation/native_text_flow.gd")
const Access = preload("res://services/accessibility_service.gd")

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

# Authentic PS1 HUD actor resources (src/main/hud_screens.c:1479-1512, 1740-1768)
const DEFAULT_HUD_MANIFEST := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const DEFAULT_HUD_SHA := ContentIdentity.HUD_MANIFEST_SHA
const DEFAULT_CURSOR_STYLE := 0x9C
const PROMPT_MARKER_ACTOR_INDEX := 2
const PROMPT_MARKER_FRAME := 1
const PROMPT_MARKER_NATIVE_X := 116
const PROMPT_MARKER_NATIVE_Y_MIN := 72
const PROMPT_MARKER_NATIVE_Y_MAX := 79

const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

var font: RefCounted = null
var font_ready: bool = false
var hud_ready: bool = false
var hud_frames: Dictionary = {}
var hud_images: Dictionary = {}
var error: String = ""
var _font_root: String = ""
var backdrop_opacity := 1.0
## T-39207: English text layer (services/text_layer_store.gd), or null for the
## original text. Presentation only: no game state or tick is touched.
var _text_layer: RefCounted = null
## Why the last line or menu kept the original text; empty when English drew.
var text_rejection: String = ""
## "english" when the last rendered line or menu drew translated text.
var last_text_source: String = "native"
## The NativeTextFlow.runtime_box the last English attempt was fitted in
## ({} before any attempt); tests compare it with the port_runtime classes.
var last_text_box: Dictionary = {}

## T-39207: bind (layer) or clear (null) the English text layer. Lines and
## menus whose record id, source hash or fit is not proved keep the original.
func set_translated_text(layer: RefCounted) -> void:
	_text_layer = layer if layer != null and layer.has_method("lookup_record") else null
	text_rejection = "" if _text_layer != null else "no text layer is bound"

## English rows for a record, or [] (text_rejection says why). `view` carries
## record_id, source_sha256, name_codes and an optional variant_index.
func _translated_rows(view: Dictionary, box: Dictionary, expected_variants: int = -1) -> Array:
	if _text_layer == null:
		text_rejection = "no text layer is bound"
		return []
	var record_id := str(view.get("record_id", ""))
	if not record_id.begins_with("talk:") or str(view.get("source_sha256", "")).is_empty():
		text_rejection = "line has no source-proved talk record id"
		return []
	var found: Dictionary = _text_layer.lookup_record(record_id, str(view.source_sha256), font)
	if found.is_empty():
		text_rejection = str(_text_layer.get("last_rejection"))
		return []
	var variants: Array = Flow.split_variants(found.segments)
	var names: Dictionary = view.get("name_codes", {}) if view.get("name_codes") is Dictionary else {}
	if expected_variants >= 0:
		if variants.size() != expected_variants:
			text_rejection = "translated option count differs from the original menu"
			return []
		var options: Array = []
		for variant: Array in variants:
			var option := Flow.classify(variant, box, font, names)
			if option.rows.is_empty():
				text_rejection = "option: %s (%s)" % [option.outcome, option.reason]
				return []
			options.append(option.rows)
		text_rejection = ""
		return options
	var segments: Array = found.segments
	if variants.size() > 1:
		var index: Variant = view.get("variant_index")
		if not index is int or index < 0 or index >= variants.size():
			text_rejection = "reply variants need a source-proved variant index"
			return []
		segments = variants[index]
	var result := Flow.classify(segments, box, font, names)
	text_rejection = "" if not result.rows.is_empty() else "%s (%s)" % [result.outcome, result.reason]
	return result.rows

func configure_accessibility(value: Dictionary) -> void:
	backdrop_opacity = clampf(float(value.get("backdrop", 1.0)), 0.0, 1.0)

## T-30002 WP9 with T-39207: the read, skip and pacing identity of a line is
## the same source-proved talk record the English lookup uses. A line without
## one gets "", so pacing keeps the native reveal and nothing is marked read.
static func line_identity(view: Dictionary) -> String:
	var record_id := str(view.get("record_id", ""))
	var source := str(view.get("source_sha256", ""))
	if not record_id.begins_with("talk:") or source.is_empty(): return ""
	return Access.line_identity(record_id, source)

## The view runtime.pacing.step_line takes for this line. pos and total are the
## native revealed and total code units; speed is native frames per unit.
## Presentation only: the caller still owns the native reveal and input.
static func pacing_view(view: Dictionary, pos: int, total: int, native_complete: bool,
		voice_complete: bool = true, speed: int = 1) -> Dictionary:
	return {"identity": line_identity(view), "pos": pos, "total": total, "speed": speed,
		"native_complete": native_complete, "voice_complete": voice_complete}

## Glyphs the frame driver draws after runtime.pacing.step_line(pacing_view(...)).
## PresentationPacing.visible_count is -1 after a reset, which is what every
## unproved line (empty identity) gets: the driver then draws the native pos,
## so the native reveal is unchanged.
static func draw_count(pacing_visible_count: int, native_pos: int) -> int:
	return native_pos if pacing_visible_count < 0 else pacing_visible_count

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
	_font_root = font_root
	font_ready = true
	if not hud_ready:
		var hud_err: String = open_hud_manifest()
		if not hud_err.is_empty():
			error = hud_err
			font_ready = false
			return hud_err
	return ""

func open_hud_manifest(manifest_path: String = "", expected_sha256: String = DEFAULT_HUD_SHA) -> String:
	hud_ready = false
	hud_frames.clear()
	hud_images.clear()
	var resolved_path: String = _resolve_hud_manifest_path(manifest_path)
	if resolved_path.is_empty() or not FileAccess.file_exists(resolved_path):
		error = "HUD manifest for cursor/prompt actors not found"
		return error
	if not expected_sha256.is_empty() and FileAccess.get_sha256(resolved_path) != expected_sha256:
		error = "HUD manifest SHA-256 mismatch for cursor/prompt actors"
		return error
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(resolved_path))
	if not parsed is Dictionary or not parsed.get("frames") is Dictionary:
		error = "HUD manifest frames table is missing or malformed"
		return error
	var base_dir: String = resolved_path.get_base_dir()
	var frames_dict: Dictionary = parsed["frames"]
	var required_frames: Array[int] = [0, 1]
	for f_idx: int in range(155, 174):
		required_frames.append(f_idx)
	for f_idx: int in required_frames:
		var key := "%d:base" % f_idx
		if not frames_dict.has(key):
			error = "HUD manifest missing required cursor/prompt actor frame: " + key
			return error
		var rec: Dictionary = frames_dict[key]
		hud_frames[key] = rec.duplicate(true)
		for part: Dictionary in rec.get("parts", []):
			for field: String in ["png", "stp_png"]:
				var spec: Dictionary = part.get(field, {})
				var rel_path: String = str(spec.get("path", ""))
				if rel_path.is_empty():
					error = "HUD actor part missing image path in " + key
					return error
				if not hud_images.has(rel_path):
					var abs_path: String = base_dir.path_join(rel_path)
					if not FileAccess.file_exists(abs_path):
						error = "HUD actor part image missing: " + rel_path
						return error
					if spec.has("sha256") and FileAccess.get_sha256(abs_path) != str(spec["sha256"]):
						error = "HUD actor part SHA-256 mismatch: " + rel_path
						return error
					var img := Image.new()
					if img.load(abs_path) != OK:
						error = "HUD actor part failed to decode: " + rel_path
						return error
					hud_images[rel_path] = img
	hud_ready = true
	return ""

func _resolve_hud_manifest_path(explicit_path: String) -> String:
	if not explicit_path.is_empty():
		return explicit_path
	var env_path: String = OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	if not env_path.is_empty() and FileAccess.file_exists(env_path):
		return env_path
	if not _font_root.is_empty():
		var sibling: String = _font_root.get_base_dir().get_base_dir().path_join("hud-production-v1/manifest.json")
		if FileAccess.file_exists(sibling):
			return sibling
	for candidate: String in [
		DEFAULT_HUD_MANIFEST,
		"/work/.asset-cache/port/hud-production-v1/manifest.json",
	]:
		if FileAccess.file_exists(candidate):
			return candidate
	return ""

func is_font_open() -> bool:
	return font_ready

func is_hud_open() -> bool:
	return hud_ready

static func resolve_cursor_actor_frame(cursor_style: int) -> int:
	# hud_cursor_actor_init (src/main/hud_screens.c:1479-1512):
	# style 0 or outside 0x9b..0xad -> frame 0 (pink/green hollow triangle);
	# 0x9b..0xad (155..173) -> stored frame cursor_style.
	var style_u8: int = cursor_style & 0xFF
	if style_u8 >= 0x9B and style_u8 < 0xAE:
		return style_u8
	return 0

static func compute_marker_bob_y(current_native_y: int, frame_count: int) -> int:
	# Exact port of hud_text_marker_bob (src/main/hud_screens.c:1760-1768):
	# Native Y steps 0x48..0x4F (72..79) when (frame_count & 3) == 0, then wraps to 0x48.
	if current_native_y < PROMPT_MARKER_NATIVE_Y_MAX:
		if (frame_count & 3) == 0:
			return current_native_y + 1
		return current_native_y
	return PROMPT_MARKER_NATIVE_Y_MIN

static func resolve_wait_marker(line_view: Dictionary, is_text_complete: bool) -> Dictionary:
	# talk_text_step (menu_speaker_girl.c:1107-1120) / talk_run (scene_library.c:2110-2115):
	# Continuation marker is actor2/frame1 at native (116, 72..79), screen anchor (276, 192..199).
	# Gated on text completion, active input-wait phase, voice completion (or voice_mode == 0),
	# and actor2 visibility flag.
	var waiting_for_input: bool = bool(line_view.get("waiting_for_input", line_view.get("line_wait", true)))
	var marker_visible_flag: bool = bool(line_view.get("marker_visible", true))
	var voice_mode: int = int(line_view.get("voice_mode", 2))
	if voice_mode not in [0, 1, 2]:
		return {"error": "voice_mode must be 0, 1, or 2"}
	var voice_ready: bool = true
	if line_view.has("voice_ready"):
		voice_ready = bool(line_view["voice_ready"]) or (voice_mode == 0)
	elif line_view.has("speaker_voice_state"):
		var sv_state: int = int(line_view["speaker_voice_state"])
		if sv_state not in [0, 1, 2]:
			return {"error": "speaker_voice_state must be 0 (pending), 1 (playing), or 2 (done)"}
		voice_ready = (sv_state == 2) or (voice_mode == 0)
	elif line_view.has("voice_complete"):
		voice_ready = bool(line_view["voice_complete"]) or (voice_mode == 0)
	elif bool(line_view.get("voice_playing", false)):
		voice_ready = (voice_mode == 0)

	var marker_native_y: int = PROMPT_MARKER_NATIVE_Y_MIN
	if line_view.has("marker_native_y"):
		if not (line_view["marker_native_y"] is int):
			return {"error": "marker_native_y must be an integer"}
		marker_native_y = int(line_view["marker_native_y"])
		if marker_native_y < PROMPT_MARKER_NATIVE_Y_MIN or marker_native_y > PROMPT_MARKER_NATIVE_Y_MAX:
			return {"error": "marker_native_y must be in native range 72..79 (0x48..0x4f)"}
	elif line_view.has("bob_offset"):
		var bob: int = int(line_view["bob_offset"])
		if bob < 0 or bob > 7:
			return {"error": "bob_offset must be in range 0..7"}
		marker_native_y = PROMPT_MARKER_NATIVE_Y_MIN + bob

	var show_marker: bool = is_text_complete and waiting_for_input and voice_ready and marker_visible_flag
	var native_pos := Vector2i(PROMPT_MARKER_NATIVE_X, marker_native_y)
	var screen_anchor := Vector2i(160 + PROMPT_MARKER_NATIVE_X, 120 + marker_native_y)
	return {
		"error": "",
		"visible": show_marker,
		"actor_index": PROMPT_MARKER_ACTOR_INDEX,
		"frame": PROMPT_MARKER_FRAME,
		"native_position": native_pos,
		"screen_anchor": screen_anchor,
		"bitmap_top_left": screen_anchor - Vector2i(8, 8),
		"voice_ready": voice_ready,
		"waiting_for_input": waiting_for_input,
	}

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
			var position := TEXTBOX_INNER.position + Vector2i(x, y)
			var shade := Color8(red << 3, green << 3, blue << 3, 230)
			target.set_pixelv(position, target.get_pixelv(position).lerp(shade, backdrop_opacity))

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

	# T-39207: English rows replace the body only when the whole record fits
	# the original window from this origin (the closing bracket keeps its cell).
	last_text_source = "native"
	var english: Array = []
	if _text_layer != null:
		last_text_box = Flow.runtime_box(text_start_x, TEXTBOX_INNER.end.x, not close_bracket_hex.is_empty() and wrapper != "none", 3)
		english = _translated_rows(line_view, last_text_box)
	if not english.is_empty():
		return _render_english_line(target, line_view, english, current_x, text_start_x, open_bracket_hex, close_bracket_hex, wrapper, visible_glyph_count)

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

	# Authentic PS1 continuation prompt marker: actor2/frame1 at native (116, 72..79),
	# gated on text completion + waiting_for_input + voice completion / voice_mode 0.
	var marker: Dictionary = resolve_wait_marker(line_view, is_text_complete)
	if not str(marker.get("error", "")).is_empty():
		error = str(marker["error"])
		return {"error": error}
	if bool(marker["visible"]):
		if not hud_ready or not hud_frames.has("1:base"):
			error = "native dialogue wait marker requires original actor2/frame1 resources"
			return {"error": error}
		var draw_err: String = _draw_hud_actor(target, PROMPT_MARKER_FRAME, marker["native_position"], false)
		if not draw_err.is_empty():
			error = draw_err
			return {"error": error}

	return {
		"error": "",
		"total_glyphs": total_body_glyphs,
		"revealed_glyphs": revealed_glyphs,
		"text_complete": is_text_complete,
		"first_visible_row": first_visible_row,
		"visible_row_count": revealed_rows.size() - first_visible_row,
		"prompt_marker": marker,
	}

## English body of render_dialogue_line: the same window, bracket, reveal and
## marker rules, with NativeLatin x offsets instead of the 14 px advance. One
## code unit is revealed per original tick.
func _render_english_line(target: Image, line_view: Dictionary, rows: Array, bracket_x: int, text_start_x: int,
		open_bracket_hex: String, close_bracket_hex: String, wrapper: String, visible_glyph_count: int) -> Dictionary:
	var total_body_glyphs: int = 0
	for row: Dictionary in rows:
		total_body_glyphs += row.x.size()
	var reveal_limit: int = total_body_glyphs if visible_glyph_count < 0 else mini(visible_glyph_count, total_body_glyphs)
	if not open_bracket_hex.is_empty():
		_draw_single_glyph(target, open_bracket_hex.hex_decode(), Vector2i(bracket_x, LINE_START_Y), 0)
	var revealed_glyphs: int = 0
	var visible_rows: int = 0
	var last_row_end_x: int = text_start_x
	var last_row_y: int = LINE_START_Y
	for row_index: int in rows.size():
		if revealed_glyphs >= reveal_limit and row_index > 0:
			break
		var row: Dictionary = rows[row_index]
		var row_y: int = LINE_START_Y + row_index * LINE_PITCH_Y
		for glyph_index: int in row.x.size():
			if revealed_glyphs >= reveal_limit:
				break
			_draw_single_glyph(target, row.codes.slice(glyph_index * 2, glyph_index * 2 + 2), Vector2i(text_start_x + row.x[glyph_index], row_y), 0)
			revealed_glyphs += 1
		visible_rows += 1
		last_row_end_x = text_start_x + int(row.width)
		last_row_y = row_y
	var is_text_complete: bool = revealed_glyphs >= total_body_glyphs
	if is_text_complete and not close_bracket_hex.is_empty() and wrapper != "none":
		_draw_single_glyph(target, close_bracket_hex.hex_decode(), Vector2i(last_row_end_x, last_row_y), 0)
	var marker: Dictionary = resolve_wait_marker(line_view, is_text_complete)
	if not str(marker.get("error", "")).is_empty():
		error = str(marker["error"])
		return {"error": error}
	if bool(marker["visible"]):
		if not hud_ready or not hud_frames.has("1:base"):
			error = "native dialogue wait marker requires original actor2/frame1 resources"
			return {"error": error}
		var draw_err: String = _draw_hud_actor(target, PROMPT_MARKER_FRAME, marker["native_position"], false)
		if not draw_err.is_empty():
			error = draw_err
			return {"error": error}
	last_text_source = "english"
	return {
		"error": "",
		"total_glyphs": total_body_glyphs,
		"revealed_glyphs": revealed_glyphs,
		"text_complete": is_text_complete,
		"first_visible_row": 0,
		"visible_row_count": visible_rows,
		"prompt_marker": marker,
		"text_source": "english",
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

	var cursor_style: int = int(choice_view.get("cursor_style", DEFAULT_CURSOR_STYLE)) & 0xFF
	var cursor_frame: int = resolve_cursor_actor_frame(cursor_style)
	var cursor_key := "%d:base" % cursor_frame
	if not hud_ready or not hud_frames.has(cursor_key):
		error = "choice menu selection requires original actor0 cursor frame resources: " + cursor_key
		return {"error": error}

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

	# T-39207: English options come from the menu record's 'q'-separated
	# variants by source index; every option must fit its row or none is used.
	# Body from x 60 (after the bracket at 46) to the highlight end (274)
	# less the closing bracket cell (NativeTextFlow "textbox_choice").
	last_text_source = "native"
	var english_options: Array = []
	if _text_layer != null:
		last_text_box = Flow.runtime_box(Flow.CHOICE_BODY_X, Flow.CHOICE_RIGHT, true, 1)
		english_options = _translated_rows(choice_view, last_text_box, options.size())
		if not english_options.is_empty():
			total_glyphs = 0
			for option_rows: Array in english_options:
				total_glyphs += option_rows[0].x.size()
			last_text_source = "english"

	var reveal_limit: int = total_glyphs if visible_glyph_count < 0 else mini(visible_glyph_count, total_glyphs)
	var revealed_glyphs: int = 0
	var selected_cursor_anchor := Vector2i.ZERO

	for display_idx: int in range(options.size()):
		var opt_record: Variant = options[display_idx]
		if not opt_record is Dictionary:
			continue
		var opt_tokens: Array = opt_record.get("tokens", [])
		var opt_y: int = LINE_START_Y + display_idx * LINE_PITCH_Y
		var is_selected: bool = (display_idx == selected_display_index)

		if is_selected:
			# Highlight row background before drawing glyphs and actor0 cursor
			var highlight_rect := Rect2i(44, opt_y - 1, 230, 14)
			_tint_rect(target, highlight_rect, Color8(40, 60, 100, 100))

		# Opening choice bracket
		var opt_x: int = 46
		_draw_single_glyph(target, open_hex.hex_decode(), Vector2i(opt_x, opt_y), 0)
		opt_x += GLYPH_ADVANCE_X

		# Option body text
		if not english_options.is_empty():
			var source_index: int = int(opt_record.get("source_index", display_idx))
			var english_row: Dictionary = english_options[source_index][0] if source_index >= 0 and source_index < english_options.size() else {"codes": PackedByteArray(), "x": PackedInt32Array(), "width": 0}
			for glyph_index: int in english_row.x.size():
				if revealed_glyphs < reveal_limit:
					_draw_single_glyph(target, english_row.codes.slice(glyph_index * 2, glyph_index * 2 + 2), Vector2i(opt_x + english_row.x[glyph_index], opt_y), 0)
					revealed_glyphs += 1
			opt_x += int(english_row.width)
		for token: Variant in (opt_tokens if english_options.is_empty() else []):
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

		if is_selected:
			var f_rec: Dictionary = hud_frames[cursor_key]
			var origin := Vector2i(int(f_rec["origin"][0]), int(f_rec["origin"][1]))
			var b_size := Vector2i(int(f_rec["bounds"][2]), int(f_rec["bounds"][3]))
			if choice_view.get("cursor_anchor") is Vector2i:
				selected_cursor_anchor = choice_view["cursor_anchor"]
			elif bool(choice_view.get("native_menu_anchor", false)):
				# menu_cursor_update(1) on normal_date_*_select_init (-0x80, 0x30 + 16*r, 0xfc, 0x10):
				# cursor_x = item.x + item.w - 4 = 120 (screen 280), cursor_y = item.y + 8 = 56 + 16*r (screen 176 + 16*r)
				selected_cursor_anchor = Vector2i(280, 176 + display_idx * LINE_PITCH_Y)
			else:
				var anchor_x: int = mini(268, TEXTBOX_INNER.end.x - b_size.x + origin.x)
				var anchor_y: int = clampi(
					opt_y + origin.y,
					TEXTBOX_INNER.position.y + origin.y,
					TEXTBOX_INNER.end.y - b_size.y + origin.y
				)
				selected_cursor_anchor = Vector2i(anchor_x, anchor_y)
			var draw_err: String = _draw_hud_actor(target, cursor_frame, selected_cursor_anchor - Vector2i(160, 120), false)
			if not draw_err.is_empty():
				error = draw_err
				return {"error": error}

	return {
		"error": "",
		"total_glyphs": total_glyphs,
		"revealed_glyphs": revealed_glyphs,
		"text_complete": (revealed_glyphs >= total_glyphs or visible_glyph_count < 0),
		"selected_index": selected_display_index,
		"cursor_actor_index": 0,
		"cursor_style": cursor_style,
		"cursor_frame": cursor_frame,
		"cursor_screen_anchor": selected_cursor_anchor,
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
		var choice_input: Dictionary = view.duplicate(false)
		for opt_key: String in ["cursor_style", "cursor_anchor", "native_menu_anchor"]:
			if options.has(opt_key):
				choice_input[opt_key] = options[opt_key]
		render_choice_menu(image, choice_input, selected_idx, visible_count)
	elif view.has("tokens"):
		var line_input: Dictionary = view.duplicate(false)
		for opt_key: String in ["waiting_for_input", "voice_ready", "voice_mode", "speaker_voice_state", "voice_complete", "marker_visible", "marker_native_y", "bob_offset"]:
			if options.has(opt_key):
				line_input[opt_key] = options[opt_key]
		render_dialogue_line(image, line_input, visible_count)

	return image

func present_to_canvas(canvas: Node2D, image: Image, frame_id: String = "dialogue_textbox") -> String:
	if canvas == null or not canvas.has_method("present"):
		return "canvas must be a valid NativeCanvas node"
	if image == null:
		return "image must not be null"
	var commands: Array[Dictionary] = [{"id": frame_id, "image": image, "position": Vector2i.ZERO}]
	return canvas.present(commands)

func _draw_hud_actor(target: Image, frame_index: int, native_position: Vector2i, semi: bool = false) -> String:
	var key := "%d:base" % frame_index
	if not hud_frames.has(key):
		return "HUD actor frame is unavailable: " + key
	var record: Dictionary = hud_frames[key]
	var origin := Vector2i(int(record["origin"][0]), int(record["origin"][1]))
	for part: Dictionary in record.get("parts", []):
		var rel_png: String = str(part["png"]["path"])
		var rel_stp: String = str(part["stp_png"]["path"])
		var src_img: Image = hud_images.get(rel_png)
		var stp_img: Image = hud_images.get(rel_stp)
		if src_img == null:
			return "HUD actor part image not loaded: " + rel_png
		var part_pos: Vector2i = Vector2i(160, 120) + native_position + Vector2i(int(part["x"]), int(part["y"])) - origin
		_blit_hud_part(target, src_img, stp_img, part_pos, semi)
	return ""

func _blit_hud_part(target: Image, source: Image, stp: Image, position: Vector2i, semi: bool) -> void:
	var sw: int = source.get_width()
	var sh: int = source.get_height()
	for y: int in range(sh):
		var py: int = position.y + y
		if py < 0 or py >= NATIVE_SIZE.y:
			continue
		for x: int in range(sw):
			var px: int = position.x + x
			if px < 0 or px >= NATIVE_SIZE.x:
				continue
			var c: Color = source.get_pixel(x, y)
			if c.a == 0:
				continue
			var rgb := Vector3i(c.r8 >> 3, c.g8 >> 3, c.b8 >> 3)
			if semi and stp != null and stp.get_pixel(x, y).r8 != 0:
				var back: Color = target.get_pixel(px, py)
				rgb = Vector3i((back.r8 >> 4) + (rgb.x >> 1), (back.g8 >> 4) + (rgb.y >> 1), (back.b8 >> 4) + (rgb.z >> 1))
			target.set_pixel(px, py, Color8(rgb.x << 3, rgb.y << 3, rgb.z << 3, 255))

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
