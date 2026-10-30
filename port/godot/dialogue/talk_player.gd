extends RefCounted
class_name NativeTalkPlayer
## Bounded SERIFU player with substitution resolution and evidenced voice service gating.
## Rejects voiced content by default unless voice support is enabled.

const Tokens = preload("res://dialogue/text_tokens.gd")
const PROFILE := "serifu-native-v1"
const COMPONENT_SCHEMA := "tokimemo.dialogue-content.v1"
const READER_SCHEMA := "tokimemo.text.v1"
const VOICE_COMMANDS: Array[int] = [1, 2, 5, 7, 10, 11, 12, 13, 15, 18]
const MAX_GROUPS := 4096
const MAX_LINES := 4096

var _groups: Array[Dictionary] = []
var _group_position: int = 0
var _line_position: int = 0
var _pending_choice: Dictionary = {}
var _line_confirmed: bool = false
var _allow_voiced: bool = false
var _voice_service: Variant = null
var _voice_cancelled: bool = false
var _substitution_context: Dictionary = {}

func set_voice_service(service: Variant) -> void:
	_voice_service = service
	if service != null:
		_allow_voiced = true

func set_substitution_context(context: Dictionary) -> void:
	_substitution_context = context.duplicate(true)

func cancel_voice() -> void:
	_voice_cancelled = true
	if _voice_service != null and _voice_service.has_method("stop_clip"):
		_voice_service.stop_clip()

func load_content(content: Dictionary, expected_route: Dictionary, options: Dictionary = {}) -> String:
	## expected_route must come from a validated external pack manifest/profile,
	## never from fields read out of content itself.
	_allow_voiced = bool(options.get("allow_voiced", false)) or (_voice_service != null)
	var prepared: Dictionary = _prepare_content(content, expected_route)
	if not str(prepared.get("error", "")).is_empty():
		return str(prepared["error"])
	_groups = prepared["groups"]
	_group_position = 0
	_line_position = 0
	_pending_choice.clear()
	_line_confirmed = false
	_voice_cancelled = false
	return ""

