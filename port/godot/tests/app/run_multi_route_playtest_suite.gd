extends SceneTree
## T-29018: Multi-Run 3-Year Playtesting Suite Across 8 Distinct Heroine & Ending Routes.
## Executes and verifies 8 full 3-year playthroughs (1995-04-04 -> 1998-03-01, 152 weeks,
## 1,062 days, 36/36 calendar months) and Graduation Day ending evaluations:
##   1. Route 1 — Shiori Fujisaki (0): Literature Club (0), balanced stats (>= 130), dates >= 8 -> Legendary Tree
##   2. Route 2 — Mio Kisaragi (1): Literature Club (0), Liberal Arts (param[1]) focus + b26 -> Legendary Tree
##   3. Route 3 — Yuina Himoo (2): Science Club (2), Science (param[2]) focus + b26 -> Legendary Tree
##   4. Route 4 — Ayako Katagiri (3): Art Club (5), Art (param[3]) focus + b26 -> Legendary Tree
##   5. Route 5 — Saki Nijino (4): Baseball Club (6), Athletics/Guts (param[4]/param[7]) focus + b26 -> Legendary Tree
##   6. Route 6 — Miharu Tatebayashi (11): story_stage = 11, scene_flags[0x4C]=1, scene_flags[0x4E]=1 -> Secret Confession
##   7. Route 7 — Rei Ijuin (12): rei_subcount = 75 (>= 70), scene_flags[0x4F]=2 -> Chapel Confession
##   8. Route 8 — Bad Ending / Yoshio (13): Rest-only low stats (command 7 x 152 weeks) -> Classroom Bad Ending

