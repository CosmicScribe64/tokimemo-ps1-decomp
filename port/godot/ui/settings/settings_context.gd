extends RefCounted
## T-30002 WP6/WP7: the only place the settings UI talks to the settings-core
## services (preferences v2, presets, language bundle, input glyphs, display
## frame service). Each call uses the documented core API when the runtime
## provides it and otherwise a thin fallback over the v1 services, so the pages
## never branch on which core packages have landed. The fallbacks never invent
## data: an unavailable feature is reported as unavailable.

const PRESET_SCRIPT := "res://services/settings_presets.gd"
const Glyphs = preload("res://services/input_glyph_service.gd")
const LanguageBundle = preload("res://services/language_bundle_service.gd")
## Shown with the English choice until a loaded bundle supplies its own
## disclosure (design WP-E). The bundle text replaces it once English applies.
const DEFAULT_DISCLOSURE := "English text is AI-translated and not yet human-reviewed; voices remain Japanese."
const PRESETS := ["original", "modern", "custom"]
## Keys that the "Original (PS1 exact)" preset owns. Window size, mode, monitor,
## volumes and bindings are desktop-only and never part of a preset. Language
## is reported separately (design: Original preset keeps the game language).
const PRESET_KEYS := ["display.aspect", "display.filter", "display.framing", "display.wing_fill",
	"display.bars", "accessibility.text_speed", "accessibility.auto_delay", "accessibility.auto_advance",
	"accessibility.font_scale", "accessibility.backdrop", "accessibility.reduce_flashes",
	"accessibility.reduce_shake", "accessibility.text_box"]
## Player-facing names for PRESET_KEYS, matching the page labels, so the
## fidelity badge names the controls the player can find.
const PRESET_LABELS := {"display.aspect": "Scene layout", "display.filter": "Scaling",
	"display.framing": "Screen framing", "display.wing_fill": "Side areas", "display.bars": "Side bars",
	"accessibility.text_speed": "Text speed", "accessibility.auto_delay": "Extra reading time after voice",
	"accessibility.auto_advance": "Auto-advance", "accessibility.font_scale": "Text size",
	"accessibility.backdrop": "Text box opacity", "accessibility.reduce_flashes": "Reduce screen flashes",
	"accessibility.reduce_shake": "Reduce screen shake", "accessibility.text_box": "Dialogue text box",
	"accessibility.skip": "Skip", "accessibility.subtitles": "Subtitles"}
## Fallback Modern bundle used only until services/settings_presets.gd lands.
## Values are limited to options that already exist; nothing here changes
## native simulation, save data or the 320x240 native image.
const FALLBACK_MODERN := {"language.bundle": "en", "accessibility.text_speed": "fast",
	"accessibility.text_box": "modern"}
## Display keys the UI applies live without Keep/Revert (widescreen design WP5).
## Core also accepts display.aspect and display.filter live; the Display page
## keeps those two in its Keep/Revert draft, which core's begin_display accepts.
const LIVE_DISPLAY_KEYS := ["display.wing_fill", "display.ui_scale", "display.bars"]
const PROMPT_STYLES := ["auto", "keyboard", "xbox", "playstation", "nintendo"]
## Player-facing names for InputGlyphService.confirm_side results.
const CONFIRM_SIDES := {"south": "bottom", "east": "right", "west": "left", "north": "top"}
## PS1 button legend shown on the Controls page (text only, no glyph art).
const PS1_LEGEND := {"confirm": "Circle", "cancel": "Cross", "square": "Square", "triangle": "Triangle",
	"start": "Start", "select": "Select", "l1": "L1", "r1": "R1", "l2": "L2", "r2": "R2"}

var runtime: Node
var menu: Control
var widgets: RefCounted

func _init(owner_runtime: Node, owner_menu: Control, owner_widgets: RefCounted) -> void:
	runtime = owner_runtime; menu = owner_menu; widgets = owner_widgets

# --- preferences -----------------------------------------------------------

func preferences() -> Object:
	return runtime.preferences

func settings() -> Dictionary:
	return runtime.preferences.settings

func has_pref(dotted: String) -> bool:
	var parts := dotted.split(".")
	var section: Variant = settings().get(parts[0])
	return section is Dictionary and section.has(parts[1])

func pref(dotted: String, fallback: Variant = null) -> Variant:
	var parts := dotted.split(".")
	if parts.size() == 1: return settings().get(dotted, fallback)
	var section: Variant = settings().get(parts[0])
	return section.get(parts[1], fallback) if section is Dictionary else fallback

## Values the Original preset uses: core presets when present, else defaults().
func original_value(dotted: String) -> Variant:
	var bundle := preset_bundle("original")
	if bundle.has(dotted): return bundle[dotted]
	var parts := dotted.split(".")
	var defaults: Dictionary = runtime.Preferences.defaults()
	var section: Variant = defaults.get(parts[0])
	return section.get(parts[1]) if section is Dictionary else null

