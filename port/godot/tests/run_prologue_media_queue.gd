extends SceneTree
## Invented resource identities; these tests establish no original-scene parity.
const Queue = preload("res://services/prologue_media_queue.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failures := 0
func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr(label)
func completion(request: Dictionary, status := "ready", phase := Session.Phase.BEFORE_MODE) -> Session.ServiceEvent:
	return Session.ServiceEvent.new(0,phase,Session.ServiceKind.COMPLETION,Queue.SERVICE,
		{"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,"resource_id":request.resource_id,"status":status})
func _initialize() -> void:
	var queue := Queue.new()
	check(queue.prepare({"17":{"id":"invented.bg","sha256":"12".repeat(32),"slot":1,"decoded":true}},
		{"23":{"id":"invented.raw","sha256":"34".repeat(32),"sectors":13,"destination":0x80180000,"native_composition":false}},{}).is_empty(), "partial prepared resources remain explicit")
	var state := State.new()
	var result := Session.FrameResult.new()
	queue.background_read(state,17,1,result)
	check(result.error.is_empty() and result.presentation.size() == 1 and queue.cd_sync(state,result) == 0, "issuing read never implies native completion")
	var request: Dictionary = result.presentation[0].request
	var wrong_phase := Session.FrameResult.new()
	queue.complete(completion(request,"ready",Session.Phase.CD),state,null,wrong_phase)
	check(not wrong_phase.error.is_empty() and queue.cd_sync(state,result) == 0, "completion cannot run in host-selected phase")
	var wrong_key := completion(request)
	wrong_key.payload.request_key = "stale"
	var rejected := Session.FrameResult.new()
	queue.complete(wrong_key,state,null,rejected)
	check(not rejected.error.is_empty() and queue.cd_sync(state,result) == 0, "request identity mismatch cannot release CD wait")
	queue.complete(completion(request),state,null,result)
	check(result.error.is_empty() and queue.cd_sync(state,result) == 1, "exact ordered completion releases wait")
	check(queue.background_decode(state,1,result) == 1 and state.opening.media.decoded["1"].buffer == 1, "first native opposite-buffer decode selects buffer1")
	var duplicate := Session.FrameResult.new()
	queue.complete(completion(request),state,null,duplicate)
	check(not duplicate.error.is_empty(), "duplicate completion rejected")
	var before: Dictionary = state.projection()
	var actors := Session.FrameResult.new()
	queue.actor_read(state,13,0x80180000,23,actors)
	check(not actors.error.is_empty() and state.projection() == before, "raw actor bytes never authorize unsupported frame composition")
	var repeat := Session.FrameResult.new()
	queue.background_read(state,17,1,repeat)
	check(not repeat.error.is_empty() and state.projection() == before, "unimplemented native cache hit cannot become invented read wait")
	queue.finish_cd_reset(state,result)
	check(state.opening.media.epoch == 1 and state.opening.media.requests.is_empty(), "CD reset increments epoch and cancels pending logical jobs")
	var stale := Session.FrameResult.new()
	queue.complete(completion(request),state,null,stale)
	check(not stale.error.is_empty(), "completion from previous epoch rejected")
	var retry_state := State.new()
	var retry := Session.FrameResult.new()
	queue.background_read(retry_state,17,1,retry)
	var retry_request: Dictionary = retry.presentation[0].request
	queue.complete(completion(retry_request,"retry"),retry_state,null,retry)
	check(queue.background_decode(retry_state,1,retry) == -1 and retry.error.is_empty(), "native decode failure requests source retry rather than advancing")
	queue.background_read(retry_state,17,1,retry,true)
	check(queue.cd_sync(retry_state,retry) == 0 and retry_state.opening.media.requests.size() == 2, "retry has a new explicit request identity")
	print("PASS: %d opening media queue checks" % checks if failures == 0 else "FAIL: %d opening media queue checks" % failures)
	quit(0 if failures == 0 else 1)
