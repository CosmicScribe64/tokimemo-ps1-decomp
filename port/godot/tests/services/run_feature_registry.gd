extends "res://tests/services/settings_test_tree.gd"
## T-39230: the feature registry is complete and Parity mode shows it.
## Part 1 enumerates every preference key (UserPreferencesService.defaults())
## and every settings overlay, pause menu and Welcome row (built on the real
## pages with every capability on: save host, Return to title, advanced packs,
## widescreen frame) and fails on any unregistered or unclassified one. Invented
## specs and an invented row prove each rule refuses.
## Part 2 switches to Parity on the Game page and checks locked rows with the
## Parity badge, the explanation, the badge line and the host rows that stay
## live; then the pause menu and the Welcome step in Parity.
const Preferences = preload("res://services/user_preferences_service.gd")
const Presets = preload("res://services/settings_presets.gd")
const Registry = preload("res://services/feature_registry.gd")
const Parity = preload("res://services/parity_mode.gd")
const Fakes = preload("res://tests/services/settings_v2_fakes.gd")
const Welcome = preload("res://ui/welcome_step.gd")

## Rows that exist only in states this test does not open: none today. A row
## listed in Registry.ROWS but never built anywhere fails as stale.
const CONDITIONAL_ROWS: Array[String] = []

class SaveHost extends Node:
	var calls: Array = []
	var titles := 0
	func _ready() -> void: add_to_group("settings_save_hosts")
	func request_settings_save(load_game: bool) -> bool:
		calls.append(load_game); return true
	func request_return_to_title() -> void: titles += 1

var seen_rows: Dictionary = {}

func _initialize() -> void: call_deferred("run")

func _process(delta: float) -> bool:
	_elapsed += delta
	if failures > 0: quit(1)
	elif _elapsed > 60.0:
		printerr("Feature registry test did not finish within 60 seconds"); quit(1)
	return false

func _fail(message: String) -> void:
	printerr(message); check(false)

func run() -> void:
	_preference_keys()
	_spec_rules()
	_registry_pin()
	_latin_typing()
	var fake := Fakes.FakeRuntime.new(); fake.name = "RegistryRuntime"; root.add_child(fake)
	var host := SaveHost.new(); root.add_child(host)
	await process_frame
	await _settings_rows(fake)
	await _pause_rows(fake, host)
	await _welcome_rows(fake)
	for key: String in Registry.ROWS:
		if not seen_rows.has(key) and key not in CONDITIONAL_ROWS: _fail("stale registry row: %s is built nowhere" % key)
	await _parity_settings_ui(fake, host)
	await _parity_pause_ui(fake, host)
	await _parity_welcome(fake)
	print("Feature registry: %d preference keys, %d features, %d rows registered and classified; Parity locks rows with badge, keeps host rows, refuses quick save, Welcome mode passed" % [
		Registry.PREFERENCES.size(), Registry.FEATURES.size(), seen_rows.size()])
	finish()

# --- part 1: completeness ------------------------------------------------------

## The reviewed pin equals the registry and locked values in force, so the
## Python gate (tools/port/parity_gate.py) refuses evidence made under any other.
func _registry_pin() -> void:
	var pin: Variant = JSON.parse_string(FileAccess.get_file_as_string(Parity.REGISTRY_PIN))
	var registry := Parity.registry_sha256(); var locked := Parity.locked_values_sha256(Preferences.defaults())
	if not pin is Dictionary or pin.get("schema") != Parity.PIN_SCHEMA:
		_fail("missing or malformed %s" % Parity.REGISTRY_PIN); return
	if pin.get("registry_sha256") != registry or pin.get("locked_values_sha256") != locked:
		_fail("registry pin is stale: set %s to registry_sha256 %s, locked_values_sha256 %s (and rerun Parity evidence)" % [Parity.REGISTRY_PIN, registry, locked])

## Latin name typing generates native input: registered, game_affecting and refused in Parity.
func _latin_typing() -> void:
	var Latin: GDScript = load("res://flow/name_entry_latin_page.gd")
	check(Registry.registered(Latin.FEATURE) and Registry.FEATURES[Latin.FEATURE]["class"] == Registry.GAME_AFFECTING)
	check(Latin.allowed(true, false))
	check(not Latin.allowed(true, true) and not Latin.allowed(false, false))

