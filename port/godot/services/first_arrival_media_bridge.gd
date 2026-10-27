extends RefCounted
## Prepared original bytes become software read buffers only after frame commit.
## Completion observations enter the next logical BEFORE_MODE phase.
const Store = preload("res://services/first_arrival_media_store.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const APPROVAL_META := &"first_arrival_media_preflight_v1"
const OWN_INTENTS := ["opening_media_request","opening_background_select","opening_arrival_actor_setup",
	"opening_media_reset","opening_scene_reset","first_arrival_background_draw","first_arrival_native_upload"]
var queue := Queue.new()
var _store: Store
var _pending: Array[Dictionary] = []
var _buffers: Dictionary = {}
var _actor_upload: Dictionary = {}
var _last_published_tick := -1
var _session: WeakRef

func prepare(store: Store) -> String:
	if _store != null: return "first-arrival media bridge already owns immutable resources"
	if store == null or store.identity().is_empty(): return "first-arrival media bridge needs an authenticated native transfer store"
	var resources := store.descriptions()
	var error: String = queue.prepare(resources.backgrounds,resources.actors,{})
	if not error.is_empty(): return error
	_store = store
	return ""

func bind_session(session: Session) -> String:
	if _store == null or session == null or not session.state is State:
		return "first-arrival media needs its prepared owning Session"
	if _session != null:
		return "" if _session.get_ref() == session else "first-arrival media belongs to another session; reset before rebinding"
	_session = weakref(session)
	return ""

func callbacks() -> Dictionary:
	var services := queue.callbacks()
	services["scene_reset"] = scene_reset
	services["actor_palette_setup"] = actor_palette_setup
	services["arrival_draw"] = arrival_draw
	return services

func scene_reset(state: State, result: Session.FrameResult) -> void:
	queue.scene_reset(state,result)
	if not result.error.is_empty(): return
	state.opening["arrival_media"] = {"background_brightness":128,"back_clear":true,"actor_initialized":false,"manifest_sha256":_store.identity()}
	state.opening.erase("actor96")
	state.opening.erase("actor96_draw_flags")

func actor_palette_setup(state: State, result: Session.FrameResult) -> void:
	if state.game_mode != 0x15 or state.mode_stage != 0 or state.mode_step != 1 or state.substep != 4:
		result.error = "first-arrival actor initialization is outside its source stage"
		return
	queue.actor_palette_setup(state,result)
	if not result.error.is_empty(): return
	state.opening["actor96"] = _store.initial_actor()
	state.opening["actor96_draw_flags"] = 0x24
	state.opening.arrival_media.actor_initialized = true
	var uploads := _store.upload_requests()
	var batch_key := JSON.stringify([state.opening.media.epoch,Store.ACTOR_SHA,uploads]).sha256_text()
	state.opening.arrival_media["upload_batch_key"] = batch_key
	for index in uploads.size():
		result.presentation.append({"kind":"first_arrival_native_upload","batch_key":batch_key,"ordinal":index,"request":uploads[index]})

func before_line0(state: State, line: int, result: Session.FrameResult) -> void:
	if line != 0 or state.game_mode != 0x15 or state.mode_stage != 0 or state.mode_step != 2 or state.substep != 0 or not state.opening.get("arrival_media") is Dictionary:
		result.error = "first-arrival line0 setup is outside the source dispatch"
		return
	state.opening.arrival_media.back_clear = false

func arrival_draw(state: State, slot: int, result: Session.FrameResult) -> void:
	if _store == null or state.mode_stage != 0 or slot != 1 or (int(state.opening.get("actor96_draw_flags",0)) & 0x80) != 0:
		result.error = "first-arrival provider needs a visible actor renderer before the stage transition"
		return
	result.presentation.append({"kind":"first_arrival_background_draw","slot":1})

func complete(event: Session.ServiceEvent, state: State, rng, result: Session.FrameResult) -> void:
	queue.complete(event,state,rng,result)

func pending_events(tick: int) -> Array[Session.ServiceEvent]:
	var events: Array[Session.ServiceEvent] = []
	for observation: Dictionary in _pending:
		events.append(Session.ServiceEvent.new(tick,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,Queue.SERVICE,observation))
	return events

func _observations(result: Session.FrameResult) -> String:
	var consumed := 0
	for entry: Dictionary in result.trace:
		if not entry.get("kind") is int or entry.kind != Session.ServiceKind.COMPLETION: continue
		if not entry.get("service") is String or entry.service != Queue.SERVICE: continue
		if consumed >= _pending.size() or not entry.get("payload") is Dictionary or not Store._same(entry.payload,_pending[consumed]):
			return "first-arrival media observation differs from the actual queued producer"
		consumed += 1
	return ""

func preflight(result: Session.FrameResult, state: State) -> String:
	if _store == null: return "first-arrival media bridge is not prepared"
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if owner == null or owner.state != state: return "first-arrival media intent belongs to another or unbound logical Session"
	var error := _observations(result)
	if not error.is_empty(): return error
	var setup_count := 0
	var upload_count := 0
	var reset_seen := false
	for intent: Dictionary in result.presentation:
		error = _intent_schema(intent)
		if not error.is_empty(): return error
		match intent.get("kind"):
			"opening_media_request":
				error = _request(intent,state)
				if not error.is_empty(): return error
			"opening_background_select":
				if intent.get("slot") != 1 or intent.get("resource_id") != Store.BG_ID:
					return "first-arrival background selection is outside its prepared source"
			"opening_arrival_actor_setup":
				setup_count += 1
				if intent.get("resource_id") != Store.ACTOR_ID or not _buffers.has(str(0x80180000)) or state.opening.get("actor96_draw_flags") != 0x24:
					return "first-arrival palette setup has no completed original actor read"
			"first_arrival_native_upload":
				var source: Variant = state.opening.get("arrival_media")
				if not source is Dictionary or not source.get("upload_batch_key") is String or intent.batch_key != source.upload_batch_key or intent.ordinal != upload_count or intent.request.id != _store.upload_requests()[upload_count].id:
					return "first-arrival upload differs from its source batch/operation order"
				upload_count += 1
			"opening_media_reset":
				reset_seen = true
				if intent.get("service") != Queue.SERVICE or intent.get("epoch") != state.opening.get("media",{}).get("epoch"):
					return "first-arrival reset differs from transactional ownership"
	if setup_count > 1 or (setup_count == 1 and (upload_count != 19 or reset_seen)) or (setup_count == 0 and upload_count != 0):
		return "first-arrival source setup requires one complete ordered native upload batch"
	if not result.committed:
		# FrameResult metadata is callback-owned output, never external peer or
		# model mutation. Keep approval when later Core phases append their trace.
		result.set_meta(APPROVAL_META,{"session_id":owner.get_instance_id(),"tick":owner.clock.logic_iterations,"media_hash":_effect_hash(result)})
	return ""

static func _effect_hash(result: Session.FrameResult) -> String:
	var intents: Array[Dictionary] = []
	var observations: Array[Dictionary] = []
	for intent: Dictionary in result.presentation:
		if intent.get("kind") is String and intent.kind in OWN_INTENTS: intents.append(intent)
	for entry: Dictionary in result.trace:
		if entry.get("kind") is int and entry.kind == Session.ServiceKind.COMPLETION and entry.get("service") is String and entry.service == Queue.SERVICE: observations.append(entry)
	return JSON.stringify([intents,observations]).sha256_text()

func _request(intent: Dictionary, state: State) -> String:
	var request: Dictionary = intent.request
	if not request.get("request_id") is String or not state.opening.get("media") is Dictionary:
		return "first-arrival native transfer lacks transactional ownership"
	var owned: Variant = state.opening.media.requests.get(request.request_id)
	if not owned is Dictionary or owned != request or request.get("status") != "pending":
		return "first-arrival native transfer differs from its transactional request"
	return ""

func _intent_schema(intent: Dictionary) -> String:
	var kind: Variant = intent.get("kind")
	if not kind is String: return "first-arrival presentation kind requires a string"
	var fields: Array
	match kind:
		"opening_media_request": fields = ["kind","service","request"]
		"opening_background_select": fields = ["kind","slot","resource_id","buffer"]
		"opening_arrival_actor_setup": fields = ["kind","resource_id"]
		"opening_media_reset": fields = ["kind","service","epoch"]
		"opening_scene_reset": fields = ["kind"]
		"first_arrival_background_draw": fields = ["kind","slot"]
		"first_arrival_native_upload": fields = ["kind","batch_key","ordinal","request"]
		_:
			return "unsupported first-arrival media intent" if kind.begins_with("opening_media_") else ""
	if intent.size() != fields.size() or not intent.has_all(fields): return "first-arrival intent fields differ from the supported schema"
	if kind == "first_arrival_native_upload":
		if not Store._sha_string(intent.batch_key) or not intent.ordinal is int or intent.ordinal < 0 or intent.ordinal >= 19: return "first-arrival upload batch identity is invalid"
		return _store.validate_upload(intent.request)
	if intent.has("service") and (not intent.service is String or intent.service != Queue.SERVICE): return "first-arrival media intent belongs to another service"
	if intent.has("slot") and (not intent.slot is int or intent.slot != 1): return "first-arrival background slot differs from source"
	if intent.has("buffer") and (not intent.buffer is int or intent.buffer not in [0,1]): return "first-arrival background buffer is invalid"
	if intent.has("epoch") and (not intent.epoch is int or intent.epoch < 0 or intent.epoch > 0xffffffff): return "first-arrival reset epoch is invalid"
	if intent.has("resource_id") and (not intent.resource_id is String or intent.resource_id != (Store.ACTOR_ID if kind == "opening_arrival_actor_setup" else Store.BG_ID)): return "first-arrival intent selects another prepared resource"
	if kind != "opening_media_request": return ""
	var request: Variant = intent.request
	var request_fields: Array[String] = ["epoch","request_id","kind","payload","resource_id","resource_sha256","status","request_key"]
	if not request is Dictionary or request.size() != 8 or not request.has_all(request_fields): return "first-arrival native request fields are malformed"
	if not request.epoch is int or request.epoch < 0 or request.epoch > 0xffffffff: return "first-arrival request epoch is invalid"
	for key: String in ["request_id","kind","resource_id","resource_sha256","status","request_key"]:
		if not request[key] is String: return "first-arrival request identity requires exact strings"
	if request.request_id.is_empty() or request.request_id.length() > 16 or not request.request_id.is_valid_int() or request.request_id.begins_with("-"): return "first-arrival request identifier is invalid"
	if request.status != "pending" or not Store._sha_string(request.request_key): return "first-arrival request status/key is invalid"
	if not request.payload is Dictionary: return "first-arrival request arguments are malformed"
	var args: Dictionary = request.payload
	if request.kind == "background":
		if request.resource_id != Store.BG_ID or request.resource_sha256 != Store.BG_TRANSFER_SHA or args.size() != 3 or not args.has_all(["lba","slot","retry"]) or not args.lba is int or args.lba != 0x494b or not args.slot is int or args.slot != 1 or not args.retry is bool: return "first-arrival background arguments differ from source"
	elif request.kind == "actors":
		if request.resource_id != Store.ACTOR_ID or request.resource_sha256 != Store.ACTOR_SHA or args.size() != 3 or not args.has_all(["lba","sectors","destination"]): return "first-arrival actor arguments differ from source"
		for key: String in ["lba","sectors","destination"]:
			if not args[key] is int: return "first-arrival actor arguments require exact integers"
		if args.lba != 0xabab or args.sectors != 13 or args.destination != 0x80180000: return "first-arrival actor read extent differs from source"
	else: return "unsupported first-arrival native request kind"
	if JSON.stringify([request.epoch,request.request_id,request.kind,args,request.resource_id,request.resource_sha256]).sha256_text() != request.request_key: return "first-arrival request key differs from its authenticated identity"
	return ""

func publish(result: Session.FrameResult, tick: int) -> String:
	if not result.committed: return "aborted frames cannot publish native media transfers"
	if _store == null: return "first-arrival media bridge is not prepared"
	if tick < 0 or tick <= _last_published_tick: return "first-arrival media frame was already published or is out of order"
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if owner == null or tick != owner.clock.logic_iterations-1 or result.state_hash != owner.state_hash(): return "first-arrival publication is not the current committed Session frame"
	var approval: Variant = result.get_meta(APPROVAL_META,null)
	if not approval is Dictionary or approval.size() != 3 or not approval.has_all(["session_id","tick","media_hash"]) or not approval.session_id is int or approval.session_id != owner.get_instance_id() or not approval.tick is int or approval.tick != tick or not approval.media_hash is String or approval.media_hash != _effect_hash(result):
		return "first-arrival media effects changed or lost their frame preflight approval"
	var error := preflight(result,owner.state as State)
	if not error.is_empty(): return error
	var consumed := 0
	for entry: Dictionary in result.trace:
		if entry.get("kind") is int and entry.kind == Session.ServiceKind.COMPLETION and entry.get("service") is String and entry.service == Queue.SERVICE: consumed += 1
	var next_pending: Array[Dictionary] = _pending.slice(consumed)
	var next_buffers: Dictionary = _buffers.duplicate()
	var next_upload: Dictionary = _actor_upload.duplicate()
	var setup_seen := false
	for intent: Dictionary in result.presentation:
		error = _intent_schema(intent)
		if not error.is_empty(): return error
		match intent.get("kind"):
			"opening_media_reset":
				next_pending.clear()
				next_buffers.clear()
				next_upload.clear()
			"opening_media_request":
				var request: Dictionary = intent.request
				var transfer := _store.transfer(request.resource_id)
				if not str(transfer.get("error","")).is_empty(): return str(transfer.error)
				# These are actual authenticated native-size bytes, not a ready flag.
				next_buffers[str(transfer.destination)] = {"bytes":transfer.bytes,"sha256":transfer.sha256,"resource_id":request.resource_id}
				next_pending.append({"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,
					"status":"ready","resource_id":request.resource_id})
			"opening_arrival_actor_setup":
				if setup_seen: return "first-arrival actor setup cannot publish twice in one frame"
				setup_seen = true
				if not next_buffers.has(str(0x80180000)) or next_buffers[str(0x80180000)].sha256 != Store.ACTOR_SHA: return "first-arrival source texture setup has no completed actor buffer"
				next_upload = {"source_sha256":Store.ACTOR_SHA,"operation_ids":[],"vram_ready":false,"visible_composition_supported":false}
			"first_arrival_native_upload":
				if next_upload.is_empty(): return "first-arrival native upload has no source setup batch"
				if intent.ordinal != next_upload.operation_ids.size() or intent.request.id != _store.upload_requests()[intent.ordinal].id: return "first-arrival native uploads are missing, repeated or out of source order"
				if not next_upload.has("batch_key"): next_upload["batch_key"] = intent.batch_key
				if next_upload.batch_key != intent.batch_key: return "first-arrival native uploads belong to different batches"
				next_upload.operation_ids.append(intent.request.id)
	if setup_seen and (not next_upload.get("operation_ids") is Array or next_upload.operation_ids.size() != 19): return "first-arrival native upload batch is incomplete"
	_pending = next_pending
	_buffers = next_buffers
	_actor_upload = next_upload
	_last_published_tick = tick
	return ""

func resource_identity() -> String:
	return _store.identity() if _store != null else ""

func completed_actor_buffer(state: State) -> Dictionary:
	# Read the published buffer only through this Session's actual completion.
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if _store == null or owner == null or owner.state != state:
		return {"error":"completed actor buffer belongs to another or unbound Session"}
	var media: Variant = state.opening.get("media")
	if not media is Dictionary or not media.get("epoch") is int or not media.get("requests") is Dictionary or not media.get("actor_pending") is Dictionary:
		return {"error":"completed actor transfer has no owned request"}
	var request: Variant = media.requests.get(media.actor_pending.get(str(0x80180000),""))
	if not request is Dictionary or request.get("kind") != "actors" or request.get("status") != "ready" or request.get("resource_id") != Store.ACTOR_ID or request.get("resource_sha256") != Store.ACTOR_SHA:
		return {"error":"actor transfer has not completed in this logical Session"}
	var pending_request: Dictionary = request.duplicate(true)
	pending_request.status = "pending" # Reuse the native request schema/hash guard.
	var error := _intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":pending_request})
	if not error.is_empty() or request.epoch != media.epoch:
		return {"error":"completed actor request has malformed or foreign ownership"}
	var buffer: Variant = _buffers.get(str(0x80180000))
	if not buffer is Dictionary or buffer.get("resource_id") != Store.ACTOR_ID or buffer.get("sha256") != Store.ACTOR_SHA or not buffer.get("bytes") is PackedByteArray or buffer.bytes.size() != 26624 or Store._sha(buffer.bytes) != Store.ACTOR_SHA:
		return {"error":"completed actor transfer has no published authentic bytes"}
	return {"error":"","bytes":buffer.bytes.duplicate(),"sha256":Store.ACTOR_SHA,
		"epoch":media.epoch,"request_key":request.request_key,"publication_tick":_last_published_tick}

func publication_tick() -> int:
	return _last_published_tick

func paint_background(target: Image, state: State) -> String:
	if _store == null or target == null or target.get_size() != Vector2i(320,240) or state.game_mode != 0x15 or state.mode_stage != 0:
		return "first-arrival background draw is outside its prepared source scope"
	var media: Variant = state.opening.get("arrival_media")
	if not media is Dictionary or not media.get("background_brightness") is int or media.background_brightness != 128 or not media.get("actor_initialized") is bool:
		return "first-arrival background needs its source reset/brightness state"
	var flags: Variant = state.opening.get("actor96_draw_flags",0)
	if not flags is int or flags != (0x24 if media.actor_initialized else 0):
		return "first-arrival requires a source actor compositor before visibility"
	var work: Variant = state.opening.get("media")
	if not work is Dictionary or not work.get("decoded") is Dictionary: return "first-arrival background cache is not initialized"
	var decoded: Variant = work.decoded.get("1")
	if decoded == null: return ""
	if not decoded is Dictionary or decoded.get("resource_id") is not String or decoded.resource_id != Store.BG_ID or decoded.get("resource_sha256") is not String or decoded.resource_sha256 != Store.BG_TRANSFER_SHA or not decoded.get("buffer") is int or decoded.buffer not in [0,1]:
		return "first-arrival draw has no authenticated selected background"
	# dec_bg_reset gives bg_brightness128 independently of UI brightness.
	target.blend_rect(_store.background_image(),Rect2i(0,0,320,160),Vector2i.ZERO)
	return ""

func diagnostics() -> Dictionary:
	var buffers: Dictionary = {}
	for destination: String in _buffers:
		buffers[destination] = {"size":_buffers[destination].bytes.size(),"sha256":_buffers[destination].sha256}
	return {"buffers":buffers,"pending_observations":_pending.size(),"actor_source_setup_requested":not _actor_upload.is_empty(),
		"native_upload_operation_ids":_actor_upload.get("operation_ids",[]).duplicate(),"vram_ready":false,"visible_actor_supported":false}

func reset_session() -> void:
	_pending.clear()
	_buffers.clear()
	_actor_upload.clear()
	_last_published_tick = -1
	_session = null
