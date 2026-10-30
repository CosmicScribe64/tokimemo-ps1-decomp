extends Node
## T-39023 (T-39018 WP-06): port player for tokimemo.input-script.v1.
##
## Delivers the script's pad edges as ordinary joypad InputEvents through
## Input.parse_input_event at game-update boundaries: this node runs first in
## every physics tick (process_physics_priority), reads the state observed after
## the last completed iteration through the caller's observe Callable, and
## flushes the events for the iteration about to run, so the production scene
## handles them in its own _input before its own _physics_process. It never sets
## capabilities, eligibility, state or queues of the game; only Input sees it.
##
## Timing rule (same as tools/emu/input_script.py): a segment's anchor A is the
## first observed iteration, at or after the previous segment's last press (or
## its anchor + run_for), whose state matches wait_until. An event {at, hold} is
## pressed for iterations A+at .. A+at+hold-1 and released before A+at+hold.
##
## observe() must return {} (no state yet) or integers frame_count, game_mode,
## mode_stage, mode_step, substep of the last completed iteration. A segment with
## expect_state also needs the observed values it names (names: hex of the entry
## name buffers; player/shiori: month, day, blood selections), checked on the
## same state as expect_next; a differing or missing value fails the script.

const SCHEMA := "tokimemo.input-script.v1"
## Godot joypad buttons for the PS1 pad names. L2/R2 are analog axes in Godot
## and are refused rather than approximated.
const BUTTONS := {"UP": JOY_BUTTON_DPAD_UP, "RIGHT": JOY_BUTTON_DPAD_RIGHT, "DOWN": JOY_BUTTON_DPAD_DOWN,
	"LEFT": JOY_BUTTON_DPAD_LEFT, "CIRCLE": JOY_BUTTON_A, "CROSS": JOY_BUTTON_B, "SQUARE": JOY_BUTTON_X,
	"TRIANGLE": JOY_BUTTON_Y, "START": JOY_BUTTON_START, "SELECT": JOY_BUTTON_BACK,
	"L1": JOY_BUTTON_LEFT_SHOULDER, "R1": JOY_BUTTON_RIGHT_SHOULDER}
const PREDICATE_KEYS := ["game_mode", "mode_stage", "mode_step", "substep"]
## Iterations a completed segment may stay in its own state before expect_next counts as missed.
const EXPECT_LIMIT := 600

signal segment_anchored(segment_id: String, frame_count: int)
signal finished(failure: String)

var device := 0
var segments: Array = []
var observe: Callable
var failure := ""
var done := false
var delivered: Array[Dictionary] = []
var anchors: Dictionary = {}
var _index := 0
var _anchor := -1
var _search_from := -1
var _waiting_since := -1
var _releases: Dictionary = {}
var _expect: Dictionary = {}
var _last_observed := -1

func _init() -> void:
	process_physics_priority = -1000

## Returns "" or the reason the script cannot be played on the port.
func configure(data: Dictionary, observer: Callable) -> String:
	if data.get("schema") != SCHEMA: return "not a %s script" % SCHEMA
	for segment: Dictionary in data.get("segments", []):
		for event: Dictionary in segment.get("events", []):
			for button: String in event.get("buttons", []):
				if not BUTTONS.has(button): return "segment %s: button %s has no port joypad mapping" % [segment.get("id"), button]
	segments = data.get("segments", [])
	observe = observer
	return "" if not segments.is_empty() else "script has no segments"

static func matches(predicate: Dictionary, state: Dictionary) -> bool:
	for key: String in PREDICATE_KEYS:
		if not predicate.has(key): continue
		var want: int = str(predicate[key]).hex_to_int() if key == "game_mode" else int(predicate[key])
		if int(state.get(key, -1)) != want: return false
	return true

func _physics_process(_delta: float) -> void:
	boundary()

