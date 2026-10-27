extends RefCounted
## Bounded boot_loop ordering seam. Callbacks must implement sourced logic.
## No original mode/actor/CD/audio behavior is supplied by this contract.
## Callbacks may mutate only the supplied state/RNG and result records. They
## must treat input/events as immutable and perform no I/O or external effects.
## A failed step restores owned records; arbitrary external effects cannot be undone.

const Math = preload("res://core/native_math.gd")
const InputFrame = preload("res://core/input_frame.gd")
const GameState = preload("res://core/game_state.gd")
const NativeClock = preload("res://core/native_clock.gd")
const NativeRng = preload("res://core/native_rng.gd")
const ModeRouter = preload("res://core/mode_router.gd")

enum Phase { BEFORE_MODE, DRAW_BEFORE_INPUT, CD, AUDIO }
enum ServiceKind { TIMER_INTERRUPT, VBLANK, COMPLETION }

class ServiceEvent extends RefCounted:
	var tick: int
	var phase: Phase
	var kind: ServiceKind
	var service: String
	var payload: Dictionary

	func _init(tick_id: int, at_phase: Phase, event_kind: ServiceKind,
		service_id: String = "", data: Dictionary = {}) -> void:
		tick = tick_id
		phase = at_phase
		kind = event_kind
		service = service_id
		payload = data.duplicate(true)

class FrameResult extends RefCounted:
	# Keep the first reported failure, including accidental later empty writes.
	var error: String:
		get: return _error
		set(value):
			if _error.is_empty():
				_error = value
	var _error: String = ""
	var committed: bool = false
	var trace: Array[Dictionary] = []
	var presentation: Array[Dictionary] = []
	var audio: Array[Dictionary] = []
	var transitions: Array[Dictionary] = []
	var state_hash: String = ""
	var event_hash: String = ""
	var service_events_consumed: int = 0

var state: GameState = GameState.new()
var clock: NativeClock = NativeClock.new()
var rng: NativeRng = NativeRng.new()
var router: ModeRouter = ModeRouter.new()
var latched_input: InputFrame = InputFrame.new(-1)
# Required logical work, even when presentation is hidden. Never bind render draws.
var draw_logic: Callable
var cd_logic: Callable
var audio_logic: Callable
var service_completion: Callable
# Optional pure final validation, after all rule/service callbacks and before
# commit. Physical publication follows synchronously through the owner.
var final_precommit: Callable

func step(next_input: InputFrame, events: Array[ServiceEvent]) -> FrameResult:
	var result: FrameResult = FrameResult.new()
	result.error = _preflight(next_input, events)
	if not result.error.is_empty():
		result.state_hash = state_hash()
		return result
	var checkpoint: Dictionary = {"state": state.projection(), "clock": clock.snapshot(),
		"rng": rng.checkpoint(), "input": latched_input}
	var initial_rng_trace: int = rng.trace.size()
	rng.begin_step(func(reason: String) -> void: result.error = reason)
	# boot_loop: mode consumes the previous draw_frame's pad latch.
	if not _deliver(Phase.BEFORE_MODE, events, result):
		return _rollback(checkpoint, result)
	result.error = router.missing_handler(state.game_mode)
	if not result.error.is_empty():
		return _rollback(checkpoint, result)
	_record("mode", result)
	router.handler(state.game_mode).call(state, latched_input, rng, result)
	if _failed(result):
		return _rollback(checkpoint, result)
	# game_mode_dispatch increments this AFTER the handler, including its resets.
	state.game_mode_timer = Math.add32(state.game_mode_timer, 1)
	state.frame_count = Math.u32(state.frame_count + 1)
	if not state.restart_request:
		_record("draw_logic", result)
		draw_logic.call(state, rng, result)
		if _failed(result):
			return _rollback(checkpoint, result)
		if not _deliver(Phase.DRAW_BEFORE_INPUT, events, result):
			return _rollback(checkpoint, result)
		latched_input = InputFrame.new(next_input.tick, next_input.held, latched_input.held)
		_record("input_latch", result)
		if not _deliver(Phase.CD, events, result):
			return _rollback(checkpoint, result)
		cd_logic.call(state, rng, result)
		if _failed(result):
			return _rollback(checkpoint, result)
		_record("cd", result)
		if not _deliver(Phase.AUDIO, events, result):
			return _rollback(checkpoint, result)
		if state.sound_active:
			audio_logic.call(state, rng, result)
			if _failed(result):
				return _rollback(checkpoint, result)
			_record("audio", result)
	if result.service_events_consumed != events.size():
		result.error = "source restart skipped scheduled service phases; inspect retained trace"
		return _rollback(checkpoint, result)
	if final_precommit.is_valid():
		final_precommit.call(state,rng,result)
		if _failed(result):
			return _rollback(checkpoint,result)
	clock.logic_iterations += 1
	result.committed = true
	result.state_hash = state_hash()
	result.event_hash = JSON.stringify([result.presentation, result.audio, result.transitions,
		result.trace, rng.trace.slice(initial_rng_trace)]).sha256_text()
	rng.end_step()
	return result

