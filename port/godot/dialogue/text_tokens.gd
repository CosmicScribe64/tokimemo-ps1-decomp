extends RefCounted
class_name NativeTextTokens
## Validates the existing reader's token view without replacing original bytes,
## and provides pure deterministic substitution resolution and inline label extraction.

const NAME_CODES: Dictionary = {
	"P": "player_form",
	"p": "player_family",
	"m": "player_given",
	"F": "player_full",
	"G": "girl_name",
	"Y": "yoshio_name",
	"S": "context_s",
	"O": "context_o",
	"W": "context_w",
	"A": "context_a",
	"M": "context_m",
}

const CONTEXT_KEY_ALIASES: Dictionary = {
	"player_form": ["player_form", "player_form_hex", "P"],
	"player_family": ["player_family", "player_family_hex", "family_name_hex", "p"],
	"player_given": ["player_given", "player_given_hex", "given_name_hex", "m"],
	"player_full": ["player_full", "player_full_hex", "F"],
	"girl_name": ["girl_name", "girl_name_hex", "G"],
	"yoshio_name": ["yoshio_name", "yoshio_name_hex", "Y"],
	"context_s": ["context_s", "context_s_hex", "S"],
	"context_o": ["context_o", "context_o_hex", "O"],
	"context_w": ["context_w", "context_w_hex", "W"],
	"context_a": ["context_a", "context_a_hex", "A"],
	"context_m": ["context_m", "context_m_hex", "M"],
}

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

static func resolve_tokens(tokens: Array, context: Dictionary) -> Dictionary:
	if tokens == null:
		return {"error": "tokens list must not be null"}
	if context == null:
		return {"error": "context must not be null"}

	var inline_label: Variant = null
	var start_index: int = 0
	var wrapper: String = ""
	var speaker_id: String = ""

	if not tokens.is_empty():
		var first: Variant = tokens[0]
		if first is Dictionary and first.get("kind") == "inline_label":
			inline_label = first.duplicate(true)
			start_index = 1
			var marker: String = str(inline_label.get("marker", ""))
			if marker == "<":
				wrapper = "parenthesis"
			elif marker in [">", "-", "+"]:
				wrapper = "bracket"
			speaker_id = str(inline_label.get("label_raw_hex", ""))

	var resolved_tokens: Array = []
	for index: int in range(start_index, tokens.size()):
		var token: Variant = tokens[index]
		if not token is Dictionary:
			return {"error": "dialogue token is malformed"}
		var kind: String = str(token.get("kind", ""))
		if kind == "substitution":
			var code: String = str(token.get("code", ""))
			var name: String = str(token.get("name", ""))
			if name.is_empty() and NAME_CODES.has(code):
				name = str(NAME_CODES[code])
			elif name.is_empty():
				name = code

			var lookup: Dictionary = _resolve_substitution_hex(context, name, code)
			if not str(lookup.get("error", "")).is_empty():
				return lookup
			var hex_value: String = str(lookup["hex"])
			for offset: int in range(0, hex_value.length(), 4):
				var glyph_hex: String = hex_value.substr(offset, 4).to_lower()
				resolved_tokens.append({"kind": "glyph", "raw_hex": glyph_hex})
		else:
			resolved_tokens.append(token.duplicate(true))

	return {
		"error": "",
		"tokens": resolved_tokens,
		"inline_label": inline_label,
		"wrapper": wrapper,
		"speaker_id": speaker_id,
	}

static func _resolve_substitution_hex(context: Dictionary, name: String, code: String) -> Dictionary:
	if name == "player_full" or code == "F":
		var full_val: Variant = _find_context_value(context, "player_full", "F")
		if full_val != null:
			return _validate_substitution_hex(full_val, "player_full")
		var fam_val: Variant = _find_context_value(context, "player_family", "p")
		var giv_val: Variant = _find_context_value(context, "player_given", "m")
		if fam_val != null and giv_val != null:
			var fam_res: Dictionary = _validate_substitution_hex(fam_val, "player_family")
			if not str(fam_res.get("error", "")).is_empty():
				return fam_res
			var giv_res: Dictionary = _validate_substitution_hex(giv_val, "player_given")
			if not str(giv_res.get("error", "")).is_empty():
				return giv_res
			return {"error": "", "hex": str(fam_res["hex"]) + str(giv_res["hex"])}
		return {"error": "missing required substitution context key: player_full"}

	var val: Variant = _find_context_value(context, name, code)
	if val == null:
		return {"error": "missing required substitution context key: " + name}
	return _validate_substitution_hex(val, name)

static func _find_context_value(context: Dictionary, name: String, code: String) -> Variant:
	var aliases: Array = CONTEXT_KEY_ALIASES.get(name, [name])
	for alias: String in aliases:
		if context.has(alias):
			return context[alias]
	if not code.is_empty() and context.has(code):
		return context[code]
	return null

static func _validate_substitution_hex(val: Variant, name: String) -> Dictionary:
	var hex_string: String = ""
	if val is String:
		hex_string = str(val).strip_edges().to_lower()
	elif val is PackedByteArray:
		hex_string = (val as PackedByteArray).hex_encode().to_lower()
	elif val is Array:
		var reconstructed := ""
		for item: Variant in val:
			if item is Dictionary and item.get("kind") == "glyph" and _is_hex(str(item.get("raw_hex", ""))):
				reconstructed += str(item["raw_hex"]).to_lower()
			elif item is String and _is_hex(item):
				reconstructed += item.to_lower()
			else:
				return {"error": "substitution token in " + name + " is not a valid glyph token"}
		hex_string = reconstructed
	else:
		return {"error": "unsupported substitution value type for " + name}

	if hex_string.is_empty() or hex_string.length() % 4 != 0 or not _is_hex(hex_string):
		return {"error": "substitution value for " + name + " must be non-empty 2-byte Shift-JIS hex"}
	return {"error": "", "hex": hex_string}

static func _is_hex(value: String) -> bool:
	if value.is_empty() or value.length() % 2 != 0:
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