## Returns "" on success or the player-facing error.
func set_pref(dotted: String, value: Variant) -> String:
	var parts := dotted.split(".")
	if not has_pref(dotted): return "This option needs a newer settings file (settings v2)."
	if parts[0] == "display" and dotted not in LIVE_DISPLAY_KEYS:
		return "Display options are applied with Apply display changes."
	if runtime.preferences.set_value(parts[0], parts[1], value): return ""
	return str(runtime.preferences.error) if not str(runtime.preferences.error).is_empty() else "Invalid setting."

# --- presets and fidelity ----------------------------------------------------

func preset_bundle(name: String) -> Dictionary:
	if ResourceLoader.exists(PRESET_SCRIPT):
		var script: Script = load(PRESET_SCRIPT)
		if script != null and script.get_script_method_list().any(func(method: Dictionary) -> bool: return method.name == "bundle"):
			var result: Variant = script.call("bundle", name)
			if result is Dictionary: return result
	if name == "modern": return FALLBACK_MODERN.duplicate()
	return {}

## Core stores the preset at profile.preset; a pre-core file has none.
func current_preset() -> String:
	var stored: Variant = pref("profile.preset") if has_pref("profile.preset") else pref("preset")
	if stored is String and stored in PRESETS: return stored
	return "original" if fidelity_changes().is_empty() else "custom"

func differs_from_original(dotted: String) -> bool:
	if runtime.preferences.has_method("differs_from_original"):
		return bool(runtime.preferences.differs_from_original(dotted))
	if not has_pref(dotted): return false
	return pref(dotted) != original_value(dotted)

## Preset keys whose effective value differs from Original, pending display included.
func fidelity_changes() -> Array[String]:
	var result: Array[String] = []
	if runtime.preferences.has_method("fidelity_report"):
		# Core reports over effective settings, so a pending preview counts.
		var report: Dictionary = runtime.preferences.fidelity_report()
		for dotted: Variant in report.get("presentation_differs", []): result.append(str(dotted))
		return result
	var effective: Dictionary = runtime.preferences.effective_settings()
	var core: bool = runtime.preferences.has_method("differs_from_original")
	for dotted: String in PRESET_KEYS:
		var parts := dotted.split(".")
		if not effective.get(parts[0], {}).has(parts[1]): continue
		var value: Variant = effective[parts[0]][parts[1]]
		if core:
			# Core answers for saved values; a pending preview that moves a
			# display key away from its saved value is reported as a change.
			if differs_from_original(dotted) or value != settings()[parts[0]][parts[1]]: result.append(dotted)
		elif value != original_value(dotted): result.append(dotted)
	return result

func preset_label(dotted: String) -> String:
	return str(PRESET_LABELS.get(dotted, dotted.get_slice(".", 1).capitalize()))

## Applies a preset. Display keys come back as a draft for Keep/Revert; every
## other key is saved at once. Returns {error, display}.
## Core presets never own the language (settings_presets.gd), so "Modern
## English" requests English here unless the caller (the Welcome step) picks
## the language itself.
func apply_preset(name: String, with_language: bool = true) -> Dictionary:
	if runtime.preferences.has_method("apply_preset"):
		if not runtime.preferences.apply_preset(name): return {"error": str(runtime.preferences.error), "display": {}}
		var language_error := ""
		if with_language and name == "modern" and language_choice() != "en": language_error = select_language("en")
		return {"error": language_error, "display": {}}
	var bundle: Dictionary = {}
	if name == "original":
		for dotted: String in PRESET_KEYS:
			if has_pref(dotted): bundle[dotted] = original_value(dotted)
	else:
		bundle = preset_bundle(name)
	var display: Dictionary = runtime.preferences.settings.display.duplicate(true)
	var display_changed := false
	var errors: Array[String] = []
	for dotted: String in bundle:
		if dotted == "language.bundle":
			var language_error := select_language(str(bundle[dotted])) if with_language else ""
			if not language_error.is_empty(): errors.append(language_error)
			continue
		if not has_pref(dotted): continue
		var parts := dotted.split(".")
		if parts[0] == "display" and dotted not in LIVE_DISPLAY_KEYS:
			if display[parts[1]] != bundle[dotted]: display[parts[1]] = bundle[dotted]; display_changed = true
			continue
		if pref(dotted) == bundle[dotted]: continue
		var error := set_pref(dotted, bundle[dotted])
		if not error.is_empty(): errors.append(error)
	return {"error": " ".join(errors), "display": display if display_changed else {}}

# --- language ----------------------------------------------------------------

