extends SceneTree
## Source control flow only; actor/media callbacks are explicit test doubles.
const Flow = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
var state := State.new()
var rng := Rng.new()
var services: Dictionary
var events: Array = []
var voice := 0
var cd := 0
var boundary: Variant = false
var actor99_done: Variant = false
var complete_line := false
var reject_setup := false
var failures := 0
var checks := 0
func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr(label)
func effect(_state, kind, value, result) -> Variant:
	events.append([kind,value])
	if kind == "page_boundary": return boundary
	if kind == "exit_done": return actor99_done
	if kind == "next_scene" and reject_setup: result.error = "rejected actual O11 setup"
	return null
func tick() -> Session.FrameResult:
	var result := Session.FrameResult.new()
	Flow._stage_09_rei_lines(state,rng,services,result)
	return result
func _initialize() -> void:
	rng.configure(Rng.OPENBIOS_PROFILE,19950404)
	state.game_mode = 0x15; state.mode_stage = 9; state.mode_step = 0
	services = {"rei_lines_effect":effect,
		"line_start":func(_s,line,_out): events.append(["line",line]),
		"line_run":func(s,_out):
			events.append(["run",s.substep])
			if complete_line: s.substep += 1,
		"arrival_draw":func(_s,slot,_out): events.append(["background",slot]),
		"outer_draw":func(_s,arg,_out): events.append(["outer",arg]),
		"cd_audio_state":func(_s): return voice,
		"cd_sync":func(_s,_out): return cd,
		"actor_read":func(_s,count,destination,lba,_out): events.append(["read",count,destination,lba])}
	check(tick().error.is_empty() and state.substep == 1 and state.opening.prologue_scene_counter == 0,"Line13 initialization differs")
	check(events[0] == ["line",13] and events[1] == ["background",1] and rng.draw_count == 1,"Line13 initialization draw/RNG order differs")
	tick(); check(state.substep == 1 and state.opening.prologue_scene_counter == 1,"CD wait advanced before real playback")
	voice = 4; tick(); check(state.substep == 2 and state.opening.prologue_scene_counter == 2,"CD start did not advance original wait")
	tick(); check(state.substep == 2,"Text did not wait for actual page boundary")
	boundary = null
	check(not tick().error.is_empty(),"Unknown page boundary was accepted")
	boundary = true; tick()
	check(state.substep == 3 and events.has(["page_transition",0]),"Actual page boundary did not reveal source layers")
	complete_line = true
	state.opening.prologue_scene_counter = 12; tick()
	check(state.substep == 4 and state.opening.prologue_scene_counter == 8,"Line13 completion or flash decrement differs")
	state.opening.actor98 = {"flags":1}
	var before := rng.draw_count
	tick(); check(state.substep == 5 and rng.draw_count == before+1,"4-to5 must consume RNG even when blink actor is already active")
	before = rng.draw_count
	tick(); check(state.substep == 5 and rng.draw_count == before+1,"Running actor99 must retain source animation wait and RNG")
	actor99_done = null
	check(not tick().error.is_empty(),"Unknown actor completion was accepted")
	actor99_done = true; before = rng.draw_count
	state.opening.actor100 = {"flags":5}
	tick(); check(state.substep == 6 and rng.draw_count == before,"5-to6 must not consume RNG or wait for actor100")
	for index in range(31): tick()
	check(state.substep == 6 and state.opening.prologue_scene_counter == 31,"Postincrement hold ended before32 iterations")
	tick(); check(state.substep == 7 and state.opening.prologue_scene_counter == 32,"32nd postincrement hold did not advance")
	tick(); check(state.substep == 8 and events.has(["line14_setup",0]) and events.has(["line",14]),"Line14 actor setup/start differs")
	tick(); check(state.substep == 9 and events.has(["line14_hold",0]),"Line14 hold/completion differs")
	tick(); check(state.substep == 10 and events.has(["read",49,0x801a4000,0xaa85]),"Stage9 CUT08 read differs from source")
	tick(); check(state.substep == 10,"Pending CUT08 prematurely issued O11 read")
	cd = 1; tick(); check(state.substep == 11 and events.has(["read",33,0x801e8000,0xac1e]),"Stage9 O11 backup read extent/destination differs")
	reject_setup = true; before = rng.draw_count
	check(not tick().error.is_empty() and state.mode_stage == 9 and rng.draw_count == before,"Failed O11 setup advanced scene or consumed RNG")
	reject_setup = false; tick()
	check(state.mode_stage == 10 and state.substep == 0 and rng.draw_count == before+1,"Source next_stage reset must precede trailing RNG predicate")
	state.mode_stage = 9; state.substep = 12
	check(not tick().error.is_empty(),"Unsupported source substep accepted")
	if failures == 0: print("PASS: %d original stage9 dialogue/page/animation/CD/RNG control-flow checks" % checks)
	quit(1 if failures else 0)
