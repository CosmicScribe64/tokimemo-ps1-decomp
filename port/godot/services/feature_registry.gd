extends RefCounted
## T-39230: every modern feature and every preference key, with its class,
## its invariant and what Parity mode does with it. Parity mode
## (services/parity_mode.gd) and the invariance harness
## (tests/support/parity_invariance.gd) read only this table, and
## tests/services/run_feature_registry.gd fails when a preference key, a
## settings row, a pause-menu row or a Welcome row is missing from it.
##
## class:
##   presentation_only  changes what the player sees or hears outside native
##                      state (window, scaling, side art, volume, text drawing)
##   input_ux           changes how physical input or the port's own menus
##                      behave; the native pad word for a given PS1 button is
##                      unchanged
##   game_affecting     can change when or whether native iterations run, or
##                      writes or restores native state (holds, auto input,
##                      skip, quick save/load, return to title)
## parity:
##   original  a preference pinned to its original value; changes are refused
##   host      a desktop setting with no PS1 counterpart (window size, volume,
##             bindings); adjustable in Parity mode, never read by native code
##   refused   a feature that cannot be used in Parity mode
## invariance (what the modern-vs-parity harness checks for the entry):
##   state_and_image  enabled in the "modern" run; per-frame state hash and
##                    native 320x240 Image hash must equal the parity run
##   state_only       enabled only in the "modern_state_only" run; may change
##                    pixels of the native Image (text drawing), never state
##   excluded         never enabled by the harness; the entry's invariant says
##                    what holds instead and which test covers it
## modern: the value the harness uses for "everything on" (preference entries
## whose invariance is not "excluded"). It may equal the original only when
## the entry's note says why.

const PRESENTATION_ONLY := "presentation_only"
const INPUT_UX := "input_ux"
const GAME_AFFECTING := "game_affecting"
const CLASSES := [PRESENTATION_ONLY, INPUT_UX, GAME_AFFECTING]
const PARITY_POLICIES := ["original", "host", "refused"]
const INVARIANCE := ["state_and_image", "state_only", "excluded"]
## Audio buses, kept equal to UserPreferencesService.BUSES by the registry test.
const BUSES := ["Master", "Voice", "BGM", "SFX", "Movie"]

const _HOLD := "Holds the presentation in app/first_arrival_boot.gd, which returns before the native tick while the hold lasts, so native iterations shift against wall-clock input. Source state is never written. Excluded from invariance; Parity pins the original value, which never holds."
const _WINDOW := "Moves or resizes the OS window only; the native 320x240 canvas, tick and RNG are unchanged (WP10 fidelity matrix)."

