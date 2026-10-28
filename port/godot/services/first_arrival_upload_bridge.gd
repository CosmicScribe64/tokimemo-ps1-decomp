extends RefCounted
## T-29025: actual completed actor read -> source CPU/queue -> word pump.
## Preflight uses local copies; caller buffers change only after frame commit.
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Producer = preload("res://presentation/prologue_actor_upload_provider.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const APPROVAL_META := &"first_arrival_upload_preflight_v1"
var _media: Media
var _store: Store
var _board_store: BoardStore
var _owned: Dictionary
var _session: WeakRef
var _producer := Producer.new()
var _pump := Pump.new()
var _batch_key := ""
var _board_batch_key := ""
var _names_bg_batch_keys: Dictionary = {}
var _cut_batch_keys: Dictionary = {}
var _stage_actor_batch_keys: Dictionary = {}
var _ready_cache: Dictionary = {}
var _ready_cache_spot: Array = []
var _last_published_tick := -1

func prepare(media: Media, store: Store, owned_runtime: Dictionary, board_store: BoardStore = null) -> String:
	if _media != null: return "first-arrival upload bridge already owns its prepared runtime"
	if media == null or store == null or store.identity().is_empty() or media.resource_identity() != store.identity():
		return "first-arrival upload bridge needs its actual authenticated media owner"
	if board_store != null and media.board_resource_identity() != BoardStore.MANIFEST_SHA:
		return "first-arrival upload bridge needs its media owner's authenticated board store"
	if not Pump._owners(owned_runtime) or not owned_runtime.get("scratch_words") is PackedByteArray or owned_runtime.scratch_words.size() != 512 or not owned_runtime.get("pending_gpu_effects") is Array or not owned_runtime.pending_gpu_effects.is_empty():
		return "first-arrival upload bridge needs explicit scratch/VRAM buffers and an empty owned queue"
	if owned_runtime.has("vram_known_mask") and (not owned_runtime.vram_known_mask is PackedByteArray or owned_runtime.vram_known_mask.size() != Pump.VRAM_BYTES):
		return "first-arrival upload bridge received a malformed VRAM ownership mask"
	_media = media
	_store = store
	_board_store = board_store
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
		var has_uploads := _has_upload_intents(result)
		result.set_meta(APPROVAL_META,{"session_id":owner.get_instance_id(),"tick":owner.clock.logic_iterations,
			"effects_hash":_effects_hash(result),"runtime_hash":_runtime_hash(_owned) if has_uploads else _fast_runtime_hash(_owned)})
	return ""

func publish(result: Session.FrameResult, tick: int) -> String:
	var owner := _owner()
	var has_uploads := _has_upload_intents(result)
	if owner == null or not result.committed or tick < 0 or tick <= _last_published_tick or tick != owner.clock.logic_iterations-1 or result.state_hash.is_empty() or (has_uploads and result.state_hash != owner.state_hash()):
		return "upload publication requires the current committed owning Session frame"
	var approval: Variant = result.get_meta(APPROVAL_META,null)
	if not approval is Dictionary or not Store._same(approval,{"session_id":owner.get_instance_id(),"tick":tick,"effects_hash":_effects_hash(result),"runtime_hash":_runtime_hash(_owned) if has_uploads else _fast_runtime_hash(_owned)}):
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
	elif preview.has("board_batch_key"):
		var next: Dictionary = _owned.duplicate(true)
		next["board_runtime_bytes"] = preview.board_bytes.duplicate()
		var pumped_board := _pump.execute_board(_board_store,preview.board_batch_key,next)
		if not pumped_board.error.is_empty(): return pumped_board.error
		_owned.clear()
		_owned.merge(next)
		_board_batch_key = preview.board_batch_key
	elif preview.has("names_bg_batch_key") or preview.has("cut_batch_key") or preview.has("stage_actor_batches"):
		var next: Dictionary = _owned.duplicate(true)
		if preview.has("names_bg_batch_key"):
			next["names_bg_runtime_bytes"] = preview.names_bg_bytes.duplicate()
			var pumped_bg := _pump.execute_names_bg(_board_store,preview.names_bg_resource_id,preview.names_bg_batch_key,next)
			if not pumped_bg.error.is_empty(): return pumped_bg.error
		if preview.has("cut_batch_key"):
			var pumped_cut := _pump.execute_cut_vram(_board_store,preview.cut_resource_id,preview.cut_batch_key,next)
			if not pumped_cut.error.is_empty(): return pumped_cut.error
		if preview.has("stage_actor_batches"):
			for sa_item: Dictionary in preview.stage_actor_batches:
				var pumped_sa := _pump.execute_stage_actor_vram(_board_store,sa_item.resource_id,sa_item.slot,sa_item.source_offset,sa_item.batch_key,next)
				if not pumped_sa.error.is_empty(): return pumped_sa.error
		_owned.clear()
		_owned.merge(next)
		if preview.has("names_bg_batch_key"):
			_names_bg_batch_keys[preview.names_bg_batch_key] = preview.names_bg_resource_id
		if preview.has("cut_batch_key"):
			_cut_batch_keys[preview.cut_batch_key] = preview.cut_resource_id
		if preview.has("stage_actor_batches"):
			for sa_item: Dictionary in preview.stage_actor_batches:
				_stage_actor_batch_keys[sa_item.batch_key] = sa_item.resource_id
	if has_uploads:
		_ready_cache.clear()
		_ready_cache_spot.clear()
	_last_published_tick = tick
	return ""

func _current_ready_spot() -> Array:
	var vram: Variant = _owned.get("vram_words")
	var known: Variant = _owned.get("vram_known_mask")
	var scratch: Variant = _owned.get("scratch_words")
	if not vram is PackedByteArray or vram.size() != Pump.VRAM_BYTES or not known is PackedByteArray or known.size() != Pump.VRAM_BYTES or not scratch is PackedByteArray or scratch.size() != 512:
		return []
	return [
		_owned.get("vram_owner"),
		_owned.get("scratch_owner"),
		scratch[0],
		vram[(256*1024+640)*2],
		known[(256*1024+640)*2],
		vram[(0*1024+960)*2],
		known[(0*1024+960)*2],
		vram[(496*1024+1)*2],
	]

func batch_ready(batch_key: String) -> String:
	var spot := _current_ready_spot()
	if spot.is_empty() or spot != _ready_cache_spot:
		_ready_cache.clear()
		_ready_cache_spot = spot
	elif _ready_cache.has(batch_key):
		return str(_ready_cache[batch_key])
	var res := ""
	if not batch_key.is_empty() and _stage_actor_batch_keys.has(batch_key):
		res = _pump.stage_actor_batch_ready(batch_key,_owned)
	elif not batch_key.is_empty() and _cut_batch_keys.has(batch_key):
		res = _pump.cut_batch_ready(batch_key,_owned)
	elif not batch_key.is_empty() and _names_bg_batch_keys.has(batch_key):
		res = _pump.names_bg_batch_ready(batch_key,_owned)
	elif not batch_key.is_empty() and batch_key == _board_batch_key:
		res = _pump.board_batch_ready(batch_key,_owned)
	elif batch_key != _batch_key or _batch_key.is_empty():
		return "actual first-arrival upload batch has not executed"
	else:
		res = _pump.batch_ready(batch_key,_owned)
	if not spot.is_empty() and res.is_empty():
		_ready_cache[batch_key] = res
	return res

func actor_render_context(store: Store) -> Dictionary:
	# Return the actual dictionary this producer/pump owns, never a caller's
	# same-shaped copy. Renderer rechecks reference identity before sampling.
	if _store == null or store == null or _store != store or _media == null or _media.resource_identity() != store.identity() or not Pump._owners(_owned):
		return {"error":"actor renderer requires this upload owner's exact prepared store/runtime"}
	return {"error":"","runtime":_owned}

func diagnostics() -> Dictionary:
	return {"batch_key":_batch_key,"batch_complete":not _batch_key.is_empty(),
		"board_batch_key":_board_batch_key,"board_batch_complete":not _board_batch_key.is_empty(),
		"names_bg_batches":_names_bg_batch_keys.duplicate(),
		"cut_batches":_cut_batch_keys.duplicate(),
		"stage_actor_batches":_stage_actor_batch_keys.duplicate(),
		"vram_ready":false,"publication_tick":_last_published_tick,
		"pending_gpu_effects":_owned.get("pending_gpu_effects",[]).size()}

func _owner() -> Session:
	return _session.get_ref() as Session if _session != null else null

func _preview(result: Session.FrameResult, state: State) -> Dictionary:
	var uploads: Array[Dictionary] = []
	var board_uploads: Array[Dictionary] = []
	var names_uploads: Array[Dictionary] = []
	var cut_uploads: Array[Dictionary] = []
	var stage_actor_uploads: Array[Dictionary] = []
	var o12_restore_seen := false
	var o06_copy_seen := false
	var o07_copy_seen := false
	for effect: Dictionary in result.presentation:
		if effect.get("kind") == "first_arrival_native_upload": uploads.append(effect)
		elif effect.get("kind") == "first_arrival_board_upload": board_uploads.append(effect)
		elif effect.get("kind") == "first_arrival_names_bg_upload": names_uploads.append(effect)
		elif effect.get("kind") == "first_arrival_cut_upload": cut_uploads.append(effect)
		elif effect.get("kind") == "first_arrival_stage_actor_upload": stage_actor_uploads.append(effect)
		elif effect.get("kind") == "first_arrival_o12_restore": o12_restore_seen = true
		elif effect.get("kind") == "first_arrival_o06_copy": o06_copy_seen = true
		elif effect.get("kind") == "first_arrival_o07_copy": o07_copy_seen = true
	var stage_vram: int = 1 if (not names_uploads.is_empty() or not cut_uploads.is_empty() or not stage_actor_uploads.is_empty()) else 0
	var batch_types := (1 if not uploads.is_empty() else 0) + (1 if not board_uploads.is_empty() else 0) + stage_vram
	if batch_types > 1:
		return {"error":"actor, board and names background upload batches cannot execute in the same frame"}
	if stage_vram == 1:
		if _board_store == null: return {"error":"stage cut/actor/names upload requires an authenticated board store"}
		if _batch_key.is_empty() or _board_batch_key.is_empty(): return {"error":"stage cut/actor/names upload requires prior actor and board VRAM ownership"}
		var cs_next := _owned.duplicate(true)
		if not cs_next.get("pending_gpu_effects") is Array or not cs_next.pending_gpu_effects.is_empty():
			return {"error":"actual stage cut/actor/names upload requires an empty native queue"}
		var out_preview: Dictionary = {"error":""}
		var temp_pump := Pump.new()
		if not names_uploads.is_empty():
			if names_uploads.size() != 1: return {"error":"actual names background upload requires one atomic decode batch"}
			var bg_item: Dictionary = names_uploads[0]
			var resource_id: String = str(bg_item.get("resource_id",""))
			var buf_key: String = "buffer_0" if bg_item.get("buffer") == 0 else "buffer_1"
			var names_source: Variant = state.opening.get("names_media")
			if not names_source is Dictionary or not names_source.get(buf_key) is Dictionary or not names_source[buf_key].get("upload_batch_key") is String:
				return {"error":"actual names background upload batch has no logical owner"}
			var names_batch_key: String = names_source[buf_key].upload_batch_key
			if _names_bg_batch_keys.has(names_batch_key):
				return {"error":"first-arrival names background upload batch has already executed"}
			if bg_item.get("batch_key") != names_batch_key:
				return {"error":"actual names background upload received a foreign or malformed operation"}
			var bg_transfer := _media.completed_names_bg_buffer(state,resource_id)
			if not bg_transfer.error.is_empty(): return bg_transfer
			cs_next["names_bg_runtime_bytes"] = bg_transfer.bytes.duplicate()
			var bg_validation := temp_pump.execute_names_bg(_board_store,resource_id,names_batch_key,cs_next)
			if not bg_validation.error.is_empty(): return bg_validation
			out_preview["names_bg_bytes"] = bg_transfer.bytes
			out_preview["names_bg_resource_id"] = resource_id
			out_preview["names_bg_batch_key"] = names_batch_key
		if not cut_uploads.is_empty():
			if cut_uploads.size() != 1: return {"error":"actual cut upload requires one atomic batch"}
			var cut_item: Dictionary = cut_uploads[0]
			var cut_id: String = str(cut_item.get("resource_id",""))
			var cut_src: Variant = state.opening.get("cut_media")
			if not cut_src is Dictionary or cut_src.get("upload_batch_key") != cut_item.get("batch_key"):
				return {"error":"actual cut upload batch has no logical owner"}
			var cut_key: String = str(cut_src.upload_batch_key)
			if _cut_batch_keys.has(cut_key):
				return {"error":"first-arrival cut upload batch has already executed"}
			var cut_transfer := _media.completed_cut_buffer(state,cut_id)
			if not cut_transfer.error.is_empty(): return cut_transfer
			var cut_val := temp_pump.execute_cut_vram(_board_store,cut_id,cut_key,cs_next)
			if not cut_val.error.is_empty(): return cut_val
			out_preview["cut_resource_id"] = cut_id
			out_preview["cut_batch_key"] = cut_key
		if not stage_actor_uploads.is_empty():
			if stage_actor_uploads.size() > 3: return {"error":"actual stage actor upload exceeds max 3 slots per frame"}
			var sa_batches: Array[Dictionary] = []
			for sa_item: Dictionary in stage_actor_uploads:
				var sa_id: String = str(sa_item.get("resource_id",""))
				var sa_slot: int = int(sa_item.get("slot",-1))
				var sa_off: int = int(sa_item.get("source_offset",-1))
				var sa_key: String = str(sa_item.get("batch_key",""))
				if _stage_actor_batch_keys.has(sa_key):
					return {"error":"first-arrival stage actor upload batch has already executed"}
				var owner_ok := false
				if sa_id == BoardStore.O_06_ID:
					if sa_slot == 5:
						var a96_5: Variant = state.opening.get("actor96")
						owner_ok = a96_5 is Dictionary and a96_5.get("upload_batch_key") == sa_key
					elif sa_slot == 6:
						var a96_6: Variant = state.opening.get("actor96")
						owner_ok = a96_6 is Dictionary and a96_6.get("upload_batch_key_2") == sa_key
					elif sa_slot == 7:
						var a99_7: Variant = state.opening.get("actor99")
						owner_ok = a99_7 is Dictionary and a99_7.get("upload_batch_key") == sa_key
				elif sa_id == BoardStore.O_07_ID:
					var a96_o07: Variant = state.opening.get("actor96")
					if sa_slot == 5:
						owner_ok = a96_o07 is Dictionary and a96_o07.get("upload_batch_key") == sa_key
					elif sa_slot == 6:
						owner_ok = a96_o07 is Dictionary and a96_o07.get("upload_batch_key_2") == sa_key
					elif sa_slot == 7:
						owner_ok = a96_o07 is Dictionary and a96_o07.get("upload_batch_key_3") == sa_key
				else:
					var actor_dict: Variant = state.opening.get("actor96" if sa_slot == 5 else "actor97")
					owner_ok = actor_dict is Dictionary and actor_dict.get("upload_batch_key") == sa_key
				if not owner_ok:
					return {"error":"actual stage actor upload batch has no logical owner"}
				var sa_transfer := _media.completed_stage_actor_buffer(state,sa_id,o12_restore_seen or o06_copy_seen or o07_copy_seen)
				if not sa_transfer.error.is_empty(): return sa_transfer
				var sa_val := temp_pump.execute_stage_actor_vram(_board_store,sa_id,sa_slot,sa_off,sa_key,cs_next)
				if not sa_val.error.is_empty(): return sa_val
				sa_batches.append({"resource_id":sa_id,"slot":sa_slot,"source_offset":sa_off,"batch_key":sa_key})
			out_preview["stage_actor_batches"] = sa_batches
		return out_preview
	if not board_uploads.is_empty():
		if _board_store == null: return {"error":"board upload requires an authenticated board store"}
		if _batch_key.is_empty(): return {"error":"board upload requires prior actor VRAM ownership"}
		if not _board_batch_key.is_empty(): return {"error":"first-arrival board upload batch has already executed"}
		if board_uploads.size() != 1: return {"error":"actual board upload requires one atomic decode+move batch"}
		var board_source: Variant = state.opening.get("board_media")
		if not board_source is Dictionary or not board_source.get("upload_batch_key") is String:
			return {"error":"actual board upload batch has no logical owner"}
		var board_batch_key: String = board_source.upload_batch_key
		var item: Dictionary = board_uploads[0]
		if item.get("batch_key") != board_batch_key or not Store._same(item.get("decode_rect"),[320,0,640,160]) or not Store._same(item.get("move_call"),[5,14,15]) or item.get("word_sha256") != BoardStore.SOFTWARE_WORD_SHA:
			return {"error":"actual board upload received a foreign or malformed operation"}
		var board_transfer := _media.completed_board_buffer(state)
		if not board_transfer.error.is_empty(): return board_transfer
		var board_next := _owned.duplicate(true)
		if not board_next.get("pending_gpu_effects") is Array or not board_next.pending_gpu_effects.is_empty():
			return {"error":"actual board upload requires an empty native queue"}
		board_next["board_runtime_bytes"] = board_transfer.bytes.duplicate()
		var board_validation := Pump.new().execute_board(_board_store,board_batch_key,board_next)
		if not board_validation.error.is_empty(): return board_validation
		return {"error":"","board_bytes":board_transfer.bytes,"board_batch_key":board_batch_key}
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

static func _has_upload_intents(result: Session.FrameResult) -> bool:
	for effect: Dictionary in result.presentation:
		if effect.get("kind") in Media.OWN_INTENTS:
			return true
	return false

static func _fast_runtime_hash(owned: Dictionary) -> String:
	var scratch: Variant = owned.get("scratch_words")
	var pending: Variant = owned.get("pending_gpu_effects")
	return JSON.stringify({
		"scratch_owner":owned.get("scratch_owner"),
		"vram_owner":owned.get("vram_owner"),
		"scratch":Store._sha(scratch) if scratch is PackedByteArray else scratch,
		"pending":pending
	}).sha256_text()

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
