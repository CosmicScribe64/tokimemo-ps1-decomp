extends RefCounted
## Code-pinned original-file role loader; never accepts executable pack code.
const Backend = preload("res://bgm_backend.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const MANIFEST_SHA256: String = "f05ba31af09abd62eaebf252f99c860d99eabb1395068d49850ced4097798191"
const FILES: Array[String] = ["executable.bin", "SYS_MIX.VH", "SYS_MIX.VB", "PRO_VOL.VH", "PRO_VOL.VB", "PRO_VOL.SEQ"]
const KEYS: Array[String] = ["EXE_sha256", "SE_header_sha256", "SE_body_sha256", "music_header_sha256", "music_body_sha256", "sequence_sha256"]
const SIZES: Array[int] = [667648, 3104, 35376, 9760, 160288, 27624]

static func sha(data: PackedByteArray) -> String:
	var context: HashingContext = HashingContext.new()
	if context.start(HashingContext.HASH_SHA256) != OK or context.update(data) != OK:
		return ""
	return context.finish().hex_encode()

static func linked(path: String) -> bool:
	var current: String = path
	while current != current.get_base_dir():
		var parent: DirAccess = DirAccess.open(current.get_base_dir())
		if parent == null or parent.is_link(current.get_file()):
			return true
		current = current.get_base_dir()
	return false

static func read_pack(root: String) -> Dictionary:
	if not ExternalPath.is_external_path(root) or not DirAccess.dir_exists_absolute(root):
		return {"error": "original BGM role requires normalized external directory without links"}
	var manifest_path: String = root.path_join("source.json")
	if not FileAccess.file_exists(manifest_path) or linked(manifest_path):
		return {"error": "first-arrival source manifest is missing or linked"}
	var manifest_file: FileAccess = FileAccess.open(manifest_path, FileAccess.READ)
	if manifest_file == null or manifest_file.get_length() > 4096:
		return {"error": "first-arrival source manifest exceeds its bound"}
	var manifest: PackedByteArray = manifest_file.get_buffer(manifest_file.get_length())
	if sha(manifest) != MANIFEST_SHA256:
		return {"error": "first-arrival source manifest differs from code-owned pin"}
	var files: Array[PackedByteArray] = []
	for index: int in range(FILES.size()):
		var path: String = root.path_join(FILES[index])
		if not FileAccess.file_exists(path) or linked(path):
			return {"error": "original BGM source is missing or linked: " + FILES[index]}
		var file: FileAccess = FileAccess.open(path, FileAccess.READ)
		if file == null or file.get_length() != SIZES[index]:
			return {"error": "original BGM source size differs: " + FILES[index]}
		var bytes: PackedByteArray = file.get_buffer(SIZES[index])
		if sha(bytes) != Backend.ORIGINAL_IDENTITIES[KEYS[index]]:
			return {"error": "original BGM source identity differs: " + FILES[index]}
		files.append(bytes)
	return {"error": "", "files": files, "identities": Backend.ORIGINAL_IDENTITIES.duplicate(true)}
