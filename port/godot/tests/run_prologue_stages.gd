extends "res://tests/run_new_game_entry.gd"

const Adapter = preload("res://flow/new_game_state_adapter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const AudioService = preload("res://services/audio_service.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")

const EXPECTED_STAGE_COUNT: int = 22
const EXPECTED_STATIC_LINE_COUNT: int = 43
const DYNAMIC_LINE_BIRTHDAY: int = 100
const DYNAMIC_LINE_BLOOD: int = 101
const PAD_START_MASK: int = 0x0800
const STAGE_FINISH: int = 21
const MODE_COMMAND_SCREEN: int = 0x34
const STAGE_RELOAD: int = 0xff


func _initialize() -> void:
	print("--- Running Prologue Stages (0..21) Test Suite ---")

	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 19950404)

	var entry: Entry = Entry.new()
	entry.begin(invented_tables(), invented_content(), rng)
	finish_names(entry, rng, true)
	for kind: String in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)
	check(entry.stage == 2 and entry.step_id == 5, "Name entry committed at stage 2 step 5")

	var pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	var receipt_path: String = OS.get_environment("TOKIMEMO_WITNESS_RECEIPT")
	var audio_svc: AudioService = AudioService.new()
	if not pack_path.is_empty():
		var cfg_err: String = audio_svc.configure_opening_pack(pack_path, receipt_path)
		check(cfg_err.is_empty(), "AudioService configured opening pack and witness receipt: " + cfg_err)

	var initial_mascot: int = int(audio_svc.opening_pack.get("constants", {}).get("initial_mascot", 0))
	var adapter_out: Adapter.Outcome = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot})
	check(adapter_out.error.is_empty(), "StateAdapter produced initial prologue state: " + adapter_out.error)
	var state: State = adapter_out.state

	# Verify PAD_START on stage 0 step 0 is evaluated after stage 0 step 0 advances mode_step to 1 (matching outer C handler)
	var service_log: Array = []
	var services: Dictionary = PlayableBoot.build_opening_services(audio_svc, service_log)
	var step0_res = Arrival.step(state, rng, PAD_START_MASK, services)
	check(step0_res.error.is_empty() and state.mode_stage == STAGE_FINISH and state.mode_step == 0 and state.substep == 0, "PAD_START during stage 0 step 0 is evaluated after step 0 advances, skipping to stage 21")

	# Reset for full non-skip 22-stage progression
	state = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot}).state
	service_log.clear()
	audio_svc = AudioService.new()
	if not pack_path.is_empty():
		audio_svc.configure_opening_pack(pack_path, receipt_path)
	services = PlayableBoot.build_opening_services(audio_svc, service_log)

	var visited_stages: Array[int] = []
	var started_lines: Array[int] = []
	var bank_loads: Array[String] = []
	var bgm_starts: Array = []
	var audio_commands: Array[int] = []

	var orig_line_start: Callable = services["line_start"]
	services["line_start"] = func(s: State, line_id: int, r) -> void:
		started_lines.append(line_id)
		orig_line_start.call(s, line_id, r)

	var orig_bank_load: Callable = services["bank_load_start"]
	services["bank_load_start"] = func(s: State, role: String, r) -> void:
		bank_loads.append(role)
		orig_bank_load.call(s, role, r)

	var orig_bank_start: Callable = services["bank_start"]
	services["bank_start"] = func(s: State, bank: int, seq: int, r) -> void:
		bgm_starts.append([bank, seq])
		orig_bank_start.call(s, bank, seq, r)

	var orig_audio_cmd: Callable = services["audio_command"]
	services["audio_command"] = func(s: State, cmd: int, r) -> void:
		audio_commands.append(cmd)
		orig_audio_cmd.call(s, cmd, r)

	var total_steps: int = 0
	while state.game_mode == 0x15 and total_steps < 4096:
		if visited_stages.is_empty() or visited_stages[-1] != state.mode_stage:
			visited_stages.append(state.mode_stage)
		var res = Arrival.step(state, rng, 0, services)
		check(res.error.is_empty(), "Prologue step %d (stage %d step %d substep %d): %s" % [total_steps, state.mode_stage, state.mode_step, state.substep, res.error])
		if not res.error.is_empty():
			break
		state.frame_count += 1
		total_steps += 1

	var expected_stages: Array[int] = []
	for stage_idx: int in range(EXPECTED_STAGE_COUNT):
		expected_stages.append(stage_idx)
	check(visited_stages == expected_stages, "Normal non-skip progression visited all 22 stages 0..21 in exact order: " + str(visited_stages))
	check(state.game_mode == MODE_COMMAND_SCREEN and state.mode_stage == STAGE_RELOAD and state.mode_step == 0, "Stage 21 finish transitioned to 0x34 stage 0xFF step 0")

	var expected_lines: Array[int] = []
	for line_idx: int in range(26):
		expected_lines.append(line_idx)
	expected_lines.append(DYNAMIC_LINE_BIRTHDAY)
	expected_lines.append(26)
	expected_lines.append(DYNAMIC_LINE_BLOOD)
	for line_idx: int in range(27, EXPECTED_STATIC_LINE_COUNT):
		expected_lines.append(line_idx)
	check(started_lines == expected_lines, "All 43 static dialogue lines (0..42) and 2 dynamic Shiori lines (100, 101) executed in exact source order")
	check(bank_loads == ["prologue_arrival", "prologue_rei", "prologue_shiori", "normal_gameplay"], "All 4 prologue bank load transitions executed in order")
	check(bgm_starts == [[0, 0], [1, 0], [1, 0], [0, 1]], "All 4 BGM sequence starts executed in order")
	check(audio_commands == [0x0605, 0x0503, 5, 0xb1, 5, 0xc1, 0x0604, 5, 0xd1, 5, 0xd1], "All SE and BGM fade/stop commands emitted in exact stage order")
	check(state.opening.get("prologue_petals", []).size() == 8, "Stage 1/2 cherry blossom petals initialized and updated with 8 actors")

	# Verify mid-prologue PAD_START skip from Stage 8 to Stage 21
	var skip_state: State = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot}).state
	var skip_steps: int = 0
	while skip_state.game_mode == 0x15 and skip_state.mode_stage < 8 and skip_steps < 2000:
		Arrival.step(skip_state, rng, 0, services)
		skip_state.frame_count += 1
		skip_steps += 1
	check(skip_state.mode_stage == 8, "Reached stage 8 before testing mid-prologue START skip")
	var skip_res = Arrival.step(skip_state, rng, PAD_START_MASK, services)
	check(skip_res.error.is_empty() and skip_state.mode_stage == STAGE_FINISH and skip_state.substep == 0, "PAD_START at stage 8 skipped directly to stage 21 substep 0")

	# Verify transaction rollback on service failure during stage 4
	var rollback_state: State = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot}).state
	while rollback_state.game_mode == 0x15 and rollback_state.mode_stage < 4:
		Arrival.step(rollback_state, rng, 0, services)
		rollback_state.frame_count += 1
	var before_proj: Dictionary = rollback_state.projection()
	var before_rng: Dictionary = rng.checkpoint()
	var failing_services: Dictionary = services.duplicate()
	failing_services["outer_draw"] = func(_s: State, _a: Variant = null, _b: Variant = null) -> void:
		_b.error = "synthetic stage 4 draw failure"
	var fail_res = Arrival.step(rollback_state, rng, 0, failing_services)
	check(not fail_res.error.is_empty() and rollback_state.projection() == before_proj and rng.checkpoint() == before_rng, "Service failure in stage 4 rolls back both state and RNG")

	print("--------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d prologue stages checks" % checks)
	quit(0 if failures == 0 else 1)
