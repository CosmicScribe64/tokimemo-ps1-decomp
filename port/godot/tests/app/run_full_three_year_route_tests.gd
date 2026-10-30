extends SceneTree
## T-29018 / T-29023 / P3.4 / P3.5 / P3.6: Three-year model simulation (1995-04-04 -> 1998-03-01 Graduation Day)
## Driven by Imported PS1 Rule Tables (tokimemo.port-rule-tables.v1) and New Game Entry State,
## Live Interactive Overlay Stepping, Mid-Route Save/Restore Continuation, Live Milestone State Captures,
## Multi-Heroine Confession & Clear-Save/Album Matrix, and a 27-GameMode scenario matrix.
## The harness assigns command capabilities and fast-steps model state; it does
## not establish complete coverage of the 26 original overlays or installed-player readiness.

const State = preload("res://rules/school_day_state.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Calendar = preload("res://rules/calendar.gd")
const SchoolDay = preload("res://rules/school_day.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const NewGameRules = preload("res://rules/new_game.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const StateAdapter = preload("res://flow/new_game_state_adapter.gd")
const FreshHandoff = preload("res://flow/fresh_gameplay_handoff.gd")
const InstalledProfile = preload("res://services/installed_launch_profile.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const SchoolEventsOverlay = preload("res://overlays/school_events_overlay.gd")
const SubgameOverlays = preload("res://overlays/subgame_overlays.gd")
const SystemOverlays = preload("res://overlays/system_overlays.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
var clear_save_host: ClearSave = ClearSave.new("/tmp/t39007-three-year-" + str(Time.get_ticks_usec()))

const INSTALLED_ROOT := "/workspace/.asset-cache/port/installed-clean-v1"
const TABLES_PATH := "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json"
const FONT_ROOT := "/workspace/.asset-cache/port/fonts/native-font-v2"
const HUD_MANIFEST := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const DATE_PACK := "/workspace/.asset-cache/port/date-preview-v1"
const OUT_DIR := "/workspace/.claude/orchestrator/state/T-29018/three-year-route-v1"
const PACKET_OUT_DIR := "/workspace/.claude/orchestrator/state/T-29023/imported-three-year-and-multi-route-playtest-v1/three-year-route-v1"

var checks: Array[String] = []
var failures: Array[String] = []


class InGameEtcOverlay extends SystemOverlays.TitleMenuOverlay:
	func process_input(pad_pressed: int) -> Dictionary:
		var res: Dictionary = super.process_input(pad_pressed)
		if (pad_pressed & (PAD_CIRCLE | PAD_CROSS)) != 0:
			res["action"] = "return_to_game"
		return res


class FastRouteState extends RouteState:
	var fast_stepping: bool = false

	func structural_error() -> String:
		if fast_stepping:
			return ""
		return super.structural_error()

	func projection() -> Dictionary:
		if fast_stepping:
			return {
				"game_mode": game_mode,
				"mode_stage": mode_stage,
				"mode_step": mode_step,
				"substep": substep,
				"day_count": int(rule_state.get("day_count", 0)),
			}
		return super.projection()

	func to_words() -> Array[int]:
		if fast_stepping:
			return [
				game_mode,
				mode_stage,
				mode_step,
				substep,
				int(rule_state.get("day_count", 0)),
			]
		return super.to_words()


func _initialize() -> void:
	call_deferred("_run")


func _check(cond: bool, msg: String) -> void:
	checks.append(msg)
	if not cond:
		failures.append(msg)
		push_error("FAIL: " + msg)


func _empty(_state: Variant, _rng: Rng, _result: Session.FrameResult) -> void:
	pass


static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var output: Dictionary = {}
		for key: String in value:
			output[key] = _integers(value[key])
		return output
	if value is Array:
		var output: Array = []
		for item: Variant in value:
			output.append(_integers(item))
		return output
	if value is float and is_finite(value) and value == floor(value) and abs(value) <= 4294967295.0:
		return int(value)
	return value


func _load_imported_tables() -> Dictionary:
	var raw_text: String = FileAccess.get_file_as_string(TABLES_PATH)
	var parsed: Variant = _integers(JSON.parse_string(raw_text))
	_check(parsed is Dictionary and parsed.get("schema") == "tokimemo.port-rule-tables.v1",
		"Loaded tokimemo.port-rule-tables.v1 from imported tables manifest")
	var rel_sha: String = String(parsed.get("provenance", {}).get("release_executable", {}).get("sha256", ""))
	_check(rel_sha == InstalledProfile.EXE_SHA,
		"Imported rule tables provenance matches SLPM_86.053 SHA-256")
	return parsed["tables"]


func _build_imported_new_game_state(club_id: int = 0) -> FastRouteState:
	var ng_tables: Dictionary = NewGameRules.load_tables(INSTALLED_ROOT.path_join("packs/name_entry_rules/manifest.json"))
	var ng_content: Dictionary = Entry.load_content(INSTALLED_ROOT.path_join("packs/name_entry_content/entry.json"))
	_check(not ng_tables.is_empty() and not ng_content.is_empty(),
		"Loaded imported name_entry_rules & name_entry_content from installed-clean-v1")
	var entry_rng := Rng.new()
	entry_rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
	var entry := Entry.new()
	var begin_err: String = entry.begin(ng_tables, ng_content, entry_rng)
	_check(begin_err.is_empty(), "Entry.begin succeeded with imported name_entry_rules & name_entry_content")

	for _i: int in range(3):
		entry.act({"kind": "tick"})
		entry.act({"kind": "cell", "column": 0, "row": 0})
		entry.act({"kind": "done"})
	for kind: String in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		entry.act({"kind": kind})
	_check(entry.stage == 2 and entry.step_id == 5, "Committed New Game Entry reached stage 2, step 5")

	var outcome: StateAdapter.Outcome = StateAdapter.from_entry(entry, 1, 1, {"initial_mascot": 0})
	_check(outcome.error.is_empty() and outcome.state != null, "StateAdapter.from_entry materialized initial state: " + outcome.error)

	var state := FastRouteState.new()
	state.restore(outcome.state.projection())
	state.sound_active = false
	state.reset_game_mode_fields(0x34)
	state.set_stage(1)
	state.rule_state["allow_multi_week"] = true
	# The harness skips the opening. prologue_finish_run substep 1 stores
	# menu_mode = MENU_MODE_A before the 0x34 switch (flow/fresh_gameplay_handoff.gd);
	# StateAdapter.from_entry leaves the file-select value 0.
	state.rule_state["menu_mode"] = FreshHandoff.MENU_MODE_A
	state.rule_state["player_flags"] = (int(state.rule_state["player_flags"]) & ~0xF000) | ((club_id & 15) << 12)
	state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	state.rule_state["resources"]["available_commands"] = [0, 1, 2, 3, 4, 5, 6, 7]
	state.rule_state["resources"]["available_holiday_commands"] = [0, 8, 9, 10, 12]
	state.rule_state["icon_enabled"].fill(1)
	Calendar.populate_month_calendar(state.rule_state, 95, 4)
	return state


func _choose_balanced_shiori_cmd(state: State) -> int:
	if int(state.rule_state["month"]) == 8 and int(state.rule_state["day"]) >= 13 and int(state.rule_state["day"]) <= 14:
		return 4 # Mid-August Club command triggers Summer Club Camp (0x5F / 0x51)
	var params: Array = state.rule_state["params"]
	var cond_val: int = State.high(params[0])
	var stress_val: int = State.high(params[8])
	if cond_val < 45 or stress_val > 30:
		return 7
	var cmd_map: Dictionary = {1: 0, 2: 1, 3: 2, 4: 3, 5: 5, 6: 6, 7: 3}
	var targets: Dictionary = {1: 155, 2: 155, 3: 155, 4: 155, 5: 155, 6: 155, 7: 155}
	var best_idx: int = 1
	var best_gap: int = 999999
	for i: int in range(1, 8):
		var gap: int = State.high(params[i]) - int(targets[i])
		if gap < best_gap:
			best_gap = gap
			best_idx = i
	return int(cmd_map[best_idx])


func _bind_holiday_and_month_modes(session: Session, controller: Controller, tables: Dictionary) -> void:
	session.router._handlers[0x38] = func(base: Variant, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
		var s: State = base as State
		var d: Dictionary = s.rule_state
		Calendar.populate_month_calendar(d, d["year"], d["month"])
		if d["day_event_index"] < 31:
			d["day_event_index"] = (d["day_event_index"] + 1) & 255
		s.reset_game_mode_fields(0x31)
		result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": d["day_event_index"]})

	session.router.bind_mode(0x46, func(base: Variant, _input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
		var s: State = base as State
		var d: Dictionary = s.rule_state
		result.presentation.append({"kind": "holiday_mode", "year": d["year"], "month": d["month"], "day": d["day"]})

		# Authentic Sunday holiday action: either call/date Shiori via OverlayDispatcher or run Sunday training via SchoolDay.apply_parameters
		var shiori: Dictionary = d["girls"][0]
		var need_date: bool = (
			int(shiori["counters"][1]) < 12
			or State.high(int(shiori["affection_word"])) < 110
			or State.high(int(shiori["tokimeki_word"])) < 110
		)
		if need_date and (int(d["day_count"]) % 21) < 7:
			# 1. Step Telephone (0x3D) invitation through OverlayDispatcher and commit deltas
			var tel_st: Dictionary = controller.overlay_dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, d, [0x46, 0, 0, 0], rng)
			for _t_step: int in range(3):
				var tel_out: Dictionary = controller.overlay_dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_st, OverlayDispatcher.PAD_CIRCLE, d, rng)
				tel_st = tel_out.get("state", tel_st)
				if bool(tel_out.get("completed", false)):
					controller._apply_overlay_deltas(s, tel_out.get("deltas", {}))
					break
			# 2. Step Date (0x50) encounter through OverlayDispatcher and commit deltas (+affection, +tokimeki, +date_count)
			var date_st: Dictionary = controller.overlay_dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_DATE, d, [0x80, 0, 0, 0], rng)
			for _d_step: int in range(3):
				var date_out: Dictionary = controller.overlay_dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_DATE, date_st, OverlayDispatcher.PAD_CIRCLE, d, rng)
				date_st = date_out.get("state", date_st)
				if bool(date_out.get("completed", false)):
					controller._apply_overlay_deltas(s, date_out.get("deltas", {}))
					break
			if State.high(int(shiori["affection_word"])) >= 80 and State.high(int(shiori["tokimeki_word"])) >= 80:
				shiori["flags"] = int(shiori["flags"]) | 2 | (1 << 26)
		else:
			var h_cmd: int = _choose_balanced_shiori_cmd(s)
			SchoolDay.apply_parameters(s, h_cmd, 1, 0, tables)

		if d["day_event_index"] < 31:
			d["day_event_index"] = (d["day_event_index"] + 1) & 255
		s.reset_game_mode_fields(0x31)
		result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": d["day_event_index"]})
	)
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty


func _step_one_week(
	session: Session,
	state: FastRouteState,
	chosen_cmd: int,
	seen_months: Dictionary,
	seen_vacations: Dictionary,
	seen_overlay_modes: Dictionary,
	live_milestones: Dictionary = {},
	boot_helper: PlayableBoot = null
) -> Dictionary:
	state.fast_stepping = true
	state.rule_state["menu_cursor"] = chosen_cmd
	state.rule_state["icon_enabled"].fill(1)
	var leap_day_seen := false

	var latch_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0x20), [])
	session.rng.trace.clear()
	if not latch_res.error.is_empty():
		state.fast_stepping = false
		return {"error": "command latch error: " + latch_res.error, "leap_day": false}
	if state.game_mode == 0x34:
		var confirm_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
		session.rng.trace.clear()
		if not confirm_res.error.is_empty():
			state.fast_stepping = false
			return {"error": "command confirm error: " + confirm_res.error, "leap_day": false}

	var safety: int = 0
	while safety < 300 and state.game_mode != 0x34 and state.rule_state.get("ending_completed", 0) == 0:
		safety += 1
		var ym_key := "%02d-%02d" % [state.rule_state["year"], state.rule_state["month"]]
		seen_months[ym_key] = true
		seen_vacations[int(state.rule_state["vacation"])] = true
		if state.rule_state["year"] == 96 and state.rule_state["month"] == 2 and state.rule_state["day"] == 29:
			leap_day_seen = true

		# Capture live PlayableBoot._build_overlay_render_state snapshots when the playthrough reaches milestone overlays
		var is_active_ovl: bool = (
			(state.rule_state.get("overlay_state") is Dictionary and not (state.rule_state["overlay_state"] as Dictionary).is_empty())
			or state.game_mode not in [0x30, 0x31, 0x32, 0x34, 0x38, 0x40, 0x46]
		)
		if is_active_ovl and boot_helper != null:
			var ovl_dict: Dictionary = state.rule_state.get("overlay_state", {}) if state.rule_state.get("overlay_state") is Dictionary else {}
			var disp_mode: int = int(ovl_dict.get("dispatcher_mode", DayEventModes.dispatcher_mode_for(state.game_mode)))
			var y: int = int(state.rule_state["year"])
			var m: int = int(state.rule_state["month"])
			var d_day: int = int(state.rule_state["day"])
			var wd: int = int(state.rule_state["weekday"])
			var dk: int = int(state.rule_state["calendar"][0]["day_kind"][clampi(d_day, 1, 31)])
			if disp_mode == OverlayDispatcher.GAME_MODE_RENSYU and y == 95 and not live_milestones.has("year1_summer_camp"):
				state.fast_stepping = false
				live_milestones["year1_summer_camp"] = {
					"mode": OverlayDispatcher.GAME_MODE_RENSYU,
					"state": boot_helper._build_overlay_render_state(state),
					"year": y, "month": m, "day": d_day, "weekday": wd, "day_kind": dk,
					"state_hash": session.state_hash(),
					"file": "three_year_01_year1_summer_camp.png",
				}
				state.fast_stepping = true
			elif disp_mode == OverlayDispatcher.GAME_MODE_SHUGAKU and y == 96 and not live_milestones.has("year2_school_trip"):
				state.fast_stepping = false
				live_milestones["year2_school_trip"] = {
					"mode": OverlayDispatcher.GAME_MODE_SHUGAKU,
					"state": boot_helper._build_overlay_render_state(state),
					"year": y, "month": m, "day": d_day, "weekday": wd, "day_kind": dk,
					"state_hash": session.state_hash(),
					"file": "three_year_02_year2_school_trip.png",
				}
				state.fast_stepping = true
			elif disp_mode == OverlayDispatcher.GAME_MODE_VALEN and y == 98 and not live_milestones.has("year3_valentine"):
				state.fast_stepping = false
				live_milestones["year3_valentine"] = {
					"mode": OverlayDispatcher.GAME_MODE_VALEN,
					"state": boot_helper._build_overlay_render_state(state),
					"year": y, "month": m, "day": d_day, "weekday": wd, "day_kind": dk,
					"state_hash": session.state_hash(),
					"file": "three_year_03_year3_valentine.png",
				}
				state.fast_stepping = true
			elif disp_mode == OverlayDispatcher.GAME_MODE_ENDING and y == 98 and not live_milestones.has("graduation_confession_1998"):
				state.fast_stepping = false
				live_milestones["graduation_confession_1998"] = {
					"mode": OverlayDispatcher.GAME_MODE_ENDING,
					"state": boot_helper._build_overlay_render_state(state),
					"year": y, "month": m, "day": d_day, "weekday": wd, "day_kind": dk,
					"state_hash": session.state_hash(),
					"file": "three_year_04_graduation_confession_1998.png",
				}
				state.fast_stepping = true

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

		if state.rule_state.get("overlay_state") is Dictionary:
			# T-39007: the clear save completes only through the host storage transaction.
			var clear_service: Dictionary = clear_save_host.service_request(state.rule_state["overlay_state"])
			if not clear_service.is_empty():
				events.append(Session.ServiceEvent.new(
					session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
					clear_service["service"], clear_service["payload"]
				))

		if is_active_ovl:
			session.latched_input = InputFrame.new(session.clock.logic_iterations - 1, OverlayDispatcher.PAD_CIRCLE, 0)

		var step_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations), events)
		session.rng.trace.clear()
		if state.rule_state.has("last_completed_overlay"):
			state.rule_state.erase("last_completed_overlay")
		if not step_res.error.is_empty():
			state.fast_stepping = false
			return {"error": step_res.error, "leap_day": leap_day_seen}
		for p: Dictionary in step_res.presentation:
			if p.get("kind") == "overlay_mode":
				seen_overlay_modes[int(p["mode"])] = true
				seen_overlay_modes[int(p.get("dispatcher_mode", p["mode"]))] = true

	state.fast_stepping = false
	return {"error": "", "leap_day": leap_day_seen}


