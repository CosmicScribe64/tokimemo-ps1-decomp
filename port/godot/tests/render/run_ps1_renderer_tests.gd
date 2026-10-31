extends SceneTree
## T-39302 (M2): the Ps1Renderer wrapper and the native OriginalGpu with synthetic primitives only.
## Every expected pixel is worked out by hand (hardware rules, psx-spx) and written next to the check;
## the full rule matrix is in tools/port/native_render/test_gpu.cpp. This runner needs the candidate
## library: tools/port/run_render_tests.sh mounts it at res://bin/ and runs both test sets in Docker.
## Exit 0 only when every check passes; exit 2 when the library is not loadable.

const Renderer = preload("res://presentation/ps1_renderer.gd")
const Memory = preload("res://core/game_memory.gd")

var failures: Array[String] = []
var checks: int = 0

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)

static func _xy(x: int, y: int) -> int:
	return (x & 0x7ff) | ((y & 0x7ff) << 16)

static func _rgb(r: int, g: int, b: int) -> int:
	return r | (g << 8) | (b << 16)

static func _c15(r: int, g: int, b: int) -> int:
	return r | (g << 5) | (b << 10)

static func _words(values: Array[int]) -> PackedByteArray:
	var bytes := PackedByteArray()
	bytes.resize(values.size() * 4)
	for i: int in values.size():
		bytes.encode_u32(i * 4, values[i] & 0xffffffff)
	return bytes

static func _pixel(renderer: Renderer, x: int, y: int) -> int:
	return renderer.store_image(Rect2i(x, y, 1, 1)).decode_u16(0)

func _put(renderer: Renderer, x: int, y: int, value: int) -> void:
	var one := PackedByteArray()
	one.resize(2)
	one.encode_u16(0, value)
	_check(renderer.load_image(Rect2i(x, y, 1, 1), one).is_empty(), "load_image of one pixel")

func _initialize() -> void:
	var renderer := Renderer.new()
	var error: String = renderer.open()
	if not error.is_empty():
		printerr("SKIP: ", error)
		quit(2)
		return
	_test_vram_roundtrip(renderer)
	_test_fill_rules(renderer)
	_test_edge_rules(renderer)
	_test_semi_transparency(renderer)
	_test_clut_lookup(renderer)
	_test_dithering(renderer)
	_test_ordering_table_and_memory(renderer)
	_test_display_image(renderer)
	_test_invalid_arguments(renderer)
	if failures.is_empty():
		print("PASS: ps1 renderer, %d checks" % checks)
		quit(0)
		return
	for failure: String in failures:
		printerr("FAIL: ", failure)
	quit(1)

func _test_vram_roundtrip(renderer: Renderer) -> void:
	renderer.reset()
	var image := PackedByteArray()
	image.resize(8)
	for i: int in 4:
		image.encode_u16(i * 2, 0x1000 + i)
	_check(renderer.load_image(Rect2i(1022, 5, 4, 1), image).is_empty(), "load_image accepts a wrapping rectangle")
	_check(_pixel(renderer, 1022, 5) == 0x1000 and _pixel(renderer, 1023, 5) == 0x1001, "load_image first pixels")
	_check(_pixel(renderer, 0, 5) == 0x1002 and _pixel(renderer, 1, 5) == 0x1003, "load_image wraps at x=1024")
	var back: PackedByteArray = renderer.store_image(Rect2i(1022, 5, 4, 1))
	_check(back == image, "store_image returns what load_image stored")
	_check(renderer.move_image(Vector2i(1022, 5), Vector2i(10, 20), Vector2i(4, 1)).is_empty(), "move_image")
	_check(_pixel(renderer, 12, 20) == 0x1002, "move_image copies VRAM to VRAM")
	var all: PackedByteArray = renderer.vram()
	_check(all.size() == Renderer.VRAM_BYTES and all.decode_u16((5 * 1024 + 1022) * 2) == 0x1000, "vram() is the whole 1 MiB")
	renderer.reset()
	_check(renderer.set_vram(all).is_empty() and _pixel(renderer, 1022, 5) == 0x1000, "set_vram restores an image")

func _test_fill_rules(renderer: Renderer) -> void:
	renderer.reset()
	# Fill: x rounds down to 16, width up to 16: x 0x15 -> 0x10, width 0x11 -> 0x20; colour (255,0,0) -> 5-bit red 31.
	renderer.fill_rect(Rect2i(0x15, 7, 0x11, 2), _rgb(255, 0, 0))
	_check(_pixel(renderer, 0x10, 7) == 31 and _pixel(renderer, 0x2f, 8) == 31, "fill covers the rounded rectangle")
	_check(_pixel(renderer, 0x0f, 7) == 0 and _pixel(renderer, 0x30, 7) == 0 and _pixel(renderer, 0x10, 9) == 0, "fill stops at the rounded edges")

