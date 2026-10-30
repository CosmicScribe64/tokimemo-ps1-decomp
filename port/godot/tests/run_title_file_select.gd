extends "res://tests/run_new_game_entry.gd"
## Boot path title -> file select -> NAME_ENT (flow/title_file_select.gd)
## against the original. Fixture tests/fixtures/title_file_select_golden.json
## comes from tools/port/title_filesel_golden.py over the emulator runs in
## .asset-cache/emu/title-filesel-golden-v1 (numbers only). For every run the
## same pad input must give the same row stream (sha256 over every movie poll
## and every boot_loop iteration: mode/stage/step/substep, frame_count,
## timer_ticks, system_data.seed, the three mode timers, scene_work, the BIOS
## rand word, pad_pressed, memory_card_result) and the same NAME_ENT handoff.
## Set TITLE_TRACE_OUT=<dir> to dump each run's rows for
## title_filesel_golden.py compare. With TITLE_ENTRY_RULES and
## TITLE_ENTRY_CONTENT naming the installed name-entry packs, the entry also
## replays the continuation run setA-entry from the handed-off rand object
## and must make the original's NAME_ENT draws up to the prologue.

const Flow = preload("res://flow/title_file_select.gd")
const EntryBoot = preload("res://app/new_game_entry_boot.gd")
const Adapter = preload("res://flow/new_game_state_adapter.gd")
const Driver = preload("res://tests/app/title_file_select_driver.gd")
const Router = preload("res://app/boot_router.gd")
const Launch = preload("res://app/installed_launch.gd")
const FIXTURE := "res://tests/fixtures/title_file_select_golden.json"

func _initialize() -> void:
	_rng_model()
	_refusals()
	var doc: Variant = JSON.parse_string(FileAccess.get_file_as_string(FIXTURE))
	check(doc is Dictionary and doc.get("schema") == "tokimemo.title-file-select-golden.v1", "fixture schema")
	if doc is Dictionary:
		for run: String in doc.runs:
			_run(run, doc.runs[run], doc.fields)
		_handoff_consumers(doc.runs.get("setA-r2", {}))
	_scene_drive()
	if doc is Dictionary: _entry_continuation(doc.runs.get("setA-r2", {}))
	print(("PASS" if failures == 0 else "FAIL") + ": %d title/file-select checks" % checks)
	quit(0 if failures == 0 else 1)

func _replay(run: Dictionary, flow: Flow) -> void:
	check(flow.begin().is_empty(), "begin")
	var poll: int = 0
	while flow.in_movie():
		var held: int = int(run.movie_held[poll]) if poll < run.movie_held.size() else 0
		poll += 1
		check(flow.step(held).is_empty(), "movie poll %d" % poll)
	var events: Array = run.logic_inputs
	var limit: int = int(run.logic_rows)
	while not flow.finished and flow.trace.size() < limit and flow.failure.is_empty() and flow.refusal.is_empty():
		var l: int = flow.logic_iterations + 1
		var held: int = 0
		for e: Dictionary in events:
			if l >= int(e.l) and l < int(e.l) + int(e.samples): held |= int(e.held)
		flow.step(held)

func _run(name: String, run: Dictionary, fields: Array) -> void:
	var flow := Flow.new()
	_replay(run, flow)
	check(flow.failure.is_empty() and flow.refusal.is_empty(), "%s: no failure (%s%s)" % [name, flow.failure, flow.refusal])
	check(flow.movie_trace.size() + flow.trace.size() == int(run.rows), "%s: row count %d, golden %d" % [name, flow.movie_trace.size() + flow.trace.size(), int(run.rows)])
	check(flow.trace_sha256() == run.sha256, "%s: row stream sha256" % name)
	# First transition that differs, to locate a regression without the golden.
	var port_transitions: Array = []
	var previous := ""
	for row: PackedInt64Array in flow.trace:
		var key := "%d/%d/%d/%d" % [row[1], row[2], row[3], row[4]]
		if key != previous: port_transitions.append(Array(row))
		previous = key
	check(port_transitions.size() == run.transitions.size(), "%s: %d transitions, golden %d" % [name, port_transitions.size(), run.transitions.size()])
	for i: int in mini(port_transitions.size(), run.transitions.size()):
		var want: Array = run.transitions[i].map(func(v): return int(v))
		if port_transitions[i] != want:
			check(false, "%s: transition %d golden %s port %s (%s)" % [name, i, str(want), str(port_transitions[i]), str(fields)])
			break
	if run.handoff != null:
		check(flow.finished, "%s: reached NAME_ENT stage 1" % name)
		var h: Dictionary = flow.handoff()
		check(flow.trace_complete and h.get("trace_sha256") == run.sha256, "%s: handoff digest covers the full golden row stream" % name)
		check(h.error.is_empty(), "%s: handoff %s" % [name, h.error])
		if h.error.is_empty():
			var got := {"l": flow.logic_iterations + 1, "fc": h.frame_count, "tt": h.timer_ticks,
				"seed": h.system_seed & 0xffffffff, "sw": h.scene_work & 0xffffffff, "rw": h.rng.state}
			for k: String in ["l", "fc", "tt", "seed", "sw", "rw"]:
				check(int(got[k]) == int(run.handoff[k]), "%s: handoff %s port %d golden %d" % [name, k, int(got[k]), int(run.handoff[k])])
	_dump(name, flow)

