extends RefCounted
const SCHEMA := "tokimemo.text-translation-pack.v1"
const NativeText = preload("res://presentation/prologue_text_presenter.gd")
var unsupported_entries: Array[String] = []
## Approved entries the native glyph set cannot draw, keyed like `entries`.
## replacement() never returns them; replacement_any() does, for window-space
## text (the modern text box) that does not use the original font.
var unsupported_text: Dictionary = {}
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
	var unsupported_rows := {}
	for key: Variant in value.entries:
		var entry: Variant = value.entries[key]
		if not key is String or not key.is_valid_int() or int(key) < 0 or not entry is Dictionary or not entry.get("speaker") is String or not entry.get("text") is String or entry.speaker.length() > 256 or entry.text.is_empty() or entry.text.length() > 8192:
			error = "Text translation entry is invalid."; return false
		var row := {"speaker": entry.speaker, "text": opened_parenthesis(entry.text)}
		if not NativeText.translation_supported(row):
			unsupported.append(key); unsupported_rows[key] = row
		else: prepared[key] = row
	if prepared.is_empty(): error = "No translated lines use the supported original glyphs."; return false
	entries = prepared; language = value.language; source_sha = value.source_payload_sha256
	unsupported_entries = unsupported; unsupported_text = unsupported_rows
	manifest = path; digest = expected; error = ""; return true
## The source can draw an opening parenthesis as a decoration piece before the
## text (for example a prologue monologue line, prologue_text_store
## "parenthesis_open" for speaker 31), so a translation of that text can close
## a parenthesis it never opened. Such a text gets the opening parenthesis
## back; any other text is unchanged.
static func opened_parenthesis(text: String) -> String:
	if text.count(")") > text.count("(") and not text.begins_with("("):
		return "(" + text
	return text

func replacement(payload_sha: String, line: int) -> Dictionary:
	if payload_sha != source_sha: return {}
	return entries.get(str(line), {}).duplicate()
## Any approved entry for the line, including ones the native glyph set lacks.
func replacement_any(payload_sha: String, line: int) -> Dictionary:
	if payload_sha != source_sha: return {}
	var key := str(line)
	return entries.get(key, unsupported_text.get(key, {})).duplicate()
