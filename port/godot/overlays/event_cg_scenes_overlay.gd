class_name EventCgScenesOverlay
extends RefCounted
## T-29018 Lane 8: EVENT.EXN Per-Girl Special CG & Ride Movie Scenes State Machine.
## Faithfully mirrors:
## - src/ovl/date_spots/girl_scene_entry.c (spot_girl_scene_frame, spot_talk_step, dispatch)
## - src/ovl/date_spots/place_scene_entry.c (spot_place_scene_frame, arrival, place runners)
## - src/ovl/date_spots/place_scenes.c (evaluation of special CG scenes and scene_flags[0..0x4A])
## - src/ovl/date_spots/ride_movies.c (amusement park ride movies: roller coaster, ghost house, ferris wheel, etc.)
## - Character scene runners: shiori_scenes.c .. yoshio_scenes.c (all 13 characters + Yoshio)

const STAGE_ARRIVAL := 0
const STAGE_SCENE := 1
const STAGE_SPECIAL_CG := 2
const STAGE_RIDE_MOVIE := 3
const STAGE_EXIT := 4

# Character Girl IDs matching D_800EECB4 (0..13)
const GIRL_SHIORI := 0
const GIRL_MIO := 1
const GIRL_YUINA := 2
const GIRL_AYAKO := 3
const GIRL_SAKI := 4
const GIRL_YUKARI := 5
const GIRL_NOZOMI := 6
const GIRL_MIRA := 7
const GIRL_YUKO := 8
const GIRL_MEGUMI := 9
const GIRL_YUMI := 10
const GIRL_MIHARU := 11
const GIRL_REI := 12
const GIRL_YOSHIO := 13

# 21 Date Spots matching spot_place (0..20)
const SPOT_NEIGHBORHOOD_PARK := 0
const SPOT_KIRAMEKI_CENTRAL_PARK := 1
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

# 5 Amusement Park Ride Movies matching spot_ride (0..4) and ride_movies.c
const RIDE_ROLLER_COASTER := 0
const RIDE_GHOST_HOUSE := 1
const RIDE_FERRIS_WHEEL := 2
const RIDE_SCREAM_RIDE := 3
const RIDE_VIRTUAL_SHIP := 4

# PS1 CD-ROM LBAs for MDEC Streaming (ride_movies.c)
const RIDE_MOVIE_LBAS := {
	RIDE_ROLLER_COASTER: 0x4330,
	RIDE_GHOST_HOUSE: 0x4331,
	RIDE_FERRIS_WHEEL: 0x4332,
	RIDE_SCREAM_RIDE: 0x4333,
	RIDE_VIRTUAL_SHIP: 0x4334,
}

# Special CG Flag indices in scene_flags (place_scenes.c:135-606)
const SCENE_FLAG_SHIORI_SPRING_PARK := 0x00     # Scene 4: Central Park Cherry Blossoms
const SCENE_FLAG_SHIORI_SUMMER_POOL := 0x01     # Scene 6: Summer Pool
const SCENE_FLAG_SHIORI_AUTUMN_PARK := 0x02     # Scene 3: Autumn Park
const SCENE_FLAG_SHIORI_WINTER_AMUSE := 0x03    # Scene 5: Winter Amusement Park

const SCENE_FLAG_MIO_SPRING_AMUSE := 0x07       # Scene 2: Spring Amusement Park
const SCENE_FLAG_MIO_SUMMER_SEA := 0x08         # Scene 3: Summer Beach
const SCENE_FLAG_MIO_AUTUMN_LIB := 0x09         # Scene 4: Autumn Library
const SCENE_FLAG_MIO_WINTER_MOVIE := 0x0A       # Scene 5: Winter Movie Theater

const SCENE_FLAG_YUINA_SPRING_ZOO := 0x0D       # Scene 2: Spring Zoo
const SCENE_FLAG_YUINA_SUMMER_BOWL := 0x0E      # Scene 3: Summer Bowling
const SCENE_FLAG_YUINA_AUTUMN_GAME := 0x0F      # Scene 4: Autumn Game Center
const SCENE_FLAG_YUINA_WINTER_PARK := 0x10      # Scene 5: Winter Central Park

const SCENE_FLAG_AYAKO_SPRING_KARAOKE := 0x13   # Scene 2: Spring Karaoke
const SCENE_FLAG_AYAKO_SUMMER_ART := 0x14       # Scene 3: Summer Art Museum
const SCENE_FLAG_AYAKO_AUTUMN_ART := 0x15       # Scene 4: Autumn Art Museum
const SCENE_FLAG_AYAKO_WINTER_PLANET := 0x16    # Scene 5: Winter Planetarium

const SCENE_FLAG_SAKI_SPRING_PARK := 0x19       # Scene 2: Spring Central Park
const SCENE_FLAG_SAKI_SUMMER_STADIUM := 0x1A    # Scene 3: Summer Stadium
const SCENE_FLAG_SAKI_AUTUMN_STADIUM := 0x1B    # Scene 4: Autumn Stadium
const SCENE_FLAG_SAKI_WINTER_SHOP := 0x1C       # Scene 5: Winter Shopping Street

