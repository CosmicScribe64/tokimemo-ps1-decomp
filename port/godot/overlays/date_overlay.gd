class_name DateOverlay
extends RefCounted
## Playable DATE (0x50), DATE2 (0x58), and DATE3 (0x5E) overlay state machine and rules.
## Mirrors src/ovl/date/entry.c, src/ovl/date/place_scenes.c, src/ovl/date/girl_birthday.c,
## src/ovl/date/player_birthday.c, src/ovl/date/rides.c, src/ovl/amusement_park_date/,
## and src/ovl/date_spots/place_scene_entry.c.

const GAME_MODE_DATE := 0x50
const GAME_MODE_BATTLE_EVENT := 0x57
const GAME_MODE_AMUSEMENT_PARK_2 := 0x58
const GAME_MODE_DATE_SCENE := 0x5E

const DATE_STAGE_ARRIVAL := 0
const DATE_STAGE_PLACE := 1
const DATE_STAGE_GIRL_BIRTHDAY := 2
const DATE_STAGE_PLAYER_BIRTHDAY := 3

const DATE2_STAGE_SNOW_MOUNTAIN := 0
const DATE2_STAGE_PLAYER_BIRTHDAY := 1
const DATE2_STAGE_GIRL_BIRTHDAY := 2
const DATE2_STAGE_KARAOKE := 3
const DATE2_STAGE_POST_BATTLE_RETURN := 4

const DATE_ARRIVAL_WAIT_GREETING := 0
const DATE_ARRIVAL_DIRECT_GREETING := 1
const DATE_ARRIVAL_CORRECTED_PLACE := 2

const DATE_MEETING_WRONG_TWO_CHOICE := 0
const DATE_MEETING_STAY_HERE_TWO := 1
const DATE_MEETING_MOVE_FIRST_TWO := 2
const DATE_MEETING_WRONG_THREE_CHOICE := 3
const DATE_MEETING_STAY_HERE_THREE := 4
const DATE_MEETING_MOVE_FIRST_THREE := 5
const DATE_MEETING_MOVE_SECOND_THREE := 6
const DATE_MEETING_NOT_HERE := 7
const DATE_MEETING_STOOD_UP := 8

const GIRL_YUKO := 0
const GIRL_SHIORI := 1
const GIRL_MIO := 2
const GIRL_YUINA := 3
const GIRL_AYAKO := 4
const GIRL_SAKI := 5
const GIRL_NOZOMI := 6
const GIRL_MIRA := 7
const GIRL_YUKARI := 8
const GIRL_MEGUMI := 9
const GIRL_YUMI := 10
const GIRL_MIHARU := 11

const SPOT_NEIGHBORHOOD_PARK := 0
const SPOT_CENTRAL_PARK := 1
const SPOT_SHOPPING_STREET := 2
const SPOT_AQUARIUM := 3
const SPOT_ZOO := 4
const SPOT_BOTANICAL_GARDEN := 5
const SPOT_PLANETARIUM := 6
const SPOT_ART_MUSEUM := 7
const SPOT_LIBRARY := 8
const SPOT_GAME_CENTER := 9
const SPOT_BOWLING_ALLEY := 10
const SPOT_KARAOKE := 11
const SPOT_AMUSEMENT_PARK := 12
const SPOT_STADIUM := 13
const SPOT_MOVIE_THEATER := 14
const SPOT_CONCERT_HALL := 15
const SPOT_POOL := 16
const SPOT_SEA := 17
const SPOT_SHRINE := 18
const SPOT_SKATING_RINK := 19
const SPOT_SKI_RESORT := 20

const RIDE_ROLLER_COASTER := 0
const RIDE_GHOST_HOUSE := 1
const RIDE_FERRIS_WHEEL := 2
const RIDE_SCREAM_MACHINE := 3
const RIDE_VIRTUAL_SHIP := 4

## 3-choice menu geometry from normal_date_three_select_init (src/main/scene_library.c:2212-2253)
## x = -0x80 (-128 -> screen x = 32), y = [0x30, 0x40, 0x50] (48, 64, 80 -> screen y = 168, 184, 200),
## w = 0xFC (252), h = 0x10 (16).
const THREE_CHOICE_MENU_X := 32
const THREE_CHOICE_MENU_W := 252
const THREE_CHOICE_MENU_H := 16
const THREE_CHOICE_MENU_Y := [168, 184, 200]

## 2x2 Karaoke song menu geometry from amusement_karaoke_return_menu_build
## (src/ovl/amusement_park_date/karaoke_scene_and_return.c:296-299)
## x = [-0x80, 0, -0x80, -0x20] -> screen x = [32, 160, 32, 128]
## y = [0x34, 0x34, 0x4C, 0x4C] -> screen y = [172, 172, 196, 196]
const KARAOKE_MENU_X := [32, 160, 32, 128]
const KARAOKE_MENU_Y := [172, 172, 196, 196]
const KARAOKE_MENU_H := 16

## Exact 21 date place names from date_place_names (asm/ovl/DATE/data/date/place_scenes.data.s:910-1093)
const DATE_PLACE_NAMES_HEX := [
	"8bdf8f8a82cc8cf68980",             # 0: 近所の公園
	"82ab82e782df82ab9286899b8cf68980", # 1: きらめき中央公園
	"83568387836283738393834f8a58",     # 2: ショッピング街
	"908591b08ad9",                     # 3: 水族館
	"93ae95a88980",                     # 4: 動物園
	"904195a88980",                     # 5: 植物園
	"83768389836c835e838a83458380",     # 6: プラネタリウム
	"94fc8f708ad9",                     # 7: 美術館
	"907d8f918ad9",                     # 8: 図書館
	"8351815b8380835a8393835e815b",     # 9: ゲームセンター
	"837b815b838a8393834f8fea",         # 10: ボーリング場
	"834a838983498350",                 # 11: カラオケ
	"97568980926e",                     # 12: 遊園地
	"8358835e835783418380",             # 13: スタジアム
	"896689e68ad9",                     # 14: 映画館
	"835283938354815b836789ef8fea",     # 15: コンサート会場
	"8376815b838b",                     # 16: プール
	"8a43",                             # 17: 海
	"905f8ed0",                         # 18: 神社
	"83588350815b83678fea",             # 19: スケート場
	"8358834c815b8fea",                 # 20: スキー場
]

## Sub-place title banners from asm/ovl/DATE/data/date/place_scenes.rodata.s
const SUB_PLACE_NAMES_HEX := {
	"avenue": "95c096d893b9",                         # 並木道
	"cherry_avenue": "8df795c096d8",                  # 桜並木
	"pond": "9272",                                   # 池
	"dolphin_show": "8343838b834a83568387815b",       # イルカショー
	"indoor_hall": "8ad993e0",                        # 館内
	"park_inside": "898093e0",                        # 園内
	"gargy_exhibit": "834b815b834d815b9357",          # ガーギー展
	"sculpture_exhibit": "92a491599357",              # 彫塑展
	"painters_exhibit": "974c96bc89e689c69357",       # 有名画家展
	"library_inside": "907d8f918ad993e0",             # 図書館内
	"video_game_corner": "8372836683498351815b83808352815b8369815b", # ビデオゲームコーナー
	"medal_game_corner": "8381835f838b8351815b83808352815b8369815b", # メダルゲームコーナー
	"bowling_alley": "837b815b838a8393834f8fea",      # ボーリング場
	"karaoke_box": "8261826e827793e0",                # ＢＯＸ内
	"boutique": "8375836583428362834e",               # ブティック
	"fancy_shop": "8374834083938356815b8356838783628376", # ファンシーショップ
	"junk_shop": "835783838393834e89ae",              # ジャンク屋
	"hero_show": "8371815b838d815b83568387815b",      # ヒーローショー
	"night_parade": "8369834383678370838c815b8368",   # ナイトパレード
	"amusement_inside": "97568980926e93e0",           # 遊園地内
	"roller_coaster": "83578346836283678352815b8358835e815b", # ジェットコースター
	"scream_machine": "90e28ba9837d8356815b839383728372815b838b", # 絶叫マシーンビビール
	"ghost_house": "8353815b83588367836e83458358",    # ゴーストハウス
	"virtual_ship": "836f815b83608383838b835683628376", # バーチャルシップ
	"ferris_wheel": "8acf97978ed4",                   # 観覧車
	"stadium_inside": "8358835e83578341838093e0",     # スタジアム内
	"concert_inside": "89ef8fea93e0",                 # 会場内
	"pool_inside": "8376815b838b93e0",                # プール内
	"coast": "8a438add",                              # 海岸
	"beach": "956c95d3",                              # 浜辺
	"deserted_island": "96b3906c9387",                # 無人島
	"festival": "898f93fa",                           # 縁日
	"fireworks": "89d489ce91e589ef",                  # 花火大会
	"skating_rink": "83588350815b8367838a8393834e",   # スケートリンク
	"ski_slope": "8351838c83938366",                  # ゲレンデ
}

