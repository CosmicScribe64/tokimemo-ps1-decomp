extends RefCounted
## GEKO player birthday, native mode 0x56 (queue type 0x33):
## src/ovl/girl_encounters/player_birthday.c. State effects only; the 29-entry
## step table (encounter_player_birthday_steps, GEKO 0x80144E94) and the P_BIRTH
## script sections come from tools/port/player_birthday_content.py.
##
## Steps this file implements, by source function:
##   0  encounter_player_birthday_init           repeat guard, done bit 0x04
##   1  encounter_player_birthday_girl_select    visitor choice or step 0x17
##   2  encounter_player_birthday_audio_load     marker-5 delay
##   6  encounter_player_birthday_closing_init   event_log_add(0xD, 0x25)
##   22/27 encounter_player_birthday_finish      New Year redirect or day_event_next(1)
##   25 encounter_player_birthday_default_script_select  section 11
## The other entries are shared scene services (CD waits, backgrounds,
## portraits, wipes, girl entry/exit, talk) owned by the presentation layer.

const Scheduler = preload("res://rules/scheduler.gd")
const Math = preload("res://core/native_math.gd")

const STEP_COUNT := 29
const STEP_DEFAULT_VISITOR := 0x17
const DONE_SEEN := 0x04
const DONE_CHIME_OFF := 0x08
const DONE_NEW_YEAR := 0x40
const DONE_EXCLUDE_TALK_GIRL := 0x80
const GAME_MODE_DAY_SCREEN := 0x31
const GAME_MODE_NEW_YEAR_A := 0x52
const GIRL_REI := 12
## GirlFlagBits.met (include/game/girl.h) is bit 1.
const FLAG_MET := 0x02
const NO_GIRL := 0xFF
const AUDIO_DELAY_MARKER := 5
const AUDIO_DELAY_TICKS := 0x80
const CLOSING_LOG_SCENE := 0xD
const CLOSING_LOG_FLAG := 0x25
const EVENT_LOG_LAST := 0xFE
## encounter_player_birthday_girl_script_select: girl -> P_BIRTH section.
const GIRL_SCRIPT_SECTION: Dictionary = {
	0: 1, 1: 6, 2: 3, 3: 4, 4: 10, 5: 7, 6: 8, 7: 5, 8: 0, 9: 9, 10: 12,
}
const DEFAULT_SCRIPT_SECTION := 11
const RANK_TABLE_KEYS: Array[String] = ["affection_weights", "tokimeki_weights",
	"ladder_kinds", "ladder_bases", "bonus_param_masks", "param_bonus"]


## Inputs read by these rules; a missing one is refused, never defaulted.
static func missing_input(data: Dictionary, tables: Dictionary) -> String:
	for key: String in ["girls", "params", "day_event_done_bits", "current_girl", "year",
			"month", "day", "weekday", "day_event_index", "calendar"]:
		if not data.has(key):
			return "player birthday: saved state lacks " + key
	if not data.get("event_log") is Array or not data.get("event_log_count") is int:
		return "player birthday: saved state lacks event_log/event_log_count written by event_log_add"
	for key: String in RANK_TABLE_KEYS:
		if not tables.has(key):
			return "player birthday: validated rule tables lack " + key
	if (int(data["day_event_done_bits"]) & DONE_EXCLUDE_TALK_GIRL) != 0 and not data.get("talk_girl_selector_plus_one") is int:
		return "player birthday: done bit 0x80 needs talk_girl_selector_plus_one, which the saved state lacks"
	return ""


## Step 0. Returns {"finish": true} when done bit 0x04 was already set
## (the source then runs encounter_player_birthday_finish), otherwise sets it,
## clears the script cursor and continues with the next step.
static func init_step(data: Dictionary) -> Dictionary:
	var done: int = int(data["day_event_done_bits"])
	if (done & DONE_SEEN) != 0:
		return {"finish": true, "group": -1, "line": -1}
	data["day_event_done_bits"] = done | DONE_SEEN
	return {"finish": false, "group": 0, "line": 0}


## get_h_yuukou / get_h_tokimeki index their girl by girl % 13.
static func _sum(data: Dictionary, girl: int, tables: Dictionary) -> int:
	var index: int = girl % 13
	return Scheduler._weighted_affection(data["girls"][index], index, tables) \
		+ Scheduler._weighted_tokimeki(data, index, tables)


