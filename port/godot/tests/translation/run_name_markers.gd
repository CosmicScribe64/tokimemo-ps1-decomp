extends SceneTree
const Markers = preload("res://presentation/name_entry_markers.gd")
const Entry = preload("res://flow/new_game_entry.gd")

func _initialize() -> void:
	var markers := Markers.new()
	var entry := Entry.new(); entry.stage = 1; entry.step_id = 1; entry.cursor = 0
	var frames: Array[int] = []
	for draw: int in 12:
		markers.advance(entry); frames.append(markers.frames[0])
	if frames != [10,10,10,11,11,11,12,12,12,10,10,10]:
		printerr("source three-draw marker durations differ: ",frames); quit(1); return
	markers.advance(entry) # One retained tick in sequence frame11.
	entry.cursor = 1; markers.advance(entry)
	entry.cursor = 0; markers.advance(entry)
	if markers.frames[0] != 10 or markers.counters[0] != 2 or markers.frames[1] != 13:
		printerr("source reactivation did not retain counter/reset frame"); quit(1); return
	entry.cursor = 12; markers.advance(entry)
	if markers.active != -1 or markers.frames.count(13) != 11:
		printerr("completed name still has active marker"); quit(1); return
	entry.step_id = 3; entry.cursor = 3; markers.advance(entry)
	if markers.active != 3 or markers.frames[3] != 10:
		printerr("given-name marker did not initialize"); quit(1); return
	print("Native NAME marker sequence checks passed")
	quit()