## Exact 11 x 21 date_place_affinity table (asm/ovl/DATE/data/date/place_scenes.data.s:737-796)
## 0 = Favorite (mult 5 pos / 3 neg), 1..2 = Liked (mult 4 pos / 4 neg),
## 3 = Neutral (mult 3 pos / 5 neg), >=4 = Disliked (mult 2 pos / 6 neg).
const DATE_PLACE_AFFINITY := [
	[0, 1, 1, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 3, 1, 1, 1, 0, 0, 2, 0], # 0: Yuko
	[2, 0, 1, 1, 1, 1, 1, 1, 0, 4, 3, 3, 2, 1, 0, 2, 3, 3, 0, 2, 0], # 1: Shiori
	[3, 2, 3, 2, 1, 1, 1, 2, 1, 2, 2, 3, 3, 4, 1, 2, 2, 2, 2, 3, 0], # 2: Mio
	[2, 1, 2, 2, 1, 2, 1, 0, 3, 2, 1, 0, 2, 2, 2, 1, 4, 4, 1, 1, 0], # 3: Yuina
	[1, 1, 2, 1, 3, 3, 1, 3, 2, 2, 1, 3, 2, 0, 2, 2, 1, 1, 1, 1, 1], # 4: Ayako
	[1, 0, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3, 1, 1, 1, 2, 1, 1, 0, 2, 2], # 5: Saki
	[3, 2, 1, 1, 2, 1, 2, 2, 3, 2, 1, 2, 2, 3, 1, 1, 0, 0, 1, 0, 0], # 6: Nozomi
	[4, 3, 2, 2, 3, 3, 2, 2, 3, 1, 3, 2, 2, 3, 1, 1, 1, 0, 1, 3, 0], # 7: Mira
	[4, 3, 1, 1, 1, 2, 2, 2, 3, 1, 1, 0, 1, 3, 2, 0, 1, 0, 0, 2, 1], # 8: Yukari
	[1, 1, 0, 1, 0, 1, 0, 3, 3, 1, 2, 3, 1, 3, 1, 1, 2, 2, 1, 2, 0], # 9: Megumi
	[2, 1, 1, 1, 1, 3, 2, 4, 4, 0, 1, 1, 0, 1, 1, 2, 0, 0, 0, 1, 0], # 10: Yumi
]

## date_syoushin_weight (asm/ovl/DATE/data/date/place_scenes.data.s:727-736)
const DATE_SYOUSHIN_WEIGHT := [-10, -8, -6, 8, 10]

## date_opening_count_a / date_opening_count_b (place_scenes.data.s:705-723)
const DATE_OPENING_COUNT_A := [3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3]
const DATE_OPENING_COUNT_B := [5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 5]

## Girl birthdays (month, day) from check_date_girl_birthday
const GIRL_BIRTHDAYS := [
	Vector2i(10, 9),  # 0: Yuko
	Vector2i(10, 24), # 1: Shiori
	Vector2i(2, 3),   # 2: Mio
	Vector2i(7, 8),   # 3: Yuina
	Vector2i(9, 25),  # 4: Ayako
	Vector2i(1, 13),  # 5: Saki
	Vector2i(12, 3),  # 6: Nozomi
	Vector2i(11, 15), # 7: Mira
	Vector2i(6, 9),   # 8: Yukari
	Vector2i(3, 28),  # 9: Megumi
	Vector2i(5, 16),  # 10: Yumi
]

## Authentic Shiori (CDROM/SERIFU/DATE_FU.BIN + TAKU3.BIN + SERIFU.BIN) scripts
const SPEAKER_SHIORI_HEX := "93a18de8" # 藤崎
const SPEAKER_PLAYER_DEFAULT_HEX := "91ce93aa" # 対頭

const ARRIVAL_WAIT_PLAYER_L0_HEX := "82dc82be8e8d904482cd814120978882c482a282c882a282e682a482be82c88142" # まだ詩織は、 来ていないようだな。
const ARRIVAL_WAIT_SHIORI_L0_HEX := "82b282df82f182cb8142208f8094f582c98e9e8ad482aa2082a982a982c182bf82e182c182c481638142" # ごめんね。 準備に時間が かかっちゃって…。
const ARRIVAL_DIRECT_PLAYER_L0_HEX := "91d282c182bd8148" # 待った？
const ARRIVAL_DIRECT_SHIORI_L1_HEX := "82a482a482f18142208e8482e08141208da1978882bd82c682b182eb82be82a982e781638142" # ううん。 私も、 今来たところだから…。
const ARRIVAL_CORRECTED_SHIORI_L1_HEX := "82a482a482f1814182a282a282cc8142208e8482e081418da1978882bd82c682b182eb2082be82a982e781638142" # ううん、いいの。 私も、今来たところ だから…。
const ARRIVAL_BAD_LOOKS_SHIORI_HEX := "81648164816481648164816481648164816481648142" # ‥‥‥‥‥‥‥‥‥‥。

## Aquarium Indoor Variant 0 (CDROM/SERIFU/DATE_FU.BIN 0x801A0540 Group 1 & 2, TAKU3.BIN classes [0, 4, 2])
const AQUARIUM_PROMPT_SHIORI_HEX := "8aef97ed82c882a88b9b82aa81412082bd82ad82b382f182a282e982cb8142" # 奇麗なお魚が、 たくさんいるね。
const AQUARIUM_CHOICES_HEX := [
	"908582cc928682cc8341815b836782be82cb8142", # 0: 水の中のアートだね。 (Class 0 Best)
	"82b782b282ad82a882a282b582bb82a482be82cb8142", # 1: すごくおいしそうだね。 (Class 4 Terrible)
	"967b939681418aef97ed82be82cb8142",         # 2: 本当、奇麗だね。 (Class 2 Mediocre)
]
const AQUARIUM_REPLIES_SHIORI_HEX := [
	"916693478141838d837d8393836583428362834e82cb8142", # 0: 素敵、ロマンティックね。
	"82c782a482b582c42082bb82f182c88e9682b582a9208d6c82a682e782ea82c882a282cc8148", # 1: どうして そんな事しか 考えられないの？
	"82a082c182bf82c982e0208aef97ed82c88b9b82aa82a282e982e68142", # 2: あっちにも 奇麗な魚がいるよ。
]
const AQUARIUM_VARIANT_CLASSES := [
	[0, 4, 2], # Variant 0: Indoor 1
	[1, 4, 2], # Variant 1: Indoor 2
	[1, 4, 3], # Variant 2: Dolphin Show
]
const AQUARIUM_VARIANT_FACES := [
	[1, 4, 2],
	[1, 4, 2],
	[1, 4, 3],
]

