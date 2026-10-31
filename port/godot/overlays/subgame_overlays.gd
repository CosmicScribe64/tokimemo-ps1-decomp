class_name SubgameOverlays
extends RefCounted
## T-29018 / T-29024 (Lane 5): Source-backed interactive state machines & rules for:
## - src/ovl/rpg_battle/ (RPG_BAT: Turn-based party combat on School Trip / Culture Festival / Date)
## - src/ovl/shooter_3d/ (TACO: 3D rail shooter at Culture Festival / Arcade)
## - src/ovl/TT/         (TT: TwinBee Time Attack vertical shoot-'em-up)

# PS1 Controller Button Masks (include/main/pad.h)
const PAD_L2       := 0x0001
const PAD_R2       := 0x0002
const PAD_L1       := 0x0004
const PAD_R1       := 0x0008
const PAD_TRIANGLE := 0x0010
const PAD_CIRCLE   := 0x0020
const PAD_CROSS    := 0x0040
const PAD_SQUARE   := 0x0080
const PAD_SELECT   := 0x0100
const PAD_START    := 0x0800
const PAD_UP       := 0x1000
const PAD_RIGHT    := 0x2000
const PAD_DOWN     := 0x4000
const PAD_LEFT     := 0x8000

const GAME_MODE_RPG_BATTLE_NATIVE := 0x81
const GAME_MODE_SHOOTER_NATIVE := 0x90
const GAME_MODE_TT_NATIVE := 0xD0

const OVERLAY_ID_RPG_BAT := 0x57
const OVERLAY_ID_TACO := 0x59
const OVERLAY_ID_TT := 0x5A

const RPG_STAGE_BATTLE_ACTIVE := 9
const RPG_STAGE_VICTORY := 10
const RPG_STAGE_BANCHO_INTRO := 11
const RPG_STAGE_DEFEAT := 12
const RPG_STAGE_EXIT := 15

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

const RPG_RESULT_GOOD := 0
const RPG_RESULT_BAD := 1

const RPG_COMMAND_NAMES: Array[String] = ["attack", "defend", "magic", "item", "run"]

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

