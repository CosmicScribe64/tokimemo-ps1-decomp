extends "res://tests/run_interactive_prologue.gd"
## Actual EntryApp/child-pad teardown; queued UI actions are a bounded fixture,
## not OS input evidence or a normal-start first-scene acceptance run.
const EntryApp = preload("res://app/new_game_entry_boot.gd")
const Consumer = preload("res://app/first_arrival_entry_consumer.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
var ownership_events := 0

func rejected_handoffs(source_entry: Entry, source_rng: Rng, launch: Dictionary, live: Dictionary, old_entry: EntryApp, probe: Consumer) -> void:
	for defect: String in ["missing_context","foreign_rng","changed_constants","changed_context","changed_launch","missing_timer_policy","foreign_provenance"]:
		var rejected := live.duplicate(true)
		var rejected_launch := launch.duplicate(true)
		match defect:
			"missing_context": rejected.erase("context")
			"foreign_rng": rejected.rng = Rng.new()
			"changed_constants": rejected.constants = {}
			"changed_context": rejected.context = {}
			"changed_launch": rejected_launch = {}
			"missing_timer_policy": rejected.provenance.erase("timer_observation_source")
			"foreign_provenance": rejected.provenance.clock_origin = "foreign fixture origin"
		var rng_before: Dictionary = source_rng.checkpoint()
		check(not probe.accept_entry(old_entry,source_entry,source_rng,rejected_launch,rejected).is_empty() and probe.entry == null and old_entry._entry_pad.get_parent() == old_entry and is_instance_valid(old_entry.profile_text_store) and source_rng.checkpoint() == rng_before,"failed handoff leaves entry pad/resources/scene and RNG unchanged: "+defect)
	for under_pad: bool in [false,true]:
		var unsafe := Consumer.new()
		var parent: Node = old_entry._entry_pad if under_pad else old_entry
		parent.add_child(unsafe)
		check(not unsafe.watch_entry(old_entry).is_empty() and not unsafe.accept_entry(old_entry,source_entry,source_rng,launch,live).is_empty() and unsafe.entry == null and unsafe.get_parent() == parent and unsafe.is_inside_tree() and old_entry._entry_pad.get_parent() == old_entry and is_instance_valid(old_entry.profile_text_store),"unsafe successor ancestry rejects before half-reparent/disposal: "+str(under_pad))
		unsafe.queue_free()

func _initialize() -> void:
	call_deferred("run_lifetime")

func dispatch(app: EntryApp, action: Dictionary = {}) -> void:
	if not action.is_empty(): app._send(action)
	app._physics_process(1.0/60.0)

func key_event(held: bool) -> InputEventKey:
	var event := InputEventKey.new()
	event.keycode = KEY_ENTER
	event.physical_keycode = KEY_ENTER
	event.pressed = held
	return event

func stop_if_failed() -> bool:
	if failures == 0: return false
	printerr("entry_consumer_lifetime stopped at first failed prerequisite; failures=",failures)
	quit(1)
	return true

func run_lifetime() -> void:
	var install_root := OS.get_environment("ENTRY_INSTALL_ROOT")
	if install_root.is_empty() or not FileAccess.file_exists(install_root.path_join("manifest.json")):
		check(false,"ENTRY_INSTALL_ROOT must name an existing installed bundle with manifest.json")
		stop_if_failed(); return
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(install_root.path_join("manifest.json")))
	if not manifest is Dictionary:
		check(false,"installed manifest is not a JSON object")
		stop_if_failed(); return
	var profile: Dictionary = Profile.read_pack(install_root,manifest)
	check(profile.get("valid",false),"original installed entry profile authenticates: "+str(profile.get("error","")))
	if stop_if_failed(): return
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	var text := TextStore.new()
	var text_error := text.open(text_root,"5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc")
	check(text_error.is_empty(),"original mascot/text resource authenticates: "+text_error)
	if stop_if_failed(): return
	var old_entry := EntryApp.new()
	old_entry.installed_launch_profile = profile
	old_entry.profile_text_store = text
	old_entry.entry_clock_policy = {"clock_origin":"desktop-fresh-entry-local; native boot/title prefix unmeasured",
		"timer_observation_source":"desktop-one-timer-per-logic-v1","native_clock_parity":false}
	root.add_child(old_entry)
	old_entry.set_physics_process(false)
	check(old_entry.failure.is_empty(),"actual entry app initializes from installed original resources: "+old_entry.failure)
	if stop_if_failed(): return
	var successor := Consumer.new()
	root.add_child(successor)
	var probe := Consumer.new()
	root.add_child(probe)
	var rejected_callback: Callable = rejected_handoffs.bind(old_entry,probe)
	old_entry.profile_finished.connect(rejected_callback)
	check(successor.watch_entry(old_entry).is_empty(),"minimal signal consumer watches actual entry scene")
	if stop_if_failed(): return
	successor.entry_owned.connect(func() -> void: ownership_events += 1)
	check(old_entry._entry_pad.get_parent() == old_entry,"pad really begins as a child of the scene that will be freed")
	# Select an actual source code unit for each name, then confirm each form.
	dispatch(old_entry)
	for _part in range(3):
		dispatch(old_entry,{"kind":"cell","column":0,"row":0})
		dispatch(old_entry,{"kind":"start"})
		dispatch(old_entry)
	for _field in range(3):
		dispatch(old_entry,{"kind":"select","value":0})
		dispatch(old_entry,{"kind":"circle"})
	dispatch(old_entry)
	dispatch(old_entry,{"kind":"start"})
	dispatch(old_entry)
	for _field in range(3):
		dispatch(old_entry,{"kind":"select","value":0})
		dispatch(old_entry,{"kind":"circle"})
	dispatch(old_entry)
	check(old_entry.controller.stage == 2 and old_entry.controller.step_id == 3,"queued UI fixture reaches actual final confirmation")
	check(old_entry.failure.is_empty(),"entry dispatch: "+old_entry.failure)
	if stop_if_failed(): return
	old_entry._entry_pad._input(key_event(true))
	old_entry._input(key_event(true))
	dispatch(old_entry)
	var pad_weak: WeakRef = weakref(old_entry._entry_pad)
	var entry_weak: WeakRef = weakref(old_entry.controller)
	var rng_weak: WeakRef = weakref(old_entry.rng)
	var text_weak: WeakRef = weakref(old_entry.profile_text_store)
	var font_weak: WeakRef = weakref(old_entry.glyphs)
	var old_model_weak: WeakRef = weakref(old_entry._entry_model)
	var old_clock_weak: WeakRef = weakref(old_entry._entry_clock)
	dispatch(old_entry)
	old_entry.profile_finished.disconnect(rejected_callback)
	check(successor.error.is_empty() and successor.entry != null,"real profile_finished signal consumes live entry context")
	if stop_if_failed(): return
	check(successor.input_owner == pad_weak.get_ref() and successor.input_owner.get_parent() == successor,"same pad is synchronously reparented before deletion")
	var live: Dictionary = successor.live_handoff()
	var retained_rng: Dictionary = successor.rng.checkpoint()
	var retained_profile := successor.entry.owned_projection()
	var retained_context: Dictionary = live.context.duplicate(true)
	check(live.context.held_input == 0x800 and live.context.timer_ticks == old_entry._entry_model.timer_ticks and live.context.frame_count == old_entry._entry_model.frame_count,"observed held START, next frame and timer counters survive actual commit")
	check(not successor.accept_entry(old_entry,successor.entry,successor.rng,profile,live).is_empty(),"duplicate ownership consumption rejected")
	old_entry.profile_finished.emit(successor.entry,successor.rng,profile,live)
	check(ownership_events == 1 and successor.error.is_empty() and is_instance_valid(old_entry) and successor.input_owner.get_parent() == successor,"duplicate profile_finished is idempotent without disposal or reparent")
	check(not successor.begin_opening({},[],[]).is_empty() and successor.opening.session == null and successor.rng.checkpoint() == retained_rng and successor.entry.owned_projection() == retained_profile and is_instance_valid(old_entry),"missing required providers cannot advance/dispose entry or initialize prologue")
	check(successor.release_entry_scene().is_empty(),"old scene queues deletion only after input/resources transfer")
	text = null
	await process_frame
	check(not is_instance_valid(old_entry),"old entry scene actually freed")
	check(is_instance_valid(pad_weak.get_ref()) and pad_weak.get_ref().get_parent() == successor,"entry-owned pad survives old parent destruction")
	check(successor.entry == entry_weak.get_ref() and successor.rng == rng_weak.get_ref() and successor.rng.checkpoint() == retained_rng and successor.entry.owned_projection() == retained_profile,"actual controller, names/profile and RNG survive without reconstruction/draws")
	check(is_instance_valid(text_weak.get_ref()) and is_instance_valid(font_weak.get_ref()) and successor.original_text == text_weak.get_ref() and successor.original_font == font_weak.get_ref() and successor.original_text.initial_constants() == live.constants,"actual authenticated resource owners survive old scene deletion")
	check(old_model_weak.get_ref() == null and old_clock_weak.get_ref() == null,"old entry clock/model are not kept ticking after handoff")
	check(successor.live_handoff().context == retained_context,"live timer/frame/brightness/residency context remains unchanged by deletion")
	var peer := Provider.new()
	check(successor.begin_opening(peer.services(),[peer],[peer.publish]).is_empty(),"retained actual entry starts source exit with explicitly invented service fixture")
	if stop_if_failed(): return
	check(successor.opening.session.rng == successor.rng and successor.opening.state.frame_count == retained_context.frame_count and successor.opening.state.timer_ticks == retained_context.timer_ticks and successor.opening.state.system_seed == retained_context.system_seed,"new session preserves exact RNG and independent frame/timer words")
	check(successor.opening.session.latched_input.held == 0x800 and successor.opening.session.latched_input.pressed == 0,"held commit START has no new native edge")
	var held: InputFrame = successor.input_owner.sample(0)
	check(held.held == 0x800 and held.pressed == 0,"reparented device table retains held START without reset")
	var unchanged := successor.opening.session.state_hash()
	var null_observation: Session.FrameResult = successor.step([null])
	check(not null_observation.committed and not null_observation.error.is_empty() and successor.opening.session.state_hash() == unchanged,"null observation rejects before sampling or progression")
	var stepped: Session.FrameResult = successor.step()
	check(stepped.committed and successor.opening.state.timer_ticks == retained_context.timer_ticks+1 and successor.opening.state.system_seed == retained_context.system_seed+0x377,"successor applies declared timer policy to new session only")
	successor.input_owner._input(key_event(false))
	var released: InputFrame = successor.input_owner.sample(1)
	check(released.held == 0 and released.released == 0x800,"real retained pad observes release after old scene is gone")
	successor.input_owner._input(key_event(true))
	var repressed: InputFrame = successor.input_owner.sample(2)
	check(repressed.pressed == 0x800,"release then repress creates exactly a fresh START edge")
	successor.queue_free()
	probe.queue_free()
	await process_frame
	check(not is_instance_valid(pad_weak.get_ref()),"successor deletion finally releases its retained pad")
	print("PASS: %d actual entry-consumer lifetime checks" % checks if failures == 0 else "FAIL: %d/%d entry-consumer checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
