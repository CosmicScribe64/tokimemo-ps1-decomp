extends RefCounted
## T-30002: desktop preferences never enter native game/save/RNG state.
signal changed(settings: Dictionary)
const SCHEMA := "tokimemo.port-settings.v1"
const Remaps = preload("res://services/input_remap_service.gd")
const BUSES := ["Master", "Voice", "BGM", "SFX", "Movie"]
const ENUMS := {
	"display.mode": ["windowed", "borderless", "exclusive"],
	"display.aspect": ["original", "wide", "auto"],
	"display.filter": ["integer", "nearest", "linear"],
	"display.fps": [60, 120, 144, 0],
	"accessibility.text_speed": ["slow", "normal", "fast", "instant"],
	"accessibility.skip": ["read", "all"],
}
const RANGES := {"display.width": [640, 7680], "display.height": [480, 4320],
	"display.monitor": [0, 31], "controls.deadzone": [0.05, 0.9],
	"controls.repeat_delay": [0.15, 1.0], "controls.repeat_interval": [0.03, 0.3],
	"accessibility.auto_delay": [0.0, 10.0], "accessibility.font_scale": [1.0, 1.5],
	"accessibility.backdrop": [0.0, 1.0]}
var path: String
var settings: Dictionary = defaults()
var pending: Dictionary = {}
var error := ""
var recovered := false

static func defaults() -> Dictionary:
	var buses := {}
	for bus: String in BUSES: buses[bus] = {"volume": 1.0, "mute": false}
	return {"display": {"mode": "windowed", "width": 960, "height": 720, "monitor": 0,
		"aspect": "original", "filter": "integer", "vsync": true, "fps": 60, "long_confirmation": false},
		"audio": {"buses": buses, "device": "Default", "mono": false, "mute_unfocused": false},
		"controls": {"swap_confirm": false, "deadzone": 0.25, "repeat_delay": 0.4,
			"repeat_interval": 0.08, "bindings": {}},
		"accessibility": {"text_speed": "normal", "auto_delay": 0.0, "skip": "read",
			"font_scale": 1.0, "backdrop": 1.0, "reduce_flashes": false, "reduce_shake": false},
		"language": {"text": "ja", "text_manifest": "", "text_sha256": "", "voice": "ja",
			"voice_manifest": "", "voice_sha256": "", "dialogue_manifest": "", "dialogue_sha256": ""}}

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

static func validate(candidate: Variant) -> Dictionary:
	if not candidate is Dictionary: return {}
	var result := defaults()
	for section: String in result:
		if not candidate.get(section) is Dictionary: return {}
		for key: String in result[section]:
			if not candidate[section].has(key):
				if section == "language" and key in ["dialogue_manifest", "dialogue_sha256"]: continue
				return {}
			var value: Variant = candidate[section][key]
			if section == "audio" and key == "buses":
				if not value is Dictionary: return {}
				for bus: String in BUSES:
					var spec: Variant = value.get(bus)
					if not spec is Dictionary or not spec.get("mute") is bool: return {}
					var volume: Variant = spec.get("volume")
					if not (volume is float or volume is int) or not is_finite(float(volume)) or volume < 0 or volume > 1: return {}
					result.audio.buses[bus] = {"volume": float(volume), "mute": spec.mute}
			elif section == "controls" and key == "bindings":
				if not value is Dictionary or value.size() > 24: return {}
				if Remaps.validated_bindings(value).is_empty(): return {}
				result.controls.bindings = value.duplicate(true)
			else:
				if not valid_value(section + "." + key, value, result[section][key]): return {}
				result[section][key] = int(value) if result[section][key] is int else value
	return result

func load_preferences() -> bool:
	settings = defaults(); pending = {}; recovered = false; error = ""
	if not FileAccess.file_exists(path): return true
	var parsed: Variant = _read(path)
	if not parsed is Dictionary or parsed.get("schema") != SCHEMA or validate(parsed.get("settings")).is_empty():
		parsed = _read(path + ".backup")
		recovered = true
	if not parsed is Dictionary or parsed.get("schema") != SCHEMA or validate(parsed.get("settings")).is_empty():
		error = "Settings were damaged. Safe defaults are active."
		return false
	settings = validate(parsed.settings)
	var marker: Variant = parsed.get("pending_display_boot_check", {})
	if not marker is Dictionary:
		error = "Display recovery marker was damaged. Safe window settings are active."
		settings.display = defaults().display
		recovered = true
	elif not marker.is_empty():
		var previous := settings.duplicate(true)
		previous.display = marker.get("previous", {})
		settings.display = previous.display if not validate(previous).is_empty() else defaults().display
		recovered = true
	if recovered: return save()
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
	if not settings.has(section) or not settings[section].has(key) or section == "display": return false
	var candidate := settings.duplicate(true); candidate[section][key] = value
	candidate = validate(candidate)
	if candidate.is_empty(): error = "Invalid setting."; return false
	var before := settings; settings = candidate
	if not save(): settings = before; return false
	changed.emit(effective_settings()); return true

func begin_display(candidate: Dictionary, now: float, first_launch: bool = false) -> bool:
	if not pending.is_empty(): error = "Keep or revert the current display change first."; return false
	var checked := settings.duplicate(true); checked.display = candidate
	checked = validate(checked)
	if checked.is_empty(): error = "Invalid display settings."; return false
	var duration := 60.0 if first_launch or candidate.long_confirmation else 15.0
	pending = {"previous": settings.display.duplicate(true), "candidate": checked.display, "deadline": now + duration}
	if not save(): pending = {}; return false
	changed.emit(effective_settings()); return true

func effective_settings() -> Dictionary:
	var result := settings.duplicate(true)
	if not pending.is_empty(): result.display = pending.candidate.duplicate(true)
	return result

func confirm_display() -> bool:
	if pending.is_empty(): return false
	var previous: Dictionary = settings.display; var marker := pending.duplicate(true)
	settings.display = pending.candidate.duplicate(true); pending = {}
	if not save(): settings.display = previous; pending = marker; return false
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
	if section == "display" or not settings.has(section): return false
	var previous: Dictionary = settings[section]; settings[section] = defaults()[section]
	if not save(): settings[section] = previous; return false
	changed.emit(effective_settings()); return true
