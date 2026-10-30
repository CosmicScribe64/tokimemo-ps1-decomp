extends RefCounted
## Presentation timing is separate from native simulation and save slots.
## T-30002 WP9: every helper is the identity at the v2 defaults, so a presenter
## that routes its values through this service draws byte-identical native
## frames until the player opts in. Nothing here reads or writes native state.
const History = preload("res://services/read_history_service.gd")
## Largest flash alpha (0..255) drawn while reduce_flashes is on.
const REDUCED_FLASH_ALPHA := 64
## Auto-advance never fires sooner than this after a line's text is complete,
## so a line without voice stays readable at the default auto_delay of 0.
const MIN_READ_BASE := 1.0
const MIN_READ_PER_UNIT := 0.05
var settings: Dictionary = {}
var read_lines: Dictionary = {}
## Optional persistent history; when bound, read state survives restarts.
var history: RefCounted = null
var _active_line := ""
var _elapsed := 0.0
var _text_elapsed := 0.0

func configure(value: Dictionary) -> void:
	settings = value.duplicate(true)
	_elapsed = 0.0; _text_elapsed = 0.0

func bind_history(value: RefCounted) -> void:
	history = value

func visible_glyphs(native_count: int, total: int) -> int:
	if native_count < 0: return total
	match settings.get("text_speed", "normal"):
		"slow": return mini(total, native_count / 2)
		"fast": return mini(total, native_count * 2)
		"instant": return total
	return mini(total, native_count)

## Identity of a T-39016 text-layer record for read/skip state.
static func line_identity(record_id: String, source_sha256: String) -> String:
	return History.text_layer_identity(record_id, source_sha256)

func mark_read(identity: String) -> void:
	if identity.is_empty(): return
	read_lines[identity] = true
	if history != null: history.mark_read(identity)

func is_read(identity: String) -> bool:
	return read_lines.has(identity) or (history != null and history.is_read(identity))

func may_skip(identity: String) -> bool:
	return settings.get("skip", "read") == "all" or is_read(identity)

## Auto-advance is on only when accessibility.auto_advance is set. auto_delay
## is purely the extra time after text and voice (v1 files, where a positive
## delay meant auto-advance, are converted by migrate_v1).
func auto_advance_enabled() -> bool:
	return settings.get("auto_advance", false) == true

## Seconds a completed line must stay on screen before auto-advance, counted
## from the end of the text reveal. `units` is the line length (0 if unknown).
static func min_reading_time(units: int) -> float:
	return MIN_READ_BASE + MIN_READ_PER_UNIT * float(maxi(units, 0))

## True once per call sequence when the line may auto-advance: the text has
## been complete for min_reading_time(units) and text and voice have both been
## complete for auto_delay.
func auto_advance(identity: String, text_complete: bool, voice_complete: bool, delta: float, choice_pending: bool = false, units: int = 0) -> bool:
	if identity != _active_line:
		_active_line = identity; _elapsed = 0.0; _text_elapsed = 0.0
	var delay := float(settings.get("auto_delay", 0.0))
	if choice_pending or not text_complete or not auto_advance_enabled():
		_elapsed = 0.0; _text_elapsed = 0.0; return false
	_text_elapsed += maxf(delta, 0.0)
	if not voice_complete:
		_elapsed = 0.0; return false
	_elapsed += maxf(delta, 0.0)
	if _elapsed < delay or _text_elapsed < min_reading_time(units): return false
	_elapsed = 0.0; _text_elapsed = 0.0
	mark_read(identity)
	return true

## Scale for the accessible text painter. 1.0 keeps the native 12x12 glyphs.
func font_scale() -> float:
	return clampf(float(settings.get("font_scale", 1.0)), 1.0, 1.5)

## Text box backdrop opacity (dialogue_textbox_presenter.configure_accessibility).
func backdrop() -> float:
	return clampf(float(settings.get("backdrop", 1.0)), 0.0, 1.0)

## 'native' draws the original box; 'modern' asks for the window-space overlay
## of T-30001 WP4 (the native image is unchanged either way).
func text_box_mode() -> String:
	return "modern" if settings.get("text_box", "native") == "modern" else "native"

## Alpha (0..255) of a full-screen flash or fade-to-white overlay drawn by the
## port's presentation layer. Native fades stay untouched.
func flash_alpha(native_alpha: int) -> int:
	if settings.get("reduce_flashes", false) != true: return native_alpha
	return mini(native_alpha, REDUCED_FLASH_ALPHA)

## Presentation offset of a screen shake. With reduce_shake it is zero; the
## native simulation keeps its own shake state.
func shake_offset(native_offset: Vector2i) -> Vector2i:
	return Vector2i.ZERO if settings.get("reduce_shake", false) == true else native_offset
