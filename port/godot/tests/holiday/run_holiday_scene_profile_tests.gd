extends SceneTree
## T-39014. Fresh holiday scene profile: original bytes, field ownership and
## explicit rejection of every missing live-session field. Media opening is
## covered separately by run_scene_adapter_tests.gd.

const SceneAdapter = preload("res://services/holiday_scene_adapter.gd")
const LaunchProfile = preload("res://services/installed_launch_profile.gd")

var failures: int = 0
var checks: int = 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: " + label)

func _initialize() -> void:
	var path: String = OS.get_environment("TOKIMEMO_HOLIDAY_SCENE_PROFILE")
	var scratch: String = OS.get_environment("TOKIMEMO_TEST_TMP")
	if path.is_empty() or scratch.is_empty():
		printerr("FAIL: TOKIMEMO_HOLIDAY_SCENE_PROFILE and TOKIMEMO_TEST_TMP are required")
		quit(1)
		return
	var loaded: Dictionary = SceneAdapter.load_fresh_profile(path)
	check(loaded["error"].is_empty(), "original profile authenticates: " + str(loaded["error"]))
	var scene: Dictionary = loaded["scene"]
	check(scene.get("dial_keys") == [255, 255, 255, 255, 255, 255, 255, 0], "dial ring from executable")
	check(scene.get("dial_pos") == 0 and scene.get("icon_count") == 14 and scene.get("background_brightness") == 128, "open_init scalars")
	for key: String in SceneAdapter.SESSION_FIELDS:
		check(not scene.has(key), "session field has no profile value: " + key)

	# Altered bytes (same schema, changed value) fail the exact identity.
	var text: String = FileAccess.get_file_as_string(path)
	var altered_path: String = scratch.path_join("altered-holiday-scene.json")
	var altered: FileAccess = FileAccess.open(altered_path, FileAccess.WRITE)
	altered.store_string(text.replace("\"icon_count\": 14", "\"icon_count\": 15"))
	altered.close()
	check(text.contains("\"icon_count\": 14"), "fixture contains the icon count field")
	check(not SceneAdapter.load_fresh_profile(altered_path)["error"].is_empty(), "altered profile is rejected")
	check(not SceneAdapter.load_fresh_profile(scratch.path_join("absent.json"))["error"].is_empty(), "absent profile is rejected")

	# The legacy entry point names the missing fields instead of defaulting them.
	var adapter: SceneAdapter = SceneAdapter.new()
	var room: Image = Image.create(320, 88, false, Image.FORMAT_RGBA8)
	var legacy_error: String = adapter.open(null, path, room)
	check(legacy_error.contains("live-session boundary fields") and adapter.initial_extension().is_empty(), "open() rejects the fresh profile explicitly: " + legacy_error)
	var session: Dictionary = {"boot_hold": 0, "clubs": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], "icon_mode": 0,
		"input_device": 1, "screen_brightness": 128, "tile_layer_enabled": 0}
	# open_fresh: each gate fails with its own error before media is touched.
	var profile_error: String = adapter.open_fresh(null, altered_path, room, session)
	check(profile_error.contains("path/hash") and adapter.initial_extension().is_empty() and adapter.error == profile_error,
		"open_fresh rejects an unauthenticated profile: " + profile_error)
	var incomplete: Dictionary = session.duplicate(true)
	incomplete.erase("clubs")
	var session_error: String = adapter.open_fresh(null, path, room, incomplete)
	check(session_error.contains("live-session holiday fields") and adapter.initial_extension().is_empty(),
		"open_fresh rejects an incomplete session: " + session_error)
	var bounded_error: String = adapter.open_fresh(null, path, room, session.merged({"screen_brightness": 999}, true))
	check(bounded_error.contains("native game_state bounds") and adapter.initial_extension().is_empty(),
		"open_fresh rejects an out-of-range session value: " + bounded_error)
	var fresh_error: String = adapter.open_fresh(null, path, room, session)
	check(fresh_error.contains("C_02_02H") and adapter.initial_extension().is_empty(), "open_fresh keeps the next-room gate")
	var composed: Dictionary = SceneAdapter.compose_fresh(scene, session)
	check(composed.size() == SceneAdapter.SCENE_FIELDS.size() + SceneAdapter.SESSION_FIELDS.size(), "complete composition")
	check(composed.get("icon_count") == 14 and composed.get("input_device") == 1, "scene and caller values kept apart")
	for key: String in SceneAdapter.SESSION_FIELDS:
		var missing: Dictionary = session.duplicate(true)
		missing.erase(key)
		check(SceneAdapter.compose_fresh(scene, missing).is_empty(), "missing session field rejected: " + key)
	var extra: Dictionary = session.duplicate(true)
	extra["dial_pos"] = 3
	check(extra.size() == SceneAdapter.SESSION_FIELDS.size() + 1, "extra-field fixture keeps all six session fields")
	check(SceneAdapter.compose_fresh(scene, extra).is_empty(), "complete session plus a scene-owned key is rejected")
	var substituted: Dictionary = session.duplicate(true)
	substituted["dial_pos"] = 3
	substituted.erase("boot_hold")
	check(SceneAdapter.compose_fresh(scene, substituted).is_empty(), "missing boot_hold with substituted scene key rejected")
	var wrong: Dictionary = session.duplicate(true)
	wrong["icon_mode"] = "0"
	check(SceneAdapter.compose_fresh(scene, wrong).is_empty(), "non-integer session field rejected")
	wrong = session.duplicate(true)
	wrong["clubs"] = 0
	check(SceneAdapter.compose_fresh(scene, wrong).is_empty(), "non-array club records rejected")
	# Native game_state bounds: every edge accepted, one past it rejected.
	var edges: Dictionary = {"icon_mode": [0, 255], "input_device": [0, 255], "screen_brightness": [0, 255],
		"tile_layer_enabled": [-128, 127], "boot_hold": [-128, 127]}
	for key: String in edges:
		for edge: int in edges[key]:
			check(not SceneAdapter.compose_fresh(scene, session.merged({key: edge}, true)).is_empty(), "%s accepts %d" % [key, edge])
		check(SceneAdapter.compose_fresh(scene, session.merged({key: edges[key][0] - 1}, true)).is_empty(), key + " below range rejected")
		check(SceneAdapter.compose_fresh(scene, session.merged({key: edges[key][1] + 1}, true)).is_empty(), key + " above range rejected")
	for count: int in [0, 11, 13]:
		var clubs: Array = []
		clubs.resize(count)
		clubs.fill(0)
		check(SceneAdapter.compose_fresh(scene, session.merged({"clubs": clubs}, true)).is_empty(), "club record count %d rejected" % count)
	for club: int in [-1, 65536]:
		var clubs: Array = session["clubs"].duplicate()
		clubs[3] = club
		check(SceneAdapter.compose_fresh(scene, session.merged({"clubs": clubs}, true)).is_empty(), "club record %d rejected" % club)
	var top_clubs: Array = session["clubs"].duplicate()
	top_clubs[11] = 65535
	check(not SceneAdapter.compose_fresh(scene, session.merged({"clubs": top_clubs}, true)).is_empty(), "club record 65535 accepted")
	_check_installed_role(path, scratch)
	if failures == 0:
		print("PASS: %d holiday scene profile checks" % checks)
	quit(1 if failures > 0 else 0)

