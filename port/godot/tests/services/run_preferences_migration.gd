extends SceneTree
## T-30002 WP1: preferences schema v2, per-key validation, v1 migration,
## presets, differs_from_original and atomic save rollback. Synthetic data only.
const Preferences = preload("res://services/user_preferences_service.gd")
const Presets = preload("res://services/settings_presets.gd")
const FIXTURES := "res://tests/fixtures/"
var failures: Array[String] = []

func check(condition: bool, label: String = "unlabelled check") -> void:
	if not condition: failures.append(label)

func _clear(path: String) -> void:
	for suffix: String in ["", ".backup", ".tmp", ".backup.tmp"]:
		DirAccess.remove_absolute(ProjectSettings.globalize_path(path + suffix))

func _install(fixture: String, path: String, backup: String = "") -> void:
	_clear(path)
	DirAccess.make_dir_recursive_absolute(ProjectSettings.globalize_path(path).get_base_dir())
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(FileAccess.get_file_as_string(FIXTURES + fixture)); file.close()
	if not backup.is_empty():
		var copy := FileAccess.open(path + ".backup", FileAccess.WRITE)
		copy.store_string(FileAccess.get_file_as_string(FIXTURES + backup)); copy.close()

func _schema_on_disk(path: String) -> String:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	return parsed.get("schema", "") if parsed is Dictionary else ""

## A valid non-default value for every key; the test fails if a key is added
## to defaults() without a row here.
const ALT := {
	"profile.preset": "custom",
	"display.mode": "borderless", "display.width": 1280, "display.height": 960, "display.monitor": 1,
	"display.aspect": "wide", "display.filter": "linear", "display.vsync": false, "display.fps": 144,
	"display.long_confirmation": true, "display.wing_fill": "source", "display.ui_scale": 1.5,
	"display.bars": null, "display.size_mode": "desktop",
	"audio.buses": {"Master": {"volume": 0.5, "mute": false}, "Voice": {"volume": 1.0, "mute": false}, "BGM": {"volume": 1.0, "mute": false}, "SFX": {"volume": 1.0, "mute": false}, "Movie": {"volume": 1.0, "mute": false}},
	"audio.device": "Other", "audio.mono": true, "audio.mute_unfocused": true,
	"controls.swap_confirm": true, "controls.deadzone": 0.5, "controls.repeat_delay": 0.6,
	"controls.repeat_interval": 0.1, "controls.bindings": {"left": [{"kind": "key", "code": KEY_H}]},
	"controls.prompt_style": "playstation",
	"accessibility.text_speed": "fast", "accessibility.auto_delay": 2.0, "accessibility.skip": "all",
	"accessibility.font_scale": 1.25, "accessibility.backdrop": 0.5, "accessibility.reduce_flashes": true,
	"accessibility.reduce_shake": true, "accessibility.auto_advance": true, "accessibility.subtitles": true,
	"accessibility.text_box": "modern",
	"language.bundle": "en", "language.allow_draft_images": true, "language.text": "en",
	"language.text_manifest": "/x.json", "language.text_sha256": "a", "language.voice": "en",
	"language.voice_manifest": "/v.json", "language.voice_sha256": "b", "language.dialogue_manifest": "/d.json",
	"language.dialogue_sha256": "c", "language.ui_text_manifest": "/u.json", "language.ui_text_sha256": "d",
}
## Keys whose change can alter the presented image (or the language) and so
## must be flagged by differs_from_original.
const EXPECTED_DIFFERS := ["profile.preset", "display.aspect", "display.filter", "display.wing_fill",
	"accessibility.text_speed", "accessibility.auto_delay", "accessibility.skip", "accessibility.font_scale",
	"accessibility.backdrop", "accessibility.reduce_flashes", "accessibility.reduce_shake",
	"accessibility.auto_advance", "accessibility.subtitles", "accessibility.text_box",
	"language.bundle", "language.allow_draft_images", "language.text", "language.text_manifest",
	"language.text_sha256", "language.voice", "language.voice_manifest", "language.voice_sha256",
	"language.dialogue_manifest", "language.dialogue_sha256", "language.ui_text_manifest", "language.ui_text_sha256"]