## Choice Reaction Monologues from SERIFU.BIN (0x80197B04 Group 0 l00..l06)
const CHOICE_REACTION_MONOLOGUE_HEX := [
	"836f83628360838a814197c782a288f38fdb82f020975e82a682bd82dd82bd82a282be82bc8142", # 0: バッチリ、良い印象を 与えたみたいだぞ。
	"82ed82e882c697c782a288f38fdb82f020975e82a682bd82dd82bd82a282be82bc8142",         # 1: わりと良い印象を 与えたみたいだぞ。
	"82dc829f814182b182f182c882e082f182be82eb82a48142",                             # 2: まぁ、こんなもんだろう。
	"82a082dc82e897c782a288f38fdb82f0208e9d82bd82ea82c882a982c182bd82a982c88142",     # 3: あまり良い印象を 持たれなかったかな。
	"88ab82a288f38fdb82f020975e82a682bf82e182c182bd82a982c88142",                     # 4: 悪い印象を 与えちゃったかな。
	"82b582a9829f82b581412089b482cc895282aa82ce82ea82c482b582dc82c182bd8142208ec082cd82a082dc82e8896a82b082c882a282f182be8142", # 5: Pool lie caught
	"82b582a9829f82b581412089b482cc895282aa82ce82ea82c482b582dc82c182bd8142208ec082cd82a082dc82e88a8a82ea82c882a282f182be8142", # 6: Skate/Ski lie caught
]

## Amusement Park Ride Choice & Roller Coaster Sub-scene (DATE_FU.BIN 0x801A22B8 Group 9, 10, 11, 12)
const AMUSEMENT_RIDE_SELECT_PROMPT_HEX := "82bb82ea82b682e181412082c782b182c98d7382b182a482a98148" # それじゃ、 どこに行こうか？
const AMUSEMENT_RIDE_SELECT_CHOICES_HEX := [
	"83578346836283678352815b8358835e815b82c98fe682eb82a48142", # 0: ジェットコースターに乗ろう。
	"8353815b83588367836e8345835882c98d7382b182a48142",         # 1: ゴーストハウスに行こう。
	"8acf97978ed482c98fe682eb82a482e68142",                     # 2: 観覧車に乗ろうよ。
]
const AMUSEMENT_COASTER_PROMPT_HEX := "96ca949282a982c182bd82cb8142" # 面白かったね。
const AMUSEMENT_COASTER_CHOICES_HEX := [
	"82a082cc83588373815b83688ab481418dc58d8282be82cb8142",     # 0: あのスピード感、最高だね。 (Class 0 Best)
	"82a082f182c882cc8e718b9f82be82dc82b582b682e182c882a28148", # 1: あんなの子供だましじゃない？ (Class 4 Terrible)
	"8c8b8d5c82d182d182c182bf82e182c182bd8142",                 # 2: 結構びびっちゃった。 (Class 2 Mediocre)
]
const AMUSEMENT_COASTER_REPLIES_HEX := [
	"97c782a982c182bd81422088ea8f8f82c98fe682ea82c481638142",   # 0: 良かった。 一緒に乗れて…。
	"82c282dc82e782c882a982c182bd81482082bb82a481638142",       # 1: つまらなかった？ そう…。
	"82a482d382d381412088d38a4f82c689b0956182c882cc82cb8142",   # 2: うふふ、 意外と臆病なのね。
]

## Karaoke Song Labels from D_8013B634 (asm/ovl/DATE2/data/amusement_park_date/karaoke_scene_and_return.data/D_8013B634.s)
const KARAOKE_SONG_LABELS_HEX := [
	"97f682cd82d982c782d982c782c9",             # 0: 恋はほどほどに
	"82c682ab82df82ab",                         # 1: ときめき
	"93f1906c82cc8e9e",                         # 2: 二人の時
	"8f97815882b582a296ec985982c782e082cc8e8d", # 3: 女々しい野郎どもの詩
]
const KARAOKE_VOCAL_LABELS_HEX := {
	0: "97f682cd8163816989cc957482ab816a",             # 恋は…（歌付き）
	3: "8f97815882b582a28163816989cc957482ab816a",     # 女々しい…（歌付き）
}

## Farewell lines from DATE_FU.BIN (0x80199F04)
const FAREWELL_TIER0_SHIORI_HEX := "8da193fa82cd82c682c482e08a7982b582a982c182bd82ed81422082dc82bd8da19378975582c182c482ad82ea82e98148"
const FAREWELL_REPEAT_SPOT_SHIORI_HEX := "82b182cc914f82e020978882bd82c682b182eb82be82a982e781412082bf82e582c182c6964f82ab82bf82e182c182bd82cb8142"
const CLOSING_WALK_HOME_SHIORI_HEX := "82bb82ea82b682e181412088ea8f8f82c98b4182e882dc82b582e582a4814120"


static func create_default_state(overrides: Dictionary = {}) -> Dictionary:
	var girl_slots: Array = []
	for g in range(12):
		girl_slots.append({
			"id": g,
			"state_rank": 1, # 1 = tokimeki/love, 2 = friend/affection, 3 = neutral
			"tokimeki": 60,
			"affection": 60,
			"syoushin": 10,
			"tokimeki_fraction": 60 << 16,
			"affection_fraction": 60 << 16,
			"syoushin_fraction": 10 << 16,
			"last_date_spot": 0xFF,
			"last_date_spot_timer": 0,
			"flags_b2_birthday_gift_given": false,
			"flags_b4_golden_week_date": false,
			"special_scenes_seen_mask": 0,
		})
	var scene_counters: Array = []
	for s in range(21):
		scene_counters.append(0)
	var state := {
		"game_mode": GAME_MODE_DATE,
		"mode_stage": DATE_STAGE_ARRIVAL,
		"mode_step": 0,
		"substep": 0,
		"frame_count": 1, # (frame_count & 3) != 0 -> direct greeting by default
		"year": 1995,
		"month": 7,
		"day": 23,
		"weekday": 6, # Sunday
		"weather": 0,
		"date_girl": GIRL_SHIORI,
		"current_girl": GIRL_SHIORI,
		"appointment_place_0": SPOT_AQUARIUM,
		"appointment_place_1": 0xFF, # 0xFF = player went to the right meeting place
		"appointment_place_2": 0xFF,
		"appointment_place_3": 0xFF,
		"appointment_girl_invited": false,
		"date_girl_missing": 0,
		"event_log_count": 1,
		"day_event_done_bits": 0,
		"day_event_stage": 0,
		"current_place": SPOT_AQUARIUM,
		"talk_date_spot": SPOT_AQUARIUM,
		"place_title_hex": DATE_PLACE_NAMES_HEX[SPOT_AQUARIUM],
		"sub_place_title_hex": SUB_PLACE_NAMES_HEX["indoor_hall"],
		"active_banner_hex": DATE_PLACE_NAMES_HEX[SPOT_AQUARIUM],
		"date_arrival": DATE_ARRIVAL_DIRECT_GREETING,
		"date_arrival_group": 6,
		"date_meeting_group": 0,
		"date_first_sunday": 0,
		"looks_rejected": false,
		"stood_up_or_missed": false,
		"param_stress": 10,
		"param_looks": 65,
		"param_trivia": 80,
		"param_athletics": 75,
		"param_science": 70,
		"param_liberal_arts": 70,
		"param_arts": 70,
		"player_bday_month": 5,
		"player_bday_day": 10,
		"birthday_gift_year": 0,
		"talk_choice": 0,
		"talk_choice_order": [0, 1, 2],
		"choice_classes": [0, 4, 2],
		"choice_faces": [1, 4, 2],
		"date_choice_mood_face": [1, 3, 2],
		"date_choice_tier": [0, 2, 1],
		"last_evaluated_class": 0,
		"last_evaluated_tier": 0,
		"last_evaluated_mood_face": 1,
		"last_reaction_line": 0,
		"farewell_group": 0,
		"closing_group": 0,
		"repeat_spot_penalty_applied": false,
		"date_parade_offered": 0,
		"date_show_taken": 0,
		"date_ride_index": RIDE_ROLLER_COASTER,
		"date_ride_chosen": 0,
		"date_park_new_rides": 1,
		"date_park_newest_rides": 0,
		"date_shop_index": 0,
		"date_movie_index": 0,
		"date_concert_index": 0,
		"date_stadium_game_day": 0,
		"date_game_center_medals_open": 0,
		"exhibit_seen_mask": 0,
		"karaoke_unlock_bits": 0x0D, # bits 0, 2, 3 unlocked alongside default song 1
		"karaoke_unlock_mode": 0,
		"karaoke_selected_song": 1,
		"karaoke_audio_frames": 0,
		"scene_counters": scene_counters,
		"date_place_history": [0xFF, 0xFF, 0xFF, 0xFF],
		"girls": girl_slots,
	}
	for k in overrides.keys():
		state[k] = overrides[k]
	return state