const SCENE_FLAG_YUKARI_SPRING_BOTAN := 0x20    # Scene 3: Spring Botanical Garden
const SCENE_FLAG_YUKARI_SUMMER_AQUA := 0x21     # Scene 4: Summer Aquarium
const SCENE_FLAG_YUKARI_AUTUMN_PARK := 0x22     # Scene 5: Autumn Central Park
const SCENE_FLAG_YUKARI_WINTER_SKATE := 0x23    # Scene 6: Winter Skating Rink

const SCENE_FLAG_NOZOMI_SPRING_ART := 0x26      # Scene 2: Spring Art Museum
const SCENE_FLAG_NOZOMI_SUMMER_SEA := 0x27      # Scene 3: Summer Beach
const SCENE_FLAG_NOZOMI_AUTUMN_BOTAN := 0x28    # Scene 4: Autumn Botanical Garden
const SCENE_FLAG_NOZOMI_WINTER_BOWL := 0x29     # Scene 5: Winter Bowling

const SCENE_FLAG_MIRA_SPRING_SHOP := 0x2C       # Scene 2: Spring Shopping Street
const SCENE_FLAG_MIRA_SUMMER_SEA := 0x2D        # Scene 3: Summer Beach
const SCENE_FLAG_MIRA_AUTUMN_AMUSE := 0x2E      # Scene 4: Autumn Amusement Park
const SCENE_FLAG_MIRA_WINTER_SKATE := 0x2F      # Scene 5: Winter Skating Rink
const SCENE_FLAG_MIRA_ARRIVAL_MEET := 0x30      # Scene 6: Arrival encounter

const SCENE_FLAG_YUKO_SPRING_GAME := 0x33       # Scene 2: Spring Game Center
const SCENE_FLAG_YUKO_SUMMER_AMUSE := 0x34      # Scene 3: Summer Amusement Park
const SCENE_FLAG_YUKO_AUTUMN_CONCERT := 0x35    # Scene 4: Autumn Concert Hall
const SCENE_FLAG_YUKO_WINTER_SKI := 0x36        # Scene 5: Winter Ski Resort
const SCENE_FLAG_YUKO_DEC_LIB_DATE := 0x37      # Scene 6: December Library date

const SCENE_FLAG_MEGUMI_SPRING_AMUSE := 0x3A    # Scene 3: Spring Amusement Park
const SCENE_FLAG_MEGUMI_SUMMER_PARK := 0x3B     # Scene 4: Summer Central Park
const SCENE_FLAG_MEGUMI_AUTUMN_ZOO := 0x3C      # Scene 5: Autumn Zoo
const SCENE_FLAG_MEGUMI_WINTER_SKI := 0x3D      # Scene 6: Winter Ski Resort

const SCENE_FLAG_YUMI_SPRING_AMUSE := 0x41      # Scene 3: Spring Amusement Park
const SCENE_FLAG_YUMI_SUMMER_PARK := 0x42       # Scene 4: Summer Central Park
const SCENE_FLAG_YUMI_AUTUMN_STADIUM := 0x43    # Scene 5: Autumn Stadium
const SCENE_FLAG_YUMI_WINTER_SKATE := 0x44      # Scene 6: Winter Skating Rink

const SCENE_FLAG_MIHARU_SPRING_PARK := 0x48     # Scene 3: Spring Central Park encounter
const SCENE_FLAG_MIHARU_SPRING_SEA := 0x4A      # Scene 5: Spring Beach encounter

# State Variables
var stage: int = STAGE_ARRIVAL
var substep: int = 0
var step_timer: int = 0
var current_girl: int = GIRL_SHIORI
var spot_place: int = SPOT_KIRAMEKI_CENTRAL_PARK
var spot_ride: int = -1
var month: int = 4
var year: int = 96
var day: int = 15

# Special CG unlock flags (0x80 bytes array matching g->scene_flags)
var scene_flags: Array[int] = []

# Relationship stats per character (0..13)
var girls_affection: Array[int] = []
var girls_tokimeki: Array[int] = []
var girls_syoushin: Array[int] = []

# Active Scene / Movie state
var active_scene_id: int = 0
var active_movie_lba: int = 0
var dialogue_speaker: String = ""
var dialogue_lines: Array[String] = []
var dialogue_choices: Array[String] = []
var selected_choice: int = -1
var last_result: Dictionary = {}

func _init() -> void:
	reset()

func reset() -> void:
	stage = STAGE_ARRIVAL
	substep = 0
	step_timer = 0
	current_girl = GIRL_SHIORI
	spot_place = SPOT_KIRAMEKI_CENTRAL_PARK
	spot_ride = -1
	month = 4
	year = 96
	day = 15
	active_scene_id = 0
	active_movie_lba = 0
	dialogue_speaker = ""
	dialogue_lines.clear()
	dialogue_choices.clear()
	selected_choice = -1
	last_result.clear()

	scene_flags.clear()
	scene_flags.resize(0x80)
	scene_flags.fill(0)

	girls_affection.clear()
	girls_affection.resize(14)
	girls_affection.fill(10)

	girls_tokimeki.clear()
	girls_tokimeki.resize(14)
	girls_tokimeki.fill(10)

	girls_syoushin.clear()
	girls_syoushin.resize(14)
	girls_syoushin.fill(0)