func _test_edge_rules(renderer: Renderer) -> void:
	renderer.reset()
	# Right triangle (0,0),(4,0),(0,4): pixel (x,y) is covered iff x + y < 4 (hypotenuse excluded, top row and
	# left column included): 10 pixels.
	renderer.gp0_stream(_words([0x20000000 | _rgb(255, 0, 0), _xy(0, 0), _xy(4, 0), _xy(0, 4)]))
	var covered: int = 0
	for y: int in 6:
		for x: int in 6:
			var inside: bool = x + y < 4
			_check((_pixel(renderer, x, y) != 0) == inside, "triangle pixel (%d,%d)" % [x, y])
			covered += 1 if inside else 0
	_check(covered == 10, "triangle covers 10 pixels")
	# A TILE covers exactly w x h: 4 x 3 at (10,10) -> x 10..13, y 10..12.
	renderer.reset()
	renderer.gp0_stream(_words([0x60000000 | _rgb(255, 0, 0), _xy(10, 10), (3 << 16) | 4]))
	_check(_pixel(renderer, 10, 10) == 31 and _pixel(renderer, 13, 12) == 31, "tile corners are drawn")
	_check(_pixel(renderer, 14, 10) == 0 and _pixel(renderer, 10, 13) == 0, "tile right and bottom edges are not")
	_check(renderer.dirty_rect() == Rect2i(10, 10, 4, 3), "dirty rect is the tile")
	# A quad is two triangles with no gap and no overlap: additive 1 over black must stay 1 everywhere.
	renderer.reset()
	renderer.gp0_word(0xe1000000 | (1 << 5))
	renderer.gp0_stream(_words([0x2a000000 | _rgb(8, 8, 8), _xy(0, 0), _xy(8, 0), _xy(0, 8), _xy(8, 8)]))
	var seen: Dictionary = {}
	for y: int in 8:
		for x: int in 8:
			seen[_pixel(renderer, x, y)] = true
	_check(seen.size() == 1 and seen.has(_c15(1, 1, 1)), "quad diagonal is neither doubled nor missing")
	# Draw area clips (inclusive) and the offset shifts: area (2,2)-(5,5), offset (3,3), 4x4 tile at (0,0).
	renderer.reset()
	renderer.gp0_word(0xe3000000 | 2 | (2 << 10))
	renderer.gp0_word(0xe4000000 | 5 | (5 << 10))
	renderer.gp0_word(0xe5000000 | 3 | (3 << 11))
	renderer.gp0_stream(_words([0x60000000 | _rgb(255, 255, 255), _xy(0, 0), (4 << 16) | 4]))
	_check(renderer.dirty_rect() == Rect2i(3, 3, 3, 3), "tile clipped to the draw area at its offset position")
	# Sprite texture coordinates wrap at 256: u0 = 254, width 4 reads u = 254, 255, 0, 1 (15-bit page at x=512).
	renderer.reset()
	renderer.gp0_word(0xe1000000 | 8 | (2 << 7))
	for u: int in [254, 255, 0, 1]:
		_put(renderer, 512 + u, 0, _c15(u & 31, 1, 1))
	renderer.gp0_stream(_words([0x65000000, _xy(0, 0), 254, (1 << 16) | 4]))
	_check(_pixel(renderer, 0, 0) == _c15(30, 1, 1) and _pixel(renderer, 1, 0) == _c15(31, 1, 1), "sprite u before the wrap")
	_check(_pixel(renderer, 2, 0) == _c15(0, 1, 1) and _pixel(renderer, 3, 0) == _c15(1, 1, 1), "sprite u wraps from 255 to 0")

func _test_semi_transparency(renderer: Renderer) -> void:
	# Back (10,20,30), front 8-bit (64,32,16) -> 5-bit (8,4,2).
	var expected: Array[Vector3i] = [Vector3i(9, 12, 16), Vector3i(18, 24, 31), Vector3i(2, 16, 28), Vector3i(12, 21, 30)]
	for mode: int in 4:
		renderer.reset()
		_put(renderer, 0, 0, _c15(10, 20, 30))
		renderer.gp0_word(0xe1000000 | (mode << 5))
		renderer.gp0_stream(_words([0x62000000 | _rgb(64, 32, 16), _xy(0, 0), (1 << 16) | 1]))
		var e: Vector3i = expected[mode]
		_check(_pixel(renderer, 0, 0) == _c15(e.x, e.y, e.z), "semi-transparency mode %d" % mode)

