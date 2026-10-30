extends SceneTree
## T-30001 WP2: SceneAperturePolicy classes and WideFrameComposer invariants.
## Optional real frames: set WIDE_FIXTURE_FRAMES to a directory of 320x240 PNGs
## (for example the opening-timing port captures, mounted read-only). When the
## variable is set, the directory must yield at least one frame.

const Policy = preload("res://presentation/scene_aperture_policy.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const WIDTHS := [320, 322, 384, 426, 428, 500, 560]

class CentreWriter extends RefCounted:
	## Hostile provider: paints every column, including the native ones.
	func fill_wings(target: Image, _wing: int, _context: Dictionary) -> String:
		target.fill(Color(1, 0, 1, 1))
		return ""

class FailingProvider extends RefCounted:
	func fill_wings(_target: Image, _wing: int, _context: Dictionary) -> String:
		return "no source state"

class CountingProvider extends RefCounted:
	var calls := 0
	func fill_wings(_target: Image, _wing: int, _context: Dictionary) -> String:
		calls += 1
		return ""

var _checks := 0
var _failures: Array[String] = []
var fixture_frames := 0

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

static func _hash(image: Image) -> String:
	var hashing := HashingContext.new()
	hashing.start(HashingContext.HASH_SHA256)
	hashing.update(var_to_bytes([image.get_width(), image.get_height(), image.get_format()]))
	hashing.update(image.get_data())
	return hashing.finish().hex_encode()

static func _random_image(seed_value: int, format: int) -> Image:
	var rng := RandomNumberGenerator.new()
	rng.seed = seed_value
	var channels := 4 if format == Image.FORMAT_RGBA8 else 3
	var data := PackedByteArray()
	data.resize(320 * 240 * channels)
	for index: int in data.size():
		data[index] = rng.randi() & 255
	return Image.create_from_data(320, 240, false, format, data)

func _initialize() -> void:
	_check_policy()
	_check_negatives()
	for seed_value: int in [1, 2, 3, 0x7fff]:
		for format: int in [Image.FORMAT_RGBA8, Image.FORMAT_RGB8]:
			_check_frame(_random_image(seed_value, format), "random %d/%d" % [seed_value, format])
	_check_fixture_frames()
	if _failures.is_empty():
		print("PASS: wide frame composer (%d checks, %d fixture frames)" % [_checks, fixture_frames])
		quit(0)
	else:
		for failure: String in _failures:
			printerr("FAIL: ", failure)
		quit(1)

func _check_frame(native: Image, tag: String) -> void:
	var composer := Composer.new()
	var before := _hash(native)
	for width: int in WIDTHS:
		var composed := composer.compose(native, width)
		_check(composer.error.is_empty() and composed != null, "%s Wl=%d composes" % [tag, width])
		if composed == null:
			continue
		_check(composed.get_size() == Vector2i(width, 240) and composed.get_format() == native.get_format(), "%s Wl=%d size/format" % [tag, width])
		_check(_hash(Composer.centre_crop(composed)) == before, "%s Wl=%d centre crop is byte-identical" % [tag, width])
		if width == 320:
			_check(_hash(composed) == before and composed != native, "%s Wl=320 is a byte-identical copy" % tag)
		else:
			var wing := (width - 320) >> 1
			var wings_black := true
			var black := Color(0, 0, 0, 1)
			for y: int in range(0, 240, 3):
				for x: int in range(wing):
					if composed.get_pixel(x, y) != black or composed.get_pixel(width - 1 - x, y) != black:
						wings_black = false
			_check(wings_black, "%s Wl=%d wings are opaque black by default" % [tag, width])
		var hostile := composer.compose(native, width, CentreWriter.new())
		_check(hostile != null and _hash(Composer.centre_crop(hostile)) == before, "%s Wl=%d provider cannot reach native columns" % [tag, width])
	_check(_hash(native) == before, "%s input Image unchanged by compose" % tag)

func _check_fixture_frames() -> void:
	var root := OS.get_environment("WIDE_FIXTURE_FRAMES")
	if root.is_empty():
		print("NOTE: WIDE_FIXTURE_FRAMES unset; real-frame pass skipped")
		return
	var dir := DirAccess.open(root)
	_check(dir != null, "fixture frame directory opens: " + root)
	if dir == null:
		return
	var names := dir.get_files()
	names.sort()
	for name: String in names:
		if not name.ends_with(".png"):
			continue
		var image := Image.load_from_file(root.path_join(name))
		if image == null or image.get_size() != Vector2i(320, 240):
			continue
		fixture_frames += 1
		_check_frame(image, "fixture " + name)
	_check(fixture_frames > 0, "fixture directory yields 320x240 frames")

func _check_policy() -> void:
	for mode: int in [0x34, 0x40, 0x42]:
		_check(Policy.classify(mode) == Policy.CLASS_A, "mode 0x%02x is Class A" % mode)
		_check(Policy.classify("0x%02x" % mode) == Policy.CLASS_A, "mode string 0x%02x is Class A" % mode)
	for mode: int in [0x01, 0x02, 0x22]:
		_check(Policy.classify(mode) == Policy.CLASS_B, "mode 0x%02x is Class B" % mode)
	for tag: String in ["VN", "BUSTUP", "bustup"]:
		_check(Policy.classify(tag) == Policy.CLASS_B, tag + " is Class B")
	_check(Policy.classify({"mode": 0x40, "tag": "BUSTUP"}) == Policy.CLASS_B, "BUSTUP tag wins over a Class A mode")
	_check(Policy.classify({"mode": 0x42}) == Policy.CLASS_A, "dictionary mode is classified")
	for scene: Variant in [0x10, 0x16, 0x43, 0x5D, 0x00, -1, 0x340, "TITLE", "TACO", "RPG_BAT", "", "0x", "0xZZ", null, 3.5, [], {}]:
		_check(Policy.classify(scene) == Policy.CLASS_C, "%s falls back to Class C" % [scene])
	var source := {"display": {"wing_fill": "source"}}
	_check(Policy.wing_source(0x40, source) == Policy.WING_CHECKER, "Class A + source uses the checker")
	_check(Policy.wing_source(0x40, {}) == Policy.WING_BLACK, "Class A defaults to black")
	_check(Policy.wing_source(0x40, {"display": {"wing_fill": "blur"}}) == Policy.WING_BLACK, "unknown wing fill is black")
	_check(Policy.wing_source(0x01, source) == Policy.WING_BLACK, "Class B never uses the checker")
	_check(Policy.wing_source(0x10, source) == Policy.WING_BLACK, "Class C never uses the checker")
	_check(Policy.wing_source(0x99, source) == Policy.WING_BLACK, "unknown mode never uses the checker")

func _check_negatives() -> void:
	var composer := Composer.new()
	var native := _random_image(9, Image.FORMAT_RGBA8)
	_check(composer.compose(null, 426) == null and not composer.error.is_empty(), "null native rejected")
	_check(composer.compose(Image.create(321, 240, false, Image.FORMAT_RGBA8), 426) == null, "wrong native size rejected")
	_check(composer.compose(Image.create(320, 239, false, Image.FORMAT_RGBA8), 426) == null, "wrong native height rejected")
	for width: int in [0, 318, 427, 562, 1000, -426]:
		_check(composer.compose(native, width) == null and not composer.error.is_empty(), "logical width %d rejected" % width)
	_check(composer.compose(native, 426, RefCounted.new()) == null, "provider without fill_wings rejected")
	_check(composer.compose(native, 426, FailingProvider.new()) == null and composer.error.contains("no source state"), "provider failure surfaces")
	var counter := CountingProvider.new()
	composer.compose(native, 320, counter)
	_check(counter.calls == 0, "Wl=320 never calls the provider")
	composer.compose(native, 426, counter)
	_check(counter.calls == 1 and composer.error.is_empty(), "wide compose calls the provider once and clears error")
	_check(Composer.centre_crop(Image.create(300, 240, false, Image.FORMAT_RGBA8)) == null, "centre crop rejects narrow images")
