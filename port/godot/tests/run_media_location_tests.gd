extends SceneTree
## Genuine external v1/v2/v3 packs must never be admitted from inside res://.
const Store = preload("res://services/school_day_media_store.gd")
var failures: int = 0

func _initialize() -> void:
	# This regression temporarily copies genuine original payload into a private
	# disposable project; never run it against the canonical source checkout.
	if OS.get_environment("TOKIMEMO_LOCATION_DISPOSABLE_PROJECT") != "1":
		push_error("location regression requires an explicitly disposable project")
		quit(2)
		return
	var project: String = ProjectSettings.globalize_path("res://").simplify_path().trim_suffix("/")
	var report: Dictionary = {"project": project, "versions": []}
	var nonce: String = "%d-%d" % [OS.get_process_id(), Time.get_ticks_usec()]
	var fixture_root: String = project.path_join(".media-location-" + nonce)
	if DirAccess.make_dir_absolute(fixture_root) != OK:
		push_error("cannot reserve a unique project-local fixture directory")
		quit(2)
		return
	var sources: Array[String] = []
	for version: int in [1, 2, 3]:
		var source: String = OS.get_environment("TOKIMEMO_MEDIA_V%d" % version)
		sources.append(source)
		var store = Store.new()
		var external_ok: bool = store.open_pack(source)
		_check(external_ok, "genuine external v%d pack opens: %s" % [version, store.error_message()])
		if not external_ok:
			_remove_tree(fixture_root)
			quit(1)
			return
		var target: String = fixture_root.path_join("v%d" % version)
		_check(_copy_tree(source, target), "copy genuine project-local v%d fixture" % version)
		_check(FileAccess.get_sha256(source.path_join("manifest.json")) == FileAccess.get_sha256(target.path_join("manifest.json")), "project-local fixture retains exact original manifest")
		var absolute_accepted: bool = store.open_pack(target)
		var absolute_error: String = store.error_message()
		_check(not absolute_accepted and not absolute_error.is_empty(), "absolute project-local v%d pack rejects before admission" % version)
		_check(_empty(store), "failed project-local open clears all v%d content" % version)
		var resource_accepted: bool = store.open_pack(ProjectSettings.localize_path(target))
		_check(not resource_accepted and _empty(store), "res spelling of genuine v%d fixture rejects" % version)
		_check(store.open_pack(source), "external v%d pack can reopen after rejection" % version)
		report["versions"].append({"version": version, "manifest_sha256": FileAccess.get_sha256(source.path_join("manifest.json")),
			"external_opens": external_ok, "absolute_project_local_accepted": absolute_accepted,
			"absolute_error": absolute_error, "resource_spelling_accepted": resource_accepted})
		_remove_tree(target)
	_remove_tree(fixture_root)
	# Test the project root itself using a genuine v3 manifest and referenced assets.
	if FileAccess.file_exists(project.path_join("manifest.json")) or DirAccess.dir_exists_absolute(project.path_join("assets")):
		_check(false, "project root fixture destinations must be unused")
		report["project_root_fixture_skipped"] = true
	else:
		_check(DirAccess.copy_absolute(sources[2].path_join("manifest.json"), project.path_join("manifest.json")) == OK, "copy project-root v3 manifest")
		_check(_copy_tree(sources[2].path_join("assets"), project.path_join("assets")), "copy project-root v3 payloads")
		var root_store = Store.new()
		var root_accepted: bool = root_store.open_pack(project)
		_check(not root_accepted and _empty(root_store), "absolute project root containing genuine v3 pack rejects")
		report["project_root_accepted"] = root_accepted
		DirAccess.remove_absolute(project.path_join("manifest.json"))
		_remove_tree(project.path_join("assets"))
	# A sibling with a matching name prefix is still outside the project.
	var sibling: String = project + "-external-location-" + nonce
	if DirAccess.make_dir_absolute(sibling) != OK:
		_check(false, "external sibling fixture destination must be unused")
		report["external_sibling_fixture_skipped"] = true
	else:
		_check(_copy_tree(sources[2], sibling), "copy external sibling fixture")
		var sibling_store = Store.new()
		var sibling_ok: bool = sibling_store.open_pack(sibling)
		_check(sibling_ok, "external sibling with project-name prefix remains valid")
		report["external_sibling_opens"] = sibling_ok
		_remove_tree(sibling)
	report["failures"] = failures
	var output: String = OS.get_environment("TOKIMEMO_LOCATION_PROOF")
	if not output.is_empty():
		var file := FileAccess.open(output, FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(report, "\t", true))
			file.close()
	print("LOCATION_PROOF: " + JSON.stringify(report))
	if failures == 0:
		print("PASS: genuine v1/v2/v3 external packs accepted; absolute project roots/subtrees and res spellings rejected; stale content cleared")
	quit(0 if failures == 0 else 1)

func _empty(store: RefCounted) -> bool:
	return store.get_background().is_empty() and store.get_background("command_7_background").is_empty() and store.get_background("holiday_room_background").is_empty() and store.get_frame("command_0_sd", 0).is_empty() and store.get_frame("holiday_command_0_sd", 0).is_empty() and store.get_sequence("command_0_sd", 0).is_empty() and store.get_route_tables().is_empty()

func _check(condition: bool, label: String) -> void:
	if not condition:
		failures += 1
		push_error("FAIL: " + label)

func _copy_tree(source: String, target: String) -> bool:
	if DirAccess.make_dir_recursive_absolute(target) != OK:
		return false
	var dir := DirAccess.open(source)
	if dir == null:
		return false
	for name in dir.get_files():
		if DirAccess.copy_absolute(source.path_join(name), target.path_join(name)) != OK:
			return false
	for name in dir.get_directories():
		if not _copy_tree(source.path_join(name), target.path_join(name)):
			return false
	return true

func _remove_tree(path: String) -> void:
	var dir := DirAccess.open(path)
	if dir == null:
		return
	for name in dir.get_files():
		DirAccess.remove_absolute(path.path_join(name))
	for name in dir.get_directories():
		_remove_tree(path.path_join(name))
	DirAccess.remove_absolute(path)