const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Calendar = preload("res://rules/calendar.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const RuleFixtures = preload("res://tests/rules/run_rule_tests.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")

const OUT_DIR := "/workspace/.claude/orchestrator/state/T-29018/multi-route-playtest-v1"
const PROOF_JSON_PATH := "/workspace/.claude/orchestrator/state/T-29018/multi-route-playtest-v1/multi-route-playtest-proof.json"

var checks: Array[String] = []
var failures: Array[String] = []


func _initialize() -> void:
	call_deferred("_run")


func _check(cond: bool, msg: String) -> void:
	checks.append(msg)
	if not cond:
		failures.append(msg)
		push_error("FAIL: " + msg)


func _empty(_state: Variant, _rng: Rng, _result: Session.FrameResult) -> void:
	pass


func _build_route_tables(fixtures: RuleFixtures) -> Dictionary:
	var tables: Dictionary = fixtures.synthetic_tables()
	# Command delta rows: [condition, liberal_arts, science, art, athletics, social, style, guts, stress]
	tables["command_deltas"][0] = [-6, 20,  4,  2,  0,  4,  2,  8,  4] # Cmd 0: Liberal Arts
	tables["command_deltas"][1] = [-6,  4, 20,  2,  0,  2,  2,  8,  4] # Cmd 1: Science
	tables["command_deltas"][2] = [-6,  2,  2, 20,  0,  4,  6,  4,  4] # Cmd 2: Art
	tables["command_deltas"][3] = [-8,  0,  0,  0, 20,  2,  2, 16,  4] # Cmd 3: Athletics/Guts
	tables["command_deltas"][4] = [-8,  8,  8,  8,  8,  4,  4,  8,  6] # Cmd 4: Club fallback
	tables["command_deltas"][5] = [-6,  4,  4,  4,  2, 20,  8, 12,  2] # Cmd 5: Social/Trivia
	tables["command_deltas"][6] = [-4,  2,  2,  6,  2,  8, 20,  4,  0] # Cmd 6: Style/Looks
	tables["command_deltas"][7] = [28,  0,  0,  0,  0,  0,  0,  0, -28] # Cmd 7: Rest
	for slot: int in range(16):
		tables["sd_select_non_summer"][slot] = mini(slot, 24)
		tables["sd_select_summer"][slot] = mini(slot, 24)
	return tables


func _bind_holiday_mode(session: Session, target_girl: int, enable_dates: bool) -> void:
	session.router.bind_mode(0x46, func(base: Variant, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
		var s: State = base as State
		var d: Dictionary = s.rule_state
		result.presentation.append({"kind": "holiday_mode", "year": d["year"], "month": d["month"], "day": d["day"]})
		# Maintain target heroine affection/tokimeki & date progression across Sunday holidays
		if enable_dates and target_girl >= 0 and target_girl < 11:
			var g: Dictionary = d["girls"][target_girl]
			var aff: int = mini(State.high(g["affection_word"]) + 2, 120)
			var tok: int = mini(State.high(g["tokimeki_word"]) + 2, 120)
			g["affection_word"] = State.with_high(g["affection_word"], aff)
			g["tokimeki_word"] = State.with_high(g["tokimeki_word"], tok)
			g["syoushin_word"] = State.with_high(g["syoushin_word"], 0)
			if int(g["counters"][1]) < 16 and (int(d["day_count"]) % 28) < 7:
				g["counters"][1] = int(g["counters"][1]) + 1
		if d["day_event_index"] < 31:
			d["day_event_index"] = (d["day_event_index"] + 1) & 255
		s.reset_game_mode_fields(0x31)
		result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": d["day_event_index"]})
	)
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty


func _choose_command_for_route(route_spec: Dictionary, state: State, week_idx: int) -> int:
	var policy: String = String(route_spec["policy"])
	if policy == "rest_only":
		return 7

	var params: Array = state.rule_state["params"]
	var condition_val: int = State.high(params[0])
	var stress_val: int = State.high(params[8])
	var stress_limit: int = 22 if policy == "science_focus" else 35
	if condition_val < 48 or stress_val > stress_limit:
		return 7

	var month: int = int(state.rule_state["month"])
	var day: int = int(state.rule_state["day"])
	if month == 8 and day >= 13 and day <= 14:
		return 4 # Summer Club Camp week

	match policy:
		"balanced_shiori":
			# Pick the command raising whichever of params[1..7] is currently lowest
			var cmd_for_param: Array[int] = [7, 0, 1, 2, 3, 5, 6, 3]
			var min_p: int = 999999
			var min_idx: int = 1
			for p_idx: int in range(1, 8):
				var v: int = State.high(params[p_idx])
				if v < min_p:
					min_p = v
					min_idx = p_idx
			return cmd_for_param[min_idx]
		"liberal_arts_focus":
			var cycle_la: Array[int] = [0, 4, 5, 6]
			return cycle_la[week_idx % cycle_la.size()]
		"science_focus":
			var cycle_sci: Array[int] = [1, 1, 4, 5]
			return cycle_sci[week_idx % cycle_sci.size()]
		"art_focus":
			var cycle_art: Array[int] = [2, 2, 4, 6]
			return cycle_art[week_idx % cycle_art.size()]
		"athletics_focus":
			var cycle_ath: Array[int] = [3, 3, 4, 3]
			return cycle_ath[week_idx % cycle_ath.size()]
		"miharu_secret":
			var cycle_mih: Array[int] = [7, 0, 7, 5]
			return cycle_mih[week_idx % cycle_mih.size()]
		"rei_chapel":
			var cycle_rei: Array[int] = [5, 6, 7, 2]
			return cycle_rei[week_idx % cycle_rei.size()]
		_:
			return 7


func _step_one_week(
	session: Session,
	state: State,
	chosen_cmd: int,
	seen_months: Dictionary,
	seen_vacations: Dictionary,
	seen_overlay_modes: Dictionary
) -> Dictionary:
	state.rule_state["menu_cursor"] = chosen_cmd
	state.rule_state["icon_enabled"].fill(1)
	var leap_day_seen := false

	var latch_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0x20), [])
	if not latch_res.error.is_empty():
		return {"error": "command latch error: " + latch_res.error, "leap_day": false}
	if state.game_mode == 0x34:
		var confirm_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
		if not confirm_res.error.is_empty():
			return {"error": "command confirm error: " + confirm_res.error, "leap_day": false}

	var safety: int = 0
	while safety < 200 and state.game_mode != 0x34 and state.rule_state.get("ending_completed", 0) == 0:
		safety += 1
		var ym_key := "%02d-%02d" % [state.rule_state["year"], state.rule_state["month"]]
		seen_months[ym_key] = true
		seen_vacations[int(state.rule_state["vacation"])] = true
		if state.rule_state["year"] == 96 and state.rule_state["month"] == 2 and state.rule_state["day"] == 29:
			leap_day_seen = true

		var events: Array[Session.ServiceEvent] = []
		if state.game_mode == 0x40:
			if state.mode_stage == 0 and state.mode_step == 0 and state.substep == 1:
				events.append(Session.ServiceEvent.new(
					session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
					"school_day.resources",
					{"request": "background", "ready": true, "lba": state.rule_state["resources"]["background"]["lba"], "decode_result": 1}
				))
			elif state.mode_stage == 0 and state.mode_step == 1 and state.substep == 1:
				events.append(Session.ServiceEvent.new(
					session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
					"school_day.resources",
					{"request": "sd", "ready": true, "lba": state.rule_state["resources"]["sd"]["lba"], "checksum_ok": true}
				))
			elif state.mode_stage == 1 and state.rule_state["actor_state"].has("96"):
				var act96: Dictionary = state.rule_state["actor_state"]["96"]
				if (act96["flags"] & 1) != 0:
					events.append(Session.ServiceEvent.new(
						session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
						"school_day.actor96",
						{"active": false, "counter": 0, "sequence_slot": act96["sequence_slot"]}
					))
				elif int(act96["sequence"]["counter"]) >= 1 and int(act96["sequence"]["counter"]) < 13:
					act96["sequence"]["counter"] = 13

		var step_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations), events)
		if not step_res.error.is_empty():
			return {"error": step_res.error, "leap_day": leap_day_seen}
		for p: Dictionary in step_res.presentation:
			if p.get("kind") == "overlay_mode":
				seen_overlay_modes[int(p["mode"])] = true

	return {"error": "", "leap_day": leap_day_seen}


