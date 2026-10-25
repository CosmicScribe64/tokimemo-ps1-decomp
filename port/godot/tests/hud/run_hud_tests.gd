extends SceneTree

const Glyphs = preload("res://presentation/native_glyphs.gd")
const Hud = preload("res://ui/native_hud.gd")
const Commands = preload("res://ui/command_hud.gd")

func _initialize() -> void:
	var failures: Array[String] = []
	_test_font_modulation(failures)
	_test_glyph_records(failures)
	_test_external_font_gate(failures)
	_test_calendar_layout(failures)
	_test_parameter_layout(failures)
	_test_command_bridge(failures)
	if failures.is_empty():
		print("PASS: HUD synthetic suite")
		quit(0)
		return
	for failure: String in failures:
		push_error(failure)
	quit(1)

func _test_font_modulation(failures: Array[String]) -> void:
	# Synthetic channels 20/25/25 reproduce the native quantization boundary.
	var word: int = 20 | (25 << 5) | (25 << 10)
	var faded: Color = Glyphs._bgr555_color(word, 0x60)
	_check(faded == Color8(123, 148, 148, 255),
		"font modulation quantizes before eight-bit expansion", failures)
	_check(Glyphs._bgr555_color(word, 0x80) == Color8(165, 206, 206, 255),
		"full brightness preserves native five-bit color", failures)
	_check(Glyphs._bgr555_color(word, 0) == Color8(0, 0, 0, 255),
		"zero brightness retains opaque black texels", failures)

func _test_glyph_records(failures: Array[String]) -> void:
	var bank := PackedByteArray()
	bank.resize(36)
	bank[0] = 0x80
	bank[17] = 0x01
	var metadata := _font_metadata(bank)
	var found: Dictionary = Glyphs.resolve(PackedByteArray([0x81, 0x40]), metadata, bank)
	_check(found.get("status") == "glyph", "known code unit resolves", failures)
	_check(found.get("glyph_index") == 0, "first source range maps to its source glyph", failures)
	_check(found.get("pixels", PackedByteArray()).size() == 144, "18-byte source glyph expands to 12x12", failures)
	_check(found["pixels"][0] == 1 and found["pixels"][1] == 0 and found["pixels"][143] == 1,
		"glyph bits retain MSB-first source ordering", failures)
	_check(Glyphs.resolve(PackedByteArray([0x81, 0x42]), metadata, bank).get("glyph_index") == 1,
		"second range begins at its explicit glyph index", failures)
	_check(Glyphs.resolve(PackedByteArray([0x81, 0x44]), metadata, bank).get("status") == "ignored_above_font_sentinel",
		"sentinel code unit is ignored as in load_string", failures)
	_check(Glyphs.resolve(PackedByteArray([0x81, 0x3F]), metadata, bank).get("status") == "renderer_error_and_string_end",
		"below-range code unit preserves source error stop", failures)
	var corrupted := bank.duplicate()
	corrupted[0] ^= 1
	_check(Glyphs.resolve(PackedByteArray([0x81, 0x40]), metadata, corrupted).get("status") == "unsupported",
		"bank hash mismatch fails closed", failures)

func _test_external_font_gate(failures: Array[String]) -> void:
	var store := Glyphs.new()
	_check(store.get_glyph(PackedByteArray([0x82, 0x4F])).get("status") == "unsupported",
		"typed getter fails before external font package is opened", failures)
	_check(not store.open_external_font("res://fake-font").is_empty(),
		"font data cannot be loaded from project resources", failures)

