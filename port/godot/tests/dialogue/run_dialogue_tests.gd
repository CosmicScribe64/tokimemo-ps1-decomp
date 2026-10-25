extends SceneTree

const TalkPlayer = preload("res://dialogue/talk_player.gd")
const SOURCE_PATH := "CDROM/SERIFU/SERIFU.BIN"
const SOURCE_SHA := "5d6e1014c69769f18f7877ad801f04f872a9829c05d4c8509602adbccd5f22ee"
const PROFILE_ID := "serifu-serifu"
const SECTION_ID := "table-0"
const TOKENS := [
	{"kind": "glyph", "raw_hex": "8140"},
	{"kind": "separator", "role": "choice_option", "code": "q", "raw_hex": "71"},
	{"kind": "glyph", "raw_hex": "8141"},
	{"kind": "separator", "role": "choice_option", "code": "q", "raw_hex": "71"},
	{"kind": "glyph", "raw_hex": "8142"},
]

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var player := TalkPlayer.new()
	var content: Dictionary = _content(3)
	content["content_sha256"] = player._component_digest(content)
	var route: Dictionary = _route(content)
	assert(player.load_content(content, route).is_empty())
	var menu: Dictionary = player.present_choice(0)
	assert(menu["order"] == [0, 1, 2])
	assert(menu["options"].size() == 3)
	assert(player.choose(menu["options"][1]["option_id"], false, true)["error"].contains("text completion"))
	assert(player.choose(menu["options"][1]["option_id"], true, false)["waiting"])
	assert(player.advance_line(true).contains("consumed"))
	var picked: Dictionary = player.choose(menu["options"][1]["option_id"], true, true)
	assert(picked["source_choice"] == 1)
	assert(player.advance_line(false).is_empty())
	assert(player.current_record()["kind"] == "line")
	assert(player.advance_line(true).is_empty())
	assert(player.current_record()["kind"] == "group_end")
	assert(player.advance_line(false).is_empty())
	assert(player.current_record()["kind"] == "group_end")
	assert(player.advance_line(true).is_empty())
	assert(player.current_record()["kind"] == "complete")

	var shuffled := TalkPlayer.three_choice_order(1)
	assert(shuffled == [1, 2, 0])
	for frame: int in range(12):
		var order: Array[int] = TalkPlayer.three_choice_order(frame)
		order.sort()
		assert(order == [0, 1, 2])

	var line_player := TalkPlayer.new()
	var line_content: Dictionary = _content(6)
	line_content["content_sha256"] = line_player._component_digest(line_content)
	assert(line_player.load_content(line_content, _route(line_content)).is_empty())
	assert(line_player.present_line()["speaker_id"] == "player_family")
	assert(line_player.confirm_line(false, true)["waiting"] == "typewriter")
	assert(line_player.confirm_line(true, false)["waiting"] == "confirm")
	assert(line_player.advance_line(false).is_empty())
	assert(line_player.confirm_line(true, true)["ready_to_advance"])
	assert(line_player.advance_line(false).is_empty())
	assert(line_player.advance_line(true).is_empty())
	assert(line_player.current_record()["kind"] == "group_end")

	_test_identity_mutations(player, content, route)
	_test_structural_mutations(player, content, route)
	_test_voice_rejection(player, content, route)
	print("dialogue synthetic checks passed: identity, cursor, voice, choices, and input gates")
	quit()

func _test_identity_mutations(player: RefCounted, original: Dictionary, route: Dictionary) -> void:
	var bad_source: Dictionary = original.duplicate(true)
	bad_source["source"]["path"] = "CDROM/SERIFU/OTHER.BIN"
	bad_source["content_sha256"] = player._component_digest(bad_source)
	var source_error: String = player.load_content(bad_source, route)
	assert(source_error.contains("trusted expected route"))

	var bad_section: Dictionary = original.duplicate(true)
	bad_section["section_id"] = "other-section"
	bad_section["content_sha256"] = player._component_digest(bad_section)
	var section_error: String = player.load_content(bad_section, route)
	assert(section_error.contains("trusted expected route"))

	var bad_body: Dictionary = original.duplicate(true)
	bad_body["groups"][0]["records"][0]["text"]["raw_hex"] = "8140"
	var digest_error: String = player.load_content(bad_body, route)
	assert(digest_error.contains("content SHA-256 mismatch"))

