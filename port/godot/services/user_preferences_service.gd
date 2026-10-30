extends RefCounted
## T-30002: desktop preferences never enter native game/save/RNG state.
## Schema v2 (WP1): per-key validation with reset_keys, pure v1 migration,
## Original/Modern/Custom presets and differs-from-original queries.
signal changed(settings: Dictionary)
const SCHEMA := "tokimemo.port-settings.v2"
const SCHEMA_V1 := "tokimemo.port-settings.v1"
const Remaps = preload("res://services/input_remap_service.gd")
const Presets = preload("res://services/settings_presets.gd")
const BUSES := ["Master", "Voice", "BGM", "SFX", "Movie"]
const ENUMS := {
	"profile.preset": ["original", "modern", "custom"],
	"display.mode": ["windowed", "borderless", "exclusive"],
	## Widescreen framing (T-30001 WP5): the existing enum is kept, so
	## display.aspect is the framing key and display.filter the scaling key.
	"display.aspect": ["original", "wide", "auto"],
	"display.filter": ["integer", "nearest", "linear"],
	"display.fps": [60, 120, 144, 0],
	"display.wing_fill": ["black", "source"],
	"display.bars": ["black"],
	"display.size_mode": ["preset", "desktop", "custom"],
	"controls.prompt_style": ["auto", "keyboard", "xbox", "playstation", "nintendo"],
	"accessibility.text_speed": ["slow", "normal", "fast", "instant"],
	"accessibility.skip": ["read", "all"],
	"accessibility.text_box": ["native", "modern"],
	"language.bundle": ["ja", "en", "custom"],
}
const RANGES := {"display.width": [640, 7680], "display.height": [480, 4320],
	"display.monitor": [0, 31], "display.ui_scale": [1.0, 2.0], "controls.deadzone": [0.05, 0.9],
	"controls.repeat_delay": [0.15, 1.0], "controls.repeat_interval": [0.03, 0.3],
	"accessibility.auto_delay": [0.0, 10.0], "accessibility.font_scale": [1.0, 1.5],
	"accessibility.backdrop": [0.0, 1.0]}
## Display keys that never resize or move the window apply immediately through
## set_value; the others (mode, size, monitor, vsync, fps, size_mode) keep the
## Keep/Revert preview through begin_display.
const LIVE_DISPLAY_KEYS := ["aspect", "filter", "wing_fill", "ui_scale", "bars"]
## Keys introduced by v2. A v1 file lacking them is not reported as reset.
const V2_KEYS := ["profile.preset", "display.wing_fill", "display.ui_scale", "display.bars",
	"display.size_mode", "controls.prompt_style", "accessibility.auto_advance",
	"accessibility.subtitles", "accessibility.text_box", "language.bundle", "language.allow_draft_images"]
## v1 accepted files without these keys.
const V1_OPTIONAL_KEYS := ["language.dialogue_manifest", "language.dialogue_sha256",
	"language.ui_text_manifest", "language.ui_text_sha256"]
## Keys that cannot change the native 320x240 image, tick, RNG or save image.
const FIDELITY_NEUTRAL_SECTIONS := ["audio", "controls"]
const FIDELITY_NEUTRAL_DISPLAY := ["mode", "width", "height", "monitor", "vsync", "fps",
	"long_confirmation", "ui_scale", "size_mode"]
var path: String
var settings: Dictionary = defaults()
var pending: Dictionary = {}
var error := ""
var recovered := false
## True after load_preferences converted a v1 file (the v1 file becomes .backup).
var migrated := false
## "section.key" names that were missing or invalid on load and now hold defaults.
var reset_keys: Array[String] = []
## Unknown "section.key" names dropped on load.
var dropped_keys: Array[String] = []

