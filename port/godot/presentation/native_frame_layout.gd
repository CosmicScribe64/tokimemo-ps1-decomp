extends RefCounted
## T-30001 WP1: pure window geometry for presenting the native 320x240 Image.
## Nothing here reads or writes game state; it maps a window size and the
## display preferences to window-space rects and point transforms.
##
## Modes. aspect=original fits the 320x240 picture as a 4:3 box with black
## bars (the same rect first_arrival_boot._layout_display computed). aspect=wide
## presents a Wl x 240 frame whose centre 320 columns are the native picture
## and whose side columns are wings. aspect=auto is wide when W/H >= 1.5.
##
## Logical width. Wl = 2 * floor(120 * W / H), clamped to 320..560. Rounding
## down (rather than to nearest) keeps Wl <= 240 * W / H, so the fit is always
## limited by height and an integer factor is >= 1 whenever H >= 240.
##
## Pixel grid. The frame origin is rounded to whole window pixels, so the bars
## differ by at most 1px. With filter=integer and a factor >= 1 every rect edge
## is a whole pixel. Fractional fits keep the exact scale, so interior edges
## (aperture inside a wide frame, frame end) may fall between pixels.

const NATIVE_SIZE := Vector2i(320, 240)
const MIN_LOGICAL_WIDTH := 320
const MAX_LOGICAL_WIDTH := 560
const AUTO_WIDE_RATIO := 1.5
## Native rows of the message window region (y = 160..239).
const TEXT_BAND_TOP := 160
const TEXT_BAND_HEIGHT := 80
const ASPECTS := ["original", "wide", "auto"]
const FILTERS := ["integer", "nearest", "linear"]

## Reads display.aspect / display.filter from either the effective settings
## dictionary ({"display": {...}}) or a flat {"aspect", "filter"} dictionary.
## Unknown or missing values fall back to the current defaults.
static func preference(prefs: Dictionary, key: String, fallback: String, allowed: Array) -> String:
	var value: Variant = prefs.get(key)
	var display: Variant = prefs.get("display")
	if display is Dictionary and display.has(key):
		value = display[key]
	return value if value is String and value in allowed else fallback

static func resolved_mode(window_size: Vector2, aspect: String) -> String:
	if aspect == "wide":
		return "wide"
	if aspect == "auto" and window_size.y > 0.0 and window_size.x / window_size.y >= AUTO_WIDE_RATIO:
		return "wide"
	return "original"

static func logical_width_for(window_size: Vector2, mode: String) -> int:
	if mode != "wide" or window_size.x <= 0.0 or window_size.y <= 0.0:
		return MIN_LOGICAL_WIDTH
	var even := 2 * floori(120.0 * window_size.x / window_size.y)
	return clampi(even, MIN_LOGICAL_WIDTH, MAX_LOGICAL_WIDTH)

