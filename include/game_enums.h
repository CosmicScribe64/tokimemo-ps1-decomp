#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

/* Enums and masks of the shared game core (T-27004, values from config/renames/proposals/core.tsv, wiki/spec/core.md
 * section 11). A field or variable keeps its fixed-width type, because an enum type would be 4 bytes and signed; the
 * field carries a comment naming the enum (CODING_STANDARDS section 4). */

/* T-27004 core.tsv GirlId */
enum GirlId {
    GIRL_SHIORI = 0,
    GIRL_MIO = 1,
    GIRL_YUINA = 2,
    GIRL_AYAKO = 3,
    GIRL_SAKI = 4,
    GIRL_YUKARI = 5,
    GIRL_NOZOMI = 6,
    GIRL_MIRA = 7,
    GIRL_YUKO = 8,
    GIRL_MEGUMI = 9, /* name table entry 9 (given name of the Mikihara entry) */
    GIRL_YUMI = 10,
    GIRL_MIHARU = 11,
    GIRL_REI = 12, /* name table entry 12 (the rival) */
    GIRL_YOSHIO = 13, /* name table entry 13 (friend, information giver) */
    GIRL_SOTOI = 14, /* name table entry 14 (butler) */
    GIRL_PLAYER = 15
};

/* T-27004 core.tsv Parameter */
enum Parameter {
    PARAM_CONDITION = 0, /* table 0x800B32D0 entry 0, GameState.param_condition */
    PARAM_LIBERAL_ARTS = 1,
    PARAM_SCIENCE = 2,
    PARAM_ART = 3,
    PARAM_ATHLETICS = 4,
    PARAM_TRIVIA = 5,
    PARAM_LOOKS = 6,
    PARAM_GUTS = 7,
    PARAM_STRESS = 8
};

/* T-27004 core.tsv Weekday */
enum Weekday {
    WEEKDAY_MONDAY = 0,
    WEEKDAY_TUESDAY = 1,
    WEEKDAY_WEDNESDAY = 2,
    WEEKDAY_THURSDAY = 3,
    WEEKDAY_FRIDAY = 4,
    WEEKDAY_SATURDAY = 5,
    WEEKDAY_SUNDAY = 6
};

/* T-27004 core.tsv Club */
enum Club {
    CLUB_LITERATURE = 0,
    CLUB_DRAMA = 1,
    CLUB_SCIENCE = 2,
    CLUB_COMPUTER = 3,
    CLUB_ART = 4,
    CLUB_BRASS_BAND = 5,
    CLUB_BASEBALL = 6,
    CLUB_SOCCER = 7,
    CLUB_TENNIS = 8,
    CLUB_SWIMMING = 9,
    CLUB_BASKETBALL = 10,
    CLUB_NONE = 11
};

/* T-27004 core.tsv Place */
enum Place {
    PLACE_NEIGHBORHOOD_PARK = 0,
    PLACE_KIRAMEKI_CENTRAL_PARK = 1,
    PLACE_SHOPPING_STREET = 2,
    PLACE_AQUARIUM = 3,
    PLACE_ZOO = 4,
    PLACE_BOTANICAL_GARDEN = 5,
    PLACE_PLANETARIUM = 6,
    PLACE_ART_MUSEUM = 7,
    PLACE_LIBRARY = 8,
    PLACE_GAME_CENTER = 9,
    PLACE_BOWLING_ALLEY = 10,
    PLACE_KARAOKE = 11,
    PLACE_AMUSEMENT_PARK = 12,
    PLACE_STADIUM = 13,
    PLACE_MOVIE_THEATER = 14,
    PLACE_CONCERT_HALL = 15,
    PLACE_POOL = 16,
    PLACE_SEA = 17,
    PLACE_SHRINE = 18,
    PLACE_SKATING_RINK = 19,
    PLACE_SKI_RESORT = 20
};

/* T-27004 core.tsv HealthState */
enum HealthState {
    HEALTH_OK = 0, /* player_flags bits 8-11 clear */
    HEALTH_INJURED = 1,
    HEALTH_NEUROSIS = 2,
    HEALTH_SICK = 3
};

