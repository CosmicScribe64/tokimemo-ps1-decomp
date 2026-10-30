extends Node
## Test-only driver of flow/title_file_select.gd. The installed router does not
## reach it: no boot screen (movie, title, file select) is drawn from original
## resources yet, so no installed scene presents this path. It runs one
## boot_loop iteration per physics tick with the live EntryPad, keeps the
## movie's measured pad-poll spacing and, at NAME_ENT stage 1, builds the
## handoff for the entry. It draws nothing.

const Flow = preload("res://flow/title_file_select.gd")
const EntryPad = preload("res://ui/entry_pad.gd")

var flow := Flow.new()
var failure: String = ""
var tick: int = 0
var handoff: Dictionary = {}
var _pad: EntryPad
var _wait_ticks: int = 0

func _ready() -> void:
	_pad = EntryPad.new()
	add_child(_pad)
	flow.keep_trace = false
	failure = flow.begin()
	_wait_ticks = flow.ticks_until_next_sample()

func _physics_process(_delta: float) -> void:
	if not handoff.is_empty() or not failure.is_empty(): return
	if not flow.failure.is_empty() or not flow.refusal.is_empty(): return
	tick += 1
	# The movie polls the pad once per decoded frame; the ticks in between are
	# vblanks inside the same blocked iteration.
	_wait_ticks -= 1
	if _wait_ticks > 0: return
	var sample = _pad.sample(tick)
	flow.step(sample.held)
	_wait_ticks = flow.ticks_until_next_sample()
	if flow.finished:
		handoff = flow.handoff()
		failure = str(handoff.get("error", ""))