func init_date_spot(girl_id: int, place_id: int, month_val: int = 4, year_val: int = 96, day_val: int = 15) -> void:
	current_girl = clampi(girl_id, 0, 13)
	spot_place = clampi(place_id, 0, 20)
	month = month_val
	year = year_val
	day = day_val
	stage = STAGE_ARRIVAL
	substep = 0
	step_timer = 0
	active_scene_id = 0
	active_movie_lba = 0
	dialogue_choices.clear()
	selected_choice = -1

# ==============================================================================
# Date Spot Special CG Condition Evaluator (place_scenes.c:130-606)
# ==============================================================================

## Evaluates whether the current date spot conditions match a special event CG scene.
## Returns true if a special scene is triggered, updating active_scene_id and scene_flags.
func evaluate_date_spot_special_scene() -> bool:
	# Miharu arrival encounter check (place_scenes.c:581-606: requires scene_flags[0x46] >= 2)
	if month >= 3 and month < 6 and scene_flags[0x46] >= 2:
		if scene_flags[SCENE_FLAG_MIHARU_SPRING_PARK] == 0:
			if spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and stage == STAGE_ARRIVAL:
				current_girl = GIRL_MIHARU
				active_scene_id = 3
				scene_flags[SCENE_FLAG_MIHARU_SPRING_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIHARU, 3)
				return true
		if scene_flags[SCENE_FLAG_MIHARU_SPRING_SEA] == 0:
			if spot_place == SPOT_SEA and stage == STAGE_ARRIVAL:
				current_girl = GIRL_MIHARU
				active_scene_id = 5
				scene_flags[SCENE_FLAG_MIHARU_SPRING_SEA] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIHARU, 5)
				return true

	match current_girl:
		GIRL_SHIORI:
			# Spring Central Park (place_scenes.c:135-139) -> Scene 4
			if spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_SHIORI_SPRING_PARK] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_SHIORI_SPRING_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SHIORI, 4)
				return true
			# Summer Pool (place_scenes.c:141-145) -> Scene 6
			elif spot_place == SPOT_POOL and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_SHIORI_SUMMER_POOL] == 0:
				active_scene_id = 6
				scene_flags[SCENE_FLAG_SHIORI_SUMMER_POOL] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SHIORI, 6)
				return true
			# Autumn Park (place_scenes.c:150-155) -> Scene 3
			elif spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_SHIORI_AUTUMN_PARK] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_SHIORI_AUTUMN_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SHIORI, 3)
				return true
			# Winter Amusement Park (place_scenes.c:165-169) -> Scene 5
			elif spot_place == SPOT_AMUSEMENT_PARK and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_SHIORI_WINTER_AMUSE] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_SHIORI_WINTER_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SHIORI, 5)
				return true

		GIRL_MIO:
			# Spring Amusement Park -> Scene 2
			if spot_place == SPOT_AMUSEMENT_PARK and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_MIO_SPRING_AMUSE] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_MIO_SPRING_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIO, 2)
				return true
			# Summer Beach -> Scene 3
			elif spot_place == SPOT_SEA and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_MIO_SUMMER_SEA] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_MIO_SUMMER_SEA] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIO, 3)
				return true
			# Autumn Library -> Scene 4
			elif spot_place == SPOT_LIBRARY and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_MIO_AUTUMN_LIB] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_MIO_AUTUMN_LIB] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIO, 4)
				return true
			# Winter Movie Theater -> Scene 5
			elif spot_place == SPOT_MOVIE_THEATER and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_MIO_WINTER_MOVIE] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_MIO_WINTER_MOVIE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIO, 5)
				return true

		GIRL_YUINA:
			# Spring Zoo -> Scene 2
			if spot_place == SPOT_ZOO and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_YUINA_SPRING_ZOO] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_YUINA_SPRING_ZOO] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUINA, 2)
				return true
			# Summer Bowling -> Scene 3
			elif spot_place == SPOT_BOWLING_ALLEY and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_YUINA_SUMMER_BOWL] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_YUINA_SUMMER_BOWL] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUINA, 3)
				return true
			# Autumn Game Center -> Scene 4
			elif spot_place == SPOT_GAME_CENTER and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_YUINA_AUTUMN_GAME] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_YUINA_AUTUMN_GAME] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUINA, 4)
				return true
			# Winter Central Park -> Scene 5
			elif spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_YUINA_WINTER_PARK] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_YUINA_WINTER_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUINA, 5)
				return true

		GIRL_AYAKO:
			# Spring Karaoke -> Scene 2
			if spot_place == SPOT_KARAOKE and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_AYAKO_SPRING_KARAOKE] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_AYAKO_SPRING_KARAOKE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_AYAKO, 2)
				return true
			# Summer Art Museum -> Scene 3
			elif spot_place == SPOT_ART_MUSEUM and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_AYAKO_SUMMER_ART] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_AYAKO_SUMMER_ART] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_AYAKO, 3)
				return true
			# Autumn Art Museum -> Scene 4
			elif spot_place == SPOT_ART_MUSEUM and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_AYAKO_AUTUMN_ART] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_AYAKO_AUTUMN_ART] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_AYAKO, 4)
				return true
			# Winter Planetarium -> Scene 5
			elif spot_place == SPOT_PLANETARIUM and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_AYAKO_WINTER_PLANET] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_AYAKO_WINTER_PLANET] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_AYAKO, 5)
				return true

		GIRL_SAKI:
			# Spring Central Park -> Scene 2
			if spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_SAKI_SPRING_PARK] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_SAKI_SPRING_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SAKI, 2)
				return true
			# Summer Stadium -> Scene 3
			elif spot_place == SPOT_STADIUM and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_SAKI_SUMMER_STADIUM] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_SAKI_SUMMER_STADIUM] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SAKI, 3)
				return true
			# Autumn Stadium -> Scene 4
			elif spot_place == SPOT_STADIUM and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_SAKI_AUTUMN_STADIUM] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_SAKI_AUTUMN_STADIUM] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SAKI, 4)
				return true
			# Winter Shopping Street -> Scene 5
			elif spot_place == SPOT_SHOPPING_STREET and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_SAKI_WINTER_SHOP] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_SAKI_WINTER_SHOP] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_SAKI, 5)
				return true

		GIRL_YUKARI:
			# Spring Botanical Garden -> Scene 3
			if spot_place == SPOT_BOTANICAL_GARDEN and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_YUKARI_SPRING_BOTAN] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_YUKARI_SPRING_BOTAN] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKARI, 3)
				return true
			# Summer Aquarium -> Scene 4
			elif spot_place == SPOT_AQUARIUM and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_YUKARI_SUMMER_AQUA] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_YUKARI_SUMMER_AQUA] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKARI, 4)
				return true
			# Autumn Central Park -> Scene 5
			elif spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_YUKARI_AUTUMN_PARK] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_YUKARI_AUTUMN_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKARI, 5)
				return true
			# Winter Skating Rink -> Scene 6
			elif spot_place == SPOT_SKATING_RINK and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_YUKARI_WINTER_SKATE] == 0:
				active_scene_id = 6
				scene_flags[SCENE_FLAG_YUKARI_WINTER_SKATE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKARI, 6)
				return true

		GIRL_NOZOMI:
			# Spring Art Museum -> Scene 2
			if spot_place == SPOT_ART_MUSEUM and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_NOZOMI_SPRING_ART] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_NOZOMI_SPRING_ART] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_NOZOMI, 2)
				return true
			# Summer Beach -> Scene 3
			elif spot_place == SPOT_SEA and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_NOZOMI_SUMMER_SEA] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_NOZOMI_SUMMER_SEA] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_NOZOMI, 3)
				return true
			# Autumn Botanical Garden -> Scene 4
			elif spot_place == SPOT_BOTANICAL_GARDEN and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_NOZOMI_AUTUMN_BOTAN] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_NOZOMI_AUTUMN_BOTAN] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_NOZOMI, 4)
				return true
			# Winter Bowling -> Scene 5
			elif spot_place == SPOT_BOWLING_ALLEY and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_NOZOMI_WINTER_BOWL] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_NOZOMI_WINTER_BOWL] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_NOZOMI, 5)
				return true

		GIRL_MIRA:
			# Spring Shopping Street -> Scene 2
			if spot_place == SPOT_SHOPPING_STREET and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_MIRA_SPRING_SHOP] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_MIRA_SPRING_SHOP] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIRA, 2)
				return true
			# Summer Beach -> Scene 3
			elif spot_place == SPOT_SEA and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_MIRA_SUMMER_SEA] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_MIRA_SUMMER_SEA] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIRA, 3)
				return true
			# Autumn Amusement Park -> Scene 4
			elif spot_place == SPOT_AMUSEMENT_PARK and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_MIRA_AUTUMN_AMUSE] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_MIRA_AUTUMN_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIRA, 4)
				return true
			# Winter Skating Rink -> Scene 5
			elif spot_place == SPOT_SKATING_RINK and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_MIRA_WINTER_SKATE] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_MIRA_WINTER_SKATE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MIRA, 5)
				return true

		GIRL_YUKO:
			# Spring Game Center -> Scene 2
			if spot_place == SPOT_GAME_CENTER and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_YUKO_SPRING_GAME] == 0:
				active_scene_id = 2
				scene_flags[SCENE_FLAG_YUKO_SPRING_GAME] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKO, 2)
				return true
			# Summer Amusement Park -> Scene 3
			elif spot_place == SPOT_AMUSEMENT_PARK and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_YUKO_SUMMER_AMUSE] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_YUKO_SUMMER_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKO, 3)
				return true
			# Autumn Concert Hall -> Scene 4
			elif spot_place == SPOT_CONCERT_HALL and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_YUKO_AUTUMN_CONCERT] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_YUKO_AUTUMN_CONCERT] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKO, 4)
				return true
			# Winter Ski Resort -> Scene 5
			elif spot_place == SPOT_SKI_RESORT and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_YUKO_WINTER_SKI] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_YUKO_WINTER_SKI] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKO, 5)
				return true
			# December Library Date (place_scenes.c:471-483) -> Scene 6
			elif spot_place == SPOT_LIBRARY and month == 12 and day in [7, 8, 10] and scene_flags[SCENE_FLAG_YUKO_DEC_LIB_DATE] == 0:
				active_scene_id = 6
				scene_flags[SCENE_FLAG_YUKO_DEC_LIB_DATE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUKO, 6)
				return true

		GIRL_MEGUMI:
			# Spring Amusement Park -> Scene 3
			if spot_place == SPOT_AMUSEMENT_PARK and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_MEGUMI_SPRING_AMUSE] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_MEGUMI_SPRING_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MEGUMI, 3)
				return true
			# Summer Central Park -> Scene 4
			elif spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_MEGUMI_SUMMER_PARK] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_MEGUMI_SUMMER_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MEGUMI, 4)
				return true
			# Autumn Zoo -> Scene 5
			elif spot_place == SPOT_ZOO and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_MEGUMI_AUTUMN_ZOO] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_MEGUMI_AUTUMN_ZOO] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MEGUMI, 5)
				return true
			# Winter Ski Resort -> Scene 6
			elif spot_place == SPOT_SKI_RESORT and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_MEGUMI_WINTER_SKI] == 0:
				active_scene_id = 6
				scene_flags[SCENE_FLAG_MEGUMI_WINTER_SKI] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_MEGUMI, 6)
				return true

		GIRL_YUMI:
			# Spring Amusement Park -> Scene 3
			if spot_place == SPOT_AMUSEMENT_PARK and month >= 3 and month < 6 and scene_flags[SCENE_FLAG_YUMI_SPRING_AMUSE] == 0:
				active_scene_id = 3
				scene_flags[SCENE_FLAG_YUMI_SPRING_AMUSE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUMI, 3)
				return true
			# Summer Central Park -> Scene 4
			elif spot_place == SPOT_KIRAMEKI_CENTRAL_PARK and month >= 6 and month < 9 and scene_flags[SCENE_FLAG_YUMI_SUMMER_PARK] == 0:
				active_scene_id = 4
				scene_flags[SCENE_FLAG_YUMI_SUMMER_PARK] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUMI, 4)
				return true
			# Autumn Stadium -> Scene 5
			elif spot_place == SPOT_STADIUM and month >= 9 and month < 12 and scene_flags[SCENE_FLAG_YUMI_AUTUMN_STADIUM] == 0:
				active_scene_id = 5
				scene_flags[SCENE_FLAG_YUMI_AUTUMN_STADIUM] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUMI, 5)
				return true
			# Winter Skating Rink -> Scene 6
			elif spot_place == SPOT_SKATING_RINK and (month == 12 or month == 1 or month == 2) and scene_flags[SCENE_FLAG_YUMI_WINTER_SKATE] == 0:
				active_scene_id = 6
				scene_flags[SCENE_FLAG_YUMI_WINTER_SKATE] = 1
				stage = STAGE_SPECIAL_CG
				dispatch_girl_scene(GIRL_YUMI, 6)
				return true

		GIRL_REI:
			# Encounters / special scenes 1..3
			active_scene_id = 1
			stage = STAGE_SPECIAL_CG
			dispatch_girl_scene(GIRL_REI, 1)
			return true

		GIRL_YOSHIO:
			# Encounter scene 1
			active_scene_id = 1
			stage = STAGE_SPECIAL_CG
			dispatch_girl_scene(GIRL_YOSHIO, 1)
			return true

	return false