# Partner girl action delays from battle_stats_and_action_delays.c:244-276
const RPG_GIRL_ACTION_DELAY := {
	0: 200,
	1: 10,
	2: 300,
	3: 300,
	4: 300,
	5: 300,
	6: 260,
	7: 240,
	8: 260,
	9: 260,
	10: 260,
	13: 170,
	14: 0x7FFFFFFF,
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

# Loot table for trip & boss encounters (trip_enemy_attacks_and_rewards.c:136-180)
const RPG_LOOT_TABLES := {
	RPG_ENCOUNTER_SNAKE: ["おたから：ハブ茶", "おたから：ハブの牙", "おたから：マングースの毛", "おたから：南国の貝殻"],
	RPG_ENCOUNTER_DEER: ["おたから：鹿せんべい", "おたから：立派な角", "おたから：古都のお守り", "おたから：竹刀の鍔"],
	RPG_ENCOUNTER_BEAR: ["おたから：木彫りの熊", "おたから：マタギの証", "おたから：鮭の切り身", "おたから：北国の結晶"],
	RPG_ENCOUNTER_DELINQUENTS: ["おたから：番長の学ラン", "おたから：木刀", "おたから：必勝ハチマキ", "おたから：サングラス"],
	RPG_ENCOUNTER_ROBOT: ["おたから：超合金パーツ", "おたから：科学部の設計図"],
	RPG_ENCOUNTER_ALIENS: ["おたから：未知の鉱石", "おたから：ＵＦＯの破片"],
}

# Creates an initial RPG_BAT state matching rpg_init_player_state (battle_stats_and_action_delays.c:7-161)
static func rpg_create_state(config: Dictionary = {}) -> Dictionary:
	var encounter: int = int(config.get("encounter", RPG_ENCOUNTER_DELINQUENTS))
	if config.has("talk_result_code") and not config.has("encounter"):
		match int(config.get("talk_result_code", 4)):
			1: encounter = RPG_ENCOUNTER_SNAKE
			2: encounter = RPG_ENCOUNTER_DEER
			3: encounter = RPG_ENCOUNTER_BEAR
			4: encounter = RPG_ENCOUNTER_DELINQUENTS
			5: encounter = RPG_ENCOUNTER_DELINQUENTS
			6: encounter = RPG_ENCOUNTER_ROBOT
			7: encounter = RPG_ENCOUNTER_ALIENS
	var condition: int = maxi(1, int(config.get("param_condition", 97)))
	var athletics: int = maxi(0, int(config.get("param_athletics", 80)))
	var girl_id: int = int(config.get("girl_id", 0))
	var saved_girl: int = int(config.get("saved_girl", girl_id))
	var boss_event: int = int(config.get("boss_event", 1 if int(config.get("talk_result_code", 5)) != 4 else 0))
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
			enemy_hp = int(config.get("enemy_hp", 1))
			enemy_slot_gauges = [enemy_hp, 0, 0]
		RPG_ENCOUNTER_DEER:
			enemy_group_name = "鹿・グレート"
			enemy_group_hex = "8ead8145834f838c815b8367"
			guest_name = "虚無僧"
			guest_hex = "8b9596b3916d"
			guest_hp = 562
			enemy_hp = int(config.get("enemy_hp", 1))
			enemy_slot_gauges = [enemy_hp, 0, 0]
		RPG_ENCOUNTER_BEAR:
			enemy_group_name = "キラーヒグマ"
			enemy_group_hex = "834c8389815b8371834f837d"
			guest_name = "マタギ"
			guest_hex = "837d835e834d"
			guest_hp = 298
			enemy_hp = int(config.get("enemy_hp", 1))
			enemy_slot_gauges = [enemy_hp, 0, 0]
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

	if config.has("enemy_slot_gauges") and config["enemy_slot_gauges"] is Array:
		enemy_slot_gauges = [
			int(config["enemy_slot_gauges"][0]) if config["enemy_slot_gauges"].size() > 0 else 0,
			int(config["enemy_slot_gauges"][1]) if config["enemy_slot_gauges"].size() > 1 else 0,
			int(config["enemy_slot_gauges"][2]) if config["enemy_slot_gauges"].size() > 2 else 0,
		]
	if config.has("enemy_hp"):
		enemy_hp = int(config["enemy_hp"])

	var practice_val: int = 300
	if player_club >= 0 and player_club < club_practice.size():
		practice_val = int(club_practice[player_club])

	return {
		"overlay": "RPG_BAT",
		"game_mode": GAME_MODE_RPG_BATTLE_NATIVE,
		"stage": int(config.get("stage", 9)), # Stage 9: rpg_stage_fight
		"encounter": encounter,
		"boss_event": boss_event,
		"enable_bancho_escalation": bool(config.get("enable_bancho_escalation", false)),
		"bancho_escalated": false,
		"player_name": str(config.get("player_name", "福原")),
		"player_name_hex": str(config.get("player_name_hex", "959f8cb4")),
		"player_hp": int(config.get("player_hp", player_max_hp)),
		"player_max_hp": player_max_hp,
		"player_mp": int(config.get("player_mp", special_gauge)),
		"player_max_mp": RPG_SPECIAL_GAUGE_MAX,
		"param_athletics": athletics,
		"player_club": player_club,
		"club_practice_points": practice_val,
		"club_move_available": club_move_available,
		"special_ready": special_ready,
		"special_gauge": special_gauge,
		"player_action": RPG_PLAYER_READY,
		"player_ready_timer": 0,
		"guard_idle_timer": 0,
		"command_cursor": int(config.get("command_cursor", 0)),
		"commands": RPG_COMMAND_NAMES.duplicate(),
		"items_remaining": int(config.get("items_remaining", 2)),
		"allow_run": bool(config.get("allow_run", encounter == RPG_ENCOUNTER_ALIENS)),
		"girl_id": girl_id,
		"saved_girl": saved_girl,
		"girl_short_name_hex": str(RPG_GIRL_SHORT_NAME_HEX.get(girl_id, "8e8d9044")),
		"girl_hp": int(config.get("girl_hp", girl_max_hp)),
		"girl_max_hp": girl_max_hp,
		"girl_action": 0,
		"girl_timer": 0,
		"guest_name": guest_name,
		"guest_hex": guest_hex,
		"guest_hp": guest_hp,
		"enemy_group_name": enemy_group_name,
		"enemy_group_hex": enemy_group_hex,
		"enemy_hp": enemy_hp,
		"enemy_max_hp": enemy_hp,
		"enemy_slot_gauges": enemy_slot_gauges,
		"enemy_action": encounter,
		"enemy_timer": 0,
		"enemy_base_damage": int(config.get("enemy_base_damage", 120)),
		"damage_scale": damage_scale,
		"last_damage": 0,
		"last_damage_slot1": 0,
		"last_damage_slot2": 0,
		"last_enemy_damage": 0,
		"last_partner_damage": 0,
		"last_guest_damage": 0,
		"round": int(config.get("round", 0)),
		"turn_phase": "player_command",
		"turn_log": [],
		"message_lines": [],
		"message_lines_hex": [],
		"last_assist": {},
		"loot_rewards": [],
		"rpg_over": 0,
		"result_code": RPG_RESULT_GOOD,
		"talk_result_code": RPG_RESULT_GOOD,
		"victory": false,
		"defeat": false,
		"escaped": false,
		"completed": false,
		"saved_game_mode": int(config.get("saved_game_mode", 0x4C)),
		"saved_mode_stage": int(config.get("saved_mode_stage", 0)),
		"saved_mode_step": int(config.get("saved_mode_step", 0)),
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

# Executes a player command (0: Attack, 1: Guard/Defend, 2: Special/Magic, 3: Item, 4: Run) in RPG_BAT
static func rpg_execute_command(state: Dictionary, command_index: int,
		target_slot: int = 0, rand_seed: int = 512) -> Dictionary:
	var max_cmd_idx: int = maxi(2, (state.get("commands", RPG_COMMAND_NAMES) as Array).size() - 1)
	state["command_cursor"] = clampi(command_index, 0, max_cmd_idx)
	var slot: int = _rpg_select_active_slot(state, target_slot)
	var gauge_gain: int = (rand_seed & 0xF)

	if command_index == 0: # Attack (たたかう)
		state["player_action"] = RPG_PLAYER_ATTACK
		state["special_gauge"] = mini(RPG_SPECIAL_GAUGE_MAX, int(state["special_gauge"]) + gauge_gain + 30)
		state["player_mp"] = int(state["special_gauge"])
		if int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1:
			state["special_ready"] = 1
		var dmg: int = rpg_player_basic_damage(int(state["param_athletics"]), rand_seed & 0xFFF)
		state["last_damage"] = dmg
		state["last_damage_slot1"] = 0
		state["last_damage_slot2"] = 0
		state["message_lines"] = [str(state["player_name"]) + "の攻撃！"]
		state["message_lines_hex"] = ["959f8cb482cc8d558c828149"]
		_rpg_apply_damage_to_slot(state, slot, dmg)
	elif command_index == 1: # Guard / Defend (ぼうぎょ)
		state["player_action"] = RPG_PLAYER_GUARD
		state["special_gauge"] = mini(RPG_SPECIAL_GAUGE_MAX, int(state["special_gauge"]) + gauge_gain + 20)
		state["player_mp"] = int(state["special_gauge"])
		if int(state["special_gauge"]) >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1:
			state["special_ready"] = 1
		state["guard_idle_timer"] = RPG_GUARD_RECOVERY_FRAMES
		var heal: int = maxi(1, int(state["player_max_hp"]) / 20)
		state["player_hp"] = mini(int(state["player_max_hp"]), int(state["player_hp"]) + heal)
		state["last_damage"] = 0
		state["message_lines"] = ["福原の体力が５％回復した"]
		state["message_lines_hex"] = ["959f8cb482cc91cc97cd82aa8254819389f1959c82b582bd"]
	elif command_index == 2: # Special / Magic (わざ)
		var avail_mp: int = maxi(int(state.get("special_gauge", 0)), int(state.get("player_mp", 0)))
		if (int(state.get("special_ready", 0)) == 1 or int(state.get("club_move_available", 1)) == 1) and avail_mp >= RPG_SPECIAL_GAUGE_COST:
			state["player_action"] = RPG_PLAYER_SPECIAL
			var next_mp: int = maxi(0, avail_mp - RPG_SPECIAL_GAUGE_COST)
			state["special_gauge"] = next_mp
			state["player_mp"] = next_mp
			state["special_ready"] = 1 if (next_mp >= RPG_SPECIAL_GAUGE_COST and int(state["club_move_available"]) == 1) else 0
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
		else:
			# Insufficient special gauge: execute basic attack fallback so turn still progresses
			state["player_action"] = RPG_PLAYER_ATTACK
			var fallback_dmg: int = rpg_player_basic_damage(int(state["param_athletics"]), rand_seed & 0xFFF)
			state["last_damage"] = fallback_dmg
			state["message_lines"] = [str(state["player_name"]) + "の攻撃！"]
			state["message_lines_hex"] = ["959f8cb482cc8d558c828149"]
			_rpg_apply_damage_to_slot(state, slot, fallback_dmg)
	elif command_index == 3: # Item (アイテム)
		var items_left: int = int(state.get("items_remaining", 0))
		if items_left > 0:
			state["items_remaining"] = items_left - 1
			var p_heal: int = maxi(50, int(state["player_max_hp"]) * 3 / 10)
			state["player_hp"] = mini(int(state["player_max_hp"]), int(state["player_hp"]) + p_heal)
			if int(state.get("girl_id", 14)) != 14 and int(state.get("girl_max_hp", 0)) < 0x7FFFFFFF:
				var g_heal: int = maxi(30, int(state["girl_max_hp"]) * 3 / 10)
				state["girl_hp"] = mini(int(state["girl_max_hp"]), int(state["girl_hp"]) + g_heal)
			state["player_action"] = RPG_PLAYER_READY
			state["last_damage"] = 0
			state["message_lines"] = ["福原は回復アイテムを使った！"]
			state["message_lines_hex"] = ["959f8cb482cd89f1959c834183438365838082f08e6782c182bd8149"]
		else:
			state["last_damage"] = 0
			state["message_lines"] = ["福原の体力が５％回復した"]
			state["message_lines_hex"] = ["959f8cb482cc91cc97cd82aa8254819389f1959c82b582bd"]
	elif command_index == 4: # Run (にげる)
		if bool(state.get("allow_run", false)) or int(state.get("encounter", 0)) == RPG_ENCOUNTER_ALIENS:
			state["escaped"] = true
			state["rpg_over"] = 2
			state["result_code"] = RPG_RESULT_BAD
			state["talk_result_code"] = RPG_RESULT_BAD
			state["stage"] = 12
			state["completed"] = true
			state["message_lines"] = ["福原達は逃げ出した…"]
			state["message_lines_hex"] = ["959f8cb4924282cd93a682b08f6f82b582bd8163"]
		else:
			state["last_damage"] = 0
			state["message_lines"] = ["回り込まれて逃げられない！"]
			state["message_lines_hex"] = ["89f182e88d9e82dc82ea82c493a682b082e782ea82c882a28149"]
	return state

static func _rpg_select_active_slot(state: Dictionary, preferred_slot: int) -> int:
	var gauges: Array = state.get("enemy_slot_gauges", [0, 0, 0])
	var slot: int = clampi(preferred_slot, 0, maxi(0, gauges.size() - 1))
	if slot < gauges.size() and int(gauges[slot]) > 0:
		return slot
	for i in range(gauges.size()):
		if int(gauges[i]) > 0:
			return i
	return slot

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
		else:
			all_down = false
	else:
		var sum_gauges: int = 0
		for g in gauges:
			sum_gauges += maxi(0, int(g))
		state["enemy_hp"] = sum_gauges
	if all_down:
		# Check for Delinquent -> Bancho escalation when enabled (delinquent_boss_escalation.c)
		if int(state.get("encounter", 0)) == RPG_ENCOUNTER_DELINQUENTS and int(state.get("boss_event", 1)) == 1 and bool(state.get("enable_bancho_escalation", false)) and not bool(state.get("bancho_escalated", false)):
			state["bancho_escalated"] = true
			state["boss_event"] = 2
			state["enemy_group_name"] = "番長"
			state["enemy_group_hex"] = "94d492b7"
			state["enemy_slot_gauges"] = [9999, 0, 0]
			state["enemy_hp"] = 9999
			state["enemy_max_hp"] = 9999
			state["damage_scale"] = 1500
			state["victory"] = false
			state["rpg_over"] = 0
			state["stage"] = RPG_STAGE_BANCHO_INTRO
			return
		state["enemy_hp"] = 0
		state["rpg_over"] = 1
		state["victory"] = true
		state["result_code"] = RPG_RESULT_GOOD
		state["talk_result_code"] = RPG_RESULT_GOOD
		state["stage"] = 10
		state["turn_phase"] = "victory"
		state["loot_rewards"] = rpg_generate_loot_rewards(int(state.get("encounter", RPG_ENCOUNTER_DELINQUENTS)), int(state.get("boss_event", 1)))

static func rpg_apply_enemy_attack(state: Dictionary, raw_damage: int, target_is_girl: bool = false) -> int:
	var scaled: int = (raw_damage * int(state.get("damage_scale", 1000))) / 1000
	if (int(state.get("player_action", 0)) & RPG_PLAYER_GUARD) != 0 and not target_is_girl:
		scaled = maxi(1, scaled / 2)
	state["last_enemy_damage"] = scaled
	state["enemy_last_damage"] = scaled
	if target_is_girl:
		state["girl_hp"] = maxi(0, int(state["girl_hp"]) - scaled)
		if int(state["girl_hp"]) == 0:
			state["rpg_over"] = 2
			state["defeat"] = true
			state["result_code"] = RPG_RESULT_BAD
			state["talk_result_code"] = RPG_RESULT_BAD
			state["stage"] = 12
			state["turn_phase"] = "defeat"
	else:
		state["player_hp"] = maxi(0, int(state["player_hp"]) - scaled)
		if int(state["player_hp"]) == 0:
			state["rpg_over"] = 2
			state["defeat"] = true
			state["result_code"] = RPG_RESULT_BAD
			state["talk_result_code"] = RPG_RESULT_BAD
			state["stage"] = 12
			state["turn_phase"] = "defeat"
	return scaled

# Generates deterministic source-backed loot rewards matching rpg_trip_loot_text (trip_enemy_attacks_and_rewards.c:136-180)
static func rpg_generate_loot_rewards(encounter: int, _boss_event: int = 1, rand_seed: int = 0) -> Array[String]:
	var pool: Array = RPG_LOOT_TABLES.get(encounter, ["おたから：記念メダル", "おたから：必勝お守り"])
	var rewards: Array[String] = []
	var count: int = 3 if (rand_seed % 5 == 0 and pool.size() >= 3) else mini(2, pool.size())
	for i in range(count):
		var item: String = str(pool[(i + rand_seed) % pool.size()])
		if not rewards.has(item):
			rewards.append(item)
	return rewards

# Partner girl & guest autonomous action resolution (partner_action_selection.c & girl_action_*.c)
static func rpg_step_partner_and_guest(state: Dictionary, target_slot: int = 0) -> Dictionary:
	if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
		return state
	var slot: int = _rpg_select_active_slot(state, target_slot)
	state["last_partner_damage"] = 0
	state["last_guest_damage"] = 0

	# Guest ally attack in trip encounters (Mongoose / Monk / Hunter)
	var guest_hp: int = int(state.get("guest_hp", 0))
	if guest_hp > 0:
		var guest_dmg: int = maxi(25, guest_hp / 2 + 40)
		state["last_guest_damage"] = guest_dmg
		_rpg_apply_damage_to_slot(state, slot, guest_dmg)
		if bool(state.get("victory", false)):
			return state

	# Partner girl autonomous action
	var girl_id: int = int(state.get("girl_id", 14))
	if girl_id != 14 and int(state.get("girl_hp", 0)) > 0:
		var p_hp: int = int(state.get("player_hp", 0))
		var p_max: int = maxi(1, int(state.get("player_max_hp", 970)))
		if p_hp < (p_max / 2) and girl_id in [0, 1, 4, 5, 9]:
			var heal_amt: int = maxi(20, p_max / 10)
			state["player_hp"] = mini(p_max, p_hp + heal_amt)
			state["girl_action"] = 2 # Cheer / Heal
		else:
			var girl_dmg: int = 60 + (girl_id % 11) * 12
			state["girl_action"] = 1 # Partner Attack
			state["last_partner_damage"] = girl_dmg
			slot = _rpg_select_active_slot(state, slot)
			_rpg_apply_damage_to_slot(state, slot, girl_dmg)
	return state

# Enemy counter-attack turn step (trip_enemy_attacks_and_rewards.c:27-110)
static func rpg_step_enemy_turn(state: Dictionary, rand_seed: int = 512) -> Dictionary:
	if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
		return state
	var encounter: int = int(state.get("encounter", RPG_ENCOUNTER_DELINQUENTS))
	var base_dmg: int = int(state.get("enemy_base_damage", 120))
	match encounter:
		RPG_ENCOUNTER_SNAKE:
			state["enemy_attack_banner"] = "　　　　　テール・パンチ　　　　　"
			state["enemy_attack_banner_hex"] = "814081408140814081408365815b838b814583708393836081408140814081408140"
		RPG_ENCOUNTER_DEER:
			state["enemy_attack_banner"] = "　　　　　暴れん坊頭突き　　　　　"
			state["enemy_attack_banner_hex"] = "81408140814081408140965c82ea82f1965693aa93cb82ab81408140814081408140"
		RPG_ENCOUNTER_BEAR:
			state["enemy_attack_banner"] = "　　　　クラッシュ・クロー　　　　"
			state["enemy_attack_banner_hex"] = "8140814081408140834e83898362835683858145834e838d815b8140814081408140"
		_:
			state["enemy_attack_banner"] = str(state.get("enemy_group_name", "不良達")) + "の攻撃！"
			state["enemy_attack_banner_hex"] = str(state.get("enemy_group_hex", "957397c79242"))

	var raw_dmg: int = maxi(1, base_dmg + ((rand_seed & 0x1F) - 16))
	var target_girl: bool = bool(state.get("enemy_target_girl", false))
	if not state.has("enemy_target_girl"):
		var gid: int = int(state.get("girl_id", 14))
		target_girl = (gid != 14 and int(state.get("girl_hp", 0)) < 0x7FFFFFFF and ((int(state.get("round", 0)) + rand_seed) % 5 == 4))
	rpg_apply_enemy_attack(state, raw_dmg, target_girl)
	return state

# Executes a complete interactive combat round (Player Command -> Partner/Guest -> Enemy Counter-Attack)
static func rpg_step_turn(state: Dictionary, command: Variant, target_slot: int = 0, rand_seed: int = 512) -> Dictionary:
	if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
		state["completed"] = true
		return state

	if int(state.get("boss_event", 0)) == 1 and not state.has("enable_bancho_escalation_override"):
		state["enable_bancho_escalation"] = true

	var cmd_idx: int = 0
	if command is String:
		match String(command).to_lower():
			"attack", "fight": cmd_idx = 0
			"defend", "guard": cmd_idx = 1
			"magic", "special", "skill", "club": cmd_idx = 2
			"item", "heal": cmd_idx = 3
			"run", "escape", "retreat": cmd_idx = 4
			_: cmd_idx = int(state.get("command_cursor", 0))
	else:
		cmd_idx = int(command)

	state["round"] = int(state.get("round", 0)) + 1
	state["turn_count"] = int(state["round"])
	state["turn_phase"] = "player_action"
	rpg_execute_command(state, cmd_idx, target_slot, rand_seed + int(state["round"]) * 17)

	if int(state.get("stage", RPG_STAGE_BATTLE_ACTIVE)) != RPG_STAGE_BANCHO_INTRO and not bool(state.get("victory", false)) and not bool(state.get("defeat", false)) and not bool(state.get("escaped", false)):
		state["turn_phase"] = "party_assist"
		rpg_step_partner_and_guest(state, target_slot)

	if int(state.get("stage", RPG_STAGE_BATTLE_ACTIVE)) != RPG_STAGE_BANCHO_INTRO and not bool(state.get("victory", false)) and not bool(state.get("defeat", false)) and not bool(state.get("escaped", false)):
		state["turn_phase"] = "enemy_turn"
		rpg_step_enemy_turn(state, rand_seed + int(state["round"]) * 31)

	# Clear one-round guard stance after enemy turn resolves
	if (int(state.get("player_action", 0)) & RPG_PLAYER_GUARD) != 0 and not bool(state.get("keep_guard_flag", false)):
		state["last_player_action"] = state["player_action"]

	if not bool(state.get("victory", false)) and not bool(state.get("defeat", false)) and not bool(state.get("escaped", false)):
		state["turn_phase"] = "player_command"

	var log_entry := {
		"round": int(state["round"]),
		"command_index": cmd_idx,
		"command_name": RPG_COMMAND_NAMES[clampi(cmd_idx, 0, RPG_COMMAND_NAMES.size() - 1)],
		"player_damage": int(state.get("last_damage", 0)),
		"partner_damage": int(state.get("last_partner_damage", 0)),
		"guest_damage": int(state.get("last_guest_damage", 0)),
		"enemy_damage": int(state.get("last_enemy_damage", 0)),
		"player_hp": int(state.get("player_hp", 0)),
		"player_mp": int(state.get("player_mp", 0)),
		"girl_hp": int(state.get("girl_hp", 0)),
		"enemy_hp": int(state.get("enemy_hp", 0)),
	}
	if not state.has("turn_log") or not (state["turn_log"] is Array):
		state["turn_log"] = []
	(state["turn_log"] as Array).append(log_entry)
	return state

# Interactive input step for RPG_BAT overlay (navigation + confirmation + combat resolution)
static func rpg_step_interactive(state: Dictionary, input: Variant = 0, rand_seed: int = 512) -> Dictionary:
	if not state.has("player_hp"):
		var init_s: Dictionary = rpg_create_state(state)
		for k in init_s.keys():
			if not state.has(k):
				state[k] = init_s[k]

	var norm: Dictionary = _normalize_input(input)
	var commands: Array = state.get("commands", RPG_COMMAND_NAMES)
	var cmd_count: int = maxi(3, commands.size())
	var cursor: int = int(state.get("command_cursor", 0))

	if int(state.get("stage", RPG_STAGE_BATTLE_ACTIVE)) == RPG_STAGE_BANCHO_INTRO:
		if bool(norm["confirm"]) or bool(norm["cancel"]) or bool(norm["start"]):
			state["stage"] = RPG_STAGE_BATTLE_ACTIVE
			state["action"] = "bancho_intro_done"
			return state

	# If battle already concluded (stage 10 victory or stage 12 defeat/escape), confirm returns to caller
	if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
		if bool(norm["confirm"]) or bool(norm["cancel"]) or bool(norm["start"]):
			state["completed"] = true
			state["stage"] = RPG_STAGE_EXIT # rpg_exit
			state["return_payload"] = rpg_build_return_payload(state)
			state["action"] = "return_to_game"
		else:
			state["action"] = "battle_concluded"
		return state

	# Explicit command string override
	if not String(norm["command"]).is_empty():
		rpg_step_turn(state, String(norm["command"]), int(state.get("target_slot", 0)), rand_seed)
		if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
			state["return_payload"] = rpg_build_return_payload(state)
			state["action"] = "combat_resolved"
		else:
			state["action"] = "turn_resolved"
		return state

	if bool(norm["up"]) or bool(norm["left"]):
		cursor = (cursor + cmd_count - 1) % cmd_count
		state["command_cursor"] = cursor
		state["action"] = "cursor_moved"
	elif bool(norm["down"]) or bool(norm["right"]):
		cursor = (cursor + 1) % cmd_count
		state["command_cursor"] = cursor
		state["action"] = "cursor_moved"
	elif bool(norm["confirm"]):
		rpg_step_turn(state, cursor, int(state.get("target_slot", 0)), rand_seed)
		if bool(state.get("victory", false)) or bool(state.get("defeat", false)) or bool(state.get("escaped", false)):
			state["return_payload"] = rpg_build_return_payload(state)
			state["action"] = "combat_resolved"
		else:
			state["action"] = "turn_resolved"
	elif bool(norm["cancel"]):
		# Cancel selects Guard/Defend or advances message timer (battle_dispatch.c:113)
		state["command_cursor"] = 1
		state["action"] = "cursor_guard"
	else:
		state["action"] = "idle"

	return state

# Builds the caller return payload matching rpg_exit (battle_lifecycle_and_channels.c:106-119)
static func rpg_build_return_payload(state: Dictionary) -> Dictionary:
	var is_win: bool = bool(state.get("victory", false))
	var res_code: int = RPG_RESULT_GOOD if is_win else RPG_RESULT_BAD
	return {
		"overlay": "RPG_BAT",
		"game_mode": GAME_MODE_RPG_BATTLE_NATIVE,
		"talk_result_code": res_code,
		"rpg_result_code": res_code,
		"current_girl": int(state.get("saved_girl", state.get("girl_id", 0))),
		"victory": is_win,
		"defeat": bool(state.get("defeat", false)),
		"escaped": bool(state.get("escaped", false)),
		"rounds_fought": int(state.get("round", 0)),
		"player_hp": int(state.get("player_hp", 0)),
		"player_mp": int(state.get("player_mp", state.get("special_gauge", 0))),
		"girl_hp": int(state.get("girl_hp", 0)),
		"loot_rewards": (state.get("loot_rewards", []) as Array).duplicate(true),
		"return_game_mode": int(state.get("saved_game_mode", 0x4C)),
		"return_mode_stage": int(state.get("saved_mode_stage", 0)),
		"return_mode_step": int(state.get("saved_mode_step", 0)),
	}


# ==============================================================================
# 2. TACO: 3D Rail Shooter (src/ovl/shooter_3d/ & include/ovl/shooter_3d.h)
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
const SHOOTER_STAGE_SAVE_AND_EXIT := 46

const SHOOTER_PRIMARY_WIDE := 0
const SHOOTER_PRIMARY_BEAM := 1
const SHOOTER_PRIMARY_VULCAN := 7

const SHOOTER_SECONDARY_HOMING := 0
const SHOOTER_SECONDARY_SPREAD := 1

const SHOOTER_BOMB_PULSE := 3
const SHOOTER_BOMB_BLAST := 4

const SHOOTER_MAX_AMMO := 24
const SHOOTER_MAX_SCORE := 9999999

# Authenticated tables from CDROM/EXEDIR/TACO.EXN (.asset-cache/port/taco-combat-tables-v1/manifest.json)
const SHOOTER_SCORE_BY_CLASS: Array[int] = [
	200, 600, 2500, 800, 5000, 12000, 5000, 3000,
	400, 1000, 3000, 1000, 600, 2000, 150000, 50000, 100000
]
const SHOOTER_DAMAGE_BY_KIND: Array[int] = [1, 3, 6, 1, 6, 1, 1, 1, 2]
const SHOOTER_SE_COOLDOWN_RANGE: Array[int] = [1, 16, 2, 4, 4, 8, 8, 8, 8, 16]

static func shooter_create_state(config: Dictionary = {}) -> Dictionary:
	var px: int = int(config.get("player_x", 0))
	var py: int = int(config.get("player_y", -0x200))
	var ch_x: int = 160 + px / 32
	var ch_y: int = 92 + py / 48
	var max_ammo: int = int(config.get("max_ammo", 24))
	var stg: int = int(config.get("stage", SHOOTER_STAGE_STAGE1))
	return {
		"overlay": "TACO",
		"game_mode": GAME_MODE_SHOOTER_NATIVE,
		"stage": stg,
		"wave": int(config.get("wave", 0)),
		"waves_per_stage": int(config.get("waves_per_stage", 2)),
		"target_stages": int(config.get("target_stages", 3)),
		"score": int(config.get("score", 142500)),
		"stages_cleared": int(config.get("stages_cleared", 0)),
		"time_left": int(config.get("time_left", 5400)), # 90 seconds at 60fps -> 1:30.00
		"native_timer": int(config.get("native_timer", 18000)),
		"frame_count": int(config.get("frame_count", 0)),
		"player": {
			"x": px,
			"y": py,
			"z": 0x800,
			"crosshair_x": ch_x,
			"crosshair_y": ch_y,
			"shield": int(config.get("shield", 0xFF)), # stats[1] = 255
			"max_shield": 0xFF,
			"mode": 1,
			"roll_phase": 0,
			"weapon": int(config.get("weapon", SHOOTER_PRIMARY_WIDE)),
			"auxiliary": int(config.get("auxiliary", SHOOTER_SECONDARY_HOMING)),
			"ammo": int(config.get("ammo", max_ammo)),
			"max_ammo": max_ammo,
			"reloading": false,
			"reload_timer": 0,
			"reload_count": 0,
			"bomb_slots": [SHOOTER_BOMB_BLAST, SHOOTER_BOMB_PULSE, 0], # argument_x=4, argument_y=3, argument_z=0
		},
		"wingman": {
			"x": -0x380,
			"y": -0x180,
			"z": 0x800,
			"shield": 0x80, # 128
			"active": 1,
		},
		"enemies": [
			{"id": 16, "class_id": 4, "kind": "taco_fighter", "x": -0x500, "y": -0x300, "z": 0x1400, "hp": 6, "max_hp": 6, "radius": 0x180, "score": 5000, "fire_cooldown": 12},
			{"id": 17, "class_id": 4, "kind": "taco_fighter", "x": 0x480, "y": -0x280, "z": 0x1800, "hp": 6, "max_hp": 6, "radius": 0x180, "score": 5000, "fire_cooldown": 16},
			{"id": 18, "class_id": 5, "kind": "morph_core", "x": 0, "y": -0x380, "z": 0x2000, "hp": 24, "max_hp": 24, "radius": 0x280, "score": 20000, "fire_cooldown": 20},
		],
		"boss": {
			"active": 1 if stg in [SHOOTER_STAGE_BOSS1, SHOOTER_STAGE_BOSS2, SHOOTER_STAGE_BOSS3] else 0,
			"kind": "arm_tail_boss" if stg == SHOOTER_STAGE_BOSS1 else ("morph_boss" if stg == SHOOTER_STAGE_BOSS2 else "collapse_boss"),
			"class_id": 14 if stg == SHOOTER_STAGE_BOSS1 else (15 if stg == SHOOTER_STAGE_BOSS2 else 16),
			"x": 0,
			"y": -0x200,
			"z": 0x1600,
			"hp": 48,
			"max_hp": 48,
			"radius": 0x380,
			"score": 150000 if stg == SHOOTER_STAGE_BOSS1 else (50000 if stg == SHOOTER_STAGE_BOSS2 else 100000),
			"phase": 0,
			"fire_cooldown": 10,
		},
		"shots": [],
		"secondary_shots": [],
		"enemy_shots": [],
		"pickups": [],
		"shield_hits": 0,
		"enemies_destroyed": 0,
		"stage_clear": false,
		"game_over": false,
		"completed": false,
		"end_reason": "",
		"entered_from_story": bool(config.get("entered_from_story", true)),
		"saved_game_mode": int(config.get("saved_game_mode", 0x48)),
		"saved_mode_stage": int(config.get("saved_mode_stage", 0)),
		"saved_mode_step": int(config.get("saved_mode_step", 0)),
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
	p["crosshair_x"] = 160 + int(p["x"]) / 32
	p["crosshair_y"] = 92 + int(p["y"]) / 48

	if trigger_roll and int(p["mode"]) == 1:
		p["mode"] = 0x81 if move_dx <= 0 else 0x82
		p["roll_phase"] = 0x7F
	elif (int(p["mode"]) & 0x80) != 0:
		p["roll_phase"] = maxi(0, int(p["roll_phase"]) - 4)
		if int(p["roll_phase"]) < 0x40:
			p["mode"] = 1

	# Advance previously fired shots in 3D space and check collisions (collision.c)
	_shooter_advance_shots_and_enemies(state)

	if bool(p.get("reloading", false)):
		p["reload_timer"] = maxi(0, int(p.get("reload_timer", 0)) - 1)
		if int(p["reload_timer"]) == 0:
			p["reloading"] = false
			p["ammo"] = int(p.get("max_ammo", 24))

	if fire_primary:
		var w: int = int(p["weapon"])
		var dmg: int = 2 if w == SHOOTER_PRIMARY_WIDE else (3 if w == SHOOTER_PRIMARY_BEAM else 1)
		if int(p.get("ammo", 24)) > 0:
			p["ammo"] = int(p.get("ammo", 24)) - 1
		state["shots"].append({
			"x": int(p["x"]),
			"y": int(p["y"]),
			"z": int(p["z"]) + 0x100,
			"damage": dmg,
			"flags": 0x98,
			"target_screen": Vector2i(int(p["crosshair_x"]), int(p["crosshair_y"])),
		})

	if fire_bomb:
		shooter_fire_bomb(state)

	if int(state["time_left"]) > 0 and int(state.get("stages_cleared", 0)) < 3:
		state["time_left"] = int(state["time_left"]) - 1
		if state.has("native_timer") and int(state["native_timer"]) > 0:
			state["native_timer"] = int(state["native_timer"]) - 1
		if int(state["time_left"]) == 0 and int(state.get("stage", SHOOTER_STAGE_STAGE1)) in [
			SHOOTER_STAGE_STAGE1, SHOOTER_STAGE_BOSS1,
			SHOOTER_STAGE_STAGE2, SHOOTER_STAGE_BOSS2,
			SHOOTER_STAGE_STAGE3, SHOOTER_STAGE_BOSS3
		]:
			state["game_over"] = true
			state["end_reason"] = "time_over"
			state["stage"] = SHOOTER_STAGE_GAME_END

	state["frame_count"] = int(state.get("frame_count", 0)) + 1
	return state

# Advances active player shots, checks hit spheres against enemies/boss, and updates enemy projectiles
static func _shooter_advance_shots_and_enemies(state: Dictionary) -> void:
	var p: Dictionary = state["player"]
	var remaining_shots: Array = []
	for shot: Dictionary in state.get("shots", []):
		shot["z"] = int(shot.get("z", 0x900)) + 0x400
		var hit_any := false
		var s_dmg: int = int(shot.get("damage", 2))
		var s_proj: Vector2i = shooter_project_3d(int(shot.get("x", 0)), int(shot.get("y", -0x200)), int(shot.get("z", 0x900)))
		var ch_pos: Vector2i = shot.get("target_screen", Vector2i(int(p.get("crosshair_x", 160)), int(p.get("crosshair_y", 80))))

		# Check active boss hit
		if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0 and int(state["boss"].get("hp", 0)) > 0:
			var boss: Dictionary = state["boss"]
			var b_proj: Vector2i = shooter_project_3d(int(boss.get("x", 0)), int(boss.get("y", -0x200)), int(boss.get("z", 0x1600)))
			if ch_pos.distance_to(b_proj) <= 44.0 or s_proj.distance_to(b_proj) <= 44.0 or absi(int(shot["x"]) - int(boss.get("x", 0))) <= int(boss.get("radius", 0x380)):
				boss["hp"] = maxi(0, int(boss["hp"]) - s_dmg)
				hit_any = true
				if int(boss["hp"]) == 0:
					boss["active"] = 0
					state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(boss.get("score", 100000)))
					state["enemies_destroyed"] = int(state.get("enemies_destroyed", 0)) + 1

		# Check active enemies hit
		if not hit_any:
			for enemy: Dictionary in state.get("enemies", []):
				if int(enemy.get("hp", 0)) <= 0:
					continue
				var e_proj: Vector2i = shooter_project_3d(int(enemy.get("x", 0)), int(enemy.get("y", -0x300)), int(enemy.get("z", 0x1800)))
				var hit_screen: bool = ch_pos.distance_to(e_proj) <= 36.0 or s_proj.distance_to(e_proj) <= 36.0
				var hit_world: bool = absi(int(shot["x"]) - int(enemy.get("x", 0))) <= int(enemy.get("radius", 0x180)) * 2 and absi(int(shot["y"]) - int(enemy.get("y", -0x300))) <= int(enemy.get("radius", 0x180)) * 2
				if hit_screen or hit_world:
					enemy["hp"] = maxi(0, int(enemy["hp"]) - s_dmg)
					hit_any = true
					if int(enemy["hp"]) == 0:
						state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(enemy.get("score", 5000)))
						state["enemies_destroyed"] = int(state.get("enemies_destroyed", 0)) + 1
					break

		if not hit_any and int(shot["z"]) < 0x2800:
			remaining_shots.append(shot)
	state["shots"] = remaining_shots

	# Advance enemy shots toward player plane z = 0x800 (enemy_bullets_and_pickups.c)
	if state.has("enemy_shots") and state["enemy_shots"] is Array:
		var next_eshots: Array = []
		for eshot: Dictionary in state["enemy_shots"]:
			var vz_step: int = maxi(1, absi(int(eshot.get("vz", 0x200))))
			eshot["z"] = int(eshot.get("z", 0x1400)) - vz_step
			if int(eshot["z"]) <= 0x800:
				var dx: int = absi(int(eshot.get("x", 0)) - int(p.get("x", 0)))
				var dy: int = absi(int(eshot.get("y", -0x200)) - int(p.get("y", -0x200)))
				var rolling: bool = (int(p.get("mode", 1)) & 0x80) != 0
				if not rolling and dx <= 0x380 and dy <= 0x380:
					var e_dmg: int = int(eshot.get("damage", 16))
					p["shield"] = maxi(0, int(p.get("shield", 0xFF)) - e_dmg)
					state["shield_hits"] = int(state.get("shield_hits", 0)) + 1
					if int(p["shield"]) == 0:
						state["game_over"] = true
						state["end_reason"] = "shield_depleted"
						state["game_end_reason"] = 2
						state["stage"] = SHOOTER_STAGE_GAME_END
			else:
				next_eshots.append(eshot)
		state["enemy_shots"] = next_eshots

# Reloads primary weapon ammunition (confirm/cancel reload mechanic)
static func shooter_reload(state: Dictionary) -> Dictionary:
	var p: Dictionary = state["player"]
	p["ammo"] = int(p.get("max_ammo", 24))
	p["reloading"] = true
	p["reload_timer"] = 4
	p["reload_count"] = int(p.get("reload_count", 0)) + 1
	return state

# Fires secondary homing/spread missiles (hud_secondary_weapons.c)
static func shooter_fire_secondary(state: Dictionary) -> Dictionary:
	var p: Dictionary = state["player"]
	var aux: int = int(p.get("auxiliary", SHOOTER_SECONDARY_HOMING))
	var sec_dmg: int = 4 if aux == SHOOTER_SECONDARY_HOMING else 3
	for enemy: Dictionary in state.get("enemies", []):
		if int(enemy.get("hp", 0)) > 0:
			enemy["hp"] = maxi(0, int(enemy["hp"]) - sec_dmg)
			if int(enemy["hp"]) == 0:
				state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(enemy.get("score", 5000)))
				state["enemies_destroyed"] = int(state.get("enemies_destroyed", 0)) + 1
			break
	if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0 and int(state["boss"].get("hp", 0)) > 0:
		var boss: Dictionary = state["boss"]
		boss["hp"] = maxi(0, int(boss["hp"]) - sec_dmg)
		if int(boss["hp"]) == 0:
			boss["active"] = 0
			state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(boss.get("score", 100000)))
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
	# Apply AoE bomb damage to active enemies & clear enemy bullets
	var bomb_dmg: int = 12 if fired == SHOOTER_BOMB_BLAST else 8
	for enemy: Dictionary in state["enemies"]:
		if int(enemy["hp"]) > 0:
			enemy["hp"] = maxi(0, int(enemy["hp"]) - bomb_dmg)
			if int(enemy["hp"]) == 0:
				state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(enemy["score"]))
				state["enemies_destroyed"] = int(state.get("enemies_destroyed", 0)) + 1
	if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0 and int(state["boss"].get("hp", 0)) > 0:
		var boss: Dictionary = state["boss"]
		boss["hp"] = maxi(0, int(boss["hp"]) - bomb_dmg)
		if int(boss["hp"]) == 0:
			boss["active"] = 0
			state["score"] = mini(SHOOTER_MAX_SCORE, int(state["score"]) + int(boss.get("score", 100000)))
	if state.has("enemy_shots") and state["enemy_shots"] is Array:
		(state["enemy_shots"] as Array).clear()
	return fired

