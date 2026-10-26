class_name DateOverlayPresenter
extends "res://presentation/date_appointment_presenter.gd"
## Authentic 320x240 PS1 presenter for DATE (0x50) and DATE2 (0x58) overlays.
## Mirrors date_mode_handler (src/ovl/date/entry.c:714-743),
## normal_date_three_select_init / make_three_select (src/main/scene_library.c:1032-1114, 2212-2253),
## and amusement_karaoke_return_frame_update (src/ovl/amusement_park_date/karaoke_scene_and_return.c:110-121).

const DateOverlayRules = preload("res://overlays/date_overlay.gd")
const MESSAGE_WINDOW_INNER_RECT := Rect2i(27, 166, 266, 52)


## Scene 1: Date Arrival & Greeting (hud_screen_frame_draw(2), hizuke_draw(0, -38),
## top-left place title banner, girl bustup at (81, 8), speaker + arrival greeting).
func render_date_arrival_greeting(state: Dictionary) -> Dictionary:
	var spot_id := int(state.get("current_place", DateOverlayRules.SPOT_AQUARIUM))
	var bg_image := _select_bg_for_spot(spot_id)
	var image := _render_date_base_frame(state, bg_image, true)

	var place_hex := String(state.get("place_title_hex", DateOverlayRules.DATE_PLACE_NAMES_HEX[spot_id]))
	_draw_place_title_banner(image, place_hex)

	var speaker_hex := DateOverlayRules.SPEAKER_SHIORI_HEX
	var greeting_hex := DateOverlayRules.ARRIVAL_DIRECT_SHIORI_L1_HEX
	var greeting_text := "ううん。 私も、 今来たところだから…。"
	if bool(state.get("looks_rejected", false)):
		greeting_hex = DateOverlayRules.ARRIVAL_BAD_LOOKS_SHIORI_HEX
		greeting_text = "‥‥‥‥‥‥‥‥‥‥。"
	elif int(state.get("date_arrival", DateOverlayRules.DATE_ARRIVAL_DIRECT_GREETING)) == DateOverlayRules.DATE_ARRIVAL_WAIT_GREETING:
		greeting_hex = DateOverlayRules.ARRIVAL_WAIT_SHIORI_L0_HEX
		greeting_text = "ごめんね。 準備に時間が かかっちゃって…。"
	elif int(state.get("date_arrival", DateOverlayRules.DATE_ARRIVAL_DIRECT_GREETING)) == DateOverlayRules.DATE_ARRIVAL_CORRECTED_PLACE:
		greeting_hex = DateOverlayRules.ARRIVAL_CORRECTED_SHIORI_L1_HEX
		greeting_text = "ううん、いいの。 私も、今来たところ だから…。"

	var sndisp := HolidayAppointmentRules.sndisp_lines(
		greeting_hex,
		greeting_text,
		0,
		HolidayAppointmentRules.SPEAKER_STYLE_GIRL
	)
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x_for_hex(speaker_hex), 50],
			"attribute": 0,
		},
	]
	for line: Dictionary in sndisp:
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
		})

	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)
	var bbox_check := verify_text_runs_fit(runs, place_hex)

	return {
		"id": "01_date_arrival_greeting",
		"image": image,
		"text_runs": runs,
		"place_hex": place_hex,
		"bbox_check": bbox_check,
	}


