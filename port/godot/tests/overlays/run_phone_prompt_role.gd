extends SceneTree
const Telephone = preload("res://presentation/telephone_overlay_presenter.gd")
var checks: int = 0
var failures: int = 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr(label)

func view() -> Dictionary:
	return {"schema":"tokimemo.phone-wait-marker-view.v1","source_function":"talk_text_step",
		"waiting_for_input":true,"k_disp_inc_result":1,"voice_mode":2,
		"speaker_voice_state":2,"marker_visible":true,"marker_native_y":72}

func render(presenter: Telephone, native: Dictionary) -> Image:
	return presenter.render_girl_phone_call_vn_scene(0,"accepted",[97,43,40,40,38,33,59,4,3],95,4,9,6,1,Vector2i(20,14),native)

func _initialize() -> void:
	var presenter := Telephone.new()
	check(presenter.render_girl_phone_call_vn_scene() == null and not presenter.error.is_empty(), "absent live phase is unsupported, not an invented ready prompt")
	var unsupported: Dictionary = view()
	unsupported.source_function = "xa_wait"
	check(not Telephone.phone_wait_marker_command(unsupported).get("error", "").is_empty(), "xa_wait is a separate source gate, never inferred as voiced talk_text_step")
	for field: String in view():
		var missing: Dictionary = view()
		missing.erase(field)
		check(not Telephone.phone_wait_marker_command(missing).get("error", "").is_empty(), "missing native projection " + field)
		for wrong: Variant in [null, {}, [], true, false, "foreign", -1, 999, 1.0]:
			if wrong is bool and view()[field] is bool: continue
			var malformed: Dictionary = view()
			malformed[field] = wrong
			check(not Telephone.phone_wait_marker_command(malformed).get("error", "").is_empty(), "malformed native projection " + field)
	var extra: Dictionary = view()
	extra["unused"] = true
	check(not Telephone.phone_wait_marker_command(extra).get("error", "").is_empty(), "unknown projection keys reject")
	for waiting: bool in [false,true]:
		for text: int in [0,1]:
			for mode: int in [0,1,2]:
				for speaker: int in [0,1,2]:
					for visible: bool in [false,true]:
						for y in range(72,80):
							var native: Dictionary = view()
							native.merge({"waiting_for_input":waiting,"k_disp_inc_result":text,"voice_mode":mode,"speaker_voice_state":speaker,"marker_visible":visible,"marker_native_y":y},true)
							var before: Dictionary = native.duplicate(true)
							var command: Dictionary = Telephone.phone_wait_marker_command(native)
							check(command.get("error") == "" and command.visible == (waiting and text == 1 and (speaker == 2 or mode == 0) and visible) and command.frame == 1 and command.position == Vector2i(276,120+y) and native == before,"source text/voice/phase/bob projection")
	check(presenter.open_telephone_preview("/work/.asset-cache/port/hud-production-v1/manifest.json","/work/.asset-cache/port/fonts/native-font-v2","/work/.asset-cache/port/date-preview-v1"),"open hash-pinned original assets: " + presenter.error)
	if not presenter.error.is_empty():
		quit(1)
		return
	var hidden: Dictionary = view()
	hidden.marker_visible = false
	var background: Image = render(presenter,hidden)
	check(background != null,"valid hidden marker scene")
	var marker_record: Dictionary = presenter._compositor.descriptor["frames"]["1:base"]
	presenter._compositor.descriptor["frames"].erase("1:base")
	check(render(presenter,view()) == null and not presenter.error.is_empty(), "missing original frame1 is unsupported without a glyph substitute")
	presenter._compositor.descriptor["frames"]["1:base"] = marker_record
	for y: int in [72,79]:
		var native: Dictionary = view()
		native.marker_native_y = y
		var before: Dictionary = native.duplicate(true)
		var actual: Image = render(presenter,native)
		check(actual != null and native == before,"drawing reads committed state without advancing bob")
		var marker := Image.create(320,240,false,Image.FORMAT_RGBA8)
		marker.fill(Color.TRANSPARENT)
		presenter._draw_actor(marker,1,Vector2i(116,y))
		var correct: bool = true
		for py in range(240):
			for px in range(320):
				var expected: Color = marker.get_pixel(px,py) if marker.get_pixel(px,py).a8 > 0 else background.get_pixel(px,py)
				if expected != actual.get_pixel(px,py): correct = false
		check(correct,"exact original actor2/frame1 at native bob endpoint")
	for key: String in ["k_disp_inc_result","speaker_voice_state"]:
		var incomplete: Dictionary = view()
		incomplete[key] = 0
		var actual: Image = render(presenter,incomplete)
		check(actual.get_data() == background.get_data(),"incomplete text/voice cannot display continuation marker: " + key)
	var slots: Array[Dictionary] = [{"day":16,"month_offset":0},{"day":23,"month_offset":0}]
	var calendar: Image = presenter.render_date_day_menu_screen(slots,0)
	var selection := Image.create(320,240,false,Image.FORMAT_RGBA8)
	selection.fill(Color.TRANSPARENT)
	presenter._draw_actor(selection,156,Vector2i(36-160,112-120))
	var unchanged: bool = true
	for py in range(240):
		for px in range(320):
			if selection.get_pixel(px,py).a8 > 0 and selection.get_pixel(px,py) != calendar.get_pixel(px,py): unchanged=false
	check(unchanged,"calendar selection cursor remains original actor0/frame156, independent from prompt role")
	print("PHONE_PROMPT_ROLE checks=%d failures=%d source_resources=true actual_dialogue_lifecycle=false" % [checks,failures])
	quit(0 if failures == 0 else 1)
