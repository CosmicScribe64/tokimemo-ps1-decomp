extends RefCounted
## Native GameMode -> overlay disc file, from src/main/overlay_loader.c
## ovl_load_start (the sector switch) and include/game_enums.h. Every row is
## checked against the C switch by tools/port/test_overlay_mode_tables.py, and
## the LBA of each file name against the ISO directory when the disc is present.
##
## The dispatcher's catalog keys (OverlayDispatcher.GAME_MODE_*) are port-local
## numbers and several equal unrelated native modes (0x51, 0x52, 0x53, 0x57,
## 0x59, 0x5A, 0x5C). No production code reads this table today: only
## tests/rules/run_school_events_family_tests.gd and
## tools/port/test_overlay_mode_tables.py use it. OverlayDispatcher still calls
## DayEventModes.dispatcher_mode_for(mode) without the queue event in several
## places, and playable_boot._native_draw calls init_overlay without it, so a
## colliding native number reaching those paths gets its catalog meaning.

## Native mode -> [overlay file stem, LBA]. 0x90 (GAME_MODE_SHOOTER) is not a
## case label; it takes the switch's default arm.
const OVERLAY_BY_NATIVE_MODE: Dictionary = {
	0x11: ["NAME_ENT", 0x95EA], 0x13: ["NAME_ENT", 0x95EA],
	0x14: ["NAME_ENT", 0x95EA], 0x15: ["NAME_ENT", 0x95EA],
	0x20: ["OPTION", 0x976A], 0x21: ["OPTION", 0x976A], 0x22: ["OPTION", 0x976A],
	0x35: ["ETC", 0x93AA], 0x36: ["ETC", 0x93AA], 0x37: ["ETC", 0x93AA],
	0x91: ["ETC", 0x93AA], 0xC2: ["ETC", 0x93AA], 0xC3: ["ETC", 0x93AA],
	0xC4: ["ETC", 0x93AA],
	0x3A: ["OLH", 0x96AA],
	0x43: ["TEL", 0x9A0A], 0x45: ["TEL", 0x9A0A],
	0x50: ["DATE", 0x922A], 0x5E: ["DATE", 0x922A],
	0x51: ["SHUGAKU", 0x98EA],
	0x52: ["SHOUGATU", 0x988A], 0x53: ["SHOUGATU", 0x988A],
	0x55: ["SHOUGATU", 0x988A], 0x5B: ["SHOUGATU", 0x988A],
	0x54: ["VALEN", 0x9ACA],
	0x56: ["GEKO", 0x946A], 0x57: ["GEKO", 0x946A], 0x59: ["GEKO", 0x946A],
	0x5A: ["GEKO", 0x946A], 0x5D: ["GEKO", 0x946A],
	0x58: ["DATE2", 0x928A], 0x5C: ["DATE2", 0x928A],
	0x5F: ["KANGEI", 0x952A], 0x60: ["KANGEI", 0x952A],
	0x61: ["OMIMAI", 0x970A], 0x62: ["OMIMAI", 0x970A],
	0x63: ["ENDING", 0x92EA],
	0x70: ["BUNKASAI", 0x916A],
	0x71: ["BUNKAKEN", 0x910A],
	0x72: ["TAIIKU", 0x99AA],
	0x73: ["RENSYU", 0x97CA],
	0x74: ["MASTER", 0x958A], 0x75: ["MASTER", 0x958A], 0x76: ["MASTER", 0x958A],
	0x80: ["BUNKA_SD", 0x91CA],
	0x81: ["RPG_BAT", 0x982A],
	0x82: ["EN_NICHI", 0x934A],
	0x90: ["TACO", 0x994A],
	0xD0: ["TT", 0x9A6A],
}

## The default arm of the sector switch.
const DEFAULT_OVERLAY: Array = ["TACO", 0x994A]

## Overlay file stem -> OverlayDispatcher catalog key, for the overlays this
## family routes. Only overlays with exactly one catalog entry are listed.
const DISPATCHER_MODE_BY_OVERLAY: Dictionary = {
	"GEKO": 0x44,
	"TAIIKU": 0x47,
	"BUNKASAI": 0x48,
	"BUNKA_SD": 0x49,
	"BUNKAKEN": 0x4A,
	"OMIMAI": 0x4B,
	"SHUGAKU": 0x4C,
	"KANGEI": 0x4D,
	"SHOUGATU": 0x4E,
	"VALEN": 0x4F,
	"RENSYU": 0x51,
	"MASTER": 0x52,
	"EN_NICHI": 0x53,
	"RPG_BAT": 0x57,
	"DATE2": 0x58,
	"TACO": 0x59,
	"TT": 0x5A,
}


static func is_overlay_mode(native_mode: int) -> bool:
	return OVERLAY_BY_NATIVE_MODE.has(native_mode)


## The overlay file the loader reads for native_mode, or "" when the mode is
## main-resident (it never reaches ovl_load_start).
static func overlay_for(native_mode: int) -> String:
	if not OVERLAY_BY_NATIVE_MODE.has(native_mode):
		return ""
	return String(OVERLAY_BY_NATIVE_MODE[native_mode][0])


static func lba_for(native_mode: int) -> int:
	if not OVERLAY_BY_NATIVE_MODE.has(native_mode):
		return -1
	return int(OVERLAY_BY_NATIVE_MODE[native_mode][1])


## Catalog key for a native overlay mode, or -1 when the overlay is not in
## DISPATCHER_MODE_BY_OVERLAY (main-resident modes, or ETC/TEL/DATE/NAME_ENT/
## OPTION whose catalog rows are not one-per-overlay).
static func dispatcher_mode_for_native(native_mode: int) -> int:
	var overlay: String = overlay_for(native_mode)
	if overlay.is_empty():
		return -1
	return int(DISPATCHER_MODE_BY_OVERLAY.get(overlay, -1))