func _preference_keys() -> void:
	var defaults := Preferences.defaults()
	var keys := Registry.preference_keys(defaults)
	for name: String in keys:
		var problem := Registry.entry_problem(name)
		if not problem.is_empty(): _fail("preference key: " + problem)
	for name: String in Registry.PREFERENCES:
		if name not in keys: _fail("stale registry entry %s is not a preference key" % name)
	for name: String in Registry.FEATURES:
		var problem := Registry.entry_problem(name)
		if not problem.is_empty(): _fail("feature: " + problem)
		if not name.begins_with("feature."): _fail("feature id %s must start with 'feature.'" % name)
	check(Registry.BUSES == Preferences.BUSES)
	# Presets bundle only registered keys that Parity pins.
	for name: String in Presets.ORIGINAL.keys() + Presets.MODERN_OVERRIDES.keys():
		if Registry.parity_policy(name) != "original": _fail("preset key %s is not pinned by Parity" % name)
		elif not Parity.same(Presets.ORIGINAL.get(name), Parity.locked_values(defaults).get(name)): _fail("Parity original of %s differs from the Original preset" % name)
	# Modern values: valid all together, and different from the original unless a note says why.
	var modern := defaults.duplicate(true)
	for name: String in Registry.PREFERENCES:
		var spec: Dictionary = Registry.PREFERENCES[name]
		if not spec.has("modern"): continue
		var parts := name.split(".")
		if Parity.same(spec.modern, defaults[parts[0]][parts[1]]) and not spec.has("note"): _fail("%s modern value equals its original without a note" % name)
		modern[parts[0]][parts[1]] = spec.modern
	if Preferences.validate(modern).is_empty(): _fail("the registry's modern values do not validate together")
	# Parity's own table check: defaults are clean, every modern value is a violation.
	check(Parity.violations(defaults, defaults).is_empty())
	var dirty := Parity.violations(modern, defaults)
	for name: String in Parity.locked_values(defaults):
		var parts := name.split(".")
		if not Parity.same(modern[parts[0]][parts[1]], defaults[parts[0]][parts[1]]) and name not in dirty and name != "profile.preset":
			_fail("violations() missed " + name)
	# Negative: an unregistered preference key is reported.
	var extra := defaults.duplicate(true); extra.accessibility["fast_forward"] = 4
	check(Parity.violations(extra, defaults).has("accessibility.fast_forward (unregistered)"))
	check(not Registry.registered("accessibility.fast_forward") and not Registry.entry_problem("accessibility.fast_forward").is_empty())

