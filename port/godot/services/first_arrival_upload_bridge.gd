extends RefCounted
## T-29025: actual completed actor read -> source CPU/queue -> word pump.
## Preflight uses local copies; caller buffers change only after frame commit.
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const Producer = preload("res://presentation/prologue_actor_upload_provider.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const APPROVAL_META := &"first_arrival_upload_preflight_v1"
var _media: Media
var _store: Store
var _owned: Dictionary
var _session: WeakRef
var _producer := Producer.new()
var _pump := Pump.new()
var _batch_key := ""
var _last_published_tick := -1

func prepare(media: Media, store: Store, owned_runtime: Dictionary) -> String:
	if _media != null: return "first-arrival upload bridge already owns its prepared runtime"
	if media == null or store == null or store.identity().is_empty() or media.resource_identity() != store.identity():
		return "first-arrival upload bridge needs its actual authenticated media owner"
	if not Pump._owners(owned_runtime) or not owned_runtime.get("scratch_words") is PackedByteArray or owned_runtime.scratch_words.size() != 512 or not owned_runtime.get("pending_gpu_effects") is Array or not owned_runtime.pending_gpu_effects.is_empty():
		return "first-arrival upload bridge needs explicit scratch/VRAM buffers and an empty owned queue"
	if owned_runtime.has("vram_known_mask") and (not owned_runtime.vram_known_mask is PackedByteArray or owned_runtime.vram_known_mask.size() != Pump.VRAM_BYTES):
		return "first-arrival upload bridge received a malformed VRAM ownership mask"
	_media = media
	_store = store
	_owned = owned_runtime
	return ""

func bind_session(session: Session) -> String:
	if _media == null or session == null or not session.state is State: return "upload bridge requires its prepared logical Session"
	if _session != null: return "" if _session.get_ref() == session else "upload bridge cannot change its Session owner"
	var error := _media.bind_session(session)
	if not error.is_empty(): return error
	_session = weakref(session)
	return ""

func preflight(result: Session.FrameResult, state: State) -> String:
	var owner := _owner()
	if owner == null or owner.state != state: return "upload preflight belongs to another or unbound Session"
	var error := _media.preflight(result,state)
	if not error.is_empty(): return error
	var preview := _preview(result,state)
	if not preview.error.is_empty(): return preview.error
	if not result.committed:
		result.set_meta(APPROVAL_META,{"session_id":owner.get_instance_id(),"tick":owner.clock.logic_iterations,
			"effects_hash":_effects_hash(result),"runtime_hash":_runtime_hash(_owned)})
	return ""

func publish(result: Session.FrameResult, tick: int) -> String:
	var owner := _owner()
	if owner == null or not result.committed or tick < 0 or tick <= _last_published_tick or tick != owner.clock.logic_iterations-1 or result.state_hash != owner.state_hash():
		return "upload publication requires the current committed owning Session frame"
	var approval: Variant = result.get_meta(APPROVAL_META,null)
	if not approval is Dictionary or not Store._same(approval,{"session_id":owner.get_instance_id(),"tick":tick,"effects_hash":_effects_hash(result),"runtime_hash":_runtime_hash(_owned)}):
		return "upload effects or owned buffers changed after their frame preflight"
	if _media.publication_tick() != tick: return "actual native media publication must precede its upload producer"
	var preview := _preview(result,owner.state as State)
	if not preview.error.is_empty(): return preview.error
	if preview.has("plan"):
		var next: Dictionary = _owned.duplicate(true)
		next["actor_runtime_bytes"] = preview.actor_bytes.duplicate()
		var produced := _producer.execute(preview.plan,preview.actor_bytes,next)
		if produced.get("status") != "effects_enqueued": return str(produced.get("diagnostic","source upload producer failed"))
		var queue_error := _pump_extents(next)
		if not queue_error.is_empty(): return queue_error
		var pumped := _pump.execute(_store,preview.batch_key,next)
		if not pumped.error.is_empty(): return pumped.error
		# Both CPU source writes and all18 destination copies succeeded locally.
		_owned.clear()
		_owned.merge(next)
		_batch_key = preview.batch_key
	_last_published_tick = tick
	return ""

func batch_ready(batch_key: String) -> String:
	if batch_key != _batch_key or _batch_key.is_empty(): return "actual first-arrival upload batch has not executed"
	return _pump.batch_ready(batch_key,_owned)

func diagnostics() -> Dictionary:
	return {"batch_key":_batch_key,"batch_complete":not _batch_key.is_empty(),"vram_ready":false,
		"publication_tick":_last_published_tick,"pending_gpu_effects":_owned.get("pending_gpu_effects",[]).size()}

func _owner() -> Session:
	return _session.get_ref() as Session if _session != null else null

func _preview(result: Session.FrameResult, state: State) -> Dictionary:
	var uploads: Array[Dictionary] = []
	for effect: Dictionary in result.presentation:
		if effect.get("kind") == "first_arrival_native_upload": uploads.append(effect)
	if uploads.is_empty(): return {"error":""}
	if not _batch_key.is_empty(): return {"error":"first-arrival actor upload batch has already executed"}
	if uploads.size() != 19: return {"error":"actual upload producer requires the complete19 source operations"}
	var source: Variant = state.opening.get("arrival_media")
	if not source is Dictionary or not source.get("upload_batch_key") is String: return {"error":"actual upload batch has no logical owner"}
	var batch_key: String = source.upload_batch_key
	for index in range(19):
		if uploads[index].get("batch_key") != batch_key or uploads[index].get("ordinal") != index or not Store._same(uploads[index].get("request"),_store.upload_requests()[index]):
			return {"error":"actual upload producer received another source operation batch"}
	var transfer := _media.completed_actor_buffer(state)
	if not transfer.error.is_empty(): return transfer
	var next := _owned.duplicate(true)
	if not next.get("pending_gpu_effects") is Array or not next.pending_gpu_effects.is_empty(): return {"error":"actual upload producer requires an empty native queue"}
	next["actor_runtime_bytes"] = transfer.bytes.duplicate()
	var plan := _producer.preflight(transfer.bytes,next)
	if plan.get("status") != "preflight_ok": return {"error":str(plan.get("diagnostic","source upload preflight failed"))}
	var produced := _producer.execute(plan,transfer.bytes,next)
	if produced.get("status") != "effects_enqueued": return {"error":str(produced.get("diagnostic","source upload preview failed"))}
	var queue_error := _pump_extents(next)
	if not queue_error.is_empty(): return {"error":queue_error}
	var validation := Pump.new().execute(_store,batch_key,next)
	if not validation.error.is_empty(): return validation
	return {"error":"","plan":plan,"actor_bytes":transfer.bytes,"batch_key":batch_key}

func _pump_extents(runtime: Dictionary) -> String:
	# The producer's gray record carries its byte extent in source.size. The
	# pump protocol also requires the explicit transfer size at the top level.
	var pending: Variant = runtime.get("pending_gpu_effects")
	if not pending is Array or pending.size() != 18 or not pending[17] is Dictionary:
		return "source producer did not enqueue its exact18 GPU records"
	var gray: Dictionary = pending[17]
	var expected: Dictionary = _store.upload_requests()[18]
	if gray.get("id") != expected.id or not Store._same(gray.get("source"),expected.source):
		return "gray GPU record has no exact source-carried byte extent"
	if gray.has("size") and (not gray["size"] is int or gray["size"] != expected.source.size):
		return "gray GPU record conflicts with its source-carried byte extent"
	gray["size"] = expected.source.size
	return ""

static func _effects_hash(result: Session.FrameResult) -> String:
	var effects: Array[Dictionary] = []
	for effect: Dictionary in result.presentation:
		if effect.get("kind") in Media.OWN_INTENTS: effects.append(effect)
	return JSON.stringify(effects).sha256_text()

static func _runtime_hash(owned: Dictionary) -> String:
	var values: Dictionary = {}
	for key: Variant in owned:
		values[key] = Store._sha(owned[key]) if owned[key] is PackedByteArray else owned[key]
	return JSON.stringify(values).sha256_text()
