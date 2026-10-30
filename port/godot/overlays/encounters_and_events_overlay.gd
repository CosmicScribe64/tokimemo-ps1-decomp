extends RefCounted
## T-29018: Lane 3: Girl Encounters (GEKO), Seasonal Visits (OMIMAI, VALEN, SHOUGATU),
## and Annual Events / Term Exams (MODE_EXAM / GYOZI) overlay state machines and rules.
## Decompiled witnesses:
## - src/ovl/girl_encounters/walk_home.c, walk_home_entry.c, walk_home_followup.c
## - src/main/scheduler.c (geko_judge, sick visit queueing)
## - src/ovl/misc_scenes/final_exam.c (exam_score_day, exam_ranking_build, exam_results_react)
## - src/ovl/valentine/valentine_girl_selection.c (valentine_select_ranked_girls_and_candidates)
## - src/ovl/sick_visit/sick_visit_scene_dialogue.c (sick_visit_talk_choice_select)
## - src/ovl/annual_events/annual_candidate_results.c

const Math = preload("res://core/native_math.gd")

const GIRL_SHIORI: int = 0
const GIRL_MIO: int = 1
const GIRL_YUINA: int = 2
const GIRL_AYAKO: int = 3
const GIRL_SAKI: int = 4
const GIRL_YUKARI: int = 5
const GIRL_NOZOMI: int = 6
const GIRL_MIRA: int = 7
const GIRL_YUKO: int = 8
const GIRL_MEGUMI: int = 9
const GIRL_YUMI: int = 10
const GIRL_MIHARU: int = 11
const GIRL_REI: int = 12
const GIRL_YOSHIO: int = 13

const GEKO_SYOUSHIN_MAX_NORMAL: int = 0x3C # 60
const GEKO_SYOUSHIN_FORCED_MIN: int = 0x4C # 76
const GEKO_CLOSENESS_RANK_MIN: int = 0x11 # 17
const GEKO_TRIGGER_PROBABILITY_PERCENT: int = 50

const VALENTINE_TOKIMEKI_TOP_MIN: int = 0x46 # 70
const VALENTINE_YUUKOU_CANDIDATE_MIN: int = 0x32 # 50
const VALENTINE_UNMET_MEGUMI_TOKIMEKI_MIN: int = 20

const EXAM_CORRECTION_TABLE: Array[int] = [14, 12, 10, 8, 6, 4, 2, 0, 0]
const EXAM_BASE_SCORE: Array[int] = [92, 80, 81, 65, 61, 63, 33, 35, 31, 68, 40, 80, 79]
const EXAM_SCORE_SPREAD: Array[int] = [3, 7, 7, 7, 7, 7, 15, 15, 15, 7, 15, 7, 7]

# exam_row_x and exam_row_y from asm/ovl/ETC/data/ETC_data_80150630.data.s
const EXAM_ROW_X: Array[int] = [
	-0x98, -0x98, -0x98, -0x98, -0x98, -0x98, -0x98,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
]
const EXAM_ROW_Y: Array[int] = [
	-0x52, -0x42, -0x32, -0x22, -0x12, -0x02, 0x0E,
	-0x52, -0x42, -0x32, -0x22, -0x12, -0x02, 0x0E,
]


# ============================================================================
# 1. GEKO: After-School Walk Home State Machine
# ============================================================================

