extends RefCounted
## Phase 4 WP-D: English-mode Latin name input for NAME_ENT.
##
## Audit (wiki/english-names.md): the original Symbols page, candidate list 38
## drawn by name_entry_symbol_page_draw (src/ovl/name_entry/name_entry.c:713),
## already holds full-width A-Z (0x8260-0x8279), a-z (0x8281-0x829A) and 0-9
## (0x824F-0x8258). No port-drawn Latin page is therefore added. This "page" is
## a locator over the native Symbols page: a typed character becomes the same
## tab/scroll/cell actions a player would make, so every stored byte comes from
## entry.cell() and is the original full-width Shift-JIS code unit.
##
## Opt-in: with `enabled` false nothing is accepted and no action is produced.
## Feed one action per logical tick: when the entry queue is empty, call
## next_action(entry) and enqueue a non-empty result. Tab, scroll and cell
## actions draw no RNG; the field limits (3/3/5 full-width units) stay native.
## A queued finish (Enter) yields the native START, which draws exactly what a
## START pad press draws (a preset when the field is empty).

const SYMBOL_LIST := 38
const SYMBOL_TAB := 2
const COLUMNS := 16
const VISIBLE_ROWS := 6
const SPACE := 0x8140
## Full-width code units decode() reads, by character. The space unit is read
## back but never typed: commit (rules/new_game.gd _compact) drops every 0x8140,
## so a typed space would use a field slot and vanish, and Space keeps its pad
## (confirm) meaning instead.
const CHAR_CODES := {" ": 0x8140, ".": 0x8144, ",": 0x8143, "-": 0x817C,
	"'": 0x8166, "!": 0x8149, "?": 0x8148, "&": 0x8195}
## Bound on actions one typed item may take (the Symbols page has 24 scroll
## positions); an entry that does not converge drops the item.
const ITEM_ACTION_LIMIT := 64
const ERASE := -1
## Queued finish: Enter ends the field with the native START action.
const FINISH := -2

var enabled := false
var error := ""
var _index: Dictionary = {}
var _pending: Array[int] = []
var _erase_phase := 0
var _item_actions := 0

## Full-width code unit for a typed character, or -1.
static func code_for_char(character: String) -> int:
	if character.length() != 1:
		return -1
	var value := character.unicode_at(0)
	if value >= 0x41 and value <= 0x5A:
		return 0x8260 + value - 0x41
	if value >= 0x61 and value <= 0x7A:
		return 0x8281 + value - 0x61
	if value >= 0x30 and value <= 0x39:
		return 0x824F + value - 0x30
	return int(CHAR_CODES.get(character, -1))

## Native storage bytes for ASCII text: big-endian full-width pairs. Empty when
## any character has no full-width form here.
static func encode(text: String) -> PackedByteArray:
	var result := PackedByteArray()
	for index: int in text.length():
		var code := code_for_char(text[index])
		if code < 0:
			return PackedByteArray()
		result.append(code >> 8)
		result.append(code & 0xFF)
	return result

## ASCII reading of a native name buffer, for port UI and logs only. Stops at
## the first NUL pair; returns "" when any unit is not Latin, digit or listed.
static func decode(bytes: PackedByteArray) -> String:
	var text := ""
	for index: int in range(0, bytes.size() - 1, 2):
		var code: int = bytes[index] << 8 | bytes[index + 1]
		if code == 0:
			break
		var character := ""
		if code >= 0x8260 and code <= 0x8279:
			character = char(0x41 + code - 0x8260)
		elif code >= 0x8281 and code <= 0x829A:
			character = char(0x61 + code - 0x8281)
		elif code >= 0x824F and code <= 0x8258:
			character = char(0x30 + code - 0x824F)
		else:
			for key: String in CHAR_CODES:
				if CHAR_CODES[key] == code:
					character = key
		if character.is_empty():
			return ""
		text += character
	return text

## Index Symbols list 38 from the authenticated expanded name-entry content.
func configure(content: Dictionary) -> bool:
	_index.clear()
	clear()
	var lists: Variant = content.get("candidate_lists")
	if not lists is Array or lists.size() <= SYMBOL_LIST or not lists[SYMBOL_LIST] is Array:
		error = "Latin input needs the expanded original name-entry content (Symbols list 38)"
		return false
	var source: Array = lists[SYMBOL_LIST]
	for offset: int in range(0, source.size() - 1, 2):
		var code: int = int(source[offset]) << 8 | int(source[offset + 1])
		if not _index.has(code):
			_index[code] = offset / 2
	error = ""
	return true

## Which full-width Latin and digit units list 38 holds (the WP-D audit).
func audit() -> Dictionary:
	var result := {"upper": 0, "lower": 0, "digits": 0, "missing": []}
	for character: String in "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789":
		var code := code_for_char(character)
		if not _index.has(code):
			result.missing.append(character)
		elif code <= 0x8258:
			result.digits += 1
		elif code <= 0x8279:
			result.upper += 1
		else:
			result.lower += 1
	return result

## Symbols-page position of a code unit, or {} when list 38 lacks it.
func locate(code: int) -> Dictionary:
	if not _index.has(code):
		return {}
	var index: int = _index[code]
	return {"index": index, "line": index / COLUMNS, "column": index % COLUMNS}

func pending() -> int:
	return _pending.size()