## One game-update boundary. Public so a runner without physics ticks can call it.
func boundary() -> void:
	if done or not failure.is_empty() or not observe.is_valid(): return
	var state: Dictionary = observe.call()
	if state.is_empty(): return
	var observed := int(state.frame_count)
	# A tick in which the game ran no iteration is not a new boundary; a tick in
	# which it ran several skipped boundaries, which is only safe with no edge due.
	if observed == _last_observed: return
	if _last_observed >= 0 and observed > _last_observed + 1 and _edge_due_between(_last_observed + 2, observed):
		_fail("iterations %d..%d ran without a boundary while an edge was due" % [_last_observed + 2, observed])
		return
	_last_observed = observed
	var next := observed + 1
	_check_expect(state)
	var events: Array[Dictionary] = []
	for button: String in _releases.keys():
		if int(_releases[button]) == next:
			events.append({"button": button, "pressed": false})
			_releases.erase(button)
	while _index < segments.size():
		var segment: Dictionary = segments[_index]
		if _anchor < 0:
			if _waiting_since < 0: _waiting_since = observed
			if observed < _search_from or not matches(segment.wait_until, state):
				var timeout := int(segment.get("timeout_iterations", 0))
				if timeout > 0 and observed - _waiting_since > timeout:
					_fail("segment %s: wait_until not reached within %d iterations" % [segment.id, timeout])
				break
			_anchor = observed
			anchors[segment.id] = observed
			segment_anchored.emit(segment.id, observed)
		var last := _anchor + int(segment.get("run_for", 0))
		for event: Dictionary in segment.events:
			var at: int = _anchor + int(event.at)
			last = maxi(last, at)
			if at == next:
				for button: String in event.buttons:
					events.append({"button": button, "pressed": true})
					_releases[button] = at + int(event.hold)
		if next <= last: break
		if segment.has("expect_next"):
			# The state observed now is already the last press's iteration: check it first,
			# so a one-iteration transition state is not missed.
			_expect = {"segment": segment.id, "after": segment.wait_until, "want": segment.expect_next,
				"values": segment.get("expect_state", {}), "since": observed}
			_check_expect(state)
			if not failure.is_empty(): return
		_index += 1
		_search_from = last
		_anchor = -1
		_waiting_since = -1
	for event: Dictionary in events:
		var input := InputEventJoypadButton.new()
		input.device = device
		input.button_index = BUTTONS[event.button]
		input.pressed = event.pressed
		input.pressure = 1.0 if event.pressed else 0.0
		Input.parse_input_event(input)
		delivered.append({"frame_count": next, "button": event.button, "pressed": event.pressed})
	if not events.is_empty(): Input.flush_buffered_events()
	if _index >= segments.size() and _releases.is_empty() and _expect.is_empty():
		done = true
		finished.emit(failure)

func _edge_due_between(first: int, last: int) -> bool:
	for button: String in _releases:
		if int(_releases[button]) >= first and int(_releases[button]) <= last: return true
	if _index < segments.size() and _anchor >= 0:
		for event: Dictionary in segments[_index].events:
			var at: int = _anchor + int(event.at)
			if at >= first and at <= last: return true
	return false

func _check_expect(state: Dictionary) -> void:
	if _expect.is_empty(): return
	if matches(_expect.after, state):
		if int(state.frame_count) - int(_expect.since) > EXPECT_LIMIT:
			_fail("segment %s: still in its own state %d iterations after its last press (expected %s)" % [_expect.segment, EXPECT_LIMIT, _expect.want])
		return
	if not matches(_expect.want, state):
		_fail("segment %s: expected %s next, observed %s" % [_expect.segment, _expect.want,
			[state.get("game_mode"), state.get("mode_stage"), state.get("mode_step"), state.get("substep")]])
		return
	var mismatches := state_mismatches(_expect.values, state)
	if not mismatches.is_empty():
		_fail("segment %s: expect_state failed: %s" % [_expect.segment, "; ".join(mismatches)])
		return
	_expect = {}

## [] when every expect_state value holds in state (hex compared lower-case).
static func state_mismatches(expect: Dictionary, state: Dictionary) -> PackedStringArray:
	var errors := PackedStringArray()
	var groups := expect.keys()
	groups.sort()
	for group: String in groups:
		var observed: Variant = state.get(group)
		var fields: Array = (expect[group] as Dictionary).keys()
		fields.sort()
		for field: String in fields:
			var want: Variant = expect[group][field]
			var got: Variant = (observed as Dictionary).get(field) if observed is Dictionary else null
			var same := false
			if want is String: same = got is String and (got as String).to_lower() == (want as String).to_lower()
			elif got != null and not got is String: same = int(got) == int(want)
			if not same:
				errors.append("%s.%s: expected %s, observed %s" % [group, field, want, "unavailable" if got == null else str(got)])
	return errors

func _fail(message: String) -> void:
	failure = message
	done = true
	for button: String in _releases.keys():
		var input := InputEventJoypadButton.new()
		input.device = device
		input.button_index = BUTTONS[button]
		input.pressed = false
		Input.parse_input_event(input)
	_releases.clear()
	Input.flush_buffered_events()
	finished.emit(failure)
