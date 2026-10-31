extends RefCounted
## T-39230: modern-vs-parity invariance harness (shared by the Godot route and
## its synthetic tests; tools/port/parity_invariance.py compares route output).
##
## The same input script runs under three profiles built only from
## services/feature_registry.gd:
##   parity             Parity mode (every 'original' entry pinned).
##   modern             Modern mode with every entry whose invariance is
##                      'state_and_image' set to its registry 'modern' value
##                      (all presentation_only and input_ux options that must
##                      leave the native 320x240 Image alone).
##   modern_state_only  modern plus the 'state_only' entries (text drawing and
##                      language), which may change native pixels but never state.
## Entries with invariance 'excluded' (every game_affecting entry, and the
## menus that pause the tree) are never enabled; the report lists them with
## their declared invariants.
## Per frame the harness keeps {frame, state, image}: the game-state hash and
## the RGBA8 sha256 of the native Image. modern must equal parity on both;
## modern_state_only on state.

const Registry = preload("res://services/feature_registry.gd")
const Parity = preload("res://services/parity_mode.gd")
const PROFILES := ["parity", "modern", "modern_state_only"]
const SCHEMA := "tokimemo.parity-invariance-profile.v1"
const FRAME_SCHEMA := "tokimemo.parity-invariance-frame.v1"

## "section.key" -> value changes for a profile ({} for parity, which is
## entered through UserPreferencesService.set_mode / force_parity).
static func profile_changes(name: String) -> Dictionary:
	if name == "parity": return {}
	var included := ["state_and_image"] if name == "modern" else ["state_and_image", "state_only"]
	var result := {"profile.mode": "modern"}
	for id: String in Registry.PREFERENCES:
		var spec: Dictionary = Registry.PREFERENCES[id]
		if spec.invariance in included and spec.has("modern"): result[id] = Parity._copy(spec.modern)
	return result

## Entries the harness never enables, with the invariant each one declares.
static func excluded() -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	for table: Dictionary in [Registry.PREFERENCES, Registry.FEATURES]:
		for id: String in table:
			if table[id].invariance == "excluded":
				result.append({"id": id, "class": table[id]["class"], "invariant": table[id].invariant})
	return result

## Applies a profile to a UserPreferencesService. Returns "" or the error.
## parity: the saved mode (a forced session already is). modern profiles need
## the session not to be forced, since Parity refuses every enhancement.
static func apply(preferences: Object, name: String) -> String:
	if name not in PROFILES: return "unknown invariance profile '%s'" % name
	if name == "parity":
		if preferences.parity_active(): return ""
		return "" if preferences.set_mode("parity") else str(preferences.error)
	if not str(preferences.parity_forced_by).is_empty():
		return "a modern invariance profile cannot run while Parity is forced (%s)" % preferences.parity_forced_by
	if preferences.parity_active() and not preferences.set_mode("modern"): return str(preferences.error)
	return "" if preferences.apply_profile(profile_changes(name)) else str(preferences.error)

## Profile record written next to the frames (invariance-profile.json).
static func profile_record(name: String, script_sha256: String) -> Dictionary:
	return {"schema": SCHEMA, "profile": name, "changes": profile_changes(name), "excluded": excluded(),
		"registry_sha256": Parity.registry_sha256(), "input_script_sha256": script_sha256,
		"compares": "state and native image" if name != "modern_state_only" else "state only"}

static func image_sha256(image: Image) -> String:
	if image == null: return ""
	var copy := image.duplicate() as Image
	if copy.get_format() != Image.FORMAT_RGBA8: copy.convert(Image.FORMAT_RGBA8)
	var context := HashingContext.new(); context.start(HashingContext.HASH_SHA256)
	context.update(copy.get_data())
	return context.finish().hex_encode()

static func frame_record(frame: int, phase: String, state_hash: String, image: Image) -> Dictionary:
	return {"schema": FRAME_SCHEMA, "frame": frame, "phase": phase, "state": state_hash, "image": image_sha256(image)}

## Compares two frame lists. Returns {equal, compared, first_divergence}; a
## divergence names the frame index, field and both values. Empty lists,
## different lengths and different frame numbers are divergences too.
static func compare(parity_frames: Array, other_frames: Array, compare_images: bool) -> Dictionary:
	var result := {"equal": false, "compared": 0, "first_divergence": {}}
	if parity_frames.is_empty() or other_frames.is_empty():
		result.first_divergence = {"index": 0, "field": "frames", "parity": parity_frames.size(), "other": other_frames.size()}
		return result
	var fields := ["frame", "phase", "state"] + (["image"] as Array if compare_images else [] as Array)
	for index: int in mini(parity_frames.size(), other_frames.size()):
		for field: String in fields:
			if parity_frames[index].get(field) != other_frames[index].get(field):
				result.compared = index
				result.first_divergence = {"index": index, "field": field, "frame": parity_frames[index].get("frame"),
					"parity": parity_frames[index].get(field), "other": other_frames[index].get(field)}
				return result
	result.compared = mini(parity_frames.size(), other_frames.size())
	if parity_frames.size() != other_frames.size():
		result.first_divergence = {"index": result.compared, "field": "length", "parity": parity_frames.size(), "other": other_frames.size()}
		return result
	result.equal = true
	return result