func _test_clut_lookup(renderer: Renderer) -> void:
	renderer.reset()
	var clut_word: int = 0 | (480 << 6)
	_put(renderer, 1, 480, _c15(31, 0, 0))
	_put(renderer, 2, 480, _c15(0, 31, 0))
	_put(renderer, 3, 480, _c15(0, 0, 31) | 0x8000)
	_put(renderer, 512, 0, 0x3210)  # texels u=0..3 -> CLUT indices 0..3
	renderer.gp0_word(0xe1000000 | 8)  # page x=512, 4-bit
	renderer.gp0_stream(_words([0x65000000, _xy(20, 20), clut_word << 16, (1 << 16) | 4]))
	_check(_pixel(renderer, 20, 20) == 0, "CLUT index 0 (zero entry) is transparent")
	_check(_pixel(renderer, 21, 20) == _c15(31, 0, 0) and _pixel(renderer, 22, 20) == _c15(0, 31, 0), "4-bit CLUT lookup")
	_check(_pixel(renderer, 23, 20) == (_c15(0, 0, 31) | 0x8000), "texel bit 15 is stored")
	# Modulation: colour 0x40 halves a 31 texel: (31*64)>>4 = 124 -> 15.
	renderer.gp0_stream(_words([0x64000000 | _rgb(64, 64, 64), _xy(0, 4), clut_word << 16, (1 << 16) | 4]))
	_check(_pixel(renderer, 1, 4) == _c15(15, 0, 0), "modulated texel is scaled by colour / 128")

func _test_dithering(renderer: Renderer) -> void:
	# Gouraud 8x8 square, all vertices 6, dither on: 5-bit value = (6 + T[y&3][x&3]) >> 3, 1 where T >= 2.
	var table: Array[Array] = [[-4, 0, -3, 1], [2, -2, 3, -1], [-3, 1, -4, 0], [3, -1, 2, -2]]
	renderer.reset()
	renderer.gp0_word(0xe1000000 | 0x200)
	var c: int = _rgb(6, 6, 6)
	renderer.gp0_stream(_words([0x38000000 | c, _xy(0, 0), c, _xy(8, 0), c, _xy(0, 8), c, _xy(8, 8)]))
	var ones: int = 0
	for y: int in 8:
		for x: int in 8:
			var e: int = (6 + int(table[y & 3][x & 3])) >> 3
			_check(_pixel(renderer, x, y) == _c15(e, e, e), "dither pixel (%d,%d)" % [x, y])
			ones += e
	_check(ones == 16, "dither sets 16 of 64 pixels")
	# The same colour flat is not dithered: 6 >> 3 = 0, nothing visible.
	renderer.reset()
	renderer.gp0_word(0xe1000000 | 0x200)
	renderer.gp0_stream(_words([0x28000000 | c, _xy(0, 0), _xy(8, 0), _xy(0, 8), _xy(8, 8)]))
	_check(renderer.vram().count(0) == Renderer.VRAM_BYTES, "flat polygon is not dithered")

func _test_ordering_table_and_memory(renderer: Renderer) -> void:
	renderer.reset()
	var memory := Memory.new()
	# Node A @0x80001000 -> node B @0x80002000, which ends the list (next = 0xffffff); each is a 3-word TILE.
	memory.write_u32(0x80001000, 0x03000000 | 0x2000)
	memory.write_u32(0x80001004, 0x60000000 | _rgb(255, 0, 0))
	memory.write_u32(0x80001008, _xy(1, 1))
	memory.write_u32(0x8000100c, (2 << 16) | 2)
	memory.write_u32(0x80002000, 0x03000000 | 0xffffff)
	memory.write_u32(0x80002004, 0x60000000 | _rgb(0, 255, 0))
	memory.write_u32(0x80002008, _xy(10, 10))
	memory.write_u32(0x8000200c, (1 << 16) | 3)
	var before: String = memory.ram.hex_encode().sha256_text()
	var result: Dictionary = renderer.draw_ordering_table_from_memory(memory, 0x80001000)
	_check(str(result["error"]).is_empty() and int(result["nodes"]) == 2 and int(result["words"]) == 6, "walk visits both nodes")
	_check(bool(result["terminated"]) and not bool(result["node_limit"]), "walk ends at the end marker")
	_check(_pixel(renderer, 1, 1) == _c15(31, 0, 0) and _pixel(renderer, 2, 2) == _c15(31, 0, 0) and _pixel(renderer, 3, 1) == 0, "node A tile")
	_check(_pixel(renderer, 10, 10) == _c15(0, 31, 0) and _pixel(renderer, 12, 10) == _c15(0, 31, 0) and _pixel(renderer, 13, 10) == 0, "node B tile")
	_check(renderer.counters().get("TILE", 0) == 2, "counters list two TILE primitives")
	_check(memory.ram.hex_encode().sha256_text() == before and memory.error.is_empty(), "the renderer never writes the game memory")
	# A list that loops is cut at the repeated node and reported as an error.
	memory.write_u32(0x80003000, 0x00000000 | 0x3000)
	var loop: Dictionary = renderer.draw_ordering_table_from_memory(memory, 0x80003000, 10)
	_check(bool(loop["cycle"]) and int(loop["nodes"]) == 1 and not bool(loop["terminated"]), "a looping list stops at the repeated node")
	_check(not str(loop["error"]).is_empty() and renderer.last_error == str(loop["error"]), "a loop is an error")
	# A long chain stops at max_nodes.
	for i: int in 20:
		memory.write_u32(0x80004000 + i * 8, 0x01000000 | (0x4000 + (i + 1) * 8))
	var chain: Dictionary = renderer.draw_ordering_table_from_memory(memory, 0x80004000, 10)
	_check(bool(chain["node_limit"]) and int(chain["nodes"]) == 10 and not str(chain["error"]).is_empty(), "a long list stops at max_nodes")
	var words: Dictionary = renderer.draw_ordering_table_from_memory(memory, 0x80004000, 1000, 5)
	_check(bool(words["word_limit"]) and int(words["words"]) == 5, "a long list stops at max_words")
	_check(not str(renderer.draw_ordering_table_from_memory(null, 0x80004000)["error"]).is_empty(), "no memory is an error")

