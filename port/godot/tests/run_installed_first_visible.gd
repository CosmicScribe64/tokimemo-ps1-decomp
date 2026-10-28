extends SceneTree
## Fresh developer-prepared original pack, real installed publication and X11
## input. This does NOT exercise the unfinished engine-native first-run import.
const Store = preload("res://services/installed_pack_store.gd")
const Router = preload("res://app/boot_router.gd")
const App = preload("res://app/first_arrival_boot.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
var expected := [[1,1],[1,3],[1,5],[1,7],[1,9],[2,1],[2,3]]
var phase := 0
var checks := 0
var frames := 0
var finishing := false
var failed := false
var initial_entry: RefCounted
var initial_rng: RefCounted
var original_scene: WeakRef
var pack_manifest_sha := ""
var audio_record := AudioEffectRecord.new()
var recording_started := false
var observer_bound := false

func check(value: bool, message: String) -> void:
	checks += 1
	if not value: _failed(message)

func _initialize() -> void:
	_start.call_deferred()

func _start() -> void:
	audio_record.format = AudioStreamWAV.FORMAT_16_BITS
	AudioServer.add_bus_effect(0,audio_record)
	var store := Store.new("/out/content-install")
	var stage := "/out/content-install/.staging/prepared-original"
	pack_manifest_sha = FileAccess.get_sha256(stage.path_join("manifest.json"))
	var published := store.publish_stage(stage,pack_manifest_sha,Profile.RELEASE,[Profile.ENTRY_CAP,Profile.OPENING_RESOURCES_CAP])
	if not published.get("valid",false):
		_failed("actual prepared pack publication failed: "+str(published.get("message","unknown")))
		return
	check(published.get("launch_ready",false),"prepared original pack has no opening resource capability")
	var host := Node.new()
	root.add_child(host)
	current_scene = host
	var decision := Router.new(store).route(host)
	if decision.get("target") != "res://app/new_game_entry.tscn" or decision.get("action") != "playable":
		_failed("normal installed boot did not choose the actual entry destination")

func _process(_delta: float) -> bool:
	if finishing: return false
	frames += 1
	if frames > 240000:
		_failed("connected entry exceeded bounded observer frames")
		return false
	var app := current_scene as App
	if app == null: return false
	if not observer_bound:
		app.frame_published.connect(_frame_published.bind(app))
		observer_bound = true
	if not app.failure.is_empty():
		_failed("actual launch stopped at "+app.failure_gate+": "+app.failure)
		return false
	if not recording_started and app.bgm.diagnostics().published_tick >= 0:
		audio_record.set_recording_active(true)
		recording_started = true

	if is_instance_valid(app._entry):
		var scene = app._entry
		if initial_entry == null:
			initial_entry = scene.controller
			initial_rng = scene.rng
			original_scene = weakref(scene)
			check(scene.profile_text_store == app.resources.text and scene.entry_clock_policy.get("native_clock_parity") == false,"original resources/policy were not injected before actual entry ready")
		if phase < expected.size() and [scene.controller.stage,scene.controller.step_id] == expected[phase]:
			_status({"phase":"entry_%d"%phase})
			phase += 1
	return false

func _frame_published(_tick: int, app: App) -> void:
	if finishing: return
	var state = app.consumer.opening.state
	if state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96_source_draw") is Dictionary:
		# Suspend host stepping after the actual committed/published boundary;
		# no rule state, RNG, pad or service completion is changed for capture.
		finishing = true
		app.set_physics_process(false)
		_finish.call_deferred(app)

func _finish(app: App) -> void:
	var session = app.consumer.opening.session
	var state = app.consumer.opening.state
	check(phase == 7 and app.consumer.entry == initial_entry and app.consumer.rng == initial_rng and session.rng == initial_rng,"actual player-created entry/RNG ownership changed")
	check(original_scene.get_ref() == null and app.consumer.input_owner.get_parent() == app.consumer,"old entry scene survived or its original pad was not retained")
	var context: Dictionary = app.consumer.live_handoff().context
	check(context.held_input == 0x800 and session.latched_input.held == 0x800,"held physical Enter was lost or replaced across handoff")
	check(state.game_mode == 0x15 and state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0,"actual source did not reach the visible actor boundary")
	check(state.opening.audio_bank.ready and state.opening.audio_bank.role == "prologue_arrival","actual bank producer did not complete")
	check(state.opening.arrival_media.actor_initialized and state.opening.actor96_draw_flags == 0xa4,"original actor was not made visible by its source transition")
	check(state.opening.prologue_text_work.line_id == 0 and state.opening.prologue_text_work.pos >= state.opening.prologue_text_work.total,"original first line did not reveal through source text work")
	check(state.opening.audio.status == "silent" and state.opening.audio.clips.is_empty(),"original narrator line was replaced by a fabricated voice completion")
	check(app.upload.diagnostics().batch_complete and not app.upload.diagnostics().vram_ready and app.upload.batch_ready(state.opening.arrival_media.upload_batch_key).is_empty(),"original bounded transfer batch did not complete or claimed whole VRAM readiness")
	check(state.opening.actor96_source_draw.source_frame_count == state.frame_count and state.opening.actor96_source_draw.frame_index == 0,"actor source draw was not recorded in the actual logical draw phase")
	check(state.opening.prologue_hud_live.screen_frame_argument == 2,"actual stage1 did not retain native HUD argument2")
	var audio_context: Dictionary = app.resources.audio.native_audio_context
	check(audio_context.state_flags == 0x30110 and audio_context.bgm_volume == 93 and audio_context.reverb_depth == [60,60],"committed source audio after-context differs")
	var bgm: Dictionary = app.bgm.diagnostics()
	if bgm.active_skips != 0: printerr("ACTIVE_AUDIO_TIMING_FAILURE ",JSON.stringify({"active_skips":bgm.active_skips,"gap_us":bgm.maximum_pump_gap_us,"phase":bgm.maximum_pump_gap_context}))
	check(bgm.active_skips == 0,"actual active original BGM generator underrun")
	audio_record.set_recording_active(false)
	var recorded := audio_record.get_recording()
	check(recording_started and recorded != null and recorded.data.size() > 44100 and recorded.data.count(0) != recorded.data.size(),"actual Master bus BGM recording is empty")
	if recorded != null:
		check(recorded.save_to_wav("/out/first-visible-mix.wav") == OK,"actual Master bus recording could not be saved")
	check(bgm.private_ticket == 0 and bgm.history.size() == 1 and bgm.history[0].kind == "bgm_play" and bgm.published_tick >= 0,"one actual native BGM start was not committed")
	check(not bgm.engine.is_empty() and bgm.sink.capacity > 0 and bgm.sink.queued <= 5145,"native engine and bounded real sink are unavailable")
	check(state.frame_count == ((context.frame_count+session.clock.logic_iterations)&0xffffffff),"actual owned frame count was reconstructed or advanced outside Session")
	check(state.opening.cold_no_card_checker_prefix.loader_calls == 2 and state.opening.cold_no_card_checker_prefix.handed_off,"actual loader checker prefix was not consumed once")
	check(app.entry_frame_provenance.get("source") == "actual-live-desktop-entry-viewport" and app.entry_frame_provenance.get("native_entry_pixel_parity") == false,"desktop framebuffer provenance was lost")
	if failed: return
	await RenderingServer.frame_post_draw
	var image := root.get_texture().get_image()
	if image == null or image.save_png("/out/first-visible.png") != OK:
		_failed("actual connected viewport capture failed")
		return
	var report := {"phase":"passed","checks":checks,"input_dispatch":"XSendEvent/libX11",
		"manifest_sha256":pack_manifest_sha,"source_pack_preparation":"fresh-original Docker developer preparation; production first-run import not exercised",
		"actual_desktop_entry":true,"same_entry_rng_pad":true,"physical_commit_held":true,
		"opening_iterations":session.clock.logic_iterations,"entry_context_frame":context.frame_count,
		"reached":"source arrival-to-board boundary; original visible actor flag0xa4 and HUDargument2","background_requested":true,"bgm":bgm,"audio_context":audio_context,"audio_capture":{"source":"actual Godot Master bus effect after observed native start","file":"first-visible-mix.wav","sample_rate":recorded.mix_rate,"stereo":recorded.stereo,"pcm_bytes":recorded.data.size(),"format":recorded.format,"hardware_PCM_parity":false},
		"first_scene_rendered":true,"first_scene_native_pixel_or_pcm_parity":false,"native_startup_parity":false,"entry_frame_provenance":app.entry_frame_provenance}
	_status(report)
	print("PASS: %d actual installed New Game visible-boundary checks"%checks)
	# Dispose the real scene while the audio server is still alive so it can
	# release its mixer-held playback reference before engine shutdown.
	app.queue_free()
	initial_entry = null
	initial_rng = null
	await create_timer(0.2).timeout
	quit(0)

func _status(record: Dictionary) -> void:
	var file := FileAccess.open("/out/entry-keyboard-status.json",FileAccess.WRITE)
	file.store_string(JSON.stringify(record,"\t"))
	file.close()

func _failed(message: String) -> void:
	if finishing and current_scene == null: return
	finishing = true
	failed = true
	_status({"phase":"failed","message":message})
	printerr(message)
	quit(1)