## Preference entries, keyed "section.key" exactly as in
## UserPreferencesService.defaults(). audio.buses and controls.bindings are one
## entry each.
const PREFERENCES := {
	"profile.preset": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "custom",
		"invariant": "Names the active value bundle; settings_presets.gd bundles only keys registered here."},
	"profile.mode": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "excluded",
		"invariant": "The Parity/Modern switch itself. Never read by native code; the harness sets it per run. Locked while Parity is forced by --parity or TOKIMEMO_PARITY=1."},
	"display.mode": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": "borderless", "invariant": _WINDOW},
	"display.width": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": 1920, "invariant": _WINDOW},
	"display.height": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": 1080, "invariant": _WINDOW},
	"display.monitor": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": 0,
		"note": "Monitor 0 is the only monitor in Docker/Xvfb runs.", "invariant": _WINDOW},
	"display.vsync": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": false,
		"invariant": "Swap interval only; the native tick runs on physics frames."},
	"display.size_mode": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": "custom", "invariant": _WINDOW},
	"display.long_confirmation": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": true,
		"invariant": "Length of the Keep/Revert prompt in the settings overlay only."},
	"display.ui_scale": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": 2.0,
		"invariant": "Scales the modern settings layer only, never the native canvas."},
	"display.fps": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": 144,
		"invariant": "Engine.max_fps caps rendered frames; native iterations run on the fixed physics tick. Parity keeps the original 60."},
	"display.aspect": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "auto",
		"invariant": "Window framing around the 4:3 canvas; the canvas Image is unchanged (T-30001)."},
	"display.filter": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "linear",
		"invariant": "Window scaling of the finished canvas texture; the canvas Image is unchanged."},
	"display.wing_fill": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "source",
		"invariant": "Fills the window areas beside the canvas; the canvas Image is unchanged."},
	"display.wing_art": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "seasonal",
		"invariant": "Decorative side art behind the canvas (T-39220); the canvas Image is unchanged."},
	"display.wing_art_command": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "fixed:school",
		"invariant": "Per scene type side art; only acts while display.wing_art is on."},
	"display.wing_art_story": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "fixed:park",
		"invariant": "Per scene type side art; only acts while display.wing_art is on."},
	"display.wing_art_other": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "fixed:town",
		"invariant": "Per scene type side art; only acts while display.wing_art is on."},
	"display.bars": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "black",
		"note": "Black is the only bar style today; a new style must stay outside the canvas.",
		"invariant": "Colour of the bars outside the canvas."},
	"audio.buses": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image",
		"modern": {"Master": {"volume": 0.5, "mute": false}, "Voice": {"volume": 0.8, "mute": false}, "BGM": {"volume": 0.3, "mute": false},
			"SFX": {"volume": 0.6, "mute": false}, "Movie": {"volume": 0.7, "mute": true}},
		"invariant": "AudioServer bus gain and mute after native audio commands are issued; the command stream is unchanged."},
	"audio.device": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": "Default",
		"note": "Containers expose only the Default device.", "invariant": "Output device selection only."},
	"audio.mono": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": true,
		"invariant": "Stereo downmix on the Master bus after native mixing."},
	"audio.mute_unfocused": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "state_and_image", "modern": true,
		"invariant": "Mutes the Master bus while the window is unfocused; the game itself already stops on focus loss."},
	"controls.swap_confirm": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": true,
		"invariant": "Swaps which physical face button produces the PS1 Circle and Cross bits; the harness presses PS1 buttons through the active mapping, so the native pad word is unchanged."},
	"controls.deadzone": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": 0.5,
		"invariant": "Stick threshold before a d-pad bit; scripted input uses digital buttons."},
	"controls.repeat_delay": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": 0.6,
		"invariant": "Key repeat inside the modern settings overlay only."},
	"controls.repeat_interval": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": 0.12,
		"invariant": "Key repeat inside the modern settings overlay only."},
	"controls.bindings": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image",
		"modern": {"square": [{"kind": "key", "code": KEY_Q}, {"kind": "button", "code": JOY_BUTTON_Y}],
			"triangle": [{"kind": "key", "code": KEY_W}, {"kind": "button", "code": JOY_BUTTON_X}]},
		"invariant": "Which physical control produces each PS1 bit; the harness presses PS1 buttons through the active mapping, so the native pad word is unchanged."},
	"controls.prompt_style": {"class": INPUT_UX, "parity": "host", "invariance": "state_and_image", "modern": "xbox",
		"invariant": "Text labels in the modern layer only."},
	"accessibility.text_speed": {"class": GAME_AFFECTING, "parity": "original", "invariance": "excluded", "invariant": _HOLD},
	"accessibility.auto_delay": {"class": GAME_AFFECTING, "parity": "original", "invariance": "excluded", "invariant": _HOLD},
	"accessibility.auto_advance": {"class": GAME_AFFECTING, "parity": "original", "invariance": "excluded",
		"invariant": "Generates confirm input after text and voice finish; changes the input stream by design. Parity pins it off."},
	"accessibility.skip": {"class": GAME_AFFECTING, "parity": "original", "invariance": "excluded",
		"invariant": "Policy of the port_skip shortcut, which pulses the native Start skip path; the shortcut itself is the refused feature feature.prologue_skip_shortcut."},
	"accessibility.font_scale": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": 1.5,
		"invariant": "Redraws dialogue text inside the native text window through the accessible painter; state unchanged, native Image differs only in the text window."},
	"accessibility.backdrop": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": 0.5,
		"invariant": "Blends the text window backdrop inside the native Image; state unchanged."},
	"accessibility.reduce_flashes": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": true,
		"invariant": "Limits frame-to-frame brightness jumps of the native Image; state unchanged."},
	"accessibility.reduce_shake": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": true,
		"invariant": "No consumer yet (AccessibilityService.shake_offset has no caller); when wired it may offset the window-space display only."},
	"accessibility.subtitles": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": true,
		"invariant": "No consumer and no settings row yet (no cue pack); when added it draws in the modern layer only."},
	"accessibility.text_box": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": "modern",
		"invariant": "Modern dialogue box drawn over the window, not into the canvas Image."},
	"accessibility.reduce_motion": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_and_image", "modern": true,
		"invariant": "Stills the decorative side art only."},
	"language.bundle": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "en",
		"invariant": "The bundle itself changes only drawn text and art: with no keyboard typing, GameState, RNG, input tick and text-work timing are unchanged; text and art regions of the native Image may change. 'en' also makes Latin name typing available, which generates native input and is registered separately as feature.latin_name_typing (excluded; invariance routes do not type)."},
	"language.allow_draft_images": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": true,
		"invariant": "Admits unreviewed translated images; same contract as language.bundle."},
	"language.text": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "ja",
		"note": "Set by the image pack picker; stays 'ja' without an installed pack.", "invariant": "Image pack language label; same contract as language.bundle."},
	"language.text_manifest": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Needs an installed pack; the run uses one only when PARITY_INVARIANCE_PACKS names it.", "invariant": "Translated image pack; same contract as language.bundle."},
	"language.text_sha256": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Pins language.text_manifest.", "invariant": "Digest of the translated image pack."},
	"language.voice": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "ja",
		"note": "Set by the voice pack picker.", "invariant": "Voice pack language label."},
	"language.voice_manifest": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Needs an installed pack.", "invariant": "Custom voice pack: replaces audio samples; the voice command stream and completion timing must stay native (T-39016)."},
	"language.voice_sha256": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Pins language.voice_manifest.", "invariant": "Digest of the voice pack."},
	"language.dialogue_manifest": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Needs an installed pack.", "invariant": "Dialogue text pack; same contract as language.bundle."},
	"language.dialogue_sha256": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Pins language.dialogue_manifest.", "invariant": "Digest of the dialogue pack."},
	"language.ui_text_manifest": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Needs an installed pack.", "invariant": "Interface text layer; same contract as language.bundle."},
	"language.ui_text_sha256": {"class": PRESENTATION_ONLY, "parity": "original", "invariance": "state_only", "modern": "",
		"note": "Pins language.ui_text_manifest.", "invariant": "Digest of the interface text layer."},
}

