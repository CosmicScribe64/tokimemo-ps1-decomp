class_name Ps1Renderer
extends RefCounted
## T-39302 (M2): the generic PS1 renderer. A 1024x512 16-bit VRAM and a GPU primitive rasterizer
## (native OriginalGpu, tools/port/native_render) that draws any scene from the game's own drawing
## commands: an ordering table held in the port's game memory, or a raw GP0 word stream.
##
## Presentation only. The game memory is read, never written; the renderer owns its VRAM and
## drawing environment and holds no game state. Output is an RGBA8 Image of the display area
## (native 320x240 by default) for NativeDisplayHost, wings and Parity mode.
##
## Colour convention: a 5-bit channel c becomes c << 3 (0..248), the rule of the emulator
## screenshots and of the port's presenters (calendar, dialogue, encounters). Pass
## replicate_bits = true for (c << 3) | (c >> 2) (0..255), the rule of the MDEC exporter.
## Hardware rules and the sampled-frame exactness record: wiki/ps1-renderer.md.

const EXTENSION_PATH: String = "res://original_render.gdextension"
const NATIVE_CLASS: StringName = &"OriginalGpu"
const VRAM_WIDTH: int = 1024
const VRAM_HEIGHT: int = 512
const VRAM_BYTES: int = VRAM_WIDTH * VRAM_HEIGHT * 2
const DEFAULT_MAX_NODES: int = 1048576
## Words one walk may send to the GPU (a real frame is a few thousand; see gpu.hpp DEFAULT_MAX_WORDS).
const DEFAULT_MAX_WORDS: int = 131072

var _gpu: Object = null
## Why the last call that returned null, an empty array or an error string failed ("" after a success).
var last_error: String = ""

## True when the native class is registered (loading the extension on first use).
static func is_available() -> bool:
	if ClassDB.class_exists(NATIVE_CLASS):
		return true
	if not FileAccess.file_exists(EXTENSION_PATH) and not ResourceLoader.exists(EXTENSION_PATH):
		return false
	if not GDExtensionManager.is_extension_loaded(EXTENSION_PATH):
		var status: GDExtensionManager.LoadStatus = GDExtensionManager.load_extension(EXTENSION_PATH)
		if status != GDExtensionManager.LOAD_STATUS_OK and status != GDExtensionManager.LOAD_STATUS_ALREADY_LOADED:
			return false
	return ClassDB.class_exists(NATIVE_CLASS)

## Creates the native GPU. Returns "" or the reason it is unavailable.
func open() -> String:
	if _gpu != null:
		return ""
	if not is_available():
		return _fail("native PS1 renderer library is not loaded (%s)" % EXTENSION_PATH)
	_gpu = ClassDB.instantiate(NATIVE_CLASS)
	if _gpu == null:
		return _fail("could not instantiate %s" % NATIVE_CLASS)
	last_error = ""
	return ""

func is_open() -> bool:
	return _gpu != null

func _fail(message: String) -> String:
	last_error = message
	return message

## False (and last_error set) before open().
func _ready_to_use() -> bool:
	if _gpu == null:
		last_error = "Ps1Renderer.open() has not succeeded"
		return false
	last_error = ""
	return true

static func _valid_rect(rect: Rect2i) -> bool:
	return rect.size.x >= 1 and rect.size.y >= 1 and rect.size.x <= VRAM_WIDTH and rect.size.y <= VRAM_HEIGHT

## Clears VRAM, the drawing environment, the display state and the counters.
func reset() -> void:
	if _ready_to_use():
		_gpu.call("reset")

# ---- VRAM -----------------------------------------------------------------------------------

## Replaces all of VRAM with `bytes` (1024x512 little-endian 16-bit words, 1 MiB).
func set_vram(bytes: PackedByteArray) -> String:
	if not _ready_to_use():
		return last_error
	return _fail_if(_gpu.call("set_vram", bytes))

func vram() -> PackedByteArray:
	if not _ready_to_use():
		return PackedByteArray()
	return _gpu.call("get_vram")

func _fail_if(error: String) -> String:
	if not error.is_empty():
		last_error = error
	return error

## CPU to VRAM (LoadImage): `words` is rect.size.x * rect.size.y little-endian 16-bit pixels.
## Coordinates wrap at the VRAM edges; the mask bits of the environment apply.
func load_image(rect: Rect2i, words: PackedByteArray) -> String:
	if not _ready_to_use():
		return last_error
	return _fail_if(_gpu.call("load_image", rect.position.x, rect.position.y, rect.size.x, rect.size.y, words))

## VRAM to CPU (StoreImage): little-endian 16-bit pixels; empty with last_error set for an invalid size.
func store_image(rect: Rect2i) -> PackedByteArray:
	if not _ready_to_use():
		return PackedByteArray()
	if not _valid_rect(rect):
		_fail("rectangle must be 1..1024 by 1..512")
		return PackedByteArray()
	return _gpu.call("store_image", rect.position.x, rect.position.y, rect.size.x, rect.size.y)

