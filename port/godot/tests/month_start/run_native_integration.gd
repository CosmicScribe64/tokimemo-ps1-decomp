extends "res://tests/month_start/run_quarter.gd"
## Real source controller/Session + original native engine and bytes. Initial
## school-day rules remain invented structural fixtures, not a played June save.
const NativeBackend = preload("res://bgm_backend.gd")
const NativeSink = preload("res://bgm_sink.gd")
const NativeService = preload("res://services/shared_month_audio_service.gd")
const OBS_SERVICE := "month_start.native_observations"
var steps: Array[Dictionary] = []
var native_service: NativeService
var native_backend: NativeBackend
var native_sink: NativeSink
var first_owner: Dictionary
var observation_cursor: int = 0
var rejected_attempts: Array[Dictionary] = []

func _initialize() -> void:
	call_deferred("run_native")

func _originals(summer: bool) -> Array[PackedByteArray]:
	var names: Array = ["SLPM_86.053", "CDROM/VAB_SE/COM_MIX.VH", "CDROM/VAB_SE/COM_MIX.VB", "CDROM/VAB_SEQ/SUMMER.VH", "CDROM/VAB_SEQ/SUMMER.VB", "CDROM/VAB_SEQ/SUMMER.SEQ"] if summer else ["SLPM_86.053", "CDROM/VAB_SE/SYS_MIX.VH", "CDROM/VAB_SE/SYS_MIX.VB", "CDROM/VAB_SEQ/PRO_VOL.VH", "CDROM/VAB_SEQ/PRO_VOL.VB", "CDROM/VAB_SEQ/PRO_VOL.SEQ"]
	var inputs: Array[PackedByteArray] = []
	for name: String in names:
		inputs.append(FileAccess.get_file_as_bytes(OS.get_environment("ORIGINAL_FILES").path_join(name)))
	return inputs

func _attach_observations(harness: Dictionary) -> void:
	var ordinary: Callable = harness["session"].service_completion
	harness["session"].service_completion = func(event: Session.ServiceEvent, base: Core, rng: Rng, result: Session.FrameResult) -> void:
		if event.service != OBS_SERVICE:
			ordinary.call(event, base, rng, result)
			return
		var payload: Dictionary = event.payload
		var pending: Variant = (base as State).rule_state["resources"].get(Bridge.PENDING)
		if event.phase != Session.Phase.BEFORE_MODE or payload.size() != 4 or not payload.get("source_frame") is int \
			or ((payload["source_frame"] + 1) & 0xffffffff) != base.frame_count \
			or not Bridge._digest_shape(payload.get("profile_sha256")) or payload["profile_sha256"] != NativeService.digest(NativeService.PROFILE) \
			or not Bridge._owner_shape(payload.get("owner")) or not Bridge._pending_shape(pending) or payload["owner"] != pending["owner"] \
			or not payload.get("observations") is Array or payload["observations"].is_empty():
			result.error = "native observations lack the exact published source frame/profile/owner"
			return
		for prior: Dictionary in result.presentation:
			if prior.get("kind") == "native_month_observation_batch":
				result.error = "duplicate native observation batch in one source frame"
				return
		for observation: Variant in payload["observations"]:
			if not observation is Dictionary or not observation.get("kind") is String or observation["kind"] not in ["original_file_copy", "native_slot_disposed", "native_bank_transfer", "native_bank_poll", "reverb_work_copy", "native_bgm_started"]:
				result.error = "unknown native observation"
				return
			for key: String in ["session_generation", "bank_epoch", "request_serial"]:
				if not observation.get(key) is int or observation[key] != payload["owner"][key]:
					result.error = "native observation owner differs from its committed tuple"
					return
			if observation.get("source_profile_sha256") != payload["profile_sha256"]:
				result.error = "native observation profile differs"
				return
		# Audit only. The source driver result, not this completion, controls ready.
		result.presentation.append({"kind": "native_month_observation_batch", "payload": payload.duplicate(true)})

