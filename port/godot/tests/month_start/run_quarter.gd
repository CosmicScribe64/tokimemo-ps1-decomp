extends "res://tests/month_start/run_month_start.gd"
## Original CALROM and real Session, with invented states. No audio/gameplay claim.
const AudioService = preload("res://services/audio_service.gd")
const Bridge = preload("res://services/month_start_audio_bridge.gd")
const Calendar = preload("res://rules/calendar.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")

class ControlledAudioModel extends AudioService:
	## TEST ONLY. Models protocol outcomes, never native preparation/PCM/readiness.
	var ready: bool = false
	var generation: int = 1
	var epoch: int = 2
	var fault: String = ""
	var started: bool = false
	var published_batches: Array[Dictionary] = []
	var prepared_projection: Dictionary = {}
	func month_start_prepare(payload: Dictionary) -> Dictionary:
		prepared_projection = payload["state"].duplicate(true)
		return {"schema": Bridge.VALIDATION, "error": "profile unavailable" if fault == "prepare" else "", "intent_sha256": Bridge.digest(payload)}
	func month_start_discard(frame: int) -> Dictionary:
		return {"schema": Bridge.VALIDATION, "error": "", "frame_count": frame}
	func month_start_operation(payload: Dictionary) -> Variant:
		if fault == "malformed":
			return {}
		var context: Dictionary = {"profile_sha256": "f".repeat(64),
			"binding_sha256": Bridge.digest(payload["request"]["binding"]),
			"native_state_sha256": "CONTROLLED TEST MODEL".sha256_text(), "raw_reverb_depth": 56}
		if fault == "cold_reverb": context["raw_reverb_depth"] = 60
		if fault == "foreign_binding": context["binding_sha256"] = "0".repeat(64)
		if fault == "foreign_profile": context["profile_sha256"] = "a".repeat(64)
		var status: Variant = (1 if ready else -1) if payload["operation"] == "poll" else 0
		if fault == "boolean_status": status = true
		var reply: Dictionary = {"schema": Bridge.PROTOCOL, "error": "", "request_sha256": Bridge.digest(payload),
			"owner": {"session_generation": generation, "bank_epoch": epoch, "request_serial": 7},
			"token": Bridge.digest(payload), "status": status, "source_context": context}
		match fault:
			"schema_dict": reply["schema"] = {}
			"hash_dict": reply["request_sha256"] = {}
			"context_hash_dict": reply["source_context"]["binding_sha256"] = {}
			"context_depth_dict": reply["source_context"]["raw_reverb_depth"] = {}
			"owner_epoch_dict": reply["owner"]["bank_epoch"] = {}
			"token_dict": reply["token"] = {}
		return reply
	func month_start_validate(intent: Dictionary, phase: String) -> Dictionary:
		var error: String = ""
		if intent["owner"]["session_generation"] != generation or intent["owner"]["bank_epoch"] != epoch:
			error = "stale model context"
		if phase == "active" and not started:
			error = "no started model context"
		if fault == "precommit" and phase == "precommit":
			error = "reserved sink was refused"
		var reply: Dictionary = {"schema": Bridge.VALIDATION, "error": error, "intent_sha256": Bridge.digest(intent)}
		if fault == "validation_schema_dict": reply["schema"] = {}
		if fault == "validation_hash_dict": reply["intent_sha256"] = {}
		return reply
	func month_start_validate_batch(intents: Array[Dictionary], projection: Dictionary, phase: String) -> Dictionary:
		var payload: Dictionary = {"intents": intents, "final_projection": projection, "phase": phase}
		var reply: Dictionary = {"schema": Bridge.VALIDATION, "error": "injected batch refusal" if fault == "batch" else "", "intent_sha256": Bridge.digest(payload)}
		if fault == "batch_schema": reply["schema"] = {}
		if fault == "batch_hash": reply["intent_sha256"] = {}
		return reply
	func month_start_publish_batch(intents: Array[Dictionary], projection: Dictionary) -> Dictionary:
		var payload: Dictionary = {"intents": intents, "final_projection": projection}
		if fault == "publication":
			return {"schema": Bridge.VALIDATION, "error": "injected publication failure", "intent_sha256": Bridge.digest(payload)}
		published_batches.append(payload.duplicate(true))
		for intent: Dictionary in intents:
			event_history.append(intent.duplicate(true))
			if intent["operation"] == "start": started = true
		return {"schema": Bridge.VALIDATION, "error": "", "intent_sha256": Bridge.digest(payload)}