func _spec_rules() -> void:
	var good := {"class": Registry.PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": 1, "invariant": "Window-space only, outside the canvas."}
	check(Registry.spec_problem("x.y", good, true).is_empty())
	var cases := [
		[{"parity": "original", "invariance": "state_and_image", "modern": 1, "invariant": "declared invariant here"}, true, "no class"],
		[{"class": "cosmetic", "parity": "original", "invariance": "state_and_image", "modern": 1, "invariant": "declared invariant here"}, true, "no class"],
		[{"class": Registry.PRESENTATION_ONLY, "invariance": "state_and_image", "modern": 1, "invariant": "declared invariant here"}, true, "no parity"],
		[{"class": Registry.PRESENTATION_ONLY, "parity": "original", "modern": 1, "invariant": "declared invariant here"}, true, "no invariance"],
		[{"class": Registry.PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": 1}, true, "invariant"],
		[{"class": Registry.PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": 1, "invariant": "short"}, true, "invariant"],
		[{"class": Registry.GAME_AFFECTING, "parity": "host", "invariance": "excluded", "invariant": "declared invariant here"}, false, "adjustable in Parity"],
		[{"class": Registry.GAME_AFFECTING, "parity": "original", "invariance": "state_only", "modern": 1, "invariant": "declared invariant here"}, true, "not excluded"],
		[{"class": Registry.PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "invariant": "declared invariant here"}, true, "no modern value"],
		[{"class": Registry.PRESENTATION_ONLY, "parity": "refused", "invariance": "excluded", "invariant": "declared invariant here"}, true, "is a preference"],
		[{"class": Registry.INPUT_UX, "parity": "original", "invariance": "excluded", "invariant": "declared invariant here"}, false, "is a feature"],
	]
	for case: Array in cases:
		var problem := Registry.spec_problem("x.y", case[0], case[1])
		if not problem.contains(case[2]): _fail("spec rule '%s' not enforced: got '%s'" % [case[2], problem])

## Problems of a surface's rows: unregistered rows, or rows leading to an
## unregistered or unclassified entry.
static func row_problems(surface: String, focus_items: Dictionary) -> Array[String]:
	var result: Array[String] = []
	for key: String in focus_items:
		var ids: Array = Registry.row_entries(key)
		if ids.is_empty(): result.append("%s row '%s' is not registered in feature_registry.gd" % [surface, key]); continue
		for id: String in ids:
			var problem := Registry.entry_problem(id)
			if not problem.is_empty(): result.append("%s row '%s': %s" % [surface, key, problem])
	return result

func _check_rows(surface: String, focus_items: Dictionary) -> void:
	for problem: String in row_problems(surface, focus_items): _fail(problem)
	for key: String in focus_items:
		if Registry.ROWS.has(key): seen_rows[key] = true

func _settings_rows(fake: Node) -> void:
	fake.toggle_menu(); await process_frame
	var menu: Control = fake.menu
	_check_rows("settings", menu.focus_items)
	menu.advanced_open = true; menu.rebuild(); await process_frame
	check(menu.page_titles.has("Game") and menu.focus_items.has("Play mode") and menu.focus_items.has("Quick save"))
	_check_rows("settings", menu.focus_items)
	# The close prompt rows only exist while a display preview is pending.
	var display: Dictionary = fake.preferences.settings.display.duplicate(true); display.width = 1280
	check(fake.preferences.begin_display(display, Time.get_ticks_msec() / 1000.0))
	menu.rebuild(); await process_frame
	_check_rows("settings (pending display)", menu.focus_items)
	check(fake.preferences.revert_display())
	# Negative: an invented control is reported and locked in Parity.
	var invented := Button.new(); invented.text = "Fast-forward"; menu.column.add_child(invented)
	menu.widgets.register(invented, "Fast-forward")
	var invented_problems := row_problems("settings", menu.focus_items)
	check(invented_problems == ["settings row 'Fast-forward' is not registered in feature_registry.gd"])
	check(Registry.row_locked_in_parity("Fast-forward"))
	menu.advanced_open = false
	fake.toggle_menu(); await process_frame

func _pause_rows(fake: Node, _host: Node) -> void:
	var pause: Control = fake.layer.get_node("PauseMenu")
	pause.open(); await process_frame
	check(pause.focus_items.has("Return to title"))
	_check_rows("pause menu", pause.focus_items)
	pause.resume(); await process_frame

func _welcome_rows(fake: Node) -> void:
	var step: Control = Welcome.new(); step.runtime = fake; step.marker_path = "user://registry-welcome.json"
	root.add_child(step); await process_frame
	check(step.focus_items.has("Play mode"))
	_check_rows("welcome", step.focus_items)
	step.queue_free(); await process_frame

# --- part 2: Parity in the UI ---------------------------------------------------

func _texts(node: Node) -> String:
	var parts: Array[String] = []
	for child: Node in node.find_children("*", "", true, false):
		if child is Label or child is Button: parts.append(child.text)
	return "\n".join(parts)

func _badged(control: Control) -> bool:
	if control is OptionButton:
		for sibling: Node in control.get_parent().get_children():
			if sibling.has_meta("parity_badge"): return true
		return false
	if control is Range:
		for sibling: Node in control.get_parent().get_child(0).get_children():
			if sibling.has_meta("parity_badge"): return true
		return false
	return (control as Button).text.ends_with("[Parity]")

func _parity_settings_ui(fake: Node, host: SaveHost) -> void:
	check(fake.preferences.apply_preset("modern"))
	check(fake.preferences.set_value("accessibility", "text_speed", "fast"))
	fake.toggle_menu(); await process_frame
	var menu: Control = fake.menu
	check(menu.parity_locked.is_empty() and not menu.fidelity_badge.text.contains("Parity"))
	var game_text := _texts(menu.tabs.get_child(menu.page_titles.find("Game")))
	check(game_text.contains("Parity plays the original game exactly"))
	var mode: OptionButton = menu.focus_items["Play mode"]
	check(mode.get_item_text(mode.selected) == Parity.LABELS.modern)
	var parity_index := Parity.MODES.find("parity")
	mode.select(parity_index); mode.item_selected.emit(parity_index); await process_frame
	check(fake.preferences.parity_active() and fake.preferences.settings.accessibility.text_speed == "normal")
	check(fake.preferences.settings.display.aspect == "original" and menu.draft.aspect == "original")
	check(menu.fidelity_badge.text.begins_with("Parity mode: ON"))
	for key: String in ["Text speed", "Auto-advance", "Skip", "Text size", "Scene layout", "Scaling", "Frame limit",
			"Side art", "Game language", "Mono output", "Dialogue text box", "preset:modern", "preset:custom", "Quick save", "Load quick save"]:
		var control: Control = menu.focus_items.get(key)
		if control == null: _fail("missing row " + key); continue
		var disabled: bool = (control as BaseButton).disabled if control is BaseButton else not (control as Range).editable
		if not disabled or not control.get_meta("parity_locked", false) or not _badged(control): _fail("row %s is not locked with a Parity badge" % key)
		if key not in menu.parity_locked: _fail("row %s missing from parity_locked" % key)
	for key: String in ["Window size", "Window mode", "Interface size", "Master", "Master mute", "Prompt style",
			"Swap confirm and cancel", "Stick deadzone", "binding:confirm", "Play mode", "page:Display", "Return to game"]:
		var control: Control = menu.focus_items.get(key)
		if control == null: _fail("missing row " + key); continue
		var disabled: bool = (control as BaseButton).disabled if control is BaseButton else not (control as Range).editable
		if disabled or control.has_meta("parity_locked"): _fail("host row %s was locked" % key)
	# A locked control cannot write even if its signal fires (the service refuses).
	var speed: OptionButton = menu.focus_items["Text speed"]
	speed.item_selected.emit(3); await process_frame
	check(fake.preferences.settings.accessibility.text_speed == "normal" and menu.notice.text == Parity.REFUSAL)
	# Quick save refused through the runtime too.
	(menu.focus_items["Quick save"] as Button).pressed.emit(); await process_frame
	check(host.calls.is_empty())
	# Host change still works.
	var prompt: OptionButton = menu.focus_items["Prompt style"]
	prompt.select(2); prompt.item_selected.emit(2); await process_frame
	check(fake.preferences.settings.controls.prompt_style == "xbox")
	# Back to Modern unlocks rows; values stay original.
	mode = menu.focus_items["Play mode"]
	var modern_index := Parity.MODES.find("modern")
	mode.select(modern_index); mode.item_selected.emit(modern_index); await process_frame
	check(not fake.preferences.parity_active() and menu.parity_locked.is_empty())
	check(not (menu.focus_items["Text speed"] as OptionButton).disabled)
	# Forced session: the play mode row locks too and says why.
	fake.preferences.force_parity("TOKIMEMO_PARITY"); menu.rebuild(); await process_frame
	check("Play mode" in menu.parity_locked and menu.fidelity_badge.text.contains("forced by TOKIMEMO_PARITY"))
	check(_texts(menu.tabs.get_child(menu.page_titles.find("Game"))).contains("forced for this session"))
	fake.toggle_menu(); await process_frame

func _parity_pause_ui(fake: Node, host: SaveHost) -> void:
	var pause: Control = fake.layer.get_node("PauseMenu")
	pause.open(); await process_frame
	for key: String in ["Quick save", "Load quick save", "Return to title"]:
		var item: Button = pause.focus_items[key]
		check(item.disabled and item.text.ends_with("[Parity]"))
	for key: String in ["Resume", "Settings", "Quit game"]: check(not (pause.focus_items[key] as Button).disabled)
	pause._save(false)
	check(pause.notice.text == Parity.REFUSAL and host.calls.is_empty() and pause.visible)
	(pause.focus_items["Return to title"] as Button).pressed.emit(); await process_frame
	check(host.titles == 0 and pause.visible)
	pause.resume(); await process_frame

func _parity_welcome(fake: Node) -> void:
	# A fresh, unforced runtime: the Welcome step picks Parity.
	var runtime := Fakes.FakeRuntime.new(); runtime.name = "WelcomeRuntime"; root.add_child(runtime); await process_frame
	check(runtime.preferences.apply_preset("modern") and not runtime.preferences.parity_active())
	var step: Control = Welcome.new(); step.runtime = runtime; step.marker_path = "user://registry-welcome-parity.json"
	if FileAccess.file_exists(step.marker_path): DirAccess.remove_absolute(ProjectSettings.globalize_path(step.marker_path))
	var done := [false]; step.finished.connect(func() -> void: done[0] = true)
	root.add_child(step); await process_frame
	check(_texts(step).contains("Parity plays the original game exactly"))
	var mode: OptionButton = step.focus_items["Play mode"]
	check(mode.get_item_text(mode.selected) == Parity.LABELS.modern)
	check(not (step.focus_items["Game language"] as OptionButton).disabled)
	var parity_index := Parity.MODES.find("parity")
	mode.select(parity_index); mode.item_selected.emit(parity_index); await process_frame; await process_frame
	var language: OptionButton = step.focus_items["Game language"]
	check(language.disabled and _badged(language) and (step.focus_items["Preset"] as OptionButton).disabled)
	check(not (step.focus_items["Window"] as OptionButton).disabled)
	step.confirm(); await process_frame
	check(done[0] and runtime.preferences.parity_active() and runtime.preferences.settings.profile.mode == "parity")
	check(runtime.preferences.settings.language.bundle == "ja" and runtime.preferences.settings.display.aspect == "original")
	step.queue_free(); await process_frame
	runtime.queue_free(); await process_frame
	fake.queue_free(); await process_frame
