extends RefCounted
## Route driver: port player for tokimemo.input-script.v2 (and v1).
##
## The same boundary rule as tools/emu/route_script.py (Rule) and
## tools/emu/route_driver/player.lua: feed() is called once per boundary, after
## a game iteration has finished and before the next one samples the pad, with
## the state of the finished iteration (post-update frame_count f). It returns
## the button edges for iteration f + 1 and records anchors, skipped segments,
## delivered edges, fired checkpoints, the first failure and a save_relaunch
## stop. It never touches game state; the caller turns the edges into ordinary
## joypad InputEvents (deliver()).
##
## Observed state keys: frame_count, game_mode, mode_stage, mode_step, substep,
## year, month, day, weekday, day_count, menu_cursor, icon_count (a key the
## caller cannot observe is absent and never matches a predicate that names it).

const SCHEMAS := ["tokimemo.input-script.v1", "tokimemo.input-script.v2"]
const PREDICATE_KEYS := ["game_mode", "mode_stage", "mode_step", "substep", "year", "month", "day", "weekday",
	"day_count", "menu_cursor", "icon_count"]
## Godot joypad buttons for the PS1 pad names (input_remap_service defaults).
## L2/R2 are analog axes in Godot and are refused rather than approximated.
const BUTTONS := {"UP": JOY_BUTTON_DPAD_UP, "RIGHT": JOY_BUTTON_DPAD_RIGHT, "DOWN": JOY_BUTTON_DPAD_DOWN,
	"LEFT": JOY_BUTTON_DPAD_LEFT, "CIRCLE": JOY_BUTTON_A, "CROSS": JOY_BUTTON_B, "SQUARE": JOY_BUTTON_X,
	"TRIANGLE": JOY_BUTTON_Y, "START": JOY_BUTTON_START, "SELECT": JOY_BUTTON_BACK,
	"L1": JOY_BUTTON_LEFT_SHOULDER, "R1": JOY_BUTTON_RIGHT_SHOULDER}
## PS1 pad masks (include/game_enums.h) for the delivery check.
const MASKS := {"L2": 0x1, "R2": 0x2, "L1": 0x4, "R1": 0x8, "TRIANGLE": 0x10, "CIRCLE": 0x20, "CROSS": 0x40,
	"SQUARE": 0x80, "SELECT": 0x100, "START": 0x800, "UP": 0x1000, "RIGHT": 0x2000, "DOWN": 0x4000, "LEFT": 0x8000}
const EXPECT_LIMIT := 600

var segments: Array = []
var checkpoints: Array = []
## unsampled_ok predicates: boundary states after which a missed pulse press is tolerated.
var unsampled_ok: Array = []
var failure := ""
var stop := ""
var done := false
var anchors: Dictionary = {}
var skipped: Array = []
var unsampled: Array = []
var delivered: Array = []
var fired: Array = []
var _index := 0
var _anchor := -1
var _search_from := -1
var _waiting_since := -1
var _releases: Dictionary = {}
var _expect: Dictionary = {}
var _last := -1
var _pulse_k := 0
var _pulse_last := -1
var _prev_when: Dictionary = {}
var _counts: Dictionary = {}
var _held_due: Dictionary = {}
var _prev_state: Dictionary = {}

## Returns "" or the reason the script cannot be played on the port.
func configure(data: Dictionary) -> String:
	if not data.get("schema") in SCHEMAS: return "not an input script (v1 or v2)"
	for segment: Dictionary in data.get("segments", []):
		var kind := str(segment.get("kind", "input"))
		if kind == "repeat": return "segment %s: unexpanded repeat (run tools/emu/route_script.py expand)" % segment.get("id")
		var names: Array = []
		for event: Dictionary in segment.get("events", []): names.append_array(event.get("buttons", []))
		if kind == "pulse":
			names.append_array(segment.pulse.buttons)
			for choice: Dictionary in segment.pulse.get("choose", []): names.append_array(choice.buttons)
		for button: String in names:
			if not BUTTONS.has(button): return "segment %s: button %s has no port joypad mapping" % [segment.get("id"), button]
	segments = data.get("segments", [])
	for checkpoint: Variant in data.get("checkpoints", []):
		if checkpoint is Dictionary: checkpoints.append(checkpoint)
	for row: Variant in data.get("unsampled_ok", []):
		if not row is Dictionary or not row.get("when") is Dictionary: return "unsampled_ok rows are {when, evidence}"
		unsampled_ok.append(row.when)
	return "" if not segments.is_empty() else "script has no segments"

