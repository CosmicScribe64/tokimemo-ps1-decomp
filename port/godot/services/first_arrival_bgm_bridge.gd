extends RefCounted
## T-29025: one AudioService-owned first-start transaction and native sink.
## Native initialization and queue preparation occur outside Session callbacks.
const Context = preload("res://services/first_arrival_audio_context.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Same = preload("res://services/first_arrival_launch_resources.gd")
const LIBRARY := "res://bin/liboriginal_audio.linux.template_release.arm64.so"
const LIBRARY_SHA := "f1eecf80c1c6538a33f6bee9c7d10a79553b1745d520688f556bbe60fb1687b9"
const DESCRIPTOR := "res://original_audio.gdextension"
const DESCRIPTOR_SHA := "9bf0cf85a0154defa2d1e440cb314be441e5ebe57889b2a2b57c86cf16003a72"
var _audio: WeakRef
var _session: WeakRef
var _live: Script
var _backend: RefCounted
var _sink: RefCounted
var _original: Dictionary = {}
var _ticket := 0
var _serial := 0
var _generation := 1
var _before: Dictionary = {}
var _after: Dictionary = {}
var _owner: Dictionary = {}
var _intent: Dictionary = {}
var _committed_after: Dictionary = {}
var _barrier_tick := -1
var _barrier_result_id := 0
var _published_tick := -1
var _last_pump_us := 0
var _max_pump_gap_us := 0
var _max_pump_gap_context: Dictionary = {}

func prepare(audio: RefCounted, host: Node, source_root: String) -> String:
	if _audio != null: return "original BGM owner was already initialized"
	if OS.get_name() != "Linux" or Engine.get_architecture_name() != "arm64":
		return "original opening BGM has no qualified native library for this platform"
	if FileAccess.get_sha256(LIBRARY) != LIBRARY_SHA or FileAccess.get_sha256(DESCRIPTOR) != DESCRIPTOR_SHA:
		return "original opening BGM native library differs from the reviewed component"
	if not GDExtensionManager.is_extension_loaded(DESCRIPTOR):
		var status := GDExtensionManager.load_extension(DESCRIPTOR)
		if status != GDExtensionManager.LOAD_STATUS_OK: return "original BGM native extension registration failed"
	if not ClassDB.class_exists("OriginalAudioEngine"): return "original BGM native class was not registered"
	# Typed native adapters cannot be parsed before registration above.
	var source_script = load("res://bgm_source_pack.gd")
	var backend_script = load("res://bgm_backend.gd")
	var sink_script = load("res://bgm_sink.gd")
	_live = load("res://bgm_live_context.gd")
	if source_script == null or backend_script == null or sink_script == null or _live == null:
		return "original BGM adapters could not be loaded"
	_original = source_script.read_pack(source_root)
	if not _original.get("error", "missing source result").is_empty(): return str(_original.error)
	_audio = weakref(audio)
	_sink = sink_script.new(host)
	_backend = backend_script.new(_sink)
	return ""

func bind_session(session: Session) -> String:
	if _session != null or _audio == null or _backend == null: return "original BGM session ownership is unavailable or reused"
	_session = weakref(session)
	session.final_precommit = final_precommit
	return ""

func _derive(state: State, context: Dictionary) -> Dictionary:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null or not context.get("state_flags") is int: return {"error":"actual native audio context is unavailable"}
	return _live.derive(state,audio,_original,context.state_flags,
		{"preset":context.get("reverb_type"),"depth":60 if context.get("reverb_depth") == [60,60] else -1,"enabled":context.get("reverb_enabled")})

func bank_published(role: String, epoch: int) -> String:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null: return "original BGM AudioService was disposed"
	var published := Context.bank_publication(audio.native_audio_context,audio,role,epoch)
	if not published.error.is_empty(): return published.error
	audio.native_audio_context = published.context.duplicate(true)
	return ""

func bank_completed(state: State) -> String:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null: return "original BGM AudioService was disposed"
	var context: Dictionary = audio.native_audio_context
	var bank: Dictionary = state.opening.get("audio_bank",{})
	if context.get("phase") != "prologue_bank_published" or bank.get("ready") != true or bank.get("epoch") != context.get("bank_epoch") or bank.get("role") != context.get("active_bank_role"):
		return "consumed bank completion differs from actual native publication"
	state.opening["native_audio_context"] = context.duplicate(true)
	return ""

func prepare_pending(state: State, _events: Array[Session.ServiceEvent]) -> String:
	if _backend == null: return "original opening BGM backend is unavailable"
	if not _committed_after.is_empty() or _ticket != 0: return ""
	var audio = _audio.get_ref()
	if audio == null: return "original BGM AudioService was disposed"
	if audio.native_audio_context.get("phase") != "prologue_bank_published": return ""
	# The actual source has scene_reset at substep0 between bank load and
	# bank_poll/start at substep1. Prepare only after the completion frame has
	# committed. No synthetic ready state or extra logical wait is introduced.
	if state.game_mode != 0x15 or state.mode_stage != 0 or state.mode_step != 1 or state.substep != 1: return ""
	if state.opening.get("audio_bank",{}).get("ready") != true: return ""
	var context: Dictionary = state.opening.get("native_audio_context",{})
	var plan := Context.start_plan(context,_original.files[5],0,0)
	if not plan.error.is_empty(): return plan.error
	var derived := _derive(state,context)
	if not derived.error.is_empty(): return derived.error
	var prepared: Dictionary = _backend.prepare_start(derived.owner,derived.owner.epoch,_original.files)
	if not prepared.error.is_empty(): return prepared.error
	_ticket = prepared.ticket
	_serial += 1
	_before = plan.before.duplicate(true)
	_after = plan.after.duplicate(true)
	_owner = derived.owner.duplicate(true)
	_intent = {"kind":"original_bgm_start","role":"prologue_arrival","bank_epoch":_owner.epoch,"request_serial":_serial,"session_generation":_generation}
	return ""

func start(state: State, bank: int, sequence: int, result: Session.FrameResult) -> void:
	if bank != 0 or sequence != 0 or _ticket == 0:
		result.error = "original first-arrival BGM was not privately prepared"
		return
	if not Same._same(state.opening.get("native_audio_context"),_before):
		result.error = "original BGM source context differs from private preparation"
		return
	var derived := _derive(state,_before)
	if not derived.error.is_empty() or not Same._same(derived.get("owner"),_owner):
		result.error = "original BGM actual bank ownership changed"
		return
	result.error = _backend.validate_precommit(_ticket,_owner)
	if not result.error.is_empty(): return
	state.opening.native_audio_context = _after.duplicate(true)
	result.audio.append(_intent.duplicate(true))

func preflight_event(event: Dictionary, state: State = null) -> String:
	if (_ticket == 0 and _committed_after.is_empty()) or not Same._same(event,_intent): return "original BGM intent differs from prepared ownership"
	if state != null and not Same._same(state.opening.get("native_audio_context"),_after): return "original BGM intent differs from transactional after-state"
	return ""

func final_precommit(state: State, _rng, result: Session.FrameResult) -> void:
	var starts := 0
	for event: Dictionary in result.audio:
		if event.get("kind") == "original_bgm_start":
			starts += 1
			result.error = preflight_event(event,state)
			if not result.error.is_empty(): return
	if _ticket == 0:
		if starts != 0: result.error = "original BGM start has no private preparation"
		return
	# A prepared token is for exactly this next source start, never a later frame.
	if starts != 1:
		result.error = "original BGM requires exactly one source start intent"
		return
	var audio = _audio.get_ref()
	var session = _session.get_ref()
	if audio == null or session == null or session.state != state or not Same._same(audio.native_audio_context,_before):
		result.error = "original BGM service or Session generation changed"
		return
	var derived := _derive(state,_after)
	var expected := _owner.duplicate(true)
	expected.state_flags = _after.state_flags
	if not derived.error.is_empty() or not Same._same(derived.get("owner"),expected):
		result.error = "original BGM final actual bank or context changed"
		return
	result.error = _backend.validate_precommit(_ticket,_owner)
	if result.error.is_empty():
		_barrier_tick = session.clock.logic_iterations
		_barrier_result_id = result.get_instance_id()

func publish(result: Session.FrameResult, tick: int) -> String:
	if not result.committed: return "aborted frame cannot publish original BGM"
	var starts := 0
	for event: Dictionary in result.audio:
		if event.get("kind") is String and event.kind == "original_bgm_start":
			starts += 1
			if not Same._same(event,_intent): return "committed original BGM intent changed"
	if _ticket == 0:
		return "original BGM start token was consumed or missing" if starts != 0 else ""
	if starts != 1 or result.get_instance_id() != _barrier_result_id: return "original BGM publication lost its exact validated frame result"
	var session = _session.get_ref()
	if session == null or _barrier_tick != tick or session.clock.logic_iterations != tick+1 or result.state_hash != session.state_hash(): return "original BGM publication lost the same-stack final barrier"
	# First publisher, same synchronous stack. No loading, generation, yielding
	# or other host publication occurs between the barrier and activation.
	var outcome: String = _backend.commit_start(_ticket,_owner)
	if not outcome.is_empty(): return outcome
	var audio = _audio.get_ref()
	audio.native_audio_context = _after.duplicate(true)
	audio.bgm_state = {"playing":true,"role":_owner.role,"bank":0,"sequence":0}
	_committed_after = _after.duplicate(true)
	_last_pump_us = Time.get_ticks_usec()
	_published_tick = tick
	_ticket = 0
	return ""

func abort_frame() -> void:
	if _ticket != 0 and _backend != null: _backend.discard(_ticket)
	_ticket = 0
	_barrier_tick = -1
	_barrier_result_id = 0
	_before.clear()
	_after.clear()
	_intent.clear()
	if _committed_after.is_empty(): _owner.clear()

func pump(state: State) -> String:
	if _committed_after.is_empty(): return ""
	var audio = _audio.get_ref()
	if audio == null or not Same._same(audio.native_audio_context,_committed_after) or not Same._same(state.opening.get("native_audio_context"),_committed_after): return "active original BGM source after-context changed"
	var bank: Variant = state.opening.get("audio_bank")
	if not bank is Dictionary or bank.get("epoch") != _owner.get("epoch") or bank.get("role") != _owner.get("role") or bank.get("ready") != true or audio.active_bank_role != _owner.get("role") or audio.bank_ready.get(_owner.get("role")) != true:
		return "active original BGM actual bank changed"
	var now := Time.get_ticks_usec()
	if now-_last_pump_us > _max_pump_gap_us:
		_max_pump_gap_us = now-_last_pump_us
		_max_pump_gap_context = {"frame":state.frame_count,"mode":state.game_mode,"stage":state.mode_stage,"step":state.mode_step,"substep":state.substep}
	_last_pump_us = now
	return _backend.pump(_owner)

func pause(value: bool) -> void:
	if _sink != null: _sink.pause(value)

func diagnostics() -> Dictionary:
	return {"active_skips":_sink.skips() if _sink != null else 0,"maximum_pump_gap_us":_max_pump_gap_us,"maximum_pump_gap_context":_max_pump_gap_context.duplicate(),"private_ticket":_ticket,"published_tick":_published_tick,"history":_backend.history.duplicate(true) if _backend != null else [],"sink":_sink.metrics() if _sink != null else {},"engine":_backend.diagnostics() if _backend != null else {}}

func reset() -> void:
	abort_frame()
	if _backend != null: _backend.reset()
	var audio = _audio.get_ref() if _audio != null else null
	if audio != null: audio.bgm_state = {"playing":false,"role":"","bank":-1,"sequence":-1}
	_generation += 1
	_committed_after.clear()
	_owner.clear()