static func evaluate_geko_judge(
	girls: Array,
	system_seed: int,
	last_picked_girl: int,
	closeness_by_girl: Array,
	girl_states: Array,
	rng_rolls_mod_100: Array
) -> Dictionary:
	# Mirrors geko_judge (src/main/scheduler.c:80070694):
	# Scans 11 girls starting at system_seed % 11.
	var start_idx: int = Math.u32(system_seed) % 11
	var roll_cursor: int = 0
	for offset: int in range(11):
		var i: int = (start_idx + offset) % 11
		if i >= girls.size():
			continue
		var g: Dictionary = girls[i]
		var met: bool = int(g.get("met", 0)) != 0
		var syoushin: int = int(g.get("syoushin", 0))
		var state_s: int = int(girl_states[i]) & 0x7F if i < girl_states.size() else 0
		var closeness: int = int(closeness_by_girl[i]) if i < closeness_by_girl.size() else 0
		if not met:
			continue
		if syoushin >= GEKO_SYOUSHIN_MAX_NORMAL:
			continue
		if state_s == 4:
			continue
		if closeness < GEKO_CLOSENESS_RANK_MIN:
			continue
		var roll: int = int(rng_rolls_mod_100[roll_cursor % rng_rolls_mod_100.size()]) if not rng_rolls_mod_100.is_empty() else 0
		roll_cursor += 1
		if (roll % 100) < GEKO_TRIGGER_PROBABILITY_PERCENT and i != last_picked_girl:
			return {
				"triggered": true,
				"girl_id": i,
				"start_index": start_idx,
				"high_girl_counter": 0,
			}
	return {
		"triggered": false,
		"girl_id": -1,
		"start_index": start_idx,
		"high_girl_counter": 0,
	}


static func check_forced_high_syoushin_encounter(
	girls: Array,
	day_kind: int,
	weekday: int,
	month: int,
	day: int,
	events_today: int
) -> Dictionary:
	# Mirrors day_event_dispatch (src/ovl/girl_encounters/walk_home.c:84):
	# On a school day (day_kind == 0, not Sunday, not Dec 24, events_today == 0),
	# scans girls 0..10 for met != 0, syoushin >= 0x4C (76), and bit 16 clear.
	if day_kind != 0 or weekday == 0 or (month == 12 and day == 24) or events_today != 0:
		return {"triggered": false, "girl_id": -1, "high_girl_counter": 0}
	for i: int in range(mini(11, girls.size())):
		var g: Dictionary = girls[i]
		var met: bool = int(g.get("met", 0)) != 0
		var syoushin: int = int(g.get("syoushin", 0))
		var flags: int = int(g.get("flags", 0))
		if met and syoushin >= GEKO_SYOUSHIN_FORCED_MIN and (flags & 0x10000) == 0:
			return {
				"triggered": true,
				"girl_id": i,
				"high_girl_counter": 1,
			}
	return {"triggered": false, "girl_id": -1, "high_girl_counter": 0}


static func select_walk_home_topic(
	girl_id: int,
	girl_state: int,
	high_girl_counter: int,
	month: int,
	day: int,
	next_day_kind: int,
	roll_mod_10: int,
	roll_mod_2: int
) -> Dictionary:
	# Mirrors encounter_walk_home_topic_select (src/ovl/girl_encounters/walk_home.c:199-277).
	var state_masked: int = girl_state & 0x7F
	var group: int = 0
	var is_nickname_prompt: bool = false
	var is_bomb_confrontation: bool = false

	if high_girl_counter != 0:
		group = 1
		is_bomb_confrontation = true
	elif state_masked >= 2:
		if (roll_mod_10 % 10) == 0:
			group = 4
			is_nickname_prompt = true
		elif (month == 7 and day == 19) or (month == 12 and day == 23) or (month == 3 and day == 22):
			group = 0xB + (roll_mod_2 & 1)
		elif next_day_kind == 1 or next_day_kind == 2:
			group = 9 + (roll_mod_2 & 1)
		else:
			if month >= 3 and month <= 5:
				group = 3
			elif month >= 6 and month <= 8:
				group = 5
			elif month >= 9 and month <= 11:
				group = 6
			else:
				group = 7
			if girl_id < 10:
				group += (roll_mod_2 & 1)
	else:
		group = 0

	return {
		"walk_home_group": group,
		"is_nickname_prompt": is_nickname_prompt,
		"is_bomb_confrontation": is_bomb_confrontation,
	}