## Features that are not preference keys. Each runtime entry point asks
## ParityMode.refusal(id) before acting.
const FEATURES := {
	"feature.settings_overlay": {"class": INPUT_UX, "parity": "host", "invariance": "excluded",
		"invariant": "F10 / L3+R3 overlay. Pauses the SceneTree, so no native iteration runs while it is open; closing resumes the same state. Needed in Parity mode for window and volume settings and to leave Parity."},
	"feature.pause_menu": {"class": INPUT_UX, "parity": "host", "invariance": "excluded",
		"invariant": "Pauses the SceneTree like the settings overlay; Resume continues from the same state."},
	"feature.quit_game": {"class": INPUT_UX, "parity": "host", "invariance": "excluded",
		"invariant": "Quits the application after confirmation; writes nothing."},
	"feature.navigation": {"class": INPUT_UX, "parity": "host", "invariance": "excluded",
		"invariant": "Page buttons, Back, Return to game, Keep/Revert and Continue inside the modern menus; they write only preferences through the entries they lead to."},
	"feature.audio_preview": {"class": PRESENTATION_ONLY, "parity": "host", "invariance": "excluded",
		"invariant": "Plays a generated test tone on a bus from the settings overlay (tree paused)."},
	"feature.controls_reset": {"class": INPUT_UX, "parity": "host", "invariance": "excluded",
		"invariant": "Restores default bindings; same contract as controls.bindings."},
	"feature.quick_save": {"class": GAME_AFFECTING, "parity": "refused", "invariance": "excluded",
		"invariant": "Writes the guarded quick slot at command selection or the first result (save host). The original game has no quick slot."},
	"feature.quick_load": {"class": GAME_AFFECTING, "parity": "refused", "invariance": "excluded",
		"invariant": "Restores native state from the quick slot. The original game has no quick slot."},
	"feature.return_to_title": {"class": GAME_AFFECTING, "parity": "refused", "invariance": "excluded",
		"invariant": "Pause-menu return to the title screen; abandons the current native state."},
	"feature.prologue_skip_shortcut": {"class": GAME_AFFECTING, "parity": "refused", "invariance": "excluded",
		"invariant": "The port_skip shortcut (Ctrl) pulses the native Start skip path after a read-history check. In Parity mode only the native Start button skips."},
	"feature.latin_name_typing": {"class": GAME_AFFECTING, "parity": "refused", "invariance": "excluded",
		"invariant": "English-mode keyboard typing on NAME_ENT (flow/name_entry_latin_page.gd, F3 toggle) turns typed keys into native tab, scroll, cell and START actions, so it is auto input that sets the stored name bytes. Its own tests cover the byte mapping. The original game has only pad entry."},
}