func _test_calendar_layout(failures: Array[String]) -> void:
	var calendar: Dictionary = Hud.build_calendar({"year": 95, "month": 4, "day": 5,
		"weekday": 5, "day_kind": 0})
	_check(calendar.get("status") == "supported", "calendar slice builds", failures)
	var actors: Array = calendar["actors"]
	_check(actors.size() == 8, "calendar emits year/month/day/weekday and marker actors", failures)
	_check(actors[0]["actor_index"] == 16 and actors[0]["sprite_id"] == 0x3C,
		"calendar actor records preserve native ascending actor draw order", failures)
	_check(actors[1]["actor_index"] == 17 and actors[1]["sprite_id"] == 0x4D,
		"year tens actor uses native actor and digit sprite IDs", failures)
	_check(actors[3]["actor_index"] == 19 and not actors[3]["visible"] and actors[3]["x"] == 0xFF,
		"single-digit month suppresses tens at native sentinel coordinates", failures)
	_check(actors[6]["actor_index"] == 22 and actors[6]["x"] == 0x90,
		"single-digit day uses the native centered ones position", failures)
	_check(actors[7]["color_mode"] == 0x43, "Saturday digit color mode is source-proved", failures)
	var sunday: Dictionary = Hud.build_calendar({"year": 95, "month": 4, "day": 5,
		"weekday": 6, "day_kind": 1})
	_check(sunday["actors"][7]["color_mode"] == 0x44, "Sunday uses native color mode", failures)
	var holiday: Dictionary = Hud.build_calendar({"year": 95, "month": 4, "day": 5,
		"weekday": 5, "day_kind": 2})
	_check(holiday["actors"][7]["color_mode"] == 0x44, "calendar day-kind overrides weekday color", failures)
	_check(Hud.build_calendar({"year": 95, "month": 13, "day": 5, "weekday": 2,
		"day_kind": 0}).get("status") == "unsupported", "out-of-range dates stop", failures)

func _test_parameter_layout(failures: Array[String]) -> void:
	var parameters := {"condition": 7, "liberal_arts": 42, "science": 999,
		"art": 0, "athletics": 18, "trivia": 100, "looks": 2, "guts": 3, "stress": 4}
	var panel: Dictionary = Hud.build_parameters(parameters)
	_check(panel.get("status") == "supported" and panel["actors"].size() == 28,
		"nine native three-digit parameters plus separator are emitted", failures)
	_check(panel["actors"][0]["actor_index"] == 32 and panel["actors"][0]["sprite_id"] == 0x44,
		"condition hundreds actor keeps source digit ordering", failures)
	_check(panel["actors"][2]["sprite_id"] == 0x4B, "condition ones digit uses source decimal extraction", failures)
	_check(panel["actors"][6]["actor_index"] == 38 and panel["actors"][6]["sprite_id"] == 0x4D,
		"science 999 occupies its native actor group", failures)
	parameters["science"] = 1000
	_check(Hud.build_parameters(parameters).get("status") == "unsupported",
		"unsupported parameter range stops", failures)

func _test_command_bridge(failures: Array[String]) -> void:
	var records: Array = []
	for icon_id in range(14):
		records.append({"icon_id": icon_id, "component_id": "synthetic-icon-atlas",
			"frame_id": "frame-%02d" % icon_id, "x": -0x76 + icon_id * 16,
			"y": -0x29, "enabled": true})
	var route: Dictionary = Commands.build(7, records)
	_check(route.get("status") == "supported" and route.get("sprites", []).size() == 14,
		"command 7 bridge accepts complete external icon records", failures)
	_check(route["sprites"][7]["selected"], "selected command retains its stable native ID", failures)
	_check(Commands.build(4, records).get("status") == "unsupported",
		"unproved command routes stop before presentation", failures)
	_check(Commands.build(7, records.slice(0, 13)).get("status") == "unsupported",
		"partial icon atlas does not silently progress", failures)

func _font_metadata(bank: PackedByteArray) -> Dictionary:
	var ranges: Array = [{"first": 0x8140, "glyph": 0}, {"first": 0x8142, "glyph": 1},
		{"first": 0x8144, "glyph": 0}]
	var raw := PackedByteArray([0x40, 0x81, 0, 0, 0x42, 0x81, 1, 0, 0x44, 0x81, 0, 0])
	return {"id": "synthetic-font", "source_path": "synthetic.bin",
		"source_sha256": "1".repeat(64), "ranges": ranges, "range_raw_hex": raw.hex_encode(),
		"glyph_count": 2, "glyph_stride": 18, "glyph_bank_sha256": _sha256(bank),
		"format": {"encoding": "1bpp_msb_first", "width": 12, "height": 12,
			"upload_width": 16, "upload_height": 12, "upload_bpp": 4}}

func _sha256(bytes: PackedByteArray) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(bytes)
	return context.finish().hex_encode()

func _check(condition: bool, label: String, failures: Array[String]) -> void:
	if not condition:
		failures.append(label)
