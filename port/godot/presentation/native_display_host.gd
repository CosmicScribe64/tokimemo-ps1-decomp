extends Control
## T-30001 WP6 (host part): the single owner of how the native 320x240 Image
## is framed in the window. It owns the TextureRect, the wide frame composer,
## the wing providers and the modern dialogue overlay mount, and lays them out
## again on resize and on ModernSettings.preferences_changed.
##
## The host only reads the native Image. aspect=original shows that Image
## as is (no composition) in the same rect the boots computed before T-30001;
## wide shows the composed Wl x 240 frame. The boots keep their own
## last_rendered_image for fidelity checks. Boot wiring is applied by the root
## agent (hook documented with the T-30001 wide-core patch notes).
##
## Window-space margins for modern UI (pause/settings/hints) come from
## ui_margin_rects(); capabilities() reports what this host can present.

signal layout_changed(layout: Dictionary)

const Layout = preload("res://presentation/native_frame_layout.gd")
const Policy = preload("res://presentation/scene_aperture_policy.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const CheckerProvider = preload("res://presentation/checker_wing_provider.gd")
const Overlay = preload("res://presentation/modern_dialogue_overlay.gd")

## Node that provides preferences_changed and preferences.effective_settings().
var settings_node_path := NodePath("/root/ModernSettings")
var display: TextureRect
var overlay: Control
var composer := Composer.new()
var layout: Dictionary = {}
## The Image currently shown: the native Image in original mode (same object,
## never modified), or the composed frame in wide mode.
var presented_image: Image
## "none", "black" or "checker" for the last present().
var last_wing_source := "none"
## Last composition problem; the host then shows black wings, never nothing.
var error := ""
var _native: Image
var _scene: Variant = null
var _wing_context: Dictionary = {}
var _prefs_override: Variant = null
var _checker: RefCounted
var _texture: ImageTexture
## A live 320x240 texture (a SubViewport's) shown instead of an Image; see
## present_texture().
var _live_texture: Texture2D

func _init() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	display = TextureRect.new()
	display.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	display.stretch_mode = TextureRect.STRETCH_SCALE
	display.texture_filter = CanvasItem.TEXTURE_FILTER_PARENT_NODE
	display.mouse_filter = Control.MOUSE_FILTER_IGNORE
	add_child(display)
	overlay = Overlay.new()
	overlay.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	add_child(overlay)

## Bars and wings are black (display.bars has only "black"), whatever the
## clear colour of the scene behind the host is.
func _draw() -> void:
	draw_rect(Rect2(Vector2.ZERO, size), Color.BLACK)

func _ready() -> void:
	resized.connect(relayout)
	var settings := get_node_or_null(settings_node_path)
	if settings != null and settings.has_signal("preferences_changed"):
		settings.preferences_changed.connect(_on_preferences_changed)
	relayout()

## Optional fixed preferences (tests, tools, or scenes without ModernSettings).
## Pass null to read ModernSettings again.
func set_preferences_override(prefs: Variant) -> void:
	_prefs_override = prefs.duplicate(true) if prefs is Dictionary else null
	relayout()

## The hud_compositor that rendered Class A frames; its checker tile feeds the
## source wings. Without one, the provider opens its own compositor.
func set_checker_compositor(hud_compositor: RefCounted) -> void:
	_checker = CheckerProvider.new(hud_compositor)

## Any object with fill_wings(target, wing_width, context) (see
## wide_frame_composer.gd) used for source wings on Class A scenes, for
## scenes whose checker is not drawn by hud_compositor.
func set_wing_provider(provider: Object) -> void:
	_checker = provider

func preferences() -> Dictionary:
	if _prefs_override is Dictionary:
		return _prefs_override
	var settings := get_node_or_null(settings_node_path) if is_inside_tree() else null
	if settings != null and settings.get("preferences") != null and settings.preferences.has_method("effective_settings"):
		return settings.preferences.effective_settings()
	return {}

## Window size the host frames into: its own rect when laid out, else the
## viewport (the boots make the host full-rect, so both agree there).
func window_size() -> Vector2:
	if size.x > 0.0 and size.y > 0.0:
		return size
	return get_viewport_rect().size if is_inside_tree() else Vector2.ZERO

## Shows a native frame. scene is a mode id or tag for SceneAperturePolicy;
## wing_context carries the checker state hud_compositor rendered with.
## Returns false (display unchanged) when the Image is not 320x240.
func present(native: Image, scene: Variant = null, wing_context: Dictionary = {}) -> bool:
	if native == null or native.is_empty() or native.get_size() != Layout.NATIVE_SIZE:
		error = "native image must be 320x240"
		return false
	_native = native
	_live_texture = null
	_scene = scene
	_wing_context = wing_context
	_refresh_image()
	return true

## Shows a live 320x240 texture (for example a SubViewport that a NativeCanvas
## draws into) without reading it back. The texture is placed on the aperture;
## in wide mode the wings stay black (the window background), because source
## wings need the native pixels. Returns false for any other size.
func present_texture(texture: Texture2D, scene: Variant = null) -> bool:
	if texture == null or Vector2i(texture.get_size()) != Layout.NATIVE_SIZE:
		error = "native texture must be 320x240"
		return false
	_live_texture = texture
	_native = null
	presented_image = null
	_scene = scene
	last_wing_source = "none" if layout.get("logical_width", Layout.NATIVE_SIZE.x) == Layout.NATIVE_SIZE.x else Policy.WING_BLACK
	_place_display()
	display.texture = texture
	return true

func relayout(_unused: Variant = null) -> void:
	var prefs := preferences()
	layout = Layout.compute(window_size(), prefs)
	if not layout.valid:
		return
	_place_display()
	overlay.apply_layout(layout, prefs)
	_refresh_image()
	layout_changed.emit(layout)

func _on_preferences_changed(_settings: Variant = null) -> void:
	relayout()

func _place_display() -> void:
	if not layout.get("valid", false):
		return
	var rect: Rect2 = layout.aperture if _live_texture != null else layout.frame
	display.position = rect.position
	display.size = rect.size

func _refresh_image() -> void:
	if _live_texture != null and layout.get("valid", false):
		last_wing_source = "none" if layout.logical_width == Layout.NATIVE_SIZE.x else Policy.WING_BLACK
		return
	if _native == null or not layout.get("valid", false):
		return
	error = ""
	var prefs := preferences()
	if layout.logical_width == Layout.NATIVE_SIZE.x:
		presented_image = _native
		last_wing_source = "none"
	else:
		var provider: Object = null
		last_wing_source = Policy.wing_source(_scene, prefs)
		if last_wing_source == Policy.WING_CHECKER:
			if _checker == null:
				_checker = CheckerProvider.new()
			provider = _checker
		var composed := composer.compose(_native, layout.logical_width, provider, _wing_context)
		if composed == null and provider != null:
			error = composer.error
			last_wing_source = Policy.WING_BLACK
			composed = composer.compose(_native, layout.logical_width)
		if composed == null:
			error = composer.error
			return
		presented_image = composed
	if _texture != null and _texture.get_size() == Vector2(presented_image.get_size()) and _texture.get_format() == presented_image.get_format():
		_texture.update(presented_image)
	else:
		_texture = ImageTexture.create_from_image(presented_image)
	display.texture = _texture

func window_to_native(point: Vector2) -> Dictionary:
	return Layout.window_to_native(layout, point)

func native_to_window(pixel: Vector2i) -> Vector2:
	return Layout.native_to_window(layout, pixel)

## Window-space rects outside the presented frame (bars), largest first and
## only those at least 1px wide and tall, for modern UI placed beside the
## picture. Empty when the frame fills the window.
func ui_margin_rects() -> Array[Rect2]:
	var result: Array[Rect2] = []
	if not layout.get("valid", false):
		return result
	for rect: Rect2 in layout.bars:
		if rect.size.x >= 1.0 and rect.size.y >= 1.0:
			result.append(rect)
	result.sort_custom(func(a: Rect2, b: Rect2) -> bool: return a.get_area() > b.get_area())
	return result

## What this host can present. Wide framing and source wings are presentation
## only; the native Image, RNG and game state are untouched in every mode.
func capabilities() -> Dictionary:
	return {"widescreen_available": true, "aspects": Layout.ASPECTS.duplicate(),
		"wing_fills": Policy.WING_FILLS.duplicate(), "source_wing_classes": [Policy.CLASS_A],
		"text_boxes": Overlay.TEXT_BOXES.duplicate(), "bars": "black"}

func show_dialogue_line(key: Dictionary) -> bool:
	return overlay.show_line(key)

func clear_dialogue_line() -> void:
	overlay.clear_line()
