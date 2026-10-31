extends RefCounted
## Native identity of the seasonal-event modes, from the source switches:
## scheduler.c::day_event_dispatch (queue type -> native mode),
## overlay_loader.c::ovl_load_start (native mode -> overlay LBA) and
## overlay_loader.c::ovl_load_enter (native mode -> entry address).
## The dispatcher's catalog ids share numbers with native modes (0x52, 0x53,
## 0x57), so seasonal routing must use this table, never a bare mode number.

## Queue type -> native mode (day_event_dispatch, scheduler.c:1840-1890).
const EVENT_NATIVE_MODE: Dictionary = {
	0xfd: 0x39, # DAY_NOTICE -> schedule_day_notice_mode_handler
	0x20: 0x72, # sports festival -> TAIIKU
	0x28: 0x42, # Christmas prep (main executable)
	0x2c: 0x41, # New Year prep (main executable)
	0x30: 0x55, # seasonal party -> SHOUGATU
	0x31: 0x54, # Valentine -> VALEN
}

## Native modes the main executable handles itself (no overlay load).
const MAIN_EXECUTABLE_HANDLERS: Dictionary = {
	0x39: "scheduler.c::schedule_day_notice_mode_handler",
	0x41: "home_screens.c::settings_new_year_prep_mode_handler",
	0x42: "home_screens.c::settings_christmas_prep_mode_handler",
}

## Native overlay mode -> {overlay, lba, sectors, entry}.
const OVERLAY_ENTRIES: Dictionary = {
	0x52: {"overlay": "SHOUGATU", "lba": 0x988A, "sectors": 0x60, "entry": 0x8013D180},
	0x53: {"overlay": "SHOUGATU", "lba": 0x988A, "sectors": 0x60, "entry": 0x801322D4},
	0x54: {"overlay": "VALEN", "lba": 0x9ACA, "sectors": 0x60, "entry": 0x80132274},
	0x55: {"overlay": "SHOUGATU", "lba": 0x988A, "sectors": 0x60, "entry": 0x8013B484},
	0x5B: {"overlay": "SHOUGATU", "lba": 0x988A, "sectors": 0x60, "entry": 0x80137144},
	0x72: {"overlay": "TAIIKU", "lba": 0x99AA, "sectors": 0x60, "entry": 0x80141280},
	0x82: {"overlay": "EN_NICHI", "lba": 0x934A, "sectors": 0x60, "entry": 0x80132000},
}

static func is_main_executable(native_mode: int) -> bool:
	return MAIN_EXECUTABLE_HANDLERS.has(native_mode)

static func overlay_entry(native_mode: int) -> Dictionary:
	return OVERLAY_ENTRIES.get(native_mode, {})
