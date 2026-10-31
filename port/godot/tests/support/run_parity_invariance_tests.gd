extends "res://tests/services/settings_test_tree.gd"
## T-39230 (T0, synthetic): the modern-vs-parity invariance harness itself.
## It runs one native input tape under the parity, modern and
## modern_state_only profiles on the real settings runtime (preferences,
## remaps, accessibility and pacing services) and a real core GameSession
## (tick, RNG, input latch, state hash). The native Image is a synthetic
## producer drawn from state words only, so this proves the harness and the
## input mapping, not the game: the installed-route run is
## tools/port/run_parity_invariance.sh.
## Negatives: a driver that lets a game_affecting entry hold ticks, and one
## that draws a presentation_only value into the native Image, are both caught;
## a profile that tries to run while Parity is forced is refused.
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Harness = preload("res://tests/support/parity_invariance.gd")
const Player = preload("res://tests/support/input_script_player.gd")
const Registry = preload("res://services/feature_registry.gd")
const Parity = preload("res://services/parity_mode.gd")
const Fakes = preload("res://tests/services/settings_v2_fakes.gd")
## Native input tape: frame -> PS1 buttons held on that frame.
const TAPE := {3: ["CIRCLE"], 4: ["CIRCLE"], 8: ["DOWN"], 9: ["DOWN", "CROSS"], 14: ["START"], 20: ["CIRCLE", "LEFT"], 21: ["TRIANGLE"]}
const FRAMES := 32

var runtime: Node
var player: Node

func _initialize() -> void: call_deferred("run")

func _fail(message: String) -> void:
	printerr(message); check(false)

func run() -> void:
	_profiles()
	runtime = Fakes.FakeRuntime.new(); runtime.name = "InvarianceRuntime"; root.add_child(runtime)
	player = Player.new(); player.remaps = runtime.remaps; root.add_child(player)
	await process_frame
	var parity := _drive("parity")
	check(player.button_index("CIRCLE") == JOY_BUTTON_A and player.button_index("TRIANGLE") == JOY_BUTTON_Y)
	var modern := _drive("modern")
	# Modern swaps confirm/cancel and remaps Square/Triangle: other physical buttons, same native bits.
	check(player.button_index("CIRCLE") == JOY_BUTTON_B and player.button_index("TRIANGLE") == JOY_BUTTON_X)
	var state_only := _drive("modern_state_only")
	check(parity.size() == FRAMES)
	# The tape reached the game: the latched input changed the state hash.
	var hashes := {}
	for record: Dictionary in parity: hashes[record.state] = true
	check(hashes.size() > 1)
	var result := Harness.compare(parity, modern, true)
	if not result.equal: _fail("modern differs from parity: %s" % [result.first_divergence])
	result = Harness.compare(parity, state_only, false)
	if not result.equal: _fail("modern_state_only differs from parity on state: %s" % [result.first_divergence])
	# The modern runs really used a different mapping and options.
	check(runtime.preferences.settings.controls.swap_confirm and runtime.preferences.settings.display.wing_art == "seasonal")
	check(runtime.preferences.settings.accessibility.font_scale == 1.5)
	# Negative 1: a game_affecting entry (text_speed) leaking into the tick loop.
	var held := _drive("modern", "hold")
	result = Harness.compare(parity, held, true)
	check(not result.equal and result.first_divergence.field in ["frame", "state", "length"])
	# Negative 2: a presentation_only value drawn into the native Image.
	var drawn := _drive("modern", "image")
	result = Harness.compare(parity, drawn, true)
	check(not result.equal and result.first_divergence.field == "image")
	check(Harness.compare(parity, drawn, false).equal)  # state-only comparison ignores it
	# Negative 5: a player that ignores the active mapping delivers other native bits.
	player.remaps = null
	var unmapped := _drive("modern")
	player.remaps = runtime.remaps
	check(not Harness.compare(parity, unmapped, true).equal)
	# Negative 3: comparator edge cases.
	check(not Harness.compare([], parity, true).equal)
	check(Harness.compare(parity, parity.slice(0, 10), true).first_divergence.field == "length")
	var renumbered := parity.duplicate(true); renumbered[5].frame += 1
	check(Harness.compare(parity, renumbered, true).first_divergence.field == "frame")
	# Negative 4: a modern profile refuses to run in a forced Parity session.
	runtime.preferences.force_parity("TOKIMEMO_PARITY")
	check(Harness.apply(runtime.preferences, "modern").contains("forced"))
	check(Harness.apply(runtime.preferences, "parity").is_empty())
	check(not Harness.apply(runtime.preferences, "turbo").is_empty())
	print("Parity invariance harness: %d frames equal across parity/modern (state+image) and modern_state_only (state); hold and image leaks detected; forced-session refusal passed" % FRAMES)
	finish()