static func matches(predicate: Dictionary, state: Dictionary) -> bool:
	for key: String in PREDICATE_KEYS:
		if not predicate.has(key): continue
		var want: int = str(predicate[key]).hex_to_int() if key == "game_mode" else int(predicate[key])
		if not state.has(key) or int(state[key]) != want: return false
	return true

## Canonical predicate text for messages (identical in every player).
static func ptext(predicate: Dictionary) -> String:
	var parts := PackedStringArray()
	for key: String in PREDICATE_KEYS:
		if predicate.has(key): parts.append("%s=%s" % [key, str(predicate[key]) if key == "game_mode" else str(int(predicate[key]))])
	return ",".join(parts)

static func key_text(state: Dictionary) -> String:
	return "%02x/%d/%d/%d" % [int(state.get("game_mode", -1)), int(state.get("mode_stage", -1)), int(state.get("mode_step", -1)), int(state.get("substep", -1))]

static func _any(predicates: Array, state: Dictionary) -> bool:
	for predicate: Dictionary in predicates:
		if matches(predicate, state): return true
	return false

func _fail(message: String) -> void:
	failure = message
	done = true

func _checkpoints(state: Dictionary) -> void:
	for checkpoint: Dictionary in checkpoints:
		var cid := str(checkpoint.id)
		var now := matches(checkpoint.when, state)
		var before: bool = _prev_when.get(cid, false)
		_prev_when[cid] = now
		if not now or before: continue
		var count: int = _counts.get(cid, 0)
		var name := cid
		if not checkpoint.get("recurring", false):
			if count > 0: continue
		else:
			if checkpoint.has("limit") and count >= int(checkpoint.limit): continue
			name = "%s-%03d" % [cid, count + 1]
		_counts[cid] = count + 1
		fired.append({"id": name, "checkpoint": cid, "frame_count": int(state.frame_count),
			"ram": bool(checkpoint.get("ram", false)), "save_state": bool(checkpoint.get("save_state", false))})

func _check_expect(state: Dictionary) -> void:
	if _expect.is_empty(): return
	if matches(_expect.after, state):
		if int(state.frame_count) - int(_expect.since) > EXPECT_LIMIT:
			_fail("segment %s: still in its own state %d iterations after its last press (expected %s)" % [_expect.segment, EXPECT_LIMIT, ptext(_expect.want)])
		return
	if not matches(_expect.want, state):
		_fail("segment %s: expected %s next, observed %s" % [_expect.segment, ptext(_expect.want), key_text(state)])
		return
	_expect = {}