@warning_ignore("integer_division")
static func apply_walk_home_choice(
	talk_choice: int,
	current_girl: int,
	girl_state: int,
	walk_home_group: int,
	girls: Array,
	frame_count: int,
	rng_rolls: Array
) -> Dictionary:
	# Mirrors encounter_walk_home_choice_apply (src/ovl/girl_encounters/walk_home.c:365-404).
	var updated_girls: Array = []
	for g_item: Variant in girls:
		updated_girls.append((g_item as Dictionary).duplicate(true))

	var state_masked: int = girl_state & 0x7F
	var followup_allowed: int = 0
	var new_group: int = walk_home_group
	var other_counter_base: int = 4

	if talk_choice == 0:
		followup_allowed = 1
		if current_girl in [GIRL_SHIORI, GIRL_MIO, GIRL_YUINA, GIRL_MIRA, GIRL_YUKO, GIRL_MEGUMI]:
			if state_masked >= 3:
				followup_allowed = 0
				other_counter_base = 0
		var roll_idx: int = 0
		for other: int in range(mini(11, updated_girls.size())):
			if other != current_girl and int(updated_girls[other].get("met", 0)) != 0:
				var other_tokimeki: int = int(updated_girls[other].get("tokimeki", 0))
				var band: int = (other_tokimeki / 20) + 1
				var r: int = int(rng_rolls[roll_idx % rng_rolls.size()]) if not rng_rolls.is_empty() else 0
				roll_idx += 1
				var delta: int = (r % maxi(1, band)) + other_counter_base
				updated_girls[other]["syoushin"] = clampi(
					int(updated_girls[other].get("syoushin", 0)) + delta, 0, 999
				)
	else:
		followup_allowed = 0
		var cur_tokimeki: int = int(updated_girls[current_girl].get("tokimeki", 0))
		var tokimeki_band: int = cur_tokimeki / 20
		var counter_random_offset: int = Math.u32(frame_count) % maxi(1, tokimeki_band + 1)
		var base_penalty: int = 10 if state_masked < 2 else 6
		updated_girls[current_girl]["syoushin"] = clampi(
			int(updated_girls[current_girl].get("syoushin", 0)) + counter_random_offset + base_penalty,
			0,
			999
		)
		new_group += 2

	return {
		"followup_allowed": followup_allowed,
		"walk_home_group": new_group,
		"other_counter_base": other_counter_base,
		"girls": updated_girls,
	}


static func finish_walk_home(
	current_girl: int,
	girls: Array,
	high_girl_counter: int = 0,
	defuse_confrontation: bool = false
) -> Array:
	# Mirrors encounter_walk_home_finish (src/ovl/girl_encounters/walk_home.c:455-475):
	# girls[current_girl].affection += 2; check_para_limit().
	var updated_girls: Array = []
	for g_item: Variant in girls:
		updated_girls.append((g_item as Dictionary).duplicate(true))
	if current_girl >= 0 and current_girl < updated_girls.size():
		updated_girls[current_girl]["affection"] = clampi(
			int(updated_girls[current_girl].get("affection", 0)) + 2, 0, 999
		)
		if high_girl_counter != 0:
			updated_girls[current_girl]["flags"] = int(updated_girls[current_girl].get("flags", 0)) | 0x10000
			if defuse_confrontation:
				updated_girls[current_girl]["syoushin"] = maxi(
					0, int(updated_girls[current_girl].get("syoushin", 0)) - 30
				)
	return updated_girls


static func apply_nickname_prompt_choice(
	accepted: bool,
	custom_nickname: String,
	previous_nickname: String
) -> Dictionary:
	# Mirrors walk_home_followup.c nickname prompt branch (walk_home_group == 4).
	if accepted and not custom_nickname.is_empty():
		return {
			"accepted": true,
			"active_nickname": custom_nickname,
		}
	return {
		"accepted": false,
		"active_nickname": previous_nickname,
	}


# ============================================================================
# 2. MODE_EXAM / GYOZI: Term Exams & Annual Events Scoring and Ranking
# ============================================================================

static func compute_exam_correction_bonus(year: int, month: int, rand_val: int) -> int:
	# Mirrors exam_correction_bonus (src/ovl/misc_scenes/final_exam.c:6-60).
	var x: int = 6
	match year:
		0x5F: # 95
			x = 0
		0x60: # 96
			x = 3
		0x61: # 97
			x = 6
		_:
			x = 6
	if month == 2:
		x -= 1
	elif month != 7 and month == 12:
		x += 1
	x = clampi(x, 0, EXAM_CORRECTION_TABLE.size() - 1)
	var a: int = EXAM_CORRECTION_TABLE[x]
	var r: int = rand_val & 7
	var d: int = r + a - 8
	if d > 0:
		return a
	return 0


