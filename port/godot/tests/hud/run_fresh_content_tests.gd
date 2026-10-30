extends SceneTree
## T-39002/T-39013: original-content acceptance for the fresh-session HUD path.
## Requires TOKIMEMO_TEST_INSTALLED_PACK (an installed runtime bundle). The HUD
## manifest identity comes from the authenticated installed inventory. The rule
## record below is a fixture for the caller-owned committed rule state; every
## HUD session field is produced by source-function mirrors, not by the test.

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const Live = preload("res://presentation/live_hud_adapter.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const Profile = preload("res://services/installed_launch_profile.gd")

var checks: int = 0
var failures: int = 0


func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL " + label)


func _initialize() -> void:
	var root := OS.get_environment("TOKIMEMO_TEST_INSTALLED_PACK")
	var parsed_manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(root.path_join("manifest.json")))
	var pack: Dictionary = Profile.read_pack(root, parsed_manifest if parsed_manifest is Dictionary else {})
	if not pack.get("valid", false) or not pack["file_identities"].has("hud_manifest"):
		printerr("installed pack is unavailable: " + str(pack.get("message", "")))
		quit(1)
		return
	var manifest: String = pack["paths"]["hud_manifest"]
	var manifest_sha: String = pack["file_identities"]["hud_manifest"]["sha256"]
	var font: String = pack["paths"]["font_root"]
	var presenter = Presenter.new()
	check(presenter.open_manifest(manifest, manifest_sha, font), "authenticated HUD manifest opens: " + presenter.error)
	var parsed: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(manifest))
	parsed["provenance"]["room_packet"]["track_sha256"] = "0".repeat(64)
	check(not Presenter.new()._validate_manifest(parsed), "incorrect original track proof is rejected")
	check(not presenter.open_manifest(manifest, "0".repeat(64), font) and presenter._manifest.is_empty(), "failed manifest reopen clears content")
	_legacy_hover_gate(manifest, manifest_sha, font)
	_fresh_session_checks(root, manifest, manifest_sha, font)
	_unlisted_manifest_is_rejected(root, manifest)
	if failures != 0:
		printerr("FAIL: %d/%d fresh HUD content checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d fresh HUD content checks (session phase, lag frame, rejection, reopen clearing)" % checks)
	quit(0)


func _legacy_context(style: int, ticks: int) -> Dictionary:
	return {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0, "icon_count": 14,
		"cursor_style": style, "hover_ticks": ticks, "family_name_hex": "82a0"}


func _legacy_hover_gate(manifest: String, manifest_sha: String, font: String) -> void:
	var legacy = Presenter.new()
	legacy.open_manifest(manifest, manifest_sha, font)
	check(not legacy.create_state(_rule(), _legacy_context(0x9C, 32)).is_empty(), "legacy settled context renders")
	check(legacy.create_state(_rule(), _legacy_context(0x9C, 0)).is_empty(), "legacy context with hover_ticks 0 is rejected")
	check(legacy.create_state(_rule(), _legacy_context(0x9C, 16)).is_empty(), "legacy context with a mid-pulse phase is rejected")


func _rule() -> Dictionary:
	var kinds: Array = [30]
	var markers: Array = [0]
	for day: int in range(1, 32):
		kinds.append(0)
		markers.append(0)
	# Icons after hud_icon_bar_init + settings_command_screen_icons_reset(0)
	# (8, 9, 10 off) and the 1995-04-04 phone rule (13 off), home_screens.c:310-330.
	var icons: Array = [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0]
	return {"year": 95, "month": 4, "day": 4, "weekday": 2,
		"params": [65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536],
		"calendar": [{"day_kind": kinds, "marker": markers}, {"day_kind": kinds.duplicate(), "marker": markers.duplicate()}],
		"menu_cursor": 0, "icon_enabled": icons,
		"player_flags": 0, "player_flags_upper": 0, "memory_card_busy": 0,
		"profile_family_bytes": [0x82, 0xa0, 0, 0, 0, 0, 0, 0]}


func _session(positions: Array) -> HudSession:
	var session := HudSession.new()
	session.game_state_init(true)
	session.hud_icon_bar_init(positions)
	session.command_screen_build(positions, 0, true)
	return session


func _frame(session: HudSession, sequences: Array, rule: Dictionary) -> void:
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	session.advance_icon_animation(sequences, rule["icon_enabled"])
	rule["menu_cursor"] = session.menu_cursor()


