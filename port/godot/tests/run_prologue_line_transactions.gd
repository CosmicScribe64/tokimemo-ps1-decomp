extends SceneTree
## Original resources + actual player lifecycle at an injected line-5 boundary.
## Unpaced logical steps and neutral fixture input are not a New Game replay.
const Lines = preload("res://services/prologue_line_services.gd")
const Store = preload("res://services/prologue_text_store.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const Bridge = preload("res://services/opening_audio_bridge.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
var checks := 0
var failures := 0
var reject_draw := false
var lines := Lines.new()
var bridge := Bridge.new()
var session := Session.new()
var state := State.new()

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ",label)

func tick(events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	return session.step(InputFrame.new(session.clock.logic_iterations),events)

func mode(current: State, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	if current.mode_step == 0:
		lines.scene_reset(current,result)
		lines.line_start(current,5,result)
		current.mode_step = 1
	else:
		lines.line_run(current,result)

func _initialize() -> void:
	call_deferred("run")

func run() -> void:
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	var audio_root := OS.get_environment("PROLOGUE_AUDIO_ROOT")
	var store := Store.new()
	var painter := Painter.new()
	var audio := Audio.new()
	check(store.open(text_root,FileAccess.get_sha256(text_root.path_join("manifest.json"))).is_empty(),"actual original text pack opens")
	check(painter.open_font(OS.get_environment("PROLOGUE_FONT_ROOT")).is_empty(),"actual original font pack opens")
	check(audio.configure_original_opening_audio_pack(audio_root,FileAccess.get_sha256(audio_root.path_join("opening-audio.json"))).is_empty(),"actual original PCM pack opens")
	check(lines.prepare(store,painter,audio).is_empty(),"source text/font/PCM preparation")
	if failures != 0:
		quit(1)
		return
	var player := AudioStreamPlayer.new()
	root.add_child(player)
	check(bridge.bind(audio,player).is_empty(),"exact v7 bridge binds player")
	state.game_mode = 0x15
	state.sound_active = true
	state.rule_state = {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],
		"girls":[{"birthday":12|(31<<4)|(3<<9)}]}
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE,1)
	session.router.bind_mode(0x15,mode)
	session.draw_logic = func(_current: State, _rng: Rng, result: Session.FrameResult) -> void:
		if reject_draw: result.error = "injected component draw failure"
	session.cd_logic = func(_current: State, _rng: Rng, _result: Session.FrameResult) -> void: pass
	session.audio_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = bridge.preflight(result,current)
	session.service_completion = func(event: Session.ServiceEvent, current: State, _rng: Rng, result: Session.FrameResult) -> void:
		AudioRules.complete(event,current,result)
	var before := session.state_hash()
	reject_draw = true
	var result := tick()
	check(not result.committed and session.state_hash() == before and not state.opening.has("prologue_text_work"),"failed start restores both numeric text and audio ownership")
	check(not player.playing and audio.voice_log.is_empty() and bridge.pending_events(0).is_empty(),"failed start emits no player or producer observation")
	check(not bridge.publish(result,0).is_empty(),"aborted start cannot publish")
	reject_draw = false
	result = tick()
	check(result.committed and not player.playing and audio.voice_log.is_empty(),"successful source start still has no effect before publication")
	check(bridge.publish(result,0).is_empty(),"source line intent publishes after commit")
	check(player.playing and audio.voice_log.size() == 1 and state.opening.audio.status == "requested","actual original player starts without changing logical ownership")
	var work_before_wait: Dictionary = state.opening.prologue_text_work.duplicate(true)
	await create_timer(0.1).timeout
	check(player.playing and player.get_playback_position() > 0.0,"actual original Yoshio PCM progresses")
	check(state.opening.prologue_text_work == work_before_wait and state.mode_step_timer == 0,"wall-clock audio cannot reveal text or advance source hold")
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(result.committed and state.opening.audio.status == "playing" and state.opening.prologue_text.iteration == 1,"queued start and source text step commit in ordered session phases")
	check(bridge.publish(result,1).is_empty(),"committed start observation acknowledged")
	await player.finished
	check(state.opening.audio.status == "playing" and bridge.pending_events(session.clock.logic_iterations).size() == 1,"actual finished signal only queues completion")
	before = session.state_hash()
	reject_draw = true
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(not result.committed and session.state_hash() == before,"rejected finished frame restores text progress, voice status, clock and RNG")
	check(bridge.pending_events(session.clock.logic_iterations).size() == 1 and not bridge.publish(result,session.clock.logic_iterations).is_empty(),"aborted frame preserves exact producer observation for retry")
	reject_draw = false
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(result.committed and state.opening.audio.status == "finished" and state.opening.prologue_text.k_disp_inc_result == 0 and state.mode_step_timer == 0,"actual speech completion cannot complete unrevealed source text")
	check(bridge.publish(result,session.clock.logic_iterations-1).is_empty() and bridge.pending_events(session.clock.logic_iterations).is_empty(),"successful retry alone acknowledges actual finish")
	var holds := 0
	var completed := false
	var loop_error := ""
	for _frame in range(12000):
		result = tick()
		if not result.committed:
			loop_error = result.error
			break
		loop_error = bridge.publish(result,session.clock.logic_iterations-1)
		if not loop_error.is_empty(): break
		if state.opening.prologue_text.k_disp_inc_result == 1: holds += 1
		if state.substep == 1:
			completed = true
			break
	check(loop_error.is_empty() and completed and holds == 61,"source reveal/scroll then exactly 61 logical completion iterations")
	check(state.opening.prologue_text_work.pos == state.opening.prologue_text_work.total and state.mode_step_timer == 0,"numeric source text fully revealed before line transition")
	check(audio.voice_log.size() == 1 and audio.voice_log[0].voice_id == 5,"retry never duplicates actual original playback")
	bridge.reset_session()
	check(not player.playing and bridge.pending_events(0).is_empty(),"explicit fresh-session reset clears playback and observations")
	player.queue_free()
	await process_frame
	check(not is_instance_valid(player),"player teardown completes")
	print("PASS: %d original text/audio transaction checks" % checks if failures == 0 else "FAIL: %d/%d transaction checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
