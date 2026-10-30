extends SceneTree
## T-39002: original-content acceptance, requiring external regenerated inputs.

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const Live = preload("res://presentation/live_hud_adapter.gd")
const EXPECTED_SHA := "55336d7e593fc9077f697184c6029da23c320aae2000447fbe6fb40c9bcb47e2"

func _initialize() -> void:
	var manifest := OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	var font := OS.get_environment("TOKIMEMO_FONT_ROOT")
	var presenter = Presenter.new()
	if not presenter.open_manifest(manifest, EXPECTED_SHA, font):
		printerr(presenter.error)
		quit(1)
		return
	var parsed: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(manifest))
	parsed["provenance"]["room_packet"]["track_sha256"] = "0".repeat(64)
	if Presenter.new()._validate_manifest(parsed):
		printerr("Incorrect original track proof was accepted")
		quit(1)
		return
	if presenter.open_manifest(manifest, "0".repeat(64), font) or not presenter._manifest.is_empty():
		printerr("Failed manifest reopen retained content")
		quit(1)
		return
	if not _fresh_session_checks(manifest, font):
		quit(1)
		return
	print("PASS: fresh HUD content, session initialization, provenance rejection, and reopen clearing")
	quit(0)

func _fresh_session_checks(manifest: String, font: String) -> bool:
	# Synthetic caller state checks ownership; not an actual-gameplay witness.
	var kinds: Array = [30]
	var markers: Array = [0]
	for day: int in range(1, 32):
		kinds.append(0)
		markers.append(0)
	var rule := {"year": 95, "month": 4, "day": 4, "weekday": 2,
		"params": [65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536],
		"calendar": [{"day_kind": kinds, "marker": markers}, {"day_kind": kinds.duplicate(), "marker": markers.duplicate()}],
		"menu_cursor": 0, "icon_enabled": [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
		"player_flags": 0, "player_flags_upper": 0, "memory_card_busy": 0,
		"profile_family_bytes": [0x82, 0xa0, 0, 0, 0, 0, 0, 0]}
	var core := {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0}
	var opening := {"origin": "source-fresh-no-card-direct-entry-development-v1",
		"command_ready": true, "icon_mode": 0, "icon_count": 14}
	var live = Live.new()
	if not live.open_fresh(manifest, font, rule, core, opening, 0):
		printerr(live.error)
		return false
	if live.profile["family_name_hex"] != "82a0" or live.initial_params != rule["params"]:
		printerr("Fresh HUD did not use caller state")
		return false
	opening["command_ready"] = false
	if live.open_fresh(manifest, font, rule, core, opening, 0) or not live.profile.is_empty() or not live.presenter._manifest.is_empty():
		printerr("Incomplete command boundary accepted or retained old content")
		return false
	opening["command_ready"] = true
	rule["profile_family_bytes"] = [0x82, 0xa0, 0x82, 0xa0, 0x82, 0xa0, 0x82, 0xa0]
	if live.open_fresh(manifest, font, rule, core, opening, 0):
		printerr("Unterminated family name accepted")
		return false
	return true
