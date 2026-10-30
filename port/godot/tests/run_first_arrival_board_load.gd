extends SceneTree
## T-29025 Step 2: Authentic prologue_board_load (stage 1 / step 0 / substep 0..1)
## verification against real prepared media, board, and HUD resource packs.
const Media = preload("res://services/first_arrival_media_bridge.gd")
const Upload = preload("res://services/first_arrival_upload_bridge.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const Cold = preload("res://flow/cold_no_card_scratch.gd")
const Pump = preload("res://services/prologue_native_gpu_pump.gd")
const ActorRenderer = preload("res://presentation/first_arrival_actor_renderer.gd")
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const Resources = preload("res://services/first_arrival_launch_resources.gd")
const ScenePresenter = preload("res://presentation/first_arrival_scene_presenter.gd")

var checks := 0
var failures := 0
var media := Media.new()
var upload := Upload.new()
var actor_renderer := ActorRenderer.new()
var hud := Hud.new()
var session := Session.new()
var state := State.new()
var owned: Dictionary
var reject_audio := false
var services: Dictionary = {}

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func tick(include_pending: bool = true) -> Session.FrameResult:
	var events: Array[Session.ServiceEvent] = []
	if include_pending:
		events = media.pending_events(session.clock.logic_iterations)
	return session.step(InputFrame.new(session.clock.logic_iterations),events)

func runtime_hash() -> String:
	return Upload._runtime_hash(owned)

func _hud_projection(current: State) -> Dictionary:
	var opening_copy: Dictionary = current.opening.duplicate(false)
	if opening_copy.get("prologue_hud_live") is Dictionary:
		opening_copy["prologue_hud_live"] = opening_copy.prologue_hud_live.duplicate(true)
	return {"game_mode":current.game_mode,"mode_stage":current.mode_stage,"mode_step":current.mode_step,"substep":current.substep,
		"frame_count":current.frame_count,"player_flags":PackedByteArray([0,0]),"pad_held_triangle":false,
		"opening":opening_copy}

func _initialize() -> void:
	var media_store := Store.new()
	var board_store := BoardStore.new()
	check(media_store.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")).is_empty(),"authentic media store opens")
	check(board_store.open_pack(OS.get_environment("BOARD_PACK"),BoardStore.MANIFEST_SHA).is_empty(),"authentic board store opens")
	check(hud.open_installed(OS.get_environment("PROLOGUE_HUD_ROOT"),"packs/prologue_hud/manifest.json",Hud.INSTALLED_PROFILE_SHA256).is_empty(),"authentic HUD opens")
	if failures != 0:
		quit(1)
		return
	check(media.prepare(media_store,board_store).is_empty(),"media bridge prepares with board store")
	var route := {"source_flow_sha256":Cold.FLOW_SHA,"cold_boot":true,"direct_new_game":true,"options_visited":false,"retained_game":false,"memory_card_load":false}
	var initial := Cold.initialize(route)
	check(initial.error.is_empty(),"cold route initializes scratch")
	var vram := PackedByteArray(); vram.resize(Pump.VRAM_BYTES); vram.fill(0xa5)
	owned = {"scratch_owner":"board-load-test-scratch","vram_owner":"board-load-test-vram",
		"scratch_words":initial.scratch_words,"vram_words":vram,"pending_gpu_effects":[]}
	check(upload.prepare(media,media_store,owned,board_store).is_empty(),"upload bridge prepares with board store")
	check(actor_renderer.prepare(media_store,upload).is_empty(),"actor renderer prepares with petal frame tables")
	state.game_mode = 0x15; state.mode_stage = 0; state.mode_step = 1; state.substep = 1
	state.sound_active = true
	state.opening["screen_brightness"] = 128
	state.opening["prologue_line"] = 0
	state.opening["prologue_text_work"] = {"line_id":0,"pos":1,"total":1}
	state.opening["prologue_hud_live"] = {"year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"title_timer":184,
		"checker_scroll_u":31,"checker_scroll_v":255,"checker_scroll_owner":"board-load-test",
		"screen_brightness":128,"checker_fade":255,"title_visible":false,"date_visible":true,"message_visible":true,
		"checker_scroll_phase":"before_outer_scroll_update"}
	session.state = state
	session.rng.configure(Rng.OPENBIOS_PROFILE,0x13572468)
	check(upload.bind_session(session).is_empty(),"upload and media bind Session")
	services = media.callbacks()
	services["arrival_draw"] = func(s: State, slot: int, out: Session.FrameResult) -> void:
		media.arrival_draw(s,slot,out,actor_renderer)
	services["outer_draw"] = func(s: State, arg: int, out: Session.FrameResult) -> void:
		var proj := _hud_projection(s)
		out.error = hud.advance_outer(proj,session.rng,{},arg)
		if out.error.is_empty(): s.opening.prologue_hud_live = proj.opening.prologue_hud_live
	session.service_completion = media.complete
	session.draw_logic = func(current: State, rng: Rng, result: Session.FrameResult) -> void:
		if current.game_mode == 0x15 and current.mode_stage != 0 and hud.source_frame_argument(_hud_projection(current)) != 2:
			result.error = "only the original first-arrival route is prepared (stage=%d step=%d substep=%d)" % [current.mode_stage, current.mode_step, current.substep]
			return
		actor_renderer.logical_draw(current, rng, result)
	session.cd_logic = func(current: State, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = upload.preflight(result,current)
	session.audio_logic = func(_state: State, _rng: Rng, result: Session.FrameResult) -> void:
		if reject_audio: result.error = "injected audio failure after upload preflight"
	# Stage 0 media + actor upload fixture to establish authentic owned VRAM before stage 1
	session.router.bind_mode(0x15,func(current: State, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
		match current.opening.get("setup_phase",0):
			0:
				media.scene_reset(current,result)
				current.opening["screen_brightness"] = 128
				media.queue.background_read(current,0x494b,1,result)
				current.opening.setup_phase = 1
			1:
				if media.queue.cd_sync(current,result) == 1:
					media.queue.background_decode(current,1,result)
					current.substep = 3
					media.queue.actor_read(current,13,0x80180000,0xabab,result)
					current.opening.setup_phase = 2
			2:
				if media.queue.cd_sync(current,result) == 1:
					current.substep = 4
					media.actor_palette_setup(current,result)
					current.opening.setup_phase = 3
			3:
				current.mode_stage = 0; current.mode_step = 2; current.substep = 3; current.mode_step_timer = 61
				current.opening["prologue_line"] = 0
				current.opening["prologue_text_work"] = {"line_id":0,"pos":1,"total":1}
				Arrival._stage_00_arrival(current,session.rng,services,result)
				current.opening.setup_phase = 4
	)
	for step_idx in range(4):
		var r := tick()
		check(r.committed and media.publish(r,step_idx).is_empty() and upload.publish(r,step_idx).is_empty(),"stage0 setup tick %d commits and publishes" % step_idx)
	check(state.mode_stage == 1 and state.mode_step == 0 and state.substep == 0 and state.opening.actor96_draw_flags == 0xa4,"reached stage 1 step 0 substep 0 visible boundary")
	check(media.diagnostics().buffers[str(0x80162000)].sha256 == Store.BG_TRANSFER_SHA,"0x80162000 holds 10-sector arrival background before board read")
	# Now bind authentic Arrival._stage_01_board .. Arrival._stage_06_yoshio_greeting for stage 1..6 progression
	session.router.bind_mode(0x15,func(current: State, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
		if current.mode_stage == 1:
			Arrival._stage_01_board(current,rng,services,result)
		elif current.mode_stage == 2:
			Arrival._stage_02_names(current,rng,services,result)
		elif current.mode_stage == 3:
			Arrival._stage_03_line_03(current,services,result)
		elif current.mode_stage == 4:
			Arrival._stage_04_line_04(current,rng,services,result)
		elif current.mode_stage == 5:
			Arrival._stage_05_transition(current,services,result)
		elif current.mode_stage == 6:
			Arrival._stage_06_yoshio_greeting(current,services,result)
		elif current.mode_stage == 7:
			Arrival._stage_07_line_11(current,services,result)
		else:
			result.error = "unexpected stage in board/names/lines test"
	)
	var before_substep0_state := session.state_hash()
	var before_substep0_vram := runtime_hash()
	reject_audio = true
	var failed_substep0 := tick()
	check(not failed_substep0.committed and session.state_hash() == before_substep0_state and runtime_hash() == before_substep0_vram,"rejected substep 0 rolls back state and buffers")
	reject_audio = false
	var substep0 := tick()
	check(substep0.committed and state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1,"prologue_board_load substep 0 commits and advances to substep 1: "+substep0.error)
	check(state.opening.get("D_800B3DB0") == 1 and state.opening.prologue_hud_live.date_visible == true and state.opening.actor96_draw_flags == 0xa4,"substep 0 sets D_800B3DB0=1, keeps date visible, and retains actor96")
	check(media.publish(substep0,4).is_empty() and upload.publish(substep0,4).is_empty(),"substep 0 publishes 9-sector board read into 0x80162000")
	check(media.diagnostics().buffers[str(0x80162000)].size == 18432 and media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.SOURCE_SHA,"0x80162000 now holds exact 9-sector P_01_02.BS bitstream")
	# Verify substep 1 while cd_sync == 0 (before completion delivery) holds in substep 1 with date visible and actor96 active
	var hold_substep1 := tick(false)
	check(hold_substep1.committed and state.mode_stage == 1 and state.mode_step == 0 and state.substep == 1 and state.opening.prologue_hud_live.date_visible == true and state.opening.actor96_draw_flags == 0xa4,"substep 1 while cd_sync==0 holds in substep 1 with hizuke_show and actor96")
	check(media.publish(hold_substep1,5).is_empty() and upload.publish(hold_substep1,5).is_empty(),"substep 1 hold frame publishes cleanly while retaining pending CD observation")
	# Verify rollback on substep 1 completion/decode frame
	var before_substep1_state := session.state_hash()
	var before_substep1_rng := session.rng.checkpoint()
	var before_substep1_vram := runtime_hash()
	reject_audio = true
	var failed_substep1 := tick(true)
	check(not failed_substep1.committed and session.state_hash() == before_substep1_state and session.rng.checkpoint() == before_substep1_rng and runtime_hash() == before_substep1_vram and media.pending_events(6).size() == 1,"rejected substep 1 completion/decode rolls back state, 32 petal RNG draws, and VRAM")
	reject_audio = false
	var expected_rng := Rng.new()
	expected_rng.restore(before_substep1_rng)
	var substep1 := tick(true)
	check(substep1.committed and state.mode_stage == 1 and state.mode_step == 1 and state.substep == 0 and state.mode_step_timer == 0,"substep 1 completion advances via mode_step_next to stage 1 step 1 substep 0: "+substep1.error)
	check(not state.opening.has("actor96") and state.opening.get("actor96_draw_flags") == 0 and not state.opening.has("actor96_source_draw"),"actor_clear(0x60) clears actor96")
	check(state.opening.prologue_hud_live.date_visible == false,"hizuke_disp_switch(0) hides HUD date pillar")
	check(state.opening.get("prologue_petals") is Array and state.opening.prologue_petals.size() == 8 and session.rng.draw_count == expected_rng.draw_count + 32,"prologue_petals_init initializes 8 petals and consumes 32 RNG draws")
	check(state.opening.get("prologue_scene_counter") == 0,"prologue_scene_counter initialized to 0")
	check(runtime_hash() == before_substep1_vram,"preflight preview leaves live VRAM untouched before publish")
	check(media.publish(substep1,6).is_empty() and upload.publish(substep1,6).is_empty(),"substep 1 publishes board decode and move_255_line(5,14,15) to owned VRAM")
	var board_key: String = state.opening.board_media.upload_batch_key
	check(upload.batch_ready(state.opening.arrival_media.upload_batch_key).is_empty() and upload.batch_ready(board_key).is_empty() and upload.diagnostics().board_batch_complete,"both actor and board VRAM batches are ready and verified")
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(320,0,640,160))) == BoardStore.SOFTWARE_WORD_SHA and Pump._region(owned.vram_known_mask,Rect2i(320,0,640,160)).count(255) == 204800,"board decoded into VRAM (320,0,640,160) with full known mask")
	check(Pump._region(owned.vram_known_mask,Rect2i(960,0,10,160)).count(255) == 3200 and Pump._region(owned.vram_known_mask,Rect2i(960,160,10,95)).count(255) == 0,"move_255_line(5,14,15) copies known top 160 rows and preserves unowned rows 160..254 without inventing ownership")
	for i in range(5,15):
		var src_col := Pump._region(owned.vram_words,Rect2i(i*64+63,0,1,255))
		var dst_col := Pump._region(owned.vram_words,Rect2i(960+(i-5),0,1,255))
		check(src_col == dst_col,"move_255_line column %d matches source x=%d at dst x=%d" % [i,i*64+63,960+(i-5)])
	var target := Image.create(320,240,false,Image.FORMAT_RGBA8); target.fill(Color.BLACK)
	check(hud.draw_checker(target,_hud_projection(state)).is_empty() and media.paint_background(target,state,actor_renderer).is_empty() and actor_renderer.paint(target,state,Vector2i(160,120)).is_empty() and hud.draw_front(target,_hud_projection(state)).is_empty(),"post-board_load frame renders checker(2), arrival_bg(1), petals, and front HUD with hidden date pillar")
	owned.vram_words[(0*1024+960)*2] ^= 1
	check(not upload.batch_ready(board_key).is_empty(),"tampering with move_255_line seam column invalidates board batch readiness")
	owned.vram_words[(0*1024+960)*2] ^= 1
	check(upload.batch_ready(board_key).is_empty(),"restoring seam column restores board batch readiness")
	# Step 3: Verify source-backed Math.rsin quarter-sine table and prologue_board_scroll (stage 1 / step 1 / substep 0..3)
	check(Math.RSIN_QUARTER_TABLE.size() == 1025 and Math.rsin(0) == 0 and Math.rsin(0x200) == 0x0b50 and Math.rsin(0x400) == 0x1000 and Math.rsin(0x600) == 0x0b50 and Math.rsin(0x800) == 0 and Math.rsin(0xa00) == -0x0b50 and Math.rsin(0xc00) == -0x1000 and Math.rsin(0xe00) == -0x0b50 and Math.rsin(0x1000) == 0 and Math.rsin(-0x200) == -0x0b50 and Math.rsin(-0xa00) == 0x0b50,"Math.rsin matches PS1 libgte 0x800A0070 and D_800DD6C0[0..1024]")
	services["line_start"] = func(s: State, line_id: int, out: Session.FrameResult) -> void:
		media.before_line0(s,line_id,out)
		if not out.error.is_empty(): return
		if line_id >= 12:
			out.error = "original prologue line is unavailable"
			return
		var total_chars := 12
		if line_id in [2, 4, 6, 7, 8, 9, 10]:
			total_chars = 2
		s.opening["prologue_text_work"] = {"line_id":line_id,"pos":0,"total":total_chars,"hold":0}
	services["line_run"] = func(s: State, _out: Session.FrameResult) -> void:
		var work: Dictionary = s.opening["prologue_text_work"]
		var line_id: int = int(work.get("line_id",-1))
		if int(work["pos"]) < int(work["total"]):
			work["pos"] = int(work["pos"]) + 1
		else:
			var hold: int = int(work.get("hold",0))
			work["hold"] = hold + 1
			var max_hold := 61
			if line_id == 2:
				max_hold = 42
			elif line_id == 3:
				max_hold = 22
			elif line_id == 4:
				max_hold = 2
			elif line_id == 5:
				max_hold = 38
			elif line_id in [6, 7, 8]:
				max_hold = 1
			elif line_id == 9:
				max_hold = 11
			elif line_id == 10:
				max_hold = 2
			elif line_id == 11:
				max_hold = 52
			if hold >= max_hold:
				s.substep += 1
		s.opening["prologue_text_work"] = work
	state.opening["arrival_media"]["back_clear"] = false
	var tick_idx := 7
	var scroll_sub0 := tick(true)
	check(scroll_sub0.committed and state.mode_stage == 1 and state.mode_step == 1 and state.substep == 1 and state.opening.get("prologue_line") == 1 and state.opening.prologue_text_work.get("line_id") == 1,"prologue_board_scroll substep 0 starts line 1 and advances to substep 1: "+scroll_sub0.error)
	check(media.publish(scroll_sub0,tick_idx).is_empty() and upload.publish(scroll_sub0,tick_idx).is_empty(),"prologue_board_scroll substep 0 frame publishes")
	tick_idx += 1
	var mid_checked_low := false
	var mid_checked_high := false
	var saw_substep2_at_320 := false
	while state.substep in [1, 2] and tick_idx < 900:
		var prev_substep := state.substep
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"prologue_board_scroll tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		var c: int = int(state.opening.get("prologue_scene_counter",-1))
		if prev_substep == 1 and state.substep == 2:
			check(c > 0 and c < 320 and state.opening.prologue_text_work.get("pos") == state.opening.prologue_text_work.get("total"),"line 1 finishes reveal + hold during board scroll before counter reaches 320")
		if not mid_checked_low and c == 128:
			mid_checked_low = true
			var petal_only := Image.create(320,240,false,Image.FORMAT_RGBA8); petal_only.fill(Color.BLACK)
			check(actor_renderer.paint(petal_only,state,Vector2i(160,120)).is_empty(),"mid-scroll (counter=128) paints 8bpp petals onto clean target")
			var pink_petal_pixels := 0
			for py in 160:
				for px in 320:
					var col := petal_only.get_pixel(px,py)
					if col.r8 >= 200 and col.b8 >= 160 and col.r8 > col.g8:
						pink_petal_pixels += 1
			check(pink_petal_pixels > 0,"mid-scroll (counter=128) 8bpp petal lookup produces cherry-blossom pink pixels (r>=200, b>=160, r>g)")
			var mid_img := Image.create(320,240,false,Image.FORMAT_RGBA8); mid_img.fill(Color.BLACK)
			check(hud.draw_checker(mid_img,_hud_projection(state)).is_empty() and media.paint_background(mid_img,state,actor_renderer).is_empty() and actor_renderer.paint(mid_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(mid_img,_hud_projection(state)).is_empty(),"mid-scroll (counter=128) renders 5-quad board panorama, falling petals, and HUD")
		if not mid_checked_high and c == 280:
			mid_checked_high = true
			var high_img := Image.create(320,240,false,Image.FORMAT_RGBA8); high_img.fill(Color.BLACK)
			check(hud.draw_checker(high_img,_hud_projection(state)).is_empty() and media.paint_background(high_img,state,actor_renderer).is_empty() and actor_renderer.paint(high_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(high_img,_hud_projection(state)).is_empty(),"mid-scroll (counter=280 >= 256) renders shifted tpage board panorama, petals, and HUD")
		if state.substep == 2 and c == 320:
			saw_substep2_at_320 = true
	check(mid_checked_low and mid_checked_high and saw_substep2_at_320,"board scroll visited counter=128, counter=280, and held at substep 2 on counter=320 before >=0x141 clamp")
	check(state.mode_stage == 1 and state.mode_step == 1 and state.substep == 3 and state.opening.get("prologue_scene_counter") == 320,"board scroll clamped 0x141->0x140 (320) and advanced to substep 3")
	var end_board_only := Image.create(320,240,false,Image.FORMAT_RGBA8); end_board_only.fill(Color.BLACK)
	check(media.paint_background(end_board_only,state,actor_renderer).is_empty(),"completed scroll (counter=320) paints board panorama cleanly")
	var expected_right_half := Image.create(320,160,false,Image.FORMAT_RGBA8)
	expected_right_half.blit_rect(board_store.preview(),Rect2i(320,0,320,160),Vector2i.ZERO)
	var actual_right_half := Image.create(320,160,false,Image.FORMAT_RGBA8)
	actual_right_half.blit_rect(end_board_only,Rect2i(0,0,320,160),Vector2i.ZERO)
	check(actual_right_half.get_data() == expected_right_half.get_data(),"completed scroll (counter=320) 5-quad srn_tpage_show output matches exact right 320x160 half of 640x160 board")
	check(actor_renderer.paint(end_board_only,state,Vector2i(160,120)).is_empty() and hud.draw_front(end_board_only,_hud_projection(state)).is_empty(),"completed scroll renders petals and HUD over board right half")
	# Step 4: Verify prologue_board_scroll substep 3..5 (0x495D read/hold/decode) and prologue_names_crossfade (stage 2 / step 0 / substep 0..4)
	var sub3 := tick(true)
	check(sub3.committed and state.mode_stage == 1 and state.mode_step == 1 and state.substep == 4 and state.mode_step_timer == 0 and media.publish(sub3,tick_idx).is_empty() and upload.publish(sub3,tick_idx).is_empty(),"substep 3 requests 0x495D into 0x80162000, zeroes mode_step_timer, and advances to substep 4: "+sub3.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].size == 20480 and media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_495D_TRANSFER_SHA,"0x80162000 holds 10-sector P_01_03.BS (0x495D) transfer")
	var sub4_frames := 0
	while state.mode_stage == 1 and state.substep == 4 and tick_idx < 900:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"substep 4 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		sub4_frames += 1
	check(sub4_frames == 122 and state.mode_stage == 1 and state.mode_step == 1 and state.substep == 5 and state.mode_step_timer == 122,"substep 4 holds for 122 frames (old_timer 0..121 >= 0x79) and advances to substep 5")
	var sub5_decode := tick(true)
	check(sub5_decode.committed and state.mode_stage == 2 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("names_media",{}).get("active_draw") == "board_scroll_transition" and media.publish(sub5_decode,tick_idx).is_empty() and upload.publish(sub5_decode,tick_idx).is_empty(),"substep 5 decodes 0x495D into VRAM (704,0,320,160) buffer 0 and advances to stage 2 step 0 substep 0: "+sub5_decode.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(704,0,320,160))) == BoardStore.BG_495D_WORD_SHA,"VRAM (704,0,320,160) holds exact 0x495D decoded RGB555 words")
	var trans_img := Image.create(320,240,false,Image.FORMAT_RGBA8); trans_img.fill(Color.BLACK)
	check(hud.draw_checker(trans_img,_hud_projection(state)).is_empty() and media.paint_background(trans_img,state,actor_renderer).is_empty() and actor_renderer.paint(trans_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(trans_img,_hud_projection(state)).is_empty(),"substep 5 transition frame renders cleanly with live VRAM overwrite")
	var names_sub0 := tick(true)
	check(names_sub0.committed and state.mode_stage == 2 and state.mode_step == 0 and state.substep == 1 and state.opening.get("prologue_line") == 2 and state.opening.get("prologue_scene_counter") == 0 and state.opening.get("names_media",{}).get("active_draw") == "dec_bg_draw" and media.publish(names_sub0,tick_idx).is_empty() and upload.publish(names_sub0,tick_idx).is_empty(),"stage 2 substep 0 requests 0x4966, starts line 2, draws 0x495D via dec_bg_draw(1), and advances to substep 1: "+names_sub0.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_4966_TRANSFER_SHA,"0x80162000 now holds 10-sector P_01_03B.BS (0x4966) transfer")
	var names_495d_img := Image.create(320,240,false,Image.FORMAT_RGBA8); names_495d_img.fill(Color.BLACK)
	check(media.paint_background(names_495d_img,state,actor_renderer).is_empty(),"stage 2 substep 0 dec_bg_draw(1) paints 0x495D close-up board")
	var actual_495d := Image.create(320,160,false,Image.FORMAT_RGBA8)
	actual_495d.blit_rect(names_495d_img,Rect2i(0,0,320,160),Vector2i.ZERO)
	check(actual_495d.get_data() == board_store.background_preview(BoardStore.BG_495D_ID).get_data(),"stage 2 dec_bg_draw(1) output matches exact 0x495D preview")
	var names_sub1_hold := tick(false)
	check(names_sub1_hold.committed and state.mode_stage == 2 and state.mode_step == 0 and state.substep == 1 and media.publish(names_sub1_hold,tick_idx).is_empty() and upload.publish(names_sub1_hold,tick_idx).is_empty(),"stage 2 substep 1 holds while 0x4966 cd_sync==0")
	tick_idx += 1
	var names_sub1_decode := tick(true)
	check(names_sub1_decode.committed and state.mode_stage == 2 and state.mode_step == 0 and state.substep == 2 and state.opening.get("prologue_scene_counter") == 128 and state.opening.get("bg_fade_amount") == 128 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("names_media",{}).get("buffer_1") is Dictionary and state.opening.get("names_media",{}).get("active_draw") == "dec_bg_draw" and media.publish(names_sub1_decode,tick_idx).is_empty() and upload.publish(names_sub1_decode,tick_idx).is_empty(),"stage 2 substep 1 decodes 0x4966 into VRAM (320,256,320,160) buffer 1 without flipping bg_show_1 and advances to substep 2: "+names_sub1_decode.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(320,256,320,160))) == BoardStore.BG_4966_WORD_SHA,"VRAM (320,256,320,160) holds exact 0x4966 decoded RGB555 words")
	var crossfade_in_frames := 0
	while state.substep == 2 and tick_idx < 900:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 2 substep 2 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		crossfade_in_frames += 1
	check(crossfade_in_frames == 39 and state.substep == 3 and state.opening.get("prologue_scene_counter") == 14 and state.opening.get("bg_fade_amount") == 14 and state.opening.get("names_media",{}).get("drawn_fade_amount") == 14,"stage 2 substep 2 steps counter 128->14 by -3 (38 steps + 1 transition frame) and reaches substep 3 with fade=14")
	var peak_img := Image.create(320,240,false,Image.FORMAT_RGBA8); peak_img.fill(Color.BLACK)
	check(hud.draw_checker(peak_img,_hud_projection(state)).is_empty() and media.paint_background(peak_img,state,actor_renderer).is_empty() and actor_renderer.paint(peak_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(peak_img,_hud_projection(state)).is_empty(),"stage 2 substep 3 renders peak bg_crossfade_draw(1) (fade=14), petals, and HUD")
	while state.substep == 3 and tick_idx < 900:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 2 substep 3 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
	check(state.substep == 4 and state.opening.get("prologue_scene_counter") == 14,"stage 2 substep 3 advances to substep 4 after line 2 completes")
	var crossfade_out_frames := 0
	while state.substep == 4 and int(state.opening.get("prologue_scene_counter",0)) < 128 and tick_idx < 900:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 2 substep 4 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		crossfade_out_frames += 1
	check(crossfade_out_frames == 29 and state.substep == 4 and state.opening.get("prologue_scene_counter") == 130 and state.opening.get("bg_fade_amount") == 126 and state.opening.get("names_media",{}).get("drawn_fade_amount") == 122,"stage 2 substep 4 steps counter 14->130 by +4 across 29 committed frames")
	# Step 5: Verify stage 2 substep 4..8, stage 3 (line 3), stage 4 (line 4), and stage 5 substep 0 up to 0xACF6
	var sub4_to_5 := tick(true)
	check(sub4_to_5.committed and state.mode_stage == 2 and state.substep == 5 and state.opening.get("prologue_scene_counter") == 128 and state.opening.get("bg_fade_amount") == 128 and state.opening.get("bg_brightness") == 128 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("names_media",{}).get("active_draw") == "bg_crossfade_draw" and media.publish(sub4_to_5,tick_idx).is_empty() and upload.publish(sub4_to_5,tick_idx).is_empty(),"stage 2 substep 4 transition frame requests 0xAB18 (CUT_12.NBN) into 0x801A4000 and advances to substep 5: "+sub4_to_5.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x801a4000)].size == 100352 and media.diagnostics().buffers[str(0x801a4000)].sha256 == BoardStore.CUT_12_SHA,"0x801A4000 holds 49-sector CUT_12.NBN (0xAB18) transfer")
	var sub5_fade_frames := 0
	var checked_sub5_mid := false
	while state.mode_stage == 2 and state.substep == 5 and tick_idx < 1500:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 2 substep 5 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		sub5_fade_frames += 1
		if not checked_sub5_mid and state.opening.get("prologue_scene_counter") == 64:
			checked_sub5_mid = true
			var fade_img := Image.create(320,240,false,Image.FORMAT_RGBA8); fade_img.fill(Color.BLACK)
			check(hud.draw_checker(fade_img,_hud_projection(state)).is_empty() and media.paint_background(fade_img,state,actor_renderer).is_empty() and actor_renderer.paint(fade_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(fade_img,_hud_projection(state)).is_empty(),"stage 2 substep 5 mid-fade (counter=64) renders dimmed 0x495D background, petals, and HUD")
	check(sub5_fade_frames == 127 and checked_sub5_mid and state.mode_stage == 2 and state.substep == 6 and state.opening.get("prologue_scene_counter") == 2 and state.opening.get("bg_brightness") == 2,"stage 2 substep 5 steps counter 128->2 by -1 across 127 frames and advances to substep 6")
	var sub6 := tick(true)
	check(sub6.committed and state.mode_stage == 2 and state.substep == 7 and state.opening.get("cut_media",{}).get("resource_id") == BoardStore.CUT_12_ID and media.publish(sub6,tick_idx).is_empty() and upload.publish(sub6,tick_idx).is_empty(),"stage 2 substep 6 runs prologue_palette_adjust on CUT_12, requests O_12 (0xAC3F), and advances to substep 7: "+sub6.error)
	tick_idx += 1
	check(upload.batch_ready(state.opening.cut_media.upload_batch_key).is_empty() and media.diagnostics().buffers[str(0x80180000)].size == 34816 and media.diagnostics().buffers[str(0x80180000)].sha256 == BoardStore.O_12_SHA,"CUT_12 uploaded to VRAM (512,0,192,256) + CLUT 494..495 and O_12 (34816B) published to 0x80180000")
	var sub7 := tick(true)
	check(sub7.committed and state.mode_stage == 2 and state.substep == 8 and media.publish(sub7,tick_idx).is_empty() and upload.publish(sub7,tick_idx).is_empty(),"stage 2 substep 7 requests 0xACDD (P_01_04B.BS) and advances to substep 8: "+sub7.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_ACDD_TRANSFER_SHA,"0x80162000 holds 10-sector P_01_04B.BS (0xACDD) transfer")
	var sub8 := tick(true)
	check(sub8.committed and state.mode_stage == 3 and state.mode_step == 0 and state.substep == 0 and media.publish(sub8,tick_idx).is_empty() and upload.publish(sub8,tick_idx).is_empty(),"stage 2 substep 8 decodes 0xACDD into buffer 1, backs up O_12 to 0x801F0000, and advances to stage 3 substep 0: "+sub8.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(320,256,320,160))) == BoardStore.BG_ACDD_WORD_SHA and media.diagnostics().buffers[str(0x801f0000)].size == 34816 and media.diagnostics().buffers[str(0x801f0000)].sha256 == BoardStore.O_12_SHA,"VRAM (320,256,320,160) holds 0xACDD words and 0x801F0000 holds backed-up O_12")
	var s3_sub0 := tick(true)
	check(s3_sub0.committed and state.mode_stage == 3 and state.substep == 1 and state.opening.get("prologue_line") == 3 and not state.opening.has("prologue_petals") and state.opening.get("bg_layer_0",{}).get("clut_row") == 494 and state.opening.get("actor96",{}).get("resource_id") == BoardStore.O_12_ID and media.publish(s3_sub0,tick_idx).is_empty() and upload.publish(s3_sub0,tick_idx).is_empty(),"stage 3 substep 0 clears petals, sets grayscale CUT_12 bg_layer_0 + O_12 actor96, starts line 3, and advances to substep 1: "+s3_sub0.error)
	tick_idx += 1
	check(upload.batch_ready(state.opening.actor96.upload_batch_key).is_empty(),"O_12 slot 5 uploaded to VRAM (320,0,64,128)")
	while state.mode_stage == 3 and state.substep == 1 and tick_idx < 1500:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 3 substep 1 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
	check(state.mode_stage == 3 and state.substep == 2 and state.opening.get("D_801220E0") == 132 and state.opening.get("bg_brightness") == 132,"stage 3 substep 1 fades in D_801220E0..E2 and bg_brightness to 132 (0x84) and completes line 3")
	var s3_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s3_img.fill(Color.BLACK)
	check(hud.draw_checker(s3_img,_hud_projection(state)).is_empty() and media.paint_background(s3_img,state,actor_renderer).is_empty() and actor_renderer.paint(s3_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s3_img,_hud_projection(state)).is_empty(),"stage 3 renders 0xACDD hallway, grayscale CUT_12 Yoshio bg_layer_0, O_12 actor96, and HUD")
	var s3_sub2 := tick(true)
	check(s3_sub2.committed and state.mode_stage == 3 and state.substep == 3 and media.publish(s3_sub2,tick_idx).is_empty() and upload.publish(s3_sub2,tick_idx).is_empty(),"stage 3 substep 2 requests CUT_05 (0xAA23) and advances to substep 3: "+s3_sub2.error)
	tick_idx += 1
	var s3_sub3 := tick(true)
	check(s3_sub3.committed and state.mode_stage == 3 and state.substep == 4 and media.publish(s3_sub3,tick_idx).is_empty() and upload.publish(s3_sub3,tick_idx).is_empty(),"stage 3 substep 3 completes CUT_05 read, requests 0xACE6 (P_01_05.BS), and advances to substep 4: "+s3_sub3.error)
	tick_idx += 1
	var s3_sub4 := tick(true)
	check(s3_sub4.committed and state.mode_stage == 4 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and media.publish(s3_sub4,tick_idx).is_empty() and upload.publish(s3_sub4,tick_idx).is_empty(),"stage 3 substep 4 decodes 0xACE6 into buffer 0, sets bg_show_1=0, and advances to stage 4 substep 0: "+s3_sub4.error)
	tick_idx += 1
	var s4_sub0 := tick(true)
	check(s4_sub0.committed and state.mode_stage == 4 and state.substep == 1 and state.opening.get("prologue_line") == 4 and not state.opening.has("actor96") and state.opening.get("bg_layer_0",{}).get("clut_row") == 495 and media.publish(s4_sub0,tick_idx).is_empty() and upload.publish(s4_sub0,tick_idx).is_empty(),"stage 4 substep 0 requests O_05 (0xABB8), uploads CUT_05, switches bg_layer_0 to color CUT_05, clears actor96, starts line 4, and advances to substep 1: "+s4_sub0.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80180000)].size == 18432 and media.diagnostics().buffers[str(0x80180000)].sha256 == BoardStore.O_05_SHA,"0x80180000 holds 9-sector O_05.NBN (0xABB8) transfer")
	var s4_sub1 := tick(true)
	check(s4_sub1.committed and state.mode_stage == 4 and state.substep == 2 and state.opening.get("actor96",{}).get("resource_id") == BoardStore.O_05_ID and media.publish(s4_sub1,tick_idx).is_empty() and upload.publish(s4_sub1,tick_idx).is_empty(),"stage 4 substep 1 uploads O_05 slot 5, initializes actor96, and advances to substep 2: "+s4_sub1.error)
	tick_idx += 1
	var s4_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s4_img.fill(Color.BLACK)
	check(hud.draw_checker(s4_img,_hud_projection(state)).is_empty() and media.paint_background(s4_img,state,actor_renderer).is_empty() and actor_renderer.paint(s4_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s4_img,_hud_projection(state)).is_empty(),"stage 4 substep 2 renders 0xACE6 background, color CUT_05 Rei bg_layer_0, O_05 actor96, and HUD")
	var installed_root := OS.get_environment("BOARD_INSTALLED_PROFILE_ROOT")
	if not installed_root.is_empty():
		var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(installed_root.path_join("manifest.json")))
		var profile := Profile.read_pack(installed_root,manifest)
		check(profile.get("valid",false) and profile.available_capabilities.has(Profile.OPENING_RESOURCES_CAP) and profile.paths.has("first_arrival_board"),"installed profile validates first_arrival_board role")
		var res := Resources.new()
		check(res.prepare(profile).is_empty() and res.board_store != null and res.board_store.read_payload().size() == 18432,"installed launch resources open board_store when first_arrival_board role is present")
		var line_state := State.new()
		line_state.opening["origin"] = "interactive-entry-owned-state-v1"
		line_state.rule_state = {"profile_family_bytes":[0x82,0xa0,0,0,0,0,0,0],"profile_given_bytes":[0x82,0xa2,0,0,0,0,0,0]}
		var first_lines := Resources.Lines.new()
		check(first_lines.prepare_first_arrival(res.text,res.painter,line_state).is_empty(),"PrologueLineServices.prepare_first_arrival authenticates silent player lines 0..4")
		var presenter := ScenePresenter.new()
		check(presenter.prepare(media,res.text,res.painter,line_state,actor_renderer).is_empty(),"FirstArrivalScenePresenter prepares with lines 0..4")
		var prev_img := Image.create(320,240,false,Image.FORMAT_RGBA8); prev_img.fill(Color.BLACK)
		state.rule_state = line_state.rule_state.duplicate(true)
		state.opening["text_speed"] = 5
		var view4 := res.text.resolve(4,state.rule_state)
		var saved_work: Dictionary = state.opening.prologue_text_work.duplicate(true)
		state.frame_count -= 1
		check(Resources.Lines.Work.begin_line(state,view4).is_empty(),"Work.begin_line initializes line 4 work at substep 0")
		state.frame_count += 1
		var adv_out := Session.FrameResult.new()
		Resources.Lines.Work.advance(state,adv_out)
		check(adv_out.error.is_empty(),"Work.advance accepts first line 4 step at stage 4 substep 2 after substep 1 CD wait: "+adv_out.error)
		state.opening.prologue_text_work["pos"] = state.opening.prologue_text_work["total"]
		var rendered := presenter.render(state,func(img: Image, s: State) -> String: return hud.draw_checker(img,_hud_projection(s)),func(img: Image, s: State) -> String: return hud.draw_front(img,_hud_projection(s)),upload.batch_ready,prev_img)
		check(str(rendered.get("error","")).is_empty() and rendered.get("image") is Image,"FirstArrivalScenePresenter renders stage 4 Rei scene + HUD + line 4 text: "+str(rendered.get("error","")))
		state.opening["prologue_text_work"] = saved_work
		state.opening.erase("prologue_text")
	else:
		print("SKIP: 7 installed-profile/resource/text-render checks require BOARD_INSTALLED_PROFILE_ROOT; remaining checks do not certify installed launch")
	while state.mode_stage == 4 and state.substep == 2 and tick_idx < 2000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 4 substep 2 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
	check(state.mode_stage == 4 and state.substep == 3,"stage 4 substep 2 completes line 4 and advances to substep 3")
	var s4_sub3 := tick(true)
	check(s4_sub3.committed and state.mode_stage == 4 and state.substep == 4 and media.publish(s4_sub3,tick_idx).is_empty() and upload.publish(s4_sub3,tick_idx).is_empty(),"stage 4 substep 3 requests CUT_12 (0xAB18) and advances to substep 4: "+s4_sub3.error)
	tick_idx += 1
	var s4_sub4 := tick(true)
	check(s4_sub4.committed and state.mode_stage == 5 and state.mode_step == 0 and state.substep == 0 and state.opening.get("actor97",{}).get("x") == -32 and state.opening.get("prologue_text_cleared") == true and media.publish(s4_sub4,tick_idx).is_empty() and upload.publish(s4_sub4,tick_idx).is_empty(),"stage 4 substep 4 restores O_12, uploads slots 5+6 and CUT_12, initializes actor97 arm at x=-32, clears text via k_sub_reset, and advances to stage 5 substep 0: "+s4_sub4.error)
	tick_idx += 1
	var s5_slide_frames := 0
	while state.mode_stage == 5 and state.substep == 0 and int(state.opening.get("prologue_scene_counter",0)) < 0x80 and tick_idx < 2500:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 5 substep 0 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		s5_slide_frames += 1
	check(s5_slide_frames == 96 and state.mode_stage == 5 and state.substep == 0 and state.opening.get("prologue_scene_counter") == 128 and state.opening.get("actor97",{}).get("x") == -160,"stage 5 substep 0 slides actor97 arm from x=-32 to x=-160 across 96 committed frames (counter 0->128)")
	# Step 6: Verify stage 5 substep 0..5 (0xACF6 color burst, arm slide out, CUT_06 + 0xACD4 + O_06 classroom setup) and stage 6 substep 0 boundary
	var s5_sub0_to_1 := tick(true)
	check(s5_sub0_to_1.committed and state.mode_stage == 5 and state.substep == 1 and state.opening.get("prologue_scene_counter") == 0x81 and state.opening.get("stage_05_se_605_count") == 1 and state.opening.get("actor97",{}).get("flags") == 5 and state.opening.get("actor97",{}).get("seq_step") == 1 and state.opening.get("actor97",{}).get("frame_index") == 1 and state.opening.get("actor97",{}).get("sequence_counter") == 1 and media.publish(s5_sub0_to_1,tick_idx).is_empty() and upload.publish(s5_sub0_to_1,tick_idx).is_empty(),"stage 5 substep 0 at counter 128->129 emits audio_command(0x605), arms actor97 seq[0], requests 0xACF6 (P_01_07.BS), and advances to substep 1: "+s5_sub0_to_1.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].size == 20480 and media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_ACF6_TRANSFER_SHA,"0x80162000 holds 10-sector P_01_07.BS (0xACF6) transfer")
	var s5_seq_frames := 1
	while state.mode_stage == 5 and state.substep == 1 and (int(state.opening.get("actor97",{}).get("flags",0)) & 1) != 0 and tick_idx < 3000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 5 substep 1 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		s5_seq_frames += 1
	check(s5_seq_frames == 126 and state.mode_stage == 5 and state.substep == 1 and state.opening.get("stage_05_se_605_count") == 2 and state.opening.get("actor97",{}).get("flags") == 0 and state.opening.get("actor97",{}).get("seq_step") == 4 and state.opening.get("actor97",{}).get("frame_index") == 0,"actor97 seq[0] steps 1..4 run for 18+18+18+72=126 frames, emit second audio_command(0x605) at step 3 counter 0, and clear flags (5 & 0xfffa = 0)")
	var s5_sub1_to_2 := tick(true)
	check(s5_sub1_to_2.committed and state.mode_stage == 5 and state.substep == 2 and state.opening.get("prologue_scene_counter") == 128 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("bg_layer_0",{}).get("clut_row") == 495 and state.opening.get("bg_layer_0",{}).get("resource_id") == BoardStore.CUT_12_ID and state.opening.get("actor97",{}).get("clut_row") == 509 and state.opening.get("actor97",{}).get("unk_43") == 0x1d and media.publish(s5_sub1_to_2,tick_idx).is_empty() and upload.publish(s5_sub1_to_2,tick_idx).is_empty(),"stage 5 substep 1 decodes 0xACF6 into buffer 0, switches CUT_12 bg_layer_0 to CLUT 495 and actor97 to CLUT 509, requests CUT_06 (0xAA54), and advances to substep 2: "+s5_sub1_to_2.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(704,0,320,160))) == BoardStore.BG_ACF6_WORD_SHA and media.diagnostics().buffers[str(0x801a4000)].size == 100352 and media.diagnostics().buffers[str(0x801a4000)].sha256 == BoardStore.CUT_06_SHA,"VRAM (704,0,320,160) holds 0xACF6 words and 0x801A4000 holds 49-sector CUT_07.NBN (0xAA54) transfer")
	var s5_burst_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s5_burst_img.fill(Color.BLACK)
	check(hud.draw_checker(s5_burst_img,_hud_projection(state)).is_empty() and media.paint_background(s5_burst_img,state,actor_renderer).is_empty() and actor_renderer.paint(s5_burst_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s5_burst_img,_hud_projection(state)).is_empty(),"stage 5 substep 2 color burst renders 0xACF6 background, full-color CUT_12 Yoshio (CLUT 495), O_12 actor96 + full-color actor97 arm (CLUT 509), and HUD")
	var s5_sub2_frames := 0
	while state.mode_stage == 5 and state.substep == 2 and tick_idx < 3000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 5 substep 2 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		s5_sub2_frames += 1
	check(s5_sub2_frames == 55 and state.mode_stage == 5 and state.substep == 3 and state.opening.get("prologue_scene_counter") == -34 and state.opening.get("actor97",{}).get("x") == 2,"stage 5 substep 2 slides actor97 arm out to x=+2 (counter 128->-34 by -3 across 54 frames + 1 advance frame) and advances to substep 3")
	var s5_sub3 := tick(true)
	check(s5_sub3.committed and state.mode_stage == 5 and state.substep == 4 and media.publish(s5_sub3,tick_idx).is_empty() and upload.publish(s5_sub3,tick_idx).is_empty(),"stage 5 substep 3 completes CUT_06 read, requests 0xACD4 (P_01_04.BS), and advances to substep 4: "+s5_sub3.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].size == 20480 and media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_ACD4_TRANSFER_SHA,"0x80162000 holds 10-sector P_01_04.BS (0xACD4) transfer")
	var s5_sub4 := tick(true)
	check(s5_sub4.committed and state.mode_stage == 5 and state.substep == 5 and media.publish(s5_sub4,tick_idx).is_empty() and upload.publish(s5_sub4,tick_idx).is_empty(),"stage 5 substep 4 completes 0xACD4 read, requests O_06 (0xABCA, 25 sectors -> 0x801F0000), and advances to substep 5: "+s5_sub4.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x801f0000)].size == 51200 and media.diagnostics().buffers[str(0x801f0000)].sha256 == BoardStore.O_06_SHA,"0x801F0000 holds 25-sector O_07.NBN (0xABCA) transfer")
	var s5_sub5 := tick(true)
	check(s5_sub5.committed and state.mode_stage == 6 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("bg_show_1") == 1 and state.opening.get("bg_layer_0",{}).get("resource_id") == BoardStore.CUT_06_ID and state.opening.get("bg_layer_0",{}).get("clut_row") == 495 and not state.opening.has("actor97") and state.opening.get("actor96",{}).get("profile") == "o06_yoshio_body" and state.opening.get("actor99",{}).get("profile") == "o06_crowd_girls" and media.publish(s5_sub5,tick_idx).is_empty() and upload.publish(s5_sub5,tick_idx).is_empty(),"stage 5 substep 5 uploads CUT_06, decodes 0xACD4 into buffer 1, copies O_06 to 0x80180000, uploads slots 5/6/7, sets up actor96 + actor99, and advances to stage 6 substep 0: "+s5_sub5.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(320,256,320,160))) == BoardStore.BG_ACD4_WORD_SHA and media.diagnostics().buffers[str(0x80180000)].size == 51200 and media.diagnostics().buffers[str(0x80180000)].sha256 == BoardStore.O_06_SHA and upload.batch_ready(state.opening.cut_media.upload_batch_key).is_empty() and upload.batch_ready(state.opening.actor96.upload_batch_key).is_empty() and upload.batch_ready(state.opening.actor96.upload_batch_key_2).is_empty() and upload.batch_ready(state.opening.actor99.upload_batch_key).is_empty(),"VRAM holds 0xACD4 words, CUT_06, and O_06 slots 5/6/7, and 0x80180000 holds copied 51200B O_06")
	var s6_ready_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s6_ready_img.fill(Color.BLACK)
	check(hud.draw_checker(s6_ready_img,_hud_projection(state)).is_empty() and media.paint_background(s6_ready_img,state,actor_renderer).is_empty() and actor_renderer.paint(s6_ready_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s6_ready_img,_hud_projection(state)).is_empty(),"stage 6 substep 0 transition frame renders 0xACD4 classroom background, color CUT_06 bg_layer_0, O_06 actor96 Yoshio + actor99 crowd girls, and HUD")
	# Step 7: Verify stage 6 (substeps 0..16, lines 5..10, actors 96..103, SE 0x503, 0xACEC + CUT_07 + O_07) and stage 7 (substeps 0..3, line 11, actors 96..98 slide, 0xAD00 boundary)
	state.opening["cd_audio_state_override"] = 0
	var s6_sub0 := tick(true)
	check(s6_sub0.committed and state.mode_stage == 6 and state.substep == 1 and state.opening.get("prologue_line") == 5 and media.publish(s6_sub0,tick_idx).is_empty() and upload.publish(s6_sub0,tick_idx).is_empty(),"stage 6 substep 0 starts line 5 and advances to substep 1: "+s6_sub0.error)
	tick_idx += 1
	var s6_sub1_hold := tick(true)
	check(s6_sub1_hold.committed and state.mode_stage == 6 and state.substep == 1 and media.publish(s6_sub1_hold,tick_idx).is_empty() and upload.publish(s6_sub1_hold,tick_idx).is_empty(),"stage 6 substep 1 holds while cd_audio_state() & 4 == 0")
	tick_idx += 1
	state.opening["cd_audio_state_override"] = 4
	var s6_sub1_ready := tick(true)
	check(s6_sub1_ready.committed and state.mode_stage == 6 and state.substep == 2 and state.mode_step_timer == 0 and media.publish(s6_sub1_ready,tick_idx).is_empty() and upload.publish(s6_sub1_ready,tick_idx).is_empty(),"stage 6 substep 1 advances to substep 2 when cd_audio_state() & 4 == 4: "+s6_sub1_ready.error)
	tick_idx += 1
	var saw_actor102 := false
	var saw_collar_10 := false
	while state.mode_stage == 6 and state.substep == 2 and tick_idx < 4000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 6 substep 2 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		if state.opening.has("actor102") and not saw_actor102:
			saw_actor102 = true
			check(state.mode_step_timer == 4 and state.opening.actor102.get("profile") == "o06_yoshio_blink" and state.opening.get("actor98",{}).get("frame_index") == 10,"stage 6 substep 2 initializes actor102 blink on the fourth FINISH_HOLD frame (mode_step_timer == 4 after line_run, prologue.c), with actor98 at collar frame 10")
		if state.opening.get("actor98",{}).get("frame_index") == 10 and not saw_collar_10:
			saw_collar_10 = true
	check(saw_actor102 and saw_collar_10 and state.substep == 3 and state.opening.get("actor102",{}).get("flags") == 0 and state.opening.get("actor102",{}).get("frame_index") == 1,"stage 6 substep 2 completes line 5, steps actor98 collar 8->10, and finishes actor102 blink at frame 1")
	while state.mode_stage == 6 and state.substep in [3, 4, 5, 6, 7, 8] and tick_idx < 5000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 6 substep %d tick %d failed: %s" % [state.substep,tick_idx,r.error])
			break
		tick_idx += 1
	check(state.mode_stage == 6 and state.substep == 9 and state.opening.get("prologue_line") == 8,"stage 6 advances through lines 6..8 to substep 9")
	var s6_sub9 := tick(true)
	check(s6_sub9.committed and state.mode_stage == 6 and state.substep == 10 and state.opening.get("prologue_line") == 9 and state.opening.get("stage_06_se_503_count") == 1 and state.opening.get("actor99",{}).get("flags") == 5 and media.publish(s6_sub9,tick_idx).is_empty() and upload.publish(s6_sub9,tick_idx).is_empty(),"stage 6 substep 9 emits audio_command(0x503), sets actor99 flags=5, starts line 9, and advances to substep 10: "+s6_sub9.error)
	tick_idx += 1
	while state.mode_stage == 6 and state.substep == 10 and tick_idx < 6000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 6 substep 10 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
	check(state.mode_stage == 6 and state.substep == 11 and state.opening.get("actor99",{}).get("frame_index") == 2 and state.opening.get("actor99",{}).get("flags") == 0 and state.opening.get("actor100",{}).get("profile") == "o06_mouth_girl_a","stage 6 substep 10 turns actor99 crowd girls to frame 2, runs actor100 Girl A mouth, and advances to substep 11")
	var s6_sub11 := tick(true)
	check(s6_sub11.committed and state.mode_stage == 6 and state.substep == 12 and state.opening.get("prologue_line") == 10 and state.opening.get("actor103",{}).get("profile") == "o06_yoshio_sweat" and state.opening.get("actor103",{}).get("frame_index") == 7 and media.publish(s6_sub11,tick_idx).is_empty() and upload.publish(s6_sub11,tick_idx).is_empty(),"stage 6 substep 11 initializes actor103 sweat (frame 7), starts line 10, and advances to substep 12: "+s6_sub11.error)
	tick_idx += 1
	var checked_s6_full_paint := false
	while state.mode_stage == 6 and state.substep == 12 and tick_idx < 7000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 6 substep 12 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		if not checked_s6_full_paint and state.opening.has("actor101"):
			checked_s6_full_paint = true
			var s6_full_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s6_full_img.fill(Color.BLACK)
			check(hud.draw_checker(s6_full_img,_hud_projection(state)).is_empty() and media.paint_background(s6_full_img,state,actor_renderer).is_empty() and actor_renderer.paint(s6_full_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s6_full_img,_hud_projection(state)).is_empty(),"stage 6 substep 12 renders full classroom scene with actors 96, 97, 98, 99, 100, 101, 102, 103 and HUD")
	check(checked_s6_full_paint and state.mode_stage == 6 and state.substep == 13 and state.opening.get("actor101",{}).get("profile") == "o06_mouth_girl_b","stage 6 substep 12 runs actor101 Girl B mouth and advances to substep 13")
	var s6_sub13 := tick(true)
	check(s6_sub13.committed and state.mode_stage == 6 and state.substep == 14 and media.publish(s6_sub13,tick_idx).is_empty() and upload.publish(s6_sub13,tick_idx).is_empty(),"stage 6 substep 13 requests 0xACEC (P_01_06.BS) and advances to substep 14: "+s6_sub13.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x80162000)].sha256 == BoardStore.BG_ACEC_TRANSFER_SHA,"0x80162000 holds 10-sector P_01_06.BS (0xACEC) transfer")
	var s6_sub14 := tick(true)
	check(s6_sub14.committed and state.mode_stage == 6 and state.substep == 15 and media.publish(s6_sub14,tick_idx).is_empty() and upload.publish(s6_sub14,tick_idx).is_empty(),"stage 6 substep 14 completes 0xACEC read, requests CUT_07 (0xAA85), and advances to substep 15: "+s6_sub14.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x801a4000)].sha256 == BoardStore.CUT_07_SHA,"0x801A4000 holds 49-sector CUT_08.NBN (0xAA85) transfer")
	var s6_sub15 := tick(true)
	check(s6_sub15.committed and state.mode_stage == 6 and state.substep == 16 and media.publish(s6_sub15,tick_idx).is_empty() and upload.publish(s6_sub15,tick_idx).is_empty(),"stage 6 substep 15 completes CUT_07 read, requests O_07 (0xABE3), and advances to substep 16: "+s6_sub15.error)
	tick_idx += 1
	check(media.diagnostics().buffers[str(0x801f0000)].sha256 == BoardStore.O_07_SHA,"0x801F0000 holds 25-sector O_08.NBN (0xABE3) transfer")
	var s6_sub16 := tick(true)
	check(s6_sub16.committed and state.mode_stage == 7 and state.mode_step == 0 and state.substep == 0 and state.opening.get("names_media",{}).get("bg_show_1") == 0 and state.opening.get("bg_layer_0",{}).get("resource_id") == BoardStore.CUT_07_ID and state.opening.get("actor96",{}).get("profile") == "o07_yoshio_body" and state.opening.get("actor96",{}).get("x") == -224 and not state.opening.has("actor99") and not state.opening.has("actor103") and media.publish(s6_sub16,tick_idx).is_empty() and upload.publish(s6_sub16,tick_idx).is_empty(),"stage 6 substep 16 decodes 0xACEC into buffer 0, uploads CUT_07, copies O_07 to 0x80180000, uploads slots 5/6/7, clears actors 97..103, sets actor96 at x=-224, and advances to stage 7 substep 0: "+s6_sub16.error)
	tick_idx += 1
	check(Store._sha(Pump._region(owned.vram_words,Rect2i(704,0,320,160))) == BoardStore.BG_ACEC_WORD_SHA and upload.batch_ready(state.opening.cut_media.upload_batch_key).is_empty() and upload.batch_ready(state.opening.actor96.upload_batch_key).is_empty() and upload.batch_ready(state.opening.actor96.upload_batch_key_2).is_empty() and upload.batch_ready(state.opening.actor96.upload_batch_key_3).is_empty(),"VRAM holds 0xACEC words, CUT_07, and O_07 slots 5/6/7")
	var s7_sub0 := tick(true)
	check(s7_sub0.committed and state.mode_stage == 7 and state.substep == 1 and state.opening.get("prologue_line") == 11 and media.publish(s7_sub0,tick_idx).is_empty() and upload.publish(s7_sub0,tick_idx).is_empty(),"stage 7 substep 0 starts line 11 and advances to substep 1: "+s7_sub0.error)
	tick_idx += 1
	var s7_sub1 := tick(true)
	check(s7_sub1.committed and state.mode_stage == 7 and state.substep == 2 and state.opening.get("actor96",{}).get("x") == -223 and media.publish(s7_sub1,tick_idx).is_empty() and upload.publish(s7_sub1,tick_idx).is_empty(),"stage 7 substep 1 advances to substep 2 and steps actor96.x from -224 to -223: "+s7_sub1.error)
	tick_idx += 1
	var checked_s7_paint := false
	while state.mode_stage == 7 and state.substep == 2 and tick_idx < 8000:
		var r := tick(true)
		if not r.committed or not media.publish(r,tick_idx).is_empty() or not upload.publish(r,tick_idx).is_empty():
			check(false,"stage 7 substep 2 tick %d failed: %s" % [tick_idx,r.error])
			break
		tick_idx += 1
		if not checked_s7_paint and state.opening.has("actor97") and state.opening.has("actor98"):
			checked_s7_paint = true
			var s7_img := Image.create(320,240,false,Image.FORMAT_RGBA8); s7_img.fill(Color.BLACK)
			check(hud.draw_checker(s7_img,_hud_projection(state)).is_empty() and media.paint_background(s7_img,state,actor_renderer).is_empty() and actor_renderer.paint(s7_img,state,Vector2i(160,120)).is_empty() and hud.draw_front(s7_img,_hud_projection(state)).is_empty(),"stage 7 substep 2 renders 0xACEC background, color CUT_07 bg_layer_0, O_07 actor96 + actor97 mouth + actor98 hand, and HUD")
	check(checked_s7_paint and state.mode_stage == 7 and state.substep == 3 and state.opening.get("actor96",{}).get("x") == -160 and state.opening.get("actor97",{}).get("x") == -160 and state.opening.get("actor98",{}).get("x") == -160,"stage 7 substep 2 slides actors 96, 97, 98 to x=-160 (-0xA0) and advances to substep 3")
	var before_s7_vram := runtime_hash()
	var s7_boundary := tick(true)
	if not s7_boundary.committed or state.substep != 4 or runtime_hash() != before_s7_vram:
		printerr("AD00 diagnostic: ",s7_boundary.error," committed=",s7_boundary.committed," substep=",state.substep," runtime_changed=",runtime_hash() != before_s7_vram)
	var ad00_publication := media.publish(s7_boundary,tick_idx) if s7_boundary.committed else s7_boundary.error
	check(s7_boundary.committed and state.substep == 4 and runtime_hash() == before_s7_vram and ad00_publication.is_empty(),"stage 7 substep3 queues authenticated AD00 read without prematurely decoding or changing VRAM: " + ad00_publication)
	print("PASS: %d first-arrival board-load checks" % checks if failures == 0 else "FAIL: %d/%d first-arrival board-load checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
