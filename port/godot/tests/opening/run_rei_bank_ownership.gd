extends SceneTree
## Synthetic ownership admission only; native playback has a separate test.
const Bridge = preload("res://services/first_arrival_bgm_bridge.gd")
const Audio = preload("res://services/audio_service.gd")
const State = preload("res://rules/new_game_route_state.gd")
class Backend extends RefCounted:
	func pump(_owner: Dictionary) -> String: return ""
var failures := 0
func check(ok: bool, label: String) -> void:
	if not ok: failures += 1; printerr(label)
func _initialize() -> void:
	var audio := Audio.new()
	var state := State.new()
	var bridge := Bridge.new()
	bridge._audio = weakref(audio)
	bridge._backend = Backend.new()
	bridge._owner = {"role":"prologue_arrival","epoch":1}
	bridge._committed_after = {"bounded_test_context":true}
	audio.native_audio_context = bridge._committed_after.duplicate()
	state.opening.native_audio_context = bridge._committed_after.duplicate()
	audio.active_bank_role = "prologue_arrival"
	audio.bank_ready.prologue_arrival = true
	audio.vab_memory.banks = {0:{"identity":"original SE"},1:{"identity":"original arrival"}}
	bridge._retained_banks = audio.vab_memory.banks.duplicate(true)
	state.opening.audio_bank = {"role":"prologue_arrival","epoch":1,"ready":true}
	check(bridge.pump(state).is_empty(),"Original active owner rejected")
	state.opening.audio_banks = {"prologue_arrival":state.opening.audio_bank.duplicate()}
	state.opening.audio_bank = {"role":"prologue_rei","epoch":2,"ready":false}
	state.opening.prologue_bgm_transition = {"role":"prologue_rei","bank_epoch":2}
	check(bridge.pump(state).is_empty(),"Real alternate pending bank invalidated retained arrival playback")
	state.opening.audio_bank.epoch = 3
	check(not bridge.pump(state).is_empty(),"Unrelated pending epoch accepted")
	state.opening.audio_bank.epoch = 2
	state.opening.audio_bank.role = "unrelated"
	check(not bridge.pump(state).is_empty(),"Unrelated pending role accepted")
	state.opening.audio_bank.role = "prologue_rei"
	audio.vab_memory.banks[1].identity = "substituted"
	check(not bridge.pump(state).is_empty(),"Changed retained arrival bank accepted")
	audio.vab_memory.banks = bridge._retained_banks.duplicate(true)
	bridge._rei_epoch = 2; audio.active_bank_role = "prologue_rei"
	check(bridge.pump(state).is_empty(),"Committed alternate publication invalidated retained bank")
	state.opening.audio_banks.prologue_arrival.epoch = 9
	check(not bridge.pump(state).is_empty(),"Retained owner epoch tamper accepted")
	if failures == 0: print("PASS: pending/active bank separation with stale-role/epoch/retained-slot rejection")
	quit(1 if failures else 0)
