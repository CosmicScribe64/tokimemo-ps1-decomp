extends RefCounted
## Opt-in reading time holds a committed presentation; it never writes source state.
var settings: Dictionary = {}
var identity := ""
var visible_count := -1
var _elapsed := 0.0
var _ready_elapsed := 0.0
var _complete := false

func configure(value: Dictionary) -> void:
	if settings == value: return
	settings = value.duplicate(true)
	_ready_elapsed = 0.0

func step(opening: Dictionary, delta: float) -> bool:
	var work: Variant = opening.get("prologue_text_work")
	var text: Variant = opening.get("prologue_text")
	var audio: Variant = opening.get("audio")
	if not work is Dictionary or not text is Dictionary or not audio is Dictionary:
		reset(); return false
	if audio.get("status") == "cancelled": reset(); return false
	if not work.get("line_id") is int or not work.get("pos") is int or not work.get("total") is int or work.total <= 0:
		reset(); return false
	var next_identity := "%d:%s" % [work.line_id, str(audio.get("epoch", ""))]
	if next_identity != identity:
		identity = next_identity; _elapsed = 0.0; _ready_elapsed = 0.0; _complete = false
	_elapsed += clampf(delta, 0.0, 0.1)
	match settings.get("text_speed", "normal"):
		"slow": visible_count = mini(work.total, floori(_elapsed * 30.0 / (maxi(int(work.get("speed", 1)), 0) + 1)))
		"fast": visible_count = mini(work.total, work.pos * 2)
		"instant": visible_count = work.total
		_: visible_count = work.pos
	var clips: Variant = audio.get("clips")
	var voice_complete: bool = clips is Array and (clips.is_empty() or (audio.get("status") == "finished" and int(audio.get("clip_index", -1)) == clips.size() - 1))
	var native_complete: bool = text.get("line_id") == work.line_id and text.get("k_disp_inc_result") == 1
	if not native_complete or not voice_complete: _ready_elapsed = 0.0; return false
	if visible_count < work.total: return true
	_complete = true
	_ready_elapsed += clampf(delta, 0.0, 0.1)
	return _ready_elapsed < float(settings.get("auto_delay", 0.0))

func reset() -> void:
	identity = ""; visible_count = -1; _elapsed = 0.0; _ready_elapsed = 0.0; _complete = false

func reading_complete() -> bool:
	return _complete
