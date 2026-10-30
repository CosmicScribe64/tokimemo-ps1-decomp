extends SceneTree
## Original resource validation plus a source-order regression, not full-scene proof.
const Board = preload("res://services/first_arrival_board_store.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
var calls: Array[String] = []

func _initialize() -> void:
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"), Board.MANIFEST_SHA)
	if not error.is_empty():
		printerr(error)
		quit(1)
		return
	var description: Dictionary = board.background_descriptions()[str(0xad00)]
	if description.buffer != 1 or description.rectangle != [320,256,320,160] or description.sha256 != Board.BG_AD00_TRANSFER_SHA:
		printerr("AD00 source transfer/alternating destination differs")
		quit(1)
		return
	var state := State.new()
	var actors: Dictionary = board.actor_descriptions()
	var cut: Dictionary = actors.get("49:2149203968:43702", {})
	var overlay: Dictionary = actors.get("9:2149515264:44028", {})
	if cut.get("id") != Board.CUT_08_ID or cut.get("sha256") != Board.CUT_08_SHA or overlay.get("id") != Board.O_08_ID or overlay.get("sha256") != Board.O_08_SHA:
		printerr("line11 source actor transfer identities differ")
		quit(1)
		return
	state.game_mode = 0x15
	state.mode_stage = 7
	state.mode_step = 0
	state.substep = 4
	var result := Session.FrameResult.new()
	Arrival._stage_07_line_11(state, {
		"cd_sync": func(_s, _r): calls.append("cd"); return 1,
		"background_decode": func(_s, _slot, _r): calls.append("decode"); return 1,
		"bank_load_start": func(_s, role, _r): calls.append(role),
		"outer_draw": func(_s, _argument, _r): calls.append("draw"),
	}, result)
	if not result.error.is_empty() or state.substep != 5 or calls != ["cd", "prologue_rei", "draw"]:
		printerr("line11 substep4 must wait for CD and load bank without decoding: ", calls)
		quit(1)
		return
	state.substep = 3
	calls.clear()
	result = Session.FrameResult.new()
	Arrival._stage_07_line_11(state, {
		"background_read":func(_s, lba, slot, _r): calls.append("read:%x:%d" % [lba,slot]),
		"cd_sync":func(_s, _r): calls.append("cd"); return 0,
		"arrival_draw":func(_s, _slot, _r): calls.append("arrival"),
		"outer_draw":func(_s, _argument, _r): calls.append("outer"),
	}, result)
	if not result.error.is_empty() or state.substep != 4 or calls != ["read:ad00:1", "cd", "arrival", "outer"]:
		printerr("line11 read must fall through to CD poll in the same source frame: ",calls)
		quit(1)
		return
	state.substep = 6
	calls.clear()
	result = Session.FrameResult.new()
	Arrival._stage_07_line_11(state, {
		"cd_sync":func(_s, _r): return 1,
		"actor_read":func(_s, count, destination, lba, _r): calls.append("read:%d:%x:%x" % [count,destination,lba]),
		"outer_draw":func(_s, _argument, _r): pass,
	}, result)
	if not result.error.is_empty() or state.substep != 7 or calls != ["read:9:801f0000:abfc"]:
		printerr("line11 O09 must load into backup RAM before source memcpy: ", calls)
		quit(1)
		return
	state.substep = 7
	result = Session.FrameResult.new()
	Arrival._stage_07_line_11(state, {"cd_sync":func(_s, _r): return 1}, result)
	if result.error.is_empty() or state.mode_stage != 7:
		printerr("line11 advanced without the required source actor/palette initialization")
		quit(1)
		return
	# Synthetic owned-state descriptor exercises the real decode callback; the
	# resource and decoded words above come from the original pack.
	var media := Media.new()
	media._board_store = board
	state.opening["names_media"] = {"bg_show_1":0,"buffer_0":{"lba":0xacec}}
	state.opening["media"] = {"epoch":1,"decoded":{},"background_pending":{"1":"ad00"},"requests":{"ad00":{"kind":"background","resource_id":Board.BG_AD00_ID,"status":"ready","request_id":"ad00"}}}
	result = Session.FrameResult.new()
	if media.background_decode(state, 1, result) != 1 or not result.error.is_empty() or state.opening.names_media.bg_show_1 != 1 or result.presentation.size() != 2:
		printerr("line11 completed background did not decode into alternate buffer at substep7")
		quit(1)
		return
	state.opening.names_media.bg_show_1 = 1
	result = Session.FrameResult.new()
	if media.background_decode(state, 1, result) != -1 or result.error.is_empty() or not result.presentation.is_empty():
		printerr("line11 decode accepted incorrect active source buffer")
		quit(1)
		return
	state.opening.names_media.bg_show_1 = 0
	state.opening["arrival_media"] = {"background_brightness":128}
	state.opening.media["actor_pending"] = {str(0x801a4000):"cut",str(0x801f0000):"actor"}
	state.opening.media.requests["cut"] = {"kind":"actors","status":"ready","resource_id":Board.CUT_08_ID,"resource_sha256":Board.CUT_08_SHA}
	state.opening.media.requests["actor"] = {"kind":"actors","status":"ready","resource_id":Board.O_08_ID,"resource_sha256":Board.O_08_SHA}
	state.opening["actor127"] = {"flags":1}
	state.opening["actor159"] = {"flags":1}
	result = Session.FrameResult.new()
	media.stage_07_to_08_setup(state,result)
	if not result.error.is_empty() or result.presentation.size() != 5 or state.opening.has("actor127") or state.opening.has("actor159") or state.opening.actor96.draw_flags != 0x84 or state.opening.actor96.brightness != 0 or state.opening.bg_layer_0.clut_row != 494 or state.opening.bg_brightness != 0 or state.opening.cut_media.white_palette != true:
		printerr("Rei transition source actor/palette/brightness setup differs: ", result.error)
		quit(1)
		return
	for intent in result.presentation:
		if not media._intent_schema(intent).is_empty():
			printerr("Rei transition produced an unsupported upload/copy intent: ",intent)
			quit(1)
			return
	print("PASS: original line11 resources, delayed decode and required transition setup")
	quit(0)
