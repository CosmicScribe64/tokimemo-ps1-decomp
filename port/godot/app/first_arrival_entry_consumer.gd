extends Node
## Minimal synchronous ownership consumer; application chooses scene activation.
## Keeping input/resources alive does not grant missing media/audio capability.
const EntryApp = preload("res://app/new_game_entry_boot.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Rng = preload("res://core/native_rng.gd")
const Pad = preload("res://ui/entry_pad.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Session = preload("res://core/game_session.gd")
const Interactive = preload("res://flow/interactive_prologue.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
signal entry_owned
var error := ""
var opening := Interactive.new()
var entry: Entry
var rng: Rng
var input_owner: Pad
var original_text: TextStore
var original_font: Glyphs
var previous_entry_frame: Image
var _old_scene: EntryApp
var _live: Dictionary = {}

func watch_entry(scene: EntryApp) -> String:
	if _old_scene != null or entry != null or scene == null or not is_inside_tree() or not scene.is_inside_tree() or scene.get_tree() != get_tree():
		return "first-arrival consumer needs one live entry scene in the same tree"
	if scene == self or scene.is_ancestor_of(self):
		return "first-arrival successor must live outside the entry subtree being released"
	_old_scene = scene
	scene.profile_finished.connect(_profile_finished)
	return ""

func _profile_finished(source_entry: Entry, source_rng: Rng, launch: Dictionary, live: Dictionary) -> void:
	if entry != null:
		# Repeated notification of the consumed handoff is idempotent. It must
		# not emit another activation signal, reparent or dispose either scene.
		if source_entry == entry and source_rng == rng and live.get("input_owner") == input_owner: return
		error = "another profile attempted to replace consumed first-arrival ownership"
		return
	error = accept_entry(_old_scene,source_entry,source_rng,launch,live)
	if error.is_empty(): entry_owned.emit()

func accept_entry(scene: EntryApp, source_entry: Entry, source_rng: Rng, launch: Dictionary, live: Dictionary) -> String:
	if entry != null or scene == null or not is_instance_valid(scene) or not is_inside_tree() or not scene.is_inside_tree() or scene.get_tree() != get_tree():
		return "first-arrival ownership can be consumed only once from a live entry scene"
	if scene == self or scene.is_ancestor_of(self):
		return "first-arrival successor cannot be freed with entry or descend from its pad"
	if source_entry == null or source_rng == null or scene.controller != source_entry or scene.rng != source_rng or source_entry.get("_rng") != source_rng or not scene.profile_complete or not scene.failure.is_empty() or not scene._completion_emitted:
		return "first-arrival ownership is not the actual committed entry application"
	if live.get("entry") != source_entry or live.get("rng") != source_rng or not live.get("input_owner") is Pad or not live.get("context") is Dictionary or not live.get("constants") is Dictionary or not live.get("provenance") is Dictionary:
		return "first-arrival live context is incomplete or belongs to another owner"
	var timer_policy: Variant = live.provenance.get("timer_observation_source")
	if not timer_policy is String or timer_policy not in ["desktop-one-timer-per-logic-v1","explicit-external-timer-observations-v1"] or scene.profile_handoff.get("provenance") != live.provenance:
		return "first-arrival timer provenance differs from the live entry owner"
	var pad: Pad = live.input_owner
	if not is_instance_valid(pad) or pad != scene._entry_pad or pad.get_parent() != scene or scene.profile_handoff.get("input_owner") != pad or scene.profile_handoff.get("context") != live.context:
		return "first-arrival must retain the exact entry-owned device and context"
	if scene.profile_text_store == null or scene.profile_text_store.initial_constants() != live.constants or launch != scene.installed_launch_profile:
		return "first-arrival original resource/launch ownership differs from entry"
	# Reparent synchronously, before the caller queues/free the old scene. Do
	# not recreate the adapter or clear/resample its held device table here.
	pad.reparent(self)
	if pad.get_parent() != self: return "entry input ownership could not move to the successor"
	entry = source_entry
	rng = source_rng
	input_owner = pad
	original_text = scene.profile_text_store
	original_font = scene.glyphs
	previous_entry_frame = scene.last_rendered_image
	_live = live.duplicate(true)
	_old_scene = scene
	return ""

func release_entry_scene() -> String:
	if entry == null or not is_instance_valid(input_owner) or input_owner.get_parent() != self or not is_instance_valid(_old_scene):
		return "old entry cannot be released before synchronous input/resource ownership transfer"
	_old_scene.queue_free()
	_old_scene = null
	return ""

func live_handoff() -> Dictionary:
	return _live.duplicate(true)

func begin_opening(services: Dictionary, participants: Array[RefCounted], publishers: Array[Callable]) -> String:
	if entry == null or not is_instance_valid(input_owner) or input_owner.get_parent() != self:
		return "first-arrival cannot begin without the live entry-owned pad and profile"
	return opening.begin(entry,rng,_live.context,_live.constants,services,participants,publishers)

func step(external_observations: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	var failed := Session.FrameResult.new()
	if opening.session == null or not is_instance_valid(input_owner):
		failed.error = "first-arrival logical session or retained input owner is unavailable"
		return failed
	var tick: int = opening.session.clock.logic_iterations
	var events: Array[Session.ServiceEvent] = external_observations.duplicate()
	for event: Session.ServiceEvent in events:
		if event == null:
			failed.error = "first-arrival service observation is null"
			return failed
	var policy: Variant = _live.provenance.get("timer_observation_source")
	if policy == "desktop-one-timer-per-logic-v1":
		for event: Session.ServiceEvent in events:
			if event.kind == Session.ServiceKind.TIMER_INTERRUPT:
				failed.error = "fixed desktop timer policy cannot double count external interrupts"
				return failed
		events.append(Session.ServiceEvent.new(tick,Session.Phase.DRAW_BEFORE_INPUT,Session.ServiceKind.TIMER_INTERRUPT))
	elif policy != "explicit-external-timer-observations-v1":
		failed.error = "first-arrival timer observation policy is missing"
		return failed
	var input: InputFrame = input_owner.sample(tick)
	return opening.step(input,events)