func _prepare_content(content: Dictionary, expected_route: Dictionary) -> Dictionary:
	if not _has_exact_keys(expected_route,
			["source_path", "source_sha256", "profile_id", "section_id", "content_sha256"]):
		return _bad("trusted expected route identity is required")
	for key: String in expected_route:
		if not expected_route[key] is String or str(expected_route[key]).is_empty():
			return _bad("trusted expected route identity fields must be nonempty strings")
	if content.get("schema") != COMPONENT_SCHEMA or content.get("profile") != PROFILE:
		return _bad("unsupported dialogue component schema or behavior profile")
	if content.get("voice_timing") != "preserved_extents_runtime_mapping_unproved":
		return _bad("dialogue voice timing profile is unknown")
	if content.get("source", null) == null or not content["source"] is Dictionary:
		return _bad("dialogue source provenance is missing")
	var source: Dictionary = content["source"]
	if not _has_exact_keys(source, ["path", "sha256", "size_bytes", "reader_schema"]):
		return _bad("dialogue source provenance fields do not match the component contract")
	var identity_pairs := [
		["source_path", source.get("path")],
		["source_sha256", source.get("sha256")],
		["profile_id", content.get("profile_id")],
		["section_id", content.get("section_id")],
		["content_sha256", content.get("content_sha256")],
	]
	for pair: Array in identity_pairs:
		if expected_route[pair[0]] != pair[1]:
			return _bad("dialogue component differs from trusted expected route field: " + pair[0])
	if not str(source["path"]).begins_with("CDROM/SERIFU/") or str(source["path"]).contains(".."):
		return _bad("dialogue source path is not a safe SERIFU path")
	if not _is_sha256(str(source["sha256"])) or not _is_sha256(str(content.get("content_sha256", ""))):
		return _bad("dialogue source or content SHA-256 is malformed")
	if not _is_exact_integer(source.get("size_bytes")) or int(source["size_bytes"]) <= 0:
		return _bad("dialogue source size is malformed")
	if source.get("reader_schema") != READER_SCHEMA:
		return _bad("dialogue source reader schema is unsupported")
	if _component_digest(content) != str(content["content_sha256"]):
		return _bad("dialogue component content SHA-256 mismatch")
	if content.get("semantic_coverage_complete") != false:
		return _bad("dialogue component makes an unsupported completeness claim")
	var groups_value: Variant = content.get("groups")
	if not groups_value is Array or groups_value.is_empty() or groups_value.size() > MAX_GROUPS:
		return _bad("dialogue group list is empty or exceeds bounds")
	var groups: Array[Dictionary] = []
	var seen_ids: Dictionary = {}
	for group_position: int in range(groups_value.size()):
		var group: Variant = groups_value[group_position]
		if not group is Dictionary or not _is_exact_integer(group.get("index")):
			return _bad("dialogue group identity is missing or malformed")
		var group_index: int = int(group["index"])
		if group_index != group_position:
			return _bad("dialogue groups are not the complete contiguous source range")
		var rows: Variant = group.get("records")
		if not rows is Array or rows.is_empty() or rows.size() > MAX_LINES:
			return _bad("dialogue group rows are empty or exceed bounds")
		var terminator_count: int = 0
		var copied_rows: Array[Dictionary] = []
		for line_position: int in range(rows.size()):
			var row: Variant = rows[line_position]
			if not row is Dictionary:
				return _bad("dialogue row is not an object")
			if not _is_exact_integer(row.get("line")) or int(row["line"]) != line_position:
				return _bad("dialogue row line indices must be contiguous from zero")
			if not _is_exact_integer(row.get("command")):
				return _bad("dialogue row command is missing or malformed")
			var command: int = int(row["command"])
			if command < 0 or command > 255 or (command != 0xFF and command > 19):
				return _bad("dialogue command is outside the canonical SERIFU set")
			var expected_id := "talk:%s:%s:g%04d:l%04d" % [
				str(source["path"]), str(content["section_id"]), group_index, line_position]
			if not row.get("id") is String or str(row["id"]) != expected_id or seen_ids.has(expected_id):
				return _bad("dialogue row ID is missing, duplicated, or not bound to its source cursor")
			seen_ids[expected_id] = true
			var locator: Variant = row.get("source_locator")
			if not locator is Dictionary or not _has_exact_keys(locator,
					["path", "source_sha256", "text_offset", "command_offset"]):
				return _bad("dialogue source locator is missing or malformed")
			if locator["path"] != source["path"] or locator["source_sha256"] != source["sha256"]:
				return _bad("dialogue source locator does not match trusted source identity")
			if not _is_exact_integer(locator.get("command_offset")):
				return _bad("dialogue command locator offset is malformed")
			if int(locator["command_offset"]) < 0 or int(locator["command_offset"]) >= int(source["size_bytes"]):
				return _bad("dialogue command locator is outside the trusted source extent")
			if command == 0xFF:
				terminator_count += 1
				if line_position != rows.size() - 1 or row.has("text") or row.has("voice"):
					return _bad("0xFF must be the final identity-bearing row without text or voice")
				if not row.get("command_name") is String or row["command_name"] != "end":
					return _bad("0xFF source command name is invalid")
				if locator.get("text_offset") != null:
					return _bad("0xFF row must not carry a text source locator")
			else:
				if not _allow_voiced and (command in VOICE_COMMANDS or row.has("voice")):
					return _bad("voice-bearing dialogue is unsupported until an evidenced voice service is available")
				if _allow_voiced and command in VOICE_COMMANDS:
					if not row.has("voice") or not row["voice"] is Dictionary:
						return _bad("voice-bearing dialogue is missing voice metadata")
					var voice_val: Variant = row["voice"]
					if not voice_val.has("pairs") or not voice_val["pairs"] is Array:
						return _bad("voice metadata is malformed")
					for pair: Variant in voice_val["pairs"]:
						if not pair is Dictionary or not _is_exact_integer(pair.get("clip_start")) or not _is_exact_integer(pair.get("clip_end")):
							return _bad("voice clip metadata is malformed")
						if int(pair["clip_start"]) < 0 or int(pair["clip_end"]) < 0:
							return _bad("voice clip bounds must be non-negative")
				var text_value: Variant = row.get("text")
				if not text_value is Dictionary or not locator.has("text_offset"):
					return _bad("dialogue row text or source offset is missing")
				if not _is_exact_integer(locator["text_offset"]) or int(locator["text_offset"]) < 0:
					return _bad("dialogue text locator offset is malformed")
				if int(locator["text_offset"]) > int(source["size_bytes"]):
					return _bad("dialogue text locator is outside the trusted source extent")
				if not text_value.has("tokens") or not text_value["tokens"] is Array:
					return _bad("dialogue token records are missing")
				var text_error: String = Tokens.validate_line(row)
				if not text_error.is_empty():
					return _bad(text_error)
			copied_rows.append(row.duplicate(true))
		if terminator_count != 1:
			return _bad("each group must contain one final 0xFF identity row")
		groups.append({"index": group_index, "records": copied_rows})
	return {"error": "", "groups": groups}


