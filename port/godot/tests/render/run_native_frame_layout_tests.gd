extends SceneTree
## T-30001 WP1: NativeFrameLayout geometry properties. Headless, no game data.

const Layout = preload("res://presentation/native_frame_layout.gd")
const SIZES := [Vector2(640, 480), Vector2(960, 720), Vector2(1280, 720), Vector2(1366, 768),
	Vector2(1920, 1080), Vector2(1920, 1200), Vector2(2560, 1080), Vector2(3440, 1440)]
const BELOW_NATIVE := Vector2(300, 200)

var _checks := 0
var _failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

## The fit first_arrival_boot._layout_display computes before T-30001.
static func _legacy_rect(area: Vector2, filter: String) -> Rect2:
	var factor := minf(area.x / 320.0, area.y / 240.0)
	if filter == "integer" and factor >= 1.0:
		factor = floorf(factor)
	var size := Vector2(320, 240) * factor
	return Rect2((area - size) * 0.5, size)

static func _whole(value: float) -> bool:
	return value == floorf(value)

static func _whole_rect(rect: Rect2) -> bool:
	return _whole(rect.position.x) and _whole(rect.position.y) and _whole(rect.end.x) and _whole(rect.end.y)

func _initialize() -> void:
	for size: Vector2 in SIZES + [BELOW_NATIVE]:
		for aspect: String in Layout.ASPECTS:
			for filter: String in Layout.FILTERS:
				_check_case(size, aspect, filter)
	_check_named_cases()
	_check_negatives()
	_check_round_trip()
	if _failures.is_empty():
		print("PASS: native frame layout (%d checks)" % _checks)
		quit(0)
	else:
		for failure: String in _failures:
			printerr("FAIL: ", failure)
		quit(1)

func _check_case(size: Vector2, aspect: String, filter: String) -> void:
	var tag := "%dx%d %s/%s" % [size.x, size.y, aspect, filter]
	var layout := Layout.compute(size, {"display": {"aspect": aspect, "filter": filter}})
	_check(layout.valid, tag + " valid")
	var factor: float = layout.factor
	var frame: Rect2 = layout.frame
	var aperture: Rect2 = layout.aperture
	var wl: int = layout.logical_width
	_check(factor > 0.0, tag + " factor > 0")
	_check(wl % 2 == 0 and wl >= 320 and wl <= 560, tag + " Wl even and within 320..560")
	_check(layout.wing_width * 2 + 320 == wl, tag + " wing width matches Wl")
	if filter == "integer" and size.x >= 320 and size.y >= 240:
		_check(factor >= 1.0 and _whole(factor), tag + " integer factor >= 1")
		_check(_whole_rect(frame) and _whole_rect(aperture), tag + " integer rects on whole pixels")
		for wing: Rect2 in layout.wings: _check(_whole_rect(wing), tag + " wing on whole pixels")
		_check(_whole_rect(layout.text_band), tag + " text band on whole pixels")
	_check(_whole(frame.position.x) and _whole(frame.position.y), tag + " frame origin whole")
	_check(frame.size.x <= size.x + 0.001 and frame.size.y <= size.y + 0.001, tag + " frame fits window")
	var left := frame.position.x
	var right := size.x - frame.end.x
	var top := frame.position.y
	var bottom := size.y - frame.end.y
	_check(absf(left - right) <= 1.0 and absf(top - bottom) <= 1.0, tag + " symmetric bars within 1px")
	_check(aperture.size.is_equal_approx(Vector2(320, 240) * factor), tag + " aperture is 320x240 * factor")
	_check(is_equal_approx(aperture.get_center().x, frame.get_center().x), tag + " aperture centred in frame")
	_check(is_equal_approx(frame.size.x, wl * factor), tag + " frame width = Wl * factor")
	var band: Rect2 = layout.text_band
	_check(is_equal_approx(band.position.y, aperture.position.y + 160.0 * factor) and is_equal_approx(band.end.y, aperture.end.y), tag + " text band covers native rows 160..239")
	_check(is_equal_approx(band.position.x, aperture.position.x) and is_equal_approx(band.size.x, aperture.size.x), tag + " text band spans the aperture")
	_check(is_equal_approx(layout.text_band_wide.size.x, frame.size.x), tag + " wide text band spans the frame")
	var expected_mode := "wide" if aspect == "wide" or (aspect == "auto" and size.x / size.y >= 1.5) else "original"
	_check(layout.mode == expected_mode, tag + " resolved mode")
	if expected_mode == "original":
		_check(wl == 320 and frame == aperture, tag + " original mode has no wings")
		var legacy := _legacy_rect(size, filter)
		if size != BELOW_NATIVE:
			_check(aperture == legacy, tag + " original reproduces first_arrival_boot rect exactly")
		else:
			# Below native the legacy rect starts between pixels; the layout
			# rounds its origin to the nearest pixel and keeps the size.
			var shift := legacy.position - aperture.position
			_check(aperture.size == legacy.size and absf(shift.x) <= 0.5 and absf(shift.y) <= 0.5,
				tag + " original below native differs from legacy by a sub-pixel origin snap only")
	else:
		var wings: Array = layout.wings
		_check(wings[0].end.is_equal_approx(Vector2(aperture.position.x, frame.end.y)), tag + " left wing ends at aperture")
		_check(is_equal_approx(wings[1].position.x, aperture.end.x) and is_equal_approx(wings[1].end.x, frame.end.x), tag + " right wing ends at frame edge")