@warning_ignore("integer_division")
static func compute_exam_day_score(
	weekday: int,
	params: Dictionary,
	biorhythm_score: int,
	health_low_nibble: int,
	correction_bonus: int,
	event_log_bonus: int,
	rand_mod_10: int,
	rand_low_2bits: int = 0
) -> Dictionary:
	# Mirrors exam_score_day (src/ovl/misc_scenes/final_exam.c:294-389).
	# Weekday: 1=Mon(Language), 2=Tue(Math), 3=Wed(Science), 4=Thu(Social), 5=Fri(Art).
	var liberal_arts: int = int(params.get("liberal_arts", 0))
	var science: int = int(params.get("science", 0))
	var trivia: int = int(params.get("trivia", 0))
	var art: int = int(params.get("art", 0))

	var primary_score: int = 0
	var sub: int = 0
	match weekday:
		1: # WEEKDAY_MONDAY: Language
			primary_score = liberal_arts
			sub = 0
		2: # WEEKDAY_TUESDAY: Math
			primary_score = science
			sub = 0
		3: # WEEKDAY_WEDNESDAY: Science
			primary_score = (science * 7) / 10
			sub = (trivia * 3) / 10
			if sub >= 0x27:
				sub = 0x26 # 38
		4: # WEEKDAY_THURSDAY: Social Studies
			primary_score = (trivia * 7) / 10
			sub = (liberal_arts * 3) / 10
			if sub >= 0x27:
				sub = 0x26 # 38
		5: # WEEKDAY_FRIDAY: Art
			primary_score = (art * 8) / 10
			sub = (trivia * 2) / 10
			if sub >= 0x1A:
				sub = 0x19 # 25
		_:
			return {"score": 0, "grade": 0, "failed": true, "raw_ret": 0}

	var ret: int = primary_score + sub + biorhythm_score - 8
	if (health_low_nibble & 2) != 0:
		ret /= 2
	if ret < 0:
		ret = 1
	ret += correction_bonus + (rand_mod_10 % 10) * event_log_bonus

	var subject_score: int = 0
	if ret < 0x80:
		subject_score = (ret * 100) / 128
	else:
		subject_score = 100 - (rand_low_2bits & 3)

	var grade: int = 0
	if subject_score < 0x28: # 40
		grade = 0
	elif subject_score < 0x3C: # 60
		grade = 1
	elif subject_score < 0x50: # 80
		grade = 2
	elif subject_score < 0x5F: # 95
		grade = 3
	else:
		grade = 4

	return {
		"score": subject_score,
		"grade": grade,
		"failed": grade == 0,
		"raw_ret": ret,
	}