func _test_display_image(renderer: Renderer) -> void:
	renderer.reset()
	renderer.gp1(0x08000001)  # 320 wide, 240 high
	renderer.gp1(0x05000000 | (240 << 10))  # display start (0,240)
	_put(renderer, 0, 240, 0x7fff)
	_put(renderer, 1, 240, _c15(16, 8, 4))
	var image: Image = renderer.display_image()
	_check(image != null and image.get_size() == Vector2i(320, 240) and image.get_format() == Image.FORMAT_RGBA8, "display image is native 320x240 RGBA8")
	_check(image.get_pixel(0, 0).to_html(false) == "f8f8f8", "5-bit 31 becomes 248 with the c << 3 rule")
	_check(image.get_pixel(1, 0).to_html(false) == "804020", "5-bit (16,8,4) becomes (128,64,32)")
	var replicated: Image = renderer.display_image(true)
	_check(replicated.get_pixel(0, 0).to_html(false) == "ffffff" and replicated.get_pixel(1, 0).to_html(false) == "844221", "bit replication gives 255 and (132,66,33)")
	_check(renderer.display_info()["y"] == 240, "display start y")

func _test_invalid_arguments(renderer: Renderer) -> void:
	_check(not renderer.set_vram(PackedByteArray([1, 2, 3])).is_empty(), "set_vram refuses a wrong size")
	_check(not renderer.load_image(Rect2i(0, 0, 4, 4), PackedByteArray([1, 2])).is_empty(), "load_image refuses short data")
	_check(not renderer.load_image(Rect2i(0, 0, 0, 4), PackedByteArray()).is_empty(), "load_image refuses an empty rectangle")
	_check(not renderer.gp0_stream(PackedByteArray([1, 2, 3])).is_empty(), "a GP0 stream must be whole words")
	_check(renderer.store_image(Rect2i(0, 0, 2000, 4)).is_empty(), "store_image refuses a 2000-wide rectangle")
	_check(renderer.read_image(Rect2i(0, 0, 0, 0)) == null, "read_image refuses an empty rectangle")
	_check(not str(renderer.draw_ordering_table(PackedByteArray([0, 0, 0]), 0)["error"]).is_empty(), "draw_ordering_table refuses a non power-of-two RAM")
	_check(renderer.store_image(Rect2i(0, 0, 0, 4)).is_empty() and not renderer.last_error.is_empty(), "a failed store_image sets last_error")
	_check(renderer.store_image(Rect2i(0, 0, 1, 1)).size() == 2 and renderer.last_error.is_empty(), "a good call clears last_error")
	var unopened := Renderer.new()
	_check(not unopened.load_image(Rect2i(0, 0, 1, 1), PackedByteArray([0, 0])).is_empty() and unopened.vram().is_empty(), "an unopened renderer reports instead of crashing")
	_check(unopened.display_image() == null and unopened.counters().is_empty() and not unopened.last_error.is_empty(), "unopened read-out is empty")
	renderer.gp1(0x03000001)
	var dark: Image = renderer.display_image()
	_check(dark != null and dark.get_pixel(0, 0).to_html(false) == "000000", "a disabled display reads black")
	renderer.gp1(0x03000000)
