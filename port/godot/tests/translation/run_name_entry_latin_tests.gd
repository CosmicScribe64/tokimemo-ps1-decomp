extends SceneTree
## Phase 4 WP-D: English-mode Latin name input over the native Symbols page.
## Needs an installed runtime pack (TOKIMEMO_TEST_INSTALLED_PACK, default
## .asset-cache/port/installed-runtime-v13) for the original entry content,
## rule tables, NAME_ENT media and font. Reconciles with run_name_extended.gd:
## the same original Symbols list 38 is the only source of inserted bytes.
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const Latin = preload("res://flow/name_entry_latin_page.gd")
const Pointer = preload("res://flow/name_entry_pointer.gd")
const Adapter = preload("res://flow/new_game_state_adapter.gd")
const SaveService = preload("res://services/save_service.gd")
const Presenter = preload("res://presentation/name_entry_image_presenter.gd")
const Text = preload("res://presentation/vn_scene_presenter.gd")
const Remaps = preload("res://services/input_remap_service.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
var failures: Array[String] = []
var checks := 0
var tables: Dictionary
var content: Dictionary

func check(value: bool, message: String) -> void:
	checks += 1
	if not value: failures.append(message)

func _fresh() -> Array:
	var rng := Rng.new()
	check(rng.configure(Rng.OPENBIOS_PROFILE,1),"RNG configuration")
	var entry := Entry.new()
	check(entry.begin(tables,content,rng).is_empty(),"entry begin")
	check(entry.act({"kind":"tick"}).is_empty(),"entry initialization tick")
	return [entry,rng]

## Drain the typer one native action per tick; returns the actions taken.
func _drain(entry: RefCounted, latin: RefCounted) -> Array[Dictionary]:
	var taken: Array[Dictionary] = []
	for step: int in 400:
		var action: Dictionary = latin.next_action(entry)
		if action.is_empty(): break
		taken.append(action)
		var result: String = entry.act(action)
		check(result.is_empty(),"native action accepted: "+str(action)+" "+result)
	check(latin.pending() == 0,"typer drained")
	return taken

func _key(unicode: int, keycode: Key = KEY_NONE, ctrl := false, echo := false, alt := false, meta := false) -> InputEventKey:
	var event := InputEventKey.new()
	event.pressed = true; event.unicode = unicode; event.keycode = keycode
	event.ctrl_pressed = ctrl; event.echo = echo; event.alt_pressed = alt; event.meta_pressed = meta
	return event

func _sha(image: Image) -> String:
	var hashing := HashingContext.new(); hashing.start(HashingContext.HASH_SHA256); hashing.update(image.get_data())
	return hashing.finish().hex_encode()

## Pinned from the base snapshot (refs/snapshots/phase4-base-2026-10-29, no
## typer code) with installed-runtime-v13: the native actions that enter "Ann"
## on the Symbols page, and the NAME_ENT frames for a fresh entry and for that
## entry after those actions.
const PINNED_ANN_ACTIONS := [{"kind":"tab","value":2},{"kind":"scroll","delta":1},{"kind":"scroll","delta":1},
	{"kind":"cell","column":6,"row":5},{"kind":"scroll","delta":1},{"kind":"scroll","delta":1},
	{"kind":"cell","column":13,"row":5},{"kind":"cell","column":13,"row":5}]
const PINNED_FRESH_FRAME := "81b118127c663ea9759b98eaaeb5bcab142ffbc18927e2cfcce243d9a42692c6"
const PINNED_ANN_FRAME := "2c1b61d623675df9aee1bc67abe9681cec82e0b2cae59c68aa02dcea8e9cb55b"

func _initialize() -> void:
	var base := OS.get_environment("TOKIMEMO_TEST_INSTALLED_PACK")
	if base.is_empty(): base = ProjectSettings.globalize_path("res://../../.asset-cache/port/installed-runtime-v13").simplify_path()
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(base.path_join("manifest.json")))
	content = Entry.load_content(base.path_join(manifest.roles.name_entry_content))
	tables = Rules.load_tables(base.path_join(manifest.roles.name_entry_rules))
	if content.is_empty() or tables.is_empty() or Entry.content_identity(content) != Entry.EXPANDED_CONTENT_SHA:
		printerr("expanded original name-entry content/rules could not load from ",base)
		quit(1)
		return
	var original_content := content.duplicate(true)

	# Static encoding: ASCII <-> native full-width code units.
	check(Latin.encode("Bob").hex_encode() == "8261828f8282","encode upper/lower to full-width units")
	check(Latin.encode("A1z").hex_encode() == "82608250829a","encode digit and lowercase bounds")
	check(Latin.encode("Boé").is_empty(),"negative: unsupported character rejects whole text")
	check(Latin.decode(Latin.encode("Zoe9")) == "Zoe9","decode round-trip")
	check(Latin.decode("8261828f828200008260".hex_decode()) == "Bob","decode stops at NUL pair")
	check(Latin.decode("82a082a2".hex_decode()) == "","negative: kana name has no Latin reading")
	check(Latin.code_for_char("#") == -1 and Latin.code_for_char("ab") == -1,"negative: unlisted or multi characters")

	# Audit of the original Symbols page (list 38).
	var latin := Latin.new()
	check(not latin.configure({}),"negative: configure needs expanded content")
	var base_content := {}
	for key: String in ["space","hiragana","katakana","preset_family","preset_given","preset_nickname"]: base_content[key] = content[key]
	check(not latin.configure(base_content),"negative: six-table content has no Symbols list")
	check(latin.configure(content),"configure from original list 38: "+latin.error)
	var audit: Dictionary = latin.audit()
	print("Symbols list 38 audit: ",audit)
	check(audit.upper == 26 and audit.lower == 26 and audit.digits == 10 and audit.missing.is_empty(),"list 38 holds full-width A-Z, a-z and 0-9")
	for character: String in "AZaz09 ":
		var place: Dictionary = latin.locate(Latin.code_for_char(character))
		var source: Array = content.candidate_lists[38]
		var code := Latin.code_for_char(character)
		check(not place.is_empty() and int(source[place.index*2]) == code >> 8 and int(source[place.index*2+1]) == code & 0xff,"locator indexes original bytes: "+character)

	# Opt-in: disabled typer accepts nothing and produces nothing.
	var pair := _fresh()
	var entry: Entry = pair[0]
	var rng: Rng = pair[1]
	check(latin.push_text("Bob") == 0 and not latin.push_erase() and not latin.push_event(_key(0x42)),"negative: disabled typer ignores input")
	check(latin.next_action(entry).is_empty(),"negative: disabled typer yields no action")
	latin.enabled = true

	# Typing is an explicit mode: outside it every default pad key (and any
	# letter) keeps its pad meaning, so L1/R1 (A/D), Square/Triangle (Q/W),
	# L2/R2 (Z/C), Enter (START in the entry) and Backspace reach the pad.
	var defaults := {KEY_A: 0x61, KEY_D: 0x64, KEY_Q: 0x71, KEY_W: 0x77, KEY_Z: 0x7a, KEY_C: 0x63, KEY_ENTER: 0, KEY_BACKSPACE: 0, KEY_ESCAPE: 0, KEY_SPACE: 0x20, KEY_TAB: 0, KEY_B: 0x62}
	for keycode: Key in defaults:
		check(not latin.captures(_key(defaults[keycode], keycode)) and not latin.push_event(_key(defaults[keycode], keycode)) and not latin.typing,"outside typing mode the key keeps its pad meaning: "+OS.get_keycode_string(keycode))
	check(latin.pending() == 0,"outside typing mode nothing is queued")
	check(not latin.push_event(_key(0, Latin.TOGGLE_KEY), 0x4) and not latin.typing,"negative: the toggle key does not enter typing while it is bound to a pad action")
	check(latin.captures(_key(0, Latin.TOGGLE_KEY)) and latin.push_event(_key(0, Latin.TOGGLE_KEY)) and latin.typing,"the unbound toggle key enters typing mode")
	check(latin.push_event(_key(0, KEY_ESCAPE)) and not latin.typing and latin.pending() == 0,"Escape leaves typing mode without queuing anything")
	latin.set_typing(true)
	for keycode: Key in [KEY_A, KEY_D, KEY_Q, KEY_W, KEY_Z, KEY_C]:
		check(latin.captures(_key(defaults[keycode], keycode)),"inside typing mode a pad-bound letter is text: "+OS.get_keycode_string(keycode))
	for keycode: Key in [KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_TAB, KEY_SPACE]:
		check(not latin.captures(_key(0x20 if keycode == KEY_SPACE else 0, keycode)),"inside typing mode navigation keys stay with the pad: "+OS.get_keycode_string(keycode))
	check(latin.push_event(_key(0, Latin.TOGGLE_KEY)) and not latin.typing,"the toggle key leaves typing mode")
	latin.set_typing(true)

	# Real default bindings through InputRemaps: every keyboard binding is a
	# pad key the typer leaves alone outside typing mode, and the toggle key
	# is unbound by default.
	var remaps := Remaps.new()
	check(remaps.configure(Preferences.defaults().controls),"default remaps configure: "+remaps.error)
	latin.set_typing(false)
	var bound_keys := 0
	for action: String in Remaps.ACTIONS:
		for spec: Dictionary in remaps.bindings[action]:
			if spec.kind != "key": continue
			var event := _key(0, spec.code)
			event.physical_keycode = spec.code
			var character := OS.get_keycode_string(spec.code)
			if character.length() == 1: event.unicode = character.to_lower().unicode_at(0)
			var mask := remaps.event_mask(event)
			bound_keys += 1
			check(not latin.push_event(event, mask) and not latin.typing and latin.pending() == 0,"default binding %s (%s, mask %x) keeps its pad meaning outside typing mode" % [character, action, mask])
	check(bound_keys >= 16,"every default keyboard binding was driven (%d)" % bound_keys)
	var toggle := _key(0, Latin.TOGGLE_KEY); toggle.physical_keycode = Latin.TOGGLE_KEY
	check(remaps.event_mask(toggle) == 0 and latin.push_event(toggle, remaps.event_mask(toggle)) and latin.typing,"the toggle key is unbound by default and enters typing mode")

	# Key events.
	check(latin.push_event(_key(0x42)),"letter key accepted")
	check(not latin.push_event(_key(0x42,KEY_NONE,true)),"negative: ctrl chord not text")
	check(not latin.push_event(_key(0x42,KEY_NONE,false,true)),"negative: key repeat echo ignored")
	check(not latin.push_event(_key(0x42,KEY_NONE,false,false,true)),"negative: alt chord not text")
	check(not latin.push_event(_key(0x42,KEY_NONE,false,false,false,true)),"negative: meta chord not text")
	check(not latin.push_event(_key(0,KEY_ENTER,false,false,true)),"negative: alt+Enter is not a finish")
	check(not latin.push_event(_key(0x20,KEY_SPACE)) and latin.push_text(" ") == 0,"negative: Space keeps its pad meaning and is never typed")
	check(latin.pending() == 1,"only the plain letter was queued")
	check(not latin.push_event(_key(0x23)),"negative: unlisted symbol key ignored")
	var motion := InputEventMouseMotion.new()
	check(not latin.push_event(motion),"negative: non-key event ignored")
	latin.clear()

	# Typing 1-4 letters, mixed case, field limits and RNG neutrality.
	var draws_before := rng.draw_count
	var state_before := rng.state
	check(latin.push_text("Bob") == 3,"three letters queued")
	var family_actions := _drain(entry,latin)
	check(entry.names.family.slice(0,6) == Latin.encode("Bob"),"family stores native full-width units")
	check(entry.names.family.slice(6,8) == PackedByteArray([0,0]),"family terminator untouched")
	check(entry.tab == 2,"typing shows the native Symbols tab")
	check(rng.draw_count == draws_before and rng.state == state_before,"typing draws no RNG")
	check(latin.push_text("x") == 1,"fourth letter queued")
	_drain(entry,latin)
	check(entry.names.family.slice(0,6) == Latin.encode("Bob"),"negative: full 3-unit family drops a fourth letter")
	# Backspace erases with the native space unit, then retypes.
	latin.push_erase()
	_drain(entry,latin)
	check(entry.names.family.slice(0,6) == Latin.encode("Bo")+PackedByteArray(content.space),"backspace restores native blank")
	check(entry.cursor == 2,"backspace leaves cursor on erased slot")
	latin.push_text("y")
	_drain(entry,latin)
	check(entry.names.family.slice(0,6) == Latin.encode("Boy"),"retype after backspace")
	check(entry.act({"kind":"start"}).is_empty() and entry.step_id == 2,"START finishes family")
	check(entry.act({"kind":"tick"}).is_empty() and entry.step_id == 3,"given field initialization")
	latin.push_text("A")
	_drain(entry,latin)
	check(entry.names.given.slice(0,6) == Latin.encode("A")+PackedByteArray(content.space)+PackedByteArray(content.space),"one letter")
	latin.push_erase(); latin.push_erase()
	_drain(entry,latin)
	check(entry.names.given.slice(0,2) == PackedByteArray(content.space) and entry.cursor == 3,"backspace at field start is a no-op after erase")
	latin.push_text("aBc")
	_drain(entry,latin)
	check(entry.names.given.slice(0,6) == Latin.encode("aBc"),"mixed case")
	check(entry.act({"kind":"start"}).is_empty() and entry.act({"kind":"tick"}).is_empty() and entry.step_id == 5,"nickname field")
	latin.push_text("Jo3y")
	_drain(entry,latin)
	check(entry.names.nickname.slice(0,8) == Latin.encode("Jo3y"),"four units in the 5-unit nickname")
	check(latin.push_text("z") == 1,"fifth letter queued")
	_drain(entry,latin)
	check(latin.push_text("q") == 1,"sixth letter queued")
	check(_drain(entry,latin).is_empty(),"full field drops the key without moving the Symbols page")
	check(entry.names.nickname.slice(0,10) == Latin.encode("Jo3yz"),"negative: full 5-unit nickname drops the sixth letter")
	# Backspace on the full nickname: the native blank sits on Symbols line 0,
	# above the lowercase rows, so the erase has to scroll back up.
	var scroll_before := entry.page_scroll
	latin.push_erase()
	var erase_actions := _drain(entry,latin)
	check(scroll_before > 0 and erase_actions.has({"kind":"scroll","delta":-1}),"erase scrolls back to the native blank: "+str(erase_actions))
	check(erase_actions[0] == {"kind":"cross"} and erase_actions[-1] == {"kind":"cross"},"erase is cross, blank, cross")
	check(entry.names.nickname.slice(0,10) == Latin.encode("Jo3y")+PackedByteArray(content.space),"backspace on a full nickname blanks the last unit")
	# Leaving the field clears any queued input instead of carrying it over.
	latin.push_text("Q")
	check(entry.act({"kind":"start"}).is_empty() and entry.step_id == 6,"nickname finished")
	check(latin.next_action(entry).is_empty() and latin.pending() == 0,"negative: queued text dropped outside name fields")

	# Replay the exact native actions on a fresh entry with no typer: the same
	# bytes, RNG and trace (Japanese-mode equivalence of the typed sequence).
	var replay_pair := _fresh()
	var replay: Entry = replay_pair[0]
	for action: Dictionary in family_actions: replay.act(action)
	check(replay.names.family == Latin.encode("Bob")+PackedByteArray([0,0]),"typed actions are ordinary native actions")

	# Enter finishes the field with the native START after the queued text, and
	# never re-presses the cell under the hand (no duplicate letter).
	var enter_pair := _fresh()
	var entered: Entry = enter_pair[0]
	var enter_rng: Rng = enter_pair[1]
	var typer := Latin.new(); typer.configure(content); typer.enabled = true
	typer.push_text("Al")
	check(not typer.push_event(_key(0,KEY_ENTER)),"negative: outside typing mode Enter stays with the pad")
	typer.set_typing(true)
	check(typer.push_event(_key(0,KEY_ENTER)) and not typer.typing,"Enter queued as the field finish and leaves typing mode")
	var draws_enter := enter_rng.draw_count
	var enter_actions := _drain(entered,typer)
	check(enter_actions[-1] == {"kind":"start"} and enter_actions.count({"kind":"start"}) == 1,"Enter yields one native START, last")
	check(entered.step_id == 2,"Enter finished the family field")
	check(entered.names.family == Latin.encode("Al")+PackedByteArray(content.space)+PackedByteArray([0,0]),"Enter stores the typed name without a duplicate letter")
	check(enter_rng.draw_count == draws_enter+1,"Enter's START draws once, as the native finish always does")
	check(entered.act({"kind":"tick"}).is_empty() and entered.step_id == 3,"given field after Enter")
	typer.set_typing(true)
	check(typer.push_event(_key(0,KEY_KP_ENTER)),"keypad Enter queued as the field finish")
	_drain(entered,typer)
	check(entered.step_id == 4,"keypad Enter finishes an empty given field natively (preset)")
	typer.set_typing(true)
	typer.enabled = false
	check(not typer.push_event(_key(0,KEY_ENTER)),"negative: disabled typer leaves Enter to the pad")
	typer.enabled = true; typer.clear()
	check(not typer.typing,"clear leaves typing mode")

	# Presets unchanged: an empty field still takes the original preset.
	var empty_pair := _fresh()
	var empty: Entry = empty_pair[0]
	var empty_rng: Rng = empty_pair[1]
	var draws := empty_rng.draw_count
	check(empty.act({"kind":"start"}).is_empty(),"empty family finish")
	check(empty_rng.draw_count == draws+1,"finish draws once as native")
	check(content.preset_family == original_content.preset_family and content.preset_nickname == original_content.preset_nickname,"preset tables untouched")
	var found := false
	for row: Array in content.preset_family: found = found or empty.names.family == PackedByteArray(row)
	check(found,"empty field receives an original preset")

	# Commit and save/reload: stored bytes equal the native full-width codes.
	for action: Dictionary in [{"kind":"tick"},{"kind":"circle"},{"kind":"circle"},{"kind":"circle"},{"kind":"tick"},{"kind":"yes"},
			{"kind":"tick"},{"kind":"circle"},{"kind":"circle"},{"kind":"circle"},{"kind":"tick"},{"kind":"yes"},{"kind":"tick"}]:
		check(entry.act(action).is_empty(),"profile flow "+str(action))
	check(entry.stage == 2 and entry.step_id == 5,"native commit reached")
	check(entry.state_bytes.slice(0xd4,0xdc) == Latin.encode("Boy")+PackedByteArray([0,0]),"family at 0xd4 in native form")
	check(entry.state_bytes.slice(0xdc,0xe4) == Latin.encode("aBc")+PackedByteArray([0,0]),"given at 0xdc in native form")
	var nick := Latin.encode("Jo3y"); nick.resize(16)
	check(entry.state_bytes.slice(0xe4,0xf4) == nick,"nickname at 0xe4 compacted and NUL padded")
	var full := Latin.encode("BoyaBc"); full.resize(16)
	check(entry.state_bytes.slice(0xc4,0xd4) == full,"full name at 0xc4 as native concatenation")
	var adapted: Adapter.Outcome = Adapter.from_entry(entry,0,0,{"initial_mascot":0})
	check(adapted.error.is_empty(),"route state adapter: "+adapted.error)
	var gameplay := {"profile_family_bytes":adapted.state.rule_state.profile_family_bytes,
		"profile_given_bytes":adapted.state.rule_state.profile_given_bytes,
		"profile_nickname_bytes":adapted.state.rule_state.profile_nickname_bytes,"state_bytes":Array(entry.state_bytes)}
	var root_dir := ProjectSettings.globalize_path("user://wp-d-latin-%d" % Time.get_ticks_usec())
	var saves := SaveService.new(root_dir)
	var snapshot := {"boundary":"stable","source_version":"wp-d-test","content_version":"wp-d-test","behavior_version":"wp-d-test",
		"gameplay_state":gameplay,"native_rng":rng.checkpoint(),"native_clocks":{},"queued_service_events":[]}
	var saved: Dictionary = saves.save_slot("latin",snapshot)
	check(saved.error.is_empty(),"save: "+str(saved.error))
	var loaded: Dictionary = saves.load_slot("latin")
	check(loaded.error.is_empty(),"reload: "+str(loaded.get("error")))
	if loaded.error.is_empty():
		var back: Dictionary = loaded.snapshot.gameplay_state
		check(PackedByteArray(back.profile_family_bytes) == Latin.encode("Boy")+PackedByteArray([0,0]),"reloaded family bytes are native codes")
		check(PackedByteArray(back.state_bytes) == entry.state_bytes,"reloaded state bytes identical")
		check(Latin.decode(PackedByteArray(back.profile_nickname_bytes)) == "Jo3y","reloaded nickname reads back")
		check(not back.has("latin_name") and back.keys().size() == gameplay.keys().size(),"no sidecar name field")

	# Pointer: typed cell can carry the native hand; clicks reach Latin cells.
	var pointer := Pointer.new()
	var pointer_pair := _fresh()
	var clicked: Entry = pointer_pair[0]
	check(clicked.act({"kind":"tab","value":2}).is_empty(),"Symbols tab")
	var place: Dictionary = latin.locate(Latin.code_for_char("B"))
	while clicked.page_scroll + 5 < place.line: clicked.act({"kind":"scroll","delta":1})
	pointer.focus_cell(place.column,place.line-clicked.page_scroll)
	var target: Dictionary = pointer.confirm(clicked)
	check(target == {"kind":"cell","column":place.column,"row":place.line-clicked.page_scroll},"pointer hit on typed cell")
	check(clicked.act(target).is_empty() and clicked.names.family.slice(0,2) == Latin.encode("B"),"pointer click inserts the same unit")
	pointer.focus_cell(16,0)
	check(pointer.confirm(clicked).get("kind","") != "cell","negative: arrow column is not a glyph cell")

	# Frames: the presenter is unchanged; a typed entry and its native replay
	# render byte-identical frames, and a disabled typer leaves frames alone.
	var font := Text.new()
	var media_path := base.path_join(manifest.roles.name_entry_media)
	if font.open_font_only(base.path_join(manifest.roles.font_root)):
		var presenter := Presenter.new()
		check(presenter.open_pack(media_path,FileAccess.get_sha256(media_path),font),"NAME_ENT media opens")
		var typed_pair := _fresh()
		var typed: Entry = typed_pair[0]
		var ann := Latin.new(); ann.configure(content); ann.enabled = true
		ann.push_text("Ann")
		var typed_actions := _drain(typed,ann)
		check(typed_actions == PINNED_ANN_ACTIONS,"typed actions equal the base-snapshot native actions: "+str(typed_actions))
		check(_sha(presenter.render(typed)) == PINNED_ANN_FRAME,"typed frame equals the pinned base-snapshot native frame")
		var idle_pair := _fresh()
		var idle_entry: Entry = idle_pair[0]
		var idle := Latin.new(); idle.configure(content)
		idle.push_text("Zed"); idle.push_event(_key(0,KEY_ENTER)); idle.next_action(idle_entry)
		check(_sha(presenter.render(idle_entry)) == PINNED_FRESH_FRAME,"disabled typer leaves the pinned base-snapshot frame")
	else:
		check(false,"native font opens: "+font.error)
	print("NAME Latin checks: ",checks," failures: ",failures)
	quit(0 if failures.is_empty() else 1)