/* T-27004 core.tsv Vacation */
enum Vacation {
    VACATION_NONE = 0, /* school term */
    VACATION_SUMMER = 1,
    VACATION_WINTER = 2, /* month 12 announcement */
    VACATION_SPRING = 3 /* month 3 announcement */
};

/* T-27004 core.tsv DayKind */
enum DayKind {
    DAY_KIND_SCHOOL_DAY = 0, /* month_calendar.day_kind */
    DAY_KIND_SUNDAY = 1, /* initial table: Sundays are 1 */
    DAY_KIND_HOLIDAY = 2, /* initial table: national holidays (April 29, May 3-5) are 2 */
    DAY_KIND_VACATION_DAY = 3 /* HYPOTHESIS: break weekday */
};

/* T-27004 core.tsv CalendarMarker */
enum CalendarMarker {
    MARKER_NONE = 0, /* month_calendar.marker[day] */
    MARKER_START_DAY = 1, /* HYPOTHESIS: set once on the first day (1995-04-04) */
    MARKER_NOTICE_A = 2, /* HYPOTHESIS: pushes a day-notice record */
    MARKER_SPORTS_FESTIVAL = 4, /* pushes DAY_EVENT_SPORTS_FESTIVAL */
    MARKER_EXAM = 5, /* exam period: school days become DAY_EVENT_EXAM */
    MARKER_NOTICE_B = 6, /* HYPOTHESIS: day-notice record */
    MARKER_CLUB_SEASON = 8, /* HYPOTHESIS: next 6 days become club tournament or camp records */
    MARKER_NOTICE_C = 10, /* HYPOTHESIS: day-notice record */
    MARKER_SCHOOL_TRIP = 11, /* pushes DAY_EVENT_SCHOOL_TRIP and a holiday week */
    MARKER_CULTURE_FESTIVAL = 12, /* pushes DAY_EVENT_CULTURE_FESTIVAL */
    MARKER_CHRISTMAS_PREP = 13, /* pushes DAY_EVENT_CHRISTMAS_PREP */
    MARKER_NOTICE_D = 14, /* HYPOTHESIS: day-notice record */
    MARKER_NEW_YEAR_PREP = 15, /* pushes DAY_EVENT_NEW_YEAR_PREP */
    MARKER_NOTICE_E = 16, /* HYPOTHESIS: day-notice record */
    MARKER_VALENTINE = 17, /* pushes DAY_EVENT_VALENTINE */
    MARKER_CAREER_CHOICE = 18, /* year 98 only: DAY_EVENT_CAREER_CHOICE */
    MARKER_SEASONAL_PARTY = 19, /* pushes DAY_EVENT_SEASONAL_PARTY */
    MARKER_NOTICE_F = 20 /* HYPOTHESIS: day-notice record */
};