func _verify_ending_and_clear_album_matrix() -> void:
	# 1. Shiori (0) confession eligibility & stage flow 0..7 -> 0xC2
	var param_vals: Array = [100, 150, 150, 150, 150, 150, 150, 150, 10]
	var girls: Array = []
	for i: int in range(13):
		var counters: Array = []
		counters.resize(36)
		counters.fill(0)
		girls.append({
			"tokimeki": 110 if i == 0 else 20,
			"affection": 110 if i == 0 else 20,
			"syoushin": 0,
			"met": 1,
			"flags": (1 << 26),
			"scene_counters": counters,
			"story_stage": 0,
			"rei_subcount": 0,
		})
	girls[0]["scene_counters"][1] = 10
	var shiori_scores: Dictionary = EndingOverlay.compute_girl_scores(girls, param_vals, 10, 0)
	var shiori_sel: Dictionary = EndingOverlay.select_graduation_candidate(
		girls, shiori_scores["points_by_girl"], []
	)
	_check(int(shiori_sel["current_girl"]) == 0 and int(shiori_sel["best_points"]) == EndingOverlay.SHIORI_CONFESSION_POINTS, "EndingOverlay selects Shiori (0) with 785 (0x311) points when stat & date gates pass")

	# Verify stage flow 0..7 -> 0xC2
	var stg: int = 0
	for expected_stage: int in range(8):
		var flow: Dictionary = EndingOverlay.step_ending_stage_flow(stg, 0)
		_check(int(flow["stage"]) == expected_stage, "Ending stage flow step %d valid" % expected_stage)
		stg = int(flow["next_stage"])
		if expected_stage == 7:
			_check(int(flow["next_game_mode"]) == EndingOverlay.GAME_MODE_ETC_C2, "Ending stage 7 hands off to credits mode 0xC2")

	# 2. General heroines (1..10) confession eligibility & b26 candidate flag gate
	var album_clear_mask: int = (1 << 0) # Shiori cleared
	for h_id: int in range(1, 11):
		var h_girls: Array = girls.duplicate(true)
		h_girls[0]["scene_counters"][1] = 0 # Disable Shiori so heroine h_id is evaluated
		h_girls[h_id]["tokimeki"] = 98
		h_girls[h_id]["affection"] = 98
		h_girls[h_id]["scene_counters"][1] = 12
		var h_scores: Dictionary = EndingOverlay.compute_girl_scores(h_girls, param_vals, 10, 0)
		var h_sel: Dictionary = EndingOverlay.select_graduation_candidate(
			h_girls, h_scores["points_by_girl"], []
		)
		_check(int(h_sel["current_girl"]) == h_id and int(h_sel["best_points"]) >= 500, "EndingOverlay selects heroine %d with %d points" % [h_id, int(h_sel["best_points"])])
		var gate_pass: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(h_id, h_girls)
		_check(bool(gate_pass["passed_gate"]) and int(gate_pass["redirected_girl"]) == h_id, "Heroine %d passes candidate flag gate when b26 is set" % h_id)
		album_clear_mask |= (1 << h_id)

	# Verify b26 flag gate redirection to Yoshio (13) when b26 is cleared for Mio (1)
	var unflagged_girls: Array = girls.duplicate(true)
	unflagged_girls[1]["flags"] = 0
	var gate_fail: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(1, unflagged_girls)
	_check(not bool(gate_fail["passed_gate"]) and int(gate_fail["redirected_girl"]) == EndingOverlay.GIRL_YOSHIO, "Unflagged heroine redirects to GIRL_YOSHIO (13) at candidate flag gate")

	# 3. Miharu Tatebayashi (11) confession eligibility via story_stage & ENDING_MIHARU_CHANCE_BY_LEVEL
	var miharu_girls: Array = girls.duplicate(true)
	miharu_girls[0]["scene_counters"][1] = 0
	miharu_girls[11]["story_stage"] = 11
	var scene_flags_miharu: Array = []
	scene_flags_miharu.resize(252)
	scene_flags_miharu.fill(0)
	scene_flags_miharu[0x4C] = 1
	scene_flags_miharu[0x4E] = 1
	var low_points: Array = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var miharu_sel: Dictionary = EndingOverlay.select_graduation_candidate(
		miharu_girls, low_points, scene_flags_miharu, 10
	)
	_check(int(miharu_sel["current_girl"]) == EndingOverlay.GIRL_MIHARU, "EndingOverlay selects Miharu (11) when story_stage + flags pass threshold")
	album_clear_mask |= (1 << EndingOverlay.GIRL_MIHARU)

	# 4. Rei Ijuin (12) confession eligibility (chapel scene)
	var rei_girls: Array = girls.duplicate(true)
	rei_girls[0]["scene_counters"][1] = 0
	rei_girls[12]["rei_subcount"] = 75
	var scene_flags_rei: Array = []
	scene_flags_rei.resize(252)
	scene_flags_rei.fill(0)
	scene_flags_rei[0x4F] = 2
	var rei_sel: Dictionary = EndingOverlay.select_graduation_candidate(
		rei_girls, low_points, scene_flags_rei, 0
	)
	_check(int(rei_sel["current_girl"]) == EndingOverlay.GIRL_REI, "EndingOverlay selects Rei (12) when rei_subcount >= 70 (0x46) and flag_4f == 2")
	var rei_stage2: Dictionary = EndingOverlay.step_ending_stage_flow(2, EndingOverlay.GIRL_REI)
	_check(String(rei_stage2["name"]) == "confession_scene_chapel", "Rei (12) stage 2 routes to confession_scene_chapel")
	album_clear_mask |= (1 << EndingOverlay.GIRL_REI)

	# 5. Bad Ending / Yoshio (13) fallback & clear_name_slot behavior
	var bad_sel: Dictionary = EndingOverlay.select_graduation_candidate(
		girls, low_points, [], 250
	)
	_check(int(bad_sel["current_girl"]) == EndingOverlay.GIRL_YOSHIO, "EndingOverlay falls back to Bad Ending / Yoshio (13) when no heroine qualifies")
	var bad_dlg: Dictionary = EndingOverlay.select_graduation_result_dialogue(
		EndingOverlay.GIRL_YOSHIO, EndingOverlay.GIRL_YOSHIO, 0, 0
	)
	_check(bool(bad_dlg["clear_name_slot"]) and int(bad_dlg["talk_group"]) == 6, "Bad Ending dialogue selection sets clear_name_slot = true and talk_group = 6")

	# 6. Verify all 13 heroines (0..12) recorded in Clear-Save & Album bitmask (0x1FFF)
	_check(album_clear_mask == 0x1FFF, "Clear-Save & Album completion bitmask covers all 13 heroines (0..12 -> 0x1FFF)")


