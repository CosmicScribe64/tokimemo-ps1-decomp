extends SceneTree
## Actual media publication/completion -> genuine HUDv2 producer -> GPU word
## pump. Dispatch and cold-route observations are fixtures, not a normal start.
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Cold = preload("res://flow/cold_no_card_scratch.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Producer = preload("res://presentation/prologue_actor_upload_provider.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
var checks := 0
var failures := 0
var media := Media.new()
var upload := Upload.new()
var session := Session.new()
var state := State.new()
var owned: Dictionary
var reject_audio := false

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func tick() -> Session.FrameResult:
	return session.step(InputFrame.new(session.clock.logic_iterations),media.pending_events(session.clock.logic_iterations))

func runtime_hash() -> String:
	return Upload._runtime_hash(owned)

func mode(current: State, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	match current.opening.get("component_phase",0):
		0:
			media.scene_reset(current,result)
			media.queue.background_read(current,0x494b,1,result)
			current.opening.component_phase = 1
		1:
			if media.queue.cd_sync(current,result) == 1:
				media.queue.background_decode(current,1,result)
				current.substep = 3
				media.queue.actor_read(current,13,0x80180000,0xabab,result)
				current.opening.component_phase = 2
		2:
			if media.queue.cd_sync(current,result) == 1:
				current.substep = 4
				media.actor_palette_setup(current,result)
				current.opening.component_phase = 3

func _initialize() -> void:
	var store := Store.new()
	check(store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")).is_empty(),"authentic media packet opens")
	check(media.prepare(store).is_empty(),"actual native read bridge prepares")
	var route := {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false}
	var initial := Cold.initialize(route)
	check(initial.error.is_empty(),"explicit source cold route initializes scratch")
	var vram := PackedByteArray(); vram.resize(Pump.VRAM_BYTES); vram.fill(0xa5)
	owned = {"scratch_owner":"source-cold-route-test-scratch","vram_owner":"explicit-test-vram",
		"scratch_words":initial.scratch_words,"vram_words":vram,"pending_gpu_effects":[]}
	check(not Upload.new().prepare(media,store,{}).is_empty(),"missing actual owner buffers rejected")
	check(upload.prepare(media,store,owned).is_empty(),"source producer and pump prepare against actual media owner")
	state.game_mode = 0x15; state.mode_step = 1; state.substep = 1
	state.sound_active = true # Explicitly exercise the later audio-failure phase.
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE,1)
	session.router.bind_mode(0x15,mode)
	session.service_completion = media.complete
	session.draw_logic = func(_state: State, _rng: Rng, _result: Session.FrameResult) -> void: pass
	session.cd_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void: result.error = upload.preflight(result,current)
	session.audio_logic = func(_state: State, _rng: Rng, result: Session.FrameResult) -> void:
		if reject_audio: result.error = "injected failure after upload preflight"
	check(upload.bind_session(session).is_empty(),"both adapters bind the exact Session")
	check(not upload.bind_session(Session.new()).is_empty(),"upload Session cannot be rebound")
	check(not media.completed_actor_buffer(state).error.is_empty(),"no completed actor bytes before an actual read")
	var before := runtime_hash()
	var result := tick()
	check(result.committed and runtime_hash() == before,"background request commit does not execute GPU or CPU writes")
	check(not upload.publish(result,0).is_empty() and runtime_hash() == before,"upload publication requires actual media publication first")
	check(media.publish(result,0).is_empty() and upload.publish(result,0).is_empty(),"background native publication precedes empty upload publication")
	result = tick()
	check(result.committed and media.publish(result,1).is_empty() and upload.publish(result,1).is_empty(),"actual background completion leads to published13-sector actor transfer")
	check(not media.completed_actor_buffer(state).error.is_empty(),"published actor bytes remain unavailable before logical completion")
	before = runtime_hash()
	var state_before := session.state_hash()
	reject_audio = true
	result = tick()
	check(not result.committed and session.state_hash() == state_before and runtime_hash() == before and media.pending_events(2).size() == 1,"failed frame after real producer/pump preview rolls back model and retains buffers/observation")
	check(not upload.publish(result,2).is_empty() and runtime_hash() == before,"rejected frame cannot execute actual producer or pump")
	reject_audio = false
	result = tick()
	check(result.committed and runtime_hash() == before,"successful setup preflight leaves actual buffers untouched: "+result.error)
	if not result.committed:
		quit(1)
		return
	var actual := media.completed_actor_buffer(state)
	check(actual.error.is_empty() and actual.bytes.size() == 26624 and actual.sha256 == Store.ACTOR_SHA,"actual completion exposes real published actor bytes")
	if not actual.error.is_empty():
		quit(1)
		return
	actual.bytes[0] ^= 1
	check(Store._sha(media.completed_actor_buffer(state).bytes) == Store.ACTOR_SHA,"completed buffer accessor returns detached bytes")
	check(not media.completed_actor_buffer(State.new()).error.is_empty(),"foreign logical state cannot access a completed actor buffer")
	var request_id: String = state.opening.media.actor_pending[str(0x80180000)]
	var original_request: Dictionary = state.opening.media.requests[request_id].duplicate(true)
	for malformed: Variant in [{},[],null,false,1.0,"1",-1]:
		state.opening.media.requests[request_id].epoch = malformed
		var malformed_before := session.state_hash()
		check(not media.completed_actor_buffer(state).error.is_empty() and session.state_hash() == malformed_before and runtime_hash() == before,"malformed completed request rejects without mutation")
	state.opening.media.requests[request_id] = original_request
	var producer := Producer.new()
	var queued := owned.duplicate(true)
	var plan := producer.preflight(media.completed_actor_buffer(state).bytes,queued)
	check(producer.execute(plan,media.completed_actor_buffer(state).bytes,queued).status == "effects_enqueued","extent negatives use actual source-produced queue")
	for malformed: Variant in [{},[],null,false,512.0,511,513]:
		for field: String in ["source_extent","transfer_extent"]:
			var bad := queued.duplicate(true)
			if field == "source_extent": bad.pending_gpu_effects[17].source.size = malformed
			else: bad.pending_gpu_effects[17]["size"] = malformed
			var bad_before := Upload._runtime_hash(bad)
			check(not upload._pump_extents(bad).is_empty() and Upload._runtime_hash(bad) == bad_before and runtime_hash() == before,"malformed/conflicting gray extent rejects without local or live writes: "+field)
	check(not upload.publish(result,2).is_empty() and runtime_hash() == before,"CPU and GPU producer cannot precede the committed media publisher")
	check(media.publish(result,2).is_empty(),"actual complete19 operation batch publishes")
	var preserved: Array[Dictionary] = result.presentation.duplicate(true)
	result.presentation.clear()
	check(not upload.publish(result,2).is_empty() and runtime_hash() == before,"dropping approved operations after commit cannot execute or consume upload batch")
	result.presentation.assign(preserved)
	owned.scratch_words[0] ^= 1
	check(not upload.publish(result,2).is_empty() and upload.diagnostics().batch_key.is_empty(),"changed owned scratch invalidates preflight before producer effects")
	owned.scratch_words[0] ^= 1
	check(upload.publish(result,2).is_empty(),"actual source producer CPU/queue then18 GPU word transfers execute after commit")
	var key: String = state.opening.arrival_media.upload_batch_key
	check(upload.batch_ready(key).is_empty() and upload.diagnostics().batch_complete and not upload.diagnostics().vram_ready and owned.pending_gpu_effects.is_empty(),"live readback proves only the actual completed actor batch")
	check(owned.vram_known_mask.count(255) == 25088 and owned.vram_words[0] == 0xa5,"actual producer/pump know only their exact transfer regions")
	check(Pump._region(owned.vram_words,Rect2i(640,256,64,128)) == store.upload_source("actor96.tpage").bytes,"actual producer uses native tpage destination")
	check(Pump._region(owned.vram_words,Rect2i(0,511,256,1)) == owned.actor_runtime_bytes.slice(0x6290,0x6490),"already-sliced final palette reaches native CLUT row without a second row offset")
	check(Pump._region(owned.vram_words,Rect2i(0,493,256,1)) == owned.scratch_words and Store._sha(owned.scratch_words) != Store._sha(initial.scratch_words),"actual grayscale CPU result and GPU STP writeback own the gray CLUT")
	before = runtime_hash()
	check(not upload.publish(result,2).is_empty() and runtime_hash() == before,"actual committed native upload cannot be replayed")
	owned.vram_words[(256*1024+640)*2] ^= 1
	check(not upload.batch_ready(key).is_empty(),"live destination mutation invalidates actor batch readiness")
	print("PASS: %d first-arrival actual upload checks" % checks if failures == 0 else "FAIL: %d/%d actual upload checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
