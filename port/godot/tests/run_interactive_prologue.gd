extends "res://tests/run_new_game_entry.gd"
## Invented service schedules test transaction mechanics; no route parity claim.

const Live = preload("res://flow/interactive_prologue.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")

class Provider extends RefCounted:
	var calls: Array[String] = []
	var published := 0
	var fail_draw := false
	var fail_publish := false
	func checkpoint() -> Dictionary:
		return {"calls":calls.duplicate()}
	func restore(record: Dictionary) -> String:
		calls.assign(record.calls)
		return ""
	func phase(_state, _rng, result) -> void:
		calls.append("phase")
		if fail_draw:
			result.error = "invented phase failure"
	func draw(_state, _a = null, _b = null, _c = null, _d = null, _e = null) -> void:
		calls.append("draw")
	func completion(event, _state, _rng, result) -> void:
		calls.append("completion")
		if event.service != "invented.ready":
			result.error = "invented unknown completion"
	func bank(_state, _role, result) -> void:
		calls.append("bank_request")
		result.audio.append({"kind":"invented_bank_request"})
	func poll(_state, _result) -> int:
		return 0
	func publish(result, _tick) -> String:
		if not result.committed:
			return "uncommitted effect escaped"
		published += 1
		return "invented host failure" if fail_publish else ""
	func services() -> Dictionary:
		var result: Dictionary = {}
		for key in Live.Arrival.REQUIRED + Live.Reload.REQUIRED:
			result[key] = draw
		result["bank_load_start"] = bank
		result["bank_poll"] = poll
		result["logical_draw"] = phase
		result["cd_tick"] = phase
		result["audio_tick"] = phase
		result["completion"] = completion
		result["name_entry_exit_draw"] = draw
		result["overlay_draw"] = draw
		return result

func committed_entry(rng: Rng) -> Entry:
	var entry := Entry.new()
	check(entry.begin(invented_tables(), invented_content(), rng).is_empty(), "invented entry prepares")
	finish_names(entry, rng, true)
	for kind: String in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)
	return entry

func _initialize() -> void:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 12345)
	var entry := committed_entry(rng)
	var before_rng: Dictionary = rng.checkpoint()
	var peer := Provider.new()
	var controller := Live.new()
	var context := {"frame_count":77,"system_seed":91,"timer_ticks":0,"screen_brightness":8,"held_input":0x800,"resident_overlay_lba":0x95ea}
	check(controller.begin(entry, rng, {}, {"initial_mascot":0}, peer.services(), [peer], [peer.publish]) != "", "no default clock/input/brightness handoff")
	check(controller.begin(entry, rng, context, {"initial_mascot":0}, peer.services(), [peer], [peer.publish]).is_empty(), "explicit profile handoff configures single-frame controller")
	check(controller.session.rng == rng and rng.checkpoint() == before_rng, "same RNG object retained without bootstrap draws")
	check(controller.state.game_mode == 0x14 and controller.state.mode_stage == 2 and controller.state.mode_step == 5, "committed profile first enters native exit stage")
	for tick in range(64):
		var frame = controller.step(InputFrame.new(tick, 0), [])
		check(frame.committed, "exit frame commits")
	check(controller.state.substep == 0 and controller.state.mode_step_timer == 64, "no prologue transition before native65tickhold")
	controller.step(InputFrame.new(64, 0), [])
	check(controller.state.substep == 1 and controller.state.opening.screen_brightness == 8, "65thtick armsfade without consumingfadeframe")
	controller.step(InputFrame.new(65, 0), [])
	controller.step(InputFrame.new(66, 0), [])
	check(controller.state.substep == 1 and controller.state.opening.screen_brightness == 0, "brightnessdecrements4perframe reachingzero without earlytransition")
	controller.step(InputFrame.new(67, 0), [])
	check(controller.state.substep == 2 and controller.state.game_mode == 0x14, "separatezero-brightnessframe armsnativeModechange")
	controller.step(InputFrame.new(68, 0), [])
	check(controller.state.game_mode == 2 and controller.state.ovl_target_game_mode == 0x15 and controller.state.opening.text_advance == 14, "mode setter enters resident overlay loader and preserves text advance")
	check(rng.checkpoint() == before_rng and controller.state.frame_count == 146, "exitpreservesRNG andadds69actualiterations")
	controller.step(InputFrame.new(69, 0), [])
	check(controller.state.game_mode == 2 and controller.state.opening.overlay_phase == 3, "resident loader start does not collapse the enter iteration")
	controller.step(InputFrame.new(70, 0), [])
	check(controller.state.game_mode == 0x15 and controller.state.frame_count == 148, "resident loader enters prologue after two actual iterations")
	var model: Dictionary = controller.state.projection()
	var logical_clock: Dictionary = controller.session.clock.snapshot()
	var calls_before: Array = peer.calls.duplicate()
	var publish_before: int = peer.published
	peer.fail_draw = true
	var failed = controller.step(InputFrame.new(71, 0), [])
	check(not failed.committed and controller.state.projection() == model and rng.checkpoint() == before_rng, "failedfirstprologuedraw restores model andRNG")
	check(controller.session.clock.snapshot() == logical_clock and peer.calls == calls_before and peer.published == publish_before, "mutablepeer rollback precedes anyhostpublication")
	peer.fail_draw = false
	var retried = controller.step(InputFrame.new(71, 0), [])
	check(retried.committed and controller.state.mode_step == 1 and peer.calls.has("bank_request"), "sameinputtick retriesonebankrequest afterrollback")
	var before_bad: Dictionary = controller.state.projection()
	var event := Session.ServiceEvent.new(72, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, "unprepared.service")
	check(not controller.step(InputFrame.new(72, 0), [event]).committed and controller.state.projection() == before_bad, "unknowncompletionrollsback beforeadvancingnativeframe")
	peer.fail_publish = true
	var committed = controller.step(InputFrame.new(72, 0), [])
	check(committed.committed and not controller.publish_error.is_empty(), "postcommithostfailure stayscommitted andterminal")
	check(not controller.step(InputFrame.new(73, 0), []).committed, "terminalpublisherfailure cannotreplaythealreadycommittedrequest")
	print("PASS: %d interactive opening transaction checks" % checks if failures == 0 else "FAIL: %d/%d interactive opening checks" % [failures, checks])
	quit(0 if failures == 0 else 1)
