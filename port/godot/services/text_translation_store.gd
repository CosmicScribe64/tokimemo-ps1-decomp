extends RefCounted
const SCHEMA := "tokimemo.text-translation-pack.v1"
const NativeText = preload("res://presentation/prologue_text_presenter.gd")
var unsupported_entries: Array[String] = []
var entries: Dictionary = {}
var source_sha := ""
var language := "ja"
var manifest := ""
var digest := ""
var error := ""
func open_pack(path: String, expected: String) -> bool:
	if not FileAccess.file_exists(path) or FileAccess.get_size(path) > 4194304 or FileAccess.get_sha256(path) != expected:
		error = "Text translation manifest is missing or changed."; return false
	var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not value is Dictionary or value.get("schema") != SCHEMA or not value.get("language") is String or value.language.is_empty() or value.language.length() > 32 or not value.get("source_payload_sha256") is String or value.source_payload_sha256.length() != 64 or not value.source_payload_sha256.is_valid_hex_number() or not value.get("entries") is Dictionary or value.entries.size() > 4096:
		error = "Text translation schema is invalid."; return false
	var prepared := {}
	var unsupported: Array[String] = []
	for key: Variant in value.entries:
		var entry: Variant = value.entries[key]
		if not key is String or not key.is_valid_int() or int(key) < 0 or not entry is Dictionary or not entry.get("speaker") is String or not entry.get("text") is String or entry.speaker.length() > 256 or entry.text.is_empty() or entry.text.length() > 8192:
			error = "Text translation entry is invalid."; return false
		if not NativeText.translation_supported(entry):
			unsupported.append(key)
		else: prepared[key] = {"speaker": entry.speaker, "text": entry.text}
	if prepared.is_empty(): error = "No translated lines use the supported original glyphs."; return false
	entries = prepared; language = value.language; source_sha = value.source_payload_sha256
	unsupported_entries = unsupported
	manifest = path; digest = expected; error = ""; return true
func replacement(payload_sha: String, line: int) -> Dictionary:
	if payload_sha != source_sha: return {}
	return entries.get(str(line), {}).duplicate()
