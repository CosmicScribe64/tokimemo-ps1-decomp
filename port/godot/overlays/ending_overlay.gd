extends RefCounted
## T-29018: Lane 4: Ending (ENDING) & Club Master (MASTER) Overlays.
## Decompiled witnesses:
## - src/main/menu_speaker_girl.c (get_h_tokimeki, get_h_yuukou, girl_points_shiori, girl_points_others, girl_scores)
## - src/ovl/ending/graduation_candidate_selection.c (ending_graduation_candidate_select, ending_candidate_flag_gate, ending_graduation_result_dialogue_select)
## - src/ovl/ending/graduation_scene_flow.c (ending_mode_stage_dispatch)
## - src/ovl/club_master/club_master_scene_and_tables.c (master_tournament_final_grade_scene, master_companion_girl_select)

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

const GAME_MODE_ENDING: int = 0xC1
const GAME_MODE_ETC_C2: int = 0xC2

const SHIORI_CLOSENESS_MIN: int = 0x55 # 85
const SHIORI_DATE_COUNT_MIN: int = 8
const SHIORI_SYOUSHIN_MAX_EXCLUSIVE: int = 0x33 # 51
const SHIORI_CONFESSION_POINTS: int = 0x311 # 785
const HEROINE_CONFESSION_POINTS_MIN: int = 500 # 0x1F4 (best_points starts at 499 = 0x1F3)
const OTHER_HEROINE_DATE_COUNT_MIN: int = 4
const REI_CONFESSION_SUBCOUNT_MIN: int = 0x46 # 70

# (&param_condition)[1..7]: liberal_arts, science, art, fitness, social, style, trivia
const GIRL_SHIORI_PARAM_MINIMUM_TAIL: Array[int] = [130, 130, 130, 120, 100, 100, 130]

const GIRL_TOKIMEKI_WEIGHT: Array[int] = [83, 113, 103, 119, 125, 106, 119, 113, 109, 106, 150, 100, 100]
const GIRL_AFFECTION_WEIGHT: Array[int] = [83, 113, 150, 119, 131, 106, 138, 100, 125, 150, 138, 100, 100]
const GIRL_BONUS_PARAM_MASK: Array[int] = [0xFE, 0x42, 0x44, 0x48, 0xD0, 0x50, 0x90, 0x68, 0x60, 0x6E, 0x70, 0x00, 0x00]
const GIRL_PARAM_LADDER_KIND: Array[int] = [1, 2, 3, 4, 2, 2, 2, 3, 2, 5, 6, 0, 0]
const GIRL_PARAM_LADDER_BASE: Array[int] = [128, 100, 90, 70, 50, 30, 10, 0]

const GIRL_PARAM_BONUS: Array = [
	[4, 3, 2, 2, 1, 0, -2, -4],
	[17, 9, 6, 3, 1, 0, -5, -15],
	[20, 15, 9, 5, 2, 0, -10, -20],
	[14, 11, 8, 5, 2, 0, -3, -6],
	[10, 6, 4, 1, 1, 0, -3, -6],
	[16, 10, 7, 5, 2, 0, -2, -7],
	[16, 12, 8, 4, 0, -4, -10, -16],
	[13, 11, 7, 4, 2, 0, -6, -13],
	[13, 10, 7, 5, 2, 0, -6, -13],
	[9, 8, 6, 4, 2, 0, -4, -8],
	[9, 7, 5, 3, 1, 0, -4, -8],
	[0, 0, 0, 0, 0, 0, 0, 0],
	[0, 0, 0, 0, 0, 0, 0, 0],
]

# Exact 20x20 girl_closeness_grid from asm/data/main/menu_speaker_girl.data/girl_closeness_grid.s
const GIRL_CLOSENESS_GRID: Array = [
	[0x00, 0x05, 0x0A, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0E, 0x0F, 0x0E, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F],
	[0x00, 0x05, 0x0A, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0E, 0x0F, 0x0E, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F],
	[0x00, 0x05, 0x0A, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0E, 0x0F, 0x0E, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F],
	[0x00, 0x05, 0x0A, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0E, 0x0F, 0x0E, 0x0F, 0x0F, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x14, 0x13, 0x13, 0x14, 0x13, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x1E, 0x1E, 0x1D, 0x1E, 0x1D, 0x1D, 0x1D, 0x1D, 0x1E, 0x1E, 0x1E, 0x1D, 0x1E],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x23],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x28, 0x28, 0x28, 0x27, 0x28, 0x27, 0x28, 0x28, 0x28, 0x27, 0x28],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2C],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x32, 0x32, 0x32, 0x32, 0x31, 0x32, 0x32, 0x32, 0x32],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3B, 0x3C],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x50, 0x50, 0x50, 0x50],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x50, 0x55, 0x54, 0x55],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x50, 0x55, 0x5A, 0x5A],
	[0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x50, 0x55, 0x5A, 0x5F],
]

