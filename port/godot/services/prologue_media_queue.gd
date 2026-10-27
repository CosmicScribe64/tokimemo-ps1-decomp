extends RefCounted
## T-29025: logical CD request/completion records, never host I/O or pixels.
## The owner supplies immutable reader-authenticated resource descriptions.
## This queue cannot establish source authenticity or a gameplay capability.

const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const SERVICE := "opening.media"
var _backgrounds: Dictionary = {}
var _actors: Dictionary = {}
var _hud: Dictionary = {}
var _prepared := false

func prepare(backgrounds: Dictionary, actors: Dictionary, hud: Dictionary) -> String:
	if _prepared:
		return "opening media descriptions are immutable; create a new queue for another pack"
	for resource: Variant in backgrounds.values() + actors.values() + ([hud] if not hud.is_empty() else []):
		if not resource is Dictionary or not resource.get("id") is String or resource.id.is_empty() or not resource.get("sha256") is String or resource.sha256.length() != 64:
			return "opening media needs immutable authenticated resource identities"
	_backgrounds = backgrounds.duplicate(true)
	_actors = actors.duplicate(true)
	_hud = hud.duplicate(true)
	_prepared = true
	return ""

func callbacks() -> Dictionary:
	return {"scene_reset":scene_reset,"background_read":background_read,"cd_sync":cd_sync,
		"background_decode":background_decode,"actor_read":actor_read,
		"actor_palette_setup":actor_palette_setup,"hud_read_start":hud_read_start,
		"finish_cd_reset":finish_cd_reset}

func _record(state: State) -> Dictionary:
	if not state.opening.has("media"):
		state.opening["media"] = {"epoch":0,"next_id":0,"requests":{},"background_pending":{},"decoded":{},"actor_pending":{},"hud_pending":""}
	return state.opening.media

func _request(state: State, kind: String, payload: Dictionary, resource: Dictionary, result: Session.FrameResult) -> String:
	var media: Dictionary = _record(state)
	if media.requests.size() >= 512:
		result.error = "opening media request bound reached"
		return ""
	var identity: String = str(media.next_id)
	media.next_id += 1
	var request := {"epoch":media.epoch,"request_id":identity,"kind":kind,"payload":payload.duplicate(true),
		"resource_id":resource.id,"resource_sha256":resource.sha256,"status":"pending"}
	request["request_key"] = JSON.stringify([request.epoch,identity,kind,payload,resource.id,resource.sha256]).sha256_text()
	media.requests[identity] = request
	result.presentation.append({"kind":"opening_media_request","service":SERVICE,"request":request.duplicate(true)})
	return identity

func scene_reset(state: State, result: Session.FrameResult) -> void:
	finish_cd_reset(state, result)
	if not result.error.is_empty():
		return
	var media: Dictionary = _record(state)
	media.decoded.clear()
	media.actor_pending.clear()
	result.presentation.append({"kind":"opening_scene_reset"})

func finish_cd_reset(state: State, result: Session.FrameResult) -> void:
	if not _prepared:
		result.error = "opening media queue is not prepared"
		return
	var media: Dictionary = _record(state)
	media.epoch += 1
	media.next_id = 0
	media.requests.clear()
	media.background_pending.clear()
	media.actor_pending.clear()
	media.hud_pending = ""
	result.presentation.append({"kind":"opening_media_reset","service":SERVICE,"epoch":media.epoch})

func background_read(state: State, lba: int, slot: int, result: Session.FrameResult, retry: bool = false) -> void:
	var resource: Variant = _backgrounds.get(str(lba))
	if not resource is Dictionary or resource.get("slot") != slot or resource.get("decoded") != true:
		result.error = "source-backed opening background/decode is unavailable for LBA %x slot %d" % [lba,slot]
		return
	# Native two-buffer cache reuse needs a separate source-backed backend.
	# Never turn a native cache hit into an invented asynchronous read delay.
	if not retry:
		for previous: Dictionary in _record(state).requests.values():
			if previous.kind == "background" and previous.payload.lba == lba:
				result.error = "opening background cache reuse is not implemented for this resource"
				return
	var identity := _request(state,"background",{"lba":lba,"slot":slot,"retry":retry},resource,result)
	if not identity.is_empty():
		_record(state).background_pending[str(slot)] = identity

