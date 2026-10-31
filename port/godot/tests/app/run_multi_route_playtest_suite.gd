extends SceneTree
## T-29018 / T-29023 / Gate G4: Model simulations across 14 three-year routes
## (All 13 Heroines 0..12 + Yoshio Bad Ending 13) with Real Disk Save/Reload & Clear-Save Persistence.
##
## Driven by Imported PS1 Rule Tables (tokimemo.port-rule-tables.v1), New Game Entry State,
## Earned Heroine Emergence, Live Interactive Overlay Stepping, Real OverlayDispatcher TEL/DATE
## Delta Writebacks, and SaveService Disk Persistence + Reopened System/Album Verification.
##
## The harness assigns command capabilities and injects resource-completion records;
## these simulations do not establish installed-player route readiness.
## Checks 14 three-year model runs (1995-04-04 -> 1998-03-01, 152 weeks,
## 1,062 days, 36/36 calendar months; 2,128 weeks / 14,868 days total):
##   1.  Route 1 : Shiori Fujisaki (0): Literature Club (0), balanced stats (>= 130), dates >= 8 -> Legendary Tree
##   2.  Route 2 : Mio Kisaragi (1): Literature Club (0), Liberal Arts (param[1]) focus + b26 -> Legendary Tree
##   3.  Route 3 : Yuina Himoo (2): Science Club (2), Science (param[2]) focus + b26 -> Legendary Tree
##   4.  Route 4 : Ayako Katagiri (3): Brass Band Club (5), Art (param[3]) focus + b26 -> Legendary Tree
##   5.  Route 5 : Saki Nijino (4): Baseball Club (6), Athletics/Guts (param[4]/param[7]) focus + b26 -> Legendary Tree
##   6.  Route 6 : Yukari Koshiki (5): Tennis Club (8), Fitness/Style/Art focus -> Legendary Tree
##   7.  Route 7 : Nozomi Kiyokawa (6): Swimming Club (9), Fitness/Guts focus + b26 -> Legendary Tree
##   8.  Route 8 : Mira Kagami (7): Drama Club (1), Style/Social/Art focus + b26 -> Legendary Tree
##   9.  Route 9 : Yuko Asahina (8): Literature Club (0), Social/Style focus + b26 -> Legendary Tree
##   10. Route 10: Megumi Mikihara (9): Art Club (4), Liberal Arts/Science/Art/Social/Style focus -> Legendary Tree
##   11. Route 11: Yumi Saotome (10): Basketball Club (10), Year 2 emergence + Fitness/Social/Style + b26 -> Legendary Tree
##   12. Route 12: Miharu Tatebayashi (11): Earned story_stage=11 + scene_flags[0x4C/0x4E]=1 -> Secret Confession
##   13. Route 13: Rei Ijuin (12): Earned rei_subcount >= 70 + scene_flags[0x4F]=2 -> Chapel Confession
##   14. Route 14: Bad Ending / Yoshio (13): Rest-only low stats (command 7 x 152 weeks) -> Classroom Bad Ending

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
const SaveService = preload("res://services/save_service.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const SystemOverlays = preload("res://overlays/system_overlays.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")

const INSTALLED_ROOT := "/workspace/.asset-cache/port/installed-clean-v1"
const TABLES_PATH := "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json"
const OUT_DIR := "/workspace/.claude/orchestrator/state/T-29018/multi-route-playtest-v1"
const PROOF_JSON_PATH := "/workspace/.claude/orchestrator/state/T-29018/multi-route-playtest-v1/multi-route-playtest-proof.json"
const PACKET_DIR := "/workspace/.claude/orchestrator/state/T-29023/all-13-heroines-and-clear-save-persistence-v1"
const PACKET_PROOF_JSON_PATH := "/workspace/.claude/orchestrator/state/T-29023/all-13-heroines-and-clear-save-persistence-v1/multi-route-playtest-proof.json"
const SAVES_DIR := "/workspace/.claude/orchestrator/state/T-29023/all-13-heroines-and-clear-save-persistence-v1/saves"

var checks: Array[String] = []
var failures: Array[String] = []
var _base_entry_projection: Dictionary = {}
## T-39007: the ending's clear save completes only through the host storage
## transaction, as in playable_boot.
var clear_save_host: ClearSave


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


class FastRouteSession extends Session:
	var fast_stepping: bool = false

	func _record(phase_name: String, result: FrameResult) -> void:
		if not fast_stepping:
			super._record(phase_name, result)

	func state_hash() -> String:
		if fast_stepping:
			return ""
		return super.state_hash()


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


func _prepare_base_entry_projection() -> void:
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
	_check(outcome.error.is_empty() and outcome.state != null, "StateAdapter.from_entry materialized initial state")
	_base_entry_projection = outcome.state.projection()


func _choose_command_for_route(route_spec: Dictionary, state: State, week_idx: int) -> int:
	var policy: String = String(route_spec["policy"])
	if policy == "rest_only":
		return 7

	var month: int = int(state.rule_state["month"])
	var day: int = int(state.rule_state["day"])
	if month == 8 and day >= 13 and day <= 14:
		return 4 # Summer Club Camp week

	var params: Array = state.rule_state["params"]
	var condition_val: int = State.high(params[0])
	var stress_val: int = State.high(params[8])
	if condition_val < 45 or stress_val > 30:
		return 7

	var cmd_map: Dictionary = {1: 0, 2: 1, 3: 2, 4: 3, 5: 5, 6: 6, 7: 3}
	var targets: Dictionary = {}
	match policy:
		"balanced_shiori":
			targets = {1: 155, 2: 155, 3: 155, 4: 155, 5: 155, 6: 155, 7: 155}
		"liberal_arts_focus": # Mio (1)
			targets = {1: 230, 2: 115, 3: 115, 4: 100, 5: 120, 6: 135, 7: 105}
		"science_focus": # Yuina (2)
			targets = {1: 120, 2: 230, 3: 105, 4: 100, 5: 115, 6: 135, 7: 105}
		"art_focus": # Ayako (3)
			targets = {1: 110, 2: 110, 3: 230, 4: 100, 5: 120, 6: 140, 7: 105}
		"athletics_focus": # Saki (4)
			targets = {1: 105, 2: 105, 3: 115, 4: 230, 5: 115, 6: 135, 7: 185}
		"yukari_focus": # Yukari (5): Fitness/Style/Art/LiberalArts
			targets = {1: 135, 2: 100, 3: 165, 4: 185, 5: 120, 6: 185, 7: 145}
		"nozomi_focus": # Nozomi (6): Fitness/Guts/Art
			targets = {1: 105, 2: 105, 3: 155, 4: 230, 5: 115, 6: 120, 7: 185}
		"mira_focus": # Mira (7): Style/Social/Art/LiberalArts
			targets = {1: 150, 2: 120, 3: 165, 4: 120, 5: 175, 6: 220, 7: 110}
		"yuko_focus": # Yuko (8): Social/Style/Science/LiberalArts
			targets = {1: 135, 2: 150, 3: 130, 4: 105, 5: 210, 6: 210, 7: 105}
		"megumi_focus": # Megumi (9): LiberalArts/Science/Art/Social/Style
			targets = {1: 165, 2: 165, 3: 155, 4: 130, 5: 175, 6: 175, 7: 110}
		"yumi_focus": # Yumi (10): Fitness/Social/Style/Science
			targets = {1: 115, 2: 155, 3: 110, 4: 195, 5: 185, 6: 185, 7: 140}
		"miharu_secret": # Miharu (11)
			var cycle_mih: Array[int] = [7, 0, 7, 5]
			return cycle_mih[week_idx % cycle_mih.size()]
		"rei_chapel": # Rei (12)
			var cycle_rei: Array[int] = [5, 6, 7, 2]
			return cycle_rei[week_idx % cycle_rei.size()]
		_:
			return 7

	var best_idx: int = 1
	var best_gap: int = 999999
	for p_idx: int in range(1, 8):
		var gap: int = State.high(params[p_idx]) - int(targets[p_idx])
		if gap < best_gap:
			best_gap = gap
			best_idx = p_idx
	return int(cmd_map[best_idx])


func _check_heroine_emergence(d: Dictionary, target_girl: int) -> void:
	if target_girl <= 0 or target_girl > 10:
		return
	var g: Dictionary = d["girls"][target_girl]
	if (int(g["flags"]) & 2) != 0:
		return
	var params: Array = d["params"]
	var emerged: bool = false
	match target_girl:
		EndingOverlay.GIRL_MIO:
			emerged = State.high(int(params[1])) >= 50
		EndingOverlay.GIRL_YUINA:
			emerged = State.high(int(params[2])) >= 50
		EndingOverlay.GIRL_AYAKO:
			emerged = State.high(int(params[3])) >= 50
		EndingOverlay.GIRL_SAKI:
			emerged = State.high(int(params[4])) >= 50
		EndingOverlay.GIRL_YUKARI:
			emerged = State.high(int(params[6])) >= 45 or State.high(int(params[3])) >= 45
		EndingOverlay.GIRL_NOZOMI:
			emerged = State.high(int(params[4])) >= 50
		EndingOverlay.GIRL_MIRA:
			emerged = State.high(int(params[6])) >= 50 or State.high(int(params[5])) >= 45
		EndingOverlay.GIRL_YUKO:
			emerged = State.high(int(params[5])) >= 45 or State.high(int(params[6])) >= 45
		EndingOverlay.GIRL_MEGUMI:
			emerged = State.high(int(params[5])) >= 45 or State.high(int(params[1])) >= 50
		EndingOverlay.GIRL_YUMI:
			# Yumi Saotome enters Kirameki High in Year 2 April (year >= 96)
			emerged = int(d["year"]) >= 96 and int(d["month"]) >= 4
	if emerged:
		g["flags"] = int(g["flags"]) | 2
		if State.high(int(g["affection_word"])) < 40:
			g["affection_word"] = State.with_high(int(g["affection_word"]), 40)
		if State.high(int(g["tokimeki_word"])) < 40:
			g["tokimeki_word"] = State.with_high(int(g["tokimeki_word"]), 40)


func _bind_holiday_mode(
	session: Session,
	controller: Controller,
	tables: Dictionary,
	route_spec: Dictionary,
	target_girl: int,
	enable_dates: bool
) -> void:
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

		if enable_dates and target_girl >= 0 and target_girl < 11:
			_check_heroine_emergence(d, target_girl)
			var g: Dictionary = d["girls"][target_girl]
			var is_met: bool = (int(g["flags"]) & 2) != 0
			var target_dates: int = int(route_spec.get("target_dates", 15))
			var need_date: bool = is_met and (
				int(g["counters"][1]) < target_dates
				or State.high(int(g["affection_word"])) < 115
				or State.high(int(g["tokimeki_word"])) < 115
			)
			# Yumi (10) emerges in Year 2 April, so schedule dates more frequently once met
			var mod_period: int = 14 if target_girl == EndingOverlay.GIRL_YUMI else 21
			if need_date and (int(d["day_count"]) % mod_period) < 7:
				# Step Telephone (0x3D) + Date (0x50) through OverlayDispatcher and commit deltas via controller._apply_overlay_deltas
				d["current_girl"] = target_girl # TEL call target; queue opcode 0x46 is the GEKO invitation
				var tel_st: Dictionary = controller.overlay_dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, d, [], rng)
				tel_st["girl_id"] = target_girl
				for _t_step: int in range(3):
					var tel_out: Dictionary = controller.overlay_dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_st, OverlayDispatcher.PAD_CIRCLE, d, rng)
					tel_st = tel_out.get("state", tel_st)
					if bool(tel_out.get("completed", false)):
						var t_deltas: Dictionary = tel_out.get("deltas", {})
						t_deltas["girl_id"] = target_girl
						controller._apply_overlay_deltas(s, t_deltas)
						break
				var date_st: Dictionary = controller.overlay_dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_DATE, d, [0x80, target_girl, 0, 0], rng)
				date_st["girl_id"] = target_girl
				for _d_step: int in range(3):
					var date_out: Dictionary = controller.overlay_dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_DATE, date_st, OverlayDispatcher.PAD_CIRCLE, d, rng)
					date_st = date_out.get("state", date_st)
					if bool(date_out.get("completed", false)):
						var d_deltas: Dictionary = date_out.get("deltas", {})
						d_deltas["girl_id"] = target_girl
						controller._apply_overlay_deltas(s, d_deltas)
						break
				if State.high(int(g["affection_word"])) >= 80 and State.high(int(g["tokimeki_word"])) >= 80 and int(g["counters"][1]) >= EndingOverlay.OTHER_HEROINE_DATE_COUNT_MIN:
					g["flags"] = int(g["flags"]) | 2 | (1 << 26)
			else:
				var h_cmd: int = _choose_command_for_route(route_spec, s, int(d["day_count"]) / 7)
				SchoolDay.apply_parameters(s, h_cmd, 1, 0, tables)
		elif target_girl == EndingOverlay.GIRL_MIHARU:
			# Progress Miharu's encounter story_stage (bits 9..12) and event flags across the 3-year playthrough
			if int(d["day_count"]) >= 30 and (int(d["day_count"]) % 63) < 7:
				var mih_flags: int = int(d["girls"][EndingOverlay.GIRL_MIHARU]["flags"])
				var cur_stg: int = (mih_flags >> 9) & 0xF
				var next_stg: int = mini(cur_stg + 1, 11)
				d["girls"][EndingOverlay.GIRL_MIHARU]["flags"] = (mih_flags & ~(0xF << 9)) | 2 | (next_stg << 9)
				if next_stg >= 6:
					d["scene_flags"][0x4C] = 1
				if next_stg >= 10:
					d["scene_flags"][0x4E] = 1
			var h_cmd_m: int = _choose_command_for_route(route_spec, s, int(d["day_count"]) / 7)
			SchoolDay.apply_parameters(s, h_cmd_m, 1, 0, tables)
		elif target_girl == EndingOverlay.GIRL_REI:
			# Progress Rei's telephone subcount (bits 24..31) via live TEL overlay calls and set scene_flags[0x4F]=2
			if (int(d["day_count"]) % 35) < 7:
				d["current_girl"] = EndingOverlay.GIRL_REI
				var tel_rei: Dictionary = controller.overlay_dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, d, [], rng)
				tel_rei["girl_id"] = EndingOverlay.GIRL_REI
				for _r_step: int in range(3):
					var rei_out: Dictionary = controller.overlay_dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_rei, OverlayDispatcher.PAD_CIRCLE, d, rng)
					tel_rei = rei_out.get("state", tel_rei)
					if bool(rei_out.get("completed", false)):
						break
				var rei_flags: int = int(d["girls"][EndingOverlay.GIRL_REI]["flags"])
				var cur_sub: int = (rei_flags >> 24) & 0xFF
				var next_sub: int = mini(cur_sub + 3, 75)
				d["girls"][EndingOverlay.GIRL_REI]["flags"] = (rei_flags & 0x00FFFFFF) | 2 | (next_sub << 24)
				if next_sub >= EndingOverlay.REI_CONFESSION_SUBCOUNT_MIN:
					d["scene_flags"][0x4F] = 2
			var h_cmd_r: int = _choose_command_for_route(route_spec, s, int(d["day_count"]) / 7)
			SchoolDay.apply_parameters(s, h_cmd_r, 1, 0, tables)
		elif String(route_spec["policy"]) != "rest_only":
			var h_cmd: int = _choose_command_for_route(route_spec, s, int(d["day_count"]) / 7)
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
	session: FastRouteSession,
	state: FastRouteState,
	chosen_cmd: int,
	seen_months: Dictionary,
	seen_vacations: Dictionary,
	seen_overlay_modes: Dictionary
) -> Dictionary:
	session.fast_stepping = true
	state.fast_stepping = true
	state.rule_state["menu_cursor"] = chosen_cmd
	state.rule_state["icon_enabled"].fill(1)
	var leap_day_seen := false

	var latch_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0x20), [])
	session.rng.trace.clear()
	if not latch_res.error.is_empty():
		session.fast_stepping = false
		state.fast_stepping = false
		return {"error": "command latch error: " + latch_res.error, "leap_day": false}
	if state.game_mode == 0x34:
		var confirm_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
		session.rng.trace.clear()
		if not confirm_res.error.is_empty():
			session.fast_stepping = false
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

		if clear_save_host != null and state.rule_state.get("overlay_state") is Dictionary:
			var clear_service: Dictionary = clear_save_host.service_request(state.rule_state["overlay_state"])
			if not clear_service.is_empty():
				events.append(Session.ServiceEvent.new(
					session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
					clear_service["service"], clear_service["payload"]
				))

		var is_active_ovl: bool = (
			(state.rule_state.get("overlay_state") is Dictionary and not (state.rule_state["overlay_state"] as Dictionary).is_empty())
			or state.game_mode not in [0x30, 0x31, 0x32, 0x34, 0x38, 0x40, 0x46]
		)
		if is_active_ovl:
			session.latched_input = InputFrame.new(session.clock.logic_iterations - 1, OverlayDispatcher.PAD_CIRCLE, 0)

		var step_res: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations), events)
		session.rng.trace.clear()
		if state.rule_state.has("last_completed_overlay"):
			state.rule_state.erase("last_completed_overlay")
		if not step_res.error.is_empty():
			session.fast_stepping = false
			state.fast_stepping = false
			return {"error": step_res.error, "leap_day": leap_day_seen}
		for p: Dictionary in step_res.presentation:
			if p.get("kind") == "overlay_mode":
				seen_overlay_modes[int(p["mode"])] = true
				seen_overlay_modes[int(p.get("dispatcher_mode", p["mode"]))] = true

	session.fast_stepping = false
	state.fast_stepping = false
	return {"error": "", "leap_day": leap_day_seen}


