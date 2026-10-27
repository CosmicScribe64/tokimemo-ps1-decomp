extends SceneTree
const Provider = preload("res://presentation/prologue_hud_source_provider.gd")
const Uploads = preload("res://presentation/prologue_actor_upload_provider.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Session = preload("res://core/game_session.gd")
const PIN := "c930a17327e5ad0bdf3b26ab8c20287775b7fc795df240b5f071ed28c4f25f23"
var checks := 0
var failures := 0
func check(ok: bool, label: String) -> void:
 checks += 1
 if not ok:
  failures += 1
  printerr(label)
func state_fixture() -> Dictionary:
 return {"game_mode":21,"mode_stage":0,"mode_step":0,"frame_count":3,"player_flags":PackedByteArray([0,0]),"opening":{"screen_brightness":10,"prologue_hud_live":{"checker_scroll_owner":"invented-source-byte-accumulator","checker_scroll_u":17,"checker_scroll_v":247,"checker_scroll_phase":"after_hud_screen_frame_draw_scroll_update","year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"screen_brightness":128,"title_timer":-64,"checker_fade":255}}}
func _initialize() -> void:
 var provider := Provider.new()
 var open_error: String = provider.open("/work",PIN)
 check(open_error.is_empty(),"pinned V2 opens: " + open_error)
 if not open_error.is_empty():
  quit(1)
  return
 var mainrom := FileAccess.get_file_as_bytes("/work/disc/files/CDROM/FUKUTEST/MAINROM.NBN")
 var exe := FileAccess.get_file_as_bytes("/work/disc/files/SLPM_86.053")
 var state := state_fixture()
 var mismatch := 0
 # Direct original source addressing is independent of packaged index payload.
 # Cover every source pixel at native UV zero and maximal scroll, all CLUTs,
 # endpoint/intermediate fades and quantized brightness values.
 for season in range(4):
  for scroll: int in [0,31]:
   for fade: int in [240,247,254,255]:
    for brightness: int in [0,37,128]:
     state.opening.screen_brightness = brightness
     state.opening.prologue_hud_live.month = [4,7,10,1][season]
     state.opening.prologue_hud_live.checker_scroll_u = scroll
     state.opening.prologue_hud_live.checker_scroll_v = scroll
     state.opening.prologue_hud_live.checker_clut_fade = fade
     var image := Image.create(320,240,false,Image.FORMAT_RGBA8)
     image.fill(Color8(8,16,24,255))
     var before := state.duplicate(true)
     check(provider.draw_checker(image,state).is_empty(),"source checker draw")
     check(state==before,"draw is pure")
     for y in range(32):
      for x in range(32):
       var packed: int = mainrom[0xc000+(y+scroll)*64+(x+scroll)/2]
       var index: int = (packed >> (4*((x+scroll)&1)))&15
       var next: int = exe.decode_u16(0x75328+season*32+index*2)
       var old: int = exe.decode_u16(0x75328+((season+3)&3)*32+index*2)
       var expected: Array[int] = []
       var pre: Array[int] = []
       for shift: int in [0,5,10]:
        var channel: int = (next>>shift)&31
        if fade!=255: channel=((255-fade)*((old>>shift)&31)+(fade-239)*channel)>>4
        pre.append(channel)
        expected.append(((channel*brightness)>>7)<<3)
       if pre==[0,0,0] and (next&0x8000)==0: expected=[8,16,24]
       var pixel := image.get_pixel(x,y)
       if [pixel.r8,pixel.g8,pixel.b8]!=expected: mismatch+=1
     check(image.get_pixel(0,0)==image.get_pixel(32,32),"native SPRT repeats UV per tile")
 check(mismatch==0,"all original source pixels and CLUT fades match; mismatches=%d"%mismatch)
 state=state_fixture()
 var initial := state.duplicate(true)
 check(provider.advance_outer(state,null,{}).is_empty(),"outer reads owner")
 check(state.opening.screen_brightness==10 and state.opening.prologue_hud_live.screen_brightness==10,"mode_step0 never increments independent brightness")
 check(state.opening.prologue_hud_live.checker_scroll_u==18 and state.opening.prologue_hud_live.checker_scroll_v==246,"actual scroll accumulated from supplied values")
 var wrap := initial.duplicate(true)
 wrap.opening.prologue_hud_live.checker_scroll_u=255
 wrap.opening.prologue_hud_live.checker_scroll_v=0
 check(provider.advance_outer(wrap,null,{}).is_empty() and wrap.opening.prologue_hud_live.checker_scroll_u==0 and wrap.opening.prologue_hud_live.checker_scroll_v==255,"u8 update wraps exactly at source store")
 for step: int in [0,1,2]:
  state.mode_step=step
  state.opening.screen_brightness=44+step
  check(provider.advance_outer(state,null,{}).is_empty() and state.opening.prologue_hud_live.screen_brightness==44+step,"every step copies Arrival brightness")
 state=initial.duplicate(true)
 check(provider.scene_reset(state,{}).is_empty(),"source reset callback")
 check(state.opening.screen_brightness==10 and state.opening.prologue_hud_live.screen_brightness==10,"reset callback does not change owner brightness")
 check(state.opening.prologue_hud_live.checker_scroll_u==17 and state.opening.prologue_hud_live.checker_scroll_v==247,"scene reset preserves prefix scroll")
 # Exercise the actual sourced Arrival branch, with explicitly invented bank
 # services. This proves owner ordering only, not a ready bank or live route.
 var route := RouteState.new()
 route.game_mode=21;route.mode_stage=0;route.mode_step=0;route.frame_count=3
 route.opening=initial.opening.duplicate(true)
 var rng := Rng.new();rng.configure(Rng.OPENBIOS_PROFILE,7)
 var result := Session.FrameResult.new()
 var services := {"bank_load_start":func(_s,_b,_o):pass,
  "scene_reset":func(s,_o):
   var payload: Dictionary = s.projection()
   check(provider.scene_reset(payload,{}).is_empty(),"actual Arrival scene reset callback")
   s.opening=payload.opening,
  "bank_poll":func(_s,_o):return 0,"arrival_draw":func(_s,_a,_o):pass,"outer_draw":func(_s,_a,_o):pass}
 Arrival._stage_00_arrival(route,rng,services,result)
 check(route.mode_step==1 and route.opening.screen_brightness==10,"actual step0 does not ramp")
 Arrival._stage_00_arrival(route,rng,services,result)
 check(route.substep==1 and route.opening.screen_brightness==1,"actual source reset0 then sole ramp1")
 var payload: Dictionary = route.projection()
 payload.player_flags=PackedByteArray([0,0])
 check(provider.advance_outer(payload,null,{}).is_empty() and payload.opening.screen_brightness==1 and payload.opening.prologue_hud_live.screen_brightness==1,"outer copies actual post-Arrival owner")
 Arrival._stage_00_arrival(route,rng,services,result)
 check(route.opening.screen_brightness==2,"actual bankwait ramp once, no duplicate owner")
 state=initial.duplicate(true)
 state.opening.erase("screen_brightness")
 var before_bad := state.duplicate(true)
 check(not provider.advance_outer(state,null,{}).is_empty() and state==before_bad,"missing brightness owner rejects atomically")
 state=initial.duplicate(true)
 state.opening.prologue_hud_live.erase("checker_scroll_owner")
 before_bad=state.duplicate(true)
 check(not provider.advance_outer(state,null,{}).is_empty() and state==before_bad,"missing prefix accumulator owner rejects atomically")
 var imported: Dictionary = provider.import_scroll({}, {"owner":"invented-prefix-accumulator","source_globals":["D_800B5BCC","D_800B5BD0"],"phase":"before_outer_scroll_update","u":113,"v":199})
 check(imported.status=="scroll_imported" and imported.hud.checker_scroll_u==113 and imported.hud.checker_scroll_v==199,"explicit owned scroll transport retains values")
 for value: int in [-1,256]:
  check(provider.import_scroll({}, {"owner":"x","source_globals":["D_800B5BCC","D_800B5BD0"],"phase":"before_outer_scroll_update","u":value,"v":0}).status=="unsupported","source u8 transport range enforced")
 for bad: Dictionary in [{},{"owner":"x","source_globals":["D_800B5BCC","D_800B5BD0"],"phase":"before_outer_scroll_update","u":{},"v":0},{"owner":"","source_globals":["D_800B5BCC","D_800B5BD0"],"phase":"before_outer_scroll_update","u":0,"v":0}]:
  check(provider.import_scroll({},bad).status=="unsupported","malformed/missing scroll refuses")
 var actor := FileAccess.get_file_as_bytes("/work/disc/files/CDROM/PROLODIR/O_01.NBN")
 var upload := Uploads.new()
 check(upload.preflight(actor,{}).diagnostic.contains("actual runtime owner"),"authentic O01 reaches missing owner guard")
 var scratch := PackedByteArray();scratch.resize(512)
 var vram := PackedByteArray();vram.resize(1024*512*2)
 var owned := {"scratch_owner":"test-owned","vram_owner":"test-vram","scratch_words":scratch,"vram_words":vram,"pending_gpu_effects":[]}
 var plan := upload.preflight(actor,owned)
 check(plan.status=="preflight_ok","actual actor plan")
 for index: int in range(1,17):
  check(plan.effects[index].stp_mode==2 and plan.effects[index].buffer_lock==1 and plan.effects[index].source_row==index-1,"native palette control retained")
 check(plan.effects[18].stp_mode==2 and plan.effects[18].source_row==0,"scratch palette native control retained")
 for malformed: Variant in [{},[],null,"19",19.0,false]:
  var bad := plan.duplicate(true);bad.effect_count=malformed
  var owner_before := owned.duplicate(true)
  var outcome := upload.execute(bad,actor,owned)
  check(outcome.status=="unsupported" and owned==owner_before,"malformed effect_count rejects without side effects")
 for key: String in ["scratch_owner","vram_owner","scratch_words","vram_words","pending_gpu_effects"]:
  var bad_owned := owned.duplicate(true);bad_owned.erase(key)
  var before := bad_owned.duplicate(true)
  check(upload.preflight(actor,bad_owned).status=="unsupported","authentic actor missing field guard")
  check(upload.execute(plan,actor,bad_owned).status=="unsupported" and bad_owned==before,"missing owner execute atomic")
 var forged := plan.duplicate(true);forged.effects[1].stp_mode=1
 forged.plan_sha256=JSON.stringify([forged.effects,forged.scratch_owner,forged.vram_owner,Uploads.ACTOR_SHA]).sha256_text()
 var before := owned.duplicate(true)
 check(upload.execute(forged,actor,owned).status=="unsupported" and owned==before,"repinned non-native mode rejects before commit")
 var executed := upload.execute(plan,actor,owned)
 check(executed.status=="effects_enqueued" and not executed.vram_ready and owned.pending_gpu_effects.size()==18,"valid original effects queue without false completion")
 check(owned.pending_gpu_effects[1].stp_mode==2 and owned.pending_gpu_effects[17].stp_mode==2,"STP mode survives actual enqueue")
 print("HUD V2 checks=%d failures=%d original_pixel_comparisons=%d mismatches=%d" % [checks,failures,4*2*4*3*1024,mismatch])
 quit(1 if failures else 0)
