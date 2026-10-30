extends SceneTree
## Isolated real-window transform regression; no original-media acceptance claim.
const Entry = preload("res://app/new_game_entry_boot.gd")
var failures: Array[String] = []

func _initialize() -> void: run.call_deferred()

func check(value: bool, label: String) -> void:
	if not value: failures.append(label)

func run() -> void:
	var runtime := root.get_node("ModernSettings")
	runtime.preferences.settings.display.width = 1024
	runtime.preferences.settings.display.height = 768
	runtime.preferences.settings.display.filter = "integer"
	runtime.apply_preferences(runtime.preferences.effective_settings())
	var entry := Entry.new(); root.add_child(entry)
	await process_frame
	check(is_equal_approx(entry.canvas.scale.x,3.0),"stored integer scale")
	var candidate: Dictionary = runtime.preferences.settings.display.duplicate(true)
	candidate.filter = "nearest"
	check(runtime.preferences.begin_display(candidate,Time.get_ticks_msec()/1000.0),"begin nearest preview")
	await process_frame
	check(runtime.preferences.settings.display.filter == "integer","preview retains stored choice")
	check(is_equal_approx(entry.canvas.scale.x,3.2),"preview uses effective nearest scale")
	check(runtime.preferences.revert_display(),"revert preview")
	await process_frame
	check(is_equal_approx(entry.canvas.scale.x,3.0),"revert restores integer scale")
	check(runtime.preferences.begin_display(candidate,Time.get_ticks_msec()/1000.0),"begin second preview")
	check(runtime.preferences.confirm_display(),"keep nearest preview")
	await process_frame
	check(is_equal_approx(entry.canvas.scale.x,3.2),"keep retains nearest scale")
	candidate.filter = "integer"
	check(runtime.preferences.begin_display(candidate,Time.get_ticks_msec()/1000.0),"preview integer from saved nearest")
	await process_frame
	check(is_equal_approx(entry.canvas.scale.x,3.0),"integer preview floors scale")
	check(runtime.preferences.revert_display(),"revert integer preview")
	await process_frame
	check(is_equal_approx(entry.canvas.scale.x,3.2),"revert returns to saved nearest")
	print("NAME display preview/keep/revert checks: ",failures)
	quit(0 if failures.is_empty() else 1)
