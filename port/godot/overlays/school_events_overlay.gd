class_name SchoolEventsOverlay
extends RefCounted
## T-29018 Lane 6: School Festivals, Sports Festival, School Trip & Summer Camp Overlays
## Source-backed state machines, mechanics and rules for:
## - TAIIKU (Sports Festival): 4-event selection, race stamina/mashing, ranking rewards, folk dance
## - BUNKASAI / BUNKA_SD / BUNKAKEN (Culture Festival): Booth prep, companion tour, stage shows
## - SHUGAKU (School Trip): 4-day itinerary, destination routing, sightseeing companion, hotel corridor
## - KANGEI / RENSYU / EN_NICHI (Summer Camp, Practice Match, Festival Stalls)

# ==============================================================================
# TAIIKU (Sports Festival) Constants & Enums
# ==============================================================================
enum SportsCompetition {
	DASH_100M = 0,       # １００ｍ走
	OBSTACLE_RACE = 1,   # 障害物競走
	THREE_LEGGED = 2,    # 二人三脚
	BREAD_EATING = 3,    # パン食い競走
}

# 16.16 fixed-point constants from runner_race_dispatch.c
const RUNNER_POS_STEP := 0x18000
const RUNNER_STAGE1_THRESHOLD := 0x140
const RUNNER_STAGE2_THRESHOLD := 0x280
const RUNNER_INPUT_SPEED_DEFAULT := 0x1000
const RUNNER_DECELERATION := -0x300

# ==============================================================================
# BUNKASAI / BUNKA_SD (Culture Festival) Constants
# ==============================================================================
enum CultureClub {
	LITERATURE = 0,      # 文芸部
	DRAMA = 1,           # 演劇部
	SCIENCE = 2,         # 科学部
	COMPUTER = 3,        # 電脳部
	BRASS_BAND = 4,      # 吹奏楽部
	ART = 5,             # 美術部
	BASEBALL = 6,        # 野球部
	SOCCER = 7,          # サッカー部
	TENNIS = 8,          # テニス部
	SWIMMING = 9,        # 水泳部
	BASKETBALL = 10,     # バスケ部
	NONE = 15            # 帰宅部 / クラス模擬店
}

# ==============================================================================
# SHUGAKU (School Trip) Constants
# ==============================================================================
enum TripDestination {
	KYOTO_NARA = 0,      # 京都・奈良
	OKINAWA = 1,         # 沖縄
	HOKKAIDO = 2,        # 北海道
}

# ==============================================================================
# RENSYU (Practice Match) Constants
# ==============================================================================
# RensyuTbl11 practice_match_point_thresholds from practice_match_mode_stage_dispatch.c:13
const PRACTICE_THRESHOLDS: Array[int] = [
	0x1E, 0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2, 0xF0, 0x10E, 0x12C, 0x14A # 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330
]

# ==============================================================================
# EN_NICHI (Festival Stalls) Constants
# ==============================================================================
enum StallGame {
	SHOOTING = 0,        # 射的
	GOLDFISH = 1,        # 金魚すくい
	RING_TOSS = 2        # 輪投げ
}

# ==============================================================================
# 1. TAIIKU (Sports Festival) Logic
# ==============================================================================

## Initializes a fresh Sports Festival state
static func sports_init_state(competition: int = SportsCompetition.DASH_100M) -> Dictionary:
	return {
		"competition": competition,
		"phase": "entry", # entry, running, finished, folk_dance
		"mode_step": 0,
		"track_position": 0, # 16.16 fixed point
		"player_speed": 0,
		"motion_increment": 0,
		"speed_update_frames": 0,
		"input_latched": false,
		"obstacle_cleared": false,
		"finish_order": [],
		"cpu_positions": [0, 0, 0],
		"cpu_speeds": [0x800, 0x900, 0x700],
		"folk_dance_participants": [],
		"folk_dance_cycle": 0,
	}

