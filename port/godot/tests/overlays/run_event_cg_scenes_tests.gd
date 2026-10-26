extends SceneTree
## T-29018 Lane 8: Comprehensive Test Suite for EVENT.EXN Per-Girl Special CG & Ride Movie Scenes.
## Verifies:
## 1. Date spot state initialization across 21 spots (0..20) and 13 characters + Yoshio.
## 2. Special CG condition evaluation and scene_flags[0..0x4A] unlocking for all 13 girls.
## 3. Amusement Park Ride Movie dispatch and PS1 MDEC streaming LBAs for all 5 rides.
## 4. Dialogue choice branching, affection/tokimeki/syoushin deltas, and stage progression.
## 5. Authentic 320x240 PS1 presentation rendering with zero glyph bounding-box violations.
## 6. Saving 4 visual proof captures into captures/.

const Overlay = preload("res://overlays/event_cg_scenes_overlay.gd")
const Presenter = preload("res://presentation/event_cg_scenes_presenter.gd")

var failures: Array[String] = []

func check(ok: bool, message: String) -> void:
	if not ok:
		failures.append(message)
		print("FAIL: " + message)
	else:
		print("PASS: " + message)

func _initialize() -> void:
	print("==================================================================")
	print("Starting T-29018 Event CG Scenes Overlays Comprehensive Test Suite")
	print("==================================================================")

	# 1. State Initialization & Spot/Girl mappings
	_test_date_spots_and_girls_init()

	# 2. Special CG Scene Flags & Route Evaluation (All 13 Girls + Yoshio)
	_test_special_cg_scene_flags_dispatch()

	# 3. Amusement Park Ride Movies (5 Rides & MDEC LBAs)
	_test_amusement_park_ride_movies()

	# 4. Dialogue Choices & Relationship Stats
	_test_dialogue_choices_and_relationship_stats()

	# 5. Authentic PS1 Presentation Rendering & Capture Generation
	_test_presentation_rendering_and_captures()

	print("==================================================================")
	if failures.is_empty():
		print("ALL T-29018 EVENT CG SCENES OVERLAY TESTS PASSED SUCCESSFULLY!")
		print("==================================================================")
		quit(0)
	else:
		print("TOTAL FAILURES: %d" % failures.size())
		for f in failures:
			print(" - " + f)
		print("==================================================================")
		quit(1)

# ==============================================================================
# 1. State Initialization Tests
# ==============================================================================

func _test_date_spots_and_girls_init() -> void:
	print("\n--- Testing Date Spots & Girls State Initialization ---")
	var ovl := Overlay.new()
	check(ovl.stage == Overlay.STAGE_ARRIVAL, "Initial stage is STAGE_ARRIVAL")
	check(ovl.current_girl == Overlay.GIRL_SHIORI, "Default girl is Shiori (0)")
	check(ovl.spot_place == Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, "Default spot is Kirameki Central Park (1)")
	check(ovl.scene_flags.size() == 0x80, "scene_flags array size is 0x80 bytes")

	# Spot name helper
	check(Overlay.get_spot_name(Overlay.SPOT_KIRAMEKI_CENTRAL_PARK) == "きらめき中央公園", "Spot 1 is きらめき中央公園")
	check(Overlay.get_spot_name(Overlay.SPOT_AMUSEMENT_PARK) == "遊園地", "Spot 12 is 遊園地")
	check(Overlay.get_spot_name(Overlay.SPOT_KARAOKE) == "カラオケBOX", "Spot 11 is カラオケBOX")

	# Girl name helper
	check(Overlay.get_girl_short_name(Overlay.GIRL_SHIORI) == "詩織", "Girl 0 is 詩織")
	check(Overlay.get_girl_short_name(Overlay.GIRL_AYAKO) == "彩子", "Girl 3 is 彩子")
	check(Overlay.get_girl_short_name(Overlay.GIRL_MIHARU) == "見晴", "Girl 11 is 見晴")
	check(Overlay.get_girl_short_name(Overlay.GIRL_YOSHIO) == "好雄", "Girl 13 is 好雄")

# ==============================================================================
# 2. Special CG Scene Flags & Route Evaluation (All 13 Girls + Yoshio)
# ==============================================================================

