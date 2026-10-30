extends SceneTree
## T-30001 WP6 host: NativeDisplayHost framing, wing selection, overlay mount
## and the native-Image fidelity invariant across aspect x filter x 8 sizes.
## Headless, no game data (synthetic native frames).

const Host = preload("res://presentation/native_display_host.gd")
const Layout = preload("res://presentation/native_frame_layout.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const SIZES := [Vector2(640, 480), Vector2(960, 720), Vector2(1280, 720), Vector2(1366, 768),
	Vector2(1920, 1080), Vector2(1920, 1200), Vector2(2560, 1080), Vector2(3440, 1440)]
const PAYLOAD := "ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12"

class FakeSettings extends Node:
	signal preferences_changed(settings: Dictionary)
	var preferences := FakePreferences.new()

class FakePreferences extends RefCounted:
	var value := {"display": {"aspect": "original", "filter": "integer", "wing_fill": "black"},
		"accessibility": {"text_box": "native", "font_scale": 1.0}}
	func effective_settings() -> Dictionary:
		return value.duplicate(true)

class NoTileCompositor extends RefCounted:
	func checker_tile(_uv: Vector2i, _season: int, _fade: int) -> Image:
		return null

class FakeTranslations extends RefCounted:
	func replacement(payload_sha: String, line: int) -> Dictionary:
		return {"speaker": "", "text": "A translated line."} if line == 0 and payload_sha.length() == 64 else {}

var _checks := 0
var _failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

static func _hash(image: Image) -> String:
	var hashing := HashingContext.new()
	hashing.start(HashingContext.HASH_SHA256)
	hashing.update(var_to_bytes([image.get_width(), image.get_height(), image.get_format()]))
	hashing.update(image.get_data())
	return hashing.finish().hex_encode()

static func _native(seed_value: int) -> Image:
	var rng := RandomNumberGenerator.new()
	rng.seed = seed_value
	var data := PackedByteArray()
	data.resize(320 * 240 * 4)
	for index: int in data.size():
		data[index] = rng.randi() & 255
	return Image.create_from_data(320, 240, false, Image.FORMAT_RGBA8, data)

static func _legacy_rect(area: Vector2, filter: String) -> Rect2:
	var factor := minf(area.x / 320.0, area.y / 240.0)
	if filter == "integer" and factor >= 1.0:
		factor = floorf(factor)
	var size := Vector2(320, 240) * factor
	return Rect2((area - size) * 0.5, size)

func _initialize() -> void:
	create_timer(300.0).timeout.connect(func() -> void:
		printerr("FAIL: host suite timed out")
		quit(1))
	_run.call_deferred()

func _run() -> void:
	var settings := FakeSettings.new()
	settings.name = "WideCoreFakeSettings"
	root.add_child(settings)
	var host: Control = Host.new()
	host.settings_node_path = NodePath("/root/WideCoreFakeSettings")
	root.add_child(host)
	await process_frame
	_check_matrix(host, settings)
	await _check_settings_signal(host, settings)
	_check_wings(host)
	_check_overlay(host)
	_check_negatives(host)
	if _failures.is_empty():
		print("PASS: native display host (%d checks)" % _checks)
		quit(0)
	else:
		for failure: String in _failures.slice(0, 40):
			printerr("FAIL: ", failure)
		printerr("FAIL count: %d" % _failures.size())
		quit(1)

