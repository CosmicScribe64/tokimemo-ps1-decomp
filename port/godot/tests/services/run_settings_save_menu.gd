extends "res://tests/services/settings_test_tree.gd"
## Synthetic host exercises production menu/queue guards, not playable-route saves.
const Controls = preload("res://app/boundary_save_controls.gd")
const Storage = preload("res://services/save_service.gd")
class Host extends Node:
	var session := preload("res://core/game_session.gd").new()
	var state := preload("res://rules/school_day_state.gd").new()
	var pad := preload("res://ui/native_pad.gd").new()
	var save_controls := Controls.new()
	var focus_paused := false
	var playback_paused := false
	var _window_focused := true
	var _application_focused := true
	var _resume_input_pending := false
	var _replay_requested := false
	var video_capture_busy := false
	var stopped := false
	var service_on_physics := false
	func _ready() -> void:
		add_to_group("settings_save_hosts"); add_child(pad)
		save_controls.storage = Storage.new("user://settings-save-menu-empty")
	func request_settings_save(load_game: bool) -> bool:
		return save_controls.request(self,Controls.Request.LOAD if load_game else Controls.Request.SAVE)
	func _physics_process(_delta: float) -> void:
		if service_on_physics: save_controls.process_queued(self)
func _initialize() -> void: call_deferred("run")
func run() -> void:
	var runtime := root.get_node("ModernSettings")
	var host := Host.new(); root.add_child(host)
	_controller(JOY_BUTTON_LEFT_STICK,true); _controller(JOY_BUTTON_RIGHT_STICK,true); await process_frame
	_controller(JOY_BUTTON_LEFT_STICK,false); _controller(JOY_BUTTON_RIGHT_STICK,false); await process_frame
	# In a game scene L3+R3 opens the pause menu (WP8); its Settings entry
	# opens the full menu, and a quick save chosen there resumes the game.
	var pause: Control = runtime.layer.get_node("PauseMenu")
	check(pause.visible and not runtime.menu.visible and paused)
	(pause.focus_items["Settings"] as Button).pressed.emit(); await process_frame
	check(runtime.menu.visible and not pause.visible)
	check(runtime.menu.focus_items.has("Quick save"))
	for page: int in 5:
		_controller(JOY_BUTTON_RIGHT_SHOULDER,true); await process_frame
		_controller(JOY_BUTTON_RIGHT_SHOULDER,false); await process_frame
	check(runtime.menu.tabs.current_tab == 5)
	_controller(JOY_BUTTON_DPAD_RIGHT,true); await process_frame
	_controller(JOY_BUTTON_DPAD_RIGHT,false); await process_frame
	if root.gui_get_focus_owner() == runtime.menu.focus_items["Load quick save"]:
		_controller(JOY_BUTTON_DPAD_UP,true); await process_frame
		_controller(JOY_BUTTON_DPAD_UP,false); await process_frame
	check(root.gui_get_focus_owner() == runtime.menu.focus_items["Quick save"])
	var capture := OS.get_environment("SETTINGS_SAVE_CAPTURE")
	if not capture.is_empty():
		await RenderingServer.frame_post_draw
		check(root.get_texture().get_image().save_png(capture) == OK)
	_controller(JOY_BUTTON_A,true); await process_frame
	check(runtime.menu.visible and paused)
	check(host.save_controls.pending == Controls.Request.NONE and host.save_controls.storage_calls == 0)
	host.service_on_physics = true
	await create_timer(0.1).timeout
	check(runtime.menu.visible and host.save_controls.pending == Controls.Request.NONE)
	_controller(JOY_BUTTON_A,false)
	await create_timer(0.1).timeout
	check(not runtime.menu.visible and not paused and not pause.visible)
	check(host.save_controls.pending == Controls.Request.NONE)
	check(host.save_controls.last_error.contains("command selection") and host.save_controls.storage_calls == 0)
	host.service_on_physics = false
	host.focus_paused = true
	check(not host.request_settings_save(false)); check(host.save_controls.pending == Controls.Request.NONE)
	host.focus_paused = false; host.video_capture_busy = true
	check(not host.request_settings_save(true)); host.video_capture_busy = false
	host.session.rng.begin_step(func(_error): pass)
	check(not host.request_settings_save(false)); host.session.rng.end_step()
	check(host.request_settings_save(false)); check(not host.request_settings_save(true))
	host.pad._devices["held"] = 0x20
	check(host.save_controls.process_queued(host)); check(host.save_controls.last_error.contains("Release"))
	check(host.save_controls.storage_calls == 0)
	host.pad.clear_devices()
	runtime.toggle_menu(); await process_frame
	runtime.menu.focus_items["Load quick save"].pressed.emit()
	check(host.save_controls.pending == Controls.Request.LOAD and not runtime.menu.visible)
	host.save_controls.cancel_pending("test complete")
	host.queue_free(); await process_frame
	check(runtime.save_host() == null)
	print("Settings save menu: real UI accept routes to guarded queue; unsupported/focus/busy/in-step/held/duplicate requests preserve storage boundary (synthetic host only)")
	finish()

func _controller(button: int, pressed: bool) -> void:
	var event := InputEventJoypadButton.new(); event.device = 0; event.button_index = button; event.pressed = pressed
	Input.parse_input_event(event)