func clear() -> void:
	typing = false
	_pending.clear()
	_erase_phase = 0
	_item_actions = 0

## Queue typed text; returns how many characters were accepted.
func push_text(text: String) -> int:
	if not enabled:
		return 0
	var accepted := 0
	for index: int in text.length():
		var code := code_for_char(text[index])
		if code >= 0 and code != SPACE and _index.has(code):
			_pending.append(code)
			accepted += 1
	return accepted

func push_erase() -> bool:
	if not enabled:
		return false
	_pending.append(ERASE)
	return true

## Queue the field finish: once the text queued before it is entered, the
## next action is the native START ({"kind": "start"}), as the Tab pad key.
func push_finish() -> bool:
	if not enabled:
		return false
	_pending.append(FINISH)
	return true

## Keyboard typing is an explicit mode, so keys bound to pad actions (the
## defaults include Q/W/A/D/Z/C, Enter and Backspace) keep their pad meaning
## outside it. TOGGLE_KEY enters it when that key is not itself bound to a pad
## action (the caller passes the key's pad mask); the entry UI also shows a
## clickable toggle that calls set_typing. Inside the mode letters, digits and
## the listed punctuation are text, Backspace erases, Enter or keypad Enter
## finishes the field (START) and leaves the mode, and Escape leaves the mode.
## Arrows, Tab, Space and every other key stay with the pad mapping.
const TOGGLE_KEY := KEY_F3

## True while keyboard typing mode is on.
var typing := false

func set_typing(value: bool) -> void:
	typing = enabled and value

## True when push_event would take this event. Pure, so the pad can ask it
## first and leave a taken key out of its own sample.
func captures(event: InputEvent, pad_mask: int = 0) -> bool:
	if not enabled or not event is InputEventKey:
		return false
	var key := event as InputEventKey
	if not key.pressed or key.echo or key.ctrl_pressed or key.alt_pressed or key.meta_pressed:
		return false
	if not typing:
		return key.keycode == TOGGLE_KEY and pad_mask == 0
	if key.keycode in [TOGGLE_KEY, KEY_ESCAPE, KEY_BACKSPACE, KEY_ENTER, KEY_KP_ENTER]:
		return true
	return key.unicode > 0x20 and code_for_char(char(key.unicode)) >= 0 and _index.has(code_for_char(char(key.unicode)))

## Takes a key event captures() accepts and queues its effect; returns true
## when the caller must mark it handled.
func push_event(event: InputEvent, pad_mask: int = 0) -> bool:
	if not captures(event, pad_mask):
		return false
	var key := event as InputEventKey
	if not typing:
		set_typing(true)
		return true
	match key.keycode:
		TOGGLE_KEY, KEY_ESCAPE:
			typing = false
			return true
		KEY_BACKSPACE:
			return push_erase()
		KEY_ENTER, KEY_KP_ENTER:
			typing = false
			return push_finish()
	return push_text(char(key.unicode)) == 1

## The next native action for the queued input, or {} when there is none.
func next_action(entry: RefCounted) -> Dictionary:
	if _pending.is_empty():
		return {}
	if not enabled or entry.stage != 1 or entry.step_id not in [1, 3, 5]:
		clear()
		return {}
	var start: int = 0 if entry.step_id == 1 else (3 if entry.step_id == 3 else 6)
	var limit: int = 3 if entry.step_id == 1 else (6 if entry.step_id == 3 else 11)
	var cursor: int = 12 if entry.cursor >= limit else entry.cursor
	if _pending[0] == FINISH:
		_pop()
		return {"kind": "start"}
	_item_actions += 1
	if _item_actions > ITEM_ACTION_LIMIT:
		error = "Latin input did not reach its Symbols cell"
		_pop()
		return {}
	if _pending[0] == ERASE:
		if _erase_phase == 0:
			if cursor == start:
				_pop()
				return next_action(entry)
			_erase_phase = 1
			return {"kind": "cross"}
		if _erase_phase == 1:
			var action := _toward(entry, SPACE)
			if action.is_empty():
				_pop()
			elif action.kind == "cell":
				_erase_phase = 2
			return action
		_pop()
		return {"kind": "cross"}
	if cursor >= limit:
		# A full field drops the key; the native cell press would be a no-op.
		_pop()
		return next_action(entry)
	var typed := _toward(entry, _pending[0])
	if typed.is_empty() or typed.kind == "cell":
		_pop()
	return typed

func _pop() -> void:
	_pending.pop_front()
	_erase_phase = 0
	_item_actions = 0

func _toward(entry: RefCounted, code: int) -> Dictionary:
	var place := locate(code)
	if place.is_empty():
		error = "Symbols list 38 lacks code unit %04x" % code
		return {}
	if entry.tab != SYMBOL_TAB:
		return {"kind": "tab", "value": SYMBOL_TAB}
	if place.line < entry.page_scroll:
		return {"kind": "scroll", "delta": -1}
	if place.line >= entry.page_scroll + VISIBLE_ROWS:
		return {"kind": "scroll", "delta": 1}
	var row: int = place.line - entry.page_scroll
	var expected := PackedByteArray([code >> 8, code & 0xFF])
	if entry.cell(place.column, row) != expected:
		error = "Symbols cell does not hold the located code unit"
		return {}
	return {"kind": "cell", "column": place.column, "row": row}
