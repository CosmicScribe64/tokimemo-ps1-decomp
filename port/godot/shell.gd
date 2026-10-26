extends Control
## Application main shell and boot entry (T-29016).
## Performs boot routing to first-launch setup or playable gameplay.
## Preserves GL Compatibility smoke capture when GODOT_SMOKE_CAPTURE is set.

const SMOKE_CAPTURE_ENV: String = "GODOT_SMOKE_CAPTURE"
const BootRouterScript = preload("res://app/boot_router.gd")

var _router: RefCounted

func _ready() -> void:
	if not OS.get_environment(SMOKE_CAPTURE_ENV).is_empty():
		_capture_rendered_smoke.call_deferred()
		return
	_boot.call_deferred()

func _boot() -> void:
	_router = BootRouterScript.new()
	var decision: Dictionary = _router.route(self)
	print("BootRouter: ", decision.get("action", "unknown"), " -> ", decision.get("target", "none"), " (", decision.get("reason", ""), ")")

func _capture_rendered_smoke() -> void:
	var capture_path: String = OS.get_environment(SMOKE_CAPTURE_ENV)
	if capture_path.is_empty():
		return
	await get_tree().process_frame
	await RenderingServer.frame_post_draw
	var capture: Image = get_viewport().get_texture().get_image()
	var error: Error = capture.save_png(capture_path)
	if error != OK:
		push_error("Could not save rendered smoke capture: %s" % error_string(error))
		get_tree().quit(1)
	else:
		print("Rendered GL Compatibility smoke capture: ", capture_path)
