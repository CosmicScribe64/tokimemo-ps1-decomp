extends RefCounted
## T-30001 WP3: source-derived Class A wings. Continues hud_compositor's
## seasonal checker tile into native x < 0 and x >= 320 at the same uv, season
## and fade, so the wing pixel at logical column c is the checker pixel at
## native x = c - wing_width. Only the wings are written; the room strip,
## actors and HUD stay inside the native 320 columns.
##
## Context keys mirror hud_compositor.render's state: checker_uv (Vector2i),
## checker_season (0..3) or month (1..12), checker_fade (239..255). The caller
## passes the same values it rendered the native frame with. Missing uv or fade
## is an error rather than a guess, because a wrong value would show a seam.

const HudCompositor = preload("res://presentation/hud_compositor.gd")

var compositor: RefCounted
## Count of fills that failed because the checker tile was unavailable.
var fallbacks := 0

func _init(hud_compositor: RefCounted = null) -> void:
	compositor = hud_compositor if hud_compositor != null else HudCompositor.new()

static func season_from(context: Dictionary) -> int:
	if context.has("checker_season"):
		return int(context.checker_season) & 3
	if context.has("month"):
		return HudCompositor.hud_season_index(int(context.month))
	return -1

func fill_wings(target: Image, wing_width: int, context: Dictionary) -> String:
	if target == null or target.get_height() != 240 or target.get_width() != 320 + 2 * wing_width or wing_width < 0:
		return "target does not match the wing width"
	if wing_width == 0:
		return ""
	if not context.get("checker_uv") is Vector2i:
		return "checker_uv missing"
	if not (context.get("checker_fade") is int and int(context.checker_fade) >= 239 and int(context.checker_fade) <= 255):
		return "checker_fade missing or outside 239..255"
	var season := season_from(context)
	if season < 0:
		return "checker season missing"
	var tile: Image = compositor.checker_tile(context.checker_uv, season, int(context.checker_fade))
	if tile == null:
		# An error, not black wings reported as checker: the native frame then
		# shows the compositor's fallback fill and the host records the failure
		# and reports "black".
		fallbacks += 1
		return "checker tile unavailable"
	if tile.get_format() != target.get_format():
		tile.convert(target.get_format())
	# The strip is whole tiles wide (a multiple of 32), so strip column s shows
	# tile column (s & 31) and matches every native x with x & 31 == s & 31.
	var strip_width := ((wing_width + 31) >> 5) << 5
	var strip := Image.create(strip_width, 240, false, target.get_format())
	for ty: int in range(0, 240, 32):
		for tx: int in range(0, strip_width, 32):
			strip.blit_rect(tile, Rect2i(0, 0, 32, 32), Vector2i(tx, ty))
	# Left wing covers native x = -wing_width .. -1, i.e. strip columns
	# strip_width - wing_width .. strip_width - 1.
	target.blit_rect(strip, Rect2i(strip_width - wing_width, 0, wing_width, 240), Vector2i.ZERO)
	# Right wing covers native x = 320 .. 319 + wing_width; 320 is a tile boundary.
	target.blit_rect(strip, Rect2i(0, 0, wing_width, 240), Vector2i(wing_width + 320, 0))
	return ""
