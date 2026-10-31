extends RefCounted
## T-29025: one AudioService-owned first-start transaction and native sink.
## Native initialization and queue preparation occur outside Session callbacks.
const Context = preload("res://services/first_arrival_audio_context.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Same = preload("res://services/first_arrival_launch_resources.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const ReiBank = preload("res://services/rei_bank_resources.gd")
const ShioriBank = preload("res://services/prologue_shiori_bank_resources.gd")
const Bank0 = preload("res://services/prologue_bank0_audio.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const LIBRARY := "res://bin/liboriginal_audio.linux.template_release.arm64.so"
const LIBRARY_SHA := "f1eecf80c1c6538a33f6bee9c7d10a79553b1745d520688f556bbe60fb1687b9"
const DESCRIPTOR := "res://original_audio.gdextension"
const DESCRIPTOR_SHA := "9bf0cf85a0154defa2d1e440cb314be441e5ebe57889b2a2b57c86cf16003a72"
## Stage10/substep8 Shiori bank race (T-39002, decided path race-emulation):
## the native core lands the CD reads over the live Rei score/header on the
## measured schedule in ShioriBank and runs the C1 fade per audio pump.
const SHIORI_NATIVE_UNAVAILABLE := "the qualified native library has no Shiori bank race support"
## native-bank0 (stage16 bank0 BGM, stage20/21 D1, stage21 COM_MIX/SPRING landing,
## 0x11 transfer, spring start): see services/prologue_bank0_audio.gd.
const BANK0_NATIVE_UNAVAILABLE := "the qualified native library has no bank0 (COM_MIX/SPRING) transfer support"
var _bank0: Dictionary = {}
var _finish_store: FinishBank
var _audio: WeakRef
var _session: WeakRef
var _live: Script
var _backend: RefCounted
var _sink: RefCounted
var _original: Dictionary = {}
var _ticket := 0
var _serial := 0
var _generation := 1
var _before: Dictionary = {}
var _after: Dictionary = {}
var _owner: Dictionary = {}
var _intent: Dictionary = {}
var _committed_after: Dictionary = {}
var _barrier_tick := -1
var _barrier_result_id := 0
var _published_tick := -1
var _last_pump_us := 0
var _max_pump_gap_us := 0
var _max_pump_gap_context: Dictionary = {}
var _retained_banks: Dictionary = {}
var _rei_epoch := 0
var _rei_started := false
var _rei_fade_ticks := 0
var _rei_frame_tick := -1
var _rei_result_id := 0
var _rei_intents: Array = []
var _rei_prepared: Dictionary = {}
var _shiori_inputs: Dictionary = {}
var _shiori_error := "Original Shiori bank inputs were not prepared"
var _shiori: Dictionary = {}
var _shiori_poll_tick := -1
## Logic-locked Rei/Shiori audio clock (T-39002 review). The original runs one
## audio_command_pump (SsSeqCalledTbyT, sector landing, C1 fade step) per
## vsync. The default backend pump refills the sink look-ahead from wall-clock
## callbacks, so the Rei sequencer position at the C1 frame would depend on
## host buffering. From Rei start the bridge renders exactly one native block
## per committed logic frame: rendered == logic_iterations - rei_tick. Blocks
## the sink cannot take yet wait in a bounded FIFO; the native state is never
## advanced ahead of logic.
const RACE_FIFO_LIMIT := 120
var _race: Dictionary = {}

## Exported Linux players ship the library beside the executable, not in the
## pack; Godot's GDExtension loader resolves it there (tools/port/build_desktop.sh).
static func library_path() -> String:
	if OS.has_feature("template"):
		return OS.get_executable_path().get_base_dir().path_join(LIBRARY.get_file())
	return LIBRARY

## Platform and reviewed-hash gates for the native component; "" when both pass.
static func native_component_error() -> String:
	if OS.get_name() != "Linux" or Engine.get_architecture_name() != "arm64":
		return "original opening BGM has no qualified native library for this platform"
	if FileAccess.get_sha256(library_path()) != LIBRARY_SHA or FileAccess.get_sha256(DESCRIPTOR) != DESCRIPTOR_SHA:
		return "original opening BGM native library differs from the reviewed component"
	return ""

func prepare(audio: RefCounted, host: Node, source_root: String) -> String:
	if _audio != null: return "original BGM owner was already initialized"
	var component_error := native_component_error()
	if not component_error.is_empty(): return component_error
	if not GDExtensionManager.is_extension_loaded(DESCRIPTOR):
		var status := GDExtensionManager.load_extension(DESCRIPTOR)
		if status != GDExtensionManager.LOAD_STATUS_OK: return "original BGM native extension registration failed"
	if not ClassDB.class_exists("OriginalAudioEngine"): return "original BGM native class was not registered"
	# Typed native adapters cannot be parsed before registration above.
	var source_script = load("res://bgm_source_pack.gd")
	var backend_script = load("res://bgm_backend.gd")
	var sink_script = load("res://bgm_sink.gd")
	_live = load("res://bgm_live_context.gd")
	if source_script == null or backend_script == null or sink_script == null or _live == null:
		return "original BGM adapters could not be loaded"
	_original = source_script.read_pack(source_root)
	if not _original.get("error", "missing source result").is_empty(): return str(_original.error)
	_audio = weakref(audio)
	# Pre-transaction I/O for the stage10/8 Shiori reads. A missing or changed
	# input is not fatal here; that transition alone is then refused.
	var pack_root: Variant = audio.get("opening_pack_root")
	if pack_root is String and not pack_root.is_empty():
		var shiori := ShioriBank.read_inputs(pack_root)
		_shiori_error = shiori.error
		if shiori.error.is_empty(): _shiori_inputs = shiori.inputs
	_sink = sink_script.new(host)
	_backend = backend_script.new(_sink)
	return ""

func bind_session(session: Session) -> String:
	if _session != null or _audio == null or _backend == null: return "original BGM session ownership is unavailable or reused"
	_session = weakref(session)
	session.final_precommit = final_precommit
	return ""

func _derive(state: State, context: Dictionary) -> Dictionary:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null or not context.get("state_flags") is int: return {"error":"actual native audio context is unavailable"}
	return _live.derive(state,audio,_original,context.state_flags,
		{"preset":context.get("reverb_type"),"depth":60 if context.get("reverb_depth") == [60,60] else -1,"enabled":context.get("reverb_enabled")})

func bank_published(role: String, epoch: int) -> String:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null: return "original BGM AudioService was disposed"
	var published := Context.bank_publication(audio.native_audio_context,audio,role,epoch)
	if not published.error.is_empty(): return published.error
	audio.native_audio_context = published.context.duplicate(true)
	return ""

func bank_completed(state: State) -> String:
	var audio = _audio.get_ref() if _audio != null else null
	if audio == null: return "original BGM AudioService was disposed"
	var context: Dictionary = audio.native_audio_context
	var bank: Dictionary = state.opening.get("audio_bank",{})
	if bank.get("role") == "prologue_rei":
		return "" if bank.get("ready") == true and bank.get("epoch") == _rei_epoch and audio.active_bank_role == "prologue_rei" else "Rei completion differs from actual alternate publication"
	if bank.get("role") == "prologue_shiori":
		# The transaction completion only acknowledges the published load; CD
		# readiness is the measured native schedule checked by shiori_poll.
		return "" if bank.get("ready") == true and not _shiori.is_empty() and bank.get("epoch") == _shiori.epoch and audio.active_bank_role == "prologue_shiori" else "Shiori completion differs from the actual in-flight load"
	if context.get("phase") != "prologue_bank_published" or bank.get("ready") != true or bank.get("epoch") != context.get("bank_epoch") or bank.get("role") != context.get("active_bank_role"):
		return "consumed bank completion differs from actual native publication"
	state.opening["native_audio_context"] = context.duplicate(true)
	return ""

func prepare_pending(state: State, _events: Array[Session.ServiceEvent]) -> String:
	if _backend == null: return "original opening BGM backend is unavailable"
	if not _committed_after.is_empty() or _ticket != 0: return ""
	var audio = _audio.get_ref()
	if audio == null: return "original BGM AudioService was disposed"
	if audio.native_audio_context.get("phase") != "prologue_bank_published": return ""
	# The actual source has scene_reset at substep0 between bank load and
	# bank_poll/start at substep1. Prepare only after the completion frame has
	# committed, on the logic frame whose bank_poll returns 1 (CD reads and
	# SPU transfer frames elapsed, AudioRules.BANK_READ_SECTORS); the session
	# increments frame_count after the mode handler, so this sees its frame.
	if state.game_mode != 0x15 or state.mode_stage != 0 or state.mode_step != 1 or state.substep != 1: return ""
	if AudioRules.bank_poll(state) != 1: return ""
	var context: Dictionary = state.opening.get("native_audio_context",{})
	var plan := Context.start_plan(context,_original.files[5],0,0)
	if not plan.error.is_empty(): return plan.error
	var derived := _derive(state,context)
	if not derived.error.is_empty(): return derived.error
	var prepared: Dictionary = _backend.prepare_start(derived.owner,derived.owner.epoch,_original.files)
	if not prepared.error.is_empty(): return prepared.error
	_ticket = prepared.ticket
	_serial += 1
	_before = plan.before.duplicate(true)
	_after = plan.after.duplicate(true)
	_owner = derived.owner.duplicate(true)
	_intent = {"kind":"original_bgm_start","role":"prologue_arrival","bank_epoch":_owner.epoch,"request_serial":_serial,"session_generation":_generation}
	return ""

func bind_finish_bank(store: FinishBank) -> String:
	if _finish_store != null: return "bank0 finish reads are already bound"
	if store == null or store.identity() != FinishBank.TRACK_SHA: return "bank0 finish reads are not authenticated"
	_finish_store = store
	return ""

func _bank0_native_ready() -> String:
	if _backend == null or _backend.get("_engine") == null: return "bank0 operation has no original native engine"
	for method: String in ["submit_command","bank0_start","begin_bank0_load","bank0_diagnostics"]:
		if not _backend._engine.has_method(method): return BANK0_NATIVE_UNAVAILABLE
	if _race.is_empty() or _shiori.is_empty() or not _shiori.get("started"): return "bank0 operation requires the logic-locked clock after the Shiori start"
	return ""

func _full_gain(state: State) -> int:
	var full: Variant = state.opening.get("native_audio_context", {}).get("bgm_volume_full")
	return int(full) if full in [69, 93] else -1

## audio_bank_start(0,n) (frame_render_audio.c): stage16 (0,1) before the
## stage21 reads, the command-reload (0,0) after the 0x11 transfer. The pack
## word is read from the authenticated bytes resident at 0x8001C000.
func _bank0_start(state: State, sequence: int, result: Session.FrameResult) -> void:
	var native := _bank0_native_ready()
	if not native.is_empty(): result.error = native; return
	var session = _session.get_ref() if _session != null else null
	var full := _full_gain(state)
	if session == null or full < 0: result.error = "bank0 start lacks its Session or output-mode gain"; return
	var record: Variant = state.opening.get("bank0_audio")
	if sequence == 1:
		if state.game_mode != 0x15 or state.mode_stage != 16 or state.mode_step != 0 or state.substep != 4 or record != null or not _bank0.is_empty():
			result.error = "bank0 sequence 1 start is outside stage16 case4"; return
		# PRO_VOL.SEQ is what boot placed at 0x8001C000; no read reaches it before stage21.
		if Bank0.pack_word0(_original.files[5]) & 0xffff != 0x40:
			state.opening["bank0_audio"] = {"phase":"stage16_ignored"}; return # source -1, nothing queued
		AudioRules.bank0_stage16_start(state, Bank0.vab_volume(_original.files[3]) - 25 - (24 if full == 69 else 0), result)
		if not result.error.is_empty(): return
		state.opening["bank0_audio"] = {"phase":"stage16","bgm_set":0,"sequence":1,"playing":1,"start_frame":int(state.frame_count)}
		result.audio.append({"kind":"native_bank0_start","sequence":1,"tick_skip":Bank0.STAGE16_TICK_SKIP})
		return
	# sequence 0: settings_command_screen_reload_calendar substep0 after poll == 1.
	if state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step != 1 or state.substep != 0 or not record is Dictionary or record.get("phase") != "transferred" or record.get("ready_frame") != int(state.frame_count):
		result.error = "bank0 sequence 0 start lacks the same-frame transfer-ready poll"; return
	var reads := Bank0.native_inputs(_finish_store)
	if not reads.error.is_empty(): result.error = reads.error; return
	if Bank0.pack_word0(reads.inputs[2]) & 0xffff != 0x40:
		result.error = "SPRING.SEQ pack word differs from the source 0x40 header"; return
	var gain: int = Bank0.vab_volume(reads.inputs[4]) - 25 - (24 if full == 69 else 0)
	AudioRules.bank0_spring_start(state, gain)
	record["phase"] = "spring"; record["bgm_set"] = 0; record["sequence"] = 0; record["playing"] = 1
	result.audio.append({"kind":"native_bank0_start","sequence":0,"tick_skip":Bank0.SPRING_TICK_SKIP})

## Media-bridge owner check for audio_command(0x11) (stage21 bank poll).
func bank0_transfer_owner(state: State) -> String:
	var native := _bank0_native_ready()
	if not native.is_empty(): return native
	var record: Variant = state.opening.get("bank0_audio")
	if _bank0.get("phase") != "loading" or not record is Dictionary or record.get("phase") != "loading": return "0x11 has no native bank0 load in flight"
	return ""

## audio_bgm_bank_ready(1) for the reload polls after the 0x11 frame T. The
## source reads the driver's own slot-1 request/in-progress bits, so the native
## engine (the driver here) answers: slot1 is ready once its transfer request
## is gone. Its answer must also agree with the measured T+2 frame
## (Bank0.TRANSFER_READY_FRAMES) the logic mirror uses; a disagreement is
## refused instead of being papered over by either side.
func bank0_slot_state(state: State) -> Dictionary:
	var native := _bank0_native_ready()
	if not native.is_empty(): return {"error":native}
	var session = _session.get_ref() if _session != null else null
	if session == null or session.state != state: return {"error":"bank0 slot poll lost its Session"}
	var record: Variant = state.opening.get("bank0_audio")
	if _bank0.get("phase") != "transferred" or not record is Dictionary or record.get("phase") != "transferred":
		return {"error":"bank0 slot poll has no native 0x11 transfer"}
	var tick: int = session.clock.logic_iterations
	var elapsed: int = tick - int(_bank0.transfer_tick)
	if elapsed < 1 or elapsed != int(state.frame_count) - int(record.get("transfer_frame", -1)):
		return {"error":"bank0 slot poll at +%d is not after the 0x11 frame" % elapsed}
	return _bank0_slot_now(tick)

func _bank0_slot_now(tick: int) -> Dictionary:
	# Blocks for the frames already committed; the same catch-up pump() does.
	var caught := _race_pump(tick - int(_race.rei_tick))
	if not caught.is_empty(): return {"error":caught}
	return _bank0_transfer_answer(tick - int(_bank0.transfer_tick))

## Native slot-1 readiness `elapsed` frames after the 0x11 frame, checked
## against the measured schedule: requests {0,1} -> {1} -> {} (slot0 moves in
## the first pump after 0x11, slot1 in the second).
func _bank0_transfer_answer(elapsed: int) -> Dictionary:
	var d: Dictionary = _backend._engine.bank0_diagnostics()
	var requests := int(d.get("transfer_requests", -1))
	if d.get("bank0_loaded") != true or requests < 0 or (requests & ~3) != 0 or not d.get("transfer_done") is bool or d.transfer_done != (requests == 0):
		return {"error":"native bank0 transfer state is outside the 0x11 slot0/slot1 schedule: " + str([d.get("bank0_loaded"), d.get("transfer_done"), requests])}
	# Slot0 always moves first, so "slot1 bit clear" and "no request left" agree.
	var ready: bool = d.transfer_done
	if ready != (elapsed >= Bank0.TRANSFER_READY_FRAMES):
		return {"error":"native slot1 transfer (ready %s, requests %d) at +%d differs from the measured ready frame +%d" % [ready, requests, elapsed, Bank0.TRANSFER_READY_FRAMES]}
	return {"error":"","ready":ready}

func start(state: State, bank: int, sequence: int, result: Session.FrameResult) -> void:
	if bank == 0 and sequence in [0, 1] and (state.game_mode == 0x34 or state.mode_stage == 16):
		_bank0_start(state, sequence, result)
		return
	if bank == 1 and sequence == 0 and state.opening.get("audio_bank",{}).get("role") == "prologue_shiori":
		var session = _session.get_ref() if _session != null else null
		if _shiori.is_empty() or _shiori.get("started") or session == null or _shiori_poll_tick != session.clock.logic_iterations or state.opening.audio_bank.get("epoch") != _shiori.epoch:
			result.error = "Shiori start has no same-frame bank poll of its in-flight load"; return
		AudioRules.bank_start(state,bank,sequence,result)
		return
	if bank == 1 and sequence == 0:
		if _rei_epoch == 0 or _rei_started or state.opening.get("audio_bank",{}).get("epoch") != _rei_epoch:
			result.error = "Rei start has no owned completed alternate bank"; return
		AudioRules.bank_start(state,bank,sequence,result)
		return
	if bank != 0 or sequence != 0 or _ticket == 0:
		result.error = "original first-arrival BGM was not privately prepared"
		return
	if not Same._same(state.opening.get("native_audio_context"),_before):
		result.error = "original BGM source context differs from private preparation"
		return
	var derived := _derive(state,_before)
	if not derived.error.is_empty() or not Same._same(derived.get("owner"),_owner):
		result.error = "original BGM actual bank ownership changed"
		return
	result.error = _backend.validate_precommit(_ticket,_owner)
	if not result.error.is_empty(): return
	state.opening.native_audio_context = _after.duplicate(true)
	result.audio.append(_intent.duplicate(true))

func preflight_event(event: Dictionary, state: State = null) -> String:
	if (_ticket == 0 and _committed_after.is_empty()) or not Same._same(event,_intent): return "original BGM intent differs from prepared ownership"
	if state != null and not Same._same(state.opening.get("native_audio_context"),_after): return "original BGM intent differs from transactional after-state"
	return ""

func final_precommit(state: State, _rng, result: Session.FrameResult) -> void:
	var starts := 0
	for event: Dictionary in result.audio:
		if event.get("kind") == "original_bgm_start":
			starts += 1
			result.error = preflight_event(event,state)
			if not result.error.is_empty(): return
	if _ticket == 0:
		if starts != 0: result.error = "original BGM start has no private preparation"
		if result.error.is_empty(): result.error = _preflight_rei(state,result)
		if result.error.is_empty() and _has_rei_action(result.audio):
			var session = _session.get_ref() if _session != null else null
			if session == null or session.state != state or _rei_result_id != 0:
				result.error = "Rei transition lost its unique Session publication barrier"; return
			_rei_frame_tick = session.clock.logic_iterations
			_rei_result_id = result.get_instance_id()
			_rei_intents = result.audio.duplicate(true)
			for event: Dictionary in result.audio:
				if event.get("kind") == "bank_load" and event.get("role") == "prologue_rei":
					_rei_prepared = _audio.get_ref().prepared_bank_roles.prologue_rei.duplicate(true)
		return
	# A prepared token is for exactly this next source start, never a later frame.
	if starts != 1:
		result.error = "original BGM requires exactly one source start intent"
		return
	var audio = _audio.get_ref()
	var session = _session.get_ref()
	if audio == null or session == null or session.state != state or not Same._same(audio.native_audio_context,_before):
		result.error = "original BGM service or Session generation changed"
		return
	var derived := _derive(state,_after)
	var expected := _owner.duplicate(true)
	expected.state_flags = _after.state_flags
	if not derived.error.is_empty() or not Same._same(derived.get("owner"),expected):
		result.error = "original BGM final actual bank or context changed"
		return
	result.error = _backend.validate_precommit(_ticket,_owner)
	if result.error.is_empty():
		_barrier_tick = session.clock.logic_iterations
		_barrier_result_id = result.get_instance_id()

func publish(result: Session.FrameResult, tick: int) -> String:
	if not result.committed: return "aborted frame cannot publish original BGM"
	var starts := 0
	for event: Dictionary in result.audio:
		if event.get("kind") is String and event.kind == "original_bgm_start":
			starts += 1
			if not Same._same(event,_intent): return "committed original BGM intent changed"
	if _ticket == 0:
		if starts != 0: return "original BGM start token was consumed or missing"
		if not _has_rei_action(result.audio) and _rei_result_id == 0: return ""
		var session = _session.get_ref() if _session != null else null
		if session == null or result.get_instance_id() != _rei_result_id or _rei_frame_tick != tick or session.clock.logic_iterations != tick+1 or result.state_hash != session.state_hash() or not Same._same(result.audio,_rei_intents):
			return "Rei publication lost its exact validated frame, intents or state"
		if not _rei_prepared.is_empty() and not Same._same(_rei_prepared,_audio.get_ref().prepared_bank_roles.get("prologue_rei")):
			return "Rei prepared input changed after the final barrier"
		_rei_frame_tick = -1; _rei_result_id = 0; _rei_intents.clear()
		var outcome := _publish_rei(result,tick)
		_rei_prepared.clear()
		return outcome
	if starts != 1 or result.get_instance_id() != _barrier_result_id: return "original BGM publication lost its exact validated frame result"
	var session = _session.get_ref()
	if session == null or _barrier_tick != tick or session.clock.logic_iterations != tick+1 or result.state_hash != session.state_hash(): return "original BGM publication lost the same-stack final barrier"
	# First publisher, same synchronous stack. No loading, generation, yielding
	# or other host publication occurs between the barrier and activation.
	var outcome: String = _backend.commit_start(_ticket,_owner)
	if not outcome.is_empty(): return outcome
	var audio = _audio.get_ref()
	audio.native_audio_context = _after.duplicate(true)
	audio.bgm_state = {"playing":true,"role":_owner.role,"bank":0,"sequence":0}
	_committed_after = _after.duplicate(true)
	_retained_banks = audio.vab_memory.banks.duplicate(true)
	_last_pump_us = Time.get_ticks_usec()
	_published_tick = tick
	_ticket = 0
	return ""

func abort_frame() -> void:
	if _ticket != 0 and _backend != null: _backend.discard(_ticket)
	_ticket = 0
	_barrier_tick = -1
	_barrier_result_id = 0
	_before.clear()
	_after.clear()
	_intent.clear()
	_rei_frame_tick = -1; _rei_result_id = 0; _rei_intents.clear()
	_rei_prepared.clear()
	_shiori_poll_tick = -1
	if _committed_after.is_empty(): _owner.clear()

func pump(state: State) -> String:
	if _committed_after.is_empty(): return ""
	var audio = _audio.get_ref()
	if audio == null or not Same._same(audio.native_audio_context,_committed_after) or not Same._same(state.opening.get("native_audio_context"),_committed_after): return "active original BGM source after-context changed"
	var pending: Variant = state.opening.get("audio_bank")
	var bank: Variant = state.opening.get("audio_banks",{}).get(_owner.get("role"),pending)
	var transition_epoch: Variant = state.opening.get("prologue_bgm_transition",{}).get("bank_epoch")
	var allowed_pending: bool = pending is Dictionary and (pending == bank or (pending.get("role") == "prologue_rei" and pending.get("epoch") == int(_owner.epoch)+1 and pending.get("ready") is bool and transition_epoch == pending.epoch) \
		or (pending.get("role") == "prologue_shiori" and _rei_started and pending.get("epoch") == _rei_epoch+1 and pending.get("ready") is bool and transition_epoch == pending.epoch and (_shiori.is_empty() or _shiori.epoch == pending.epoch)))
	var active_role := "prologue_shiori" if not _shiori.is_empty() else ("prologue_rei" if _rei_epoch != 0 else str(_owner.role))
	if not _bank0.is_empty():
		# native-bank0: prologue_finish_run's audio_bank_state_reset replaces the
		# Shiori bank record by an empty pending one; the software VAB owner keeps
		# the Shiori roles (the native engine alone holds COM_MIX/SPRING).
		var shiori_record: bool = pending is Dictionary and pending.get("role") == "prologue_shiori" and pending.get("epoch") == _shiori.epoch and pending.get("ready") == true
		var reset_record: bool = pending is Dictionary and pending.get("role") == "" and pending.get("epoch") == int(_shiori.epoch)+1 and pending.get("ready") == false
		allowed_pending = (shiori_record or reset_record) and state.opening.get("bank0_audio") is Dictionary
		bank = state.opening.get("audio_banks",{}).get(_owner.get("role"))
		var fade: Variant = state.opening.get("bgm_fade")
		if fade is Dictionary and fade.get("command") == 0xd1 and fade.get("request") == true and (AudioRules.check_cd(state)&4) != 0:
			return Bank0.XA_UNSUPPORTED
	if not allowed_pending or not bank is Dictionary or bank.get("epoch") != _owner.get("epoch") or bank.get("role") != _owner.get("role") or bank.get("ready") != true or audio.active_bank_role != active_role or audio.bank_ready.get(_owner.get("role")) != true:
		return "active original BGM actual bank changed"
	for slot: int in [0,1]:
		if audio.vab_memory.banks.get(slot) != _retained_banks.get(slot): return "active original BGM retained slot changed"
	if not _shiori.is_empty() and not _shiori.get("started") and (AudioRules.check_cd(state)&4) != 0:
		# The native C1 fade runs in master-volume mode (state_flags 0x100000,
		# no XA); the measured path has no XA until the stage11 poll.
		return "XA playback during the in-flight Shiori bank race is unsupported"
	var now := Time.get_ticks_usec()
	if now-_last_pump_us > _max_pump_gap_us:
		_max_pump_gap_us = now-_last_pump_us
		_max_pump_gap_context = {"frame":state.frame_count,"mode":state.game_mode,"stage":state.mode_stage,"step":state.mode_step,"substep":state.substep}
	_last_pump_us = now
	if not _race.is_empty():
		var session = _session.get_ref() if _session != null else null
		if session == null: return "logic-locked Rei clock lost its Session"
		return _race_pump(session.clock.logic_iterations - int(_race.rei_tick))
	return _backend.pump(_owner)

## Render native blocks up to exactly `target` blocks after Rei start, one
## per committed logic frame, never more. Only sink capacity is wall-clock.
func _race_pump(target: int) -> String:
	if _backend == null or _backend._engine == null: return "logic-locked Rei clock has no original native engine"
	if not _backend.error.is_empty(): return _backend.error
	if _backend._owner != _owner or _backend.sink.generation != _backend._active_sink_generation:
		return "logic-locked Rei clock lost its original engine/sink ownership"
	var validation: String = _backend.sink.validate_active(_backend._active_sink_generation)
	if not validation.is_empty(): return validation
	if int(_race.rendered) > target:
		return "logic-locked Rei clock rendered %d blocks ahead of logic %d" % [_race.rendered,target]
	var fifo: Array = _race.fifo
	while not fifo.is_empty() and _backend.sink.can_push():
		var pushed: String = _backend.sink.push(fifo.pop_front())
		if not pushed.is_empty(): _backend.error = pushed; return pushed
	while int(_race.rendered) < target:
		var block: Dictionary = _backend._engine.render_block()
		if not block.error.is_empty(): _backend.error = block.error; return block.error
		_race.rendered = int(_race.rendered)+1
		if fifo.is_empty() and _backend.sink.can_push():
			var pushed: String = _backend.sink.push(block.pcm)
			if not pushed.is_empty(): _backend.error = pushed; return pushed
		else: fifo.append(block.pcm)
		_race.max_fifo = maxi(int(_race.max_fifo),fifo.size())
		if fifo.size() > RACE_FIFO_LIMIT:
			_backend.error = "logic ran more than %d frames ahead of the audio sink during the logic-locked Rei clock" % RACE_FIFO_LIMIT
			return _backend.error
	return ""

func _preflight_rei(state: State, result: Session.FrameResult) -> String:
	if _committed_after.is_empty(): return ""
	var audio = _audio.get_ref()
	var seen: Dictionary = {}
	for event: Dictionary in result.audio:
		if _has_rei_action([event]) and not _gameplay_audio(event):
			if seen.has(event.get("kind")): return "Duplicate native Rei action in one frame"
			seen[event.kind] = event.get("command",event.get("role",true))
		if event.get("kind") == "bank_load" and event.get("role") == "prologue_shiori":
			# prologue.c:1733-1735: audio_command(5), audio_command(0xC1), then
			# audio_bank_load_start(1,0,0,0xCBD4,0xCBAA,0xCBA5) over the live Rei
			# score/header. next_stage() has already run: stage11/substep0.
			if _rei_epoch == 0 or not _rei_started or not _shiori.is_empty() or event.get("bank_epoch") != _rei_epoch+1 or state.mode_stage != 11 or state.mode_step != 0 or state.substep != 0:
				return "Shiori load differs from original stage10/substep8 transition"
			if seen.get("native_bgm_fade") != 0xc1: return "Shiori load lacks the preceding source C1 fade"
			if not _shiori_error.is_empty(): return _shiori_error
			var changed := ShioriBank.validate(_shiori_inputs)
			if not changed.is_empty(): return changed
			var native := _shiori_native_ready()
			if not native.is_empty(): return native
			if _race.is_empty(): return "Shiori race requires the logic-locked Rei audio clock"
		if event.get("kind") == "bank_load" and event.get("role") == "prologue_rei":
			if _rei_epoch != 0 or event.get("bank_epoch") != int(_owner.epoch)+1 or state.mode_stage != 7 or state.substep != 5:
				return "Rei load differs from original stage7 alternate-bank transition"
			var probe := ReiBank.Memory.new()
			var validation := ReiBank.publish(probe,audio.prepared_bank_roles.get("prologue_rei",{}))
			if not validation.is_empty(): return validation
		elif event.get("kind") == "native_bgm_fade" and event.get("command") == 0xc1:
			var fade: Variant = state.opening.get("bgm_fade")
			if not _rei_started or not _shiori.is_empty() or state.mode_stage != 11 or state.substep != 0 or (int(state.opening.get("audio_state_flags",0))&0x100000)==0 or not fade is Dictionary or fade.get("command") != 0xc1 or fade.get("rate") != 0xc0 or fade.get("decrement") != 1 or fade.get("ticks") != 0 or fade.get("active") != true:
				return "C1 fade is outside its source stage10/substep8 command"
			if (AudioRules.check_cd(state)&4) != 0: return "XA playback during the in-flight Shiori bank race is unsupported"
		elif event.get("kind") in ["native_bank0_start","native_bank0_load","native_audio_command","native_restore_bgm"] or (event.get("kind") == "native_audio_control" and not _bank0.is_empty()) or (event.get("kind") == "native_bgm_fade" and event.get("command") == 0xd1):
			var problem := _preflight_bank0(event, state, seen)
			if not problem.is_empty(): return problem
		elif event.get("kind") == "native_bgm_fade":
			if event.get("command") != 0xb1 or state.mode_stage != 7 or state.substep != 5 or (int(state.opening.get("audio_state_flags",0))&0x100000)==0 or not state.opening.get("bgm_fade") is Dictionary: return "Rei fade is outside its source command"
		elif event.get("kind") == "native_bgm_fade_tick":
			if not event.get("xa_active") is bool or event.xa_active != ((AudioRules.check_cd(state)&4)!=0) or event.get("tick") != _rei_fade_ticks+1 or event.get("tick") != state.opening.get("bgm_fade",{}).get("ticks") or event.get("gain") != state.opening.get("bgm_fade",{}).get("gain"):
				return "Rei fade tick differs from source state"
		elif event.get("kind") == "bgm_play" and event.get("role") == "prologue_shiori":
			var session = _session.get_ref() if _session != null else null
			if _shiori.is_empty() or _shiori.get("started") or session == null or _shiori_poll_tick != session.clock.logic_iterations or event.get("bank_epoch") != _shiori.epoch or event.get("bank") != 1 or event.get("sequence") != 0:
				return "Shiori play differs from its polled in-flight bank"
		elif event.get("kind") == "bgm_play":
			if event.get("role") != "prologue_rei" or event.get("bank_epoch") != _rei_epoch or event.get("bank") != 1 or event.get("sequence") != 0 or _rei_started:
				return "Rei play differs from its published alternate-bank owner"
	if (seen.get("native_bgm_fade") == 0xc1) != (seen.get("bank_load") == "prologue_shiori"):
		return "C1 fade and Shiori load must arrive together in the source stage10/substep8 frame"
	return ""

func _preflight_bank0(event: Dictionary, state: State, seen: Dictionary) -> String:
	var native := _bank0_native_ready()
	if not native.is_empty(): return native
	var session = _session.get_ref() if _session != null else null
	if session == null: return "bank0 operation lost its Session"
	var tick: int = session.clock.logic_iterations
	var record: Variant = state.opening.get("bank0_audio")
	if not record is Dictionary: return "bank0 operation has no source bank0 record"
	var closing: bool = state.game_mode == 0x15 and state.mode_stage == 21 and state.mode_step == 0 and state.substep in [0, 1]
	var fade: Variant = state.opening.get("bgm_fade")
	match event.kind:
		"native_bank0_start":
			if event.get("sequence") == 1:
				if not _bank0.is_empty() or record.get("phase") != "stage16" or state.mode_stage != 16 or state.substep != 5 or event.get("tick_skip") != Bank0.STAGE16_TICK_SKIP:
					return "bank0 sequence 1 start differs from stage16 case4"
			elif event.get("sequence") != 0 or _bank0.get("phase") != "transferred" or record.get("phase") != "spring" or tick - int(_bank0.transfer_tick) != Bank0.TRANSFER_READY_FRAMES or event.get("tick_skip") != Bank0.SPRING_TICK_SKIP:
				return "bank0 sequence 0 start differs from the transfer-ready reload poll"
			else:
				# The engine must have finished slot1 by now; otherwise its 0x200
				# would defer the start while the logic mirror plays SPRING.
				var slot := _bank0_slot_now(tick)
				if not slot.error.is_empty(): return slot.error
		"native_audio_control":
			if event.get("command") != 5 or not closing or _bank0.get("phase") != "stage16": return "bank0 audio_command(5) is outside prologue stages 20/21"
		"native_bgm_fade":
			if not closing or _bank0.get("phase") != "stage16" or seen.get("native_audio_control") != 5 or not fade is Dictionary or fade.get("command") != 0xd1 or fade.get("request") != true:
				return "D1 fade is outside its source stage20/21 commands"
			if (AudioRules.check_cd(state)&4) != 0: return Bank0.XA_UNSUPPORTED
			if fade.get("ignored", 0) not in [0, 1] or (fade.get("ignored", 0) == 0 and _bank0.has("d1_tick")) or (fade.get("ignored", 0) == 1 and _bank0.get("d1_tick") != tick-1):
				return "D1 fade differs from the stage20 command and the ignored stage21 repeat"
		"native_bank0_load":
			if not closing or state.substep != 1 or _bank0.get("phase") != "stage16" or _bank0.get("d1_tick") != tick-1 or record.get("phase") != "loading" or record.get("load_frame") != int(state.frame_count)-1:
				return "bank0 load differs from prologue_finish_run case0"
			var inputs := Bank0.native_inputs(_finish_store)
			if not inputs.error.is_empty(): return inputs.error
		"native_audio_command" when _gameplay_audio(event):
			# After the stage 21 handoff the gameplay's audio_command calls go to
			# the same sound driver: settings_command_screen_icon_confirm's
			# audio_command(3) (audio_xa_forget) and (0x500), once the SPRING
			# start of the reload has made bank0 the playing bank.
			if _bank0.get("phase") != "spring" or record.get("phase") != "spring" or state.game_mode == 0x15:
				return "gameplay audio_command(0x%x) has no playing bank0 owner" % int(event.get("command"))
		"native_restore_bgm":
			# restore_bgm (scheduler.c:86) and the confirm's audio_bgm_is_playing
			# check: audio_bgm_playing is 1 from the reload's audio_bank_start(0, 0)
			# (frame_render_audio.c:715) and nothing on this route stops it, so
			# neither starts the bank again. Any other state is not modeled.
			if event.get("bank") != 0 or event.get("sequence") != 0 or _bank0.get("phase") != "spring" or record.get("phase") != "spring" or record.get("playing") != 1:
				return "restore_bgm with audio_bgm_playing 0 (audio_bank_start(0, 0) again) is not modeled"
		"native_audio_command":
			if event.get("command") != 0x11 or _bank0.get("phase") != "loading" or record.get("phase") != "transferred" or state.game_mode != 0x34 or state.mode_stage != 0xff or state.mode_step != 1:
				return "0x11 differs from the reload bank poll"
			var elapsed: int = tick - int(_bank0.load_tick)
			if elapsed != Bank0.READ_DONE_FRAMES[4] or record.get("transfer_frame") - record.get("load_frame") != elapsed:
				return "0x11 poll at +%d differs from the measured reads-complete frame +%d" % [elapsed, Bank0.READ_DONE_FRAMES[4]]
	return ""

## audio_bank_load_poll at stage11/substep2. The first call always returns -1
## (frame_render_audio.c); with all three reads done it queues 0x15. The
## native core checks residency itself; this refuses a poll the measured
## schedule says would precede completion or the C1 fade finish.
func shiori_poll(state: State, result: Session.FrameResult) -> int:
	var session = _session.get_ref() if _session != null else null
	var bank: Variant = state.opening.get("audio_bank")
	if _shiori.is_empty() or _shiori.get("started") or session == null or session.state != state or not bank is Dictionary or bank.get("role") != "prologue_shiori" or bank.get("epoch") != _shiori.epoch or _shiori_poll_tick != -1:
		result.error = "Shiori bank poll has no owned in-flight load"; return 0
	if state.game_mode != 0x15 or state.mode_stage != 11 or state.mode_step != 0 or state.substep != 2:
		result.error = "Shiori bank poll is outside stage11/substep2"; return 0
	var elapsed: int = session.clock.logic_iterations - int(_shiori.tick)
	if elapsed < ShioriBank.READS_COMPLETE_PUMP:
		result.error = "Shiori CD reads are incomplete at the bank poll (+%d, measured complete at +%d)" % [elapsed,ShioriBank.READS_COMPLETE_PUMP]; return 0
	if elapsed <= ShioriBank.MEASURED.fade_finish:
		result.error = "Shiori dispose before the C1 fade finished is outside the measured path (+%d)" % elapsed; return 0
	_shiori_poll_tick = session.clock.logic_iterations
	return -1

func _shiori_native_ready() -> String:
	if _backend == null or _backend.get("_engine") == null: return "Shiori operation has no original native engine"
	for method: String in ["begin_fade","begin_shiori_load","finish_shiori"]:
		if not _backend._engine.has_method(method): return SHIORI_NATIVE_UNAVAILABLE
	return ""

func _shiori_native(method: String, args: Array) -> String:
	# Same ownership guards as bgm_backend.rei_action, for the race methods.
	var ready := _shiori_native_ready()
	if not ready.is_empty(): return ready
	if _backend._owner != _owner or _backend.sink.generation != _backend._active_sink_generation:
		return "Shiori operation lost its original engine/sink ownership"
	var validation: String = _backend.sink.validate_active(_backend._active_sink_generation)
	if not validation.is_empty(): return validation
	return _backend._engine.callv(method,args)

## audio_command calls of the gameplay after the handoff; 0x11 stays the
## reload's bank0 transfer. audio_command(3) is audio_xa_forget (the command
## select confirm); 0x5PN is audio_command_se_a, the class-5 effect of VAB
## se_slot_a (the confirm's 0x500, the school day's 0x501/0x502 in
## school_day.c:460-463), and 0x15PN its key off (sound_driver.c:725-735).
const GAMEPLAY_AUDIO_COMMANDS := [3]

static func _gameplay_audio(event: Dictionary) -> bool:
	if event.get("kind") != "native_audio_command" or not event.get("command") is int: return false
	var command: int = event.command
	return command in GAMEPLAY_AUDIO_COMMANDS or (command >= 0x500 and command <= 0x5ff) or (command >= 0x1500 and command <= 0x15ff)

func _has_rei_action(events: Array) -> bool:
	for event: Dictionary in events:
		if event.get("kind") in ["native_bank0_start","native_bank0_load","native_audio_command","native_restore_bgm"]: return true
		if event.get("kind") == "native_audio_control" and not _bank0.is_empty(): return true
		if event.get("kind") in ["native_bgm_fade","native_bgm_fade_tick","bgm_play"] or (event.get("kind") == "bank_load" and event.get("role") in ["prologue_rei","prologue_shiori"]): return true
	return false

func _publish_rei(result: Session.FrameResult, tick: int = -1) -> String:
	var audio = _audio.get_ref() if _audio != null else null
	if not _race.is_empty():
		# This frame's native commands follow exactly tick-rei_tick pumps.
		var caught := _race_pump(tick - int(_race.rei_tick))
		if not caught.is_empty(): return caught
		if int(_race.rendered) != tick - int(_race.rei_tick): return "logic-locked Rei clock is not at frame %d" % tick
	for event: Dictionary in result.audio:
		var outcome := ""
		match event.get("kind"):
			"bank_load":
				if event.get("role") == "prologue_rei":
					var inputs: Dictionary = audio.prepared_bank_roles.prologue_rei.inputs
					outcome = _backend.rei_action(_owner,"load",[inputs.music_header,inputs.music_body,inputs.sequence])
					if outcome.is_empty(): _rei_epoch = event.bank_epoch
				elif event.get("role") == "prologue_shiori":
					outcome = ShioriBank.validate(_shiori_inputs)
					if outcome.is_empty(): outcome = _shiori_native("begin_shiori_load",[ShioriBank.native_inputs(_shiori_inputs),ShioriBank.flat_schedule()])
					if outcome.is_empty(): _shiori = {"epoch":event.bank_epoch,"tick":tick,"started":false,"phase":int(_race.rendered)}
			"native_bank0_start":
				if event.sequence == 0:
					var slot := _bank0_slot_now(tick)
					if not slot.error.is_empty(): return slot.error
				var started: Dictionary = _backend._engine.bank0_start(event.sequence, event.tick_skip)
				outcome = str(started.get("error", "bank0 start failed"))
				if outcome.is_empty() and started.get("status") != 1: outcome = "native pack word at 0x8001C000 differs from the source start decision"
				if outcome.is_empty():
					if event.sequence == 1: _bank0 = {"phase":"stage16","start_tick":tick}
					else: _bank0.phase = "spring"; _bank0["spring_tick"] = tick
			"native_audio_control":
				if not _bank0.is_empty(): outcome = _backend._engine.submit_command(5)
			"native_bank0_load":
				var reads := Bank0.native_inputs(_finish_store)
				outcome = reads.error
				if outcome.is_empty(): outcome = _backend._engine.begin_bank0_load(reads.inputs, Bank0.flat_schedule())
				if outcome.is_empty(): _bank0.phase = "loading"; _bank0["load_tick"] = tick
			"native_audio_command" when _gameplay_audio(event):
				outcome = _backend._engine.submit_command(int(event.command))
			"native_restore_bgm":
				pass # audio_bgm_playing is 1: restore_bgm starts nothing.
			"native_audio_command":
				var landed: Dictionary = _backend._engine.bank0_diagnostics()
				if int(landed.get("bank0_load_pump", -1)) != tick - int(_bank0.load_tick) or landed.get("bank0_complete") != true:
					outcome = "native bank0 landing +%s differs from logic +%d at the 0x11 poll" % [landed.get("bank0_load_pump"), tick - int(_bank0.load_tick)]
				else: outcome = _backend._engine.submit_command(0x11)
				if outcome.is_empty(): _bank0.phase = "transferred"; _bank0["transfer_tick"] = tick
			"native_bgm_fade":
				if event.get("command") == 0xd1:
					outcome = _backend._engine.submit_command(0xd1)
					if outcome.is_empty() and not _bank0.has("d1_tick"): _bank0["d1_tick"] = tick
				elif event.get("command") == 0xc1:
					var fade: Array = AudioRules.BGM_FADES[0xc1]
					outcome = _shiori_native("begin_fade",[fade[0],fade[1],true])
				else: outcome = _backend.rei_action(_owner,"fade")
			"native_bgm_fade_tick":
				outcome = _backend.rei_action(_owner,"tick",[],event.xa_active)
				if outcome.is_empty(): _rei_fade_ticks = event.tick
			"bgm_play":
				if event.get("role") == "prologue_shiori":
					# 0x15 dispose/transfer and 0x23/0x24/0x2E/0x200 run in the
					# native core's next audio pump (measured +182).
					var pumps := int(_backend._engine.diagnostics().get("shiori_pump",-1))
					if pumps != tick - int(_shiori.tick): outcome = "native Shiori pump +%d differs from logic +%d at the bank poll" % [pumps,tick - int(_shiori.tick)]
					else: outcome = _shiori_native("finish_shiori",[])
					if outcome.is_empty(): _shiori.started = true; _shiori_poll_tick = -1
				else:
					outcome = _backend.rei_action(_owner,"start",[],(AudioRules.check_cd(_session.get_ref().state)&4)!=0)
					if outcome.is_empty():
						_rei_started = true
						_race = {"rei_tick":tick,"rendered":0,"fifo":[],"max_fifo":0}
		if not outcome.is_empty(): return outcome
	return ""

func pause(value: bool) -> void:
	if _sink != null: _sink.pause(value)

func shiori_prepared_role(audio: RefCounted) -> Dictionary:
	# Software VAB bookkeeping is unchanged at stage10/8: nothing reaches SPU
	# until the stage11 poll, so publication restores the current checkpoint.
	if not _shiori_error.is_empty(): return {"error":_shiori_error}
	return {"error":"","memory":audio.vab_memory.checkpoint(),"inputs":_shiori_inputs,"sequence":PackedByteArray(),
		"metadata":{"role":"prologue_shiori","slot":2,"address":0x39010,"native_transfer":"stage11/substep2 poll"}}

func diagnostics() -> Dictionary:
	return {"bank0":_bank0.duplicate(),"race_clock":{"rei_tick":_race.rei_tick,"rendered":_race.rendered,"fifo":_race.fifo.size(),"max_fifo":_race.max_fifo} if not _race.is_empty() else {},"shiori":_shiori.duplicate(),"active_skips":_sink.skips() if _sink != null else 0,"maximum_pump_gap_us":_max_pump_gap_us,"maximum_pump_gap_context":_max_pump_gap_context.duplicate(),"private_ticket":_ticket,"published_tick":_published_tick,"history":_backend.history.duplicate(true) if _backend != null else [],"sink":_sink.metrics() if _sink != null else {},"engine":_backend.diagnostics() if _backend != null else {}}

func reset() -> void:
	abort_frame()
	if _backend != null: _backend.reset()
	var audio = _audio.get_ref() if _audio != null else null
	if audio != null: audio.bgm_state = {"playing":false,"role":"","bank":-1,"sequence":-1}
	_generation += 1
	_committed_after.clear()
	_owner.clear()
	_retained_banks.clear()
	_rei_epoch = 0; _rei_started = false; _rei_fade_ticks = 0
	_shiori.clear(); _shiori_poll_tick = -1; _race.clear(); _bank0.clear()
