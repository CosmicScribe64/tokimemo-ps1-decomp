extends RefCounted
## Source name_entry_slot_markers_update + actor_sprites sequence advancement.
## State advances once per source draw; read-only rendering never advances it.
var counters: Array[int] = []
var steps: Array[int] = []
var frames: Array[int] = []
var active := -1
var draws := 0

func _init() -> void:
	counters.resize(11); counters.fill(0)
	steps.resize(11); steps.fill(0)
	frames.resize(11); frames.fill(13)

func advance(entry: RefCounted) -> bool:
	var selected := -1
	if entry.stage == 1 and entry.step_id in [1,3,5]:
		var first: int = (entry.step_id-1)/2*3
		var limit: int = 11 if first == 6 else first+3
		if entry.cursor >= first and entry.cursor < limit: selected = entry.cursor
	var previous := frames.duplicate()
	frames.fill(13)
	if selected >= 0:
		# Reactivation resets sequence index/flags, but source leaves the counter.
		if active != selected: steps[selected] = 0
		frames[selected] = 10+steps[selected]
		counters[selected] += 1 # actor_clear sets increment divisor to one.
		if counters[selected] >= 3:
			counters[selected] = 0
			steps[selected] = (steps[selected]+1)%3
	active = selected
	draws += 1
	return frames != previous

func snapshot() -> Dictionary:
	return {"source":"name_entry_slot_markers_update/actor_sprites","draws":draws,
		"active":active,"frames":frames.duplicate(),"steps":steps.duplicate(),"counters":counters.duplicate()}
