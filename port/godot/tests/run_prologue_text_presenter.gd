extends SceneTree
## Original-pack rendering checks; this is not a normal opening route replay.
const Store = preload("res://services/prologue_text_store.gd")
const Work = preload("res://rules/prologue_text_work.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Presenter = preload("res://presentation/prologue_text_presenter.gd")
const Session = preload("res://core/game_session.gd")
var checks := 0
var failures := 0
func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr(label)
func _initialize() -> void:
	var text_root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	var font_root := OS.get_environment("PROLOGUE_FONT_ROOT")
	if text_root.is_empty() or font_root.is_empty():
		print("SKIP: explicit external original text and font inputs required")
		quit(0)
		return
	var store := Store.new()
	var presenter := Presenter.new()
	check(store.open(text_root,FileAccess.get_sha256(text_root.path_join("manifest.json"))).is_empty(), "strict actual text pack opens")
	check(presenter.open_font(font_root).is_empty(), "strict original glyph pack opens")
	if failures != 0:
		quit(1)
		return
	# Use the authenticated source palette independently of the rendered plan.
	var descriptor: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(font_root.path_join("font.json")))
	var shadow_word: int = descriptor.palettes[2].words[1]
	var sample: PackedByteArray = store.resolve(19,{"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0]}).speaker_raw.slice(0,2)
	check(shadow_word != 0 and (shadow_word & 0x7fff) != 0,"original nonblack shadow CLUT needs the source vertex modulation")
	for level: int in [0,64,128]:
		var plan: Dictionary = presenter._glyph_plan(sample,Vector2i(30,170),level)
		check(plan.error.is_empty() and not plan.pixels.is_empty() and plan.pixels[0].color.is_equal_approx(Color.BLACK),"source shadow vertex RGB zero modulates its palette at foreground brightness: "+str(level))
	# Invented profile selections use ordinary code units from the font repertoire.
	var profile := {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],
		"girls":[{"birthday":12|(31<<4)|(3<<9)}]}
	var ids: Array = range(43)
	ids.append_array([100,101])
	for identity: int in ids:
		var view: Dictionary = store.resolve(identity,profile)
		check(view.error.is_empty() and presenter.preflight(view).is_empty(), "original line font coverage: " + str(identity))
		var state := State.new()
		Work.reset(state)
		check(Work.begin_line(state,view).is_empty(), "numeric source text work begins: " + str(identity))
		var complete := 0
		for frame in range(1,12001):
			state.frame_count = frame
			var result := Session.FrameResult.new()
			Work.advance(state,result)
			if not result.error.is_empty():
				break
			complete = state.opening.prologue_text.k_disp_inc_result
			if complete == 1:
				break
		check(complete == 1, "source reveal and scroll complete: " + str(identity))
		var before: Dictionary = state.projection()
		var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
		target.fill(Color.TRANSPARENT)
		var rendered: Dictionary = presenter.render_into(target,view,state.opening.prologue_text_work,128)
		check(rendered.error.is_empty() and rendered.get("bounding_box_violations") == 0 and rendered.get("pixels",0) > 0, "foreground and shadow stay in message bounds: " + str(identity))
		var duplicate := Image.create(320,240,false,Image.FORMAT_RGBA8)
		duplicate.fill(Color.TRANSPARENT)
		presenter.render_into(duplicate,view,state.opening.prologue_text_work,128)
		check(duplicate.get_data() == target.get_data() and before == state.projection(), "render frequency cannot advance source text: " + str(identity))
		if identity in [0,19,100]:
			check(target.save_png("/out/prologue-text-" + str(identity) + ".png") == OK, "source text image saved")
		if identity == 0:
			var invalid: Dictionary = state.opening.prologue_text_work.duplicate(true)
			invalid.entries[0].x = 500
			var untouched := Image.create(320,240,false,Image.FORMAT_RGBA8)
			untouched.fill(Color.TRANSPARENT)
			var original: PackedByteArray = untouched.get_data()
			check(not presenter.render_into(untouched,view,invalid,128).error.is_empty() and untouched.get_data() == original, "overflow rejects before touching target")
	print("PASS: %d original prologue text rendering checks" % checks if failures == 0 else "FAIL: %d/%d original prologue text checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
