extends SceneTree

const TalkPlayer = preload("res://dialogue/talk_player.gd")
const TRUSTED_ROUTE := {
	"source_path": "CDROM/SERIFU/SERIFU.BIN",
	"source_sha256": "5d6e1014c69769f18f7877ad801f04f872a9829c05d4c8509602adbccd5f22ee",
	"profile_id": "serifu-serifu",
	"section_id": "table-0",
	"content_sha256": "122171babc3eccad9950aed6604ba114597c8658bd451d0e09c9836c14776b2a",
}

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var actual: Variant = JSON.parse_string(FileAccess.get_file_as_string("/original/dialogue.json"))
	assert(actual is Dictionary)
	var player := TalkPlayer.new()
	assert(player.load_content(actual, TRUSTED_ROUTE).is_empty())
	var rows: int = 0
	while player.current_record().get("kind") == "line":
		var record: Dictionary = player.current_record()["record"]
		assert(record["command"] == 6)
		assert(player.present_line()["speaker_id"] == "player_family")
		assert(player.confirm_line(false, true)["waiting"] == "typewriter")
		assert(player.confirm_line(true, false)["waiting"] == "confirm")
		assert(player.confirm_line(true, true)["ready_to_advance"])
		assert(player.advance_line(true).is_empty())
		rows += 1
	assert(rows == 12)
	assert(player.current_record()["kind"] == "group_end")
	assert(player.advance_line(false).is_empty())
	assert(player.current_record()["kind"] == "group_end")

	var changed_source: Dictionary = actual.duplicate(true)
	changed_source["source"]["path"] = "CDROM/SERIFU/OTHER.BIN"
	_reseal(player, changed_source)
	assert(player.load_content(changed_source, TRUSTED_ROUTE).contains("trusted expected route"))

	var changed_sha: Dictionary = actual.duplicate(true)
	changed_sha["source"]["sha256"] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	_reseal(player, changed_sha)
	assert(player.load_content(changed_sha, TRUSTED_ROUTE).contains("trusted expected route"))

	var changed_section: Dictionary = actual.duplicate(true)
	changed_section["section_id"] = "other-section"
	_reseal(player, changed_section)
	assert(player.load_content(changed_section, TRUSTED_ROUTE).contains("trusted expected route"))

	var changed_digest: Dictionary = actual.duplicate(true)
	changed_digest["content_sha256"] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	assert(player.load_content(changed_digest, TRUSTED_ROUTE).contains("trusted expected route"))

	var changed_payload: Dictionary = actual.duplicate(true)
	changed_payload["groups"][0]["records"][0]["text"]["raw_hex"] = "8140"
	assert(player.load_content(changed_payload, TRUSTED_ROUTE).contains("content SHA-256 mismatch"))

	var changed_locator: Dictionary = actual.duplicate(true)
	changed_locator["groups"][0]["records"][0]["source_locator"]["path"] = "CDROM/SERIFU/OTHER.BIN"
	_reseal(player, changed_locator)
	assert(player.load_content(changed_locator, _route_with_digest(TRUSTED_ROUTE,
		str(changed_locator["content_sha256"]))).contains("source locator"))

	var duplicate_id: Dictionary = actual.duplicate(true)
	duplicate_id["groups"][0]["records"][1]["id"] = duplicate_id["groups"][0]["records"][0]["id"]
	_reseal(player, duplicate_id)
	assert(player.load_content(duplicate_id, _route_with_digest(TRUSTED_ROUTE,
		str(duplicate_id["content_sha256"]))).contains("row ID"))

	var line_gap: Dictionary = actual.duplicate(true)
	line_gap["groups"][0]["records"][0]["line"] = 17
	_reseal(player, line_gap)
	assert(player.load_content(line_gap, _route_with_digest(TRUSTED_ROUTE,
		str(line_gap["content_sha256"]))).contains("line indices"))

	var voice_row: Dictionary = actual.duplicate(true)
	voice_row["groups"][0]["records"][0]["voice"] = {
		"speaker": "runtime_current_girl",
		"pairs": [{"variant": 0, "clip_start": 1, "clip_end": 2}],
	}
	_reseal(player, voice_row)
	var before: Dictionary = player.current_record()
	assert(player.load_content(voice_row, _route_with_digest(TRUSTED_ROUTE,
		str(voice_row["content_sha256"]))).contains("voice-bearing dialogue is unsupported"))
	assert(player.current_record() == before)
	print("original SERIFU check passed: 12 rows; source, digest, cursor, and voice mutation negatives passed")
	quit()

func _reseal(player: RefCounted, content: Dictionary) -> void:
	content["content_sha256"] = player._component_digest(content)

func _route_with_digest(route: Dictionary, digest: String) -> Dictionary:
	var result: Dictionary = route.duplicate(true)
	result["content_sha256"] = digest
	return result
