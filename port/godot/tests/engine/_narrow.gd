extends RefCounted
## Test helper: the engine's return narrowing (GameEngine._narrow is static).
const GameEngine = preload("res://engine/engine.gd")

static func apply(value: int, c_type: String) -> int:
	return GameEngine._narrow(value, c_type)
