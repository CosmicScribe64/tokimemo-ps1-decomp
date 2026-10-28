extends RefCounted
## Pure committed-state composition. Provider provenance/readiness belongs to root.
## No capture plate, fallback HUD, source advancement or upload-ready assertion.
const State = preload("res://rules/new_game_route_state.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Store = preload("res://services/prologue_text_store.gd")
const TextPainter = preload("res://presentation/prologue_text_presenter.gd")
var _media: Media
var _text: Store
var _painter: TextPainter
var _actor: RefCounted
var _last_text_key: String = ""
var _last_text_region: Image

func prepare(media: Media, text: Store, painter: TextPainter, initial_state: State, actor_renderer: RefCounted = null) -> String:
	if _media != null: return "first-arrival presenter already owns its prepared resources"
	if media == null or media.resource_identity().is_empty() or text == null or text.initial_constants().is_empty() or painter == null or initial_state == null:
		return "first-arrival presenter needs original media, text, font and live owned profile"
	for line_idx in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]:
		var resolved_line := text.resolve(line_idx,initial_state.rule_state)
		var error: String = painter.preflight(resolved_line)
		if not error.is_empty(): return error
	_media = media
	_text = text
	_painter = painter
	_actor = actor_renderer
	return ""

func render(state: State, checker: Callable, front: Callable, uploads_ready: Callable, previous_frame: Image = null) -> Dictionary:
	if _media == null or state == null or not checker.is_valid() or not front.is_valid(): return {"error":"original first-arrival HUD draw provider is unavailable"}
	var opening_snapshot: Dictionary = state.opening.duplicate(true)
	var frame_snapshot: int = state.frame_count
	var snapshot := {"opening": opening_snapshot, "frame_count": frame_snapshot}
	var view_state := State.new()
	view_state.game_mode = state.game_mode
	view_state.mode_stage = state.mode_stage
	view_state.mode_step = state.mode_step
	view_state.substep = state.substep
	view_state.frame_count = state.frame_count
	view_state.rule_state = state.rule_state
	view_state.opening = opening_snapshot.duplicate(true)
	var source: Variant = view_state.opening.get("arrival_media")
	if not source is Dictionary or not source.get("back_clear") is bool or not source.get("actor_initialized") is bool: return {"error":"first-arrival framebuffer/actor source state is unknown"}
	if view_state.opening.has("prologue_text_work") and not source.actor_initialized: return {"error":"first-arrival text cannot precede source actor setup"}
	if source.get("actor_initialized") == true:
		if not uploads_ready.is_valid() or not source.get("upload_batch_key") is String: return {"error":"first-arrival original texture/CLUT upload provider is unavailable"}
		var ready: Variant = uploads_ready.call(source.upload_batch_key)
		if not ready is String or not ready.is_empty(): return {"error":ready if ready is String else "unknown first-arrival upload readiness response"}
	var pre_cut_board: bool = view_state.mode_stage == 1 or (view_state.mode_stage == 2 and view_state.substep <= 5)
	if pre_cut_board and view_state.opening.get("board_media") is Dictionary and view_state.opening.board_media.get("decoded") == true:
		var board_key: Variant = view_state.opening.board_media.get("upload_batch_key")
		if not uploads_ready.is_valid() or not board_key is String: return {"error":"first-arrival board upload provider is unavailable"}
		var board_ready: Variant = uploads_ready.call(board_key)
		if not board_ready is String or not board_ready.is_empty(): return {"error":board_ready if board_ready is String else "unknown first-arrival board upload readiness response"}
	if view_state.opening.get("names_media") is Dictionary:
		for buf_key: String in ["buffer_0", "buffer_1"]:
			if view_state.opening.names_media.get(buf_key) is Dictionary:
				var names_key: Variant = view_state.opening.names_media[buf_key].get("upload_batch_key")
				if not uploads_ready.is_valid() or not names_key is String: return {"error":"first-arrival names background upload provider is unavailable"}
				var names_ready: Variant = uploads_ready.call(names_key)
				if not names_ready is String or not names_ready.is_empty(): return {"error":names_ready if names_ready is String else "unknown first-arrival names upload readiness response"}
	if view_state.opening.get("bg_layer_0") is Dictionary:
		var cut_key: Variant = view_state.opening.bg_layer_0.get("cut_batch_key")
		if not uploads_ready.is_valid() or not cut_key is String: return {"error":"first-arrival cut upload provider is unavailable"}
		var cut_ready: Variant = uploads_ready.call(cut_key)
		if not cut_ready is String or not cut_ready.is_empty(): return {"error":cut_ready if cut_ready is String else "unknown first-arrival cut upload readiness response"}
	for actor_key: String in ["actor96", "actor97", "actor98", "actor99", "actor100", "actor101", "actor102", "actor103"]:
		if view_state.opening.get(actor_key) is Dictionary and view_state.opening[actor_key].has("upload_batch_key"):
			var sa_key: Variant = view_state.opening[actor_key].get("upload_batch_key")
			if not uploads_ready.is_valid() or not sa_key is String: return {"error":"first-arrival stage actor upload provider is unavailable"}
			var sa_ready: Variant = uploads_ready.call(sa_key)
			if not sa_ready is String or not sa_ready.is_empty(): return {"error":sa_ready if sa_ready is String else "unknown first-arrival stage actor upload readiness response"}
			if view_state.opening[actor_key].has("upload_batch_key_2"):
				var sa_key2: Variant = view_state.opening[actor_key].get("upload_batch_key_2")
				if not sa_key2 is String: return {"error":"first-arrival stage actor upload provider is unavailable"}
				var sa_ready2: Variant = uploads_ready.call(sa_key2)
				if not sa_ready2 is String or not sa_ready2.is_empty(): return {"error":sa_ready2 if sa_ready2 is String else "unknown first-arrival stage actor upload readiness response"}
			if view_state.opening[actor_key].has("upload_batch_key_3"):
				var sa_key3: Variant = view_state.opening[actor_key].get("upload_batch_key_3")
				if not sa_key3 is String: return {"error":"first-arrival stage actor upload provider is unavailable"}
				var sa_ready3: Variant = uploads_ready.call(sa_key3)
				if not sa_ready3 is String or not sa_ready3.is_empty(): return {"error":sa_ready3 if sa_ready3 is String else "unknown first-arrival stage actor upload readiness response"}
	var image: Image
	if source.back_clear:
		image = Image.create(320,240,false,Image.FORMAT_RGBA8)
		image.fill(Color.BLACK)
	else:
		if previous_frame == null or previous_frame.get_size() != Vector2i(320,240): return {"error":"first-arrival needs the prior committed framebuffer when clearing is disabled"}
		image = previous_frame.duplicate()
	var error := _draw(checker,image,view_state,snapshot)
	if error.is_empty(): error = _media.paint_background(image,view_state,_actor)
	if error.is_empty() and (view_state.opening.get("actor96_draw_flags") == 0xa4 or view_state.opening.get("actor97_draw_flags") == 0xa4 or view_state.opening.get("actor99_draw_flags") == 0xa4 or view_state.opening.get("prologue_petals") is Array):
		if _actor == null or not _actor.has_method("paint"): error = "visible arrival has no prepared original actor painter"
		else: error = _actor.paint(image,view_state,Vector2i(160,120))
	if error.is_empty(): error = _draw(front,image,view_state,snapshot)
	if not error.is_empty(): return {"error":error}
	var msg_visible: bool = bool(view_state.opening.get("prologue_hud_live",{}).get("message_visible",true)) and not bool(view_state.opening.get("prologue_text_cleared",false))
	if msg_visible and view_state.opening.get("prologue_text_work") is Dictionary:
		var work: Dictionary = view_state.opening.prologue_text_work
		var line_id: Variant = work.get("line_id")
		var valid_line1: bool = line_id == 1 and (view_state.mode_stage == 1 or (view_state.mode_stage == 2 and view_state.substep == 0 and view_state.opening.get("names_media",{}).get("active_draw") == "board_scroll_transition"))
		var valid_line2: bool = line_id == 2 and (view_state.mode_stage == 2 or (view_state.mode_stage == 3 and view_state.substep == 0 and not view_state.opening.has("bg_layer_0")))
		var valid_line3: bool = line_id == 3 and (view_state.mode_stage == 3 or (view_state.mode_stage == 4 and view_state.substep == 0))
		var valid_line4: bool = line_id == 4 and view_state.mode_stage == 4
		var valid_line5_10: bool = line_id in [5, 6, 7, 8, 9, 10] and (view_state.mode_stage == 6 or (view_state.mode_stage == 7 and view_state.substep == 0))
		var valid_line11: bool = line_id == 11 and view_state.mode_stage == 7
		if not line_id is int or (line_id != 0 and not valid_line1 and not valid_line2 and not valid_line3 and not valid_line4 and not valid_line5_10 and not valid_line11): return {"error":"first-arrival presenter supports only the original first twelve lines"}
		var text_view := _text.resolve(int(line_id),view_state.rule_state)
		var brightness: Variant = view_state.opening.get("screen_brightness")
		if not brightness is int: return {"error":"first-arrival UI brightness has no owned value"}
		var text_key := "%s:%s:%s:%s:%s" % [str(line_id), str(work.get("pos")), str(work.get("total")), str(brightness), JSON.stringify(work.get("entries", []))]
		if _last_text_region != null and _last_text_key == text_key and text_view.get("error", "") == "":
			image.blit_rect(_last_text_region, Rect2i(0, 0, 266, 52), Vector2i(27, 166))
		else:
			var drawn := _painter.render_into(image,text_view,work,brightness)
			if not str(drawn.get("error","")).is_empty(): return drawn
			_last_text_region = image.get_region(Rect2i(27, 166, 266, 52))
			_last_text_key = text_key
	if state.opening != snapshot.opening or view_state.opening != snapshot.opening or state.frame_count != snapshot.frame_count or view_state.frame_count != snapshot.frame_count: return {"error":"first-arrival drawing changed simulation state"}
	return {"error":"","image":image}

static func _draw(callback: Callable, target: Image, state: State, snapshot: Dictionary) -> String:
	var outcome: Variant = callback.call(target,state)
	if state.opening != snapshot.opening or state.frame_count != snapshot.frame_count: return "first-arrival HUD renderer changed its read-only view state"
	if outcome is String: return outcome
	if outcome is Dictionary and outcome.get("error") is String: return outcome.error
	return "first-arrival HUD renderer returned an unknown result"
