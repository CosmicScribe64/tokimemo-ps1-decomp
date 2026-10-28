extends RefCounted
## T-29025: numeric-only text-system state for prologue_line_start/run.
## Text bytes stay in PrologueTextStore's resolved view, never in GameState.

const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")

const SOURCE_SPEED := 5
const SCROLL_ENABLED := 0x80
const MAX_TEXT_ENTRIES := 0x34
const MAX_ROWS := 6


static func reset(state: State) -> String:
	if state == null:
		return "prologue text reset needs route state"
	state.opening["text_speed"] = SOURCE_SPEED
	state.opening.erase("prologue_text_work")
	state.opening.erase("prologue_text")
	return ""


static func begin_line(state: State, resolved_view: Dictionary) -> String:
	if state == null:
		return "prologue text line needs route state"
	if typeof(resolved_view.get("line_id")) != TYPE_INT or resolved_view.line_id < 0:
		return "prologue text line identity is missing"
	if typeof(resolved_view.get("speaker")) != TYPE_INT or not resolved_view.get("speaker_raw") is PackedByteArray:
		return "prologue text speaker view is malformed"
	if typeof(state.opening.get("text_speed")) != TYPE_INT or state.opening.text_speed != SOURCE_SPEED:
		return "prologue scene text speed was not initialized from source"
	var speaker_raw: PackedByteArray = resolved_view.speaker_raw
	if speaker_raw.is_empty() or speaker_raw.size() % 2 != 0 or speaker_raw.has(0):
		return "prologue speaker view has an unsupported byte extent"
	var rows: Variant = resolved_view.get("rows")
	if not rows is Array or rows.is_empty() or rows.size() > MAX_ROWS:
		return "prologue resolved text row count is unsupported"
	if rows.size() + 1 > MAX_TEXT_ENTRIES:
		return "prologue text exceeds the native text-entry capacity"

	var speaker_glyphs: int = speaker_raw.size() / 2
	var speaker_x := -0x80
	if speaker_raw.size() == 2:
		speaker_x = -0x72
	elif speaker_raw.size() == 4:
		speaker_x = -0x79
	var entries: Array[Dictionary] = [{
		"glyph_count": speaker_glyphs,
		"x": speaker_x,
		"y": 0x32,
		"active": speaker_glyphs > 0,
		"attribute": 0,
		"row": -1,
	}]
	var total := speaker_glyphs
	for row_index in rows.size():
		var row: Variant = rows[row_index]
		if not row is Dictionary or typeof(row.get("screen_x")) != TYPE_INT or typeof(row.get("screen_y")) != TYPE_INT:
			return "prologue resolved row geometry is malformed"
		if row.screen_x not in [-0x80, -0x56, -0x48] or row.screen_y not in [0x32, 0x42, 0x52]:
			return "prologue resolved row geometry differs from native sndisp"
		var row_raw: Variant = row.get("raw")
		var tokens: Variant = row.get("tokens")
		if not row_raw is PackedByteArray or not tokens is Array or row_raw.size() % 2 != 0:
			return "prologue resolved row glyphs are malformed"
		if tokens.size() * 2 != row_raw.size():
			return "prologue row glyph tokens do not cover the resolved bytes"
		for glyph_index in tokens.size():
			var token: Variant = tokens[glyph_index]
			if not token is Dictionary or token.get("kind") != "glyph" or token.get("row") != row_index:
				return "prologue row contains a non-glyph or misindexed token"
			var raw_hex: Variant = token.get("raw_hex")
			if not raw_hex is String or raw_hex.length() != 4 or not _hex_pair_matches(raw_hex, row_raw, glyph_index * 2):
				return "prologue row token bytes differ from the resolved row"
		var count: int = tokens.size()
		total += count
		if total > 0x7fff:
			return "prologue text total exceeds native signed glyph counter"
		entries.append({
			"glyph_count": count,
			"x": row.screen_x,
			"y": row.screen_y,
			"active": count > 0,
			"attribute": 0,
			"row": row_index,
		})
	if total > 0x7fff:
		return "prologue text total exceeds native signed glyph counter"

	# gnsx queues the speaker as entry 0. k_sub_disp_start(1) starts at its end.
	state.opening["prologue_text_work"] = {
		"line_id": resolved_view.line_id,
		"entries": entries,
		"pos": speaker_glyphs,
		"total": total,
		"first_line": 1,
		"lines_total": entries.size(),
		"speed": SOURCE_SPEED,
		"scroll_state": SCROLL_ENABLED,
		"next_frame": Math.u32(state.frame_count + 1),
	}
	state.opening.erase("prologue_text")
	return ""