## Mirrors date_init (src/ovl/date/entry.c:671-712)
static func date_init(state: Dictionary) -> void:
	state["game_mode"] = GAME_MODE_DATE
	state["mode_stage"] = DATE_STAGE_ARRIVAL
	state["mode_step"] = 0
	state["day_event_done_bits"] = int(state.get("day_event_done_bits", 0)) | 8
	var spot := int(state.get("appointment_place_0", SPOT_AQUARIUM))
	state["current_place"] = spot
	state["talk_date_spot"] = spot
	state["current_girl"] = int(state.get("date_girl", GIRL_SHIORI))
	state["place_title_hex"] = DATE_PLACE_NAMES_HEX[clampi(spot, 0, DATE_PLACE_NAMES_HEX.size() - 1)]
	state["active_banner_hex"] = state["place_title_hex"]

	var fc := int(state.get("frame_count", 1))
	var girl_id := int(state.get("date_girl", GIRL_SHIORI))
	if (fc & 3) == 0 and girl_id != GIRL_YUINA:
		state["date_arrival"] = DATE_ARRIVAL_WAIT_GREETING
	else:
		state["date_arrival"] = DATE_ARRIVAL_DIRECT_GREETING


## Mirrors date_meeting_place_check (src/ovl/date/place_scenes.c:4483-4519)
static func date_meeting_place_check(state: Dictionary) -> Dictionary:
	if int(state.get("date_girl_missing", 0)) != 0:
		state["date_meeting_group"] = DATE_MEETING_STOOD_UP
		state["stood_up_or_missed"] = true
		return {
			"status": "stood_up",
			"meeting_group": DATE_MEETING_STOOD_UP,
		}
	var place_1 := int(state.get("appointment_place_1", 0xFF))
	if place_1 != 0xFF:
		var place_3 := int(state.get("appointment_place_3", 0xFF))
		if place_3 != 0xFF:
			state["date_meeting_group"] = DATE_MEETING_WRONG_THREE_CHOICE
		else:
			state["date_meeting_group"] = DATE_MEETING_WRONG_TWO_CHOICE
		return {
			"status": "wrong_place_prompt",
			"meeting_group": int(state["date_meeting_group"]),
			"choice_count": 3 if place_3 != 0xFF else 2,
		}
	return {
		"status": "correct_place",
		"meeting_group": -1,
	}


## Mirrors date_meeting_place_resolve & date_wrong_place_outcome (src/ovl/date/place_scenes.c:4531-4593)
static func date_meeting_place_resolve(state: Dictionary, talk_choice: int) -> Dictionary:
	state["talk_choice"] = talk_choice
	var place_3 := int(state.get("appointment_place_3", 0xFF))
	var is_three := (place_3 != 0xFF)
	if talk_choice == 0:
		state["date_meeting_group"] = DATE_MEETING_STAY_HERE_THREE if is_three else DATE_MEETING_STAY_HERE_TWO
	elif talk_choice == 1:
		state["date_meeting_group"] = DATE_MEETING_MOVE_FIRST_THREE if is_three else DATE_MEETING_MOVE_FIRST_TWO
		state["date_arrival"] = DATE_ARRIVAL_CORRECTED_PLACE
		state["current_place"] = int(state.get("appointment_place_2", state["current_place"]))
	else:
		state["date_meeting_group"] = DATE_MEETING_MOVE_SECOND_THREE
		state["date_arrival"] = DATE_ARRIVAL_CORRECTED_PLACE
		state["current_place"] = place_3

	state["event_log_count"] = int(state.get("event_log_count", 1)) - 1
	var cur_place := int(state["current_place"])
	state["talk_date_spot"] = cur_place
	state["place_title_hex"] = DATE_PLACE_NAMES_HEX[clampi(cur_place, 0, DATE_PLACE_NAMES_HEX.size() - 1)]
	state["active_banner_hex"] = state["place_title_hex"]

	var true_place := int(state.get("appointment_place_1", cur_place))
	if cur_place != true_place:
		state["date_meeting_group"] = DATE_MEETING_NOT_HERE
		state["stood_up_or_missed"] = true
		return {
			"found_girl": false,
			"meeting_group": DATE_MEETING_NOT_HERE,
		}
	# Girl was waiting at true_place: syoushin += 8 (place_scenes.c:4591)
	var girl: Dictionary = state["girls"][int(state["current_girl"])]
	_add_girl_int_stat(girl, "syoushin", 8)
	return {
		"found_girl": true,
		"meeting_group": int(state["date_meeting_group"]),
		"date_arrival": int(state["date_arrival"]),
	}


## Mirrors date_arrival_init & date_looks_check (src/ovl/date/place_scenes.c:337-378, 4617-4631)
static func date_arrival_and_looks_check(state: Dictionary) -> Dictionary:
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	var girl_state := clampi(int(girl.get("state_rank", 1)), 1, 3)
	var arr := int(state.get("date_arrival", DATE_ARRIVAL_DIRECT_GREETING))
	var rank_offset := (1 if girl_state >= 2 else 0) + (1 if girl_state >= 3 else 0)
	var arrival_group := rank_offset * ((1 if arr == 0 else 0) + 1) + arr * 6 - (3 if arr == 2 else 0)
	state["date_arrival_group"] = arrival_group
	state["date_first_sunday"] = 1 if (int(state.get("weekday", 6)) == 6 and int(state.get("day", 1)) < 8) else 0

	var looks := int(state.get("param_looks", 65))
	if looks < 30 or (looks < 35 and girl_id == GIRL_MIRA):
		state["date_arrival_group"] = 0x0C
		state["looks_rejected"] = true
		_add_girl_int_stat(girl, "tokimeki", -1)
		_add_girl_int_stat(girl, "affection", -1)
		_add_girl_int_stat(girl, "syoushin", 5)
	else:
		state["looks_rejected"] = false

	return {
		"date_arrival": arr,
		"date_arrival_group": int(state["date_arrival_group"]),
		"looks_rejected": bool(state["looks_rejected"]),
		"date_first_sunday": int(state["date_first_sunday"]),
	}