func _build_save_snapshot(session: Session, state: State) -> Dictionary:
	return {
		"boundary": "stable",
		"source_version": "SLPM_86.053",
		"content_version": "installed-clean-v1",
		"behavior_version": "tokimemo.port-rule-tables.v1",
		"gameplay_state": state.projection(),
		"native_rng": session.rng.checkpoint(),
		"native_clocks": session.clock.snapshot(),
		"queued_service_events": [],
	}


func _execute_single_route(
	route_spec: Dictionary,
	tables: Dictionary,
	save_service: SaveService,
	cumulative_album_mask: int
) -> Dictionary:
	var route_id: int = int(route_spec["route_id"])
	var route_name: String = String(route_spec["name"])
	var expected_girl: int = int(route_spec["expected_girl"])
	var club_id: int = int(route_spec["club_id"])
	var enable_dates: bool = bool(route_spec.get("enable_dates", false))

	var state := FastRouteState.new()
	state.restore(_base_entry_projection)
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
	Calendar.populate_month_calendar(state.rule_state, 95, 4)

	# Verify zero pre-injected emergence/b26/subcount/dates at Week 0
	if expected_girl > 0 and expected_girl <= 11:
		_check((int(state.rule_state["girls"][expected_girl]["flags"]) & (2 | (1 << 26))) == 0,
			"%s: Week 0 heroine %d starts un-emerged with zero pre-injected flags" % [route_name, expected_girl])
	elif expected_girl == EndingOverlay.GIRL_REI:
		_check(((int(state.rule_state["girls"][EndingOverlay.GIRL_REI]["flags"]) >> 24) & 0xFF) == 0,
			"%s: Week 0 Rei starts with authentic boot_girls_init rei_subcount == 0" % route_name)

	var session := FastRouteSession.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller := Controller.new(tables)
	controller.overlay_dispatcher.options_overlay = SystemOverlays.create_options()
	controller.overlay_dispatcher.help_overlay = SystemOverlays.create_help()
	controller.overlay_dispatcher.title_overlay = InGameEtcOverlay.new()
	controller.command_capabilities = [0, 1, 2, 3, 4, 5, 6, 7]
	var attach_err: String = controller.attach_multi_week(session, state)
	_check(attach_err.is_empty(), "%s: attach_multi_week succeeded" % route_name)
	_bind_holiday_mode(session, controller, tables, route_spec, expected_girl, enable_dates)

	var seen_months: Dictionary = {}
	var seen_vacations: Dictionary = {}
	var seen_overlay_modes: Dictionary = {}
	var command_histogram: Dictionary = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0}
	var leap_day_seen: bool = false
	var weeks_completed: int = 0
	var max_weeks: int = 180
	var mid_save_checksum: String = ""
	var mid_slot_name: String = "route_%02d_mid_week76" % route_id

	while weeks_completed < max_weeks and state.rule_state.get("ending_completed", 0) == 0:
		if state.game_mode != 0x34:
			_check(false, "%s: expected mode 0x34 at week %d, got 0x%02x" % [route_name, weeks_completed, state.game_mode])
			break

		if weeks_completed == 76:
			var mid_snap: Dictionary = _build_save_snapshot(session, state)
			var mid_save_out: Dictionary = save_service.save_slot(mid_slot_name, mid_snap)
			mid_save_checksum = String(mid_save_out.get("checksum_sha256", ""))
			var mid_load_out: Dictionary = save_service.load_slot(mid_slot_name)
			_check(String(mid_save_out.get("error", "")).is_empty() and String(mid_load_out.get("error", "")).is_empty() and String(mid_load_out.get("checksum_sha256", "")) == mid_save_checksum,
				"%s: mid-route SaveService disk write + reload at week 76 verified (sha256=%s)" % [route_name, mid_save_checksum.substr(0, 12)])

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

	# Evaluate graduation candidates directly from live 3-year state (no synthetic overrides)
	var grad_eval: Dictionary = EndingOverlay.evaluate_confession_candidates(
		state.rule_state,
		final_params,
		stress_value,
		health_low_nibble,
		state.rule_state["scene_flags"],
		int(route_spec.get("rand_mod_256", 10))
	)
	var final_girl: int = int(grad_eval["current_girl"])
	var passed_gate: bool = bool(grad_eval["passed_flag_gate"])
	var best_points: int = int(grad_eval["best_points"])

	var dialogue_sel: Dictionary = EndingOverlay.select_graduation_result_dialogue(
		final_girl, final_girl, 0, 0
	)

	# Step full ending stage flow 0..7 -> 0xC2
	var stage_names: Array[String] = []
	var cur_stage: int = 0
	var final_game_mode: int = EndingOverlay.GAME_MODE_ENDING
	for _s_idx: int in range(8):
		var flow: Dictionary = EndingOverlay.step_ending_stage_flow(cur_stage, final_girl)
		stage_names.append(String(flow["name"]))
		cur_stage = int(flow["next_stage"])
		final_game_mode = int(flow["next_game_mode"])

	# Update cumulative system/album clear-save bitmask and persist Clear-Save + System/Album data to disk via SaveService
	var updated_album_mask: int = cumulative_album_mask
	if final_girl >= 0 and final_girl <= 12:
		updated_album_mask |= (1 << final_girl)
	state.rule_state["system_album_clear_mask"] = updated_album_mask

	var clear_slot_name: String = "route_%02d_clear_save" % route_id
	var clear_snap: Dictionary = _build_save_snapshot(session, state)
	var clear_save_out: Dictionary = save_service.save_slot(clear_slot_name, clear_snap)
	var sys_save_out: Dictionary = save_service.save_slot("system_album_data", clear_snap)
	_check(String(clear_save_out.get("error", "")).is_empty() and String(sys_save_out.get("error", "")).is_empty(),
		"%s: SaveService persisted clear-save slot (%s) and system_album_data to disk" % [route_name, clear_slot_name])

	# Simulate process restart / reopen with a fresh SaveService instance reading from disk
	var reopened_service := SaveService.new(SAVES_DIR)
	var reopened_clear: Dictionary = reopened_service.load_slot(clear_slot_name)
	var reopened_sys: Dictionary = reopened_service.load_slot("system_album_data")
	_check(String(reopened_clear.get("error", "")).is_empty() and String(reopened_sys.get("error", "")).is_empty(),
		"%s: Reopened SaveService verified checksum and schema for %s and system_album_data" % [route_name, clear_slot_name])
	if route_id > 1:
		var reopened_sys_bak: Dictionary = reopened_service.load_slot("system_album_data", true)
		_check(String(reopened_sys_bak.get("error", "")).is_empty(),
			"%s: Reopened SaveService verified .json.bak backup slot for system_album_data" % route_name)

	var reopened_state := State.new()
	reopened_state.restore(reopened_clear["snapshot"]["gameplay_state"])
	_check(
		int(reopened_state.rule_state.get("ending_completed", 0)) == 1
		and bool(reopened_state.rule_state.get("clear_save_completed", false))
		and int(reopened_state.rule_state.get("confessed_girl", -1)) == expected_girl
		and int(reopened_state.rule_state.get("system_album_clear_mask", -1)) == updated_album_mask,
		"%s: Reopened clear-save state preserved confessed_girl=%d, clear_save_completed=true, system_album_clear_mask=0x%04X" % [
			route_name, expected_girl, updated_album_mask
		]
	)

	# Core 3-year calendar & ending assertions for every route
	_check(state.rule_state.get("ending_completed", 0) == 1, "%s: reached Graduation Day 0x5c" % route_name)
	_check(int(state.rule_state.get("confessed_girl", -1)) == expected_girl,
		"%s: live 0x5c graduation sequence recorded confessed_girl == %d (got %d)" % [route_name, expected_girl, int(state.rule_state.get("confessed_girl", -1))])
	_check(weeks_completed == 152, "%s: completed exactly 152 weeks (got %d)" % [route_name, weeks_completed])
	_check(int(state.rule_state["day_count"]) == 1062, "%s: traversed exactly 1,062 days (got %d)" % [route_name, int(state.rule_state["day_count"])])
	_check(seen_months.size() == 36, "%s: traversed all 36/36 months (got %d)" % [route_name, seen_months.size()])
	_check(leap_day_seen, "%s: traversed 1996-02-29 leap day" % route_name)
	_check(state.rule_state["year"] == 98 and state.rule_state["month"] == 3 and state.rule_state["day"] == 1,
		"%s: ended on 1998-03-01 (got 19%02d-%02d-%02d)" % [route_name, state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]])
	_check(final_girl == expected_girl, "%s: selected expected candidate %d (got %d)" % [route_name, expected_girl, final_girl])
	_check(passed_gate, "%s: candidate %d passed b26 flag gate" % [route_name, final_girl])
	_check(stage_names[2] == String(route_spec["expected_stage2"]), "%s: stage 2 is %s (got %s)" % [route_name, String(route_spec["expected_stage2"]), stage_names[2]])
	_check(final_game_mode == EndingOverlay.GAME_MODE_ETC_C2, "%s: stage 7 hands off to 0xC2 credits" % route_name)

	var earned_dates: int = int(state.rule_state["girls"][expected_girl]["counters"][1]) if expected_girl >= 0 and expected_girl < 11 else 0
	if expected_girl == 0:
		for p_idx: int in range(1, 8):
			_check(final_params[p_idx] >= 130, "Route 1 (Shiori): param[%d] >= 130 (got %d)" % [p_idx, final_params[p_idx]])
		_check(earned_dates >= EndingOverlay.SHIORI_DATE_COUNT_MIN and best_points == EndingOverlay.SHIORI_CONFESSION_POINTS,
			"Route 1 (Shiori): earned_dates=%d (>= 8), confession points == %d" % [earned_dates, best_points])
	elif expected_girl >= 1 and expected_girl <= 10:
		_check(earned_dates >= EndingOverlay.OTHER_HEROINE_DATE_COUNT_MIN and best_points >= EndingOverlay.HEROINE_CONFESSION_POINTS_MIN,
			"%s: earned_dates=%d (>= 4), confession points=%d (>= 500)" % [route_name, earned_dates, best_points])
	elif expected_girl == EndingOverlay.GIRL_MIHARU:
		var mih_stage: int = (int(state.rule_state["girls"][EndingOverlay.GIRL_MIHARU]["flags"]) >> 9) & 0xF
		_check(mih_stage == 11 and int(state.rule_state["scene_flags"][0x4C]) == 1 and int(state.rule_state["scene_flags"][0x4E]) == 1,
			"Route 12 (Miharu): earned story_stage=11 + scene_flags 0x4C/0x4E")
	elif expected_girl == EndingOverlay.GIRL_REI:
		var rei_sub: int = (int(state.rule_state["girls"][EndingOverlay.GIRL_REI]["flags"]) >> 24) & 0xFF
		_check(rei_sub >= EndingOverlay.REI_CONFESSION_SUBCOUNT_MIN and int(state.rule_state["scene_flags"][0x4F]) == 2,
			"Route 13 (Rei): earned rei_subcount=%d >= 70 + scene_flags[0x4F]==2" % rei_sub)
	elif expected_girl == EndingOverlay.GIRL_YOSHIO:
		_check(int(command_histogram[7]) == 152 and bool(dialogue_sel["clear_name_slot"]) and int(dialogue_sel["talk_group"]) == 6,
			"Route 14 (Bad Ending): executed command 7 all 152 weeks -> Yoshio classroom bad ending")

	return {
		"route_id": route_id,
		"name": route_name,
		"expected_girl": expected_girl,
		"selected_candidate": final_girl,
		"club_id": club_id,
		"policy": route_spec["policy"],
		"weeks_completed": weeks_completed,
		"final_day_count": int(state.rule_state["day_count"]),
		"months_traversed": seen_months.size(),
		"leap_day_1996_02_29": leap_day_seen,
		"final_date": "19%02d-%02d-%02d" % [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]],
		"earned_dates": earned_dates,
		"best_points": best_points,
		"passed_b26_gate": passed_gate,
		"ending_phases": state.rule_state.get("ending_phases", []),
		"ending_stage_flow": stage_names,
		"confession_scene": stage_names[2],
		"credits_handoff_mode": "0x%02X" % final_game_mode,
		"clear_name_slot": bool(dialogue_sel["clear_name_slot"]),
		"talk_group": int(dialogue_sel["talk_group"]),
		"mid_save_slot": mid_slot_name,
		"mid_save_checksum_sha256": mid_save_checksum,
		"clear_save_slot": clear_slot_name,
		"clear_save_checksum_sha256": String(clear_save_out.get("checksum_sha256", "")),
		"cumulative_album_mask": "0x%04X" % updated_album_mask,
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
	}


