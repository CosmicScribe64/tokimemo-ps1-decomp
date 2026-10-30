extends SceneTree
## T-39002: stage 21 prologue_finish_run (prologue.c:2714-2741) and the start
## of mode 0x34 stage 0xFF. Part 1 drives Arrival.step with test doubles for
## source order, dispatch, START, RNG and rollback. Part 2 uses the original
## board pack (BOARD_PACK) for the retained O12 actor and the exported stage21
## sector reads (FINISH_BANK_PACK) for the five audio_bank_load_start reads,
## their completion order, the command-reload poll and its explicit refusal.
## Stages 12-20 are not reachable on the actual route, so the stage21 state
## enters through the START skip (mode_stage_set(0x15)) from the stage11 O12
## state that run_rei_line17.gd builds from real board bytes. Part 3 commits
## stage21 frames on a real Session bound to a prepared media bridge
## (FIRST_ARRIVAL_MEDIA_ROOT/SHA) and checks preflight and publish: the five
## buffers, the completions cd_command_reset forgets, and their delivery.
const Flow = preload("res://flow/prologue_arrival.gd")
const BankLoader = preload("res://services/native_bank_loader.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const Handoff = preload("res://flow/opening_command_handoff.gd")
const Board = preload("res://services/first_arrival_board_store.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
const START := 0x0800
var log: Array = []
var fail_on := ""
var failures := 0
var checks := 0
var completed: Array = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr(label)

func _rng() -> Rng:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 0x24681357)
	return rng

func _double(name: String) -> Callable:
	return func(a = null, b = null, c = null, d = null, e = null) -> Variant:
		var args: Array = [a, b, c, d, e].filter(func(v): return not (v is State or v is Session.FrameResult or v == null))
		log.append([name] + args)
		for v: Variant in [a, b, c, d, e]:
			if v is Session.FrameResult and fail_on == name: v.error = "double refused " + name
		return 0

func _doubles() -> Dictionary:
	var services: Dictionary = {}
	for key: String in Flow.REQUIRED + Flow.FINISH_SERVICES:
		services[key] = _double(key)
	return services

func _finish_state(substep: int = 0) -> State:
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 21; s.mode_step = 0; s.substep = substep
	s.opening["screen_brightness"] = 0x40
	s.opening["text_speed"] = 5
	s.opening["D_800B3DB0"] = 1
	s.rule_state["menu_mode"] = 0
	return s

func _names() -> Array: return log.map(func(e): return e[0])

func _flow_checks() -> void:
	var services := _doubles()
	var rng := _rng()
	var before_rng: Dictionary = rng.checkpoint()
	var s := _finish_state()
	log.clear()
	var r := Flow.step(s, rng, 0, services)
	check(r.error.is_empty() and s.substep == 1 and s.game_mode == 0x15 and s.mode_stage == 21, "Substep0 did not advance: " + r.error)
	check(log == [["finish_cd_reset"], ["finish_bank_reset"], ["audio_command", 5], ["audio_command", 0xd1], ["finish_bank_load", Flow.FINISH_BANK_ARGS], ["outer_draw", 2]], "Substep0 source order differs: " + str(log))
	check(s.opening.D_800B5BD4 == 0x0f and s.opening.screen_brightness == 0x80, "Substep0 globals differ")
	check(not _names().has("arrival_draw"), "Stage21 drew the prologue background (no dec_bg_draw in the source)")
	log.clear()
	s.mode_stage_timer = 7; s.mode_step_timer = 3; s.game_mode_timer = 9
	r = Flow.step(s, rng, 0, services)
	check(r.error.is_empty() and [s.game_mode, s.mode_stage, s.mode_step, s.substep, s.game_mode_timer, s.mode_stage_timer, s.mode_step_timer] == [0x34, 0xff, 0, 0, 0, 0, 0], "Substep1 mode switch differs")
	check(s.opening.D_800B3DB0 == 0 and s.opening.text_speed == 3 and s.rule_state.menu_mode == 1 and s.rule_state.voice_wait_frames == 0 and s.rule_state.message_title_visible == false, "Substep1 fields differ")
	check(log == [["outer_draw", 2]], "Substep1 must only reach the tail frame draw: " + str(log))
	check(rng.checkpoint() == before_rng, "Stage21 consumed RNG")
	# START during substep0: the tail sets stage 21 substep 0 again.
	var again := _finish_state()
	log.clear()
	r = Flow.step(again, rng, START, services)
	check(r.error.is_empty() and again.mode_stage == 21 and again.substep == 0 and _names().count("finish_bank_load") == 1, "START at substep0 did not re-arm stage21")
	log.clear(); Flow.step(again, rng, 0, services)
	check(_names().slice(0, 2) == ["finish_cd_reset", "finish_bank_reset"] and _names().has("finish_bank_load") and again.substep == 1, "Repeated substep0 did not reissue the reset and reads")
	# START on the switch frame: source soft-lock, refused and rolled back.
	var sw := _finish_state(1)
	var before: Dictionary = sw.projection()
	log.clear()
	r = Flow.step(sw, rng, START, services)
	check(r.error == Flow.FINISH_SWITCH_START_ERROR and sw.projection() == before and r.presentation.is_empty(), "START on the switch frame was not refused: " + r.error)
	# Dispatch: mode_step / substep outside the source.
	for bad: Array in [[1, 0], [0, 2], [0, -1], [0, 255]]:
		var b := _finish_state(bad[1]); b.mode_step = bad[0]
		log.clear()
		r = Flow.step(b, rng, 0, services)
		check(r.error == Flow.FINISH_DISPATCH_ERROR and log.is_empty(), "Stage21 accepted step/substep %s" % str(bad))
	# A refused service rolls back the whole frame (no partial globals).
	for refused: String in ["finish_cd_reset", "finish_bank_reset", "audio_command", "finish_bank_load", "outer_draw"]:
		var f := _finish_state()
		var snap: Dictionary = f.projection()
		fail_on = refused
		r = Flow.step(f, rng, 0, services)
		fail_on = ""
		check(r.error == "double refused " + refused and f.projection() == snap and r.audio.is_empty() and r.presentation.is_empty(), "Refused %s left partial stage21 state" % refused)
	for missing: String in Flow.FINISH_SERVICES:
		var partial := services.duplicate(); partial.erase(missing)
		var m := _finish_state()
		r = Flow.step(m, rng, 0, partial)
		check(r.error == "prologue finish needs service: " + missing and m.substep == 0, "Stage21 ran without " + missing)
	check(rng.checkpoint() == before_rng, "Stage21 checks consumed RNG")
	# Audio commands through the actual rules. The normal path sends D1 in
	# stage20 and again here; with the stage20 request still set the rules
	# refuse the restart (the source keeps rate/accumulator unchanged).
	var a := _finish_state()
	a.opening["prologue_bgm_state"] = {"playing": true, "gain": 60}
	var audio := Session.FrameResult.new()
	AudioRules.audio_command(a, 5, audio); AudioRules.audio_command(a, 0xd1, audio)
	check(audio.error.is_empty() and a.opening.bgm_fade.request == true, "First D1 differs: " + audio.error)
	AudioRules.audio_command(a, 0xd1, audio)
	check(audio.error == "BGM fade restart while a source fade request is outstanding is unsupported", "Stage21 D1 over an outstanding stage20 fade was not refused: " + audio.error)
	completed.append("flow")

func _stage10_case8() -> State:
	var s := State.new()
	s.game_mode = 0x15; s.mode_stage = 10; s.mode_step = 0; s.substep = 8
	s.opening["actor96"] = {"profile":"o11_rei_lines15","index":96,"x":-160,"upload_batch_key":"a".sha256_text(),"upload_batch_key_2":"b".sha256_text()}
	s.opening["actor96_draw_flags"] = 0xa4
	s.opening["names_media"] = {"bg_show_1":1,"buffer_0":{"lba":0xacec},"buffer_1":{"lba":0xacf6},"active_draw":"dec_bg_draw"}
	s.opening["cut12_palette_adjusted"] = "cut12"
	s.opening["media"] = {"epoch":3,"next_id":2,"background_pending":{},"decoded":{},"hud_pending":"","actor_pending":{str(0x801a4000):"cut12",str(0x801f0000):"o12"},"requests":{
		"cut12":{"request_id":"cut12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.CUT_12_ID,"resource_sha256":Board.CUT_12_SHA,"payload":{"lba":0xab18,"sectors":49,"destination":0x801a4000}},
		"o12":{"request_id":"o12","epoch":3,"status":"ready","kind":"actors","resource_id":Board.O_12_ID,"resource_sha256":Board.O_12_SHA,"payload":{"lba":0xac3f,"sectors":17,"destination":0x801f0000}}}}
	return s

func _skipped_state(media: Media) -> State:
	# Case8 O12 setup, mode_stage_next, then the START skip's mode_stage_set(0x15).
	var s := _stage10_case8()
	var setup := Session.FrameResult.new()
	media.rei_lines15_effect(s, "next_scene", 0, setup)
	check(setup.error.is_empty(), "Exact case8 setup rejected: " + setup.error)
	s.next_stage()
	s.set_stage(21)
	return s

func _services(media: Media) -> Dictionary:
	var services := _doubles()
	services.merge(media.callbacks(), true)
	services["audio_command"] = _double("audio_command")
	services["outer_draw"] = _double("outer_draw")
	services["arrival_draw"] = _double("arrival_draw")
	return services

func _complete(media: Media, s: State, request: Dictionary) -> String:
	var event := Session.ServiceEvent.new(0, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, Queue.SERVICE,
		{"epoch":request.epoch,"request_id":request.request_id,"request_key":request.request_key,"status":"ready","resource_id":request.resource_id})
	var r := Session.FrameResult.new()
	media.complete(event, s, null, r)
	return r.error

func _pack_checks(board: Board, finish: FinishBank) -> void:
	var wrong := FinishBank.new()
	check(not wrong.open_pack(OS.get_environment("BOARD_PACK")).is_empty(), "Finish bank reader opened the board pack")
	check(finish.descriptions().size() == 5 and finish.identity() == FinishBank.TRACK_SHA, "Finish bank descriptions differ")
	var seq: Dictionary = finish.transfer(FinishBank.resource_id("sequence"))
	check(seq.error.is_empty() and seq.bytes.size() == 0x25 * 2048 and (seq.bytes.decode_u32(0) & 0xffff) == 0x40, "SPRING.SEQ read lacks the 0x40 package word audio_bank_start checks")
	check(seq.bytes.slice(18516, 20480) == PackedByteArray(Array(range(1964)).map(func(_i): return 0)), "SPRING.SEQ sector padding differs")
	var record: Dictionary = {"field":"sequence","argument":"f","lba":0xd37d,"sectors":0x25,"destination":0x8001c000,"size":75776,"sha256":FinishBank.READS[2][6],"path":"reads/sequence.bin","primary_file":"CDROM/VAB_SEQ/SPRING.SEQ","over_read_sectors":27,"segments":[
		{"path":"CDROM/VAB_SEQ/SPRING.SEQ","lba":0xd37d,"file_size":18516,"file_sha256":FinishBank.READS[2][7][0][3],"sectors_in_read":10,"read_offset":0,"sector_padding_zero":true},
		{"path":"CDROM/VAB_SEQ/SPRING.VB","lba":0xd387,"file_size":158992,"file_sha256":FinishBank.READS[2][7][1][3],"sectors_in_read":27,"read_offset":20480,"sector_padding_zero":true}]}
	check(FinishBank._check_record(record, FinishBank.READS[2]), "Exact SPRING.SEQ record identity rejected")
	for change: Array in [["over_read_sectors", 0], ["destination", 0x8002e800], ["sectors", 10]]:
		var bad := record.duplicate(true); bad[change[0]] = change[1]
		check(not FinishBank._check_record(bad, FinishBank.READS[2]), "SPRING.SEQ record accepted " + change[0])
	var trimmed := record.duplicate(true); trimmed.segments.pop_back()
	check(not FinishBank._check_record(trimmed, FinishBank.READS[2]), "SPRING.SEQ record accepted a file-only extent")
	completed.append("pack")

func _media_checks(board: Board, finish: FinishBank) -> void:
	var media := Media.new()
	media._board_store = board
	media._finish_bank = finish
	check(media.queue.prepare({}, board.actor_descriptions(), {}, {}, finish.descriptions()).is_empty(), "Queue rejected the stage21 bank descriptions")
	var services := _services(media)
	var rng := _rng()
	var before_rng: Dictionary = rng.checkpoint()
	var s := _skipped_state(media)
	# The Shiori bank the stage11 poll left ready (opening_audio_rules record).
	s.opening["audio_bank"] = {"epoch":7, "role":"prologue_shiori", "ready":true}
	var actor_before: Dictionary = s.opening.actor96.duplicate(true)
	var flags_before: Variant = s.opening.get("actor96_draw_flags")
	check(actor_before.profile == "o12_rei_line17" and actor_before.tpage_slot == 5, "Skip state lacks the retained O12 actor")
	log.clear()
	var r := Flow.step(s, rng, 0, services)
	check(r.error.is_empty() and s.substep == 1, "Stage21 substep0 rejected: " + r.error)
	var media_record: Dictionary = s.opening.media
	check(media_record.epoch == 4 and media_record.bank_pending.size() == 5 and media_record.requests.size() == 5 and media_record.actor_pending.is_empty(), "cd_command_reset/bank requests differ: " + str(media_record.keys()))
	var reads: Array = media_record.bank_pending.map(func(id): return media_record.requests[id])
	var expected := [[0xbf98, 3, 0x80010000, "se_header"], [0xbf79, 0x20, 0x801b8000, "se_body"], [0xd37d, 0x25, 0x8001c000, "sequence"], [0xd387, 0x50, 0x801c8000, "music_body"], [0xd3d5, 6, 0x80012000, "music_header"]]
	for i in range(5):
		var q: Dictionary = reads[i]
		check(q.kind == "bank" and q.payload.lba == expected[i][0] and q.payload.sectors == expected[i][1] and q.payload.destination == expected[i][2] and q.resource_id == FinishBank.resource_id(expected[i][3]) and q.resource_sha256 == FinishBank.READS[i][6], "Bank read %d differs from source order" % i)
		check(media._intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":q}).is_empty(), "Bank read %d failed the publication schema" % i)
	var kinds: Array = r.presentation.map(func(p): return p.kind)
	check(kinds == ["opening_cd_command_reset", "opening_media_request", "opening_media_request", "opening_media_request", "opening_media_request", "opening_media_request"], "Stage21 presentation order differs: " + str(kinds))
	check(media._intent_schema(r.presentation[0]).is_empty(), "cd_command_reset intent failed its schema")
	var loader: Dictionary = s.opening.audio_bank_loader
	var native := BankLoader.new()
	check(native.restore(loader).is_empty(), "Stage21 bank record is not a NativeBankLoader checkpoint")
	check(loader.se_header_0 == 0xbf98 and loader.music_header_0 == 0xd3d5 and loader.se_header_1 == 0 and loader.music_header_1 == 0 and loader.skip_se == 0 and loader.skip_music == 0 and loader.load_requested == 1 and loader.bank == 0 and loader.cd_baseline == 0 and loader.bgm_set == 0xff and loader.transfer_polls == 0 and loader.load_flags == [0, 0], "audio_bank_load_start fields differ: " + str(loader))
	# audio_bank_state_reset leaves no bank pending or ready: the earlier
	# role's record is invalidated and no BGM consumer sees it as ready.
	check(s.opening.audio_bank == {"epoch":8, "role":"", "ready":false}, "Stage21 kept the earlier role's bank record: " + str(s.opening.audio_bank))
	check(AudioRules.bank_poll(s) == 0, "Opening bank poll still reports the earlier bank ready")
	var bgm_start := Session.FrameResult.new()
	AudioRules.bank_start(s, 0, 0, bgm_start)
	check(bgm_start.error == "prologue BGM start requires a ready transactional sound bank", "BGM start accepted the earlier bank after stage21: " + bgm_start.error)
	check(s.opening.actor96 == actor_before and s.opening.get("actor96_draw_flags") == flags_before, "Stage21 changed the retained actor")
	# Wrong identities in a published request.
	var tampered: Dictionary = reads[2].duplicate(true); tampered.resource_sha256 = FinishBank.READS[3][6]
	check(media._intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":tampered}) == "first-arrival bank read extent differs from source", "Tampered bank sha accepted")
	tampered = reads[2].duplicate(true); tampered.payload.destination = 0x8002e800
	check(media._intent_schema({"kind":"opening_media_request","service":Queue.SERVICE,"request":tampered}) == "first-arrival bank read extent differs from source", "Tampered bank destination accepted")
	# Replay and ownership of the load.
	var replay := Session.FrameResult.new()
	s.substep = 0
	media.finish_bank_load(s, Flow.FINISH_BANK_ARGS, replay)
	check(replay.error == "stage21 bank load requires the same-frame cd_command_reset and audio_bank_state_reset" and media_record.bank_pending.size() == 5, "Bank load replayed without a reset")
	var alt := Session.FrameResult.new()
	media.finish_bank_load(s, [0, 0xbf98, 0xbf79, 0xcca6, 0xcca1, 0xcc9d], alt)
	check(alt.error == "stage21 bank load differs from prologue_finish_run case 0", "Alternate triple accepted at stage21")
	s.substep = 1
	var late := Session.FrameResult.new()
	media.finish_bank_reset(s, late)
	check(late.error == "audio_bank_state_reset is outside prologue_finish_run case 0", "Bank reset accepted substep1")
	var direct := Session.FrameResult.new()
	media.queue.bank_read(s, 3, 0x80010000, 0xbf98, direct)
	check(direct.error == "opening bank read is outside prologue_finish_run case 0", "Bank read accepted outside case0")
	var unknown := Session.FrameResult.new()
	media.queue.bank_read(s, 3, 0x80016000, 0xbf98, unknown)
	check(unknown.error == "source-backed opening bank read is unavailable for LBA bf98", "Bank read accepted another destination")
	var bare := Media.new()
	bare.queue.prepare({}, {}, {}, {}, {})
	var unprepared := Session.FrameResult.new()
	var fresh := _finish_state()
	bare.finish_bank_load(fresh, Flow.FINISH_BANK_ARGS, unprepared)
	check(unprepared.error == Media.FINISH_BANK_UNPREPARED, "Unprepared bank reads were not refused")
	# Substep1: the mode switch; START on that frame is refused.
	var switch_before: Dictionary = s.projection()
	r = Flow.step(s, rng, START, services)
	check(r.error == Flow.FINISH_SWITCH_START_ERROR and s.projection() == switch_before, "Switch-frame START accepted")
	r = Flow.step(s, rng, 0, services)
	check(r.error.is_empty() and [s.game_mode, s.mode_stage, s.mode_step, s.substep] == [0x34, 0xff, 0, 0] and s.opening.actor96 == actor_before, "Switch frame differs: " + r.error)
	check(rng.checkpoint() == before_rng, "Stage21 frames consumed RNG")
	# Stage 0xFF step 1 substep 0 (settings_command_screen_reload_calendar).
	# Step 0 (setup_reset) has no production owner yet; this test sets step 1
	# the way its mode_step_next would, to reach the poll.
	var step0 := Session.FrameResult.new()
	media.finish_bank_poll(s, step0)
	check(step0.error == "stage21 bank poll is outside the command reload calendar step", "Poll accepted mode_step 0: " + step0.error)
	s.set_step(1)
	var early := Session.FrameResult.new()
	var p0 := media.finish_bank_poll(s, early)
	check(p0 == -1 and early.error.is_empty(), "Poll before any read completed differs")
	# Ownership: the poll needs the NativeBankLoader record of this load.
	var owned: Dictionary = s.opening.audio_bank_loader.duplicate(true)
	for tamper: Array in [["load_requested", 0], ["bank", 1], ["cd_baseline", null], ["transfer_polls", 1]]:
		var record := owned.duplicate(true); record[tamper[0]] = tamper[1]
		s.opening.audio_bank_loader = record
		var bad := Session.FrameResult.new()
		media.finish_bank_poll(s, bad)
		var expected_error: String = Media.FINISH_BANK_TRANSFER_UNSUPPORTED if tamper[0] == "transfer_polls" else "stage21 bank poll has no owned audio_bank_load_start"
		check(bad.error == expected_error, "Poll accepted a loader record with %s = %s: %s" % [tamper[0], str(tamper[1]), bad.error])
	s.opening.erase("audio_bank_loader")
	var missing := Session.FrameResult.new()
	media.finish_bank_poll(s, missing)
	check(missing.error == "stage21 bank poll has no owned audio_bank_load_start", "Poll accepted a missing loader record")
	s.opening.audio_bank_loader = owned
	# Completion order: a later read alone does not count (CD queue order).
	check(_complete(media, s, reads[1]).is_empty() and media.queue.bank_reads_done(s) == 0, "Out-of-order completion counted")
	check(_complete(media, s, reads[0]).is_empty() and media.queue.bank_reads_done(s) == 2, "In-order completions not counted")
	check(_complete(media, s, reads[0]) == "opening media completion is stale, duplicate or mismatched", "Duplicate completion accepted")
	var stale: Dictionary = reads[2].duplicate(true); stale.epoch = 3
	check(not _complete(media, s, stale).is_empty(), "Stale-epoch completion accepted")
	check(_complete(media, s, reads[2]).is_empty() and _complete(media, s, reads[3]).is_empty(), "Completions 3/4 rejected")
	var four := Session.FrameResult.new()
	check(media.finish_bank_poll(s, four) == -1 and four.error.is_empty(), "Poll with four reads did not wait")
	# Through Reload.step: the waiting poll leaves substep 0 and draws.
	var reload_services: Dictionary = {}
	for key: String in Reload.REQUIRED: reload_services[key] = _double(key)
	reload_services["bank_poll"] = media.finish_bank_poll
	log.clear()
	var waiting := Reload.step(s, reload_services)
	check(waiting.error.is_empty() and s.substep == 0 and not _names().has("bank_start"), "Reload advanced on a -1 poll")
	check(_complete(media, s, reads[4]).is_empty() and media.queue.bank_reads_done(s) == 5, "Fifth completion rejected")
	# A completed read from another CD epoch does not count (cd_command_reset).
	var first_read: Dictionary = s.opening.media.requests[s.opening.media.bank_pending[0]]
	first_read.epoch -= 1
	check(media.queue.bank_reads_done(s) == 0, "A bank read from another media epoch was counted")
	first_read.epoch += 1
	var before_poll: Dictionary = s.projection()
	var refused := Reload.step(s, reload_services)
	check(refused.error == Media.FINISH_BANK_TRANSFER_UNSUPPORTED and s.projection() == before_poll, "Bank transfer was not refused at audio_command 0x11: " + refused.error)
	var boot: Variant = load("res://app/first_arrival_boot.gd").new()
	boot.media = media
	var delegated := Session.FrameResult.new()
	check(boot._bank_poll(s, delegated) == -1 and delegated.error == Media.FINISH_BANK_TRANSFER_UNSUPPORTED, "Boot poll did not delegate mode 0x34 to the stage21 reads: " + delegated.error)
	boot.free()
	s.substep = 1
	var wrong := Session.FrameResult.new()
	media.finish_bank_poll(s, wrong)
	check(wrong.error == "stage21 bank poll is outside the command reload calendar step", "Poll accepted substep1")
	completed.append("media")

## The production audio_command (first_arrival_boot._services) on the START
## skip state, where no prologue BGM sequence is playing. prologue.c:2721
## issues 0xD1 unconditionally; the rules refuse it, and the frame rolls back.
func _skip_audio_checks(board: Board, finish: FinishBank) -> void:
	var media := Media.new()
	media._board_store = board
	media._finish_bank = finish
	media.queue.prepare({}, board.actor_descriptions(), {}, {}, finish.descriptions())
	var services := _services(media)
	services["audio_command"] = AudioRules.audio_command
	var s := _skipped_state(media)
	var before: Dictionary = s.projection()
	var r := Flow.step(s, _rng(), 0, services)
	check(r.error == "BGM fade requires the actual playing sequence gain" and s.projection() == before and r.presentation.is_empty(), "START-skip stage21 D1 outcome differs: " + r.error)
	# After stage20's D1 fade has run out (audio_bgm_fade_finish), the rules
	# still hold the request and refuse the stage21 D1 as a restart.
	var faded := _finish_state()
	faded.opening["prologue_bgm_state"] = {"playing": true, "gain": 60}
	var fade := Session.FrameResult.new()
	AudioRules.audio_command(faded, 0xd1, fade)
	for frame in range(4000):
		if faded.opening.bgm_fade.active != true: break
		faded.frame_count += 1
		AudioRules.bgm_fade_tick(faded, fade)
	check(fade.error.is_empty() and faded.opening.bgm_fade.active == false and faded.opening.prologue_bgm_state.playing == false, "Stage20 D1 fade did not finish")
	AudioRules.audio_command(faded, 0xd1, fade)
	check(fade.error == "BGM fade restart while a source fade request is outstanding is unsupported", "Stage21 D1 after a finished fade differs: " + fade.error)
	completed.append("skip_audio")

func _merge(inner: Session.FrameResult, outer: Session.FrameResult) -> void:
	outer.error = inner.error
	outer.audio.append_array(inner.audio)
	outer.presentation.append_array(inner.presentation)
	outer.transitions.append_array(inner.transitions)

## Stage21 frames committed on a real Session: preflight approves them and
## publish turns the five bank reads into buffers and pending completions.
func _publish_checks(board: Board, finish: FinishBank) -> void:
	var store := Store.new()
	var opened := store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"), OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA"))
	var media := Media.new()
	if opened.is_empty(): opened = media.prepare(store, board, finish)
	check(opened.is_empty(), "Media bridge did not prepare with the stage21 bank reader: " + opened)
	if not opened.is_empty(): return
	var session := Session.new()
	var s := _skipped_state(media)
	session.state = s
	session.rng.configure(Rng.OPENBIOS_PROFILE, 0x24681357)
	check(media.bind_session(session).is_empty(), "Media bridge did not bind the stage21 Session")
	var services := _services(media)
	var pressed := [0]
	check(session.router.bind_mode(0x15, func(base, _input, rng, result) -> void: _merge(Flow.step(base, rng, pressed[0], services), result)).is_empty(), "Mode 0x15 did not bind")
	session.service_completion = media.complete
	session.draw_logic = func(_state, _rng, _result) -> void: pass
	session.cd_logic = func(current, _rng, result) -> void: result.error = media.preflight(result, current)
	session.audio_logic = func(_state, _rng, _result) -> void: pass
	var destinations: Array = FinishBank.READS.map(func(row): return str(row[4]))
	var shas: Array = FinishBank.READS.map(func(row): return row[6])
	# Frame 0: substep0 with START held; the tail re-arms stage21 substep0.
	pressed[0] = START
	var r := session.step(InputFrame.new(session.clock.logic_iterations), [])
	check(r.committed and r.error.is_empty() and s.mode_stage == 21 and s.substep == 0, "Re-armed stage21 frame did not commit: " + r.error)
	check(media.publish(r, 0).is_empty(), "Stage21 bank reads did not publish")
	var diag := media.diagnostics()
	check(diag.buffers.keys() == destinations and diag.buffers.values().map(func(b): return b.sha256) == shas, "Published bank buffers differ from FinishBank.READS: " + str(diag.buffers))
	check(diag.pending_observations == 5, "First stage21 frame did not queue five completions")
	var first_epoch: int = s.opening.media.epoch
	# Frame 1: substep0 again. cd_command_reset forgets frame 0's completions.
	pressed[0] = 0
	r = session.step(InputFrame.new(session.clock.logic_iterations), [])
	check(r.committed and r.error.is_empty() and s.substep == 1 and s.opening.media.epoch == first_epoch + 1, "Second stage21 substep0 did not commit: " + r.error)
	check(media.publish(r, 1).is_empty(), "Second stage21 bank reads did not publish")
	diag = media.diagnostics()
	var events: Array[Session.ServiceEvent] = media.pending_events(session.clock.logic_iterations)
	check(diag.pending_observations == 5 and events.all(func(e): return e.payload.epoch == first_epoch + 1), "cd_command_reset kept completions from before the reset: " + str(diag.pending_observations))
	check(events.map(func(e): return e.payload.request_id) == s.opening.media.bank_pending, "Pending completions differ from the reissued reads")
	check(diag.buffers.keys() == destinations and diag.buffers.values().map(func(b): return b.sha256) == shas, "Reissued bank buffers differ from FinishBank.READS")
	# Frame 2: the switch frame consumes the five completions in CD order.
	r = session.step(InputFrame.new(session.clock.logic_iterations), events)
	check(r.committed and r.error.is_empty() and [s.game_mode, s.mode_stage, s.mode_step, s.substep] == [0x34, 0xff, 0, 0] and media.queue.bank_reads_done(s) == 5, "Switch frame did not consume the bank completions: " + r.error)
	check(media.publish(r, 2).is_empty() and media.diagnostics().pending_observations == 0, "Consumed bank completions were not published")
	# opening_cd_command_reset is owned only by mode 0x15 stage 21 step 0.
	var foreign := Session.FrameResult.new()
	foreign.presentation.append({"kind":"opening_cd_command_reset", "service":Queue.SERVICE, "epoch":s.opening.media.epoch})
	check(media.preflight(foreign, s) == "first-arrival cd_command_reset differs from prologue_finish_run ownership", "cd_command_reset accepted mode 0x34")
	var mode := [s.game_mode, s.mode_stage, s.mode_step]
	for at: Array in [[0x15, 20, 0], [0x15, 21, 1]]:
		s.game_mode = at[0]; s.mode_stage = at[1]; s.mode_step = at[2]
		check(media.preflight(foreign, s) == "first-arrival cd_command_reset differs from prologue_finish_run ownership", "cd_command_reset accepted %s" % str(at))
	s.game_mode = mode[0]; s.mode_stage = mode[1]; s.mode_step = mode[2]
	completed.append("publish")

class _Opening extends RefCounted:
	var command_ready := true
	var state: Object
	var handoff: Dictionary = {}
	var taken := 0
	func take_command_handoff() -> Dictionary:
		taken += 1
		return handoff

class _Controller extends RefCounted:
	var failure: Variant = ""
	var attached: Array = []
	func attach(session: Variant, state: Variant) -> Variant:
		attached.append([session, state])
		return failure

func _hud_at_command_select() -> HudSession:
	var hud := HudSession.new()
	var positions: Array = range(14).map(func(i): return [-120 + i * 20, 80])
	hud.game_state_init(true)
	hud.command_screen_build(positions, 0, true)
	hud.enter_command_select(0x34, 1, 0)
	return hud

func _handoff_checks() -> void:
	check(Handoff.connect_gameplay(RefCounted.new(), null, null).error == Handoff.NOT_READY, "Handoff accepted an opening without command_ready")
	check(Handoff.connect_gameplay(null, null, HudSession.new()).error == Handoff.NOT_READY, "Handoff accepted no opening")
	var s := State.new()
	s.game_mode = 0x34; s.mode_stage = 1; s.mode_step = 0
	s.opening["command_ready"] = true
	var session := Session.new()
	var opening := _Opening.new()
	opening.state = s
	opening.handoff = {"error":"", "session":session, "state":s, "rng":session.rng}
	var hud := _hud_at_command_select()
	check(hud.at_command_select(), "Constructed HUD session is not at command select")
	var controller := _Controller.new()
	# Not at 0x34/1/0, or the state does not record command_ready.
	s.mode_stage = 0xff
	check(Handoff.connect_gameplay(opening, controller, hud).error == Handoff.NOT_READY, "Handoff accepted stage 0xFF")
	s.mode_stage = 1
	s.opening.command_ready = false
	check(Handoff.connect_gameplay(opening, controller, hud).error == Handoff.NOT_READY, "Handoff accepted a state without command_ready")
	s.opening.command_ready = true
	opening.command_ready = false
	check(Handoff.connect_gameplay(opening, controller, hud).error == Handoff.NOT_READY, "Handoff accepted an opening without command_ready")
	opening.command_ready = true
	# HUD: none, another type, or a session that never reached command select.
	var unbuilt := HudSession.new(); unbuilt.game_state_init(true)
	for bad_hud: Variant in [null, "hud", unbuilt]:
		check(Handoff.connect_gameplay(opening, controller, bad_hud).error == Handoff.HUD_REQUIRED, "Handoff accepted HUD %s" % str(bad_hud))
	# Controller: none, or one without attach.
	check(Handoff.connect_gameplay(opening, null, hud).error == Handoff.CONTROLLER_REQUIRED, "Handoff accepted no controller")
	check(Handoff.connect_gameplay(opening, RefCounted.new(), hud).error == Handoff.CONTROLLER_REQUIRED, "Handoff accepted a controller without attach")
	check(opening.taken == 0 and controller.attached.is_empty(), "A refused handoff consumed the opening")
	# The opening refuses its own handoff.
	opening.handoff = {"error":"opening handoff already taken"}
	check(Handoff.connect_gameplay(opening, controller, hud).error == "opening handoff already taken" and controller.attached.is_empty(), "Handoff ignored the opening's refusal")
	opening.handoff = {"error":"", "session":session, "state":s, "rng":session.rng}
	# attach fails, with a message or a non-string result.
	controller.failure = "school day controller refused"
	check(Handoff.connect_gameplay(opening, controller, hud).error == "school day controller refused", "Handoff ignored a failed attach")
	controller.failure = 0
	check(Handoff.connect_gameplay(opening, controller, hud).error == Handoff.CONTROLLER_REQUIRED, "Handoff accepted a non-string attach result")
	controller.failure = ""
	controller.attached.clear()
	var done := Handoff.connect_gameplay(opening, controller, hud)
	check(done.error.is_empty() and done.session == session and done.state == s and done.rng == session.rng and done.hud == hud and controller.attached == [[session, s]], "Handoff did not attach the same session and state")
	completed.append("handoff")

## native-bank0: with a native owner (media.bank0_owner) the stage21 reads are
## also published to the engine, and the reload poll follows the measured
## landing (prologue_bank0_audio.gd): -1 until +131 even with all five queue
## completions, then 0x11, then 1 with 0x22/0x21/0x2F two frames later.
func _bank0_media_checks(board: Board, finish: FinishBank) -> void:
	var media := Media.new()
	media._board_store = board
	media._finish_bank = finish
	check(media.queue.prepare({}, board.actor_descriptions(), {}, {}, finish.descriptions()).is_empty(), "bank0 queue rejected the stage21 descriptions")
	var owner_error := [""]
	media.bank0_owner = func(_s) -> String: return owner_error[0]
	# Stand-in for the BGM owner's native slot-1 answer (bank0_slot_state).
	var slot_answer: Array = [{"error":"","ready":false}]
	media.bank0_slot_state = func(_s) -> Dictionary: return slot_answer[0]
	var services := _services(media)
	var rng := _rng()
	var s := _skipped_state(media)
	s.opening["audio_bank"] = {"epoch":7, "role":"prologue_shiori", "ready":true}
	s.opening["bank0_audio"] = {"phase":"stage16","bgm_set":0,"sequence":1,"playing":1,"start_frame":0}
	s.opening["prologue_bgm_state"] = {"playing":true,"role":"bank0_pro_vol","bank":0,"sequence":1,"gain":93}
	s.opening["bgm_fade"] = {"active":true,"gain":93,"ticks":0,"command":0xd1,"rate":0x80,"decrement":1,"accumulator":0,"request":true,"started_frame":0}
	s.frame_count = 9000
	var r := Flow.step(s, rng, 0, services)
	var record: Dictionary = s.opening.bank0_audio
	check(r.error.is_empty() and r.audio == [{"kind":"native_bank0_load"}] and record.phase == "loading" and record.load_frame == 9000, "Stage21 did not hand the reads to the native owner: %s %s" % [r.error, str(r.audio)])
	r = Flow.step(s, rng, 0, services)
	check(r.error.is_empty() and [s.game_mode, s.mode_stage, s.mode_step, s.substep] == [0x34, 0xff, 0, 0], "bank0 switch frame differs: " + r.error)
	s.set_step(1)
	var reads: Array = s.opening.media.bank_pending.map(func(id): return s.opening.media.requests[id])
	for q: Dictionary in reads: check(_complete(media, s, q).is_empty(), "bank0 read completion rejected")
	check(media.queue.bank_reads_done(s) == 5, "bank0 queue did not complete the five reads")
	# The port queue completes all reads at once; the measured drive does not.
	for k: int in [1, 15, 69, 115, 130]:
		s.frame_count = 9000 + k
		var wait := Session.FrameResult.new()
		check(media.finish_bank_poll(s, wait) == -1 and wait.error.is_empty() and wait.audio.is_empty() and s.opening.audio_bank_loader.transfer_polls == 0, "Poll at +%d passed the measured landing: %s" % [k, wait.audio])
	s.frame_count = 9131
	owner_error[0] = "the qualified native library has no bank0 (COM_MIX/SPRING) transfer support"
	var refused := Session.FrameResult.new()
	check(media.finish_bank_poll(s, refused) == -1 and refused.error == owner_error[0] and refused.audio.is_empty(), "0x11 without a native owner accepted: " + refused.error)
	s.opening.audio_bank_loader.transfer_polls = 0
	owner_error[0] = ""
	var transfer := Session.FrameResult.new()
	check(media.finish_bank_poll(s, transfer) == -1 and transfer.error.is_empty() and transfer.audio == [{"kind":"native_audio_command","command":0x11}], "0x11 differs at +131: %s %s" % [transfer.error, transfer.audio])
	check(record.phase == "transferred" and record.transfer_frame == 9131 and s.opening.audio_bank_loader.transfer_polls == 1, "0x11 record differs: " + str(record))
	check(s.opening.prologue_bgm_state.playing == false and s.opening.bgm_fade.active == false and not s.opening.bgm_fade.has("request"), "0x11 did not end the D1 fade on the BGM slot")
	s.frame_count = 9132
	var slot0 := Session.FrameResult.new()
	check(media.finish_bank_poll(s, slot0) == -1 and slot0.error.is_empty() and slot0.audio.is_empty() and not record.has("ready_frame"), "Poll before the slot1 transfer returned ready")
	# The engine's slot-1 state must agree with the measured +2 frame, both ways.
	var polls: int = s.opening.audio_bank_loader.transfer_polls
	slot_answer[0] = {"error":"","ready":true}
	var early := Session.FrameResult.new()
	check(media.finish_bank_poll(s, early) == -1 and early.error == "native slot1 readiness differs from the measured ready frame" and s.opening.audio_bank_loader.transfer_polls == polls, "native slot1 ready at +1 accepted: " + early.error)
	slot_answer[0] = {"error":"native slot1 transfer (ready true, requests 0) at +1 differs from the measured ready frame +2"}
	var owner_refused := Session.FrameResult.new()
	check(media.finish_bank_poll(s, owner_refused) == -1 and owner_refused.error == slot_answer[0].error, "owner slot refusal not propagated: " + owner_refused.error)
	var answering := media.bank0_slot_state
	media.bank0_slot_state = Callable()
	var no_slot := Session.FrameResult.new()
	check(media.finish_bank_poll(s, no_slot) == -1 and no_slot.error == "bank0 slot poll has no native transfer owner", "slot poll without a native answer accepted: " + no_slot.error)
	media.bank0_slot_state = answering
	s.frame_count = 9133
	slot_answer[0] = {"error":"","ready":false}
	var late := Session.FrameResult.new()
	check(media.finish_bank_poll(s, late) == -1 and late.error == "native slot1 readiness differs from the measured ready frame" and not record.has("ready_frame"), "native slot1 pending at +2 accepted: " + late.error)
	slot_answer[0] = {"error":"","ready":true}
	# +133 through Reload.step: poll 1, then audio_bank_start(0,0) in the same frame.
	s.frame_count = 9133
	var reload_services: Dictionary = {}
	for key: String in Reload.REQUIRED: reload_services[key] = _double(key)
	reload_services["bank_poll"] = media.finish_bank_poll
	log.clear()
	var ready := Reload.step(s, reload_services)
	check(ready.error.is_empty() and s.substep == 1 and _names().has("bank_start") and record.ready_frame == 9133, "Reload did not start bank0 on the ready poll: " + ready.error)
	check(s.opening.audio_bank_loader.load_flags[0] == 0 and s.opening.audio_bank_loader.transfer_polls == 3, "ready poll loader record differs: " + str(s.opening.audio_bank_loader))
	completed.append("bank0")

func _initialize() -> void:
	_flow_checks()
	_handoff_checks()
	var board := Board.new()
	var error := board.open_pack(OS.get_environment("BOARD_PACK"), Board.MANIFEST_SHA)
	var finish := FinishBank.new()
	if error.is_empty(): error = finish.open_pack(OS.get_environment("FINISH_BANK_PACK"))
	if not error.is_empty():
		printerr(error); failures += 1
	else:
		_pack_checks(board, finish)
		_media_checks(board, finish)
		_skip_audio_checks(board, finish)
		_publish_checks(board, finish)
		_bank0_media_checks(board, finish)
	# A script error ends a section early without failing a check.
	check(completed == ["flow", "handoff", "pack", "media", "skip_audio", "publish", "bank0"], "Check sections did not all run to the end: " + str(completed))
	if failures == 0: print("PASS: %d original stage21 handoff checks" % checks)
	quit(1 if failures else 0)