# Spawns a source-backed enemy wave for stage 11 (formation), 21 (swoop), or 31 (swarm)
static func shooter_spawn_wave(state: Dictionary, stage_id: int, wave_idx: int) -> void:
	state["stage"] = stage_id
	state["wave"] = wave_idx
	if state.has("boss") and state["boss"] is Dictionary:
		state["boss"]["active"] = 0
	var base_hp: int = 6 + (stage_id / 10) * 2
	var wave_offset: int = (wave_idx % 3) * 0x180
	state["enemies"] = [
		{
			"id": 16 + wave_idx * 4,
			"class_id": 4,
			"kind": "taco_fighter" if stage_id == SHOOTER_STAGE_STAGE1 else ("swoop_raider" if stage_id == SHOOTER_STAGE_STAGE2 else "swarm_interceptor"),
			"x": -0x480 + wave_offset,
			"y": -0x280,
			"z": 0x1400,
			"hp": base_hp,
			"max_hp": base_hp,
			"radius": 0x180,
			"score": 5000,
			"fire_cooldown": 10,
		},
		{
			"id": 17 + wave_idx * 4,
			"class_id": 4,
			"kind": "taco_fighter" if stage_id == SHOOTER_STAGE_STAGE1 else ("swoop_raider" if stage_id == SHOOTER_STAGE_STAGE2 else "swarm_interceptor"),
			"x": 0x480 - wave_offset,
			"y": -0x240,
			"z": 0x1600,
			"hp": base_hp,
			"max_hp": base_hp,
			"radius": 0x180,
			"score": 5000,
			"fire_cooldown": 14,
		},
		{
			"id": 18 + wave_idx * 4,
			"class_id": 5,
			"kind": "morph_core" if stage_id != SHOOTER_STAGE_STAGE3 else "guardian_ring",
			"x": 0,
			"y": -0x300,
			"z": 0x1C00,
			"hp": base_hp * 2,
			"max_hp": base_hp * 2,
			"radius": 0x280,
			"score": 12000 if stage_id != SHOOTER_STAGE_STAGE1 else 20000,
			"fire_cooldown": 16,
		},
	]