## Scene 2: Date 3-Choice Prompt (make_three_select + normal_date_three_select_init + menu_bar_show(1) + hand cursor).
func render_date_three_choice_prompt(state: Dictionary, selected_row: int = 0, choices_hex: Array = []) -> Dictionary:
	var spot_id := int(state.get("talk_date_spot", DateOverlayRules.SPOT_AQUARIUM))
	var bg_image := _select_bg_for_spot(spot_id)
	var image := _render_date_base_frame(state, bg_image, true)

	var banner_hex := String(state.get("sub_place_title_hex", DateOverlayRules.SUB_PLACE_NAMES_HEX["indoor_hall"]))
	_draw_place_title_banner(image, banner_hex)

	var active_choices := choices_hex if choices_hex.size() == 3 else DateOverlayRules.AQUARIUM_CHOICES_HEX
	var row_clamped := clampi(selected_row, 0, 2)

	# normal_date_three_select_init: x = -0x80 (screen 32), y = [0x30, 0x40, 0x50] (screen 168, 184, 200), w = 0xFC (252), h = 0x10 (16)
	var bar_rect := Rect2i(
		DateOverlayRules.THREE_CHOICE_MENU_X,
		DateOverlayRules.THREE_CHOICE_MENU_Y[row_clamped],
		DateOverlayRules.THREE_CHOICE_MENU_W,
		DateOverlayRules.THREE_CHOICE_MENU_H
	)
	_draw_semi_bar(image, bar_rect, Vector3i(0x2F >> 3, 0x7F >> 3, 0x2F >> 3))

	# make_three_select (src/main/scene_library.c:1032-1114):
	# set_kanji_string(-0x74, 0x32 + i*0x10, 0, choice_buf[i], 0)
	var runs: Array[Dictionary] = []
	for i in range(3):
		runs.append({
			"hex": String(active_choices[i]),
			"position": [-0x74, 0x32 + i * 0x10],
			"attribute": 0,
		})
	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)

	# Hand cursor (Frame 156 / sprite 0x9C, actor 0) positioned on hovered row
	var cursor_actor: Dictionary = _compositor._actor(0, 1, 0x9C, Vector2i(104, 0x30 + row_clamped * 0x10 + 8))
	_compositor._draw_actor(image, cursor_actor)

	var bbox_check := verify_text_runs_fit(runs, banner_hex)
	return {
		"id": "02_date_three_choice_prompt",
		"image": image,
		"text_runs": runs,
		"place_hex": banner_hex,
		"selected_row": row_clamped,
		"bar_rect": bar_rect,
		"bbox_check": bbox_check,
	}


## Scene 3: Date Spot Reaction (girl expression change + Tier 0 Best reaction dialogue after choice evaluation).
func render_date_spot_reaction(state: Dictionary, chosen_index: int = 0) -> Dictionary:
	var eval_res := DateOverlayRules.date_choice_score(state, chosen_index)
	var spot_id := int(state.get("talk_date_spot", DateOverlayRules.SPOT_AQUARIUM))
	var bg_image := _select_bg_for_spot(spot_id)
	var image := _render_date_base_frame(state, bg_image, true)

	var banner_hex := String(state.get("sub_place_title_hex", DateOverlayRules.SUB_PLACE_NAMES_HEX["indoor_hall"]))
	_draw_place_title_banner(image, banner_hex)

	var idx := clampi(chosen_index, 0, 2)
	var reply_hex: String = DateOverlayRules.AQUARIUM_REPLIES_SHIORI_HEX[idx]
	var reply_texts := [
		"素敵、ロマンティックね。",
		"どうして そんな事しか 考えられないの？",
		"あっちにも 奇麗な魚がいるよ。",
	]
	var speaker_hex := DateOverlayRules.SPEAKER_SHIORI_HEX
	var sndisp := HolidayAppointmentRules.sndisp_lines(
		reply_hex,
		reply_texts[idx],
		0,
		HolidayAppointmentRules.SPEAKER_STYLE_GIRL
	)
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x_for_hex(speaker_hex), 50],
			"attribute": 0,
		},
	]
	for line: Dictionary in sndisp:
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
		})

	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)
	var bbox_check := verify_text_runs_fit(runs, banner_hex)

	return {
		"id": "03_date_spot_reaction",
		"image": image,
		"text_runs": runs,
		"place_hex": banner_hex,
		"evaluation": eval_res,
		"bbox_check": bbox_check,
	}