func _dump(name: String, flow: Flow) -> void:
	var out := OS.get_environment("TITLE_TRACE_OUT")
	if out.is_empty(): return
	var lines: Array[String] = []
	for row: PackedInt64Array in flow.movie_trace: lines.append("M" + ",".join(Array(row).slice(0, 4).map(str)))
	for row: PackedInt64Array in flow.trace: lines.append("L" + ",".join(Array(row).map(str)))
	var handoff := {}
	if flow.finished:
		var h: Dictionary = flow.handoff()
		handoff = {"fc": h.frame_count, "tt": h.timer_ticks, "seed": h.system_seed & 0xffffffff,
			"sw": h.scene_work & 0xffffffff, "rw": h.rng.state}
	var f := FileAccess.open(out.path_join(name + ".json"), FileAccess.WRITE)
	if f != null:
		f.store_string(JSON.stringify({"lines": lines, "handoff": handoff}))
		f.close()

func _rng_model() -> void:
	# OpenBIOS rand: seed * 0x41C64E6D + 0x3039, result (seed >> 16) & 0x7FFF.
	# Golden setA: srand(935) then one draw leaves the word at 994615380.
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 935)
	rng.next("title_models_link")
	check(rng.state == 994615380, "BIOS rand word after srand(935) and one draw")
	rng.reseed(1276393)
	var words: Array[int] = []
	for i: int in 3:
		rng.next("probe")
		words.append(rng.state)
	check(words == [1789269614, 1252081935, 152401308], "golden setA iris draws from srand(1276393)")

func _refusals() -> void:
	var flow := Flow.new()
	check(flow.begin().is_empty(), "refusal begin")
	check(not flow.step(0x10000).is_empty() and not flow.failure.is_empty(), "second-pad bits are refused")
	# Idle title menu: the 0x259-frame timeout enters the attract opening.
	flow = Flow.new()
	flow.keep_trace = false
	flow.begin()
	var guard: int = 0
	while flow.refusal.is_empty() and flow.failure.is_empty() and guard < 5000:
		flow.step(0)
		guard += 1
	check(flow.refusal.begins_with("ATTRACT_UNSUPPORTED"), "idle title refuses the attract opening: " + flow.refusal)
	check(flow.state.game_mode == Flow.MODE_TITLE and flow.state.mode_stage == 2, "refusal happens at the title menu")
	# Option entry: DOWN at the menu, then accept.
	flow = Flow.new()
	flow.keep_trace = false
	flow.begin()
	guard = 0
	var phase: int = 0
	while flow.refusal.is_empty() and flow.failure.is_empty() and guard < 5000:
		var held: int = 0
		if flow.state.game_mode == Flow.MODE_TITLE and flow.state.mode_stage == 1 and phase == 0:
			held = Flow.PAD_START; phase = 1
		elif flow.state.game_mode == Flow.MODE_TITLE and flow.state.mode_stage == 2 and flow.state.mode_step == 1 and phase == 1:
			held = Flow.PAD_DOWN; phase = 2
		elif phase == 2:
			phase = 3
		elif phase == 3:
			held = Flow.PAD_CIRCLE; phase = 4
		flow.step(held)
		guard += 1
	check(flow.refusal.begins_with("OPTION_UNSUPPORTED"), "the option screen is refused: " + flow.refusal)

