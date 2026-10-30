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
	if OS.get_environment("TOKIMEMO_ENTRY_DEBUG") == "1":
		# Lets tools/port/desktop_verify/verify_player.py check the BGM bridge's
		# native-component gate in an exported player without driving the opening.
		var bridge: Script = load("res://services/first_arrival_bgm_bridge.gd")
		# The bridge gains library_path()/native_component_error() with the
		# deferred packaging hunk; report their absence instead of a script error.
		var methods: Array = []
		if bridge != null:
			for method: Dictionary in bridge.get_script_method_list():
				methods.append(method.get("name", ""))
		if "library_path" in methods and "native_component_error" in methods:
			print("NativeAudioGate: ", JSON.stringify({"library_path": bridge.call("library_path"),
				"error": bridge.call("native_component_error")}))
		else:
			print("NativeAudioGate: ", JSON.stringify({"library_path": "",
				"error": "BGM bridge has no native component gate (deferred packaging hunk not applied)"}))
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
