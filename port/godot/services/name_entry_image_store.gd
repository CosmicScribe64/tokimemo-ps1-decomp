extends RefCounted
## T-39004: authenticated original NAME_ENT frame images, independent of language.
const Paths = preload("res://services/external_content_path.gd")
const Checks = preload("res://services/translated_image_store.gd")
const SOURCE_SHA := "8bfdd21c0fc2b63fb9b3041a21c7fdba82f3a071e6375193cdc4e679e165cb2f"
const OVERLAY_SHA := "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2"
const BACKGROUND_SHA := "7e9491995df6d5a373d5636c8148b1768bddef3113db49a512c813d6c37874da"
const MAINROM_SHA := "f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c"
var error := ""
var manifest: Dictionary = {}
var images: Dictionary = {}

func _fail(message: String) -> bool:
	manifest.clear()
	images.clear()
	error = message
	return false

func open_pack(path: String, expected: String) -> bool:
	_fail("")
	if not Checks._hash(expected) or not Paths.is_external_file(path) or FileAccess.get_size(path) > 1024 * 1024 or FileAccess.get_sha256(path) != expected:
		return _fail("NAME_ENT image manifest path/hash differs")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.name-entry-media.v1" or not parsed.get("frames") is Dictionary or not parsed.get("backgrounds") is Array:
		return _fail("NAME_ENT image schema/source differs")
	if not parsed.get("inputs") is Dictionary or not parsed.inputs.get("name") is Dictionary or parsed.inputs.name.get("sha256") != SOURCE_SHA:
		return _fail("NAME_ENT original identity differs")
	if not parsed.inputs.get("overlay") is Dictionary or parsed.inputs.overlay.get("sha256") != OVERLAY_SHA:
		return _fail("NAME_ENT overlay identity differs")
	if not parsed.inputs.get("background") is Dictionary or parsed.inputs.background.get("sha256") != BACKGROUND_SHA:
		return _fail("NAME_ENT base background identity differs")
	if not parsed.get("base_background") is Dictionary or not parsed.base_background.get("png") is Dictionary or parsed.base_background.png.get("width") != 320 or parsed.base_background.png.get("height") != 240:
		return _fail("NAME_ENT base background geometry differs")
	if not preload("res://services/mdec_model_identity.gd").valid(parsed.base_background.get("decode")):
		return _fail("NAME_ENT decoder model identity differs")
	if parsed["frames"].size() != 29 or parsed["backgrounds"].size() != 6:
		return _fail("NAME_ENT image inventory differs")
	if not parsed.get("digits") is Dictionary or parsed.digits.size() != 20 or not parsed.get("prompts") is Dictionary:
		return _fail("NAME_ENT digit/heading inventory differs")
	var records: Array = parsed["frames"].values() + parsed["backgrounds"]
	records.append(parsed.base_background)
	if parsed.has("cursor") or parsed.has("marker_sequence"):
		if not parsed.inputs.get("mainrom") is Dictionary or parsed.inputs.mainrom.get("sha256") != MAINROM_SHA:
			return _fail("NAME_ENT cursor original identity differs")
		var pointer: Variant = parsed.get("cursor")
		if not pointer is Dictionary or pointer.get("frame") != 156 or not pointer.get("origin") is Array or pointer.origin.size() != 2 or pointer.origin[0] != 0 or pointer.origin[1] != 3 or not pointer.get("png") is Dictionary or pointer.png.get("width") != 24 or pointer.png.get("height") != 16:
			return _fail("NAME_ENT cursor geometry differs")
		var sequence: Variant = parsed.get("marker_sequence")
		if not sequence is Dictionary or sequence.get("pointer") != 0x801aaf48 or not sequence.get("steps") is Array or sequence.steps.size() != 3:
			return _fail("NAME_ENT marker sequence differs")
		for index: int in 3:
			var step: Variant = sequence.steps[index]
			if not step is Array or step.size() != 2 or step[0] != 10+index or step[1] != 3: return _fail("NAME_ENT marker sequence step differs")
		records.append(pointer)
	if parsed.has("cursors"):
		if not parsed.get("cursor") is Dictionary or not parsed.cursors is Dictionary or parsed.cursors.size() != 20:
			return _fail("NAME_ENT cursor style inventory differs")
		var indices: Array[int] = [0]
		for index: int in range(155,174): indices.append(index)
		for index: int in indices:
			var pointer: Variant = parsed.cursors.get(str(index))
			if not pointer is Dictionary or pointer.get("frame") != index or not pointer.get("png") is Dictionary:
				return _fail("NAME_ENT cursor style identity differs")
			for field: String in ["bounds","origin"]:
				var values: Variant = pointer.get(field)
				if not values is Array or values.size() != (4 if field == "bounds" else 2): return _fail("NAME_ENT cursor style geometry differs")
				for value: Variant in values:
					if not Checks._integer(value) or abs(float(value)) > 64: return _fail("NAME_ENT cursor style geometry exceeds limits")
			if pointer.bounds[2] != pointer.png.get("width") or pointer.bounds[3] != pointer.png.get("height"):
				return _fail("NAME_ENT cursor style bounds differ")
			records.append(pointer)
	for kind: String in ["small", "wide"]:
		for digit: int in range(10):
			var spec: Variant = parsed.digits.get("%s-%d" % [kind, digit])
			if not spec is Dictionary or spec.get("width") != (8 if kind == "small" else 16) or spec.get("height") != 16:
				return _fail("NAME_ENT digit identity differs")
			records.append({"png":spec,"stp_png":spec})
	for key: String in ["family", "given", "nickname", "birthday", "confirm", "shiori_family", "shiori_given"]:
		var prompt: Variant = parsed.prompts.get(key)
		if not prompt is String or prompt.length() > 256 or prompt.length() % 4 != 0:
			return _fail("NAME_ENT heading encoding differs")
		for letter: String in prompt:
			if letter not in "0123456789abcdef": return _fail("NAME_ENT heading is not hexadecimal")
	var pending: Dictionary = {}
	for row: Variant in records:
		if not row is Dictionary:
			return _fail("NAME_ENT image record differs")
		for key: String in ["png", "stp_png"]:
			var spec: Variant = row.get(key)
			if not spec is Dictionary or not Checks._relative(spec.get("path")) or not Checks._hash(spec.get("sha256")) or not Checks._integer(spec.get("width")) or not Checks._integer(spec.get("height")) or not Checks._integer(spec.get("size")):
				return _fail("NAME_ENT image payload fields differ")
			if int(spec["width"]) < 1 or int(spec["height"]) < 1 or int(spec["width"]) > 384 or int(spec["height"]) > 256 or int(spec["size"]) < 1 or int(spec["size"]) > 1024 * 1024:
				return _fail("NAME_ENT image payload limit exceeded")
			var absolute: String = path.get_base_dir().path_join(spec["path"])
			if not Paths.is_external_file(absolute) or FileAccess.get_size(absolute) != int(spec["size"]) or FileAccess.get_sha256(absolute) != spec["sha256"]:
				return _fail("NAME_ENT image payload path/hash differs")
			var image := Image.new()
			if image.load_png_from_buffer(FileAccess.get_file_as_bytes(absolute)) != OK or image.get_size() != Vector2i(spec["width"], spec["height"]):
				return _fail("NAME_ENT PNG decode/dimensions differ")
			pending[spec["path"]] = image
		if row.png.width != row.stp_png.width or row.png.height != row.stp_png.height:
			return _fail("NAME_ENT color and mask dimensions differ")
	for index: int in range(29):
		if not parsed["frames"].has(str(index)):
			return _fail("NAME_ENT frame identity is missing")
		var frame: Dictionary = parsed["frames"][str(index)]
		for field: String in ["bounds", "origin"]:
			var values: Variant = frame.get(field)
			if not values is Array or values.size() != (4 if field == "bounds" else 2):
				return _fail("NAME_ENT frame geometry differs")
			for value: Variant in values:
				if not Checks._integer(value) or abs(float(value)) > 4096:
					return _fail("NAME_ENT frame geometry is out of bounds")
		if frame.get("frame_index") != index or frame.bounds[2] != frame.png.width or frame.bounds[3] != frame.png.height:
			return _fail("NAME_ENT frame bounds differ from its payload")
	for index: int in range(6):
		var background: Dictionary = parsed.backgrounds[index]
		if background.png.width != 128 or background.png.height != 128 or background.get("variant") != "source-page-background-%d" % index:
			return _fail("NAME_ENT background page identity differs")
	manifest = parsed
	images = pending
	error = ""
	return true

func frame_image(index: int, translations: RefCounted = null) -> Image:
	var frame: Dictionary = manifest.get("frames", {}).get(str(index), {})
	if frame.is_empty():
		return null
	var native: Image = images[frame["png"]["path"]]
	if translations != null:
		var candidate: Image = translations.replacement(SOURCE_SHA, index, "name-entry-actor-clut-10", frame["png"]["sha256"])
		if candidate != null and candidate.get_size() == native.get_size():
			var mask: Image = images[frame["stp_png"]["path"]]
			var safe := true
			for byte: int in mask.get_data():
				if byte != 0: safe = false
			for y: int in range(candidate.get_height()):
				for x: int in range(candidate.get_width()):
					if candidate.get_pixel(x,y).a8 not in [0,255]: safe = false
			if safe: return candidate
	return native