## Settings overlay, pause menu and Welcome rows, keyed by the focus key the
## widgets register (ui/settings/settings_widgets.gd), to the entries they
## change. Dynamic keys use ROW_PREFIXES.
const ROWS := {
	# settings shell
	"Keep display": ["feature.navigation"], "Revert": ["feature.navigation"], "Return to game": ["feature.navigation"],
	"prompt:keep": ["feature.navigation"], "prompt:revert": ["feature.navigation"],
	"Apply display changes": ["feature.navigation"], "advanced_open": ["feature.navigation"], "advanced_close": ["feature.navigation"],
	# Game page
	"Play mode": ["profile.mode"], "Quick save": ["feature.quick_save"], "Load quick save": ["feature.quick_load"],
	# Language page
	"Game language": ["language.bundle", "language.text", "language.text_manifest", "language.text_sha256",
		"language.dialogue_manifest", "language.dialogue_sha256", "language.ui_text_manifest", "language.ui_text_sha256"],
	"Draft English artwork (unreviewed)": ["language.allow_draft_images"],
	"custom_images": ["language.text", "language.text_manifest", "language.text_sha256"],
	"custom_images_clear": ["language.text", "language.text_manifest", "language.text_sha256"],
	"custom_dialogue": ["language.dialogue_manifest", "language.dialogue_sha256"],
	"custom_dialogue_clear": ["language.dialogue_manifest", "language.dialogue_sha256"],
	"custom_text_layer": ["language.ui_text_manifest", "language.ui_text_sha256"],
	"custom_text_layer_clear": ["language.ui_text_manifest", "language.ui_text_sha256"],
	"custom_voice": ["language.voice", "language.voice_manifest", "language.voice_sha256"],
	"custom_voice_clear": ["language.voice", "language.voice_manifest", "language.voice_sha256"],
	# Display page
	"Window mode": ["display.mode"], "Monitor": ["display.monitor"],
	"Window size": ["display.width", "display.height", "display.size_mode"],
	"Scaling": ["display.filter"], "Scene layout": ["display.aspect"], "Side areas": ["display.wing_fill"],
	"Interface size": ["display.ui_scale"], "Frame limit": ["display.fps"], "Vertical sync": ["display.vsync"],
	"Long display confirmation (60 seconds)": ["display.long_confirmation"], "Side art": ["display.wing_art"],
	"Side art: command screens": ["display.wing_art_command"], "Side art: dates and talks": ["display.wing_art_story"],
	"Side art: other scenes": ["display.wing_art_other"],
	# Audio page
	"Output device": ["audio.device"], "Mute in background": ["audio.mute_unfocused"], "Mono output": ["audio.mono"],
	# Controls page
	"Prompt style": ["controls.prompt_style"], "Swap confirm and cancel": ["controls.swap_confirm"],
	"Stick deadzone": ["controls.deadzone"], "Menu repeat delay": ["controls.repeat_delay"],
	"Menu repeat interval": ["controls.repeat_interval"], "reset_keyboard": ["feature.controls_reset"],
	"reset_controller": ["feature.controls_reset"], "Reset controls": ["feature.controls_reset"],
	# Text & Story page
	"Text speed": ["accessibility.text_speed"], "Auto-advance": ["accessibility.auto_advance"],
	"Extra reading time after voice (seconds)": ["accessibility.auto_delay"], "Skip": ["accessibility.skip"],
	"Text size": ["accessibility.font_scale"], "Text box opacity": ["accessibility.backdrop"],
	"Dialogue text box": ["accessibility.text_box"], "Reduce screen flashes": ["accessibility.reduce_flashes"],
	"Reduce screen shake": ["accessibility.reduce_shake"], "Reduce motion": ["accessibility.reduce_motion"],
	# pause menu
	"Resume": ["feature.pause_menu"], "Settings": ["feature.settings_overlay"],
	"Return to title": ["feature.return_to_title"], "Quit game": ["feature.quit_game"],
	"quit_confirm": ["feature.quit_game"], "quit_cancel": ["feature.quit_game"],
	# Welcome step
	"Preset": ["profile.preset"], "Window": ["display.mode"], "try_window": ["display.mode"],
	"keep_window": ["display.mode"], "revert_window": ["display.mode"], "continue": ["feature.navigation"],
}
## Focus-key prefixes for rows built per action, page, preset or bus.
const ROW_PREFIXES := {"binding:": ["controls.bindings"], "page:": ["feature.navigation"], "preset:": ["profile.preset"]}
## Per-bus audio rows: "<bus>" (volume slider), "<bus> mute", "<bus> preview".
const BUS_ROW_SUFFIXES := {"": ["audio.buses"], " mute": ["audio.buses"], " preview": ["feature.audio_preview"]}

