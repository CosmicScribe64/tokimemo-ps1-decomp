extends "res://tests/run_installed_first_scene.gd"
var observing_hold := false
func _start() -> void:
	var settings := root.get_node("ModernSettings")
	if OS.get_environment("TOKIMEMO_TEST_START_KEY") == "g":
		var mapped: Dictionary = settings.remaps.rebind("start", {"kind":"key", "code":KEY_G})
		check(settings.preferences.set_value("controls", "bindings", mapped.bindings), "custom physical Start binding saves")
	check(settings.preferences.set_value("accessibility", "text_speed", "slow"), "slow text preference saves")
	check(settings.preferences.set_value("accessibility", "auto_delay", 2.0), "reading delay preference saves")
	check(settings.preferences.set_value("accessibility", "font_scale", 1.3), "larger text preference saves")
	super._start()
func _process(delta: float) -> bool:
	if observing_hold: return false
	return super._process(delta)
func _finish(app: App) -> void:
	observing_hold = true; finishing = false; app._active = true
	var settings := root.get_node("ModernSettings")
	await physics_frame
	await process_frame
	var session = app.consumer.opening.session
	var tick: int = session.clock.logic_iterations
	var before: Dictionary = app.consumer.opening.state.projection()
	var rng_before: Dictionary = app.consumer.rng.checkpoint()
	var visible: int = settings.pacing.visible_count
	await create_timer(0.5).timeout
	check(session.clock.logic_iterations == tick, "slow reading hold advanced source session")
	check(app.consumer.opening.state.projection() == before and app.consumer.rng.checkpoint() == rng_before, "reading hold changed source state or RNG")
	check(settings.pacing.visible_count > visible, "slow glyphs did not advance during held simulation")
	check(app.failure.is_empty(), "render-only refresh failed: " + app.failure)
	settings.toggle_menu()
	visible = settings.pacing.visible_count
	await create_timer(0.25).timeout
	check(settings.pacing.visible_count == visible and session.clock.logic_iterations == tick, "settings pause advanced reading or simulation")
	settings.toggle_menu()
	var deadline := Time.get_ticks_msec() + 12000
	while session.clock.logic_iterations == tick and Time.get_ticks_msec() < deadline and app.failure.is_empty():
		await process_frame
	check(session.clock.logic_iterations > tick, "reading hold never released")
	check(app.failure.is_empty(), "source failed after reading hold: " + app.failure)
	if failed: return
	var text_manifest := "/out/settings-text-fixture.json"
	var file := FileAccess.open(text_manifest, FileAccess.WRITE)
	file.store_string(JSON.stringify({"schema": "tokimemo.text-translation-pack.v1", "language": "en", "source_payload_sha256": settings.PrologueText.PAYLOAD_SHA, "entries": {"0": {"speaker": "", "text": "I'm new at Kirameki High School."}}}))
	file.close()
	app._active = false
	settings.preferences.set_value("accessibility", "font_scale", 1.0)
	settings.preferences.set_value("accessibility", "text_speed", "normal")
	app.presenter.configure_accessibility(settings.preferences.settings.accessibility)
	var state = app.consumer.opening.state
	var native: Dictionary = app.presenter.render(state, app._draw_checker, app._draw_front, app.upload.batch_ready, app.last_rendered_image)
	check(native.error.is_empty(), "native same-checkpoint render succeeds")
	check(settings.select_text_pack(text_manifest).is_empty(), "custom text pack selects in live game")
	app.presenter.text_translations = settings.text_pack
	var translated: Dictionary = app.presenter.render(state, app._draw_checker, app._draw_front, app.upload.batch_ready, app.last_rendered_image)
	check(translated.error.is_empty(), "native translation render succeeds")
	var differences := 0
	var outside := 0
	for y: int in range(240):
		for x: int in range(320):
			if native.image.get_pixel(x,y) != translated.image.get_pixel(x,y):
				differences += 1
				if not Rect2i(27,166,266,52).has_point(Vector2i(x,y)): outside += 1
	check(differences > 0 and outside == 0, "translation changed pixels outside original text bounds")
	app._display.texture.update(translated.image)
	await RenderingServer.frame_post_draw
	check(root.get_texture().get_image().save_png("/out/native-style-english.png") == OK, "native-style translated dialogue screenshot saves")
	native.image.save_png("/out/native-checkpoint.png"); translated.image.save_png("/out/english-checkpoint.png")
	check(settings.select_text_pack("").is_empty(), "Japanese dialogue resets independently")
	app.presenter.text_translations = settings.text_pack
	var restored: Dictionary = app.presenter.render(state, app._draw_checker, app._draw_front, app.upload.batch_ready, app.last_rendered_image)
	check(restored.error.is_empty() and restored.image.get_data() == native.image.get_data(), "Japanese restore differs at identical checkpoint")
	app._display.texture.update(restored.image)
	await RenderingServer.frame_post_draw
	check(root.get_texture().get_image().save_png("/out/accessible-opening.png") == OK, "accessible opening screenshot saves")
	_status({"phase":"passed", "checks":checks, "source_tick_held":tick, "source_rng_unchanged":true, "glyphs_advanced_during_hold":true, "menu_paused_presentation":true, "eventual_resume":true})
	print("PASS: %d actual installed reading-hold checks" % checks)
	app.queue_free(); await create_timer(0.2).timeout; quit(0)