func _check_named_cases() -> void:
	var expected := {
		Vector2(1920, 1080): [426, 4.0, Rect2(108, 60, 1704, 960)],
		Vector2(1280, 720): [426, 3.0, Rect2(1, 0, 1278, 720)],
		Vector2(1366, 768): [426, 3.0, Rect2(44, 24, 1278, 720)],
		Vector2(1920, 1200): [384, 5.0, Rect2(0, 0, 1920, 1200)],
		Vector2(2560, 1080): [560, 4.0, Rect2(160, 60, 2240, 960)],
		Vector2(3440, 1440): [560, 6.0, Rect2(40, 0, 3360, 1440)],
		Vector2(640, 480): [320, 2.0, Rect2(0, 0, 640, 480)],
	}
	for size: Vector2 in expected:
		var layout := Layout.compute(size, {"aspect": "wide", "filter": "integer"})
		var row: Array = expected[size]
		_check(layout.logical_width == row[0] and layout.factor == row[1] and layout.frame == row[2],
			"wide integer %s gives Wl=%d factor=%s frame=%s (got %d %s %s)" % [size, row[0], row[1], row[2], layout.logical_width, layout.factor, layout.frame])
	var original := Layout.compute(Vector2(1920, 1080), {"aspect": "original", "filter": "integer"})
	_check(original.aperture == Rect2(320, 60, 1280, 960), "original integer 1920x1080 is 4x centred")
	var fractional := Layout.compute(Vector2(1920, 1080), {"aspect": "original", "filter": "nearest"})
	_check(fractional.aperture == Rect2(240, 0, 1440, 1080) and fractional.factor == 4.5, "original fractional 1920x1080 is 4.5x")
	var below := Layout.compute(BELOW_NATIVE, {"aspect": "original", "filter": "integer"})
	_check(below.factor < 1.0 and below.factor > 0.0, "below-native keeps a fractional fallback, never zero")
	# auto switches exactly at W/H = 1.5.
	_check(Layout.compute(Vector2(1500, 1000), {"aspect": "auto"}).mode == "wide", "auto is wide at 1.5")
	_check(Layout.compute(Vector2(1499, 1000), {"aspect": "auto"}).mode == "original", "auto is original below 1.5")
	_check(Layout.compute(Vector2(1280, 720), {"aspect": "auto"}).mode == "wide", "auto is wide at 16:9")
	_check(Layout.compute(Vector2(960, 720), {"aspect": "auto"}).mode == "original", "auto is original at 4:3")
	# Odd sizes keep bars within 1px and whole-pixel integer rects.
	var odd := Layout.compute(Vector2(1001, 757), {"aspect": "wide", "filter": "integer"})
	_check(_whole_rect(odd.frame) and _whole_rect(odd.aperture), "odd window integer rects whole")
	# Narrow window that would have rounded Wl above its width still gives factor 1.
	var narrow := Layout.compute(Vector2(321, 240), {"aspect": "wide", "filter": "integer"})
	_check(narrow.factor == 1.0 and narrow.frame.size.x <= 321.0, "narrow wide window keeps factor 1 and fits")