func current_record() -> Dictionary:
	if _group_position >= _groups.size():
		return {"kind": "complete"}
	var group_data: Dictionary = _groups[_group_position]
	var records: Array = group_data["records"]
	if _line_position >= records.size():
		return {"error": "dialogue cursor passed the validated terminator"}
	var row: Dictionary = records[_line_position]
	if int(row["command"]) == 0xFF:
		return {"kind": "group_end", "group": int(group_data["index"]),
			"line": int(row["line"]), "record_id": row["id"]}
	return {"kind": "line", "record": row.duplicate(true)}

func present_choice(frame_count: int, choice_context: int = 0) -> Dictionary:
	if frame_count < 0 or frame_count > 0xFFFFFFFF:
		return {"error": "native frame count must be an unsigned 32-bit value"}
	if choice_context < 0 or choice_context > 0xFF:
		return {"error": "choice context must be an unsigned byte"}
	var current: Dictionary = current_record()
	if current.get("kind") != "line":
		return {"error": current.get("error", "current row is not a dialogue line")}
	var row: Dictionary = current["record"]
	var command: int = int(row["command"])
	if command not in [3, 8, 19]:
		return {"error": "command %d needs an unimplemented native talk service" % command}
	var split: Dictionary = Tokens.split_choice_options(row["text"]["tokens"])
	if not str(split.get("error", "")).is_empty():
		return split
	var options: Array = split["options"]
	var expected: int = 3 if command == 3 else 2
	if options.size() != expected:
		return {"error": "choice command %d has %d options; expected %d" % [command, options.size(), expected]}
	var order: Array = [0, 1, 2] if command == 3 else [0, 1]
	if command == 3 and choice_context != 0xFF:
		order = three_choice_order(frame_count)
	_pending_choice = {"record_id": row["id"], "order": order, "source_options": options}
	var presented: Array[Dictionary] = []
	for display_position: int in range(order.size()):
		var source_option: int = order[display_position]
		var option_tokens: Array = options[source_option].duplicate(true)
		if not _substitution_context.is_empty():
			var resolved := Tokens.resolve_tokens(option_tokens, _substitution_context)
			if str(resolved.get("error", "")).is_empty():
				option_tokens = resolved["tokens"]
		presented.append({"option_id": "%s:option:%d" % [row["id"], source_option],
			"source_index": source_option, "display_position": display_position,
			"tokens": option_tokens})
	return {"error": "", "record_id": row["id"], "command": command,
		"options": presented, "order": order.duplicate(), "text_complete_required": true,
		"voice_required": false}

