extends SceneTree
## Bounded installed-package smoke and fail-closed profile checks.
const Provider = preload("res://presentation/prologue_hud_source_provider.gd")

var checks := 0
var failures := 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ", label)

func _initialize() -> void:
	call_deferred("run")

func _profile_copy(root: String, name: String, mutate: Callable) -> Array:
	var profile_path := root.path_join("packs/prologue_hud/manifest.json")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(profile_path))
	if not parsed is Dictionary:
		return ["", ""]
	mutate.call(parsed)
	var path := root.path_join(name)
	var file := FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return ["", ""]
	file.store_string(JSON.stringify(parsed, "\t", false))
	file = null
	return [name, FileAccess.get_sha256(path)]

func _remove_profile_copy(root: String, profile_copy: Array) -> bool:
	if profile_copy.size() < 1 or str(profile_copy[0]).is_empty():
		return true
	var path := root.path_join(str(profile_copy[0]))
	if not FileAccess.file_exists(path):
		return true
	return DirAccess.remove_absolute(path) == OK

func run() -> void:
	var root := OS.get_environment("PROLOGUE_HUD_ROOT")
	if root.is_empty() or not root.is_absolute_path():
		check(false, "external installed bundle path is supplied")
		_finish()
		return
	var profile_rel := "packs/prologue_hud/manifest.json"
	var pin := FileAccess.get_sha256(root.path_join(profile_rel))
	var provider = Provider.new()
	check(provider.open_installed(root, "../manifest.json", pin) != "",
		"profile traversal is rejected")
	check(provider.open_installed(root, profile_rel, "0".repeat(64)) != "",
		"wrong profile hash is rejected")
	var bad_path := _profile_copy(root, "negative-path.json", func(p: Dictionary) -> void:
		p["resources"]["prologue_assets"]["directory"] = "packs/prologue_hud/../escape")
	check(not str(bad_path[0]).is_empty() and
		provider.open_installed(root, bad_path[0], bad_path[1]) != "",
		"malformed installed resource path is rejected")
	check(_remove_profile_copy(root, bad_path), "malformed path fixture is removed")
	var bad_release := _profile_copy(root, "negative-release.json", func(p: Dictionary) -> void:
		p["release_profile"] = "foreign-release")
	check(not str(bad_release[0]).is_empty() and
		provider.open_installed(root, bad_release[0], bad_release[1]) != "",
		"foreign release identity is rejected")
	check(_remove_profile_copy(root, bad_release), "foreign release fixture is removed")
	var bad_manifest := _profile_copy(root, "negative-manifest.json", func(p: Dictionary) -> void:
		p["resources"]["prologue_assets"]["manifest_sha256"] = "0".repeat(64))
	check(not str(bad_manifest[0]).is_empty() and
		provider.open_installed(root, bad_manifest[0], bad_manifest[1]) != "",
		"foreign resource pack is rejected")
	check(_remove_profile_copy(root, bad_manifest), "foreign resource fixture is removed")
	var bad_source_pins := _profile_copy(root, "negative-source-pins.json", func(p: Dictionary) -> void:
		p["source_evidence"][0]["sha256"] = "0".repeat(64))
	check(not str(bad_source_pins[0]).is_empty() and
		provider.open_installed(root, bad_source_pins[0], bad_source_pins[1]) != "",
		"repinned source evidence is rejected")
	check(_remove_profile_copy(root, bad_source_pins), "source pin fixture is removed")
	var malformed_title := _profile_copy(root, "negative-title.json", func(p: Dictionary) -> void:
		p["title"] = {"raw_hex": {}})
	check(not str(malformed_title[0]).is_empty() and
		provider.open_installed(root, malformed_title[0], malformed_title[1]) != "",
		"malformed title metadata is rejected")
	check(_remove_profile_copy(root, malformed_title), "malformed title fixture is removed")
	var open_error: String = provider.open_installed(root, profile_rel, pin)
	if not open_error.is_empty(): print("installed-open diagnostic: ", open_error)
	check(open_error.is_empty(),
		"pinned installed package opens without source checkout")
	check(provider.title_bytes().size() == 10,
		"title resource returns the pinned fixed-length byte extent")
	check(provider.frame_parts(60).get("status") == "frame_parts",
		"source-backed date actor parts load from installed assets")
	check(provider.checker_tile(0).get("status") == "asset",
		"indexed checker data loads from installed assets")
	check(provider.calendar_image(1995, 4, "calendar-grid").get("status") == "asset",
		"first profiled calendar tile loads from installed assets")
	var live := {"game_mode": 21, "mode_stage": 0, "stage_init_substep": 0,
		"title_timer": -64, "screen_brightness": 0, "year": 95, "month": 4,
		"day": 4, "weekday": 1, "day_kind": 0, "checker_scroll_u": 0,
		"checker_scroll_v": 0, "checker_scroll_phase": "after_hud_screen_frame_draw_scroll_update"}
	check(provider.stage0_contract(live).get("status") == "stage0_resources_supported",
		"source-bound stage contract validates a compatible live state")
	var profile: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(root.path_join(profile_rel)))
	var asset_root := root.path_join(profile["resources"]["prologue_assets"]["directory"])
	var assets_manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(
		asset_root.path_join(profile["resources"]["prologue_assets"]["manifest"])))
	var color_record: Dictionary = assets_manifest["frames"]["60:base"]["parts"][0]["png"]
	var color_path := asset_root.path_join(color_record["path"])
	var color_bytes := FileAccess.get_file_as_bytes(color_path)
	check(not color_bytes.is_empty() and DirAccess.remove_absolute(color_path) == OK,
		"test fixture can remove a required selected frame image")
	check(provider.open_installed(root, profile_rel, pin) != "",
		"missing required frame image blocks installed provider open")
	var restored := FileAccess.open(color_path, FileAccess.WRITE)
	if restored != null:
		restored.store_buffer(color_bytes)
		restored = null
	check(FileAccess.get_file_as_bytes(color_path) == color_bytes,
		"temporary required-image removal is restored after the check")
	_finish()

func _finish() -> void:
	print("PROLOGUE_HUD_INSTALLED checks=%d failures=%d" % [checks, failures])
	quit(0 if failures == 0 else 1)
