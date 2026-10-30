extends RefCounted
## T-29012: action-level NAME_ENT stage1/2 controller; no captured state import.
## Native name_finish and birthday_finalize are run at their source UI actions.
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const CoreState = preload("res://core/game_state.gd")
const Math = preload("res://core/native_math.gd")
const CONTENT_SHA: String = "7458e8d0ee2d286c17ffe9b0d4201544ac716325273a97e02e5b12ab9fd660af"
const EXPANDED_CONTENT_SHA: String = "c54066fcfebc3526abecab420a62c1817bca61e9d570c2fab391be6e8b6f8243"
const TAB_DONE: int = 4
var stage: int = 1
var step_id: int = 0
var cursor: int = 0
var tab: int = 0
var reading: int = 0
var page_scroll: int = 0
var error: String = ""
var blocked: String = ""
var names: Dictionary = {}
var player: Dictionary = {}
var shiori: Dictionary = {}
var state_bytes := PackedByteArray()
var known_bytes := PackedByteArray()
var trace: Array[Dictionary] = []
var _tables: Dictionary = {}
var _content: Dictionary = {}
var _rng: Rng

## The BIOS rand seed the entry starts from. On the source New Game path the
## last srand before NAME_ENT is file_select_init_settings'
## srand(system_data.seed) (file_select.c:735); nothing reseeds again until the
## prologue's srand(frame_count) (prologue.c:206). Between that srand and
## NAME_ENT the source also draws rand() & 0xF in the file-select iris update
## (file_select.c:584-593), once per frame and edge hit. The port does not run
## those draws, so the NAME_ENT draws (name_finish, birthday_finalize, the
## girls[1..4] club draws in name_entry_new_game_setup) are deterministic for a
## given input but do not match the original sequence. The installed entry
## reads system_seed right after the sys_init reset (new_game_entry_boot.gd),
## so the value is 0; the original's system_data.seed already carries the
## boot/title timer_tick history (main_loop.c:91-98). Never a host clock.
static func file_select_seed(model: CoreState) -> int:
	return Math.u32(model.system_seed)

static func _byte_array(value: Variant, count: int = -1) -> bool:
	if not value is Array or (count >= 0 and value.size() != count): return false
	for v in value:
		if typeof(v) != TYPE_INT or v < 0 or v > 255: return false
	return true

static func content_valid(content: Dictionary) -> bool:
	if content.size() not in [6,9] or not _byte_array(content.get("space"),2): return false
	for key in ["hiragana","katakana"]:
		if not content.get(key) is Array or content[key].size() != 17: return false
		for row in content[key]:
			if not _byte_array(row) or row.size() > 11: return false
	for key in ["preset_family","preset_given","preset_nickname"]:
		if not content.get(key) is Array or content[key].size() != 15: return false
		for row in content[key]:
			if not _byte_array(row,12 if key == "preset_nickname" else 8) or row[-2] != 0 or row[-1] != 0: return false
	if content.size() == 9:
		for key: String in ["candidate_lists","reading_labels","reading_rows"]:
			if not content.get(key) is Array or content[key].size() != (9 if key == "reading_rows" else 45): return false
			for row in content[key]:
				if not _byte_array(row) or row.size() > (916 if key == "candidate_lists" else 128): return false
				if key != "reading_rows" and (row.is_empty() or row.size()%2 != 0 or row.has(0)): return false
	return true

static func content_identity(content: Dictionary) -> String:
	return EXPANDED_CONTENT_SHA if content.size() == 9 else CONTENT_SHA

static func load_content(path: String) -> Dictionary:
	if not Rules._external_manifest_path_valid(path): return {}
	var file: FileAccess = FileAccess.open(path,FileAccess.READ)
	if file == null or file.get_length() <= 0 or file.get_length() > 65536: return {}
	var length: int = file.get_length()
	var bytes: PackedByteArray = file.get_buffer(length)
	if bytes.size() != length or file.get_length() != length or not Rules._external_manifest_path_valid(path): return {}
	var hash := HashingContext.new(); hash.start(HashingContext.HASH_SHA256); hash.update(bytes)
	var actual_sha := hash.finish().hex_encode()
	if actual_sha not in [CONTENT_SHA,EXPANDED_CONTENT_SHA]: return {}
	var value: Variant = JSON.parse_string(bytes.get_string_from_utf8())
	if not value is Dictionary: return {}
	for key in value:
		if not value[key] is Array: return {}
		var rows: Array = [value[key]] if key == "space" else value[key]
		for row in rows:
			if not row is Array: return {}
			for i in row.size():
				if typeof(row[i]) not in [TYPE_INT,TYPE_FLOAT] or row[i] != int(row[i]): return {}
				row[i] = int(row[i])
	return value if content_valid(value) and content_identity(value) == actual_sha else {}

