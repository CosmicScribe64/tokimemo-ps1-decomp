extends RefCounted
## T-29025: one native opening iteration per supplied input/service schedule.
## Source exit: name_entry.c::name_entry_mode_stage_shiori/exit_to_prologue.
## Presentation and physical I/O publish only after the session commits.
## The application verifies clock/residency/resource provenance before begin;
## this controller checks the supplied context's shape and supported route.

const Entry = preload("res://flow/new_game_entry.gd")
const Adapter = preload("res://flow/new_game_state_adapter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const CoreState = preload("res://core/game_state.gd")
const Session = preload("res://core/game_session.gd")
const Router = preload("res://core/mode_router.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")

class OpeningRouter extends Router:
	var entry_exit: Callable
	var resident_loader: Callable
	func missing_handler(mode: int) -> String:
		if mode == 0x14:
			return "name-entry exit handler is unavailable" if not entry_exit.is_valid() else ""
		if mode == 2:
			return "opening resident-overlay handler is unavailable" if not resident_loader.is_valid() else ""
		return super.missing_handler(mode)
	func handler(mode: int) -> Callable:
		if mode == 0x14:
			return entry_exit
		return resident_loader if mode == 2 else super.handler(mode)

var session: Session
var state: State
var error := ""
var command_ready := false
var publish_error := ""
var _entry: Entry
var _services: Dictionary = {}
var _participants: Array[RefCounted] = []
var _owners: Array[Object] = []
var _publish: Array[Callable] = []
var _in_step := false
var _handed_off := false

func begin(entry: Entry, rng: Rng, context: Dictionary, constants: Dictionary,
		services: Dictionary, participants: Array[RefCounted], publishers: Array[Callable]) -> String:
	if session != null:
		return "an opening session already owns this controller"
	if entry == null or rng == null or entry.get("_rng") != rng or not rng.configuration_error().is_empty():
		return "opening must retain the committed entry's configured RNG object"
	for key: String in ["frame_count", "system_seed", "timer_ticks", "screen_brightness", "held_input", "resident_overlay_lba"]:
		if typeof(context.get(key)) != TYPE_INT:
			return "opening needs explicit current clock, brightness and input context"
	if context.frame_count < 0 or context.frame_count > 0xffffffff or context.system_seed < -0x80000000 or context.system_seed > 0xffffffff or context.screen_brightness < 0 or context.screen_brightness > 128 or context.held_input < 0 or context.held_input > 0xffff:
		return "opening context exceeds native field bounds"
	if context.resident_overlay_lba != 0x95ea:
		return "interactive exit only supports a prepared resident NAME_ENT context"
	if context.timer_ticks < -0x80000000 or context.timer_ticks > 0xffffffff:
		return "opening timer context exceeds native word bounds"
	var required: Array = Arrival.REQUIRED + Reload.REQUIRED + ["name_entry_exit_draw", "overlay_draw", "logical_draw", "cd_tick", "audio_tick", "completion"]
	for key: String in required:
		if not services.get(key) is Callable or not services[key].is_valid():
			return "interactive opening needs prepared service: " + key
	for participant: RefCounted in participants:
		if participant == null or not participant.has_method("checkpoint") or not participant.has_method("restore"):
			return "opening mutable service needs checkpoint/restore ownership"
	for publisher: Callable in publishers:
		if not publisher.is_valid():
			return "opening publisher is unavailable"
	var prepared: Adapter.Outcome = Adapter.from_entry(entry, context.frame_count, context.system_seed, constants)
	if not prepared.error.is_empty():
		return prepared.error
	prepared.state.timer_ticks = Math.s32(context.timer_ticks)
	# NAME_ENT commit calls mode_step_next: its exit begins at stage 2/step 5,
	# substep/timer zero, before the native 65-tick hold and brightness fade.
	prepared.state.reset_game_mode_fields(0x14)
	prepared.state.mode_stage = 2
	prepared.state.mode_step = 5
	prepared.state.ovl_target_game_mode = 0x14
	prepared.state.opening["overlay_phase"] = 0
	prepared.state.opening["resident_overlay_lba"] = context.resident_overlay_lba
	prepared.state.opening["screen_brightness"] = context.screen_brightness
	prepared.state.opening["origin"] = "interactive-entry-owned-state-v1"
	prepared.state.opening["clock_profile"] = "explicit desktop input/service schedule; native clock values supplied by owner"
	var next := Session.new()
	var router := OpeningRouter.new()
	router.entry_exit = _exit_step
	router.resident_loader = _resident_loader_step
	next.router = router
	var failure: String = router.bind_mode(0x15, _prologue_step)
	if failure.is_empty():
		failure = router.bind_mode(0x34, _command_reload_step)
	if not failure.is_empty():
		return failure
	next.state = prepared.state
	next.rng = rng
	next.latched_input = InputFrame.new(-1, context.held_input, context.held_input)
	next.draw_logic = _logical_draw
	next.cd_logic = _cd_tick
	next.audio_logic = _audio_tick
	next.service_completion = _completion
	_entry = entry
	_services = services.duplicate()
	_participants = participants.duplicate()
	_publish = publishers.duplicate()
	# Callable handles alone do not establish the lifetime of RefCounted peers.
	for value: Variant in _services.values() + _publish:
		if not value is Callable:
			continue
		var owner: Object = value.get_object()
		if owner != null and not _owners.has(owner):
			_owners.append(owner)
	session = next
	state = prepared.state
	error = ""
	return ""

func step(input: InputFrame, completions: Array[Session.ServiceEvent]) -> Session.FrameResult:
	var failed := Session.FrameResult.new()
	if session == null or _in_step or command_ready or not publish_error.is_empty():
		failed.error = "opening cannot step before preparation, reentrantly, or after handoff/failure"
		return failed
	var checkpoints: Array[Dictionary] = []
	for participant: RefCounted in _participants:
		var value: Variant = participant.call("checkpoint")
		if not value is Dictionary:
			failed.error = "opening service checkpoint is not a data record"
			return failed
		checkpoints.append(value.duplicate(true))
	_in_step = true
	var result: Session.FrameResult = session.step(input, completions)
	if not result.committed:
		for index in range(_participants.size() - 1, -1, -1):
			var restored: Variant = _participants[index].call("restore", checkpoints[index])
			if not restored is String or not restored.is_empty():
				publish_error = "opening service rollback failed"
		_in_step = false
		return result
	# Prepared publishers validate support during the logical callbacks. Host I/O
	# failure after commit is terminal; it must never replay an already issued effect.
	for publisher: Callable in _publish:
		var outcome: Variant = publisher.call(result, input.tick)
		if outcome is String and not outcome.is_empty():
			publish_error = outcome
			break
		if outcome is Dictionary and not str(outcome.get("error", "")).is_empty():
			publish_error = str(outcome.error)
			break
		if not outcome is String and not outcome is Dictionary:
			publish_error = "opening publisher returned an unknown completion result"
			break
	command_ready = publish_error.is_empty() and state.game_mode == 0x34 and state.mode_stage == 1 and bool(state.opening.get("command_ready", false))
	_in_step = false
	return result

func take_command_handoff() -> Dictionary:
	if not command_ready or _in_step or _handed_off or not publish_error.is_empty():
		return {"error":"opening has no committed command-selection handoff"}
	_handed_off = true
	return {"error":"","session":session,"state":state,"rng":session.rng,"entry":_entry}

func _exit_step(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var current: State = base as State
	if current == null or current.mode_stage != 2 or current.mode_step != 5:
		result.error = "NAME_ENT exit requires committed Shiori profile stage 2/step 5"
		return
	current.mode_step_timer = Math.add32(current.mode_step_timer, 1)
	match current.substep:
		0:
			if Math.u32(current.mode_step_timer) >= 0x41:
				current.substep += 1
		1:
			var brightness: int = int(current.opening["screen_brightness"])
			if brightness >= 4:
				current.opening["screen_brightness"] = brightness - 4
			else:
				current.substep += 1
				current.opening["screen_brightness"] = 0
		2:
			current.reset_game_mode_fields(0x15)
			# game_mode_set calls game_mode_request_overlay even for a resident
			# NAME_ENT image. Its loader still consumes start and enter iterations.
			current.ovl_target_game_mode = 0x15
			current.game_mode = 2
			current.rule_state["voice_wait_frames"] = 0
			current.rule_state["message_title_visible"] = false
			current.opening["text_advance"] = 14
		_:
			result.error = "NAME_ENT exit substep is outside original dispatch"
	if result.error.is_empty():
		_services["name_entry_exit_draw"].call(current, result)

func _resident_loader_step(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	# overlay_loader.c::ovl_load_start/ovl_load_enter, resident LBA 0x95EA only.
	var current: State = base as State
	if current.ovl_target_game_mode != 0x15 or current.opening.get("resident_overlay_lba") != 0x95ea:
		result.error = "opening overlay transition is outside the prepared resident NAME_ENT path"
		return
	match int(current.opening.get("overlay_phase", -1)):
		0:
			current.opening["overlay_phase"] = 3
		3:
			var step: int = current.mode_step
			var substep: int = current.substep
			var mode_scratch: int = current.game_mode_scratch
			var stage_scratch: int = current.mode_stage_scratch
			current.game_mode = current.ovl_target_game_mode
			current.set_stage(current.mode_stage)
			current.set_step(step)
			current.substep = substep
			current.game_mode_scratch = mode_scratch
			current.mode_stage_scratch = stage_scratch
			current.opening["overlay_phase"] = 0
		_:
			result.error = "opening resident-overlay loader phase is unsupported"
	if result.error.is_empty():
		_services["overlay_draw"].call(current, result)

func _merge(inner: Session.FrameResult, outer: Session.FrameResult) -> void:
	outer.error = inner.error
	outer.audio.append_array(inner.audio)
	outer.presentation.append_array(inner.presentation)
	outer.transitions.append_array(inner.transitions)

func _prologue_step(base: CoreState, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	_merge(Arrival.step(base as State, rng, input.pressed, _services), result)

func _command_reload_step(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var current: State = base as State
	if current.mode_stage != 0xff:
		result.error = "opening controller only owns command reload; hand off the same session for selection"
		return
	_merge(Reload.step(current, _services), result)

func _logical_draw(base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
	_services["logical_draw"].call(base, rng, result)

func _cd_tick(base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
	_services["cd_tick"].call(base, rng, result)

func _audio_tick(base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
	_services["audio_tick"].call(base, rng, result)

func _completion(event: Session.ServiceEvent, base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
	_services["completion"].call(event, base, rng, result)