func _native_frame(harness: Dictionary, scheduled: Array[Session.ServiceEvent]) -> Dictionary:
	var session: Session = harness["session"]
	var month: MonthStart = harness["controller"].month_start
	var frame: int = harness["state"].frame_count
	var prepare: String = month.prepare_services(harness["state"])
	check(prepare.is_empty(), "real native frame prepares outside Session: " + prepare)
	var before: Dictionary = native_backend.month_start_context()
	var history: Array[Dictionary] = native_service.month_observations.duplicate(true)
	var result: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations), scheduled)
	check(native_backend.month_start_context() == before and native_service.month_observations == history, "actual native engine/history do not mutate inside source callbacks or fences")
	check(result.committed and result.error.is_empty(), "actual source/native Session commits: " + result.error)
	var publication: String = month.publish_services(result)
	check(publication.is_empty(), "one exact real native batch publishes outside Session: " + publication)
	check(month.discard_services(frame).is_empty(), "private native ticket cleanup outside Session")
	var next: Array[Session.ServiceEvent] = []
	var fresh: Array[Dictionary] = native_service.month_observations.slice(observation_cursor)
	observation_cursor = native_service.month_observations.size()
	if not fresh.is_empty():
		var pending: Dictionary = harness["state"].rule_state["resources"][Bridge.PENDING]
		next.append(Session.ServiceEvent.new(session.clock.logic_iterations, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, OBS_SERVICE,
			{"source_frame": frame, "profile_sha256": NativeService.digest(NativeService.PROFILE), "owner": pending["owner"].duplicate(true), "observations": fresh}))
	steps.append({"source_frame": frame, "committed": result.committed, "error": result.error,
		"publication_error": publication, "state_hash": result.state_hash,
		"game_mode": harness["state"].game_mode, "stage": harness["state"].mode_stage, "step": harness["state"].mode_step,
		"service_events_consumed": result.service_events_consumed, "audio": result.audio,
		"new_observations": fresh, "native": native_backend.month_start_context()})
	return {"result": result, "next": next}

func _reject_native_attempt(harness: Dictionary, scheduled: Array[Session.ServiceEvent], mutation: String) -> void:
	var session: Session = harness["session"]
	var month: MonthStart = harness["controller"].month_start
	var frame: int = harness["state"].frame_count
	var before: Dictionary = _checkpoint(harness)
	var native_before: Dictionary = native_backend.month_start_context()
	var observations_before: Array[Dictionary] = native_service.month_observations.duplicate(true)
	var draw: Callable = session.draw_logic
	if mutation == "draw": harness["draw_settings"]["failure"] = true
	else:
		session.draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
			draw.call(base, rng, result)
			for intent: Dictionary in result.audio:
				if intent.get("kind") == Bridge.KIND: intent["kind"] = "bgm_play"
	check(month.prepare_services(harness["state"]).is_empty(), "real private native load prepares for transactional refusal")
	var result: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations), scheduled)
	check(not result.committed and not result.error.is_empty() and _checkpoint(harness) == before, "source failure/relabel abort restores actual Session state/RNG/calendar/queue: " + mutation)
	check(not month.publish_services(result).is_empty(), "aborted real native source frame refuses publication")
	check(month.discard_services(frame).is_empty(), "aborted real native private ticket is discarded")
	check(native_backend.month_start_context() == native_before and native_service.month_observations == observations_before, "real engine/cursor/banks/history preserved across source rollback: " + mutation)
	check(result.service_events_consumed == 0 and result.audio.is_empty(), "aborted native attempt does not consume CALROM completion or emit legacy audio")
	session.draw_logic = draw
	harness["draw_settings"]["failure"] = false
	rejected_attempts.append({"mutation": mutation, "error": result.error, "committed": result.committed, "native_unchanged": native_backend.month_start_context() == native_before})