## Mirrors date_birthday_check (src/ovl/date/place_scenes.c:4656-4703)
static func date_birthday_check(state: Dictionary) -> Dictionary:
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	var spot := int(state.get("current_place", SPOT_AQUARIUM))
	var uses_date2 := (spot == SPOT_SHOPPING_STREET or spot == SPOT_AMUSEMENT_PARK)

	var m := int(state.get("month", 7))
	var d := int(state.get("day", 23))
	var y := int(state.get("year", 1995))

	# Player birthday check: within 7 days after player birthday and gift not yet given this year
	var pm := int(state.get("player_bday_month", 5))
	var pd := int(state.get("player_bday_day", 10))
	if m == pm and d >= pd and d <= pd + 6 and int(state.get("birthday_gift_year", 0)) != y:
		if uses_date2:
			state["game_mode"] = GAME_MODE_AMUSEMENT_PARK_2
			state["mode_stage"] = DATE2_STAGE_PLAYER_BIRTHDAY
		else:
			state["game_mode"] = GAME_MODE_DATE
			state["mode_stage"] = DATE_STAGE_PLAYER_BIRTHDAY
		state["mode_step"] = 0
		return {"triggered": "player_birthday", "game_mode": state["game_mode"], "mode_stage": state["mode_stage"]}

	# Girl birthday check: within +/- 3 days of girl birthday and b2 flag not set
	if girl_id >= 0 and girl_id < GIRL_BIRTHDAYS.size():
		var gb: Vector2i = GIRL_BIRTHDAYS[girl_id]
		if m == gb.x and absi(d - gb.y) <= 3 and not bool(girl.get("flags_b2_birthday_gift_given", false)):
			if uses_date2:
				state["game_mode"] = GAME_MODE_AMUSEMENT_PARK_2
				state["mode_stage"] = DATE2_STAGE_GIRL_BIRTHDAY
			else:
				state["game_mode"] = GAME_MODE_DATE
				state["mode_stage"] = DATE_STAGE_GIRL_BIRTHDAY
			state["mode_step"] = 0
			return {"triggered": "girl_birthday", "game_mode": state["game_mode"], "mode_stage": state["mode_stage"]}

	state["game_mode"] = GAME_MODE_DATE
	state["mode_stage"] = DATE_STAGE_PLACE
	state["mode_step"] = 0
	return {"triggered": "none", "game_mode": state["game_mode"], "mode_stage": state["mode_stage"]}


## Mirrors date_girl_birthday_apply_gift_choice (src/ovl/date/girl_birthday.c:328-344)
## and amusement_girl_birthday_choice_Outcome (src/ovl/amusement_park_date/scene_choices_and_saved_return.c:177-193)
static func apply_girl_birthday_gift_choice(state: Dictionary, talk_choice: int) -> Dictionary:
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	girl["flags_b2_birthday_gift_given"] = true
	state["talk_choice"] = talk_choice
	if talk_choice == 0:
		_add_girl_int_stat(girl, "tokimeki", 3)
		_add_girl_int_stat(girl, "affection", 2)
		_add_girl_int_stat(girl, "syoushin", -15)
	elif talk_choice == 1:
		_add_girl_int_stat(girl, "tokimeki", 2)
		_add_girl_int_stat(girl, "affection", 1)
		_add_girl_int_stat(girl, "syoushin", -10)
	else:
		_add_girl_int_stat(girl, "affection", 1)
		_add_girl_int_stat(girl, "syoushin", 10)
	state["game_mode"] = GAME_MODE_DATE
	state["mode_stage"] = DATE_STAGE_PLACE
	state["mode_step"] = 0
	return {
		"tokimeki": int(girl["tokimeki"]),
		"affection": int(girl["affection"]),
		"syoushin": int(girl["syoushin"]),
	}


## Mirrors date_player_birthday_receive_gift (src/ovl/date/player_birthday.c:130-156)
static func apply_player_birthday_gift(state: Dictionary) -> void:
	state["birthday_gift_year"] = int(state.get("year", 1995))
	state["game_mode"] = GAME_MODE_DATE
	state["mode_stage"] = DATE_STAGE_PLACE
	state["mode_step"] = 0


## Mirrors junban_init (src/main/scene_library.c:2181-2193)
static func junban_init(frame_count: int) -> Array:
	var order := [0, 0, 0]
	var first := posmod(frame_count, 3)
	order[0] = first
	if (frame_count & 1) != 0:
		order[1] = (first + 1) % 3
		order[2] = (first + 2) % 3
	else:
		order[1] = (first + 2) % 3
		order[2] = (first + 1) % 3
	return order


## Mirrors date_choice_classes_derive (src/ovl/date/place_scenes.c:1569-1590)
static func date_choice_classes_derive(choice_classes: Array) -> Dictionary:
	var mood_faces := [1, 1, 1]
	var tiers := [0, 0, 0]
	for i in range(3):
		var c := int(choice_classes[i])
		var face := c
		if face == 0:
			face = 1
		if face == 4:
			face = 3
		mood_faces[i] = face

		var tier := c - 1
		if tier < 0:
			tier = 0
		if tier >= 3:
			tier = 2
		tiers[i] = tier
	return {
		"date_choice_mood_face": mood_faces,
		"date_choice_tier": tiers,
	}


## Mirrors date_score_multipliers (src/ovl/date/place_scenes.c:3437-3506)
static func date_score_multipliers(score_delta: int, param_trivia: int, girl_id: int, date_spot: int) -> Vector2i:
	var trivia_mult := 0
	var place_mult := 0
	var g := clampi(girl_id, 0, DATE_PLACE_AFFINITY.size() - 1)
	var s := clampi(date_spot, 0, DATE_PLACE_AFFINITY[0].size() - 1)
	var affinity: int = DATE_PLACE_AFFINITY[g][s]

	if score_delta > 0:
		for i in range(5):
			if param_trivia == 0:
				trivia_mult = 0
				break
			if param_trivia >= 100 - (i * 25):
				trivia_mult = 5 - i
				break
		if affinity == 0:
			place_mult = 5
		elif affinity == 1 or affinity == 2:
			place_mult = 4
		elif affinity == 3:
			place_mult = 3
		else:
			place_mult = 2
	else:
		for i in range(5):
			if param_trivia == 0:
				trivia_mult = 8
				break
			if param_trivia >= 100 - (i * 25):
				trivia_mult = i + 3
				break
		if affinity == 0:
			place_mult = 3
		elif affinity == 1 or affinity == 2:
			place_mult = 4
		elif affinity == 3:
			place_mult = 5
		else:
			place_mult = 6
	return Vector2i(trivia_mult, place_mult)


