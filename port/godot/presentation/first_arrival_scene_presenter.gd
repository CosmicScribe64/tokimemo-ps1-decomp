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

func prepare(media: Media, text: Store, painter: TextPainter, initial_state: State) -> String:
	if _media != null: return "first-arrival presenter already owns its prepared resources"
	if media == null or media.resource_identity().is_empty() or text == null or text.initial_constants().is_empty() or painter == null or initial_state == null:
		return "first-arrival presenter needs original media, text, font and live owned profile"
	var line0 := text.resolve(0,initial_state.rule_state)
	var error: String = painter.preflight(line0)
	if not error.is_empty(): return error
	_media = media
	_text = text
	_painter = painter
	return ""

func render(state: State, checker: Callable, front: Callable, uploads_ready: Callable, previous_frame: Image = null) -> Dictionary:
	if _media == null or state == null or not checker.is_valid() or not front.is_valid(): return {"error":"original first-arrival HUD draw provider is unavailable"}
	var snapshot := state.projection()
	var view_state := State.new()
	view_state.restore(snapshot)
	var source: Variant = view_state.opening.get("arrival_media")
	if not source is Dictionary or not source.get("back_clear") is bool or not source.get("actor_initialized") is bool: return {"error":"first-arrival framebuffer/actor source state is unknown"}
	if view_state.opening.has("prologue_text_work") and not source.actor_initialized: return {"error":"first-arrival text cannot precede source actor setup"}
	if source.get("actor_initialized") == true:
		if not uploads_ready.is_valid() or not source.get("upload_batch_key") is String: return {"error":"first-arrival original texture/CLUT upload provider is unavailable"}
		var ready: Variant = uploads_ready.call(source.upload_batch_key)
		if not ready is String or not ready.is_empty(): return {"error":ready if ready is String else "unknown first-arrival upload readiness response"}
	var image: Image
	if source.back_clear:
		image = Image.create(320,240,false,Image.FORMAT_RGBA8)
		image.fill(Color.BLACK)
	else:
		if previous_frame == null or previous_frame.get_size() != Vector2i(320,240): return {"error":"first-arrival needs the prior committed framebuffer when clearing is disabled"}
		image = previous_frame.duplicate()
	var error := _draw(checker,image,view_state,snapshot)
	if error.is_empty(): error = _media.paint_background(image,view_state)
	if error.is_empty(): error = _draw(front,image,view_state,snapshot)
	if not error.is_empty(): return {"error":error}
	if view_state.opening.get("prologue_text_work") is Dictionary:
		var work: Dictionary = view_state.opening.prologue_text_work
		if not work.get("line_id") is int or work.line_id != 0: return {"error":"first-arrival presenter supports only the original first line"}
		var text_view := _text.resolve(0,view_state.rule_state)
		var brightness: Variant = view_state.opening.get("screen_brightness")
		if not brightness is int: return {"error":"first-arrival UI brightness has no owned value"}
		var drawn := _painter.render_into(image,text_view,work,brightness)
		if not str(drawn.get("error","")).is_empty(): return drawn
	if state.projection() != snapshot or view_state.projection() != snapshot: return {"error":"first-arrival drawing changed simulation state"}
	return {"error":"","image":image}

static func _draw(callback: Callable, target: Image, state: State, snapshot: Dictionary) -> String:
	var outcome: Variant = callback.call(target,state)
	if state.projection() != snapshot: return "first-arrival HUD renderer changed its read-only view state"
	if outcome is String: return outcome
	if outcome is Dictionary and outcome.get("error") is String: return outcome.error
	return "first-arrival HUD renderer returned an unknown result"