## 'ja', 'en' or 'custom'.
func language_choice() -> String:
	var stored: Variant = pref("language.bundle")
	if stored is String and stored in ["ja", "en", "custom"]: return stored
	var language: Dictionary = settings().language
	for key: String in ["text_manifest", "dialogue_manifest", "ui_text_manifest"]:
		if not str(language.get(key, "")).is_empty(): return "custom"
	return "ja"

func language_available(choice: String) -> bool:
	if runtime.has_method("language_available"): return bool(runtime.language_available(choice))
	return choice == "ja" or runtime.has_method("select_language")

## Starts a language switch. Returns "" or a visible fallback message.
func select_language(choice: String) -> String:
	if runtime.has_method("select_language"): return str(runtime.select_language(choice))
	if choice != "ja": return "English needs the installed language bundle, which this build does not provide yet. Japanese is kept."
	var errors: Array[String] = []
	for selector: String in ["select_image_pack", "select_text_pack", "select_ui_text_pack"]:
		if runtime.has_method(selector):
			var error := str(runtime.call(selector, ""))
			if not error.is_empty(): errors.append(error)
	return " ".join(errors)

func language_loading() -> bool:
	if "language_loading" in runtime: return bool(runtime.language_loading)
	var bundle: Variant = runtime.get("language_bundle")
	return bundle != null and "loading" in bundle and bool(bundle.loading)

## The core LanguageBundleService the runtime owns, or null.
func language_bundle() -> Object:
	var service: Variant = runtime.get("language_bundle")
	return service if service is Object and service.has_method("coverage_line") else null

## Counts reported by the language bundle service, or {} when unknown.
func language_coverage() -> Dictionary:
	if runtime.has_method("language_coverage"):
		var result: Variant = runtime.language_coverage()
		if result is Dictionary: return result
	var service := language_bundle()
	if service != null and service.coverage is Dictionary: return service.coverage
	return {}

func coverage_line() -> String:
	var coverage := language_coverage()
	if coverage.has("text_records_approved"):
		# Core LanguageBundleService keys (services/language_bundle_service.gd).
		return "Coverage: %d approved text records (%d drawable with the original font), %d dialogue lines, %d reviewed images; %d images stay original Japanese." % [
			int(coverage.get("text_records_approved", 0)), int(coverage.get("text_records_representable", 0)),
			int(coverage.get("dialogue_lines", 0)), int(coverage.get("images_reviewed", 0)),
			int(coverage.get("images_original_japanese", 0))]
	if coverage.is_empty():
		var parts: Array[String] = []
		if runtime.packs.images != null and runtime.packs.images.has_method("asset_count"):
			parts.append("%d translated images" % runtime.packs.images.asset_count())
		if runtime.packs.text_layer != null and runtime.packs.text_layer.has_method("entry_count"):
			parts.append("%d translated interface texts" % runtime.packs.text_layer.entry_count())
		if runtime.text_pack.entries.size() > 0: parts.append("%d translated dialogue lines" % runtime.text_pack.entries.size())
		if parts.is_empty(): return "Coverage: original Japanese text and images."
		return "Coverage: " + ", ".join(parts) + ". Everything else stays original Japanese."
	return "Coverage: %d of %d text records translated, %d reviewed images; %d images stay original Japanese." % [
		int(coverage.get("representable_text", coverage.get("approved_text", 0))), int(coverage.get("total_text", coverage.get("approved_text", 0))),
		int(coverage.get("reviewed_images", 0)), int(coverage.get("japanese_images", 0))]

## Player-facing names for the language choices (LanguageBundleService.LABELS:
## 'Japanese (Original)', 'English (AI translation)', 'Custom packs').
static func language_names() -> Dictionary:
	return LanguageBundle.LABELS.duplicate()

func language_label() -> String:
	return str(LanguageBundle.LABELS.get(language_choice(), LanguageBundle.LABELS.ja))

## The AI-translation disclosure: the loaded bundle's own text when English is
## applied, otherwise the design wording. Always shown with the English option.
func language_disclosure() -> String:
	var service := language_bundle()
	if service != null and "disclosure" in service and service.disclosure is String and not str(service.disclosure).is_empty():
		return str(service.disclosure)
	return DEFAULT_DISCLOSURE

# --- display -------------------------------------------------------------------

func frame_service() -> Object:
	var service: Variant = runtime.get("display_frame")
	return service if service is Object else null

## Scene layout (widescreen) is offered only when the frame service says so.
func widescreen_available() -> bool:
	var service := frame_service()
	return service != null and "widescreen_available" in service and bool(service.widescreen_available)

func ui_scale() -> float:
	var value: Variant = pref("display.ui_scale", 1.0)
	return clampf(float(value), 1.0, 2.0) if (value is float or value is int) else 1.0

