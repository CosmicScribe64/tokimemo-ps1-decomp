extends "res://rules/school_day_state.gd"
## Opening service/control records share GameSession's transaction boundary.

const _OPENING_SCRIPT_PATH := "res://rules/new_game_route_state.gd"

var opening: Dictionary = {}


func projection() -> Dictionary:
	var result: Dictionary = super.projection()
	result["opening"] = opening.duplicate(true)
	return result


func to_words() -> Array[int]:
	var result: Array[int] = super.to_words()
	for value: int in JSON.stringify(opening).to_utf8_buffer():
		result.append(value)
	return result

## JSON.stringify(to_words()) without the per-byte list (GameState.words_json).
## A subclass script falls back to its own to_words().
func words_json() -> String:
	if get_script().resource_path != _OPENING_SCRIPT_PATH: return JSON.stringify(to_words())
	return _words_text.render(JSON.stringify(core_words()), [rule_state, opening])


func restore(checkpoint_data: Dictionary) -> void:
	super.restore(checkpoint_data)
	opening = checkpoint_data.get("opening", {}).duplicate(true)
