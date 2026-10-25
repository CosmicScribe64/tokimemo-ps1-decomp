extends RefCounted
## Canonical pad snapshots, after device mapping. pad_input.c::pad_poll edges.
## Raw devices, turbo, menu repeat and mouse conversion remain separate work.

const Math = preload("res://core/native_math.gd")

var tick: int:
	get: return _tick
var held: int:
	get: return _held
var pressed: int:
	get: return _pressed
var released: int:
	get: return _released

var _tick: int
var _held: int
var _pressed: int
var _released: int

func _init(tick_id: int = 0, held_word: int = 0, previous_word: int = 0) -> void:
	_tick = tick_id
	_held = Math.u32(held_word)
	_pressed = Math.u32(~previous_word & _held)
	_released = Math.u32(previous_word & ~_held)

func snapshot() -> Dictionary:
	return {"tick": tick, "held": held, "pressed": pressed, "released": released}