# Spawns the stage boss for stage 12 (Arm-Tail), 22 (Morph), or 32 (Collapse)
static func shooter_spawn_boss(state: Dictionary, boss_stage: int) -> void:
	if boss_stage == 0:
		boss_stage = SHOOTER_STAGE_BOSS1
	elif boss_stage == 1:
		boss_stage = SHOOTER_STAGE_BOSS2
	elif boss_stage == 2:
		boss_stage = SHOOTER_STAGE_BOSS3
	state["stage"] = boss_stage
	state["enemies"] = []
	var boss_hp: int = 36 if boss_stage == SHOOTER_STAGE_BOSS1 else (48 if boss_stage == SHOOTER_STAGE_BOSS2 else 60)
	var boss_score: int = 150000 if boss_stage == SHOOTER_STAGE_BOSS1 else (50000 if boss_stage == SHOOTER_STAGE_BOSS2 else 100000)
	var boss_kind: String = "arm_tail_boss" if boss_stage == SHOOTER_STAGE_BOSS1 else ("morph_boss" if boss_stage == SHOOTER_STAGE_BOSS2 else "collapse_boss")
	state["boss"] = {
		"active": 1,
		"kind": boss_kind,
		"class_id": 14 if boss_stage == SHOOTER_STAGE_BOSS1 else (15 if boss_stage == SHOOTER_STAGE_BOSS2 else 16),
		"x": 0,
		"y": -0x200,
		"z": 0x1600,
		"hp": boss_hp,
		"max_hp": boss_hp,
		"radius": 0x380,
		"score": boss_score,
		"phase": 0,
		"fire_cooldown": 8,
	}