/* T-27004 core.tsv DayEventType */
enum DayEventType {
    DAY_EVENT_SCHOOL_DAY = 0x00, /* -> mode 0x40 */
    DAY_EVENT_HOLIDAY = 0x10, /* -> mode 0x46 */
    DAY_EVENT_SPORTS_FESTIVAL = 0x20, /* -> mode 0x72 (TAIIKU) */
    DAY_EVENT_CULTURE_FESTIVAL = 0x24,
    DAY_EVENT_CLUB_MASTER_A = 0x25, /* HYPOTHESIS: club tournament scene, mode 0x74 */
    DAY_EVENT_CLUB_MASTER_B = 0x26, /* HYPOTHESIS: -> mode 0x75 */
    DAY_EVENT_CLUB_MASTER_C = 0x27, /* HYPOTHESIS: -> mode 0x76 */
    DAY_EVENT_CHRISTMAS_PREP = 0x28, /* -> mode 0x42 (pre_xmas) */
    DAY_EVENT_NEW_YEAR_PREP = 0x2C, /* -> mode 0x41 (pre_syogatu) */
    DAY_EVENT_SEASONAL_PARTY = 0x30, /* HYPOTHESIS: name; English reading of the overlay prefix */
    DAY_EVENT_VALENTINE = 0x31, /* -> mode 0x54 (VALEN) */
    DAY_EVENT_GIRL_BIRTHDAY = 0x32, /* -> mode 0x57 (GEKO) */
    DAY_EVENT_PLAYER_BIRTHDAY = 0x33, /* -> mode 0x56 (GEKO) */
    DAY_EVENT_EXAM = 0x40, /* -> mode 0x35 (ETC) */
    DAY_EVENT_SCHOOL_TRIP = 0x41, /* -> mode 0x51 (SHUGAKU) */
    DAY_EVENT_CLUB_CAMP = 0x42, /* HYPOTHESIS: name; the KANGEI overlay is rated low confidence */
    DAY_EVENT_CAREER_GUIDANCE = 0x43, /* -> mode 0x36 (ETC) */
    DAY_EVENT_GIRL_ENCOUNTER = 0x44, /* HYPOTHESIS: name; placeholder resolved by the scheduler */
    DAY_EVENT_SPECIAL_EVENT = 0x45, /* unk_01 girl, unk_02 scene -> mode 0x5D */
    DAY_EVENT_GIRL_INVITATION = 0x46, /* -> mode 0x5A (date_sasoi_judge: a girl invites the player) */
    DAY_EVENT_SICK_VISIT = 0x47, /* -> mode 0x61 (OMIMAI) */
    DAY_EVENT_MEETING = 0x48, /* HYPOTHESIS: name; mode 0x5B hosts generic scenes */
    DAY_EVENT_AMUSEMENT_PARK = 0x49, /* -> mode 0x58 (DATE2) */
    DAY_EVENT_CAREER_CHOICE = 0x4A, /* -> mode 0x37 (ETC) */
    DAY_EVENT_APPOINTMENT = 0x80, /* written by appointment_launch (unk_01 girl, unk_02 place) */
    DAY_EVENT_SKIP = 0xFA, /* index skipped by the queue reader */
    DAY_EVENT_DAY_END = 0xFB,
    DAY_EVENT_WEEK_END = 0xFC,
    DAY_EVENT_DAY_NOTICE = 0xFD, /* HYPOTHESIS: announcement screen */
    DAY_EVENT_MONTH_START = 0xFE,
    DAY_EVENT_NONE = 0xFF /* cleared slot = end of the queue */
};

