extends "res://tests/services/settings_test_tree.gd"
## T-39230: Parity mode in the preferences service and the settings runtime.
## Persistence and migration of profile.mode, the atomic switch into Parity,
## refusal of every locked key and refused feature (with negatives), the save
## guard against direct writes, re-locking a hand-edited Parity file, and a
## launch-forced session that never rewrites the saved profile.
const Preferences = preload("res://services/user_preferences_service.gd")
const Parity = preload("res://services/parity_mode.gd")
const Registry = preload("res://services/feature_registry.gd")
const Fakes = preload("res://tests/services/settings_v2_fakes.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const TitleContinue = preload("res://app/title_continue.gd")
const PATH := "user://parity-mode-test.json"

class SaveHost extends Node:
	var calls: Array = []
	func _ready() -> void: add_to_group("settings_save_hosts")
	func request_settings_save(load_game: bool) -> bool:
		calls.append(load_game); return true

class SkipPad extends Node:
	var pulses: Array = []
	func request_button_pulse(mask: int) -> void: pulses.append(mask)

func _initialize() -> void: call_deferred("run")

func _fail(message: String) -> void:
	printerr(message); check(false)

func _clean(path: String) -> void:
	for suffix: String in ["", ".backup", ".tmp", ".backup.tmp"]:
		if FileAccess.file_exists(path + suffix): DirAccess.remove_absolute(ProjectSettings.globalize_path(path + suffix))

func _write_file(path: String, record: Dictionary) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE); file.store_string(JSON.stringify(record)); file.close()

func _disk(path: String) -> Dictionary:
	return JSON.parse_string(FileAccess.get_file_as_string(path)).settings

func run() -> void:
	_launch_options()
	_defaults_and_migration()
	_switch_and_refusals()
	_save_guard_and_relock()
	_forced_session()
	_forced_display_marker()
	await _runtime_refusals()
	await _quick_save_keys()
	print("Parity mode: launch options, persistence/migration, atomic switch, locked-key and feature refusals, save guard, relock, forced session, forced display marker, F5/F9 and Continue refusals passed")
	finish()

func _launch_options() -> void:
	check(Parity.forced_by(PackedStringArray(["--parity"]), {}) == "--parity")
	check(Parity.forced_by(PackedStringArray([]), {"TOKIMEMO_PARITY": "1"}) == "TOKIMEMO_PARITY")
	# Negatives: other values and look-alike arguments do not force Parity.
	check(Parity.forced_by(PackedStringArray([]), {"TOKIMEMO_PARITY": "0"}) == "")
	check(Parity.forced_by(PackedStringArray([]), {"TOKIMEMO_PARITY": "true"}) == "")
	check(Parity.forced_by(PackedStringArray(["--parity=0", "--paritymode"]), {}) == "")
	check(Parity.invariance_run({"MODERN_INVARIANCE": "1"}) and not Parity.invariance_run({"MODERN_INVARIANCE": "yes"}))

func _defaults_and_migration() -> void:
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.settings.profile.mode == "modern" and not prefs.parity_active())
	# A v2 file written before profile.mode existed: migrated to 'modern', not a reset.
	var legacy := Preferences.defaults(); legacy.profile.erase("mode"); legacy.display.aspect = "wide"
	_write_file(PATH, {"schema": Preferences.SCHEMA, "settings": legacy})
	prefs = Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.settings.profile.mode == "modern" and "profile.mode" not in prefs.reset_keys and prefs.settings.display.aspect == "wide")
	# A v1 file migrates to v2 with mode 'modern'.
	var v1 := Preferences.defaults(); v1.erase("profile")
	_write_file(PATH, {"schema": Preferences.SCHEMA_V1, "settings": v1})
	prefs = Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.migrated and prefs.settings.profile.mode == "modern")
	check(_disk(PATH).profile.mode == "modern")
	# Negative: an invalid mode is reset and reported, never taken as Parity.
	var bad := Preferences.defaults(); bad.profile.mode = "turbo"
	_write_file(PATH, {"schema": Preferences.SCHEMA, "settings": bad})
	prefs = Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.settings.profile.mode == "modern" and "profile.mode" in prefs.reset_keys)
	check(not prefs.set_mode("turbo") and prefs.error == "Unknown play mode.")