@warning_ignore("integer_division")
static func build_exam_ranking(
	player_total: int,
	girls: Array,
	scene_flags: Array,
	failed_subject_count: int = 0,
	rival_scores_override: Array = []
) -> Dictionary:
	# Mirrors exam_ranking_build and exam_results_react (src/ovl/misc_scenes/final_exam.c:747-990).
	var ranking_value: Array[int] = []
	ranking_value.resize(14)
	for i: int in range(13):
		if i < rival_scores_override.size():
			ranking_value[i] = clampi(int(rival_scores_override[i]), 0, 500)
		else:
			ranking_value[i] = clampi(EXAM_BASE_SCORE[i] * 5, 0, 500)
	ranking_value[13] = clampi(player_total, 0, 500)

	# Deduplicate ties just like exam_ranking_build (lines 869-879)
	var changed: int = 1
	while changed != 0:
		changed = 0
		for j: int in range(14):
			for i: int in range(j + 1, 14):
				if ranking_value[i] == ranking_value[j]:
					changed += 1
					ranking_value[i] -= 1

	# Sort descending into exam_ranking_order[0..13] (lines 880-894)
	var values: Array[int] = ranking_value.duplicate()
	var ranking_order: Array[int] = []
	ranking_order.resize(14)
	for j: int in range(14):
		var selected: int = 0
		var maximum: int = values[0]
		for i: int in range(1, 14):
			if maximum < values[i]:
				maximum = values[i]
				selected = i
		values[selected] = -9999
		ranking_order[j] = selected

	# Compute school-wide ranks (1..200) Monotonic with score
	var ranking_rank: Array[int] = []
	ranking_rank.resize(14)
	var max_score: int = ranking_value[ranking_order[0]]
	for pos: int in range(14):
		var who: int = ranking_order[pos]
		var sc: int = ranking_value[who]
		var computed_rank: int = maxi(1, 200 - (sc * 199) / 500)
		ranking_rank[who] = computed_rank

	# Ensure strict rank ordering matching score ordering
	for pos: int in range(1, 14):
		var prev_who: int = ranking_order[pos - 1]
		var cur_who: int = ranking_order[pos]
		if ranking_rank[cur_who] <= ranking_rank[prev_who]:
			ranking_rank[cur_who] = ranking_rank[prev_who] + 1

	if max_score >= 466:
		var top_who: int = ranking_order[0]
		var offset: int = ranking_rank[top_who] - 1
		for i: int in range(14):
			ranking_rank[i] = maxi(1, ranking_rank[i] - offset)

	# Relationship bonuses (lines 975-989)
	var updated_girls: Array = []
	for g_item: Variant in girls:
		updated_girls.append((g_item as Dictionary).duplicate(true))
	var updated_flags: Array = scene_flags.duplicate()
	if updated_flags.size() < 0xD8:
		updated_flags.resize(0xD8)

	for i: int in range(mini(11, updated_girls.size())):
		if ranking_value[i] < ranking_value[13]:
			if int(updated_flags[0xC8 + i]) == 0:
				updated_girls[i]["tokimeki"] = clampi(int(updated_girls[i].get("tokimeki", 0)) + 11, 0, 999)
				updated_flags[0xC8 + i] = 1
			else:
				updated_girls[i]["tokimeki"] = clampi(int(updated_girls[i].get("tokimeki", 0)) + 2, 0, 999)

	if ranking_rank[13] == 1:
		for i: int in range(mini(11, updated_girls.size())):
			updated_girls[i]["tokimeki"] = clampi(int(updated_girls[i].get("tokimeki", 0)) + 2, 0, 999)

	var player_order_index: int = ranking_order.find(13)
	var player_in_lower_half: int = 1 if player_order_index >= 7 else 0
	var remedial_required: bool = failed_subject_count >= 3

	return {
		"ranking_value": ranking_value,
		"ranking_order": ranking_order,
		"ranking_rank": ranking_rank,
		"player_score": ranking_value[13],
		"player_rank": ranking_rank[13],
		"player_order_index": player_order_index,
		"player_in_lower_half": player_in_lower_half,
		"remedial_required": remedial_required,
		"next_day_marker": 0x17 if remedial_required else 0,
		"girls": updated_girls,
		"scene_flags": updated_flags,
	}


# ============================================================================
# 3. VALEN / SHOUGATU / OMIMAI: Seasonal Visits
# ============================================================================