/* T-27004 core.tsv GameMode */
enum GameMode {
    GAME_MODE_BOOT = 0x00,
    GAME_MODE_BOOT_WAIT = 0x01, /* HYPOTHESIS: waits for boot_hold, then mode 0 */
    GAME_MODE_OVERLAY_LOAD = 0x02,
    GAME_MODE_CALL_ENTRY = 0x03,
    GAME_MODE_START = 0x10, /* initial mode set by game_state_init */
    GAME_MODE_OPENING = 0x11, /* NAME_ENT entry 0x80144AD0: attract picture story and opening movie */
    GAME_MODE_TITLE = 0x12,
    GAME_MODE_FILE_SELECT = 0x13, /* NAME_ENT entry 0x8014231C: memory-card, system-file and album screens */
    GAME_MODE_NAME_ENTRY = 0x14, /* NAME_ENT 0x8013209C */
    GAME_MODE_PROLOGUE = 0x15, /* NAME_ENT 0x8014AF28 */
    GAME_MODE_OPTION = 0x20, /* OPTION 0x801335E8 */
    GAME_MODE_OPTION_2 = 0x21, /* OPTION 0x801389A0 */
    GAME_MODE_OPTION_3 = 0x22, /* OPTION 0x8013C780 */
    GAME_MODE_SCHEDULE = 0x30,
    GAME_MODE_DAY_SCREEN = 0x31,
    GAME_MODE_WEEK_END = 0x32,
    GAME_MODE_COMMAND_SCREEN = 0x34,
    GAME_MODE_EXAM = 0x35, /* ETC 0x801465F8: final exams and rank list */
    GAME_MODE_CAREER_GUIDANCE = 0x36, /* ETC 0x8014ABD0 */
    GAME_MODE_CAREER_CHOICE = 0x37, /* ETC 0x801492C0: job or college choice */
    GAME_MODE_MONTH_START = 0x38,
    GAME_MODE_DAY_NOTICE = 0x39,
    GAME_MODE_HELP = 0x3A, /* OLH on-line help */
    GAME_MODE_SCHOOL_DAY = 0x40, /* week_day: school-day command screen */
    GAME_MODE_NEW_YEAR_PREP = 0x41,
    GAME_MODE_CHRISTMAS_PREP = 0x42,
    GAME_MODE_TELEPHONE = 0x43, /* TEL 0x801397D0 */
    GAME_MODE_RUMOR = 0x44, /* uwasa_main (rumor report) */
    GAME_MODE_TELEPHONE_PROFILE = 0x45, /* HYPOTHESIS: profile and phone book page of TEL */
    GAME_MODE_HOLIDAY = 0x46,
    GAME_MODE_DATE = 0x50, /* DATE 0x801448D4 (appointment launched) */
    GAME_MODE_SCHOOL_TRIP = 0x51, /* SHUGAKU 0x80137490 */
    GAME_MODE_NEW_YEAR_A = 0x52, /* HYPOTHESIS: name; English form of the overlay prefix */
    GAME_MODE_NEW_YEAR_B = 0x53, /* HYPOTHESIS: name; English form of the overlay prefix */
    GAME_MODE_VALENTINE = 0x54, /* VALEN 0x80132274 */
    GAME_MODE_SEASONAL_PARTY = 0x55, /* HYPOTHESIS: name; queue type 0x30 is the seasonal party */
    GAME_MODE_PLAYER_BIRTHDAY = 0x56, /* GEKO 0x80134040 (queue type 0x33) */
    GAME_MODE_GIRL_BIRTHDAY = 0x57, /* GEKO 0x80134C90 (queue type 0x32) */
    GAME_MODE_AMUSEMENT_PARK = 0x58, /* DATE2 0x80132214 (queue type 0x49) */
    GAME_MODE_GIRL_ENCOUNTER = 0x59, /* HYPOTHESIS: name; reading of the forced-encounter use */
    GAME_MODE_GIRL_INVITATION = 0x5A, /* GEKO 0x801426D0 (queue type 0x46) */
    GAME_MODE_MEETING = 0x5B, /* HYPOTHESIS: name; generic scene host of SHOUGATU */
    GAME_MODE_AMUSEMENT_PARK_2 = 0x5C, /* DATE2 0x80137F64 */
    GAME_MODE_SPECIAL_EVENT = 0x5D, /* GEKO 0x8013882C (queue type 0x45) */
    GAME_MODE_DATE_SCENE = 0x5E, /* DATE 0x8014C08C: girl-specific scenes */
    GAME_MODE_CLUB_CAMP = 0x5F, /* HYPOTHESIS: name; KANGEI overlay, queue type 0x42 */
    GAME_MODE_CLUB_CAMP_2 = 0x60, /* HYPOTHESIS: name; KANGEI overlay */
    GAME_MODE_SICK_VISIT = 0x61, /* OMIMAI 0x80133120 (queue type 0x47) */
    GAME_MODE_SICK_VISIT_2 = 0x62, /* OMIMAI 0x80132000 */
    GAME_MODE_ENDING = 0x63, /* ENDING 0x80132334 */
    GAME_MODE_CULTURE_FESTIVAL_A = 0x70, /* BUNKASAI 0x80142700 (queue type 0x24, club id < 6) */
    GAME_MODE_CULTURE_FESTIVAL_B = 0x71, /* BUNKAKEN 0x80144F70 (queue type 0x24, club id >= 6) */
    GAME_MODE_SPORTS_FESTIVAL = 0x72, /* TAIIKU 0x80141280 */
    GAME_MODE_PRACTICE_MATCH = 0x73, /* RENSYU 0x80132040 */
    GAME_MODE_CLUB_MASTER_A = 0x74, /* MASTER entry 0x80133A2C (queue type 0x25) */
    GAME_MODE_CLUB_MASTER_B = 0x75, /* MASTER entry 0x80139150 (queue type 0x26) */
    GAME_MODE_CLUB_MASTER_C = 0x76, /* MASTER entry 0x80132140 (queue type 0x27) */
    GAME_MODE_CULTURE_STAGE = 0x80, /* BUNKA_SD 0x80137890 */
    GAME_MODE_RPG_BATTLE = 0x81, /* RPG_BAT 0x8013B5B0 */
    GAME_MODE_FESTIVAL_STALLS = 0x82, /* HYPOTHESIS: name; the overlay prefix EN_NICHI stays untranslated */
    GAME_MODE_SHOOTER = 0x90, /* TACO (loader default) */
    GAME_MODE_ALBUM = 0x91, /* ETC 0x801407F0 */
    GAME_MODE_ETC_C2 = 0xC2, /* ETC 0x80142974 */
    GAME_MODE_ETC_C3 = 0xC3, /* ETC 0x8014636C */
    GAME_MODE_ETC_C4 = 0xC4, /* ETC 0x80145A6C */
    GAME_MODE_TT_GAME = 0xD0, /* TT hidden mini game */
    GAME_MODE_DEBUG_VRAM_VIEW = 0xFE,
    GAME_MODE_EXIT = 0xFF /* exit_function */
};