static func entry(id: String) -> Dictionary:
	if PREFERENCES.has(id): return PREFERENCES[id]
	return FEATURES.get(id, {})

static func registered(id: String) -> bool:
	return PREFERENCES.has(id) or FEATURES.has(id)

static func class_of(id: String) -> String:
	return str(entry(id).get("class", ""))

static func parity_policy(id: String) -> String:
	return str(entry(id).get("parity", ""))

static func preference_ids() -> Array:
	return PREFERENCES.keys()

## Flattened "section.key" names of a preferences dictionary, with audio.buses
## and controls.bindings as single keys (how the registry lists them).
static func preference_keys(values: Dictionary) -> Array[String]:
	var result: Array[String] = []
	for section: Variant in values:
		if not values[section] is Dictionary:
			result.append(str(section)); continue
		for key: Variant in values[section]: result.append("%s.%s" % [section, key])
	return result

## Entries a settings row changes, or [] when the row is unregistered.
static func row_entries(focus_key: String) -> Array:
	if ROWS.has(focus_key): return ROWS[focus_key]
	for prefix: String in ROW_PREFIXES:
		if focus_key.begins_with(prefix) and focus_key.length() > prefix.length(): return ROW_PREFIXES[prefix]
	for bus: String in BUSES:
		for suffix: String in BUS_ROW_SUFFIXES:
			if focus_key == bus + suffix: return BUS_ROW_SUFFIXES[suffix]
	return []

## True when Parity mode must lock the row: any entry it changes is pinned
## to its original value or refused. An unregistered row is locked too, so a
## new control is never live in Parity mode before it is classified.
static func row_locked_in_parity(focus_key: String) -> bool:
	var ids := row_entries(focus_key)
	if ids.is_empty(): return true
	for id: String in ids:
		if parity_policy(id) != "host": return true
	return false

## Structural problems of one entry ("" when valid). Used by the registry test
## and by ParityMode before it trusts the table.
static func entry_problem(id: String) -> String:
	if not registered(id): return "%s is not registered" % id
	return spec_problem(id, entry(id), PREFERENCES.has(id))

## Problems of a spec; tests pass invented specs to prove each rule refuses.
static func spec_problem(id: String, spec: Dictionary, is_preference: bool) -> String:
	if str(spec.get("class", "")) not in CLASSES: return "%s has no class (one of %s)" % [id, CLASSES]
	if str(spec.get("parity", "")) not in PARITY_POLICIES: return "%s has no parity policy (one of %s)" % [id, PARITY_POLICIES]
	if str(spec.get("invariance", "")) not in INVARIANCE: return "%s has no invariance (one of %s)" % [id, INVARIANCE]
	if str(spec.get("invariant", "")).strip_edges().length() < 12: return "%s does not declare its invariant" % id
	if spec["class"] == GAME_AFFECTING:
		if spec["parity"] == "host": return "%s is game_affecting but adjustable in Parity mode" % id
		if spec["invariance"] != "excluded": return "%s is game_affecting but not excluded from invariance" % id
	if is_preference:
		if spec["parity"] == "refused": return "%s is a preference; use parity 'original', not 'refused'" % id
		if spec["invariance"] != "excluded" and not spec.has("modern"): return "%s has no modern value for the invariance harness" % id
	elif spec["parity"] == "original":
		return "%s is a feature; use parity 'host' or 'refused'" % id
	return ""
