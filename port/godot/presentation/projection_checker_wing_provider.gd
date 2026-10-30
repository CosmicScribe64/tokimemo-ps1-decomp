extends RefCounted
## T-30001 WP6: Class A source wings for scenes whose checker comes from
## prologue_hud_source_provider.draw_checker (the first-arrival reload frames,
## mode 0x34), not from hud_compositor. The provider draws the same full
## checker board (hud_screen_frame_draw(0)) into a scratch 320x240 Image with
## the frame's own HUD projection, then repeats its 32-column period into the
## wings, so the wing pixel at native x < 0 or x >= 320 is the checker pixel
## at the same x modulo 32. Only frames whose source frame argument is 0 (the
## full board) are filled; any other frame is an error and the host falls
## back to black wings. Nothing here touches the native Image or game state.
##
## Context: {"projection": Dictionary} as passed to draw_checker.

var source: Object

func _init(checker_source: Object) -> void:
	source = checker_source

func fill_wings(target: Image, wing_width: int, context: Dictionary) -> String:
	if target == null or target.get_height() != 240 or target.get_width() != 320 + 2 * wing_width or wing_width < 0:
		return "target does not match the wing width"
	if wing_width == 0:
		return ""
	if source == null or not source.has_method("draw_checker") or not source.has_method("source_frame_argument"):
		return "checker source unavailable"
	var projection: Variant = context.get("projection")
	if not projection is Dictionary:
		return "projection missing"
	if int(source.source_frame_argument(projection)) != 0:
		return "only the full checker board (frame argument 0) continues into the wings"
	var board := Image.create(320, 240, false, target.get_format())
	board.fill(Color(0, 0, 0, 1))
	var failure: Variant = source.draw_checker(board, projection)
	if not failure is String or not failure.is_empty():
		return "checker draw failed: %s" % failure
	# 320 is a multiple of 32, so columns 288..319 are one whole tile period.
	var period := board.get_region(Rect2i(288, 0, 32, 240))
	for x: int in range(0, wing_width):
		# Left wing column x shows native x - wing_width; right wing column
		# wing_width + 320 + x shows native 320 + x.
		var left_native := x - wing_width
		target.blit_rect(period, Rect2i(posmod(left_native, 32), 0, 1, 240), Vector2i(x, 0))
		target.blit_rect(period, Rect2i(posmod(x, 32), 0, 1, 240), Vector2i(wing_width + 320 + x, 0))
	return ""
