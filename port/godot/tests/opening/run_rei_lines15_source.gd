extends SceneTree
## Stage10 source control flow only; actor/media callbacks are explicit test
## doubles (prologue.c::prologue_lines_15_and_16_run).
const Flow = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var state := State.new()
var services: Dictionary
var events: Array = []
var voice := 0
var cd := 0
var decoded: Variant = 1
var complete_line := false
var reject_setup := false
var failures := 0
var checks := 0
func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr(label)
func effect(_state, kind, value, result) -> Variant:
	events.append([kind,value])
	if kind == "next_scene" and reject_setup: result.error = "rejected actual O12 setup"
	return null
func tick() -> Session.FrameResult:
	events.clear()
	var result := Session.FrameResult.new()
	Flow._stage_10_lines_15_16(state,services,result)
	return result
func names() -> Array: return events.map(func(e): return e[0])
func _initialize() -> void:
	state.game_mode = 0x15; state.mode_stage = 10; state.mode_step = 0
	services = {"rei_lines15_effect":effect,
		"line_start":func(_s,line,_out): events.append(["line",line]),
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["background",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"cd_audio_state":func(_s): return voice,
		"cd_sync":func(_s,_out): return cd,
		"actor_read":func(_s,count,destination,lba,_out): events.append(["read",count,destination,lba]),
		"background_read":func(_s,lba,slot,_out,retry := false): events.append(["bg_read",lba,slot,retry]),
		"background_decode":func(_s,slot,_out): events.append(["decode",slot]); return decoded,
		"audio_command":func(_s,command,_out): events.append(["audio",command]),
		"bank_load_start":func(_s,role,_out): events.append(["bank",role])}
	check(tick().error.is_empty() and state.substep == 1 and state.opening.prologue_line == 15,"Line15 start differs")
	check(names() == ["line","background","outer"],"Substep0 must start line15 then dec_bg_draw without moving x: " + str(names()))
	tick(); check(state.substep == 1 and names() == ["wait_voice","background","outer"],"CD wait advanced before playback or moved x")
	voice = 4; tick(); check(state.substep == 2 and names() == ["wait_voice","advance_x","background","outer"],"Post-increment substep2 must already move actor96")
	tick(); check(state.substep == 2 and names() == ["lip_sync","run","advance_x","background","outer"],"Line15 lip/text/x order differs: " + str(names()))
	complete_line = true; tick(); complete_line = false
	check(state.substep == 3,"Line15 completion did not advance")
	tick(); check(state.substep == 4 and events[0] == ["line",16] and names().has("advance_x"),"Line16 start or x move differs")
	tick(); check(names() == ["run","advance_x","background","outer"],"Line16 must not refresh lip-sync actors")
	complete_line = true; tick(); complete_line = false
	tick(); check(state.substep == 6 and events[0] == ["read",49,0x801a4000,0xab18],"Stage10 CUT12 read differs from source")
	tick(); check(state.substep == 6 and not names().has("palette_adjust"),"Pending CUT12 read was palette-adjusted")
	cd = 1; tick(); check(state.substep == 7 and events[0] == ["palette_adjust",0] and events[1] == ["read",17,0x801f0000,0xac3f],"Palette adjust or O12 read to 0x801f0000 differs")
	cd = 0; tick(); check(state.substep == 7 and not names().has("bg_read"),"ACF6 read started before O12 completed")
	cd = 1; tick(); check(state.substep == 8 and events[0] == ["bg_read",0xacf6,1,false],"ACF6 dec_bg_cd_read differs")
	decoded = -1; tick()
	check(state.mode_stage == 10 and state.substep == 8 and events.has(["bg_read",0xacf6,1,true]) and not names().has("next_scene"),"Failed ACF6 decode must reset and reread")
	decoded = 1; reject_setup = true
	check(not tick().error.is_empty() and state.mode_stage == 10,"Rejected O12 setup advanced the stage")
	reject_setup = false; tick()
	var order: Array = events.filter(func(e): return e[0] in ["next_scene","audio","bank"])
	check(state.mode_stage == 11 and state.substep == 0 and order == [["next_scene",0],["audio",5],["audio",0xc1],["bank","prologue_shiori"]],"Case8 setup/audio order differs: " + str(order))
	check(not names().has("advance_x") and names().has("background"),"Post-switch substep0 must skip the x move but draw")
	state.mode_stage = 10; state.substep = 9
	check(not tick().error.is_empty(),"Unsupported stage10 substep accepted")
	state.substep = 0; services.erase("rei_lines15_effect")
	check(not tick().error.is_empty(),"Stage10 ran without its O11 actor service")
	if failures == 0: print("PASS: %d original stage10 line15/16 read/decode/audio control-flow checks" % checks)
	quit(1 if failures else 0)