func _preflight(next_input: InputFrame, events: Array[ServiceEvent]) -> String:
	var rng_failure: String = rng.configuration_error()
	if not rng_failure.is_empty():
		return rng_failure
	if clock.paused or state.paused:
		return "paused stepping requires sourced boot_paused_step; resume clock first"
	if state.restart_request:
		return "restart boundary requires boot initialization, outside bounded slice"
	if next_input == null or next_input.tick != clock.logic_iterations:
		return "input tick does not match next logic iteration"
	var missing: String = router.missing_handler(state.game_mode)
	if not missing.is_empty():
		return missing
	if not draw_logic.is_valid() or not cd_logic.is_valid() or not audio_logic.is_valid():
		return "logical draw/CD/audio callbacks must be supplied before stepping"
	var last_phase: int = -1
	for event: ServiceEvent in events:
		if event == null or event.tick != clock.logic_iterations:
			return "service tick does not match logic iteration"
		if event.phase < last_phase or event.phase > Phase.AUDIO or event.phase < Phase.BEFORE_MODE:
			return "service events must use nondecreasing native phases"
		if event.kind < ServiceKind.TIMER_INTERRUPT or event.kind > ServiceKind.COMPLETION:
			return "unsupported service event kind"
		if event.kind == ServiceKind.COMPLETION and (event.service.is_empty() or not service_completion.is_valid()):
			return "service completion needs an explicit service ID and receiver"
		last_phase = event.phase
	return ""

func _deliver(phase: Phase, events: Array[ServiceEvent], result: FrameResult) -> bool:
	for event: ServiceEvent in events:
		if event.phase != phase:
			continue
		match event.kind:
			ServiceKind.TIMER_INTERRUPT:
				state.timer_interrupt()
			ServiceKind.VBLANK:
				clock.vblanks += 1
			ServiceKind.COMPLETION:
				service_completion.call(event, state, rng, result)
		result.trace.append({"iteration": clock.logic_iterations, "phase": "service:%d" % phase,
			"source": "external-schedule", "service": event.service, "kind": event.kind,
			"payload": event.payload.duplicate(true), "state": state.projection()})
		if _failed(result):
			return false
		result.service_events_consumed += 1
	return true

func _failed(result: FrameResult) -> bool:
	if result.error.is_empty():
		result.error = rng.step_error()
	if result.error.is_empty():
		result.error = rng.configuration_error()
	return not result.error.is_empty()

func _rollback(checkpoint: Dictionary, result: FrameResult) -> FrameResult:
	state.restore(checkpoint["state"])
	clock.restore(checkpoint["clock"])
	rng.end_step()
	rng.restore(checkpoint["rng"])
	latched_input = checkpoint["input"]
	result.presentation.clear()
	result.audio.clear()
	result.transitions.clear()
	result.service_events_consumed = 0
	result.state_hash = state_hash()
	# Retained trace records describe an aborted attempt, never committed events.
	result.event_hash = ""
	return result

func _record(phase_name: String, result: FrameResult) -> void:
	result.trace.append({"iteration": clock.logic_iterations, "phase": phase_name,
		"source": "src/main/main_loop.c::boot_loop", "state": state.projection(),
		"input": latched_input.snapshot(), "rng_state": rng.state})

func state_hash() -> String:
	# Rendered-frame count and wall-clock debt cannot change rule state hashes.
	return JSON.stringify([state.to_words(), latched_input.tick, latched_input.held,
		latched_input.pressed, latched_input.released, rng.profile, rng.state,
		rng.draw_count, clock.logic_iterations, clock.vblanks]).sha256_text()
