extends "res://tests/run_interactive_prologue.gd"
## Live input adapter + original mascot; invented entry profile/services/clock
## exercise source handoff semantics, not normal-start or native clock parity.
const ContextOwner = preload("res://flow/entry_runtime_context.gd")
const Core = preload("res://core/game_state.gd")
const Clock = preload("res://core/native_clock.gd")
const EntryPad = preload("res://ui/entry_pad.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
func key(pad: EntryPad, held: bool) -> void:
	var event:=InputEventKey.new()
	event.physical_keycode=KEY_ENTER
	event.pressed=held
	pad._input(event)
func _initialize() -> void:
	call_deferred("run_context")
func run_context() -> void:
	var app_script: Script = load("res://app/new_game_entry_boot.gd")
	check(app_script != null and app_script.can_instantiate(),"proposed live entry app handoff script compiles")
	var rng:=Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,12345)
	var entry:=committed_entry(rng)
	var original_rng:Dictionary=rng.checkpoint()
	var text:=TextStore.new()
	check(text.open("/original-cache/port/t29025-prologue-text-store-v1","5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc").is_empty(),"original mascot pack authenticates")
	var pad:=EntryPad.new()
	root.add_child(pad)
	var model:=Core.new()
	model.frame_count=501 # invented existing app clock prefix, not an RNG seed.
	model.system_seed=0 # sys_init bzero; independent timer observations follow.
	model.timer_ticks=0
	var clock:=Clock.new()
	clock.logic_iterations=12 # session-local sample index need not equal frame.
	var owner:=ContextOwner.new()
	var policy:Dictionary={"clock_origin":"invented-current-app-prefix","timer_observation_source":"desktop-one-timer-per-logic-v1","native_clock_parity":false}
	check(owner.bind(model,clock,pad,policy).is_empty(),"explicit live owners and deterministic desktop timer bind")
	check(not owner.observe_timer_interrupts(1).is_empty(),"fixed desktop policy cannot accidentally double count external timer events")
	check(not owner.export_profile(entry,rng,text).get("error","").is_empty(),"no default input brightness or residency observation")
	key(pad,true)
	var sample:InputFrame=pad.sample(clock.logic_iterations)
	check(sample.pressed==0x800,"actual input adapter records held START key event")
	var resident:Dictionary={"lba":0x95ea,"source_sha256":ContextOwner.NAME_ENT_SHA,"content_sha256":Entry.CONTENT_SHA,"ready":true}
	for foreign: Dictionary in [{},{"lba":0x9a0a},{"source_sha256":"a".repeat(64)},{"ready":false}]:
		var bad_residency: Dictionary = {} if foreign.is_empty() else resident.duplicate(true)
		bad_residency.merge(foreign,true)
		check(not owner.record_completed_iteration(sample,8,bad_residency).is_empty(),"missing or foreign NAME_ENT residency rejects")
		check(model.frame_count==501 and model.system_seed==0 and model.timer_ticks==0 and clock.logic_iterations==12,"rejected context does not fabricate clock/timer progress")
	check(not owner.record_completed_iteration(InputFrame.new(12,0x800),8,resident).is_empty(),"forged snapshot cannot replace the retained physical input owner")
	check(owner.record_completed_iteration(sample,8,resident).is_empty(),"actual completed iteration records rendered UI brightness and prepared NAME_ENT")
	var handoff:Dictionary=owner.export_profile(entry,rng,text)
	check(handoff.error.is_empty(),"committed profile exports context")
	check(handoff.context.frame_count==502 and handoff.context.system_seed==0x377 and handoff.context.timer_ticks==1,"post-dispatch frame increment and independent native timer words")
	check(handoff.context.screen_brightness==8 and handoff.context.held_input==0x800 and handoff.context.resident_overlay_lba==0x95ea,"actual observed UI/input/residency retained")
	check(handoff.rng==rng and rng.checkpoint()==original_rng and handoff.input_owner==pad,"same RNG/device owners retained without draws or reconstruction")
	check(handoff.context.frame_count!=handoff.context.system_seed and rng.state!=handoff.context.frame_count and rng.state!=handoff.context.system_seed,"frame RNG and system seed are separate values")
	check(handoff.constants==text.initial_constants(),"initial mascot comes from authenticated original store")
	check(not owner.export_profile(entry,rng,TextStore.new()).get("error","").is_empty(),"missing authentic mascot does not become zero")
	model.frame_count+=1
	check(not owner.export_profile(entry,rng,text).get("error","").is_empty(),"stale frame context cannot be reused")
	model.frame_count-=1
	var peer:=Provider.new()
	var live:=Live.new()
	check(live.begin(entry,rng,handoff.context,handoff.constants,peer.services(),[peer],[peer.publish]).is_empty(),"actual committed-entry handoff starts live exit controller")
	check(live.state.timer_ticks==1 and live.state.system_seed==0x377,"timer counter and system seed survive adapter handoff")
	check(live.session.latched_input.held==0x800 and live.session.latched_input.pressed==0,"held commit START is latched without a fresh edge")
	check(pad.sample(0).pressed==0,"same device owner does not reedge held START")
	key(pad,false)
	var reached_seed:bool=false
	for tick:int in range(100):
		var seed_frame:int=live.state.frame_count
		var reseed_now:bool=live.state.game_mode==0x15 and live.state.mode_step==1 and live.state.substep==0
		var timer:=Session.ServiceEvent.new(tick,Session.Phase.DRAW_BEFORE_INPUT,Session.ServiceKind.TIMER_INTERRUPT)
		var result:Session.FrameResult=live.step(pad.sample(tick),[timer])
		check(result.committed,"one actual input/timer schedule iteration commits")
		if not result.committed:break
		if reseed_now:
			reached_seed=true
			check(rng.state==seed_frame and rng.draw_count==0,"source prologue stage0 reseeds current frame, not entry or system seed")
			check(live.state.system_seed!=rng.state and live.state.timer_ticks==tick+2,"timer observations remain independent across reseed")
			break
	check(reached_seed,"source exit hold/fade/resident loader naturally reaches reseed without forced completion")
	var external:=ContextOwner.new()
	var external_model:=Core.new()
	external_model.frame_count=900
	external_model.system_seed=0
	external_model.timer_ticks=0
	var external_clock:=Clock.new()
	check(external.bind(external_model,external_clock,pad,{"clock_origin":"invented-external-schedule","timer_observation_source":"explicit-external-timer-observations-v1","native_clock_parity":false}).is_empty(),"explicit external timer schedule is a separate policy")
	check(external.observe_timer_interrupts(3).is_empty() and external.observe_timer_seed_step().is_empty(),"actual timer observations and explicit seed-step source calls")
	check(external_model.frame_count==900 and external_model.timer_ticks==3 and external_model.system_seed==4*0x377,"neither timer callback nor seed-step increments frame/RNG")
	check(rng.state!=external_model.system_seed,"external system-seed changes do not reseed libc RNG")
	pad.queue_free()
	await process_frame
	print("entry_context checks=%d failures=%d"%[checks,failures])
	quit(1 if failures else 0)