func _test_structural_mutations(player: RefCounted, original: Dictionary, route: Dictionary) -> void:
	var malformed_term: Dictionary = original.duplicate(true)
	malformed_term["groups"][0]["records"][1] = {"command": 255}
	_reseal(player, malformed_term)
	assert(player.load_content(malformed_term, _route(malformed_term)).contains("line indices"))

	var duplicate_id: Dictionary = original.duplicate(true)
	duplicate_id["groups"][0]["records"][1]["id"] = duplicate_id["groups"][0]["records"][0]["id"]
	_reseal(player, duplicate_id)
	assert(player.load_content(duplicate_id, _route(duplicate_id)).contains("row ID"))

	var line_gap: Dictionary = original.duplicate(true)
	line_gap["groups"][0]["records"][0]["line"] = 17
	_reseal(player, line_gap)
	assert(player.load_content(line_gap, _route(line_gap)).contains("line indices"))

func _test_voice_rejection(player: RefCounted, original: Dictionary, route: Dictionary) -> void:
	var before: Dictionary = player.current_record()
	var voice_content: Dictionary = original.duplicate(true)
	voice_content["groups"][0]["records"][0]["voice"] = {
		"speaker": "runtime_current_girl",
		"pairs": [{"variant": 0, "clip_start": 1, "clip_end": 2}],
	}
	_reseal(player, voice_content)
	var voice_error: String = player.load_content(voice_content, _route(voice_content))
	assert(voice_error.contains("voice-bearing dialogue is unsupported"))
	assert(player.current_record() == before)

func _reseal(player: RefCounted, content: Dictionary) -> void:
	content["content_sha256"] = player._component_digest(content)

func _route(content: Dictionary) -> Dictionary:
	return {"source_path": content["source"]["path"],
		"source_sha256": content["source"]["sha256"],
		"profile_id": content["profile_id"], "section_id": content["section_id"],
		"content_sha256": content["content_sha256"]}

func _content(command: int) -> Dictionary:
	var row: Dictionary = {"id": "talk:%s:%s:g0000:l0000" % [SOURCE_PATH, SECTION_ID],
		"line": 0, "command": command, "command_name": "choice_three" if command == 3 else "girl_reply_b",
		"source_locator": {"path": SOURCE_PATH, "source_sha256": SOURCE_SHA,
			"text_offset": 200, "command_offset": 100},
		"text": {"raw_hex": "8140718141718142", "tokens": TOKENS,
			"unknown_token_count": 0, "inline_label": null}}
	var terminator: Dictionary = {"id": "talk:%s:%s:g0000:l0001" % [SOURCE_PATH, SECTION_ID],
		"line": 1, "command": 255, "command_name": "end",
		"source_locator": {"path": SOURCE_PATH, "source_sha256": SOURCE_SHA,
			"text_offset": null, "command_offset": 101}}
	return {"schema": "tokimemo.dialogue-content.v1", "profile": "serifu-native-v1",
		"source": {"path": SOURCE_PATH, "sha256": SOURCE_SHA, "size_bytes": 4096,
			"reader_schema": "tokimemo.text.v1"},
		"profile_id": PROFILE_ID, "section_id": SECTION_ID,
		"groups": [{"index": 0, "records": [row, terminator]}],
		"voice_timing": "preserved_extents_runtime_mapping_unproved",
		"text_encoding": "original_bytes_with_reader_token_view",
		"semantic_coverage_complete": false, "content_sha256": ""}
