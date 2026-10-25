class_name NativeGlyphs
extends RefCounted
## Source-backed bitmap lookup and unpacking for the original game's 12x12 glyphs.
## The external pack is prepared by tools/assets/text_font.py through the
## verified font_content adapter; no original source format is decoded here.

const FORMAT := {
	"encoding": "1bpp_msb_first",
	"width": 12,
	"height": 12,
	"upload_width": 16,
	"upload_height": 12,
	"upload_bpp": 4,
}
const SOURCE_SHA256 := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const DESCRIPTOR_SHA256 := "675a40455cb34ab2f26203e43208bcb9d610ce0c428a4fd4d6bcee1d037a9e77"
const PALETTE_LABELS := ["text_palette_0", "text_palette_1",
	"text_shadow_palette_0", "text_shadow_palette_1"]
const PALETTE_ADDRESSES := [0x800B3FAC, 0x800B3FCC, 0x800B3FEC, 0x800B400C]
const PROOF_ASSEMBLY := "asm/data/main/text_system.data.s"

var _metadata: Dictionary = {}
var _glyph_bank := PackedByteArray()
var _palettes: Array = []

func open_external_font(root_path: String) -> String:
	_metadata.clear()
	_glyph_bank.clear()
	_palettes.clear()
	if not _no_root_symlink(root_path) or not root_path.ends_with("/.asset-cache/port/fonts/native-font-v2"):
		return "font root must be an external non-symlink .asset-cache/port package"
	var descriptor_path: String = root_path.path_join("font.json")
	var payload_path: String = root_path.path_join("glyph_bank.bin")
	if _is_link(descriptor_path) or _is_link(payload_path):
		return "font descriptor or bank cannot be symlinked"
	if not FileAccess.file_exists(descriptor_path) or not FileAccess.file_exists(payload_path):
		return "font descriptor or glyph bank is missing"
	if FileAccess.get_size(descriptor_path) > 256 * 1024:
		return "font descriptor exceeds 256 KiB"
	if DESCRIPTOR_SHA256.is_empty() or FileAccess.get_sha256(descriptor_path) != DESCRIPTOR_SHA256:
		return "font descriptor hash differs from the reviewed external source pack"
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(descriptor_path))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.native-font.v1":
		return "unsupported external font descriptor schema"
	var expected_keys := ["schema", "schema_version", "metadata", "payload",
		"payload_size_bytes", "payload_sha256", "source_proof", "palettes"]
	if not _exact_keys(parsed, expected_keys) or int(parsed.get("schema_version", 0)) != 1:
		return "font descriptor fields/version do not match v1"
	if parsed["payload"] != "glyph_bank.bin":
		return "font payload locator is unsupported"
	var metadata: Variant = parsed["metadata"]
	if not metadata is Dictionary or str(metadata.get("source_sha256", "")) != SOURCE_SHA256:
		return "font source is not the reviewed original executable"
	if str(metadata.get("source_path", "")) != "SLPM_86.053" or int(metadata.get("glyph_count", 0)) != 3418:
		return "font source identity/count is unsupported"
	if FileAccess.get_size(payload_path) != int(parsed["payload_size_bytes"]) or int(parsed["payload_size_bytes"]) != 3418 * 18:
		return "font payload size differs from the source glyph extent"
	if not _is_sha256(str(parsed["payload_sha256"])) or FileAccess.get_sha256(payload_path) != str(parsed["payload_sha256"]):
		return "font payload hash mismatch"
	var proof: Variant = parsed["source_proof"]
	if not proof is Dictionary or proof.get("source_sha256") != SOURCE_SHA256 or proof.get("source_path") != "SLPM_86.053":
		return "font source proof does not identify the accepted original executable"
	if proof.get("reader") != "tools.assets.text_font.font_metadata" or proof.get("proof_reader") != "tools.assets.text_font.verify_font_source_proof" or proof.get("palette_assembly_path") != PROOF_ASSEMBLY:
		return "font descriptor lacks canonical source reader provenance"
	var labels: Dictionary = {}
	for label_record: Variant in proof.get("verified_data_labels", []):
		if label_record is Dictionary:
			labels[str(label_record.get("label", ""))] = true
	if not labels.has("text_font_range_table") or not labels.has("text_font_glyphs"):
		return "font source proof did not cover both range and bitmap labels"
	var palette_error: String = _validate_palettes(parsed["palettes"])
	if not palette_error.is_empty():
		return palette_error
	var bank: PackedByteArray = FileAccess.get_file_as_bytes(payload_path)
	var error: String = _validate(metadata, bank)
	if not error.is_empty():
		return error
	_metadata = metadata.duplicate(true)
	_glyph_bank = bank
	_palettes = parsed["palettes"].duplicate(true)
	return ""

func get_glyph(code_unit: PackedByteArray) -> Dictionary:
	if _metadata.is_empty():
		return {"status": "unsupported", "diagnostic": "external font pack is not open"}
	return resolve(code_unit, _metadata, _glyph_bank)