# ==============================================================================
# Amusement Park Ride Movies (ride_movies.c:40-58)
# ==============================================================================

## Triggers an amusement park ride movie (0: Roller Coaster, 1: Ghost House, 2: Ferris Wheel, 3: Scream Ride, 4: Virtual Ship)
func trigger_ride_movie(ride_id: int) -> Dictionary:
	spot_ride = clampi(ride_id, 0, 4)
	active_movie_lba = RIDE_MOVIE_LBAS.get(spot_ride, 0x4330)
	stage = STAGE_RIDE_MOVIE
	substep = 0
	step_timer = 0

	var ride_name := "Roller Coaster"
	match spot_ride:
		RIDE_ROLLER_COASTER:
			ride_name = "Roller Coaster"
			dialogue_lines = ["「キャーッ！速い〜！」", "「すごい迫力だったね！」"]
		RIDE_GHOST_HOUSE:
			ride_name = "Ghost House"
			dialogue_lines = ["「きゃっ！びっくりした…」", "「ちょっと怖かったかな」"]
		RIDE_FERRIS_WHEEL:
			ride_name = "Ferris Wheel"
			dialogue_lines = ["「わあ、遠くまで見えるよ！」", "「街が小さく見えるね」"]
		RIDE_SCREAM_RIDE:
			ride_name = "Scream Ride"
			dialogue_lines = ["「足がすくみそう…！」", "「でも楽しかったよ！」"]
		RIDE_VIRTUAL_SHIP:
			ride_name = "Virtual Ship"
			dialogue_lines = ["「本当に宇宙を飛んでるみたい！」", "「技術ってすごいね」"]

	dialogue_speaker = get_girl_short_name(current_girl)
	dialogue_choices = [
		"「すごく楽しかったね」",
		"「ちょっとドキドキしたよ」",
		"「もう乗るのはやめよう…」"
	]
	selected_choice = -1

	last_result = {
		"ride_id": spot_ride,
		"ride_name": ride_name,
		"lba": active_movie_lba,
		"streaming_mode": 1,
	}
	return last_result