func present_line(context: Dictionary = {}) -> Dictionary:
	var current: Dictionary = current_record()
	if current.get("kind") != "line":
		return {"error": current.get("error", "current row is not a dialogue line")}
	var row: Dictionary = current["record"]
	var command: int = int(row["command"])
	if command not in [0, 1, 2, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 18]:
		return {"error": "command %d needs an unimplemented native talk service" % command}

	var speaker_id: String = "player_family"
	var wrapper: String = "parenthesis"
	var completion: String = "wait_for_confirm_after_text"
	if command in [4, 6]:
		speaker_id = "player_family"
		wrapper = "parenthesis"
	elif command == 0:
		speaker_id = "player_family"
		wrapper = "bracket"
	elif command in [1, 2, 5]:
		speaker_id = "runtime_current_girl"
		wrapper = "quote"
	elif command == 7:
		speaker_id = "inline_or_effect"
		wrapper = "quote"
	elif command == 10:
		speaker_id = "yoshio"
		wrapper = "quote"
	elif command == 11:
		speaker_id = "shiori"
		wrapper = "quote"
	elif command == 12:
		speaker_id = "runtime_current_girl"
		wrapper = "quote"
		completion = "auto"
	elif command in [13, 18]:
		speaker_id = "runtime_current_girl"
		wrapper = "none"
		completion = "auto" if command == 18 else "wait_for_confirm_after_text"
	elif command == 14:
		speaker_id = "player_family"
		wrapper = "parenthesis"
		completion = "auto"
	elif command == 15:
		speaker_id = "runtime_current_girl"
		wrapper = "quote"

	var effective_context := _substitution_context.duplicate(true)
	effective_context.merge(context, true)

	var raw_tokens: Array = row["text"]["tokens"]
	var resolved := Tokens.resolve_tokens(raw_tokens, effective_context)
	if not str(resolved.get("error", "")).is_empty():
		return {"error": resolved["error"]}

	var line_tokens: Array = resolved["tokens"]
	if resolved.get("inline_label") != null:
		if not str(resolved.get("speaker_id", "")).is_empty():
			speaker_id = resolved["speaker_id"]
		if not str(resolved.get("wrapper", "")).is_empty():
			wrapper = resolved["wrapper"]

	var voice_required := false
	var voice_info := {}
	if row.has("voice") and row["voice"] is Dictionary:
		var voice_dict: Dictionary = row["voice"]
		var pairs: Array = voice_dict.get("pairs", [])
		var variant_index: int = int(_pending_choice.get("source_choice", 0))
		var selected_pair := {}
		for p: Variant in pairs:
			if p is Dictionary and int(p.get("variant", 0)) == variant_index:
				selected_pair = p
				break
		if selected_pair.is_empty() and not pairs.is_empty() and pairs[0] is Dictionary:
			selected_pair = pairs[0]
		if not selected_pair.is_empty():
			var c_start: int = int(selected_pair.get("clip_start", 0))
			var c_end: int = int(selected_pair.get("clip_end", 0))
			if c_start != 0 or c_end != 0:
				voice_required = true
				voice_info = {
					"speaker": str(voice_dict.get("speaker", speaker_id)),
					"clip_start": c_start,
					"clip_end": c_end,
					"variant": variant_index,
				}

	return {
		"error": "",
		"kind": "line",
		"record_id": row["id"],
		"tokens": line_tokens,
		"speaker_id": speaker_id,
		"wrapper": wrapper,
		"voice_required": voice_required,
		"voice_info": voice_info,
		"completion": completion,
		"command": command,
	}

func confirm_line(text_complete: bool, confirm_pressed: bool, voice_complete: bool = true) -> Dictionary:
	var view: Dictionary = present_line()
	if not str(view.get("error", "")).is_empty():
		return view
	if not text_complete:
		return {"error": "", "waiting": "typewriter"}
	if view.get("voice_required", false) and not _voice_cancelled:
		var v_done: bool = voice_complete
		if _voice_service != null and _voice_service.has_method("is_voice_complete"):
			v_done = bool(_voice_service.is_voice_complete())
		if not v_done:
			return {"error": "", "waiting": "voice"}
	if not confirm_pressed and view.get("completion") != "auto":
		return {"error": "", "waiting": "confirm"}
	_line_confirmed = true
	return {"error": "", "ready_to_advance": true, "record_id": view["record_id"]}