func _switch_and_refusals() -> void:
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences())
	# Modern profile with enhancements on, so the switch has work to do.
	check(prefs.apply_preset("modern"))
	check(prefs.set_values({"accessibility.text_speed": "fast", "accessibility.auto_advance": true, "audio.mono": true,
		"language.bundle": "en", "display.wing_art": "seasonal", "accessibility.font_scale": 1.5}))
	check(prefs.apply_profile({"display.fps": 144}))
	check(not Parity.violations(prefs.settings, Preferences.defaults()).is_empty())
	var bus_volume: Dictionary = prefs.settings.audio.buses.duplicate(true); bus_volume.BGM.volume = 0.4
	check(prefs.set_value("audio", "buses", bus_volume))
	check(prefs.set_mode("parity") and prefs.parity_active())
	check(Parity.violations(prefs.settings, Preferences.defaults()).is_empty())
	var locked := Parity.locked_values(Preferences.defaults())
	for name: String in locked:
		var parts := name.split(".")
		if not Parity.same(prefs.settings[parts[0]][parts[1]], locked[name]): _fail("parity left %s at %s" % [name, prefs.settings[parts[0]][parts[1]]])
	# Host settings survive the switch.
	check(is_equal_approx(prefs.settings.audio.buses.BGM.volume, 0.4))
	# Persisted: a new instance loads Parity.
	var reloaded := Preferences.new(PATH); check(reloaded.load_preferences())
	check(reloaded.parity_active() and reloaded.parity_relocked.is_empty() and reloaded.reset_keys.is_empty())
	# Every locked key refuses a non-original value, one by one.
	var modern_values := {}
	for name: String in Registry.PREFERENCES:
		if Registry.parity_policy(name) == "original" and Registry.PREFERENCES[name].has("modern"): modern_values[name] = Registry.PREFERENCES[name].modern
	modern_values["accessibility.text_speed"] = "instant"; modern_values["accessibility.auto_advance"] = true
	modern_values["accessibility.skip"] = "all"; modern_values["accessibility.auto_delay"] = 2.0
	var before := JSON.stringify(prefs.settings, "", true)
	for name: String in modern_values:
		if Parity.same(modern_values[name], locked[name]): continue
		var parts: PackedStringArray = name.split(".")
		var refused := prefs.apply_profile({name: modern_values[name]}) if parts[0] == "display" else prefs.set_value(parts[0], parts[1], modern_values[name])
		if refused: _fail("parity accepted %s = %s" % [name, modern_values[name]])
		elif prefs.error != Parity.REFUSAL: _fail("%s refused with '%s'" % [name, prefs.error])
		if prefs.parity_refusal(name, modern_values[name]).is_empty(): _fail("parity_refusal empty for " + name)
	check(JSON.stringify(prefs.settings, "", true) == before)
	# One locked key in a batch refuses the whole write.
	check(not prefs.set_values({"controls.prompt_style": "xbox", "accessibility.reduce_flashes": true}))
	check(prefs.settings.controls.prompt_style == "auto")
	# Host keys stay adjustable; original values of locked keys are accepted.
	check(prefs.set_value("controls", "prompt_style", "xbox") and prefs.set_value("audio", "mute_unfocused", true))
	check(prefs.set_value("accessibility", "text_speed", "normal"))
	check(prefs.parity_refusal("controls.deadzone", 0.5).is_empty())
	# Presets: only Original.
	check(not prefs.apply_preset("modern") and prefs.error == Parity.REFUSAL)
	check(not prefs.apply_preset("custom") and prefs.apply_preset("original"))
	# Display preview: window size yes, framing or frame limit no.
	var display: Dictionary = prefs.settings.display.duplicate(true); display.width = 1280
	check(prefs.begin_display(display, 0.0) and prefs.confirm_display() and prefs.settings.display.width == 1280)
	display.aspect = "wide"; check(not prefs.begin_display(display, 0.0) and prefs.pending.is_empty())
	display.aspect = "original"; display.fps = 0; check(not prefs.begin_display(display, 0.0))
	# Restoring a category keeps Parity clean.
	check(prefs.restore_category("accessibility") and Parity.violations(prefs.settings, Preferences.defaults()).is_empty())
	# Feature refusals.
	for feature: String in ["feature.quick_save", "feature.quick_load", "feature.return_to_title", "feature.prologue_skip_shortcut"]:
		check(Parity.refusal(feature, true) == Parity.REFUSAL and Parity.refusal(feature, false).is_empty())
	for feature: String in ["feature.settings_overlay", "feature.pause_menu", "feature.quit_game"]:
		check(Parity.refusal(feature, true).is_empty())
	# Negative: an unregistered feature is refused in either mode.
	check(not Parity.refusal("feature.fast_forward", false).is_empty() and not Parity.refusal("feature.fast_forward", true).is_empty())
	# Leaving Parity keeps the original values and unlocks every option.
	check(prefs.set_mode("modern") and not prefs.parity_active())
	check(prefs.settings.display.aspect == "original" and prefs.set_value("accessibility", "text_speed", "fast"))

