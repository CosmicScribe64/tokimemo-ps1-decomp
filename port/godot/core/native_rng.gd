extends RefCounted
## T-29002: the game's rand/srand are BIOS A0[2f]/A0[30] trampolines.
## This measured profile is OpenBIOS d9384092, NOT verified Sony BIOS parity.

const Math = preload("res://core/native_math.gd")
const OPENBIOS_PROFILE: String = "openbios-d93840921b5ad7ed515e95b0e414f1b1e2038b1a"

var profile: String = ""
var state: int = 0
var draw_count: int = 0
var trace: Array[Dictionary] = []
var error: String = "RNG profile is unconfigured"
var _step_error: String = ""
var _failure_receiver: Callable

func configuration_error() -> String:
	if profile != OPENBIOS_PROFILE:
		return "RNG profile is unconfigured or unsupported"
	return error

func begin_step(failure_receiver: Callable = Callable()) -> void:
	_step_error = ""
	_failure_receiver = failure_receiver

func end_step() -> void:
	_failure_receiver = Callable()

func step_error() -> String:
	return _step_error

func _refuse(reason: String) -> int:
	error = reason
	if _step_error.is_empty():
		_step_error = reason
	if _failure_receiver.is_valid():
		_failure_receiver.call(reason)
	return -1

func configure(profile_id: String, seed_word: int) -> bool:
	if profile_id != OPENBIOS_PROFILE:
		profile = ""
		_refuse("unsupported BIOS RNG profile: " + profile_id)
		return false
	profile = profile_id
	reseed(seed_word)
	return true

func reseed(seed_word: int) -> void:
	state = Math.u32(seed_word)
	draw_count = 0
	trace.clear()
	error = "" if profile == OPENBIOS_PROFILE else "RNG profile is unconfigured"

func next(caller_id: String) -> int:
	if profile != OPENBIOS_PROFILE:
		return _refuse("RNG profile is unconfigured")
	if caller_id.is_empty():
		return _refuse("RNG draw requires a source caller ID")
	var before: int = state
	var product: Math.WordPair = Math.multu32(state, 0x41c64e6d)
	state = Math.u32(product.lo + 0x3039)
	var result: int = (state >> 16) & 0x7fff
	draw_count += 1
	trace.append({"caller": caller_id, "before": before, "after": state, "result": result})
	error = ""
	return result

func checkpoint() -> Dictionary:
	return {"profile": profile, "state": state, "draw_count": draw_count,
		"trace": trace.duplicate(true), "error": error, "step_error": _step_error}

func restore(checkpoint_data: Dictionary) -> void:
	profile = checkpoint_data["profile"]
	state = checkpoint_data["state"]
	draw_count = checkpoint_data["draw_count"]
	trace.assign(checkpoint_data["trace"])
	error = checkpoint_data["error"]
	_step_error = checkpoint_data["step_error"]