func choose(option_id: String, text_complete: bool, input_pressed: bool) -> Dictionary:
	if _pending_choice.is_empty():
		return {"error": "no choice is active"}
	if not text_complete:
		return {"error": "native choice input is gated until text completion"}
	if not input_pressed:
		return {"error": "", "waiting": true}
	for source_index: int in _pending_choice["order"]:
		var expected_id: String = "%s:option:%d" % [_pending_choice["record_id"], source_index]
		if option_id == expected_id:
			var selection := {"error": "", "record_id": _pending_choice["record_id"],
				"choice_id": option_id, "source_choice": source_index}
			_pending_choice["source_choice"] = source_index
			_line_confirmed = true
			return selection
	return {"error": "choice ID is not an option in the active native row"}

func advance_line(advance_line: bool) -> String:
	var current: Dictionary = current_record()
	if not advance_line:
		return ""
	if current.get("kind") == "group_end":
		_group_position += 1
		_line_position = 0
		_pending_choice.clear()
		_line_confirmed = false
		_voice_cancelled = false
		return ""
	if current.get("kind") != "line":
		return str(current.get("error", "dialogue has completed"))
	if not _pending_choice.is_empty() and not _pending_choice.has("source_choice"):
		return "selected dialogue choice must be consumed before advancing"
	var command: int = int(current["record"]["command"])
	if command in [0, 1, 2, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 18, 3, 8, 19] and not _line_confirmed:
		return "dialogue line must complete its native input gate before advancing"
	if command not in [0, 1, 2, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 18, 3, 8, 19]:
		return "command %d needs an unimplemented native talk service" % command
	_line_position += 1
	_line_confirmed = false
	_voice_cancelled = false
	_pending_choice.clear()
	return ""

static func three_choice_order(frame_count: int) -> Array[int]:
	## Exact junban_init arithmetic, before make_three_select consumes it.
	var first: int = frame_count % 3
	var second: int
	var third: int
	if first == 0:
		second = first + (frame_count & 1) + 1
		third = first + ((frame_count + 1) & 1) + 1
	elif first == 1:
		second = (first - 1) + ((frame_count & 1) * 2)
		third = (first - 1) + (((frame_count + 1) & 1) * 2)
	else:
		second = (first - 1) - (frame_count & 1)
		third = (first - 1) - ((frame_count + 1) & 1)
	return [first, second, third]

func _component_digest(content: Dictionary) -> String:
	var material: Dictionary = content.duplicate(true)
	material.erase("content_sha256")
	var normalized: Variant = _normalize_digest_value(material)
	var canonical: String = JSON.stringify(normalized, "", true, false)
	return canonical.sha256_text()

func _normalize_digest_value(value: Variant) -> Variant:
	if typeof(value) == TYPE_DICTIONARY:
		var result: Dictionary = {}
		var keys: Array = value.keys()
		keys.sort()
		for key: Variant in keys:
			result[str(key)] = _normalize_digest_value(value[key])
		return result
	if typeof(value) == TYPE_ARRAY:
		var items: Array = []
		for item: Variant in value:
			items.append(_normalize_digest_value(item))
		return items
	if typeof(value) == TYPE_INT:
		return str(value)
	if typeof(value) == TYPE_FLOAT:
		var number: float = float(value)
		if not is_finite(number) or floor(number) != number:
			return "__invalid_fractional_number__"
		return str(int(number))
	return value

func _has_exact_keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key: String in expected:
		if not value.has(key):
			return false
	return true

func _is_exact_integer(value: Variant) -> bool:
	if typeof(value) == TYPE_INT:
		return true
	if typeof(value) != TYPE_FLOAT:
		return false
	var number: float = float(value)
	return is_finite(number) and floor(number) == number and absf(number) <= 2147483647.0

func _is_sha256(value: String) -> bool:
	if value.length() != 64:
		return false
	for index: int in range(value.length()):
		var code: int = value.unicode_at(index)
		if not ((code >= 48 and code <= 57) or (code >= 97 and code <= 102)):
			return false
	return true

func _bad(message: String) -> Dictionary:
	return {"error": message}
