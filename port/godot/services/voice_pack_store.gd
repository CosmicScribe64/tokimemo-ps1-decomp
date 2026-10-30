extends RefCounted
## Custom voice never replaces original event authentication or source routing.
const SCHEMA := "tokimemo.voice-pack.v1"
const FIELDS := ["line_id", "clip_index", "speaker", "start_lba", "end_lba", "file_number", "channel"]
var clips: Dictionary = {}
var language := "ja"
var error := ""
var manifest := ""
var digest := ""

static func identity(value: Dictionary) -> String:
	var key: String = str(value.get("source_sha256", ""))
	for field: String in FIELDS: key += ":" + str(int(value.get(field, -1)))
	return key

func open_pack(path: String, expected: String) -> bool:
	if not FileAccess.file_exists(path) or FileAccess.get_size(path) > 4194304 or FileAccess.get_sha256(path) != expected:
		error = "Voice manifest is missing or changed."; return false
	var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not value is Dictionary or value.get("schema") != SCHEMA or not value.get("language") is String or value.language.is_empty() or value.language.length() > 32 or not value.get("clips") is Array or value.clips.size() > 4096:
		error = "Voice pack schema is invalid."; return false
	var prepared := {}
	for entry: Variant in value.clips:
		if not entry is Dictionary or not entry.get("path") is String or not entry.get("sha256") is String or not entry.get("source_sha256") is String or entry.source_sha256.length() != 64:
			error = "Voice clip identity is incomplete."; return false
		for field: String in FIELDS:
			var number: Variant = entry.get(field)
			if not (number is int or number is float) or not is_finite(float(number)) or number != floor(number) or number < 0 or number > 0x7fffffff:
				error = "Voice clip source address is invalid."; return false
		var relative: String = entry.path
		if relative.is_empty() or relative.is_absolute_path() or relative.contains(":") or relative.contains("\\") or ".." in relative.split("/") or relative.get_extension().to_lower() != "wav":
			error = "Voice clip path must stay inside its pack."; return false
		var full := path.get_base_dir().path_join(relative)
		var current := path.get_base_dir()
		for component: String in relative.split("/"):
			var directory := DirAccess.open(current)
			if directory == null or directory.is_link(component): error = "Voice pack links are unsupported."; return false
			current = current.path_join(component)
		if not FileAccess.file_exists(full) or FileAccess.get_size(full) > 67108864 or FileAccess.get_sha256(full) != entry.sha256:
			error = "Voice clip is missing or changed."; return false
		var stream := AudioStreamWAV.load_from_file(full)
		if stream == null or stream.format != AudioStreamWAV.FORMAT_16_BITS or stream.mix_rate < 8000 or stream.mix_rate > 192000 or stream.get_length() <= 0 or stream.get_length() > 300:
			error = "Voice clips must be bounded 16-bit PCM WAV files."; return false
		var key := identity(entry)
		if prepared.has(key): error = "Voice pack repeats a source clip."; return false
		prepared[key] = {"path": full, "sha256": entry.sha256}
	clips = prepared; language = value.language; manifest = path; digest = expected; error = ""; return true

func replacement(event: Dictionary) -> AudioStreamWAV:
	var request: Variant = event.get("xa_request")
	if not request is Dictionary: return null
	var key_fields: Dictionary = request.duplicate()
	for field: String in ["line_id", "clip_index", "speaker"]: key_fields[field] = event.get(field, -1)
	var key := identity(key_fields)
	if not clips.has(key): return null
	var clip: Dictionary = clips[key]
	if FileAccess.get_sha256(clip.path) != clip.sha256: return null
	return AudioStreamWAV.load_from_file(clip.path)
