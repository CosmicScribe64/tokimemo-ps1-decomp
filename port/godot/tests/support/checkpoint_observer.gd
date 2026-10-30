extends RefCounted
## T-39019 (T-39018 WP-02): writes tokimemo.checkpoint-receipt.v2 records that
## bind the native 320x240 canvas image at write time (fixes T-39008 C2).
##
## record() encodes the CPU-side composed Image (or a 1x SubViewport readback)
## to PNG in memory, hashes those bytes, writes them, re-reads the file and
## refuses the receipt if the file hash differs. The receipt also carries the
## RGBA8 pixel hash, the clocks the caller observed after the completed update
## (frame_count, game_mode, mode_stage, mode_step, substep), the RNG draw count,
## language/aspect/filter settings, content and translation pack hashes, this
## script's hash, the producer script's hash and the renderer identity.
## verify() recomputes the binding; a changed PNG, receipt field or observer
## breaks it. The observer reads state only; it never advances the game.

const SCHEMA := "tokimemo.checkpoint-receipt.v2"
const SELF_PATH := "res://tests/support/checkpoint_observer.gd"
const NATIVE_SIZE := Vector2i(320, 240)
const SOURCES := ["cpu_composed_image", "subviewport_1x_readback"]
const CLOCK_KEYS := ["frame_count", "game_mode", "mode_stage", "mode_step", "substep"]

var out_dir: String
var producer_path: String
var receipts: Dictionary = {}
var errors: Array[String] = []

func _init(directory: String, producer: String) -> void:
	out_dir = directory
	producer_path = producer

static func sha256_bytes(data: PackedByteArray) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(data)
	return context.finish().hex_encode()

static func rgba_sha256(image: Image) -> String:
	var copy := image.duplicate() as Image
	if copy.get_format() != Image.FORMAT_RGBA8: copy.convert(Image.FORMAT_RGBA8)
	return sha256_bytes(copy.get_data())

static func settings_identity(tree: SceneTree) -> Dictionary:
	# Effective preferences when the ModernSettings autoload exists; otherwise the
	# fidelity defaults are reported as such, never guessed.
	var node: Node = tree.root.get_node_or_null("ModernSettings") if tree != null else null
	if node == null:
		return {"source": "no ModernSettings node: fidelity defaults", "language": "ja", "aspect": "original", "filter": "integer"}
	var effective: Dictionary = node.preferences.effective_settings()
	return {"source": "ModernSettings.effective_settings", "language": str(effective.language.text),
		"voice": str(effective.language.voice), "aspect": str(effective.display.aspect), "filter": str(effective.display.filter),
		"text_pack_sha256": str(effective.language.text_sha256), "ui_text_pack_sha256": str(effective.language.ui_text_sha256),
		"dialogue_pack_sha256": str(effective.language.dialogue_sha256)}

static func renderer_identity() -> Dictionary:
	return {"engine": Engine.get_version_info().get("string", ""),
		"rendering_method": str(RenderingServer.get_current_rendering_method()),
		"adapter": RenderingServer.get_video_adapter_name(), "vendor": RenderingServer.get_video_adapter_vendor(),
		"api": RenderingServer.get_video_adapter_api_version()}

