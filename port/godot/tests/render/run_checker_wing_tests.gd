extends SceneTree
## T-30001 WP3: checker wing provider seams and hud_compositor checker identity.
## Optional differential: set HUD_COMPOSITOR_BASE to the pre-T-30001 copy of
## presentation/hud_compositor.gd (mounted read-only); the native checkerboard
## must then match it byte for byte for every season, fade and a uv grid.

const HudCompositor = preload("res://presentation/hud_compositor.gd")
const Provider = preload("res://presentation/checker_wing_provider.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const Policy = preload("res://presentation/scene_aperture_policy.gd")
const UVS := [Vector2i(0, 0), Vector2i(5, -3), Vector2i(17, 29), Vector2i(-40, 100), Vector2i(31, 31), Vector2i(1023, -777)]
const WIDTHS := [322, 384, 426, 428, 500, 560]

class NoTileCompositor extends RefCounted:
	func checker_tile(_uv: Vector2i, _season: int, _fade: int) -> Image:
		return null

var _checks := 0
var _failures: Array[String] = []
var differential_frames := 0

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

static func _native_checker(compositor: RefCounted, uv: Vector2i, season: int, fade: int) -> Image:
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	compositor.call("_draw_checkerboard", image, uv, season, fade)
	return image

func _initialize() -> void:
	var compositor := HudCompositor.new()
	_check_tile_function(compositor)
	_check_seams(compositor)
	_check_negatives(compositor)
	_check_base_differential(compositor)
	if _failures.is_empty():
		print("PASS: checker wings (%d checks, %d differential frames)" % [_checks, differential_frames])
		quit(0)
	else:
		for failure: String in _failures:
			printerr("FAIL: ", failure)
		quit(1)

func _check_tile_function(compositor: RefCounted) -> void:
	for season: int in 4:
		for fade: int in [239, 247, 255]:
			for uv: Vector2i in UVS:
				var native := _native_checker(compositor, uv, season, fade)
				var tile: Image = compositor.checker_tile(uv, season, fade)
				var bad := 0
				for y: int in 240:
					for x: int in 320:
						if native.get_pixel(x, y) != tile.get_pixel(x & 31, y & 31):
							bad += 1
				_check(tile.get_size() == Vector2i(32, 32) and bad == 0, "tile function reproduces the native checker s=%d f=%d uv=%s (%d diffs)" % [season, fade, uv, bad])
				tile.fill(Color.RED)
				_check(compositor.checker_tile(uv, season, fade).get_pixel(0, 0) != Color.RED, "checker_tile returns a copy s=%d f=%d uv=%s" % [season, fade, uv])
	var spring: Image = compositor.checker_tile(Vector2i.ZERO, 0, 255)
	var summer: Image = compositor.checker_tile(Vector2i.ZERO, 1, 255)
	_check(spring.get_data() != summer.get_data(), "seasons give different tiles")
	_check(compositor.checker_tile(Vector2i.ZERO, 1, 239).get_data() != summer.get_data(), "fade 239 differs from 255")

func _check_seams(compositor: RefCounted) -> void:
	var provider := Provider.new(compositor)
	var composer := Composer.new()
	for season: int in 4:
		for fade: int in [239, 255]:
			for uv: Vector2i in UVS:
				var native := _native_checker(compositor, uv, season, fade)
				var context := {"checker_uv": uv, "checker_season": season, "checker_fade": fade}
				for width: int in WIDTHS:
					var tag := "s=%d f=%d uv=%s Wl=%d" % [season, fade, uv, width]
					var composed := composer.compose(native, width, provider, context)
					_check(composed != null and composer.error.is_empty(), tag + " composes: " + composer.error)
					if composed == null:
						continue
					var wing := (width - 320) >> 1
					var bad := 0
					for y: int in 240:
						# Independent of the tile function: the native checker repeats
						# every 32 columns, so wing column c must equal native column
						# (c - wing) & 31.
						for c: int in range(wing):
							if composed.get_pixel(c, y) != native.get_pixel((c - wing) & 31, y):
								bad += 1
						for c: int in range(wing + 320, width):
							if composed.get_pixel(c, y) != native.get_pixel((c - wing) & 31, y):
								bad += 1
						if composed.get_pixel(wing - 1, y) != native.get_pixel(31, y) or composed.get_pixel(wing, y) != native.get_pixel(0, y):
							bad += 1
						if composed.get_pixel(wing + 319, y) != native.get_pixel(319, y) or composed.get_pixel(wing + 320, y) != native.get_pixel(0, y):
							bad += 1
					_check(bad == 0, tag + " seam and wing pixels continue the checker (%d diffs)" % bad)
					_check(Composer.centre_crop(composed).get_data() == native.get_data(), tag + " centre byte-identical")
	# Month resolves the season the way render() does.
	var by_month := composer.compose(_native_checker(compositor, Vector2i.ZERO, 2, 255), 426, provider,
		{"checker_uv": Vector2i.ZERO, "month": 10, "checker_fade": 255})
	var by_season := composer.compose(_native_checker(compositor, Vector2i.ZERO, 2, 255), 426, provider,
		{"checker_uv": Vector2i.ZERO, "checker_season": 2, "checker_fade": 255})
	_check(by_month != null and by_month.get_data() == by_season.get_data(), "month 10 resolves autumn like render()")
	# A non-RGBA target still gets the same colours.
	var rgb := _native_checker(compositor, Vector2i(3, 4), 3, 239)
	rgb.convert(Image.FORMAT_RGB8)
	var rgb_composed := composer.compose(rgb, 426, provider, {"checker_uv": Vector2i(3, 4), "checker_season": 3, "checker_fade": 239})
	_check(rgb_composed != null and rgb_composed.get_pixel(52, 10) == rgb.get_pixel((52 - 53) & 31, 10), "RGB8 target continues the checker")

func _check_negatives(compositor: RefCounted) -> void:
	var provider := Provider.new(compositor)
	var target := Image.create(426, 240, false, Image.FORMAT_RGBA8)
	_check(provider.fill_wings(target, 53, {"checker_season": 0, "checker_fade": 255}) == "checker_uv missing", "missing uv rejected")
	_check(not provider.fill_wings(target, 53, {"checker_uv": Vector2i.ZERO, "checker_season": 0}).is_empty(), "missing fade rejected")
	_check(not provider.fill_wings(target, 53, {"checker_uv": Vector2i.ZERO, "checker_season": 0, "checker_fade": 238}).is_empty(), "fade 238 rejected")
	_check(not provider.fill_wings(target, 53, {"checker_uv": Vector2i.ZERO, "checker_fade": 255}).is_empty(), "missing season rejected")
	_check(not provider.fill_wings(target, 52, {"checker_uv": Vector2i.ZERO, "checker_season": 0, "checker_fade": 255}).is_empty(), "wing/target mismatch rejected")
	var composer := Composer.new()
	var native := _native_checker(compositor, Vector2i.ZERO, 0, 255)
	_check(composer.compose(native, 426, provider, {}) == null and composer.error.contains("checker_uv"), "composer surfaces provider errors")
	# No tile: the compositor fills the native frame teal, so black wings would
	# show a seam. The provider must fail rather than claim checker wings.
	var no_tile := Provider.new(NoTileCompositor.new())
	var state := {"checker_uv": Vector2i.ZERO, "checker_season": 0, "checker_fade": 255}
	_check(no_tile.fill_wings(target, 53, state) == "checker tile unavailable" and no_tile.fallbacks == 1, "unavailable tile is an error, not silent black wings")
	_check(composer.compose(native, 426, no_tile, state) == null and composer.error.contains("checker tile unavailable"), "composer surfaces the unavailable tile")
	# Policy gate: the checker is only selected for Class A with wing_fill=source.
	var source := {"display": {"wing_fill": "source"}}
	_check(Policy.wing_source(0x34, source) == Policy.WING_CHECKER and Policy.wing_source(0x42, source) == Policy.WING_CHECKER, "Class A + source selects the checker")
	_check(Policy.wing_source(0x34, {"display": {"wing_fill": "black"}}) == Policy.WING_BLACK, "Class A + black stays black")
	for scene: Variant in [0x01, "VN", "BUSTUP", 0x10, 0x43, "TACO", null]:
		_check(Policy.wing_source(scene, source) == Policy.WING_BLACK, "%s never selects the checker" % [scene])

func _check_base_differential(compositor: RefCounted) -> void:
	var path := OS.get_environment("HUD_COMPOSITOR_BASE")
	if path.is_empty():
		print("NOTE: HUD_COMPOSITOR_BASE unset; base differential skipped")
		return
	var base_script: Variant = load(path)
	_check(base_script is GDScript, "base compositor loads: " + path)
	if not base_script is GDScript:
		return
	var base: RefCounted = base_script.new()
	for season: int in 4:
		for fade: int in range(239, 256):
			for uv: Vector2i in UVS + [Vector2i(-1, -1), Vector2i(16, 0), Vector2i(0, 16)]:
				var current := _native_checker(compositor, uv, season, fade)
				var before := _native_checker(base, uv, season, fade)
				differential_frames += 1
				_check(current.get_data() == before.get_data(), "native checker unchanged vs base s=%d f=%d uv=%s" % [season, fade, uv])