# Complete interactive step machine for TACO (stage/wave progression, targeting, firing/reload, enemy shots, boss, clear/game-over)
static func shooter_step_interactive(state: Dictionary, input: Variant = 0, pad_held: int = 0) -> Dictionary:
	if not state.has("player"):
		var init_t: Dictionary = shooter_create_state(state)
		for k in init_t.keys():
			if not state.has(k):
				state[k] = init_t[k]

	var norm: Dictionary = _normalize_input(input, pad_held)
	var stg: int = int(state.get("stage", SHOOTER_STAGE_STAGE1))

	# Handle post-gameplay flow: GAME_END (45) -> RESULT (8) -> SCORES (5) -> EXIT (46)
	if stg == SHOOTER_STAGE_GAME_END:
		if bool(norm["confirm"]) or bool(norm["start"]):
			state["result_tally"] = shooter_compute_result_tally(state)
			state["stage"] = SHOOTER_STAGE_RESULT
			state["action"] = "enter_result"
		else:
			state["action"] = "game_end_wait"
		return state
	elif stg == SHOOTER_STAGE_RESULT:
		if not state.has("result_tally"):
			state["result_tally"] = shooter_compute_result_tally(state)
		if bool(norm["confirm"]) or bool(norm["start"]):
			var tally_total: int = int(state["result_tally"]["total_score"])
			state["score"] = tally_total
			var inserted: int = shooter_try_insert_ranking(state, tally_total, str(state.get("player_initials", "ＦＵＫ")))
			state["inserted_rank_slot"] = inserted
			state["stage"] = SHOOTER_STAGE_SCORES
			state["action"] = "enter_scores"
		else:
			state["action"] = "result_view"
		return state
	elif stg == SHOOTER_STAGE_SCORES or stg == SHOOTER_STAGE_NAME_ENTRY:
		if bool(norm["confirm"]) or bool(norm["cancel"]) or bool(norm["start"]):
			state["stage"] = SHOOTER_STAGE_EXIT
			state["completed"] = true
			state["return_payload"] = shooter_build_return_payload(state)
			state["action"] = "return_to_game"
		else:
			state["action"] = "scores_view"
		return state
	elif stg == SHOOTER_STAGE_EXIT:
		state["completed"] = true
		state["return_payload"] = shooter_build_return_payload(state)
		state["action"] = "return_to_game"
		return state

	# Active gameplay in stages 11/12/21/22/31/32
	if bool(norm["cancel"]) and not bool(norm["confirm"]):
		shooter_reload(state)
		shooter_fire_secondary(state)

	shooter_step_frame(
		state,
		int(norm["dx"]),
		int(norm["dy"]),
		bool(norm["roll"]),
		bool(norm["confirm"]),
		bool(norm["bomb"])
	)

	# Enemy projectile spawning during active combat
	if not bool(state.get("game_over", false)):
		for enemy: Dictionary in state.get("enemies", []):
			if int(enemy.get("hp", 0)) <= 0:
				continue
			enemy["fire_cooldown"] = int(enemy.get("fire_cooldown", 12)) - 1
			if int(enemy["fire_cooldown"]) <= 0:
				enemy["fire_cooldown"] = 14
				state["enemy_shots"].append({
					"x": int(enemy["x"]),
					"y": int(enemy["y"]),
					"z": int(enemy.get("z", 0x1600)),
					"vz": 0x280,
					"damage": 16,
				})
		if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0 and int(state["boss"].get("hp", 0)) > 0:
			var boss: Dictionary = state["boss"]
			boss["fire_cooldown"] = int(boss.get("fire_cooldown", 10)) - 1
			if int(boss["fire_cooldown"]) <= 0:
				boss["fire_cooldown"] = 10
				state["enemy_shots"].append({
					"x": int(state["player"]["x"]),
					"y": int(state["player"]["y"]),
					"z": int(boss.get("z", 0x1600)),
					"vz": 0x300,
					"damage": 24,
				})

	# Check wave / boss progression
	stg = int(state.get("stage", SHOOTER_STAGE_STAGE1))
	if stg in [SHOOTER_STAGE_STAGE1, SHOOTER_STAGE_STAGE2, SHOOTER_STAGE_STAGE3]:
		var any_alive := false
		for enemy: Dictionary in state.get("enemies", []):
			if int(enemy.get("hp", 0)) > 0:
				any_alive = true
				break
		if not any_alive:
			var cur_wave: int = int(state.get("wave", 0))
			var max_waves: int = maxi(1, int(state.get("waves_per_stage", 2)))
			if cur_wave + 1 < max_waves:
				shooter_spawn_wave(state, stg, cur_wave + 1)
				state["action"] = "wave_advance"
			else:
				var boss_stg: int = SHOOTER_STAGE_BOSS1 if stg == SHOOTER_STAGE_STAGE1 else (SHOOTER_STAGE_BOSS2 if stg == SHOOTER_STAGE_STAGE2 else SHOOTER_STAGE_BOSS3)
				shooter_spawn_boss(state, boss_stg)
				state["action"] = "boss_encounter"
				return state
	elif stg in [SHOOTER_STAGE_BOSS1, SHOOTER_STAGE_BOSS2, SHOOTER_STAGE_BOSS3]:
		var boss_hp: int = int(state.get("boss", {}).get("hp", 0))
		if boss_hp <= 0:
			state["stages_cleared"] = int(state.get("stages_cleared", 0)) + 1
			var target_stgs: int = maxi(1, int(state.get("target_stages", 3)))
			if int(state["stages_cleared"]) >= target_stgs or stg == SHOOTER_STAGE_BOSS3:
				state["stage_clear"] = true
				state["end_reason"] = "mission_complete"
				state["stage"] = SHOOTER_STAGE_GAME_END
				state["result_tally"] = shooter_compute_result_tally(state)
				state["action"] = "stage_clear"
			elif stg == SHOOTER_STAGE_BOSS1:
				shooter_spawn_wave(state, SHOOTER_STAGE_STAGE2, 0)
				state["action"] = "stage_advance"
			elif stg == SHOOTER_STAGE_BOSS2:
				shooter_spawn_wave(state, SHOOTER_STAGE_STAGE3, 0)
				state["action"] = "stage_advance"
			return state

	state["action"] = "game_over" if bool(state.get("game_over", false)) else "stepped"
	return state

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

