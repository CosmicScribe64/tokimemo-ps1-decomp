extends SceneTree
## Genuine authenticated packet under the active project, using POSIX aliases.
const Presenter = preload("res://presentation/command_hud_presenter.gd")

func _initialize() -> void:
	var vulnerable: bool = OS.get_environment("HUD_EXPECT_ALIAS") == "1"
	var root: String = ProjectSettings.globalize_path("res://").trim_suffix("/")
	var manifest: String = OS.get_environment("HUD_MANIFEST")
	var digest: String = OS.get_environment("HUD_MANIFEST_SHA256")
	var font: String = OS.get_environment("HUD_FONT_ROOT")
	var presenter: Presenter = Presenter.new()
	var checks: Dictionary = {}
	var observed: Dictionary = {}
	checks["genuine external packet opens"] = presenter.open_manifest(manifest, digest, font)
	checks["normal absolute inside-project packet refuses"] = not presenter.open_manifest(root.path_join("inside-hud/manifest.json"), digest, font)
	for prefix: String in ["//", "///"]:
		var alias: String = prefix + root.trim_prefix("/") + "/inside-hud/manifest.json"
		var allowed: bool = Presenter._safe_path(alias)
		var opened: bool = presenter.open_manifest(alias, digest, font)
		observed[prefix] = {"path": alias, "safe_path": allowed, "authenticated_pack_opens": opened}
		checks["path alias disposition " + prefix] = allowed == vulnerable
		checks["genuine pack alias disposition " + prefix] = opened == vulnerable
	checks["failed alias reopen clears capability"] = vulnerable or presenter._manifest.is_empty()
	checks["genuine external sibling still opens"] = presenter.open_manifest(root + "-sibling/manifest.json", digest, font)
	checks["external double-slash spelling disposition"] = Presenter._safe_path("/" + manifest) == vulnerable
	var failures: Array[String] = []
	for label: String in checks:
		if not checks[label]:
			failures.append(label)
	var report: Dictionary = {"expected_vulnerable_baseline": vulnerable, "checks": checks,
		"failures": failures, "observed": observed,
		"presenter_sha256": FileAccess.get_sha256(root.path_join("presentation/command_hud_presenter.gd"))}
	var output: FileAccess = FileAccess.open(OS.get_environment("HUD_PATH_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify(report, "\t"))
	output.close()
	print("Alias checks: %d, failures: %d, vulnerable baseline: %s" % [checks.size(), failures.size(), vulnerable])
	for failure: String in failures:
		push_error(failure)
	quit(0 if failures.is_empty() else 1)