/* T-27004 core.tsv InputDevice */
enum InputDevice {
    INPUT_MOUSE = 0, /* controller id byte high nibble 1 */
    INPUT_PAD = 1, /* other id */
    INPUT_NONE = 0xFF /* nothing connected */
};

/* T-27004 core.tsv VoiceMode */
enum VoiceMode {
    VOICE_OFF = 0,
    VOICE_ON = 1,
    VOICE_CANCELABLE = 2
};

/* T-27004 core.tsv MenuMode */
enum MenuMode {
    MENU_MODE_MOUSE_LIKE = 0, /* stored value 0 */
    MENU_MODE_A = 1, /* stored value 1 */
    MENU_MODE_MOUSE_PLUS_A = 2, /* stored value 2 */
    MENU_MODE_B = 3 /* stored value 3 */
};

/* T-27004 core.tsv IconMode */
enum IconMode {
    ICON_MODE_TYPE_A = 0,
    ICON_MODE_TYPE_B = 1,
    ICON_MODE_FREE = 2
};

/* T-27004 core.tsv PadBits */
#define PAD_L2 0x0001
#define PAD_R2 0x0002
#define PAD_L1 0x0004
#define PAD_R1 0x0008
#define PAD_TRIANGLE 0x0010
#define PAD_CIRCLE 0x0020 /* confirm: settings screens act on pad_pressed & 0x20 */
#define PAD_CROSS 0x0040 /* cancel/back: pad_pressed & 0x40 leaves a settings screen */
#define PAD_SQUARE 0x0080
#define PAD_SELECT 0x0100 /* pad_pressed & 0x100 steps menu_mode in menu_check */
#define PAD_START 0x0800 /* pause toggle on either pad (while pause_allowed is 1) */
#define PAD_UP 0x1000 /* menu cursor: up and left move to the previous item */
#define PAD_RIGHT 0x2000
#define PAD_DOWN 0x4000
#define PAD_LEFT 0x8000 /* previous item */

/* T-27004 core.tsv MouseButtons */
#define MOUSE_RIGHT 4 /* GameState.mouse_buttons tested with confirm_button == 1 */
#define MOUSE_LEFT 8 /* tested with confirm_button == 0 */

/* T-27004 core.tsv PlayerFlag */
#define PLAYER_FLAG_16 0x00010000 /* ScheduleFlags.b16 */
#define PLAYER_FLAG_AMUSEMENT_PARK_DONE 0x00040000 /* HYPOTHESIS: set when queue type 0x49 is scheduled (August Wednesday) */
#define PLAYER_FLAG_PLACE_REPEAT 0x00080000 /* HYPOTHESIS: DATE sets it when the same place is chosen repeatedly */

/* T-27056 R4 weekday command IDs; fixed-width storage retained. */
enum SchoolDayCommand {
    SCHOOL_DAY_COMMAND_LIBERAL_ARTS = 0, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_SCIENCE = 1, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_ART = 2, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_ATHLETICS = 3, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_CLUB = 4, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_TRIVIA = 5, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_LOOKS = 6, /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
    SCHOOL_DAY_COMMAND_REST = 7 /* Weekday command, queue girl byte and last_command low nibble; m-weekday contract. */
};

#endif