static func select_valentine_candidates(
	girls: Array,
	closeness_tokimeki: Array,
	closeness_yuukou: Array
) -> Dictionary:
	# Mirrors valentine_select_ranked_girls_and_candidates
	# (src/ovl/valentine/valentine_girl_selection.c:45-121).
	var updated_girls: Array = []
	for g_item: Variant in girls:
		updated_girls.append((g_item as Dictionary).duplicate(true))

	var unmet_megumi_selected: int = 0
	if updated_girls.size() > GIRL_MEGUMI:
		var megumi: Dictionary = updated_girls[GIRL_MEGUMI]
		if int(megumi.get("met", 0)) == 0 and int(megumi.get("tokimeki", 0)) >= VALENTINE_UNMET_MEGUMI_TOKIMEKI_MIN:
			unmet_megumi_selected = 1
			megumi["affection"] = 50

	# Top 3 ranked girls with get_h_tokimeki(i) >= 0x46 (70), sorted by affection + tokimeki
	var ranked_girls: Array[int] = [0xFF, 0xFF, 0xFF]
	var ranked_scores: Array[int] = [-1, -1, -1]
	for i: int in range(mini(11, updated_girls.size())):
		var g: Dictionary = updated_girls[i]
		var h_toki: int = int(closeness_tokimeki[i]) if i < closeness_tokimeki.size() else 0
		if int(g.get("met", 0)) != 0 and h_toki >= VALENTINE_TOKIMEKI_TOP_MIN:
			var combined: int = int(g.get("affection", 0)) + int(g.get("tokimeki", 0))
			if ranked_scores[0] < combined:
				ranked_scores[2] = ranked_scores[1]
				ranked_girls[2] = ranked_girls[1]
				ranked_scores[1] = ranked_scores[0]
				ranked_girls[1] = ranked_girls[0]
				ranked_scores[0] = combined
				ranked_girls[0] = i
			elif ranked_scores[1] < combined:
				ranked_scores[2] = ranked_scores[1]
				ranked_girls[2] = ranked_girls[1]
				ranked_scores[1] = combined
				ranked_girls[1] = i
			elif ranked_scores[2] < combined:
				ranked_scores[2] = combined
				ranked_girls[2] = i

	# Candidate list: met girls with get_h_yuukou(i) >= 0x32 (50), or unmet Megumi
	var candidates: Array[int] = []
	for i: int in range(mini(11, updated_girls.size())):
		var g: Dictionary = updated_girls[i]
		var h_yuukou: int = int(closeness_yuukou[i]) if i < closeness_yuukou.size() else 0
		if (int(g.get("met", 0)) != 0 and h_yuukou >= VALENTINE_YUUKOU_CANDIDATE_MIN) or (i == GIRL_MEGUMI and unmet_megumi_selected != 0):
			g["flags"] = int(g.get("flags", 0)) | 0x100 # flags.f.b8 = 1
			candidates.append(i)

	var count: int = candidates.size()
	if count >= 2:
		var shiori_pos: int = candidates.find(GIRL_SHIORI)
		if shiori_pos != -1:
			var shiori_toki: int = int(updated_girls[GIRL_SHIORI].get("tokimeki", 0))
			if shiori_toki >= VALENTINE_TOKIMEKI_TOP_MIN or unmet_megumi_selected != 0:
				var last_girl: int = candidates[count - 1]
				candidates[count - 1] = GIRL_SHIORI
				candidates[shiori_pos] = last_girl
			else:
				var first_girl: int = candidates[0]
				candidates[0] = GIRL_SHIORI
				candidates[shiori_pos] = first_girl
		if unmet_megumi_selected != 0:
			var megumi_pos: int = candidates.find(GIRL_MEGUMI)
			if megumi_pos != -1:
				var first_girl: int = candidates[0]
				candidates[0] = GIRL_MEGUMI
				candidates[megumi_pos] = first_girl

	return {
		"ranked_girls": ranked_girls,
		"ranked_scores": ranked_scores,
		"candidates": candidates,
		"candidate_count": count,
		"unmet_megumi_selected": unmet_megumi_selected,
		"girls": updated_girls,
	}


static func select_new_year_visitor(girls: Array, girl_states: Array) -> int:
	# Mirrors SHOUGATU func_8013288C (wiki/spec/o-shougatu.md):
	# Scans met girls 0..10 with (state & 0x7F) < 2; picks lowest state, ties broken by affection + tokimeki.
	var best_girl: int = -1
	var best_state: int = 2
	var best_sum: int = -1
	for i: int in range(mini(11, girls.size())):
		var g: Dictionary = girls[i]
		if int(g.get("met", 0)) == 0:
			continue
		var st: int = int(girl_states[i]) & 0x7F if i < girl_states.size() else 7
		if st >= 2:
			continue
		var sum_score: int = int(g.get("affection", 0)) + int(g.get("tokimeki", 0))
		if st < best_state or (st == best_state and sum_score > best_sum):
			best_state = st
			best_sum = sum_score
			best_girl = i
	return best_girl