# ==============================================================================
# Girl Scene Dispatch & Execution (girl_scene_entry.c:242-290)
# ==============================================================================

func dispatch_girl_scene(girl_id: int, scene_id: int) -> void:
	current_girl = clampi(girl_id, 0, 13)
	active_scene_id = scene_id
	substep = 0
	step_timer = 0
	selected_choice = -1
	dialogue_speaker = get_girl_short_name(current_girl)

	match current_girl:
		GIRL_SHIORI:
			_setup_shiori_scene(scene_id)
		GIRL_MIO:
			_setup_mio_scene(scene_id)
		GIRL_YUINA:
			_setup_yuina_scene(scene_id)
		GIRL_AYAKO:
			_setup_ayako_scene(scene_id)
		GIRL_SAKI:
			_setup_saki_scene(scene_id)
		GIRL_YUKARI:
			_setup_yukari_scene(scene_id)
		GIRL_NOZOMI:
			_setup_nozomi_scene(scene_id)
		GIRL_MIRA:
			_setup_mira_scene(scene_id)
		GIRL_YUKO:
			_setup_yuko_scene(scene_id)
		GIRL_MEGUMI:
			_setup_megumi_scene(scene_id)
		GIRL_YUMI:
			_setup_yumi_scene(scene_id)
		GIRL_MIHARU:
			_setup_miharu_scene(scene_id)
		GIRL_REI:
			_setup_rei_scene(scene_id)
		GIRL_YOSHIO:
			_setup_yoshio_scene(scene_id)

