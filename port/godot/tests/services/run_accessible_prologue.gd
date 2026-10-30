extends "res://tests/services/settings_test_tree.gd"
const Store = preload("res://services/prologue_text_store.gd")
const Work = preload("res://rules/prologue_text_work.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Session = preload("res://core/game_session.gd")
func _initialize() -> void:
	var pack := OS.get_environment("SETTINGS_ORIGINAL_PACK")
	check(not pack.is_empty())
	var text := Store.new(); var painter := Painter.new()
	var source := pack.path_join("packs/prologue_text")
	check(text.open(source, FileAccess.get_sha256(source.path_join("manifest.json"))).is_empty())
	check(painter.open_font(pack.path_join(".asset-cache/port/fonts/native-font-v2")).is_empty())
	var profile := {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],"girls":[{"birthday":12|(31<<4)|(3<<9)}]}
	var count := 0
	for identity: int in range(12):
		var view: Dictionary = text.resolve(identity, profile)
		var state := State.new(); Work.reset(state)
		check(Work.begin_line(state, view).is_empty())
		for frame: int in range(1, 12001):
			state.frame_count = frame
			var result := Session.FrameResult.new(); Work.advance(state, result)
			check(result.error.is_empty())
			if state.opening.prologue_text.k_disp_inc_result == 1: break
		var before: Dictionary = state.projection()
		# English native layout keeps the original speaker where gnsx puts it,
		# with the original inset from the window's left border: the speaker's
		# columns (window left edge to its last glyph and shadow) on row 0,
		# native y 166..185, are pixel-identical to the original. The native
		# first body row shares that row further right, so it is excluded.
		var speaker_layout: Array = painter.translation_layout({"speaker":"", "text":"Hello."}, view.speaker_raw)
		if not view.speaker_raw.is_empty():
			check(speaker_layout.size() >= 2 and speaker_layout[0][0].x == Painter.ORIGIN.x + Work.speaker_x_for(view.speaker_raw.size()) and speaker_layout[0][0].x > Painter.TEXT_BOUNDS.position.x + 1)
			var original_image := Image.create(320,240,false,Image.FORMAT_RGBA8); original_image.fill(Color(0.2,0.3,0.4))
			var english_image := Image.create(320,240,false,Image.FORMAT_RGBA8); english_image.fill(Color(0.2,0.3,0.4))
			check(str(painter.render_into(original_image,view,state.opening.prologue_text_work,128).get("error","")).is_empty())
			check(str(painter.render_translation(english_image,view,state.opening.prologue_text_work,128,{"speaker":"", "text":"Hello."}).get("error","")).is_empty())
			var speaker_end: int = Painter.ORIGIN.x + Work.speaker_x_for(view.speaker_raw.size()) + (view.speaker_raw.size() / 2 - 1) * Painter.ADVANCE + 13
			var band := Rect2i(Painter.TEXT_BOUNDS.position.x, 166, speaker_end - Painter.TEXT_BOUNDS.position.x, 20)
			check(original_image.get_region(band).get_data() == english_image.get_region(band).get_data())
		if identity == 0:
			var translated := {"speaker":"", "text":"I'm new at Kirameki High School."}
			var layout: Array = painter.translation_layout(translated,view.speaker_raw)
			check(layout.size() <= 3 and layout.size() >= 2)
			check(layout[1][1].x - layout[1][0].x < 14)
			check(layout[1][0].x + painter._latin_metrics(layout[1][0].pair).x == Painter.TRANSLATED_LEFT)
			check(layout[0][0].x == Painter.ORIGIN.x + Work.speaker_x_for(view.speaker_raw.size()))
			check(painter.translation_layout({"speaker":"","text":"Wide words ".repeat(100)},view.speaker_raw).is_empty())
			check(not Painter.translation_supported({"speaker":"","text":"Unsupported\nnewline"}))
			check(not Painter.translation_supported({"speaker":"","text":"   "}))
			var target := Image.create(320,240,false,Image.FORMAT_RGBA8)
			target.fill(Color(0.2,0.3,0.4))
			var pristine := target.get_data()
			var rejected := painter.render_translation(target,view,state.opening.prologue_text_work,128,{"speaker":"","text":"Wide words ".repeat(100)})
			check(not rejected.error.is_empty() and target.get_data() == pristine)
			var drawn := painter.render_translation(target,view,state.opening.prologue_text_work,128,translated)
			check(drawn.error.is_empty() and drawn.bounding_box_violations == 0)
			check(before == state.projection())
		for scale: float in [1.1, 1.3, 1.5]:
			var target := Image.create(320, 240, false, Image.FORMAT_RGBA8); target.fill(Color(0.2, 0.3, 0.4))
			var result: Dictionary = painter.render_accessible(target, view, state.opening.prologue_text_work, 128, scale, 0.8)
			check(result.error.is_empty()); check(result.bounding_box_violations == 0)
			check(before == state.projection()); count += 1
			if identity == 0 and scale == 1.5: target.save_png("/work/.asset-cache/accessibility-text.png")
	print("Accessible opening: %d original-font layouts, unchanged source state" % count)
	finish()
