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
	check(result.error.is_empty() and queue.cd_sync(state,result) == 0, "landed bytes alone do not release the source read latency")
	state.frame_count = 17
	check(queue.cd_sync(state,result) == 0 and Queue.cd_wait_frames(state) == 1, "background read waits floor(14 + 0.4 * 10) vsyncs")
	state.frame_count = 18
	check(result.error.is_empty() and queue.cd_sync(state,result) == 1, "exact ordered completion releases wait on its scheduled logic frame")
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
	_schedule_checks()
	print("PASS: %d opening media queue checks" % checks if failures == 0 else "FAIL: %d opening media queue checks" % failures)
	quit(0 if failures == 0 else 1)

## Bounded read-latency schedule: source model, measured table, serialization,
## wrap-around and negatives. Measured values: .asset-cache/emu/opening-timing-v2.
func _schedule_checks() -> void:
	for pair: Array in [[49,33],[25,24],[17,20],[33,27],[65,40],[13,19],[10,18],[9,17],[1,14]]:
		check(Queue.model_read_vsyncs(pair[0]) == pair[1], "model floor(14 + 0.4 * %d) = %d" % pair)
	for bad: int in [0,-1,256]:
		check(Queue.model_read_vsyncs(bad) == -1, "model refuses %d sectors" % bad)
	var exceptions := 0
	for key: String in Queue.MEASURED_READ_VSYNCS:
		var lba := key.get_slice(":",1)
		var sectors: int = Queue.MEASURED_READ_SECTORS[lba]
		var measured: int = Queue.MEASURED_READ_VSYNCS[key]
		check(Queue.read_vsyncs(0x15,int(key.get_slice(":",0)),lba.hex_to_int(),sectors) == measured, "measured read %s is authoritative" % key)
		var delta: int = measured - Queue.model_read_vsyncs(sectors)
		check(absi(delta) <= 1, "measured read %s stays within one vsync of the source model" % key)
		if delta != 0: exceptions += 1
	check(Queue.MEASURED_READ_VSYNCS.size() == 32 and exceptions == 2, "32 opening reads, exactly two sub-vsync model exceptions")
	check(Queue.read_vsyncs(0x15,2,0xacdd,10) == 18 and Queue.model_read_vsyncs(10) == 18, "stage2 ACDD waits 18 iterations (request issued one vsync late by the overrunning 2/0/7 frame)")
	check(Queue.read_vsyncs(0x15,3,0xabb8,9) == 18 and Queue.model_read_vsyncs(9) == 17, "stage3 ABB8 uses its measured 18, not the model 17")
	check(Queue.read_vsyncs(0x15,3,0xabb8,9,true) == 17, "a retry is never taken from the measured table")
	check(Queue.read_vsyncs(0x15,3,0xabb8,13) == Queue.model_read_vsyncs(13), "a different extent is never taken from the measured table")
	check(Queue.read_vsyncs(0x14,3,0xabb8,9) == 17 and Queue.read_vsyncs(0x15,12,0xabb8,9) == 17, "other modes and unmeasured stages use the model")
	var queue := Queue.new()
	queue.prepare({"17":{"id":"invented.bg","sha256":"12".repeat(32),"slot":1,"decoded":true},"18":{"id":"invented.bg2","sha256":"56".repeat(32),"slot":1,"decoded":true}},{},{})
	# Two reads queued in one frame: the ring serializes them (18 + 18).
	var state := State.new()
	state.frame_count = 100
	var result := Session.FrameResult.new()
	queue.background_read(state,17,1,result)
	queue.background_read(state,18,1,result)
	check(result.error.is_empty() and state.opening.media.cd_ready_frame == 136, "second read in flight starts after the first completes")
	for intent: Dictionary in result.presentation:
		queue.complete(completion(intent.request),state,null,result)
	state.frame_count = 135
	check(queue.cd_sync(state,result) == 0, "serialized reads not ready one frame early")
	state.frame_count = 136
	check(queue.cd_sync(state,result) == 1 and result.error.is_empty(), "serialized reads ready on their scheduled frame")
	# Deadline passed but the authenticated bytes have not landed.
	var late := State.new()
	var late_result := Session.FrameResult.new()
	queue.background_read(late,17,1,late_result)
	late.frame_count = 1000
	check(queue.cd_sync(late,late_result) == 0, "elapsed latency never substitutes for the native completion")
	# u32 frame_count wrap.
	var wrap := State.new()
	wrap.frame_count = 0xfffffff0
	var wrap_result := Session.FrameResult.new()
	queue.background_read(wrap,17,1,wrap_result)
	queue.complete(completion(wrap_result.presentation[0].request),wrap,null,wrap_result)
	check(wrap.opening.media.cd_ready_frame == 2, "deadline wraps as a u32 frame counter")
	wrap.frame_count = 0xffffffff
	check(queue.cd_sync(wrap,wrap_result) == 0 and Queue.cd_wait_frames(wrap) == 3, "wrapped deadline still pending")
	wrap.frame_count = 2
	check(queue.cd_sync(wrap,wrap_result) == 1, "wrapped deadline releases on schedule")
	# A CD reset cancels the schedule with the requests.
	var reset := State.new()
	var reset_result := Session.FrameResult.new()
	queue.background_read(reset,17,1,reset_result)
	queue.finish_cd_reset(reset,reset_result)
	check(not reset.opening.media.has("cd_ready_frame") and queue.cd_sync(reset,reset_result) == 1, "CD reset clears the in-flight schedule")
	# Determinism: identical inputs give identical state words.
	var a := State.new()
	var b := State.new()
	for s: State in [a,b]:
		s.frame_count = 7
		var r := Session.FrameResult.new()
		queue.background_read(s,17,1,r)
	check(a.to_words() == b.to_words(), "schedule is a pure function of logic frames")