static func advance(state: State, result: Session.FrameResult) -> void:
	if result == null:
		return
	if not result.error.is_empty():
		return
	if state == null or not state.opening.get("prologue_text_work") is Dictionary:
		result.error = "prologue text work is not initialized"
		return
	var work: Dictionary = state.opening.prologue_text_work.duplicate(true)
	var frame: int = state.frame_count
	var stage4_first_advance: bool = work.get("line_id") == 4 and state.mode_stage == 4 and state.mode_step == 0 and state.substep == 2 and not state.opening.has("prologue_text") and typeof(work.get("next_frame")) == TYPE_INT and frame >= int(work.next_frame)
	if frame < 0 or frame > 0xffffffff or (work.get("next_frame") != frame and not stage4_first_advance):
		result.error = "prologue text work has a missing, duplicate or unexpected frame"
		return
	if not _work_is_valid(work):
		result.error = "prologue text work counters or entries are malformed"
		return
	var previous_text: Variant = state.opening.get("prologue_text")
	if previous_text is Dictionary and previous_text.get("line_id") == work.line_id and previous_text.get("iteration") == Math.u32(frame - 1):
		var published_scroll: Variant = previous_text.get("scroll_state")
		if typeof(published_scroll) != TYPE_INT or published_scroll not in [0, SCROLL_ENABLED, SCROLL_ENABLED + 1]:
			result.error = "prologue text scroll feedback is outside the source states"
			return
		work.scroll_state = published_scroll
	if not _work_is_valid(work):
		result.error = "prologue text work counters or entries are malformed"
		return

	var current_line := _get_now_k(work)
	var complete := 0
	if int(work.scroll_state) & SCROLL_ENABLED != 0:
		var scroll_phase: int = int(work.scroll_state) & 0x7f
		if int(work.first_line) + 2 >= current_line:
			complete = _k_disp_inc(work, frame)
			current_line = _get_now_k(work)
			if current_line == int(work.first_line) + 3 and int(work.lines_total) >= int(work.first_line) + 3:
				var hidden: Dictionary = work.entries[current_line]
				hidden.active = false
				work.entries[current_line] = hidden
		elif scroll_phase == 0:
			var first: Dictionary = work.entries[int(work.first_line)]
			first.active = false
			work.entries[int(work.first_line)] = first
			_shift_row(work, int(work.first_line) + 1)
			_shift_row(work, int(work.first_line) + 2)
			work.scroll_state += 1
		else:
			_shift_row(work, int(work.first_line) + 1)
			_shift_row(work, int(work.first_line) + 2)
			var middle: Dictionary = work.entries[int(work.first_line) + 1]
			if int(middle.y) < 0x33:
				middle.y = 0x32
				work.entries[int(work.first_line) + 1] = middle
				var bottom: Dictionary = work.entries[int(work.first_line) + 2]
				bottom.y = 0x42
				work.entries[int(work.first_line) + 2] = bottom
				work.scroll_state -= 1
				work.first_line += 1
				var entering: Dictionary = work.entries[int(work.first_line) + 2]
				entering.active = true
				work.entries[int(work.first_line) + 2] = entering
	else:
		complete = _k_disp_inc(work, frame)
		_get_now_k(work)

	work.next_frame = Math.u32(frame + 1)
	state.opening["prologue_text_work"] = work
	state.opening["prologue_text"] = {
		"line_id": work.line_id,
		"k_disp_inc_result": complete,
		"iteration": frame,
		"scroll_state": work.scroll_state,
	}


static func _work_is_valid(work: Dictionary) -> bool:
	if not work.get("entries") is Array or work.entries.size() < 2 or work.entries.size() > MAX_ROWS + 1:
		return false
	for key: String in ["pos", "total", "first_line", "lines_total", "speed", "scroll_state", "next_frame", "line_id"]:
		if typeof(work.get(key)) != TYPE_INT:
			return false
	if work.line_id < 0 or work.speed != SOURCE_SPEED or work.pos < 0 or work.total < work.pos or work.total > 0x7fff:
		return false
	if work.scroll_state not in [0, SCROLL_ENABLED, SCROLL_ENABLED + 1] or work.next_frame < 0 or work.next_frame > 0xffffffff:
		return false
	if work.lines_total != work.entries.size() or work.first_line < 1 or work.first_line >= work.lines_total:
		return false
	if work.first_line > maxi(1,work.lines_total - 3) or (work.scroll_state == SCROLL_ENABLED + 1 and work.first_line + 3 >= work.lines_total):
		return false
	var total := 0
	for index in work.entries.size():
		var entry: Variant = work.entries[index]
		if not entry is Dictionary or typeof(entry.get("glyph_count")) != TYPE_INT or typeof(entry.get("x")) != TYPE_INT or typeof(entry.get("y")) != TYPE_INT or typeof(entry.get("active")) != TYPE_BOOL or typeof(entry.get("attribute")) != TYPE_INT or typeof(entry.get("row")) != TYPE_INT:
			return false
		if entry.glyph_count < 0 or entry.x not in [-0x80, -0x79, -0x72, -0x56, -0x48] or entry.y < 0x32 or entry.y > 0x52 or entry.y % 2 != 0 or entry.attribute != 0:
			return false
		if entry.row != (-1 if index == 0 else index - 1):
			return false
		if index == 0 and (entry.glyph_count <= 0 or work.pos < entry.glyph_count):
			return false
		total += entry.glyph_count
	return total == work.total


static func _get_now_k(work: Dictionary) -> int:
	var glyph_end := 0
	for index in range(int(work.pos)):
		glyph_end += int(work.entries[index].glyph_count)
		if glyph_end >= int(work.pos):
			return index
	return 0xff


static func _k_disp_inc(work: Dictionary, frame: int) -> int:
	if frame % (int(work.speed) + 1) == 0 and int(work.pos) < int(work.total):
		work.pos += 1
		return 0
	return 1 if int(work.pos) == int(work.total) else 0


static func _shift_row(work: Dictionary, index: int) -> void:
	var entry: Dictionary = work.entries[index]
	entry.y -= 2
	work.entries[index] = entry


static func _hex_pair_matches(value: String, raw: PackedByteArray, offset: int) -> bool:
	if offset < 0 or offset + 2 > raw.size():
		return false
	var bytes := raw.slice(offset, offset + 2).hex_encode()
	return value.to_lower() == bytes