## Step 1, encounter_player_birthday_girl_select. Met girls 0..10: the lowest
## get_g_zyotai_s & 0x7F wins; on equal state the candidate must have a
## strictly greater yuukou + tokimeki than the incumbent. Before any winner the
## incumbent is 0xFF, whose sums are girl 0xFF % 13 = 8's. With done bit 0x80
## a girl whose id + 1 equals talk_girl_selector_plus_one is skipped.
static func select_girl(data: Dictionary, tables: Dictionary) -> Dictionary:
	var done: int = int(data["day_event_done_bits"])
	var excluded: int = -1
	if (done & DONE_EXCLUDE_TALK_GIRL) != 0:
		excluded = int(data["talk_girl_selector_plus_one"])
	var best_girl: int = NO_GIRL
	var best_state: int = 4
	for i: int in range(11):
		if (int(data["girls"][i]["flags"]) & FLAG_MET) == 0:
			continue
		if excluded >= 0 and i + 1 == excluded:
			continue
		var state: int = Scheduler.girl_state_rank(data, i, tables) & 0x7F
		if state < best_state or (state == best_state and _sum(data, best_girl, tables) < _sum(data, i, tables)):
			best_girl = i
			best_state = state
	data["talk_girl_selector_plus_one"] = 0
	var final_state: int = Scheduler.girl_state_rank(data, best_girl % 13, tables) & 0x7F
	if final_state >= 2 or best_girl == NO_GIRL or (done & DONE_CHIME_OFF) != 0:
		return {"next_step": STEP_DEFAULT_VISITOR, "girl": best_girl, "section": -1}
	data["current_girl"] = best_girl
	return {"next_step": 2, "girl": best_girl, "section": int(GIRL_SCRIPT_SECTION[best_girl])}


## Step 2: frames to hold before the bank load when today's marker is 5.
static func audio_delay_ticks(data: Dictionary) -> int:
	var day: int = int(data["day"])
	if int(data["calendar"][0]["marker"][day]) == AUDIO_DELAY_MARKER:
		return AUDIO_DELAY_TICKS
	return 0


## Step 4: the door-chime cue is shown unless done bit 0x08 is set.
static func chime_enabled(data: Dictionary) -> bool:
	return (int(data["day_event_done_bits"]) & DONE_CHIME_OFF) == 0


## Step 6, the event_log_add(0xD, 0x25) of encounter_player_birthday_closing_init.
## talk_clear has just zeroed talk_rank_override (talk_work[30]), so the rank
## is get_g_zyotai_h of the current girl, or 0 for Rei.
static func closing_event_log(data: Dictionary, tables: Dictionary) -> Dictionary:
	var girl: int = int(data["current_girl"])
	var rank: int = 0 if girl == GIRL_REI else Scheduler.girl_log_rank(data, girl % 13, tables)
	var entry: Dictionary = {"girl": girl, "scene": CLOSING_LOG_SCENE, "flag": CLOSING_LOG_FLAG,
		"rank": rank, "year": int(data["year"]), "month": int(data["month"]),
		"day": int(data["day"]), "weekday": int(data["weekday"])}
	var count: int = int(data["event_log_count"])
	var log: Array = data["event_log"]
	while log.size() <= count:
		log.append({})
	log[count] = entry
	count += 1
	if count >= 0xFF:
		count = EVENT_LOG_LAST
	data["event_log_count"] = count
	return entry


## Steps 22/27, encounter_player_birthday_finish after its audio restore:
## done bit 0x40 redirects to GAME_MODE_NEW_YEAR_A (audio command 0xCF);
## otherwise day_event_next(1) advances the queue to the day screen.
static func finish(data: Dictionary) -> Dictionary:
	if (int(data["day_event_done_bits"]) & DONE_NEW_YEAR) != 0:
		return {"mode": GAME_MODE_NEW_YEAR_A, "audio_command": 0xCF}
	data["day_event_index"] = Math.u8(int(data["day_event_index"]) + 1)
	return {"mode": GAME_MODE_DAY_SCREEN, "audio_command": -1}


## The scene presentation services this mode needs, by step-table function.
## OverlayDispatcher refuses mode 0x56 while any of them has no port.
const PRESENTATION_SERVICES: Array[String] = [
	"scene_wait_cd_read", "dec_init", "change_dec_bg", "read_bustup",
	"portrait_show_checked", "scene_bg_wipe_in", "place_init",
	"normal_date_girl_in", "normal_date_girl_out", "normal_date_bg_out",
	"encounter_player_birthday_talk",
]