## Steps the runner race forward one frame (runner_race_dispatch.c:1008-1036)
static func sports_step_race(state: Dictionary, circle_pressed: bool, player_athletics: int) -> Dictionary:
	var base_speed := RUNNER_INPUT_SPEED_DEFAULT + (player_athletics * 16)
	var motion_limit := 0x20000 + (player_athletics * 0x400)

	if circle_pressed:
		state["input_latched"] = true

	state["speed_update_frames"] = int(state.get("speed_update_frames", 0)) + 1
	if state["speed_update_frames"] >= 16:
		if state["input_latched"]:
			if state["motion_increment"] < 0:
				state["motion_increment"] = 0
			state["player_speed"] = base_speed
		else:
			state["player_speed"] = RUNNER_DECELERATION
		state["speed_update_frames"] = 0
		state["input_latched"] = false

	# Position integrate
	state["motion_increment"] = clampi(
		int(state["motion_increment"]) + int(state["player_speed"]),
		-0x10000,
		motion_limit
	)
	state["track_position"] = int(state["track_position"]) + int(state["motion_increment"])

	# CPU update
	var cpos: Array = state["cpu_positions"]
	var cspeeds: Array = state["cpu_speeds"]
	for i in range(3):
		cpos[i] = int(cpos[i]) + int(cspeeds[i])

	# Check finish line (threshold 0x400000 = ~64m/100m scaled)
	const FINISH_LINE := 0x400000
	if state["track_position"] >= FINISH_LINE and not (0 in state["finish_order"]):
		var order: Array = state["finish_order"]
		order.append(0) # Player is index 0

	for i in range(3):
		var cpu_idx := i + 1
		if cpos[i] >= FINISH_LINE and not (cpu_idx in state["finish_order"]):
			state["finish_order"].append(cpu_idx)

	# When all finished or player finished
	if state["finish_order"].size() >= 4 or (state["track_position"] >= FINISH_LINE and state["finish_order"].size() > 0):
		state["phase"] = "finished"

	return state

## Calculates placement rewards matching sports_girl_scene_result_update (o-taiiku.md:26, 32)
static func sports_calculate_ranking_rewards(place: int, selected_girl: int, girls_state: Array) -> Dictionary:
	var result := {
		"place": place,
		"selected_girl": selected_girl,
		"tokimeki_all_delta": 0,
		"selected_girl_tokimeki_delta": 0,
		"selected_girl_affection_delta": 0,
	}

	# D_801499D4[0] == 0 (1st Place): all 13 girls gain 4 tokimeki, selected girl gains +2 tokimeki, +10 affection
	if place == 1:
		result["tokimeki_all_delta"] = 4
		result["selected_girl_tokimeki_delta"] = 2
		result["selected_girl_affection_delta"] = 10
	# D_801499D4[1] == 0 (2nd Place): all 13 girls gain 2 tokimeki, selected girl gains +1 tokimeki, +5 affection
	elif place == 2:
		result["tokimeki_all_delta"] = 2
		result["selected_girl_tokimeki_delta"] = 1
		result["selected_girl_affection_delta"] = 5
	# 3rd place
	elif place == 3:
		result["tokimeki_all_delta"] = 1
		result["selected_girl_tokimeki_delta"] = 0
		result["selected_girl_affection_delta"] = 2
	# 4th place: no reward
	else:
		result["tokimeki_all_delta"] = 0
		result["selected_girl_tokimeki_delta"] = 0
		result["selected_girl_affection_delta"] = 0

	# Apply deltas to girls_state
	for i in range(girls_state.size()):
		var g: Dictionary = girls_state[i]
		g["tokimeki"] = clampi(int(g.get("tokimeki", 0)) + result["tokimeki_all_delta"], 0, 100)
		if i == selected_girl:
			g["tokimeki"] = clampi(int(g.get("tokimeki", 0)) + result["selected_girl_tokimeki_delta"], 0, 100)
			g["affection"] = clampi(int(g.get("affection", 0)) + result["selected_girl_affection_delta"], 0, 100)

	return result

