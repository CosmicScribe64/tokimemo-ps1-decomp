extends Control

const SMOKE_CAPTURE_ENV: String = "GODOT_SMOKE_CAPTURE"

func _ready() -> void:
    _capture_rendered_smoke.call_deferred()

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
