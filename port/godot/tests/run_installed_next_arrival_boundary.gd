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
var observed_first_line := false

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
	if frames > 240000:
		_failed("connected entry exceeded bounded observer frames")
		return false
	var app := current_scene as App
	if app == null: return false
	if not app.failure.is_empty():
		finishing = true
		_finish_boundary.call_deferred(app)
		return false

	if app.consumer != null and app.consumer.opening.state != null:
		var work: Variant = app.consumer.opening.state.opening.get("prologue_text_work")
		if work is Dictionary and work.get("line_id") == 0 and work.get("pos",0) >= work.get("total",1):
			observed_first_line = true

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

func _finish_boundary(app: App) -> void:
	var session = app.consumer.opening.session
	var state = session.state
	check(observed_first_line,"source did not reveal original first line before next boundary")
	check(phase == 7 and app.consumer.entry == initial_entry and session.rng == initial_rng,"actual owned entry context changed")
	check(app.failure_gate == "source_transaction" and state.mode_stage == 0,"next unsupported scene escaped its source transaction")
	check(not app.resources.audio.bgm_state.playing and app.bgm.diagnostics().private_ticket == 0 and app.bgm.diagnostics().engine.is_empty(),"terminal boundary did not dispose original BGM")
	if failed: return
	_status({"phase":"passed","checks":checks,"actual_desktop_entry":true,"first_line_revealed":true,
		"normal_progression_no_new_START_edge":true,"next_blocker":app.failure,"failure_gate":app.failure_gate,
		"opening_iterations":session.clock.logic_iterations,"mode":state.game_mode,"stage":state.mode_stage,
		"step":state.mode_step,"substep":state.substep,"timer":state.mode_step_timer,
		"failure_audio_diagnostics":app.failure_audio_diagnostics,"first_scene_only":true,"whole_new_game_accepted":false})
	print("PASS: actual source continued past first line to explicit next blocker: ",app.failure)
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
