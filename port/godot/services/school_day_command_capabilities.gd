extends RefCounted
## Ordinary weekly command capabilities for the installed school-day
## controller (scenes/school_day_controller.gd command_capabilities, checked by
## flow/command_screen.gd confirm). A command is offered only when its
## original background, SD set and frames are open from the installed packs:
## school_day_media (commands 0 study and 7 rest), spring_media (1..3),
## weekday56_media (5, 6) and club_media (4, per club). Each pack is read by
## its store, which checks its own manifest identity; the installed launch
## profile has already checked every file against the bundle inventory.
## This is the same capability source as app/playable_boot.gd's development
## path (adapter.open_packs + exact_media.open), opened from installed roles.

const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")
const ExactMedia = preload("res://services/native_activity_media.gd")
const ROLES := ["school_day_media", "spring_media", "weekday56_media", "club_media"]

var adapter: MediaAdapter
var exact_media: ExactMedia
var error := ""

## paths: the launch profile's role paths. Missing roles leave the
## capability closed (an empty command list), never partly filled.
func open_installed(paths: Dictionary) -> String:
	adapter = null
	exact_media = null
	error = ""
	for role: String in ROLES:
		if not paths.get(role) is String or str(paths[role]).is_empty():
			error = "installed ordinary command media lacks the %s role" % role
			return error
	var next := MediaAdapter.new()
	var failure: String = next.open_packs(paths.school_day_media, paths.spring_media, paths.weekday56_media, paths.club_media)
	if failure.is_empty():
		var exact := ExactMedia.new()
		failure = exact.open(next)
		if failure.is_empty():
			for command: int in [0, 7]:
				if next.has_command(command):
					next.command_resources[command] = exact.get_resources(command)
			adapter = next
			exact_media = exact
	error = failure
	return error

## Commands the controller may confirm; empty while closed.
func available() -> Array[int]:
	return adapter.get_available_commands() if adapter != null else ([] as Array[int])

## Sets the controller's validated capability list (empty when closed, so
## flow/command_screen.gd refuses a confirm with the same source error).
func apply(controller: Object) -> String:
	if controller == null or not controller.get("command_capabilities") is Array:
		return "school-day controller has no command capability record"
	var commands: Array[int] = available()
	controller.get("command_capabilities").assign(commands)
	return ""
