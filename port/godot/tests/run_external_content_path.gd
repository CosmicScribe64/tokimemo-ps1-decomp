extends SceneTree
## Invented filesystem fixtures for source and packaged-player exclusions.
const PathGuard = preload("res://services/external_content_path.gd")
var checks: Dictionary = {}

func check(ok: bool, label: String) -> void:
	checks[label] = ok
	if not ok:
		printerr("FAIL: " + label)

func write(path: String) -> void:
	DirAccess.make_dir_recursive_absolute(path.get_base_dir())
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	file.store_string("invented data\n")
	file.close()

func _initialize() -> void:
	var base: String = "/tmp/external-content-path-tests"
	var project: String = base.path_join("project")
	var engine: String = base.path_join("engine/Godot")
	var external: String = base.path_join("original/manifest.json")
	var sibling: String = project + "-sibling/manifest.json"
	var bundle: String = base.path_join("Game.app")
	var executable: String = bundle.path_join("Contents/MacOS/Game")
	var bundled_payload: String = bundle.path_join("Contents/Resources/original.json")
	for path: String in [project.path_join("inside.json"), engine, external, sibling, executable, bundled_payload]:
		write(path)
	var source_roots: PackedStringArray = PathGuard._exclusion_roots(project + "/", engine)
	var packed_roots: PackedStringArray = PathGuard._exclusion_roots("", executable)
	check(source_roots == PackedStringArray([project, engine.get_base_dir()]), "source context excludes project and engine directory")
	check(packed_roots == PackedStringArray([bundle]), "virtual resources exclude complete actual app-shaped container")
	check(PathGuard._outside_roots(external, source_roots, false), "source mode accepts existing external manifest")
	check(PathGuard._outside_roots(external, packed_roots, false), "empty resource mode accepts existing external manifest")
	check(not PathGuard._outside_roots(project.path_join("inside.json"), source_roots, false), "absolute active project file refused")
	check(not PathGuard._outside_roots(project, source_roots, false), "active project directory refused")
	check(PathGuard._outside_roots(sibling, source_roots, false), "genuine sibling sharing project prefix accepted")
	check(not PathGuard._outside_roots(engine, source_roots, false), "engine executable container refused")
	check(not PathGuard._outside_roots(bundled_payload, packed_roots, false), "packaged Resources payload refused")
	check(not PathGuard._outside_roots(executable, packed_roots, false), "packaged executable refused")
	check(not PathGuard._outside_roots(bundle, packed_roots, false), "whole packaged bundle refused")
	check(not PathGuard._outside_roots(bundled_payload.to_upper(), packed_roots, true), "macOS conservative case-folded bundle exclusion")
	for path: String in ["relative.json", "res://original.json", "/" + external,
		base + "/original//manifest.json", base + "/original/./manifest.json",
		base + "/original/../original/manifest.json", base + "/original\\manifest.json",
		base + "/original:manifest.json", base + "/missing.json"]:
		check(not PathGuard._outside_roots(path, packed_roots, false), "reject unsafe or absent path: " + path)
	var disk: DirAccess = DirAccess.open("/")
	var leaf_link: String = base.path_join("linked.json")
	var parent_link: String = base.path_join("linked-parent")
	check(disk.create_link(external, leaf_link) == OK, "create invented leaf symlink")
	check(not PathGuard._outside_roots(leaf_link, packed_roots, false), "leaf symlink rejected")
	check(disk.create_link(external.get_base_dir(), parent_link) == OK, "create invented directory symlink")
	check(not PathGuard._outside_roots(parent_link.path_join("manifest.json"), packed_roots, false), "parent symlink rejected")
	check(PathGuard._exclusion_roots("", "").is_empty(), "missing filesystem context fails closed")
	check(PathGuard._exclusion_roots("relative", engine).is_empty(), "invalid nonempty resource context fails closed")
	check(PathGuard._exclusion_roots("", leaf_link).is_empty(), "symlinked executable context fails closed")
	check(not PathGuard._outside_roots(external, PackedStringArray(), false), "empty exclusion capability never grants access")
	check(not PathGuard._outside_roots(external, PackedStringArray(["/"]), false), "actual root project excludes the full filesystem")
	check(PathGuard.is_external_file(external), "public runtime file API accepts invented external file")
	check(PathGuard.is_external_path(external.get_base_dir()), "public runtime path API accepts external directory")
	check(not PathGuard.is_external_file(external.get_base_dir()), "file API refuses directory")
	check(not PathGuard.is_external_path(OS.get_executable_path()), "public runtime API refuses its actual executable")
	var failures: Array[String] = []
	for label: String in checks:
		if not checks[label]:
			failures.append(label)
	var report: Dictionary = {"checks": checks, "failures": failures,
		"resource_root": ProjectSettings.globalize_path("res://"), "executable": OS.get_executable_path()}
	var output: String = OS.get_environment("TOKIMEMO_EXTERNAL_PATH_PROOF")
	if not output.is_empty():
		var file: FileAccess = FileAccess.open(output, FileAccess.WRITE)
		file.store_string(JSON.stringify(report, "\t"))
		file.close()
	print("External content paths: %d checks, %d failures" % [checks.size(), failures.size()])
	quit(0 if failures.is_empty() else 1)