static func evaluate_christmas_mansion_dress_gate(year_stage: int, looks: int, athletics: int) -> Dictionary:
	# Mirrors christmas_mansion_entry (src/ovl/new_year/christmas_mansion_entry.c:45-90):
	# Dress/Looks gate: looks >= stage * 30 - (stage >= 2 ? 10 : 0) + 70 -> 70 (Y1), 100 (Y2), 120 (Y3)
	# Athletics gate: athletics >= stage * 25 + 150 -> 150 (Y1), 175 (Y2), 200 (Y3)
	var stg: int = clampi(year_stage, 0, 2)
	var looks_min: int = stg * 30 - (10 if stg >= 2 else 0) + 70
	var athletics_min: int = stg * 25 + 150
	return {
		"looks_min": looks_min,
		"athletics_min": athletics_min,
		"dress_gate_passed": looks >= looks_min,
		"athletics_gate_passed": athletics >= athletics_min,
	}


static func apply_christmas_party_rewards(selected_girl: int, girls: Array) -> Array:
	# Mirrors SHOUGATU func_8013BA7C (wiki/spec/o-shougatu.md):
	# Selected girl gets tokimeki +8, affection +8, syoushin -10; other met/flagged girls get affection +1.
	var updated_girls: Array = []
	for g_item: Variant in girls:
		updated_girls.append((g_item as Dictionary).duplicate(true))
	for i: int in range(mini(11, updated_girls.size())):
		if i == selected_girl:
			updated_girls[i]["tokimeki"] = clampi(int(updated_girls[i].get("tokimeki", 0)) + 8, 0, 999)
			updated_girls[i]["affection"] = clampi(int(updated_girls[i].get("affection", 0)) + 8, 0, 999)
			updated_girls[i]["syoushin"] = clampi(int(updated_girls[i].get("syoushin", 0)) - 10, 0, 999)
		elif int(updated_girls[i].get("met", 0)) != 0:
			updated_girls[i]["affection"] = clampi(int(updated_girls[i].get("affection", 0)) + 1, 0, 999)
	return updated_girls


static func select_sick_visit_dialogue(
	current_girl: int,
	last_command: int,
	club_id: int,
	girl_rank: int,
	year: int,
	month: int,
	rand_bit: int
) -> Dictionary:
	# Mirrors sick_visit_talk_choice_select (src/ovl/sick_visit/sick_visit_scene_dialogue.c:163-305).
	var cmd_low: int = last_command & 0xF
	var variant: int = 0
	var talk_group: int = 0
	var r: int = rand_bit & 1

	match current_girl:
		GIRL_SHIORI:
			if cmd_low < 2:
				variant = 0
				talk_group = 0
			elif cmd_low == 2:
				variant = 0
				talk_group = 2
			elif cmd_low == 3:
				variant = 0
				talk_group = 4
			elif cmd_low == 4:
				variant = 0
				talk_group = (club_id * 2) + 6
				if club_id == 0xA:
					talk_group -= 4
			else:
				variant = 3
				talk_group = 0x1A
			if r != 0:
				talk_group += 1
				if talk_group == 5:
					variant = 2
		GIRL_MIO:
			if cmd_low < 2:
				variant = 0
				talk_group = 0
			else:
				variant = 0
				talk_group = (club_id * 2) + 2
			if r != 0:
				talk_group += 1
		GIRL_YUINA:
			if cmd_low < 2:
				variant = 0
				talk_group = 0
			else:
				variant = 4
				talk_group = (club_id * 2) - 2
			if r != 0:
				talk_group += 1
		GIRL_SAKI:
			if cmd_low < 4:
				variant = 0
				talk_group = r
			elif r != 0 or (club_id == 6 and (year == 0x62 or (year == 0x61 and month >= 9))) or (club_id == 7 and year == 0x62):
				if (girl_rank & 0x7F) < 2:
					variant = 6
					talk_group = 4
				else:
					variant = 5
					talk_group = 5
			else:
				variant = 0
				talk_group = club_id - 4
		_:
			variant = 0
			talk_group = r

	return {
		"dialogue_variant": variant,
		"talk_group": talk_group,
	}