static func defaults() -> Dictionary:
	var buses := {}
	for bus: String in BUSES: buses[bus] = {"volume": 1.0, "mute": false}
	return {"profile": {"preset": "original"},
		"display": {"mode": "windowed", "width": 960, "height": 720, "monitor": 0,
		"aspect": "original", "filter": "integer", "vsync": true, "fps": 60, "long_confirmation": false,
		"wing_fill": "black", "ui_scale": 1.0, "bars": "black", "size_mode": "preset"},
		"audio": {"buses": buses, "device": "Default", "mono": false, "mute_unfocused": false},
		"controls": {"swap_confirm": false, "deadzone": 0.25, "repeat_delay": 0.4,
			"repeat_interval": 0.08, "bindings": {}, "prompt_style": "auto"},
		"accessibility": {"text_speed": "normal", "auto_delay": 0.0, "skip": "read",
			"font_scale": 1.0, "backdrop": 1.0, "reduce_flashes": false, "reduce_shake": false,
			"auto_advance": false, "subtitles": false, "text_box": "native"},
		"language": {"bundle": "ja", "allow_draft_images": false,
			"text": "ja", "text_manifest": "", "text_sha256": "", "voice": "ja",
			"voice_manifest": "", "voice_sha256": "", "dialogue_manifest": "", "dialogue_sha256": "",
			"ui_text_manifest": "", "ui_text_sha256": ""}}

func _init(storage_path: String = "user://desktop-settings.json") -> void:
	path = storage_path

static func valid_value(key: String, value: Variant, original: Variant) -> bool:
	if original is int and value is float and is_finite(value) and value == floor(value): value = int(value)
	if ENUMS.has(key): return value in ENUMS[key]
	if RANGES.has(key):
		return (value is int or value is float) and is_finite(float(value)) and value >= RANGES[key][0] and value <= RANGES[key][1] and (not original is int or value == floor(value))
	if original is bool: return value is bool
	if original is String: return value is String and value.length() <= 4096
	return false

static func _normalized(value: Variant, original: Variant) -> Variant:
	if original is int: return int(value)
	if original is float: return float(value)
	return value

## Per-key validation. Returns {} only when the candidate is not a Dictionary
## (structural corruption); otherwise {"settings", "reset_keys", "dropped_keys"}
## where every missing or invalid key holds its default and is listed.
static func sanitize(candidate: Variant) -> Dictionary:
	if not candidate is Dictionary: return {}
	var result := defaults()
	var reset: Array[String] = []
	var dropped: Array[String] = []
	for section: Variant in candidate:
		if not (section is String or section is StringName) or not result.has(section):
			dropped.append(str(section)); continue
		if candidate[section] is Dictionary:
			for key: Variant in candidate[section]:
				if not (key is String or key is StringName) or not result[section].has(key): dropped.append("%s.%s" % [section, key])
	for section: String in result:
		var source: Variant = candidate.get(section)
		for key: String in result[section]:
			var name := section + "." + key
			if not source is Dictionary or not source.has(key):
				reset.append(name); continue
			var value: Variant = source[key]
			if name == "audio.buses":
				if not value is Dictionary: reset.append(name); continue
				for bus: String in BUSES:
					var spec: Variant = value.get(bus)
					var volume: Variant = spec.get("volume") if spec is Dictionary else null
					if not spec is Dictionary or not spec.get("mute") is bool or not (volume is float or volume is int) or not is_finite(float(volume)) or volume < 0 or volume > 1:
						reset.append(name + "." + bus); continue
					result.audio.buses[bus] = {"volume": float(volume), "mute": spec.mute}
			elif name == "controls.bindings":
				if not value is Dictionary or value.size() > 24 or Remaps.validated_bindings(value).is_empty():
					reset.append(name); continue
				result.controls.bindings = value.duplicate(true)
			elif valid_value(name, value, result[section][key]):
				result[section][key] = _normalized(value, result[section][key])
			else:
				reset.append(name)
	return {"settings": result, "reset_keys": reset, "dropped_keys": dropped}