func _setup_shiori_scene(scene_id: int) -> void:
	match scene_id:
		4: # Spring Central Park Cherry Blossoms
			dialogue_lines = [
				"「きれいな桜ね…」",
				"「ふたりで来られて嬉しいな」"
			]
			dialogue_choices = [
				"「桜が満開だね」",
				"「本当にきれいだね」",
				"「花粉症が辛いよ」"
			]
		6: # Summer Pool
			dialogue_lines = [
				"「水が冷たくて気持ちいいね」",
				"「一緒に泳ごうよ！」"
			]
			dialogue_choices = [
				"「水着、すごく似合ってるよ」",
				"「よし、競争しよう！」",
				"「日焼け止め塗った？」"
			]
		3: # Autumn Park
			dialogue_lines = [
				"「紅葉が色づいてきれい…」",
				"「秋風が心地いいわね」"
			]
			dialogue_choices = [
				"「詩織と歩けて楽しいよ」",
				"「少し肌寒くなってきたね」",
				"「早く帰ろうか」"
			]
		5: # Winter Amusement Park
			dialogue_lines = [
				"「イルミネーションが素敵ね！」",
				"「夢の中にいるみたい」"
			]
			dialogue_choices = [
				"「手をつないで行こう」",
				"「本当に綺麗だね」",
				"「寒くてたまらないよ」"
			]
		_:
			dialogue_lines = ["「今日はいいお天気ね」", "「楽しみにしていたの」"]
			dialogue_choices = ["「楽しもうね」", "「そうだね」", "「疲れたな」"]

func _setup_mio_scene(scene_id: int) -> void:
	match scene_id:
		4: # Autumn Library
			dialogue_lines = [
				"「静かで落ち着きますね…」",
				"「おすすめの本があるんです」"
			]
			dialogue_choices = [
				"「未緒のおすすめなら読みたい」",
				"「本に詳しいんだね」",
				"「活字は苦手だな」"
			]
		_:
			dialogue_lines = ["「ご一緒できて光栄です…」", "「迷惑じゃないでしょうか」"]
			dialogue_choices = ["「嬉しいよ」", "「気にしないで」", "「少しね」"]

func _setup_yuina_scene(scene_id: int) -> void:
	match scene_id:
		4: # Autumn Game Center
			dialogue_lines = [
				"「ふふふ…我が科学力の実験台にしてくれる！」",
				"「このマシンをハッキング…いえ、攻略するわ！」"
			]
			dialogue_choices = [
				"「結奈のハイスコアを応援するよ」",
				"「壊さないでくれよ？」",
				"「怪しいこと言わないで」"
			]
		_:
			dialogue_lines = ["「ふん、付き合ってあげるわ」", "「私の計画に狂いはない」"]
			dialogue_choices = ["「頼もしいね」", "「よろしく」", "「怖いな…」"]