## Scene 4: Amusement Park Ride / Karaoke Sub-scene Choice.
## When subscene_kind == "amusement_coaster": renders Roller Coaster sub-scene ("ジェットコースター") 3-choice prompt.
## When subscene_kind == "karaoke_date2": renders DATE2 Stage 3 Karaoke Box ("ＢＯＸ内") 2x2 song choice menu.
func render_amusement_or_karaoke_subscene(
	state: Dictionary,
	selected_row: int = 0,
	subscene_kind: String = "amusement_coaster"
) -> Dictionary:
	if subscene_kind == "karaoke_date2":
		DateOverlayRules.setup_date_spot(state, DateOverlayRules.SPOT_KARAOKE)
		var bg_image := _select_bg_for_spot(DateOverlayRules.SPOT_KARAOKE)
		var image := _render_date_base_frame(state, bg_image, true)
		var banner_hex: String = DateOverlayRules.SUB_PLACE_NAMES_HEX["karaoke_box"]
		_draw_place_title_banner(image, banner_hex)

		var song_idx := clampi(selected_row, 0, 3)
		var song_hex: String = DateOverlayRules.KARAOKE_SONG_LABELS_HEX[song_idx]
		var song_width_px := (song_hex.length() / 4) * 16
		var bar_rect := Rect2i(
			DateOverlayRules.KARAOKE_MENU_X[song_idx],
			DateOverlayRules.KARAOKE_MENU_Y[song_idx],
			song_width_px,
			DateOverlayRules.KARAOKE_MENU_H
		)
		_draw_semi_bar(image, bar_rect, Vector3i(0x2F >> 3, 0x7F >> 3, 0x2F >> 3))

		# amusement_karaoke_return_menu_build: x = [-0x80, 0, -0x80, -0x20], y = [0x34, 0x34, 0x4C, 0x4C]
		var gx := [-0x80, 0, -0x80, -0x20]
		var gy := [0x34, 0x34, 0x4C, 0x4C]
		var runs: Array[Dictionary] = []
		for i in range(4):
			runs.append({
				"hex": String(DateOverlayRules.KARAOKE_SONG_LABELS_HEX[i]),
				"position": [gx[i], gy[i]],
				"attribute": 0,
			})
		_compositor.set_text_runs(runs)
		_compositor._draw_text(image)

		var cursor_actor: Dictionary = _compositor._actor(
			0, 1, 0x9C,
			Vector2i(gx[song_idx] + song_width_px - 4, gy[song_idx] + 8)
		)
		_compositor._draw_actor(image, cursor_actor)
		var bbox_check := verify_text_runs_fit(runs, banner_hex)
		return {
			"id": "04_amusement_or_karaoke_subscene",
			"subscene_kind": subscene_kind,
			"image": image,
			"text_runs": runs,
			"place_hex": banner_hex,
			"selected_row": song_idx,
			"bar_rect": bar_rect,
			"bbox_check": bbox_check,
		}

	# Default: Amusement Park Roller Coaster sub-scene ("ジェットコースター")
	DateOverlayRules.setup_date_spot(state, DateOverlayRules.SPOT_AMUSEMENT_PARK)
	DateOverlayRules.select_amusement_park_ride(state, DateOverlayRules.RIDE_ROLLER_COASTER)
	var res := render_date_three_choice_prompt(
		state,
		selected_row,
		DateOverlayRules.AMUSEMENT_COASTER_CHOICES_HEX
	)
	res["id"] = "04_amusement_or_karaoke_subscene"
	res["subscene_kind"] = subscene_kind
	return res


## Renders the shared DATE/DATE2 frame:
## 1. hud_screen_frame_draw(2) checkerboard background
## 2. 320x160 date venue background at (0, 0)
## 3. Optional girl bustup at (81, 8) (actor_pool[31] at (0, -32))
## 4. hizuke_draw(0, -38) shifted calendar pillar + message window (actor 1 at (0, 75) + gradient Rect2i(27, 166, 266, 52))
func _render_date_base_frame(state: Dictionary, bg_image: Image, show_portrait: bool) -> Image:
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	_draw_checkerboard(image)
	_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	_compositor.last_coverage.fill(Color.BLACK)
	_compositor.set_text_runs([])

	if bg_image != null:
		_compositor._blit(image, bg_image, null, Vector2i(0, 0), false)

	if show_portrait and _shiori_portrait_image != null:
		var portrait_info: Dictionary = _date_manifest.get("shiori_portrait", {"screen_x": 81, "screen_y": 8})
		var portrait_pos := Vector2i(int(portrait_info["screen_x"]), int(portrait_info["screen_y"]))
		_compositor._blit(image, _shiori_portrait_image, null, portrait_pos, false)

	var date_info := {
		"year": int(state.get("year", 95)) % 100,
		"month": int(state.get("month", 7)),
		"day": int(state.get("day", 23)),
		"weekday": int(state.get("weekday", 6)),
		"day_kind": 1,
	}
	var cal: Dictionary = Layout.build_calendar(date_info)
	var commands: Array[Dictionary] = []
	for record: Dictionary in cal["actors"]:
		if record["visible"]:
			commands.append(_compositor._actor(
				record["actor_index"],
				4,
				record["sprite_id"],
				Vector2i(record["x"], record["y"] - 38),
				"hud-%02x" % record["color_mode"] if record["color_mode"] else "base"
			))
	commands.append(_compositor._actor(1, 7, 0x3E, Vector2i(0, 75)))
	commands.append({"kind": "gradient", "id": -1, "priority": 8, "rect": MESSAGE_WINDOW_INNER_RECT})
	commands.append({"kind": "gradient", "id": -1, "priority": 5, "rect": Rect2i(294, 7, 20, 114)})
	commands.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		return a["priority"] > b["priority"] if a["priority"] != b["priority"] else a["id"] > b["id"]
	)
	for cmd: Dictionary in commands:
		if cmd.get("kind") == "gradient":
			_compositor._gradient(image, cmd["rect"])
		else:
			_compositor._draw_actor(image, cmd)

	return image


