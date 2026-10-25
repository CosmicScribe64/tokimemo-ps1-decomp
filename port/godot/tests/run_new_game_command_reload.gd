extends SceneTree

const State = preload("res://rules/new_game_route_state.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")

var checks: int = 0
var failures: int = 0
var calls: Array = []
var bank_status: Variant = 0
var cd_status: Variant = 0
var fail_draw: bool = false


func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL " + label)


func _initialize() -> void:
	var state := State.new()
	state.game_mode = 0x34
	state.mode_stage = 255
	state.rule_state = {
		"year": 95,
		"month": 4,
		"menu_mode": 7,
	}
	state.opening = {
		"screen_brightness": 0,
		"hud_mascot_choice": 255,
	}

	var services: Dictionary = {
		"setup_reset": func(s, r):
			calls.append("reset")
			s.next_step(),
		"bank_poll": func(s, r):
			calls.append("poll")
			return bank_status,
		"bank_start": func(s, a, b, r):
			calls.append(["bank", a, b]),
		"hud_read_start": func(s, n, d, l, r):
			calls.append(["read", n, d, l]),
		"cd_sync": func(s, r):
			calls.append("sync")
			return cd_status,
		"calendar_upload": func(s, y, m, b, r):
			calls.append(["calendar", y, m, b]),
		"mascot_set": func(s, n, on, r):
			calls.append(["mascot", n, on]),
		"command_build": func(s, r):
			calls.append("build")
			s.next_step(),
		"room_finish": func(s, r):
			calls.append("room")
			s.set_stage(1),
		"frame_draw": func(s, title, r):
			calls.append(["frame", title])
			if fail_draw:
				r.error = "synthetic draw failure",
		"outer_draw": func(s, r):
			pass
	}

	check(not Reload.step(state, {}).error.is_empty() and state.mode_step == 0, "unbound services reject before mutation")
	check(Reload.step(state, services).error.is_empty() and state.mode_step == 1 and state.mode_step_timer == 0, "reset owns next-step timer reset")

	calls.clear()
	Reload.step(state, services)
	check(calls == ["poll", ["frame", false]] and state.substep == 0, "unready actual bank status preserves wait")

	bank_status = 1
	calls.clear()
	Reload.step(state, services)
	check(calls == ["poll", ["bank", 0, 0], ["frame", false]] and state.substep == 1, "ready bank starts exact bank/sequence")

	calls.clear()
	Reload.step(state, services)
	check(calls == [["read", 149, 0x801a0000, 0x9b94], ["frame", false]] and state.substep == 2, "native HUD read begins separately")

	calls.clear()
	Reload.step(state, services)
	check(state.mode_step == 1 and state.substep == 2 and calls == ["sync", ["frame", false]], "incomplete CD preserves wait")

	cd_status = 1
	calls.clear()
	Reload.step(state, services)
	check(state.mode_step == 2 and state.rule_state.menu_mode == 3 and state.opening.D_800B5BC8 == 254, "calendar upload advances with exact source scalars")
	check(calls == ["sync", ["calendar", 95, 4, 0], ["frame", true]] and state.opening.screen_brightness == 4, "postdispatch step enables title and brightness immediately")

	var before: Dictionary = state.projection()
	fail_draw = true
	check(not Reload.step(state, services).error.is_empty() and state.projection() == before, "late service failure rolls back opening and rule state")

	fail_draw = false
	Reload.step(state, services)
	check(state.mode_step == 3, "command build owns step advance")

	Reload.step(state, services)
	check(state.mode_stage == 1 and state.mode_step == 0, "room finish owns command-ready transition")

	print(("PASS" if failures == 0 else "FAIL") + ": %d command reload checks" % checks)
	quit(0 if failures == 0 else 1)