## The entry and the opening adapter take the boot-path context unchanged.
func _handoff_consumers(run: Dictionary) -> void:
	check(not run.is_empty(), "setA-r2 run is in the fixture")
	if run.is_empty(): return
	var flow := Flow.new()
	flow.keep_trace = false
	_replay(run, flow)
	var h: Dictionary = flow.handoff()
	check(EntryBoot.title_handoff_error(h).is_empty(), "entry accepts the boot-path handoff: " + EntryBoot.title_handoff_error(h))
	var broken := h.duplicate()
	broken.rng = Rng.new()
	check(not EntryBoot.title_handoff_error(broken).is_empty(), "entry refuses an unconfigured rand object")
	broken = h.duplicate()
	broken.erase("scene_work")
	check(not EntryBoot.title_handoff_error(broken).is_empty(), "entry refuses a handoff without scene_work")
	check(not EntryBoot.title_handoff_error({}).is_empty(), "entry refuses an empty handoff")
	# The entry keeps drawing from the handed-off rand object.
	var entry := Entry.new()
	check(entry.begin(invented_tables(), invented_content(), h.rng).is_empty(), "entry begins on the boot-path rand state")
	finish_names(entry, h.rng, true)
	for kind in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)
	var with_boot = Adapter.from_entry(entry, h.frame_count, h.system_seed, {"initial_mascot": 255}, {"scene_work": h.scene_work})
	check(with_boot.error.is_empty(), "adapter takes the boot-path scene_work: " + with_boot.error)
	if with_boot.error.is_empty():
		check(with_boot.state.rule_state.scene_work == h.scene_work and h.scene_work == int(run.handoff.sw), "scene_work reaches the opening state")
		check(not with_boot.state.opening.unsourced.has("scene_work"), "scene_work is no longer unsourced")
		check(with_boot.state.frame_count == int(run.handoff.fc) and (with_boot.state.system_seed & 0xffffffff) == int(run.handoff.seed), "clock words reach the opening state")
	var direct = Adapter.from_entry(entry, 0, 0, {"initial_mascot": 255})
	check(direct.error.is_empty() and direct.state.opening.unsourced.has("scene_work"), "direct entry keeps scene_work unsourced")
	check(not Adapter.from_entry(entry, 0, 0, {"initial_mascot": 255}, {"scene_work": "x"}).error.is_empty(), "adapter refuses a malformed boot context")

## The test driver runs the flow from its own physics callback and the live
## pad: START skips the movie and the intro, Circle picks New Game, Circle
## passes the no-card message and accepts the new-game prompt. Pulses go
## through the pad owner's sample. No installed route reaches this path.
func _scene_drive() -> void:
	var scene := Driver.new()
	root.add_child(scene)
	# _initialize runs before the first frame, so the tree has not sent READY.
	if scene._pad == null: scene._ready()
	var pulses := {}
	var guard: int = 0
	while guard < 6000 and scene.failure.is_empty() and scene.flow.refusal.is_empty() and scene.handoff.is_empty():
		var s = scene.flow.state
		var want: int = 0
		if s.game_mode == Flow.MODE_MOVIE and not pulses.has("movie"): want = Flow.PAD_START; pulses["movie"] = true
		elif s.game_mode == Flow.MODE_TITLE and s.mode_stage == 1 and not pulses.has("intro"): want = Flow.PAD_START; pulses["intro"] = true
		elif s.game_mode == Flow.MODE_TITLE and s.mode_stage == 2 and s.mode_step == 1 and not pulses.has("menu"): want = Flow.PAD_CIRCLE; pulses["menu"] = true
		elif s.game_mode == Flow.MODE_FILE_SELECT and s.mode_stage == 1 and s.mode_step == 3 and not pulses.has("card"): want = Flow.PAD_CIRCLE; pulses["card"] = true
		elif s.game_mode == Flow.MODE_FILE_SELECT and s.mode_stage == 1 and s.mode_step == 5 and not pulses.has("prompt"): want = Flow.PAD_CIRCLE; pulses["prompt"] = true
		if want != 0: scene._pad.request_button_pulse(want)
		scene._physics_process(1.0 / 60.0)
		guard += 1
	check(scene.failure.is_empty() and scene.flow.refusal.is_empty(), "driver runs without refusal: " + scene.failure + scene.flow.refusal)
	check(pulses.size() == 5, "every scripted pulse was used: " + str(pulses.keys()))
	check(not scene.handoff.is_empty() and EntryBoot.title_handoff_error(scene.handoff).is_empty(), "driver reaches NAME_ENT stage 1 with a valid handoff")
	# keep_trace is off before begin(): no row is kept, so no row digest is claimed.
	check(not scene.flow.trace_complete and not scene.handoff.has("trace_sha256") and scene.flow.trace_sha256().is_empty(), "a handoff without the full row stream carries no trace_sha256")
	scene.queue_free()
	# No installed route reaches the boot path: the router has no opt-in and
	# the launch owner names no boot-path scene.
	check(not Router.new().has_method("is_boot_title_requested"), "the installed router has no boot-path opt-in")
	check(not (load("res://app/installed_launch.gd") as Script).get_script_constant_map().has("BOOT_TITLE_SCENE"), "the launch owner names no boot-path scene")