## Selects folk dance participants matching folk_dance_scene.c:45-88
static func sports_select_folk_dance(met_girls: Array, rng_seed: int = 42) -> Dictionary:
	var count := met_girls.size()
	if count < 3:
		return {
			"eligible": false,
			"participants": [],
			"variant": 2 # skip folk dance
		}

	# Pick two distinct non-zero girls
	var candidates: Array[int] = []
	for g in met_girls:
		if g != 0:
			candidates.append(g)

	if candidates.size() < 2:
		return {"eligible": false, "participants": [], "variant": 2}

	var first_girl: int = candidates[rng_seed % candidates.size()]
	var remaining: Array = candidates.filter(func(x: int) -> bool: return x != first_girl)
	var second_girl: int = int(remaining[(rng_seed / 3) % remaining.size()])

	# Initial order: [Shiori (0), first_girl, second_girl]
	var participants := [0, first_girl, second_girl]

	# Shuffle order rand() & 0x1F swaps
	var swaps: int = (rng_seed & 0x1F) % 7 + 1
	var temp_rng := rng_seed
	for _s in range(swaps):
		temp_rng = (temp_rng * 1103515245 + 12345) & 0x7FFFFFFF
		var k := (temp_rng >> 16) % 3
		temp_rng = (temp_rng * 1103515245 + 12345) & 0x7FFFFFFF
		var l := (temp_rng >> 16) % 3
		var t: int = participants[k]
		participants[k] = participants[l]
		participants[l] = t

	return {
		"eligible": true,
		"participants": participants,
		"variant": 1,
		"event_log_id": 0x13
	}

# ==============================================================================
# 2. BUNKASAI / BUNKA_SD (Culture Festival) Logic
# ==============================================================================

## Evaluates class booth / club preparation
static func culture_booth_preparation_check(player_club: int, stats: Dictionary, practice_points: int) -> Dictionary:
	var success := false
	var quality_score := 0
	var stat_name := ""

	match player_club:
		CultureClub.LITERATURE:
			stat_name = "humanities"
			quality_score = int(stats.get("humanities", 0)) + practice_points / 2
			success = quality_score >= 80
		CultureClub.DRAMA:
			stat_name = "expression"
			quality_score = int(stats.get("art", 0)) + int(stats.get("look", 0)) / 2 + practice_points / 2
			success = quality_score >= 85
		CultureClub.SCIENCE:
			stat_name = "science"
			quality_score = int(stats.get("science", 0)) + practice_points / 2
			success = quality_score >= 80
		CultureClub.COMPUTER:
			stat_name = "computer"
			quality_score = int(stats.get("science", 0)) + int(stats.get("humanities", 0)) / 2 + practice_points / 2
			success = quality_score >= 80
		CultureClub.BRASS_BAND:
			stat_name = "music"
			quality_score = int(stats.get("art", 0)) + practice_points / 2
			success = quality_score >= 85
		CultureClub.ART:
			stat_name = "art"
			quality_score = int(stats.get("art", 0)) + practice_points / 2
			success = quality_score >= 80
		_:
			# General class booth (café/exhibit): uses general look + trivia
			stat_name = "general"
			quality_score = int(stats.get("look", 0)) + int(stats.get("trivia", 0))
			success = quality_score >= 70

	return {
		"club": player_club,
		"stat_checked": stat_name,
		"quality_score": quality_score,
		"success": success,
		"reputation_delta": 5 if success else -2,
		"stress_delta": 5 if success else 15
	}

## Culture festival visitor selection matching culture_visitor_pick in mode_stage_dispatch.c:22-38
static func culture_visitor_pick(met_girls: Array, rng_seed: int = 123) -> int:
	if met_girls.is_empty():
		return 0 # Shiori default
	return met_girls[rng_seed % met_girls.size()]

## Tour companion invitation routing matching companion_menu.c:75-92
static func culture_companion_invite(girl_id: int, affection: int, tokimeki: int, declined_girl: int = -1) -> Dictionary:
	if girl_id == declined_girl:
		return {
			"accepted": false,
			"dialogue_group": 4, # Refusal response
			"reason": "already_declined"
		}

	# Acceptance threshold: affection >= 25 or tokimeki >= 20
	var accepts: bool = (affection >= 25 or tokimeki >= 20)
	if accepts:
		return {
			"accepted": true,
			"dialogue_group": 1,
			"girl_id": girl_id,
			"relationship_delta": {"tokimeki": 3, "affection": 5}
		}
	else:
		return {
			"accepted": false,
			"dialogue_group": 4,
			"declined_girl": girl_id,
			"relationship_delta": {"tokimeki": 0, "affection": -1}
		}

