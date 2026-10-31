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
##
## T-39220: with display.wing_art on, a WingArtProvider fills the wings and a
## backdrop TextureRect behind the frame fills the side bars and the top and
## bottom bands at the same pixel scale. Motion follows a presentation clock
## (clock_usec), never game state or the game RNG; the month and scene come
## from present()'s wing_context or set_calendar_month(), which a
## WingArtSceneFeed (presentation/wing_art_scene_feed.gd) drives read-only from
## the game state. Each scene class may override the mode
## (display.wing_art_command/_story/_other). A missing or damaged
## wing_art pack only leaves black wings and records `error`.

signal layout_changed(layout: Dictionary)

const Layout = preload("res://presentation/native_frame_layout.gd")
const Policy = preload("res://presentation/scene_aperture_policy.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const CheckerProvider = preload("res://presentation/checker_wing_provider.gd")
const Overlay = preload("res://presentation/modern_dialogue_overlay.gd")
const WingArt = preload("res://presentation/wing_art_provider.gd")
const USER_OVERRIDES_PATH := "user://wing-art-overrides.json"

## Node that provides preferences_changed and preferences.effective_settings().
var settings_node_path := NodePath("/root/ModernSettings")
var display: TextureRect
## Window backdrop for wing art (behind display; hidden unless wing art shows).
var backdrop_display: TextureRect
var overlay: Control
var composer := Composer.new()
var layout: Dictionary = {}
## The Image currently shown: the native Image in original mode (same object,
## never modified), or the composed frame in wide mode.
var presented_image: Image
## "none", "black", "checker" or "wing_art" for the last present().
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
## Wing art provider (created when display.wing_art is first turned on).
var wing_art: RefCounted
## Pack directory for wing art; empty means discover (env, then install).
var wing_art_pack_path := ""
## Last in-game month the boot reported (1..12), 0 when unknown.
var calendar_month := 0
## Presentation clock in microseconds; tests replace it.
var clock_usec: Callable = func() -> int: return Time.get_ticks_usec()
## Style id shown by the last refresh ("" when wing art is off).
var last_wing_art_style := ""
## True once a WingArtSceneFeed reports the game's month and scene facts.
## Until then "seasonal" and "scene" can only show the default (April) look,
## and the settings page says so.
var wing_art_follows_game := false
var _wing_art_opened := false
var _clock_origin := -1
var _last_tick := -1
var _animating := false
var _backdrop_texture: ImageTexture
var _preview_art: RefCounted

func _init() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	backdrop_display = TextureRect.new()
	backdrop_display.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	backdrop_display.stretch_mode = TextureRect.STRETCH_SCALE
	backdrop_display.texture_filter = CanvasItem.TEXTURE_FILTER_PARENT_NODE
	backdrop_display.mouse_filter = Control.MOUSE_FILTER_IGNORE
	backdrop_display.visible = false
	add_child(backdrop_display)
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

## Wing art pack directory (the installed wing_art role). Reopens the pack.
func set_wing_art_pack(path: String) -> void:
	wing_art_pack_path = path
	_wing_art_opened = false
	wing_art = null
	_preview_art = null
	_refresh_image()

## The in-game month (1..12), read by the boot from game state. Only selects
## the seasonal style; nothing is written back.
func set_calendar_month(month: int) -> void:
	calendar_month = month if month >= 1 and month <= 12 else 0
	_refresh_image()

func presentation_tick() -> int:
	var now: int = clock_usec.call()
	if _clock_origin < 0:
		_clock_origin = now
	return WingArt.tick_for_usec(maxi(now - _clock_origin, 0))

func _process(_delta: float) -> void:
	if _animating and presentation_tick() != _last_tick:
		_refresh_image()

func _open_wing_art() -> RefCounted:
	if _wing_art_opened:
		return wing_art
	_wing_art_opened = true
	wing_art = WingArt.new()
	var path := wing_art_pack_path
	if path.is_empty():
		path = OS.get_environment("TOKIMEMO_WING_ART_PACK")
	if path.is_empty():
		path = WingArt.discover_pack_path()
	if wing_art.open_pack(path) and FileAccess.file_exists(USER_OVERRIDES_PATH):
		wing_art.set_user_overrides(JSON.parse_string(FileAccess.get_file_as_string(USER_OVERRIDES_PATH)))
	return wing_art

## Context for the wing art provider: month, scene facts, tick, visible side.
func wing_art_context(prefs: Dictionary, mode: String) -> Dictionary:
	var context := _wing_context.duplicate()
	if not context.has("month") and calendar_month > 0:
		context["month"] = calendar_month
	context["scene"] = _scene
	context["wing_art_mode"] = mode
	context["reduce_motion"] = WingArt.accessibility_flag(prefs, "reduce_motion")
	context["reduce_flashes"] = WingArt.accessibility_flag(prefs, "reduce_flashes")
	context["tick"] = presentation_tick()
	var factor: float = maxf(float(layout.get("factor", 1.0)), 0.0001)
	context["visible_side"] = floori(layout.aperture.position.x / factor) if layout.get("valid", false) else 0
	return context

func _refresh_image() -> void:
	if _live_texture != null and layout.get("valid", false):
		last_wing_source = "none" if layout.logical_width == Layout.NATIVE_SIZE.x else Policy.WING_BLACK
		return
	if _native == null or not layout.get("valid", false):
		return
	error = ""
	var prefs := preferences()
	var art_mode := WingArt.mode_for_scene(prefs, _scene)
	var art: RefCounted = null
	var art_context: Dictionary = {}
	if art_mode != "off":
		art = _open_wing_art()
		if art.is_ready():
			art_context = wing_art_context(prefs, art_mode)
		else:
			error = "wing art unavailable: " + str(art.error)
			art = null
	var checker: bool = layout.logical_width != Layout.NATIVE_SIZE.x and Policy.wing_source(_scene, prefs) == Policy.WING_CHECKER
	# The checker continuation is the scene's own art and wins over wing art.
	var backdrop: Image = art.render(art_mode, art_context) if art != null and not checker else null
	last_wing_art_style = str(art.last_style) if art != null and not checker else ""
	_animating = art != null and not checker and not art_context.get("reduce_motion", false)
	_last_tick = int(art_context.get("tick", -1))
	_show_backdrop(backdrop, art)
	if layout.logical_width == Layout.NATIVE_SIZE.x:
		presented_image = _native
		last_wing_source = "none"
	else:
		var provider: Object = null
		var context := _wing_context
		last_wing_source = Policy.wing_source(_scene, prefs)
		if last_wing_source == Policy.WING_CHECKER:
			if _checker == null:
				_checker = CheckerProvider.new()
			provider = _checker
		elif backdrop != null:
			provider = art
			context = art_context
			last_wing_source = "wing_art"
		var composed := composer.compose(_native, layout.logical_width, provider, context)
		if composed == null and provider != null:
			error = composer.error
			_show_backdrop(null, null)
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

func _show_backdrop(image: Image, art: RefCounted) -> void:
	if image == null or art == null:
		backdrop_display.visible = false
		return
	var factor: float = layout.factor
	var origin := Vector2(art.geometry.aperture_x, art.geometry.aperture_y)
	backdrop_display.position = layout.aperture.position - origin * factor
	backdrop_display.size = Vector2(image.get_size()) * factor
	if _backdrop_texture != null and _backdrop_texture.get_size() == Vector2(image.get_size()) and _backdrop_texture.get_format() == image.get_format():
		_backdrop_texture.update(image)
	else:
		_backdrop_texture = ImageTexture.create_from_image(image)
	backdrop_display.texture = _backdrop_texture
	backdrop_display.visible = true

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
	var art_ready: bool = wing_art != null and wing_art.is_ready()
	return {"widescreen_available": true, "aspects": Layout.ASPECTS.duplicate(),
		"wing_fills": Policy.WING_FILLS.duplicate(), "source_wing_classes": [Policy.CLASS_A],
		"text_boxes": Overlay.TEXT_BOXES.duplicate(), "bars": "wing_art" if backdrop_display.visible else "black",
		"wing_art_modes": WingArt.all_modes(), "wing_art_available": art_ready,
		"wing_art_follows_game": wing_art_follows_game}

## A still preview of a wing art mode around the current native frame, at
## logical size (480 x 270 is the 16:9 window), for the settings page.
func wing_art_preview(mode: String, width: int = 480) -> Image:
	var art := _open_wing_art()
	if not art.is_ready():
		return null
	if _preview_art == null:
		_preview_art = art.spawn()
	var prefs := preferences()
	var context := wing_art_context(prefs, mode)
	return _preview_art.preview(mode, context, _native, width)

func show_dialogue_line(key: Dictionary) -> bool:
	return overlay.show_line(key)

func clear_dialogue_line() -> void:
	overlay.clear_line()