func _check_matrix(host: Control, _settings: Node) -> void:
	var native := _native(11)
	var native_hash := _hash(native)
	for aspect: String in Layout.ASPECTS:
		for filter: String in Layout.FILTERS:
			for window: Vector2 in SIZES:
				var tag := "%s/%s %dx%d" % [aspect, filter, window.x, window.y]
				host.set_preferences_override({"display": {"aspect": aspect, "filter": filter}})
				host.size = window
				host.relayout()
				_check(host.present(native, 0x10), tag + " presents")
				_check(_hash(native) == native_hash, tag + " native Image unchanged")
				var shown: Image = host.presented_image
				_check(Vector2i(host.display.texture.get_size()) == shown.get_size(), tag + " texture matches the presented Image size")
				_check(_hash(Composer.centre_crop(shown)) == native_hash, tag + " displayed centre is byte-identical to native")
				_check(host.display.position == host.layout.frame.position and host.display.size == host.layout.frame.size, tag + " display rect is the layout frame")
				# The frame is Wl x 240 native pixels scaled uniformly; a texture
				# of any other width would be stretched non-uniformly into it.
				_check(shown.get_width() == host.layout.logical_width and shown.get_height() == 240, tag + " presented width equals the layout's logical width")
				_check(is_equal_approx(host.layout.frame.size.x / shown.get_width(), host.layout.frame.size.y / shown.get_height()), tag + " frame scales the presented Image uniformly")
				if host.layout.logical_width == 320:
					_check(host.presented_image == native and shown.get_size() == Vector2i(320, 240), tag + " Wl=320 bypasses the composer")
					if aspect == "original":
						_check(Rect2(host.display.position, host.display.size) == _legacy_rect(window, filter), tag + " original rect equals the pre-T-30001 boot fit")
				else:
					_check(shown.get_size() == Vector2i(host.layout.logical_width, 240), tag + " wide texture is Wl x 240")
					_check(host.last_wing_source == "black", tag + " Class C wings are black")
				var centre: Vector2 = host.native_to_window(Vector2i(160, 120))
				_check(host.window_to_native(centre).pixel == Vector2i(160, 120), tag + " pointer transform round trip")
	_check(host.capabilities().widescreen_available, "capability flag exposed")
	host.set_preferences_override({"display": {"aspect": "original", "filter": "integer"}})
	host.size = Vector2(1920, 1080)
	host.relayout()
	var margins: Array[Rect2] = host.ui_margin_rects()
	_check(margins.size() >= 2 and margins[0].size.x >= 320.0, "original 1920x1080 reports side margins for modern UI")
	host.size = Vector2(640, 480)
	host.relayout()
	_check(host.ui_margin_rects().is_empty(), "640x480 original has no margins")

func _check_settings_signal(host: Control, settings: Node) -> void:
	host.set_preferences_override(null)
	host.size = Vector2(1920, 1080)
	settings.preferences.value.display.aspect = "original"
	host.relayout()
	_check(host.layout.mode == "original", "host reads ModernSettings preferences")
	settings.preferences.value.display.aspect = "wide"
	settings.preferences_changed.emit(settings.preferences.effective_settings())
	await process_frame
	_check(host.layout.mode == "wide" and host.layout.logical_width == 426, "preferences_changed relayouts to wide")
	settings.preferences.value.display.aspect = "original"
	settings.preferences_changed.emit(settings.preferences.effective_settings())
	host.size = Vector2(1280, 720)
	await process_frame
	_check(host.layout.window_size == Vector2(1280, 720), "resize relayouts")

func _check_wings(host: Control) -> void:
	var compositor := HudCompositor.new()
	host.set_checker_compositor(compositor)
	var native := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	compositor.call("_draw_checkerboard", native, Vector2i(7, 3), 1, 255)
	var context := {"checker_uv": Vector2i(7, 3), "checker_season": 1, "checker_fade": 255}
	host.size = Vector2(1920, 1080)
	host.set_preferences_override({"display": {"aspect": "wide", "wing_fill": "source"}})
	host.present(native, 0x40, context)
	_check(host.last_wing_source == "checker" and host.error.is_empty(), "Class A + source shows checker wings")
	var shown: Image = host.presented_image
	_check(shown.get_pixel(52, 5) == native.get_pixel((52 - 53) & 31, 5), "checker wing continues the native checker")
	host.present(native, 0x01, context)
	_check(host.last_wing_source == "black" and host.presented_image.get_pixel(52, 5) == Color(0, 0, 0, 1), "Class B stays black with source")
	host.set_preferences_override({"display": {"aspect": "wide", "wing_fill": "black"}})
	host.present(native, 0x40, context)
	_check(host.last_wing_source == "black", "Class A with black pref stays black")
	host.set_preferences_override({"display": {"aspect": "wide", "wing_fill": "source"}})
	# The fallback frame must be newly composed: present checker wings first,
	# then fail with a different native Image, so a frame left over from the
	# previous present() cannot pass.
	host.present(native, 0x40, context)
	var fresh := _native(91)
	host.present(fresh, 0x40, {})
	_check(host.last_wing_source == "black" and not host.error.is_empty() and host.presented_image != null, "missing checker state falls back to black wings and records why")
	_check_black_fallback(host, fresh, "missing checker state")
	host.set_checker_compositor(NoTileCompositor.new())
	host.present(native, 0x40, context)
	_check(host.last_wing_source == "black" and host.error.contains("checker tile unavailable"), "unavailable checker tile is an error reported as black, not checker: " + host.error)
	_check_black_fallback(host, native, "unavailable checker tile")
	host.set_checker_compositor(compositor)
	host.set_preferences_override({"display": {"aspect": "original", "wing_fill": "source"}})
	host.present(native, 0x40, context)
	_check(host.last_wing_source == "none" and host.presented_image == native, "original mode never composes, even with source wings")

