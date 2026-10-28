extends SceneTree
const Cached=preload("res://presentation/prologue_text_presenter.gd")
const Reference=preload("res://tests/fixtures/prologue_text_presenter_uncached.gd")
const Store=preload("res://services/prologue_text_store.gd")
const Work=preload("res://rules/prologue_text_work.gd")
const State=preload("res://rules/new_game_route_state.gd")
const Session=preload("res://core/game_session.gd")
var checks=0
var failures=0
var successful_renders=0
var rejected_renders=0
var cached=Cached.new()
var reference=Reference.new()
var pair=PackedByteArray()
func check(ok:bool,label:String)->void:
 checks+=1
 if not ok:failures+=1;printerr("FAIL: "+label)
func compare(view:Dictionary,work:Dictionary,brightness:int,label:String)->void:
 var a=Image.create(320,240,false,Image.FORMAT_RGBA8);a.fill(Color(0.2,0.3,0.4,1))
 var b=a.duplicate()
 var original_pixels=a.get_data()
 var before=work.duplicate(true)
 var ca=cached.render_into(a,view,work,brightness)
 var rb=reference.render_into(b,view,work,brightness)
 if ca.error.is_empty():successful_renders+=1
 else:rejected_renders+=1
 check(ca==rb and a.get_data()==b.get_data() and work==before and (ca.error.is_empty() or a.get_data()==original_pixels),label)
func _initialize()->void:
 var font=OS.get_environment("PROLOGUE_FONT_ROOT")
 var store=Store.new()
 check(store.open(OS.get_environment("PROLOGUE_TEXT_ROOT"),"5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc").is_empty(),"actual text opens")
 check(cached.open_font(font).is_empty() and reference.open_font(font).is_empty(),"both original fonts open")
 var profile={"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],"girls":[{"birthday":12|(31<<4)|(3<<9)}]}
 var ids:Array=range(43);ids.append_array([100,101])
 var bench_view:Dictionary;var bench_work:Dictionary
 var scroll_cases=0
 for identity:int in ids:
  var view=store.resolve(identity,profile)
  var state=State.new();Work.reset(state)
  check(Work.begin_line(state,view).is_empty(),"source work starts")
  var sampled_scroll=false
  for frame in range(1,12001):
   state.frame_count=frame
   var result=Session.FrameResult.new();Work.advance(state,result)
   if not result.error.is_empty():break
   var work:Dictionary=state.opening.prologue_text_work
   if not sampled_scroll and work.scroll_state==129:
    compare(view,work,128,"actual source scroll frame equal");sampled_scroll=true;scroll_cases+=1
   if state.opening.prologue_text.k_disp_inc_result==1:break
  var work:Dictionary=state.opening.prologue_text_work
  compare(view,work,128,"complete source frame equal")
  compare(view,work,128,"repeated cached source frame equal")
  if identity in [0,19,100]:
   for level:int in [0,64]:compare(view,work,level,"brightness source frame equal")
  if identity==0:bench_view=view;bench_work=work.duplicate(true);pair=view.speaker_raw.slice(0,2)
 var repeated=PackedByteArray()
 for i in 4:repeated.append_array(pair)
 var synthetic={"error":"","line_id":999,"speaker_raw":pair+pair,"rows":[{"raw":repeated}]}
 var layout={"line_id":999,"pos":6,"total":6,"entries":[{"row":-1,"glyph_count":2,"x":-128,"y":50,"active":true,"attribute":0},{"row":0,"glyph_count":4,"x":-128,"y":66,"active":true,"attribute":0}]}
 for level:int in [0,64,128]:
  compare(synthetic,layout,level,"repeated glyph absolute layout equality")
  compare(synthetic,layout,level,"repeated glyph hot equality")
 var shifted=layout.duplicate(true);shifted.entries[1].x+=1;shifted.entries[1].y-=1
 compare(synthetic,shifted,128,"changed current layout has distinct glyph keys")
 var overlap=layout.duplicate(true);overlap.entries[1].y=50
 compare(synthetic,overlap,128,"overlap native OT order preserved")
 for defect:String in ["glyph_count","attribute","layout","row","active_type"]:
  var bad=bench_work.duplicate(true)
  match defect:
   "glyph_count":bad.entries[0].glyph_count+=1
   "attribute":bad.entries[0].attribute=1
   "layout":bad.entries[0].x=500
   "row":bad.entries[0].row=99
   "active_type":bad.entries[0].active={}
  compare(bench_view,bad,128,"malformed current work on hot view equal")
 var bad_view=bench_view.duplicate(true);bad_view.rows[0].raw=PackedByteArray([1])
 compare(bad_view,bench_work,128,"malformed row rejects cached lookup")
 var plan=cached._glyph_plan(pair,Vector2i(30,170),128)
 var original_color=plan.pixels[0].color
 plan.pixels[0].color=Color.MAGENTA
 check(cached._glyph_plan(pair,Vector2i(30,170),128).pixels[0].color==original_color,"returned plan deep copy")
 var hit=cached._glyph_plan(pair,Vector2i(30,170),128)
 hit.pixels[0].color=Color.GREEN
 check(cached._glyph_plan(pair,Vector2i(30,170),128).pixels[0].color==original_color,"hot plan returned deep copy")
 for index in range(280):cached._glyph_plan(pair,Vector2i(30+index,170),128)
 check(cached._glyph_plans.size()<=256,"glyph cache bound")
 for index in range(70):
  var raw=PackedByteArray()
  for i in index+1:raw.append_array(pair)
  check(cached.preflight({"error":"","speaker_raw":pair,"rows":[{"raw":raw}]}).is_empty(),"membership cache unique source view")
 check(cached._verified_views.size()<=64,"membership cache bound")
 check(not cached.open_font("/missing-font").is_empty() and cached._glyph_plans.is_empty() and cached._verified_views.is_empty(),"failed font reopen clears caches")
 var target=Image.create(320,240,false,Image.FORMAT_RGBA8)
 check(not cached.render_into(target,bench_view,bench_work,128).error.is_empty(),"failed font reopen cannot reuse plan")
 check(cached.open_font(font).is_empty() and cached._glyph_plans.is_empty() and cached._verified_views.is_empty(),"valid font reopen resets source scope")
 compare(bench_view,bench_work,128,"reopened source frame equal")
 var runs=12
 var start=Time.get_ticks_usec()
 for i in runs:reference.render_into(target,bench_view,bench_work,128)
 var old_us=Time.get_ticks_usec()-start
 start=Time.get_ticks_usec()
 for i in runs:cached.render_into(target,bench_view,bench_work,128)
 var new_us=Time.get_ticks_usec()-start
 print(JSON.stringify({"checks":checks,"failures":failures,"source_lines":45,"successful_render_comparisons":successful_renders,"rejected_render_comparisons":rejected_renders,"scroll_cases":scroll_cases,"benchmark_runs":runs,"reference_us":old_us,"cached_us":new_us,"speedup":float(old_us)/maxi(1,new_us)}))
 quit(0 if failures==0 else 1)
