extends "res://tests/opening/run_opening_frame_timing_route.gd"
## T-39027: the whole-opening frame timing route with the observe-only pacing
## probe attached. Everything the timing route checks (substep order, frame_count
## deltas against tests/fixtures/opening_frame_timing.json) still applies; this
## script adds /out/pacing-probe.json (logical ticks and host frames) for
## tools/port/frame_pacing_report.py to compare with the emulator's per-vsync
## samples. The probe only reads published state. Runner:
## tools/port/run_presentation_pacing.sh <new-out-dir> <pad>.

const Probe = preload("res://tests/support/pacing_probe.gd")
const OPENING_MODE := 0x15
var probe: Probe

func _start() -> void:
	probe = Probe.new()
	root.add_child(probe)
	super._start()

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	if probe != null and state != null:
		var opening_key: Variant = null
		if state.game_mode == OPENING_MODE:
			opening_key = "%d/%d/%d" % [state.mode_stage, state.mode_step, state.substep]
		probe.observe(tick, int(state.frame_count), opening_key)
	super._frame_published(tick, app)

func _script_entry(entry: Node) -> void:
	# The entry scene is ready for input: the first interactive moment.
	if probe != null: probe.mark_interactive()
	super._script_entry(entry)

func _finish_rei(app: App) -> void:
	if probe != null:
		check(not probe.overflowed, "pacing probe overflowed its frame budget")
		check(probe.write("/out/pacing-probe.json"), "pacing probe receipt could not be written")
	super._finish_rei(app)
