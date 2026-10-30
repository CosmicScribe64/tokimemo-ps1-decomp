extends RefCounted
## T-30002 WP5: the display facts the settings pages read. Scene layout
## (widescreen) is available because every native boot now frames its picture
## with NativeDisplayHost; drawable_info() reports the window size and the
## native picture factor from the same NativeFrameLayout the host uses.

const Layout = preload("res://presentation/native_frame_layout.gd")

var widescreen_available := true
var _runtime: Node

func _init(runtime: Node) -> void:
	_runtime = runtime

func drawable_info() -> Dictionary:
	var window: Window = _runtime.get_window() if _runtime != null and _runtime.is_inside_tree() else null
	var size: Vector2i = window.size if window != null else Vector2i(640, 480)
	var prefs: Dictionary = _runtime.preferences.effective_settings() if _runtime != null else {}
	var layout := Layout.compute(Vector2(size), prefs)
	return {"window": size, "factor": float(layout.get("factor", 1.0)), "mode": str(layout.get("mode", "original")),
		"logical_width": int(layout.get("logical_width", Layout.NATIVE_SIZE.x))}
