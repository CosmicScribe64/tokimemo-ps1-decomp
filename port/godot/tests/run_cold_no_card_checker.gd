extends "res://tests/run_interactive_prologue.gd"
## Source-derived cold-route checker ownership and loader-phase regression.
## This is not a runtime capture or a claim about general first-arrival routes.

const Cold = preload("res://flow/cold_no_card_checker.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Math = preload("res://core/native_math.gd")
const NativeGlyphsScript = preload("res://presentation/native_glyphs.gd")
const HudProvider = preload("res://presentation/prologue_hud_source_provider.gd")
const HudProfileSha := "c930a17327e5ad0bdf3b26ab8c20287775b7fc795df240b5f071ed28c4f25f23"
# Integrated successor: accepted V3 plus the separately reviewed installed
# resource reader. The sealed checker2 packet retains its exact V3 binding.
const HudProviderSha := "5c117f2ae624305a82cc128166844827f7afd9c089b5de5b128b2b0544efd499"

class CheckerProvider extends Provider:
	var checker: RefCounted
	var fail_after_checker := false

	func services() -> Dictionary:
		var result: Dictionary = super.services()
		result["overlay_draw"] = overlay_draw
		return result

	func overlay_draw(state: State, result: Session.FrameResult) -> void:
		calls.append("overlay_draw")
		checker.overlay_draw(state, result)
		if fail_after_checker and result.error.is_empty():
			result.error = "injected failure after checker update"

func _route() -> Dictionary:
	return {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"title_new_game":true,
		"no_card_answer":true,"options_visited":false,"retained_game":false,
		"memory_card_load":false,"attract_visited":false,"debug_intervention":false}

func _case(frame_count: int, brightness: int) -> Dictionary:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 12345)
	var entry: Entry = committed_entry(rng)
	var peer := CheckerProvider.new()
	peer.checker = Cold.new()
	var controller := Live.new()
	var context := {"frame_count":frame_count,"system_seed":91,"timer_ticks":0,
		"screen_brightness":brightness,"held_input":0x800,"resident_overlay_lba":0x95ea}
	var begin_error: String = controller.begin(entry,rng,context,{"initial_mascot":0},
		peer.services(),[peer],[peer.publish])
	check(begin_error.is_empty(),"interactive cold-route controller begins")
	var checker: RefCounted = peer.checker
	var prepare_error: String = checker.prepare(controller.session,_route())
	check(prepare_error.is_empty(),"source flow contract owns committed interactive Session")
	return {"rng":rng,"peer":peer,"controller":controller,"checker":checker,
		"entry_frame":frame_count,"brightness":brightness}

func _before_first_loader(record: Dictionary, held: int = 0x800) -> int:
	var controller: Live = record.controller
	var checker: RefCounted = record.checker
	var ticks: int = 67 + int(record.brightness / 4)
	var all_committed := true
	for tick in range(ticks):
		var frame: Session.FrameResult = controller.step(InputFrame.new(tick,held),[])
		if not frame.committed:
			all_committed = false
			break
	var expected_frame: int = Math.u32(record.entry_frame + ticks)
	var owned: Dictionary = controller.state.opening.get(Cold.RECORD,{})
	check(all_committed and controller.state.game_mode == 2 and controller.state.opening.overlay_phase == 0,
		"65-tick hold, brightness fade and separate mode setter reach the resident loader")
	check(controller.state.frame_count == expected_frame and expected_frame == owned.get("first_loader_frame"),
		"first loader frame uses wrapped native frame-count arithmetic")
	check(owned.get("u") == 2 and owned.get("v") == 255 and owned.get("loader_calls") == 0,
		"held exit changes no checker prefix before the two loader callbacks")
	return ticks