func _test_special_cg_scene_flags_dispatch() -> void:
	print("\n--- Testing Special CG Scene Flags & Scene Dispatch ---")
	var ovl := Overlay.new()

	# Shiori (Girl 0): 4 CG scenes
	# Spring Central Park -> Scene 4 (scene_flags[0])
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 4, 96)
	var trig_shiori_sp: bool = ovl.evaluate_date_spot_special_scene()
	check(trig_shiori_sp, "Shiori spring park triggers special scene")
	check(ovl.active_scene_id == 4, "Shiori spring scene is Scene 4")
	check(ovl.scene_flags[0x00] == 1, "Shiori scene_flags[0] is set to 1")

	# Shiori Summer Pool -> Scene 6 (scene_flags[1])
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_POOL, 7, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 6 and ovl.scene_flags[0x01] == 1, "Shiori summer pool unlocks scene_flags[1]")

	# Shiori Autumn Park -> Scene 3 (scene_flags[2])
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 10, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x02] == 1, "Shiori autumn park unlocks scene_flags[2]")

	# Shiori Winter Amusement Park -> Scene 5 (scene_flags[3])
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_AMUSEMENT_PARK, 12, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 5 and ovl.scene_flags[0x03] == 1, "Shiori winter amusement park unlocks scene_flags[3]")

	# Mio (Girl 1): scenes 2..5 (scene_flags 0x7..0xA)
	ovl.init_date_spot(Overlay.GIRL_MIO, Overlay.SPOT_AMUSEMENT_PARK, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x07] == 1, "Mio spring amusement park unlocks scene_flags[0x7]")

	ovl.init_date_spot(Overlay.GIRL_MIO, Overlay.SPOT_SEA, 8, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x08] == 1, "Mio summer sea unlocks scene_flags[0x8]")

	ovl.init_date_spot(Overlay.GIRL_MIO, Overlay.SPOT_LIBRARY, 10, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 4 and ovl.scene_flags[0x09] == 1, "Mio autumn library unlocks scene_flags[0x9]")

	ovl.init_date_spot(Overlay.GIRL_MIO, Overlay.SPOT_MOVIE_THEATER, 1, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 5 and ovl.scene_flags[0x0A] == 1, "Mio winter movie theater unlocks scene_flags[0xA]")

	# Yuina (Girl 2): scenes 2..5 (scene_flags 0xD..0x10)
	ovl.init_date_spot(Overlay.GIRL_YUINA, Overlay.SPOT_ZOO, 5, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x0D] == 1, "Yuina spring zoo unlocks scene_flags[0xD]")

	ovl.init_date_spot(Overlay.GIRL_YUINA, Overlay.SPOT_BOWLING_ALLEY, 7, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x0E] == 1, "Yuina summer bowling unlocks scene_flags[0xE]")

	ovl.init_date_spot(Overlay.GIRL_YUINA, Overlay.SPOT_GAME_CENTER, 10, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 4 and ovl.scene_flags[0x0F] == 1, "Yuina autumn game center unlocks scene_flags[0xF]")

	ovl.init_date_spot(Overlay.GIRL_YUINA, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 2, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 5 and ovl.scene_flags[0x10] == 1, "Yuina winter central park unlocks scene_flags[0x10]")

	# Ayako (Girl 3): scenes 2..5 (scene_flags 0x13..0x16)
	ovl.init_date_spot(Overlay.GIRL_AYAKO, Overlay.SPOT_KARAOKE, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x13] == 1, "Ayako spring karaoke unlocks scene_flags[0x13]")

	ovl.init_date_spot(Overlay.GIRL_AYAKO, Overlay.SPOT_ART_MUSEUM, 8, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x14] == 1, "Ayako summer art museum unlocks scene_flags[0x14]")

	ovl.init_date_spot(Overlay.GIRL_AYAKO, Overlay.SPOT_ART_MUSEUM, 10, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 4 and ovl.scene_flags[0x15] == 1, "Ayako autumn art museum unlocks scene_flags[0x15]")

	ovl.init_date_spot(Overlay.GIRL_AYAKO, Overlay.SPOT_PLANETARIUM, 12, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 5 and ovl.scene_flags[0x16] == 1, "Ayako winter planetarium unlocks scene_flags[0x16]")

	# Saki (Girl 4): scenes 2..5 (scene_flags 0x19..0x1C)
	ovl.init_date_spot(Overlay.GIRL_SAKI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x19] == 1, "Saki spring central park unlocks scene_flags[0x19]")

	# Yukari (Girl 5): scenes 3..6 (scene_flags 0x20..0x23)
	ovl.init_date_spot(Overlay.GIRL_YUKARI, Overlay.SPOT_BOTANICAL_GARDEN, 5, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x20] == 1, "Yukari spring botanical garden unlocks scene_flags[0x20]")

	# Nozomi (Girl 6): scenes 2..5 (scene_flags 0x26..0x29)
	ovl.init_date_spot(Overlay.GIRL_NOZOMI, Overlay.SPOT_ART_MUSEUM, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x26] == 1, "Nozomi spring art museum unlocks scene_flags[0x26]")

	# Mira (Girl 7): scenes 2..6 (scene_flags 0x2C..0x30)
	ovl.init_date_spot(Overlay.GIRL_MIRA, Overlay.SPOT_SHOPPING_STREET, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x2C] == 1, "Mira spring shopping street unlocks scene_flags[0x2C]")

	# Yuko (Girl 8): scenes 2..6 (scene_flags 0x33..0x37)
	ovl.init_date_spot(Overlay.GIRL_YUKO, Overlay.SPOT_GAME_CENTER, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 2 and ovl.scene_flags[0x33] == 1, "Yuko spring game center unlocks scene_flags[0x33]")

	ovl.init_date_spot(Overlay.GIRL_YUKO, Overlay.SPOT_LIBRARY, 12, 96, 8)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 6 and ovl.scene_flags[0x37] == 1, "Yuko Dec 8 library date unlocks scene_flags[0x37]")

	# Megumi (Girl 9): scenes 3..6 (scene_flags 0x3A..0x3D)
	ovl.init_date_spot(Overlay.GIRL_MEGUMI, Overlay.SPOT_AMUSEMENT_PARK, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x3A] == 1, "Megumi spring amusement park unlocks scene_flags[0x3A]")

	# Yumi (Girl 10): scenes 3..6 (scene_flags 0x41..0x44)
	ovl.init_date_spot(Overlay.GIRL_YUMI, Overlay.SPOT_AMUSEMENT_PARK, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x41] == 1, "Yumi spring amusement park unlocks scene_flags[0x41]")

	# Miharu (Girl 11): bump encounters (requires scene_flags[0x46] >= 2 per place_scenes.c:581)
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 4, 96)
	ovl.scene_flags[0x46] = 2
	ovl.evaluate_date_spot_special_scene()
	check(ovl.current_girl == Overlay.GIRL_MIHARU, "Miharu bump encounter overrides girl to Miharu")
	check(ovl.active_scene_id == 3 and ovl.scene_flags[0x48] == 1, "Miharu spring park bump unlocks scene_flags[0x48]")

	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_SEA, 5, 96)
	ovl.scene_flags[0x46] = 2
	ovl.evaluate_date_spot_special_scene()
	check(ovl.current_girl == Overlay.GIRL_MIHARU, "Miharu sea bump overrides girl to Miharu")
	check(ovl.active_scene_id == 5 and ovl.scene_flags[0x4A] == 1, "Miharu spring sea bump unlocks scene_flags[0x4A]")

	# Rei (Girl 12) & Yoshio (Girl 13)
	ovl.init_date_spot(Overlay.GIRL_REI, Overlay.SPOT_ART_MUSEUM, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.current_girl == Overlay.GIRL_REI and ovl.active_scene_id == 1, "Rei encounter dispatches Scene 1")

	ovl.init_date_spot(Overlay.GIRL_YOSHIO, Overlay.SPOT_GAME_CENTER, 4, 96)
	ovl.evaluate_date_spot_special_scene()
	check(ovl.current_girl == Overlay.GIRL_YOSHIO and ovl.active_scene_id == 1, "Yoshio encounter dispatches Scene 1")