func run_native() -> void:
	var descriptor: String = "res://original_audio.gdextension"
	check(FileAccess.get_sha256("res://bin/liboriginal_audio.linux.template_release.arm64.so") == "f1eecf80c1c6538a33f6bee9c7d10a79553b1745d520688f556bbe60fb1687b9", "exact reviewed native binary is present before explicit bootstrap")
	if not GDExtensionManager.is_extension_loaded(descriptor):
		check(GDExtensionManager.load_extension(descriptor) == GDExtensionManager.LOAD_STATUS_OK, "explicit native bootstrap registers extension without cache preloading")
	check(ClassDB.class_exists("OriginalAudioEngine"), "actual native class registered before first-arrival factory")
	source_path = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	check(source.open_source(source_path).is_empty(), "authenticated original CALROM opens")
	var host := Node.new()
	root.add_child(host)
	native_sink = NativeSink.new(host)
	native_backend = NativeBackend.new(native_sink)
	native_service = NativeService.new()
	first_owner = NativeBackend.ORIGINAL_IDENTITIES.duplicate(true)
	first_owner.merge({"role": "prologue_arrival", "epoch": 1, "ready": true, "load_set": 0, "sequence_pack_set": 0,
		"sequence": 0, "music_slot": 1, "SE_slot": 0, "state_flags": 0x30010, "reverb_preset": 3, "reverb_depth": 60, "reverb_enabled": true})
	var prepared: Dictionary = native_backend.prepare_start(first_owner, 1, _originals(false))
	check(prepared.get("error") == "", "genuine original first-arrival engine prepares")
	if not prepared.get("error") is String or not prepared["error"].is_empty():
		quit(1)
		return
	check(native_backend.commit_start(prepared["ticket"], first_owner).is_empty(), "genuine first-arrival native player starts")
	var player: AudioStreamPlayer = native_sink._active
	var harness: Dictionary = _harness(_state(95, 6))
	harness["state"].sound_active = true
	# Exact source first-arrival before->playing flags; no captured-save recovery.
	var main_flags: int = (first_owner["state_flags"] | 0x100) & ~0x1000 & ~0x100000
	check(native_service.bind_month_start_backend(native_backend, _originals(true), main_flags, harness["state"].game_mode).is_empty(), "same live native engine binds proved main-driver context")
	check(harness["controller"].month_start.audio.bind_owner(native_service).is_empty(), "real six-method adapter binds actual source controller")
	_attach_observations(harness)
	# Let the genuine first-arrival player/mixer become active before this
	# isolated source boundary; no calendar or native bank driver step is faked.
	var pump_error: String = ""
	var first_play_until: int = Time.get_ticks_msec() + 150
	while Time.get_ticks_msec() < first_play_until and pump_error.is_empty():
		pump_error = native_backend.pump(first_owner)
		await process_frame
	check(pump_error.is_empty(), "actual first-arrival playback pumps before warm source transition")
	var initial: Dictionary = native_backend.month_start_context()
	var empty: Array[Session.ServiceEvent] = []
	var requested: Dictionary = _native_frame(harness, empty)
	check(requested["result"].committed and harness["state"].substep == 1 and requested["result"].audio.is_empty(), "source CALROM request precedes native BGM request")
	var scheduled: Array[Session.ServiceEvent] = [_event(harness, _payload(harness))]
	_reject_native_attempt(harness, scheduled, "draw")
	_reject_native_attempt(harness, scheduled, "relabel")
	var loaded: Dictionary = _native_frame(harness, scheduled)
	check(harness["state"].mode_step == 1 and loaded["result"].audio.size() == 1 and loaded["result"].audio[0]["operation"] == "load", "authenticated calendar promotion commits real native load request")
	var polls: Array = []
	scheduled = loaded["next"]
	for index: int in range(6):
		if not failures.is_empty(): break
		var stepped: Dictionary = _native_frame(harness, scheduled)
		for intent: Dictionary in stepped["result"].audio:
			if intent["operation"] == "poll": polls.append(intent["status"])
		scheduled = stepped["next"]
		check(native_sink._active == player, "source wait/start keeps one actual AudioStreamPlayer")
		if harness["state"].mode_stage == 1: break
	check(polls == [-1, -1, 1], "actual original driver performs -1,-1,1 readiness, no controlled provider")
	check(harness["state"].mode_stage == 1, "real native ready poll/start advances original source stage")
	var exited: Dictionary = _native_frame(harness, scheduled)
	check(exited["result"].committed and harness["state"].game_mode == 0x31 and harness["state"].rule_state["day_event_index"] == 8, "actual started owner permits source queue7->8 exit")
	check(exited["result"].service_events_consumed == 1 and not harness["state"].rule_state["resources"].has(Bridge.PENDING), "start observations consumed next BEFORE_MODE before source clears pending")
	var after: Dictionary = native_backend.month_start_context()
	check(after.get("vm_initializations") == 1 and after.get("gain") == 86 and after.get("raw_reverb_depth") == 56, "same original VM uses SUMMER native gain86/depth56")
	check(after.get("sample_cursor") == initial.get("sample_cursor"), "warm source operations preserve the absolute decoded cursor")
	check(native_sink._active == player, "native load/wait/start/exit never replaces the actual player")
	var report: Dictionary = {"scope": "source controller/Session plus real native audio in invented structural day state; not played June/F9 recovery or accepted native provider",
		"checks": checks, "failures": failures, "polls": polls, "same_player": native_sink._active == player,
		"initial_native": initial, "final_native": after, "steps": steps,
		"source_state": harness["state"].projection(), "native_observations": native_service.month_observations,
		"rejected_attempts": rejected_attempts, "native_parent_review": "V4 corrective successor; independent recheck pending", "actual_june_route": false}
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_CALENDAR_OUTPUT").path_join("native-integration.json"), FileAccess.WRITE)
	if output != null:
		output.store_string(JSON.stringify(report, "\t"))
		output.close()
	native_service.reset_month_start_owner()
	host.queue_free()
	native_service = null
	native_backend = null
	native_sink = null
	# Match the native parent's asynchronous AudioServer teardown. This runs
	# after all source assertions; no Session/native driver frame advances.
	await process_frame
	await create_timer(0.2).timeout
	print(JSON.stringify({"checks": checks, "failures": failures, "polls": polls, "actual_june_route": false}))
	quit(0 if failures.is_empty() else 1)
