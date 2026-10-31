extends RefCounted
## T-39230: Parity mode. A top-level play mode stored at profile.mode
## ('parity' | 'modern') in tokimemo.port-settings.v2. In Parity mode every
## registered preference whose policy is 'original' holds its original value,
## every 'refused' feature is unavailable, and only 'host' settings (window,
## volume, bindings, prompt style) can change. See wiki/parity-mode.md.
##
## The launch can force Parity for one session with --parity (after "--" on
## the Godot command line, or anywhere in the user arguments) or
## TOKIMEMO_PARITY=1. A forced session never rewrites the saved mode or the
## saved enhancement values (UserPreferencesService keeps them aside).
##
## Test gates call gate(): routes that produce differential, golden or
## playtest evidence fail fast unless Parity is active and clean, or the run
## is an explicit modern-invariance run (MODERN_INVARIANCE=1).

const Registry = preload("res://services/feature_registry.gd")
const MODES := ["parity", "modern"]
const LABELS := {"parity": "Parity (original game)", "modern": "Modern"}
const BADGE := "Parity"
const IDENTITY_SCHEMA := "tokimemo.parity-mode-identity.v1"
const IDENTITY_FILE := "parity-mode.json"
const ENV_FORCE := "TOKIMEMO_PARITY"
const ENV_INVARIANCE := "MODERN_INVARIANCE"
const ARG_FORCE := "--parity"
## Player-facing explanation, shown on the Game page and the Welcome step.
const EXPLANATION := "Parity plays the original game exactly: Japanese text, 4:3 picture, original text speed and no quick save. Every enhancement is locked and marked Parity. Window size, volume and controls stay adjustable because they never reach the game. Use it to check that nothing modern changes how the game plays. Modern unlocks every option."
const REFUSAL := "Parity mode keeps the original game. Switch to Modern on the Game page to change this."
const FORCED_REFUSAL := "Parity mode was forced for this session (--parity or TOKIMEMO_PARITY=1); it ends when the game restarts without it."

## Which launch option forces Parity: "--parity", "TOKIMEMO_PARITY" or "".
## The arguments and environment are parameters so tests can pass their own.
static func forced_by(arguments: PackedStringArray, environment: Dictionary) -> String:
	if ARG_FORCE in arguments: return ARG_FORCE
	if str(environment.get(ENV_FORCE, "")) == "1": return ENV_FORCE
	return ""

static func launch_forced_by() -> String:
	var arguments := OS.get_cmdline_user_args()
	arguments.append_array(OS.get_cmdline_args())
	return forced_by(arguments, {ENV_FORCE: OS.get_environment(ENV_FORCE)})

static func invariance_run(environment: Dictionary = {}) -> bool:
	if environment.is_empty(): return OS.get_environment(ENV_INVARIANCE) == "1"
	return str(environment.get(ENV_INVARIANCE, "")) == "1"

## Original values of every 'original'-policy preference, as "section.key".
## `defaults` is UserPreferencesService.defaults() (passed in to keep this
## script free of a preload cycle); profile.preset is pinned to 'original'.
static func locked_values(defaults: Dictionary) -> Dictionary:
	var result := {}
	for id: String in Registry.PREFERENCES:
		if Registry.parity_policy(id) != "original": continue
		var parts := id.split(".")
		result[id] = defaults[parts[0]][parts[1]]
	result["profile.preset"] = "original"
	return result

## Settings with every locked key set to its original value.
static func lock(values: Dictionary, defaults: Dictionary) -> Dictionary:
	var result := values.duplicate(true)
	var locked := locked_values(defaults)
	for id: String in locked:
		var parts := id.split(".")
		if result.get(parts[0]) is Dictionary: result[parts[0]][parts[1]] = _copy(locked[id])
	return result

## Every reason the values are not Parity-clean: a locked key away from its
## original value, or a preference key the registry does not know (anything
## unregistered is refused, so a new key cannot slip past Parity unclassified).
static func violations(values: Dictionary, defaults: Dictionary) -> Array[String]:
	var result: Array[String] = []
	var locked := locked_values(defaults)
	for name: String in Registry.preference_keys(values):
		if not Registry.registered(name): result.append(name + " (unregistered)"); continue
		var problem := Registry.entry_problem(name)
		if not problem.is_empty(): result.append(problem); continue
		if locked.has(name) and not same(_read(values, name), locked[name]): result.append(name)
	return result

## "" when the feature may run now, else the player-facing refusal.
static func refusal(id: String, parity_active: bool) -> String:
	if not Registry.registered(id): return "Unregistered feature %s is refused." % id
	if parity_active and Registry.parity_policy(id) != "host": return REFUSAL
	return ""

