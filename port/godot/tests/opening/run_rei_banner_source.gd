extends SceneTree
## Synthetic services test source control flow; no presentation acceptance.
const Arrival = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var state := State.new()
var effects: Array = []
var reads: Array = []
var lines: Array = []
var cd_status := 0
var voice_status := 0
var failure := ""
var checks := 0
var failures := 0
var services: Dictionary

func check(value: bool, message: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr(message)

func tick() -> Session.FrameResult:
	var result := Session.FrameResult.new()
	Arrival._stage_08_rei_banner(state, services, result)
	return result

func _initialize() -> void:
	state.game_mode = 0x15; state.mode_stage = 8; state.mode_step = 0; state.substep = 0
	state.opening = {"prologue_scene_counter":0,"actor96":{"brightness":0}}
	services = {
		"rei_banner_effect":func(_s, kind, value, out): effects.append([kind,value]); out.error = failure,
		"line_start":func(_s, line, _out): lines.append(line),
		"line_run":func(s, _out): s.substep += 1,
		"cd_sync":func(_s, _out): return cd_status,
		"cd_audio_state":func(_s): return voice_status,
		"actor_read":func(_s, count, destination, lba, _out): reads.append([count,destination,lba]),
		"arrival_draw":func(_s, _slot, _out): pass,
		"outer_draw":func(_s, _argument, _out): pass,
	}
	for i in 63: check(tick().error.is_empty(),"source flash frame failed")
	check(state.substep == 0 and state.opening.prologue_scene_counter == 252 and state.opening.actor96.brightness == 63,"flash must retain63 frames before256 boundary")
	check(effects[0] == ["flash",0] and effects[-1] == ["flash",248],"flash draws pre-increment color")
	tick()
	check(state.substep == 1 and state.opening.prologue_scene_counter == 0 and state.opening.actor96.brightness == 63,"64th flash frame resets counter without extra tint")
	for i in 11: tick()
	check(state.substep == 1 and state.opening.prologue_scene_counter == 11,"white hold uses original post-increment comparison")
	tick()
	check(state.substep == 2 and state.opening.prologue_scene_counter == 0 and effects[-1][0] == "color_cut","12th white frame selects color cut")
	for i in 16: tick()
	check(state.substep == 2 and state.opening.prologue_scene_counter == 128 and state.opening.actor96.brightness == 120 and lines.is_empty(),"fade samples tint before adding8")
	tick()
	check(state.substep == 3 and state.opening.actor96.brightness == 128 and lines == [12],"17th fade starts source line12")
	tick(); check(state.substep == 3,"unstarted voice must retain source wait")
	voice_status = 4
	tick(); check(state.substep == 4,"native XA playing bit releases source wait")
	tick(); check(state.substep == 5 and effects[-1][0] == "lip_sync","lip sync follows line run even when line advances")
	tick(); check(state.substep == 6 and reads == [[49,0x801a4000,0xaae7]],"first read is49sectors at cut destination")
	tick(); check(state.substep == 6 and reads.size() == 1,"pending CD must not issue actor read")
	cd_status = 1
	tick(); check(state.substep == 7 and reads[1] == [25,0x80180000,0xac05],"second read follows clear and uses25sectors")
	failure = "injected presentation failure"
	check(tick().error == failure and state.mode_stage == 8,"failed source effects cannot advance scene")
	failure = ""
	tick(); check(state.mode_stage == 9 and effects[-1][0] == "next_scene","source setup precedes next stage")
	print("PASS: %d Rei banner source-flow checks" % checks if failures == 0 else "FAIL: Rei banner source flow")
	quit(0 if failures == 0 else 1)
