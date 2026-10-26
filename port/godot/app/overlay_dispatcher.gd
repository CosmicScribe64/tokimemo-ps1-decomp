class_name OverlayDispatcher
extends RefCounted
## T-29018: Unified PS1 GameMode & Overlay Dispatcher for the Godot 4.7.2 Port.
## Connects all 26 PS1 overlays (src/ovl/*) and core modes into a single runtime
## router with deterministic input stepping and authentic 320x240 frame rendering.

const VnPresenter = preload("res://presentation/vn_scene_presenter.gd")
const SystemOverlays = preload("res://overlays/system_overlays.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const TelephonePresenter = preload("res://presentation/telephone_overlay_presenter.gd")
const DateOverlay = preload("res://overlays/date_overlay.gd")
const DatePresenter = preload("res://presentation/date_overlay_presenter.gd")
const EncountersOverlay = preload("res://overlays/encounters_and_events_overlay.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const EncountersEndingPresenter = preload("res://presentation/encounters_and_ending_presenter.gd")
const SchoolEventsOverlay = preload("res://overlays/school_events_overlay.gd")
const SchoolEventsPresenter = preload("res://presentation/school_events_presenter.gd")
const SubgameOverlays = preload("res://overlays/subgame_overlays.gd")
const SubgamePresenter = preload("res://presentation/subgame_overlays_presenter.gd")
const EventCgOverlay = preload("res://overlays/event_cg_scenes_overlay.gd")
const EventCgPresenter = preload("res://presentation/event_cg_scenes_presenter.gd")

# PS1 GameMode Table (src/main/scheduler.c & include/common.h)
const GAME_MODE_OPENING             := 0x01
const GAME_MODE_PROLOGUE            := 0x15
const GAME_MODE_COMMAND_SCREEN      := 0x34
const GAME_MODE_EXAM                := 0x35
const GAME_MODE_OPTION              := 0x36
const GAME_MODE_OLH                 := 0x37
const GAME_MODE_TEL                 := 0x3D
const GAME_MODE_TEL_ACTION          := 0x3E
const GAME_MODE_ETC                 := 0x3F
const GAME_MODE_WEEKDAY_ACTIVITY    := 0x40
const GAME_MODE_GEKO                := 0x44
const GAME_MODE_HOLIDAY             := 0x46
const GAME_MODE_TAIIKU              := 0x47
const GAME_MODE_BUNKASAI            := 0x48
const GAME_MODE_BUNKA_SD            := 0x49
const GAME_MODE_BUNKAKEN            := 0x4A
const GAME_MODE_OMIMAI              := 0x4B
const GAME_MODE_SHUGAKU             := 0x4C
const GAME_MODE_KANGEI              := 0x4D
const GAME_MODE_SHOUGATU            := 0x4E
const GAME_MODE_VALEN               := 0x4F
const GAME_MODE_DATE                := 0x50
const GAME_MODE_RENSYU              := 0x51
const GAME_MODE_MASTER              := 0x52
const GAME_MODE_EN_NICHI            := 0x53
const GAME_MODE_RPG_BATTLE          := 0x57
const GAME_MODE_AMUSEMENT_PARK_DATE := 0x58
const GAME_MODE_TACO                := 0x59
const GAME_MODE_TT                  := 0x5A
const GAME_MODE_ENDING              := 0x5C
const GAME_MODE_EVENT_CG            := 0x5E

const OVERLAY_CATALOG := {
	GAME_MODE_OPENING:             {"overlay": "ETC",      "name": "Opening & Title Menu",          "source": "src/ovl/misc_scenes/"},
	GAME_MODE_EXAM:                {"overlay": "GYOZI",    "name": "Term Exam Results",             "source": "src/main/school_events.c"},
	GAME_MODE_OPTION:              {"overlay": "OPTION",   "name": "Options & Sound Test",          "source": "src/ovl/options/"},
	GAME_MODE_OLH:                 {"overlay": "OLH",      "name": "Online Help (Q&A Corner)",      "source": "src/ovl/help/"},
	GAME_MODE_TEL:                 {"overlay": "TEL",      "name": "Telephone Directory & Call",    "source": "src/ovl/telephone/"},
	GAME_MODE_TEL_ACTION:          {"overlay": "TEL",      "name": "Yoshio Girl Profile & Call",    "source": "src/ovl/telephone/"},
	GAME_MODE_ETC:                 {"overlay": "ETC",      "name": "Misc Scenes & Title Screen",    "source": "src/ovl/misc_scenes/"},
	GAME_MODE_GEKO:                {"overlay": "GEKO",     "name": "After-School Walk Home",        "source": "src/ovl/girl_encounters/"},
	GAME_MODE_TAIIKU:              {"overlay": "TAIIKU",   "name": "Sports Festival",               "source": "src/ovl/sports_festival/"},
	GAME_MODE_BUNKASAI:            {"overlay": "BUNKASAI", "name": "Culture Festival",              "source": "src/ovl/culture_festival/"},
	GAME_MODE_BUNKA_SD:            {"overlay": "BUNKA_SD", "name": "Culture Festival SD Stage",     "source": "src/ovl/culture_festival_sd/"},
	GAME_MODE_BUNKAKEN:            {"overlay": "BUNKAKEN", "name": "Culture Club Tour",             "source": "src/ovl/culture_club_tour/"},
	GAME_MODE_OMIMAI:              {"overlay": "OMIMAI",   "name": "Sick Visit",                    "source": "src/ovl/sick_visit/"},
	GAME_MODE_SHUGAKU:             {"overlay": "SHUGAKU",  "name": "School Trip",                   "source": "src/ovl/school_trip/"},
	GAME_MODE_KANGEI:              {"overlay": "KANGEI",   "name": "Freshman Welcome Party",        "source": "src/ovl/welcome_party/"},
	GAME_MODE_SHOUGATU:            {"overlay": "SHOUGATU", "name": "New Year & Christmas",          "source": "src/ovl/new_year/"},
	GAME_MODE_VALEN:               {"overlay": "VALEN",    "name": "Valentine's Day",               "source": "src/ovl/valentine/"},
	GAME_MODE_DATE:                {"overlay": "DATE",     "name": "Date Spot Overlay",             "source": "src/ovl/date/"},
	GAME_MODE_RENSYU:              {"overlay": "RENSYU",   "name": "Club Practice Match & Camp",    "source": "src/ovl/club_practice/"},
	GAME_MODE_MASTER:              {"overlay": "MASTER",   "name": "Club Master Duel",              "source": "src/ovl/club_master/"},
	GAME_MODE_EN_NICHI:            {"overlay": "EN_NICHI", "name": "Night Festival Stalls",         "source": "src/ovl/night_stalls/"},
	GAME_MODE_RPG_BATTLE:          {"overlay": "RPG_BAT",  "name": "Turn-Based Party RPG Battle",   "source": "src/ovl/rpg_battle/"},
	GAME_MODE_AMUSEMENT_PARK_DATE: {"overlay": "DATE2",    "name": "Amusement Park & Karaoke Date", "source": "src/ovl/amusement_park_date/"},
	GAME_MODE_TACO:                {"overlay": "TACO",     "name": "3D Space Shooter Minigame",     "source": "src/ovl/shooter_3d/"},
	GAME_MODE_TT:                  {"overlay": "TT",       "name": "TwinBee Time Attack",           "source": "src/ovl/TT/"},
	GAME_MODE_ENDING:              {"overlay": "ENDING",   "name": "Confession & Ending Roll",      "source": "src/ovl/ending/"},
	GAME_MODE_EVENT_CG:            {"overlay": "EVENT",    "name": "Special Event CG & Ride Movie", "source": "src/ovl/date_spots/"},
}

var vn_presenter: VnPresenter
var telephone_presenter: TelephonePresenter
var date_presenter: DatePresenter
var encounters_ending_presenter: EncountersEndingPresenter
var school_events_presenter: SchoolEventsPresenter
var subgame_presenter: SubgamePresenter
var event_cg_presenter: EventCgPresenter

var options_overlay: SystemOverlays.OptionsOverlay
var help_overlay: SystemOverlays.HelpOverlay
var title_overlay: SystemOverlays.TitleMenuOverlay
var error: String = ""
var _ready: bool = false


func open_resources(font_root: String, hud_manifest_path: String, date_pack_path: String) -> String:
	vn_presenter = VnPresenter.new()
	var err: String = vn_presenter.init_resources(font_root, hud_manifest_path)
	if not err.is_empty():
		error = "vn_presenter init failed: " + err
		return error

	telephone_presenter = TelephonePresenter.new()
	if not telephone_presenter.open_date_preview(hud_manifest_path, font_root, date_pack_path):
		error = "telephone_presenter init failed: " + telephone_presenter.error
		return error

	date_presenter = DatePresenter.new()
	if not date_presenter.open_date_preview(hud_manifest_path, font_root, date_pack_path):
		error = "date_presenter init failed: " + date_presenter.error
		return error

	encounters_ending_presenter = EncountersEndingPresenter.new()
	err = encounters_ending_presenter.load_manifest(hud_manifest_path, font_root)
	if not err.is_empty():
		error = "encounters_ending_presenter init failed: " + err
		return error

	school_events_presenter = SchoolEventsPresenter.new()
	err = school_events_presenter.init_resources(font_root, hud_manifest_path)
	if not err.is_empty():
		error = "school_events_presenter init failed: " + err
		return error

	subgame_presenter = SubgamePresenter.new()
	err = subgame_presenter.init_resources(font_root)
	if not err.is_empty():
		error = "subgame_presenter init failed: " + err
		return error

	event_cg_presenter = EventCgPresenter.new()
	err = event_cg_presenter.init_resources(font_root, hud_manifest_path)
	if not err.is_empty():
		error = "event_cg_presenter init failed: " + err
		return error

	options_overlay = SystemOverlays.create_options()
	help_overlay = SystemOverlays.create_help()
	title_overlay = SystemOverlays.create_title_menu()
	_ready = true
	error = ""
	return ""


static func supported_modes() -> Array[int]:
	var modes: Array[int] = []
	for k: int in OVERLAY_CATALOG.keys():
		modes.append(k)
	modes.sort()
	return modes


func handles_mode(mode: int) -> bool:
	return OVERLAY_CATALOG.has(mode)


func describe_mode(mode: int) -> Dictionary:
	return OVERLAY_CATALOG.get(mode, {})


# PS1 Controller Button Constants (common.h / pad.h)
const PAD_SELECT   := 0x0100
const PAD_START    := 0x0800
const PAD_UP       := 0x1000
const PAD_RIGHT    := 0x2000
const PAD_DOWN     := 0x4000
const PAD_LEFT     := 0x8000
const PAD_TRIANGLE := 0x0010
const PAD_CIRCLE   := 0x0020
const PAD_CROSS    := 0x0040
const PAD_SQUARE   := 0x0080


static func _default_tel_candidate_slots() -> Array[Dictionary]:
	return [
		{"year": 95, "month": 4, "day": 16, "month_offset": 0, "day_kind": 1},
		{"year": 95, "month": 4, "day": 23, "month_offset": 0, "day_kind": 1},
		{"year": 95, "month": 4, "day": 29, "month_offset": 0, "day_kind": 2},
		{"year": 95, "month": 4, "day": 30, "month_offset": 0, "day_kind": 1},
	]


static func _default_girls_roster() -> Array[Dictionary]:
	var girls: Array[Dictionary] = []
	for i in range(13):
		girls.append({
			"girl_id": i,
			"met": 1,
			"rank": 10 + i * 5,
			"points": 200,
			"tokimeki": 75,
			"affection": 70,
			"syoushin": 10,
			"flags": (1 << 26),
			"scene_counters": [0, 8, 2, 0, 0],
		})
	return girls


static func _step_cursor(cursor: int, option_count: int, pad_pressed: int) -> int:
	if option_count <= 1:
		return 0
	if (pad_pressed & PAD_UP) != 0 or (pad_pressed & PAD_LEFT) != 0:
		return (cursor + option_count - 1) % option_count
	if (pad_pressed & PAD_DOWN) != 0 or (pad_pressed & PAD_RIGHT) != 0:
		return (cursor + 1) % option_count
	return clampi(cursor, 0, option_count - 1)


func step_mode(mode: int, state: Dictionary, pad_pressed: int) -> Dictionary:
	var next_state: Dictionary
	if state.has("state") and state["state"] is Dictionary:
		next_state = (state["state"] as Dictionary).duplicate(true)
		for k in state.keys():
			if k != "state" and k != "action" and k != "mode":
				next_state[k] = state[k]
	else:
		next_state = state.duplicate(true)
	next_state["step_count"] = int(next_state.get("step_count", 0)) + 1
	next_state["last_pad"] = pad_pressed
	var cursor: int = int(next_state.get("cursor", 0))
	var stage: int = int(next_state.get("stage", 0))
	var girl_id: int = int(next_state.get("girl_id", 0))
	var action: String = "stepped"

	match mode:
		GAME_MODE_OPTION:
			if state.has("cursor"):
				options_overlay.cursor = clampi(cursor, 0, 7)
			if state.has("stage"):
				options_overlay.mode_stage = stage
			var res: Dictionary = options_overlay.process_input(pad_pressed)
			next_state["cursor"] = options_overlay.cursor
			next_state["stage"] = options_overlay.mode_stage
			next_state["settings"] = options_overlay.get_settings()
			res["mode"] = mode
			res["state"] = next_state
			return res

		GAME_MODE_OLH:
			if state.has("cursor"):
				help_overlay.cursor = clampi(cursor, 0, 7)
			if state.has("stage"):
				help_overlay.mode_stage = stage
			var res: Dictionary = help_overlay.process_input(pad_pressed)
			next_state["cursor"] = help_overlay.cursor
			next_state["stage"] = help_overlay.mode_stage
			res["mode"] = mode
			res["state"] = next_state
			return res

		GAME_MODE_OPENING, GAME_MODE_ETC:
			if state.has("cursor"):
				title_overlay.cursor = clampi(cursor, 0, 1)
			if state.has("stage"):
				title_overlay.stage = stage
			var res: Dictionary = title_overlay.process_input(pad_pressed)
			next_state["cursor"] = title_overlay.cursor
			next_state["stage"] = title_overlay.stage
			res["mode"] = mode
			res["state"] = next_state
			return res

		GAME_MODE_TEL:
			var sub := String(next_state.get("subscreen", ""))
			if sub == "day_select":
				stage = 1
			elif sub == "call_response":
				stage = 2
			elif sub == "place_select":
				stage = 0

			if stage == 0:
				var page: int = int(next_state.get("page", 0))
				var avail: Array[int] = TelephoneOverlay.build_date_place_availability(
					page, 95, 4, 9, [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1], {}, 4
				)
				next_state["avail_table"] = avail
				cursor = _step_cursor(cursor, 11, pad_pressed)
				next_state["subscreen"] = "place_select"
				if (pad_pressed & PAD_CIRCLE) != 0:
					if cursor == 10:
						next_state["page"] = 1 - page
						action = "page_flip"
					else:
						next_state["place_id"] = page * 10 + cursor
						next_state["selected_place"] = page * 10 + cursor
						next_state["candidate_slots"] = _default_tel_candidate_slots()
						stage = 1
						cursor = 0
						next_state["subscreen"] = "day_select"
						action = "advance"
				elif (pad_pressed & PAD_CROSS) != 0:
					action = "return_to_game"
			elif stage == 1:
				var slots: Array = next_state.get("candidate_slots", _default_tel_candidate_slots())
				var slot_count: int = maxi(1, slots.size())
				cursor = _step_cursor(cursor, slot_count, pad_pressed)
				next_state["subscreen"] = "day_select"
				if (pad_pressed & PAD_CIRCLE) != 0:
					var chosen_slot: Dictionary = slots[cursor] if cursor < slots.size() else {"year": 95, "month": 4, "day": 16}
					next_state["chosen_slot"] = chosen_slot
					var place_id: int = int(next_state.get("place_id", TelephoneOverlay.PLACE_AQUARIUM))
					var eval_res: Dictionary = TelephoneOverlay.evaluate_invitation(girl_id, place_id, 120, 80, 80)
					next_state["invitation_evaluation"] = eval_res
					next_state["response_kind"] = String(next_state.get("response_kind", "accepted"))
					stage = 2
					cursor = 0
					next_state["subscreen"] = "call_response"
					action = "advance"
				elif (pad_pressed & PAD_CROSS) != 0:
					stage = 0
					cursor = 0
					next_state["subscreen"] = "place_select"
					action = "back"
			else:
				next_state["subscreen"] = "call_response"
				if (pad_pressed & PAD_CIRCLE) != 0 or (pad_pressed & PAD_CROSS) != 0:
					stage = 3
					action = "return_to_game"

		GAME_MODE_TEL_ACTION:
			var roster: Array[Dictionary] = _default_girls_roster()
			next_state["appraisal_list"] = TelephoneOverlay.build_full_appraisal_list(roster)
			cursor = _step_cursor(cursor, 11, pad_pressed)
			if (pad_pressed & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) != 0 and not state.has("girl_id"):
				girl_id = cursor
				next_state["girl_id"] = girl_id
			var prof: Dictionary = TelephoneOverlay.GIRL_PROFILES.get(girl_id, TelephoneOverlay.GIRL_PROFILES[0])
			next_state["zodiac_sign"] = TelephoneOverlay.get_zodiac_sign(int(prof["birthday_month"]), int(prof["birthday_day"]))
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				if stage > 0:
					stage -= 1
					action = "back"
				else:
					action = "return_to_game"

		GAME_MODE_DATE:
			var date_state: Dictionary = DateOverlay.create_default_state(next_state)
			var spot_id: int = int(date_state.get("current_place", DateOverlay.SPOT_AQUARIUM))
			DateOverlay.setup_date_spot(date_state, spot_id)
			if stage == 0:
				DateOverlay.date_arrival_and_looks_check(date_state)
				cursor = _step_cursor(cursor, 3, pad_pressed)
				if (pad_pressed & PAD_CIRCLE) != 0:
					stage = 1
					cursor = 0
					action = "advance"
				elif (pad_pressed & PAD_CROSS) != 0:
					action = "return_to_game"
			elif stage == 1:
				cursor = _step_cursor(cursor, 3, pad_pressed)
				if (pad_pressed & PAD_CIRCLE) != 0:
					next_state["chosen_index"] = cursor
					next_state["choice_evaluation"] = DateOverlay.date_choice_score(date_state, cursor)
					stage = 2
					action = "advance"
				elif (pad_pressed & PAD_CROSS) != 0:
					stage = 0
					action = "back"
			else:
				cursor = _step_cursor(cursor, 3, pad_pressed)
				if (pad_pressed & PAD_CIRCLE) != 0 or (pad_pressed & PAD_CROSS) != 0:
					next_state["finish_summary"] = DateOverlay.finish_date(date_state)
					stage = 3
					action = "return_to_game"
			next_state["place_title_hex"] = date_state["place_title_hex"]
			next_state["sub_place_title_hex"] = date_state["sub_place_title_hex"]

		GAME_MODE_AMUSEMENT_PARK_DATE:
			var date2_state: Dictionary = DateOverlay.create_default_state(next_state)
			var sub_kind: String = String(next_state.get("subscene_kind", "karaoke_date2"))
			var opt_count: int = 4 if sub_kind == "karaoke_date2" else 3
			cursor = _step_cursor(cursor, opt_count, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					if sub_kind == "karaoke_date2":
						next_state["karaoke_result"] = DateOverlay.finish_date2_karaoke_song(date2_state, cursor, true)
					else:
						DateOverlay.select_amusement_park_ride(date2_state, DateOverlay.RIDE_ROLLER_COASTER)
						next_state["coaster_score"] = DateOverlay.date_choice_score(date2_state, cursor)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				if sub_kind == "karaoke_date2" and stage == 0:
					next_state["karaoke_result"] = DateOverlay.finish_date2_karaoke_song(date2_state, cursor, false)
				action = "return_to_game"

		GAME_MODE_GEKO:
			cursor = _step_cursor(cursor, 2, pad_pressed)
			var girls_list: Array[Dictionary] = _default_girls_roster()
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					var topic: Dictionary = EncountersOverlay.select_walk_home_topic(girl_id, 1, 0, 10, 18, 0, 1, 0)
					var choice_out: Dictionary = EncountersOverlay.apply_walk_home_choice(
						cursor, girl_id, 1, int(topic["walk_home_group"]), girls_list, int(next_state["step_count"]), [15, 25]
					)
					next_state["walk_home_topic"] = topic
					next_state["walk_home_outcome"] = choice_out
					stage = 1
					action = "advance"
				else:
					next_state["girls"] = EncountersOverlay.finish_walk_home(girl_id, girls_list)
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_EXAM:
			cursor = _step_cursor(cursor, 5, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					var empty_flags: Array[int] = []
					empty_flags.resize(0xD8)
					empty_flags.fill(0)
					var ranking: Dictionary = EncountersOverlay.build_exam_ranking(
						int(next_state.get("player_total", 485)), _default_girls_roster(), empty_flags
					)
					next_state["exam_ranking"] = ranking
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_OMIMAI:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["sick_visit_dialogue"] = EncountersOverlay.select_sick_visit_dialogue(
						girl_id, cursor, int(next_state.get("club_id", 0)), 1, 96, 6, 0
					)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_SHOUGATU:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			var girls_list: Array[Dictionary] = _default_girls_roster()
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					var visitor: int = EncountersOverlay.select_new_year_visitor(girls_list, [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
					next_state["new_year_visitor"] = visitor
					next_state["christmas_girls"] = EncountersOverlay.apply_christmas_party_rewards(girl_id, girls_list)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_VALEN:
			cursor = _step_cursor(cursor, 2, pad_pressed)
			var girls_list: Array[Dictionary] = _default_girls_roster()
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["valentine_candidates"] = EncountersOverlay.select_valentine_candidates(
						girls_list, [80, 75, 70, 65, 60, 55, 50, 50, 50, 50, 50], [75, 70, 65, 60, 55, 50, 50, 50, 50, 50, 50]
					)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_MASTER:
			cursor = _step_cursor(cursor, 2, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["master_grade"] = EndingOverlay.compute_master_tournament_grade(
						int(next_state.get("practice_points", 385))
					)
					next_state["master_companion"] = EndingOverlay.select_master_companion(
						int(next_state.get("club_id", 3)), _default_girls_roster(), [80, 75, 70, 85, 80, 70, 65, 60, 60, 60, 60]
					)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_ENDING:
			cursor = _step_cursor(cursor, 2, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				var flow: Dictionary = EndingOverlay.step_ending_stage_flow(stage, girl_id)
				next_state["ending_flow"] = flow
				next_state["ending_dialogue"] = EndingOverlay.select_graduation_result_dialogue(girl_id, girl_id, cursor, 0)
				if int(flow.get("next_game_mode", EndingOverlay.GAME_MODE_ENDING)) == EndingOverlay.GAME_MODE_ETC_C2:
					stage = 7
					next_state["stage"] = stage
					next_state["cursor"] = cursor
					return {
						"action": "handoff_credits",
						"next_mode": EndingOverlay.GAME_MODE_ETC_C2,
						"mode": mode,
						"cursor": cursor,
						"stage": stage,
						"state": next_state,
					}
				stage = int(flow.get("next_stage", stage + 1))
				action = "advance"
			elif (pad_pressed & PAD_CROSS) != 0:
				if stage > 0:
					stage -= 1
					action = "back"
				else:
					action = "return_to_game"

		GAME_MODE_TAIIKU:
			if not next_state.has("cpu_positions"):
				var init_race: Dictionary = SchoolEventsOverlay.sports_init_state(int(next_state.get("competition", cursor)))
				for k in init_race.keys():
					if not next_state.has(k):
						next_state[k] = init_race[k]
			cursor = _step_cursor(cursor, 4, pad_pressed)
			if (pad_pressed & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) != 0 and not state.has("competition"):
				next_state["competition"] = cursor
			if (pad_pressed & PAD_CIRCLE) != 0:
				var athletics: int = int(next_state.get("player_athletics", 80))
				for _tick in range(16):
					SchoolEventsOverlay.sports_step_race(next_state, true, athletics)
				if stage == 0:
					stage = 1
					next_state["phase"] = "running"
					action = "advance"
				elif stage == 1:
					stage = 2
					next_state["phase"] = "finished"
					next_state["ranking_rewards"] = SchoolEventsOverlay.sports_calculate_ranking_rewards(
						int(next_state.get("player_place", 1)), girl_id, _default_girls_roster()
					)
					action = "advance"
				else:
					stage = 3
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_BUNKASAI, GAME_MODE_BUNKA_SD, GAME_MODE_BUNKAKEN:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			next_state["selected_booth"] = cursor
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					if mode == GAME_MODE_BUNKASAI:
						next_state["booth_prep"] = SchoolEventsOverlay.culture_booth_preparation_check(
							int(next_state.get("club_id", 0)), {"humanities": 80, "science": 80, "art": 85, "look": 75, "trivia": 75}, 120
						)
						next_state["companion_invite"] = SchoolEventsOverlay.culture_companion_invite(girl_id, 60, 65)
					elif mode == GAME_MODE_BUNKA_SD:
						next_state["stage_show"] = SchoolEventsOverlay.culture_stage_show_evaluate(
							cursor, int(next_state.get("club_id", 1)), 80, true
						)
					else:
						next_state["visitor_girl"] = SchoolEventsOverlay.culture_visitor_pick([0, 1, 2, 3, 4], cursor + 1)
						next_state["companion_invite"] = SchoolEventsOverlay.culture_companion_invite(girl_id, 60, 65)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_SHUGAKU:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			var dest: int = int(next_state.get("destination", cursor))
			if (pad_pressed & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) != 0 and not state.has("destination"):
				dest = cursor
				next_state["destination"] = dest
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["trip_state"] = SchoolEventsOverlay.trip_init(dest)
					next_state["trip_deltas"] = SchoolEventsOverlay.trip_response_stat_deltas(cursor)
					stage = 1
					action = "advance"
				elif stage == 1:
					next_state["corridor_event"] = SchoolEventsOverlay.trip_hotel_corridor_encounter(cursor, girl_id)
					stage = 2
					action = "advance"
				else:
					stage = 3
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_KANGEI:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			next_state["selected_booth"] = cursor
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["camp_day"] = SchoolEventsOverlay.camp_step_day(
						int(next_state.get("day", 1)), int(next_state.get("club_id", 6)), 120, 85
					)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_RENSYU:
			cursor = _step_cursor(cursor, 5, pad_pressed)
			var club_id: int = int(next_state.get("club_id", 6 + (cursor % 5)))
			if (pad_pressed & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) != 0 and not state.has("club_id"):
				club_id = 6 + (cursor % 5)
				next_state["club_id"] = club_id
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					var eval_match: Dictionary = SchoolEventsOverlay.practice_match_evaluate(
						club_id, int(next_state.get("practice_points", 200)), int(next_state.get("match_count", 2)), girl_id
					)
					next_state["practice_evaluation"] = eval_match
					next_state["victory"] = bool(eval_match["victory"])
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_EN_NICHI:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["stall_result"] = SchoolEventsOverlay.stall_game_play(
						cursor, int(next_state.get("hold_frames", 15)), girl_id
					)
					stage = 1
					action = "advance"
				else:
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_RPG_BATTLE:
			var custom_enemy_hp: int = int(next_state.get("enemy_hp", -1))
			if not next_state.has("player_hp"):
				var init_rpg: Dictionary = SubgameOverlays.rpg_create_state(next_state)
				for k in init_rpg.keys():
					if not next_state.has(k):
						next_state[k] = init_rpg[k]
			cursor = _step_cursor(int(next_state.get("command_cursor", cursor)), 3, pad_pressed)
			next_state["command_cursor"] = cursor
			if (pad_pressed & PAD_CIRCLE) != 0:
				SubgameOverlays.rpg_execute_command(
					next_state, cursor, int(next_state.get("target_slot", 0)), int(next_state.get("rand_seed", 512))
				)
				if custom_enemy_hp > 0 and int(next_state.get("last_damage", 0)) > 0:
					next_state["enemy_hp"] = maxi(0, custom_enemy_hp - int(next_state.get("last_damage", 0)))
				if bool(next_state.get("victory", false)) or bool(next_state.get("defeat", false)) or stage >= 1:
					stage += 1
					action = "return_to_game"
				else:
					stage = 1
					action = "advance"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

		GAME_MODE_TACO:
			if not next_state.has("player"):
				var init_taco: Dictionary = SubgameOverlays.shooter_create_state(next_state)
				for k in init_taco.keys():
					if not next_state.has(k):
						next_state[k] = init_taco[k]
			cursor = _step_cursor(cursor, 3, pad_pressed)
			var move_dx: int = -1 if (pad_pressed & PAD_LEFT) != 0 else (1 if (pad_pressed & PAD_RIGHT) != 0 else 0)
			var move_dy: int = -1 if (pad_pressed & PAD_UP) != 0 else (1 if (pad_pressed & PAD_DOWN) != 0 else 0)
			var fire_primary: bool = (pad_pressed & PAD_CIRCLE) != 0
			var fire_bomb: bool = (pad_pressed & PAD_SQUARE) != 0
			SubgameOverlays.shooter_step_frame(
				next_state, move_dx, move_dy, (pad_pressed & PAD_TRIANGLE) != 0, fire_primary, fire_bomb
			)
			next_state["frame"] = int(next_state.get("frame", 0)) + 1
			next_state["bullet_count"] = (next_state.get("shots", []) as Array).size()
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					stage = 1
					action = "advance"
				else:
					next_state["result_tally"] = SubgameOverlays.shooter_compute_result_tally(next_state)
					stage = 2
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				next_state["result_tally"] = SubgameOverlays.shooter_compute_result_tally(next_state)
				action = "return_to_game"

		GAME_MODE_TT:
			if not next_state.has("player"):
				var tt_cfg: Dictionary = next_state.duplicate(true)
				if tt_cfg.has("state") and not (tt_cfg["state"] is int):
					tt_cfg.erase("state")
				var init_tt: Dictionary = SubgameOverlays.tt_create_state(tt_cfg)
				for k in init_tt.keys():
					if k == "state":
						next_state["tt_state"] = init_tt[k]
					elif not next_state.has(k):
						next_state[k] = init_tt[k]
			cursor = _step_cursor(cursor, 3, pad_pressed)
			var tt_player: Dictionary = next_state["player"]
			if (pad_pressed & PAD_LEFT) != 0:
				tt_player["x"] = clampi(int(tt_player["x"]) - 8, -120, 120)
			elif (pad_pressed & PAD_RIGHT) != 0:
				tt_player["x"] = clampi(int(tt_player["x"]) + 8, -120, 120)
			if (pad_pressed & PAD_UP) != 0:
				tt_player["y"] = clampi(int(tt_player["y"]) - 8, 56, 224)
				next_state["last_bell_collected"] = SubgameOverlays.tt_bell_collect(next_state, cursor % 3)
				next_state["score"] = int(next_state.get("score", 0)) + 500
			elif (pad_pressed & PAD_DOWN) != 0:
				tt_player["y"] = clampi(int(tt_player["y"]) + 8, 56, 224)
			next_state["elapsed_frames"] = int(next_state.get("elapsed_frames", 4250)) + 1
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					next_state["last_bell_hit"] = SubgameOverlays.tt_bell_hit_by_shot(next_state, cursor % 3, 0)
					next_state["bell_hits"] = int(next_state.get("bell_hit_counter", 0))
					stage = 1
					action = "advance"
				elif stage == 1:
					next_state["last_bell_collected"] = SubgameOverlays.tt_bell_collect(next_state, cursor % 3)
					next_state["score"] = int(next_state.get("score", 0)) + 500
					stage = 2
					action = "advance"
				else:
					stage = 3
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				SubgameOverlays.tt_trigger_smart_bomb(next_state)
				action = "return_to_game"

		GAME_MODE_EVENT_CG:
			cursor = _step_cursor(cursor, 3, pad_pressed)
			var ev_ovl := EventCgOverlay.new()
			ev_ovl.init_date_spot(
				girl_id,
				int(next_state.get("spot_place", EventCgOverlay.SPOT_KIRAMEKI_CENTRAL_PARK)),
				int(next_state.get("month", 4)),
				int(next_state.get("year", 96)),
				int(next_state.get("day", 15))
			)
			if (pad_pressed & PAD_CIRCLE) != 0:
				if stage == 0:
					if String(next_state.get("scene_kind", "")) == "ride_movie":
						next_state["ride_movie"] = ev_ovl.trigger_ride_movie(int(next_state.get("ride_id", 2)))
					else:
						ev_ovl.evaluate_date_spot_special_scene()
						next_state["active_scene_id"] = ev_ovl.active_scene_id
					stage = 1
					action = "advance"
				elif stage == 1:
					ev_ovl.evaluate_date_spot_special_scene()
					next_state["choice_result"] = ev_ovl.handle_choice(cursor)
					stage = 2
					action = "advance"
				else:
					ev_ovl.advance_step()
					stage = 3
					action = "return_to_game"
			elif (pad_pressed & PAD_CROSS) != 0:
				action = "return_to_game"

	next_state["cursor"] = cursor
	next_state["stage"] = stage
	next_state["girl_id"] = girl_id
	var out: Dictionary = next_state.duplicate(true)
	out["action"] = action
	out["mode"] = mode
	out["cursor"] = cursor
	out["stage"] = stage
	out["state"] = next_state
	return out


func render_mode(mode: int, state: Dictionary = {}) -> Dictionary:
	if not _ready:
		return {"error": "OverlayDispatcher is not initialized", "mode": mode}
	if not OVERLAY_CATALOG.has(mode):
		return {"error": "Unsupported overlay mode: 0x%02X" % mode, "mode": mode}

	var image: Image = null
	var id_prefix: String = String(OVERLAY_CATALOG[mode]["overlay"]).to_lower()
	var girl_id: int = int(state.get("girl_id", 0))
	var cursor: int = int(state.get("cursor", 0))
	var stage: int = int(state.get("stage", 0))

	match mode:
		GAME_MODE_OPTION:
			if state.has("cursor"):
				options_overlay.cursor = clampi(cursor, 0, 7)
			if state.has("stage"):
				options_overlay.mode_stage = stage
			image = options_overlay.render(vn_presenter)
		GAME_MODE_OLH:
			if state.has("cursor"):
				help_overlay.cursor = clampi(cursor, 0, 7)
			if state.has("stage"):
				help_overlay.mode_stage = stage
			image = help_overlay.render(vn_presenter)
		GAME_MODE_OPENING, GAME_MODE_ETC:
			if state.has("cursor"):
				title_overlay.cursor = clampi(cursor, 0, 1)
			if state.has("stage"):
				title_overlay.stage = stage
			image = title_overlay.render(vn_presenter)
		GAME_MODE_TEL:
			var sub := String(state.get("subscreen", ""))
			if sub.is_empty():
				sub = "day_select" if stage == 1 else ("call_response" if stage >= 2 else "place_select")
			if sub == "day_select":
				var slots: Array[Dictionary] = []
				var raw_slots: Array = state.get("candidate_slots", _default_tel_candidate_slots() if state.has("cursor") else [])
				for s in raw_slots:
					if s is Dictionary:
						slots.append(s)
				image = telephone_presenter.render_date_day_menu_screen(slots, cursor)
			elif sub == "call_response":
				image = telephone_presenter.render_girl_phone_call_vn_scene(
					girl_id,
					String(state.get("response_kind", "accepted"))
				)
			else:
				image = telephone_presenter.render_date_place_menu_screen(
					int(state.get("page", 0)),
					cursor
				)
		GAME_MODE_TEL_ACTION:
			image = telephone_presenter.render_girl_profile_notebook_screen(girl_id)
		GAME_MODE_DATE:
			var date_state: Dictionary = DateOverlay.create_default_state(state)
			if stage == 1:
				var out_c: Dictionary = date_presenter.render_overlay_three_choice_prompt(
					date_state,
					cursor
				)
				image = out_c.get("image")
			elif stage >= 2:
				var out_r: Dictionary = date_presenter.render_date_spot_reaction(
					date_state,
					int(date_state.get("chosen_index", cursor))
				)
				image = out_r.get("image")
			else:
				var out_a: Dictionary = date_presenter.render_date_arrival_greeting(date_state)
				image = out_a.get("image")
		GAME_MODE_AMUSEMENT_PARK_DATE:
			var date2_state: Dictionary = DateOverlay.create_default_state(state)
			var out_k: Dictionary = date_presenter.render_amusement_or_karaoke_subscene(
				date2_state,
				cursor,
				String(date2_state.get("subscene_kind", "karaoke_date2"))
			)
			image = out_k.get("image")
		GAME_MODE_GEKO:
			image = encounters_ending_presenter.render_walk_home_invitation_frame(
				cursor,
				girl_id if state.has("girl_id") else -1,
				state
			)
		GAME_MODE_EXAM:
			image = encounters_ending_presenter.render_term_exam_results_frame(state)
		GAME_MODE_OMIMAI, GAME_MODE_SHOUGATU, GAME_MODE_VALEN:
			image = encounters_ending_presenter.render_valentine_chocolate_frame(girl_id, state)
		GAME_MODE_MASTER, GAME_MODE_ENDING:
			image = encounters_ending_presenter.render_legendary_tree_confession_frame(girl_id, state)
		GAME_MODE_TAIIKU:
			image = school_events_presenter.render_sports_festival_hud(state)
		GAME_MODE_BUNKASAI, GAME_MODE_BUNKA_SD, GAME_MODE_BUNKAKEN, GAME_MODE_KANGEI:
			image = school_events_presenter.render_culture_festival_booth(state)
		GAME_MODE_SHUGAKU, GAME_MODE_EN_NICHI:
			image = school_events_presenter.render_school_trip_companion(state)
		GAME_MODE_RENSYU:
			image = school_events_presenter.render_club_practice_match_camp(state)
		GAME_MODE_RPG_BATTLE:
			var rpg_state: Dictionary = state.duplicate(true)
			if not rpg_state.has("player_hp"):
				var base_rpg: Dictionary = SubgameOverlays.rpg_create_state(state)
				for k in rpg_state.keys():
					base_rpg[k] = rpg_state[k]
				rpg_state = base_rpg
			if state.has("cursor") and not state.has("command_cursor"):
				rpg_state["command_cursor"] = clampi(cursor, 0, 2)
			image = subgame_presenter.render_rpg_battle(rpg_state)
		GAME_MODE_TACO:
			var taco_state: Dictionary = state.duplicate(true)
			if not taco_state.has("player"):
				var base_taco: Dictionary = SubgameOverlays.shooter_create_state(state)
				for k in taco_state.keys():
					base_taco[k] = taco_state[k]
				taco_state = base_taco
			image = subgame_presenter.render_taco_shooter(taco_state)
		GAME_MODE_TT:
			var tt_state: Dictionary = state.duplicate(true)
			if not tt_state.has("player"):
				var base_tt: Dictionary = SubgameOverlays.tt_create_state(state)
				for k in tt_state.keys():
					base_tt[k] = tt_state[k]
				tt_state = base_tt
			image = subgame_presenter.render_tt_twinbee(tt_state)
		GAME_MODE_EVENT_CG:
			var scene_kind := String(state.get("scene_kind", ""))
			if scene_kind.is_empty():
				if girl_id == 3:
					scene_kind = "ayako_karaoke"
				elif girl_id == 11:
					scene_kind = "miharu_encounter"
				else:
					scene_kind = "shiori_cg"
			if scene_kind == "ayako_karaoke":
				image = event_cg_presenter.render_ayako_karaoke_scene(cursor)
			elif scene_kind == "ride_movie":
				image = event_cg_presenter.render_ride_movie_scene()
			elif scene_kind == "miharu_encounter":
				image = event_cg_presenter.render_miharu_encounter_scene(cursor)
			else:
				image = event_cg_presenter.render_shiori_cg_scene(cursor)

	if image == null or image.is_empty():
		return {"error": "Overlay renderer returned empty image for mode 0x%02X" % mode, "mode": mode}

	var cmd_id := "overlay_%s_0x%02x" % [id_prefix, mode]
	return {
		"error": "",
		"id": cmd_id,
		"mode": mode,
		"overlay": OVERLAY_CATALOG[mode]["overlay"],
		"image": image,
		"commands": [{
			"id": cmd_id,
			"image": image,
			"position": Vector2i.ZERO,
			"blend_mode": 0,
		}],
	}
