extends RefCounted
## T-29002: nominal school-day 60/1 logic clock; no claim of measured NTSC rate.
## Vblanks and timer interrupts are explicit observations, never inferred ticks.

const TICKS_PER_SECOND: int = 60
const NANOSECONDS_PER_SECOND: int = 1000000000
const MAX_ELAPSED_NS: int = 1000000000000

var logic_iterations: int = 0
var vblanks: int = 0
var rendered_frames: int = 0
var pending_steps: int = 0
var paused: bool = false
var _phase: int = 0

@warning_ignore("integer_division")
func queue_elapsed_ns(elapsed_ns: int) -> String:
	if elapsed_ns < 0 or elapsed_ns > MAX_ELAPSED_NS:
		return "elapsed_ns outside 0..1000000000000; split long stalls explicitly"
	if paused:
		return ""
	var scaled: int = _phase + elapsed_ns * TICKS_PER_SECOND
	var due: int = scaled / NANOSECONDS_PER_SECOND
	if pending_steps > 9223372036854775807 - due:
		return "pending logic step count exceeds int64"
	_phase = scaled % NANOSECONDS_PER_SECOND
	pending_steps += due
	return ""

func consume_pending_step() -> bool:
	if paused or pending_steps == 0:
		return false
	pending_steps -= 1
	return true

func snapshot() -> Dictionary:
	return {"logic": logic_iterations, "vblanks": vblanks, "rendered": rendered_frames,
		"pending": pending_steps, "phase": _phase, "paused": paused}

func restore(checkpoint_data: Dictionary) -> void:
	logic_iterations = checkpoint_data["logic"]
	vblanks = checkpoint_data["vblanks"]
	rendered_frames = checkpoint_data["rendered"]
	pending_steps = checkpoint_data["pending"]
	_phase = checkpoint_data["phase"]
	paused = checkpoint_data["paused"]
