extends RefCounted
## Presentation timing is separate from native simulation and save slots.
var settings: Dictionary = {}
var read_lines: Dictionary = {}
var _active_line := ""
var _elapsed := 0.0

func configure(value: Dictionary) -> void:
	settings = value.duplicate(true)
	_elapsed = 0.0

func visible_glyphs(native_count: int, total: int) -> int:
	if native_count < 0: return total
	match settings.get("text_speed", "normal"):
		"slow": return mini(total, native_count / 2)
		"fast": return mini(total, native_count * 2)
		"instant": return total
	return mini(total, native_count)

func mark_read(identity: String) -> void:
	if not identity.is_empty(): read_lines[identity] = true

func may_skip(identity: String) -> bool:
	return settings.get("skip", "read") == "all" or read_lines.has(identity)

func auto_advance(identity: String, text_complete: bool, voice_complete: bool, delta: float, choice_pending: bool = false) -> bool:
	if identity != _active_line:
		_active_line = identity; _elapsed = 0.0
	var delay: float = settings.get("auto_delay", 0.0)
	if choice_pending or not text_complete or not voice_complete or delay <= 0:
		_elapsed = 0.0; return false
	_elapsed += maxf(delta, 0.0)
	if _elapsed < delay: return false
	_elapsed = 0.0
	mark_read(identity)
	return true