func _fresh_session_checks(root: String, manifest: String, manifest_sha: String, font: String) -> void:
	var tables: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(manifest))["tables"]
	var positions: Array = []
	for point: Array in tables["icon_positions"]:
		positions.append([int(point[0]), int(point[1])])
	var sequences: Array = tables["sequences"]
	var rule := _rule()
	var core := {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0}
	var live = Live.new()

	var captured := {"schema": "tokimemo.development-hud-context.v1", "boundary_sha256": Live.BOUNDARY_SHA,
		"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0, "icon_count": 14,
		"cursor_style": 0x9C, "hover_ticks": 32, "family_name_hex": "82a0"}
	check(not live.open_fresh(root, rule, core, captured) and live.profile.is_empty(), "captured boundary context is rejected by the fresh path")
	var synthetic := {"origin": "source-fresh-no-card-direct-entry-development-v1", "command_ready": true, "icon_mode": 0, "icon_count": 14}
	check(not live.open_fresh(root, rule, core, synthetic), "synthetic command-ready record is rejected")

	var session := _session(positions)
	check(not live.open_fresh(root, rule, core, session), "session before command select is rejected")
	_frame(session, sequences, rule)
	check(session.enter_command_select(0x34, 1, 0).is_empty(), "session reaches command select after source build and frame")
	check(not live.open_fresh(manifest.get_base_dir(), rule, core, session) and live.presenter._manifest.is_empty() and live.fresh_session == null, "a HUD packet directory is not an installed runtime and clears")
	var probe := HudSession.new()
	var handle: Variant = probe
	handle.set("_command_select", true)
	probe.icon_mode = 0
	probe.icon_count = 14
	check(not live.open_fresh(root, rule, core, probe) and live.fresh_session == null, "field assignment cannot open the fresh HUD")
	rule["menu_cursor"] = 1
	check(not live.open_fresh(root, rule, core, session), "rule cursor differing from the session is rejected")
	rule["menu_cursor"] = session.menu_cursor()

	check(session.hover_countdown == 32, "first command frame restarts the hover countdown")
	check(live.open_fresh(root, rule, core, session), "fresh HUD opens from the session: " + live.error)
	var restart: Dictionary = live.compose(rule, core, [], [])
	check(not restart.is_empty(), "restart phase (unit scale) renders: " + live.error)

	_frame(session, sequences, rule)
	var pulse: Dictionary = live.compose(rule, core, [], [])
	check(pulse.is_empty() and live.error.contains("scaled hover texels"), "scaled pulse phase is withheld explicitly without the hover texels")
	check(live.fresh_session == session and not live.presenter._manifest.is_empty(), "withheld frame keeps the open capability")

	for frame: int in range(31):
		_frame(session, sequences, rule)
	check(session.hover_countdown == 0, "countdown settles after 32 frames")
	var settled: Dictionary = live.compose(rule, core, [], [])
	check(not settled.is_empty(), "settled phase renders: " + live.error)

	# The settled fresh frame equals the legacy settled presenter for the same
	# hover record, cursor placement and icon sequence phase.
	var legacy = Presenter.new()
	legacy.open_manifest(manifest, manifest_sha, font)
	var legacy_state: Dictionary = legacy.create_state(rule, {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0,
		"icon_mode": 0, "icon_count": 14, "cursor_style": 0x9C, "hover_ticks": 32, "family_name_hex": "82a0"})
	legacy.set_animation_phase(legacy_state, session.icon_steps, session.icon_elapsed)
	legacy._compositor.set_checker_uv(Presenter.NativeCompositor.compute_checker_uv(0, 0))
	var expected: Dictionary = legacy.canvas_command(legacy_state)
	check(not settled.is_empty() and not expected.is_empty() and settled["id"] == expected["id"], "settled fresh frame matches the settled presenter")

	session.begin_outer_frame()
	session.command_screen_frame(0, 0x80)
	session.advance_icon_animation(sequences, rule["icon_enabled"])
	var styled: Dictionary = live.compose(rule, core, [], [])
	check(session.cursor_style == 0x9D and not styled.is_empty() and styled["id"] != settled["id"], "live cursor style change reaches the HUD")

	_lag_frame_checks(live, session, sequences, rule, core, manifest, manifest_sha, font, positions)

	core["mode_stage"] = 2
	check(live.compose(rule, core, [], []).is_empty(), "fresh path rejects modes without a source-owned transition")
	core["mode_stage"] = 1

	check(not live.open_fresh("/nonexistent-installed-runtime", rule, core, session), "failed reopen is rejected")
	check(live.profile.is_empty() and live.fresh_session == null and live.presenter._manifest.is_empty() and live.compose(rule, core, [], []).is_empty(), "failed reopen clears previous fresh state")
	rule["profile_family_bytes"] = [0x82, 0xa0, 0x82, 0xa0, 0x82, 0xa0, 0x82, 0xa0]
	check(not live.open_fresh(root, rule, core, session), "unterminated family name is rejected")