## Stage show evaluation matching BUNKA_SD stage show flow
static func culture_stage_show_evaluate(show_type: int, player_club: int, prep_score: int, held_circle: bool) -> Dictionary:
	var bonus: int = 15 if held_circle else 0
	var final_score: int = prep_score + bonus
	var rating := "normal"

	if final_score >= 90:
		rating = "great_success"
	elif final_score >= 60:
		rating = "success"
	else:
		rating = "poor"

	return {
		"show_type": show_type,
		"final_score": final_score,
		"rating": rating,
		"tokimeki_reward": 4 if rating == "great_success" else (2 if rating == "success" else 0)
	}

# ==============================================================================
# 3. SHUGAKU (School Trip) Logic
# ==============================================================================

## Day 1 departure and destination routing
static func trip_init(destination: int = TripDestination.KYOTO_NARA) -> Dictionary:
	var dest_name := "京都"
	match destination:
		TripDestination.KYOTO_NARA: dest_name = "京都"
		TripDestination.OKINAWA: dest_name = "沖縄"
		TripDestination.HOKKAIDO: dest_name = "北海道"

	return {
		"destination": destination,
		"destination_name": dest_name,
		"day": 1,
		"mode_step": 0,
		"current_girl": -1,
		"alternate_dialogue_mode": false,
		"events_completed": []
	}

## Sightseeing companion selector matching sightseeing_and_companion_selection.c:188-225 (trip_low_status_girl_select)
static func trip_select_companion(met_girls: Array, girl_records: Array) -> int:
	var best_i: int = -1
	var best_s: int = 4 # status rank

	for i in met_girls:
		if i >= girl_records.size():
			continue
		var rec: Dictionary = girl_records[i]
		var status_rank: int = int(rec.get("status_rank", 2)) & 0x7F
		var t_plus_y: int = int(rec.get("tokimeki", 0)) + int(rec.get("affection", 0))

		if status_rank < best_s:
			best_i = i
			best_s = status_rank
		elif status_rank == best_s and best_i >= 0:
			var best_rec: Dictionary = girl_records[best_i]
			var best_t_plus_y: int = int(best_rec.get("tokimeki", 0)) + int(best_rec.get("affection", 0))
			if t_plus_y > best_t_plus_y:
				best_i = i
				best_s = status_rank

	if best_i >= 0 and best_s < 2:
		return best_i
	return -1 # None eligible, defaults to Yoshio or solo

## Direct response stat deltas from C source (func_801330E8 / func_801350F4 / func_801392B4)
static func trip_response_stat_deltas(choice: int) -> Dictionary:
	# choice 0: tokimeki +3, affection +2, syoushin -15
	if choice == 0:
		return {
			"tokimeki": 3,
			"affection": 2,
			"syoushin": -15
		}
	# choice 1: tokimeki +2, affection +1, syoushin -10
	elif choice == 1:
		return {
			"tokimeki": 2,
			"affection": 1,
			"syoushin": -10
		}
	# other choice: affection +1, syoushin +10
	else:
		return {
			"tokimeki": 0,
			"affection": 1,
			"syoushin": 10
		}

## Corridor choices and return matching corridor_choices_and_return.c
static func trip_hotel_corridor_encounter(corridor_choice: int, encounter_girl: int) -> Dictionary:
	var result := {
		"encounter_girl": encounter_girl,
		"event": "pillow_fight",
		"tokimeki_delta": 1,
		"affection_delta": 2
	}
	match corridor_choice:
		0:
			result["event"] = "talk_in_corridor"
			result["tokimeki_delta"] = 2
			result["affection_delta"] = 3
		1:
			result["event"] = "pillow_fight"
			result["tokimeki_delta"] = 1
			result["affection_delta"] = 1
		2:
			result["event"] = "teacher_patrol_evasion"
			result["tokimeki_delta"] = 1
			result["affection_delta"] = 2

	return result

