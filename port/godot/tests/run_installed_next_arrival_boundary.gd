extends SceneTree
## Fresh developer-prepared original pack, real installed publication and X11
## input. This does NOT exercise the unfinished engine-native first-run import.
const Store = preload("res://services/installed_pack_store.gd")
const Router = preload("res://app/boot_router.gd")
const App = preload("res://app/first_arrival_boot.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const Receipts = preload("res://tests/route_receipts.gd")
const FastRoute = preload("res://tests/fast_route/fast_route.gd")
## Development checkpoints (tests/checkpoint/runtime_checkpoint.gd), FAST
## routes only: TOKIMEMO_DEV_CHECKPOINT_SAVE=<file> writes the runtime at the
## first frame boundary whose committed key is TOKIMEMO_DEV_CHECKPOINT_KEY
## (default 34/1/0/0); TOKIMEMO_DEV_RESUME=<file> restores one into the freshly
## booted app before its first logic frame and marks the run as resumed
## (/out/resumed-from.json). TOKIMEMO_DEV_IDENTITY is the route identity the
## runner computed (bundle, library, bridge, sources); a resume refuses a
## checkpoint written under another identity.
const Checkpoint = preload("res://tests/checkpoint/runtime_checkpoint.gd")
const EngineRecorder = preload("res://tests/checkpoint/engine_recorder.gd")
const Backend = preload("res://bgm_backend.gd")
var _dev_checkpoint_save := OS.get_environment("TOKIMEMO_DEV_CHECKPOINT_SAVE")
var _dev_checkpoint_key := OS.get_environment("TOKIMEMO_DEV_CHECKPOINT_KEY") if not OS.get_environment("TOKIMEMO_DEV_CHECKPOINT_KEY").is_empty() else "34/1/0/0"
var _dev_resume := OS.get_environment("TOKIMEMO_DEV_RESUME")
var _dev_identity := OS.get_environment("TOKIMEMO_DEV_IDENTITY")
var _dev_saved := false
## Per-frame image/PCM receipts in both modes; the FAST harness only with
## TOKIMEMO_FAST_ROUTE=1 (tests/fast_route/fast_route.gd).
var receipts := Receipts.new()
var fast: FastRoute = FastRoute.new() if FastRoute.enabled() else null
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
var observed_second_line := false
var observed_third_line := false
var observed_fourth_line := false
var observed_fifth_line := false
var observed_sixth_line := false
var observed_seventh_line := false
var observed_eighth_line := false
var observed_ninth_line := false
var observed_tenth_line := false
var observed_eleventh_line := false
var observed_twelfth_line := false
var observed_board_read_substep := false
var observed_board_decode_complete := false
var observed_board_scroll_mid := false
var observed_board_scroll_complete := false
var observed_names_495d_decoded := false
var observed_names_crossfade_peak := false
var observed_names_crossfade_end := false
var observed_stage2_fade_out := false
var observed_line_03_yoshio_gray := false
var observed_line_04_rei := false
var observed_scene_05_color_burst := false
var observed_scene_06_yoshio_ready := false
var observed_scene_06_yoshio_greeting := false
var observed_line_11_yoshio := false
var observer_bound := false
var board_complete_bgm: Dictionary = {}
var board_scroll_complete_bgm: Dictionary = {}
var names_crossfade_peak_bgm: Dictionary = {}
var names_crossfade_end_bgm: Dictionary = {}
var stage2_fade_out_bgm: Dictionary = {}
var line_03_yoshio_gray_bgm: Dictionary = {}
var line_04_rei_bgm: Dictionary = {}
var scene_05_color_burst_bgm: Dictionary = {}
var scene_06_yoshio_ready_bgm: Dictionary = {}
var scene_06_yoshio_greeting_bgm: Dictionary = {}
var line_11_yoshio_bgm: Dictionary = {}

func check(value: bool, message: String) -> void:
	checks += 1
	if not value: _failed(message)

func _initialize() -> void:
	receipts.install()
	if fast != null:
		var fast_error := fast.install(self)
		if not fast_error.is_empty():
			_failed(fast_error)
			return
	if not _dev_checkpoint_save.is_empty() or not _dev_resume.is_empty():
		if fast == null or _dev_identity.is_empty():
			_failed("dev checkpoints need a FAST route and the runner's TOKIMEMO_DEV_IDENTITY")
			return
		Backend.engine_factory = Callable(EngineRecorder, "create")
	_start.call_deferred()

func _finalize() -> void:
	receipts.finish()

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
	if not _dev_resume.is_empty() and current_scene is App:
		_dev_resume_now(current_scene)
		if finishing: return false
	elif not _dev_checkpoint_save.is_empty() and not _dev_saved and current_scene is App:
		_dev_save_if_key(current_scene)
		if finishing: return false
	frames += 1
	if frames > 240000:
		_failed("connected entry exceeded bounded observer frames")
		return false
	var app := current_scene as App
	if app == null: return false
	if not observer_bound:
		app.frame_published.connect(_frame_published.bind(app))
		receipts.bind(app)
		observer_bound = true
	if not app.failure.is_empty():
		finishing = true
		_finish_boundary.call_deferred(app)
		return false

	if app.consumer != null and app.consumer.opening.state != null:
		var work: Variant = app.consumer.opening.state.opening.get("prologue_text_work")
		if work is Dictionary and work.get("line_id") == 0 and work.get("pos",0) >= work.get("total",1):
			observed_first_line = true
		if work is Dictionary and work.get("line_id") == 1 and work.get("pos",0) >= work.get("total",1):
			observed_second_line = true
		if work is Dictionary and work.get("line_id") == 2 and work.get("pos",0) >= work.get("total",1):
			observed_third_line = true
		if work is Dictionary and work.get("line_id") == 3 and work.get("pos",0) >= work.get("total",1):
			observed_fourth_line = true
		if work is Dictionary and work.get("line_id") == 4 and work.get("pos",0) >= work.get("total",1):
			observed_fifth_line = true
		if work is Dictionary and work.get("line_id") == 5 and work.get("pos",0) >= work.get("total",1):
			observed_sixth_line = true
		if work is Dictionary and work.get("line_id") == 6 and work.get("pos",0) >= work.get("total",1):
			observed_seventh_line = true
		if work is Dictionary and work.get("line_id") == 7 and work.get("pos",0) >= work.get("total",1):
			observed_eighth_line = true
		if work is Dictionary and work.get("line_id") == 8 and work.get("pos",0) >= work.get("total",1):
			observed_ninth_line = true
		if work is Dictionary and work.get("line_id") == 9 and work.get("pos",0) >= work.get("total",1):
			observed_tenth_line = true
		if work is Dictionary and work.get("line_id") == 10 and work.get("pos",0) >= work.get("total",1):
			observed_eleventh_line = true
		if work is Dictionary and work.get("line_id") == 11 and work.get("pos",0) >= work.get("total",1):
			observed_twelfth_line = true

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
	if finishing or app.consumer == null or app.consumer.opening.state == null: return
	var state = app.consumer.opening.state
	if state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("board_media",{}).get("read_requested") == true and state.opening.get("board_media",{}).get("decoded") == false:
		observed_board_read_substep = true
	if state.mode_stage == 1 and state.mode_step == 1 and state.substep == 0 and state.opening.get("board_media",{}).get("decoded") == true and not observed_board_decode_complete:
		observed_board_decode_complete = true
		board_complete_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/board-load-complete.png")
	if state.mode_stage == 1 and state.mode_step == 1 and state.substep in [1, 2] and state.opening.get("prologue_scene_counter") == 128 and not observed_board_scroll_mid:
		observed_board_scroll_mid = true
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/board-scroll-mid.png")
	if state.mode_stage == 1 and state.mode_step == 1 and state.substep == 3 and state.opening.get("prologue_scene_counter") == 320 and not observed_board_scroll_complete:
		observed_board_scroll_complete = true
		board_scroll_complete_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/board-scroll-complete.png")
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep in [1, 2] and state.opening.get("names_media",{}).get("active_draw") == "dec_bg_draw" and not observed_names_495d_decoded:
		observed_names_495d_decoded = true
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/names-495d-decoded.png")
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 3 and state.opening.get("names_media",{}).get("drawn_fade_amount") == 14 and state.opening.get("prologue_text_work",{}).get("line_id") == 2 and state.opening.get("prologue_text_work",{}).get("pos",0) >= state.opening.get("prologue_text_work",{}).get("total",1) and not observed_names_crossfade_peak:
		observed_names_crossfade_peak = true
		names_crossfade_peak_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/names-crossfade-peak.png")
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 4 and state.opening.get("prologue_scene_counter") == 130 and not observed_names_crossfade_end:
		observed_names_crossfade_end = true
		names_crossfade_end_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/names-crossfade-end.png")
	if state.mode_stage == 2 and state.mode_step == 0 and state.substep == 5 and state.opening.get("prologue_scene_counter") == 64 and not observed_stage2_fade_out:
		observed_stage2_fade_out = true
		stage2_fade_out_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/stage2-fade-out.png")
	if state.mode_stage == 3 and state.mode_step == 0 and state.substep == 1 and state.opening.get("D_801220E0") == 132 and state.opening.get("prologue_text_work",{}).get("line_id") == 3 and state.opening.get("prologue_text_work",{}).get("pos",0) >= state.opening.get("prologue_text_work",{}).get("total",1) and not observed_line_03_yoshio_gray:
		observed_line_03_yoshio_gray = true
		line_03_yoshio_gray_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/line-03-yoshio-gray.png")
	if state.mode_stage == 4 and state.mode_step == 0 and state.substep == 2 and state.opening.get("prologue_text_work",{}).get("line_id") == 4 and state.opening.get("prologue_text_work",{}).get("pos",0) >= state.opening.get("prologue_text_work",{}).get("total",1) and not observed_line_04_rei:
		observed_line_04_rei = true
		line_04_rei_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/line-04-rei.png")
	if state.mode_stage == 5 and state.mode_step == 0 and state.substep == 2 and state.opening.get("prologue_scene_counter") == 128 and not observed_scene_05_color_burst:
		observed_scene_05_color_burst = true
		scene_05_color_burst_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/scene-05-color-burst.png")
	if state.mode_stage == 6 and state.mode_step == 0 and state.substep in [0, 1] and not observed_scene_06_yoshio_ready:
		observed_scene_06_yoshio_ready = true
		scene_06_yoshio_ready_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/scene-06-yoshio-ready.png")
	if state.mode_stage == 6 and state.mode_step == 0 and state.substep == 12 and state.opening.has("actor103") and state.opening.get("prologue_text_work",{}).get("line_id") == 10 and state.opening.get("prologue_text_work",{}).get("pos",0) >= state.opening.get("prologue_text_work",{}).get("total",1) and not observed_scene_06_yoshio_greeting:
		observed_scene_06_yoshio_greeting = true
		scene_06_yoshio_greeting_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/scene-06-yoshio-greeting.png")
	if state.mode_stage == 7 and state.mode_step == 0 and state.substep == 2 and state.opening.get("actor96",{}).get("x") == -160 and state.opening.get("prologue_text_work",{}).get("line_id") == 11 and state.opening.get("prologue_text_work",{}).get("pos",0) >= state.opening.get("prologue_text_work",{}).get("total",1) and not observed_line_11_yoshio:
		observed_line_11_yoshio = true
		line_11_yoshio_bgm = app.bgm.diagnostics()
		app.set_physics_process(false)
		_capture_and_resume.call_deferred(app,"/out/line-11-yoshio.png")

func _capture_and_resume(app: App, path: String) -> void:
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
	await RenderingServer.frame_post_draw
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
	# FAST mode renders with the dummy driver: there is no window image to read.
	var image: Image = root.get_texture().get_image() if fast == null else null
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
	# The window capture needs a real renderer (none in FAST mode); the native
	# image beside it is saved in both modes for tools/port/compare_route_modes.py.
	if image != null and not image.is_empty():
		image.save_png(path)
	if is_instance_valid(app) and app.last_rendered_image != null:
		app.last_rendered_image.save_png(path.get_basename() + ".native.png")
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
	if is_instance_valid(app) and not finishing:
		app.set_physics_process(true)

func _finish_boundary(app: App) -> void:
	var session = app.consumer.opening.session
	var state = session.state
	check(observed_first_line,"source did not reveal original first line before next boundary")
	check(phase == 7 and app.consumer.entry == initial_entry and session.rng == initial_rng,"actual owned entry context changed")
	var has_board: bool = app.installed_launch_profile.get("paths",{}).has("first_arrival_board")
	var has_opening_audio: bool = app.installed_launch_profile.get("paths",{}).has("opening_audio")
	if has_board and has_opening_audio:
		check(observed_board_read_substep and observed_board_decode_complete and observed_second_line and observed_board_scroll_mid and observed_board_scroll_complete and observed_names_495d_decoded and observed_third_line and observed_names_crossfade_peak and observed_names_crossfade_end and observed_stage2_fade_out and observed_fourth_line and observed_line_03_yoshio_gray and observed_fifth_line and observed_line_04_rei and observed_scene_05_color_burst and observed_scene_06_yoshio_ready and observed_sixth_line and observed_seventh_line and observed_eighth_line and observed_ninth_line and observed_tenth_line and observed_eleventh_line and observed_scene_06_yoshio_greeting and observed_twelfth_line and observed_line_11_yoshio,"source did not publish prologue stages 1..7 substeps through prologue_line_11_run")
		check(board_complete_bgm.get("active_skips",-1) == 0 and board_scroll_complete_bgm.get("active_skips",-1) == 0 and names_crossfade_peak_bgm.get("active_skips",-1) == 0 and names_crossfade_end_bgm.get("active_skips",-1) == 0 and stage2_fade_out_bgm.get("active_skips",-1) == 0 and line_03_yoshio_gray_bgm.get("active_skips",-1) == 0 and line_04_rei_bgm.get("active_skips",-1) == 0 and scene_05_color_burst_bgm.get("active_skips",-1) == 0 and scene_06_yoshio_ready_bgm.get("active_skips",-1) == 0 and scene_06_yoshio_greeting_bgm.get("active_skips",-1) == 0 and line_11_yoshio_bgm.get("active_skips",-1) == 0,"active BGM skips occurred during prologue stages 1..7")
		check(app.failure_gate == "source_transaction" and app.failure == "source-backed opening background/decode is unavailable for LBA ad00 slot 1" and state.mode_stage == 7 and state.mode_step == 0 and state.substep == 3 and state.opening.get("actor96_draw_flags") == 0xa4 and state.opening.get("actor96",{}).get("profile") == "o07_yoshio_body" and state.opening.get("actor96",{}).get("x") == -160 and state.opening.get("D_800B3DB0") == 1 and state.opening.get("prologue_line") == 11 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("bg_layer_0",{}).get("clut_row") == 495 and state.opening.get("bg_layer_0",{}).get("resource_id") == "prologue.cut_07.0xaa85" and state.opening.get("stage_05_se_605_count") == 2 and state.opening.get("stage_06_se_503_count") == 1,"next unsupported scene escaped its source transaction after prologue_line_11_run (gate=%s failure=%s stage=%s step=%s substep=%s)" % [app.failure_gate, app.failure, str(state.mode_stage), str(state.mode_step), str(state.substep)])
	elif has_board:
		check(observed_board_read_substep and observed_board_decode_complete and observed_second_line and observed_board_scroll_mid and observed_board_scroll_complete and observed_names_495d_decoded and observed_third_line and observed_names_crossfade_peak and observed_names_crossfade_end and observed_stage2_fade_out and observed_fourth_line and observed_line_03_yoshio_gray and observed_fifth_line and observed_line_04_rei and observed_scene_05_color_burst and observed_scene_06_yoshio_ready,"source did not publish prologue_board_load, prologue_board_scroll, prologue_names_crossfade, prologue_line_03_run, prologue_line_04_run, and prologue_scene_05_transition_run substeps")
		check(board_complete_bgm.get("active_skips",-1) == 0 and board_scroll_complete_bgm.get("active_skips",-1) == 0 and names_crossfade_peak_bgm.get("active_skips",-1) == 0 and names_crossfade_end_bgm.get("active_skips",-1) == 0 and stage2_fade_out_bgm.get("active_skips",-1) == 0 and line_03_yoshio_gray_bgm.get("active_skips",-1) == 0 and line_04_rei_bgm.get("active_skips",-1) == 0 and scene_05_color_burst_bgm.get("active_skips",-1) == 0 and scene_06_yoshio_ready_bgm.get("active_skips",-1) == 0,"active BGM skips occurred during prologue stages 1..6")
		check(app.failure_gate == "source_transaction" and app.failure == "original prologue line is unavailable" and state.mode_stage == 6 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96_draw_flags") == 0xa4 and state.opening.get("actor99_draw_flags") == 0xa4 and not state.opening.has("actor97") and state.opening.get("actor96",{}).get("profile") == "o06_yoshio_body" and state.opening.get("actor99",{}).get("profile") == "o06_crowd_girls" and state.opening.get("D_800B3DB0") == 1 and state.opening.get("prologue_line") == 4 and state.opening.get("prologue_text_cleared") == true and state.opening.get("names_media",{}).get("bg_show_1") == 1 and state.opening.get("bg_layer_0",{}).get("clut_row") == 495 and state.opening.get("bg_layer_0",{}).get("resource_id") == "prologue.cut_06.0xaa54" and state.opening.get("stage_05_se_605_count") == 2,"next unsupported scene escaped its source transaction after scene_05_transition (gate=%s failure=%s stage=%s step=%s substep=%s)" % [app.failure_gate, app.failure, str(state.mode_stage), str(state.mode_step), str(state.substep)])
	else:
		check(app.failure_gate == "source_transaction" and state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor96_draw_flags") == 0xa4,"next unsupported scene escaped its source transaction")
	check(not app.resources.audio.bgm_state.playing and app.bgm.diagnostics().private_ticket == 0 and app.bgm.diagnostics().engine.is_empty(),"terminal boundary did not dispose original BGM")
	if failed: return
	_status({"phase":"passed","checks":checks,"actual_desktop_entry":true,"first_line_revealed":true,
		"second_line_revealed":observed_second_line,
		"third_line_revealed":observed_third_line,
		"fourth_line_revealed":observed_fourth_line,
		"fifth_line_revealed":observed_fifth_line,
		"sixth_line_revealed":observed_sixth_line,
		"seventh_line_revealed":observed_seventh_line,
		"eighth_line_revealed":observed_eighth_line,
		"ninth_line_revealed":observed_ninth_line,
		"tenth_line_revealed":observed_tenth_line,
		"eleventh_line_revealed":observed_eleventh_line,
		"twelfth_line_revealed":observed_twelfth_line,
		"normal_progression_no_new_START_edge":true,"observed_board_read_substep":observed_board_read_substep,
		"observed_board_decode_complete":observed_board_decode_complete,
		"observed_board_scroll_mid":observed_board_scroll_mid,
		"observed_board_scroll_complete":observed_board_scroll_complete,
		"observed_names_495d_decoded":observed_names_495d_decoded,
		"observed_names_crossfade_peak":observed_names_crossfade_peak,
		"observed_names_crossfade_end":observed_names_crossfade_end,
		"observed_stage2_fade_out":observed_stage2_fade_out,
		"observed_line_03_yoshio_gray":observed_line_03_yoshio_gray,
		"observed_line_04_rei":observed_line_04_rei,
		"observed_scene_05_color_burst":observed_scene_05_color_burst,
		"observed_scene_06_yoshio_ready":observed_scene_06_yoshio_ready,
		"observed_scene_06_yoshio_greeting":observed_scene_06_yoshio_greeting,
		"observed_line_11_yoshio":observed_line_11_yoshio,
		"board_complete_bgm":board_complete_bgm,
		"board_scroll_complete_bgm":board_scroll_complete_bgm,
		"names_crossfade_peak_bgm":names_crossfade_peak_bgm,
		"names_crossfade_end_bgm":names_crossfade_end_bgm,
		"stage2_fade_out_bgm":stage2_fade_out_bgm,
		"line_03_yoshio_gray_bgm":line_03_yoshio_gray_bgm,
		"line_04_rei_bgm":line_04_rei_bgm,
		"scene_05_color_burst_bgm":scene_05_color_burst_bgm,
		"scene_06_yoshio_ready_bgm":scene_06_yoshio_ready_bgm,
		"scene_06_yoshio_greeting_bgm":scene_06_yoshio_greeting_bgm,
		"line_11_yoshio_bgm":line_11_yoshio_bgm,
		"next_blocker":app.failure,"failure_gate":app.failure_gate,
		"opening_iterations":session.clock.logic_iterations,"mode":state.game_mode,"stage":state.mode_stage,
		"step":state.mode_step,"substep":state.substep,"timer":state.mode_step_timer,
		"failure_audio_diagnostics":app.failure_audio_diagnostics,"first_scene_only":true,"whole_new_game_accepted":false})
	print("PASS: actual source continued past first line to explicit next blocker: ",app.failure)
	app.queue_free()
	initial_entry = null
	initial_rng = null
	await create_timer(0.2).timeout
	quit(0)

## Observer-side objects whose plain variables a checkpoint carries.
func _dev_observers() -> Dictionary:
	var observers := {"observer": self, "receipts": receipts}
	if fast != null: observers["fast"] = fast
	return observers

func _dev_save_if_key(app: App) -> void:
	if app.consumer == null or app.consumer.opening.state == null or app.consumer.opening.session == null: return
	var state = app.consumer.opening.state
	if "%02x/%d/%d/%d" % [state.game_mode, state.mode_stage, state.mode_step, state.substep] != _dev_checkpoint_key: return
	_dev_saved = true
	var tick: int = app.consumer.opening.session.clock.logic_iterations
	var started := Time.get_ticks_msec()
	var error := Checkpoint.save(_dev_checkpoint_save, app, _dev_observers(), {"key": _dev_checkpoint_key, "tick": tick,
		"frame_count": int(state.frame_count), "identity": _dev_identity, "observer": get_script().resource_path})
	if not error.is_empty():
		_failed("dev checkpoint save: " + error)
		return
	var file := FileAccess.open("/out/checkpoint-saved.json", FileAccess.WRITE)
	file.store_string(JSON.stringify({"path": _dev_checkpoint_save, "key": _dev_checkpoint_key, "tick": tick, "frame_count": int(state.frame_count),
		"sha256": FileAccess.get_sha256(_dev_checkpoint_save), "save_ms": Time.get_ticks_msec() - started}, "\t"))
	file.close()
	print("DEV CHECKPOINT saved %s at %s tick %d" % [_dev_checkpoint_save, _dev_checkpoint_key, tick])

func _dev_resume_now(app: App) -> void:
	var path := _dev_resume
	_dev_resume = ""
	var started := Time.get_ticks_msec()
	var loaded := Checkpoint.load_file(path)
	if not loaded.error.is_empty():
		_failed("dev resume: " + loaded.error)
		return
	var meta: Dictionary = loaded.data.meta
	if meta.get("identity") != _dev_identity or meta.get("observer") != get_script().resource_path:
		_failed("dev resume: checkpoint was written for another route identity or observer")
		return
	var error := Checkpoint.restore(loaded.data, app, _dev_observers())
	if not error.is_empty():
		_failed("dev resume: " + error)
		return
	var file := FileAccess.open("/out/resumed-from.json", FileAccess.WRITE)
	file.store_string(JSON.stringify({"path": path, "sha256": FileAccess.get_sha256(path), "key": meta.key, "tick": meta.tick,
		"frame_count": meta.frame_count, "identity": meta.identity, "restore_ms": Time.get_ticks_msec() - started}, "\t"))
	file.close()
	receipts.resumed_from = path
	# route-speed.json of a resumed run covers the resumed frames only (the
	# restored wall-clock marks belong to the process that saved).
	receipts.restart_speed_window()
	print("DEV RESUME from %s at %s tick %d" % [path, meta.key, meta.tick])

func _status(record: Dictionary) -> void:
	if fast != null: fast.observe_status(record)
	var file := FileAccess.open("/out/entry-keyboard-status.json",FileAccess.WRITE)
	file.store_string(JSON.stringify(record,"\t"))
	file.close()

func _failed(message: String) -> void:
	if finishing and current_scene == null: return
	finishing = true
	failed = true
	_status({"phase":"failed","message":message})
	printerr(message)
	if is_instance_valid(current_scene): current_scene.queue_free()
	initial_entry = null
	initial_rng = null
	quit(1)
