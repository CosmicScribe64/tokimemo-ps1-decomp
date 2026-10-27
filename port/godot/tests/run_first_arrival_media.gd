extends SceneTree
## Real prepared transfer bytes at an injected media boundary, not an app route.
const Store = preload("res://services/first_arrival_media_store.gd")
const Bridge = preload("res://services/first_arrival_media_bridge.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
var checks := 0
var failures := 0
var bridge := Bridge.new()
var session := Session.new()
var state := State.new()
var reject_draw := false

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ",label)

func tick(events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	return session.step(InputFrame.new(session.clock.logic_iterations),events)

func mode(current: State, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	match current.opening.get("component_phase",0):
		0:
			bridge.scene_reset(current,result)
			bridge.queue.background_read(current,0x494b,1,result)
			current.opening.component_phase = 1
		1:
			if bridge.queue.cd_sync(current,result) == 1:
				bridge.queue.background_decode(current,1,result)
				current.substep = 3
				bridge.queue.actor_read(current,13,0x80180000,0xabab,result)
				current.opening.component_phase = 2
		2:
			if bridge.queue.cd_sync(current,result) == 1:
				current.substep = 4
				bridge.actor_palette_setup(current,result)
				current.opening.component_phase = 3

func _initialize() -> void:
	var root := OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT")
	var expected := OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")
	var store := Store.new()
	check(not store.open(root,"0".repeat(64)).is_empty(),"unapproved manifest rejected")
	var opened := store.open(root,expected)
	check(opened.is_empty(),"exact native source transfer pack opens: "+opened)
	if failures != 0:
		quit(1)
		return
	check(not store.open(root,expected).is_empty(),"prepared source store is immutable")
	var native_bg := store.transfer(Store.BG_ID)
	check(native_bg.bytes.size() == 20480 and Store._sha(native_bg.bytes.slice(0,18432)) == Store.BG_SHA and Store._sha(native_bg.bytes.slice(18432)) == Store.BG_ADJACENT_SHA,"ten native sectors retain the adjacent file sector outside the decoder extent")
	native_bg.bytes[0] ^= 1
	check(Store._sha(store.transfer(Store.BG_ID).bytes) == Store.BG_TRANSFER_SHA,"returned transfer cannot mutate authenticated bytes")
	var image := store.background_image()
	var before_image := image.get_data()
	image.fill(Color.MAGENTA)
	check(store.background_image().get_data() == before_image,"returned image cannot mutate prepared pixels")
	check(bridge.prepare(store).is_empty(),"finite media bridge prepares")
	state.game_mode = 0x15
	state.mode_step = 1
	state.substep = 1
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE,1)
	check(bridge.bind_session(session).is_empty(),"media publication binds the actual logical Session")
	session.router.bind_mode(0x15,mode)
	session.service_completion = bridge.complete
	session.draw_logic = func(_state: State, _rng: Rng, result: Session.FrameResult) -> void:
		if reject_draw: result.error = "injected media draw failure"
	session.cd_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = bridge.preflight(result,current)
	session.audio_logic = func(_state: State, _rng: Rng, _result: Session.FrameResult) -> void: pass
	var before := session.state_hash()
	reject_draw = true
	var result := tick()
	check(not result.committed and session.state_hash() == before and bridge.diagnostics().buffers.is_empty(),"failed request frame has no model or native buffer effect")
	check(not bridge.publish(result,0).is_empty() and bridge.pending_events(0).is_empty(),"failed frame cannot emit a CD ready observation")
	reject_draw = false
	result = tick()
	check(result.committed and bridge.diagnostics().buffers.is_empty() and bridge.queue.cd_sync(state,result) == 0,"committed pending read is not ready before real transfer publication")
	var source_intents: Array[Dictionary] = result.presentation.duplicate(true)
	var native_request: Dictionary = result.presentation[-1].request
	native_request.epoch += 1
	native_request.request_key = JSON.stringify([native_request.epoch,native_request.request_id,native_request.kind,native_request.payload,native_request.resource_id,native_request.resource_sha256]).sha256_text()
	check(not bridge.publish(result,0).is_empty() and bridge.diagnostics().buffers.is_empty() and bridge.pending_events(0).is_empty(),"self-consistent foreign owner mutation after preflight cannot publish")
	result.presentation.assign(source_intents)
	result.presentation.clear()
	check(not bridge.publish(result,0).is_empty() and bridge.diagnostics().buffers.is_empty() and bridge.pending_events(0).is_empty(),"dropping every preflight-approved intent cannot consume publication without the read")
	result.presentation.assign(source_intents)
	check(bridge.publish(result,0).is_empty(),"committed background read publishes real prepared bytes")
	var diagnostics := bridge.diagnostics()
	check(diagnostics.buffers[str(0x80162000)].size == 20480 and diagnostics.buffers[str(0x80162000)].sha256 == Store.BG_TRANSFER_SHA and diagnostics.pending_observations == 1,"native background buffer and explicit producer receipt installed")
	var immutable_diagnostics := bridge.diagnostics()
	check(not bridge.publish(result,0).is_empty() and bridge.diagnostics() == immutable_diagnostics,"duplicate publication cannot repeat the CD operation")
	before = session.state_hash()
	reject_draw = true
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(not result.committed and session.state_hash() == before and bridge.pending_events(1).size() == 1,"failed completion/decode frame restores model and retains actual CD observation")
	reject_draw = false
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(result.committed and state.opening.media.decoded["1"].buffer == 1,"completed read selects the source opposite decode buffer")
	check(bridge.publish(result,1).is_empty(),"successful retry acknowledges background and transfers actor once")
	diagnostics = bridge.diagnostics()
	check(diagnostics.buffers[str(0x80180000)].size == 26624 and diagnostics.buffers[str(0x80180000)].sha256 == Store.ACTOR_SHA and diagnostics.pending_observations == 1,"actual thirteen-sector actor source installed with pending receipt")
	var actual: Session.ServiceEvent = bridge.pending_events(2)[0]
	var invalid_fields := {"epoch":[{},[],null,false,1.0,"1",-1],"request_id":[{}],"request_key":[null],"status":[[]],"resource_id":[false]}
	for key: String in invalid_fields:
		for value: Variant in invalid_fields[key]:
			var payload := actual.payload.duplicate(true)
			payload[key] = value
			var bad := Session.ServiceEvent.new(2,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,actual.service,payload)
			before = session.state_hash()
			result = tick([bad])
			check(not result.committed and not result.error.is_empty() and session.state_hash() == before and bridge.pending_events(2).size() == 1,"malformed actual completion rejects before mutation: "+key)
	result = tick(bridge.pending_events(session.clock.logic_iterations))
	check(result.committed and state.opening.actor96_draw_flags == 0x24 and state.opening.actor96.frame_index == 0,"source actor initializes hidden without advancing its sequence")
	var complete_uploads: Array[Dictionary] = result.presentation.duplicate(true)
	var before_incomplete := bridge.diagnostics()
	result.presentation.pop_back()
	check(not bridge.preflight(result,state).is_empty() and not bridge.publish(result,2).is_empty() and bridge.diagnostics() == before_incomplete and bridge.pending_events(2).size() == 1,"incomplete upload batch cannot acknowledge a receipt or change host buffers")
	result.presentation.assign(complete_uploads)
	check(bridge.publish(result,2).is_empty() and bridge.diagnostics().actor_source_setup_requested and not bridge.diagnostics().visible_actor_supported and not bridge.diagnostics().vram_ready,"source upload batch remains explicit without granting VRAM or visible composition")
	var uploads := store.upload_requests()
	check(uploads.size() == 19 and bridge.diagnostics().native_upload_operation_ids.size() == 19 and uploads[0].slot == 26 and uploads[17].operation == "color_palette_to_gray" and uploads[18].palette_index == 13,"ordered tpage, sixteen palettes, grayscale and gray-palette requests preserved")
	check(uploads[0].destination == {"x_words":640,"y":256,"width_words":64,"height":128} and uploads[16].destination.y == 511,"native tpage and final CLUT rectangle identities retained")
	check(store.upload_source(uploads[0].id).bytes.size() == 16384 and store.upload_source(uploads[16].id).bytes.size() == 512,"upload sources retain actual authenticated native byte extents")
	var changed: Dictionary = uploads[0].duplicate(true)
	changed.destination.x_words = 0
	check(not store.validate_upload(changed).is_empty() and not store.upload_source(uploads[18].id).error.is_empty(),"wrong native destination and unowned gray scratch source fail closed")
	var draw := Session.FrameResult.new()
	bridge.arrival_draw(state,1,draw)
	check(draw.error.is_empty(),"hidden stage0 draw is supported")
	state.opening.actor96_draw_flags = 0xa4
	bridge.arrival_draw(state,1,draw)
	check(not draw.error.is_empty(),"source visibility transition blocks before an unsupported actor draw")
	# Missing BGM must fail the real Arrival dispatcher before any BG request.
	var gate_state := State.new()
	gate_state.game_mode = 0x15; gate_state.mode_step = 1; gate_state.substep = 1
	var services := bridge.callbacks()
	services.bank_load_start = func(_s: State, _role: String, _out: Session.FrameResult) -> void: pass
	services.bank_poll = func(_s: State, _out: Session.FrameResult) -> int: return 1
	services.bank_start = func(_s: State, _bank: int, _sequence: int, out: Session.FrameResult) -> void: out.error = "original opening BGM (0,0) is unavailable"
	services.line_start = func(_s: State, _line: int, _out: Session.FrameResult) -> void: pass
	services.line_run = func(_s: State, _out: Session.FrameResult) -> void: pass
	services.outer_draw = func(_s: State, _frame: int, _out: Session.FrameResult) -> void: pass
	before = JSON.stringify(gate_state.projection())
	var gated := Arrival.step(gate_state,session.rng,0,services)
	check(gated.error.contains("BGM") and gated.presentation.is_empty() and JSON.stringify(gate_state.projection()) == before,"missing source BGM aborts before background request or progression")
	bridge.reset_session()
	check(bridge.diagnostics().buffers.is_empty() and bridge.pending_events(0).is_empty(),"explicit reset clears software transfer buffers and observations")
	print("PASS: %d first-arrival native media checks" % checks if failures == 0 else "FAIL: %d/%d first-arrival media checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