## Mirrors date_choice_score (src/ovl/date/place_scenes.c:3416-3435)
static func date_choice_score(state: Dictionary, selected_choice: int) -> Dictionary:
	state["talk_choice"] = selected_choice
	var classes: Array = state.get("choice_classes", [0, 4, 2])
	var derived := date_choice_classes_derive(classes)
	state["date_choice_mood_face"] = derived["date_choice_mood_face"]
	state["date_choice_tier"] = derived["date_choice_tier"]

	var response_class := clampi(int(classes[clampi(selected_choice, 0, 2)]), 0, 4)
	var spot := int(state.get("talk_date_spot", SPOT_AQUARIUM))
	var variant := int(state["scene_counters"][spot])
	var reaction_line := response_class

	# Venue-specific stat checks that override response_class/tier/reaction_line
	# Planetarium science check (place_scenes.c:838-848)
	if spot == SPOT_PLANETARIUM and (variant % 2) == 1 and selected_choice == 0 and int(state.get("param_science", 70)) < 60:
		response_class = 3
		state["date_choice_tier"][0] = 2
		reaction_line = 3
	# Pool athletics check (place_scenes.c:1306-1317)
	elif spot == SPOT_POOL and (variant % 2) == 0 and selected_choice == 0 and int(state.get("param_athletics", 75)) < 70:
		response_class = 4
		state["date_choice_tier"][0] = 2
		reaction_line = 5
	# Skating Rink athletics check (place_scenes.c:1460-1471)
	elif spot == SPOT_SKATING_RINK and (variant % 2) == 1 and selected_choice == 0 and int(state.get("param_athletics", 75)) < 80:
		response_class = 4
		state["date_choice_tier"][0] = 2
		reaction_line = 6
	# Ski Resort athletics check (place_scenes.c:1525-1536)
	elif spot == SPOT_SKI_RESORT and (variant % 2) == 0 and selected_choice == 0 and int(state.get("param_athletics", 75)) < 90:
		response_class = 4
		state["date_choice_tier"][0] = 2
		reaction_line = 6

	var tokimeki_delta := 3 - response_class
	var affection_delta := 2 - response_class
	var syoushin_weight: int = DATE_SYOUSHIN_WEIGHT[response_class]

	var trivia := int(state.get("param_trivia", 80))
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]

	var m1 := date_score_multipliers(tokimeki_delta, trivia, girl_id, spot)
	var m2 := date_score_multipliers(affection_delta, trivia, girl_id, spot)

	var tok_fp_delta := int(((tokimeki_delta * m1.x * m1.y) * 65536) / 16)
	var aff_fp_delta := int(((affection_delta * m2.x * m2.y) * 65536) / 16)
	var syo_fp_delta := int(((syoushin_weight * m2.x * m2.y) * 65536) / 16)

	girl["tokimeki_fraction"] = clampi(int(girl["tokimeki_fraction"]) + tok_fp_delta, 0, 100 << 16)
	girl["affection_fraction"] = clampi(int(girl["affection_fraction"]) + aff_fp_delta, 0, 100 << 16)
	girl["syoushin_fraction"] = clampi(int(girl["syoushin_fraction"]) + syo_fp_delta, 0, 100 << 16)
	girl["tokimeki"] = int(girl["tokimeki_fraction"]) >> 16
	girl["affection"] = int(girl["affection_fraction"]) >> 16
	girl["syoushin"] = int(girl["syoushin_fraction"]) >> 16

	state["last_evaluated_class"] = response_class
	state["last_evaluated_tier"] = int(state["date_choice_tier"][clampi(selected_choice, 0, 2)])
	state["last_evaluated_mood_face"] = int(state["date_choice_mood_face"][clampi(selected_choice, 0, 2)])
	state["last_reaction_line"] = reaction_line

	return {
		"response_class": response_class,
		"tier": int(state["last_evaluated_tier"]),
		"mood_face": int(state["last_evaluated_mood_face"]),
		"reaction_line": reaction_line,
		"tokimeki_fp_delta": tok_fp_delta,
		"affection_fp_delta": aff_fp_delta,
		"syoushin_fp_delta": syo_fp_delta,
		"tokimeki_fraction": int(girl["tokimeki_fraction"]),
		"affection_fraction": int(girl["affection_fraction"]),
		"syoushin_fraction": int(girl["syoushin_fraction"]),
	}


