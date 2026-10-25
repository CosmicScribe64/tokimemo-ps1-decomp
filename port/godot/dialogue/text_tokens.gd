extends RefCounted
class_name NativeTextTokens
## Validates the existing reader's token view without replacing original bytes.

static func validate_line(line: Dictionary) -> String:
	if not line.get("id") is String or str(line["id"]).is_empty() or not line.has("command") or not line.has("text"):
		return "dialogue row is missing stable ID, command, or text"
	if not _is_exact_integer(line["command"]) or int(line["command"]) < 0 or int(line["command"]) > 19:
		return "dialogue command is outside supported SERIFU commands"
	var text: Variant = line["text"]
	if not text is Dictionary or not text.get("raw_hex") is String or not text.get("tokens") is Array:
		return "dialogue text payload or token list is malformed"
	var raw_hex: String = str(text["raw_hex"])
	if not _is_hex(raw_hex):
		return "dialogue payload has malformed original bytes"
	if int(text.get("unknown_token_count", -1)) < 0:
		return "dialogue unknown-token count is invalid"
	var reconstructed_hex: String = ""
	for token: Variant in text["tokens"]:
		if not token is Dictionary or not token.get("kind") is String or not token.get("raw_hex") is String:
			return "dialogue token lacks kind or original bytes"
		if not _is_hex(str(token["raw_hex"])):
			return "dialogue token has malformed original bytes"
		reconstructed_hex += str(token["raw_hex"])
	if reconstructed_hex != raw_hex:
		return "dialogue tokens do not reconstruct the source bytes"
	return ""

static func split_choice_options(tokens: Array) -> Dictionary:
	var options: Array = [[]]
	for token: Variant in tokens:
		if not token is Dictionary:
			return {"error": "choice token is malformed"}
		if token.get("kind") == "separator" and token.get("role") == "choice_option":
			options.append([])
		else:
			options[-1].append(token.duplicate(true))
	return {"error": "", "options": options}

static func _is_hex(value: String) -> bool:
	if value.length() % 2 != 0:
		return false
	for index: int in range(value.length()):
		var code: int = value.unicode_at(index)
		if not ((code >= 48 and code <= 57) or (code >= 97 and code <= 102)
				or (code >= 65 and code <= 70)):
			return false
	return true

static func _is_exact_integer(value: Variant) -> bool:
	if typeof(value) == TYPE_INT:
		return true
	if typeof(value) != TYPE_FLOAT:
		return false
	var number: float = float(value)
	return is_finite(number) and floor(number) == number and absf(number) <= 2147483647.0