## The d-pad frame moves menu_cursor_0 to 1 while menu_items still hovers 0
## (menu_check_2 runs first) and the held right moves the cursor actor by
## cursor_speed after the item placement. The countdown is settled, so the
## frame has unit scale and renders.
func _lag_frame_checks(live: Live, session: HudSession, sequences: Array, rule: Dictionary, core: Dictionary,
	manifest: String, manifest_sha: String, font: String, positions: Array) -> void:
	session.begin_outer_frame()
	session.command_screen_frame(0x2000, 0x2000)
	session.advance_icon_animation(sequences, rule["icon_enabled"])
	rule["menu_cursor"] = session.menu_cursor()
	check(session.menu_cursor() == 1 and session.hover_index() == 0 and session.hover_countdown == 0, "d-pad frame lags the hover record by one frame")
	var lag: Dictionary = live.compose(rule, core, [], [])
	check(not lag.is_empty(), "lag frame renders: " + live.error)
	var state: Dictionary = live.presenter.create_state(rule, session.hud_context(0x34, 1, 0, "82a0"))
	live.presenter.set_animation_phase(state, session.icon_steps, session.icon_elapsed)
	var actors: Array = live.presenter.build_actors(state)
	var cursor_actor: Dictionary = {}
	var icon_variants: Dictionary = {}
	for actor: Dictionary in actors:
		if actor.get("kind") != "actor":
			continue
		if actor["id"] == 0:
			cursor_actor = actor
		elif actor["id"] >= 64 and actor["id"] < 78:
			icon_variants[actor["id"] - 64] = actor["variant"]
	var rule_cursor := Vector2i(positions[1][0] + 6, positions[1][1] - 2)
	check(cursor_actor.get("position") == Vector2i(session.cursor_x, session.cursor_y) and cursor_actor.get("position") == rule_cursor + Vector2i(3, 0), "cursor actor sits at the session cursor, not the rule placement")
	check(icon_variants.get(0) == "icon-52" and icon_variants.get(1) == "icon-53", "hover variant follows menu_items hover 0, not the selected icon 1")
	var legacy = Presenter.new()
	legacy.open_manifest(manifest, manifest_sha, font)
	var legacy_state: Dictionary = legacy.create_state(rule, _legacy_context(session.cursor_style, 32))
	legacy.set_animation_phase(legacy_state, session.icon_steps, session.icon_elapsed)
	legacy._compositor.set_checker_uv(Presenter.NativeCompositor.compute_checker_uv(0, 0))
	var expected: Dictionary = legacy.canvas_command(legacy_state)
	check(not expected.is_empty() and not lag.is_empty() and lag["id"] != expected["id"], "lag frame differs from the rule-cursor render")


## A reserialized HUD manifest still validates, but it is not the file the
## installed inventory lists, so the fresh path refuses the copied runtime.
func _unlisted_manifest_is_rejected(root: String, manifest: String) -> void:
	var copy := "/tmp/t39013-unlisted-%d" % Time.get_ticks_usec()
	if not _copy_tree(root, copy):
		check(false, "installed runtime copy for the unlisted manifest check")
		return
	var copied_manifest := copy.path_join(manifest.trim_prefix(root).trim_prefix("/"))
	var reserialized := JSON.stringify(JSON.parse_string(FileAccess.get_file_as_string(manifest)), "\t")
	var file := FileAccess.open(copied_manifest, FileAccess.WRITE)
	file.store_string(reserialized)
	file.close()
	check(Presenter.new()._validate_manifest(JSON.parse_string(reserialized)), "reserialized manifest still validates")
	var session := HudSession.new()
	session.game_state_init(true)
	var tables: Dictionary = JSON.parse_string(reserialized)["tables"]
	var positions: Array = []
	for point: Array in tables["icon_positions"]:
		positions.append([int(point[0]), int(point[1])])
	session.command_screen_build(positions, 0, true)
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	session.enter_command_select(0x34, 1, 0)
	var rule := _rule()
	var live = Live.new()
	check(session.at_command_select() and not live.open_fresh(copy, rule, {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0}, session) and live.error.contains("installed runtime"), "valid but unlisted HUD manifest is rejected: " + live.error)


func _copy_tree(from: String, to: String) -> bool:
	if DirAccess.make_dir_recursive_absolute(to) != OK:
		return false
	for name: String in DirAccess.get_files_at(from):
		if DirAccess.copy_absolute(from.path_join(name), to.path_join(name)) != OK:
			return false
	for name: String in DirAccess.get_directories_at(from):
		if not _copy_tree(from.path_join(name), to.path_join(name)):
			return false
	return true