func _setup_ayako_scene(scene_id: int) -> void:
	match scene_id:
		2: # Spring Karaoke Box
			dialogue_lines = [
				"「ねえ、英語の歌歌っていい？」",
				"「一緒にデュエットしよう！」"
			]
			dialogue_choices = [
				"「よし、歌おう！」",
				"「彩子の歌を聴きたいな」",
				"「演歌でもいい？」"
			]
		_:
			dialogue_lines = ["「ハロー！楽しんでる？」", "「グッドな一日にしようね！」"]
			dialogue_choices = ["「最高だよ！」", "「うん」", "「うるさいな」"]

func _setup_saki_scene(scene_id: int) -> void:
	match scene_id:
		3: # Summer Stadium
			dialogue_lines = [
				"「がんばれー！ファイト！」",
				"「一生懸命な姿って元気もらえるよね！」"
			]
			dialogue_choices = [
				"「沙希の応援は最高だな」",
				"「一緒に盛り上がろう！」",
				"「大声出すと疲れるよ」"
			]
		_:
			dialogue_lines = ["「気合入れていこうね！」", "「根性だよ、根性！」"]
			dialogue_choices = ["「負けないぞ！」", "「そうだね」", "「マイペースでいくよ」"]

func _setup_yukari_scene(scene_id: int) -> void:
	match scene_id:
		3: # Spring Botanical Garden
			dialogue_lines = [
				"「まあ…かわいらしいお花たち…」",
				"「心が洗われるようでございます」"
			]
			dialogue_choices = [
				"「ゆかりさんにぴったりですね」",
				"「良い香りがしますね」",
				"「花より団子だな」"
			]
		_:
			dialogue_lines = ["「ご機嫌麗しゅうございます」", "「のんびり参りましょう」"]
			dialogue_choices = ["「光栄です」", "「はい」", "「歩くの遅いな」"]

func _setup_nozomi_scene(scene_id: int) -> void:
	match scene_id:
		3: # Summer Beach
			dialogue_lines = [
				"「波の音っていいよね！」",
				"「向こうの島まで泳いでみようか！」"
			]
			dialogue_choices = [
				"「望は本当にタフだね」",
				"「競争してみようか！」",
				"「溺れちゃうよ…」"
			]
		_:
			dialogue_lines = ["「今日も元気いっぱいだよ！」", "「いい汗流そう！」"]
			dialogue_choices = ["「爽やかだね」", "「うん」", "「運動はパス」"]

func _setup_mira_scene(scene_id: int) -> void:
	match scene_id:
		2: # Spring Shopping Street
			dialogue_lines = [
				"「美しさは日々のケアからよ」",
				"「あのお店、新作が入ったみたい」"
			]
			dialogue_choices = [
				"「魅羅さんはいつも輝いているね」",
				"「何でも似合いそうだね」",
				"「買い物長くなりそう…」"
			]
		_:
			dialogue_lines = ["「私をエスコートできるなんて幸運ね」", "「期待してるわよ」"]
			dialogue_choices = ["「光栄です」", "「頑張るよ」", "「偉そうだな」"]

func _setup_yuko_scene(scene_id: int) -> void:
	match scene_id:
		2: # Spring Game Center
			dialogue_lines = [
				"「よーし！格ゲーで勝負だよ！」",
				"「負けた方がジュースおごりね！」"
			]
			dialogue_choices = [
				"「手加減しないからな！」",
				"「夕子、うまいんだよな」",
				"「ゲームは苦手だよ」"
			]
		_:
			dialogue_lines = ["「あはは！超楽しいじゃん！」", "「次はどこ行こっか！」"]
			dialogue_choices = ["「行こう行こう！」", "「そうだね」", "「少し休もう」"]

func _setup_megumi_scene(scene_id: int) -> void:
	match scene_id:
		4: # Summer Central Park
			dialogue_lines = [
				"「あっ、あの子犬…かわいい…」",
				"「動物って癒されますね」"
			]
			dialogue_choices = [
				"「愛ちゃんみたいに優しいね」",
				"「なでてみようか」",
				"「噛まれるかもしれないよ」"
			]
		_:
			dialogue_lines = ["「あの…今日はありがとうございます」", "「ドキドキしちゃって…」"]
			dialogue_choices = ["「僕もだよ」", "「楽しもう」", "「落ち着いて」"]

func _setup_yumi_scene(scene_id: int) -> void:
	match scene_id:
		3: # Spring Amusement Park
			dialogue_lines = [
				"「先輩！あっちのアトラクション乗ろ！」",
				"「早く早くー！走っていこ！」"
			]
			dialogue_choices = [
				"「よし、走ろう！」",
				"「優美は元気いっぱいだな」",
				"「待って、急ぐなよ」"
			]
		_:
			dialogue_lines = ["「先輩と一緒で嬉しいな！」", "「子ども扱いしないでね！」"]
			dialogue_choices = ["「大人っぽいよ」", "「頼りにしてるよ」", "「まだまだ子どもだな」"]

