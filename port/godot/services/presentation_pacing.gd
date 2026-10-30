extends RefCounted
## Opt-in reading time holds a committed presentation; it never writes source state.
const Access = preload("res://services/accessibility_service.gd")
var settings: Dictionary = {}
var identity := ""
var visible_count := -1
var _elapsed := 0.0
var _ready_elapsed := 0.0
## Time since the text reveal finished (drives the auto-advance minimum).
var _text_elapsed := 0.0
var _total := 0
var _complete := false
var _advance_taken := false

func configure(value: Dictionary) -> void:
	if settings == value: return
	settings = value.duplicate(true)
	_ready_elapsed = 0.0; _text_elapsed = 0.0

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
		identity = next_identity; _elapsed = 0.0; _ready_elapsed = 0.0; _text_elapsed = 0.0; _complete = false; _advance_taken = false
	_elapsed += clampf(delta, 0.0, 0.1)
	_total = work.total
	match settings.get("text_speed", "normal"):
		"slow": visible_count = mini(work.total, floori(_elapsed * 30.0 / (maxi(int(work.get("speed", 1)), 0) + 1)))
		"fast": visible_count = mini(work.total, work.pos * 2)
		"instant": visible_count = work.total
		_: visible_count = work.pos
	var clips: Variant = audio.get("clips")
	var voice_complete: bool = clips is Array and (clips.is_empty() or (audio.get("status") == "finished" and int(audio.get("clip_index", -1)) == clips.size() - 1))
	var native_complete: bool = text.get("line_id") == work.line_id and text.get("k_disp_inc_result") == 1
	if native_complete and visible_count >= work.total: _text_elapsed += clampf(delta, 0.0, 0.1)
	else: _text_elapsed = 0.0
	if not native_complete or not voice_complete: _ready_elapsed = 0.0; return false
	if visible_count < work.total: return true
	_complete = true
	_ready_elapsed += clampf(delta, 0.0, 0.1)
	return _ready_elapsed < float(settings.get("auto_delay", 0.0))

func reset() -> void:
	identity = ""; visible_count = -1; _elapsed = 0.0; _ready_elapsed = 0.0; _text_elapsed = 0.0; _complete = false
	_advance_taken = false; _total = 0

func reading_complete() -> bool:
	return _complete

## T-30002 WP9 hook for the dialogue textbox and VN presenters. `line` is a
## read-only view: {"identity": text-layer identity (read_history_service.
## text_layer_identity) or another stable presenter id, "pos": native revealed
## code units, "total": code units in the line, "speed": native frames per unit
## (optional), "native_complete": the native reveal finished, "voice_complete":
## the line's voice finished (optional, true when silent)}. visible_count is the
## count to draw. Returns true while presentation holds the next native input
## sample. At the defaults ('normal', auto_delay 0) it never holds and
## visible_count == pos: a native completion counts as the reveal being done
## even if pos has not reached total. Only 'slow' can hold for the reveal.
func step_line(line: Dictionary, delta: float) -> bool:
	if not line.get("identity") is String or line.identity.is_empty() or not line.get("pos") is int or not line.get("total") is int or line.total <= 0:
		reset(); return false
	if line.identity != identity:
		identity = line.identity; _elapsed = 0.0; _ready_elapsed = 0.0; _text_elapsed = 0.0; _complete = false; _advance_taken = false
	var dt := clampf(delta, 0.0, 0.1)
	_elapsed += dt
	_total = line.total
	var pos: int = clampi(line.pos, 0, line.total)
	var native_complete: bool = line.get("native_complete") == true
	var voice_complete: bool = line.get("voice_complete", true) == true
	var speed: String = str(settings.get("text_speed", "normal"))
	match speed:
		"slow": visible_count = mini(line.total, floori(_elapsed * 30.0 / (maxi(int(line.get("speed", 1)), 0) + 1)))
		"fast": visible_count = line.total if native_complete else mini(line.total, pos * 2)
		"instant": visible_count = line.total
		_: visible_count = pos
	var reveal_done: bool = native_complete and (speed != "slow" or visible_count >= line.total)
	if not reveal_done:
		_text_elapsed = 0.0; _ready_elapsed = 0.0
		# Slow pacing holds the native input until every glyph is shown.
		return native_complete and voice_complete
	_text_elapsed += dt
	if not voice_complete:
		_ready_elapsed = 0.0; return false
	_complete = true
	_ready_elapsed += dt
	return _ready_elapsed < float(settings.get("auto_delay", 0.0))

## One-shot: true once per line when accessibility.auto_advance is on, the text
## has been complete for Access.min_reading_time(total) and text and voice have
## both been complete for auto_delay. The caller then pulses the source-faithful
## confirm input; pacing never advances native state itself.
func take_auto_advance() -> bool:
	if _advance_taken or not _complete or settings.get("auto_advance", false) != true: return false
	if _ready_elapsed < float(settings.get("auto_delay", 0.0)) or _text_elapsed < Access.min_reading_time(_total): return false
	_advance_taken = true
	return true