## Sets up any of the 21 date spots (0..20) per date_place_setup_* in src/ovl/date/place_scenes.c
static func setup_date_spot(state: Dictionary, spot_id: int) -> Dictionary:
	var s := clampi(spot_id, 0, 20)
	state["current_place"] = s
	state["talk_date_spot"] = s
	state["place_title_hex"] = DATE_PLACE_NAMES_HEX[s]
	var visit_count := int(state["scene_counters"][s])
	var m := int(state.get("month", 7))
	var d := int(state.get("day", 23))
	var y := int(state.get("year", 1995))
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	var sub_banner: String = String(DATE_PLACE_NAMES_HEX[s])
	var sub_spot_key: String = "default"

	match s:
		SPOT_NEIGHBORHOOD_PARK:
			sub_banner = DATE_PLACE_NAMES_HEX[SPOT_NEIGHBORHOOD_PARK]
			sub_spot_key = "neighborhood_park"
			state["choice_classes"] = [1, 1, 2]
		SPOT_CENTRAL_PARK:
			if (visit_count % 2) == 0:
				if m == 4 and d < 16:
					sub_banner = SUB_PLACE_NAMES_HEX["cherry_avenue"]
					sub_spot_key = "cherry_avenue"
				else:
					sub_banner = SUB_PLACE_NAMES_HEX["avenue"]
					sub_spot_key = "avenue"
				state["choice_classes"] = [0, 2, 4]
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["pond"]
				sub_spot_key = "pond"
				state["choice_classes"] = [1, 3, 2]
		SPOT_SHOPPING_STREET:
			var shop_idx := visit_count % 3
			state["date_shop_index"] = shop_idx
			if shop_idx == 0:
				sub_banner = SUB_PLACE_NAMES_HEX["boutique"]
				sub_spot_key = "boutique"
				state["choice_classes"] = [0, 2, 4]
			elif shop_idx == 1:
				sub_banner = SUB_PLACE_NAMES_HEX["fancy_shop"]
				sub_spot_key = "fancy_shop"
				state["choice_classes"] = [1, 2, 3]
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["junk_shop"]
				sub_spot_key = "junk_shop"
				state["choice_classes"] = [2, 1, 4]
		SPOT_AQUARIUM:
			var aq_var := visit_count % 3
			if aq_var == 2:
				sub_banner = SUB_PLACE_NAMES_HEX["dolphin_show"]
				sub_spot_key = "dolphin_show"
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["indoor_hall"]
				sub_spot_key = "indoor_hall"
			state["choice_classes"] = AQUARIUM_VARIANT_CLASSES[aq_var].duplicate()
			state["choice_faces"] = AQUARIUM_VARIANT_FACES[aq_var].duplicate()
		SPOT_ZOO:
			sub_banner = SUB_PLACE_NAMES_HEX["park_inside"]
			sub_spot_key = "zoo_inside_%d" % (visit_count % 4)
			state["choice_classes"] = [1, 2, 4]
		SPOT_BOTANICAL_GARDEN:
			sub_banner = SUB_PLACE_NAMES_HEX["park_inside"]
			sub_spot_key = "botanical_inside_%d" % (visit_count % 2)
			state["choice_classes"] = [1, 2, 3]
		SPOT_PLANETARIUM:
			sub_banner = SUB_PLACE_NAMES_HEX["indoor_hall"]
			sub_spot_key = "planetarium_inside_%d" % (visit_count % 2)
			state["choice_classes"] = [0, 2, 4]
		SPOT_ART_MUSEUM:
			if m == 4 or m == 5:
				sub_banner = SUB_PLACE_NAMES_HEX["gargy_exhibit"]
				sub_spot_key = "gargy_exhibit"
			elif m == 7 or m == 8:
				sub_banner = SUB_PLACE_NAMES_HEX["sculpture_exhibit"]
				sub_spot_key = "sculpture_exhibit"
			elif m == 10 or m == 11:
				sub_banner = SUB_PLACE_NAMES_HEX["painters_exhibit"]
				sub_spot_key = "painters_exhibit"
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["indoor_hall"]
				sub_spot_key = "permanent_exhibit"
			if sub_spot_key != "permanent_exhibit":
				var mask := int(state.get("exhibit_seen_mask", 0))
				if (mask & (1 << girl_id)) != 0:
					_add_girl_int_stat(girl, "syoushin", 8)
					_add_girl_int_stat(girl, "affection", -2)
				else:
					state["exhibit_seen_mask"] = mask | (1 << girl_id)
			state["choice_classes"] = [0, 2, 4]
		SPOT_LIBRARY:
			sub_banner = SUB_PLACE_NAMES_HEX["library_inside"]
			sub_spot_key = "library_inside_%d" % (visit_count % 3)
			state["param_liberal_arts"] = int(state.get("param_liberal_arts", 70)) + 2
			state["param_science"] = int(state.get("param_science", 70)) + 2
			state["param_trivia"] = int(state.get("param_trivia", 80)) + 2
			state["choice_classes"] = [0, 2, 3]
		SPOT_GAME_CENTER:
			var medals_open := 1 if (y > 1995 or (y == 1995 and m >= 9)) else 0
			state["date_game_center_medals_open"] = medals_open
			if (visit_count % 3) == 2 and medals_open != 0:
				sub_banner = SUB_PLACE_NAMES_HEX["medal_game_corner"]
				sub_spot_key = "medal_game_corner"
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["video_game_corner"]
				sub_spot_key = "video_game_corner"
			state["choice_classes"] = [1, 2, 4]
		SPOT_BOWLING_ALLEY:
			sub_banner = SUB_PLACE_NAMES_HEX["bowling_alley"]
			sub_spot_key = "bowling_lane_%d" % (visit_count % 3)
			state["choice_classes"] = [1, 2, 4]
		SPOT_KARAOKE:
			sub_banner = SUB_PLACE_NAMES_HEX["karaoke_box"]
			if (visit_count % 3) == 2:
				sub_spot_key = "karaoke_date2_song_select"
				state["game_mode"] = GAME_MODE_AMUSEMENT_PARK_2
				state["mode_stage"] = DATE2_STAGE_KARAOKE
				state["mode_step"] = 0
			else:
				sub_spot_key = "karaoke_box_%d" % (visit_count % 3)
			state["choice_classes"] = [0, 4, 2]
		SPOT_AMUSEMENT_PARK:
			state["date_park_new_rides"] = 1 if ((y == 1995 and m > 3) or y >= 1996) else 0
			state["date_park_newest_rides"] = 1 if ((y == 1996 and m > 3) or y >= 1997) else 0
			state["date_parade_offered"] = 1 if m == 8 else 0
			if (visit_count % 3) == 2:
				sub_banner = SUB_PLACE_NAMES_HEX["hero_show"]
				sub_spot_key = "hero_show"
				state["date_show_taken"] = 1
				state["choice_classes"] = [1, 3, 2]
			else:
				var ride_idx := int(state.get("date_ride_index", RIDE_ROLLER_COASTER))
				sub_banner = _ride_banner_hex(ride_idx)
				sub_spot_key = "ride_%d" % ride_idx
				state["date_show_taken"] = 0
				state["choice_classes"] = [0, 4, 2]
		SPOT_STADIUM:
			sub_banner = SUB_PLACE_NAMES_HEX["stadium_inside"]
			var game_day := 1 if ((d >= 8 and d < 15) or (d >= 22 and d < 29)) else 0
			state["date_stadium_game_day"] = game_day
			sub_spot_key = "stadium_game_day" if game_day == 1 else "stadium_normal"
			state["choice_classes"] = [1, 2, 4]
		SPOT_MOVIE_THEATER:
			sub_banner = SUB_PLACE_NAMES_HEX["indoor_hall"]
			var film_idx := (y - 1995) * 4 + int((m - 1) / 3)
			film_idx = clampi(film_idx, 0, 10)
			state["date_movie_index"] = film_idx
			sub_spot_key = "movie_%d" % film_idx
			state["choice_classes"] = [0, 2, 4]
		SPOT_CONCERT_HALL:
			sub_banner = SUB_PLACE_NAMES_HEX["concert_inside"]
			var prog_idx := clampi((y - 1995) * 4 + int((m - 1) / 3), 0, 11)
			state["date_concert_index"] = prog_idx
			sub_spot_key = "concert_%d" % prog_idx
			state["choice_classes"] = [0, 2, 4]
		SPOT_POOL:
			sub_banner = SUB_PLACE_NAMES_HEX["pool_inside"]
			sub_spot_key = "pool_inside_%d" % (visit_count % 2)
			state["choice_classes"] = [0, 2, 4]
		SPOT_SEA:
			var sea_var := visit_count % 3
			if sea_var == 2:
				sub_banner = SUB_PLACE_NAMES_HEX["deserted_island"]
				sub_spot_key = "deserted_island"
			elif sea_var == 1:
				sub_banner = SUB_PLACE_NAMES_HEX["beach"]
				sub_spot_key = "beach"
			else:
				sub_banner = SUB_PLACE_NAMES_HEX["coast"]
				sub_spot_key = "coast"
			if girl_id == GIRL_NOZOMI and int(state.get("param_athletics", 75)) < 100:
				_add_girl_int_stat(girl, "tokimeki", -2)
				_add_girl_int_stat(girl, "affection", -1)
			state["choice_classes"] = [0, 2, 4]
		SPOT_SHRINE:
			if m == 8 and d <= 7:
				sub_banner = SUB_PLACE_NAMES_HEX["fireworks"]
				sub_spot_key = "fireworks"
			elif (m == 7 and d >= 16) or (m == 8 and d >= 20):
				sub_banner = SUB_PLACE_NAMES_HEX["festival"]
				sub_spot_key = "festival_stalls"
			else:
				sub_banner = DATE_PLACE_NAMES_HEX[SPOT_SHRINE]
				sub_spot_key = "shrine_grounds"
			state["choice_classes"] = [0, 2, 3]
		SPOT_SKATING_RINK:
			sub_banner = SUB_PLACE_NAMES_HEX["skating_rink"]
			sub_spot_key = "skating_rink_%d" % (visit_count % 2)
			state["choice_classes"] = [0, 2, 4]
		SPOT_SKI_RESORT:
			sub_banner = SUB_PLACE_NAMES_HEX["ski_slope"]
			sub_spot_key = "ski_slope_%d" % (visit_count % 2)
			state["choice_classes"] = [0, 2, 4]

	state["sub_place_title_hex"] = sub_banner
	state["active_banner_hex"] = sub_banner
	state["talk_choice_order"] = junban_init(int(state.get("frame_count", 1)))
	var derived := date_choice_classes_derive(state["choice_classes"])
	state["date_choice_mood_face"] = derived["date_choice_mood_face"]
	state["date_choice_tier"] = derived["date_choice_tier"]

	return {
		"spot_id": s,
		"sub_spot_key": sub_spot_key,
		"place_title_hex": state["place_title_hex"],
		"sub_place_title_hex": sub_banner,
		"game_mode": int(state["game_mode"]),
		"mode_stage": int(state["mode_stage"]),
	}


## Selects an amusement park ride (0..4) and updates the sub-place banner.
static func select_amusement_park_ride(state: Dictionary, ride_index: int) -> Dictionary:
	var r := clampi(ride_index, 0, 4)
	state["date_ride_index"] = r
	state["date_ride_chosen"] = 1
	var banner := _ride_banner_hex(r)
	state["sub_place_title_hex"] = banner
	state["active_banner_hex"] = banner
	state["choice_classes"] = [0, 4, 2]
	var derived := date_choice_classes_derive(state["choice_classes"])
	state["date_choice_mood_face"] = derived["date_choice_mood_face"]
	state["date_choice_tier"] = derived["date_choice_tier"]
	return {
		"ride_index": r,
		"sub_place_title_hex": banner,
	}


## Evaluates DATE2 Karaoke song completion or early cancel (src/ovl/amusement_park_date/karaoke_scene_and_return.c:404-428)
static func finish_date2_karaoke_song(state: Dictionary, song_index: int, completed_full_song: bool) -> Dictionary:
	state["karaoke_selected_song"] = clampi(song_index, 0, 3)
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	if completed_full_song:
		state["talk_choice"] = 0
		_add_girl_int_stat(girl, "tokimeki", 2)
		_add_girl_int_stat(girl, "affection", 1)
	else:
		state["talk_choice"] = 1
		_add_girl_int_stat(girl, "affection", 1)
	# amusement_return_to_date (karaoke_scene_and_return.c:167-171):
	# game_mode_set(GAME_MODE_DATE); mode_stage_set(1); mode_step_set(0x11);
	state["game_mode"] = GAME_MODE_DATE
	state["mode_stage"] = DATE_STAGE_PLACE
	state["mode_step"] = 0x11
	return {
		"game_mode": GAME_MODE_DATE,
		"mode_stage": DATE_STAGE_PLACE,
		"mode_step": 0x11,
		"tokimeki": int(girl["tokimeki"]),
		"affection": int(girl["affection"]),
	}


