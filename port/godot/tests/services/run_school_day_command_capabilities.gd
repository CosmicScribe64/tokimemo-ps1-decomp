extends SceneTree
## services/school_day_command_capabilities.gd against an installed bundle
## (TOKIMEMO_BUNDLE, e.g. installed-runtime-v18): the four ordinary-command
## roles open, every weekly command 0..7 is offered with its original
## background and SD resources, the school-day controller receives exactly
## that list, and a missing or wrong role leaves the capability closed.

const Capabilities = preload("res://services/school_day_command_capabilities.gd")
const SchoolDay = preload("res://scenes/school_day_controller.gd")
const LiveHud = preload("res://presentation/live_hud_adapter.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")

var checks := 0
var failures := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr("FAIL: " + label)

func _initialize() -> void:
	var bundle := OS.get_environment("TOKIMEMO_BUNDLE")
	var paths := {}
	for role: String in Capabilities.ROLES:
		paths[role] = bundle.path_join("packs").path_join(role)
	var closed := Capabilities.new()
	var partial := paths.duplicate()
	partial.erase("club_media")
	check(not closed.open_installed(partial).is_empty() and closed.available().is_empty(), "a missing role leaves the capability closed")
	var swapped := paths.duplicate()
	swapped["spring_media"] = paths["weekday56_media"]
	check(not closed.open_installed(swapped).is_empty() and closed.available().is_empty(), "a pack in the wrong role is refused")
	var open := Capabilities.new()
	var error := open.open_installed(paths)
	check(error.is_empty(), "installed ordinary command media opens: " + error)
	if error.is_empty():
		check(open.available() == [0, 1, 2, 3, 4, 5, 6, 7], "weekly commands 0..7 are offered: %s" % str(open.available()))
		for command: int in [0, 1, 2, 3, 5, 6, 7]:
			var resources: Dictionary = open.adapter.get_resources(command)
			check(open.adapter.has_command(command) and resources.get("frames") is Dictionary and not resources["frames"].is_empty(), "command %d has original SD frames" % command)
			check(not open.adapter.get_background_commands(command).is_empty(), "command %d has its original background" % command)
		for club: int in MediaAdapter.CLUB_ROUTES:
			check(open.adapter.has_command(4, club), "club %d has its original media" % club)
		var tables: Variant = LiveHud._integers(JSON.parse_string(FileAccess.get_file_as_string(bundle.path_join("packs/rule_tables/manifest.json"))))
		var controller := SchoolDay.new(tables["tables"] if tables is Dictionary else {})
		check(controller.error.is_empty(), "school-day controller opens with the installed rule tables: " + controller.error)
		check(open.apply(controller).is_empty() and controller.command_capabilities == [0, 1, 2, 3, 4, 5, 6, 7], "controller receives the offered commands")
		check(closed.apply(controller).is_empty() and controller.command_capabilities.is_empty(), "a closed capability clears the controller list")
	print("%s: school-day command capabilities %d checks, %d failures" % ["PASS" if failures == 0 else "FAIL", checks, failures])
	quit(0 if failures == 0 else 1)