func actor_read(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	var resource: Variant = _actors.get(str(lba))
	if not resource is Dictionary or resource.get("sectors") != sectors or resource.get("destination") != destination:
		result.error = "source-backed opening actor composition is unavailable for LBA %x" % lba
		return
	var hidden_arrival: bool = resource.get("hidden_stage0_profile") is String and resource.hidden_stage0_profile == "o01_hidden.v1" and lba == 0xabab and sectors == 13 and destination == 0x80180000 and state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 1 and state.substep == 3
	if not resource.get("native_composition") is bool or (not resource.native_composition and not hidden_arrival):
		result.error = "source-backed visible actor composition is unavailable for LBA %x" % lba
		return
	var identity := _request(state,"actors",{"lba":lba,"sectors":sectors,"destination":destination},resource,result)
	if not identity.is_empty():
		_record(state).actor_pending[str(destination)] = identity

func hud_read_start(state: State, sectors: int, destination: int, lba: int, result: Session.FrameResult) -> void:
	if _hud.get("lba") != lba or _hud.get("sectors") != sectors or _hud.get("destination") != destination or _hud.get("native_composition") != true:
		result.error = "source-backed opening command HUD is unavailable"
		return
	var identity := _request(state,"hud",{"lba":lba,"sectors":sectors,"destination":destination},_hud,result)
	if not identity.is_empty():
		_record(state).hud_pending = identity

func cd_sync(state: State, _result: Session.FrameResult) -> int:
	if not _prepared:
		_result.error = "opening media queue is not prepared"
		return 0
	for request: Dictionary in _record(state).requests.values():
		if request.status == "pending":
			return 0
	return 1

func background_decode(state: State, slot: int, result: Session.FrameResult) -> int:
	var media: Dictionary = _record(state)
	var request: Variant = media.requests.get(media.background_pending.get(str(slot), ""))
	if not request is Dictionary or request.kind != "background" or request.status not in ["ready","retry"]:
		result.error = "opening background decode has no matching completed read"
		return -1
	if request.status == "retry":
		return -1
	var previous: Dictionary = media.decoded.get(str(slot), {"buffer":0})
	var buffer: int = 1 - int(previous.buffer)
	media.decoded[str(slot)] = {"resource_id":request.resource_id,"resource_sha256":request.resource_sha256,"buffer":buffer}
	result.presentation.append({"kind":"opening_background_select","slot":slot,"resource_id":request.resource_id,"buffer":buffer})
	return buffer

func actor_palette_setup(state: State, result: Session.FrameResult) -> void:
	var media: Dictionary = _record(state)
	var request: Variant = media.requests.get(media.actor_pending.get(str(0x80180000), ""))
	if not request is Dictionary or request.kind != "actors" or request.status != "ready":
		result.error = "arrival actor initialization has no completed native actor resource"
		return
	result.presentation.append({"kind":"opening_arrival_actor_setup","resource_id":request.resource_id})

func complete(event: Session.ServiceEvent, state: State, _rng, result: Session.FrameResult) -> void:
	if event == null or event.kind != Session.ServiceKind.COMPLETION or event.service != SERVICE or event.phase != Session.Phase.BEFORE_MODE:
		result.error = "opening media completion belongs to the wrong service phase"
		return
	var payload: Dictionary = event.payload
	if payload.size() != 5 or not payload.has_all(["epoch","request_id","request_key","status","resource_id"]):
		result.error = "opening media completion identity is malformed"
		return
	if not payload.epoch is int or payload.epoch < 0 or payload.epoch > 0xffffffff:
		result.error = "opening media completion epoch must be an exact native-range integer"
		return
	for key: String in ["request_id","request_key","status","resource_id"]:
		if not payload[key] is String or payload[key].is_empty() or payload[key].length() > 128:
			result.error = "opening media completion identity requires bounded strings"
			return
	if not state.opening.get("media") is Dictionary:
		result.error = "opening media completion has no transactional request owner"
		return
	var media: Dictionary = state.opening.media
	var request: Variant = media.requests.get(payload.request_id)
	if not request is Dictionary or request.status != "pending" or payload.epoch != media.epoch or payload.request_key != request.request_key or payload.resource_id != request.resource_id:
		result.error = "opening media completion is stale, duplicate or mismatched"
		return
	if payload.status != "ready" and not (payload.status == "retry" and request.kind == "background"):
		result.error = "opening media completion status is unsupported"
		return
	request.status = payload.status