func _check_negatives() -> void:
	var defaults := Layout.compute(Vector2(1920, 1080), {})
	_check(defaults.aspect == "original" and defaults.filter == "integer" and defaults.mode == "original", "missing prefs default to original/integer")
	var junk := Layout.compute(Vector2(1920, 1080), {"display": {"aspect": "stretch", "filter": 3}})
	_check(junk.aspect == "original" and junk.filter == "integer", "unknown prefs fall back to defaults")
	for bad: Vector2 in [Vector2.ZERO, Vector2(-5, 100), Vector2(100, 0), Vector2(INF, 100), Vector2(NAN, 10)]:
		var layout := Layout.compute(bad, {"aspect": "wide"})
		_check(not layout.valid, "invalid window %s is rejected" % bad)
		_check(Layout.window_to_native(layout, Vector2(5, 5)).inside == false, "invalid layout transforms report outside")
	# Wide never exceeds 560 or produces a non-native aperture height.
	var ultra := Layout.compute(Vector2(10000, 240), {"aspect": "wide", "filter": "integer"})
	_check(ultra.logical_width == 560 and ultra.aperture.size == Vector2(320, 240), "ultrawide clamps to 560 at 1x")

func _check_round_trip() -> void:
	for size: Vector2 in SIZES + [BELOW_NATIVE]:
		for aspect: String in ["original", "wide"]:
			for filter: String in Layout.FILTERS:
				var layout := Layout.compute(size, {"aspect": aspect, "filter": filter})
				var tag := "%dx%d %s/%s" % [size.x, size.y, aspect, filter]
				var bad := 0
				for py: int in range(0, 240, 7):
					for px: int in range(0, 320, 7):
						var window_point := Layout.native_to_window(layout, Vector2i(px, py))
						var back: Dictionary = Layout.window_to_native(layout, window_point)
						if not back.inside or back.pixel != Vector2i(px, py):
							bad += 1
				_check(bad == 0, tag + " native->window->native round trip (%d misses)" % bad)
				# Window grid -> native -> window -> native lands on the same pixel.
				var misses := 0
				for wy: int in range(0, int(size.y), 13):
					for wx: int in range(0, int(size.x), 17):
						var first: Dictionary = Layout.window_to_native(layout, Vector2(wx, wy))
						var again: Dictionary = Layout.window_to_native(layout, Layout.native_to_window(layout, first.pixel))
						if again.pixel != first.pixel:
							misses += 1
				_check(misses == 0, tag + " window->native->window->native stable (%d misses)" % misses)
				var outside_left: Dictionary = Layout.window_to_native(layout, layout.aperture.position - Vector2(1, 1))
				_check(not outside_left.inside and outside_left.pixel == Vector2i.ZERO, tag + " outside top-left clamps to (0,0)")
				var outside_right: Dictionary = Layout.window_to_native(layout, layout.aperture.end + Vector2(2, 2))
				_check(not outside_right.inside and outside_right.pixel == Vector2i(319, 239), tag + " outside bottom-right clamps to (319,239)")
				var logical: Dictionary = Layout.window_to_logical(layout, layout.aperture.position + Vector2(0.25, 0.25) * layout.factor)
				_check(logical.inside and logical.pixel == Vector2i(layout.wing_width, 0), tag + " aperture origin is logical column wing_width")