## Mirrors date_farewell_speak, date_closing_speak, date_scene_choose, date_ambush_check, and date_end
## (src/ovl/date/place_scenes.c:3114-3195, 3680-3960)
static func finish_date(state: Dictionary, trigger_ambush: bool = false) -> Dictionary:
	var girl_id := int(state.get("current_girl", GIRL_SHIORI))
	var girl: Dictionary = state["girls"][girl_id]
	var girl_state := clampi(int(girl.get("state_rank", 1)), 1, 3)
	var tier := clampi(int(state.get("last_evaluated_tier", 0)), 0, 2)
	var spot := int(state.get("talk_date_spot", SPOT_AQUARIUM))

	# Farewell group calculation (place_scenes.c:3114-3163)
	var farewell_group := (girl_state - 1) * 3 + tier
	var repeat_penalty := false
	var last_spot := int(girl.get("last_date_spot", 0xFF))
	var last_timer := int(girl.get("last_date_spot_timer", 0))
	if spot == last_spot and last_timer != 0 and spot != SPOT_NEIGHBORHOOD_PARK and spot != SPOT_SHRINE and int(state.get("date_parade_offered", 0)) == 0 and not bool(state.get("appointment_girl_invited", false)):
		repeat_penalty = true
		if girl_id == GIRL_NOZOMI and girl_state < 3:
			farewell_group = 0x13
		else:
			farewell_group = 0x0B
		_add_girl_int_stat(girl, "tokimeki", -1)
		_add_girl_int_stat(girl, "affection", -2)
		_add_girl_int_stat(girl, "syoushin", 5)

	state["farewell_group"] = farewell_group
	state["repeat_spot_penalty_applied"] = repeat_penalty

	# Closing speak group (place_scenes.c:3166-3189):
	# rank 1 (tokimeki) -> 0 (walk home together), rank 2 -> 1, rank 3 -> 2/3
	var closing_group := girl_state - 1
	if tier == 2 and girl_state == 3:
		closing_group = 3
	state["closing_group"] = closing_group

	# Check special scene handoff (date_scene_choose -> GAME_MODE_DATE_SCENE 0x5E)
	var special_scene_triggered := _check_special_date_scene(state, girl, spot)
	if special_scene_triggered != "":
		state["game_mode"] = GAME_MODE_DATE_SCENE
		return {
			"outcome": "special_scene",
			"special_scene": special_scene_triggered,
			"game_mode": GAME_MODE_DATE_SCENE,
			"farewell_group": farewell_group,
			"closing_group": closing_group,
		}

	# Check delinquent ambush on eligible outdoor/arcade spots (date_ambush_check -> GAME_MODE_BATTLE_EVENT 0x57)
	if trigger_ambush and spot in [
		SPOT_NEIGHBORHOOD_PARK, SPOT_CENTRAL_PARK, SPOT_SHOPPING_STREET,
		SPOT_ZOO, SPOT_BOTANICAL_GARDEN, SPOT_GAME_CENTER,
		SPOT_AMUSEMENT_PARK, SPOT_SEA, SPOT_SHRINE
	]:
		state["game_mode"] = GAME_MODE_BATTLE_EVENT
		return {
			"outcome": "ambush_battle",
			"game_mode": GAME_MODE_BATTLE_EVENT,
			"farewell_group": farewell_group,
			"closing_group": closing_group,
		}

	# Normal date_end (src/ovl/date/place_scenes.c:3680-3738)
	state["scene_counters"][spot] = int(state["scene_counters"][spot]) + 1
	var mapped_last_spot := spot
	if mapped_last_spot == SPOT_LIBRARY:
		mapped_last_spot = SPOT_ART_MUSEUM
	elif mapped_last_spot == SPOT_PLANETARIUM:
		mapped_last_spot = SPOT_BOTANICAL_GARDEN
	girl["last_date_spot"] = mapped_last_spot
	girl["last_date_spot_timer"] = 12

	if spot == SPOT_SHRINE:
		_add_girl_int_stat(girl, "tokimeki", 1)

	var m := int(state.get("month", 7))
	var d := int(state.get("day", 23))
	if m == 5 and (d == 4 or d == 5) and bool(girl.get("flags_b4_golden_week_date", false)):
		_add_girl_int_stat(girl, "tokimeki", 2)
	if m == 5 and (d == 3 or d == 4):
		girl["flags_b4_golden_week_date"] = true
	else:
		girl["flags_b4_golden_week_date"] = false

	_add_girl_int_stat(girl, "syoushin", -40)

	var hist: Array = state.get("date_place_history", [0xFF, 0xFF, 0xFF, 0xFF])
	state["date_place_history"] = [spot, hist[0], hist[1], hist[2]]
	state["day_event_stage"] = int(state.get("day_event_stage", 0)) + 1

	return {
		"outcome": "completed",
		"farewell_group": farewell_group,
		"closing_group": closing_group,
		"repeat_spot_penalty_applied": repeat_penalty,
		"mapped_last_spot": mapped_last_spot,
		"last_date_spot_timer": 12,
		"day_event_stage": int(state["day_event_stage"]),
	}


static func _ride_banner_hex(ride_index: int) -> String:
	match ride_index:
		RIDE_ROLLER_COASTER:
			return SUB_PLACE_NAMES_HEX["roller_coaster"]
		RIDE_GHOST_HOUSE:
			return SUB_PLACE_NAMES_HEX["ghost_house"]
		RIDE_FERRIS_WHEEL:
			return SUB_PLACE_NAMES_HEX["ferris_wheel"]
		RIDE_SCREAM_MACHINE:
			return SUB_PLACE_NAMES_HEX["scream_machine"]
		RIDE_VIRTUAL_SHIP:
			return SUB_PLACE_NAMES_HEX["virtual_ship"]
		_:
			return SUB_PLACE_NAMES_HEX["roller_coaster"]


static func _check_special_date_scene(state: Dictionary, girl: Dictionary, spot: int) -> String:
	if int(girl.get("state_rank", 1)) != 1:
		return ""
	var m := int(state.get("month", 7))
	var mask := int(girl.get("special_scenes_seen_mask", 0))
	if int(state.get("current_girl", GIRL_SHIORI)) == GIRL_SHIORI:
		if spot == SPOT_CENTRAL_PARK and (m == 3 or m == 4) and (mask & 1) == 0:
			girl["special_scenes_seen_mask"] = mask | 1
			return "shiori_spring_central_park"
		if spot == SPOT_POOL and (m == 7 or m == 8) and (mask & 2) == 0:
			girl["special_scenes_seen_mask"] = mask | 2
			return "shiori_summer_pool"
		if spot == SPOT_CENTRAL_PARK and (m == 10 or m == 11) and (mask & 4) == 0:
			girl["special_scenes_seen_mask"] = mask | 4
			return "shiori_autumn_central_park"
		if spot == SPOT_LIBRARY and (m == 12 or m == 1 or m == 2) and (mask & 8) == 0:
			girl["special_scenes_seen_mask"] = mask | 8
			return "shiori_winter_library"
	return ""


static func _add_girl_int_stat(girl: Dictionary, key: String, delta: int) -> void:
	var next_val := clampi(int(girl.get(key, 0)) + delta, 0, 100)
	girl[key] = next_val
	girl["%s_fraction" % key] = next_val << 16