func _loader_pair(record: Dictionary, first_tick: int, first_fail_on_second: bool = false) -> bool:
	var controller: Live = record.controller
	var peer: CheckerProvider = record.peer
	var first: Session.FrameResult = controller.step(InputFrame.new(first_tick,0x800),[])
	check(first.committed and controller.state.opening[Cold.RECORD].loader_calls == 1,
		"resident cache-hit start draw commits exactly once")
	var premature_before: Dictionary = controller.state.projection()
	var premature: Dictionary = record.checker.handoff(controller.state)
	check(not str(premature.get("error","")).is_empty() and controller.state.projection() == premature_before,
		"handoff before the required enter callback rejects unchanged")
	if first_fail_on_second:
		peer.fail_after_checker = true
		var before: Dictionary = controller.state.projection()
		var rng_before: Dictionary = record.rng.checkpoint()
		var calls_before: Array[String] = peer.calls.duplicate()
		var failed: Session.FrameResult = controller.step(InputFrame.new(first_tick+1,0x800),[])
		check(not failed.committed and controller.state.projection() == before and record.rng.checkpoint() == rng_before,
			"failed second loader draw rolls back model and RNG")
		check(peer.calls == calls_before and controller.session.clock.logic_iterations == first_tick+1,
			"failed later loader callback rolls participant back and preserves retry tick")
		check(controller.state.opening[Cold.RECORD].loader_calls == 1,
			"failed checker update is removed with the session transaction")
		peer.fail_after_checker = false
	var second: Session.FrameResult = controller.step(InputFrame.new(first_tick+1,0x800),[])
	check(second.committed and controller.state.game_mode == 0x15 and controller.state.opening[Cold.RECORD].loader_calls == 2,
		"resident cache-hit enter draw retries and commits once")
	return true

func _initialize() -> void:
	_test_routes_and_state_binding()
	_test_loader_pairs_and_scroll_import()
	_test_transaction_retry()
	_test_stale_duplicate_foreign_and_bounds()
	_test_exact_opening_field_types()
	_test_handoff_overlay_phase_guard()
	print("PASS: %d cold no-card checker checks" % checks if failures == 0 else "FAIL: %d/%d cold checker checks" % [failures,checks])
	quit(0 if failures == 0 else 1)

func _test_routes_and_state_binding() -> void:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,12345)
	var entry: Entry = committed_entry(rng)
	var peer := CheckerProvider.new()
	peer.checker = Cold.new()
	var controller := Live.new()
	var context := {"frame_count":91,"system_seed":91,"timer_ticks":0,
		"screen_brightness":8,"held_input":0x800,"resident_overlay_lba":0x95ea}
	check(controller.begin(entry,rng,context,{"initial_mascot":0},peer.services(),[peer],[peer.publish]).is_empty(),
		"route validation controller begins without checker ownership")
	var checker: RefCounted = peer.checker
	var session: Session = controller.session
	var initial: Dictionary = session.state.projection()
	var bad_routes: Array[Dictionary] = []
	var missing := _route()
	missing.erase("attract_visited")
	bad_routes.append(missing)
	var extra := _route()
	extra["unverified"] = false
	bad_routes.append(extra)
	var wrong_sha := _route()
	wrong_sha.source_flow_sha256 = "0".repeat(64)
	bad_routes.append(wrong_sha)
	var wrong_type := _route()
	wrong_type.no_card_answer = 1
	bad_routes.append(wrong_type)
	for key: String in ["cold_boot","title_new_game","no_card_answer"]:
		var not_cold := _route()
		not_cold[key] = false
		bad_routes.append(not_cold)
	for key: String in ["options_visited","retained_game","memory_card_load","attract_visited","debug_intervention"]:
		var excluded := _route()
		excluded[key] = true
		bad_routes.append(excluded)
	var rejected_all := true
	for bad: Dictionary in bad_routes:
		if checker.prepare(session,bad).is_empty() or session.state.projection() != initial:
			rejected_all = false
	check(rejected_all,"malformed/source-excluded route observations reject without mutation")
	check(checker.prepare(session,_route()).is_empty() and session.state.opening[Cold.RECORD].u == 2,
		"valid route binds initial 2/255 scroll prefix after rejected observations")
	check(not checker.prepare(session,_route()).is_empty(),"checker cannot acquire a second Session owner")

