extends RefCounted
## T-39060: presentation preferences and window captures shared by the QA
## observers (run_modern_command_route.gd, run_english_playthrough.gd,
## run_visual_qa_screens.gd). Presentation only.
##
## ROUTE_PREFS: JSON object of "section.key" -> value written through
##   ModernSettings.preferences.set_values before the first scene; the written
##   preferences are echoed to /out/route-prefs.json.
## ROUTE_WINDOW: "<W>x<H>" window size the observer keeps on published frames.

## Applies ROUTE_PREFS to the root's ModernSettings. Returns "" when nothing
## was requested or the preferences were written, else the reason.
static func apply(root: Window) -> String:
	var raw := OS.get_environment("ROUTE_PREFS")
	if raw.is_empty():
		return ""
	var prefs: Variant = JSON.parse_string(raw)
	var settings: Node = root.get_node_or_null("ModernSettings")
	if not prefs is Dictionary or settings == null:
		return "ROUTE_PREFS must be a JSON object and ModernSettings must exist"
	if not settings.preferences.set_values(prefs):
		return "ROUTE_PREFS rejected: " + str(settings.preferences.error)
	var file := FileAccess.open("/out/route-prefs.json", FileAccess.WRITE)
	file.store_string(JSON.stringify({"requested": prefs, "effective": settings.preferences.effective_settings()}, "\t"))
	file.close()
	return ""

## ROUTE_WINDOW as a size, or Vector2i.ZERO when unset or malformed.
static func window_size() -> Vector2i:
	var spec := OS.get_environment("ROUTE_WINDOW").split("x")
	if spec.size() == 2 and spec[0].is_valid_int() and spec[1].is_valid_int():
		return Vector2i(int(spec[0]), int(spec[1]))
	return Vector2i.ZERO

## The root viewport texture holds only the stretched content area; the window
## shows it centred on the black clear colour, so pad it to the window size.
static func window_image(image: Image, window_size: Vector2i) -> Image:
	if image == null or image.get_size() == window_size:
		return image
	var framed := Image.create(window_size.x, window_size.y, false, image.get_format())
	framed.fill(Color.BLACK)
	framed.blit_rect(image, Rect2i(Vector2i.ZERO, image.get_size()), (window_size - image.get_size()) / 2)
	return framed
