extends SceneTree
## T-39016: synthetic contract tests for the source-keyed text layer (no game data).
const Store = preload("res://services/text_layer_store.gd")
const Packs = preload("res://services/translation_pack_service.gd")
const Layout = preload("res://presentation/native_latin_layout.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
var failures: Array[String] = []

class StubFont extends RefCounted:
	## Every code has a 3-texel-wide glyph at columns 4..6 except `missing`
	## (absent) and `blank` (resolves but has no texels).
	var missing := PackedByteArray([0x82, 0x9a])
	var blank := PackedByteArray([0x82, 0x99])
	func get_glyph(pair: PackedByteArray) -> Dictionary:
		if pair == missing:
			return {"status": "unsupported"}
		var pixels := PackedByteArray()
		pixels.resize(144)
		if pair != blank and pair != PackedByteArray([0x81, 0x40]):
			for y: int in 12:
				for x: int in [4, 5, 6]:
					pixels[y * 12 + x] = 1
		return {"status": "glyph", "pixels": pixels}

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func write_json(path: String, value: Dictionary) -> String:
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(value))
	file.close()
	return FileAccess.get_sha256(path)

func _initialize() -> void:
	var folder := ProjectSettings.globalize_path("user://text-layer-tests")
	DirAccess.make_dir_recursive_absolute(folder)
	var container_sha := "c".repeat(64)
	var source := PackedByteArray([0x82, 0xa0, 0x82, 0xa2])
	var raw := Store.terminated(source)
	var raw_sha := Store._sha(raw)
	var physical := Store.source_id("SLPM_86.053", 0x6f000, source)
	check(physical == "SLPM_86.053:0006f000:" + raw_sha.left(12), "physical id follows the producer scheme")
	check(Store.source_id("SLPM_86.053", 0x6f000, raw + PackedByteArray([0x41, 0])) == physical, "id hashes through the first NUL")
	var pack := {"schema": Store.SCHEMA, "language": "en",
		"approval": {"approver": "user", "date": "2026-10-29", "instruction": "x", "scope": "x", "source": "x"},
		"font": {"source_path": "SLPM_86.053", "source_sha256": "a".repeat(64), "glyph_bank_sha256": "b".repeat(64)},
		"containers": {"SLPM_86.053": {"sha256": container_sha, "size": 10}},
		"entries": {
			physical: {"text": "Hi", "segments": [["g", "82678289"]], "context_status": "ready_for_review",
				"container": "SLPM_86.053", "offset": 0x6f000, "raw_sha256": raw_sha, "raw_length": raw.size()},
			"talk:CDROM/X.BIN:main:g0000:l0000": {"text": "A[[T001:row]]B", "segments": [["g", "8260"], ["t", "20", "separator"], ["g", "8261"]], "context_status": "ready_for_review"},
			"c:UNIT:1": {"text": "z", "segments": [["g", "829a"]], "context_status": "ready_for_review"},
			"c:UNIT:2": {"text": "y", "segments": [["g", "8299"]], "context_status": "ready_for_review"},
			"c:UNIT:3": {"text": " ", "segments": [["g", "8140"]], "context_status": "ready_for_review"}}}
	var path := folder.path_join("pack.json")
	var sha := write_json(path, pack)
	var font := StubFont.new()
	var store := Store.new()
	check(store.open_pack(path, sha), "valid pack opens: " + store.error)
	check(store.entry_count() == 5 and store.language == "en", "all entries prepared")
	var found := store.lookup_source("SLPM_86.053", container_sha, 0x6f000, source, font)
	check(found.get("codes", PackedByteArray()) == PackedByteArray([0x82, 0x67, 0x82, 0x89]), "decode-time lookup returns original glyph codes")
	check(store.lookup_source("SLPM_86.053", "d".repeat(64), 0x6f000, source, font).is_empty() and store.last_rejection.contains("container"), "different container keeps the original")
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f002, source, font).is_empty(), "different offset keeps the original")
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f000, PackedByteArray([0x82, 0xa0]), font).is_empty(), "different source bytes keep the original")
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f000, source, null).is_empty(), "no font, no translation")
	var tokens := store.lookup_record("talk:CDROM/X.BIN:main:g0000:l0000", "", font)
	check(not tokens.is_empty() and not tokens.has("codes") and tokens.segments.size() == 3 and tokens.segments[1].raw == PackedByteArray([0x20]), "token records keep their source bytes and expose no flat codes")
	check(store.lookup_record("c:UNIT:1", "", font).is_empty() and store.last_rejection.contains("glyph"), "missing original glyph keeps the original")
	check(store.lookup_record("c:UNIT:2", "", font).is_empty(), "blank original glyph keeps the original")
	check(not store.lookup_record("c:UNIT:3", "", font).is_empty(), "full-width space is the accepted blank glyph")
	found.codes[0] = 0
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f000, source, font).codes[0] == 0x82, "callers cannot mutate the pack")
	# Layout: 3-texel glyphs advance 5 pixels (trim 4, width 3 - 1 + 3); space advances 4.
	var offsets := Layout.offsets(font, PackedByteArray([0x82, 0x60, 0x81, 0x40, 0x82, 0x61]), 14)
	check(offsets == PackedInt32Array([-4, 5, 5]), "proportional offsets: " + str(offsets))
	check(Layout.offsets(font, PackedByteArray([0x82, 0x60, 0x81, 0x40, 0x82, 0x61]), 13).is_empty(), "overwide run is rejected")
	check(Layout.offsets(font, PackedByteArray([0x82, 0x9a]), 100).is_empty(), "missing glyph is rejected by layout")
	# Negative opens: each clears the previous pack.
	check(not store.open_pack(path, "0".repeat(64)) and store.entry_count() == 0 and store.language.is_empty(), "wrong hash rejected and cleared")
	var bad := pack.duplicate(true)
	bad.entries[physical].raw_sha256 = "e".repeat(64)
	var bad_path := folder.path_join("bad-id.json")
	check(not store.open_pack(bad_path, write_json(bad_path, bad)), "entry whose id disagrees with its source hash is rejected")
	# The id binds only the first 12 hex digits of the source hash, so these
	# entries open and are found by id; only the full-hash and length guard in
	# lookup_source rejects them.
	var guard_path := folder.path_join("guard.json")
	bad = pack.duplicate(true)
	bad.entries[physical].raw_sha256 = raw_sha.left(12) + ("0" if raw_sha[12] != "0" else "1").repeat(52)
	check(store.open_pack(guard_path, write_json(guard_path, bad)), "entry sharing the id hash prefix opens: " + store.error)
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f000, source, font).is_empty() and store.last_rejection == "source bytes differ from the record identity", "full source hash mismatch keeps the original: " + store.last_rejection)
	bad = pack.duplicate(true)
	bad.entries[physical].raw_length = raw.size() + 1
	check(store.open_pack(guard_path, write_json(guard_path, bad)), "entry with a different source length opens: " + store.error)
	check(store.lookup_source("SLPM_86.053", container_sha, 0x6f000, source, font).is_empty() and store.last_rejection == "source bytes differ from the record identity", "source length mismatch keeps the original: " + store.last_rejection)
	bad = pack.duplicate(true)
	bad.entries["c:UNIT:1"].segments = [["g", "41"]]
	check(not store.open_pack(bad_path, write_json(bad_path, bad)), "single-byte glyph segment is rejected")
	bad = pack.duplicate(true)
	bad.entries["c:UNIT:1"]["container"] = "CDROM/UNLISTED.BIN"
	check(not store.open_pack(bad_path, write_json(bad_path, bad)), "unlisted container is rejected")
	bad = pack.duplicate(true)
	bad.erase("approval")
	check(not store.open_pack(bad_path, write_json(bad_path, bad)), "pack without approval is rejected")
	check(not store.open_pack("res://project.godot", FileAccess.get_sha256("res://project.godot")), "pack inside the project is rejected")
	# Service: failed selection keeps the active layer, empty path clears it.
	var packs := Packs.new()
	check(packs.select_text_layer(path, sha) and packs.text_layer != null, "service selects a valid layer")
	var active: RefCounted = packs.text_layer
	check(not packs.select_text_layer(path, "0".repeat(64)) and packs.text_layer == active and not packs.text_layer_error.is_empty(), "failed selection keeps the active layer")
	check(packs.select_text_layer("", "") and packs.text_layer == null, "empty selection restores original text")
	# Preferences: settings saved before T-39016 lack the layer keys and stay valid.
	var older: Dictionary = Preferences.defaults()
	older.language.erase("ui_text_manifest")
	older.language.erase("ui_text_sha256")
	var validated: Dictionary = Preferences.validate(older)
	check(not validated.is_empty() and validated.language.ui_text_manifest == "", "older settings load with original text")
	var chosen: Dictionary = Preferences.defaults()
	chosen.language.ui_text_manifest = path
	chosen.language.ui_text_sha256 = sha
	check(Preferences.validate(chosen).language.ui_text_sha256 == sha, "layer selection persists")
	chosen.language.ui_text_sha256 = 7
	check(Preferences.validate(chosen).is_empty(), "malformed layer preference is rejected")
	print("Text layer failures: ", failures)
	quit(0 if failures.is_empty() else 1)