func _test_loader_pairs_and_scroll_import() -> void:
	for case: Array in [[100,8],[101,8],[100,128],[101,128],[0xfffffff0,128]]:
		var record: Dictionary = _case(case[0],case[1])
		var ticks: int = _before_first_loader(record)
		var first: int = record.controller.state.frame_count
		var expected: int = Math.u32(case[0] + 67 + int(case[1] / 4))
		check(first == expected and first == record.controller.state.opening[Cold.RECORD].first_loader_frame,
			"both frame parities and maximum brightness predict exact first cache-hit frame")
		_loader_pair(record,ticks)
		var state: State = record.controller.state
		var result_before: Dictionary = state.projection()
		var duplicate := Session.FrameResult.new()
		record.checker.overlay_draw(state,duplicate)
		check(not duplicate.error.is_empty() and state.projection() == result_before,
			"duplicate third callback is rejected without state mutation")
		var handoff: Dictionary = record.checker.handoff(state)
		check(handoff.get("error","").is_empty() and handoff.transport.u == 4 and handoff.transport.v == 254,
			"exact two-call transfer hands off u=4/v=254 independent of start parity")
		if case[1] == 128:
			_test_real_hud_scroll_import(handoff.transport)
		var after_handoff: Dictionary = state.projection()
		var after_handoff_call := Session.FrameResult.new()
		record.checker.overlay_draw(state,after_handoff_call)
		check(not after_handoff_call.error.is_empty() and state.projection() == after_handoff,
			"post-handoff loader callback is rejected unchanged")

func _test_real_hud_scroll_import(transport: Dictionary) -> void:
	var root := OS.get_environment("TOKIMEMO_ROOT")
	check(root == "/repo", "test harness supplies the read-only source repository root")
	var script_path := ProjectSettings.globalize_path("res://presentation/prologue_hud_source_provider.gd")
	check(FileAccess.get_sha256(script_path) == HudProviderSha,
		"exact accepted HUD v3 provider source matches its pin")
	if FileAccess.get_sha256(script_path) != HudProviderSha:
		return
	var provider: RefCounted = HudProvider.new()
	var open_error: String = provider.open(root,HudProfileSha)
	check(open_error.is_empty(),"exact V3 HUD profile/resources open for imported scroll regression: " + open_error)
	if not open_error.is_empty():
		return
	var imported: Dictionary = provider.import_scroll({},transport)
	check(imported.get("status") == "scroll_imported" and imported.hud.get("checker_scroll_u") == 4
		and imported.hud.get("checker_scroll_v") == 254
		and imported.hud.get("checker_scroll_phase") == "before_outer_scroll_update"
		and imported.hud.get("checker_scroll_owner") == transport.owner,
		"actual V3 import_scroll consumes the checked post-loader transport")

func _test_transaction_retry() -> void:
	var record: Dictionary = _case(222,8)
	var ticks: int = _before_first_loader(record)
	_loader_pair(record,ticks,true)
	var handoff: Dictionary = record.checker.handoff(record.controller.state)
	check(handoff.get("error","").is_empty() and handoff.transport.u == 4 and handoff.transport.v == 254,
		"failed later frame retries to one exact two-call checker handoff")

func _test_stale_duplicate_foreign_and_bounds() -> void:
	var changed_even: Dictionary = Cold._updated(255,0,100)
	var changed_odd: Dictionary = Cold._updated(255,0,101)
	check(changed_even == {"u":0,"v":0} and changed_odd == {"u":0,"v":255},
		"native u8 helper wraps U and applies V update only for odd frame")
	var record: Dictionary = _case(333,8)
	var ticks: int = _before_first_loader(record)
	var state: State = record.controller.state
	var checker: RefCounted = record.checker
	var original: Dictionary = state.projection()
	state.frame_count = Math.u32(state.frame_count + 1)
	var stale_result := Session.FrameResult.new()
	checker.overlay_draw(state,stale_result)
	check(not stale_result.error.is_empty() and state.opening[Cold.RECORD].loader_calls == 0,
		"stale frame callback is rejected before prefix mutation")
	state.restore(original)
	var record_before: Dictionary = state.projection()
	var malformed: Dictionary = state.opening[Cold.RECORD].duplicate(true)
	malformed.erase("source_flow_sha256")
	state.opening[Cold.RECORD] = malformed
	var malformed_state: Dictionary = state.projection()
	var malformed_result := Session.FrameResult.new()
	checker.overlay_draw(state,malformed_result)
	check(not malformed_result.error.is_empty() and state.projection() == malformed_state,
		"malformed owned record rejects without further mutation")
	state.restore(record_before)
	var foreign := State.new()
	foreign.restore(state.projection())
	var foreign_before: Dictionary = foreign.projection()
	var foreign_result := Session.FrameResult.new()
	checker.overlay_draw(foreign,foreign_result)
	check(not foreign_result.error.is_empty() and foreign.projection() == foreign_before
		and not checker.handoff(foreign).get("error","").is_empty(),
		"foreign state cannot reuse or hand off the checker owner")
	check(ticks == 69,"brightness-8 cold route includes exact pre-loader hold/fade/mode ticks")