## An installed bundle that carries the fresh profile as holiday_scene_source
## must still report the holiday_scene gameplay role as missing.
func _check_installed_role(profile_path: String, scratch: String) -> void:
	var root: String = scratch.path_join("installed-holiday-source")
	var relative: String = "packs/holiday_scene_source/manifest.json"
	check(DirAccess.make_dir_recursive_absolute(root.path_join(relative.get_base_dir())) == OK, "bundle fixture directory")
	var bytes: PackedByteArray = FileAccess.get_file_as_bytes(profile_path)
	var stream: FileAccess = FileAccess.open(root.path_join(relative), FileAccess.WRITE)
	stream.store_buffer(bytes)
	stream.close()
	var manifest: Dictionary = {"schema": LaunchProfile.SCHEMA, "version": 1, "release_profile": LaunchProfile.RELEASE,
		"source": {"executable_sha256": LaunchProfile.EXE_SHA},
		"files": {relative: {"size": bytes.size(), "sha256": FileAccess.get_sha256(root.path_join(relative))}},
		"roles": {"holiday_scene_source": relative}}
	var result: Dictionary = LaunchProfile.read_pack(root, manifest)
	check(result.get("valid") == true, "installed bundle with holiday_scene_source is readable: " + str(result.get("message")))
	check(result.get("paths", {}).has("holiday_scene_source") and not result.get("paths", {}).has("holiday_scene"), "source role is not the gameplay role")
	check(result.get("missing_gameplay_roles", []).has("holiday_scene"), "holiday_scene stays a missing gameplay role")
	check(not LaunchProfile.GAMEPLAY_ROLES.has("holiday_scene_source"), "source role is not a gameplay role")
