extends SceneTree
## Invented glyph bytes; tests source-timed counters and SCROLLHALF only.

const Work = preload("res://rules/prologue_text_work.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Store = preload("res://services/prologue_text_store.gd")

var checks := 0
var failures := 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr(label)

func row(index: int, raw_hex: String, x: int, y: int) -> Dictionary:
	var raw := raw_hex.hex_decode()
	var tokens: Array[Dictionary] = []
	for offset in range(0, raw.size(), 2):
		tokens.append({"kind":"glyph", "row":index, "raw_hex":raw.slice(offset, offset + 2).hex_encode()})
	return {"raw":raw, "tokens":tokens, "screen_x":x, "screen_y":y}

func view(line_id: int, row_hexes: Array[String]) -> Dictionary:
	var rows: Array[Dictionary] = []
	var x_values := [-0x56, -0x48, -0x48, -0x48, -0x48, -0x48]
	var y_values := [0x32, 0x42, 0x52, 0x52, 0x52, 0x52]
	for index in row_hexes.size():
		rows.append(row(index, row_hexes[index], x_values[index], y_values[index]))
	return {"error":"", "line_id":line_id, "speaker":0x0f,
		"speaker_raw":PackedByteArray([0x81, 0x40]), "rows":rows}

func advance(state: State, frame: int) -> Session.FrameResult:
	state.frame_count = frame
	var result := Session.FrameResult.new()
	Work.advance(state, result)
	return result

func _initialize() -> void:
	var state := State.new()
	check(Work.reset(state).is_empty() and state.opening.text_speed == 5, "scene reset sets source typewriter speed")
	var resolved := view(7, ["8140", "81408141", "8140"])
	check(Work.begin_line(state, resolved).is_empty(), "resolved store view starts a line")
	var work: Dictionary = state.opening.prologue_text_work
	check(work.entries.size() == 4 and work.entries[0].row == -1, "speaker occupies native entry zero")
	check(work.entries[0].glyph_count == 1 and work.entries[0].x == -0x72 and work.entries[0].y == 0x32 and work.pos == 1, "speaker is active and skipped by k_disp_start(1)")
	check(work.entries[1].glyph_count == 1 and work.entries[2].glyph_count == 2 and work.entries[3].glyph_count == 1 and work.total == 5, "rows retain glyph counts without storing source bytes")
	check(work.first_line == 1 and work.lines_total == 4 and work.speed == 5 and work.scroll_state == 0x80, "source first-line and SCROLLHALF state initialized")
	check(not work.has("raw") and not work.has("speaker_raw"), "work state contains no text bytes")
	var first := advance(state, 1)
	check(first.error.is_empty() and state.opening.prologue_text.k_disp_inc_result == 0 and state.opening.prologue_text_work.pos == 1, "non-sixth frame does not reveal a glyph")
	var duplicate := advance(state, 1)
	check(not duplicate.error.is_empty() and state.opening.prologue_text_work.pos == 1, "duplicate frame is rejected without advancing work")
	var revealed := advance(state, 6)
	check(not revealed.error.is_empty() and state.opening.prologue_text_work.pos == 1, "missing frame is rejected")
	var next := advance(state, 2)
	check(next.error.is_empty() and state.opening.prologue_text_work.pos == 1, "expected frame can proceed after duplicate rejection")
	for frame in range(3, 24):
		var result := advance(state, frame)
		check(result.error.is_empty(), "sequential text frame accepted")
	check(state.opening.prologue_text_work.pos == 4, "glyph progress follows absolute frame_count modulo six")
	var last_glyph := advance(state, 24)
	check(last_glyph.error.is_empty() and state.opening.prologue_text_work.pos == 5 and state.opening.prologue_text.k_disp_inc_result == 0, "last glyph increment returns zero on reveal frame")
	var completed := advance(state, 25)
	check(completed.error.is_empty() and state.opening.prologue_text.k_disp_inc_result == 1, "complete result arrives on the following frame")

	var scrolling := State.new()
	Work.reset(scrolling)
	check(Work.begin_line(scrolling, view(8, ["8140", "8140", "8140", "8140"])).is_empty(), "four-row invented line starts")
	var scroll_work: Dictionary = scrolling.opening.prologue_text_work
	scroll_work.pos = 5
	scrolling.opening.prologue_text_work = scroll_work
	var scroll := advance(scrolling, 1)
	check(scroll.error.is_empty() and scrolling.opening.prologue_text.k_disp_inc_result == 0, "SCROLLHALF branch suppresses typewriter result")
	check(scrolling.opening.prologue_text_work.scroll_state == 0x81 and not scrolling.opening.prologue_text_work.entries[1].active and scrolling.opening.prologue_text_work.entries[2].y == 64, "first scroll half hides the outgoing row and moves next rows by two")
	for frame in range(2, 9):
		var result := advance(scrolling, frame)
		check(result.error.is_empty(), "SCROLLHALF frame accepted")
	check(scrolling.opening.prologue_text_work.first_line == 2 and scrolling.opening.prologue_text_work.scroll_state == 0x80, "scroll clamps and advances first line")
	check(scrolling.opening.prologue_text_work.entries[2].y == 50 and scrolling.opening.prologue_text_work.entries[3].y == 66 and scrolling.opening.prologue_text_work.entries[4].y == 82 and scrolling.opening.prologue_text_work.entries[4].active, "scroll clamps rows and reveals the entering row")
	var audio_reset := State.new()
	Work.reset(audio_reset)
	check(Work.begin_line(audio_reset, view(10, ["8140", "8140", "8140", "8140"])).is_empty(), "audio reset fixture line starts")
	var audio_work: Dictionary = audio_reset.opening.prologue_text_work
	audio_work.pos = 5
	audio_reset.opening.prologue_text_work = audio_work
	var audio_scroll := advance(audio_reset, 1)
	check(audio_scroll.error.is_empty(), "initial scroll state publishes")
	audio_reset.opening.prologue_text.scroll_state = 0
	var after_audio := advance(audio_reset, 2)
	check(after_audio.error.is_empty() and audio_reset.opening.prologue_text_work.scroll_state == 0 and audio_reset.opening.prologue_text.scroll_state == 0, "audio-owned completion scroll reset feeds the next text frame")

	var malformed := State.new()
	Work.reset(malformed)
	var bad_view := view(9, ["8140"])
	bad_view.rows[0].tokens[0].raw_hex = "8141"
	var before := malformed.projection()
	check(not Work.begin_line(malformed, bad_view).is_empty() and malformed.projection() == before, "mismatched source token fails before changing state")
	# Malformed numeric state must fail without logging an engine index error.
	for mutation: String in ["zero_speaker","position_before_speaker","scroll_without_rows","first_line","missing_identity","feedback_scroll_without_rows"]:
		var invalid := State.new()
		Work.reset(invalid)
		Work.begin_line(invalid,view(11,["8140"]))
		var invalid_work: Dictionary = invalid.opening.prologue_text_work
		match mutation:
			"zero_speaker":
				invalid_work.entries[0].glyph_count = 0
				invalid_work.entries[1].glyph_count = 0
				invalid_work.pos = 0
				invalid_work.total = 0
			"position_before_speaker": invalid_work.pos = 0
			"scroll_without_rows": invalid_work.scroll_state = 0x81
			"first_line": invalid_work.first_line = 2
			"missing_identity":
				invalid_work.erase("line_id")
				invalid.opening.prologue_text = {"line_id":11,"iteration":0,"scroll_state":0x80}
			"feedback_scroll_without_rows": invalid.opening.prologue_text = {"line_id":11,"iteration":0,"scroll_state":0x81}
		invalid.frame_count = 1
		var invalid_before := invalid.projection()
		var rejected := Session.FrameResult.new()
		Work.advance(invalid,rejected)
		check(not rejected.error.is_empty() and invalid.projection() == invalid_before,"malformed work rejects before mutation: "+mutation)
	var actual_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	check(not actual_root.is_empty(), "actual prologue text pack is supplied for the 43-view logical pass")
	if not actual_root.is_empty():
		var manifest_path := actual_root.path_join("manifest.json")
		var store := Store.new()
		var store_error: String = store.open(actual_root, FileAccess.get_sha256(manifest_path))
		check(store_error.is_empty(), "pinned actual prologue text pack opens: " + store_error)
		var actual_state := {"profile_family_bytes":[130,160,0,0,0,0,0,0],
			"profile_given_bytes":[130,162,0,0,0,0,0,0],
			"girls":[{"birthday":12|(31<<4)|(3<<9)}]}
		for line_id in 43:
			var actual_view: Dictionary = store.resolve(line_id, actual_state)
			check(actual_view.error.is_empty(), "actual resolved line available: " + str(line_id))
			if not actual_view.error.is_empty():
				continue
			var actual_work := State.new()
			Work.reset(actual_work)
			check(Work.begin_line(actual_work, actual_view).is_empty(), "actual line initializes logical text work: " + str(line_id))
			var expected_total: int = actual_view.speaker_raw.size() / 2
			for actual_row: Dictionary in actual_view.rows:
				expected_total += actual_row.tokens.size()
			var actual_line: Dictionary = actual_work.opening.prologue_text_work
			check(actual_line.total == expected_total and actual_line.pos == actual_view.speaker_raw.size() / 2, "actual line starts after speaker with exact glyph total: " + str(line_id))
			var frame := 1
			var complete := 0
			while complete == 0 and frame <= 12000:
				var actual_result := advance(actual_work, frame)
				if not actual_result.error.is_empty():
					break
				complete = int(actual_work.opening.prologue_text.k_disp_inc_result)
				frame += 1
			check(complete == 1, "actual line reaches source text completion without the 61-tick route hold: " + str(line_id))
			check(not JSON.stringify(actual_work.opening.prologue_text_work).contains("raw_hex"), "actual text state retains counts, not source wording: " + str(line_id))
	print("PASS: %d prologue text work checks" % checks if failures == 0 else "FAIL: %d/%d prologue text work checks" % [failures, checks])
	quit(0 if failures == 0 else 1)
