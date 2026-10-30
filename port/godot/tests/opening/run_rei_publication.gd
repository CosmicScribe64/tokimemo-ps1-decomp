extends SceneTree
## Exercise exact same-stack publication admission with a counting native sink.
const Bridge = preload("res://services/first_arrival_bgm_bridge.gd")
const Audio = preload("res://services/audio_service.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
class Backend extends RefCounted:
	var calls := 0
	func rei_action(_owner: Dictionary,_action: String,_inputs: Array = [],_xa: bool = false) -> String:
		calls += 1
		return ""
var failures := 0
func check(ok: bool, label: String) -> void:
	if not ok: failures += 1; printerr(label)
func _initialize() -> void:
	var audio := Audio.new()
	var state := State.new(); state.mode_stage = 7; state.substep = 5
	state.opening.audio_state_flags = 0x100000
	state.opening.bgm_fade = {"active":true,"ticks":1,"gain":83}
	var session := Session.new(); session.state = state
	var bridge := Bridge.new(); bridge._audio = weakref(audio); bridge._session = weakref(session)
	var backend := Backend.new(); bridge._backend = backend
	bridge._committed_after = {"test":true}
	var result := Session.FrameResult.new()
	result.audio.append({"kind":"native_bgm_fade","command":0xb1})
	bridge.final_precommit(state,session.rng,result)
	check(result.error.is_empty(),"Valid source fade failed final barrier")
	session.clock.logic_iterations = 1; result.committed = true; result.state_hash = session.state_hash()
	result.audio[0].command = 0xc1
	check(not bridge.publish(result,0).is_empty() and backend.calls == 0,"Altered committed intent reached native engine")
	result.audio[0].command = 0xb1
	check(not bridge.publish(result,1).is_empty() and backend.calls == 0,"Stale publication tick reached native engine")
	var replacement := Session.FrameResult.new(); replacement.audio = result.audio.duplicate(true); replacement.committed = true; replacement.state_hash = result.state_hash
	check(not bridge.publish(replacement,0).is_empty() and backend.calls == 0,"Different committed result reached native engine")
	check(bridge.publish(result,0).is_empty() and backend.calls == 1,"Validated same-stack result did not publish exactly once")
	check(not bridge.publish(result,0).is_empty() and backend.calls == 1,"Replayed result reached native engine")
	var duplicate := Session.FrameResult.new(); duplicate.audio = result.audio.duplicate(true); duplicate.audio.append(result.audio[0].duplicate())
	bridge.final_precommit(state,session.rng,duplicate)
	check(not duplicate.error.is_empty() and backend.calls == 1,"Duplicate native action passed final validation")
	var pack_root := OS.get_environment("REI_BANK_PACK")
	if not pack_root.is_empty():
		var prepared = load("res://services/rei_bank_resources.gd").prepare(pack_root)
		check(prepared.error.is_empty(),"Original Rei preparation failed")
		audio.prepared_bank_roles.prologue_rei = prepared
		bridge._owner = {"epoch":1,"role":"prologue_arrival"}
		var loading := Session.FrameResult.new()
		loading.audio.append({"kind":"bank_load","role":"prologue_rei","bank_epoch":2})
		bridge.final_precommit(state,session.rng,loading)
		check(loading.error.is_empty(),"Authenticated Rei load failed final validation")
		session.clock.logic_iterations = 2; loading.committed = true; loading.state_hash = session.state_hash()
		audio.prepared_bank_roles.prologue_rei.inputs.music_body[0] ^= 1
		check(not bridge.publish(loading,1).is_empty() and backend.calls == 1,"Changed prepared input crossed final barrier into native engine")
	if failures == 0: print("PASS: native Rei publication rejects altered, stale, replacement, duplicate and replayed frames")
	quit(1 if failures else 0)