func _unowned_session() -> Dictionary:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,12345)
	var entry: Entry = committed_entry(rng)
	var peer := CheckerProvider.new()
	peer.checker = Cold.new()
	var controller := Live.new()
	var context := {"frame_count":91,"system_seed":91,"timer_ticks":0,
		"screen_brightness":8,"held_input":0x800,"resident_overlay_lba":0x95ea}
	var begin_error: String = controller.begin(entry,rng,context,{"initial_mascot":0},
		peer.services(),[peer],[peer.publish])
	check(begin_error.is_empty(),"malformed-field prepare fixture begins")
	return {"controller":controller,"checker":peer.checker}

func _test_exact_opening_field_types() -> void:
	var malformed_values: Array = [{},[],null,false,1.5,"wrong"]
	var fields := {"origin":"interactive-entry-owned-state-v1",
		"resident_overlay_lba":0x95ea,"overlay_phase":0}
	var prepare_rejected := true
	for field: String in fields:
		for bad: Variant in malformed_values:
			var fixture := _unowned_session()
			var session: Session = fixture.controller.session
			var checker: RefCounted = fixture.checker
			session.state.opening[field] = bad
			var before: Dictionary = session.state.projection()
			var error: String = checker.prepare(session,_route())
			if error.is_empty() or session.state.projection() != before:
				prepare_rejected = false
	check(prepare_rejected,"prepare rejects dictionary/array/null/bool/float/string opening field values without mutation")

	var callback_rejected := {"origin":true,"resident_overlay_lba":true,"overlay_phase":true}
	for field: String in fields:
		for bad: Variant in malformed_values:
			var fixture: Dictionary = _case(500,8)
			_before_first_loader(fixture)
			var state: State = fixture.controller.state
			# OverlayLoader has entered its resident draw callback at the exact
			# source phase, but the callback has not yet mutated checker state.
			state.opening["overlay_phase"] = 3
			state.opening[field] = bad
			var before: Dictionary = state.projection()
			var result := Session.FrameResult.new()
			fixture.checker.overlay_draw(state,result)
			if result.error.is_empty() or not result.presentation.is_empty() or state.projection() != before:
				callback_rejected[field] = false
	for field: String in callback_rejected:
		check(callback_rejected[field],"overlay callback rejects malformed " + field + " with no presentation or state mutation")

func _test_handoff_overlay_phase_guard() -> void:
	var fixture: Dictionary = _case(700,8)
	var ticks: int = _before_first_loader(fixture)
	_loader_pair(fixture,ticks)
	var state: State = fixture.controller.state
	var original_phase: Variant = state.opening.overlay_phase
	check(original_phase is int and original_phase == 0,
		"two actual loader callbacks leave handoff at integer overlay phase zero")
	var malformed_phases: Array = [3,{},[],null,false,0.0,"0"]
	var rejected_all := true
	for bad: Variant in malformed_phases:
		state.opening.overlay_phase = bad
		var before: Dictionary = state.projection()
		var rejected: Dictionary = fixture.checker.handoff(state)
		if str(rejected.get("error","")).is_empty() or state.projection() != before:
			rejected_all = false
		state.opening.overlay_phase = original_phase
	check(rejected_all,"handoff rejects stale/malformed phase values unchanged after both real callbacks")
	var valid: Dictionary = fixture.checker.handoff(state)
	check(str(valid.get("error","")).is_empty() and valid.get("transport",{}).get("u") == 4
		and valid.get("transport",{}).get("v") == 254,
		"restoring exact integer phase zero permits the valid checker handoff")