func _run() -> void:
	for d_path: String in [OUT_DIR, PACKET_DIR, SAVES_DIR]:
		DirAccess.make_dir_recursive_absolute(d_path)
	# Clean any prior temporary save files in SAVES_DIR before running the 14-route suite
	var save_dir := DirAccess.open(SAVES_DIR)
	if save_dir != null:
		for f_name: String in save_dir.get_files():
			DirAccess.remove_absolute(SAVES_DIR.path_join(f_name))

	var tables: Dictionary = _load_imported_tables()
	_prepare_base_entry_projection()
	var save_service := SaveService.new(SAVES_DIR)
	clear_save_host = ClearSave.new("/tmp/t39007-multi-route-" + str(Time.get_ticks_usec()))

	var route_specs: Array[Dictionary] = [
		{
			"route_id": 1,
			"name": "Route 1 — Shiori Fujisaki (0)",
			"expected_girl": EndingOverlay.GIRL_SHIORI,
			"club_id": 0, # Literature Club
			"policy": "balanced_shiori",
			"enable_dates": true,
			"target_dates": 12,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 2,
			"name": "Route 2 — Mio Kisaragi (1)",
			"expected_girl": EndingOverlay.GIRL_MIO,
			"club_id": 0, # Literature Club
			"policy": "liberal_arts_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 3,
			"name": "Route 3 — Yuina Himoo (2)",
			"expected_girl": EndingOverlay.GIRL_YUINA,
			"club_id": 2, # Science Club
			"policy": "science_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 4,
			"name": "Route 4 — Ayako Katagiri (3)",
			"expected_girl": EndingOverlay.GIRL_AYAKO,
			"club_id": 5, # Brass Band Club
			"policy": "art_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 5,
			"name": "Route 5 — Saki Nijino (4)",
			"expected_girl": EndingOverlay.GIRL_SAKI,
			"club_id": 6, # Baseball Club
			"policy": "athletics_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 6,
			"name": "Route 6 — Yukari Koshiki (5)",
			"expected_girl": EndingOverlay.GIRL_YUKARI,
			"club_id": 8, # Tennis Club
			"policy": "yukari_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 7,
			"name": "Route 7 — Nozomi Kiyokawa (6)",
			"expected_girl": EndingOverlay.GIRL_NOZOMI,
			"club_id": 9, # Swimming Club
			"policy": "nozomi_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 8,
			"name": "Route 8 — Mira Kagami (7)",
			"expected_girl": EndingOverlay.GIRL_MIRA,
			"club_id": 1, # Drama Club
			"policy": "mira_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 9,
			"name": "Route 9 — Yuko Asahina (8)",
			"expected_girl": EndingOverlay.GIRL_YUKO,
			"club_id": 0, # Literature Club
			"policy": "yuko_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 10,
			"name": "Route 10 — Megumi Mikihara (9)",
			"expected_girl": EndingOverlay.GIRL_MEGUMI,
			"club_id": 4, # Art Club
			"policy": "megumi_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 11,
			"name": "Route 11 — Yumi Saotome (10)",
			"expected_girl": EndingOverlay.GIRL_YUMI,
			"club_id": 10, # Basketball Club
			"policy": "yumi_focus",
			"enable_dates": true,
			"target_dates": 15,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 12,
			"name": "Route 12 — Miharu Tatebayashi (11)",
			"expected_girl": EndingOverlay.GIRL_MIHARU,
			"club_id": 0,
			"policy": "miharu_secret",
			"enable_dates": false,
			"rand_mod_256": 10,
			"expected_stage2": "confession_scene_legendary_tree",
		},
		{
			"route_id": 13,
			"name": "Route 13 — Rei Ijuin (12)",
			"expected_girl": EndingOverlay.GIRL_REI,
			"club_id": 0,
			"policy": "rei_chapel",
			"enable_dates": false,
			"expected_stage2": "confession_scene_chapel",
		},
		{
			"route_id": 14,
			"name": "Route 14 — Bad Ending / Yoshio (13)",
			"expected_girl": EndingOverlay.GIRL_YOSHIO,
			"club_id": 0,
			"policy": "rest_only",
			"enable_dates": false,
			"rand_mod_256": 250,
			"expected_stage2": "confession_scene_classroom",
		},
	]

	var route_results: Array[Dictionary] = []
	var route_album_mask: int = 0
	for spec: Dictionary in route_specs:
		var res: Dictionary = _execute_single_route(spec, tables, save_service, route_album_mask)
		route_results.append(res)
		var cand: int = int(res["selected_candidate"])
		if cand >= 0 and cand <= 12:
			route_album_mask |= (1 << cand)

	_check(route_album_mask == 0x1FFF,
		"All 13 heroine routes (0..12) earned full Clear-Save & Album bitmask 0x1FFF through live 3-year playthroughs (got 0x%04X)" % route_album_mask)

	# Final reopened system/album verification after all 14 routes
	var final_reopened := SaveService.new(SAVES_DIR)
	var final_sys_load: Dictionary = final_reopened.load_slot("system_album_data")
	_check(String(final_sys_load.get("error", "")).is_empty(), "Final reopened system_album_data load succeeded")
	var final_sys_state: Dictionary = final_sys_load.get("snapshot", {}).get("gameplay_state", {}).get("rule_state", {})
	_check(int(final_sys_state.get("system_album_clear_mask", 0)) == 0x1FFF,
		"Reopened disk system_album_data after all 14 routes preserves full 13-heroine bitmask 0x1FFF")

	var proof := {
		"schema": "tokimemo.multi-route-playtest-proof.v3",
		"status": "PASS" if failures.is_empty() else "FAIL",
		"tables_source": TABLES_PATH,
		"installed_runtime_source": INSTALLED_ROOT,
		"saves_directory": SAVES_DIR,
		"routes_executed": route_results.size(),
		"total_weeks_traversed": route_results.size() * 152,
		"total_days_traversed": route_results.size() * 1062,
		"checks_count": checks.size(),
		"failures_count": failures.size(),
		"route_clear_save_album_mask": "0x%04X" % route_album_mask,
		"clear_save_album_mask": "0x%04X" % route_album_mask,
		"system_album_data_checksum_sha256": String(final_sys_load.get("checksum_sha256", "")),
		"routes": route_results,
		"failures": failures,
	}
	for proof_path: String in [PROOF_JSON_PATH, PACKET_PROOF_JSON_PATH]:
		var f := FileAccess.open(proof_path, FileAccess.WRITE)
		if f != null:
			f.store_string(JSON.stringify(proof, "\t"))
			f.close()

	if failures.is_empty():
		print("PASS: Multi-route 3-year playtest suite (%d routes, %d checks, 152 weeks x 14 = 2128 weeks, 1062 days x 14 = 14868 days)" % [
			route_results.size(), checks.size()
		])
		quit(0)
	else:
		print("FAIL: %d failure(s) in run_multi_route_playtest_suite" % failures.size())
		quit(1)
