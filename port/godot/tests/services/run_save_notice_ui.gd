extends SceneTree
## UI-only save notice lifetime/input checks; no native simulation objects are stepped.
const Controls = preload("res://app/boundary_save_controls.gd")
var failures: Array[String] = []
var checks: int = 0

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var host: Node = Node.new()
	root.add_child(host)
	var controls: Controls = Controls.new()
	controls.notice = Label.new()
	controls.notice_timer = Timer.new()
	controls.notice_timer.one_shot = true
	controls.notice_timer.wait_time = 0.12
	controls.notice_timer.process_mode = Node.PROCESS_MODE_ALWAYS
	controls.notice_timer.timeout.connect(controls._expire_notice)
	host.add_child(controls.notice)
	host.add_child(controls.notice_timer)
	var native_tick: int = 12345
	controls.last_error = "retained observer diagnostic"
	controls.last_action = "save"
	controls.last_checkpoint = "command"
	controls._message("Saved command selection.", true)
	_check(controls.notice.visible and controls.notice.text == "Saved command selection.",
		"success shows only the short status without a persistent F5/F9 help line")
	_check(not controls.notice_timer.is_stopped(), "success starts a separate UI-only expiry timer")
	_check(controls.last_error == "retained observer diagnostic" and controls.last_action == "save"
		and controls.last_checkpoint == "command", "success notice preserves detailed diagnostics and observer metadata")
	_check(native_tick == 12345, "showing success status does not advance simulation state")
	await create_timer(0.2).timeout
	_check(not controls.notice.visible and controls.notice.text.is_empty(), "success status clears after its transient lifetime")
	controls._error("Save unavailable: checkpoint is outside the command/first-result save boundaries")
	_check(controls.notice.visible and controls.notice.text.contains("outside the command/first-result")
		and controls.last_error.contains("outside the command/first-result"), "failure remains visible with full diagnostic detail")
	var event: InputEventKey = InputEventKey.new()
	event.physical_keycode = KEY_F5
	event.pressed = true
	_check(controls.handle_input(host, event), "F5 is still consumed by save controls")
	_check(controls.last_error == "Save controls are unavailable." and controls.notice.visible,
		"hotkey failure remains visible and available through last_error")
	_check(native_tick == 12345, "input/error display does not advance simulation state")
	print(JSON.stringify({"checks": checks, "failures": failures}))
	host.free()
	quit(1 if not failures.is_empty() else 0)

func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok: failures.append(message)