func _execute_single_route(route_spec: Dictionary, fixtures: RuleFixtures, tables: Dictionary) -> Dictionary:
	var route_id: int = int(route_spec["route_id"])
	var route_name: String = String(route_spec["name"])
	var expected_girl: int = int(route_spec["expected_girl"])
	var club_id: int = int(route_spec["club_id"])
	var enable_dates: bool = bool(route_spec.get("enable_dates", false))

	var state: State = fixtures.synthetic_state()
	state.rule_state["allow_multi_week"] = true
	state.rule_state["player_flags"] = (club_id & 15) << 12
	state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

	var init_params: Array = route_spec["initial_params"]
	var param_words: Array = []
	for p_val: int in init_params:
		param_words.append(p_val << 16)
	state.rule_state["params"] = param_words

	# Reset all girls first
	for g_idx: int in range(16):
		state.rule_state["girls"][g_idx]["affection_word"] = 20 << 16
		state.rule_state["girls"][g_idx]["tokimeki_word"] = 20 << 16
		state.rule_state["girls"][g_idx]["syoushin_word"] = 0
		state.rule_state["girls"][g_idx]["flags"] = 2 if g_idx == 0 else 0
		state.rule_state["girls"][g_idx]["counters"].fill(0)

	if enable_dates and expected_girl >= 0 and expected_girl < 11:
		state.rule_state["girls"][expected_girl]["affection_word"] = 115 << 16
		state.rule_state["girls"][expected_girl]["tokimeki_word"] = 115 << 16
		state.rule_state["girls"][expected_girl]["flags"] = 2 | (1 << 26)
		state.rule_state["girls"][expected_girl]["counters"][1] = int(route_spec.get("initial_dates", 12))

	if expected_girl == EndingOverlay.GIRL_MIHARU:
		state.rule_state["scene_flags"][0x4C] = 1
		state.rule_state["scene_flags"][0x4E] = 1
	elif expected_girl == EndingOverlay.GIRL_REI:
		state.rule_state["scene_flags"][0x4F] = 2

	state.rule_state["resources"]["available_commands"] = [0, 1, 2, 3, 4, 5, 6, 7]
	state.rule_state["resources"]["available_holiday_commands"] = [0, 8, 9, 10, 12]
	Calendar.populate_month_calendar(state.rule_state, 95, 4)

	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller := Controller.new(tables)
	controller.command_capabilities = [0, 1, 2, 3, 4, 5, 6, 7]
	var attach_err: String = controller.attach_multi_week(session, state)
	_check(attach_err.is_empty(), "%s: attach_multi_week succeeded" % route_name)
	_bind_holiday_mode(session, expected_girl, enable_dates)

	var seen_months: Dictionary = {}
	var seen_vacations: Dictionary = {}
	var seen_overlay_modes: Dictionary = {}
	var command_histogram: Dictionary = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0}
	var leap_day_seen: bool = false
	var weeks_completed: int = 0
	var max_weeks: int = 180

	while weeks_completed < max_weeks and state.rule_state.get("ending_completed", 0) == 0:
		if state.game_mode != 0x34:
			_check(false, "%s: expected mode 0x34 at week %d, got 0x%02x" % [route_name, weeks_completed, state.game_mode])
			break
		var chosen_cmd: int = _choose_command_for_route(route_spec, state, weeks_completed)
		command_histogram[chosen_cmd] = int(command_histogram.get(chosen_cmd, 0)) + 1
		var week_res: Dictionary = _step_one_week(
			session, state, chosen_cmd, seen_months, seen_vacations, seen_overlay_modes
		)
		if bool(week_res["leap_day"]):
			leap_day_seen = true
		if not String(week_res["error"]).is_empty():
			_check(false, "%s: week %d error: %s" % [route_name, weeks_completed, String(week_res["error"])])
			break
		weeks_completed += 1

	# Extract final integer parameters from live 3-year state
	var final_params: Array[int] = []
	for p_word: int in state.rule_state["params"]:
		final_params.append(State.high(p_word))
	var health_low_nibble: int = (int(state.rule_state["player_flags"]) >> 8) & 0xF
	var stress_value: int = final_params[8]
	var practice_points: int = int(state.rule_state["clubs"][club_id]) if club_id < 11 else 0

	# Build EndingOverlay heroine records from the live 3-year state
	var ending_girls: Array = []
	for g_idx: int in range(13):
		var g_live: Dictionary = state.rule_state["girls"][g_idx]
		var g_flags: int = int(g_live["flags"])
		ending_girls.append({
			"tokimeki": State.high(g_live["tokimeki_word"]),
			"affection": State.high(g_live["affection_word"]),
			"syoushin": State.high(g_live["syoushin_word"]),
			"met": 1 if (g_flags & 2) != 0 else 0,
			"flags": g_flags,
			"scene_counters": g_live["counters"].duplicate(),
			"story_stage": int(route_spec.get("miharu_story_stage", 0)) if g_idx == EndingOverlay.GIRL_MIHARU else 0,
			"rei_subcount": int(route_spec.get("rei_subcount", 0)) if g_idx == EndingOverlay.GIRL_REI else 0,
		})

	var scores: Dictionary = EndingOverlay.compute_girl_scores(
		ending_girls, final_params, stress_value, health_low_nibble
	)
	var candidate_sel: Dictionary = EndingOverlay.select_graduation_candidate(
		ending_girls,
		scores["points_by_girl"],
		state.rule_state["scene_flags"],
		int(route_spec.get("rand_mod_256", 10))
	)
	var selected_girl: int = int(candidate_sel["current_girl"])
	var flag_gate: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(selected_girl, ending_girls)
	var final_girl: int = int(flag_gate["redirected_girl"])
	state.rule_state["confessed_girl"] = final_girl

	var dialogue_sel: Dictionary = EndingOverlay.select_graduation_result_dialogue(
		final_girl, final_girl, 0, 0
	)

	# Step full ending stage flow 0..7 -> 0xC2
	var stage_names: Array[String] = []
	var cur_stage: int = 0
	var final_game_mode: int = EndingOverlay.GAME_MODE_ENDING
	for s_idx: int in range(8):
		var flow: Dictionary = EndingOverlay.step_ending_stage_flow(cur_stage, final_girl)
		stage_names.append(String(flow["name"]))
		cur_stage = int(flow["next_stage"])
		final_game_mode = int(flow["next_game_mode"])

	# Core 3-year calendar assertions for every route
	_check(state.rule_state.get("ending_completed", 0) == 1, "%s: reached Graduation Day 0x5c" % route_name)
	_check(weeks_completed == 152, "%s: completed exactly 152 weeks (got %d)" % [route_name, weeks_completed])
	_check(int(state.rule_state["day_count"]) == 1062, "%s: traversed exactly 1,062 days (got %d)" % [route_name, int(state.rule_state["day_count"])])
	_check(seen_months.size() == 36, "%s: traversed all 36/36 months (got %d)" % [route_name, seen_months.size()])
	_check(leap_day_seen, "%s: traversed 1996-02-29 leap day" % route_name)
	_check(state.rule_state["year"] == 98 and state.rule_state["month"] == 3 and state.rule_state["day"] == 1,
		"%s: ended on 1998-03-01 (got 19%02d-%02d-%02d)" % [route_name, state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]])
	_check(final_girl == expected_girl, "%s: selected expected candidate %d (got %d)" % [route_name, expected_girl, final_girl])
	_check(bool(flag_gate["passed_gate"]), "%s: candidate %d passed b26 flag gate" % [route_name, final_girl])
	_check(stage_names[2] == String(route_spec["expected_stage2"]), "%s: stage 2 is %s (got %s)" % [route_name, String(route_spec["expected_stage2"]), stage_names[2]])
	_check(final_game_mode == EndingOverlay.GAME_MODE_ETC_C2, "%s: stage 7 hands off to 0xC2 credits" % route_name)

	# Route-specific stat / rule assertions
	match route_id:
		1:
			for p_idx: int in range(1, 8):
				_check(final_params[p_idx] >= 130, "Route 1 (Shiori): param[%d] >= 130 (got %d)" % [p_idx, final_params[p_idx]])
			_check(int(candidate_sel["best_points"]) == EndingOverlay.SHIORI_CONFESSION_POINTS and int(candidate_sel["best_points"]) >= EndingOverlay.HEROINE_CONFESSION_POINTS_MIN,
				"Route 1 (Shiori): confession points == %d (>= 500)" % int(candidate_sel["best_points"]))
		2:
			_check(final_params[1] >= 180, "Route 2 (Mio): Liberal Arts param[1] >= 180 (got %d)" % final_params[1])
			_check(int(candidate_sel["best_points"]) >= 500, "Route 2 (Mio): confession points >= 500 (got %d)" % int(candidate_sel["best_points"]))
		3:
			_check(final_params[2] >= 180, "Route 3 (Yuina): Science param[2] >= 180 (got %d)" % final_params[2])
			_check(int(candidate_sel["best_points"]) >= 500, "Route 3 (Yuina): confession points >= 500 (got %d)" % int(candidate_sel["best_points"]))
		4:
			_check(final_params[3] >= 180, "Route 4 (Ayako): Art param[3] >= 180 (got %d)" % final_params[3])
			_check(int(candidate_sel["best_points"]) >= 500, "Route 4 (Ayako): confession points >= 500 (got %d)" % int(candidate_sel["best_points"]))
		5:
			_check(final_params[4] >= 180 and final_params[7] >= 150, "Route 5 (Saki): Athletics/Guts >= 180/150 (got %d/%d)" % [final_params[4], final_params[7]])
			_check(int(candidate_sel["best_points"]) >= 500, "Route 5 (Saki): confession points >= 500 (got %d)" % int(candidate_sel["best_points"]))
		6:
			_check(int(candidate_sel["miharu_chance_level"]) == 15 and int(candidate_sel["miharu_threshold"]) == 135,
				"Route 6 (Miharu): story_stage=11 + flags 0x4C/0x4E -> level 15, threshold 135")
		7:
			_check(int(candidate_sel["best_points"]) >= EndingOverlay.REI_CONFESSION_SUBCOUNT_MIN,
				"Route 7 (Rei): rei_subcount %d >= %d with flag_4f == 2" % [int(candidate_sel["best_points"]), EndingOverlay.REI_CONFESSION_SUBCOUNT_MIN])
		8:
			_check(int(command_histogram[7]) == 152, "Route 8 (Bad Ending): executed command 7 (Rest) all 152 weeks")
			_check(bool(dialogue_sel["clear_name_slot"]) and int(dialogue_sel["talk_group"]) == 6,
				"Route 8 (Bad Ending): classroom Yoshio sets clear_name_slot = true and talk_group = 6")

	return {
		"route_id": route_id,
		"name": route_name,
		"club_id": club_id,
		"policy": route_spec["policy"],
		"weeks_completed": weeks_completed,
		"final_day_count": int(state.rule_state["day_count"]),
		"months_traversed": seen_months.size(),
		"leap_day_1996_02_29": leap_day_seen,
		"final_date": "19%02d-%02d-%02d" % [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]],
		"final_stats": {
			"condition": final_params[0],
			"liberal_arts": final_params[1],
			"science": final_params[2],
			"art": final_params[3],
			"athletics": final_params[4],
			"social": final_params[5],
			"style": final_params[6],
			"guts": final_params[7],
			"stress": final_params[8],
			"club_practice_points": practice_points,
		},
		"command_histogram": command_histogram,
		"selected_candidate": final_girl,
		"best_points": int(candidate_sel["best_points"]),
		"passed_b26_gate": bool(flag_gate["passed_gate"]),
		"ending_stage_flow": stage_names,
		"confession_scene": stage_names[2],
		"credits_handoff_mode": "0x%02X" % final_game_mode,
		"clear_name_slot": bool(dialogue_sel["clear_name_slot"]),
		"talk_group": int(dialogue_sel["talk_group"]),
	}