func begin(tables: Dictionary, content: Dictionary, rng: Rng) -> String:
	if not Rules._content_valid(tables) or not content_valid(content) or content.space != tables.space or not rng.configuration_error().is_empty():
		return "New Game entry needs authenticated rule/entry packs and configured native RNG"
	_tables = tables.duplicate(true); _content = content.duplicate(true); _rng = rng
	stage = 1; step_id = 0; cursor = 0; tab = 0; error = ""; blocked = ""; trace.clear()
	reading = 0; page_scroll = 0
	names.clear()
	for part in ["family","given","nickname"]:
		var data := PackedByteArray()
		for i in (5 if part == "nickname" else 3): data.append_array(PackedByteArray(content.space))
		data.resize(12 if part == "nickname" else 8)
		names[part] = data
	player = {"month":255,"day":255,"blood":255}; shiori = player.duplicate()
	# Zero is storage for unknown bytes, not proof of native boot/service state.
	# Only boot_game_data_init-owned bytes are valid; no session boundary is ready.
	var fresh := PackedByteArray(); fresh.resize(Rules.SIZE)
	state_bytes = Rules.boot_data(fresh,tables).bytes
	known_bytes.resize(Rules.SIZE); known_bytes.fill(0)
	for span in [[0x3d,0x53c],[0x54c,0x71d],[0x724,0x73c],[0x75c,0xf5f],[0xf64,0xf68]]:
		for offset in range(span[0],span[1]): known_bytes[offset] = 255
	return ""

func owned_projection() -> Dictionary:
	return {"bytes":state_bytes.duplicate(),"known_bytes":known_bytes.duplicate(),"command_ready":false,
		"provenance":"boot_game_data_init/profile stores only; other zero bytes are unknown placeholders, not native boot evidence"}

func _record(action: String, before: int) -> void:
	trace.append({"stage":stage,"step":step_id,"action":action,"rng_before":before,"rng_after":_rng.state,"draw_count":_rng.draw_count})

func active_part() -> String:
	return "family" if step_id <= 1 else ("given" if step_id <= 3 else "nickname")

func profile() -> Dictionary:
	return player if stage == 1 else shiori

func cell(column: int, row: int) -> PackedByteArray:
	if column < 0 or column > 16 or row < 0 or row > 5: return PackedByteArray()
	if tab in [2,3]:
		if not _content.has("candidate_lists") or column >= 16: return PackedByteArray()
		var index := 38 if tab == 2 else reading
		var source: Array
		var offset: int
		if tab == 3 and reading != 38 and row == 0:
			source = _content.reading_labels[index]; offset = column*2
		else:
			source = _content.candidate_lists[index]
			offset = ((row-(1 if tab == 3 and reading != 38 else 0)+page_scroll)*16+column)*2
		return PackedByteArray(source.slice(offset,offset+2)) if offset+1 < source.size() else PackedByteArray(_content.space)
	if tab not in [0,1]: return PackedByteArray()
	if column%6 == 5 or (row == 2 and column in [9,10]) or (row == 3 and column in [3,4]) or (row == 5 and column >= 11): return PackedByteArray(_content.space)
	var source: Array = _content["hiragana" if tab == 0 else "katakana"][column/6+row*3]
	var index: int = (column%6)*2
	return PackedByteArray(source.slice(index,index+2)) if index+1 < source.size() else PackedByteArray()

func reading_cell(column: int, row: int) -> PackedByteArray:
	if not _content.has("reading_rows") or column < 0 or column > 16 or row < 0 or row > 2: return PackedByteArray()
	if column%6 == 5: return PackedByteArray(_content.space)
	var source: Array = _content.reading_rows[row*3+column/6]
	var offset := column%6*2
	return PackedByteArray(source.slice(offset,offset+2)) if offset+1 < source.size() else PackedByteArray(_content.space)

func _clamp_page() -> void:
	var index := 38 if tab == 2 else reading
	var count: int = _content.candidate_lists[index].size()/2
	var preceding_rows := 5 if tab == 2 or reading == 38 else 4
	if count < page_scroll*16+preceding_rows*16:
		page_scroll = maxi(0,(count>>4)-preceding_rows)

func normalize_name_cursor() -> void:
	if stage == 1 and step_id in [1,3,5]:
		var limit := 3 if step_id == 1 else (6 if step_id == 3 else 11)
		if cursor >= limit: cursor = 12

func _finish_name() -> void:
	# name_entry_name_finish draws unconditionally, even when no preset is copied.
	var part: String = active_part()
	var width: int = 10 if part == "nickname" else 6
	var preset: int = _rng.next("name_entry_name_finish:"+part)%15
	var blanks: int = 0
	for i in width:
		if names[part][i] == _content.space[i&1]: blanks += 1
	if names[part][blanks] == 0 or blanks == width or names[part][0] == 0:
		names[part] = PackedByteArray(_content["preset_"+part][preset])
	step_id += 1