# ==============================================================================
# 3. Amusement Park Ride Movies Tests
# ==============================================================================

func _test_amusement_park_ride_movies() -> void:
	print("\n--- Testing Amusement Park Ride Movies (ride_movies.c) ---")
	var ovl := Overlay.new()
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_AMUSEMENT_PARK, 5, 96)

	# Ride 0: Roller Coaster (LBA 0x4330)
	var r0 := ovl.trigger_ride_movie(Overlay.RIDE_ROLLER_COASTER)
	check(r0["ride_id"] == 0, "Roller coaster ride id is 0")
	check(r0["lba"] == 0x4330, "Roller coaster LBA is 0x4330")
	check(ovl.stage == Overlay.STAGE_RIDE_MOVIE, "Stage switched to STAGE_RIDE_MOVIE")

	# Ride 1: Ghost House (LBA 0x4331)
	var r1 := ovl.trigger_ride_movie(Overlay.RIDE_GHOST_HOUSE)
	check(r1["lba"] == 0x4331, "Ghost house LBA is 0x4331")

	# Ride 2: Ferris Wheel (LBA 0x4332)
	var r2 := ovl.trigger_ride_movie(Overlay.RIDE_FERRIS_WHEEL)
	check(r2["lba"] == 0x4332, "Ferris wheel LBA is 0x4332")

	# Ride 3: Scream Ride (LBA 0x4333)
	var r3 := ovl.trigger_ride_movie(Overlay.RIDE_SCREAM_RIDE)
	check(r3["lba"] == 0x4333, "Scream ride LBA is 0x4333")

	# Ride 4: Virtual Ship (LBA 0x4334)
	var r4 := ovl.trigger_ride_movie(Overlay.RIDE_VIRTUAL_SHIP)
	check(r4["lba"] == 0x4334, "Virtual ship LBA is 0x4334")