# Builds the exact caller return payload matching save_and_exit.c (shooter_exit_step_save_settings & shooter_exit_step_return)
static func shooter_build_return_payload(state: Dictionary) -> Dictionary:
	var tally: Dictionary = state.get("result_tally", shooter_compute_result_tally(state))
	var rankings: Array = state.get("rankings", [])
	var top_scores: Array[int] = [
		int(rankings[0]["score"]) if rankings.size() > 0 else 2500000,
		int(rankings[1]["score"]) if rankings.size() > 1 else 2000000,
		int(rankings[2]["score"]) if rankings.size() > 2 else 1500000,
	]
	var top_names: Array[String] = [
		str(rankings[0]["name"]) if rankings.size() > 0 else "ＳＨＩ",
		str(rankings[1]["name"]) if rankings.size() > 1 else "ＹＵＩ",
		str(rankings[2]["name"]) if rankings.size() > 2 else "ＳＡＫ",
	]
	var from_story: bool = bool(state.get("entered_from_story", true))
	var tot_score: int = int(tally.get("total_score", state.get("score", 0)))
	return {
		"overlay": "TACO",
		"game_mode": GAME_MODE_SHOOTER_NATIVE,
		"menu_hit_3": 0x9F,
		"system_data_flag": 1,
		"high_scores": top_scores,
		"high_score_names": top_names,
		"stages_cleared": int(state.get("stages_cleared", 0)),
		"score": tot_score,
		"total_score": tot_score,
		"result_tally": tally,
		"stage_clear": bool(state.get("stage_clear", int(state.get("stages_cleared", 0)) >= 3)),
		"game_over": bool(state.get("game_over", false)),
		"end_reason": str(state.get("end_reason", "")),
		"return_game_mode": int(state.get("saved_game_mode", 0x48)) if from_story else 0x12,
		"return_mode_stage": int(state.get("saved_mode_stage", 0)),
		"return_mode_step": int(state.get("saved_mode_step", 0)),
	}


# ==============================================================================
# 3. TT: TwinBee Time Attack (src/ovl/TT/ & include/ovl/tt.h)
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
const TT_STATE_EXIT := 9

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
const TT_HITBOX_BOSS := {"x": 0, "y": 0, "half_w": 24, "half_h": 16}