func _save_guard_and_relock() -> void:
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences()); check(prefs.set_mode("parity"))
	var disk_before := FileAccess.get_file_as_string(PATH)
	# A direct write (as the pack pickers do) cannot be persisted in Parity.
	prefs.settings.language.text_manifest = "/somewhere/english.json"
	check(not prefs.save() and prefs.error == Parity.REFUSAL)
	check(FileAccess.get_file_as_string(PATH) == disk_before)
	prefs.settings.language.text_manifest = ""
	# An unregistered key is a violation too.
	var odd := Preferences.defaults(); odd.display["turbo"] = true
	check(Parity.violations(odd, Preferences.defaults()).has("display.turbo (unregistered)"))
	# A hand-edited Parity file is re-locked on load and the keys reported.
	var edited := _disk(PATH); edited.display.aspect = "wide"; edited.accessibility.text_speed = "instant"
	_write_file(PATH, {"schema": Preferences.SCHEMA, "settings": edited})
	var reloaded := Preferences.new(PATH); check(reloaded.load_preferences())
	check(reloaded.parity_active() and reloaded.settings.display.aspect == "original" and reloaded.settings.accessibility.text_speed == "normal")
	check("display.aspect" in reloaded.parity_relocked and "accessibility.text_speed" in reloaded.parity_relocked)
	check("display.aspect" in reloaded.reset_keys)
	check(_disk(PATH).display.aspect == "original")

func _forced_session() -> void:
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.apply_preset("modern") and prefs.set_value("accessibility", "text_speed", "fast"))
	prefs.force_parity("--parity")
	check(prefs.parity_active() and prefs.parity_forced_by == "--parity")
	check(prefs.settings.display.aspect == "original" and prefs.settings.accessibility.text_speed == "normal")
	check(prefs.effective_settings().profile.mode == "parity")
	# The switch belongs to the launch option for this session.
	check(not prefs.set_mode("modern") and prefs.error == Parity.FORCED_REFUSAL)
	check(not prefs.set_mode("parity") and prefs.error == Parity.FORCED_REFUSAL)
	check(not prefs.apply_profile({"profile.mode": "modern"}))
	check(not prefs.set_value("accessibility", "text_speed", "fast"))
	# A host change is saved into the saved profile, which keeps its modern values.
	var buses: Dictionary = prefs.settings.audio.buses.duplicate(true); buses.Voice.volume = 0.25
	check(prefs.set_value("audio", "buses", buses))
	var disk := _disk(PATH)
	check(disk.profile.mode == "modern" and disk.display.aspect == "auto" and disk.accessibility.text_speed == "fast")
	check(is_equal_approx(disk.audio.buses.Voice.volume, 0.25))
	# Reloading while forced keeps the session locked.
	check(prefs.load_preferences() and prefs.parity_active() and prefs.settings.display.aspect == "original")
	# Unforced next launch: the modern profile is back.
	var next := Preferences.new(PATH); check(next.load_preferences())
	check(not next.parity_active() and next.settings.display.aspect == "auto" and is_equal_approx(next.settings.audio.buses.Voice.volume, 0.25))
	# Negative: an empty reason does not force anything.
	next.force_parity(""); check(not next.parity_active())

