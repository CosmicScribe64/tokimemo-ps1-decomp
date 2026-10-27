extends SceneTree
## Injected renderer boundary and invented HUD callbacks. No app/scene capture.
const MediaStore = preload("res://services/first_arrival_media_store.gd")
const Bridge = preload("res://services/first_arrival_media_bridge.gd")
const TextStore = preload("res://services/prologue_text_store.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Presenter = preload("res://presentation/first_arrival_scene_presenter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Work = preload("res://rules/prologue_text_work.gd")
var checks := 0
var failures := 0
func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ",label)
func checker(target: Image, _state: State) -> String:
	target.fill(Color.MAGENTA)
	return ""
func front(target: Image, _state: State) -> String:
	target.set_pixel(300,200,Color.GREEN)
	return ""
func ready_fixture(_batch: String) -> String:
	return "" # Invented positive predicate; not a VRAM receipt or readiness proof.
func _initialize() -> void:
	var media := MediaStore.new()
	check(media.open(OS.get_environment("FIRST_ARRIVAL_MEDIA_ROOT"),OS.get_environment("FIRST_ARRIVAL_MEDIA_SHA")).is_empty(),"actual native media pack opens")
	var bridge := Bridge.new()
	check(bridge.prepare(media).is_empty(),"original media prepares")
	var text := TextStore.new()
	check(text.open(OS.get_environment("PROLOGUE_TEXT_ROOT"),"5e9c0b09e1afca9a62f6441c78ac0731e8da9bae8af65a655f578322af4f99dc").is_empty(),"original text prepares")
	var painter := Painter.new()
	check(painter.open_font(OS.get_environment("PROLOGUE_FONT_ROOT")).is_empty(),"original font prepares")
	if failures != 0:
		quit(1)
		return
	var state := State.new()
	state.game_mode = 0x15
	state.mode_step = 1
	state.rule_state = {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0]}
	var reset := Session.FrameResult.new()
	bridge.scene_reset(state,reset)
	state.opening.screen_brightness = 128
	state.opening.media.decoded["1"] = {"buffer":1,"resource_id":MediaStore.BG_ID,"resource_sha256":MediaStore.BG_TRANSFER_SHA}
	var presenter := Presenter.new()
	check(presenter.prepare(bridge,text,painter,state).is_empty(),"pure presenter prepares against actual text view")
	check(not presenter.render(state,Callable(),front,Callable()).error.is_empty(),"missing genuine HUD callback fails instead of fallback")
	var before := state.projection()
	var result := presenter.render(state,checker,front,Callable())
	check(result.error.is_empty() and state.projection() == before,"pre-text layered rendering cannot advance source state")
	if not result.error.is_empty():
		quit(1)
		return
	var first: Image = result.image
	var expected := Image.create(320,240,false,Image.FORMAT_RGBA8)
	expected.fill(Color.MAGENTA)
	expected.blend_rect(media.background_image(),Rect2i(0,0,320,160),Vector2i.ZERO)
	expected.set_pixel(300,200,Color.GREEN)
	check(first.get_data() == expected.get_data(),"checker precedes source background and foreground follows")
	state.opening.screen_brightness = 0
	check(presenter.render(state,checker,front,Callable()).image.get_data() == first.get_data(),"source BGbrightness128 is independent of UI brightness")
	var mutating: Callable = func(_target: Image, view: State) -> String:
		view.frame_count += 1
		return ""
	before = state.projection()
	check(not presenter.render(state,mutating,front,Callable()).error.is_empty() and state.projection() == before,"HUD mutation of cloned render state is rejected without changing simulation")
	state.opening.arrival_media.actor_initialized = true
	state.opening.actor96_draw_flags = 0x24
	state.opening.arrival_media.upload_batch_key = "1".repeat(64)
	check(not presenter.render(state,checker,front,Callable()).error.is_empty(),"source cache never substitutes for missing texture/CLUT provider")
	state.opening.screen_brightness = 128
	Work.reset(state)
	check(Work.begin_line(state,text.resolve(0,state.rule_state)).is_empty(),"original first line creates numeric work")
	var text_state: Dictionary = state.projection()
	var rendered := presenter.render(state,checker,front,ready_fixture)
	check(rendered.error.is_empty() and state.projection() == text_state,"injected positive callback renders committed original body without state mutation")
	state.opening.arrival_media.back_clear = false
	check(not presenter.render(state,checker,front,ready_fixture).error.is_empty(),"disabled clear requires the actual prior committed framebuffer")
	check(presenter.render(state,checker,front,ready_fixture,first).error.is_empty(),"supplied prior framebuffer is accepted without modifying it")
	state.opening.actor96_draw_flags = 0xa4
	check(not presenter.render(state,checker,front,ready_fixture,first).error.is_empty(),"visible actor transition requires a real source renderer")
	print("PASS: %d bounded first-arrival compositor checks" % checks if failures == 0 else "FAIL: %d/%d compositor checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
