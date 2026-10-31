extends SceneTree
## ending_candidate_flag_gate and the rand() draw of
## ending_graduation_candidate_select (src/ovl/ENDING/graduation_candidate_selection.c).
## Bounded rule checks only: no route, content or presenter.
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const NativeRng = preload("res://core/native_rng.gd")

var failures: Array[String] = []
var checks: int = 0


func _initialize() -> void:
	var girls: Array = []
	for i in 15:
		girls.append({"flags": 0, "met": 0, "story_stage": 0})
	for heroine in range(0, 13):
		var r: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(heroine, girls)
		_check(r.error.is_empty() and r.redirected_girl == heroine and r.next_mode_stage == -1 and r.system_flag_or == 4,
			"candidate %d is never redirected and advances one step (|= 4)" % heroine)
	for f9 in 8:
		girls[14].flags = f9 << 9
		var y: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(EndingOverlay.GIRL_YOSHIO, girls)
		var low: bool = f9 < 3
		_check(y.next_mode_stage == (7 if low else -1) and y.system_flag_or == (8 if low else 4),
			"Yoshio with girls[14] bits 9..11 = %d %s" % [f9, "goes to stage 7 (|= 8)" if low else "advances (|= 4)"])
	girls[14].flags = (1 << 26) | (1 << 8) | (1 << 12)
	_check(EndingOverlay.evaluate_candidate_flag_gate(0xFF, girls).next_mode_stage == 7, "bits outside 9..11 are ignored; u8 candidates above 13 take the Yoshio test")
	_check(not EndingOverlay.evaluate_candidate_flag_gate(EndingOverlay.GIRL_YOSHIO, girls.slice(0, 14)).error.is_empty(),
		"a missing girls[14] flag word is refused, not defaulted")

	# rand(): drawn once and only on its source condition (Yoshio candidate,
	# Miharu story stage nonzero, scene_flags[0x4D] zero).
	var sel_girls: Array = []
	for i in 13:
		sel_girls.append({"met": 0, "story_stage": 0, "rei_subcount": 0})
	var points: Array = []
	points.resize(13)
	points.fill(0)
	var flags: Array = []
	flags.resize(0x100)
	flags.fill(0)
	var calls: Array[int] = [0]
	var rand := func() -> int:
		calls[0] += 1
		return 0
	var no_stage: Dictionary = EndingOverlay.select_graduation_candidate(sel_girls, points, flags, rand)
	_check(calls[0] == 0 and not no_stage.rand_drawn and no_stage.current_girl == EndingOverlay.GIRL_YOSHIO, "no Miharu story stage: rand() is not called")
	sel_girls[EndingOverlay.GIRL_MIHARU].story_stage = 1
	flags[0x4D] = 1
	EndingOverlay.select_graduation_candidate(sel_girls, points, flags, rand)
	_check(calls[0] == 0, "scene_flags[0x4D] set: rand() is not called")
	flags[0x4D] = 0
	var drawn: Dictionary = EndingOverlay.select_graduation_candidate(sel_girls, points, flags, rand)
	_check(calls[0] == 1 and drawn.rand_drawn and drawn.current_girl == EndingOverlay.GIRL_MIHARU, "rand() is called once and 0 is below every nonzero threshold")
	var refused: Dictionary = EndingOverlay.select_graduation_candidate(sel_girls, points, flags)
	_check(not String(refused.rand_error).is_empty() and refused.current_girl == EndingOverlay.GIRL_YOSHIO, "without rand() the draw is refused, not taken as 0")
	sel_girls[0].met = 1
	points[0] = 500
	calls[0] = 0
	var shiori: Dictionary = EndingOverlay.select_graduation_candidate(sel_girls, points, flags, rand)
	_check(calls[0] == 0 and shiori.current_girl == 0, "a qualifying heroine skips the Miharu draw")

	_dispatcher_checks()

	if failures.is_empty():
		print("PASS: ending candidate gate %d checks" % checks)
		quit(0)
	else:
		for message: String in failures:
			push_error(message)
		print("FAIL: ending candidate gate %d of %d checks" % [failures.size(), checks])
		quit(1)


# The production entry: overlay_dispatcher.init_overlay and step_overlay for
# mode 0x5C, with the rule-state shape the route holds (girls[i].flags words).
func _dispatcher_checks() -> void:
	var od: OverlayDispatcher = OverlayDispatcher.new()
	var girls: Array = []
	for i in 16:
		girls.append({"flags": 0})
	girls[EndingOverlay.GIRL_MIHARU].flags = 1 << 9 # story_stage 1
	girls[14].flags = 3 << 9
	var flags: Array = []
	flags.resize(0x100)
	flags.fill(0)
	var rule_state: Dictionary = {"girls": girls, "scene_flags": flags}

	var refused: Dictionary = od.init_overlay(EndingOverlay.GAME_MODE_ENDING, rule_state)
	_check(not String(refused.get("error", "")).is_empty() and (refused.phases as Array).is_empty(),
		"init_overlay without rng refuses the Miharu draw instead of choosing an ending")
	var stepped: Dictionary = od.step_overlay(EndingOverlay.GAME_MODE_ENDING, refused, 0x20, rule_state)
	_check(stepped.action == "rejected" and not String(stepped.get("error", "")).is_empty() and not stepped.completed,
		"step_overlay fails closed on a refused sequence")
	var paused: Dictionary = od.step_overlay(EndingOverlay.GAME_MODE_ENDING, refused, 0, rule_state)
	_check(paused.action == "rejected", "a refused sequence does not wait for input")

	var rng: NativeRng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	var seq: Dictionary = od.init_overlay(EndingOverlay.GAME_MODE_ENDING, rule_state, [], rng)
	_check(String(seq.get("error", "")).is_empty() and rng.draw_count == 1 and rng.trace[0].caller == "ending_graduation_candidate_select",
		"init_overlay with the native rng draws once for the Miharu chance")
	_check(seq.get("system_flag_or") == 4 and seq.get("gate_next_mode_stage") == -1,
		"the gate's side effects are carried into the sequence")
	var expect_girl: int = EndingOverlay.GIRL_MIHARU if (int(rng.trace[0].result) % 256) < EndingOverlay.ENDING_MIHARU_CHANCE_BY_LEVEL[1] else EndingOverlay.GIRL_YOSHIO
	_check(seq.current_girl == expect_girl, "the drawn value decides Miharu against the level-1 threshold")

	flags[0x4D] = 1 # no draw: Yoshio candidate, gate reads girls[14]
	girls[14].flags = 2 << 9
	var low: Dictionary = od.init_overlay(EndingOverlay.GAME_MODE_ENDING, rule_state)
	_check(String(low.get("error", "")).is_empty() and low.system_flag_or == 8 and low.gate_next_mode_stage == 7,
		"Yoshio with girls[14] bits 9..11 below 3: stage 7 and |= 8 are carried")
	girls[14].flags = 3 << 9
	var high: Dictionary = od.init_overlay(EndingOverlay.GAME_MODE_ENDING, rule_state)
	_check(high.system_flag_or == 4 and high.gate_next_mode_stage == -1, "otherwise mode_step_next and |= 4 are carried")
	girls.resize(14)
	var no_record: Dictionary = od.init_overlay(EndingOverlay.GAME_MODE_ENDING, rule_state)
	_check(not String(no_record.get("error", "")).is_empty() and (no_record.phases as Array).is_empty(),
		"a missing girls[14] record refuses the sequence instead of becoming the bad ending")


func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
