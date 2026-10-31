extends SceneTree
## T-39220 visual QA: renders wing art previews around real 320x240 captures
## exactly as NativeDisplayHost frames them (integer scale, wide layout): the
## backdrop behind, then the composed Wl x 240 frame. Every window image is
## checked for an exact centre. Inputs (environment):
##   WING_ART_PACK      wing_art pack directory (from wing_art_content.py)
##   WING_ART_SCENES    "name=path;..." 320x240 (or exact 3x 960x720) captures
##   WING_ART_OUT       output directory (must exist)
##   WING_ART_ANIM      "1" to also write per-season animation frames (30 Hz,
##                      one PNG per tick, lossless, centre checked per frame)
##   WING_ART_FRAMES    directory for animation frames (default: WING_ART_OUT)
## Writes <out>/<w>x<h>/<mode>/<scene>.png, <out>/anim-<season>/NNN.png and
## <out>/report.json. Original-derived output: keep it under .asset-cache.

const WingArt = preload("res://presentation/wing_art_provider.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const Layout = preload("res://presentation/native_frame_layout.gd")

## Scene facts used for "scene" mode (what a boot would pass).
const SCENE_FACTS := {
	"opening-dialogue": {"scene": 0x01, "stage": "opening"},
	"command-select": {"scene": 0x34},
	"flash-cg": {"scene": "CG"},
	"name-entry": {"stage": "name_entry"},
	"classroom": {"location": "school"},
	"prologue-board": {"stage": "prologue"},
}
const MONTHS := {"spring": 4, "summer": 7, "autumn": 10, "winter": 1}

var report := {"images": [], "failures": []}

func _load_scene(path: String) -> Image:
	var image := Image.load_from_file(path)
	if image == null or image.is_empty():
		return null
	image.convert(Image.FORMAT_RGBA8)
	if image.get_size() == Vector2i(960, 720):
		var small := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		for y: int in 240:
			for x: int in 320:
				small.set_pixel(x, y, image.get_pixel(x * 3, y * 3))
		image = small
	return image if image.get_size() == Vector2i(320, 240) else null

static func _scaled(image: Image, factor: int) -> Image:
	var out := image.duplicate()
	out.resize(image.get_width() * factor, image.get_height() * factor, Image.INTERPOLATE_NEAREST)
	return out

## One window image the way NativeDisplayHost draws it.
func _window(art: RefCounted, mode: String, native: Image, window: Vector2, context: Dictionary) -> Image:
	var layout := Layout.compute(window, {"display": {"aspect": "wide", "filter": "integer"}})
	var factor := int(layout.factor)
	var out := Image.create(int(window.x), int(window.y), false, Image.FORMAT_RGBA8)
	out.fill(Color(0, 0, 0, 1))
	var ctx := context.duplicate()
	ctx["wing_art_mode"] = mode
	ctx["visible_side"] = floori(layout.aperture.position.x / factor)
	var backdrop: Image = art.render(mode, ctx)
	if backdrop != null:
		var big := _scaled(backdrop, factor)
		var pos: Vector2 = layout.aperture.position - Vector2(art.geometry.aperture_x, art.geometry.aperture_y) * factor
		out.blit_rect(big, Rect2i(Vector2i.ZERO, big.get_size()), Vector2i(pos))
	var composer := Composer.new()
	var frame := composer.compose(native, layout.logical_width, art if backdrop != null else null, ctx)
	if frame == null:
		report.failures.append("compose failed: " + composer.error)
		return out
	var big_frame := _scaled(frame, factor)
	out.blit_rect(big_frame, Rect2i(Vector2i.ZERO, big_frame.get_size()), Vector2i(layout.frame.position))
	var centre := out.get_region(Rect2i(Vector2i(layout.aperture.position), Vector2i(320, 240) * factor))
	if centre.get_data() != _scaled(native, factor).get_data():
		report.failures.append("centre not exact: %s %s" % [mode, window])
	return out

func _save(image: Image, path: String) -> void:
	DirAccess.make_dir_recursive_absolute(path.get_base_dir())
	image.save_png(path)
	report.images.append(path)

func _initialize() -> void:
	var pack := OS.get_environment("WING_ART_PACK")
	var out := OS.get_environment("WING_ART_OUT")
	var art: RefCounted = WingArt.new()
	if not art.open_pack(pack) or out.is_empty():
		print("FAIL: cannot open pack or no output: " + art.error)
		quit(1); return
	var scenes := {}
	for pair: String in OS.get_environment("WING_ART_SCENES").split(";", false):
		var parts := pair.split("=")
		var image := _load_scene(parts[1])
		if image == null:
			report.failures.append("scene did not load: " + pair)
			continue
		scenes[parts[0]] = image
	var tick := 22
	for window: Vector2 in [Vector2(1920, 1080), Vector2(2560, 1080)]:
		var tag := "%dx%d" % [int(window.x), int(window.y)]
		var names: Array = scenes.keys() if window.x == 1920 else ["opening-dialogue", "command-select", "prologue-board"]
		for season: String in MONTHS:
			for name: String in names:
				if not scenes.has(name): continue
				var fresh: RefCounted = art.spawn()
				_save(_window(fresh, "seasonal", scenes[name], window, {"month": MONTHS[season], "tick": tick}), "%s/%s/seasonal-%s/%s.png" % [out, tag, season, name])
		for name: String in names:
			if not scenes.has(name): continue
			var ctx: Dictionary = SCENE_FACTS.get(name, {}).duplicate()
			ctx["month"] = 4; ctx["tick"] = tick
			_save(_window(art.spawn(), "scene", scenes[name], window, ctx), "%s/%s/scene/%s.png" % [out, tag, name])
		for style: String in ["spring_day"]:
			for name: String in ["opening-dialogue", "command-select"]:
				if not scenes.has(name): continue
				_save(_window(art.spawn(), "fixed:" + style, scenes[name], window, {"tick": tick}), "%s/%s/fixed-%s/%s.png" % [out, tag, style, name])
		# Location styles (fixed:<style> options), around the opening dialogue
		# and the command screen.
		for style: String in ["school", "park", "town", "seaside", "legend_tree"]:
			for name: String in ["opening-dialogue", "command-select"]:
				if not scenes.has(name): continue
				_save(_window(art.spawn(), "fixed:" + style, scenes[name], window, {"month": 4, "tick": tick}), "%s/%s/location-%s/%s.png" % [out, tag, style, name])
	var frames_dir := OS.get_environment("WING_ART_FRAMES") if not OS.get_environment("WING_ART_FRAMES").is_empty() else out
	if OS.get_environment("WING_ART_ANIM") == "1" and scenes.has("opening-dialogue"):
		for season: String in MONTHS:
			var anim: RefCounted = art.spawn()
			# 30 Hz frames (one per presentation tick): 4 s per season, and
			# summer's full 8 s fireworks loop (240 ticks).
			var count := 240 if season == "summer" else 120
			for i: int in count:
				var frame := _window(anim, "seasonal", scenes["opening-dialogue"], Vector2(960, 540), {"month": MONTHS[season], "tick": i})
				_save(frame, "%s/anim-%s/%03d.png" % [frames_dir, season, i])
		# Season change: spring -> summer dissolve on one provider.
		var change: RefCounted = art.spawn()
		for i: int in 48:
			var month := 4 if i < 8 else 7
			_save(_window(change, "seasonal", scenes["opening-dialogue"], Vector2(960, 540), {"month": month, "tick": i}), "%s/anim-season-change/%03d.png" % [frames_dir, i])
	var file := FileAccess.open(out.path_join("report.json"), FileAccess.WRITE)
	file.store_string(JSON.stringify({"images": report.images.size(), "failures": report.failures}, "\t") + "\n")
	file.close()
	print("%s: wing art previews (%d images, %d failures)" % ["PASS" if report.failures.is_empty() else "FAIL", report.images.size(), report.failures.size()])
	for failure: String in report.failures.slice(0, 20):
		print("FAIL: " + failure)
	quit(0 if report.failures.is_empty() else 1)
