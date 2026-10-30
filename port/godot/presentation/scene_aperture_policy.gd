extends RefCounted
## T-30001 WP2: which wing fill a scene may use in a wide frame.
##
## Class A: the weekday activity strip modes (command screen 0x34, school day
##   0x40, club practice 0x42). hud_compositor.render fills the whole frame
##   with the seasonal checker tile before it blits the 320x88 room at (0,72)
##   (hud_compositor.gd:199 and 202-205), so the checker may continue into the
##   wings. The room strip, actors and HUD stay inside the native 320 columns.
## Class B: VN backdrop + portrait stage (date 0x01, walk home 0x02,
##   telephone 0x22, and scenes tagged VN or BUSTUP). Always pillarboxed.
## Class C: everything else, including title, calendar, CGs, ending, movies
##   and subgames. Unknown or malformed ids also land here (pillarbox).

const CLASS_A := "A"
const CLASS_B := "B"
const CLASS_C := "C"
const CLASS_A_MODES := [0x34, 0x40, 0x42]
const CLASS_B_MODES := [0x01, 0x02, 0x22]
const CLASS_B_TAGS := ["VN", "BUSTUP"]
const WING_BLACK := "black"
const WING_CHECKER := "checker"
const WING_FILLS := ["black", "source"]

## scene: an int mode id, a hex/decimal mode string ("0x34", "52"), a scene
## tag ("VN", "BUSTUP"), or a Dictionary with "mode" and/or "tag".
static func classify(scene: Variant) -> String:
	if scene is Dictionary:
		var tag: Variant = scene.get("tag")
		if tag is String and tag.to_upper() in CLASS_B_TAGS:
			return CLASS_B
		return classify(scene.get("mode")) if scene.has("mode") else CLASS_C
	if scene is String:
		if scene.to_upper() in CLASS_B_TAGS:
			return CLASS_B
		var text: String = scene.strip_edges().to_lower()
		if text.begins_with("0x") and text.length() > 2 and text.substr(2).is_valid_hex_number():
			return _classify_mode(text.substr(2).hex_to_int())
		if text.is_valid_int():
			return _classify_mode(text.to_int())
		return CLASS_C
	if scene is int:
		return _classify_mode(scene)
	return CLASS_C

static func _classify_mode(mode: int) -> String:
	if mode in CLASS_A_MODES:
		return CLASS_A
	if mode in CLASS_B_MODES:
		return CLASS_B
	return CLASS_C

## display.wing_fill from effective settings or a flat dictionary; anything
## unknown is "black".
static func wing_fill_preference(prefs: Dictionary) -> String:
	var value: Variant = prefs.get("wing_fill")
	var display: Variant = prefs.get("display")
	if display is Dictionary and display.has("wing_fill"):
		value = display.wing_fill
	return value if value is String and value in WING_FILLS else "black"

## The wing provider kind for a scene: checker only for Class A when the user
## opted into source wings; black otherwise.
static func wing_source(scene: Variant, prefs: Dictionary) -> String:
	if classify(scene) == CLASS_A and wing_fill_preference(prefs) == "source":
		return WING_CHECKER
	return WING_BLACK
