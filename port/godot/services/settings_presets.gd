extends RefCounted
## T-30002 WP1: presets are plain value bundles over presentation-affecting keys.
## Desktop-only keys (window size/mode/monitor, ui_scale, volumes, bindings,
## prompt style) never change the native 320x240 image, so no preset owns them.
## Language is not part of any preset: 'Original (PS1 exact)' leaves the game
## language unchanged and the fidelity badge reports language separately.

const NAMES := ["original", "modern", "custom"]
const LABELS := {"original": "Original (PS1 exact)", "modern": "Modern English", "custom": "Custom"}

## Every value equals the v2 default, so a fresh install is the Original preset.
const ORIGINAL := {
	"display.aspect": "original",
	"display.filter": "integer",
	"display.wing_fill": "black",
	"display.wing_art": "off",
	"display.bars": "black",
	"accessibility.text_speed": "normal",
	"accessibility.auto_delay": 0.0,
	"accessibility.auto_advance": false,
	"accessibility.skip": "read",
	"accessibility.font_scale": 1.0,
	"accessibility.backdrop": 1.0,
	"accessibility.reduce_flashes": false,
	"accessibility.reduce_shake": false,
	"accessibility.text_box": "native",
}

## Overrides on top of ORIGINAL. Wing art stays black: 'source' wings are
## proved only for Class A scenes and stay a separate opt-in (T-30001), and
## decorative wing art (display.wing_art) stays off in every preset (T-39220).
const MODERN_OVERRIDES := {
	"display.aspect": "auto",
	"display.filter": "nearest",
	"accessibility.text_box": "modern",
}

static func keys() -> Array:
	return ORIGINAL.keys()

static func bundle(name: String) -> Dictionary:
	match name:
		"original": return ORIGINAL.duplicate(true)
		"modern":
			var result := ORIGINAL.duplicate(true)
			for key: String in MODERN_OVERRIDES: result[key] = MODERN_OVERRIDES[key]
			return result
	return {}

static func read(settings: Dictionary, key: String) -> Variant:
	var parts := key.split(".")
	if parts.size() != 2 or not settings.get(parts[0]) is Dictionary: return null
	return settings[parts[0]].get(parts[1])

static func same(left: Variant, right: Variant) -> bool:
	if (left is float or left is int) and (right is float or right is int) and not left is bool and not right is bool:
		return is_equal_approx(float(left), float(right))
	return typeof(left) == typeof(right) and left == right

## True when every key of the named preset holds its preset value.
static func matches(settings: Dictionary, name: String) -> bool:
	var values := bundle(name)
	if values.is_empty(): return false
	for key: String in values:
		if not same(read(settings, key), values[key]): return false
	return true