## VRAM to VRAM (MoveImage).
func move_image(source: Vector2i, destination: Vector2i, size: Vector2i) -> String:
	if not _ready_to_use():
		return last_error
	return _fail_if(_gpu.call("move_image", source.x, source.y, destination.x, destination.y, size.x, size.y))

## Fill rectangle (ClearImage): rgb24 is 0xBBGGRR as in the GP0(02h) word; ignores the draw area.
func fill_rect(rect: Rect2i, rgb24: int) -> void:
	if _ready_to_use():
		_gpu.call("fill_rect", rect.position.x, rect.position.y, rect.size.x, rect.size.y, rgb24)

# ---- drawing commands -----------------------------------------------------------------------

## One GP0 word (command or parameter), as written to the GPU data port.
func gp0_word(word: int) -> void:
	if _ready_to_use():
		_gpu.call("gp0_word", word)

## A raw GP0 word stream: little-endian 32-bit words, commands and their parameters in order.
func gp0_stream(words: PackedByteArray) -> String:
	if not _ready_to_use():
		return last_error
	return _fail_if(_gpu.call("gp0_words", words))

## A GP1 display-control word (display start, display mode, texture-disable permission, ...).
func gp1(word: int) -> void:
	if _ready_to_use():
		_gpu.call("gp1", word)

## Walks the ordering table / primitive linked list at `address` in `ram` exactly as DrawOTag's DMA
## does (tag word: next pointer in the low 24 bits, payload word count in the high 8 bits; the walk
## ends after the node whose next pointer has bit 23 set). `ram` is the main RAM image, read only.
## The walk is bounded: a loop (a node reached twice), more than `max_nodes` nodes or more than
## `max_words` words stops it and sets "error" (also last_error); what was drawn before stays drawn.
## Returns {nodes, words, terminated, node_limit, word_limit, cycle, error}.
func draw_ordering_table(ram: PackedByteArray, address: int, max_nodes: int = DEFAULT_MAX_NODES, max_words: int = DEFAULT_MAX_WORDS) -> Dictionary:
	if not _ready_to_use():
		return {"error": last_error}
	var result: Dictionary = _gpu.call("draw_otag", ram, address, max_nodes, max_words)
	_fail_if(str(result.get("error", "")))
	return result

## draw_ordering_table() on the port's game memory (core/game_memory.gd); only its `ram` is read.
func draw_ordering_table_from_memory(memory: RefCounted, address: int, max_nodes: int = DEFAULT_MAX_NODES, max_words: int = DEFAULT_MAX_WORDS) -> Dictionary:
	var ram: Variant = memory.get("ram") if memory != null else null
	if not ram is PackedByteArray:
		return {"error": _fail("game memory has no PackedByteArray ram")}
	return draw_ordering_table(ram, address, max_nodes, max_words)

# ---- read-out -------------------------------------------------------------------------------

## The display area the GP1 commands selected: {x, y, width, height, enabled, true_color}.
func display_info() -> Dictionary:
	if not _ready_to_use():
		return {}
	return _gpu.call("display_info")

## A VRAM rectangle as an RGBA8 Image (alpha 255); null with last_error set for an invalid size.
func read_image(rect: Rect2i, replicate_bits: bool = false) -> Image:
	if not _ready_to_use():
		return null
	if not _valid_rect(rect):
		_fail("rectangle must be 1..1024 by 1..512")
		return null
	var bytes: PackedByteArray = _gpu.call("read_rgba", rect.position.x, rect.position.y, rect.size.x, rect.size.y, replicate_bits)
	return Image.create_from_data(rect.size.x, rect.size.y, false, Image.FORMAT_RGBA8, bytes)

## The display area as an RGBA8 Image: native 320x240 (or the width and height GP1(08h) selected).
## A display switched off by GP1(03h) gives an opaque black image of the same size.
func display_image(replicate_bits: bool = false) -> Image:
	var info: Dictionary = display_info()
	if info.is_empty():
		return null
	var rect := Rect2i(int(info["x"]), int(info["y"]), int(info["width"]), int(info["height"]))
	if not bool(info["enabled"]):
		var black: Image = Image.create(rect.size.x, rect.size.y, false, Image.FORMAT_RGBA8)
		black.fill(Color.BLACK)
		return black
	return read_image(rect, replicate_bits)

# ---- diagnostics ----------------------------------------------------------------------------

## Draw area, offset, texture page and mode, window, mask and dither state.
func environment() -> Dictionary:
	return _gpu.call("environment") if _ready_to_use() else {}

## Primitive and command counts since clear_counters(), by name (POLY_FT4, SPRT_16, SEMI_2_SUB, ...).
func counters() -> Dictionary:
	return _gpu.call("counters") if _ready_to_use() else {}

func clear_counters() -> void:
	if _ready_to_use():
		_gpu.call("clear_counters")

## Bounding box of the pixels written since clear_dirty(); zero size when none.
func dirty_rect() -> Rect2i:
	return _gpu.call("dirty_rect") if _ready_to_use() else Rect2i()

func clear_dirty() -> void:
	if _ready_to_use():
		_gpu.call("clear_dirty")
