extends "res://tests/run_new_game_entry.gd"

const Adapter = preload("res://flow/new_game_state_adapter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const Memory = preload("res://services/native_vab_memory.gd")


func _initialize() -> void:
	print("--- Running Connected Opening Route Test Suite ---")

	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 19950404)

	# =========================================================================
	# Step 1: Committed Name Entry
	# =========================================================================
	var entry := Entry.new()
	entry.begin(invented_tables(), invented_content(), rng)
	finish_names(entry, rng, true)

	for kind in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)

	check(entry.stage == 2 and entry.step_id == 5, "Name entry reached committed confirmation stage (stage 2, step 5)")

	# =========================================================================
	# Step 2: Fresh Route State Adapter
	# =========================================================================
	var pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
	var initial_mascot: int = 0
	var opening_pack: Dictionary = {}
	if not pack_path.is_empty():
		var json_text: String = FileAccess.get_file_as_string(pack_path + "/opening.json")
		if not json_text.is_empty():
			opening_pack = JSON.parse_string(json_text)
			if opening_pack.has("constants") and opening_pack.constants.has("initial_mascot"):
				initial_mascot = int(opening_pack.constants.initial_mascot)

	var adapter_outcome = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot})
	check(adapter_outcome.error.is_empty(), "State adapter successfully materialized fresh route: " + adapter_outcome.error)
	var state: State = adapter_outcome.state
	check(state != null, "State adapter produced valid State object")
	check(state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 0, "Initial game mode is 0x15 stage 0 step 0")
	check(state.rule_state.year == 95 and state.rule_state.month == 4 and state.rule_state.day == 4, "Initial rule date is 1995-04-04")
	check(state.opening.hud_mascot_choice == initial_mascot, "Initial mascot matches authenticated opening constant")
	check(not state.opening.command_ready, "Command ready is false at beginning of opening")

	# =========================================================================
	# Step 3: Prologue Arrival (Stage 0) with Native VAB Memory
	# =========================================================================
	var memory := Memory.new()
	var service_calls: Array = []
	var active_bank_role: String = ""
	var bank_installed: Dictionary = {
		"prologue_arrival": false,
		"normal_gameplay": false,
	}

	var install_bank_role = func(role: String) -> bool:
		active_bank_role = role
		if not opening_pack.is_empty() and opening_pack.has("bank_roles") and opening_pack.bank_roles.has(role):
			var assets: Dictionary = opening_pack.bank_roles[role].assets
			var se_h: Dictionary = assets["se_header"]
			var se_b: Dictionary = assets["se_body"]
			var mu_h: Dictionary = assets["music_header"]
			var mu_b: Dictionary = assets["music_body"]

			var se_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-se_header.bin").slice(0, int(se_h.size))
			var se_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-se_body.bin").slice(0, int(se_b.size))
			var mu_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-music_header.bin").slice(0, int(mu_h.size))
			var mu_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-music_body.bin").slice(0, int(mu_b.size))

			var res0 = memory.install(0, 0x1010, 0x10000, se_header_bytes, se_body_bytes, {"header_sha256": se_h.sha256, "body_sha256": se_b.sha256})
			var res1 = memory.install(1, 0x11010, 0x28000, mu_header_bytes, mu_body_bytes, {"header_sha256": mu_h.sha256, "body_sha256": mu_b.sha256})
			if res0.installed and res1.installed:
				bank_installed[role] = true
				return true
			return false
		else:
			bank_installed[role] = true
			return true

	var cd_sync_done: bool = true
	var services: Dictionary = {
		"bank_load_start": func(s, role, r):
			service_calls.append(["bank_load_start", role])
			install_bank_role.call(role),
		"bank_poll": func(s, r):
			service_calls.append("bank_poll")
			if s.game_mode == 0x15:
				return 1 if bank_installed["prologue_arrival"] else 0
			else:
				return 1 if bank_installed["normal_gameplay"] else 0,
		"bank_start": func(s, a, b, r):
			service_calls.append(["bank_start", a, b]),
		"scene_reset": func(s, r):
			service_calls.append("scene_reset"),
		"background_read": func(s, lba, arg, r, retry = false):
			service_calls.append(["bg_read", lba, arg, retry]),
		"cd_sync": func(s, r):
			service_calls.append("cd_sync")
			return 1 if cd_sync_done else 0,
		"background_decode": func(s, arg, r):
			service_calls.append(["bg_decode", arg])
			return 0,
		"actor_read": func(s, count, dest, lba, r):
			service_calls.append(["actor_read", count, dest, lba]),
		"actor_palette_setup": func(s, r):
			service_calls.append("actor_palette_setup"),
		"line_start": func(s, line, r):
			service_calls.append(["line_start", line]),
		"line_run": func(s, r):
			service_calls.append("line_run")
			s.substep += 1,
		"arrival_draw": func(s, arg, r):
			service_calls.append(["arrival_draw", arg]),
		"outer_draw": func(s, a = null, b = null):
			service_calls.append(["outer_draw", a, b]),

		# Command reload services
		"setup_reset": func(s, r):
			service_calls.append("setup_reset")
			s.next_step(),
		"hud_read_start": func(s, n, d, l, r):
			service_calls.append(["hud_read_start", n, d, l]),
		"calendar_upload": func(s, y, m, b, r):
			service_calls.append(["calendar_upload", y, m, b]),
		"mascot_set": func(s, n, on, r):
			service_calls.append(["mascot_set", n, on]),
		"command_build": func(s, r):
			service_calls.append("command_build")
			s.next_step(),
		"room_finish": func(s, r):
			service_calls.append("room_finish")
			s.set_stage(1),
		"frame_draw": func(s, title = false, r = null):
			service_calls.append(["frame_draw", title]),
	}

	# Step 0: bank_load_start("prologue_arrival")
	var res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty(), "Prologue step 0 succeeded: " + res.error)
	check(state.mode_step == 1 and state.substep == 0, "Advanced to step 1 substep 0")
	check(bank_installed["prologue_arrival"], "Prologue arrival VAB sound bank installed into software sound RAM")

	# Step 1, Substep 0: scene reset
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue step 1 substep 0 reset scene and advanced to substep 1")

	# Step 1, Substep 1: bank poll & start
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 2, "Prologue step 1 substep 1 confirmed bank ready and started bg read")

	# Step 1, Substep 2: timer advance (17 frames)
	while state.substep == 2:
		Arrival.step(state, rng, 0, services)
	check(state.substep == 3, "Prologue step 1 substep 2 timer elapsed and advanced to substep 3")

	# Step 1, Substep 3: CD sync & bg decode & actor read
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 4, "Prologue step 1 substep 3 decoded background and started actor read")

	# Step 1, Substep 4: CD sync & actor palette setup
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.mode_step == 2 and state.substep == 0, "Prologue step 1 substep 4 completed and advanced to step 2")

	# Step 2, Substep 0: line start
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue step 2 substep 0 started dialogue line")

	# Step 2, Substep 1: line run
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 2, "Prologue step 2 substep 1 completed line execution")

	# Step 2, Substep 2: line timer / finish skip via START (0x800)
	res = Arrival.step(state, rng, 0x800, services)
	check(res.error.is_empty() and state.mode_stage == 21, "START button pressed transitioned to prologue finish stage 21")

	# =========================================================================
	# Step 4: Prologue Finish (Stage 21) -> Command Screen Transition
	# =========================================================================
	# Substep 0: loads normal gameplay bank (COM_MIX + SPRING)
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue finish substep 0 initiated normal gameplay bank install")
	check(bank_installed["normal_gameplay"], "Normal gameplay bank COM_MIX + SPRING successfully installed into software sound RAM")

	# Substep 1: switches mode to 0x34 stage 0xFF
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty(), "Prologue finish substep 1 succeeded: " + res.error)
	check(state.game_mode == 0x34 and state.mode_stage == 0xff and state.mode_step == 0, "Transitioned to command screen mode 0x34 stage FF step 0")

	# =========================================================================
	# Step 5: Command Screen Mode Stage Reload (0x34 Stage 0xFF)
	# =========================================================================
	# Step 0: setup reset
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 1 and state.substep == 0, "Command reload step 0 reset setup and advanced to step 1")

	# Step 1, Substep 0: bank poll (verified ready!) -> bank start
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.substep == 1, "Command reload step 1 verified normal gameplay bank ready and advanced")

	# Step 1, Substep 1: hud read start
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.substep == 2, "Command reload step 1 started HUD read")

	# Step 1, Substep 2: CD sync -> calendar upload
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 2, "Command reload step 1 completed calendar upload and advanced to step 2")
	check((state.rule_state.menu_mode & ~3) == 0 and state.opening.D_800B5BC8 == 0xfe, "Calendar upload applied exact menu mode mask and D_800B5BC8")

	# Step 2: command build
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 3, "Command reload step 2 completed command build and advanced to step 3")

	# Step 3: room finish -> mode_stage 1!
	res = Reload.step(state, services)
	check(res.error.is_empty(), "Command reload step 3 room finish executed without error")
	check(state.mode_stage == 1, "Command screen transitioned to Stage 1 (April 4 Command Screen Ready)")

	# =========================================================================
	# Step 6: Final Destination Invariants
	# =========================================================================
	check(state.game_mode == 0x34, "Final game mode is 0x34 (COMMAND_SCREEN)")
	check(state.mode_stage == 1, "Final mode stage is 1 (Active room command menu)")
	check(state.rule_state.year == 95, "Rule state year is 95")
	check(state.rule_state.month == 4, "Rule state month is 4 (April)")
	check(state.rule_state.day == 4, "Rule state day is 4 (April 4)")

	# Sound RAM verification
	if not opening_pack.is_empty():
		check(memory.banks.has(0) and memory.banks.has(1), "Both sound bank slots (SE COM_MIX, BGM SPRING) resident in sound RAM")
		check(memory.banks[0].address == 0x1010 and memory.banks[0].sample_bytes == 0xf3c0, "SE bank slot 0 has COM_MIX at 0x1010 (62,400 bytes)")
		check(memory.banks[1].address == 0x11010 and memory.banks[1].sample_bytes == 0x26d10, "BGM bank slot 1 has SPRING at 0x11010 (158,992 bytes)")

	# Transaction rollback verification
	var checkpoint: Dictionary = state.projection()
	state.opening.screen_brightness = 200
	state.restore(checkpoint)
	check(state.projection() == checkpoint, "Full connected route state supports deterministic transaction rollback")

	print("--------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d connected opening route checks" % checks)
	quit(0 if failures == 0 else 1)
