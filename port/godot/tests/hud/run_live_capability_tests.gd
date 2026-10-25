extends SceneTree
const Live = preload("res://presentation/live_hud_adapter.gd")

func _initialize() -> void:
	var live: Live = Live.new()
	var manifest: String = OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	var font: String = OS.get_environment("TOKIMEMO_FONT_ROOT")
	var context: String = OS.get_environment("TOKIMEMO_HUD_CONTEXT")
	var checks: Dictionary = {}
	checks["reviewed external content opens without oracle fixtures"] = live.open(manifest, Live.MANIFEST_SHA, font, context, Live.CONTEXT_SHA)
	checks["wrong reviewed identity refuses reopen"] = not live.open(manifest, "bad", font, context, Live.CONTEXT_SHA)
	checks["failed reopen clears nested content and caller seeds"] = live.presenter._manifest.is_empty() and live.profile.is_empty() and live.initial_icons.is_empty() and live.initial_params.is_empty()
	checks["failed reopen cannot compose prior content"] = live.compose({}, {}, [], []).is_empty() and not live.error.is_empty()
	var failures: Array[String] = []
	for label: String in checks:
		if not checks[label]:
			failures.append(label)
	var report: Dictionary = {"checks": checks, "failures": failures}
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("live-capability.json"), FileAccess.WRITE)
	output.store_string(JSON.stringify(report, "\t"))
	output.close()
	print(JSON.stringify(report))
	quit(0 if failures.is_empty() else 1)
