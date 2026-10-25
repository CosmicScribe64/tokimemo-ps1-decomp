extends "res://rules/school_day_state.gd"
## Opening service/control records share GameSession's transaction boundary.

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


func restore(checkpoint_data: Dictionary) -> void:
	super.restore(checkpoint_data)
	opening = checkpoint_data.get("opening", {}).duplicate(true)
