extends RefCounted
## T-29025: data-only installed paths; capabilities follow validated resources.

const ExternalPath = preload("res://services/external_content_path.gd")
const Rules = preload("res://rules/new_game.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const SCHEMA := "tokimemo.installed-runtime.v1"
const RELEASE := "original-ja-ps-the-best"
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const ENTRY_CAP := "new_game.entry"
const OPENING_RESOURCES_CAP := "new_game.opening_resources"
const GAMEPLAY_CAP := "new_game.gameplay"
const FILE_ROLES := ["name_entry_rules", "name_entry_content", "name_entry_media", "rule_tables", "hud_manifest", "hud_context", "holiday_scene", "holiday_text"]
const DIR_ROLES := ["font_root", "school_day_media", "spring_media", "weekday56_media", "club_media", "date_preview", "opening_pack", "geko_dialogue", "prologue_text", "first_arrival_media", "first_arrival_board", "prologue_hud", "opening_audio", "original_first_arrival_bgm"]
const ENTRY_ROLES := ["name_entry_rules", "name_entry_content", "font_root"]
const OPENING_RESOURCE_ROLES := ["prologue_text", "first_arrival_media", "prologue_hud", "opening_pack"]
const ROLE_MANIFESTS := {"prologue_text":"manifest.json","first_arrival_media":"manifest.json",
	"first_arrival_board":"manifest.json",
	"prologue_hud":"manifest.json","opening_audio":"opening-audio.json","opening_pack":"opening.json",
	"original_first_arrival_bgm":"source.json"}
const GAMEPLAY_ROLES := ["rule_tables", "school_day_media", "spring_media", "weekday56_media", "club_media", "hud_manifest", "hud_context", "holiday_scene", "holiday_text", "date_preview", "opening_pack"]
const MAX_FILES := 20000
const MAX_TOTAL_BYTES := 1024 * 1024 * 1024

static func read_pack(root: String, manifest: Dictionary) -> Dictionary:
	if not ExternalPath.is_external_path(root) or not DirAccess.dir_exists_absolute(root):
		return _failure("installed runtime must be an external directory without links")
	if not manifest.get("schema") is String or manifest.schema != SCHEMA or not _integer(manifest.get("version")) or manifest.version != 1 or not manifest.get("release_profile") is String or manifest.release_profile != RELEASE:
		return _failure("unsupported installed runtime profile")
	if not manifest.get("source") is Dictionary or not manifest.source.get("executable_sha256") is String or manifest.source.executable_sha256 != EXE_SHA:
		return _failure("installed runtime source release differs")
	if not manifest.get("files") is Dictionary or manifest.files.is_empty() or manifest.files.size() > MAX_FILES or not manifest.get("roles") is Dictionary:
		return _failure("installed runtime inventory is missing or outside bounds")
	var files: Dictionary = manifest.files
	var observed: Dictionary = {}
	var pending: Array[String] = [""]
	while not pending.is_empty():
		var prefix: String = pending.pop_back()
		if prefix.length() > 1024 or prefix.count("/") > 32:
			return _failure("installed runtime directory nesting exceeds bounds")
		var directory := DirAccess.open(root.path_join(prefix))
		if directory == null:
			return _failure("installed runtime directory is unreadable")
		directory.include_hidden = true
		for name: String in directory.get_files():
			var relative: String = name if prefix.is_empty() else prefix.path_join(name)
			if directory.is_link(name):
				return _failure("installed runtime contains a linked file")
			if relative != "manifest.json":
				observed[relative] = true
			if observed.size() > MAX_FILES:
				return _failure("installed runtime file count exceeds bounds")
		for name: String in directory.get_directories():
			if directory.is_link(name) or pending.size() >= MAX_FILES:
				return _failure("installed runtime contains linked or excessive directories")
			pending.append(name if prefix.is_empty() else prefix.path_join(name))
	if observed.size() != files.size():
		return _failure("installed runtime file inventory differs from disk")
	var total: int = 0
	for relative: Variant in files:
		if not relative is String or not safe_relative(relative) or relative == "manifest.json":
			return _failure("installed runtime inventory has an unsafe path")
		if not observed.has(relative):
			return _failure("installed runtime inventory names a missing file")
		var record: Variant = files[relative]
		if not record is Dictionary or record.size() != 2 or not _integer(record.get("size")) or not _sha(record.get("sha256")):
			return _failure("installed runtime file identity is malformed")
		total += int(record.size)
		if total > MAX_TOTAL_BYTES:
			return _failure("installed runtime exceeds its total byte bound")
		var path: String = root.path_join(relative)
		if not ExternalPath.is_external_file(path) or FileAccess.get_size(path) != int(record.size) or FileAccess.get_sha256(path) != record.sha256:
			return _failure("installed runtime file is missing, linked, or changed: " + relative)
	var paths: Dictionary = {}
	for role: Variant in manifest.roles:
		if not role is String or role not in FILE_ROLES + DIR_ROLES or not manifest.roles[role] is String or not safe_relative(manifest.roles[role]):
			return _failure("installed runtime has an unsupported role or locator")
		var relative: String = manifest.roles[role]
		if role in FILE_ROLES:
			if not files.has(relative):
				return _failure("installed runtime role lacks a file identity: " + role)
		else:
			var found := false
			for inventory_path: String in files:
				if inventory_path.begins_with(relative + "/"):
					found = true
					break
			if not found or not DirAccess.dir_exists_absolute(root.path_join(relative)):
				return _failure("installed runtime directory role lacks inventoried files: " + role)
		paths[role] = root.path_join(relative)
	var role_manifests: Dictionary = {}
	for role: String in ROLE_MANIFESTS:
		if not paths.has(role): continue
		var relative: String = manifest.roles[role].path_join(ROLE_MANIFESTS[role])
		if not files.has(relative): return _failure("installed opening role has no inventoried manifest: " + role)
		role_manifests[role] = {"relative":relative,"path":root.path_join(relative),
			"sha256":files[relative].sha256,"size":int(files[relative].size)}
	var missing_entry := missing_roles(paths, ENTRY_ROLES)
	var available: Array[String] = []
	if missing_entry.is_empty():
		var tables: Dictionary = Rules.load_tables(paths.name_entry_rules)
		var content: Dictionary = Entry.load_content(paths.name_entry_content)
		var glyphs := Glyphs.new()
		if tables.is_empty() or content.is_empty() or tables.get("space") != content.get("space"):
			return _failure("installed New Game table or entry payload failed authentication")
		var font_error: String = glyphs.open_external_font(paths.font_root)
		if not font_error.is_empty():
			return _failure("installed New Game font failed authentication: " + font_error)
		available.append(ENTRY_CAP)
	var missing_opening := missing_roles(paths,OPENING_RESOURCE_ROLES)
	if missing_entry.is_empty() and missing_opening.is_empty():
		# This describes inventoried resources. The launch owner still opens
		# every source reader before entry, and BGM/route readiness stays separate.
		available.append(OPENING_RESOURCES_CAP)
	# File inventory completeness cannot authenticate a playable route. Gameplay
	# stays unavailable until its readers and live opening are connected.
	var missing_gameplay := missing_roles(paths, GAMEPLAY_ROLES)
	return {"valid": true, "paths": paths, "available_capabilities": available,
		"role_manifests":role_manifests,"missing_opening_resource_roles":missing_opening,
		"installation_manifest_sha256":FileAccess.get_sha256(root.path_join("manifest.json")),
		"unavailable_capabilities": [GAMEPLAY_CAP],
		"missing_entry_roles": missing_entry, "missing_gameplay_roles": missing_gameplay,
		"coverage": manifest.get("coverage", {}).duplicate(true) if manifest.get("coverage") is Dictionary else {},
		"message": "", "profile_root": root}

static func missing_roles(paths: Dictionary, roles: Array) -> Array[String]:
	var missing: Array[String] = []
	for role: String in roles:
		if not paths.has(role):
			missing.append(role)
	return missing

static func safe_relative(path: String) -> bool:
	if path.is_empty() or path.begins_with("/") or path.contains("\\") or path.contains(":"):
		return false
	for component: String in path.split("/", true):
		if component in ["", ".", ".."]:
			return false
	return true

static func _integer(value: Variant) -> bool:
	return typeof(value) in [TYPE_INT, TYPE_FLOAT] and is_finite(float(value)) and float(value) == floor(float(value)) and value >= 0 and value <= MAX_TOTAL_BYTES

static func _sha(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true

static func _failure(message: String) -> Dictionary:
	return {"valid": false, "message": message}