## Strict form used for edits: {} when any key is invalid or unknown, or a
## key is missing that both v1 and v2 require. Keys v1 treated as optional and
## keys added by v2 may be absent and take their defaults, so v1-shaped
## dictionaries from older callers still validate.
static func validate(candidate: Variant) -> Dictionary:
	var report := sanitize(candidate)
	if report.is_empty() or not report.dropped_keys.is_empty(): return {}
	for name: String in report.reset_keys:
		var parts := name.split(".")
		var absent: bool = not candidate.has(parts[0]) or (candidate[parts[0]] is Dictionary and not candidate[parts[0]].has(parts[1]))
		if not absent or (name not in V1_OPTIONAL_KEYS and name not in V2_KEYS): return {}
	return report.settings

## Pure v1 -> v2 conversion: v2 defaults plus every valid v1 value, including
## bindings and custom pack paths/sha. Only v1 keys can appear in reset_keys.
static func migrate_v1(v1_settings: Variant) -> Dictionary:
	var report := sanitize(v1_settings)
	if report.is_empty(): return {}
	var result: Dictionary = report.settings
	var reset: Array[String] = []
	for name: String in report.reset_keys:
		if name not in V2_KEYS and name not in V1_OPTIONAL_KEYS and not name.begins_with("profile."): reset.append(name)
	# v1 had no bundle choice: a user-picked text, dialogue or artwork pack keeps
	# working as 'custom'. Voice is not a text language (it stays Japanese
	# Original), so a voice pack alone keeps the bundle 'ja'.
	var language: Dictionary = result.language
	for key: String in ["text_manifest", "dialogue_manifest", "ui_text_manifest"]:
		if not language[key].is_empty(): language.bundle = "custom"
	# v1 auto-advanced whenever auto_delay > 0; keep that behaviour explicit.
	result.accessibility.auto_advance = result.accessibility.auto_delay > 0.0
	result.profile.preset = "original" if Presets.matches(result, "original") else "custom"
	return {"settings": result, "reset_keys": reset, "dropped_keys": report.dropped_keys}

func _decode(parsed: Variant) -> Dictionary:
	if not parsed is Dictionary or not parsed.get("settings") is Dictionary: return {}
	var report: Dictionary
	match parsed.get("schema"):
		SCHEMA: report = sanitize(parsed.settings)
		SCHEMA_V1:
			report = migrate_v1(parsed.settings)
			if not report.is_empty(): report.migrated = true
		_: return {}
	if report.is_empty(): return {}
	report.marker = parsed.get("pending_display_boot_check", {})
	return report

func load_preferences() -> bool:
	settings = defaults(); pending = {}; recovered = false; migrated = false; error = ""
	reset_keys = []; dropped_keys = []
	if not FileAccess.file_exists(path): return true
	var outcome := _decode(_read(path))
	if outcome.is_empty():
		outcome = _decode(_read(path + ".backup"))
		recovered = true
	if outcome.is_empty():
		error = "Settings were damaged. Safe defaults are active."
		return false
	settings = outcome.settings
	reset_keys.assign(outcome.reset_keys); dropped_keys.assign(outcome.dropped_keys)
	migrated = outcome.get("migrated", false)
	var marker: Variant = outcome.marker
	if not marker is Dictionary:
		error = "Display recovery marker was damaged. Safe window settings are active."
		settings.display = defaults().display
		recovered = true
	elif not marker.is_empty():
		var previous := settings.duplicate(true)
		previous.display = marker.get("previous", {})
		var checked := sanitize(previous)
		settings.display = checked.settings.display if marker.get("previous") is Dictionary else defaults().display
		recovered = true
	if recovered or migrated or not reset_keys.is_empty() or not dropped_keys.is_empty(): return save()
	return true

func _read(file_path: String) -> Variant:
	if not FileAccess.file_exists(file_path) or FileAccess.get_size(file_path) > 131072: return null
	return JSON.parse_string(FileAccess.get_file_as_string(file_path))