# ==============================================================================
# 4. Dialogue Choices & Relationship Stats Tests
# ==============================================================================

func _test_dialogue_choices_and_relationship_stats() -> void:
	print("\n--- Testing Dialogue Choices & Relationship Stats ---")
	var ovl := Overlay.new()
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 4, 96)
	ovl.dispatch_girl_scene(Overlay.GIRL_SHIORI, 4)

	# Initial stats: affection=10, tokimeki=10, syoushin=0
	check(ovl.dialogue_choices.size() == 3, "Scene has 3 dialogue choices")

	# Choice 0: Good response (+3 aff, +2 toki, -5 syou)
	var res0 := ovl.handle_choice(0)
	check(res0["choice_idx"] == 0, "Handled choice 0")
	check(res0["affection_delta"] == 3, "Choice 0 grants +3 affection")
	check(res0["tokimeki_delta"] == 2, "Choice 0 grants +2 tokimeki")
	check(ovl.girls_affection[Overlay.GIRL_SHIORI] == 13, "Affection updated to 13")
	check(ovl.girls_tokimeki[Overlay.GIRL_SHIORI] == 12, "Tokimeki updated to 12")

	# Reset and test Choice 2: Bad response (-2 aff, -1 toki, +5 syou)
	ovl.reset()
	ovl.init_date_spot(Overlay.GIRL_SHIORI, Overlay.SPOT_KIRAMEKI_CENTRAL_PARK, 4, 96)
	ovl.dispatch_girl_scene(Overlay.GIRL_SHIORI, 4)
	var res2 := ovl.handle_choice(2)
	check(res2["affection_delta"] == -2, "Choice 2 applies -2 affection")
	check(res2["tokimeki_delta"] == -1, "Choice 2 applies -1 tokimeki")
	check(res2["syoushin_delta"] == 5, "Choice 2 applies +5 syoushin")
	check(ovl.girls_affection[Overlay.GIRL_SHIORI] == 8, "Affection reduced to 8")
	check(ovl.girls_tokimeki[Overlay.GIRL_SHIORI] == 9, "Tokimeki reduced to 9")
	check(ovl.girls_syoushin[Overlay.GIRL_SHIORI] == 5, "Syoushin increased to 5")

	# Progression to exit
	ovl.advance_step()
	ovl.advance_step()
	check(ovl.stage == Overlay.STAGE_EXIT, "Scene completed and transitioned to STAGE_EXIT")