## Returns the layout dictionary:
##   valid, aspect (requested), mode (original|wide), filter, window_size,
##   logical_width, wing_width (logical columns per side), factor, integer,
##   frame (Rect2: the Wl x 240 presentation), aperture (Rect2: native 320x240),
##   wings ([left, right] Rect2, zero width in original mode),
##   bars ([left, right, top, bottom] Rect2 outside the frame),
##   text_band (Rect2 over native rows 160..239 inside the aperture),
##   text_band_wide (the same rows across the whole frame, wings included).
static func compute(window_size: Vector2, prefs: Dictionary = {}) -> Dictionary:
	var aspect := preference(prefs, "aspect", "original", ASPECTS)
	var filter := preference(prefs, "filter", "integer", FILTERS)
	var result := {"valid": false, "aspect": aspect, "filter": filter, "window_size": window_size,
		"mode": "original", "logical_width": MIN_LOGICAL_WIDTH, "wing_width": 0, "factor": 1.0,
		"integer": false, "frame": Rect2(), "aperture": Rect2(), "wings": [Rect2(), Rect2()],
		"bars": [Rect2(), Rect2(), Rect2(), Rect2()], "text_band": Rect2(), "text_band_wide": Rect2()}
	if not (window_size.x > 0.0 and window_size.y > 0.0 and is_finite(window_size.x) and is_finite(window_size.y)):
		return result
	var mode := resolved_mode(window_size, aspect)
	var wl := logical_width_for(window_size, mode)
	var wing := (wl - NATIVE_SIZE.x) >> 1
	var factor := minf(window_size.x / wl, window_size.y / NATIVE_SIZE.y)
	if filter == "integer" and factor >= 1.0:
		factor = floorf(factor)
	var frame_size := Vector2(wl, NATIVE_SIZE.y) * factor
	var frame := Rect2(((window_size - frame_size) * 0.5).round(), frame_size)
	var aperture := Rect2(frame.position + Vector2(wing * factor, 0.0), Vector2(NATIVE_SIZE) * factor)
	var band_top := aperture.position.y + TEXT_BAND_TOP * factor
	var band_height := TEXT_BAND_HEIGHT * factor
	result.merge({"valid": true, "mode": mode, "logical_width": wl, "wing_width": wing,
		"factor": factor, "integer": factor == floorf(factor), "frame": frame, "aperture": aperture,
		"wings": [Rect2(frame.position, Vector2(wing * factor, frame.size.y)),
			Rect2(Vector2(aperture.end.x, frame.position.y), Vector2(wing * factor, frame.size.y))],
		"bars": [Rect2(0.0, 0.0, frame.position.x, window_size.y),
			Rect2(frame.end.x, 0.0, maxf(window_size.x - frame.end.x, 0.0), window_size.y),
			Rect2(frame.position.x, 0.0, frame.size.x, frame.position.y),
			Rect2(frame.position.x, frame.end.y, frame.size.x, maxf(window_size.y - frame.end.y, 0.0))],
		"text_band": Rect2(aperture.position.x, band_top, aperture.size.x, band_height),
		"text_band_wide": Rect2(frame.position.x, band_top, frame.size.x, band_height)}, true)
	return result

## Window point -> native pixel. "pixel" is clamped to 0..319 x 0..239;
## "inside" says whether the point lay within the aperture before clamping.
static func window_to_native(layout: Dictionary, point: Vector2) -> Dictionary:
	if not layout.get("valid", false):
		return {"pixel": Vector2i.ZERO, "inside": false}
	var aperture: Rect2 = layout.aperture
	var relative := (point - aperture.position) / float(layout.factor)
	var pixel := Vector2i(floori(relative.x), floori(relative.y))
	var inside := pixel.x >= 0 and pixel.y >= 0 and pixel.x < NATIVE_SIZE.x and pixel.y < NATIVE_SIZE.y
	return {"pixel": pixel.clamp(Vector2i.ZERO, NATIVE_SIZE - Vector2i.ONE), "inside": inside}

## Native pixel -> window point at that pixel's centre. Out-of-range pixels
## are clamped first.
static func native_to_window(layout: Dictionary, pixel: Vector2i) -> Vector2:
	if not layout.get("valid", false):
		return Vector2.ZERO
	var clamped := pixel.clamp(Vector2i.ZERO, NATIVE_SIZE - Vector2i.ONE)
	var aperture: Rect2 = layout.aperture
	return aperture.position + (Vector2(clamped) + Vector2(0.5, 0.5)) * float(layout.factor)

## Window point -> column/row of the Wl x 240 presentation frame (clamped);
## native x = logical x - wing_width.
static func window_to_logical(layout: Dictionary, point: Vector2) -> Dictionary:
	if not layout.get("valid", false):
		return {"pixel": Vector2i.ZERO, "inside": false}
	var frame: Rect2 = layout.frame
	var relative := (point - frame.position) / float(layout.factor)
	var pixel := Vector2i(floori(relative.x), floori(relative.y))
	var limit := Vector2i(int(layout.logical_width), NATIVE_SIZE.y)
	var inside := pixel.x >= 0 and pixel.y >= 0 and pixel.x < limit.x and pixel.y < limit.y
	return {"pixel": pixel.clamp(Vector2i.ZERO, limit - Vector2i.ONE), "inside": inside}
