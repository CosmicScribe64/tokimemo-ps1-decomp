extends SceneTree
const Provider = preload("res://presentation/prologue_hud_source_provider.gd")
const PIN := "c930a17327e5ad0bdf3b26ab8c20287775b7fc795df240b5f071ed28c4f25f23"
var checks := 0
var failures := 0
func check(ok: bool, label: String) -> void:
 checks += 1
 if not ok:
  failures+=1
  printerr(label)
func transport() -> Dictionary:
 return {"owner":"invented-owned-prefix","source_globals":["D_800B5BCC","D_800B5BD0"],"phase":"before_outer_scroll_update","u":255,"v":0}
func fixture() -> Dictionary:
 return {"game_mode":21,"mode_stage":0,"mode_step":0,"frame_count":3,"player_flags":PackedByteArray([0,0]),"pad_held_triangle":false,"opening":{"screen_brightness":10,"prologue_hud_live":{"checker_scroll_owner":"invented-owned-prefix","checker_scroll_u":17,"checker_scroll_v":247,"checker_scroll_phase":"after_hud_screen_frame_draw_scroll_update","year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"screen_brightness":10,"title_timer":-64,"checker_fade":255}}}
func _initialize() -> void:
 var provider := Provider.new()
 var error := provider.open("/work",PIN)
 check(error.is_empty(),"open exact V2 resources: "+error)
 if not error.is_empty():
  quit(1)
  return
 var variants := {
  "source_globals":[{},null,"D_800B5BCC",0,0.0,false,[],["D_800B5BCC"],["D_800B5BCC",{}],[null,"D_800B5BD0"],[1,"D_800B5BD0"],["D_800B5BCC",true],["D_800B5BD0","D_800B5BCC"],["D_800B5BCC","D_800B5BD0","extra"],PackedStringArray(["D_800B5BCC","D_800B5BD0"])],
  "phase":[{},null,0,0.0,false,[],["before_outer_scroll_update"],"invented-phase"],
  "owner":[{},null,0,false,[],""],
  "u":[{},null,"0",0.0,false,[],-1,256],
  "v":[{},null,"0",0.0,false,[],-1,256]}
 for field: String in variants:
  for malformed: Variant in variants[field]:
   var live := {"preserved":{"marker":[1,2,3]}}
   var live_before := live.duplicate(true)
   var bad := transport();bad[field]=malformed
   var before := bad.duplicate(true)
   var result := provider.import_scroll(live,bad)
   check(result.get("status")=="unsupported" and typeof(result.get("diagnostic"))==TYPE_STRING and not result.get("diagnostic","").is_empty(),"malformed transport structured rejection: "+field)
   check(live==live_before and bad==before,"transport failure is pure: "+field)
 var live := {"preserved":{"marker":[1,2,3]}}
 var positive := provider.import_scroll(live,transport())
 check(positive.get("status")=="scroll_imported" and positive.hud.checker_scroll_u==255 and positive.hud.checker_scroll_v==0,"valid byte transport accepted after negatives")
 positive.hud.preserved.marker[0]=99
 check(live.preserved.marker[0]==1,"positive import deep copy preserves live input")
 var projection_variants := {
  "checker_scroll_phase":[{},null,0,false,[],["after_hud_screen_frame_draw_scroll_update"],"unknown"],
  "checker_scroll_owner":[{},null,0,false,[],""],
  "checker_scroll_u":[{},null,0.0,false,[],-1,256],
  "checker_scroll_v":[{},null,0.0,false,[],-1,256],
  "month":[{},null,"4",0.0,false,[]],
  "title_visible":[{},null,0,[]],
  "date_visible":[{},null,0,[]],
  "message_visible":[{},null,0,[]],
  "checker_clut_fade":[{},null,0.0,false,[]],
  "title_badge":[{},null,0,false,[],{"visible":{}},{"visible":true,"offset":{},"text_pos":Vector2i.ZERO},{"visible":true,"offset":Vector2i.ZERO,"text_pos":[]}]
 }
 for field: String in projection_variants:
  for malformed: Variant in projection_variants[field]:
   var state := fixture();state.opening.prologue_hud_live[field]=malformed
   var before := state.duplicate(true)
   var target := Image.create(320,240,false,Image.FORMAT_RGBA8);target.fill(Color8(8,16,24,255))
   var pixels_before := target.get_data()
   var result := {}
   check(not provider.scene_reset(state,result).is_empty(),"reset rejects malformed HUD projection: "+field)
   check(not provider.advance_outer(state,null,result).is_empty(),"outer rejects malformed HUD projection: "+field)
   check(not provider.draw_checker(target,state).is_empty() and not provider.draw_front(target,state).is_empty(),"draws reject malformed HUD projection: "+field)
   check(state==before and result.is_empty() and target.get_data()==pixels_before,"projection failures preserve state/result/target: "+field)
 for malformed: Variant in [{},null,0,[]]:
  var state := fixture();state.pad_held_triangle=malformed
  var before := state.duplicate(true)
  check(not provider.advance_outer(state,null,{}).is_empty() and state==before,"triangle observation type rejected before mutation")
 var state := fixture()
 check(provider.advance_outer(state,null,{}).is_empty(),"valid outer accepted after malformed projections")
 print("HUD V3 schema checks=%d failures=%d"%[checks,failures])
 quit(1 if failures else 0)
