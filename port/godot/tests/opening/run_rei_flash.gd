extends SceneTree
const Presenter = preload("res://presentation/first_arrival_scene_presenter.gd")
const State = preload("res://rules/new_game_route_state.gd")
var failures := 0
func check(ok: bool, label: String) -> void:
	if not ok: failures += 1; printerr(label)
func _initialize() -> void:
	var state := State.new(); state.frame_count = 9
	state.opening.rei_banner_primitive = {"kind":"flash","level":15,"frame_count":9}
	var image := Image.create(320,240,false,Image.FORMAT_RGBA8); image.fill(Color8(40,80,120))
	var lower := image.get_region(Rect2i(0,160,320,80)).get_data()
	check(Presenter._paint_rei_flash(image,state).is_empty(),"Committed source flash rejected")
	# RGB5 additive level:15+dither(-4) =>1;15+dither(+1)=>2.
	check(image.get_pixel(0,0) == Color8(49,90,132),"PS1 dither/additive RGB5 first pixel differs")
	check(image.get_pixel(3,0) == Color8(57,99,140),"PS1 dither phase differs")
	check(image.get_region(Rect2i(0,160,320,80)).get_data() == lower,"Flash changed native HUD/message region")
	state.opening.rei_banner_primitive.frame_count = 8
	var unchanged := image.get_data()
	check(not Presenter._paint_rei_flash(image,state).is_empty() and image.get_data() == unchanged,"Stale source flash mutated pixels")
	state.opening.rei_banner_primitive = {"kind":"white","level":255,"frame_count":9}
	check(Presenter._paint_rei_flash(image,state).is_empty() and image.get_data() == unchanged,"White backing layer incorrectly covered already composed actors")
	if failures == 0: print("PASS: source Rei RGB5 additive dither, HUD exclusion and frame ownership")
	quit(1 if failures else 0)
