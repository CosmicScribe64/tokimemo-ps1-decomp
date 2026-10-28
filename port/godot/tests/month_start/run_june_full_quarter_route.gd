extends SceneTree
## T-29023 / T-29032 / G2: Full first-quarter June continuation verification
## (`1995-05-28` -> `1995-06-01` quarterly 0x38 transition -> `1995-06-03` Saturday
## Sports Festival 0x72 -> all four June Sundays `1995-06-04`, `1995-06-11`,
## `1995-06-18`, `1995-06-25` -> all June weekday activities through `1995-06-30`),
## 320x240 summer HUD PNG captures, disk save + fresh restart/reload matching
## continuation (`1995-06-04` -> `1995-06-30`), and source/native short cases
## for all 5 G2 month/quarter/September/year-rollover/vacation predicates.

const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const State = preload("res://rules/school_day_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const HolidayController = preload("res://scenes/holiday_controller.gd")
const MonthStart = preload("res://scenes/month_start_controller.gd")
const CalendarSource = preload("res://services/native_calendar_source.gd")
const BankRequest = preload("res://services/month_start_bank_request.gd")
const Calendar = preload("res://rules/calendar.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const SaveService = preload("res://services/save_service.gd")
const AudioService = preload("res://services/audio_service.gd")
const MediaStore = preload("res://services/school_day_media_store.gd")
const ExactMedia = preload("res://services/native_activity_media.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const HudPresenter = preload("res://presentation/command_hud_presenter.gd")
const SchoolEventsPresenter = preload("res://presentation/school_events_presenter.gd")
const Bridge = preload("res://services/month_start_audio_bridge.gd")
const NativeBackend = preload("res://bgm_backend.gd")
const NativeSink = preload("res://bgm_sink.gd")
const NativeService = preload("res://services/shared_month_audio_service.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")

const EXPECTED_LIB_SHA: String = "f1eecf80c1c6538a33f6bee9c7d10a79553b1745d520688f556bbe60fb1687b9"
const EXPECTED_MAY28_HASH: String = "ad782763f6f6cccc6fcd504ce06cb60f44d4f5f104d07d867621dd92d39aae45"
const EXPECTED_JUNE1_SCHEDULE_RNG: int = 2966941936
const EXPECTED_HUD_SHA: String = "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"
const PAD_CONFIRM: int = 0x20

class PredicateAudioOwner extends AudioService:
	## Short-case protocol owner for G2 quarterly/September predicate verification.
	var ready: bool = false
	var generation: int = 1
	var epoch: int = 2
	var started: bool = false
	var published_batches: Array[Dictionary] = []

	func month_start_prepare(_payload: Dictionary) -> Dictionary:
		return {"schema": Bridge.VALIDATION, "error": "", "intent_sha256": Bridge.digest(_payload)}

	func month_start_discard(frame: int) -> Dictionary:
		return {"schema": Bridge.VALIDATION, "error": "", "frame_count": frame}

	func month_start_operation(payload: Dictionary) -> Variant:
		var context: Dictionary = {
			"profile_sha256": "f".repeat(64),
			"binding_sha256": Bridge.digest(payload["request"]["binding"]),
			"native_state_sha256": "G2_PREDICATE_AUDIO_OWNER".sha256_text(),
			"raw_reverb_depth": 56,
		}
		var status: int = (1 if ready else -1) if payload["operation"] == "poll" else 0
		return {
			"schema": Bridge.PROTOCOL,
			"error": "",
			"request_sha256": Bridge.digest(payload),
			"owner": {"session_generation": generation, "bank_epoch": epoch, "request_serial": 7},
			"token": Bridge.digest(payload),
			"status": status,
			"source_context": context,
		}

	func month_start_validate(intent: Dictionary, phase: String) -> Dictionary:
		var error: String = ""
		if phase == "active" and not started:
			error = "no started model context"
		return {"schema": Bridge.VALIDATION, "error": error, "intent_sha256": Bridge.digest(intent)}

	func month_start_validate_batch(intents: Array[Dictionary], projection: Dictionary, phase: String) -> Dictionary:
		var payload: Dictionary = {"intents": intents, "final_projection": projection, "phase": phase}
		return {"schema": Bridge.VALIDATION, "error": "", "intent_sha256": Bridge.digest(payload)}

	func month_start_publish_batch(intents: Array[Dictionary], projection: Dictionary) -> Dictionary:
		var payload: Dictionary = {"intents": intents, "final_projection": projection}
		published_batches.append(payload.duplicate(true))
		for intent: Dictionary in intents:
			event_history.append(intent.duplicate(true))
			if intent["operation"] == "start":
				started = true
		return {"schema": Bridge.VALIDATION, "error": "", "intent_sha256": Bridge.digest(payload)}

class PredicateNoticeOwner extends RefCounted:
	var history: Array[Dictionary] = []

	func month_start_notice(payload: Dictionary) -> Dictionary:
		return {
			"schema": "tokimemo.month-notice.v1",
			"error": "",
			"request_sha256": Bridge.digest(payload),
			"token": Bridge.digest(payload),
			"source_sha256": "e".repeat(64),
		}

	func month_start_notice_validate(intent: Dictionary, _phase: String) -> Dictionary:
		return {"schema": "tokimemo.month-notice-validation.v1", "error": "", "intent_sha256": Bridge.digest(intent)}

	func month_start_notice_publish(intent: Dictionary) -> Dictionary:
		history.append(intent.duplicate(true))
		return {"schema": "tokimemo.month-notice-validation.v1", "error": "", "intent_sha256": Bridge.digest(intent)}

var checks: int = 0
var failures: Array[String] = []
var native_service: NativeService
var native_backend: NativeBackend
var native_sink: NativeSink
var first_owner: Dictionary = {}

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures.append(label)
		push_error(label)

static func _originals(disc_root: String, summer: bool = false) -> Array[PackedByteArray]:
	var names: Array = [
		"SLPM_86.053",
		"CDROM/VAB_SE/COM_MIX.VH",
		"CDROM/VAB_SE/COM_MIX.VB",
		"CDROM/VAB_SEQ/SUMMER.VH",
		"CDROM/VAB_SEQ/SUMMER.VB",
		"CDROM/VAB_SEQ/SUMMER.SEQ",
	] if summer else [
		"SLPM_86.053",
		"CDROM/VAB_SE/SYS_MIX.VH",
		"CDROM/VAB_SE/SYS_MIX.VB",
		"CDROM/VAB_SEQ/PRO_VOL.VH",
		"CDROM/VAB_SEQ/PRO_VOL.VB",
		"CDROM/VAB_SEQ/PRO_VOL.SEQ",
	]
	var inputs: Array[PackedByteArray] = []
	for name: String in names:
		inputs.append(FileAccess.get_file_as_bytes(disc_root.path_join(name)))
	return inputs

static func _integers(value: Variant) -> Variant:
	if value is float:
		return int(value)
	if value is Array:
		var out: Array = []
		for item: Variant in value:
			out.append(_integers(item))
		return out
	if value is Dictionary:
		var out: Dictionary = {}
		for k: Variant in value:
			out[_integers(k)] = _integers(value[k])
		return out
	return value

static func _build_expected_dates() -> Array[String]:
	var out: Array[String] = ["95-05-28", "95-05-29", "95-05-30", "95-05-31"]
	for d: int in range(1, 31):
		out.append("95-06-%02d" % d)
	return out

static func _build_expected_june4_to_june30_dates() -> Array[String]:
	var out: Array[String] = []
	for d: int in range(4, 31):
		out.append("95-06-%02d" % d)
	return out

static func _calrom_record(cal_source: CalendarSource, year: int, month: int, tail: int) -> Dictionary:
	var original: Dictionary = cal_source.month(year, month)
	var record: Dictionary = {}
	for key: String in ["day_kind", "marker"]:
		var row: Array = []
		row.resize(32)
		row.fill(tail)
		for index: int in range(original[key].size()):
			row[index] = original[key][index]
		record[key] = row
	return record

static func _make_month_start_state(
	template_state: Dictionary,
	cal_source: CalendarSource,
	year: int,
	month: int,
	vacation: int = 0
) -> State:
	var st := State.new()
	st.restore(template_state.duplicate(true))
	st.game_mode = 0x38
	st.mode_stage = 0
	st.mode_step = 0
	st.substep = 0
	st.frame_count = 117
	st.sound_active = true
	var data: Dictionary = st.rule_state
	data["year"] = year
	data["month"] = month
	data["day"] = 1
	data["weekday"] = int(cal_source.month(year, month)["marker"][0])
	data["vacation"] = vacation
	data["actor_state"] = {}
	data["day_event_index"] = 7
	data["day_events"][7] = [0xfe, 0, 0, 0]
	data["day_events"][8] = [0, 0, 0, 15]
	data["resources"] = {MonthStart.SOURCE_KEY: CalendarSource.SOURCE_SHA}
	var prev_year: int = year - 1 if month == 1 else year
	var prev_month: int = 12 if month == 1 else month - 1
	data["calendar"] = [
		_calrom_record(cal_source, prev_year, prev_month, 173),
		_calrom_record(cal_source, year, month, 219),
	]
	return st

static func _step_session(sess: Session, events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	return sess.step(InputFrame.new(sess.clock.logic_iterations), events)

func _verify_g2_predicates(
	calendar_path: String,
	tables: Dictionary,
	template_state: Dictionary
) -> Dictionary:
	var cal_source := CalendarSource.new()
	var open_err: String = cal_source.open_source(calendar_path)
	check(open_err.is_empty() and cal_source.identity() == CalendarSource.SOURCE_SHA, "G2 predicates: CALROM.NBN source opens with exact SHA-256")

	var summary: Dictionary = {
		"ordinary_months": [],
		"quarterly_months": [],
		"september_notices": [],
		"year_rollovers": [],
		"vacation_banks": [],
	}

	var shared_ctrl := Controller.new(tables)
	check(shared_ctrl.error.is_empty(), "G2 predicates: shared SchoolDayController initializes with original tables")
	check(shared_ctrl.month_start.open_source(calendar_path).is_empty(), "G2 predicates: MonthStartController opens CALROM.NBN")

	# (a) Ordinary month transitions (May 1 95-05-01 and July 1 95-07-01)
	for pair: Array in [[95, 5], [95, 7]]:
		var y: int = int(pair[0])
		var m: int = int(pair[1])
		shared_ctrl.month_start.reset_services()
		check(MonthStart.branch_for(y, m, 1) == "ordinary", "G2 predicate (a): %02d-%02d-01 branch_for is ordinary" % [y, m])
		var st: State = _make_month_start_state(template_state, cal_source, y, m, 0)
		var sess := Session.new()
		sess.rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(shared_ctrl.attach(sess, st).is_empty(), "G2 predicate (a): Controller attaches for %02d-%02d" % [y, m])
		sess.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = HudSeason.draw_step(base as State, result)
			if result.error.is_empty():
				result.error = shared_ctrl.month_start.stage_services(base as State, result)["error"]
		sess.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = shared_ctrl.month_start.precommit_services(base as State, result)
		sess.audio_logic = sess.cd_logic

		var rng_before: Dictionary = sess.rng.checkpoint()
		var req_res: Session.FrameResult = _step_session(sess)
		check(req_res.committed and st.substep == 1, "G2 predicate (a): %02d-%02d-01 substep 0 issues CALROM read request" % [y, m])
		var payload: Dictionary = st.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
		payload["ready"] = true
		var ev := Session.ServiceEvent.new(sess.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, MonthStart.SERVICE, payload)
		var up_res: Session.FrameResult = _step_session(sess, [ev])
		check(up_res.committed and st.mode_stage == 1 and up_res.audio.is_empty(), "G2 predicate (a): %02d-%02d-01 ordinary upload rotates calendar without BGM load" % [y, m])
		var next_expected: Dictionary = cal_source.month(y, m + 1)
		check(st.rule_state["calendar"][1]["day_kind"].slice(0, next_expected["day_kind"].size()) == next_expected["day_kind"], "G2 predicate (a): %02d-%02d-01 uploads exact next month %02d-%02d day_kind" % [y, m, y, m + 1])
		var ex_res: Session.FrameResult = _step_session(sess)
		check(ex_res.committed and st.game_mode == 0x31 and int(st.rule_state["day_event_index"]) == 8 and sess.rng.checkpoint() == rng_before, "G2 predicate (a): %02d-%02d-01 exits 0x38->0x31 with zero RNG consumption" % [y, m])
		summary["ordinary_months"].append("%02d-%02d-01" % [y, m])

	# (b) Quarterly transitions (June 1, Sept 1, Dec 1, March 1 seasonal COM_MIX + SPRING/SUMMER/AUTUMN/WINTER + HUD fade 0xef->0xf0)
	var quarter_specs: Array = [
		{"year": 95, "month": 6, "season": 1, "bank": [0xd4e7, 0xd49e, 0xd488], "notice": false, "label": "SUMMER"},
		{"year": 95, "month": 9, "season": 2, "bank": [0xc70b, 0xc6c3, 0xc6b6], "notice": true, "label": "AUTUMN"},
		{"year": 95, "month": 12, "season": 3, "bank": [0xd8cb, 0xd87e, 0xd874], "notice": false, "label": "WINTER"},
		{"year": 96, "month": 3, "season": 0, "bank": [0xd3d5, 0xd387, 0xd37d], "notice": false, "label": "SPRING"},
	]
	for spec: Dictionary in quarter_specs:
		var y: int = int(spec["year"])
		var m: int = int(spec["month"])
		shared_ctrl.month_start.reset_services()
		var st: State = _make_month_start_state(template_state, cal_source, y, m, 0)
		var req_Preview: Dictionary = MonthStart.bank_request(st.rule_state)
		check(req_Preview.get("season") == spec["season"], "G2 predicate (b): %02d-%02d-01 (%s) season selector is %d" % [y, m, spec["label"], spec["season"]])
		check(
			[req_Preview["binding"]["se_header"], req_Preview["binding"]["se_body"]] == [0xbf98, 0xbf79]
			and [req_Preview["binding"]["music_header"], req_Preview["binding"]["music_body"], req_Preview["binding"]["sequence"]] == spec["bank"],
			"G2 predicate (b): %02d-%02d-01 (%s) binds COM_MIX (0xbf98/0xbf79) + %s bank %s" % [y, m, spec["label"], spec["label"], str(spec["bank"])]
		)
		var audio_owner := PredicateAudioOwner.new()
		check(shared_ctrl.month_start.audio.bind_owner(audio_owner).is_empty(), "G2 predicate (b): audio owner binds for %02d-%02d" % [y, m])
		var notice_owner := PredicateNoticeOwner.new()
		if bool(spec["notice"]):
			check(shared_ctrl.month_start.bind_notice_owner(notice_owner).is_empty(), "G2 predicate (b): notice owner binds for %02d-%02d" % [y, m])
		var sess := Session.new()
		sess.rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(shared_ctrl.attach(sess, st).is_empty(), "G2 predicate (b): Controller attaches for %02d-%02d" % [y, m])
		sess.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = HudSeason.draw_step(base as State, result)
			if result.error.is_empty():
				result.error = shared_ctrl.month_start.stage_services(base as State, result)["error"]
		sess.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = shared_ctrl.month_start.precommit_services(base as State, result)
		sess.audio_logic = sess.cd_logic

		# Step 1: request CALROM
		var f0: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (b): prepare_services substep 0 for %02d-%02d" % [y, m])
		var r0: Session.FrameResult = _step_session(sess)
		check(r0.committed and shared_ctrl.month_start.publish_services(r0).is_empty() and shared_ctrl.month_start.discard_services(f0).is_empty(), "G2 predicate (b): %02d-%02d request frame commits" % [y, m])

		# Align frame_count so upload frame has (old_frame & 3) == 3, advancing 0xef -> 0xf0 in HudSeason.draw_step
		st.frame_count = 119
		var f1: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (b): prepare_services upload frame for %02d-%02d" % [y, m])
		var payload: Dictionary = st.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
		payload["ready"] = true
		var ev := Session.ServiceEvent.new(sess.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, MonthStart.SERVICE, payload)
		var r1: Session.FrameResult = _step_session(sess, [ev])
		check(r1.committed and shared_ctrl.month_start.publish_services(r1).is_empty() and shared_ctrl.month_start.discard_services(f1).is_empty(), "G2 predicate (b): %02d-%02d upload + BGM load commits" % [y, m])
		check(int(st.rule_state["resources"].get(HudSeason.KEY, -1)) == 0xf0, "G2 predicate (b): %02d-%02d-01 initializes HUD fade 0xef->0xf0 on eligible frame" % [y, m])
		check(int(HudSeason.view(st.rule_state).get("checker_season", -1)) == int(spec["season"]), "G2 predicate (b): %02d-%02d-01 HudSeason.view checker_season is %d" % [y, m, spec["season"]])

		# Step 3: ready poll + start
		audio_owner.ready = true
		var f2: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (b): prepare_services ready poll/start for %02d-%02d" % [y, m])
		var r2: Session.FrameResult = _step_session(sess)
		check(r2.committed and st.mode_stage == 1 and r2.audio.size() == 2 and r2.audio[0]["operation"] == "poll" and r2.audio[1]["operation"] == "start", "G2 predicate (b): %02d-%02d-01 ready poll + start commits" % [y, m])
		check(shared_ctrl.month_start.publish_services(r2).is_empty() and shared_ctrl.month_start.discard_services(f2).is_empty(), "G2 predicate (b): %02d-%02d-01 poll/start batch publishes" % [y, m])

		# Step 4: exit 0x38 -> 0x31
		var f3: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (b): prepare_services exit for %02d-%02d" % [y, m])
		var r3: Session.FrameResult = _step_session(sess)
		check(r3.committed and st.game_mode == 0x31 and int(st.rule_state["day_event_index"]) == 8 and shared_ctrl.month_start.publish_services(r3).is_empty() and shared_ctrl.month_start.discard_services(f3).is_empty(), "G2 predicate (b): %02d-%02d-01 exits 0x38 -> 0x31" % [y, m])
		summary["quarterly_months"].append("%02d-%02d-01:%s" % [y, m, spec["label"]])

	# (c) September notice predicates (95-09-01 and 97-09-01 notice + amusement reset; 96-09-01 skip + amusement reset)
	for sep_year: int in [95, 96, 97]:
		shared_ctrl.month_start.reset_services()
		var expected_branch: String = "september96_skip" if sep_year == 96 else "september_notice"
		check(MonthStart.branch_for(sep_year, 9, 1) == expected_branch, "G2 predicate (c): %02d-09-01 branch_for is %s" % [sep_year, expected_branch])
		var st: State = _make_month_start_state(template_state, cal_source, sep_year, 9, 0)
		st.rule_state["player_flags_upper"] |= 4
		var audio_owner := PredicateAudioOwner.new()
		var notice_owner := PredicateNoticeOwner.new()
		if sep_year != 96:
			check(shared_ctrl.month_start.audio.bind_owner(audio_owner).is_empty(), "G2 predicate (c): audio owner binds for %02d-09-01" % sep_year)
			check(shared_ctrl.month_start.bind_notice_owner(notice_owner).is_empty(), "G2 predicate (c): notice owner binds for %02d-09-01" % sep_year)
		var sess := Session.new()
		sess.rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(shared_ctrl.attach(sess, st).is_empty(), "G2 predicate (c): Controller attaches for %02d-09-01" % sep_year)
		sess.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = HudSeason.draw_step(base as State, result)
			if result.error.is_empty():
				result.error = shared_ctrl.month_start.stage_services(base as State, result)["error"]
		sess.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = shared_ctrl.month_start.precommit_services(base as State, result)
		sess.audio_logic = sess.cd_logic

		var f0: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (c): prepare_services substep 0 for %02d-09-01" % sep_year)
		var r0: Session.FrameResult = _step_session(sess)
		check(r0.committed and shared_ctrl.month_start.publish_services(r0).is_empty() and shared_ctrl.month_start.discard_services(f0).is_empty(), "G2 predicate (c): %02d-09-01 request commits" % sep_year)

		var f1: int = st.frame_count
		check(shared_ctrl.month_start.prepare_services(st).is_empty(), "G2 predicate (c): prepare_services upload for %02d-09-01" % sep_year)
		var payload: Dictionary = st.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
		payload["ready"] = true
		var ev := Session.ServiceEvent.new(sess.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, MonthStart.SERVICE, payload)
		var r1: Session.FrameResult = _step_session(sess, [ev])
		check(r1.committed and shared_ctrl.month_start.publish_services(r1).is_empty() and shared_ctrl.month_start.discard_services(f1).is_empty(), "G2 predicate (c): %02d-09-01 upload commits" % sep_year)
		check(( int(st.rule_state["player_flags_upper"]) & 4 ) == 0, "G2 predicate (c): %02d-09-01 clears amusement bit (player_flags_upper & 4)" % sep_year)
		if sep_year == 96:
			check(st.mode_stage == 1 and r1.audio.is_empty() and notice_owner.history.is_empty(), "G2 predicate (c): 96-09-01 skips notice and BGM load and enters stage 1 directly")
			check(BankRequest.build({"year": 96, "month": 9, "day": 1, "vacation": 0})["vacation"] == 1, "G2 predicate (c): 96-09-01 BankRequest.build applies vacation=1 override")
		else:
			check(
				notice_owner.history.size() == 1
				and notice_owner.history[0]["payload"]["date"] == [sep_year, 9, 1]
				and notice_owner.history[0]["payload"]["calls"] == ["text_system_reset", "message_window_set_title_hidden", "message_window_title_show", "gnsx", "sndisp", "k_disp_start"],
				"G2 predicate (c): %02d-09-01 publishes exact September term-start notice receipt" % sep_year
			)
		summary["september_notices"].append("%02d-09-01:%s" % [sep_year, expected_branch])

	# (d) Year rollover predicates:
	#     1. Calendar year rollovers (95-12-31 -> 96-01-01, 96-12-31 -> 97-01-01) + CALROM upload (including 96 leap-year Feb 29 vs 97 Feb 28)
	#     2. School-year April rollovers (96-04-01 Year 2 and 97-04-01 Year 3 CALROM upload + April 4 marker==2 class/grade promotion queue events 0x48 / 0x43)
	for dec_year: int in [95, 96]:
		shared_ctrl.month_start.reset_services()
		var st_dec: State = _make_month_start_state(template_state, cal_source, dec_year, 12, 2)
		st_dec.rule_state["day"] = 31
		st_dec.rule_state["weekday"] = (int(cal_source.month(dec_year, 12)["marker"][0]) + 30) % 7
		st_dec.rule_state["calendar"] = [
			_calrom_record(cal_source, dec_year, 12, 173),
			_calrom_record(cal_source, dec_year + 1, 1, 219),
		]
		check(Calendar.advance_day(st_dec).is_empty(), "G2 predicate (d): Calendar.advance_day rolls %02d-12-31 -> %02d-01-01" % [dec_year, dec_year + 1])
		check(
			[st_dec.rule_state["year"], st_dec.rule_state["month"], st_dec.rule_state["day"], st_dec.rule_state["vacation"]] == [dec_year + 1, 1, 1, 2],
			"G2 predicate (d): %02d-01-01 has year=%d month=1 day=1 vacation=2" % [dec_year + 1, dec_year + 1]
		)
		var sess_jan := Session.new()
		sess_jan.rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(shared_ctrl.attach(sess_jan, st_dec).is_empty(), "G2 predicate (d): Controller attaches for %02d-01-01" % (dec_year + 1))
		sess_jan.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = shared_ctrl.month_start.stage_services(base as State, result)["error"]
		sess_jan.cd_logic = func(_base: CoreState, _rng: Rng, _result: Session.FrameResult) -> void:
			pass
		sess_jan.audio_logic = sess_jan.cd_logic
		check(_step_session(sess_jan).committed, "G2 predicate (d): %02d-01-01 substep 0 requests CALROM" % (dec_year + 1))
		var jan_payload: Dictionary = st_dec.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
		jan_payload["ready"] = true
		var jan_ev := Session.ServiceEvent.new(sess_jan.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, MonthStart.SERVICE, jan_payload)
		check(_step_session(sess_jan, [jan_ev]).committed, "G2 predicate (d): %02d-01-01 uploads next-year Feb CALROM record" % (dec_year + 1))
		var expected_feb_days: int = 29 if (dec_year + 1) == 96 else 28
		check(int(st_dec.rule_state["calendar"][1]["day_kind"][0]) == expected_feb_days, "G2 predicate (d): %02d-02 CALROM day count is %d" % [dec_year + 1, expected_feb_days])
		summary["year_rollovers"].append("%02d-12-31->%02d-01-01" % [dec_year, dec_year + 1])

	for apr_year: int in [96, 97]:
		shared_ctrl.month_start.reset_services()
		var st_apr: State = _make_month_start_state(template_state, cal_source, apr_year, 3, 3)
		st_apr.rule_state["year"] = apr_year
		st_apr.rule_state["month"] = 3
		st_apr.rule_state["day"] = 31
		st_apr.rule_state["weekday"] = (int(cal_source.month(apr_year, 3)["marker"][0]) + 30) % 7
		st_apr.rule_state["calendar"] = [
			_calrom_record(cal_source, apr_year, 3, 173),
			_calrom_record(cal_source, apr_year, 4, 219),
		]
		check(Calendar.advance_day(st_apr).is_empty(), "G2 predicate (d): Calendar.advance_day advances %02d-03-31 -> %02d-04-01" % [apr_year, apr_year])
		check(
			[st_apr.rule_state["year"], st_apr.rule_state["month"], st_apr.rule_state["day"], st_apr.rule_state["vacation"]] == [apr_year, 4, 1, 3],
			"G2 predicate (d): %02d-04-01 enters school year with vacation=3 (spring vacation through April 4)" % apr_year
		)
		var sess_apr := Session.new()
		sess_apr.rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(shared_ctrl.attach(sess_apr, st_apr).is_empty(), "G2 predicate (d): Controller attaches for %02d-04-01" % apr_year)
		sess_apr.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
			result.error = shared_ctrl.month_start.stage_services(base as State, result)["error"]
		sess_apr.cd_logic = func(_base: CoreState, _rng: Rng, _result: Session.FrameResult) -> void:
			pass
		sess_apr.audio_logic = sess_apr.cd_logic
		check(_step_session(sess_apr).committed, "G2 predicate (d): %02d-04-01 substep 0 requests CALROM" % apr_year)
		var apr_payload: Dictionary = st_apr.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
		apr_payload["ready"] = true
		var apr_ev := Session.ServiceEvent.new(sess_apr.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, MonthStart.SERVICE, apr_payload)
		check(_step_session(sess_apr, [apr_ev]).committed, "G2 predicate (d): %02d-04-01 promotes April CALROM and uploads May CALROM" % apr_year)
		check(int(st_apr.rule_state["calendar"][0]["marker"][4]) == 2, "G2 predicate (d): %02d-04-04 CALROM marker is 2 (new school year ceremony / class assignment)" % apr_year)
		# Verify Scheduler.build_week for the week containing April 4 in Year 2 (96-04-04 -> 0x48) and Year 3 (97-04-04 -> 0x43)
		st_apr.rule_state["allow_multi_week"] = true
		st_apr.rule_state["day"] = 1
		st_apr.rule_state["weekday"] = Calendar.weekday_for_date(apr_year, 4, 1)
		st_apr.rule_state["last_command"] = 0
		var rng_apr := Rng.new()
		rng_apr.configure(Rng.OPENBIOS_PROFILE, 19950404)
		check(Scheduler.build_week(st_apr, tables, rng_apr).is_empty(), "G2 predicate (d): Scheduler.build_week succeeds for %02d-04-04 week" % apr_year)
		var expected_ceremony_code: int = 0x48 if apr_year == 96 else 0x43
		var found_ceremony: bool = false
		var found_fd: bool = false
		for ev_rec: Array in st_apr.rule_state["day_events"]:
			if int(ev_rec[0]) == 0xfd:
				found_fd = true
			if int(ev_rec[0]) == expected_ceremony_code:
				found_ceremony = true
		check(found_fd and found_ceremony, "G2 predicate (d): %02d-04-04 queue contains 0xfd pre-marker and 0x%02x school-year rollover event" % [apr_year, expected_ceremony_code])
		summary["year_rollovers"].append("%02d-04-01:0x%02x" % [apr_year, expected_ceremony_code])

	# (e) Vacation bank selection predicates:
	#     - Summer vacation (CALROM 95-07: marker[10]==5 exams, marker[24]==6 closing ceremony, day_kind[24]==3; Calendar July 19->20 vacation=1, bank [0xd7d1, 0xd7af, 0xd7a4] -> Sept 1 vacation=0)
	#     - Winter vacation (CALROM 95-12: marker[24]==0x0d Christmas Eve, marker[25]==0x0e closing ceremony, day_kind[25]==3; 96-01 marker[8]==0x10; Calendar Dec 24->25 vacation=2, bank [0xd73f, 0xd71e, 0xd717] -> Jan 8 vacation=0)
	#     - Spring vacation (CALROM 96-03: marker[24]==0x14 closing ceremony, day_kind[20]==2, day_kind[25]==3; Calendar March 19->20 vacation=3, bank [0xd780, 0xd752, 0xd743] -> April 5 vacation=0)
	var july95: Dictionary = cal_source.month(95, 7)
	var dec95: Dictionary = cal_source.month(95, 12)
	var jan96: Dictionary = cal_source.month(96, 1)
	var mar96: Dictionary = cal_source.month(96, 3)
	check(int(july95["marker"][10]) == 5 and int(july95["marker"][24]) == 6 and int(july95["day_kind"][24]) == 3, "G2 predicate (e): CALROM 95-07 has exam marker 5, closing ceremony marker 6, and vacation day_kind 3")
	check(int(dec95["marker"][24]) == 0x0d and int(dec95["marker"][25]) == 0x0e and int(dec95["day_kind"][25]) == 3 and int(jan96["marker"][8]) == 0x10, "G2 predicate (e): CALROM 95-12-24..25 and 96-01-08 carry exact winter vacation markers/day_kind")
	check(int(mar96["marker"][24]) == 0x14 and int(mar96["day_kind"][20]) == 2 and int(mar96["day_kind"][25]) == 3, "G2 predicate (e): CALROM 96-03-20..25 carry exact spring vacation markers/day_kind")

	var vac_cases: Array = [
		{
			"name": "summer_vacation",
			"before_date": [95, 7, 19],
			"start_date": [95, 7, 20],
			"end_before": [95, 8, 31],
			"end_after": [95, 9, 1],
			"vacation_id": 1,
			"expected_bank": [0xd7d1, 0xd7af, 0xd7a4],
		},
		{
			"name": "winter_vacation",
			"before_date": [95, 12, 24],
			"start_date": [95, 12, 25],
			"end_before": [96, 1, 7],
			"end_after": [96, 1, 8],
			"vacation_id": 2,
			"expected_bank": [0xd73f, 0xd71e, 0xd717],
		},
		{
			"name": "spring_vacation",
			"before_date": [96, 3, 19],
			"start_date": [96, 3, 20],
			"end_before": [96, 4, 4],
			"end_after": [96, 4, 5],
			"vacation_id": 3,
			"expected_bank": [0xd780, 0xd752, 0xd743],
		},
	]
	for vc: Dictionary in vac_cases:
		var b_date: Array = vc["before_date"]
		var s_date: Array = vc["start_date"]
		var eb_date: Array = vc["end_before"]
		var ea_date: Array = vc["end_after"]
		var st_vac: State = _make_month_start_state(template_state, cal_source, int(b_date[0]), int(b_date[1]), 0)
		st_vac.rule_state["day"] = int(b_date[2])
		st_vac.rule_state["weekday"] = Calendar.weekday_for_date(int(b_date[0]), int(b_date[1]), int(b_date[2]))
		Calendar.update_vacation_for_date(st_vac.rule_state)
		check(int(st_vac.rule_state["vacation"]) == 0, "G2 predicate (e): %s before start (%s) has vacation=0" % [vc["name"], str(b_date)])
		check(Calendar.advance_day(st_vac).is_empty(), "G2 predicate (e): %s advances to start date %s" % [vc["name"], str(s_date)])
		check(
			[st_vac.rule_state["year"], st_vac.rule_state["month"], st_vac.rule_state["day"], st_vac.rule_state["vacation"]]
			== [int(s_date[0]), int(s_date[1]), int(s_date[2]), int(vc["vacation_id"])],
			"G2 predicate (e): %s start date %s selects vacation=%d" % [vc["name"], str(s_date), vc["vacation_id"]]
		)
		var bank_req: Dictionary = BankRequest.build(st_vac.rule_state)
		check(
			[bank_req["binding"]["music_header"], bank_req["binding"]["music_body"], bank_req["binding"]["sequence"]] == vc["expected_bank"],
			"G2 predicate (e): %s BankRequest selects %s" % [vc["name"], str(vc["expected_bank"])]
		)
		st_vac.rule_state["year"] = int(eb_date[0])
		st_vac.rule_state["month"] = int(eb_date[1])
		st_vac.rule_state["day"] = int(eb_date[2])
		st_vac.rule_state["weekday"] = Calendar.weekday_for_date(int(eb_date[0]), int(eb_date[1]), int(eb_date[2]))
		Calendar.update_vacation_for_date(st_vac.rule_state)
		check(int(st_vac.rule_state["vacation"]) == int(vc["vacation_id"]), "G2 predicate (e): %s last day %s retains vacation=%d" % [vc["name"], str(eb_date), vc["vacation_id"]])
		check(Calendar.advance_day(st_vac).is_empty(), "G2 predicate (e): %s advances past end date to %s" % [vc["name"], str(ea_date)])
		check(
			[st_vac.rule_state["year"], st_vac.rule_state["month"], st_vac.rule_state["day"], st_vac.rule_state["vacation"]]
			== [int(ea_date[0]), int(ea_date[1]), int(ea_date[2]), 0],
			"G2 predicate (e): %s end transition %s restores vacation=0" % [vc["name"], str(ea_date)]
		)
		summary["vacation_banks"].append("%s:%d" % [vc["name"], vc["vacation_id"]])

	return summary

func _compose_hud_capture(
	presenter: HudPresenter,
	rs: Dictionary,
	frame_count: int,
	bg_image: Image,
	actor_draw_commands: Array[Dictionary],
	text_runs: Array[Dictionary],
	command_room: bool
) -> Image:
	var params: Array[int] = []
	for word: Variant in rs["params"]:
		params.append(int(word) >> 16)
	var season: Dictionary = HudSeason.view(rs)
	var day: int = clampi(int(rs["day"]), 1, 31)
	var day_kind: int = int(rs["calendar"][0]["day_kind"][day])
	var selected: int = clampi(int(rs.get("menu_cursor", 0)), 0, 13)
	var view: Dictionary = {
		"year": int(rs["year"]),
		"month": int(rs["month"]),
		"day": day,
		"weekday": int(rs["weekday"]),
		"day_kind": day_kind,
		"parameters": params,
		"selected": selected,
		"icon_enabled": rs["icon_enabled"].slice(0, 14),
		"cursor_frame": 0,
		"icon_steps": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"icon_elapsed": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"checker_season": int(season.get("checker_season", 1)),
		"checker_fade": int(season.get("checker_fade", 255)),
		"parameter_colors": [0, 0, 0, 0, 0, 0, 0, 0, 0],
	}
	var health_mode: int = (int(rs.get("player_flags", 0)) >> 8) & 15
	presenter._compositor.set_checker_uv(
		HudCompositor.compute_checker_uv(frame_count, health_mode)
	)
	presenter._compositor.set_text_runs(text_runs)
	var scene_commands: Array[Dictionary] = []
	if not command_room and bg_image != null:
		scene_commands.append({
			"id": "capture-bg:left",
			"image": bg_image,
			"position": Vector2i(0, 72),
		})
	if not command_room:
		for cmd: Dictionary in actor_draw_commands:
			scene_commands.append(cmd)
	return presenter._compositor.render(view, null, scene_commands, command_room)

func _init() -> void:
	call_deferred("_run")

func _run() -> void:
	var original_root: String = OS.get_environment("ORIGINAL_FILES")
	if original_root.is_empty():
		original_root = "/workspace/disc/files"
	var calendar_path: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	if calendar_path.is_empty():
		calendar_path = original_root.path_join("CDROM/FUKUTEST/CALROM.NBN")
	var tables_path: String = OS.get_environment("TOKIMEMO_TABLES")
	if tables_path.is_empty():
		tables_path = "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json"
	var boundary_path: String = OS.get_environment("TOKIMEMO_BOUNDARY")
	if boundary_path.is_empty():
		boundary_path = "/workspace/.claude/orchestrator/state/T-29004/evidence/development-boundary.json"
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	if media_path.is_empty():
		media_path = "/workspace/.asset-cache/port/holiday-command0-current-964ce"
	var hud_manifest_path: String = OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	if hud_manifest_path.is_empty():
		hud_manifest_path = "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
	var font_root: String = OS.get_environment("TOKIMEMO_FONT_ROOT")
	if font_root.is_empty():
		font_root = "/workspace/.asset-cache/port/fonts/native-font-v2"
	var ref_path: String = OS.get_environment("TOKIMEMO_RESUME_REFERENCE")
	if ref_path.is_empty():
		ref_path = "/workspace/.claude/orchestrator/state/T-29023/runs/checkpoint-current-v2-through-june4/resume-reference.json"
	var captures_dir: String = OS.get_environment("TOKIMEMO_JUNE_CAPTURES")
	if captures_dir.is_empty():
		captures_dir = "/workspace/.claude/orchestrator/state/T-29023/june-full-month-continuation-v1/captures"
	var out_dir: String = OS.get_environment("TOKIMEMO_CALENDAR_OUTPUT")

	var descriptor: String = "res://original_audio.gdextension"
	check(FileAccess.get_sha256("res://bin/liboriginal_audio.linux.template_release.arm64.so") == EXPECTED_LIB_SHA, "warm V4 GDExtension library SHA matches f1eecf80...")
	if not GDExtensionManager.is_extension_loaded(descriptor):
		check(GDExtensionManager.load_extension(descriptor) == GDExtensionManager.LOAD_STATUS_OK, "explicit GDExtension bootstrap succeeds")
	check(ClassDB.class_exists("OriginalAudioEngine"), "OriginalAudioEngine GDExtension class is registered")

	var hud_presenter := HudPresenter.new()
	check(hud_presenter.open_manifest(hud_manifest_path, EXPECTED_HUD_SHA, font_root), "HudPresenter opens production HUD manifest and font: " + hud_presenter.error)
	var school_events_presenter := SchoolEventsPresenter.new()
	check(school_events_presenter.init_resources(font_root, hud_manifest_path).is_empty(), "SchoolEventsPresenter opens font and HUD manifest")
	var hud_strings: Dictionary = hud_presenter._manifest["content"]["strings"]
	var sunday_text_runs: Array[Dictionary] = [
		{"hex": "904196d8", "position": [-121, 50], "attribute": 0},
		{"hex": str(hud_strings["narration_prefix"]["raw_hex"]) + str(hud_strings["ordinary_command"]["raw_hex"]), "position": [-86, 50], "attribute": 0},
	]

	# 1. Initialize live OriginalAudioEngine on first-arrival and pump warm audio
	var host := Node.new()
	root.add_child(host)
	native_sink = NativeSink.new(host)
	native_backend = NativeBackend.new(native_sink)
	native_service = NativeService.new()
	first_owner = NativeBackend.ORIGINAL_IDENTITIES.duplicate(true)
	first_owner.merge({
		"role": "prologue_arrival",
		"epoch": 1,
		"ready": true,
		"load_set": 0,
		"sequence_pack_set": 0,
		"sequence": 0,
		"music_slot": 1,
		"SE_slot": 0,
		"state_flags": 0x30010,
		"reverb_preset": 3,
		"reverb_depth": 60,
		"reverb_enabled": true,
	})
	var prepared: Dictionary = native_backend.prepare_start(first_owner, 1, _originals(original_root, false))
	check(str(prepared.get("error", "")).is_empty(), "warm prologue_arrival native engine prepares")
	check(native_backend.commit_start(prepared.get("ticket", -1), first_owner).is_empty(), "warm prologue_arrival native engine starts")
	var active_player: AudioStreamPlayer = native_sink._active
	check(active_player != null, "warm AudioStreamPlayer created by NativeSink")

	var main_flags: int = (int(first_owner["state_flags"]) | 0x100) & ~0x1000 & ~0x100000
	check(native_service.bind_month_start_backend(native_backend, _originals(original_root, true), main_flags, 0x38).is_empty(), "SharedMonthAudioService binds warm NativeBackend with COM_MIX + SUMMER")

	var pump_error: String = ""
	var pump_until: int = Time.get_ticks_msec() + 150
	while Time.get_ticks_msec() < pump_until and pump_error.is_empty():
		pump_error = native_backend.pump(first_owner)
		await process_frame
	check(pump_error.is_empty(), "warm prologue_arrival playback pumps cleanly before May28->June30 route")
	var initial_native: Dictionary = native_backend.month_start_context()
	check(int(initial_native.get("sample_cursor", 0)) > 0, "warm sample_cursor advanced during initial pump")

	# 2. Configure SchoolDayController, MonthStartController, HolidayController, MediaStore, and ActorService
	var table_pack: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(tables_path)))
	var boundary_doc: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(boundary_path)))
	check(table_pack is Dictionary and boundary_doc is Dictionary, "table pack and boundary JSON loaded")

	var controller := Controller.new(table_pack["tables"])
	check(controller.error.is_empty(), "SchoolDayController initializes with original tables")
	check(controller.month_start.open_source(calendar_path).is_empty(), "MonthStartController opens original CALROM.NBN")
	check(controller.month_start.audio.bind_owner(native_service).is_empty(), "MonthStartController binds SharedMonthAudioService")

	var media_store := MediaStore.new()
	check(media_store.open_pack(media_path), "MediaStore opens command0/holiday media pack")
	var cmd0_resources: Dictionary = ExactMedia.from_media(media_store, "command_0_sd")
	var holiday_resources: Dictionary = ExactMedia.from_media(media_store, "holiday_command_0_sd")
	var cmd0_bg_img: Image = media_store.get_background("command_0_background").get("image_data")
	var holiday_room_bg_img: Image = media_store.get_background("holiday_room_background").get("image_data")
	var holiday_act_bg_img: Image = media_store.get_background("holiday_command_0_background").get("image_data")
	var cmd0_frame0_img: Image = cmd0_resources["frames"]["0"]["image"]
	check(cmd0_bg_img != null and holiday_room_bg_img != null and holiday_act_bg_img != null and cmd0_frame0_img != null, "background and SD images loaded from MediaStore")

	var holiday_tables: Dictionary = _integers(media_store.get_route_tables())
	var holiday_controller := HolidayController.new(table_pack["tables"], holiday_tables)
	check(holiday_controller.error.is_empty(), "HolidayController initializes with original holiday route tables")
	var actor_service := ActorService.new()

	var parsed_ref: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(ref_path)))
	check(parsed_ref is Dictionary and parsed_ref.has("target"), "resume-reference.json target witness loaded")
	var target: Dictionary = parsed_ref["target"]

	# 2b. Verify all 5 G2 month/quarter/September/year-rollover/vacation predicate families against real CALROM.NBN
	var g2_predicates_summary: Dictionary = _verify_g2_predicates(
		calendar_path, table_pack["tables"], target["state"]
	)

	var session := Session.new()
	var state := State.new()
	state.restore(target["state"])
	controller.command_capabilities.assign([0, 7])
	check(controller.attach_multi_week(session, state).is_empty(), "SchoolDayController attaches multi-week and overlay handlers")
	check(session.router._handlers.has(0x72), "SchoolDayController.attach_multi_week binds 0x72 Sports Festival overlay handler")
	check(holiday_controller.attach(session, state).is_empty(), "HolidayController attaches Sunday and post-Sunday handlers")
	session.clock.restore(target["clock"])
	session.rng.restore(target["rng"])
	var latch: Dictionary = target["input_latch"]
	var prev_mask: int = (int(latch["held"]) & ~int(latch["pressed"])) | int(latch["released"])
	session.latched_input = InputFrame.new(int(latch["tick"]), int(latch["held"]), prev_mask)
	check(session.state_hash() == EXPECTED_MAY28_HASH, "restored 1995-05-28 state hash matches frozen witness")

	var staged_services: Array[Dictionary] = []
	var pending_services: Array[Dictionary] = []
	var last_actor_draw_commands: Array[Dictionary] = []

	session.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		var current: State = base as State
		result.error = HudSeason.draw_step(current, result)
		if not result.error.is_empty():
			return
		var prepared_services: Array[Dictionary] = []
		var calendar_stage: Dictionary = controller.month_start.stage_services(current, result)
		if not calendar_stage["error"].is_empty():
			result.error = calendar_stage["error"]
			return
		prepared_services.append_array(calendar_stage["services"])
		for intent: Dictionary in result.presentation:
			var kind: String = str(intent.get("kind", ""))
			if kind == "school_day_load":
				prepared_services.append({
					"service": "school_day.resources",
					"payload": {
						"request": intent["request"],
						"ready": true,
						"lba": intent["background_lba"] if intent["request"] == "background" else intent["sd_lba"],
						"decode_result": 1,
						"checksum_ok": true,
					},
				})
			elif kind == "holiday_load":
				var h_payload: Dictionary = {
					"request": intent["request"],
					"lba": intent["lba"],
					"ready": true,
				}
				if intent["request"] != "sd":
					h_payload["decode_result"] = 1
				prepared_services.append({
					"service": "holiday.resources",
					"payload": h_payload,
				})
			elif kind == "school_day_actors":
				var init_act: Dictionary = actor_service.initialize(intent["actors"], cmd0_resources)
				if not init_act["error"].is_empty():
					result.error = init_act["error"]
					return
				current.rule_state["actor_state"] = init_act["actor_state"]
			elif kind == "holiday_actors":
				var init_h: Dictionary = actor_service.initialize(intent["actors"], holiday_resources)
				if not init_h["error"].is_empty():
					result.error = init_h["error"]
					return
				current.rule_state["actor_state"] = init_h["actor_state"]
		if not current.rule_state["actor_state"].is_empty():
			var res_for_draw: Dictionary = holiday_resources if current.game_mode == 0x46 else cmd0_resources
			var drawn: Dictionary = actor_service.native_draw(
				current.rule_state["actor_state"],
				res_for_draw,
				session.clock.logic_iterations,
				Vector2i(80, 56) if current.game_mode == 0x46 else Vector2i(160, 120)
			)
			if not drawn["error"].is_empty():
				result.error = drawn["error"]
				return
			current.rule_state["actor_state"] = drawn["actor_state"]
			last_actor_draw_commands.assign(drawn["draw_commands"])
			for svc: Dictionary in drawn["services"]:
				if current.game_mode == 0x46 and svc.get("service") == "school_day.actor96":
					prepared_services.append({"service": "holiday.actor96", "payload": svc["payload"]})
				else:
					prepared_services.append(svc)
		staged_services.append_array(prepared_services)

	session.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		if base.game_mode == 0x38 or (base as State).rule_state["resources"].has(Bridge.PENDING) or (base as State).rule_state["resources"].has("month_notice_pending"):
			result.error = controller.month_start.precommit_services(base as State, result)
	session.audio_logic = session.cd_logic

	var save_root: String = (out_dir if not out_dir.is_empty() else captures_dir.get_base_dir()).simplify_path().path_join("saves")
	var save_slot_name: String = "june04_sunday_boundary"
	var save_file_path: String = save_root.path_join(save_slot_name + ".json")
	if FileAccess.file_exists(save_file_path):
		DirAccess.remove_absolute(save_file_path)
	if FileAccess.file_exists(save_file_path + ".bak"):
		DirAccess.remove_absolute(save_file_path + ".bak")
	var save_service := SaveService.new(save_root)
	var june4_saved: bool = false
	var june4_save_tick: int = -1
	var june4_save_hash: String = ""
	var june4_save_checksum: String = ""
	var recorded_inputs_from_june4: Array[Dictionary] = []

	var seen_dates: Array[String] = []
	var june1_polls: Array[int] = []
	var june1_operations: Array[String] = []
	var june1_entered_tick: int = -1
	var june1_exited_tick: int = -1
	var june1_upload_season_fade: int = -1
	var may29_schedule_rng: int = -1
	var sports_festival_steps: int = 0
	var sports_festival_completed: bool = false
	var sports_festival_scratch_verified: bool = false
	var june_sundays_holiday_menu: Array[String] = []
	var june_sundays_weekday_menu: Array[String] = []
	var june_weekday_activity_dates: Array[String] = []
	var june_week_ends_reached: int = 0
	var june30_activity_completed: bool = false
	var captures_meta: Dictionary = {}

	var expansion_phase: String = "waiting_menu"
	var expansion_tick: int = 0
	var held_mask: int = 0
	var prev_held_mask: int = 0
	var max_steps: int = 3000
	var step_count: int = 0
	var step_failure: String = ""
	var all_overlay_modes: Array[int] = DayEventModes.all_overlay_native_modes()

	while step_count < max_steps and step_failure.is_empty() and not june30_activity_completed:
		var tick: int = session.clock.logic_iterations
		var rs: Dictionary = state.rule_state
		var date_str: String = "%02d-%02d-%02d" % [int(rs["year"]), int(rs["month"]), int(rs["day"])]
		if seen_dates.is_empty() or seen_dates[-1] != date_str:
			seen_dates.append(date_str)

		var is_overlay_active: bool = state.game_mode in all_overlay_modes
		var on_controllable_menu: bool = (state.game_mode in [0x34, 0x46] and state.mode_stage == 1 and state.mode_step == 0) or is_overlay_active

		if int(rs["year"]) == 95 and int(rs["month"]) == 6 and state.game_mode == 0x46 and state.mode_stage == 1 and state.mode_step == 0:
			if not june_sundays_holiday_menu.has(date_str):
				june_sundays_holiday_menu.append(date_str)
		if int(rs["year"]) == 95 and int(rs["month"]) == 6 and state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0:
			if not june_sundays_weekday_menu.has(date_str):
				june_sundays_weekday_menu.append(date_str)

		# Serialize disk save at the first controllable June Sunday boundary (1995-06-04 at 0x46/1/0)
		if not june4_saved and date_str == "95-06-04" and state.game_mode == 0x46 and state.mode_stage == 1 and state.mode_step == 0:
			june4_saved = true
			june4_save_tick = tick
			june4_save_hash = session.state_hash()
			var snapshot_to_save: Dictionary = {
				"boundary": "stable",
				"source_version": "SLPM_86.053",
				"content_version": "hud-production-v1",
				"behavior_version": "T-29023-june-v1",
				"gameplay_state": {
					"state": state.projection(),
					"latched_input": session.latched_input.snapshot(),
				},
				"native_rng": session.rng.checkpoint(),
				"native_clocks": session.clock.snapshot(),
				"queued_service_events": pending_services.duplicate(true),
			}
			var save_res: Dictionary = save_service.save_slot(save_slot_name, snapshot_to_save)
			check(str(save_res.get("error", "")).is_empty(), "SaveService serializes 1995-06-04 0x46/1/0 boundary to disk: " + str(save_res.get("error", "")))
			june4_save_checksum = str(save_res.get("checksum_sha256", ""))
			check(not june4_save_checksum.is_empty() and FileAccess.file_exists(save_file_path), "1995-06-04 disk save file exists with SHA-256 checksum")

		# Input state machine: confirm controllable menus and interactive overlay steps
		if tick > 3367:
			if expansion_phase == "pressed":
				if tick - expansion_tick >= 2:
					held_mask = 0
					expansion_phase = "waiting_menu"
			elif expansion_phase == "ready":
				var wait_ticks: int = 2 if is_overlay_active else 8
				if tick - expansion_tick >= wait_ticks:
					held_mask = PAD_CONFIRM
					expansion_tick = tick
					expansion_phase = "pressed"
			elif on_controllable_menu:
				expansion_tick = tick
				expansion_phase = "ready"

		if state.game_mode == 0x38 and june1_entered_tick < 0:
			june1_entered_tick = tick

		if june4_saved:
			recorded_inputs_from_june4.append({
				"tick": tick,
				"held": held_mask,
				"prev_held": prev_held_mask,
			})

		var events: Array[Session.ServiceEvent] = []
		for desc: Dictionary in pending_services:
			events.append(Session.ServiceEvent.new(
				tick,
				Session.Phase.BEFORE_MODE,
				Session.ServiceKind.COMPLETION,
				desc["service"],
				desc["payload"]
			))
		staged_services.clear()
		var month_frame: int = state.frame_count
		var is_month_38: bool = state.game_mode == 0x38
		var prep_err: String = controller.month_start.prepare_services(state) if is_month_38 else ""
		if not prep_err.is_empty():
			step_failure = "prepare_services failed: " + prep_err
			break
		var input_frame := InputFrame.new(tick, held_mask, prev_held_mask)
		prev_held_mask = held_mask
		var result: Session.FrameResult = session.step(input_frame, events)
		if not result.committed:
			if is_month_38:
				controller.month_start.discard_services(month_frame)
			step_failure = "step failed at tick %d (%s mode 0x%02x stage %d step %d): %s" % [
				tick, date_str, state.game_mode, state.mode_stage, state.mode_step, result.error
			]
			break
		pending_services = staged_services.duplicate(true)
		var pub_err: String = controller.month_start.publish_services(result) if (is_month_38 or state.game_mode == 0x38) else ""
		var disc_err: String = controller.month_start.discard_services(month_frame) if is_month_38 else ""
		if not pub_err.is_empty() or not disc_err.is_empty():
			step_failure = "postcommit month service error: %s %s" % [pub_err, disc_err]
			break

		step_count += 1

		if state.game_mode == 0x30 and state.mode_step == 3 and may29_schedule_rng < 0:
			may29_schedule_rng = session.rng.state
		for raw_audio: Variant in result.audio:
			if raw_audio is Dictionary and raw_audio.get("kind") == Bridge.KIND:
				var op: String = str(raw_audio.get("operation", ""))
				june1_operations.append(op)
				if op == "load":
					june1_upload_season_fade = int(rs["resources"].get(HudSeason.KEY, -1))
					var img_june1: Image = _compose_hud_capture(
						hud_presenter, rs, state.frame_count, cmd0_bg_img, [], [], false
					)
					if img_june1 != null:
						var p1: String = captures_dir.path_join("01-june01-quarter-transition-320x240.png")
						img_june1.save_png(p1)
						captures_meta["01-june01-quarter-transition-320x240.png"] = {
							"tick": session.clock.logic_iterations,
							"date": date_str,
							"mode": state.game_mode,
							"sha256": FileAccess.get_sha256(p1),
						}
				elif op == "poll":
					june1_polls.append(int(raw_audio.get("status", 0)))
		if june1_entered_tick >= 0 and june1_exited_tick < 0 and state.game_mode == 0x31 and int(rs["day_event_index"]) == 8:
			june1_exited_tick = session.clock.logic_iterations

		for p_item: Dictionary in result.presentation:
			if p_item.get("kind") == "overlay_mode" and int(p_item.get("mode", 0)) == 0x72:
				sports_festival_steps += 1
				if not captures_meta.has("02-june03-sports-festival-320x240.png"):
					var ovl_dict: Dictionary = p_item.get("overlay_state", {}) if p_item.get("overlay_state") is Dictionary else {}
					var img_june3: Image = school_events_presenter.render_sports_festival_hud(ovl_dict)
					if img_june3 != null:
						var p2: String = captures_dir.path_join("02-june03-sports-festival-320x240.png")
						img_june3.save_png(p2)
						captures_meta["02-june03-sports-festival-320x240.png"] = {
							"tick": session.clock.logic_iterations,
							"date": date_str,
							"mode": 0x72,
							"sha256": FileAccess.get_sha256(p2),
						}
				if bool(p_item.get("completed", false)):
					sports_festival_completed = true
					sports_festival_scratch_verified = rs.get("overlay_scratch", []).size() == 256 and rs.has("last_completed_overlay")
					rs.erase("overlay_scratch")
					rs.erase("last_completed_overlay")
			elif p_item.get("kind") == "week_end_hud_init" and int(rs["month"]) == 6:
				june_week_ends_reached += 1

		if date_str == "95-06-04" and state.game_mode == 0x46 and state.mode_stage == 1 and state.mode_step == 0 and not captures_meta.has("03-june04-sunday-room-320x240.png"):
			var img_june4: Image = _compose_hud_capture(
				hud_presenter, rs, state.frame_count, holiday_room_bg_img, [], sunday_text_runs, false
			)
			if img_june4 != null:
				var p3: String = captures_dir.path_join("03-june04-sunday-room-320x240.png")
				img_june4.save_png(p3)
				captures_meta["03-june04-sunday-room-320x240.png"] = {
					"tick": session.clock.logic_iterations,
					"date": date_str,
					"mode": state.game_mode,
					"sha256": FileAccess.get_sha256(p3),
				}

		if int(rs["year"]) == 95 and int(rs["month"]) == 6 and state.game_mode == 0x40 and state.mode_stage == 1:
			if not june_weekday_activity_dates.has(date_str):
				june_weekday_activity_dates.append(date_str)
			if date_str == "95-06-05" and not captures_meta.has("04-june05-weekday-activity-320x240.png"):
				var img_june5: Image = _compose_hud_capture(
					hud_presenter, rs, state.frame_count, cmd0_bg_img, last_actor_draw_commands, [], false
				)
				if img_june5 != null:
					var p4: String = captures_dir.path_join("04-june05-weekday-activity-320x240.png")
					img_june5.save_png(p4)
					captures_meta["04-june05-weekday-activity-320x240.png"] = {
						"tick": session.clock.logic_iterations,
						"date": date_str,
						"mode": state.game_mode,
						"sha256": FileAccess.get_sha256(p4),
					}
			elif date_str == "95-06-30" and not captures_meta.has("05-june30-weekday-activity-320x240.png"):
				var img_june30: Image = _compose_hud_capture(
					hud_presenter, rs, state.frame_count, cmd0_bg_img, last_actor_draw_commands, [], false
				)
				if img_june30 != null:
					var p5: String = captures_dir.path_join("05-june30-weekday-activity-320x240.png")
					img_june30.save_png(p5)
					captures_meta["05-june30-weekday-activity-320x240.png"] = {
						"tick": session.clock.logic_iterations,
						"date": date_str,
						"mode": state.game_mode,
						"sha256": FileAccess.get_sha256(p5),
					}

		if date_str == "95-06-30" and state.game_mode == 0x40 and state.mode_stage == 2:
			june30_activity_completed = true

	var expected_dates: Array[String] = _build_expected_dates()
	check(step_failure.is_empty(), "simulation stepped cleanly from 1995-05-28 through 1995-06-30: " + step_failure)
	check(june30_activity_completed, "completed Friday 1995-06-30 weekday activity")
	check(seen_dates == expected_dates, "visited all 34 calendar dates in exact order from 1995-05-28 through 1995-06-30")
	check(may29_schedule_rng == EXPECTED_JUNE1_SCHEDULE_RNG, "1995-05-29 schedule build RNG matches 2966941936")
	check(june1_entered_tick == 3631 and june1_exited_tick == 3637, "1995-06-01 mode 0x38 runs ticks 3631..3637")
	check(june1_operations == ["load", "poll", "poll", "poll", "start"], "1995-06-01 native audio operations are load, poll, poll, poll, start")
	check(june1_polls == [-1, -1, 1], "1995-06-01 native driver polls are [-1, -1, 1]")
	check(june1_upload_season_fade == 0xf0, "1995-06-01 CALROM upload initializes summer HUD fade")
	check(sports_festival_completed and sports_festival_steps > 0 and sports_festival_scratch_verified, "1995-06-03 Saturday Sports Festival (0x72) stepped interactively and completed")
	check(june_sundays_holiday_menu == ["95-06-04", "95-06-11", "95-06-18", "95-06-25"], "all 4 June Sundays reached controllable 0x46/1/0 holiday command menu")
	check(june_sundays_weekday_menu == ["95-06-04", "95-06-11", "95-06-18", "95-06-25"], "all 4 June Sundays reached controllable 0x34/1/0 post-Sunday weekday command menu")
	check(june_weekday_activity_dates.size() == 25, "all 25 non-festival June weekdays/Saturdays executed 0x40 weekday activity (got %d)" % june_weekday_activity_dates.size())
	check(june_week_ends_reached == 4, "all 4 June Saturdays (June 3, 10, 17, 24) executed 0x32 week-end transition (got %d)" % june_week_ends_reached)
	check(captures_meta.size() == 5, "saved all 5 summer 320x240 PNG captures")

	var primary_final_hash: String = session.state_hash()
	var primary_final_projection: Dictionary = state.projection()
	var primary_final_rng: Dictionary = session.rng.checkpoint()
	var primary_final_clock: Dictionary = session.clock.snapshot()

	# 3. Fresh Restart/Reload from 1995-06-04 disk save and replay through 1995-06-30
	check(june4_saved and recorded_inputs_from_june4.size() > 0, "1995-06-04 save and recorded input suffix captured (%d ticks)" % recorded_inputs_from_june4.size())
	var reload_service := SaveService.new(save_root)
	var loaded_save: Dictionary = reload_service.load_slot(save_slot_name)
	check(str(loaded_save.get("error", "")).is_empty(), "SaveService loads and validates 1995-06-04 disk save: " + str(loaded_save.get("error", "")))
	check(str(loaded_save.get("checksum_sha256", "")) == june4_save_checksum, "reloaded disk save SHA-256 checksum matches saved checksum")
	var loaded_snapshot: Dictionary = loaded_save.get("snapshot", {})

	var restarted_session := Session.new()
	var restarted_state := State.new()
	var restarted_controller := Controller.new(table_pack["tables"])
	var restarted_holiday := HolidayController.new(table_pack["tables"], holiday_tables)
	var restarted_actor_service := ActorService.new()
	check(restarted_controller.error.is_empty() and restarted_holiday.error.is_empty(), "fresh restarted Controller and HolidayController initialize cleanly")
	check(restarted_controller.month_start.open_source(calendar_path).is_empty(), "fresh restarted MonthStartController opens CALROM.NBN")
	check(restarted_controller.month_start.audio.bind_owner(native_service).is_empty(), "fresh restarted MonthStartController binds SharedMonthAudioService")

	restarted_state.restore(loaded_snapshot["gameplay_state"]["state"])
	restarted_controller.command_capabilities.assign([0, 7])
	check(restarted_controller.attach_multi_week(restarted_session, restarted_state).is_empty(), "fresh restarted SchoolDayController attaches to restarted session")
	check(restarted_holiday.attach(restarted_session, restarted_state).is_empty(), "fresh restarted HolidayController attaches to restarted session")
	restarted_session.clock.restore(loaded_snapshot["native_clocks"])
	restarted_session.rng.restore(loaded_snapshot["native_rng"])
	var rl_latch: Dictionary = loaded_snapshot["gameplay_state"]["latched_input"]
	var rl_prev_mask: int = (int(rl_latch["held"]) & ~int(rl_latch["pressed"])) | int(rl_latch["released"])
	restarted_session.latched_input = InputFrame.new(int(rl_latch["tick"]), int(rl_latch["held"]), rl_prev_mask)
	check(restarted_session.state_hash() == june4_save_hash, "restarted session state_hash at 1995-06-04 matches saved boundary hash")

	var restarted_staged_services: Array[Dictionary] = []
	var restarted_pending_services: Array[Dictionary] = []
	for item: Variant in loaded_snapshot["queued_service_events"]:
		if item is Dictionary:
			restarted_pending_services.append(item.duplicate(true))

	restarted_session.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		var current: State = base as State
		result.error = HudSeason.draw_step(current, result)
		if not result.error.is_empty():
			return
		var prepared_services: Array[Dictionary] = []
		var calendar_stage: Dictionary = restarted_controller.month_start.stage_services(current, result)
		if not calendar_stage["error"].is_empty():
			result.error = calendar_stage["error"]
			return
		prepared_services.append_array(calendar_stage["services"])
		for intent: Dictionary in result.presentation:
			var kind: String = str(intent.get("kind", ""))
			if kind == "school_day_load":
				prepared_services.append({
					"service": "school_day.resources",
					"payload": {
						"request": intent["request"],
						"ready": true,
						"lba": intent["background_lba"] if intent["request"] == "background" else intent["sd_lba"],
						"decode_result": 1,
						"checksum_ok": true,
					},
				})
			elif kind == "holiday_load":
				var h_payload: Dictionary = {
					"request": intent["request"],
					"lba": intent["lba"],
					"ready": true,
				}
				if intent["request"] != "sd":
					h_payload["decode_result"] = 1
				prepared_services.append({
					"service": "holiday.resources",
					"payload": h_payload,
				})
			elif kind == "school_day_actors":
				var init_act: Dictionary = restarted_actor_service.initialize(intent["actors"], cmd0_resources)
				if not init_act["error"].is_empty():
					result.error = init_act["error"]
					return
				current.rule_state["actor_state"] = init_act["actor_state"]
			elif kind == "holiday_actors":
				var init_h: Dictionary = restarted_actor_service.initialize(intent["actors"], holiday_resources)
				if not init_h["error"].is_empty():
					result.error = init_h["error"]
					return
				current.rule_state["actor_state"] = init_h["actor_state"]
		if not current.rule_state["actor_state"].is_empty():
			var res_for_draw: Dictionary = holiday_resources if current.game_mode == 0x46 else cmd0_resources
			var drawn: Dictionary = restarted_actor_service.native_draw(
				current.rule_state["actor_state"],
				res_for_draw,
				restarted_session.clock.logic_iterations,
				Vector2i(80, 56) if current.game_mode == 0x46 else Vector2i(160, 120)
			)
			if not drawn["error"].is_empty():
				result.error = drawn["error"]
				return
			current.rule_state["actor_state"] = drawn["actor_state"]
			for svc: Dictionary in drawn["services"]:
				if current.game_mode == 0x46 and svc.get("service") == "school_day.actor96":
					prepared_services.append({"service": "holiday.actor96", "payload": svc["payload"]})
				else:
					prepared_services.append(svc)
		restarted_staged_services.append_array(prepared_services)

	restarted_session.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		if base.game_mode == 0x38 or (base as State).rule_state["resources"].has(Bridge.PENDING) or (base as State).rule_state["resources"].has("month_notice_pending"):
			result.error = restarted_controller.month_start.precommit_services(base as State, result)
	restarted_session.audio_logic = restarted_session.cd_logic

	var restarted_seen_dates: Array[String] = []
	var restarted_failure: String = ""
	for rec_in: Dictionary in recorded_inputs_from_june4:
		var r_tick: int = restarted_session.clock.logic_iterations
		var r_rs: Dictionary = restarted_state.rule_state
		var r_date: String = "%02d-%02d-%02d" % [int(r_rs["year"]), int(r_rs["month"]), int(r_rs["day"])]
		if restarted_seen_dates.is_empty() or restarted_seen_dates[-1] != r_date:
			restarted_seen_dates.append(r_date)
		var r_events: Array[Session.ServiceEvent] = []
		for desc: Dictionary in restarted_pending_services:
			r_events.append(Session.ServiceEvent.new(
				r_tick,
				Session.Phase.BEFORE_MODE,
				Session.ServiceKind.COMPLETION,
				desc["service"],
				desc["payload"]
			))
		restarted_staged_services.clear()
		var r_month_frame: int = restarted_state.frame_count
		var r_is_month_38: bool = restarted_state.game_mode == 0x38
		var r_prep_err: String = restarted_controller.month_start.prepare_services(restarted_state) if r_is_month_38 else ""
		if not r_prep_err.is_empty():
			restarted_failure = "restarted prepare_services failed: " + r_prep_err
			break
		var r_input := InputFrame.new(int(rec_in["tick"]), int(rec_in["held"]), int(rec_in["prev_held"]))
		var r_result: Session.FrameResult = restarted_session.step(r_input, r_events)
		if not r_result.committed:
			if r_is_month_38:
				restarted_controller.month_start.discard_services(r_month_frame)
			restarted_failure = "restarted step failed at tick %d: %s" % [r_tick, r_result.error]
			break
		restarted_pending_services = restarted_staged_services.duplicate(true)
		var r_pub_err: String = restarted_controller.month_start.publish_services(r_result) if (r_is_month_38 or restarted_state.game_mode == 0x38) else ""
		var r_disc_err: String = restarted_controller.month_start.discard_services(r_month_frame) if r_is_month_38 else ""
		if not r_pub_err.is_empty() or not r_disc_err.is_empty():
			restarted_failure = "restarted postcommit error: %s %s" % [r_pub_err, r_disc_err]
			break

	var expected_june4_dates: Array[String] = _build_expected_june4_to_june30_dates()
	check(restarted_failure.is_empty(), "restarted continuation stepped cleanly from 1995-06-04 through 1995-06-30: " + restarted_failure)
	check(restarted_seen_dates == expected_june4_dates, "restarted continuation visited all 27 remaining June dates (95-06-04..95-06-30) in exact order")
	check(restarted_state.game_mode == 0x40 and restarted_state.mode_stage == 2, "restarted continuation terminated at 1995-06-30 0x40/2")
	check(restarted_session.state_hash() == primary_final_hash, "restarted continuation final state_hash matches uninterrupted primary run")
	check(restarted_state.projection() == primary_final_projection, "restarted continuation final state.projection() matches uninterrupted primary run")
	check(restarted_session.rng.checkpoint() == primary_final_rng, "restarted continuation final RNG checkpoint matches uninterrupted primary run")
	check(restarted_session.clock.snapshot() == primary_final_clock, "restarted continuation final clock snapshot matches uninterrupted primary run")

	var final_native: Dictionary = native_backend.month_start_context()
	check(int(final_native.get("vm_initializations", 0)) == 1, "OriginalAudioEngine VM initialized exactly once")
	check(int(final_native.get("gain", 0)) == 86 and int(final_native.get("raw_reverb_depth", 0)) == 56, "OriginalAudioEngine SUMMER gain 86 and raw_reverb_depth 56 preserved")
	check(bool(final_native.get("started", false)), "OriginalAudioEngine SUMMER sequence 0 remains started")
	check(native_sink._active == active_player, "warm transition preserves exact AudioStreamPlayer instance")

	var report: Dictionary = {
		"checks": checks,
		"failures": failures,
		"step_count": step_count,
		"final_tick": session.clock.logic_iterations,
		"seen_dates": seen_dates,
		"june1_entered_tick": june1_entered_tick,
		"june1_exited_tick": june1_exited_tick,
		"june1_operations": june1_operations,
		"june1_polls": june1_polls,
		"sports_festival_steps": sports_festival_steps,
		"sports_festival_completed": sports_festival_completed,
		"june_sundays_holiday_menu": june_sundays_holiday_menu,
		"june_sundays_weekday_menu": june_sundays_weekday_menu,
		"june_weekday_activity_dates": june_weekday_activity_dates,
		"june_week_ends_reached": june_week_ends_reached,
		"june30_activity_completed": june30_activity_completed,
		"restart_reload": {
			"save_tick": june4_save_tick,
			"save_hash": june4_save_hash,
			"save_checksum_sha256": june4_save_checksum,
			"replayed_ticks": recorded_inputs_from_june4.size(),
			"restarted_dates_visited": restarted_seen_dates.size(),
			"final_hash_matched": restarted_session.state_hash() == primary_final_hash,
			"final_hash": primary_final_hash,
		},
		"g2_predicates": g2_predicates_summary,
		"captures": captures_meta,
		"final_native": final_native,
	}
	if not out_dir.is_empty():
		var f := FileAccess.open(out_dir.path_join("june-full-quarter-route.json"), FileAccess.WRITE)
		if f != null:
			f.store_string(JSON.stringify(report, "\t"))
			f.close()

	restarted_session.draw_logic = Callable()
	restarted_session.cd_logic = Callable()
	restarted_session.audio_logic = Callable()
	restarted_session.service_completion = Callable()
	restarted_session.router._handlers.clear()
	restarted_controller.month_start.reset_services()
	restarted_controller = null
	restarted_holiday = null
	restarted_actor_service = null
	restarted_session = null
	restarted_state = null

	session.draw_logic = Callable()
	session.cd_logic = Callable()
	session.audio_logic = Callable()
	session.service_completion = Callable()
	session.router._handlers.clear()
	cmd0_resources.clear()
	holiday_resources.clear()
	last_actor_draw_commands.clear()
	media_store = null
	actor_service = null
	holiday_controller = null
	hud_presenter = null
	school_events_presenter = null
	controller.month_start.reset_services()
	controller = null
	session = null
	state = null
	native_service.reset_month_start_owner()
	active_player = null
	host.queue_free()
	native_service = null
	native_backend = null
	native_sink = null
	await process_frame
	await create_timer(0.2).timeout
	await process_frame
	print(JSON.stringify({
		"checks": checks,
		"failures": failures,
		"dates_visited": seen_dates.size(),
		"first_date": seen_dates[0] if not seen_dates.is_empty() else "",
		"last_date": seen_dates[-1] if not seen_dates.is_empty() else "",
		"june_sundays": june_sundays_holiday_menu,
		"june_weekday_activities": june_weekday_activity_dates.size(),
		"sports_festival_completed": sports_festival_completed,
		"restart_reload_matched": report["restart_reload"]["final_hash_matched"],
		"replayed_ticks": recorded_inputs_from_june4.size(),
		"g2_predicates": g2_predicates_summary,
		"captures": captures_meta.size(),
	}))
	quit(0 if failures.is_empty() else 1)