static func tt_create_state(config: Dictionary = {}) -> Dictionary:
	var stg_idx: int = clampi(int(config.get("stage_index", 0)), 0, 3)
	var px: int = int(config.get("player_x", 0))
	var py: int = int(config.get("player_y", 192))
	return {
		"overlay": "TT",
		"game_mode": GAME_MODE_TT_NATIVE,
		"state": int(config.get("state", TT_STATE_STAGE)),
		"sub_state": int(config.get("sub_state", 0x40)),
		"stage_index": stg_idx, # 0..3 (Stages 1..4)
		"tt_stages_cleared": int(config.get("tt_stages_cleared", stg_idx)), # run[0xB]
		"target_stages": int(config.get("target_stages", 4)),
		"elapsed_frames": int(config.get("elapsed_frames", 4250)), # 1:10.83
		"lives": int(config.get("lives", 2)),
		"score": int(config.get("score", 0)),
		"bell_hit_counter": int(config.get("bell_hit_counter", 0)), # run[0xD]
		"scroll_speed": 0x18000, # step 3 of 6 on speed gauge
		"scroll_progress": int(config.get("scroll_progress", 0)),
		"player": {
			"active": 1,
			"state": 0x40,
			"x": px,       # [-120, 120] relative to center 160
			"y": py,       # [56, 224]
			"pos_x_fixed": px << 16,
			"pos_y_fixed": py << 16,
			"invulnerable": int(config.get("invulnerable", 0)),
			"has_options": int(config.get("has_options", 1)), # player[0x53]
			"speed_level": int(config.get("speed_level", 4)), # player[0x54]
			"shot_kind": int(config.get("shot_kind", 9)),     # player[0x55]: 8=Normal, 9=Twin, 10=3-Way
			"bombs": int(config.get("bombs", 2)),
			"left_arm_swing": 0,
			"right_arm_swing": 0,
		},
		"player_shots": [],
		"player_bombs": [],
		"bells": [
			{"active": 1, "state": 0x40, "x": -36, "y": 96, "kind": 1, "cycles": 0, "vy": -0x18000, "vx": 0}, # Blue Twin-Shot Bell
			{"active": 1, "state": 0x40, "x": 42, "y": 116, "kind": 0, "cycles": 0, "vy": 0x10000, "vx": 0},  # Yellow Bell
			{"active": 1, "state": 0x40, "x": 4, "y": 76, "kind": 3, "cycles": 0, "vy": -0x10000, "vx": 0},   # White Option Bell
		],
		"air_enemies": [
			{"active": 1, "state": 0x40, "kind": 0x40, "x": -52, "y": 64, "hp": 4, "max_hp": 4, "vy": 2},
			{"active": 1, "state": 0x40, "kind": 0x41, "x": 52, "y": 68, "hp": 4, "max_hp": 4, "vy": 2},
		],
		"ground_targets": [
			{"active": 1, "state": 0x40, "x": -32, "y": 104, "hp": 6, "drops_smart_bomb": true},
			{"active": 1, "state": 0x40, "x": 36, "y": 88, "hp": 6, "drops_smart_bomb": false},
		],
		"enemy_shots": [
			{"active": 1, "state": 0x40, "kind": 0xA0, "x": -24, "y": 132, "vx": 0, "vy": 4},
			{"active": 1, "state": 0x40, "kind": 0xA0, "x": 28, "y": 140, "vx": 0, "vy": 4},
		],
		"boss": {
			"active": 1,
			"boss_id": int(config.get("boss_id", stg_idx)), # 0: Rope Arm, 1: Swing Ring, 2: Multipart, 3: Beam/Ring
			"x": 0,
			"y": 54,
			"vx": 2,
			"core_hp": int(config.get("boss_hp", 64)),
			"max_core_hp": int(config.get("boss_hp", 64)),
			"side_hp": [24, 24, 24, 24],
			"fire_cooldown": 16,
		},
		# Default rankings from tt_config_load (engine_initialization.c:66-80)
		"rankings": [
			{"time": 0x63C8, "name_codes": [0x14, 0x24, 0x18], "name": "Ｋ．Ｏ"}, # 7:05.73
			{"time": 0x71D8, "name_codes": [0x17, 0x24, 0x0A], "name": "Ｎ．Ａ"}, # 8:05.73
			{"time": 0x7FE8, "name_codes": [0x16, 0x24, 0x12], "name": "Ｍ．Ｉ"}, # 9:05.73
		],
		"ranking_slot": int(config.get("ranking_slot", -1)),
		"stage_clear": false,
		"victory": false,
		"game_over": false,
		"completed": false,
		"sys_exit_flag": 0,
		"options_dirty": int(config.get("options_dirty", 0)),
		"saved_game_mode": int(config.get("saved_game_mode", 0x48)),
		"saved_mode_stage": int(config.get("saved_mode_stage", 0)),
		"saved_mode_step": int(config.get("saved_mode_step", 0)),
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
	if bell.has("hits") and int(bell["hits"]) > int(state.get("bell_hit_counter", 0)):
		state["bell_hit_counter"] = int(bell["hits"])
	state["bell_hit_counter"] = int(state["bell_hit_counter"]) + 1
	var hit_cnt: int = int(state["bell_hit_counter"])
	bell["hits"] = hit_cnt
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
		state["score"] = int(state.get("score", 0)) + (1000 if kind != 0 else 500)
		match kind:
			0:
				bell["active"] = 0
			1: # Twin Shot
				p["shot_kind"] = 9
				bell["state"] = 0x90
				bell["popup_label"] = 40
			2: # Speed Up
				p["speed_level"] = mini(10, int(p["speed_level"]) + 1)
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
			enemy["hp"] = 0
			state["score"] = int(state.get("score", 0)) + 200
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

# Spawns player main/twin/3-way shots and option follower shots (title_and_projectile_pools.c)
static func tt_fire_player_shot(state: Dictionary) -> Array:
	var p: Dictionary = state["player"]
	var px: int = int(p.get("x", 0))
	var py: int = int(p.get("y", 192))
	var skind: int = int(p.get("shot_kind", 9))
	if not state.has("player_shots") or not (state["player_shots"] is Array):
		state["player_shots"] = []
	var spawned: Array = []
	if skind == 8: # Normal single shot
		var s := {"active": 1, "x": px, "y": py - 8, "vx": 0, "vy": -12, "damage": 4, "shot_kind": 8}
		state["player_shots"].append(s)
		spawned.append(s)
	elif skind == 10: # 3-Way Max Shot
		for vx_val: int in [-4, 0, 4]:
			var s := {"active": 1, "x": px + vx_val, "y": py - 8, "vx": vx_val, "vy": -12, "damage": 6, "shot_kind": 10}
			state["player_shots"].append(s)
			spawned.append(s)
	else: # 9: Twin Shot
		for off_x: int in [-6, 6]:
			var s := {"active": 1, "x": px + off_x, "y": py - 8, "vx": 0, "vy": -12, "damage": 5, "shot_kind": 9}
			state["player_shots"].append(s)
			spawned.append(s)

	if int(p.get("has_options", 0)) != 0:
		for opt_dx: int in [-20, 20]:
			var os := {"active": 1, "x": clampi(px + opt_dx, -120, 120), "y": py, "vx": 0, "vy": -12, "damage": 3, "shot_kind": 8, "from_option": true}
			state["player_shots"].append(os)
			spawned.append(os)
	return spawned

# Launches a ground bomb from TwinBee's arm and damages ground targets / boss (bomb_pool.c)
static func tt_fire_ground_bomb(state: Dictionary) -> Dictionary:
	var p: Dictionary = state["player"]
	p["right_arm_swing"] = 8
	if not state.has("player_bombs") or not (state["player_bombs"] is Array):
		state["player_bombs"] = []
	var bomb := {"active": 1, "x": int(p.get("x", 0)), "y": int(p.get("y", 192)) - 24, "vy": -8, "damage": 16}
	state["player_bombs"].append(bomb)
	tt_trigger_smart_bomb(state)
	for gt: Dictionary in state.get("ground_targets", []):
		if int(gt.get("active", 0)) != 0 and int(gt.get("hp", 0)) > 0:
			gt["hp"] = maxi(0, int(gt["hp"]) - 16)
			if int(gt["hp"]) == 0:
				gt["active"] = 0
				state["score"] = int(state.get("score", 0)) + 500
	if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0:
		var boss: Dictionary = state["boss"]
		boss["core_hp"] = maxi(0, int(boss.get("core_hp", 0)) - 16)
		state["score"] = int(state.get("score", 0)) + 400
	return bomb

# Resets/spawns entities for a new TwinBee stage (0..3 -> Stages 1..4)
static func tt_spawn_stage_entities(state: Dictionary, stage_idx: int) -> void:
	var clamped_stg: int = clampi(stage_idx, 0, 3)
	state["stage_index"] = clamped_stg
	state["state"] = TT_STATE_STAGE
	state["sub_state"] = 0x40
	state["player_shots"] = []
	state["player_bombs"] = []
	state["air_enemies"] = [
		{"active": 1, "state": 0x40, "kind": 0x40 + clamped_stg, "x": -48, "y": 64, "hp": 4 + clamped_stg * 2, "max_hp": 4 + clamped_stg * 2, "vy": 2},
		{"active": 1, "state": 0x40, "kind": 0x41 + clamped_stg, "x": 48, "y": 68, "hp": 4 + clamped_stg * 2, "max_hp": 4 + clamped_stg * 2, "vy": 2},
	]
	state["bells"] = [
		{"active": 1, "state": 0x40, "x": -36, "y": 96, "kind": 1, "cycles": 0, "vy": -0x18000, "vx": 0},
		{"active": 1, "state": 0x40, "x": 0, "y": 116, "kind": 0, "cycles": 0, "vy": 0x10000, "vx": 0},
		{"active": 1, "state": 0x40, "x": 36, "y": 80, "kind": 3, "cycles": 0, "vy": -0x10000, "vx": 0},
	]
	state["enemy_shots"] = []
	var boss_hp: int = 32 + clamped_stg * 16
	state["boss"] = {
		"active": 1,
		"boss_id": clamped_stg,
		"x": 0,
		"y": 54,
		"vx": 2,
		"core_hp": boss_hp,
		"max_core_hp": boss_hp,
		"side_hp": [16, 16, 16, 16],
		"fire_cooldown": 14,
	}

# Complete interactive step machine for TT (movement, shots, bell cycling/pickup, bombs, waves/bosses, lives, tt_stages_cleared writeback)
static func tt_step_interactive(state: Dictionary, input: Variant = 0, pad_held: int = 0) -> Dictionary:
	if not state.has("player"):
		var init_tt: Dictionary = tt_create_state(state)
		for k in init_tt.keys():
			if not state.has(k):
				state[k] = init_tt[k]

	var norm: Dictionary = _normalize_input(input, pad_held)
	var tt_st: int = int(state.get("state", TT_STATE_STAGE))

	# Post-stage / post-game states
	if tt_st == TT_STATE_STAGE_CLEAR:
		var next_stg: int = int(state.get("tt_stages_cleared", int(state.get("stage_index", 0)) + 1))
		var target_stgs: int = maxi(1, int(state.get("target_stages", 4)))
		if next_stg < target_stgs:
			tt_spawn_stage_entities(state, next_stg)
			state["action"] = "next_stage"
		else:
			state["stage_clear"] = true
			state["victory"] = true
			tt_try_insert_ranking(state, int(state.get("elapsed_frames", 4250)))
			state["state"] = TT_STATE_RANKING_VIEW
			state["action"] = "enter_rankings"
		return state
	elif tt_st == TT_STATE_GAME_OVER:
		if bool(norm["confirm"]) or bool(norm["cancel"]) or bool(norm["start"]):
			state["state"] = TT_STATE_RANKING_VIEW
			state["action"] = "enter_rankings"
		else:
			state["action"] = "game_over_wait"
		return state
	elif tt_st == TT_STATE_RANKING_VIEW or tt_st == TT_STATE_NAME_ENTRY or tt_st == TT_STATE_VICTORY:
		if bool(norm["confirm"]) or bool(norm["cancel"]) or bool(norm["start"]):
			state["state"] = TT_STATE_EXIT
			state["sys_exit_flag"] = 1
			state["completed"] = true
			state["return_payload"] = tt_build_return_payload(state)
			state["action"] = "return_to_game"
		else:
			state["action"] = "ranking_view"
		return state
	elif tt_st == TT_STATE_EXIT:
		state["sys_exit_flag"] = 1
		state["completed"] = true
		state["return_payload"] = tt_build_return_payload(state)
		state["action"] = "return_to_game"
		return state

	# Active TT_STATE_STAGE (2) gameplay
	var p: Dictionary = state["player"]
	var step_px: int = 4 + clampi(int(p.get("speed_level", 4)), 1, 10)
	if int(norm["dx"]) != 0:
		p["x"] = clampi(int(p.get("x", 0)) + int(norm["dx"]) * step_px, -120, 120)
		p["pos_x_fixed"] = int(p["x"]) << 16
	if int(norm["dy"]) != 0:
		p["y"] = clampi(int(p.get("y", 192)) + int(norm["dy"]) * step_px, 56, 224)
		p["pos_y_fixed"] = int(p["y"]) << 16

	if int(p.get("invulnerable", 0)) > 0:
		p["invulnerable"] = int(p["invulnerable"]) - 1
		if int(p["invulnerable"]) == 0:
			p["state"] = 0x40

	if bool(norm["confirm"]):
		tt_fire_player_shot(state)
	if bool(norm["bomb"]) or (bool(norm["cancel"]) and not bool(norm["confirm"])):
		tt_fire_ground_bomb(state)

	# Advance player shots and resolve collisions with bells, air enemies, and boss
	var remaining_pshots: Array = []
	for pshot: Dictionary in state.get("player_shots", []):
		pshot["x"] = int(pshot.get("x", 0)) + int(pshot.get("vx", 0))
		pshot["y"] = int(pshot.get("y", 180)) + int(pshot.get("vy", -12))
		var s_pos := Vector2i(int(pshot["x"]), int(pshot["y"]))
		var s_dmg: int = int(pshot.get("damage", 4))
		var hit := false

		# Check bell hits (cycling_pickups.c::tt_bell_hit_by_shot)
		var bells: Array = state.get("bells", [])
		for bi in range(bells.size()):
			var bell: Dictionary = bells[bi]
			if int(bell.get("active", 0)) == 0 or int(bell.get("state", 0x40)) == 0x90:
				continue
			var b_pos := Vector2i(int(bell["x"]), int(bell["y"]))
			if tt_box_overlap(s_pos, TT_HITBOX_SHOT, b_pos, TT_HITBOX_PICKUP):
				tt_bell_hit_by_shot(state, bi, s_pos.x - b_pos.x)
				state["last_bell_hit"] = bell
				hit = true
				break

		# Check air enemy hits
		if not hit:
			for enemy: Dictionary in state.get("air_enemies", []):
				if int(enemy.get("active", 0)) == 0 or int(enemy.get("state", 0x40)) != 0x40:
					continue
				var e_pos := Vector2i(int(enemy["x"]), int(enemy["y"]))
				if tt_box_overlap(s_pos, TT_HITBOX_SHOT, e_pos, TT_HITBOX_AIR_ENEMY):
					var e_hp: int = maxi(0, int(enemy.get("hp", 4)) - s_dmg)
					enemy["hp"] = e_hp
					if e_hp == 0:
						enemy["state"] = 0x70
						state["score"] = int(state.get("score", 0)) + 200
					hit = true
					break

		# Check boss hits (boss_parts_and_hits.c)
		if not hit and state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0:
			var boss: Dictionary = state["boss"]
			var boss_pos := Vector2i(int(boss.get("x", 0)), int(boss.get("y", 54)))
			if tt_box_overlap(s_pos, TT_HITBOX_SHOT, boss_pos, TT_HITBOX_BOSS):
				var cur_bhp: int = int(boss.get("hp", boss.get("core_hp", 0)))
				var next_bhp: int = maxi(0, cur_bhp - s_dmg)
				boss["core_hp"] = next_bhp
				boss["hp"] = next_bhp
				state["score"] = int(state.get("score", 0)) + 100
				hit = true

		if not hit and int(pshot["y"]) > 8:
			remaining_pshots.append(pshot)
	state["player_shots"] = remaining_pshots

	# Update bells movement & check player AABB overlap for bell collection (tt_bell_collect)
	var p_pos := Vector2i(int(p.get("x", 0)), int(p.get("y", 192)))
	var bells_list: Array = state.get("bells", [])
	for bi in range(bells_list.size()):
		var bell: Dictionary = bells_list[bi]
		if int(bell.get("active", 0)) == 0:
			continue
		if int(bell.get("state", 0x40)) == 0x90:
			bell["active"] = 0
			continue
		var b_pos_pre := Vector2i(int(bell["x"]), int(bell["y"]))
		if tt_box_overlap(p_pos, TT_HITBOX_PLAYER, b_pos_pre, TT_HITBOX_PICKUP):
			state["last_bell_collected"] = tt_bell_collect(state, bi)
			continue
		var vy_fixed: int = int(bell.get("vy", 0x10000))
		bell["y"] = clampi(int(bell.get("y", 96)) + (2 if vy_fixed > 0 else -2), 24, 228)
		if vy_fixed < 0:
			bell["vy"] = vy_fixed + 0x8000
			if int(bell["vy"]) >= 0:
				bell["vy"] = 0x10000
		var b_pos := Vector2i(int(bell["x"]), int(bell["y"]))
		if tt_box_overlap(p_pos, TT_HITBOX_PLAYER, b_pos, TT_HITBOX_PICKUP):
			state["last_bell_collected"] = tt_bell_collect(state, bi)

	# Update enemy shots & check player collision / lives / Game Over
	var next_eshots: Array = []
	for eshot: Dictionary in state.get("enemy_shots", []):
		if int(eshot.get("active", 0)) == 0:
			continue
		eshot["x"] = int(eshot.get("x", 0)) + int(eshot.get("vx", 0))
		eshot["y"] = int(eshot.get("y", 132)) + int(eshot.get("vy", 4))
		var es_pos := Vector2i(int(eshot["x"]), int(eshot["y"]))
		if tt_box_overlap(p_pos, TT_HITBOX_PLAYER, es_pos, TT_HITBOX_SHOT):
			eshot["active"] = 0
			_tt_apply_player_hit(state)
		elif int(eshot["y"]) < 236:
			next_eshots.append(eshot)
	state["enemy_shots"] = next_eshots

	# Check boss defeat -> stage clear & tt_stages_cleared writeback
	if state.has("boss") and state["boss"] is Dictionary and int(state["boss"].get("active", 0)) != 0:
		var boss: Dictionary = state["boss"]
		var cur_boss_hp: int = int(boss.get("hp", boss.get("core_hp", 0)))
		if cur_boss_hp <= 0:
			boss["active"] = 0
			boss["core_hp"] = 0
			boss["hp"] = 0
			var cleared: int = mini(4, int(state.get("stage_index", 0)) + 1)
			state["tt_stages_cleared"] = cleared
			state["score"] = int(state.get("score", 0)) + 5000
			var target_stgs: int = maxi(1, int(state.get("target_stages", 4)))
			if cleared >= target_stgs:
				state["stage_clear"] = true
				state["victory"] = true
			state["state"] = TT_STATE_STAGE_CLEAR
			state["return_payload"] = tt_build_return_payload(state)
			state["action"] = "stage_cleared"
			return state
		else:
			boss["fire_cooldown"] = int(boss.get("fire_cooldown", 14)) - 1
			if int(boss["fire_cooldown"]) <= 0:
				boss["fire_cooldown"] = 14
				state["enemy_shots"].append({
					"active": 1,
					"state": 0x40,
					"kind": 0xA0,
					"x": int(boss.get("x", 0)),
					"y": int(boss.get("y", 54)) + 12,
					"vx": 0,
					"vy": 6,
				})

	if int(state.get("elapsed_frames", 0)) < TT_MAX_TIME_FRAMES:
		state["elapsed_frames"] = int(state.get("elapsed_frames", 0)) + 1
	state["scroll_progress"] = int(state.get("scroll_progress", 0)) + 1
	state["action"] = "game_over" if bool(state.get("game_over", false)) else "stepped"
	return state

static func tt_step_frame(state: Dictionary, input: Variant = 0, pad_held: int = 0) -> Dictionary:
	return tt_step_interactive(state, input, pad_held)

static func _tt_apply_player_hit(state: Dictionary) -> void:
	var p: Dictionary = state["player"]
	if int(p.get("invulnerable", 0)) > 0 or int(p.get("invincible_timer", 0)) > 0:
		return
	p["has_options"] = 0
	p["shot_kind"] = 8
	var cur_lives: int = int(state.get("lives", 2))
	if cur_lives > 0:
		state["lives"] = cur_lives - 1
		p["invulnerable"] = 30
		p["state"] = 0x60
	else:
		state["lives"] = 0
		p["active"] = 0
		p["state"] = 0x80
		state["game_over"] = true
		state["state"] = TT_STATE_GAME_OVER
		state["tt_stages_cleared"] = int(state.get("tt_stages_cleared", state.get("stage_index", 0)))
		state["return_payload"] = tt_build_return_payload(state)

# Builds the caller return payload matching tt_main tail (frame_loop_and_pause.c:82-111)
# and game_mode_tt_game_mode_handler (overlay_loader.c:584-595)
static func tt_build_return_payload(state: Dictionary) -> Dictionary:
	var stages_cleared: int = clampi(int(state.get("tt_stages_cleared", state.get("stage_index", 0))), 0, 4)
	var elapsed: int = clampi(int(state.get("elapsed_frames", 4250)), 0, TT_MAX_TIME_FRAMES)
	var opt_dirty: int = int(state.get("options_dirty", 0))
	return {
		"overlay": "TT",
		"game_mode": GAME_MODE_TT_NATIVE,
		"tt_stages_cleared": stages_cleared,
		"elapsed_frames": elapsed,
		"formatted_time": tt_format_time(elapsed)["text"],
		"score": int(state.get("score", 0)),
		"lives_remaining": int(state.get("lives", 0)),
		"rankings": (state.get("rankings", []) as Array).duplicate(true),
		"ranking_slot": int(state.get("ranking_slot", -1)),
		"stage_clear": bool(state.get("stage_clear", stages_cleared >= 4)),
		"victory": bool(state.get("victory", stages_cleared >= 4)),
		"game_over": bool(state.get("game_over", false)),
		"sys_exit_flag": int(state.get("sys_exit_flag", 1)),
		"return_game_mode": int(state.get("saved_game_mode", 0x48)) if opt_dirty == 0 else 0x00,
		"return_mode_stage": int(state.get("saved_mode_stage", 0)),
		"return_mode_step": int(state.get("saved_mode_step", 0)),
	}

# ==============================================================================
# 4. Subgame Overlay Entry / Step / Return Router
# ==============================================================================

static func enter_overlay(subgame_id: Variant, config: Dictionary = {}) -> Dictionary:
	var kind: String = _resolve_subgame_kind(subgame_id)
	match kind:
		"taco":
			return shooter_create_state(config)
		"tt":
			return tt_create_state(config)
		"rpg_bat":
			return rpg_create_state(config)
		_:
			return {"error": "Unsupported subgame overlay: " + str(subgame_id)}

static func step_overlay(arg0: Variant, arg1: Variant = 0, arg2: Variant = 0, arg3: int = 0) -> Dictionary:
	var subgame_id: Variant = arg0
	var state: Dictionary = {}
	var input: Variant = 0
	var pad_held: int = 0
	if arg0 is Dictionary:
		state = arg0
		subgame_id = state.get("overlay", state.get("subgame_id", "rpg_bat"))
		input = arg1
		pad_held = int(arg2) if arg2 is int else 0
	else:
		state = arg1 if arg1 is Dictionary else {}
		input = arg2
		pad_held = arg3
	var kind: String = _resolve_subgame_kind(subgame_id)
	match kind:
		"taco":
			return shooter_step_interactive(state, input, pad_held)
		"tt":
			return tt_step_interactive(state, input, pad_held)
		"rpg_bat":
			return rpg_step_interactive(state, input, int(state.get("rand_seed", 512)))
		_:
			return {"error": "Unsupported subgame overlay: " + str(subgame_id)}

static func build_return_payload(arg0: Variant, arg1: Variant = null) -> Dictionary:
	var subgame_id: Variant = arg0
	var state: Dictionary = {}
	if arg0 is Dictionary:
		state = arg0
		subgame_id = state.get("overlay", state.get("subgame_id", "rpg_bat"))
	elif arg1 is Dictionary:
		state = arg1
	var kind: String = _resolve_subgame_kind(subgame_id)
	match kind:
		"taco":
			return shooter_build_return_payload(state)
		"tt":
			return tt_build_return_payload(state)
		"rpg_bat":
			return rpg_build_return_payload(state)
		_:
			return {"error": "Unsupported subgame overlay: " + str(subgame_id)}

static func _resolve_subgame_kind(subgame_id: Variant) -> String:
	if subgame_id is int:
		match int(subgame_id):
			0x59, 0x90:
				return "taco"
			0x5A, 0xD0, 0xD1:
				return "tt"
			0x57, 0x81:
				return "rpg_bat"
	var s: String = str(subgame_id).strip_edges().to_lower()
	if s in ["taco", "shooter", "shooter_3d", "0x59", "0x90"]:
		return "taco"
	if s in ["tt", "twinbee", "twinbee_time", "0x5a", "0xd0"]:
		return "tt"
	if s in ["rpg_bat", "rpg", "rpg_battle", "0x57", "0x81"]:
		return "rpg_bat"
	return s

static func _normalize_input(input: Variant, pad_held: int = 0) -> Dictionary:
	var pressed: int = 0
	var held: int = pad_held
	var dx: int = 0
	var dy: int = 0
	var up := false
	var down := false
	var left := false
	var right := false
	var confirm := false
	var cancel := false
	var bomb := false
	var roll := false
	var start := false
	var cmd_str := ""

	if input is int:
		pressed = int(input)
		held = pad_held if pad_held != 0 else pressed
	elif input is String:
		var token: String = String(input).strip_edges().to_lower()
		match token:
			"up":
				pressed |= PAD_UP
				up = true
				dy = -1
			"down":
				pressed |= PAD_DOWN
				down = true
				dy = 1
			"left":
				pressed |= PAD_LEFT
				left = true
				dx = -1
			"right":
				pressed |= PAD_RIGHT
				right = true
				dx = 1
			"confirm", "fire", "shoot", "circle":
				pressed |= PAD_CIRCLE
				confirm = true
			"cancel", "reload", "cross":
				pressed |= PAD_CROSS
				cancel = true
			"bomb", "square":
				pressed |= PAD_SQUARE
				bomb = true
			"roll", "triangle":
				pressed |= PAD_TRIANGLE
				roll = true
			"start", "pause":
				pressed |= PAD_START
				start = true
			"attack", "defend", "guard", "magic", "special", "item", "run":
				cmd_str = token
	elif input is Dictionary:
		var d: Dictionary = input
		pressed = int(d.get("pressed", d.get("pad_pressed", 0)))
		held = int(d.get("held", d.get("pad_held", pressed)))
		dx = int(d.get("dx", d.get("move_dx", 0)))
		dy = int(d.get("dy", d.get("move_dy", 0)))
		up = bool(d.get("up", false))
		down = bool(d.get("down", false))
		left = bool(d.get("left", false))
		right = bool(d.get("right", false))
		confirm = bool(d.get("confirm", d.get("fire", d.get("fire_primary", false))))
		cancel = bool(d.get("cancel", d.get("reload", false)))
		bomb = bool(d.get("bomb", d.get("fire_bomb", false)))
		roll = bool(d.get("roll", d.get("trigger_roll", false)))
		start = bool(d.get("start", false))
		cmd_str = str(d.get("command", ""))

	var mask: int = pressed | held
	if (mask & PAD_LEFT) != 0:
		left = true
		dx = -1
	elif (mask & PAD_RIGHT) != 0:
		right = true
		dx = 1
	if (mask & PAD_UP) != 0:
		up = true
		dy = -1
	elif (mask & PAD_DOWN) != 0:
		down = true
		dy = 1
	if (pressed & PAD_CIRCLE) != 0:
		confirm = true
	if (pressed & PAD_CROSS) != 0:
		cancel = true
	if (pressed & PAD_SQUARE) != 0:
		bomb = true
	if (pressed & (PAD_TRIANGLE | PAD_L1 | PAD_R1)) != 0:
		roll = true
	if (pressed & PAD_START) != 0:
		start = true

	if left and dx == 0:
		dx = -1
	elif right and dx == 0:
		dx = 1
	if up and dy == 0:
		dy = -1
	elif down and dy == 0:
		dy = 1

	return {
		"pressed": pressed,
		"held": held,
		"dx": dx,
		"dy": dy,
		"up": up,
		"down": down,
		"left": left,
		"right": right,
		"confirm": confirm,
		"cancel": cancel,
		"bomb": bomb,
		"roll": roll,
		"start": start,
		"command": cmd_str,
	}