## key: catalog port_observer_key. clocks: post-update values (CLOCK_KEYS
## required, integers). rng: {"draw_count": int, ...}. content: pack hashes.
## extra: producer-specific observations copied verbatim (e.g. opening state).
## Returns the receipt, or {} with errors[] filled when it cannot be bound.
func record(key: String, image: Image, source: String, clocks: Dictionary, rng: Dictionary,
		settings: Dictionary, content: Dictionary, extra: Dictionary = {}) -> Dictionary:
	if key.is_empty() or not key.is_valid_filename():
		return _refuse("invalid checkpoint key '%s'" % key)
	if receipts.has(key):
		return _refuse("checkpoint %s already recorded" % key)
	if image == null or image.get_size() != NATIVE_SIZE:
		return _refuse("%s: native canvas must be %dx%d, got %s" % [key, NATIVE_SIZE.x, NATIVE_SIZE.y, image.get_size() if image != null else "null"])
	if not source in SOURCES:
		return _refuse("%s: unknown image source %s" % [key, source])
	for name: String in CLOCK_KEYS:
		if not clocks.get(name) is int:
			return _refuse("%s: clock %s must be an integer" % [key, name])
	if not rng.get("draw_count") is int:
		return _refuse("%s: rng.draw_count must be an integer" % key)
	var png := image.save_png_to_buffer()
	var png_sha := sha256_bytes(png)
	var png_path := out_dir.path_join(key + ".png")
	var file := FileAccess.open(png_path, FileAccess.WRITE)
	if file == null:
		return _refuse("%s: cannot write %s" % [key, png_path])
	file.store_buffer(png); file.close()
	if FileAccess.get_sha256(png_path) != png_sha:
		return _refuse("%s: written PNG differs from the encoded bytes" % key)
	var receipt := {"schema": SCHEMA, "key": key,
		"native_canvas": {"width": image.get_width(), "height": image.get_height(), "source": source,
			"png": key + ".png", "png_sha256": png_sha, "rgba8_sha256": rgba_sha256(image)},
		"clocks": clocks.duplicate(true), "rng": rng.duplicate(true), "settings": settings.duplicate(true),
		"content": content.duplicate(true), "renderer": renderer_identity(),
		"observer": {"path": SELF_PATH, "sha256": FileAccess.get_sha256(SELF_PATH),
			"producer": producer_path, "producer_sha256": FileAccess.get_sha256(producer_path)},
		"extra": extra.duplicate(true)}
	receipt["receipt_sha256"] = receipt_digest(receipt)
	receipts[key] = receipt
	return receipt

## Digest over the receipt without its own digest field (sorted JSON). JSON
## numbers read back as floats, so integral floats are hashed as integers.
static func receipt_digest(receipt: Dictionary) -> String:
	var body: Dictionary = _canonical(receipt)
	body.erase("receipt_sha256")
	return sha256_bytes(JSON.stringify(body, "", true).to_utf8_buffer())

static func _canonical(value: Variant) -> Variant:
	if value is Dictionary:
		var result := {}
		for key: Variant in value: result[str(key)] = _canonical(value[key])
		return result
	if value is Array:
		return (value as Array).map(func(item: Variant) -> Variant: return _canonical(item))
	if value is float and is_finite(value) and value == floorf(value) and absf(value) < 9.0e15:
		return int(value)
	return value

## Empty string when the PNG next to the receipt and the receipt itself still
## match what was bound at write time.
static func verify(receipt: Dictionary, directory: String) -> String:
	if receipt.get("schema") != SCHEMA: return "not a v2 receipt"
	if receipt.get("receipt_sha256") != receipt_digest(receipt): return "receipt fields changed after binding"
	var canvas: Dictionary = receipt.get("native_canvas", {})
	var path := directory.path_join(str(canvas.get("png", "")))
	if not FileAccess.file_exists(path): return "bound PNG is missing: " + path
	if FileAccess.get_sha256(path) != canvas.get("png_sha256"): return "PNG changed after binding: " + path
	var image := Image.load_from_file(path)
	if image == null or image.get_size() != NATIVE_SIZE: return "bound PNG is not a native canvas"
	if rgba_sha256(image) != canvas.get("rgba8_sha256"): return "decoded pixels differ from the bound RGBA hash"
	return ""

## Writes all receipts as one JSON file (checkpoint-receipts.json).
func write(path: String = "") -> String:
	var target := path if not path.is_empty() else out_dir.path_join("checkpoint-receipts.json")
	var file := FileAccess.open(target, FileAccess.WRITE)
	if file == null: return "cannot write " + target
	file.store_string(JSON.stringify({"schema": SCHEMA + "-set", "receipts": receipts, "errors": errors}, "\t", true))
	file.close()
	return ""

func _refuse(message: String) -> Dictionary:
	errors.append(message)
	return {}