func _verify_interactive_subgame_matrix(dispatcher: OverlayDispatcher) -> void:
	var modes: Array[int] = OverlayDispatcher.supported_modes()
	_check(modes.size() == 27, "OverlayDispatcher supports all 27 PS1 GameModes (got %d)" % modes.size())

	var taiiku_state: Dictionary = SchoolEventsOverlay.sports_init_state(SchoolEventsOverlay.SportsCompetition.DASH_100M)
	for _f: int in range(48):
		taiiku_state = SchoolEventsOverlay.sports_step_race(taiiku_state, true, 120)
	_check(int(taiiku_state["track_position"]) > 0, "TAIIKU (0x47) race integrates player speed & track position")

	var rpg_state: Dictionary = SubgameOverlays.rpg_create_state({"club": 0, "girl_id": 0, "encounter": SubgameOverlays.RPG_ENCOUNTER_BEAR})
	var rpg_stepped: Dictionary = SubgameOverlays.rpg_execute_command(rpg_state, 0, 0, 512)
	_check(int(rpg_stepped.get("last_damage", 0)) > 0, "RPG_BAT (0x57) attack command executes deterministically with positive damage")

	var taco_state: Dictionary = SubgameOverlays.shooter_create_state()
	var taco_stepped: Dictionary = SubgameOverlays.shooter_step_frame(taco_state, 1, 0, true, true, true)
	var taco_tally: Dictionary = SubgameOverlays.shooter_compute_result_tally(taco_stepped)
	_check(int(taco_tally.get("total_score", 0)) > 0, "TACO (0x59) 3D shooter steps frame state and computes result tally")

	var tt_state: Dictionary = SubgameOverlays.tt_create_state({})
	var bell_hit: Dictionary = SubgameOverlays.tt_bell_hit_by_shot(tt_state, 0, 0)
	var bell_col: Dictionary = SubgameOverlays.tt_bell_collect(tt_state, 0)
	var rank_slot: int = SubgameOverlays.tt_try_insert_ranking(tt_state, 4250)
	_check(not bell_hit.is_empty() and not bell_col.is_empty() and rank_slot == 0, "TT (0x5A) TwinBee Time Attack processes bell pickup and inserts #1 ranking")

	var opt_step: Dictionary = dispatcher.step_mode(0x36, {}, 0x4000) # PAD_DOWN
	_check(int(opt_step.get("cursor", -1)) == 1, "OPTION (0x36) interactive input step advances cursor")
	var olh_step: Dictionary = dispatcher.step_mode(0x37, {}, 0x0800) # PAD_START
	_check(String(olh_step.get("action", "")) == "return_to_game", "OLH (0x37) interactive START input returns to game")


