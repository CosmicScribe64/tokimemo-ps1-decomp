extends SceneTree
## Authenticated originals copied into an ignored disposable project are refused.
const Presenter = preload("res://presentation/command_hud_presenter.gd")
var checks: Array[String] = []
var failures: Array[String] = []

func check(ok: bool, label: String) -> void:
	checks.append(label)
	if not ok:
		failures.append(label)

func _initialize() -> void:
	var root: String = ProjectSettings.globalize_path("res://").trim_suffix("/")
	var manifest: String = OS.get_environment("HUD_MANIFEST")
	var digest: String = OS.get_environment("HUD_MANIFEST_SHA256")
	var font: String = OS.get_environment("HUD_FONT_ROOT")
	var presenter: Presenter = Presenter.new()
	check(presenter.open_manifest(manifest, digest, font), "reviewed genuine external packet opens")
	check(not Presenter._safe_path(root) and not Presenter._safe_path(root + "/"), "absolute active project root is refused")
	check(not Presenter._safe_path(root.path_join("inside-hud/manifest.json")), "absolute active project descendant is refused")
	check(not presenter.open_manifest(root.path_join("inside-hud/manifest.json"), digest, font), "authenticated original packet inside project is refused")
	check(presenter._manifest.is_empty(), "failed project reopen clears the previous capability")
	check(Presenter._safe_path(root + "-sibling/manifest.json"), "external sibling sharing the project name prefix remains valid")
	check(presenter.open_manifest(root + "-sibling/manifest.json", digest, font), "authenticated packet in genuine external sibling opens")
	check(not Presenter._safe_path("res://inside-hud/manifest.json"), "resource URI remains refused")
	check(not Presenter._safe_path(root + "/../project-sibling/manifest.json"), "parent traversal remains refused")
	check(not presenter.open_manifest(manifest, digest, root.path_join("fonts")), "font root cannot enter active project through an absolute path")
	var report: Dictionary = {"checks": checks, "failures": failures, "project": root}
	var output: FileAccess = FileAccess.open(OS.get_environment("HUD_PATH_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify(report, "\t"))
	output.close()
	for failure: String in failures:
		push_error(failure)
	print("External path checks: %d, failures: %d" % [checks.size(), failures.size()])
	quit(0 if failures.is_empty() else 1)
