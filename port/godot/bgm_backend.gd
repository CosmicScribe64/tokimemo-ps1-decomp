extends RefCounted
## First-arrival only. Canonical AudioService owns this adapter at integration.
const Sink = preload("res://bgm_sink.gd")
const ORIGINAL_IDENTITIES: Dictionary = {
	"EXE_sha256": "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
	"SE_header_sha256": "3d38d81c115f91918c318d106fad8e03c0f7ed686eeb4f7bc9bb0efe46eef404",
	"SE_body_sha256": "23df753d5e39259ce010f716707d151cba7a1a8c29fd2ebfdf5a0bf864bb0b86",
	"music_header_sha256": "05f7477864b4ca41a60fdbf943949b9f9a9cd83dcb74088d6529b3d957081032",
	"music_body_sha256": "dc7a06a4d0d90c7a21ac2415cc0f78c2c6bf3ef7f207c6443727fd3d69b588b9",
	"sequence_sha256": "16feb604805496721c8797d481fd4244dc6b501c8a5f609d46f51a22c3acc70a"}
var sink: Sink
var generation: int = 1
var _serial: int = 0
var _prepared: Dictionary = {}
var _engine: RefCounted
var _active_sink_generation: int = 0
var _owner: Dictionary = {}
var history: Array[Dictionary] = []
var render_times_us: Array[float] = []
var push_times_us: Array[int] = []
var error: String = ""
## Development seam for tests/checkpoint/runtime_checkpoint.gd only: when
## valid, it creates each native engine (a recording wrapper around the
## OriginalAudioEngine) so a dev checkpoint can replay the engine's calls.
## The app and every acceptance or parity route leave it unset.
static var engine_factory: Callable = Callable()

func bind_month_context(flags: int, mode: int) -> String:
	if _engine == null or not _prepared.is_empty() or not error.is_empty():
		return "genuine committed first-arrival owner required"
	var validation: String = sink.validate_active(_active_sink_generation)
	if not validation.is_empty():
		return validation
	return _engine.bind_source_context(flags, mode)

func month_start_context() -> Dictionary:
	if _engine == null or not error.is_empty():
		return {"error": "genuine native audio owner required"}
	var validation: String = sink.validate_active(_active_sink_generation)
	if not validation.is_empty():
		return {"error": validation}
	var context: Dictionary = _engine.month_start_context()
	context["sink_generation"] = sink.generation
	context["backend_generation"] = generation
	return context

func prepare_month(files: Array, poll: bool) -> Dictionary:
	var before: Dictionary = month_start_context()
	if not before["error"].is_empty():
		return before
	var reply: Dictionary = _engine.prepare_month(files, poll)
	if reply["error"].is_empty():
		reply["sink_generation"] = sink.generation
		reply["backend_generation"] = generation
	return reply

func validate_month(stage: Dictionary) -> String:
	if _engine == null or stage.get("backend_generation") != generation or stage.get("sink_generation") != _active_sink_generation:
		return "native bank/session/sink ownership changed"
	var validation: String = sink.validate_active(_active_sink_generation)
	if not validation.is_empty():
		return validation
	return _engine.validate_month(stage["ticket"], stage["before"], stage["expected_after"])

func publish_month(stage: Dictionary) -> Dictionary:
	var validation: String = validate_month(stage)
	if not validation.is_empty():
		return {"error": validation}
	return _engine.publish_month(stage["ticket"], stage["before"], stage["expected_after"])

func discard_month(ticket: int) -> void:
	if _engine != null:
		_engine.discard_month(ticket)

func _init(output: Sink) -> void:
	sink = output

static func validate_owner(owner: Dictionary, queued_epoch: Variant) -> String:
	if not queued_epoch is int or queued_epoch <= 0 or not owner.get("epoch") is int or owner["epoch"] != queued_epoch:
		return "bank epoch is stale or unproven"
	if not owner.get("ready") is bool or not owner["ready"]:
		return "bank is not ready"
	for identity: String in ORIGINAL_IDENTITIES:
		if not owner.get(identity) is String or owner[identity] != ORIGINAL_IDENTITIES[identity]:
			return "live original bank identity differs: " + identity
	var selectors: Dictionary = {"load_set": 0, "sequence_pack_set": 0, "sequence": 0, "music_slot": 1, "SE_slot": 0}
	for key: String in selectors:
		if not owner.get(key) is int or owner[key] != selectors[key]:
			return "unsupported original selector: " + key
	if not owner.get("role") is String or owner["role"] != "prologue_arrival":
		return "unsupported original bank role"
	if not owner.get("state_flags") is int or owner["state_flags"] < 0 or owner["state_flags"] > 0xffffffff:
		return "actual native audio flags required"
	var flags: int = owner["state_flags"]
	if (flags & 0x30) not in [0x10, 0x20] or (flags & 0x10000000) != 0:
		return "unsupported gain or preserved reverb state"
	if not owner.get("reverb_preset") is int or owner["reverb_preset"] != 3 or not owner.get("reverb_depth") is int or owner["reverb_depth"] != 60 or not owner.get("reverb_enabled") is bool or not owner["reverb_enabled"]:
		return "actual first-arrival reverb context required"
	return ""

