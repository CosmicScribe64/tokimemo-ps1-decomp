class_name OverlayDispatcher
extends RefCounted
## T-29018: GameMode and overlay dispatcher for the Godot 4.7.2 port.
## Routes catalogued modes to input handlers and 320x240 presenters.
## Catalog membership does not establish original-game fidelity or route readiness;
## resource loading includes preview presenters.

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
const DayEventModes = preload("res://rules/day_event_modes.gd")

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

## T-39006: with saved state (production), only these step_mode branches run.
## System menus carry no event outcome. TACO/TT return only their own subgame
## payload fields. BUNKASAI runs CultureFestival instead of step_mode. The
## ending keeps its separate path. Every other event mode reads invented
## rosters, parameters or fixed rewards in step_mode and is rejected.
const PRODUCTION_MENU_MODES: Array[int] = [GAME_MODE_OPENING, GAME_MODE_ETC, GAME_MODE_OPTION, GAME_MODE_OLH]
const PRODUCTION_PAYLOAD_MODES: Array[int] = [GAME_MODE_TACO, GAME_MODE_TT]
const PRODUCTION_REJECTIONS: Dictionary = {
	GAME_MODE_TEL: "phone_date_invitation_resolve stress/guts/affection/syoushin writes, girl_scores closeness and appointment flag D_80140560 are not ported; step_mode swaps looks/closeness and defaults closeness to 80",
	GAME_MODE_TEL_ACTION: "profile/call branch reads the invented preview roster",
	GAME_MODE_DATE: "date spot state comes from DateOverlay.create_default_state, not saved state",
	GAME_MODE_AMUSEMENT_PARK_DATE: "date state comes from DateOverlay.create_default_state, not saved state",
	GAME_MODE_GEKO: "walk-home topic inputs are fixed and the roster is invented",
	GAME_MODE_EXAM: "exam ranking defaults player total 485, zero flags and the invented roster",
	GAME_MODE_OMIMAI: "sick-visit dialogue inputs are fixed",
	GAME_MODE_SHOUGATU: "New Year/Christmas use the invented roster and a fixed eligibility list",
	GAME_MODE_VALEN: "Valentine candidates use fixed affection lists and the invented roster",
	GAME_MODE_MASTER: "club master defaults practice points to 385 and fixed companion lists",
	GAME_MODE_TAIIKU: "sports rewards use the invented roster and default athletics 80",
	GAME_MODE_BUNKA_SD: "stage show evaluation uses fixed inputs; BUNKA_SD is entered from BUNKASAI scenes",
	GAME_MODE_BUNKAKEN: "culture tour visitor list and invite inputs are fixed",
	GAME_MODE_SHUGAKU: "school trip deltas are fixed",
	GAME_MODE_KANGEI: "camp day uses fixed parameters",
	GAME_MODE_RENSYU: "practice match defaults practice points and match count",
	GAME_MODE_EN_NICHI: "stall game inputs are fixed",
	GAME_MODE_RPG_BATTLE: "battle uses a fixed rand_seed instead of native rand()",
	GAME_MODE_EVENT_CG: "date-spot special scene defaults spot, month and year",
}
const CultureFestival = preload("res://rules/culture_festival.gd")
const NativeRng = preload("res://core/native_rng.gd")

## Validated original rule tables (school-day table pack); set by the controller.
var rule_tables: Dictionary = {}

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
	if not telephone_presenter.open_telephone_preview(hud_manifest_path, font_root, date_pack_path):
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


## Root hooks 4 and 6 (Phase 4 WP-J, WP-I): binds the active English text
## layer and image store on every live presenter, or clears them (Japanese:
## both null). No producer writes presenter binding rows yet, so bind_english
## gets none and binds only the exact-identity image replacements. Callers
## re-render the current frame afterwards. Presentation only.
func apply_language(layer: RefCounted, images: RefCounted) -> void:
	if not _ready:
		return
	for presenter: RefCounted in [vn_presenter, telephone_presenter, date_presenter, encounters_ending_presenter,
			school_events_presenter, subgame_presenter, event_cg_presenter]:
		if presenter == null:
			continue
		if presenter.has_method("bind_english"):
			if layer == null and images == null:
				presenter.clear_english()
			else:
				presenter.bind_english(layer, [], images)
		if presenter.has_method("set_translated_text"):
			presenter.set_translated_text(layer)


