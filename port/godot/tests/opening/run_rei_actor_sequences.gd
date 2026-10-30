extends SceneTree
const Sequences = preload("res://services/rei_actor_sequences.gd")
const Renderer = preload("res://presentation/rei_actor_renderer.gd")
var failed := false

func check(ok: bool, label: String) -> void:
	if not ok:
		failed = true
		push_error(label)

func _initialize() -> void:
	var path := OS.get_environment("REI_BOARD_PACK")
	var bytes := FileAccess.get_file_as_bytes(path.path_join("o_09.nbn"))
	var source := Sequences.new()
	check(source.prepare(bytes).is_empty(),"original O10 tables authenticate")
	check(Renderer.new().source.prepare(bytes).is_empty(),"Rei renderer uses the same authenticated tables")
	if failed: quit(1); return
	var blink := {"sequence":"blink","seq_step":0,"sequence_counter":0,"flags":3,"speed":1}
	var drawn: Array = []
	for tick in 10:
		var result := source.draw_and_advance(blink,tick)
		check(result.error.is_empty(),"blink source row accepted")
		drawn.append(result.get("frame_index"))
	check(drawn == [1,1,2,2,3,3,4,4,3,3],"draw-before-advance preserves all five original rows")
	check(blink.flags == 0 and blink.seq_step == 0 and blink.frame_index == 1,"flags3 stops and resets blink")
	for name in ["crowd_left","crowd_right"]:
		var actor := {"sequence":name,"seq_step":0,"sequence_counter":0,"flags":5,"speed":1}
		for tick in 23:
			check(source.draw_and_advance(actor,tick).error.is_empty(),"crowd advances original duration")
		check(actor.flags == 5 and actor.seq_step == 3,"crowd remains active before final draw")
		var last := source.draw_and_advance(actor,23)
		check(last.frame_index == (3 if name == "crowd_left" else 7) and actor.flags == 0 and actor.seq_step == 3,"flags5 stops on last original frame after24 draws")
	var hold := {"sequence":"blink","seq_step":3,"sequence_counter":0,"flags":0,"speed":1}
	check(source.draw_and_advance(hold,90).frame_index == 4 and hold.seq_step == 3,"line14 source sequence index3 is not image frame3")
	var invalid := {"sequence":"blink","seq_step":5,"sequence_counter":0,"flags":3,"speed":1}
	var before := invalid.duplicate(true)
	check(not source.draw_and_advance(invalid,91).error.is_empty() and invalid == before,"invalid cursor rejected without mutation")
	check(source.parts("body",0).size() == 4 and source.parts("crowd",5).size() == 2,"original multi-part frames retained")
	check(source.parts("crowd",5)[0].palette_flags == 0x1a08,"per-part palette/ABR flags are retained")
	var speed128 := {"sequence":"blink","seq_step":0,"sequence_counter":0,"flags":1,"speed":0x80}
	check(source.draw_and_advance(speed128,127).error.is_empty() and speed128.sequence_counter == 0,"speed80 does not increment before divisor boundary")
	check(source.draw_and_advance(speed128,128).error.is_empty() and speed128.sequence_counter == 1,"source speed80 falls through to frame modulo128")
	bytes[0] ^= 1
	check(not source.prepare(bytes).is_empty(),"changed original resource rejected")
	check(source.parts("body",0).is_empty() and not source.draw_and_advance(blink,129).error.is_empty(),"failed reopen clears previous authenticated tables")
	if not failed: print("PASS: original Rei sequence decode, draw ordering and terminal flags")
	quit(1 if failed else 0)