func save() -> bool:
	var record := {"schema": SCHEMA, "settings": settings, "pending_display_boot_check": pending}
	var absolute := ProjectSettings.globalize_path(path)
	if DirAccess.make_dir_recursive_absolute(absolute.get_base_dir()) != OK:
		error = "Cannot create the settings directory."; return false
	var file := FileAccess.open(path + ".tmp", FileAccess.WRITE)
	if file == null: error = "Cannot write settings."; return false
	file.store_string(JSON.stringify(record, "\t") + "\n"); file.flush()
	var write_error := file.get_error(); file.close()
	if write_error != OK: error = "Could not flush settings."; return false
	if FileAccess.file_exists(path):
		var backup := FileAccess.open(path + ".backup.tmp", FileAccess.WRITE)
		if backup == null: error = "Cannot preserve previous settings."; return false
		backup.store_buffer(FileAccess.get_file_as_bytes(path)); backup.flush()
		var backup_error := backup.get_error(); backup.close()
		if backup_error != OK: error = "Cannot flush previous settings."; return false
		if DirAccess.rename_absolute(absolute + ".backup.tmp", absolute + ".backup") != OK:
			error = "Cannot preserve previous settings."; return false
	if DirAccess.rename_absolute(absolute + ".tmp", absolute) != OK:
		error = "Cannot replace settings atomically."; return false
	error = ""; return true

func set_value(section: String, key: String, value: Variant) -> bool:
	return set_values({section + "." + key: value})

## One atomic write of several "section.key" values (for example a language
## change that sets every pack key together). All or nothing: an invalid value
## or a failed save leaves settings and pending exactly as they were.
func set_values(changes: Dictionary) -> bool:
	return _write(changes, false)

func _write(changes: Dictionary, allow_profile: bool) -> bool:
	if changes.is_empty(): return false
	var candidate := settings.duplicate(true)
	var touched: Array[String] = []
	for name: Variant in changes:
		var parts: PackedStringArray = str(name).split(".")
		if parts.size() != 2 or not candidate.has(parts[0]) or not candidate[parts[0]].has(parts[1]):
			error = "Unknown setting."; return false
		if parts[0] == "display" and parts[1] not in LIVE_DISPLAY_KEYS:
			error = "Window settings need the display preview."; return false
		if parts[0] == "profile" and not allow_profile:
			error = "Choose a preset with apply_preset."; return false
		candidate[parts[0]][parts[1]] = changes[name]
		touched.append(str(name))
	if not allow_profile: _follow_preset(candidate, touched)
	candidate = validate(candidate)
	if candidate.is_empty(): error = "Invalid setting."; return false
	var before := settings; var before_pending := pending.duplicate(true)
	settings = candidate
	if not pending.is_empty():
		# A live display key changed during a window preview: the preview and the
		# value it reverts to both carry the new choice.
		for key: String in LIVE_DISPLAY_KEYS:
			pending.candidate[key] = settings.display[key]; pending.previous[key] = settings.display[key]
	if not save(): settings = before; pending = before_pending; return false
	changed.emit(effective_settings()); return true

## Editing a preset key away from the active preset's value selects 'custom'.
static func _follow_preset(candidate: Dictionary, touched: Array) -> void:
	var active: String = candidate.profile.preset
	if active == "custom": return
	var owned := Presets.keys()
	for name: String in touched:
		if name in owned and not Presets.matches(candidate, active):
			candidate.profile.preset = "custom"; return

## Applies a preset bundle in one atomic write. 'custom' keeps every value.
func apply_preset(name: String) -> bool:
	if name not in Presets.NAMES: error = "Unknown preset."; return false
	var changes := Presets.bundle(name)
	changes["profile.preset"] = name
	return _write(changes, true)

## True when the key's current value makes presentation differ from the
## original game: preset keys against the Original bundle, language keys
## against the Japanese defaults, the preset name against 'original'.
## Desktop-only keys (window, UI scale, audio, controls) are always false.
func differs_from_original(key: String) -> bool:
	return static_differs(effective_settings(), key)