func _profiles() -> void:
	check(Harness.profile_changes("parity").is_empty())
	var modern := Harness.profile_changes("modern")
	var state_only := Harness.profile_changes("modern_state_only")
	check(modern.get("profile.mode") == "modern" and state_only.get("profile.mode") == "modern")
	for id: String in Registry.PREFERENCES:
		var spec: Dictionary = Registry.PREFERENCES[id]
		if id == "profile.mode": continue  # the switch itself, set per run
		match spec.invariance:
			"state_and_image": check(modern.has(id) and state_only.has(id))
			"state_only": check(not modern.has(id) and state_only.has(id))
			"excluded": check(not modern.has(id) and not state_only.has(id))
	# Every game_affecting entry is excluded and listed with its invariant.
	var listed := {}
	for row: Dictionary in Harness.excluded(): listed[row.id] = row
	for table: Dictionary in [Registry.PREFERENCES, Registry.FEATURES]:
		for id: String in table:
			if table[id]["class"] == Registry.GAME_AFFECTING:
				check(listed.has(id) and str(listed[id].invariant).length() > 12)
	var record := Harness.profile_record("modern", "0".repeat(64))
	check(record.registry_sha256 == Parity.registry_sha256() and record.compares == "state and native image")

## Runs the tape under a profile. leak: "" (honest driver), "hold" (skips a
## tick every third frame while text speed is not original: what a
## game_affecting hold does), "image" (draws the interface scale into the
## native Image: what a leaking presentation option would do).
func _drive(profile: String, leak: String = "") -> Array:
	var error := Harness.apply(runtime.preferences, profile)
	if not error.is_empty(): _fail("apply %s: %s" % [profile, error]); return []
	if leak == "hold":
		# Excluded entries are never enabled by the harness; the leak test sets one by hand.
		if not runtime.preferences.set_value("accessibility", "text_speed", "slow"): _fail("cannot set text_speed: " + runtime.preferences.error)
	var session := Session.new()
	check(session.rng.configure(session.rng.OPENBIOS_PROFILE, 19950404))
	var noop := func(_a: Variant, _b: Variant, _c: Variant) -> void: pass
	session.draw_logic = noop; session.cd_logic = noop; session.audio_logic = noop
	# Synthetic mode handler: folds the latched native pad word into state, so
	# any change in delivered input changes the state hash.
	session.state.game_mode = 0x15
	check(session.router.bind_mode(0x15, func(current: RefCounted, input: RefCounted, _rng: RefCounted, _result: RefCounted) -> void:
		current.mode_step = (int(current.mode_step) * 31 + (int(input.held) & 0xFF) + (int(input.held) >> 8)) & 0xFF
		current.substep = (int(current.substep) + int(input.pressed != 0)) & 0xFF).is_empty())
	var frames: Array = []
	var previous := 0
	var pressed: Array[int] = []
	for tick: int in FRAMES:
		for index: int in pressed: _press(index, false)
		pressed.clear()
		for button: String in TAPE.get(tick, []):
			var index: int = player.button_index(button)
			if index < 0: _fail("no controller button for %s under %s" % [button, profile]); continue
			_press(index, true); pressed.append(index)
		Input.flush_buffered_events()
		var held: int = runtime.remaps.held_mask()
		if leak == "hold" and runtime.preferences.settings.accessibility.text_speed != "normal" and tick % 3 == 0: continue
		var stepped = session.step(InputFrame.new(session.clock.logic_iterations, held, previous), [])
		previous = held
		if not stepped.error.is_empty(): _fail("%s tick %d: %s" % [profile, tick, stepped.error])
		frames.append(Harness.frame_record(int(session.state.frame_count), "synthetic", session.state_hash(), _native_image(session, leak)))
	for index: int in pressed: _press(index, false)
	Input.flush_buffered_events()
	if leak == "hold": runtime.preferences.set_value("accessibility", "text_speed", "normal")
	return frames

func _press(index: int, down: bool) -> void:
	var event := InputEventJoypadButton.new(); event.device = 0; event.button_index = index; event.pressed = down
	event.pressure = 1.0 if down else 0.0
	Input.parse_input_event(event)

## Synthetic native producer: pixels from the state hash only.
func _native_image(session: RefCounted, leak: String) -> Image:
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var digest: PackedByteArray = session.state_hash().sha256_buffer()
	for i: int in 16:
		image.fill_rect(Rect2i((i % 4) * 80, (i / 4) * 60, 80, 60), Color8(digest[i], digest[i + 8], digest[(i * 3) % 32], 255))
	if leak == "image":
		var scale: float = runtime.preferences.settings.display.ui_scale
		image.set_pixel(0, 0, Color(scale / 2.0, 0, 0, 1))
	return image