func act(action: Dictionary) -> String:
	# One queued logical action per tick. Presentation callbacks only enqueue.
	if _rng == null: return "entry is not configured"
	if not error.is_empty(): return error
	normalize_name_cursor()
	var before: int = _rng.state
	var kind: String = str(action.get("kind",""))
	if stage == 1 and step_id in [0,2,4,6,8]:
		if kind != "tick": return "entry initialization requires a logical tick"
		if step_id == 0: cursor = 0; tab = 0
		elif step_id == 2:
			cursor = 3
			if tab == TAB_DONE: tab = 0
		elif step_id == 4:
			cursor = 6
			if tab == TAB_DONE: tab = 0
		elif step_id == 6: cursor = 0
		step_id += 1
	elif stage == 2 and step_id in [0,2]:
		if kind != "tick": return "entry initialization requires a logical tick"
		if step_id == 0: cursor = 0
		step_id += 1
	elif stage == 1 and step_id in [1,3,5]:
		var start: int = 0 if step_id == 1 else (3 if step_id == 3 else 6)
		var limit: int = 3 if step_id == 1 else (6 if step_id == 3 else 11)
		if kind == "tab":
			if typeof(action.get("value")) != TYPE_INT or action.value not in [0,1,2,3]: return "name tab outside native index range"
			if action.value in [2,3] and not _content.has("candidate_lists"): return "Symbols and Kanji require a rebuilt original name-entry content pack"
			tab = action.value
			if tab in [2,3]: _clamp_page()
		elif kind == "reading":
			if tab != 3 or typeof(action.get("column")) != TYPE_INT or typeof(action.get("row")) != TYPE_INT or action.column < 0 or action.column > 16 or action.row < 0 or action.row > 2: return "reading cell outside native grid"
			var selected: int = action.row*15+action.column%6+action.column/6*5
			if selected != reading: page_scroll = 0
			reading = selected
			_clamp_page()
		elif kind == "scroll":
			if tab not in [2,3] or typeof(action.get("delta")) != TYPE_INT or action.delta not in [-1,1]: return "scroll outside native candidate page"
			page_scroll = maxi(0,page_scroll+action.delta)
			_clamp_page()
		elif kind == "slot":
			if typeof(action.get("value")) != TYPE_INT or action.value < start or action.value >= limit: return "name slot outside active field"
			cursor = action.value
		elif kind == "cell":
			if typeof(action.get("column")) != TYPE_INT or typeof(action.get("row")) != TYPE_INT: return "native cell requires integer coordinates"
			var glyph: PackedByteArray = cell(action.column,action.row)
			if glyph.size() != 2: return "native cell has unresolved pair bytes"
			if cursor < limit:
				names[active_part()][(cursor-start)*2] = glyph[0]; names[active_part()][(cursor-start)*2+1] = glyph[1]
				cursor += 1
		elif kind == "cross": cursor = maxi(start,(limit-1 if cursor == 12 else cursor-1))
		elif kind in ["start","done"]:
			# CIRCLE on the native DONE tab changes the tab; START does not.
			if kind == "done": tab = TAB_DONE
			_finish_name()
		elif kind != "tick": return "unsupported name input action"
	elif (stage == 1 and step_id == 7) or (stage == 2 and step_id == 1):
		if kind == "select":
			var count: int = [12,31,4][cursor]
			if not Rules._selection_valid(action.get("value"),count): return "birthday selection outside native index range"
			profile()[["month","day","blood"][cursor]] = action.value
		elif kind == "cross": cursor = maxi(0,cursor-1)
		elif kind in ["circle","start"]:
			if kind == "circle": cursor += 1
			if cursor >= 3 or kind == "start":
				var finalized: Rules.Outcome = Rules.finalize_birthday(profile(),_rng,"name_entry_birthday_finalize:"+("player" if stage == 1 else "shiori"))
				if not finalized.error.is_empty(): return finalized.error
				for i in 3: profile()[["month","day","blood"][i]] = finalized.selections[i]
				step_id += 1
		elif kind != "tick": return "unsupported birthday input action"
	elif (stage == 1 and step_id == 9) or (stage == 2 and step_id == 3):
		if kind in ["start","yes"]:
			if stage == 1: stage = 2; step_id = 0
			else: step_id = 4
		elif kind in ["cross","no"]: step_id = 0
		elif kind != "tick": return "unsupported profile confirmation action"
	elif stage == 2 and step_id == 4:
		if kind != "tick": return "native commit requires a logical tick"
		var editable: Dictionary = {"family":names.family.slice(0,6),"given":names.given.slice(0,6),"nickname":names.nickname.slice(0,10)}
		var outcome: Rules.Outcome = Rules.setup(state_bytes,editable,player,shiori,_tables,_rng)
		if not outcome.error.is_empty(): return outcome.error
		state_bytes = outcome.bytes; step_id = 5
		blocked = "NAME_ENT exit/prologue requires native fade timer, scene assets, dialogue/audio services and mode0x15 controller; no command boundary produced"
	elif stage == 2 and step_id == 5:
		return blocked
	else: return "unsupported NAME_ENT stage/step"
	_record(kind,before)
	return ""