static func static_differs(values: Dictionary, key: String) -> bool:
	var parts := key.split(".")
	if parts.size() != 2 or not defaults().has(parts[0]) or not defaults()[parts[0]].has(parts[1]): return false
	var current: Variant = Presets.read(values, key)
	if Presets.ORIGINAL.has(key): return not Presets.same(current, Presets.ORIGINAL[key])
	if parts[0] in FIDELITY_NEUTRAL_SECTIONS: return false
	if parts[0] == "display" and parts[1] in FIDELITY_NEUTRAL_DISPLAY: return false
	if parts[0] == "accessibility" and parts[1] == "subtitles": return current == true
	return not Presets.same(current, defaults()[parts[0]][parts[1]])

## Badge data: presentation exactness and language are reported separately.
func fidelity_report() -> Dictionary:
	var values := effective_settings()
	var presentation: Array[String] = []
	var language: Array[String] = []
	for section: String in values:
		for key: String in values[section]:
			var name := section + "." + key
			if name == "profile.preset" or not static_differs(values, name): continue
			if section == "language": language.append(name)
			else: presentation.append(name)
	return {"presentation_exact": presentation.is_empty(), "language_original": language.is_empty(),
		"presentation_differs": presentation, "language_differs": language, "preset": values.profile.preset}

func begin_display(candidate: Dictionary, now: float, first_launch: bool = false) -> bool:
	if not pending.is_empty(): error = "Keep or revert the current display change first."; return false
	var checked := settings.duplicate(true); checked.display = candidate
	checked = validate(checked)
	if checked.is_empty(): error = "Invalid display settings."; return false
	var duration := 60.0 if first_launch or candidate.long_confirmation else 15.0
	pending = {"previous": settings.display.duplicate(true), "candidate": checked.display, "deadline": now + duration}
	if not save(): pending = {}; return false
	changed.emit(effective_settings()); return true

## Settings as presented now. During a window preview the candidate display is
## shown, and the preset follows it exactly as confirm_display would, so the
## preset row and the fidelity badge never disagree.
func effective_settings() -> Dictionary:
	var result := settings.duplicate(true)
	if not pending.is_empty():
		result.display = pending.candidate.duplicate(true)
		_follow_preset(result, _live_display_names())
	return result

static func _live_display_names() -> Array:
	return LIVE_DISPLAY_KEYS.map(func(key: String) -> String: return "display." + key)

func confirm_display() -> bool:
	if pending.is_empty(): return false
	var previous: Dictionary = settings.display; var marker := pending.duplicate(true)
	var previous_preset: String = settings.profile.preset
	settings.display = pending.candidate.duplicate(true); pending = {}
	_follow_preset(settings, _live_display_names())
	if not save(): settings.display = previous; settings.profile.preset = previous_preset; pending = marker; return false
	changed.emit(settings.duplicate(true)); return true

func revert_display() -> bool:
	if pending.is_empty(): return true
	pending = {}
	# Reverting must remain safe even if the disk became read-only. The durable
	# sentinel still recovers the same previous configuration on the next boot.
	var saved := save()
	changed.emit(settings.duplicate(true)); return saved

func tick(now: float, focused: bool = true) -> void:
	if not pending.is_empty() and (not focused or now >= float(pending.deadline)): revert_display()

func restore_category(section: String) -> bool:
	if section in ["display", "profile"] or not settings.has(section): return false
	var previous: Dictionary = settings[section]; var previous_preset: String = settings.profile.preset
	settings[section] = defaults()[section]
	var touched: Array = []
	for key: String in settings[section]: touched.append(section + "." + key)
	_follow_preset(settings, touched)
	if not save(): settings[section] = previous; settings.profile.preset = previous_preset; return false
	changed.emit(effective_settings()); return true
