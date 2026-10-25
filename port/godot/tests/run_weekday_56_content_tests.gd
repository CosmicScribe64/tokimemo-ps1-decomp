extends SceneTree
## Optional original-pack regression. Run only in a disposable copied project.

const Store = preload("res://services/weekday_56_content_store.gd")
var _failures := 0
var _checks := 0

func _initialize() -> void:
	var pack := OS.get_environment("T29014_WEEKDAY56_PACK")
	if OS.get_environment("TOKIMEMO_LOCATION_DISPOSABLE_PROJECT") != "1" or pack.is_empty():
		push_error("weekday56 regression needs a disposable project and an explicit external pack")
		quit(2)
		return
	var store = Store.new()
	if not _check(store.open_pack(pack), "original exact pack: " + store.error_message()):
		quit(1)
		return
	for command in [5, 6]:
		var component: Dictionary = store.get_command(command)
		_check(component["route"]["command"] == command, "command identity")
		_check(component["route"]["selector_index"] == (10 if command == 5 else 12), "native distinct selector offset")
		_check(component["sd"]["profile_index"] == (24 if command == 5 else 20), "selected original SD layout")
		_check(component["sd"]["frames"].size() == (12 if command == 5 else 15), "all physical SD frames")
		_check(component["sd"]["sequences"][0]["sequence_index"] == 0, "source sequence0 is present")
		_check(Vector2i(int(component["background"]["active_size"][0]), int(component["background"]["active_size"][1])) == Vector2i(320, 88), "active background size")
		_check(Vector2i(int(component["background"]["padded_size"][0]), int(component["background"]["padded_size"][1])) == Vector2i(320, 96), "padded background size")
		var bg: Image = store.get_background(command)
		_check(bg != null and bg.get_size() == Vector2i(320, 88), "active original image")
		for frame in component["sd"]["frames"]:
			var loaded: Dictionary = store.get_sd_frame(command, int(frame["frame_index"]))
			_check(not loaded.is_empty() and loaded.get("image") != null and loaded.get("stp_image") != null, "original physical frame and STP")
		component["sd"]["frames"].clear()
		_check(not store.get_command(command)["sd"]["frames"].is_empty(), "getter owns copied metadata")
		_check(store.get_sd_frame(command, 9999).is_empty(), "unknown frame has no fallback")
	for values in [[4,4,15,0,0,0], [5,6,15,0,0,0], [5,4,0,0,0,0], [5,4,15,1,0,0], [5,4,15,0,1,0], [5,4,15,0,0,1]]:
		_check(store.get_command(values[0],values[1],values[2],values[3],values[4],values[5]).is_empty() and not store.error_message().is_empty(), "unsupported domain fails explicitly")
	_check(store.get_background(5, 6) == null, "background getter rejects unavailable season")
	_check(store.get_sd_frame(5, 0, 4, 15, 1).is_empty(), "SD getter rejects unavailable scene")
	for month in [3,4,5]:
		_check(not store.get_command(5, month).is_empty(), "supported spring month")
	var project := ProjectSettings.globalize_path("res://").simplify_path().trim_suffix("/")
	var unique := "weekday56-%d-%d" % [OS.get_process_id(), Time.get_ticks_usec()]
	var scratch := "/tmp/" + unique
	if not _check(DirAccess.make_dir_absolute(scratch) == OK, "reserve exclusive scratch"):
		quit(1)
		return
	var project_child := project.path_join(unique)
	if not _check(DirAccess.make_dir_absolute(project_child) == OK, "reserve exclusive project fixture"):
		_remove_tree(scratch)
		quit(1)
		return
	_check(_copy_tree(pack, project_child), "copy genuine project-local fixture")
	for path in ["res://" + unique, project_child, project, "//" + pack.trim_prefix("/"), pack + "/../" + pack.get_file()]:
		_check(not store.open_pack(path) and store.get_command(5).is_empty(), "reject unsafe/project path and clear getters")
	# The active project root receives only newly owned files, never existing ones.
	var root_assets := project.path_join("assets")
	var root_manifest := project.path_join("manifest.json")
	if _check(not DirAccess.dir_exists_absolute(root_assets) and not FileAccess.file_exists(root_manifest), "project root fixture names unused"):
		_check(_copy_tree(pack.path_join("assets"), root_assets), "copy genuine root assets")
		_check(DirAccess.copy_absolute(pack.path_join("manifest.json"), root_manifest) == OK, "copy genuine root manifest")
		_check(not store.open_pack(project), "reject genuine pack at active project root")
		_remove_tree(root_assets)
		DirAccess.remove_absolute(root_manifest)
	var copy := scratch.path_join("pack")
	_check(_copy_tree(pack, copy), "copy writable external genuine pack")
	_check(store.open_pack(copy), "external genuine copy accepted")
	var bg_name: String = store.get_command(5)["background"]["image"]["path"]
	var bg_path := copy.path_join("assets").path_join(bg_name)
	var original_bg := FileAccess.get_file_as_bytes(bg_path)
	var corrupted := original_bg.duplicate()
	corrupted[corrupted.size() - 1] ^= 1
	_write(bg_path, corrupted)
	_check(store.get_background(5) == null and store.get_command(6).is_empty(), "post-open corruption clears all content")
	_check(not store.open_pack(copy), "PNG corruption fails on open")
	_write(bg_path, original_bg)
	var manifest_path := copy.path_join("manifest.json")
	var original_manifest := FileAccess.get_file_as_bytes(manifest_path)
	var changed: Dictionary = JSON.parse_string(original_manifest.get_string_from_utf8())
	changed["components"]["command_5"]["sd"]["actor_contract"]["anchor"] = [0, 0]
	_write(manifest_path, JSON.stringify(changed).to_utf8_buffer())
	_check(not store.open_pack(copy) and store.error_message().contains("reviewed exact"), "coherent metadata mutation rejected")
	_write(manifest_path, original_manifest)
	var parent := DirAccess.open(scratch)
	var linked := scratch.path_join("linked")
	_check(parent.create_link(copy, linked) == OK, "create root symlink")
	_check(not store.open_pack(linked), "root symlink rejected")
	DirAccess.remove_absolute(linked)
	DirAccess.remove_absolute(bg_path)
	_check(DirAccess.open(bg_path.get_base_dir()).create_link(pack.path_join("assets").path_join(bg_name), bg_path) == OK, "create PNG symlink")
	_check(not store.open_pack(copy), "PNG symlink rejected")
	DirAccess.remove_absolute(bg_path)
	_write(bg_path, original_bg)
	DirAccess.remove_absolute(manifest_path)
	_check(parent.create_link(pack.path_join("manifest.json"), manifest_path) == OK, "create manifest symlink")
	_check(not store.open_pack(copy), "manifest symlink rejected")
	DirAccess.remove_absolute(manifest_path)
	_write(manifest_path, original_manifest)
	_check(store.open_pack(copy), "restored external pack accepted")
	_check(not store.open_pack(scratch.path_join("missing")) and store.get_command(5).is_empty(), "failed reopen clears prior content")
	_remove_tree(project_child)
	_remove_tree(scratch)
	print("weekday56: %d checks; %d failures" % [_checks, _failures])
	quit(0 if _failures == 0 else 1)

func _check(condition: bool, label: String) -> bool:
	_checks += 1
	if not condition:
		_failures += 1
		push_error(label)
	return condition

func _write(path: String, raw: PackedByteArray) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE)
	if _check(file != null, "open fixture file"):
		file.store_buffer(raw)
		file.close()

func _copy_tree(source: String, target: String) -> bool:
	if DirAccess.make_dir_recursive_absolute(target) != OK:
		return false
	var directory := DirAccess.open(source)
	if directory == null:
		return false
	for name in directory.get_files():
		if directory.is_link(name) or DirAccess.copy_absolute(source.path_join(name), target.path_join(name)) != OK:
			return false
	for name in directory.get_directories():
		if directory.is_link(name) or not _copy_tree(source.path_join(name), target.path_join(name)):
			return false
	return true

func _remove_tree(path: String) -> void:
	var directory := DirAccess.open(path)
	if directory == null:
		return
	for name in directory.get_files():
		DirAccess.remove_absolute(path.path_join(name))
	for name in directory.get_directories():
		var child := path.path_join(name)
		if directory.is_link(name):
			DirAccess.remove_absolute(child)
		else:
			_remove_tree(child)
	DirAccess.remove_absolute(path)