const GIRL_SYOUSHIN_WEIGHT: Array[int] = [0, -4, -2, -4, -2, -2, -2, -2, -4, -4, -3, 0]
const GIRL_POINTS_MASK_STRONG: Array[int] = [0x00, 0x04, 0x02, 0x01, 0x01, 0x08, 0x08, 0x02, 0x04, 0x00, 0x04, 0x00]
const GIRL_POINTS_MASK_NORMAL: Array[int] = [0x00, 0x0A, 0x04, 0x06, 0x88, 0x82, 0x81, 0x14, 0x0A, 0x76, 0x05, 0x00]
const GIRL_POINTS_PARAM_CURVE: Array[int] = [-100, -75, -50, -25, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
const GIRL_POINTS_STRESS_CURVE: Array[int] = [100, 85, 65, 50, 37, 25, 17, 0, -17, -34, -50, -63, -75, -87, -100, -100]
const ENDING_MIHARU_CHANCE_BY_LEVEL: Array[int] = [1, 30, 50, 60, 70, 80, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 256]


# ============================================================================
# 1. Closeness & Confession Points (menu_speaker_girl.c:1303-1526)
# ============================================================================

@warning_ignore("integer_division")
static func compute_get_h_yuukou(girl_index: int, girl: Dictionary) -> int:
	# Mirrors get_h_yuukou (src/main/menu_speaker_girl.c:1388-1402).
	var affection: int = int(girl.get("affection", 0))
	var weight: int = GIRL_AFFECTION_WEIGHT[girl_index] if girl_index < GIRL_AFFECTION_WEIGHT.size() else 100
	var ret: int = (affection * weight) / 100
	return clampi(ret, 0, 100)


@warning_ignore("integer_division")
static func compute_get_h_tokimeki(girl_index: int, girl: Dictionary, param_values: Array) -> int:
	# Mirrors get_h_tokimeki (src/main/menu_speaker_girl.c:1303-1386).
	# param_values[0..7] corresponds to (&param_condition)[0..7]:
	# [0]=condition, [1]=liberal_arts, [2]=science, [3]=art, [4]=fitness, [5]=social, [6]=style, [7]=trivia
	var tokimeki: int = int(girl.get("tokimeki", 0))
	var weight: int = GIRL_TOKIMEKI_WEIGHT[girl_index] if girl_index < GIRL_TOKIMEKI_WEIGHT.size() else 100
	var ret: int = (tokimeki * weight) / 100
	var ladder_kind: int = GIRL_PARAM_LADDER_KIND[girl_index] if girl_index < GIRL_PARAM_LADDER_KIND.size() else 0
	var offsets: Array[int] = []
	match ladder_kind:
		1:
			offsets = [50, 35, 20, 20, 5, 5, 0]
		2:
			offsets = [0, 0, 0, 0, 0, 0, 0]
		3:
			offsets = [50, 50, 50, 20, 20, 20, 20]
		4:
			offsets = [10, 10, 10, 10, 0, 0, 0]
		5:
			offsets = [20, 20, 20, 20, 0, -5, 0]
		6:
			offsets = [0, 0, 0, 0, -5, -5, -5]
		_:
			offsets = []

	if not offsets.is_empty() and girl_index < GIRL_BONUS_PARAM_MASK.size():
		var mask: int = GIRL_BONUS_PARAM_MASK[girl_index]
		for param_idx: int in range(1, 7):
			if ((mask >> param_idx) & 1) != 0:
				var pval: int = int(param_values[param_idx]) if param_idx < param_values.size() else 0
				var step: int = 7
				for j: int in range(7):
					if pval > (GIRL_PARAM_LADDER_BASE[j] + offsets[j]):
						step = j
						break
				ret += int(GIRL_PARAM_BONUS[girl_index][step])

	return clampi(ret, 0, 100)


@warning_ignore("integer_division")
static func compute_girl_points_shiori(
	closeness_shiori: int,
	health_low_nibble: int,
	param_values: Array,
	shiori_girl: Dictionary
) -> int:
	# Mirrors girl_points_shiori (src/main/menu_speaker_girl.c:1405-1435).
	if closeness_shiori < SHIORI_CLOSENESS_MIN:
		return 0
	if (health_low_nibble & 0xF) != 0:
		return 0
	for i: int in range(1, 8):
		var pval: int = int(param_values[i]) if i < param_values.size() else 0
		if pval < GIRL_SHIORI_PARAM_MINIMUM_TAIL[i - 1]:
			return 0
	var counters: Array = shiori_girl.get("scene_counters", shiori_girl.get("counters", []))
	var scene_count: int = 0
	for i: int in range(1, 36):
		if i < counters.size():
			scene_count += int(counters[i])
	scene_count = maxi(scene_count, int(shiori_girl.get("date_count", 0)))
	if scene_count < SHIORI_DATE_COUNT_MIN:
		return 0
	if int(shiori_girl.get("syoushin", 0)) >= SHIORI_SYOUSHIN_MAX_EXCLUSIVE:
		return 0
	return SHIORI_CONFESSION_POINTS


@warning_ignore("integer_division")
static func compute_girl_points_others(
	closeness_by_girl: Array,
	girls: Array,
	param_values: Array,
	stress_value: int
) -> Array[int]:
	# Mirrors girl_points_others (src/main/menu_speaker_girl.c:1437-1503).
	var points: Array[int] = []
	points.resize(13)
	for idx: int in range(13):
		points[idx] = 0

	var stress_clamped: int = mini(stress_value, 140)
	for m: int in range(1, 11):
		if m >= girls.size():
			continue
		var g: Dictionary = girls[m]
		var cl: int = int(closeness_by_girl[m]) if m < closeness_by_girl.size() else 0
		points[m] = cl * 4
		if m == GIRL_YUINA:
			points[2] += GIRL_POINTS_STRESS_CURVE[clampi(stress_clamped / 10, 0, GIRL_POINTS_STRESS_CURVE.size() - 1)]
		points[m] += int(g.get("syoushin", 0)) * GIRL_SYOUSHIN_WEIGHT[m]

	for i: int in range(1, 11):
		for m: int in range(7):
			var param_idx: int = 7 - m
			var pval: int = int(param_values[param_idx]) if param_idx < param_values.size() else 0
			var bucket: int = clampi(pval / 10, 0, GIRL_POINTS_PARAM_CURVE.size() - 1)
			if ((GIRL_POINTS_MASK_STRONG[i] >> (7 - m)) & 1) != 0:
				points[i] += GIRL_POINTS_PARAM_CURVE[bucket]
				points[i] += GIRL_POINTS_PARAM_CURVE[bucket] / 2
			if ((GIRL_POINTS_MASK_NORMAL[i] >> (7 - m)) & 1) != 0:
				points[i] += GIRL_POINTS_PARAM_CURVE[bucket]

	for m: int in range(1, 11):
		if m >= girls.size():
			continue
		var counters: Array = girls[m].get("scene_counters", girls[m].get("counters", []))
		var scene_count: int = 0
		for j: int in range(1, 36):
			if j < counters.size():
				scene_count += int(counters[j])
		scene_count = maxi(scene_count, int(girls[m].get("date_count", 0)))
		if scene_count >= OTHER_HEROINE_DATE_COUNT_MIN:
			points[m] += scene_count * 10
		else:
			points[m] = 0

	return points


@warning_ignore("integer_division")
static func compute_girl_scores(
	girls: Array,
	param_values: Array,
	stress_value: int,
	health_low_nibble: int
) -> Dictionary:
	# Mirrors girl_scores (src/main/menu_speaker_girl.c:1505-1526).
	var weighted_tokimeki: Array[int] = []
	var weighted_affection: Array[int] = []
	var closeness_by_girl: Array[int] = []
	weighted_tokimeki.resize(13)
	weighted_affection.resize(13)
	closeness_by_girl.resize(13)

	for i: int in range(13):
		var g: Dictionary = girls[i] if i < girls.size() else {}
		var wt: int = compute_get_h_tokimeki(i, g, param_values) / 5
		var wa: int = compute_get_h_yuukou(i, g) / 5
		if wt >= 20:
			wt = 19
		if wa >= 20:
			wa = 19
		weighted_tokimeki[i] = wt
		weighted_affection[i] = wa
		closeness_by_girl[i] = int(GIRL_CLOSENESS_GRID[wt][wa])

	var points_by_girl: Array[int] = compute_girl_points_others(
		closeness_by_girl, girls, param_values, stress_value
	)
	var shiori_girl: Dictionary = girls[0] if not girls.is_empty() else {}
	points_by_girl[0] = compute_girl_points_shiori(
		closeness_by_girl[0], health_low_nibble, param_values, shiori_girl
	)

	return {
		"weighted_tokimeki": weighted_tokimeki,
		"weighted_affection": weighted_affection,
		"closeness_by_girl": closeness_by_girl,
		"points_by_girl": points_by_girl,
	}


# ============================================================================
# 2. ENDING: Graduation Confession Candidate Selection & Stage Flow
# ============================================================================

static func select_graduation_candidate(
	girls: Array,
	points_by_girl: Array,
	scene_flags: Array,
	rand_mod_256: int = 0
) -> Dictionary:
	# Mirrors ending_graduation_candidate_select
	# (src/ovl/ending/graduation_candidate_selection.c:43-89).
	var current_girl: int = GIRL_YOSHIO
	var best_points: int = HEROINE_CONFESSION_POINTS_MIN - 1 # 499 (0x1F3)

	var rei_girl: Dictionary = girls[GIRL_REI] if girls.size() > GIRL_REI else {}
	var rei_subcount: int = int(rei_girl.get("rei_subcount", 0))
	var flag_4f: int = int(scene_flags[0x4F]) if scene_flags.size() > 0x4F else 0
	var shiori_points: int = int(points_by_girl[0]) if not points_by_girl.is_empty() else 0

	if shiori_points < HEROINE_CONFESSION_POINTS_MIN and rei_subcount >= REI_CONFESSION_SUBCOUNT_MIN and flag_4f == 2:
		current_girl = GIRL_REI
		best_points = rei_subcount
	else:
		for i: int in range(mini(11, girls.size())):
			var g: Dictionary = girls[i]
			var pts: int = int(points_by_girl[i]) if i < points_by_girl.size() else 0
			# Strict > guarantees earlier index wins ties (0=Shiori > 1=Mio > ... > 10=Yumi).
			if int(g.get("met", 0)) != 0 and pts > best_points:
				current_girl = i
				best_points = pts

	var miharu_chance_level: int = 0
	var miharu_threshold: int = 0
	if current_girl == GIRL_YOSHIO and girls.size() > GIRL_MIHARU:
		var miharu_girl: Dictionary = girls[GIRL_MIHARU]
		var story_stage: int = int(miharu_girl.get("story_stage", 0)) & 0xF
		var flag_4c: int = int(scene_flags[0x4C]) if scene_flags.size() > 0x4C else 0
		var flag_4d: int = int(scene_flags[0x4D]) if scene_flags.size() > 0x4D else 0
		var flag_4e: int = int(scene_flags[0x4E]) if scene_flags.size() > 0x4E else 0
		miharu_chance_level = story_stage
		if flag_4c > 0:
			miharu_chance_level += 2
		if flag_4e > 0:
			miharu_chance_level += 2
		if miharu_chance_level >= 18:
			miharu_chance_level = 17
		miharu_threshold = ENDING_MIHARU_CHANCE_BY_LEVEL[miharu_chance_level]
		if story_stage != 0 and flag_4d == 0 and (rand_mod_256 % 256) < miharu_threshold:
			current_girl = GIRL_MIHARU

	return {
		"current_girl": current_girl,
		"best_points": best_points,
		"year": 0x62, # 98
		"month": 3,
		"day": 1,
		"weekday": 0, # WEEKDAY_SUNDAY
		"miharu_chance_level": miharu_chance_level,
		"miharu_threshold": miharu_threshold,
	}


static func evaluate_candidate_flag_gate(current_girl: int, girls: Array) -> Dictionary:
	# Mirrors ending_candidate_flag_gate
	# (src/ovl/ending/graduation_candidate_selection.c:91-125).
	if current_girl in [GIRL_MIO, GIRL_YUINA, GIRL_AYAKO, GIRL_SAKI, GIRL_NOZOMI, GIRL_MIRA, GIRL_YUKO, GIRL_YUMI]:
		var g: Dictionary = girls[current_girl] if current_girl < girls.size() else {}
		var b26: bool = (int(g.get("flags", 0)) & (1 << 26)) != 0
		if not b26:
			return {
				"passed_gate": false,
				"redirected_girl": GIRL_YOSHIO,
				"next_mode_step": 0xE,
			}
	return {
		"passed_gate": true,
		"redirected_girl": current_girl,
		"next_mode_step": -1,
	}


static func select_graduation_result_dialogue(
	current_girl: int,
	saved_girl: int,
	choice_result: int,
	career_choice: int
) -> Dictionary:
	# Mirrors ending_graduation_result_dialogue_select
	# (src/ovl/ending/graduation_candidate_selection.c:139-192).
	var talk_group: int = 0
	var restored_girl: int = current_girl
	var clear_name_slot: bool = false

	if current_girl == GIRL_YOSHIO:
		if saved_girl == GIRL_YOSHIO:
			talk_group = 6 if career_choice == 0 else career_choice
			clear_name_slot = true
		else:
			talk_group = 6
			restored_girl = saved_girl
	elif current_girl == GIRL_REI:
		talk_group = 1 if choice_result == 0 else 2
	else:
		if choice_result == 0:
			talk_group = 2 if career_choice == 0 else 1
		else:
			talk_group = 3

	return {
		"talk_group": talk_group,
		"current_girl": restored_girl,
		"clear_name_slot": clear_name_slot,
	}


static func step_ending_stage_flow(mode_stage: int, current_girl: int) -> Dictionary:
	# Mirrors ending_mode_stage_dispatch (src/ovl/ending/graduation_scene_flow.c:80134DD0):
	# Stages 0..7 -> transitions to GAME_MODE_ETC_C2 (0xC2) at stage 7.
	match mode_stage:
		0:
			return {"stage": 0, "name": "graduation_ceremony_intro", "next_stage": 1, "next_game_mode": GAME_MODE_ENDING}
		1:
			return {"stage": 1, "name": "corridor_knock_or_yoshio", "next_stage": 2, "next_game_mode": GAME_MODE_ENDING}
		2:
			var location: String = "chapel" if current_girl == GIRL_REI else ("classroom" if current_girl == GIRL_YOSHIO else "legendary_tree")
			return {"stage": 2, "name": "confession_scene_" + location, "next_stage": 3, "next_game_mode": GAME_MODE_ENDING}
		3:
			return {"stage": 3, "name": "confession_closeup_choice", "next_stage": 4, "next_game_mode": GAME_MODE_ENDING}
		4:
			return {"stage": 4, "name": "confession_choice_reaction", "next_stage": 5, "next_game_mode": GAME_MODE_ENDING}
		5:
			return {"stage": 5, "name": "post_confession_transition", "next_stage": 6, "next_game_mode": GAME_MODE_ENDING}
		6:
			return {"stage": 6, "name": "epilogue_text_scroll", "next_stage": 7, "next_game_mode": GAME_MODE_ENDING}
		_:
			return {"stage": 7, "name": "handoff_to_credits", "next_stage": 7, "next_game_mode": GAME_MODE_ETC_C2}


static func normalize_rule_state_for_ending(rule_state: Dictionary) -> Dictionary:
	var param_values: Array[int] = []
	if rule_state.get("param_values") is Array and not (rule_state["param_values"] as Array).is_empty():
		for v: Variant in rule_state["param_values"]:
			param_values.append(int(v))
	elif rule_state.get("params") is Array:
		for p_word: Variant in rule_state["params"]:
			var pw: int = int(p_word)
			param_values.append((pw >> 16) if absi(pw) >= 65536 else pw)
	while param_values.size() < 9:
		param_values.append(0)

	var eval_girls: Array = []
	var src_girls: Array = rule_state.get("girls", []) if rule_state.get("girls") is Array else []
	for g_idx: int in range(13):
		var g_rec: Dictionary = src_girls[g_idx] if g_idx < src_girls.size() and src_girls[g_idx] is Dictionary else {}
		var toki: int = int(g_rec.get("tokimeki", 0))
		if not g_rec.has("tokimeki") and g_rec.has("tokimeki_word"):
			var tw: int = int(g_rec["tokimeki_word"])
			toki = (tw >> 16) if absi(tw) >= 65536 else tw
		var aff: int = int(g_rec.get("affection", 0))
		if not g_rec.has("affection") and g_rec.has("affection_word"):
			var aw: int = int(g_rec["affection_word"])
			aff = (aw >> 16) if absi(aw) >= 65536 else aw
		var syou: int = int(g_rec.get("syoushin", 0))
		if not g_rec.has("syoushin") and g_rec.has("syoushin_word"):
			var sw: int = int(g_rec["syoushin_word"])
			syou = (sw >> 16) if absi(sw) >= 65536 else sw
		var flags_val: int = int(g_rec.get("flags", 0))
		var met_val: int = int(g_rec.get("met", 1 if (flags_val & 2) != 0 else 0))
		var counters_arr: Array = []
		if g_rec.get("scene_counters") is Array:
			counters_arr = (g_rec["scene_counters"] as Array).duplicate()
		elif g_rec.get("counters") is Array:
			counters_arr = (g_rec["counters"] as Array).duplicate()
		else:
			counters_arr.resize(36)
			counters_arr.fill(0)
		eval_girls.append({
			"tokimeki": toki,
			"affection": aff,
			"syoushin": syou,
			"met": met_val,
			"flags": flags_val,
			"scene_counters": counters_arr,
			"date_count": int(g_rec.get("date_count", 0)),
			"story_stage": int(g_rec.get("story_stage", (flags_val >> 9) & 0xF)),
			"rei_subcount": int(g_rec.get("rei_subcount", (flags_val >> 24) & 0xFF)),
		})

	var stress_val: int = int(rule_state.get("stress", param_values[8] if param_values.size() > 8 else 0))
	var health_nibble: int = int(rule_state.get("health_low_nibble", (int(rule_state.get("player_flags", 0)) >> 8) & 0xF))
	var scene_flags: Array = rule_state.get("scene_flags", []) if rule_state.get("scene_flags") is Array else []
	return {
		"girls": eval_girls,
		"param_values": param_values,
		"stress_value": stress_val,
		"health_low_nibble": health_nibble,
		"scene_flags": scene_flags,
	}


static func evaluate_confession_candidates(
	state_or_girls: Variant,
	param_values: Array = [],
	stress_value: int = 0,
	health_low_nibble: int = 0,
	scene_flags: Array = [],
	rand_mod_256: int = 0
) -> Dictionary:
	# Pure PS1 graduation eligibility evaluation (src/ovl/ending/graduation_candidate_selection.c).
	# Never fabricates affection; returns GIRL_YOSHIO (13) when no heroine qualifies.
	var eval_girls: Array = []
	var eff_params: Array = param_values
	var eff_stress: int = stress_value
	var eff_health: int = health_low_nibble
	var eff_scene_flags: Array = scene_flags

	if state_or_girls is Dictionary:
		var norm: Dictionary = normalize_rule_state_for_ending(state_or_girls)
		eval_girls = norm["girls"]
		if eff_params.is_empty():
			eff_params = norm["param_values"]
		if eff_stress == 0:
			eff_stress = int(norm["stress_value"])
		if eff_health == 0:
			eff_health = int(norm["health_low_nibble"])
		if eff_scene_flags.is_empty():
			eff_scene_flags = norm["scene_flags"]
	elif state_or_girls is Array:
		eval_girls = state_or_girls

	var scores: Dictionary = compute_girl_scores(eval_girls, eff_params, eff_stress, eff_health)
	var sel: Dictionary = select_graduation_candidate(
		eval_girls, scores["points_by_girl"], eff_scene_flags, rand_mod_256
	)
	var candidate_girl: int = int(sel["current_girl"])
	var gate: Dictionary = evaluate_candidate_flag_gate(candidate_girl, eval_girls)
	var final_girl: int = int(gate["redirected_girl"])
	var eligible: bool = final_girl >= 0 and final_girl < GIRL_YOSHIO
	var phases: Array = (
		["confession", "staff_roll", "last_photo", "clear_save"]
		if eligible
		else ["yoshio_bad_ending", "staff_roll", "clear_save"]
	)

	return {
		"eligible": eligible,
		"current_girl": final_girl,
		"candidate_before_gate": candidate_girl,
		"passed_flag_gate": bool(gate["passed_gate"]),
		"best_points": int(sel["best_points"]),
		"points_by_girl": scores["points_by_girl"],
		"closeness_by_girl": scores["closeness_by_girl"],
		"ending_type": "confession" if eligible else "yoshio_bad_ending",
		"phases": phases,
	}


static func create_graduation_sequence(state_or_girls: Variant, rand_mod_256: int = 0) -> Dictionary:
	var eval_res: Dictionary = evaluate_confession_candidates(state_or_girls, [], 0, 0, [], rand_mod_256)
	var eligible: bool = bool(eval_res["eligible"])
	var current_girl: int = int(eval_res["current_girl"])
	var phases: Array = eval_res["phases"].duplicate()
	var first_phase: String = String(phases[0])
	return {
		"eligible": eligible,
		"current_girl": current_girl,
		"girl_id": current_girl,
		"confessed_girl": current_girl,
		"best_points": int(eval_res["best_points"]),
		"ending_type": String(eval_res["ending_type"]),
		"phases": phases,
		"phase_index": 0,
		"phase": first_phase,
		"history": [first_phase],
		"stage": 0,
		"choice_cursor": 0,
		"clear_save_completed": false,
		"clear_save_status": "none",
		"clear_save_attempts": 0,
		"clear_name_slot": not eligible,
		"album_clear_mask": 0,
		"completed": false,
		"action": "continue",
	}


# T-39007 (review R11): clear save (mode 0xC4, wiki spec o-etc 6.3).
# Source src/ovl/misc_scenes/staff_roll_and_clear_save.c clear_save_ask_answer
# (lines 520-531): PAD_CIRCLE starts the card check (yes), PAD_CROSS goes to
# mode_stage_set(2) (no); there is no cursor and no other input is read.
# Yes requests a write from the app host, which answers with an
# "ending.clear_save" completion. A successful completion finishes the ending
# directly in apply_clear_save_completion; the serialisable state never holds
# a trusted "saved" status that a later step would honour.
#
# Port difference: in the source a failed write goes to mode_step 0xFF
# (clear_save_no_card_step, lines 791-822), which waits for 0x860 twice and
# then retries the card check; it offers no way to decline. The port's failure
# is a save-folder write error that a retry may never fix, so after one
# acknowledging press of 0x860 it returns to the yes/no ask, where the player
# can retry or decline.
#
# Presentation: the ending overlay, including this phase's question text
# (clear_save_ask_init set_kanji_string calls) and the failure message, is not
# rendered by the port yet; this phase is unpresented.
const PAD_CIRCLE: int = 0x0020
const PAD_CROSS: int = 0x0040
## clear_save_no_card_step substeps 1 and 3: START | CROSS | CIRCLE.
const PAD_ACKNOWLEDGE: int = 0x0860
const CLEAR_SAVE_STATUSES: Array[String] = ["none", "prompt", "requested", "failed"]
const RECEIPT_KEYS: Array[String] = ["service", "girl", "counter", "album_clear_mask", "checksum_sha256"]

static func clear_save_receipt_error(receipt: Variant, girl: int) -> String:
	if not receipt is Dictionary or receipt.size() != RECEIPT_KEYS.size() or not RECEIPT_KEYS.all(func(key: String) -> bool: return receipt.has(key)):
		return "clear-save receipt fields differ"
	if receipt["service"] != "ending.clear_save" or not receipt["girl"] is int or receipt["girl"] != girl & 0xF:
		return "clear-save receipt belongs to another request"
	if not receipt["counter"] is int or receipt["counter"] < 1 or receipt["counter"] > 0x63:
		return "clear-save receipt counter is out of range"
	if not receipt["album_clear_mask"] is int or receipt["album_clear_mask"] < 0 or receipt["album_clear_mask"] > 0x1FFF:
		return "clear-save receipt album mask is out of range"
	if receipt["girl"] < 13 and (receipt["album_clear_mask"] & (1 << receipt["girl"])) == 0:
		return "clear-save receipt album mask omits the saved girl"
	var checksum: Variant = receipt["checksum_sha256"]
	if not checksum is String or checksum.length() != 64 or not checksum.is_valid_hex_number():
		return "clear-save receipt checksum is malformed"
	return ""

## Applies the host's clear-save completion to a requested sequence. Success
## returns the finished ending (completed, clear_save_completed and the album
## mask from the receipt); failure returns the sequence at the failure notice.
## The caller delivers the finished state in the same step; a stored copy is
## never stepped into completion again (see _sanitize_clear_save).
static func apply_clear_save_completion(seq_state: Dictionary, payload: Dictionary) -> Dictionary:
	var s: Dictionary = seq_state.duplicate(true)
	var request: Variant = s.get("clear_save_request")
	if s.get("phase") != "clear_save" or s.get("clear_save_status") != "requested" or not request is Dictionary:
		return {"error": "clear-save completion arrived without a pending request", "state": seq_state}
	if payload.get("girl") != request.get("girl") or payload.get("attempt") != request.get("attempt") or not payload.get("ok") is bool:
		return {"error": "clear-save completion identity differs from the request", "state": seq_state}
	s.erase("clear_save_request")
	if payload["ok"]:
		var receipt_error: String = clear_save_receipt_error(payload.get("receipt"), int(s.get("current_girl", GIRL_YOSHIO)))
		if not receipt_error.is_empty():
			return {"error": receipt_error, "state": seq_state}
		s["clear_save_status"] = "none"
		s["clear_save_receipt"] = (payload["receipt"] as Dictionary).duplicate()
		s["clear_save_completed"] = true
		s["album_clear_mask"] = payload["receipt"]["album_clear_mask"]
		s.erase("clear_save_error")
		s["completed"] = true
		s["ending_completed"] = 1
		s["action"] = "ending_complete"
	else:
		if not payload.get("error") is String or (payload["error"] as String).is_empty():
			return {"error": "clear-save failure needs a reason", "state": seq_state}
		s["clear_save_status"] = "failed"
		s["clear_save_error"] = payload["error"]
		s["completed"] = false
		s["action"] = "clear_save_failed"
	return {"error": "", "state": s}

# A stored state can only be prompt, requested or failed. Saved/completed
# fields, receipts and masks in the incoming state are discarded.
static func _sanitize_clear_save(s: Dictionary) -> void:
	var status: Variant = s.get("clear_save_status", "none")
	if not status is String or status not in CLEAR_SAVE_STATUSES:
		status = "none"
	if s.get("phase") != "clear_save":
		status = "none"
	elif status == "none":
		status = "prompt"
	if status == "requested":
		var request: Variant = s.get("clear_save_request")
		if not request is Dictionary or request.get("girl") != int(s.get("current_girl", GIRL_YOSHIO)) & 0xF \
				or request.get("attempt") != s.get("clear_save_attempts"):
			status = "prompt"
	if status == "failed" and not (s.get("clear_save_error") is String and not (s["clear_save_error"] as String).is_empty()):
		status = "prompt"
	s["clear_save_status"] = status
	s.erase("clear_save_receipt")
	if status != "requested":
		s.erase("clear_save_request")
	if status != "failed":
		s.erase("clear_save_error")
	s["clear_save_completed"] = false
	s["album_clear_mask"] = 0
	if not s.get("clear_save_attempts") is int or s["clear_save_attempts"] < 0:
		s["clear_save_attempts"] = 0

static func _step_clear_save(s: Dictionary, pad_pressed: int) -> Dictionary:
	s["completed"] = false
	match s["clear_save_status"]:
		"requested":
			s["action"] = "clear_save_pending"
			return s
		"failed":
			# Port difference (see above): one acknowledging press returns to the ask.
			if (pad_pressed & PAD_ACKNOWLEDGE) != 0:
				s["clear_save_status"] = "prompt"
				s.erase("clear_save_error")
				s["action"] = "clear_save_ask"
			else:
				s["action"] = "wait_input"
			return s
	# clear_save_ask_answer: circle is tested first, then cross.
	if (pad_pressed & PAD_CIRCLE) != 0:
		s["clear_save_attempts"] += 1
		s["clear_save_status"] = "requested"
		s["clear_save_request"] = {"girl": int(s.get("current_girl", GIRL_YOSHIO)) & 0xF, "attempt": s["clear_save_attempts"]}
		s["action"] = "clear_save_requested"
	elif (pad_pressed & PAD_CROSS) != 0:
		s["clear_save_status"] = "none"
		s["clear_save_declined"] = true
		s["completed"] = true
		s["ending_completed"] = 1
		s["action"] = "ending_complete"
	else:
		s["action"] = "wait_input"
	return s


static func step_graduation_sequence(seq_state: Dictionary, pad_pressed: int, rule_state: Dictionary = {}) -> Dictionary:
	var s: Dictionary = seq_state.duplicate(true)
	if not s.has("phases") or not (s["phases"] is Array) or (s["phases"] as Array).is_empty():
		var init_seq: Dictionary = create_graduation_sequence(rule_state if not rule_state.is_empty() else s)
		for k: String in init_seq.keys():
			if not s.has(k):
				s[k] = init_seq[k]

	_sanitize_clear_save(s)
	if s.get("phase") == "clear_save":
		return _step_clear_save(s, pad_pressed)

	if pad_pressed == 0:
		s["completed"] = false
		s["action"] = "wait_input"
		return s

	# Directional input toggles choice cursor during confession/bad-ending dialogue
	if (pad_pressed & 0x1000) != 0 or (pad_pressed & 0x4000) != 0:
		s["choice_cursor"] = 1 - int(s.get("choice_cursor", 0))
		s["completed"] = false
		s["action"] = "cursor_move"
		return s

	# Confirm (PAD_CIRCLE=0x20 or PAD_START=0x0800) advances through sequence phases
	if (pad_pressed & 0x20) != 0 or (pad_pressed & 0x0800) != 0:
		var phases: Array = s["phases"]
		var idx: int = int(s.get("phase_index", 0))
		if idx + 1 < phases.size():
			idx += 1
			var next_phase: String = String(phases[idx])
			s["phase_index"] = idx
			s["phase"] = next_phase
			s["stage"] = mini(idx * 2, 7)
			var hist: Array = s.get("history", []) if s.get("history") is Array else []
			if hist.is_empty() or String(hist[hist.size() - 1]) != next_phase:
				hist.append(next_phase)
			s["history"] = hist
			if next_phase == "staff_roll":
				s["next_mode"] = GAME_MODE_ETC_C2
			elif next_phase == "clear_save":
				s["clear_save_status"] = "prompt"
			s["completed"] = false
			s["action"] = "phase_advance"
		else:
			s["completed"] = true
			s["ending_completed"] = 1
			s["action"] = "ending_complete"
		return s

	s["completed"] = false
	return s


# ============================================================================
# 3. MASTER: Club Master Tournament Grade & Companion Selection
# ============================================================================

static func compute_master_tournament_grade(practice_points: int, current_flags: int = 0) -> Dictionary:
	# Mirrors master_tournament_final_grade_scene
	# (src/ovl/club_master/club_master_scene_and_tables.c:451-469).
	var margin: int = practice_points - 350
	var grade: int = 0
	var updated_flags: int = current_flags
	if margin < -30:
		grade = 0
	elif margin < -10:
		grade = 1
	elif margin < 0:
		grade = 2
	elif margin < 10:
		grade = 3
		updated_flags |= (1 << 30) # b30 = 1 (club master crown)
	elif margin < 30:
		grade = 4
		updated_flags |= (1 << 30)
	else:
		grade = 5
		updated_flags |= (1 << 30)

	var tokimeki_bonus: int = 0x19 if grade >= 3 else 0x12 # 25 vs 18
	return {
		"margin": margin,
		"grade": grade,
		"club_master_crowned": grade >= 3,
		"tokimeki_bonus": tokimeki_bonus,
		"player_flags": updated_flags,
	}


static func select_master_companion(
	club_slot: int,
	girls: Array,
	closeness_tokimeki: Array
) -> Dictionary:
	# Mirrors master_companion_girl_select
	# (src/ovl/club_master/club_master_scene_and_tables.c:218-262).
	var best_girl: int = -1
	var best_score: int = 29 # threshold > 29 (requires >= 30)

	if club_slot in [3, 4, 5, 6]:
		var club_girl: int = club_slot
		if club_girl < girls.size() and int(girls[club_girl].get("met", 0)) != 0:
			var sc: int = int(closeness_tokimeki[club_girl]) if club_girl < closeness_tokimeki.size() else 0
			if sc >= 30:
				return {"companion_girl": club_girl, "score": sc}

	for i: int in range(mini(11, girls.size())):
		if int(girls[i].get("met", 0)) != 0:
			var sc: int = int(closeness_tokimeki[i]) if i < closeness_tokimeki.size() else 0
			if sc > best_score:
				best_score = sc
				best_girl = i

	return {
		"companion_girl": best_girl,
		"score": best_score if best_girl != -1 else 0,
	}
