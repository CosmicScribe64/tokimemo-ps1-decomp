extends RefCounted
## T29030: native identities, not a gameplay completion or factory catalog.
## Existing bounded modes alone are bindable. Pending/preview records never
## expose callbacks or change original source eligibility.
const IMPLEMENTED_HANDLERS: Dictionary = {
  0x15: "src/ovl/name_entry/prologue.c::prologue_mode_handler",
  0x30: "src/main/scheduler.c::schedule_init",
  0x31: "src/main/scheduler.c::schedule_day_screen_handler",
  0x32: "src/main/scheduler.c::schedule_week_end_mode_handler",
  0x34: "src/main/home_screens.c::settings_command_screen_mode_handler",
  0x38: "src/main/scheduler.c::schedule_month_start_mode_handler",
  0x40: "src/main/school_day.c::week_day",
  0x46: "src/main/holiday.c::holiday_mode_handler"
}
const MODES: Dictionary = {
  0x0: {
    "symbol": "GAME_MODE_BOOT",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x1: {
    "symbol": "GAME_MODE_BOOT_WAIT",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x2: {
    "symbol": "GAME_MODE_OVERLAY_LOAD",
    "classification": "reviewed_pending",
    "bindable": false,
    "scope": "T29025 resident NAME_ENT loader subset; production composition pending",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x3: {
    "symbol": "GAME_MODE_CALL_ENTRY",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x10: {
    "symbol": "GAME_MODE_START",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x11: {
    "symbol": "GAME_MODE_OPENING",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38378,
    "entry": 2148813520
  },
  0x12: {
    "symbol": "GAME_MODE_TITLE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x13: {
    "symbol": "GAME_MODE_FILE_SELECT",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38378,
    "entry": 2148803356
  },
  0x14: {
    "symbol": "GAME_MODE_NAME_ENTRY",
    "classification": "reviewed_pending",
    "bindable": false,
    "scope": "T29025 entry/exit/context subset; production New Game composition pending",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38378,
    "entry": 2148737180
  },
  0x15: {
    "symbol": "GAME_MODE_PROLOGUE",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/ovl/name_entry/prologue.c::prologue_mode_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38378,
    "entry": 2148839208
  },
  0x20: {
    "symbol": "GAME_MODE_OPTION",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38762,
    "entry": 2148742632
  },
  0x21: {
    "symbol": "GAME_MODE_OPTION_2",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38762,
    "entry": 2148764064
  },
  0x22: {
    "symbol": "GAME_MODE_OPTION_3",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38762,
    "entry": 2148779904
  },
  0x30: {
    "symbol": "GAME_MODE_SCHEDULE",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/scheduler.c::schedule_init",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x31: {
    "symbol": "GAME_MODE_DAY_SCREEN",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/scheduler.c::schedule_day_screen_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x32: {
    "symbol": "GAME_MODE_WEEK_END",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/scheduler.c::schedule_week_end_mode_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x34: {
    "symbol": "GAME_MODE_COMMAND_SCREEN",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/home_screens.c::settings_command_screen_mode_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x35: {
    "symbol": "GAME_MODE_EXAM",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148820472
  },
  0x36: {
    "symbol": "GAME_MODE_CAREER_GUIDANCE",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148838352
  },
  0x37: {
    "symbol": "GAME_MODE_CAREER_CHOICE",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148831936
  },
  0x38: {
    "symbol": "GAME_MODE_MONTH_START",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/scheduler.c::schedule_month_start_mode_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x39: {
    "symbol": "GAME_MODE_DAY_NOTICE",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x3a: {
    "symbol": "GAME_MODE_HELP",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38570,
    "entry": 2148737024
  },
  0x40: {
    "symbol": "GAME_MODE_SCHOOL_DAY",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/school_day.c::week_day",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x41: {
    "symbol": "GAME_MODE_NEW_YEAR_PREP",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x42: {
    "symbol": "GAME_MODE_CHRISTMAS_PREP",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x43: {
    "symbol": "GAME_MODE_TELEPHONE",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39434,
    "entry": 2148767696
  },
  0x44: {
    "symbol": "GAME_MODE_RUMOR",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x45: {
    "symbol": "GAME_MODE_TELEPHONE_PROFILE",
    "classification": "reviewed_pending",
    "bindable": false,
    "scope": "T29020 first Yoshio/Shiori phone subset; services and actual route pending",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39434,
    "entry": 2148737024
  },
  0x46: {
    "symbol": "GAME_MODE_HOLIDAY",
    "classification": "implemented_subset",
    "bindable": true,
    "scope": "src/main/holiday.c::holiday_mode_handler",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0x50: {
    "symbol": "GAME_MODE_DATE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37418,
    "entry": 2148813012
  },
  0x51: {
    "symbol": "GAME_MODE_SCHOOL_TRIP",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39146,
    "entry": 2148758672
  },
  0x52: {
    "symbol": "GAME_MODE_NEW_YEAR_A",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39050,
    "entry": 2148782464
  },
  0x53: {
    "symbol": "GAME_MODE_NEW_YEAR_B",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39050,
    "entry": 2148737748
  },
  0x54: {
    "symbol": "GAME_MODE_VALENTINE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39626,
    "entry": 2148737652
  },
  0x55: {
    "symbol": "GAME_MODE_SEASONAL_PARTY",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39050,
    "entry": 2148775044
  },
  0x56: {
    "symbol": "GAME_MODE_PLAYER_BIRTHDAY",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37994,
    "entry": 2148745280
  },
  0x57: {
    "symbol": "GAME_MODE_GIRL_BIRTHDAY",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37994,
    "entry": 2148748432
  },
  0x58: {
    "symbol": "GAME_MODE_AMUSEMENT_PARK",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37514,
    "entry": 2148737556
  },
  0x59: {
    "symbol": "GAME_MODE_GIRL_ENCOUNTER",
    "classification": "reviewed_pending",
    "bindable": false,
    "scope": "T29020 bounded Shiori GEKO subset; original resources and actual route pending",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37994,
    "entry": 2148737604
  },
  0x5a: {
    "symbol": "GAME_MODE_GIRL_INVITATION",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37994,
    "entry": 2148804304
  },
  0x5b: {
    "symbol": "GAME_MODE_MEETING",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39050,
    "entry": 2148757828
  },
  0x5c: {
    "symbol": "GAME_MODE_AMUSEMENT_PARK_2",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37514,
    "entry": 2148761444
  },
  0x5d: {
    "symbol": "GAME_MODE_SPECIAL_EVENT",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37994,
    "entry": 2148763692
  },
  0x5e: {
    "symbol": "GAME_MODE_DATE_SCENE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37418,
    "entry": 2148843660
  },
  0x5f: {
    "symbol": "GAME_MODE_CLUB_CAMP",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38186,
    "entry": 2148741112
  },
  0x60: {
    "symbol": "GAME_MODE_CLUB_CAMP_2",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38186,
    "entry": 2148737024
  },
  0x61: {
    "symbol": "GAME_MODE_SICK_VISIT",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38666,
    "entry": 2148741408
  },
  0x62: {
    "symbol": "GAME_MODE_SICK_VISIT_2",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38666,
    "entry": 2148737024
  },
  0x63: {
    "symbol": "GAME_MODE_ENDING",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37610,
    "entry": 2148737844
  },
  0x70: {
    "symbol": "GAME_MODE_CULTURE_FESTIVAL_A",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37226,
    "entry": 2148804352
  },
  0x71: {
    "symbol": "GAME_MODE_CULTURE_FESTIVAL_B",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37130,
    "entry": 2148814704
  },
  0x72: {
    "symbol": "GAME_MODE_SPORTS_FESTIVAL",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39338,
    "entry": 2148799104
  },
  0x73: {
    "symbol": "GAME_MODE_PRACTICE_MATCH",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38858,
    "entry": 2148737088
  },
  0x74: {
    "symbol": "GAME_MODE_CLUB_MASTER_A",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38282,
    "entry": 2148743724
  },
  0x75: {
    "symbol": "GAME_MODE_CLUB_MASTER_B",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38282,
    "entry": 2148766032
  },
  0x76: {
    "symbol": "GAME_MODE_CLUB_MASTER_C",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38282,
    "entry": 2148737344
  },
  0x80: {
    "symbol": "GAME_MODE_CULTURE_STAGE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37322,
    "entry": 2148759696
  },
  0x81: {
    "symbol": "GAME_MODE_RPG_BATTLE",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 38954,
    "entry": 2148775344
  },
  0x82: {
    "symbol": "GAME_MODE_FESTIVAL_STALLS",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37706,
    "entry": 2148737024
  },
  0x90: {
    "symbol": "GAME_MODE_SHOOTER",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 39242,
    "entry": 2148737024
  },
  0x91: {
    "symbol": "GAME_MODE_ALBUM",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148796400
  },
  0xc2: {
    "symbol": "GAME_MODE_ETC_C2",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148804980
  },
  0xc3: {
    "symbol": "GAME_MODE_ETC_C3",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148819820
  },
  0xc4: {
    "symbol": "GAME_MODE_ETC_C4",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "lba": 37802,
    "entry": 2148817516
  },
  0xd0: {
    "symbol": "GAME_MODE_TT_GAME",
    "classification": "preview_only",
    "bindable": false,
    "scope": "diagnostic math/presentation only; native entry/caller/lifecycle not accepted",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch",
    "dedicated_loader": "src/main/game_mode.c::game_mode_tt_game_mode_handler; not an ordinary loader arm"
  },
  0xfe: {
    "symbol": "GAME_MODE_DEBUG_VRAM_VIEW",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  },
  0xff: {
    "symbol": "GAME_MODE_EXIT",
    "classification": "unsupported",
    "bindable": false,
    "scope": "no registered source implementation in this bounded runtime",
    "dispatch_source": "src/main/game_mode.c::game_mode_dispatch"
  }
}

static func describe(mode: int) -> Dictionary:
	return MODES.get(mode, {}).duplicate(true)

static func can_bind(mode: int) -> bool:
	return IMPLEMENTED_HANDLERS.has(mode)

static func all_native_ids() -> Array[int]:
	var result: Array[int] = []
	result.assign(MODES.keys())
	result.sort()
	return result
