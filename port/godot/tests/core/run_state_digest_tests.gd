extends RefCounted
## Per-frame digest fast paths keep their exact values and guard semantics:
## GameState.words_json (core/canonical_words.gd) renders the same text as
## JSON.stringify(to_words()), GameSession.state_hash memo recomputes on any
## change, FrameResult.event_hash rendered on first read equals the digest
## computed at commit even after the result's lists are edited, and the upload
## bridge's buffer SHA memo follows in-place byte writes.

const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const SchoolDayState = preload("res://rules/school_day_state.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")

## A subclass that overrides to_words() must keep its own words.
class ReducedState extends RouteState:
	func to_words() -> Array[int]:
		return [game_mode, mode_stage]

var failures: Array[String] = []

func run() -> Array[String]:
	failures.clear()
	_test_words_json()
	_test_state_hash_memo()
	_test_lazy_event_hash()
	_test_buffer_sha_memo()
	return failures

func _check(condition: bool, message: String) -> void:
	if not condition: failures.append("state digest: " + message)

static func _reference_state_hash(session: Session) -> String:
	return JSON.stringify([session.state.to_words(), session.latched_input.tick, session.latched_input.held,
		session.latched_input.pressed, session.latched_input.released, session.rng.profile, session.rng.state,
		session.rng.draw_count, session.clock.logic_iterations, session.clock.vblanks]).sha256_text()

func _route_state() -> RouteState:
	var state := RouteState.new()
	state.game_mode = 0x15
	state.frame_count = 0xfffffffe
	state.system_seed = -5
	state.rule_state = {"year": 95, "calendar": [{"day_kind": [0, 1, 2]}], "name": "藤崎 \"x\"\n", "f": 0.5}
	state.opening = {"screen_brightness": 128, "nested": {"list": [1, 2.25, "é"], "empty": {}}, "z": null, "b": true}
	return state

func _test_words_json() -> void:
	var core := CoreState.new()
	core.mode_stage = 3
	_check(core.words_json() == JSON.stringify(core.to_words()), "core words text")
	var school := SchoolDayState.new()
	school.rule_state = {"k": [3, {"a": -1}], "s": "あ"}
	_check(school.words_json() == JSON.stringify(school.to_words()), "school-day words text")
	var state := _route_state()
	_check(state.words_json() == JSON.stringify(state.to_words()), "route words text")
	# Every change re-renders: in place, type-only (1 vs 1.0), key order, size.
	state.opening.nested.list[0] = 7
	_check(state.words_json() == JSON.stringify(state.to_words()), "in-place nested edit re-renders")
	state.opening.screen_brightness = 128.0
	_check(state.words_json() == JSON.stringify(state.to_words()), "int to float re-renders")
	var reordered := {}
	for key: String in ["z", "b", "nested", "screen_brightness"]: reordered[key] = state.opening[key]
	state.opening = reordered
	_check(state.words_json() == JSON.stringify(state.to_words()), "reordered keys keep sorted canonical text")
	state.rule_state.erase("f")
	state.opening = {}
	_check(state.words_json() == JSON.stringify(state.to_words()), "shrunk and empty records")
	var reduced := ReducedState.new()
	reduced.opening = {"ignored": 1}
	_check(reduced.words_json() == JSON.stringify([0, 0]), "to_words override is respected")

func _session() -> Session:
	var session := Session.new()
	session.state = _route_state()
	session.latched_input = InputFrame.new(4, 0x10, 0)
	return session

func _test_state_hash_memo() -> void:
	var session := _session()
	var first := session.state_hash()
	_check(first == _reference_state_hash(session), "digest equals JSON of to_words")
	_check(session.state_hash() == first, "unchanged state reuses digest")
	session.state.opening.nested.list.append(1)
	_check(session.state_hash() != first and session.state_hash() == _reference_state_hash(session), "nested change recomputes")
	session.state.opening.nested.list.pop_back()
	_check(session.state_hash() == first, "restored value returns the original digest")
	session.state.rule_state.year = 96
	_check(session.state_hash() == _reference_state_hash(session), "rule change recomputes")
	session.rng.state = 99
	_check(session.state_hash() == _reference_state_hash(session), "RNG change recomputes")
	session.latched_input = InputFrame.new(5, 0x10, 0x10)
	_check(session.state_hash() == _reference_state_hash(session), "input change recomputes")

func _test_lazy_event_hash() -> void:
	var result := Session.FrameResult.new()
	result.presentation.append({"kind": "draw", "rect": [1, 2], "bytes": PackedByteArray([1, 2])})
	result.audio.append({"kind": "voice"})
	result.trace.append({"phase": "mode", "state": {"a": 1}})
	var rng_trace: Array = [{"caller": "x", "result": 3}]
	var eager := JSON.stringify([result.presentation, result.audio, result.transitions, result.trace, rng_trace]).sha256_text()
	result.defer_event_hash([result.presentation.duplicate(true), result.audio.duplicate(true),
		result.transitions.duplicate(true), result.trace.duplicate(), rng_trace.duplicate(true)])
	result.presentation[0].rect.append(3)
	result.presentation[0].bytes[0] = 9
	result.presentation.clear()
	result.audio.append({"kind": "late"})
	result.trace.append({"phase": "late"})
	rng_trace.clear()
	_check(result.event_hash == eager, "first read equals the commit-time digest")
	_check(result.event_hash == eager, "repeated read is stable")
	result.event_hash = ""
	_check(result.event_hash.is_empty(), "rollback clears the digest")
	var failed := Session.FrameResult.new()
	_check(failed.event_hash.is_empty(), "uncommitted result has no digest")

func _test_buffer_sha_memo() -> void:
	var vram := PackedByteArray()
	vram.resize(4096)
	var owned := {"vram_words": vram, "scratch_owner": "owner", "pending_gpu_effects": []}
	var reference := func(data: Dictionary) -> String:
		var values: Dictionary = {}
		for key: Variant in data:
			if data[key] is PackedByteArray:
				var hashing := HashingContext.new()
				hashing.start(HashingContext.HASH_SHA256)
				hashing.update(data[key])
				values[key] = hashing.finish().hex_encode()
			else: values[key] = data[key]
		return JSON.stringify(values).sha256_text()
	var first := Upload._runtime_hash(owned)
	_check(first == reference.call(owned), "runtime hash equals full SHA rendering")
	_check(Upload._runtime_hash(owned) == first, "unchanged buffers reuse digests")
	owned.vram_words[100] = 7
	_check(Upload._runtime_hash(owned) != first and Upload._runtime_hash(owned) == reference.call(owned), "in-place byte write recomputes")
	owned.vram_words[100] = 0
	_check(Upload._runtime_hash(owned) == first, "restored bytes match the original digest")
	owned.vram_words.resize(4097)
	_check(Upload._runtime_hash(owned) == reference.call(owned), "resize recomputes")