func get_glyph_image(code_unit: PackedByteArray, palette_row: int,
		palette_family: int = 0, brightness: int = 0x80) -> Dictionary:
	if palette_row < 0 or palette_row >= 8 or palette_family < 0 or palette_family > 1:
		return {"status": "unsupported", "diagnostic": "text palette selector is outside native table bounds"}
	if brightness < 0 or brightness > 0x80:
		return {"status": "unsupported", "diagnostic": "screen brightness is outside the source byte range"}
	var glyph: Dictionary = get_glyph(code_unit)
	if glyph.get("status") != "glyph":
		return glyph
	var palette: Dictionary = _palettes[palette_family]
	var words: Array = palette["words"]
	var foreground_word: int = int(words[palette_row * 2 + 1])
	var color: Color = _bgr555_color(foreground_word, brightness)
	var pixels: PackedByteArray = glyph["pixels"]
	var image: Image = Image.create(12, 12, false, Image.FORMAT_RGBA8)
	image.fill(Color(0, 0, 0, 0))
	for pixel_index in range(pixels.size()):
		if pixels[pixel_index] != 0:
			image.set_pixel(pixel_index % 12, pixel_index / 12, color)
	return {"status": "glyph", "id": glyph["id"], "image": image,
		"palette_label": palette["label"], "palette_row": palette_row,
		"foreground_word": foreground_word, "source_sha256": _metadata["source_sha256"]}

static func resolve(code_unit: PackedByteArray, metadata: Dictionary,
		glyph_bank: PackedByteArray) -> Dictionary:
	var error: String = _validate(metadata, glyph_bank)
	if not error.is_empty():
		return {"status": "unsupported", "diagnostic": error}
	if code_unit.size() == 1:
		return {"status": "terminal_byte_before_nul", "raw_hex": code_unit.hex_encode()}
	if code_unit.size() != 2:
		return {"status": "unsupported", "diagnostic": "glyph code unit must be exactly two bytes"}
	if code_unit[0] == 0 or code_unit[1] == 0:
		return {"status": "string_end", "raw_hex": code_unit.hex_encode()}
	var code: int = (int(code_unit[0]) << 8) | int(code_unit[1])
	var ranges: Array = metadata["ranges"]
	if code < int(ranges[0]["first"]):
		return {"status": "renderer_error_and_string_end", "code": code}
	if code >= int(ranges[-1]["first"]):
		return {"status": "ignored_above_font_sentinel", "code": code}
	for index in range(1, ranges.size()):
		if code < int(ranges[index]["first"]):
			var glyph_index: int = code - int(ranges[index - 1]["first"]) + int(ranges[index - 1]["glyph"])
			if glyph_index < 0 or glyph_index >= int(metadata["glyph_count"]):
				return {"status": "unsupported", "diagnostic": "source range resolves outside glyph bank", "code": code}
			var byte_offset: int = glyph_index * 18
			return {
				"status": "glyph",
				"id": str(metadata["id"]) + ":g%04d" % glyph_index,
				"glyph_index": glyph_index,
				"pixels": expand_glyph(glyph_bank.slice(byte_offset, byte_offset + 18)),
			}
	return {"status": "unsupported", "diagnostic": "range table has no terminating sentinel", "code": code}

static func expand_glyph(record: PackedByteArray) -> PackedByteArray:
	## text_glyph_expand expands 18 source bytes into 144 0/1 texels, MSB first.
	if record.size() != 18:
		return PackedByteArray()
	var pixels := PackedByteArray()
	pixels.resize(144)
	for byte_index in range(18):
		for bit_index in range(8):
			pixels[byte_index * 8 + bit_index] = (int(record[byte_index]) >> (7 - bit_index)) & 1
	return pixels

static func _validate(metadata: Dictionary, glyph_bank: PackedByteArray) -> String:
	for key in ["id", "source_path", "source_sha256", "ranges", "range_raw_hex",
			"glyph_count", "glyph_stride", "glyph_bank_sha256", "format"]:
		if not metadata.has(key):
			return "font metadata is missing " + key
	if not _format_valid(metadata["format"]):
		return "font bitmap/upload format is unsupported"
	if typeof(metadata["id"]) != TYPE_STRING or str(metadata["id"]).is_empty():
		return "font identity is missing"
	if typeof(metadata["source_path"]) != TYPE_STRING or str(metadata["source_path"]).is_empty():
		return "font source path is missing"
	if not _is_sha256(str(metadata["source_sha256"])) or not _is_sha256(str(metadata["glyph_bank_sha256"])):
		return "font source hashes are malformed"
	if int(metadata["glyph_stride"]) != 18 or int(metadata["glyph_count"]) <= 0:
		return "font glyph stride/count is unsupported"
	if glyph_bank.size() != int(metadata["glyph_count"]) * 18:
		return "font glyph bank length does not match metadata"
	if _sha256(glyph_bank) != str(metadata["glyph_bank_sha256"]):
		return "font glyph bank hash does not match metadata"
	var ranges: Variant = metadata["ranges"]
	if not ranges is Array or ranges.size() < 2:
		return "font range table is missing its sentinel"
	var raw := PackedByteArray()
	var prior_first: int = -1
	for item in ranges:
		if not item is Dictionary or not item.has("first") or not item.has("glyph"):
			return "font range entry is malformed"
		var first: int = int(item["first"])
		var glyph: int = int(item["glyph"])
		if first < 0 or first > 0xFFFF or first <= prior_first or glyph < 0 or glyph > 0xFFFF:
			return "font ranges are not strictly ordered 16-bit records"
		raw.append(first & 0xFF)
		raw.append((first >> 8) & 0xFF)
		raw.append(glyph & 0xFF)
		raw.append((glyph >> 8) & 0xFF)
		prior_first = first
	if raw.hex_encode() != str(metadata["range_raw_hex"]).to_lower():
		return "font range metadata differs from the canonical raw table"
	return ""