## pad_held: the mask the game sampled for the finished iteration, or -1 when
## the caller cannot observe it. A press that should be held there and is not
## is an input slip and stops the run, except a pulse press after a boundary
## state listed in the script's unsampled_ok (a loading state in which the
## original skips the pad read): that one is recorded in unsampled.
func feed(state: Dictionary, pad_held: int = -1) -> Array:
	if done: return []
	var observed := int(state.frame_count)
	if observed == _last: return []
	if _last >= 0 and observed > _last + 1 and _edge_due_between(_last + 2, observed):
		_fail("iterations %d..%d ran without a boundary while an edge was due" % [_last + 2, observed])
		return []
	if pad_held >= 0:
		var names: Array = _held_due.keys()
		names.sort()
		for button: String in names:
			var span: Array = _held_due[button]
			if observed >= int(span[0]) and observed <= int(span[1]) and (pad_held & int(MASKS[button])) == 0:
				if bool(span[2]) or _prev_state.is_empty() or not _any(unsampled_ok, _prev_state):
					_fail("input slip: %s due for iteration %d is not in the sampled pad %04x (previous boundary %s)" % [button, observed,
						pad_held, "none" if _prev_state.is_empty() else key_text(_prev_state)])
					return []
				unsampled.append({"frame_count": observed, "button": button})
			if observed >= int(span[1]): _held_due.erase(button)
	_last = observed
	_prev_state = state
	var nxt := observed + 1
	_checkpoints(state)
	_check_expect(state)
	if done: return []
	var edges: Array = []
	var release_now: Array = []
	for button: String in _releases.keys():
		if int(_releases[button]) == nxt: release_now.append(button)
	release_now.sort()
	for button: String in release_now:
		edges.append([button, false])
		_releases.erase(button)
	while _index < segments.size():
		var segment: Dictionary = segments[_index]
		var kind := str(segment.get("kind", "input"))
		if _anchor < 0:
			if _waiting_since < 0: _waiting_since = observed
			if observed < _search_from or not matches(segment.wait_until, state):
				var timeout := int(segment.get("timeout_iterations", 0))
				if timeout > 0 and observed - _waiting_since > timeout:
					_fail("segment %s: wait_until not reached within %d iterations" % [segment.id, timeout])
				break
			_anchor = observed
			anchors[segment.id] = observed
			_pulse_k = 0
			_pulse_last = -1
			if segment.has("only_if") and not _any(segment.only_if, state):
				skipped.append(segment.id)
				_index += 1
				_search_from = observed
				_anchor = -1
				_waiting_since = -1
				continue
		if kind == "save_relaunch":
			stop = "segment %s: save_relaunch slot %d" % [segment.id, int(segment.slot)]
			done = true
			break
		var last := observed
		if kind == "pulse":
			var pulse: Dictionary = segment.pulse
			if not matches(pulse.until, state):
				var due: int = _anchor + int(pulse.get("offset", 1)) + _pulse_k * int(pulse.every)
				if _pulse_k >= int(pulse.max):
					if observed >= _pulse_last + int(pulse.every):
						_fail("segment %s: pulse exhausted after %d presses without %s" % [segment.id, int(pulse.max), ptext(pulse.until)])
					break
				if due == nxt:
					_pulse_k += 1
					_pulse_last = nxt
					if not _any(pulse.get("skip_when", []), state):
						var buttons: Array = pulse.buttons
						for choice: Dictionary in pulse.get("choose", []):
							if matches(choice.when, state):
								buttons = choice.buttons
								break
						for button: String in buttons:
							edges.append([button, true])
							_releases[button] = nxt + int(pulse.hold)
							_held_due[button] = [nxt, nxt + int(pulse.hold) - 1, false]
				break
		else:
			last = _anchor + int(segment.get("run_for", 0))
			for event: Dictionary in segment.get("events", []):
				var at: int = _anchor + int(event.at)
				last = maxi(last, at)
				if at == nxt:
					for button: String in event.buttons:
						edges.append([button, true])
						_releases[button] = at + int(event.hold)
						_held_due[button] = [at, at + int(event.hold) - 1, true]
			if nxt <= last: break
		if segment.has("expect_next"):
			_expect = {"segment": segment.id, "after": segment.wait_until, "want": segment.expect_next, "since": observed}
			_check_expect(state)
			if done: return []
		_index += 1
		_search_from = last
		_anchor = -1
		_waiting_since = -1
	for edge: Array in edges:
		delivered.append({"frame_count": nxt, "button": edge[0], "pressed": edge[1]})
	if not done and _index >= segments.size() and _releases.is_empty() and _expect.is_empty(): done = true
	return edges

func _edge_due_between(first: int, last_frame: int) -> bool:
	for button: String in _releases:
		if int(_releases[button]) >= first and int(_releases[button]) <= last_frame: return true
	if _index < segments.size() and _anchor >= 0:
		var segment: Dictionary = segments[_index]
		if str(segment.get("kind", "input")) == "pulse":
			var pulse: Dictionary = segment.pulse
			var due: int = _anchor + int(pulse.get("offset", 1)) + _pulse_k * int(pulse.every)
			return _pulse_k < int(pulse.max) and due >= first and due <= last_frame
		for event: Dictionary in segment.get("events", []):
			var at: int = _anchor + int(event.at)
			if at >= first and at <= last_frame: return true
	return false

## The id of the segment the player is on (where a stop happened), "" past the end.
func current_segment_id() -> String:
	return str(segments[_index].id) if _index < segments.size() else ""

## The unsupported row that pins this stop: same side and stop text and the
## segment the player stopped on (at_segment); {} when none does. Identical to
## tools/emu/route_script.py pinned_row.
static func pinned_row(script: Dictionary, side: String, stop_text: String, segment_id: String) -> Dictionary:
	if stop_text.is_empty(): return {}
	for row: Dictionary in script.get("unsupported", []):
		if str(row.get("side")) == side and str(row.get("stop")) == stop_text and str(row.get("at_segment", "")) == segment_id:
			return row
	return {}

## Sends edges as joypad InputEvents; the production input owner sees only these.
static func deliver(edges: Array, device: int = 0) -> void:
	for edge: Array in edges:
		var input := InputEventJoypadButton.new()
		input.device = device
		input.button_index = BUTTONS[edge[0]]
		input.pressed = edge[1]
		input.pressure = 1.0 if edge[1] else 0.0
		Input.parse_input_event(input)
	if not edges.is_empty(): Input.flush_buffered_events()

## Releases every held button (after a failure or stop).
func release_all(device: int = 0) -> void:
	var edges: Array = []
	for button: String in _releases.keys(): edges.append([button, false])
	_releases.clear()
	deliver(edges, device)