func prepare_start(owner: Dictionary, queued_epoch: Variant, files: Array[PackedByteArray]) -> Dictionary:
	if not error.is_empty():
		return {"error": error}
	if not _prepared.is_empty():
		return {"error": "only one private first-arrival preparation may be pending"}
	var validation: String = validate_owner(owner, queued_epoch)
	if not validation.is_empty():
		return {"error": validation}
	var raw_candidate: Variant = engine_factory.call() if engine_factory.is_valid() else ClassDB.instantiate("OriginalAudioEngine")
	if not raw_candidate is RefCounted:
		return {"error": "registered original audio class did not create an owned engine"}
	var candidate: RefCounted = raw_candidate
	var gain: int = 69 if (int(owner["state_flags"]) & 0x20) != 0 else 93
	validation = candidate.initialize_inputs(files, gain)
	if not validation.is_empty():
		return {"error": validation}
	var pcm: PackedVector2Array = PackedVector2Array()
	for _block: int in range(Sink.PREBUFFER_BLOCKS):
		var block: Dictionary = candidate.render_block()
		if not block["error"].is_empty():
			candidate.dispose()
			return {"error": block["error"]}
		pcm.append_array(block["pcm"])
	_serial += 1
	var ticket: int = _serial
	validation = sink.reserve(ticket, pcm)
	if not validation.is_empty():
		candidate.dispose()
		return {"error": validation}
	_prepared[ticket] = {"engine": candidate, "owner": owner.duplicate(true), "generation": generation, "sink_generation": sink.generation}
	return {"error": "", "ticket": ticket}

func validate_precommit(ticket: int, current: Dictionary) -> String:
	if not _prepared.has(ticket):
		return "start token missing or consumed"
	var staged: Dictionary = _prepared[ticket]
	var validation: String = validate_owner(current, staged["owner"]["epoch"])
	if not validation.is_empty():
		return validation
	if staged["owner"] != current or staged["generation"] != generation or staged["sink_generation"] != sink.generation:
		return "start ownership changed"
	if _engine != null:
		return "unproved active BGM replacement is unsupported"
	return sink.validate(ticket)

func commit_start(ticket: int, current: Dictionary) -> String:
	# Integrator calls this at the final precommit barrier, without yielding.
	# A separate post-frame caller cannot treat this error as state rollback.
	var validation: String = validate_precommit(ticket, current)
	if not validation.is_empty():
		return validation
	var staged: Dictionary = _prepared[ticket]
	validation = sink.activate_validated(ticket)
	if not validation.is_empty():
		return validation
	_engine = staged["engine"]
	_active_sink_generation = sink.generation
	_owner = staged["owner"].duplicate(true)
	_prepared.erase(ticket)
	history.append({"kind": "bgm_play", "epoch": _owner["epoch"], "generation": generation, "ticket": ticket})
	return ""

func discard(ticket: int) -> void:
	if not _prepared.has(ticket):
		return
	var engine: RefCounted = _prepared[ticket]["engine"]
	engine.dispose()
	sink.discard(ticket)
	_prepared.erase(ticket)

func pump(current: Dictionary) -> String:
	if not error.is_empty():
		return error
	if _engine == null:
		return "no active BGM"
	if sink.generation != _active_sink_generation:
		error = "active sink ownership was invalidated"
		return error
	if _owner != current:
		return "active BGM epoch or context changed; explicit disposal required"
	# A render frame may span several source callbacks. Refill the bounded
	# look-ahead rather than producing only one callback per rendered frame.
	for _block: int in range(Sink.PREBUFFER_BLOCKS):
		if not sink.can_push():
			break
		var block: Dictionary = _engine.render_block()
		if not block["error"].is_empty():
			error = block["error"]
			return error
		render_times_us.append(float(block["render_us"]))
		var start: int = Time.get_ticks_usec()
		error = sink.push(block["pcm"])
		push_times_us.append(Time.get_ticks_usec() - start)
		if not error.is_empty():
			return error
	return error

func stop(current: Dictionary) -> String:
	if _engine == null or current != _owner:
		return "stop owner differs"
	if _engine.diagnostics()["stopped"]:
		return ""
	var validation: String = _engine.stop_sequence()
	if not validation.is_empty():
		return validation
	# Already generated source samples precede this stop in the sink timeline.
	# Preserve that bounded look-ahead rather than dropping samples and jumping
	# the audible cursor. Stop audibility includes this declared sink latency.
	history.append({"kind": "bgm_stop", "epoch": _owner["epoch"], "generation": generation})
	return ""

func rei_action(current: Dictionary, action: String, inputs: Array = [], xa_active: bool = false) -> String:
	if _engine == null or current != _owner or sink.generation != _active_sink_generation:
		return "Rei operation lost its original engine/sink ownership"
	var validation: String = sink.validate_active(_active_sink_generation)
	if not validation.is_empty(): return validation
	match action:
		"load": return _engine.load_rei_bank(inputs)
		"fade": return _engine.begin_rei_fade()
		"tick": return _engine.tick_rei_fade(xa_active)
		"start": return _engine.start_rei(xa_active)
	return "Unknown original Rei operation"

func diagnostics() -> Dictionary:
	return _engine.diagnostics() if _engine != null else {}

func reset() -> void:
	generation += 1
	for raw_ticket: Variant in _prepared.keys():
		discard(int(raw_ticket))
	if _engine != null:
		_engine.dispose()
	_engine = null
	_active_sink_generation = 0
	_owner.clear()
	sink.reset()
	history.clear()
	render_times_us.clear()
	push_times_us.clear()
	error = ""