## Changes ("section.key" -> value) that Parity mode refuses, in order.
static func refused_changes(changes: Dictionary, defaults: Dictionary) -> Array[String]:
	var locked := locked_values(defaults)
	var result: Array[String] = []
	for name: Variant in changes:
		var key := str(name)
		if not Registry.registered(key): result.append(key); continue
		if locked.has(key) and not same(changes[name], locked[key]): result.append(key)
	return result

static func same(left: Variant, right: Variant) -> bool:
	if (left is float or left is int) and (right is float or right is int) and not left is bool and not right is bool:
		return is_equal_approx(float(left), float(right))
	if left is Dictionary and right is Dictionary: return JSON.stringify(left, "", true) == JSON.stringify(right, "", true)
	return typeof(left) == typeof(right) and left == right

static func _read(values: Dictionary, name: String) -> Variant:
	var parts := name.split(".")
	var section: Variant = values.get(parts[0])
	return section.get(parts[1]) if section is Dictionary else null

static func _copy(value: Variant) -> Variant:
	return value.duplicate(true) if value is Dictionary or value is Array else value

## sha256 of the registry table, so evidence names the classification it used.
static func registry_sha256() -> String:
	return JSON.stringify([Registry.PREFERENCES, Registry.FEATURES, Registry.ROWS, Registry.ROW_PREFIXES], "", true).sha256_text()

static func locked_values_sha256(defaults: Dictionary) -> String:
	return JSON.stringify(locked_values(defaults), "", true).sha256_text()

## Reviewed digests of the current registry and locked values. tools/port/parity_gate.py
## refuses evidence whose digests differ from this pin, and
## tests/services/run_feature_registry.gd fails until the pin matches the
## registry, so a registry change must update the pin (and rerun evidence).
const REGISTRY_PIN := "res://services/parity-registry-pin.json"
const PIN_SCHEMA := "tokimemo.parity-registry-pin.v1"
const GATE_NOT_EVIDENCE := "not_evidence"

# --- test gate -------------------------------------------------------------

## Parity identity of the running game, from the ModernSettings autoload.
static func identity(tree: SceneTree) -> Dictionary:
	var result := {"schema": IDENTITY_SCHEMA, "play_mode": "unknown", "forced_by": "", "invariance_run": invariance_run(),
		"registry_sha256": registry_sha256(), "locked_values_sha256": "", "violations": [], "source": ""}
	var node: Node = tree.root.get_node_or_null("ModernSettings") if tree != null else null
	if node == null or not "preferences" in node or not node.preferences.has_method("parity_active"):
		result.source = "no ModernSettings autoload with Parity support"
		return result
	var preferences: Object = node.preferences
	var defaults: Dictionary = preferences.defaults()
	result.source = "ModernSettings.preferences"
	result.play_mode = "parity" if preferences.parity_active() else "modern"
	result.forced_by = str(preferences.parity_forced_by)
	result.locked_values_sha256 = locked_values_sha256(defaults)
	result.violations = violations(preferences.effective_settings(), defaults) if result.play_mode == "parity" else []
	return result

## Test gate for evidence producers. Returns {ok, error, identity}. ok only
## when Parity is active with no violation, or this is an explicit
## modern-invariance run. When out_dir is set the identity is written there as
## parity-mode.json (tools/port/parity_gate.py checks it).
## `evidence` false is for QA routes that apply modern preferences on purpose
## (visual QA, English playthrough): the route runs, and the record says
## gate "not_evidence", which no comparison accepts.
static func gate(tree: SceneTree, out_dir: String = "", evidence: bool = true) -> Dictionary:
	var current := identity(tree)
	var error := ""
	if evidence and current.play_mode == "parity":
		if not current.violations.is_empty(): error = "Parity mode is active but not clean: %s" % ", ".join(current.violations)
	elif evidence and not current.invariance_run:
		error = "Parity mode is not active (%s). Run with --parity or TOKIMEMO_PARITY=1; modern-invariance runs go through tools/port/run_parity_invariance.sh." % (current.source if current.play_mode == "unknown" else "play mode " + current.play_mode)
	if not out_dir.is_empty():
		var file := FileAccess.open(out_dir.path_join(IDENTITY_FILE), FileAccess.WRITE)
		if file == null:
			if error.is_empty(): error = "cannot write %s" % out_dir.path_join(IDENTITY_FILE)
		else:
			var record := current.duplicate(true); record.error = error
			record.gate = GATE_NOT_EVIDENCE if not evidence else ("passed" if error.is_empty() else "failed")
			file.store_string(JSON.stringify(record, "\t", true) + "\n"); file.close()
	return {"ok": error.is_empty(), "error": error, "identity": current}