func _initialize() -> void:
	var path := "user://prefs-v2/desktop-settings.json"

	# Fresh install: v2 defaults equal the Original preset, nothing differs.
	_clear(path)
	var fresh := Preferences.new(path)
	check(fresh.load_preferences(), "fresh load")
	check(fresh.settings.profile.preset == "original" and Presets.matches(fresh.settings, "original"), "defaults are the Original preset")
	check(fresh.settings.language.bundle == "ja" and fresh.settings.display.aspect == "original" and fresh.settings.display.wing_fill == "black" and fresh.settings.accessibility.text_box == "native", "fresh install keeps current behaviour")
	check(fresh.fidelity_report().presentation_exact and fresh.fidelity_report().language_original, "fresh install badge is exact")

	# Complete v1 file: every value survives, including bindings and pack paths/sha.
	_install("settings_v1_complete.json", path)
	var v1_text := FileAccess.get_file_as_string(path)
	var migrated := Preferences.new(path)
	check(migrated.load_preferences(), "complete v1 loads")
	check(migrated.migrated and migrated.reset_keys.is_empty() and migrated.dropped_keys.is_empty(), "complete v1 migrates without resets: %s" % [migrated.reset_keys])
	var v1: Dictionary = JSON.parse_string(v1_text).settings
	for section: String in v1:
		for key: String in v1[section]:
			var expected: Variant = v1[section][key]
			var actual: Variant = migrated.settings[section][key]
			if section == "audio" and key == "buses":
				for bus: String in expected:
					check(is_equal_approx(actual[bus].volume, expected[bus].volume) and actual[bus].mute == expected[bus].mute, "bus %s preserved" % bus)
			elif expected is Dictionary:
				check(JSON.stringify(actual) == JSON.stringify(expected), "%s.%s preserved" % [section, key])
			else:
				check(Presets.same(actual, expected), "%s.%s preserved (%s != %s)" % [section, key, actual, expected])
	check(migrated.settings.controls.bindings.left[0].code == KEY_H, "v1 binding preserved as an integer code")
	check(migrated.settings.language.bundle == "custom", "user-picked v1 packs become the custom bundle")
	check(migrated.settings.accessibility.auto_advance, "v1 auto_delay > 0 keeps auto-advance on")
	# A voice pack is not a text language: alone it keeps the bundle 'ja'.
	var voice_only := Preferences.migrate_v1(JSON.parse_string(FileAccess.get_file_as_string(FIXTURES + "settings_v1_voice_only.json")).settings)
	check(not voice_only.is_empty() and voice_only.settings.language.bundle == "ja", "v1 voice-only pack migrates to bundle ja")
	check(voice_only.settings.language.voice_manifest == "/packs/voice/manifest.json", "v1 voice pack path survives")
	check(voice_only.settings.profile.preset == "original" and not voice_only.settings.accessibility.auto_advance, "voice-only v1 stays Original without auto-advance")
	check(migrated.settings.profile.preset == "custom", "non-original v1 values select the custom preset")
	check(_schema_on_disk(path) == Preferences.SCHEMA, "migration writes v2")
	check(FileAccess.get_file_as_string(path + ".backup") == v1_text, "v1 file kept as the backup")
	var again := Preferences.new(path)
	check(again.load_preferences() and not again.migrated and again.reset_keys.is_empty(), "v2 file reloads cleanly")
	check(JSON.stringify(again.settings) == JSON.stringify(migrated.settings), "v2 reload is identical")
	# migrate_v1 is pure: same input, same output, input untouched.
	var source: Dictionary = JSON.parse_string(v1_text).settings
	var before := JSON.stringify(source)
	check(JSON.stringify(Preferences.migrate_v1(source)) == JSON.stringify(Preferences.migrate_v1(source)), "migrate_v1 is deterministic")
	check(JSON.stringify(source) == before, "migrate_v1 leaves its input unchanged")
	check(Preferences.migrate_v1([1, 2]).is_empty(), "migrate_v1 rejects a non-dictionary")

	# Missing keys: only those keys reset, everything else survives.
	_install("settings_v1_missing_keys.json", path)
	var missing := Preferences.new(path)
	check(missing.load_preferences(), "v1 with missing keys loads")
	var missing_sorted := missing.reset_keys.duplicate(); missing_sorted.sort()
	check(missing_sorted == ["accessibility.reduce_shake", "audio.mono"], "only missing v1 keys reset: %s" % [missing.reset_keys])
	check(missing.settings.display.aspect == "auto" and missing.settings.display.filter == "linear" and missing.settings.accessibility.text_speed == "slow", "other values survive")
	check(is_equal_approx(missing.settings.audio.buses.Master.volume, 0.5) and missing.settings.accessibility.reduce_flashes, "neighbours of missing keys survive")
	check(missing.settings.audio.mono == false and missing.settings.accessibility.reduce_shake == false, "missing keys hold defaults")
	check(missing.settings.language.bundle == "ja", "no packs means the Japanese bundle")

	# Invalid values: per key, unknown keys dropped.
	_install("settings_v1_invalid_values.json", path)
	var invalid := Preferences.new(path)
	check(invalid.load_preferences(), "v1 with invalid values loads")
	for name: String in ["display.aspect", "display.fps", "audio.buses.Voice", "controls.swap_confirm", "controls.deadzone", "controls.bindings", "accessibility.text_speed"]:
		check(name in invalid.reset_keys, "%s reset" % name)
	check(invalid.reset_keys.size() == 7, "no other key reset: %s" % [invalid.reset_keys])
	check("display.hdr" in invalid.dropped_keys and "telemetry" in invalid.dropped_keys, "unknown keys dropped: %s" % [invalid.dropped_keys])
	check(invalid.settings.display.aspect == "original" and invalid.settings.display.fps == 60 and invalid.settings.controls.deadzone == 0.25, "invalid values hold defaults")
	check(invalid.settings.controls.bindings.is_empty(), "reserved F10 binding rejected")
	check(is_equal_approx(invalid.settings.audio.buses.BGM.volume, 0.75) and is_equal_approx(invalid.settings.accessibility.auto_delay, 2.0) and invalid.settings.display.filter == "nearest", "valid neighbours survive")
	var on_disk: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(path)).settings
	check(not on_disk.has("telemetry") and not on_disk.display.has("hdr"), "dropped keys are not written back")

	# A v2 file with a missing new key resets only that key.
	var v2_partial := Preferences.defaults(); v2_partial.display.erase("wing_fill"); v2_partial.accessibility.text_box = "sideways"
	v2_partial.display.aspect = "wide"
	_clear(path)
	var partial_file := FileAccess.open(path, FileAccess.WRITE)
	partial_file.store_string(JSON.stringify({"schema": Preferences.SCHEMA, "settings": v2_partial, "pending_display_boot_check": {}})); partial_file.close()
	var partial := Preferences.new(path)
	check(partial.load_preferences() and partial.reset_keys.size() == 2 and "display.wing_fill" in partial.reset_keys and "accessibility.text_box" in partial.reset_keys, "v2 per-key reset: %s" % [partial.reset_keys])
	check(partial.settings.display.aspect == "wide", "v2 neighbours survive")

	# Corrupt file: the existing .backup recovery path, then safe defaults.
	_install("settings_v1_corrupt.json", path, "settings_v1_complete.json")
	var corrupt := Preferences.new(path)
	check(corrupt.load_preferences(), "corrupt file recovers from backup")
	check(corrupt.recovered and corrupt.settings.display.width == 1280 and corrupt.settings.controls.bindings.left[0].code == KEY_H, "backup values restored")
	_install("settings_v1_corrupt.json", path)
	var lost := Preferences.new(path)
	check(not lost.load_preferences() and not lost.error.is_empty(), "corrupt file without backup reports an error")
	check(JSON.stringify(lost.settings) == JSON.stringify(Preferences.defaults()), "safe defaults are active")
	var wrong_schema := FileAccess.open(path, FileAccess.WRITE); wrong_schema.store_string(JSON.stringify({"schema": "tokimemo.port-settings.v9", "settings": Preferences.defaults()})); wrong_schema.close()
	check(not Preferences.new(path).load_preferences(), "unknown schema is not trusted")

	# Crash sentinel from a v1 session restores the previous v1 display.
	var v1_marker: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(FIXTURES + "settings_v1_complete.json"))
	var previous: Dictionary = v1_marker.settings.display.duplicate(true); previous.width = 960; previous.height = 720
	v1_marker.pending_display_boot_check = {"previous": previous, "candidate": v1_marker.settings.display, "deadline": 10.0}
	_clear(path)
	var marker_file := FileAccess.open(path, FileAccess.WRITE); marker_file.store_string(JSON.stringify(v1_marker)); marker_file.close()
	var sentinel := Preferences.new(path)
	check(sentinel.load_preferences() and sentinel.recovered and sentinel.settings.display.width == 960 and sentinel.settings.display.mode == "borderless", "v1 sentinel restores the previous display")

	# Presets round-trip through set_value.
	_clear(path)
	var prefs := Preferences.new(path); prefs.load_preferences()
	var emitted := []
	prefs.changed.connect(func(value: Dictionary) -> void: emitted.append(value))
	check(prefs.apply_preset("modern") and prefs.settings.profile.preset == "modern" and Presets.matches(prefs.settings, "modern"), "Modern preset applies")
	check(emitted.size() == 1, "one change event per preset")
	for name: String in ["original", "modern"]:
		check(prefs.apply_preset(name), "apply %s" % name)
		var bundle := Presets.bundle(name)
		for key: String in bundle:
			var parts := key.split(".")
			check(prefs.set_value(parts[0], parts[1], bundle[key]), "%s %s round-trips through set_value" % [name, key])
		check(prefs.settings.profile.preset == name, "%s survives re-setting its own values" % name)
		var reread := Preferences.new(path); reread.load_preferences()
		check(reread.settings.profile.preset == name and Presets.matches(reread.settings, name), "%s persists" % name)
	check(prefs.apply_preset("original"))
	check(prefs.set_value("audio", "mono", true) and prefs.set_value("display", "ui_scale", 1.5) and prefs.settings.profile.preset == "original", "desktop-only edits keep the preset")
	check(prefs.set_value("display", "wing_fill", "source") and prefs.settings.profile.preset == "custom", "editing a preset field selects custom")
	check(prefs.apply_preset("modern"))
	check(prefs.set_value("accessibility", "reduce_shake", true) and prefs.settings.profile.preset == "custom", "accessibility edit selects custom")
	check(prefs.apply_preset("original") and prefs.set_value("accessibility", "text_box", "modern") and prefs.settings.profile.preset == "custom", "text_box edit selects custom")
	check(prefs.apply_preset("original") and prefs.restore_category("accessibility") and prefs.settings.profile.preset == "original", "restoring defaults keeps Original")
	check(not prefs.apply_preset("ultra"), "unknown preset rejected")
	check(not prefs.set_value("profile", "preset", "modern"), "preset only through apply_preset")
	var custom_before := JSON.stringify(prefs.settings)
	check(prefs.apply_preset("custom") and prefs.settings.profile.preset == "custom", "custom preset applies")
	prefs.settings.profile.preset = "original"
	check(JSON.stringify(prefs.settings) == custom_before, "custom preset keeps every value")

	# Widescreen and language keys: validated, live, defaults.
	check(prefs.set_value("display", "aspect", "auto") and prefs.set_value("display", "ui_scale", 2.0), "live display keys apply without a preview")
	check(prefs.pending.is_empty(), "live keys need no Keep/Revert")
	check(not prefs.set_value("display", "wing_fill", "stretch") and prefs.settings.display.wing_fill != "stretch", "wing_fill rejects invented values")
	check(not prefs.set_value("accessibility", "text_box", "large"), "text_box rejects unknown values")
	check(not prefs.set_value("display", "ui_scale", 2.5) and not prefs.set_value("display", "ui_scale", 0.5), "ui_scale range enforced")
	check(not prefs.set_value("display", "width", 1280), "window size still needs the display preview")
	check(not prefs.set_value("language", "bundle", "fr") and prefs.set_value("language", "bundle", "en"), "language bundle validated")
	check(not prefs.set_value("controls", "prompt_style", "steam") and prefs.set_value("controls", "prompt_style", "nintendo"), "prompt style validated")
	check(prefs.set_values({"language.bundle": "custom", "language.ui_text_manifest": "/u.json", "language.ui_text_sha256": "f".repeat(64)}), "atomic multi-key write")
	check(not prefs.set_values({"language.bundle": "ja", "language.ui_text_manifest": 7}) and prefs.settings.language.bundle == "custom", "invalid multi-key write changes nothing")

	# Live display key during a window preview reaches both sides of the preview.
	var display: Dictionary = prefs.settings.display.duplicate(true); display.width = 1600; display.height = 900
	check(prefs.begin_display(display, 0.0), "preview starts")
	check(prefs.set_value("display", "wing_fill", "source") and prefs.effective_settings().display.wing_fill == "source", "live key visible during preview")
	check(prefs.revert_display() and prefs.settings.display.wing_fill == "source" and prefs.settings.display.width != 1600, "revert keeps the live key and drops the size")

	# A window preview that also changes a preset key: the preset row and the
	# badge agree during the preview, after confirm and after revert.
	var framing := Preferences.new("user://prefs-v2/framing.json"); _clear(framing.path); framing.load_preferences()
	check(framing.apply_preset("original"), "Original preset applied")
	var wide: Dictionary = framing.settings.display.duplicate(true); wide.aspect = "wide"; wide.width = 1280; wide.height = 720
	check(framing.begin_display(wide, 0.0), "wide preview starts under Original")
	var during := framing.fidelity_report()
	check(framing.effective_settings().profile.preset == "custom" and during.preset == "custom" and not during.presentation_exact, "preview shows the custom preset with a non-exact badge")
	check(framing.settings.profile.preset == "original", "the saved preset is unchanged until confirm")
	check(framing.revert_display() and framing.effective_settings().profile.preset == "original" and framing.fidelity_report().presentation_exact, "revert restores Original and the exact badge")
	check(framing.begin_display(wide, 0.0) and framing.confirm_display(), "wide preview confirmed")
	check(framing.settings.profile.preset == "custom" and framing.settings.display.aspect == "wide", "confirming a wide preview selects custom")
	var reloaded_framing := Preferences.new(framing.path)
	check(reloaded_framing.load_preferences() and reloaded_framing.settings.profile.preset == "custom", "custom preset persists after confirm")
	var sized := Preferences.new("user://prefs-v2/sized.json"); _clear(sized.path); sized.load_preferences()
	var bigger: Dictionary = sized.settings.display.duplicate(true); bigger.width = 1280; bigger.height = 960
	check(sized.begin_display(bigger, 0.0) and sized.effective_settings().profile.preset == "original" and sized.confirm_display() and sized.settings.profile.preset == "original", "a window-only preview keeps the Original preset")

	# differs_from_original for every key.
	var base := Preferences.defaults()
	var all_keys: Array[String] = []
	for section: String in base:
		for key: String in base[section]: all_keys.append(section + "." + key)
	for name: String in all_keys:
		check(ALT.has(name), "ALT covers %s" % name)
		check(not Preferences.static_differs(base, name), "%s does not differ at defaults" % name)
		if not ALT.has(name) or ALT[name] == null: continue
		var changed := base.duplicate(true)
		var parts := name.split(".")
		changed[parts[0]][parts[1]] = ALT[name]
		check(Preferences.static_differs(changed, name) == (name in EXPECTED_DIFFERS), "differs_from_original(%s)" % name)
	check(not Preferences.static_differs(base, "display.nonexistent") and not Preferences.static_differs(base, "bogus"), "unknown keys never differ")
	var badge := Preferences.new("user://prefs-v2/badge.json"); _clear(badge.path); badge.load_preferences()
	check(badge.set_value("language", "bundle", "en"), "language set")
	var report := badge.fidelity_report()
	check(report.presentation_exact and not report.language_original, "language is reported separately from presentation")
	check(badge.apply_preset("original") and badge.settings.language.bundle == "en", "Original preset leaves the language unchanged")
	check(badge.apply_preset("modern") and not badge.fidelity_report().presentation_exact, "Modern preset is not presentation-exact")
	check(badge.differs_from_original("display.aspect") and not badge.differs_from_original("display.width"), "instance query follows effective settings")

	# Strict validate (edits): v1-shaped dictionaries pass, invalid values do not.
	var v1_shaped: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(FIXTURES + "settings_v1_missing_keys.json")).settings
	v1_shaped.audio.mono = false; v1_shaped.accessibility.reduce_shake = false
	var strict := Preferences.validate(v1_shaped)
	check(not strict.is_empty() and strict.language.bundle == "ja" and strict.display.wing_fill == "black", "v1-shaped dictionary validates with v2 defaults")
	v1_shaped.accessibility.erase("reduce_flashes")
	check(Preferences.validate(v1_shaped).is_empty(), "a missing key that v1 required is rejected")
	var bad := Preferences.defaults(); bad.display.wing_fill = "mirror"
	check(Preferences.validate(bad).is_empty(), "strict validate rejects an invalid value")
	var extra := Preferences.defaults(); extra.display["hdr"] = true
	check(Preferences.validate(extra).is_empty(), "strict validate rejects unknown keys")
	check(Preferences.sanitize("text").is_empty(), "non-dictionary is structural corruption")

	# Atomic save: a write failure rolls back in memory.
	var blocker := "user://prefs-v2-blocked"
	DirAccess.remove_absolute(ProjectSettings.globalize_path(blocker))
	var block := FileAccess.open(blocker, FileAccess.WRITE); block.store_string("not a directory"); block.close()
	var stuck := Preferences.new(blocker + "/settings.json")
	check(stuck.load_preferences(), "missing file under a blocked directory loads defaults")
	var snapshot := JSON.stringify(stuck.settings)
	var stuck_events := []
	stuck.changed.connect(func(value: Dictionary) -> void: stuck_events.append(value))
	check(not stuck.set_value("audio", "mono", true) and JSON.stringify(stuck.settings) == snapshot, "failed save rolls back set_value")
	check(not stuck.apply_preset("modern") and JSON.stringify(stuck.settings) == snapshot, "failed save rolls back a preset")
	check(not stuck.set_values({"language.bundle": "en", "language.allow_draft_images": true}) and JSON.stringify(stuck.settings) == snapshot, "failed save rolls back a multi-key write")
	check(stuck_events.is_empty() and not stuck.error.is_empty(), "no change event after a failed save")
	DirAccess.remove_absolute(ProjectSettings.globalize_path(blocker))

	if failures.is_empty():
		print("Preferences v2: defaults, v1 complete/missing/invalid/corrupt migration, sentinel, presets, live widescreen/language keys, differs_from_original, atomic rollback passed")
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: " + failure)
		quit(1)