func _entry_continuation(run: Dictionary) -> void:
	var rules_path := OS.get_environment("TITLE_ENTRY_RULES")
	var content_path := OS.get_environment("TITLE_ENTRY_CONTENT")
	if rules_path.is_empty() or content_path.is_empty() or not run.has("continuation"):
		print("SKIP entry continuation: TITLE_ENTRY_RULES/TITLE_ENTRY_CONTENT unset")
		return
	var cont: Dictionary = run.continuation
	var flow := Flow.new()
	flow.keep_trace = false
	_replay(run, flow)
	var rng: Rng = flow.handoff().rng
	check(rng.state == int(cont.first_before), "first NAME_ENT draw starts from the handed-off word")
	var tables: Dictionary = Rules.load_tables(rules_path)
	var content: Dictionary = Entry.load_content(content_path)
	var entry := Entry.new()
	check(entry.begin(tables, content, rng).is_empty(), "entry begins with the installed packs")
	# The selections the continuation run made (its RAM at the first prologue
	# dispatch, and its two birthday_finalize draws): every name left empty
	# (name_finish copies a preset), player day 28 with month and blood left
	# unset (drawn: February, B), Shiori December 21 with blood O.
	var actions: Array = [{"kind":"start"}, {"kind":"start"}, {"kind":"start"},
		{"kind":"circle"}, {"kind":"select","value":27}, {"kind":"start"}, {"kind":"yes"},
		{"kind":"select","value":11}, {"kind":"circle"}, {"kind":"select","value":20}, {"kind":"circle"},
		{"kind":"select","value":3}, {"kind":"start"}, {"kind":"yes"}]
	var draws_before: int = rng.draw_count
	var guard: int = 0
	while not (entry.stage == 2 and entry.step_id == 5) and guard < 200:
		var automatic: bool = (entry.stage == 1 and entry.step_id in [0,2,4,6,8]) or (entry.stage == 2 and entry.step_id in [0,2,4])
		var error: String = entry.act({"kind":"tick"}) if automatic or actions.is_empty() else entry.act(actions.pop_front())
		check(error.is_empty(), "entry action: " + error)
		guard += 1
	check(entry.stage == 2 and entry.step_id == 5, "entry commits the profile")
	var callers: Array = rng.trace.slice(rng.trace.size() - (rng.draw_count - draws_before)).map(func(t): return t.caller)
	check(rng.draw_count - draws_before == int(cont.draws), "NAME_ENT draws %d, original %d: %s" % [rng.draw_count - draws_before, int(cont.draws), str(callers)])
	check(rng.state == int(cont.prologue_rand_word), "rand word at the prologue %d, original %d" % [rng.state, int(cont.prologue_rand_word)])
	check(entry.state_bytes.decode_u32(0xf8) & 0x7ff == int(cont.player_birthday) & 0x7ff, "player birthday %x, original %x" % [entry.state_bytes.decode_u32(0xf8), int(cont.player_birthday)])
	check(entry.state_bytes.decode_u32(0x1cc) & 0x7ff == int(cont.shiori_birthday) & 0x7ff, "Shiori birthday %x, original %x" % [entry.state_bytes.decode_u32(0x1cc), int(cont.shiori_birthday)])
	for g: int in 5:
		check(entry.state_bytes[0x1ca + g * 0x38] & 0xf0 == int(cont.club_bytes[g]) & 0xf0, "girl %d club %x, original %x" % [g, entry.state_bytes[0x1ca + g * 0x38] >> 4, int(cont.club_bytes[g]) >> 4])