## Drawable size and native scale readout.
func drawable_readout() -> String:
	var service := frame_service()
	if service != null and service.has_method("drawable_info"):
		var info: Dictionary = service.drawable_info()
		return "Drawing area %d×%d, game picture ×%s" % [int(info.get("window", Vector2i.ZERO).x), int(info.get("window", Vector2i.ZERO).y), _factor_text(float(info.get("factor", 1.0)))]
	var window := runtime.get_window()
	var size: Vector2i = window.size if window != null else Vector2i(640, 480)
	var display: Dictionary = runtime.preferences.effective_settings().display
	var factor := minf(size.x / 320.0, size.y / 240.0)
	if display.filter == "integer": factor = maxf(floorf(factor), 1.0)
	return "Drawing area %d×%d, game picture ×%s (320×240 original)" % [size.x, size.y, _factor_text(factor)]

static func _factor_text(factor: float) -> String:
	return str(int(factor)) if is_equal_approx(factor, roundf(factor)) else "%.2f" % factor

# --- controls ------------------------------------------------------------------

func glyphs() -> Object:
	var service: Variant = runtime.get("glyphs")
	return service if service is Object else null

## Controller family for prompts: the chosen style, or under "auto" the
## last controller the core glyph service saw ("generic" when none).
func prompt_family() -> String:
	var style := str(pref("controls.prompt_style", "auto"))
	if style in Glyphs.FAMILIES: return style
	var service := glyphs()
	if service != null and "last_family" in service: return str(service.last_family)
	return "generic"

## Text prompt through core InputGlyphService.binding_label, after swap_confirm.
func prompt(action: String, controller: bool) -> String:
	var remaps: RefCounted = runtime.remaps
	var mapped := action
	if remaps.swap_confirm and action in ["confirm", "cancel"]: mapped = "cancel" if action == "confirm" else "confirm"
	var family := prompt_family() if controller else "generic"
	for kinds: Array in ([["button"], ["axis"]] if controller else [["key"], ["mouse"]]):
		for spec: Dictionary in remaps.bindings.get(mapped, []):
			if spec.kind in kinds: return Glyphs.binding_label(spec, family)
	return str(remaps.prompt(action, controller))

## Confirm side from core InputGlyphService.confirm_side.
func confirm_side_label() -> String:
	var side: String = Glyphs.new().confirm_side(runtime.remaps)
	if side.is_empty(): return "Confirm: no controller face button bound"
	return "Confirm: %s face button" % CONFIRM_SIDES[side]

## Per-device reset: "keyboard" or "controller". Falls back to a full reset
## only when the glyph service is missing, and says so.
func reset_device(family: String) -> String:
	if runtime.remaps.has_method("reset_device"):
		# Core WP3: InputRemapService.reset_device returns {error, bindings}.
		var reset: Dictionary = runtime.remaps.reset_device(family)
		if not str(reset.get("error", "")).is_empty(): return str(reset.error)
		return "" if runtime.preferences.set_value("controls", "bindings", reset.bindings) else str(runtime.preferences.error)
	var service := glyphs()
	if service != null and service.has_method("reset_device"):
		var result: Variant = service.reset_device(family)
		if result is Dictionary and result.has("bindings"):
			return "" if runtime.preferences.set_value("controls", "bindings", result.bindings) else str(runtime.preferences.error)
		return str(result) if result is String else ""
	var current: Dictionary = settings().controls.bindings.duplicate(true)
	var defaults: Dictionary = runtime.Remaps.defaults()
	var kinds := ["key", "mouse"] if family == "keyboard" else ["button", "axis"]
	var merged := {}
	for action: String in current:
		var kept: Array = current[action].filter(func(spec: Dictionary) -> bool: return spec.kind not in kinds)
		var restored: Array = defaults[action].filter(func(spec: Dictionary) -> bool: return spec.kind in kinds)
		merged[action] = kept + restored
	if runtime.Remaps.validated_bindings(merged).is_empty(): return "That reset would conflict with other bindings."
	return "" if runtime.preferences.set_value("controls", "bindings", merged) else str(runtime.preferences.error)

# --- text & story / audio -----------------------------------------------------

## Options with no presentation consumer anywhere yet, not even the opening.
## reduce_shake: AccessibilityService.shake_offset has no caller yet (T-30002 WP9).
const UNWIRED_OPTIONS := ["reduce_shake"]

## Route consumers verified by WP9; until then options are tagged "opening only".
func scope_tag(option: String) -> String:
	if option in UNWIRED_OPTIONS: return "not wired yet"
	var service: Variant = runtime.get("accessibility")
	if service is Object and service.has_method("verified_routes"):
		var routes: Variant = service.verified_routes(option)
		if routes is Array and routes.has("all"): return ""
	return "opening only"

## Native voice_mode (GameSaveImage 0xE20) shown read-only, or null.
func native_voice_mode() -> Variant:
	return runtime.native_voice_mode() if runtime.has_method("native_voice_mode") else null