static func supported_modes() -> Array[int]:
	var modes: Array[int] = []
	for k: int in OVERLAY_CATALOG.keys():
		modes.append(k)
	modes.sort()
	return modes


func handles_mode(mode: int) -> bool:
	var disp_mode: int = DayEventModes.dispatcher_mode_for(mode)
	return OVERLAY_CATALOG.has(mode) or OVERLAY_CATALOG.has(disp_mode)


static func is_interactive_overlay_mode(mode: int) -> bool:
	return OVERLAY_CATALOG.has(mode) or DayEventModes.NATIVE_TO_DISPATCHER_MODE.has(mode)


func describe_mode(mode: int) -> Dictionary:
	if OVERLAY_CATALOG.has(mode):
		return OVERLAY_CATALOG[mode]
	var disp_mode: int = DayEventModes.dispatcher_mode_for(mode)
	return OVERLAY_CATALOG.get(disp_mode, {})


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


## Invented roster for step_mode previews only. step_overlay with saved state
## never reaches a step_mode branch that reads it (see PRODUCTION_* below).
static func _preview_girls_roster() -> Array[Dictionary]:
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
	var disp_mode: int = DayEventModes.dispatcher_mode_for(mode) if not OVERLAY_CATALOG.has(mode) else mode
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

	match disp_mode:
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

			var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(next_state)
			var eff_year: int = int(live_ctx["year"]) if int(live_ctx["year"]) > 0 else int(next_state.get("year", 0))
			var eff_month: int = int(live_ctx["month"]) if int(live_ctx["month"]) > 0 else int(next_state.get("month", 1))
			var eff_day: int = int(live_ctx["day"]) if int(live_ctx["day"]) > 0 else int(next_state.get("day", 1))
			var progress: Array = live_ctx["progress_state"] if not (live_ctx["progress_state"] as Array).is_empty() else [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
			var qflags: Dictionary = live_ctx["quarter_flags"] if live_ctx["quarter_flags"] is Dictionary else {}

			if stage == 0:
				var page: int = int(next_state.get("page", 0))
				var avail: Array[int] = TelephoneOverlay.build_date_place_availability(
					page, eff_year, eff_month, eff_day, progress, qflags, eff_month
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
						var cal0: Dictionary = live_ctx.get("cal0", {})
						var cal1: Dictionary = live_ctx.get("cal1", {})
						var live_slots: Array = []
						if not cal0.is_empty() and not cal1.is_empty():
							live_slots = TelephoneOverlay.build_available_date_slots(
								eff_year, eff_month, eff_day, cal0, cal1, live_ctx.get("appointment_words", [0, 0, 0, 0, 0, 0, 0, 0])
							)
						next_state["candidate_slots"] = live_slots if not live_slots.is_empty() else _default_tel_candidate_slots()
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
					var chosen_slot: Dictionary = slots[cursor] if cursor < slots.size() else {"year": eff_year, "month": eff_month, "day": eff_day}
					next_state["chosen_slot"] = chosen_slot
					var place_id: int = int(next_state.get("place_id", TelephoneOverlay.PLACE_AQUARIUM))
					var stats_arr: Array = live_ctx.get("stats", [])
					var trivia_val: int = int(stats_arr[5]) if stats_arr.size() > 5 else int(next_state.get("trivia", 120))
					var looks_val: int = int(stats_arr[6]) if stats_arr.size() > 6 else int(next_state.get("looks", 80))
					var closeness_val: int = int(next_state.get("closeness", 80))
					var eval_res: Dictionary = TelephoneOverlay.evaluate_invitation(girl_id, place_id, trivia_val, closeness_val, looks_val)
					next_state["invitation_evaluation"] = eval_res
					var default_resp: String = "accepted" if bool(eval_res.get("accepted", true)) else String(eval_res.get("outcome", "refused_soft"))
					next_state["response_kind"] = String(next_state.get("response_kind", default_resp))
					if bool(eval_res.get("accepted", false)) and next_state.get("appointment_words") is Array:
						var appt_words: Array = (next_state["appointment_words"] as Array).duplicate()
						TelephoneOverlay.book_appointment(
							appt_words,
							girl_id,
							place_id,
							int(chosen_slot.get("year", eff_year)),
							int(chosen_slot.get("month", eff_month)),
							int(chosen_slot.get("day", eff_day))
						)
						next_state["appointment_words"] = appt_words
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
			var roster: Array[Dictionary] = _preview_girls_roster()
			next_state["appraisal_list"] = TelephoneOverlay.build_full_appraisal_list(roster)
			cursor = _step_cursor(cursor, 11, pad_pressed)
			if (pad_pressed & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) != 0:
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
			var girls_list: Array[Dictionary] = _preview_girls_roster()
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
						int(next_state.get("player_total", 485)), _preview_girls_roster(), empty_flags
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
			var girls_list: Array[Dictionary] = _preview_girls_roster()
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
			var girls_list: Array[Dictionary] = _preview_girls_roster()
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
						int(next_state.get("club_id", 3)), _preview_girls_roster(), [80, 75, 70, 85, 80, 70, 65, 60, 60, 60, 60]
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
						int(next_state.get("player_place", 1)), girl_id, _preview_girls_roster()
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
			if not next_state.has("player_hp"):
				var rpg_cfg: Dictionary = next_state.duplicate(true)
				if int(rpg_cfg.get("stage", 0)) == 0:
					rpg_cfg.erase("stage")
				if rpg_cfg.has("enemy_hp") and not rpg_cfg.has("enemy_slot_gauges") and int(rpg_cfg["enemy_hp"]) > 0:
					rpg_cfg["enemy_slot_gauges"] = [int(rpg_cfg["enemy_hp"]), 0, 0]
				if state.has("cursor") and not rpg_cfg.has("command_cursor"):
					rpg_cfg["command_cursor"] = cursor
				var init_rpg: Dictionary = SubgameOverlays.rpg_create_state(rpg_cfg)
				for k in init_rpg.keys():
					if k == "stage" or k == "enemy_slot_gauges" or not next_state.has(k):
						next_state[k] = init_rpg[k]
			elif state.has("cursor") and not state.has("command_cursor"):
				next_state["command_cursor"] = cursor
			if not next_state.has("enable_bancho_escalation_override"):
				next_state["enable_bancho_escalation_override"] = bool(next_state.get("enable_bancho_escalation", false))
			SubgameOverlays.rpg_step_interactive(
				next_state, pad_pressed, int(next_state.get("rand_seed", 512))
			)
			cursor = int(next_state.get("command_cursor", cursor))
			stage = int(next_state.get("stage", stage))
			action = String(next_state.get("action", "stepped"))
			if bool(next_state.get("completed", false)):
				if not next_state.has("return_payload"):
					next_state["return_payload"] = SubgameOverlays.rpg_build_return_payload(next_state)
				action = "return_to_game"

		GAME_MODE_TACO:
			if not next_state.has("player"):
				var taco_cfg: Dictionary = next_state.duplicate(true)
				if int(taco_cfg.get("stage", 0)) < SubgameOverlays.SHOOTER_STAGE_RESULT:
					taco_cfg.erase("stage")
				var init_taco: Dictionary = SubgameOverlays.shooter_create_state(taco_cfg)
				for k in init_taco.keys():
					if k == "stage" or not next_state.has(k):
						next_state[k] = init_taco[k]
			if next_state.get("shots") is Array:
				for shot_item: Variant in next_state["shots"]:
					if shot_item is Dictionary and not shot_item.has("target_screen") and shot_item.has("target_screen_x"):
						shot_item["target_screen"] = Vector2i(int(shot_item["target_screen_x"]), int(shot_item["target_screen_y"]))
			SubgameOverlays.shooter_step_interactive(
				next_state, pad_pressed, int(next_state.get("pad_held", pad_pressed))
			)
			if next_state.get("shots") is Array:
				for shot_item: Variant in next_state["shots"]:
					if shot_item is Dictionary and shot_item.get("target_screen") is Vector2i:
						var ts: Vector2i = shot_item["target_screen"]
						shot_item["target_screen_x"] = int(ts.x)
						shot_item["target_screen_y"] = int(ts.y)
						shot_item.erase("target_screen")
			next_state["frame"] = int(next_state.get("frame_count", 0))
			next_state["bullet_count"] = (next_state.get("shots", []) as Array).size()
			stage = int(next_state.get("stage", SubgameOverlays.SHOOTER_STAGE_STAGE1))
			action = String(next_state.get("action", "stepped"))
			if bool(next_state.get("completed", false)):
				if not next_state.has("return_payload"):
					next_state["return_payload"] = SubgameOverlays.shooter_build_return_payload(next_state)
				action = "return_to_game"

		GAME_MODE_TT:
			if next_state.has("state") and not (next_state["state"] is int):
				if next_state.has("tt_state") and next_state["tt_state"] is int:
					next_state["state"] = int(next_state["tt_state"])
				else:
					next_state.erase("state")
			if not next_state.has("player"):
				var tt_cfg: Dictionary = next_state.duplicate(true)
				if tt_cfg.has("state") and not (tt_cfg["state"] is int):
					tt_cfg.erase("state")
				var init_tt: Dictionary = SubgameOverlays.tt_create_state(tt_cfg)
				for k in init_tt.keys():
					if k == "state":
						next_state["state"] = init_tt[k]
						next_state["tt_state"] = init_tt[k]
					elif not next_state.has(k):
						next_state[k] = init_tt[k]
			SubgameOverlays.tt_step_interactive(
				next_state, pad_pressed, int(next_state.get("pad_held", pad_pressed))
			)
			var cur_tt_state: int = int(next_state.get("state", SubgameOverlays.TT_STATE_STAGE))
			next_state["tt_state"] = cur_tt_state
			next_state["bell_hits"] = int(next_state.get("bell_hit_counter", 0))
			stage = cur_tt_state
			action = String(next_state.get("action", "stepped"))
			if bool(next_state.get("completed", false)):
				if not next_state.has("return_payload"):
					next_state["return_payload"] = SubgameOverlays.tt_build_return_payload(next_state)
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


func init_overlay(mode: int, rule_state: Dictionary = {}, event: Array = [], _rng: Variant = null) -> Dictionary:
	var event_opcode: int = int(event[0]) if not event.is_empty() else -1
	var disp_mode: int = DayEventModes.dispatcher_mode_for(mode, event_opcode)
	if disp_mode == GAME_MODE_ENDING or mode == EndingOverlay.GAME_MODE_ENDING:
		var seq: Dictionary = EndingOverlay.create_graduation_sequence(rule_state)
		seq["mode"] = mode
		seq["dispatcher_mode"] = GAME_MODE_ENDING
		seq["event_opcode"] = event_opcode
		return seq

	var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(rule_state)
	var girl_id: int = int(rule_state.get("current_girl", 0))
	if event.size() > 1 and int(event[1]) >= 0 and int(event[1]) < 13:
		girl_id = int(event[1])
	var club_id: int = (int(rule_state.get("player_flags", 0)) >> 12) & 0xF
	return {
		"mode": mode,
		"dispatcher_mode": disp_mode,
		"event_opcode": event_opcode,
		"girl_id": girl_id,
		"club_id": club_id,
		"stage": 0,
		"cursor": 0,
		"page": 0,
		"subscreen": "place_select" if disp_mode == GAME_MODE_TEL else "",
		"year": int(live_ctx["year"]),
		"month": int(live_ctx["month"]),
		"day": int(live_ctx["day"]),
		"weekday": int(live_ctx["weekday"]),
		"day_kind": int(live_ctx["day_kind"]),
		"day_count": int(live_ctx["day_count"]),
		"player_birthday": int(live_ctx["player_birthday"]),
		"stats": live_ctx["stats"],
		"progress_state": live_ctx["progress_state"],
		"quarter_flags": live_ctx["quarter_flags"],
		"cal0": live_ctx["cal0"],
		"cal1": live_ctx["cal1"],
		"appointment_words": live_ctx["appointment_words"],
		"completed": false,
		"deltas": {},
	}


static func _ending_step_out(mode: int, next_seq: Dictionary) -> Dictionary:
	var done: bool = bool(next_seq.get("completed", false))
	var ending_deltas: Dictionary = {}
	if done:
		ending_deltas = {
			"ending_completed": 1,
			"confessed_girl": int(next_seq.get("confessed_girl", EndingOverlay.GIRL_YOSHIO)),
			"ending_type": String(next_seq.get("ending_type", "yoshio_bad_ending")),
			"ending_phases": next_seq.get("history", []).duplicate(),
			"clear_save_completed": bool(next_seq.get("clear_save_completed", false)),
			"album_clear_mask": int(next_seq.get("album_clear_mask", 0)),
			"clear_name_slot": bool(next_seq.get("clear_name_slot", false)),
		}
	next_seq["deltas"] = ending_deltas
	return {
		"completed": done,
		"action": String(next_seq.get("action", "stepped")),
		"mode": mode,
		"dispatcher_mode": GAME_MODE_ENDING,
		"state": next_seq,
		"deltas": ending_deltas,
	}

## T-39007: the ending step that delivers a host clear-save completion. It
## replaces the input step of the frame the completion arrives in, so a
## successful save finishes the ending without a stored "saved" state.
static func clear_save_step_out(mode: int, overlay_state: Dictionary, payload: Dictionary) -> Dictionary:
	var applied: Dictionary = EndingOverlay.apply_clear_save_completion(overlay_state, payload)
	if not applied["error"].is_empty():
		return {"error": applied["error"]}
	var out: Dictionary = _ending_step_out(mode, applied["state"])
	out["error"] = ""
	return out

func step_overlay(mode: int, overlay_state: Dictionary, pad_pressed: int, rule_state: Dictionary = {}, rng: Variant = null) -> Dictionary:
	var disp_mode: int = int(overlay_state.get("dispatcher_mode", DayEventModes.dispatcher_mode_for(mode)))
	var current_state: Dictionary = overlay_state.duplicate(true)
	if current_state.is_empty():
		current_state = init_overlay(mode, rule_state, [], rng)
		disp_mode = int(current_state.get("dispatcher_mode", disp_mode))

	# Interactive menu/VN overlays wait for explicit player input; real-time action subgames (TACO, TT) step frames without auto-completing.
	if pad_pressed == 0 and disp_mode not in [GAME_MODE_TACO, GAME_MODE_TT]:
		current_state["completed"] = false
		current_state["action"] = "wait_input"
		return {
			"completed": false,
			"action": "wait_input",
			"mode": mode,
			"dispatcher_mode": disp_mode,
			"state": current_state,
			"deltas": {},
		}

	if disp_mode == GAME_MODE_ENDING or mode == EndingOverlay.GAME_MODE_ENDING or mode == EndingOverlay.GAME_MODE_ETC_C2:
		return _ending_step_out(mode, EndingOverlay.step_graduation_sequence(current_state, pad_pressed, rule_state))

	if not rule_state.is_empty():
		if disp_mode == GAME_MODE_BUNKASAI:
			return _step_culture_festival(mode, current_state, pad_pressed, rule_state, rng)
		if disp_mode not in PRODUCTION_MENU_MODES and disp_mode not in PRODUCTION_PAYLOAD_MODES:
			return _production_rejection(mode, disp_mode, current_state,
				String(PRODUCTION_REJECTIONS.get(disp_mode, "no source-backed production handler")))
		var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(rule_state)
		for k: String in ["year", "month", "day", "weekday", "day_kind", "day_count", "player_birthday", "stats", "progress_state", "quarter_flags", "cal0", "cal1"]:
			if not current_state.has(k) or (current_state[k] is int and int(current_state[k]) <= 0):
				current_state[k] = live_ctx[k]
		if not current_state.has("appointment_words") and live_ctx.has("appointment_words"):
			current_state["appointment_words"] = live_ctx["appointment_words"]

	var step_res: Dictionary = step_mode(disp_mode, current_state, pad_pressed)
	var next_state: Dictionary = step_res.get("state", step_res).duplicate(true)
	var act: String = String(step_res.get("action", "stepped"))
	var completed: bool = (act == "return_to_game" or bool(next_state.get("completed", false)))
	# Without saved state this is a preview: it exits but writes nothing.
	var deltas: Dictionary = _payload_outcome(disp_mode, next_state, rule_state) if completed and not rule_state.is_empty() else {}

	next_state["completed"] = completed
	next_state["action"] = act
	next_state["deltas"] = deltas
	return {
		"completed": completed,
		"action": act,
		"mode": mode,
		"dispatcher_mode": disp_mode,
		"cursor": int(next_state.get("cursor", 0)),
		"stage": int(next_state.get("stage", 0)),
		"state": next_state,
		"deltas": deltas,
	}


func _production_rejection(mode: int, disp_mode: int, current_state: Dictionary, reason: String) -> Dictionary:
	var message: String = "overlay 0x%02X (dispatcher 0x%02X) refused with saved state: %s" % [mode, disp_mode, reason]
	current_state["completed"] = false
	current_state["action"] = "rejected"
	return {
		"completed": false,
		"action": "rejected",
		"error": message,
		"mode": mode,
		"dispatcher_mode": disp_mode,
		"state": current_state,
		"deltas": {},
	}


## BUNKASAI with saved state: CIRCLE runs the source phase 0..7 selection
## (CultureFestival.plan), the next CIRCLE applies the phase 8 writes. The
## festival has no cancel exit in source, so CROSS is refused without effect.
func _step_culture_festival(mode: int, current_state: Dictionary, pad_pressed: int, rule_state: Dictionary, rng: Variant) -> Dictionary:
	var stage: int = int(current_state.get("stage", 0))
	var out: Dictionary = {"completed": false, "mode": mode, "dispatcher_mode": GAME_MODE_BUNKASAI, "deltas": {}}
	if (pad_pressed & PAD_CIRCLE) == 0:
		out["action"] = "cancel_rejected" if (pad_pressed & PAD_CROSS) != 0 else "wait_input"
		current_state["action"] = out["action"]
		out["state"] = current_state
		return out
	if rule_tables.is_empty():
		return _production_rejection(mode, GAME_MODE_BUNKASAI, current_state, "validated rule tables are not attached")
	var native_rng: NativeRng = rng as NativeRng
	if stage == 0:
		var festival_plan: Dictionary = CultureFestival.plan(rule_state, rule_tables, native_rng)
		if not String(festival_plan["error"]).is_empty():
			return _production_rejection(mode, GAME_MODE_BUNKASAI, current_state, festival_plan["error"])
		current_state["culture_plan"] = festival_plan
		current_state["girl_id"] = festival_plan["girl"]
		current_state["stage"] = 1
		current_state["action"] = "advance"
		out["action"] = "advance"
		out["stage"] = 1
		out["state"] = current_state
		return out
	if not current_state.get("culture_plan") is Dictionary:
		return _production_rejection(mode, GAME_MODE_BUNKASAI, current_state, "phase 8 reached without a phase 0..7 plan")
	var written: Dictionary = {}
	for key: String in ["girls", "event_log", "event_log_count", "params", "clubs", "player_flags", "year", "month", "day", "weekday", "day_event_done_bits"]:
		if rule_state.has(key):
			written[key] = rule_state[key].duplicate(true) if rule_state[key] is Array else rule_state[key]
	var applied: Dictionary = CultureFestival.apply(written, current_state["culture_plan"], rule_tables)
	var deltas: Dictionary = {
		"source": "src/ovl/culture_festival/festival_entry.c::culture_result_apply,culture_event_log_add",
		"writes": applied["writes"],
		"state_writes": {"girls": written["girls"], "event_log": written["event_log"], "event_log_count": written["event_log_count"]},
	}
	current_state["stage"] = 2
	current_state["completed"] = true
	current_state["action"] = "return_to_game"
	current_state["deltas"] = deltas
	out["completed"] = true
	out["action"] = "return_to_game"
	out["stage"] = 2
	out["state"] = current_state
	out["deltas"] = deltas
	return out


## Production completion fields: subgame payload values only, no invented
## affection/tokimeki and no defaults for absent payload fields.
func _payload_outcome(disp_mode: int, next_state: Dictionary, rule_state: Dictionary) -> Dictionary:
	if disp_mode not in PRODUCTION_PAYLOAD_MODES:
		return {}
	var payload: Dictionary = next_state.get("return_payload", {}) if next_state.get("return_payload") is Dictionary else {}
	var deltas: Dictionary = {"source": "subgame return payload"}
	# delta key -> payload key
	var keys: Dictionary = {"high_scores": "high_scores", "high_score_names": "high_score_names",
		"stages_cleared": "stages_cleared", "score": "score", "total_score": "total_score",
		"menu_hit_3": "menu_hit_3", "system_data_flag": "system_data_flag"}
	if disp_mode == GAME_MODE_TT:
		SubgameOverlays.tt_apply_writeback_to_rule_state(next_state, rule_state)
		keys = {"tt_stages_cleared": "tt_stages_cleared", "tt_rankings": "rankings", "tt_last_time_frames": "elapsed_frames"}
	for key: String in keys:
		var source_key: String = keys[key]
		if payload.has(source_key):
			deltas[key] = payload[source_key].duplicate(true) if payload[source_key] is Array else payload[source_key]
	deltas["return_payload"] = payload.duplicate(true)
	return deltas


func render_mode(mode: int, state: Dictionary = {}) -> Dictionary:
	if not _ready:
		return {"error": "OverlayDispatcher is not initialized", "mode": mode}
	var disp_mode: int = DayEventModes.dispatcher_mode_for(mode) if not OVERLAY_CATALOG.has(mode) else mode
	if not OVERLAY_CATALOG.has(disp_mode):
		return {"error": "Unsupported overlay mode: 0x%02X" % mode, "mode": mode}

	var image: Image = null
	var id_prefix: String = String(OVERLAY_CATALOG[disp_mode]["overlay"]).to_lower()
	var girl_id: int = int(state.get("girl_id", 0))
	var cursor: int = int(state.get("cursor", 0))
	var stage: int = int(state.get("stage", 0))

	match disp_mode:
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
			telephone_presenter.set_live_context(TelephoneOverlay.extract_live_context(state))
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
				var prompt_dict: Dictionary = state.get("native_prompt", {
					"schema": "tokimemo.phone-wait-marker-view.v1",
					"source_function": "talk_text_step",
					"waiting_for_input": true,
					"k_disp_inc_result": 1,
					"voice_mode": 0,
					"speaker_voice_state": 0,
					"marker_visible": false,
					"marker_native_y": 0x48,
				})
				image = telephone_presenter.render_girl_phone_call_vn_scene(
					girl_id,
					String(state.get("response_kind", "accepted")),
					[],
					-1, -1, -1, -1, -1,
					Vector2i(20, 14),
					prompt_dict
				)
			else:
				var avail_arr: Array = state.get("avail_table", []) if state.get("avail_table") is Array else []
				image = telephone_presenter.render_date_place_menu_screen(
					int(state.get("page", 0)),
					cursor,
					avail_arr
				)
		GAME_MODE_TEL_ACTION:
			telephone_presenter.set_live_context(TelephoneOverlay.extract_live_context(state))
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
			if tt_state.has("state") and not (tt_state["state"] is int):
				tt_state["state"] = int(tt_state.get("tt_state", SubgameOverlays.TT_STATE_STAGE))
			if not tt_state.has("player"):
				var base_tt: Dictionary = SubgameOverlays.tt_create_state(tt_state)
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
		"overlay": OVERLAY_CATALOG[disp_mode]["overlay"],
		"image": image,
		"commands": [{
			"id": cmd_id,
			"image": image,
			"position": Vector2i.ZERO,
			"blend_mode": 0,
		}],
	}
