extends RefCounted
## External original content must stay outside source resources and the player.
## Only existing canonical POSIX files/directories are supported by this reader.

static func is_external_path(path: String) -> bool:
	var roots: PackedStringArray = _exclusion_roots(
		ProjectSettings.globalize_path("res://"), OS.get_executable_path())
	return _outside_roots(path, roots, OS.has_feature("macos"))

static func is_external_file(path: String) -> bool:
	return is_external_path(path) and FileAccess.file_exists(path)

static func _canonical(path: String) -> bool:
	return path.begins_with("/") and not path.begins_with("//") \
		and not path.contains("\\") and not path.contains(":") \
		and path.simplify_path() == path

static func _exclusion_roots(resource_root: String, executable: String) -> PackedStringArray:
	var roots := PackedStringArray()
	# A packed res:// filesystem has no host directory. Never turn its empty
	# globalized path into '/' or use the process working directory instead.
	if not resource_root.is_empty():
		var project: String = resource_root.trim_suffix("/") if resource_root != "/" else "/"
		if not _canonical(project) or not DirAccess.dir_exists_absolute(project) or not _unlinked(project):
			return PackedStringArray()
		roots.append(project)
	if not _canonical(executable) or not FileAccess.file_exists(executable) or not _unlinked(executable):
		return PackedStringArray()
	var container: String = executable.get_base_dir()
	# The pinned macOS export places its binary in App.app/Contents/MacOS.
	# Exclude the complete observed bundle, including Resources and siblings.
	if container.get_file() == "MacOS" and container.get_base_dir().get_file() == "Contents":
		var bundle: String = container.get_base_dir().get_base_dir()
		if bundle.get_file().ends_with(".app"):
			container = bundle
	if not _canonical(container) or not DirAccess.dir_exists_absolute(container):
		return PackedStringArray()
	roots.append(container)
	return roots

static func _outside_roots(path: String, roots: PackedStringArray, fold_case: bool) -> bool:
	if roots.is_empty() or not _canonical(path):
		return false
	var compared: String = path.to_lower() if fold_case else path
	for root: String in roots:
		if not _canonical(root):
			return false
		var excluded: String = root.to_lower() if fold_case else root
		if excluded == "/" or compared == excluded or compared.begins_with(excluded + "/"):
			return false
	if not FileAccess.file_exists(path) and not DirAccess.dir_exists_absolute(path):
		return false
	return _unlinked(path)

static func _unlinked(path: String) -> bool:
	var directory: DirAccess = DirAccess.open("/")
	if directory == null:
		return false
	var current: String = ""
	for component: String in path.split("/", false):
		if component == "." or component == "..":
			return false
		current += "/" + component
		if directory.is_link(current):
			return false
	return true
