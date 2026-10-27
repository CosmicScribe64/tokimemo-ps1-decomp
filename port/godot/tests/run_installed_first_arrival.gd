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

func check(value: bool, message: String) -> void:
	checks += 1
	if not value: _failed(message)

func _initialize() -> void:
	_start.call_deferred()

func _start() -> void:
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
	if frames > 2400:
		_failed("connected entry exceeded bounded observer frames")
		return false
	var app := current_scene as App
	if app == null: return false
	if not app.failure.is_empty():
		if app.failure_gate == "bgm_backend":
			finishing = true
			_finish.call_deferred(app)
		else: _failed("actual launch stopped at "+app.failure_gate+": "+app.failure)
		return false
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

func _finish(app: App) -> void:
	var session = app.consumer.opening.session
	var state = app.consumer.opening.state
	check(phase == 7 and app.consumer.entry == initial_entry and app.consumer.rng == initial_rng and session.rng == initial_rng,"actual player-created entry/RNG ownership changed")
	check(original_scene.get_ref() == null and app.consumer.input_owner.get_parent() == app.consumer,"old entry scene survived or its original pad was not retained")
	var context: Dictionary = app.consumer.live_handoff().context
	check(context.held_input == 0x800 and session.latched_input.held == 0x800,"held physical Enter was lost or replaced across handoff")
	check(state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 1 and state.substep == 1,"missing BGM changed the actual source boundary")
	check(state.opening.audio_bank.ready and state.opening.audio_bank.role == "prologue_arrival","actual bank producer did not complete before BGM validation")
	check(state.opening.media.requests.is_empty() and app.media.diagnostics().buffers.is_empty(),"missing BGM issued a background/actor request")
	check(not state.opening.has("prologue_text_work") and not state.opening.arrival_media.actor_initialized,"missing BGM started text or actor setup")
	check(state.frame_count == ((context.frame_count+session.clock.logic_iterations)&0xffffffff),"actual owned frame count was reconstructed or advanced outside Session")
	check(state.opening.cold_no_card_checker_prefix.loader_calls == 2 and state.opening.cold_no_card_checker_prefix.handed_off,"actual loader checker prefix was not consumed once")
	check(app.entry_frame_provenance.get("source") == "actual-live-desktop-entry-viewport" and app.entry_frame_provenance.get("native_entry_pixel_parity") == false,"desktop framebuffer provenance was lost")
	check(not app.upload.diagnostics().batch_complete and not app.upload.diagnostics().vram_ready,"BGM rejection manufactured an upload readiness receipt")
	if failed: return
	await RenderingServer.frame_post_draw
	var image := root.get_texture().get_image()
	if image == null or image.save_png("/out/bgm-boundary.png") != OK:
		_failed("actual connected viewport capture failed")
		return
	var report := {"phase":"passed","checks":checks,"input_dispatch":"XSendEvent/libX11",
		"manifest_sha256":pack_manifest_sha,"source_pack_preparation":"fresh-original Docker developer preparation; production first-run import not exercised",
		"actual_desktop_entry":true,"same_entry_rng_pad":true,"physical_commit_held":true,
		"opening_iterations":session.clock.logic_iterations,"entry_context_frame":context.frame_count,
		"blocked_at":"source bank_start(0,0), original BGM backend unavailable","background_requested":false,
		"first_scene_accepted":false,"native_startup_parity":false,"entry_frame_provenance":app.entry_frame_provenance}
	_status(report)
	print("PASS: %d actual installed New Game BGM-boundary checks"%checks)
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