## Trip day completion matching trip_day_event_complete in destination_label_and_day_resume.c:32-59
static func trip_complete_day(state: Dictionary, girls_state: Array) -> Dictionary:
	var current_girl: int = int(state.get("current_girl", -1))
	var alternate: bool = bool(state.get("alternate_dialogue_mode", false))

	if alternate:
		if girls_state.size() > 10:
			girls_state[10]["affection"] = clampi(int(girls_state[10].get("affection", 0)) + 1, 0, 100)
	elif current_girl >= 0 and current_girl < girls_state.size():
		girls_state[current_girl]["tokimeki"] = clampi(int(girls_state[current_girl].get("tokimeki", 0)) + 1, 0, 100)
		girls_state[current_girl]["affection"] = clampi(int(girls_state[current_girl].get("affection", 0)) + 1, 0, 100)

	state["day"] = int(state.get("day", 1)) + 1
	return state

# ==============================================================================
# 4. KANGEI / RENSYU / EN_NICHI (Camp, Practice Match, Festival Stalls) Logic
# ==============================================================================

## Practice match evaluation matching practice_result_grade_calculate in practice_match_mode_stage_dispatch.c:516-548
static func practice_match_evaluate(club_id: int, practice_points: int, match_count: int, attending_girl: int = -1) -> Dictionary:
	var count_idx := clampi(match_count, 0, PRACTICE_THRESHOLDS.size() - 1)
	var threshold: int = PRACTICE_THRESHOLDS[count_idx]
	var diff: int = practice_points - threshold

	var grade: int = 0
	if diff < -30:
		grade = 0
	elif diff < -10:
		grade = 1
	elif diff < 0:
		grade = 2
	elif diff >= 30:
		grade = 5
	elif diff >= 10:
		grade = 4
	else:
		grade = 3

	var victory: bool = (grade >= 3)
	var tokimeki_all := 2 if victory else 0
	var guts_delta := 0 if victory else 10
	var stress_delta := 0 if victory else 10
	var attending_girl_affection := 2 if (attending_girl >= 0 and attending_girl != 0xFF) else 0

	return {
		"club_id": club_id,
		"practice_points": practice_points,
		"threshold": threshold,
		"diff": diff,
		"grade": grade,
		"victory": victory,
		"tokimeki_all_delta": tokimeki_all,
		"guts_delta": guts_delta,
		"stress_delta": stress_delta,
		"attending_girl": attending_girl,
		"attending_girl_affection_delta": attending_girl_affection,
		"new_match_count": match_count + (1 if victory else 0)
	}

## Summer camp cooking and daily progression matching KANGEI day_dispatch_and_script_tables.c
static func camp_step_day(day_idx: int, club_id: int, practice_points: int, stamina: int) -> Dictionary:
	var fatigue_cost := 15
	var gained_practice := 6

	return {
		"day": day_idx,
		"club_id": club_id,
		"new_stamina": clampi(stamina - fatigue_cost, 0, 100),
		"new_practice_points": practice_points + gained_practice,
		"cooking_quality": (stamina > 40)
	}

## Festival stall game play matching festival_stall_year_dispatch.c
static func stall_game_play(game_type: int, hold_frames: int, date_girl: int = -1) -> Dictionary:
	var score := 0
	var won_prize := false

	match game_type:
		StallGame.SHOOTING:
			# Target hit timing: hold between 10 and 20 frames gives bullseye
			if hold_frames >= 10 and hold_frames <= 20:
				score = 100
				won_prize = true
			else:
				score = clampi(60 - absi(hold_frames - 15) * 5, 0, 100)
		StallGame.GOLDFISH:
			# Gentle rhythm: hold <= 8 frames
			if hold_frames in [5, 6, 7]:
				score = 90
				won_prize = true
			else:
				score = 40
		StallGame.RING_TOSS:
			if hold_frames in [12, 13, 14]:
				score = 95
				won_prize = true
			else:
				score = 50

	var affection_bonus := 5 if won_prize else 1
	var tokimeki_bonus := 3 if won_prize else 0

	return {
		"game_type": game_type,
		"score": score,
		"won_prize": won_prize,
		"date_girl": date_girl,
		"affection_delta": affection_bonus,
		"tokimeki_delta": tokimeki_bonus
	}