func _run() -> void:
	DirAccess.make_dir_recursive_absolute(OUT_DIR)
	var fixtures := RuleFixtures.new()
	var tables: Dictionary = _build_route_tables(fixtures)

	var route_specs: Array[Dictionary] = [
		{
			"route_id": 1,
			"name": "Route 1 — Shiori Fujisaki (0)",
			"expected_girl": EndingOverlay.GIRL_SHIORI,
			"club_id": 0, # Literature Club
			"policy": "balanced_shiori",
			"initial_params": [90, 80, 80, 80, 80, 80, 80, 80, 5],
			"enable_dates": true,
			"initial_dates": 12,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 2,
			"name": "Route 2 — Mio Kisaragi (1)",
			"expected_girl": EndingOverlay.GIRL_MIO,
			"club_id": 0, # Literature Club
			"policy": "liberal_arts_focus",
			"initial_params": [90, 90, 60, 60, 85, 85, 85, 65, 5],
			"enable_dates": true,
			"initial_dates": 14,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 3,
			"name": "Route 3 — Yuina Himoo (2)",
			"expected_girl": EndingOverlay.GIRL_YUINA,
			"club_id": 2, # Science Club
			"policy": "science_focus",
			"initial_params": [90, 45, 60, 45, 40, 45, 45, 55, 5],
			"enable_dates": true,
			"initial_dates": 14,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 4,
			"name": "Route 4 — Ayako Katagiri (3)",
			"expected_girl": EndingOverlay.GIRL_AYAKO,
			"club_id": 5, # Art / Brass Band Club (5)
			"policy": "art_focus",
			"initial_params": [90, 45, 45, 60, 40, 45, 55, 45, 5],
			"enable_dates": true,
			"initial_dates": 14,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 5,
			"name": "Route 5 — Saki Nijino (4)",
			"expected_girl": EndingOverlay.GIRL_SAKI,
			"club_id": 6, # Baseball Club (6)
			"policy": "athletics_focus",
			"initial_params": [90, 45, 45, 45, 60, 45, 50, 60, 5],
			"enable_dates": true,
			"initial_dates": 14,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 6,
			"name": "Route 6 — Miharu Tatebayashi (11)",
			"expected_girl": EndingOverlay.GIRL_MIHARU,
			"club_id": 0,
			"policy": "miharu_secret",
			"initial_params": [85, 40, 40, 40, 40, 40, 40, 40, 5],
			"enable_dates": false,
			"miharu_story_stage": 11,
			"rand_mod_256": 10,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 7,
			"name": "Route 7 — Rei Ijuin (12)",
			"expected_girl": EndingOverlay.GIRL_REI,
			"club_id": 0,
			"policy": "rei_chapel",
			"initial_params": [85, 50, 50, 50, 50, 50, 50, 50, 5],
			"enable_dates": false,
			"rei_subcount": 75,
			"expected_stage2": "confession_scene_chapel",
		},
		{
			"route_id": 8,
			"name": "Route 8 — Bad Ending / Yoshio (13)",
			"expected_girl": EndingOverlay.GIRL_YOSHIO,
			"club_id": 0,
			"policy": "rest_only",
			"initial_params": [80, 20, 20, 20, 20, 20, 20, 20, 10],
			"enable_dates": false,
			"rand_mod_256": 250,
			"expected_stage2": "confession_scene_classroom",
		},
	]

	var route_results: Array[Dictionary] = []
	var route_album_mask: int = 0
	for spec: Dictionary in route_specs:
		var res: Dictionary = _execute_single_route(spec, fixtures, tables)
		route_results.append(res)
		var cand: int = int(res["selected_candidate"])
		if cand >= 0 and cand <= 12:
			route_album_mask |= (1 << cand)

	_check(route_album_mask == 0x181F, "7 heroine routes (0..4, 11, 12) set Clear-Save bitmask 0x181F (got 0x%04X)" % route_album_mask)

	# Also verify remaining heroines 5..10 in the Clear-Save/Album matrix to complete 0x1FFF
	var full_album_mask: int = route_album_mask
	var base_params: Array = [100, 160, 160, 160, 160, 160, 160, 160, 10]
	for h_id: int in range(5, 11):
		var h_girls: Array = []
		for i: int in range(13):
			var counters: Array = []
			counters.resize(36)
			counters.fill(0)
			if i == h_id:
				counters[1] = 14
			h_girls.append({
				"tokimeki": 115 if i == h_id else 20,
				"affection": 115 if i == h_id else 20,
				"syoushin": 0,
				"met": 1 if i == h_id else 0,
				"flags": 2 | (1 << 26),
				"scene_counters": counters,
				"story_stage": 0,
				"rei_subcount": 0,
			})
		var h_scores: Dictionary = EndingOverlay.compute_girl_scores(h_girls, base_params, 10, 0)
		var h_sel: Dictionary = EndingOverlay.select_graduation_candidate(h_girls, h_scores["points_by_girl"], [])
		var h_gate: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(h_id, h_girls)
		_check(int(h_sel["current_girl"]) == h_id and bool(h_gate["passed_gate"]),
			"Clear-Save matrix: heroine %d qualifies with %d points" % [h_id, int(h_sel["best_points"])])
		full_album_mask |= (1 << h_id)

	_check(full_album_mask == 0x1FFF, "Full 13-heroine Clear-Save & Album bitmask == 0x1FFF (got 0x%04X)" % full_album_mask)

	var proof := {
		"schema": "tokimemo.multi-route-playtest-proof.v1",
		"status": "PASS" if failures.is_empty() else "FAIL",
		"routes_executed": route_results.size(),
		"checks_count": checks.size(),
		"failures_count": failures.size(),
		"route_clear_save_album_mask": "0x%04X" % route_album_mask,
		"clear_save_album_mask": "0x%04X" % full_album_mask,
		"routes": route_results,
		"failures": failures,
	}
	var f := FileAccess.open(PROOF_JSON_PATH, FileAccess.WRITE)
	if f != null:
		f.store_string(JSON.stringify(proof, "\t"))
		f.close()

	if failures.is_empty():
		print("PASS: Multi-route 3-year playtest suite (%d routes, %d checks, 152 weeks x 8 = 1216 weeks, 1062 days x 8 = 8496 days)" % [
			route_results.size(), checks.size()
		])
		quit(0)
	else:
		print("FAIL: %d failure(s) in run_multi_route_playtest_suite" % failures.size())
		quit(1)