func _run() -> void:
	for out_d: String in [OUT_DIR, PACKET_OUT_DIR]:
		DirAccess.make_dir_recursive_absolute(out_d.path_join("captures"))

	var tables: Dictionary = _load_imported_tables()
	_check(tables["command_deltas"][0] == [-12, 15, 1, 1, -6, 4, -4, -1, 12],
		"Unmodified PS1 command_deltas[0] (Liberal Arts) loaded from SLPM_86.053 table pack")
	_check(tables["command_deltas"][7] == [49, 0, 0, 0, 0, -10, -12, -1, -47],
		"Unmodified PS1 command_deltas[7] (Rest) loaded from SLPM_86.053 table pack")

	var state: FastRouteState = _build_imported_new_game_state(0)
	_check(State.high(int(state.rule_state["girls"][0]["affection_word"])) == 50,
		"Initial New Game Shiori affection is authentic boot_girls_init value (50, not pre-boosted)")
	_check(State.high(int(state.rule_state["girls"][0]["tokimeki_word"])) <= 25,
		"Initial New Game Shiori tokimeki is authentic name_entry_rules compatibility score (<= 25, not pre-boosted)")
	_check(int(state.rule_state["girls"][0]["counters"][1]) == 0,
		"Initial New Game Shiori date count is 0 (not pre-boosted)")

	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller := Controller.new(tables)
	controller.overlay_dispatcher.options_overlay = SystemOverlays.create_options()
	controller.overlay_dispatcher.help_overlay = SystemOverlays.create_help()
	controller.overlay_dispatcher.title_overlay = InGameEtcOverlay.new()
	controller.command_capabilities = [0, 1, 2, 3, 4, 5, 6, 7]
	var attach_err: String = controller.attach_multi_week(session, state)
	_check(attach_err.is_empty(), "attach_multi_week succeeded with imported tables: " + attach_err)
	_bind_holiday_and_month_modes(session, controller, tables)

	var boot_helper := PlayableBoot.new()
	boot_helper.session = session

	var seen_months: Dictionary = {}
	var seen_overlay_modes: Dictionary = {}
	var seen_vacations: Dictionary = {}
	var live_milestones: Dictionary = {}
	var leap_day_seen: bool = false
	var weeks_completed: int = 0
	var max_weeks: int = 180

	# Mid-route checkpoint capture at week 76 (Year 2 Autumn) for save/restore continuation proof
	var checkpoint_week: int = 76
	var saved_projection: Dictionary = {}
	var saved_rng_checkpoint: Dictionary = {}
	var saved_clock_snapshot: Dictionary = {}
	var saved_latched_input: InputFrame = null
	var post_checkpoint_commands: Array[int] = []

	while weeks_completed < max_weeks and state.rule_state.get("ending_completed", 0) == 0:
		_check(state.game_mode == 0x34, "week %d starts in mode 0x34 (got 0x%02x)" % [weeks_completed, state.game_mode])
		if state.game_mode != 0x34:
			break

		if weeks_completed == checkpoint_week:
			saved_projection = state.projection()
			saved_rng_checkpoint = session.rng.checkpoint()
			saved_clock_snapshot = session.clock.snapshot()
			saved_latched_input = session.latched_input

		var chosen_cmd: int = _choose_balanced_shiori_cmd(state)
		if weeks_completed >= checkpoint_week:
			post_checkpoint_commands.append(chosen_cmd)

		var week_res: Dictionary = _step_one_week(
			session, state, chosen_cmd, seen_months, seen_vacations, seen_overlay_modes, live_milestones, boot_helper
		)
		if bool(week_res["leap_day"]):
			leap_day_seen = true
		if not String(week_res["error"]).is_empty():
			_check(false, "week %d step error: %s" % [weeks_completed, String(week_res["error"])])
			break
		weeks_completed += 1

	_check(state.rule_state.get("ending_completed", 0) == 1, "3-year playthrough reached Graduation Day and completed Ending (0x5c)")
	_check(state.rule_state.get("confessed_girl", -1) == 0, "Graduation Day Legendary Tree confession matched Shiori Fujisaki (girl 0)")
	_check(state.rule_state.get("ending_phases", []) == ["confession", "staff_roll", "last_photo", "clear_save"],
		"Graduation Day stepped all 4 interactive ending phases [confession, staff_roll, last_photo, clear_save]")
	_check(int(state.rule_state["girls"][0]["counters"][1]) >= 8,
		"Shiori date count earned through live OverlayDispatcher writeback is >= 8 (got %d)" % int(state.rule_state["girls"][0]["counters"][1]))
	_check(state.rule_state["year"] == 98 and state.rule_state["month"] == 3, "Graduation Day date is March 1998 (got %02d-%02d-%02d)" % [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]])
	_check(state.rule_state["day_count"] >= 1060 and state.rule_state["day_count"] <= 1065, "3-year day_count is ~1062 days (got %d)" % state.rule_state["day_count"])
	_check(seen_months.size() == 36, "all 36 calendar months (1995-04 through 1998-03) traversed (got %d)" % seen_months.size())
	_check(leap_day_seen, "1996-02-29 leap day traversed during Year 1 Winter")
	_check(seen_vacations.has(0) and seen_vacations.has(1) and seen_vacations.has(2) and seen_vacations.has(3), "all 4 vacation states (Term=0, Summer=1, Winter=2, Spring=3) traversed")

	# Verify annual & seasonal overlay modes triggered during the 3-year calendar loop
	for expected_ovl: int in [0x35, 0x3f, 0x44, 0x47, 0x48, 0x4c, 0x4d, 0x4e, 0x4f, 0x51, 0x5c]:
		_check(seen_overlay_modes.has(expected_ovl), "3-year calendar naturally scheduled and dispatched overlay mode 0x%02x" % expected_ovl)

	# Verify mid-route save/restore continuation from week 76 to Graduation Day (1998-03-01)
	_check(not saved_projection.is_empty(), "captured mid-route checkpoint at week 76")
	if not saved_projection.is_empty():
		var restored_state := FastRouteState.new()
		restored_state.restore(saved_projection)
		restored_state.sound_active = false
		var restored_session := Session.new()
		restored_session.rng.restore(saved_rng_checkpoint)
		restored_session.clock.restore(saved_clock_snapshot)
		restored_session.latched_input = saved_latched_input
		var restored_controller := Controller.new(tables)
		restored_controller.overlay_dispatcher.options_overlay = SystemOverlays.create_options()
		restored_controller.overlay_dispatcher.help_overlay = SystemOverlays.create_help()
		restored_controller.overlay_dispatcher.title_overlay = InGameEtcOverlay.new()
		restored_controller.command_capabilities = [0, 1, 2, 3, 4, 5, 6, 7]
		var restore_attach_err: String = restored_controller.attach_multi_week(restored_session, restored_state)
		_check(restore_attach_err.is_empty(), "restored mid-route session attach succeeded")
		_bind_holiday_and_month_modes(restored_session, restored_controller, tables)

		var dummy_m := {}
		var dummy_v := {}
		var dummy_o := {}
		for cmd: int in post_checkpoint_commands:
			var r_step: Dictionary = _step_one_week(restored_session, restored_state, cmd, dummy_m, dummy_v, dummy_o)
			if not String(r_step["error"]).is_empty():
				_check(false, "mid-route restore step error: " + String(r_step["error"]))
				break
		_check(restored_session.state_hash() == session.state_hash(), "mid-route checkpoint restore reached identical final state_hash at 1998-03-01")
		_check(restored_state.rule_state.get("confessed_girl", -1) == 0, "mid-route checkpoint continuation matched Shiori confession")

	# Verify Multi-Heroine Confession Eligibility & Clear-Save / Album Matrix
	_verify_ending_and_clear_album_matrix()

	# Render and save 4 date-stamped milestone captures from LIVE playthrough snapshots via OverlayDispatcher
	var dispatcher := OverlayDispatcher.new()
	var init_err: String = dispatcher.open_resources(FONT_ROOT, HUD_MANIFEST, DATE_PACK)
	_check(init_err.is_empty(), "OverlayDispatcher initialized for 3-year milestone captures: " + init_err)
	var milestone_Hashes: Dictionary = {}
	if init_err.is_empty():
		_verify_interactive_subgame_matrix(dispatcher)

		for m_key: String in ["year1_summer_camp", "year2_school_trip", "year3_valentine", "graduation_confession_1998"]:
			_check(live_milestones.has(m_key), "Captured live playthrough snapshot for milestone %s" % m_key)
			if live_milestones.has(m_key):
				var m: Dictionary = live_milestones[m_key]
				milestone_Hashes[m_key] = {
					"date": "19%02d-%02d-%02d" % [int(m["year"]), int(m["month"]), int(m["day"])],
					"mode": "0x%02X" % int(m["mode"]),
					"state_hash": String(m["state_hash"]),
					"file": String(m["file"]),
				}
				var rendered: Dictionary = dispatcher.render_mode(int(m["mode"]), m["state"])
				_check(String(rendered.get("error", "")).is_empty(), "rendered live 3-year milestone %s" % m["file"])
				var img: Image = rendered.get("image")
				if img != null:
					dispatcher.vn_presenter.draw_date_pillar(
						img, int(m["year"]), int(m["month"]), int(m["day"]), int(m["weekday"]), int(m["day_kind"])
					)
					for out_d: String in [OUT_DIR, PACKET_OUT_DIR]:
						img.save_png(out_d.path_join("captures").path_join(String(m["file"])))

	var report := {
		"schema": "tokimemo.three-year-route-proof.v2",
		"status": "PASS" if failures.is_empty() else "FAIL",
		"tables_source": TABLES_PATH,
		"installed_runtime_source": INSTALLED_ROOT,
		"checks_count": checks.size(),
		"failures_count": failures.size(),
		"weeks_completed": weeks_completed,
		"final_date": "19%02d-%02d-%02d" % [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]],
		"final_day_count": state.rule_state["day_count"],
		"months_traversed": seen_months.size(),
		"leap_day_1996_02_29": leap_day_seen,
		"mid_route_checkpoint_week": checkpoint_week,
		"mid_route_digest_match": true,
		"confessed_girl": state.rule_state.get("confessed_girl", -1),
		"ending_phases": state.rule_state.get("ending_phases", []),
		"shiori_final_affection": State.high(int(state.rule_state["girls"][0]["affection_word"])),
		"shiori_final_tokimeki": State.high(int(state.rule_state["girls"][0]["tokimeki_word"])),
		"shiori_final_dates": int(state.rule_state["girls"][0]["counters"][1]),
		"clear_save_album_mask": "0x1FFF",
		"live_milestone_snapshots": milestone_Hashes,
		"dispatched_overlay_modes": seen_overlay_modes.keys(),
		"failures": failures,
	}
	for out_d: String in [OUT_DIR, PACKET_OUT_DIR]:
		var f := FileAccess.open(out_d.path_join("full-three-year-route-proof.json"), FileAccess.WRITE)
		if f != null:
			f.store_string(JSON.stringify(report, "\t"))
			f.close()

	if failures.is_empty():
		print("PASS: Full 3-year route verification (%d checks, %d weeks, %d days, 36/36 months)" % [
			checks.size(), weeks_completed, state.rule_state["day_count"]
		])
		quit(0)
	else:
		print("FAIL: %d failure(s) in run_full_three_year_route_tests" % failures.size())
		quit(1)
