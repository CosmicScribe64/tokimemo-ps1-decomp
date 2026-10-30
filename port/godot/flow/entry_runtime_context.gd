extends RefCounted
## T-29025: explicit live entry context, independent timer/RNG ownership.
## This exports observations; it does not simulate missing boot/title or timers.
const Core = preload("res://core/game_state.gd")
const Clock = preload("res://core/native_clock.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Math = preload("res://core/native_math.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Rng = preload("res://core/native_rng.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const EntryPad = preload("res://ui/entry_pad.gd")
const NAME_ENT_SHA := "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2"
var state: Core
var clock: Clock
var input_owner: EntryPad
var policy: Dictionary = {}
var _last_input: InputFrame
var _brightness: int = -1
var _resident: Dictionary = {}
var _last_frame: int = -1
var _last_logic: int = -1
var _interrupt_observations: int = 0
var _seed_step_observations: int = 0
var _timer_log: Array[Dictionary] = []

func bind(runtime_state: Core, runtime_clock: Clock, live_pad: EntryPad, observation_policy: Dictionary) -> String:
	if state != null: return "entry runtime context already has an owner"
	if runtime_state == null or runtime_clock == null or not is_instance_valid(live_pad): return "entry requires live application state, clock and input owners"
	if observation_policy.size() != 3 or not observation_policy.has_all(["clock_origin", "timer_observation_source", "native_clock_parity"]): return "entry clock/timer observation policy is incomplete"
	for key: String in ["clock_origin", "timer_observation_source"]:
		if not observation_policy[key] is String or observation_policy[key].is_empty() or observation_policy[key].length() > 256: return "entry observation origin must be explicit"
	if not observation_policy["native_clock_parity"] is bool or observation_policy["native_clock_parity"] != false: return "native clock parity requires separate evidence; this desktop exporter makes no parity claim"
	if observation_policy["timer_observation_source"] not in ["desktop-one-timer-per-logic-v1", "explicit-external-timer-observations-v1"]: return "unsupported explicit timer observation policy"
	if runtime_state.frame_count < 0 or runtime_state.frame_count > 0xffffffff or runtime_clock.logic_iterations < 0: return "live application frame/iteration is invalid"
	if runtime_state.system_seed < -0x80000000 or runtime_state.system_seed > 0xffffffff or runtime_state.timer_ticks < -0x80000000 or runtime_state.timer_ticks > 0xffffffff: return "live independent timer words exceed native bounds"
	state = runtime_state
	clock = runtime_clock
	input_owner = live_pad
	policy = observation_policy.duplicate(true)
	return ""

func observe_timer_interrupts(count: int) -> String:
	if state == null or count < 1 or count > 4096: return "timer observation requires a bound owner and bounded positive count"
	if policy["timer_observation_source"] != "explicit-external-timer-observations-v1": return "external interrupts cannot be added to the deterministic desktop timer policy"
	for _i: int in range(count): state.timer_interrupt()
	_interrupt_observations += count
	_timer_log.append({"kind":"timer_tick","count":count,"at_logic":clock.logic_iterations})
	return ""

func observe_timer_seed_step() -> String:
	if state == null: return "timer seed step requires a live owner"
	state.timer_seed_step()
	_seed_step_observations += 1
	_timer_log.append({"kind":"timer_seed_step","count":1,"at_logic":clock.logic_iterations})
	return ""

func record_completed_iteration(sample: InputFrame, rendered_ui_brightness: int, residency: Dictionary) -> String:
	if state == null or sample == null or clock.paused or not is_instance_valid(input_owner) or not input_owner.owns_sample(sample): return "entry iteration requires an active live state/clock/input owner"
	if sample.tick != clock.logic_iterations or sample.held > 0xffff: return "entry sample does not belong to this actual logic iteration"
	if rendered_ui_brightness < 0 or rendered_ui_brightness > 128: return "entry UI brightness was not explicitly observed"
	if residency.size() != 4 or not residency.has_all(["lba", "source_sha256", "content_sha256", "ready"]): return "NAME_ENT residency ownership is incomplete"
	if not residency["lba"] is int or residency["lba"] != 0x95ea or not residency["source_sha256"] is String or residency["source_sha256"] != NAME_ENT_SHA or not residency["content_sha256"] is String or residency["content_sha256"] not in [Entry.CONTENT_SHA,Entry.EXPANDED_CONTENT_SHA] or not residency["ready"] is bool or not residency["ready"]: return "NAME_ENT source resources are not verified/prepared"
	# Explicit desktop adaptation: one callback after dispatch and before the
	# next input latch. Frequency/phase parity with the PS1 timer is unmeasured.
	if policy["timer_observation_source"] == "desktop-one-timer-per-logic-v1":
		state.timer_interrupt()
		_interrupt_observations += 1
		_timer_log.append({"kind":"timer_tick","count":1,"at_logic":clock.logic_iterations,"phase":"DRAW_BEFORE_INPUT"})
	# boot_loop increments frame_count after dispatch, including the profile
	# commit. The next controller starts at the NEXT mode iteration's frame.
	state.frame_count = Math.u32(state.frame_count + 1)
	clock.logic_iterations += 1
	_last_frame = state.frame_count
	_last_logic = clock.logic_iterations
	_last_input = InputFrame.new(sample.tick, sample.held, sample.held)
	_brightness = rendered_ui_brightness
	_resident = residency.duplicate(true)
	return ""

func export_profile(entry: Entry, rng: Rng, original_text: TextStore) -> Dictionary:
	if state == null or _last_input == null or not is_instance_valid(input_owner) or _brightness < 0 or _resident.is_empty(): return {"error":"entry has no completed live clock/input/presentation/resource observation"}
	if state.frame_count != _last_frame or clock.logic_iterations != _last_logic: return {"error":"entry context is stale relative to the application owner"}
	if entry == null or entry.stage != 2 or entry.step_id != 5 or entry.get("_rng") != rng or rng == null or not rng.configuration_error().is_empty(): return {"error":"profile handoff requires committed entry and its exact live RNG object"}
	if original_text == null: return {"error":"profile handoff requires authenticated original mascot constants"}
	var constants: Dictionary = original_text.initial_constants()
	if constants.size() != 1 or not constants.get("initial_mascot") is int or constants["initial_mascot"] < 0 or constants["initial_mascot"] > 255: return {"error":"authenticated original mascot constant is unavailable"}
	return {"error":"", "entry":entry, "rng":rng, "input_owner":input_owner,
		"context":{"frame_count":state.frame_count,"system_seed":state.system_seed,"timer_ticks":state.timer_ticks,"screen_brightness":_brightness,"held_input":_last_input.held,"resident_overlay_lba":_resident["lba"]},
		"constants":constants,
		"provenance":{"clock_origin":policy["clock_origin"],"frame_boundary":"next-mode-iteration-after-profile-commit","logic_iterations":clock.logic_iterations,"input_tick":_last_input.tick,"native_clock_parity":false,"timer_observation_source":policy["timer_observation_source"],"timer_ticks":state.timer_ticks,"timer_interrupt_observations":_interrupt_observations,"timer_seed_step_observations":_seed_step_observations,"rng_checkpoint":rng.checkpoint(),"resident":_resident.duplicate(true)}}

func timer_observation_log() -> Array[Dictionary]:
	return _timer_log.duplicate(true)