func _select_bg_for_spot(spot_id: int) -> Image:
	if _date_bg_images.has(spot_id):
		return _date_bg_images[spot_id]
	# Indoor/water/hall venues use aquarium_17809 (place_id 3), outdoor/park venues use park_17112 (place_id 0)
	if spot_id in [
		DateOverlayRules.SPOT_AQUARIUM,
		DateOverlayRules.SPOT_PLANETARIUM,
		DateOverlayRules.SPOT_ART_MUSEUM,
		DateOverlayRules.SPOT_LIBRARY,
		DateOverlayRules.SPOT_GAME_CENTER,
		DateOverlayRules.SPOT_BOWLING_ALLEY,
		DateOverlayRules.SPOT_KARAOKE,
		DateOverlayRules.SPOT_MOVIE_THEATER,
		DateOverlayRules.SPOT_CONCERT_HALL,
		DateOverlayRules.SPOT_POOL,
	] and _date_bg_images.has(DateOverlayRules.SPOT_AQUARIUM):
		return _date_bg_images[DateOverlayRules.SPOT_AQUARIUM]
	return _date_bg_images.get(DateOverlayRules.SPOT_NEIGHBORHOOD_PARK)


## Verifies that every glyph in both the top-left place banner and the bottom message window
## stays strictly inside its target container bounding box.
static func verify_text_runs_fit(runs: Array[Dictionary], place_hex: String) -> Dictionary:
	var violations: Array[Dictionary] = []
	var checked_glyphs := 0

	# 1. Verify place title banner glyphs inside banner rect Rect2i(16, 30, glyph_count * 14 + 8, 20)
	var banner_glyphs := place_hex.length() / 4
	var banner_rect := Rect2i(16, 30, banner_glyphs * 14 + 8, 20)
	for i in range(banner_glyphs):
		checked_glyphs += 1
		var gx := 160 - 142 + i * 14
		var gy := 120 - 88
		var grect := Rect2i(gx, gy, 14, 14)
		if not banner_rect.encloses(grect):
			violations.append({
				"container": "place_banner",
				"glyph_index": i,
				"glyph_rect": grect,
				"container_rect": banner_rect,
			})

	# 2. Verify message window runs inside MESSAGE_WINDOW_INNER_RECT Rect2i(27, 166, 266, 52)
	for r_idx in range(runs.size()):
		var run: Dictionary = runs[r_idx]
		var hx := String(run.get("hex", ""))
		var pos: Array = run.get("position", [0, 0])
		var start_x := 160 + int(pos[0])
		var start_y := 120 + int(pos[1])
		var gcount := hx.length() / 4
		for i in range(gcount):
			checked_glyphs += 1
			var grect := Rect2i(start_x + i * 14, start_y, 14, 14)
			if not MESSAGE_WINDOW_INNER_RECT.encloses(grect):
				violations.append({
					"container": "message_window",
					"run_index": r_idx,
					"glyph_index": i,
					"glyph_rect": grect,
					"container_rect": MESSAGE_WINDOW_INNER_RECT,
				})

	return {
		"violation_count": violations.size(),
		"checked_glyph_count": checked_glyphs,
		"violations": violations,
	}