func _validate_palettes(value: Variant) -> String:
	if not value is Array or value.size() != 4:
		return "font pack must provide all four original text palette tables"
	for index in range(4):
		var table: Variant = value[index]
		if not table is Dictionary:
			return "font palette record is malformed"
		if table.get("label") != PALETTE_LABELS[index] or int(table.get("address", -1)) != PALETTE_ADDRESSES[index]:
			return "font palette identity/address differs from the native source table"
		if int(table.get("length", 0)) != 0x20 or int(table.get("rows", 0)) != 8 or int(table.get("colors_per_row", 0)) != 2:
			return "font palette dimensions differ from LoadSquare source geometry"
		var words: Variant = table.get("words")
		if not words is Array or words.size() != 16:
			return "font palette word table is incomplete"
		var raw := PackedByteArray()
		for word_value: Variant in words:
			if not _is_exact_u16(word_value):
				return "font palette contains a non-u16 word"
			raw.append(int(word_value) & 0xFF)
			raw.append((int(word_value) >> 8) & 0xFF)
		if raw.hex_encode() != str(table.get("raw_hex", "")).to_lower() or not _is_sha256(str(table.get("sha256", ""))) or _sha256(raw) != str(table.get("sha256", "")):
			return "font palette bytes/hash do not match the source table"
	return ""

func _no_root_symlink(root: String) -> bool:
	if not root.is_absolute_path() or root.begins_with("res://"):
		return false
	var current := "/"
	for part in root.trim_prefix("/").split("/"):
		if part.is_empty():
			continue
		current = current.path_join(part)
		if _is_link(current):
			return false
	return true

func _is_link(path: String) -> bool:
	var dir := DirAccess.open(path.get_base_dir())
	return dir != null and dir.is_link(path.get_file())

static func _exact_keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key in expected:
		if not value.has(key):
			return false
	return true

func _is_exact_u16(value: Variant) -> bool:
	if value is bool or not (value is int or value is float):
		return false
	if value is float and (not is_finite(value) or floor(value) != value):
		return false
	return value >= 0 and value <= 0xFFFF

static func _format_valid(value: Variant) -> bool:
	if not value is Dictionary or not _exact_keys(value, ["encoding", "width", "height",
			"upload_width", "upload_height", "upload_bpp"]):
		return false
	if value["encoding"] != FORMAT["encoding"]:
		return false
	for key in ["width", "height", "upload_width", "upload_height", "upload_bpp"]:
		if not (value[key] is int or value[key] is float) or value[key] is bool:
			return false
		if value[key] is float and (not is_finite(value[key]) or floor(value[key]) != value[key]):
			return false
		if int(value[key]) != FORMAT[key]:
			return false
	return true

static func _is_sha256(value: String) -> bool:
	if value.length() != 64:
		return false
	for character in value:
		if not "0123456789abcdef".contains(character):
			return false
	return true

static func _sha256(bytes: PackedByteArray) -> String:
	var context := HashingContext.new()
	if context.start(HashingContext.HASH_SHA256) != OK:
		return ""
	context.update(bytes)
	return context.finish().hex_encode()

static func _bgr555_color(word: int, brightness: int) -> Color:
	## PSX channel order is R/G/B in bits 0..4/5..9/10..14. Text sprites set
	## semi-transparency off; STP is therefore retained as provenance but ignored.
	## Quantize texture modulation to native five-bit channels before display expansion.
	var red5: int = ((word & 0x1F) * brightness) >> 7
	var green5: int = (((word >> 5) & 0x1F) * brightness) >> 7
	var blue5: int = (((word >> 10) & 0x1F) * brightness) >> 7
	var red8: int = (red5 << 3) | (red5 >> 2)
	var green8: int = (green5 << 3) | (green5 >> 2)
	var blue8: int = (blue5 << 3) | (blue5 >> 2)
	return Color8(red8, green8, blue8, 255)