func _check_black_fallback(host: Control, native: Image, tag: String) -> void:
	var shown: Image = host.presented_image
	var width: int = host.layout.logical_width
	_check(shown != native and shown.get_size() == Vector2i(width, 240), tag + ": fallback is a new Wl x 240 frame")
	if shown.get_size() != Vector2i(width, 240):
		return
	_check(_hash(Composer.centre_crop(shown)) == _hash(native), tag + ": fallback centre is the new native Image")
	var wing := (width - 320) >> 1
	var black := true
	for y: int in range(0, 240, 7):
		for x: int in [0, wing - 1, wing + 320, width - 1]:
			black = black and shown.get_pixel(x, y) == Color(0, 0, 0, 1)
	_check(black, tag + ": fallback wings are black")

func _check_overlay(host: Control) -> void:
	var native := _native(5)
	var before := _hash(native)
	host.overlay.set_stores(FakeTranslations.new())
	host.size = Vector2(960, 720)
	host.set_preferences_override({"display": {"aspect": "original"}, "accessibility": {"text_box": "modern"}})
	host.present(native, 0x01)
	_check(host.show_dialogue_line({"payload_sha": PAYLOAD, "line": 0}) and host.overlay.visible, "overlay shows through the host")
	# presented_image is what the TextureRect's texture was built or updated
	# from (the headless dummy renderer does not reflect ImageTexture.update in
	# get_image, so the texture is not read back here).
	var with_overlay: Image = host.presented_image
	host.clear_dialogue_line()
	var without_overlay: Image = host.presented_image
	_check(_hash(native) == before, "overlay on/off leaves the native Image unchanged")
	_check(with_overlay.get_data() == without_overlay.get_data(), "overlay on/off leaves the displayed texture unchanged")
	_check(_hash(with_overlay) == before, "displayed Image is the native Image")
	_check(host.get_child(1) == host.overlay and host.get_child(0) == host.display, "overlay is mounted above the display")
	host.set_preferences_override({"display": {"aspect": "original"}, "accessibility": {"text_box": "native"}})
	host.show_dialogue_line({"payload_sha": PAYLOAD, "line": 0})
	_check(not host.overlay.visible, "text_box=native hides the overlay")

func _check_negatives(host: Control) -> void:
	var good := _native(3)
	host.set_preferences_override({"display": {"aspect": "original"}})
	host.present(good)
	var texture: Texture2D = host.display.texture
	_check(not host.present(Image.create(320, 200, false, Image.FORMAT_RGBA8)) and not host.error.is_empty(), "wrong-size native rejected")
	_check(not host.present(null), "null native rejected")
	_check(host.display.texture == texture and host.presented_image == good, "rejected frames leave the display unchanged")
	host.size = Vector2.ZERO
	host.relayout()
	_check(host.layout.valid == (host.window_size().x > 0.0), "zero-size host falls back to the viewport size")