## A forced session that starts a window preview and is interrupted (crash or
## quit during Keep/Revert) must not leak the locked display values into the
## saved profile through the display recovery marker.
func _forced_display_marker() -> void:
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences())
	check(prefs.apply_preset("modern"))
	check(prefs.set_values({"display.aspect": "auto", "display.filter": "linear"}))
	prefs.force_parity("TOKIMEMO_PARITY")
	var candidate: Dictionary = prefs.settings.display.duplicate(true)
	candidate.width = 1280; candidate.height = 960
	check(prefs.begin_display(candidate, 0.0))
	var marker: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(PATH)).pending_display_boot_check
	check(marker.previous.aspect == "auto" and marker.previous.filter == "linear")
	check(marker.candidate.aspect == "auto" and marker.candidate.filter == "linear" and int(marker.candidate.width) == 1280)
	# Unforced relaunch recovers from the marker and keeps the modern values.
	var next := Preferences.new(PATH); next.load_preferences()
	check(next.recovered and not next.parity_active())
	check(next.settings.profile.mode == "modern" and next.settings.display.aspect == "auto" and next.settings.display.filter == "linear")
	# Negative control: an unforced Parity profile still records its own (locked) display.
	_clean(PATH)
	var saved := Preferences.new(PATH); check(saved.load_preferences() and saved.set_mode("parity"))
	var unforced_candidate: Dictionary = saved.settings.display.duplicate(true); unforced_candidate.width = 1280
	check(saved.begin_display(unforced_candidate, 0.0))
	marker = JSON.parse_string(FileAccess.get_file_as_string(PATH)).pending_display_boot_check
	check(marker.previous.aspect == "original")

## F5/F9 reach BoundarySaveControls.handle_input before anything else in
## playable_boot._input, and title Continue loads through request(). Parity
## must refuse all three, read from the ModernSettings autoload.
func _quick_save_keys() -> void:
	var settings: Node = root.get_node_or_null("ModernSettings")
	var added := settings == null
	if added:
		settings = Fakes.FakeRuntime.new(); settings.name = "ModernSettings"; root.add_child(settings)
		await process_frame
	var saved_preferences: Object = settings.preferences
	_clean(PATH)
	var prefs := Preferences.new(PATH); check(prefs.load_preferences())
	settings.preferences = prefs
	var app := Node.new(); root.add_child(app)
	var title: Control = TitleContinue.new(); title.save_root = OS.get_user_data_dir().path_join("parity-title-saves")
	check(prefs.set_mode("parity"))
	for key: int in [KEY_F5, KEY_F9]:
		var controls := Controls.new()
		check(controls.handle_input(app, _key(key)))
		check(controls.pending == Controls.Request.NONE and controls.last_error == Parity.REFUSAL)
	check(not Controls.new().request(app, Controls.Request.SAVE))
	check(Controls.parity_refusal(app, Controls.Request.LOAD) == Parity.REFUSAL)
	root.add_child(title)
	check(title.continue_check().error == Parity.REFUSAL)
	# Modern: the same keys pass the Parity check and stop at the next guard
	# (no storage configured here), so the refusal above is Parity's alone.
	check(prefs.set_mode("modern"))
	var modern := Controls.new()
	check(modern.handle_input(app, _key(KEY_F5)) and modern.last_error == "Save controls are unavailable.")
	check(Controls.parity_refusal(app, Controls.Request.LOAD) == "")
	check(title.continue_check().error != Parity.REFUSAL)
	settings.preferences = saved_preferences
	app.queue_free(); title.queue_free()
	if added: settings.queue_free()
	await process_frame

func _key(code: int) -> InputEventKey:
	var event := InputEventKey.new(); event.physical_keycode = code as Key; event.pressed = true
	return event

func _runtime_refusals() -> void:
	var runtime := Fakes.FakeRuntime.new(); runtime.name = "ParityRuntime"; root.add_child(runtime)
	await process_frame
	var host := SaveHost.new(); root.add_child(host); await process_frame
	check(runtime.preferences.set_mode("parity"))
	runtime.request_save(false); runtime.request_save(true)
	check(host.calls.is_empty())
	var pad := SkipPad.new(); root.add_child(pad)
	check(not runtime.request_prologue_skip(pad) and pad.pulses.is_empty())
	for selector: String in ["select_image_pack", "select_text_pack", "select_ui_text_pack", "select_voice_pack"]:
		check(str(runtime.call(selector, "/no/such/pack.json")) == Parity.REFUSAL)
	check(runtime.preferences.settings.language.text_manifest == "")
	# Modern: the same requests reach the save host (negative control).
	check(runtime.preferences.set_mode("modern"))
	runtime.request_save(false)
	check(host.calls == [false])
	runtime.queue_free(); host.queue_free(); pad.queue_free()
	await process_frame