func _setup_miharu_scene(scene_id: int) -> void:
	match scene_id:
		3: # Spring Park encounter
			dialogue_lines = [
				"「あ、ごめんなさい…って」",
				"「もしかして、デート中ですか？」"
			]
			dialogue_choices = [
				"「見晴ちゃん、偶然だね」",
				"「人違いじゃない？」",
				"「邪魔しないでくれ」"
			]
		5: # Spring Sea encounter
			dialogue_lines = [
				"「また会っちゃいましたね！」",
				"「これって運命の赤い糸かも…なんて！」"
			]
			dialogue_choices = [
				"「不思議な縁だね」",
				"「偶然って重なるね」",
				"「尾行でもしてるの？」"
			]
		_:
			dialogue_lines = ["「あ、見つかっちゃった！」", "「えへへ、こんにちは！」"]
			dialogue_choices = ["「こんにちは」", "「びっくりした」", "「誰？」"]

func _setup_rei_scene(scene_id: int) -> void:
	dialogue_lines = [
		"「フッ…庶民のデートスポットか」",
		"「少しは楽しませてくれるのだろうな」"
	]
	dialogue_choices = [
		"「庶民の楽しみを教えてあげるよ」",
		"「レイも楽しんでいってよ」",
		"「気に食わないなら帰っていいよ」"
	]

func _setup_yoshio_scene(scene_id: int) -> void:
	dialogue_lines = [
		"「よぉ相棒！デートの調子はどうだ？」",
		"「何か困ったら情報屋の俺に任せろ！」"
	]
	dialogue_choices = [
		"「好雄、邪魔するなよ！」",
		"「情報よろしく頼むよ」",
		"「誰かとデートじゃないのか？」"
	]

# ==============================================================================
# Dialogue Choice Handler & Stat Deltas
# ==============================================================================

func handle_choice(choice_idx: int) -> Dictionary:
	selected_choice = clampi(choice_idx, 0, mini(2, dialogue_choices.size() - 1))

	var aff_delta: int = 0
	var toki_delta: int = 0
	var syou_delta: int = 0

	match selected_choice:
		0: # Good choice
			aff_delta = 3
			toki_delta = 2
			syou_delta = -5
		1: # Neutral / okay choice
			aff_delta = 1
			toki_delta = 0
			syou_delta = 0
		2: # Bad / cold choice
			aff_delta = -2
			toki_delta = -1
			syou_delta = 5

	girls_affection[current_girl] = clampi(girls_affection[current_girl] + aff_delta, 0, 100)
	girls_tokimeki[current_girl] = clampi(girls_tokimeki[current_girl] + toki_delta, 0, 100)
	girls_syoushin[current_girl] = clampi(girls_syoushin[current_girl] + syou_delta, 0, 100)

	substep += 1

	last_result = {
		"choice_idx": selected_choice,
		"affection_delta": aff_delta,
		"tokimeki_delta": toki_delta,
		"syoushin_delta": syou_delta,
		"new_affection": girls_affection[current_girl],
		"new_tokimeki": girls_tokimeki[current_girl],
	}
	return last_result

func advance_step() -> void:
	substep += 1
	step_timer = 0
	if substep >= 3:
		stage = STAGE_EXIT

static func get_girl_short_name(girl_id: int) -> String:
	match girl_id:
		GIRL_SHIORI: return "詩織"
		GIRL_MIO: return "未緒"
		GIRL_YUINA: return "結奈"
		GIRL_AYAKO: return "彩子"
		GIRL_SAKI: return "沙希"
		GIRL_YUKARI: return "ゆかり"
		GIRL_NOZOMI: return "望"
		GIRL_MIRA: return "魅羅"
		GIRL_YUKO: return "夕子"
		GIRL_MEGUMI: return "愛"
		GIRL_YUMI: return "優美"
		GIRL_MIHARU: return "見晴"
		GIRL_REI: return "レイ"
		GIRL_YOSHIO: return "好雄"
		_: return ""

static func get_spot_name(spot_id: int) -> String:
	match spot_id:
		SPOT_NEIGHBORHOOD_PARK: return "近所の公園"
		SPOT_KIRAMEKI_CENTRAL_PARK: return "きらめき中央公園"
		SPOT_SHOPPING_STREET: return "ショッピング街"
		SPOT_AQUARIUM: return "水族館"
		SPOT_ZOO: return "動物園"
		SPOT_BOTANICAL_GARDEN: return "植物園"
		SPOT_PLANETARIUM: return "プラネタリウム"
		SPOT_ART_MUSEUM: return "美術館"
		SPOT_LIBRARY: return "図書館"
		SPOT_GAME_CENTER: return "ゲームセンター"
		SPOT_BOWLING_ALLEY: return "ボウリング場"
		SPOT_KARAOKE: return "カラオケBOX"
		SPOT_AMUSEMENT_PARK: return "遊園地"
		SPOT_STADIUM: return "スタジアム"
		SPOT_MOVIE_THEATER: return "映画館"
		SPOT_CONCERT_HALL: return "コンサートホール"
		SPOT_POOL: return "市民プール"
		SPOT_SEA: return "海"
		SPOT_SHRINE: return "神社"
		SPOT_SKATING_RINK: return "スケート場"
		SPOT_SKI_RESORT: return "スキー場"
		_: return "デートスポット"
