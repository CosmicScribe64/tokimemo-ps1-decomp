class_name SubgameOverlays
extends RefCounted
## T-29018 (Lane 7): Deterministic rules & state machines for the three interactive
## subgame overlays decompiled in:
## - src/ovl/rpg_battle/ (RPG_BAT: Turn-based party combat on School Trip / Culture Festival)
## - src/ovl/shooter_3d/ (TACO: 3D rail shooter at Culture Festival / Arcade)
## - src/ovl/TT/         (TT: TwinBee Time Attack vertical shoot-'em-up)

# ==============================================================================
# 1. RPG_BAT (src/ovl/rpg_battle/ & include/ovl/rpg_battle.h)
# ==============================================================================

const RPG_ENCOUNTER_SNAKE := 0x10000
const RPG_ENCOUNTER_DEER := 0x20000
const RPG_ENCOUNTER_BEAR := 0x40000
const RPG_ENCOUNTER_DELINQUENTS := 0x100000
const RPG_ENCOUNTER_ROBOT := 0x08000000
const RPG_ENCOUNTER_ALIENS := 0x80000000

const RPG_PLAYER_ATTACK := 0x1
const RPG_PLAYER_SPECIAL := 0x2
const RPG_PLAYER_GUARD := 0x100
const RPG_PLAYER_READY := 0x10000
const RPG_COMMAND_ATTACK := 0x1000000
const RPG_COMMAND_GUARD := 0x2000000
const RPG_COMMAND_SPECIAL := 0x4000000

const RPG_PLAYER_READY_DELAY := 120
const RPG_GUARD_RECOVERY_FRAMES := 151
const RPG_SPECIAL_GAUGE_COST := 100
const RPG_SPECIAL_GAUGE_MAX := 170
const RPG_DAMAGE_MAXIMUM := 9999

# Initial partner girl hit points (include/ovl/rpg_battle.h:47-59)
const RPG_GIRL_INITIAL_HP := {
	0: 256,        # Shiori
	1: 99,         # Mio
	2: 312,        # Yuina
	3: 304,        # Ayako
	4: 189,        # Saki
	5: 298,        # Yukari
	6: 1878,       # Nozomi
	7: 277,        # Mira
	8: 365,        # Yuko
	9: 173,        # Megumi
	10: 265,       # Yumi
	13: 573,       # Yoshio
	14: 0x7FFFFFFF # Sentinel / None
}

# Partner girl short display names (labels_intro_and_wave_transition.c)
const RPG_GIRL_SHORT_NAME_HEX := {
	0: "8e8d9044",     # 詩織
	1: "96a28f8f",     # 未緒
	2: "8c8b93de",     # 結奈
	3: "8dca8e71",     # 彩子
	4: "8db98af3",     # 沙希
	5: "82e482a982e8", # ゆかり
	6: "965d",         # 望
	7: "96a39785",     # 魅羅
	8: "975b8e71",     # 夕子
	9: "88a4",         # 愛
	10: "974494fc",    # 優美
	13: "8d449759",    # 好雄
}

# Club special move banners (src/ovl/rpg_battle/*club_move*.c)
const RPG_CLUB_MOVE_BANNERS := {
	0: {"name": "　　　　　　メガトン辞書　　　　　　", "hex": "8140814081408140814081408381834b836783938eab8f91814081408140814081408140"},
	1: {"name": "　　　　　　　脱力漫才　　　　　　　", "hex": "8140814081408140814081408140924597cd969f8dcb8140814081408140814081408140"},
	2: {"name": "　　　　　　世界征服ロボ　　　　　　", "hex": "81408140814081408140814090a28a4590aa959e838d837b814081408140814081408140"},
	3: {"name": "　　　　　サイバーファング　　　　　", "hex": "8140814081408140814083548343836f815b837483408393834f81408140814081408140"},
	4: {"name": "　　　　　　　色素分解　　　　　　　", "hex": "81408140814081408140814081409046916695aa89f08140814081408140814081408140"},
	5: {"name": "　　　　ハーメルンのホラ吹き　　　　", "hex": "8140814081408140836e815b8381838b839382cc837a8389908182ab8140814081408140"},
	6: {"name": "　　　　　　超球百裂弾　　　　　　　", "hex": "81408140814081408140814092b48b85955397f492658140814081408140814081408140"},
	7: {"name": "　　　　　レッドカード乱舞　　　　　", "hex": "81408140814081408140838c83628368834a815b83689790959181408140814081408140"},
	8: {"name": "　　　　　　火の鳥サーブ　　　　　　", "hex": "81408140814081408140814089ce82cc92b98354815b8375814081408140814081408140"},
	9: {"name": "　　　　　　　水竜破　　　　　　　", "hex": "8140814081408140814081408140908597b3946a8140814081408140814081408140"},
	10: {"name": "　　　　　ナイアガラダンク　　　　　", "hex": "81408140814081408140836983438341834b8389835f8393834e81408140814081408140"},
	11: {"name": "　　　　　　　気分最高　　　　　　　", "hex": "81408140814081408140814081408b4395aa8dc58d828140814081408140814081408140"},
}

