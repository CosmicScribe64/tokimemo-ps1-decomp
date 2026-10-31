extends RefCounted
## T-39220: read-only bridge from the running game to NativeDisplayHost's wing
## art. The boot or root step owns one feed per display host and calls
## present() (or observe() and the host's own present()) every frame.
##
## The feed only reads: rule_state.month and game_mode from the game state,
## and the scene facts the caller already holds (stage, the BS background it
## is drawing, a location name). It never writes the state, never draws from
## the game RNG and keeps no reference to the state between calls, so the
## native frame, RNG and state hash are the same with the feed attached or not.
##
## attach() also points the host at the installed wing_art pack and sets
## host.wing_art_follows_game, which the settings page reads to stop marking
## "Seasonal" and "Match the scene" as not active yet.

const FACT_KEYS := ["stage", "background", "location"]

var host: Object
## Last month pushed to the host (1..12), 0 before the first one.
var month := 0
## Wing context passed with the last present().
var last_context: Dictionary = {}

## Connects a NativeDisplayHost. launch_profile is the installed launch profile
## (paths.wing_art) or null to let the host discover the pack itself.
func attach(display_host: Object, launch_profile: Variant = null) -> String:
	if display_host == null or not display_host.has_method("set_calendar_month") or not display_host.has_method("present"):
		host = null
		return "wing art feed needs a NativeDisplayHost"
	host = display_host
	month = 0
	var path := pack_path(launch_profile)
	if not path.is_empty():
		host.set_wing_art_pack(path)
	host.wing_art_follows_game = true
	return ""

func detach() -> void:
	if host != null:
		host.wing_art_follows_game = false
	host = null
	month = 0

## The installed wing_art role directory, or "" when the profile has none.
static func pack_path(launch_profile: Variant) -> String:
	if launch_profile is Dictionary and launch_profile.get("paths") is Dictionary:
		var value: Variant = launch_profile.paths.get("wing_art", "")
		return value if value is String else ""
	return ""

## The in-game month (1..12) or 0 when the state has none. Read only.
static func month_of(state: Variant) -> int:
	if not state is Object:
		return 0
	var rules: Variant = state.get("rule_state")
	if not rules is Dictionary:
		return 0
	var value: Variant = rules.get("month")
	return value if value is int and value >= 1 and value <= 12 else 0

## The scene id the host classifies: facts.scene, else the game mode.
static func scene_of(state: Variant, facts: Dictionary = {}) -> Variant:
	if facts.has("scene"):
		return facts.scene
	if state is Object and state.get("game_mode") is int:
		return state.get("game_mode")
	return null

## The wing context for one frame: the month and the non-empty scene facts.
static func context_for(state: Variant, facts: Dictionary = {}) -> Dictionary:
	var context := {}
	for key: String in FACT_KEYS:
		var value: Variant = facts.get(key)
		if value is String and not value.is_empty():
			context[key] = value
	var value_month := month_of(state)
	if value_month > 0:
		context["month"] = value_month
	return context

## Pushes the month to the host when it changed. Returns true on a change.
func observe(state: Variant) -> bool:
	var value := month_of(state)
	if host == null or value == 0 or value == month:
		return false
	month = value
	host.set_calendar_month(value)
	return true

## Presents a native frame with the game's scene id and facts.
func present(native: Image, state: Variant, facts: Dictionary = {}) -> bool:
	if host == null:
		return false
	observe(state)
	last_context = context_for(state, facts)
	return host.present(native, scene_of(state, facts), last_context)