class ControlledNoticeModel extends RefCounted:
	var history: Array[Dictionary] = []
	var fault: String = ""
	func month_start_notice(payload: Dictionary) -> Dictionary:
		var reply: Dictionary = {"schema": "tokimemo.month-notice.v1", "error": "", "request_sha256": Bridge.digest(payload),
			"token": Bridge.digest(payload), "source_sha256": "e".repeat(64)}
		if fault == "schema_dict": reply["schema"] = {}
		if fault == "hash_dict": reply["request_sha256"] = {}
		if fault == "token_dict": reply["token"] = {}
		return reply
	func month_start_notice_validate(intent: Dictionary, _phase: String) -> Dictionary:
		return {"schema": "tokimemo.month-notice-validation.v1", "error": "", "intent_sha256": Bridge.digest(intent)}
	func month_start_notice_publish(intent: Dictionary) -> Dictionary:
		history.append(intent.duplicate(true))
		return {"schema": "tokimemo.month-notice-validation.v1", "error": "", "intent_sha256": Bridge.digest(intent)}

func _harness(state: State = null, open_original: bool = true) -> Dictionary:
	var harness: Dictionary = super._harness(state, open_original)
	var month: MonthStart = harness["controller"].month_start
	var source_draw: Callable = harness["session"].draw_logic
	harness["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
		result.error = HudSeason.draw_step(base as State, result)
		if result.error.is_empty():
			source_draw.call(base, rng, result)
	harness["session"].audio_logic = func(base: Core, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = month.precommit_services(base as State, result)
	harness["session"].cd_logic = harness["session"].audio_logic
	return harness

func _step(harness: Dictionary, events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	var controller: MonthStart = harness["controller"].month_start
	var frame: int = harness["state"].frame_count
	controller.prepare_services(harness["state"])
	var before_history: String = JSON.stringify(harness["model"].checkpoint()) if harness.has("model") else ""
	var result: Session.FrameResult = super._step(harness, events)
	if harness.has("model"):
		check(JSON.stringify(harness["model"].checkpoint()) == before_history, "pure rule/precommit callbacks never publish AudioService effects")
	if result.committed:
		check(controller.publish_services(result).is_empty(), "committed model service effects publish after Session returns")
	else:
		check(not controller.publish_services(result).is_empty(), "aborted Session result refuses service publication")
	check(controller.discard_services(frame).is_empty(), "private preparation cleanup runs outside Session")
	return result

func _with_model(year: int = 95, month: int = 6, notice: bool = false) -> Dictionary:
	var harness: Dictionary = _harness(_state(year, month))
	harness["state"].sound_active = true # Controlled source audio phase, never a runtime patch.
	var model := ControlledAudioModel.new()
	check(harness["controller"].month_start.audio.bind_owner(model).is_empty(), "test-only controlled AudioService protocol binds")
	harness["model"] = model
	if notice:
		var text := ControlledNoticeModel.new()
		check(harness["controller"].month_start.bind_notice_owner(text).is_empty(), "test-only source notice model binds")
		harness["notice"] = text
	return harness

func _upload(harness: Dictionary) -> Session.FrameResult:
	_request(harness)
	return _step(harness, [_event(harness, _payload(harness))])

func _initialize() -> void:
	source_path = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	check(source.open_source(source_path).is_empty(), "original CALROM opens")
	_test_lifecycle()
	_test_completion_rejections()
	_test_calendar_integrity()
	_test_transaction_retry()
	_test_music()
	_test_refusals()
	_test_lifetime_and_shapes()
	_test_year_roll()
	_test_season_fade()
	_test_owned_batch()
	_test_publication_shapes()
	var harness: Dictionary = _harness(_state(96, 9))
	var state: State = harness["state"]
	state.rule_state["player_flags_upper"] |= 4
	var requested: Session.FrameResult = _step(harness)
	check(requested.committed and state.substep == 1, "Sep96 skip still issues real CALROM request")
	if requested.committed:
		var uploaded: Session.FrameResult = _step(harness, [_event(harness, _payload(harness))])
		check(uploaded.committed and state.mode_stage == 1, "Sep96 skip uploads then advances stage without audio")
		check(uploaded.audio.is_empty(), "Sep96 skip never invents BGM helper call")
		check((state.rule_state["player_flags_upper"] & 4) == 0, "Sep96 common amusement reset executes")
		check(state.rule_state["resources"].get("hud_season_fade") == 0xef, "Sep96 common source EF executes")
	_finish()

func _test_publication_shapes() -> void:
	var paths: Array = [["state"], ["state", "frame_count"], ["state", "game_mode"],
		["state", "sound_active"], ["state", "rule_state"], ["state", "rule_state", "resources"],
		["state", "rule_state", "year"], ["state", "rule_state", "calendar"], ["state", "rule_state", "params"],
		["state", "rule_state", "resources", Bridge.EMISSION, "frame_count"],
		["state", "rule_state", "resources", Bridge.PENDING]]
	for september: bool in [false, true]:
		var harness: Dictionary = _with_model(95, 9, true) if september else _with_model()
		_request(harness)
		var month: MonthStart = harness["controller"].month_start
		var frame: int = harness["state"].frame_count
		check(month.prepare_services(harness["state"]).is_empty(), "genuine native source load prepares before malformed publication table")
		var events: Array[Session.ServiceEvent] = [_event(harness, _payload(harness))]
		var committed: Session.FrameResult = harness["session"].step(InputFrame.new(harness["session"].clock.logic_iterations), events)
		check(committed.committed, "publication shape negative starts from real committed Session")
		var baseline: Array[Dictionary] = committed.trace.duplicate(true)
		for phase: String in ["draw_logic", "audio"]:
			for path: Array in paths:
				for bad: Variant in [{}, [], null, true, 1.25, "foreign", "__missing__"]:
					if path.back() == "sound_active" and bad is bool: continue
					committed.trace = baseline.duplicate(true)
					for record: Dictionary in committed.trace:
						if record.get("phase") == phase:
							var parent: Dictionary = record
							for index: int in range(path.size() - 1): parent = parent[path[index]]
							if bad is String and bad == "__missing__": parent.erase(path.back())
							else: parent[path.back()] = bad
					var error: String = month.publish_services(committed)
					check(not error.is_empty() and committed.committed, "malformed %s projection %s type%d is terminal committed/error" % [phase, str(path), typeof(bad)])
					check(harness["model"].event_history.is_empty() and (not september or harness["notice"].history.is_empty()), "malformed publication projection has no native or notice effects")
			for mutation: String in ["missing", "duplicate", "empty_first", "foreign_phase"]:
				committed.trace = baseline.duplicate(true)
				for record: Dictionary in committed.trace:
					if record.get("phase") == phase:
						match mutation:
							"missing": committed.trace.erase(record)
							"duplicate": committed.trace.append(record.duplicate(true))
							"empty_first": committed.trace.push_front({"phase": phase, "state": {}})
							"foreign_phase": record["phase"] = {}
						break
				check(not month.publish_services(committed).is_empty() and committed.committed, "missing/duplicate/malformed source trace record refuses cleanly: " + mutation)
				check(harness["model"].event_history.is_empty() and (not september or harness["notice"].history.is_empty()), "trace record rejection preserves both service histories")
		committed.trace = baseline.duplicate(true)
		check(month.publish_services(committed).is_empty() and harness["model"].event_history.size() == 1, "clean original publication remains possible after all rejected malformed metadata")
		check(not september or harness["notice"].history.size() == 1, "clean original September notice publishes once after refused metadata")
		check(month.discard_services(frame).is_empty(), "shape-table private staging cleanup outside Session")
	for reply: Dictionary in [{}, {"error": {}}, {"error": []}, {"error": null}, {"error": true}, {"error": 1.25}, {"error": 0}]:
		check(not Bridge.result_error(reply).is_empty(), "missing/malformed internal error envelope cannot mean success")

func _test_owned_batch() -> void:
	for phase: String in ["load", "poll", "ready"]:
		for mutation: String in ["rename", "clear", "drop_tail", "replace", "duplicate", "erase_pending", "rewrite_receipt", "projection_rewrite", "reverse", "inactive"]:
			if mutation == "reverse" and phase != "ready": continue
			var harness: Dictionary = _with_model()
			if phase == "load": _request(harness)
			else: check(_upload(harness).committed, "source load precedes owned batch mutation")
			harness["model"].ready = phase == "ready"
			var before_history: int = harness["model"].event_history.size()
			var original_draw: Callable = harness["session"].draw_logic
			harness["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
				original_draw.call(base, rng, result)
				match mutation:
					"rename": result.audio[0]["kind"] = "bgm_play"
					"clear": result.audio.clear()
					"drop_tail": result.audio.pop_back()
					"replace": result.audio.assign([{"kind": "bgm_play", "sequence": 0}])
					"duplicate": result.audio.append(result.audio[0].duplicate(true))
					"erase_pending":
						result.audio.clear()
						(base as State).rule_state["resources"].erase(Bridge.PENDING)
						(base as State).rule_state["resources"].erase("month_audio_emission")
					"rewrite_receipt":
						result.audio[0]["kind"] = "bgm_play"
						(base as State).rule_state["resources"][Bridge.EMISSION] = {"frame_count": base.frame_count - 1, "intents": [Bridge.digest(result.audio[0])]}
					"inactive": base.sound_active = false
					"projection_rewrite":
						result.audio[0]["payload"]["source_projection"]["rule_state"]["year"] = 97
						result.audio[0]["payload"]["state_sha256"] = Bridge.digest(result.audio[0]["payload"]["source_projection"])
						(base as State).rule_state["resources"][Bridge.EMISSION]["intents"][0] = Bridge.digest(result.audio[0])
					"reverse": result.audio.reverse()
			var events: Array[Session.ServiceEvent] = []
			if phase == "load": events.append(_event(harness, _payload(harness)))
			_reject(harness, "complete owned %s batch refuses %s" % [phase, mutation], events)
			check(harness["model"].event_history.size() == before_history, "owned batch rejection leaves actual service history at its before state")
	for mutation: String in ["rename", "clear", "replace", "duplicate", "erase_pending", "rewrite_receipt"]:
		var harness: Dictionary = _with_model(95, 9, true)
		_request(harness)
		var original_draw: Callable = harness["session"].draw_logic
		harness["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
			original_draw.call(base, rng, result)
			var owned: Dictionary = {}
			for intent: Dictionary in result.presentation:
				if intent.get("kind") == "month_start_notice": owned = intent
			match mutation:
				"rename": owned["kind"] = "unrelated_presentation"
				"rewrite_receipt":
					owned["kind"] = "unrelated_presentation"
					(base as State).rule_state["resources"]["month_notice_pending"] = owned.duplicate(true)
				"clear", "replace", "erase_pending":
					result.presentation.erase(owned)
					if mutation == "replace": result.presentation.append({"kind": "unrelated_presentation"})
					if mutation == "erase_pending": (base as State).rule_state["resources"].erase("month_notice_pending")
				"duplicate": result.presentation.append(owned.duplicate(true))
		_reject(harness, "complete September text/audio batch refuses " + mutation, [_event(harness, _payload(harness))])
		check(harness["notice"].history.is_empty() and harness["model"].event_history.is_empty(), "rejected September source batch publishes neither notice nor audio")
	# Other source audio remains legal in both ordinary and quarter frames.
	for quarter: bool in [false, true]:
		var harness: Dictionary = _with_model() if quarter else _harness()
		var original_draw: Callable = harness["session"].draw_logic
		harness["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
			original_draw.call(base, rng, result)
			result.audio.push_front({"kind": "se_play", "command": 14})
			result.audio.append({"kind": "bgm_play", "sequence": 0, "source_context": {"owner": "unrelated source service"}})
		var uploaded: Session.FrameResult = _upload(harness)
		check(uploaded.committed and uploaded.audio.front()["kind"] == "se_play" and uploaded.audio.back()["kind"] == "bgm_play", "legitimate unrelated ordinary/quarter audio survives exact owned batch validation")
	for phase: String in ["load", "ready", "notice"]:
		for mutation: String in ["rename", "clear", "reverse"]:
			if mutation == "reverse" and phase != "ready": continue
			var harness: Dictionary = _with_model(95, 9, true) if phase == "notice" else _with_model()
			if phase == "ready":
				check(_upload(harness).committed, "initial native batch publishes before ready publication attack")
				harness["model"].ready = true
			else: _request(harness)
			var month: MonthStart = harness["controller"].month_start
			var source_frame: int = harness["state"].frame_count
			check(month.prepare_services(harness["state"]).is_empty(), "private native batch prepared for postcommit attack")
			var events: Array[Session.ServiceEvent] = []
			if phase != "ready": events.append(_event(harness, _payload(harness)))
			var committed: Session.FrameResult = harness["session"].step(InputFrame.new(harness["session"].clock.logic_iterations), events)
			check(committed.committed, "postcommit attack starts with committed complete source batch")
			var history: int = harness["model"].event_history.size()
			if phase == "notice":
				for intent: Dictionary in committed.presentation:
					if intent.get("kind") == "month_start_notice":
						if mutation == "rename": intent["kind"] = "unrelated_presentation"
						else: committed.presentation.erase(intent)
						break
			else:
				match mutation:
					"rename": committed.audio[0]["kind"] = "bgm_play"
					"clear": committed.audio.clear()
					"reverse": committed.audio.reverse()
			check(not month.publish_services(committed).is_empty() and committed.committed, "postcommit changed complete batch is terminal, never rollback or success")
			check(harness["model"].event_history.size() == history and (phase != "notice" or harness["notice"].history.is_empty()), "changed publication envelope touches neither native nor notice service history")
			check(month.discard_services(source_frame).is_empty(), "postcommit rejected batch discards only private staging")
	var ready: Dictionary = _with_model()
	check(_upload(ready).committed, "load publishes one backend batch")
	var load: Dictionary = ready["model"].published_batches.front()["intents"][0]["payload"]
	check(load["state_sha256"] == Bridge.digest(load["source_projection"]) and load["source_projection"]["rule_state"]["calendar"][0] == ready["state"].rule_state["calendar"][0], "load receipt binds exact actual projection after authenticated CALROM promotion")
	check(Bridge.digest(ready["model"].prepared_projection) != load["state_sha256"] and ready["model"].prepared_projection["frame_count"] == load["frame_count"], "prepare and post-promotion load are distinct source snapshots in the same native frame")
	ready["model"].ready = true
	check(_step(ready).committed and ready["model"].published_batches.size() == 2, "ready poll/start publish once together, not per operation")
	var final_batch: Array = ready["model"].published_batches.back()["intents"]
	check(final_batch.size() == 2 and final_batch[0]["operation"] == "poll" and final_batch[1]["operation"] == "start", "one backend publication receives exact ordered ready poll/start siblings")
	check(final_batch[0]["payload"]["frame_count"] == final_batch[1]["payload"]["frame_count"] and final_batch[0]["payload"]["state_sha256"] != final_batch[1]["payload"]["state_sha256"] \
		and final_batch[1]["payload"]["source_projection"]["rule_state"]["resources"][Bridge.PENDING]["operation"] == "poll", "same-frame start projection contains its proved ready-poll receipt without normalizing either source snapshot")

func _test_season_fade() -> void:
	var harness: Dictionary = _with_model()
	var state: State = harness["state"]
	var before: Dictionary = HudSeason.view(state.rule_state)
	check(before.get("checker_season") == 0 and before.get("checker_fade") == 255, "June request retains previous spring CLUT until source upload")
	check(_upload(harness).committed, "source quarter upload sets EF")
	check(state.rule_state["resources"].get(HudSeason.KEY) == 239, "EF remains until source frame has low bits3")
	var rng_count: int = harness["session"].rng.draw_count
	for index: int in range(75):
		var old_frame: int = state.frame_count
		var old_fade: int = state.rule_state["resources"][HudSeason.KEY]
		var result: Session.FrameResult = _step(harness)
		check(result.committed and state.mode_step == 1, "actual controller waits while model has no completion")
		var expected: int = ((old_fade + 1) & 255) if (old_frame & 3) == 3 and old_fade >= 239 else old_fade
		check(state.rule_state["resources"][HudSeason.KEY] == expected, "source u8 advances on old frame low bits3 and wraps afterFF")
		var projection: Dictionary = state.projection()
		var view: Dictionary = HudSeason.view(state.rule_state)
		check(view.get("checker_season") == 1 and view.get("checker_fade") == (expected if expected >= 239 else 255), "render consumes committed source fade including final target after wrap")
		for render in range(3): HudSeason.view(state.rule_state)
		check(state.projection() == projection, "render and repeated presentation do not advance native fade")
	check(state.rule_state["resources"][HudSeason.KEY] == 0 and harness["session"].rng.draw_count == rng_count, "source fade reaches zero without RNG")
	state.rule_state["resources"][HudSeason.KEY] = 239
	while state.frame_count & 3 != 3: check(_step(harness).committed, "advance controlled wait to eligible source frame")
	harness["draw_settings"]["failure"] = true
	var snapshot: Dictionary = _checkpoint(harness)
	check(not _step(harness).committed and _checkpoint(harness) == snapshot, "downstream draw rejection rolls back eligible fade/frame/RNG")
	harness["draw_settings"]["failure"] = false
	check(_step(harness).committed and state.rule_state["resources"][HudSeason.KEY] == 240, "same source frame retries eligible fade once")
	var compositor := HudCompositor.new()
	var previous: Image = compositor._get_season_checker_tile(0, 255)
	var ef: Image = compositor._get_season_checker_tile(1, 239)
	check(ef.get_data() == previous.get_data(), "EF counter uses exact previous BGR555 CLUT")
	check(compositor._get_season_checker_tile(1, 255).get_data() != previous.get_data(), "FF uses summer CLUT")
	var result := Session.FrameResult.new()
	result.trace.append({"phase": "mode", "state": {"frame_count": state.frame_count - 1}})
	check(HudSeason.draw_step(state, result).is_empty() and not HudSeason.draw_step(state, result).is_empty(), "duplicate logical source draw is refused")
	for bad: Variant in [{}, true, -1, 256]:
		state.rule_state["resources"][HudSeason.KEY] = bad
		check(not HudSeason.view(state.rule_state)["error"].is_empty(), "malformed fade refuses composition without coercion")
		check(not HudSeason.draw_step(state, Session.FrameResult.new()).is_empty(), "malformed fade refuses logical draw without mutation")
	var september: State = _state(95, 8)
	september.rule_state["day"] = 31
	september.rule_state["player_flags_upper"] |= 4
	check(Calendar.advance_day(september).is_empty() and september.rule_state["month"] == 9 and (september.rule_state["player_flags_upper"] & 4) != 0, "calendar advance preserves amusement bit for source scheduler reset caller")

func _test_music() -> void:
	for row: Array in [[96, 3, false], [95, 6, false], [95, 9, true], [97, 9, true], [95, 12, false]]:
		var harness: Dictionary = _with_model(row[0], row[1], row[2])
		var state: State = harness["state"]
		state.rule_state["player_flags_upper"] |= 4
		var rng_before: Dictionary = harness["session"].rng.checkpoint()
		var loaded: Session.FrameResult = _upload(harness)
		check(loaded.committed and [state.mode_stage, state.mode_step, state.substep] == [0, 1, 0], "quarter load enters native music wait: " + loaded.error)
		if not loaded.committed: continue
		check(loaded.audio.size() == 1 and loaded.audio[0]["operation"] == "load", "source load is the sole initial native audio operation")
		check(loaded.audio[0]["payload"]["request"]["commands"] == [0xc1, 5, 0x71], "quarter C1 then day helper5/71 order")
		check(state.rule_state["resources"]["hud_season_fade"] == 0xef, "every quarter sets source EF")
		check((state.rule_state["player_flags_upper"] & 4) == (0 if row[1] == 9 else 4), "amusement bit clear is September-only")
		var notices: Array = loaded.presentation.filter(func(i): return i.get("kind") == "month_start_notice")
		check(notices.size() == (1 if row[2] else 0), "only actual September-notice branch prepares native text")
		var waited: Session.FrameResult = _step(harness)
		check(waited.committed and state.mode_step == 1 and waited.audio.size() == 1 and waited.audio[0]["status"] == -1, "unready controlled observation waits without sequence start")
		harness["model"].ready = true # Explicit controlled test observation, never production readiness.
		var started: Session.FrameResult = _step(harness)
		check(started.committed and state.mode_stage == 1 and started.audio.size() == 2 \
			and started.audio[0]["operation"] == "poll" and started.audio[1]["operation"] == "start", "ready poll/start preserve same native frame order")
		check(started.audio[-1]["payload"]["request"]["start_commands"] == [0x22, 0x21, 0x2f, 0x200], "source bank0 sequence0 start command order")
		var exited: Session.FrameResult = _step(harness)
		check(exited.committed and state.game_mode == 0x31 and state.rule_state["day_event_index"] == 8, "quarter exits actual FE queue7 to8")
		check(harness["session"].rng.checkpoint() == rng_before, "month source phases consume no RNG")

func _test_refusals() -> void:
	var bridge := Bridge.new()
	for bad: Variant in [null, 1, true, {}, RefCounted.new(), AudioService.new()]:
		check(not bridge.bind_owner(bad).is_empty(), "non-native or malformed service cannot grant quarter capability")
	var unavailable: Dictionary = _harness(_state(95, 6))
	_request(unavailable)
	_reject(unavailable, "real missing native context refuses upload/audio transaction", [_event(unavailable, _payload(unavailable))])
	for fault: String in ["prepare", "malformed", "foreign_binding", "boolean_status", "precommit", "batch", "batch_schema", "batch_hash",
		"schema_dict", "hash_dict", "context_hash_dict", "context_depth_dict", "owner_epoch_dict", "token_dict",
		"validation_schema_dict", "validation_hash_dict"]:
		var harness: Dictionary = _with_model()
		_request(harness)
		harness["model"].fault = fault
		_reject(harness, "native audio hard negative " + fault, [_event(harness, _payload(harness))])
		check(harness["model"].event_history.is_empty(), "rejected load never publishes model audio")
	var reverb: Dictionary = _with_model()
	reverb["model"].fault = "cold_reverb"
	check(_upload(reverb).committed, "old reverb depth may persist during source load request")
	reverb["model"].ready = true
	_reject(reverb, "actual SUMMER start must apply source raw depth56")
	var zero_epoch: Dictionary = _with_model()
	_request(zero_epoch)
	zero_epoch["model"].epoch = 0
	_reject(zero_epoch, "shared native owner requires positive generation/epoch/serial", [_event(zero_epoch, _payload(zero_epoch))])
	for fault: String in ["foreign_profile", "boolean_status", "precommit"]:
		var harness: Dictionary = _with_model()
		check(_upload(harness).committed, "prepare committed load before stale wait test")
		harness["model"].fault = fault
		_reject(harness, "wait refuses " + fault)
	var stale: Dictionary = _with_model()
	check(_upload(stale).committed, "source load before owner epoch change")
	stale["model"].epoch += 1
	_reject(stale, "foreign owner epoch refuses continuation")
	stale = _with_model()
	check(_upload(stale).committed, "source load before bridge reset")
	stale["controller"].month_start.audio.reset()
	_reject(stale, "reset bridge refuses stale pending operation")
	var september: Dictionary = _with_model(95, 9)
	_request(september)
	_reject(september, "September never skips missing actual notice", [_event(september, _payload(september))])
	var rollback: Dictionary = _with_model()
	_request(rollback)
	rollback["draw_settings"]["failure"] = true
	_reject(rollback, "downstream failure rolls back calendar/quarter/audio intents", [_event(rollback, _payload(rollback))])
	check(rollback["model"].event_history.is_empty(), "aborted prepared load leaves old service history unchanged")
	rollback["draw_settings"]["failure"] = false
	check(_step(rollback, [_event(rollback, _payload(rollback))]).committed, "discarded preparation can retry cleanly")

func _test_lifetime_and_shapes() -> void:
	var duplicate: Dictionary = _with_model()
	var loaded: Session.FrameResult = _upload(duplicate)
	var history: int = duplicate["model"].event_history.size()
	check(not duplicate["controller"].month_start.publish_services(loaded).is_empty(), "committed operation cannot be published twice")
	check(duplicate["model"].event_history.size() == history, "duplicate publication adds no audio history")
	duplicate["state"].rule_state["resources"][Bridge.PENDING]["ready"] = true
	var waited: Session.FrameResult = _step(duplicate)
	check(waited.committed and duplicate["state"].mode_step == 1 and not duplicate["model"].started, "an arbitrary rule ready flag cannot replace actual owner observation")
	for key: String in ["bridge_generation", "owner", "request", "token", "status", "source_profile_sha256"]:
		var harness: Dictionary = _with_model()
		check(_upload(harness).committed, "load before malformed pending " + key)
		harness["state"].rule_state["resources"][Bridge.PENDING][key] = {}
		_reject(harness, "malformed pending is structured refusal: " + key)
	var inactive: Dictionary = _with_model()
	_request(inactive)
	inactive["state"].sound_active = false
	_reject(inactive, "inactive source audio phase cannot bypass final native fence", [_event(inactive, _payload(inactive))])
	var rewound: Dictionary = _with_model()
	check(_upload(rewound).committed, "load before unsupported checkpoint rewind")
	rewound["state"].frame_count -= 1
	_reject(rewound, "state-only rewind requires a restored actual native owner")
	var injected: Dictionary = _with_model()
	_request(injected)
	var original_draw: Callable = injected["session"].draw_logic
	injected["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
		original_draw.call(base, rng, result)
		if not result.audio.is_empty(): result.audio.append(result.audio[0].duplicate(true))
	_reject(injected, "duplicate staged native command batch is atomic", [_event(injected, _payload(injected))])
	for field: String in ["request", "status", "operation", "source_profile_sha256", "context"]:
		var corrupted: Dictionary = _with_model()
		_request(corrupted)
		var prior_draw: Callable = corrupted["session"].draw_logic
		corrupted["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
			prior_draw.call(base, rng, result)
			var pending: Dictionary = (base as State).rule_state["resources"].get(Bridge.PENDING, {})
			match field:
				"request": pending["request"]["binding"]["sequence"] = 0xd37d
				"status": pending["status"] = 1
				"operation": pending["operation"] = "start"
				"source_profile_sha256": pending["source_profile_sha256"] = "b".repeat(64)
				"context": result.audio[0]["source_context"] = {}
		_reject(corrupted, "late source metadata corruption refuses before publication: " + field, [_event(corrupted, _payload(corrupted))])
	var changed: Dictionary = _with_model()
	_request(changed)
	var draw: Callable = changed["session"].draw_logic
	var owner: ControlledAudioModel = changed["model"]
	changed["session"].draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
		draw.call(base, rng, result)
		owner.epoch += 1 # Controlled external invalidation before the final fence.
	_reject(changed, "owner invalidated before commit refuses without native publication", [_event(changed, _payload(changed))])
	for fault: String in ["schema_dict", "hash_dict", "token_dict"]:
		var september: Dictionary = _with_model(95, 9, true)
		_request(september)
		september["notice"].fault = fault
		_reject(september, "malformed notice receipt " + fault, [_event(september, _payload(september))])
		check(september["notice"].history.is_empty(), "rejected notice never publishes native text")
	for key: String in ["token", "source_sha256", "payload", "notice_generation"]:
		var september: Dictionary = _with_model(95, 9, true)
		check(_upload(september).committed, "source notice committed before pending corruption")
		september["state"].rule_state["resources"]["month_notice_pending"][key] = {}
		_reject(september, "foreign source notice pending fails shape before owner callback: " + key)
	var publish_failed: Dictionary = _with_model()
	_request(publish_failed)
	var source_frame: int = publish_failed["state"].frame_count
	var month: MonthStart = publish_failed["controller"].month_start
	check(month.prepare_services(publish_failed["state"]).is_empty(), "private source ticket prepares outside Session")
	var completion: Array[Session.ServiceEvent] = [_event(publish_failed, _payload(publish_failed))]
	var committed: Session.FrameResult = publish_failed["session"].step(InputFrame.new(publish_failed["session"].clock.logic_iterations), completion)
	check(committed.committed and publish_failed["state"].mode_step == 1, "pure commit precedes external defensive publication result")
	publish_failed["model"].fault = "publication"
	check(not month.publish_services(committed).is_empty() and committed.committed and publish_failed["model"].event_history.is_empty(), "publication error stays postcommit and cannot be called rollback")
	check(month.discard_services(source_frame).is_empty(), "postcommit failure releases private ticket outside Session")

func _test_year_roll() -> void:
	for year: int in [95, 96, 97]:
		var harness: Dictionary = _harness(_state(year, 12))
		var state: State = harness["state"]
		state.game_mode = 0x30
		state.mode_step = 4
		state.rule_state["allow_multi_week"] = true
		state.rule_state["day"] = 31
		state.rule_state["weekday"] = (source.month(year, 12)["marker"][0] + 30) % 7
		state.rule_state["calendar"] = [_record(year, 12, 173), _record(year + 1, 1, 219)]
		var before: Array = state.rule_state["calendar"].duplicate(true)
		var advanced: Session.FrameResult = _step(harness)
		check(advanced.committed and [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]] == [year + 1, 1, 1], "actual scheduler advances December31 to next-year January1")
		check(state.rule_state["calendar"] == before, "year roll retains source calendar buffers before FE upload")
		state.game_mode = 0x38 # Isolated controller entry after separately tested actual advance.
		state.mode_step = 0
		var uploaded: Session.FrameResult = _upload(harness)
		check(uploaded.committed and state.rule_state["calendar"][0] == before[1], "new-year CALROM promotion uses the correct next-year record")
		check(uploaded.audio.is_empty(), "January ordinary month does not invent June music")
