extends RefCounted
## Invented public cases; original witness checks live outside res://.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Fixture = preload("res://tests/rules/run_rule_tests.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")
const Weekend = preload("res://rules/week_end.gd")
const Controller = preload("res://scenes/school_day_controller.gd")

var failures: Array[String] = []

func check(ok: bool, message: String) -> void:
	if not ok:
		failures.append(message)

func _empty(_state: State, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

func _dispatch(base: State, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	Ordinary.dispatch(base, result)

func _weekend(base: State, _input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	Weekend.step(base, rng, result)

func fixture() -> Session:
	var session: Session = Session.new()
	var state: State = Fixture.new().synthetic_state()
	state.rule_state["day"] = 5
	state.rule_state["weekday"] = 2
	state.rule_state["params"][0] = 90 << 16
	state.rule_state["day_events"][0] = [0, 0, 0, 15]
	state.rule_state["day_events"][1] = [0xfb, 0, 0, 0]
	state.rule_state["day_events"][2] = [0, 0, 0, 15]
	state.rule_state["day_event_index"] = 1
	state.reset_game_mode_fields(0x31)
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	session.router.bind_mode(0x31, _dispatch)
	session.router.bind_mode(0x32, _weekend)
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty
	return session

func run() -> Array[String]:
	failures.clear()
	var session: Session = fixture()
	var state: State = session.state as State
	state.rule_state["girls"][0]["affection_word"] = (150 << 16) | 13
	state.rule_state["girls"][0]["flags"] |= 0x30000
	var result: Session.FrameResult = session.step(InputFrame.new(0), [])
	check(result.committed and state.game_mode == 0x40, "day-end dispatches next school day")
	check(state.rule_state["day"] == 6 and state.rule_state["day_event_index"] == 2, "day-end advances calendar and queue once")
	check(state.rule_state["girls"][0]["affection_word"] == (128 << 16) | 13, "girl clamp preserves fractional bits")
	check((state.rule_state["girls"][0]["flags"] & 0x30000) == 0, "low resentment rearms native encounter/rumor flags")
	check(session.rng.draw_count == 0, "ordinary day end consumes no invented RNG")
	for unsupported: int in [0x10, 0x20, 0x32, 0x33, 0x44, 0x46]:
		session = fixture()
		state = session.state as State
		state.rule_state["day_events"][2] = [unsupported, 0, 0, 0]
		var before: String = session.state_hash()
		result = session.step(InputFrame.new(0), [])
		check(not result.committed and not result.error.is_empty() and session.state_hash() == before, "unsupported branch0x%02x rolls back nested state and calendar" % unsupported)
	for resentment: int in [61, 76, 91]:
		session = fixture()
		state = session.state as State
		state.rule_state["girls"][0]["syoushin_word"] = resentment << 16
		var before: String = session.state_hash()
		result = session.step(InputFrame.new(0), [])
		check(not result.committed and session.state_hash() == before, "reachable native girl branch stops before progression")
	session = fixture()
	state = session.state as State
	state.rule_state["day_events"][1] = [0xfc, 0, 0, 0]
	state.reset_game_mode_fields(0x32)
	for index: int in range(3):
		result = session.step(InputFrame.new(index), [])
		check(result.committed, "healthy week-end stage%d commits" % index)
	check(state.game_mode == 0x31 and state.rule_state["day_event_index"] == 2, "healthy week-end returns to next queue entry")
	check(session.rng.draw_count == 0 and state.rule_state["day"] == 5, "healthy week-end adds neither random draws nor calendar day")
	session = fixture()
	state = session.state as State
	state.reset_game_mode_fields(0x32)
	state.mode_stage = 1
	state.rule_state["params"][0] = 0
	var before: String = session.state_hash()
	result = session.step(InputFrame.new(0), [])
	check(not result.committed and session.state_hash() == before and session.rng.draw_count == 0, "health-dialogue stop restores consumed RNG and mode timer")
	session = fixture()
	state = session.state as State
	state.rule_state["day_events"][1] = [0x32, 0, 0, 0]
	state.rule_state["day_events"][2] = [0x32, 1, 0, 0]
	state.rule_state["day_events"][3] = [0, 7, 0, 15]
	state.rule_state["day_event_done_bits"] = 3
	result = session.step(InputFrame.new(0), [])
	check(result.committed and state.rule_state["day_event_index"] == 3 and state.game_mode == 0x40, "birthday done-bit checks sequentially re-read the advanced queue record")
	session = fixture()
	state = session.state as State
	state.rule_state["day_events"][1] = [0x32, 0, 0, 0]
	state.rule_state["day_events"][2] = [0, 7, 0, 15]
	state.rule_state["day_event_done_bits"] = 3
	result = session.step(InputFrame.new(0), [])
	check(result.committed and state.rule_state["day_event_index"] == 3 and state.game_mode == 0x34, "native second birthday check uses girl byte even when advanced type differs")
	_test_cache()
	_test_counter_width()
	_test_profile_snapshot_bounds()
	return failures

func _test_profile_snapshot_bounds() -> void:
	var state: State = fixture().state as State
	for index: int in range(Ordinary.PROGRESS_STATE_BYTES):
		state.rule_state["progress_state"][index] = (index * 7 + 13) & 255
	var prefix: Array = state.rule_state["progress_state"].slice(0, Ordinary.PARAMETER_SNAPSHOT_START)
	var mirror_source: Array = state.rule_state["progress_state"].slice(0, Ordinary.MIRROR_SOURCE_BYTES)
	check(Ordinary.snapshot_parameters(state).is_empty(), "profile snapshot accepts invented exact native bounds")
	check(state.rule_state["progress_state"].slice(0, Ordinary.PARAMETER_SNAPSHOT_START) == prefix, "captured profile/name prefix remains untouched")
	check(state.rule_state["progress_state"].slice(Ordinary.MIRROR_SNAPSHOT_START) == mirror_source, "source-proven first32 progress bytes refresh the native mirrored tail")
	state.rule_state["params"][0] += 17
	check(Ordinary.snapshot_parameters(state).is_empty(), "parameter-only updates preserve immutable-profile signature")
	for mutation: String in ["short_progress", "long_progress", "bad_byte", "short_params", "bad_word", "changed_name", "changed_flags", "changed_mirror", "changed_mirror_source"]:
		var damaged: State = State.new()
		damaged.restore(state.projection())
		match mutation:
			"short_progress": damaged.rule_state["progress_state"].pop_back()
			"long_progress": damaged.rule_state["progress_state"].append(0)
			"bad_byte": damaged.rule_state["progress_state"][0] = 256
			"short_params": damaged.rule_state["params"].pop_back()
			"bad_word": damaged.rule_state["params"][0] = 2147483648
			"changed_name": damaged.rule_state["progress_state"][Ordinary.PROFILE_SNAPSHOT_START] ^= 1
			"changed_flags": damaged.rule_state["player_flags"] ^= 1
			"changed_mirror": damaged.rule_state["progress_state"][Ordinary.MIRROR_SNAPSHOT_START] ^= 1
			"changed_mirror_source": damaged.rule_state["progress_state"][0] ^= 1
		var before: Dictionary = damaged.projection()
		check(not Ordinary.snapshot_parameters(damaged).is_empty() and damaged.projection() == before, "invalid/edited profile rejects before any snapshot write: " + mutation)
	var restored: State = State.new()
	restored.restore(state.projection())
	check(restored.rule_state[Ordinary.PROFILE_HASH_FIELD] == state.rule_state[Ordinary.PROFILE_HASH_FIELD] and restored.to_words() == state.to_words(), "immutable-profile guard lives in projected/restorable/hashed rule state")
	var session: Session = fixture()
	var guarded: State = session.state as State
	check(Ordinary.snapshot_parameters(guarded).is_empty(), "rollback fixture anchors valid source mirror")
	guarded.rule_state["progress_state"][Ordinary.MIRROR_SNAPSHOT_START] ^= 1
	guarded.reset_game_mode_fields(0x32)
	var before_hash: String = session.state_hash()
	var result: Session.FrameResult = session.step(InputFrame.new(0), [])
	check(not result.committed and "immutable profile edit" in result.error and session.state_hash() == before_hash, "tampered mirror fails the native snapshot site with full transaction rollback")

func _test_counter_width() -> void:
	var session: Session = fixture()
	var state: State = session.state as State
	var controller: Controller = Controller.new(Fixture.new().synthetic_tables())
	check(controller.attach(session, state).is_empty(), "counter-width case attaches controller")
	state.reset_game_mode_fields(0x40)
	state.mode_stage = 1
	state.rule_state["day_event_index"] = 0
	state.rule_state["actor_state"] = {"96": {"id": 96, "flags": 0, "sequence_slot": 0, "sequence": {"counter": 65536, "active": false}}}
	var result: Session.FrameResult = session.step(InputFrame.new(0), [])
	var condition_word: int = 0
	for byte: int in range(4):
		condition_word |= state.rule_state["progress_state"][0x58 + byte] << (8 * byte)
	check(result.committed and condition_word == state.rule_state["params"][0], "snapshot award uses the same signed16 counter narrowing as native result logic")

func _test_cache() -> void:
	var session: Session = fixture()
	var state: State = session.state as State
	var tables: Dictionary = Fixture.new().synthetic_tables()
	var controller: Controller = Controller.new(tables)
	check(controller.attach(session, state).is_empty(), "controller attaches continuation modes")
	state.reset_game_mode_fields(0x40)
	state.rule_state["day_event_index"] = 0
	state.rule_state["background_cache"] = [tables["weekday_background_lbas"][0], 999, 0, 0]
	state.rule_state["background_show"] = 1
	var result: Session.FrameResult = session.step(InputFrame.new(0), [])
	check(result.committed and state.substep == 2 and state.rule_state["background_show"] == 0, "native cached background selects its exact slot without waiting for CD")
	check(result.presentation.size() == 1 and result.presentation[0]["kind"] == "school_day_cached_background", "cache hit emits an explicit presentation request")