# ==============================================================================
# 5. Authentic Presentation Rendering & Captures
# ==============================================================================

func _test_presentation_rendering_and_captures() -> void:
	print("\n--- Testing Authentic 320x240 Presentation Rendering & Captures ---")
	var presenter := Presenter.new()

	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	if not DirAccess.dir_exists_absolute(font_root):
		font_root = "/work/.asset-cache/port/fonts/native-font-v2"
	if not DirAccess.dir_exists_absolute(font_root):
		font_root = ProjectSettings.globalize_path("res://../../.asset-cache/port/fonts/native-font-v2")

	var hud_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
	if not FileAccess.file_exists(hud_manifest):
		hud_manifest = "/work/.asset-cache/port/hud-production-v1/manifest.json"
	if not FileAccess.file_exists(hud_manifest):
		hud_manifest = ProjectSettings.globalize_path("res://../../.asset-cache/port/hud-production-v1/manifest.json")

	var font_err: String = presenter.init_resources(font_root, hud_manifest)
	check(font_err.is_empty(), "Presenter init_resources loaded native font: " + font_err)

	# Captures output directory (isolated in T-29018 state)
	var captures_dir := "/workspace/.claude/orchestrator/state/T-29018/event-cg-scenes-v1/captures"
	if not DirAccess.dir_exists_absolute(captures_dir):
		captures_dir = ProjectSettings.globalize_path("res://../../.claude/orchestrator/state/T-29018/event-cg-scenes-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	# 1. Render Shiori Special CG Scene (Scene 4: Central Park Cherry Blossoms)
	var img_shiori: Image = presenter.render_shiori_cg_scene(0)
	check(img_shiori != null and img_shiori.get_size() == Vector2i(320, 240), "Rendered Shiori Special CG Scene is 320x240")
	var path_shiori := captures_dir.path_join("shiori_spring_cg.png")
	var err_s := img_shiori.save_png(path_shiori)
	check(err_s == OK, "Saved captures/shiori_spring_cg.png")

	# 2. Render Ayako Karaoke Special Date Scene (Scene 2: Karaoke Box Duet)
	var img_ayako: Image = presenter.render_ayako_karaoke_scene(0)
	check(img_ayako != null and img_ayako.get_size() == Vector2i(320, 240), "Rendered Ayako Karaoke Scene is 320x240")
	var path_ayako := captures_dir.path_join("ayako_karaoke_scene.png")
	var err_a := img_ayako.save_png(path_ayako)
	check(err_a == OK, "Saved captures/ayako_karaoke_scene.png")

	# 3. Render Amusement Park Ride Movie Scene (Ferris Wheel Viewport LBA 0x4332)
	var img_movie: Image = presenter.render_ride_movie_scene()
	check(img_movie != null and img_movie.get_size() == Vector2i(320, 240), "Rendered Ferris Wheel Ride Movie Scene is 320x240")
	var path_movie := captures_dir.path_join("ferris_wheel_movie.png")
	var err_m := img_movie.save_png(path_movie)
	check(err_m == OK, "Saved captures/ferris_wheel_movie.png")

	# 4. Render Miharu Date Encounter Scene (Scene 3: Park Arrival Bump)
	var img_miharu: Image = presenter.render_miharu_encounter_scene(0)
	check(img_miharu != null and img_miharu.get_size() == Vector2i(320, 240), "Rendered Miharu Date Encounter Scene is 320x240")
	var path_miharu := captures_dir.path_join("miharu_date_encounter.png")
	var err_mi := img_miharu.save_png(path_miharu)
	check(err_mi == OK, "Saved captures/miharu_date_encounter.png")

	# Section 12a Boundary Assertions
	check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding box violations encountered across all 4 scenes")
	if not presenter.bounding_box_violations.is_empty():
		for v in presenter.bounding_box_violations:
			print(" - VIOLATION: " + v)

	check(presenter.recorded_glyphs.size() > 50, "Extensive glyph recording coverage (> 50 glyphs rendered)")
	print("Total glyphs rendered and validated: %d" % presenter.recorded_glyphs.size())