# All 11 girl partner assist moves (src/ovl/rpg_battle/move_assist_*.c, drama_move_and_partner_assists.c:167-203)
const RPG_PARTNER_ASSISTS := {
	0: {
		"girl": "shiori",
		"lines": ["藤崎「もう、", "なにバカなこと言ってるのよ。"],
		"lines_hex": ["93a18de8817582e082a48141", "82c882c9836f834a82c882b182c68cbe82c182c482e982cc82e68142"],
		"effect": "effect_a",
		"pose_anim": 0xD
	},
	1: {
		"girl": "mio",
		"lines": ["如月「いいかげんにして下さい…。"],
		"lines_hex": ["94408c8e817582a282a282a982b082f182c982b582c489ba82b382a281638142"],
		"effect": "effect_b",
		"pose_anim": 0xD
	},
	2: {
		"girl": "yuina",
		"lines": ["紐緒「馬鹿な事言ってるんじゃないわ"],
		"lines_hex": ["95528f8f8175946e8ead82c88e968cbe82c182c482e982f182b682e182c882a282ed"],
		"effect": "yuina_flash_x5",
		"pose_anim": 0xD
	},
	3: {
		"girl": "ayako",
		"lines": ["片桐「はっ、", "いいかげんにしなさいっ！"],
		"lines_hex": ["95d08bcb817582cd82c18141", "82a282a282a982b082f182c982b582c882b382a282c18149"],
		"effect": "effect_a",
		"pose_anim": 0xD
	},
	4: {
		"girl": "saki",
		"lines": ["虹野「な、なんだかな…"],
		"lines_hex": ["93f896ec817582c8814182c882f182be82a982c88163"],
		"effect": "sweatdrop",
		"pose_anim": 0xD
	},
	5: {
		"girl": "yukari",
		"lines": ["古式「まぁ、おもしろいですねぇ。"],
		"lines_hex": ["8cc38eae817582dc829f814182a882e082b582eb82a282c582b782cb82a58142"],
		"effect": "laugh",
		"pose_anim": 6
	},
	6: {
		"girl": "nozomi",
		"lines": ["清川「いいかげんに…してっ！"],
		"lines_hex": ["90b490ec817582a282a282a982b082f182c9816382b582c482c18149"],
		"lines_rough": ["清川「いいかげんに…しろっ！"],
		"lines_rough_hex": ["90b490ec817582a282a282a982b082f182c9816382b582eb82c18149"],
		"effect": "effect_b",
		"effect_rough": "effect_a",
		"pose_anim": 0xD
	},
	7: {
		"girl": "mira",
		"lines": ["鏡「つまらないわ…よっ！"],
		"lines_hex": ["8bbe817582c282dc82e782c882a282ed816382e682c18149"],
		"effect": "effect_a",
		"pose_anim": 0xD
	},
	8: {
		"girl": "yuko",
		"lines": ["朝日奈「つまんな〜い！"],
		"lines_hex": ["92a993fa93de817582c282dc82f182c8816082a28149"],
		"effect": "effect_a",
		"pose_anim": 0xD
	},
	9: {
		"girl": "megumi",
		"lines": ["美樹原「いいかげんに…", "い、いえ…やっぱりいいです…"],
		"lines_hex": ["94fc8ef78cb4817582a282a282a982b082f182c98163", "82a2814182a282a6816382e282c182cf82e882a282a282c582b78163"],
		"effect": "sweatdrop",
		"pose_anim": 0xD
	},
	10: {
		"girl": "yumi",
		"lines": ["早乙女「優美つまんなーい！"],
		"lines_hex": ["918189b38f978175974494fc82c282dc82f182c8815b82a28149"],
		"effect": "effect_a",
		"pose_anim": 0xD
	}
}

