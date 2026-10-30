extends RefCounted
## T-30002 test support for the settings UI (see ui/settings/settings_context.gd).
## Preferences, presets, remaps and the glyph service are the real settings-core
## v2 services. select_language (no bundle is installed in tests), the display
## frame and the native voice route are faked here.
const Glyphs = preload("res://services/input_glyph_service.gd")
const LanguageBundle = preload("res://services/language_bundle_service.gd")
## Stands in for an applied bundle's own disclosure text.
const BUNDLE_DISCLOSURE := "Test bundle: English text is AI-translated; voices remain Japanese."

## The real core preferences on a fresh file, recording preset calls.
class CorePrefs extends "res://services/user_preferences_service.gd":
	var presets_applied: Array[String] = []
	func _init() -> void:
		super("user://settings-ui-core-v2.json")
		for suffix: String in ["", ".tmp", ".backup", ".backup.tmp"]:
			if FileAccess.file_exists(path + suffix): DirAccess.remove_absolute(ProjectSettings.globalize_path(path + suffix))
	func apply_preset(name: String) -> bool:
		presets_applied.append(name)
		return super(name)

class FakeFrame extends RefCounted:
	var widescreen_available := true
	func drawable_info() -> Dictionary:
		return {"window": Vector2i(1920, 1080), "factor": 4.0}

## settings_runtime now owns glyphs and language_bundle (the real services;
## the bundle is never loaded here, only its disclosure is set) and a
## display_frame, which the fake replaces with a fixed one.
class FakeRuntime extends "res://services/settings_runtime.gd":
	var language_loading := false
	func _init() -> void:
		preferences = CorePrefs.new()
		display_frame = FakeFrame.new()
	func _ready() -> void:
		super()
		display_frame = FakeFrame.new()
	## Stands in for the root/WP5 hook: request the bundle and record the choice.
	func select_language(choice: String) -> String:
		language_loading = true
		return "" if preferences.set_value("language", "bundle", choice) else str(preferences.error)
	## The fake behaves as if language/en were installed.
	func language_available(choice: String) -> bool:
		return choice in LanguageBundle.CHOICES
	## Coverage in core LanguageBundleService key names.
	func language_coverage() -> Dictionary:
		return {"text_records_approved": 120, "text_records_representable": 90, "dialogue_lines": 40,
			"images_reviewed": 44, "images_original_japanese": 173, "draft_images": 0}
	func native_voice_mode() -> Variant:
		return "On"