# Creates an initial RPG_BAT state matching rpg_init_player_state (battle_stats_and_action_delays.c:7-161)
static func rpg_create_state(config: Dictionary = {}) -> Dictionary:
	var encounter: int = int(config.get("encounter", RPG_ENCOUNTER_DELINQUENTS))
	var condition: int = maxi(1, int(config.get("param_condition", 97)))
	var athletics: int = maxi(0, int(config.get("param_athletics", 80)))
	var girl_id: int = int(config.get("girl_id", 0))
	var boss_event: int = int(config.get("boss_event", 1))
	var club_grades: Array = config.get("club_grades", [0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
	var club_practice: Array = config.get("club_practice", [0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

	if encounter == RPG_ENCOUNTER_ALIENS:
		girl_id = 2 # Yuina forced in alien encounter (battle_stats_and_action_delays.c:55)

	var player_club: int = 0xFF
	for i in range(mini(12, club_grades.size())):
		if int(club_grades[i]) == 3:
			player_club = i
			break
	if player_club == 11:
		var total_practice: int = 0
		for i in range(mini(12, club_practice.size())):
			total_practice += int(club_practice[i])
		if total_practice != 0:
			player_club = 0xFF

	var club_move_available: int = 0
	var special_ready: int = 0
	var special_gauge: int = 0
	if player_club != 0xFF and (encounter & 0x800F0000) == 0:
		club_move_available = 1
		special_ready = 1
		special_gauge = 100

	var player_max_hp: int = condition * 10
	var girl_max_hp: int = int(RPG_GIRL_INITIAL_HP.get(girl_id, 0x7FFFFFFF))

	var enemy_group_name := "不良達"
	var enemy_group_hex := "957397c79242"
	var guest_name := ""
	var guest_hex := ""
	var guest_hp: int = 0
	var enemy_hp: int = 65536
	var enemy_slot_gauges: Array[int] = [0, 0, 0]
	var damage_scale: int = 1000

	match encounter:
		RPG_ENCOUNTER_SNAKE:
			enemy_group_name = "ハブ・ロード"
			enemy_group_hex = "836e83758145838d815b8368"
			guest_name = "マングース"
			guest_hex = "837d8393834f815b8358"
			guest_hp = 117
			enemy_hp = 1
		RPG_ENCOUNTER_DEER:
			enemy_group_name = "鹿・グレート"
			enemy_group_hex = "8ead8145834f838c815b8367"
			guest_name = "虚無僧"
			guest_hex = "8b9596b3916d"
			guest_hp = 562
			enemy_hp = 1
		RPG_ENCOUNTER_BEAR:
			enemy_group_name = "キラーヒグマ"
			enemy_group_hex = "834c8389815b8371834f837d"
			guest_name = "マタギ"
			guest_hex = "837d835e834d"
			guest_hp = 298
			enemy_hp = 1
		RPG_ENCOUNTER_DELINQUENTS:
			if boss_event == 1:
				enemy_group_name = "不良達"
				enemy_group_hex = "957397c79242"
				enemy_slot_gauges = [5000, 5000, 5000]
				damage_scale = 1000
			else:
				enemy_group_name = "番長"
				enemy_group_hex = "94d492b7"
				enemy_slot_gauges = [9999, 9999, 9999]
				damage_scale = 1500
			enemy_hp = 65536
		RPG_ENCOUNTER_ROBOT:
			enemy_group_name = "真・世界征服ロボ"
			enemy_group_hex = "905e814590a28a4590aa959e838d837b"
			enemy_hp = 5000
			enemy_slot_gauges = [5000, 0, 0]
			damage_scale = 1000
		RPG_ENCOUNTER_ALIENS:
			enemy_group_name = "宇宙人"
			enemy_group_hex = "89469288906c"
			enemy_hp = 0x7FFFFFFF
			enemy_slot_gauges = [9999, 9999, 9999]
			damage_scale = 1000

	var practice_val: int = 300
	if player_club >= 0 and player_club < club_practice.size():
		practice_val = int(club_practice[player_club])

	return {
		"stage": 9, # Stage 9: rpg_stage_fight
		"encounter": encounter,
		"boss_event": boss_event,
		"player_name": str(config.get("player_name", "福原")),
		"player_name_hex": str(config.get("player_name_hex", "959f8cb4")),
		"player_hp": player_max_hp,
		"player_max_hp": player_max_hp,
		"param_athletics": athletics,
		"player_club": player_club,
		"club_practice_points": practice_val,
		"club_move_available": club_move_available,
		"special_ready": special_ready,
		"special_gauge": special_gauge,
		"player_action": RPG_PLAYER_READY,
		"player_ready_timer": 0,
		"guard_idle_timer": 0,
		"command_cursor": 0,
		"girl_id": girl_id,
		"girl_short_name_hex": str(RPG_GIRL_SHORT_NAME_HEX.get(girl_id, "8e8d9044")),
		"girl_hp": girl_max_hp,
		"girl_max_hp": girl_max_hp,
		"girl_action": 0,
		"guest_name": guest_name,
		"guest_hex": guest_hex,
		"guest_hp": guest_hp,
		"enemy_group_name": enemy_group_name,
		"enemy_group_hex": enemy_group_hex,
		"enemy_hp": enemy_hp,
		"enemy_slot_gauges": enemy_slot_gauges,
		"enemy_action": 0,
		"damage_scale": damage_scale,
		"last_damage": 0,
		"last_damage_slot1": 0,
		"last_damage_slot2": 0,
		"message_lines": [],
		"message_lines_hex": [],
		"last_assist": {},
		"rpg_over": 0,
		"victory": false,
		"defeat": false,
	}

# Basic attack damage roll matching rpg_player_basic_damage (player_damage.c:175-200)
static func rpg_player_basic_damage(param_athletics: int, rand_4096: int = 512,
		rand2_base: int = 100, rand2_crit: int = 750) -> int:
	var lv: int = mini(300, maxi(0, param_athletics))
	var base: int = clampi(rand2_base, 0, 200) + (lv * 9) / 2
	var dmg: int = base
	if rand_4096 == 0:
		dmg = RPG_DAMAGE_MAXIMUM
	elif rand_4096 < 64:
		dmg = 0
	elif rand_4096 < 270:
		dmg = base + clampi(rand2_crit, 0, 1500)
	if dmg >= 10000:
		dmg = RPG_DAMAGE_MAXIMUM
	return dmg

# Club special move damage roll matching rpg_player_damage_roll (player_damage.c:5-173)
static func rpg_player_club_damage(club_id: int, practice_points: int,
		encounter: int = RPG_ENCOUNTER_DELINQUENTS, rand_bonus: int = 250) -> Dictionary:
	var stat: int = clampi(practice_points, 50, 550)
	var base: int = stat * 10 - 500
	var primary: int = 0
	var slot1: int = 0
	var slot2: int = 0

	match club_id:
		0: # Literature (メガトン辞書)
			primary = (base * 3) / 5 + 1500 + clampi(rand_bonus, 0, 1000)
		1: # Drama (脱力漫才)
			if encounter == RPG_ENCOUNTER_ROBOT:
				primary = 4000 + clampi(rand_bonus, 0, 400)
			else:
				var d: int = (base * 2) / 5 + 1000
				primary = d + clampi(rand_bonus, 0, 500)
				slot1 = d + clampi(rand_bonus, 0, 500)
				slot2 = d + clampi(rand_bonus, 0, 500)
		2: # Science (世界征服ロボ)
			primary = RPG_DAMAGE_MAXIMUM
			slot1 = 2500 + clampi(rand_bonus * 10, 0, 5000)
			slot2 = 2500 + clampi(rand_bonus * 10, 0, 5000)
		3: # Computer (サイバーファング)
			var d: int = (base * 3) / 5 + 1000
			primary = d + clampi(rand_bonus, 0, 1000)
			slot1 = d + clampi(rand_bonus, 0, 1000)
			slot2 = d + clampi(rand_bonus, 0, 1000)
		4: # Art (色素分解)
			var d: int = (base * 2) / 5 + 1000
			primary = d + clampi(rand_bonus, 0, 500)
			slot1 = d + clampi(rand_bonus, 0, 500)
			slot2 = d + clampi(rand_bonus, 0, 500)
		5: # Brass Band (ハーメルンのホラ吹き)
			var d: int = (base * 2) / 5 + 1000
			primary = d + clampi(rand_bonus, 0, 1000)
			slot1 = d + clampi(rand_bonus, 0, 1000)
			slot2 = d + clampi(rand_bonus, 0, 1000)
		6: # Baseball (超球百裂弾)
			var d: int = (base * 3) / 5 + 1500
			primary = d + clampi(rand_bonus, 0, 1000)
			slot1 = d + clampi(rand_bonus, 0, 1000)
			slot2 = d + clampi(rand_bonus, 0, 1000)
		7: # Soccer (レッドカード乱舞)
			primary = (base * 4) / 5 + 1500 + clampi(rand_bonus, 0, 1000)
		8: # Tennis (火の鳥サーブ)
			primary = (base * 7) / 10 + 1500 + clampi(rand_bonus, 0, 1000)
		9: # Swimming (水竜破)
			var d: int = (base * 7) / 10 + 1500
			primary = d + clampi(rand_bonus, 0, 1000)
			slot1 = d + clampi(rand_bonus, 0, 1000)
			slot2 = d + clampi(rand_bonus, 0, 1000)
		10: # Basketball (ナイアガラダンク)
			primary = (base * 4) / 5 + 2000 + clampi(rand_bonus, 0, 1000)
		11: # No Club / Go-Home (気分最高)
			primary = 500 + clampi(rand_bonus, 0, 500)
		_:
			primary = 1000

	primary = mini(RPG_DAMAGE_MAXIMUM, primary)
	slot1 = mini(RPG_DAMAGE_MAXIMUM, slot1)
	slot2 = mini(RPG_DAMAGE_MAXIMUM, slot2)
	return {"primary": primary, "slot1": slot1, "slot2": slot2}

# Resolves partner assist move matching rpg_move_assist_dispatch (drama_move_and_partner_assists.c:167-203)
static func rpg_resolve_partner_assist(girl_id: int, nozomi_rough: bool = false) -> Dictionary:
	if not RPG_PARTNER_ASSISTS.has(girl_id):
		return {"girl_id": girl_id, "supported": false, "lines": [], "lines_hex": [], "effect": "none"}
	var entry: Dictionary = RPG_PARTNER_ASSISTS[girl_id]
	if girl_id == 6 and nozomi_rough:
		return {
			"girl_id": 6,
			"girl": "nozomi",
			"supported": true,
			"lines": entry["lines_rough"].duplicate(),
			"lines_hex": entry["lines_rough_hex"].duplicate(),
			"effect": entry["effect_rough"],
			"pose_anim": entry["pose_anim"]
		}
	return {
		"girl_id": girl_id,
		"girl": entry["girl"],
		"supported": true,
		"lines": entry["lines"].duplicate(),
		"lines_hex": entry["lines_hex"].duplicate(),
		"effect": entry["effect"],
		"pose_anim": entry["pose_anim"]
	}

# Executes a player command (0: Attack, 1: Guard, 2: Special) in RPG_BAT
static func rpg_execute_command(state: Dictionary, command_index: int,
		target_slot: int = 0, rand_seed: int = 512) -> Dictionary:
	state["command_cursor"] = clampi(command_index, 0, 2)
	var slot: int = clampi(target_slot, 0, 2)
	var gauge_gain: int = (rand_seed & 0xF)

	if command_index == 0: # Attack (たたかう)
		state["player_action"] = RPG_PLAYER_ATTACK
		state["special_gauge"] = mini(RPG_SPECIAL_GAUGE_MAX, int(state["special_gauge"]) + gauge_gain + 30)
		if int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1:
			state["special_ready"] = 1
		var dmg: int = rpg_player_basic_damage(int(state["param_athletics"]), rand_seed & 0xFFF)
		state["last_damage"] = dmg
		state["message_lines"] = [str(state["player_name"]) + "の攻撃！"]
		state["message_lines_hex"] = ["959f8cb4"]
		_rpg_apply_damage_to_slot(state, slot, dmg)
	elif command_index == 1: # Guard (ぼうぎょ)
		state["player_action"] = RPG_PLAYER_GUARD
		state["special_gauge"] = mini(RPG_SPECIAL_GAUGE_MAX, int(state["special_gauge"]) + gauge_gain + 20)
		if int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1:
			state["special_ready"] = 1
		state["guard_idle_timer"] = RPG_GUARD_RECOVERY_FRAMES
		var heal: int = maxi(1, int(state["player_max_hp"]) / 20)
		state["player_hp"] = mini(int(state["player_max_hp"]), int(state["player_hp"]) + heal)
		state["message_lines"] = ["福原の体力が５％回復した"]
		state["message_lines_hex"] = ["959f8cb482cc91cc97cd82aa8254819389f1959c82b582bd"]
	elif command_index == 2: # Special (わざ)
		if int(state["special_ready"]) == 1 and int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST:
			state["player_action"] = RPG_PLAYER_SPECIAL
			state["special_gauge"] = maxi(0, int(state["special_gauge"]) - RPG_SPECIAL_GAUGE_COST)
			state["special_ready"] = 1 if (int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1) else 0
			var club_id: int = int(state["player_club"])
			var banner: Dictionary = RPG_CLUB_MOVE_BANNERS.get(club_id, RPG_CLUB_MOVE_BANNERS[0])
			state["message_lines"] = [banner["name"]]
			state["message_lines_hex"] = [banner["hex"]]
			var roll: Dictionary = rpg_player_club_damage(club_id, int(state["club_practice_points"]), int(state["encounter"]), rand_seed % 500)
			state["last_damage"] = int(roll["primary"])
			state["last_damage_slot1"] = int(roll["slot1"])
			state["last_damage_slot2"] = int(roll["slot2"])
			if club_id == 1: # Drama club triggers partner assist!
				var assist: Dictionary = rpg_resolve_partner_assist(int(state["girl_id"]))
				state["last_assist"] = assist
			_rpg_apply_damage_to_slot(state, slot, int(roll["primary"]))
			if int(roll["slot1"]) > 0:
				_rpg_apply_damage_to_slot(state, 1, int(roll["slot1"]))
			if int(roll["slot2"]) > 0:
				_rpg_apply_damage_to_slot(state, 2, int(roll["slot2"]))
	return state

static func _rpg_apply_damage_to_slot(state: Dictionary, slot: int, dmg: int) -> void:
	var gauges: Array = state["enemy_slot_gauges"]
	if slot >= 0 and slot < gauges.size() and int(gauges[slot]) > 0:
		gauges[slot] = maxi(0, int(gauges[slot]) - dmg)
	var all_down := true
	for g in gauges:
		if int(g) > 0:
			all_down = false
			break
	if int(state["encounter"]) != RPG_ENCOUNTER_DELINQUENTS:
		state["enemy_hp"] = maxi(0, int(state["enemy_hp"]) - dmg)
		if int(state["enemy_hp"]) == 0:
			all_down = true
	if all_down:
		state["enemy_hp"] = 0
		state["rpg_over"] = 1
		state["victory"] = true
		state["stage"] = 10

static func rpg_apply_enemy_attack(state: Dictionary, raw_damage: int, target_is_girl: bool = false) -> int:
	var scaled: int = (raw_damage * int(state.get("damage_scale", 1000))) / 1000
	if (int(state.get("player_action", 0)) & RPG_PLAYER_GUARD) != 0 and not target_is_girl:
		scaled = maxi(1, scaled / 2)
	if target_is_girl:
		state["girl_hp"] = maxi(0, int(state["girl_hp"]) - scaled)
		if int(state["girl_hp"]) == 0:
			state["rpg_over"] = 2
			state["defeat"] = true
			state["stage"] = 12
	else:
		state["player_hp"] = maxi(0, int(state["player_hp"]) - scaled)
		if int(state["player_hp"]) == 0:
			state["rpg_over"] = 2
			state["defeat"] = true
			state["stage"] = 12
	return scaled


# ==============================================================================
# 2. TACO — 3D Rail Shooter (src/ovl/shooter_3d/ & include/ovl/shooter_3d.h)
# ==============================================================================

const SHOOTER_STAGE_LOAD := 0
const SHOOTER_STAGE_OPENING := 1
const SHOOTER_STAGE_TITLE := 2
const SHOOTER_STAGE_MENU := 3
const SHOOTER_STAGE_SETTINGS := 4
const SHOOTER_STAGE_SCORES := 5
const SHOOTER_STAGE_DEMO := 6
const SHOOTER_STAGE_HOW_TO_PLAY := 7
const SHOOTER_STAGE_RESULT := 8
const SHOOTER_STAGE_NAME_ENTRY := 9
const SHOOTER_STAGE_GAME_START := 10
const SHOOTER_STAGE_STAGE1 := 11
const SHOOTER_STAGE_BOSS1 := 12
const SHOOTER_STAGE_STAGE2 := 21
const SHOOTER_STAGE_BOSS2 := 22
const SHOOTER_STAGE_STAGE3 := 31
const SHOOTER_STAGE_BOSS3 := 32
const SHOOTER_STAGE_GAME_END := 45
const SHOOTER_STAGE_EXIT := 46

const SHOOTER_PRIMARY_WIDE := 0
const SHOOTER_PRIMARY_BEAM := 1
const SHOOTER_PRIMARY_VULCAN := 7

const SHOOTER_SECONDARY_HOMING := 0
const SHOOTER_SECONDARY_SPREAD := 1

const SHOOTER_BOMB_PULSE := 3
const SHOOTER_BOMB_BLAST := 4

const SHOOTER_MAX_SCORE := 9999999

static func shooter_create_state(config: Dictionary = {}) -> Dictionary:
	return {
		"stage": int(config.get("stage", SHOOTER_STAGE_STAGE1)),
		"score": int(config.get("score", 142500)),
		"stages_cleared": int(config.get("stages_cleared", 0)),
		"time_left": int(config.get("time_left", 5400)), # 90 seconds at 60fps -> 1:30.00
		"player": {
			"x": int(config.get("player_x", 0)),
			"y": int(config.get("player_y", -0x200)),
			"z": 0x800,
			"shield": int(config.get("shield", 0xFF)), # stats[1] = 255
			"mode": 1,
			"roll_phase": 0,
			"weapon": int(config.get("weapon", SHOOTER_PRIMARY_WIDE)),
			"auxiliary": int(config.get("auxiliary", SHOOTER_SECONDARY_HOMING)),
			"bomb_slots": [SHOOTER_BOMB_BLAST, SHOOTER_BOMB_PULSE, 0], # argument_x=4, argument_y=3, argument_z=0
		},
		"wingman": {
			"x": -0x380,
			"y": -0x180,
			"z": 0x800,
			"shield": 0x80, # 128
		},
		"enemies": [
			{"id": 16, "kind": "taco_fighter", "x": -0x500, "y": -0x300, "z": 0x1400, "hp": 6, "radius": 0x180, "score": 5000},
			{"id": 17, "kind": "taco_fighter", "x": 0x480, "y": -0x280, "z": 0x1800, "hp": 6, "radius": 0x180, "score": 5000},
			{"id": 18, "kind": "morph_core", "x": 0, "y": -0x380, "z": 0x2000, "hp": 24, "radius": 0x280, "score": 20000},
		],
		"shots": [],
		"rankings": [
			{"score": 2500000, "stage": 3, "name": "ＳＨＩ"},
			{"score": 2000000, "stage": 3, "name": "ＹＵＩ"},
			{"score": 1500000, "stage": 2, "name": "ＳＡＫ"},
			{"score": 1000000, "stage": 2, "name": "ＭＩＯ"},
			{"score": 500000, "stage": 1, "name": "ＹＯＳ"},
		]
	}

# 3D perspective projection matching TACO's GTE horizon camera
static func shooter_project_3d(x: int, y: int, z: int) -> Vector2i:
	var safe_z: int = maxi(0x100, z)
	var persp: int = 0x200
	var sx: int = 160 + (x * persp) / safe_z
	var sy: int = 120 + (y * persp) / safe_z
	return Vector2i(sx, sy)

# Steps player movement, barrel roll, and timer (player_control.c:6-240)
static func shooter_step_frame(state: Dictionary, move_dx: int = 0, move_dy: int = 0,
		trigger_roll: bool = false, fire_primary: bool = false, fire_bomb: bool = false) -> Dictionary:
	var p: Dictionary = state["player"]
	p["x"] = clampi(int(p["x"]) + move_dx * 0x80, -0x1000, 0x1000)
	p["y"] = clampi(int(p["y"]) + move_dy * 0x60, -0x800, 0x180)

	if trigger_roll and int(p["mode"]) == 1:
		p["mode"] = 0x81 if move_dx <= 0 else 0x82
		p["roll_phase"] = 0x7F
	elif (int(p["mode"]) & 0x80) != 0:
		p["roll_phase"] = maxi(0, int(p["roll_phase"]) - 4)
		if int(p["roll_phase"]) < 0x40:
			p["mode"] = 1

	if fire_primary:
		var w: int = int(p["weapon"])
		var dmg: int = 2 if w == SHOOTER_PRIMARY_WIDE else (3 if w == SHOOTER_PRIMARY_BEAM else 1)
		state["shots"].append({"x": int(p["x"]), "y": int(p["y"]), "z": int(p["z"]) + 0x100, "damage": dmg, "flags": 0x98})

	if fire_bomb:
		shooter_fire_bomb(state)

	if int(state["time_left"]) > 0:
		state["time_left"] = int(state["time_left"]) - 1

	return state

# Fires a bomb from the front slot and shifts remaining slots (bombs.c:5-68)
static func shooter_fire_bomb(state: Dictionary) -> int:
	var p: Dictionary = state["player"]
	var slots: Array = p["bomb_slots"]
	var fired: int = int(slots[0])
	if fired == 0:
		return 0
	slots[0] = int(slots[1])
	slots[1] = int(slots[2])
	slots[2] = 0
	# Apply AoE bomb damage to active enemies
	var bomb_dmg: int = 12 if fired == SHOOTER_BOMB_BLAST else 8
	for enemy: Dictionary in state["enemies"]:
		if int(enemy["hp"]) > 0:
			enemy["hp"] = maxi(0, int(enemy["hp"]) - bomb_dmg)
			if int(enemy["hp"]) == 0:
				state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(enemy["score"]))
	return fired

# Computes result screen bonuses matching shooter_result_step_compute (result_screen.c:59-109)
static func shooter_compute_result_tally(state: Dictionary) -> Dictionary:
	var stages_cleared: int = int(state.get("stages_cleared", 0))
	var base_score: int = int(state.get("score", 0))
	var p: Dictionary = state["player"]
	var shield: int = int(p.get("shield", 0))
	var time_left: int = int(state.get("time_left", 0))
	var slots: Array = p.get("bomb_slots", [0, 0, 0])

	var stage_bonus: int = stages_cleared * 1000000
	var shield_bonus: int = 0
	var time_bonus: int = 0
	if stages_cleared >= 3:
		shield_bonus = 500000 if shield == 0xFF else shield * 1000
		time_bonus = time_left * 100
	else:
		shield_bonus = shield * 100
		time_bonus = 0

	var bomb_stock_bonus: int = 0
	if int(slots[2]) != 0:
		bomb_stock_bonus = 500000
	elif int(slots[1]) != 0:
		bomb_stock_bonus = 250000
	elif int(slots[0]) != 0:
		bomb_stock_bonus = 100000

	var total_score: int = mini(SHOOTER_MAX_SCORE, base_score + stage_bonus + shield_bonus + time_bonus + bomb_stock_bonus)
	return {
		"base_score": base_score,
		"stage_bonus": stage_bonus,
		"shield_bonus": shield_bonus,
		"time_bonus": time_bonus,
		"bomb_stock_bonus": bomb_stock_bonus,
		"total_score": total_score
	}

# Inserts a score into the 5-slot ranking table (score_rankings.c)
static func shooter_try_insert_ranking(state: Dictionary, total_score: int, name_str: String = "ＦＵＫ") -> int:
	var rankings: Array = state["rankings"]
	for i in range(rankings.size()):
		if total_score >= int(rankings[i]["score"]):
			rankings.insert(i, {"score": total_score, "stage": int(state["stages_cleared"]), "name": name_str})
			if rankings.size() > 5:
				rankings.resize(5)
			return i
	return -1


# ==============================================================================
# 3. TT — TwinBee Time Attack (src/ovl/TT/ & include/ovl/TT.h)
# ==============================================================================

const TT_STATE_INIT := 0
const TT_STATE_TITLE := 1
const TT_STATE_STAGE := 2
const TT_STATE_STAGE_CLEAR := 3
const TT_STATE_VICTORY := 4
const TT_STATE_GAME_OVER := 5
const TT_STATE_NAME_ENTRY := 6
const TT_STATE_RANKING_VIEW := 7
const TT_STATE_RESET := 8

const TT_MAX_TIME_FRAMES := 0x8CA0 # 36000 frames -> 9:59.99

# Exact 32-entry bell kind sequence from asm/ovl/TT/data/TT_data_80152B00.data.s:3654-3687
const TT_BELL_KIND_SEQUENCE: Array[int] = [
	0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 2,
	0, 0, 0, 0, 3,
	0, 0, 0, 0, 6,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
]

# Hitboxes from asm/ovl/TT/data/TT_data_80152B00.data.s:3691-3723
const TT_HITBOX_PLAYER := {"x": 0, "y": -1, "half_w": 3, "half_h": 4}
const TT_HITBOX_SHOT := {"x": 0, "y": 0, "half_w": 5, "half_h": 3}
const TT_HITBOX_AIR_ENEMY := {"x": 0, "y": 0, "half_w": 5, "half_h": 5}
const TT_HITBOX_PICKUP := {"x": 0, "y": 0, "half_w": 6, "half_h": 6}

static func tt_create_state(config: Dictionary = {}) -> Dictionary:
	return {
		"state": int(config.get("state", TT_STATE_STAGE)),
		"sub_state": int(config.get("sub_state", 0x40)),
		"stage_index": int(config.get("stage_index", 0)), # 0..3 (Stages 1..4)
		"elapsed_frames": int(config.get("elapsed_frames", 4250)), # 1:10.83
		"lives": int(config.get("lives", 2)),
		"bell_hit_counter": int(config.get("bell_hit_counter", 0)), # run[0xD]
		"scroll_speed": 0x18000, # step 3 of 6 on speed gauge
		"player": {
			"active": 1,
			"state": 0x40,
			"x": int(config.get("player_x", 0)),       # [-120, 120] relative to center 160
			"y": int(config.get("player_y", 192)),     # [56, 224]
			"pos_x_fixed": int(config.get("player_x", 0)) << 16,
			"pos_y_fixed": int(config.get("player_y", 192)) << 16,
			"invulnerable": 0,
			"has_options": int(config.get("has_options", 1)), # player[0x53]
			"speed_level": int(config.get("speed_level", 4)), # player[0x54]
			"shot_kind": int(config.get("shot_kind", 9)),     # player[0x55]: 8=Normal, 9=Twin, 10=3-Way
			"left_arm_swing": 0,
			"right_arm_swing": 0,
		},
		"bells": [
			{"active": 1, "state": 0x40, "x": -36, "y": 96, "kind": 1, "cycles": 0, "vy": -0x18000}, # Blue Twin-Shot Bell
			{"active": 1, "state": 0x40, "x": 42, "y": 116, "kind": 0, "cycles": 0, "vy": 0x10000},  # Yellow Bell
			{"active": 1, "state": 0x40, "x": 4, "y": 76, "kind": 3, "cycles": 0, "vy": -0x10000},   # White Option Bell
		],
		"air_enemies": [
			{"active": 1, "state": 0x40, "kind": 0x40, "x": -52, "y": 64},
			{"active": 1, "state": 0x40, "kind": 0x41, "x": 52, "y": 68},
		],
		"enemy_shots": [
			{"active": 1, "state": 0x40, "kind": 0xA0, "x": -24, "y": 132},
			{"active": 1, "state": 0x40, "kind": 0xA0, "x": 28, "y": 140},
		],
		"boss": {
			"active": 1,
			"boss_id": int(config.get("boss_id", 0)), # 0: Rope Arm, 1: Swing Ring, 2: Multipart, 3: Beam/Ring
			"x": 0,
			"y": 54,
			"core_hp": 64,
			"side_hp": [24, 24, 24, 24],
		},
		# Default rankings from tt_config_load (engine_initialization.c:66-80)
		"rankings": [
			{"time": 0x63C8, "name_codes": [0x14, 0x24, 0x18], "name": "Ｋ．Ｏ"}, # 7:05.73
			{"time": 0x71D8, "name_codes": [0x17, 0x24, 0x0A], "name": "Ｎ．Ａ"}, # 8:05.73
			{"time": 0x7FE8, "name_codes": [0x16, 0x24, 0x12], "name": "Ｍ．Ｉ"}, # 9:05.73
		]
	}

# Formats elapsed frames into TwinBee Time Attack M:SS.CC digits matching tt_time_text_set (text_and_time_display.c:91-108)
static func tt_format_time(elapsed_frames: int) -> Dictionary:
	var t: int = clampi(elapsed_frames, 0, TT_MAX_TIME_FRAMES)
	if t >= TT_MAX_TIME_FRAMES:
		return {"minutes": 9, "sec_tens": 5, "sec_ones": 9, "cs_tens": 9, "cs_ones": 9, "text": "9:59.99"}
	var m: int = t / 3600
	var s_tens: int = (t % 3600) / 600
	var s_ones: int = (t % 600) / 60
	var cs_tens: int = ((t % 60) * 10) / 60
	var cs_ones: int = ((t * 10) / 6) % 10
	return {
		"minutes": m,
		"sec_tens": s_tens,
		"sec_ones": s_ones,
		"cs_tens": cs_tens,
		"cs_ones": cs_ones,
		"text": "%d:%d%d.%d%d" % [m, s_tens, s_ones, cs_tens, cs_ones]
	}

# Exact box overlap check matching tt_box_overlap (level_clear_and_collisions.c:234-260)
static func tt_box_overlap(pos_a: Vector2i, hitbox_a: Dictionary, pos_b: Vector2i, hitbox_b: Dictionary) -> bool:
	var x1: int = int(hitbox_a["x"]) + pos_a.x
	var y1: int = int(hitbox_a["y"]) + pos_a.y
	var x2: int = int(hitbox_b["x"]) + pos_b.x
	var y2: int = int(hitbox_b["y"]) + pos_b.y
	var dx: int = absi(x1 - x2)
	var dy: int = absi(y1 - y2)
	if dx < int(hitbox_a["half_w"]) + int(hitbox_b["half_w"]):
		if dy < int(hitbox_a["half_h"]) + int(hitbox_b["half_h"]):
			return true
	return false

# Handles a shot hitting a bell matching tt_bell_hit_by_shot (cycling_pickups.c:277-373)
static func tt_bell_hit_by_shot(state: Dictionary, bell_index: int, delta_x: int = 0) -> Dictionary:
	var bells: Array = state["bells"]
	if bell_index < 0 or bell_index >= bells.size():
		return {}
	var bell: Dictionary = bells[bell_index]
	var p: Dictionary = state["player"]
	state["bell_hit_counter"] = int(state["bell_hit_counter"]) + 1
	var hit_cnt: int = int(state["bell_hit_counter"])
	var bstate: int = int(bell["state"])

	if bstate == 0x20 or bstate == 0x40:
		bell["state"] = 0x40
		if hit_cnt >= 0x15: # 21
			state["bell_hit_counter"] = 1
			hit_cnt = 1
			bell["cycles"] = int(bell.get("cycles", 0)) + 1
			if int(bell["cycles"]) >= 2:
				state["bell_hit_counter"] = 0
				hit_cnt = 0
				bell["state"] = 0x60
		if int(bell["state"]) == 0x40:
			var kind: int = TT_BELL_KIND_SEQUENCE[clampi(hit_cnt, 0, TT_BELL_KIND_SEQUENCE.size() - 1)]
			# Suppress redundant powerup kinds matching cycling_pickups.c:311-332
			if kind == 1 and int(p["shot_kind"]) == 9:
				kind = 0
			elif kind == 2 and int(p["speed_level"]) >= 10:
				kind = 0
			elif kind == 3 and int(p["has_options"]) != 0:
				kind = 0
			elif kind == 6 and int(p["speed_level"]) == 0:
				kind = 0
			bell["kind"] = kind
	elif bstate == 0x60:
		if hit_cnt >= 4:
			bell["state"] = 0x70
			bell["vy"] = 0x20000
			return bell
	elif bstate == 0x70:
		bell["active"] = 0
		return bell

	bell["vy"] = -0x18000
	bell["vx"] = 0x1000 if delta_x < -1 else (-0x1000 if delta_x >= 2 else 0)
	return bell

# Collects a bell pickup matching tt_bell_collect (player_and_asset_uploads.c:810-919)
static func tt_bell_collect(state: Dictionary, bell_index: int) -> Dictionary:
	var bells: Array = state["bells"]
	if bell_index < 0 or bell_index >= bells.size():
		return {}
	var bell: Dictionary = bells[bell_index]
	var p: Dictionary = state["player"]
	var bstate: int = int(bell["state"])

	if bstate == 0x40 or bstate == 0x60:
		var kind: int = int(bell["kind"])
		match kind:
			0:
				bell["active"] = 0
			1: # Twin Shot
				p["shot_kind"] = 9
				bell["state"] = 0x90
				bell["popup_label"] = 40
			2: # Speed Up
				p["speed_level"] = int(p["speed_level"]) + 1
				bell["state"] = 0x90
				bell["popup_label"] = 41
			3: # 3 Option Followers
				p["has_options"] = 1
				bell["state"] = 0x90
				bell["popup_label"] = 42
			6: # Speed Down
				p["speed_level"] = maxi(1, int(p["speed_level"]) - 1)
				bell["state"] = 0x90
				bell["popup_label"] = 43
	elif bstate == 0x70:
		if int(p["invulnerable"]) == 0:
			p["state"] = 0x60
			p["invulnerable"] = 1
			p["has_options"] = 0
			bell["active"] = 0
	return bell

# Triggers smart bomb ground pickup matching tt_smart_bomb (level_clear_and_collisions.c:185-227)
static func tt_trigger_smart_bomb(state: Dictionary) -> void:
	for enemy: Dictionary in state["air_enemies"]:
		if int(enemy.get("active", 0)) != 0 and int(enemy.get("state", 0)) == 0x40:
			enemy["state"] = 0x70
	for shot: Dictionary in state["enemy_shots"]:
		if int(shot.get("active", 0)) != 0 and int(shot.get("state", 0)) == 0x40:
			shot["active"] = 0

# Tries to insert elapsed_frames into the 3-slot ranking table matching tt_ranking_try_insert (ranking_name_entry.c:5-39)
static func tt_try_insert_ranking(state: Dictionary, elapsed_frames: int) -> int:
	var rankings: Array = state["rankings"]
	var slot: int = -1
	if elapsed_frames < int(rankings[0]["time"]):
		rankings[2] = rankings[1].duplicate(true)
		rankings[1] = rankings[0].duplicate(true)
		slot = 0
	elif elapsed_frames < int(rankings[1]["time"]):
		rankings[2] = rankings[1].duplicate(true)
		slot = 1
	elif elapsed_frames < int(rankings[2]["time"]):
		slot = 2
	else:
		return -1

	rankings[slot] = {
		"time": elapsed_frames,
		"name_codes": [0x0A, 0x2F, 0x2F], # "A  " initial name entry state (ranking_name_entry.c:32-34)
		"name": "Ａ　　"
	}
	state["ranking_slot"] = slot
	return slot
