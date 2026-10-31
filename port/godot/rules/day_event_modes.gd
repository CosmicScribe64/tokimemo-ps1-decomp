extends RefCounted
## scheduler.c::day_event_dispatch switch, include/game_enums.h.
## Maps DayEventType queue opcodes to OverlayDispatcher GameMode values.
## EVENT_MODES is checked against the C switch, and the event and native rows
## of the overlay families GEKO, SHUGAKU, SHOUGATU and KANGEI against
## OverlayLoadTable (overlay_loader.c), by tools/port/test_overlay_mode_tables.py.

const EVENT_MODES: Dictionary = {
	0x00: 0x40, 0x10: 0x46, 0xfe: 0x38, 0xfd: 0x39,
	0x20: 0x72, 0x25: 0x74, 0x26: 0x75, 0x27: 0x76,
	0x28: 0x42, 0x2c: 0x41, 0x30: 0x55, 0x31: 0x54,
	0x32: 0x57, 0x33: 0x56, 0x40: 0x35, 0x41: 0x51,
	0x42: 0x5f, 0x43: 0x36, 0x44: 0x59, 0x45: 0x5d,
	0x46: 0x5a, 0x47: 0x61, 0x48: 0x5b, 0x49: 0x58,
	0x4a: 0x37, 0x80: 0x50, 0xfc: 0x32, 0xff: 0x34,
}

## Queue opcodes 0x32/0x33 (birthdays, native 0x57/0x56), 0x44 (encounter, native 0x59),
## 0x45 (special event, native 0x5D) and 0x46 (girl invitation, native 0x5A) all run in
## the GEKO overlay (include/game_enums.h); the player's own call is TEL, never a queue
## event. 0x41 loads SHUGAKU, 0x42 KANGEI (native 0x5F), and 0x30/0x48 SHOUGATU (native
## 0x55/0x5B share LBA 0x988A in overlay_loader.c).
# Seasonal rows follow rules/seasonal_mode_identity.gd: 0xfd (native 0x39),
# 0x28 (native 0x42) and 0x2c (native 0x41) run in the main executable and have
# no overlay; birthdays 0x32/0x33 load GEKO (overlay_loader.c:275-282).
# Meeting 0x48 (native 0x5B GAME_MODE_MEETING) loads SHOUGATU at LBA 0x988A,
# entry D_80137144 (overlay_loader.c:290-296, 478-479).
const EVENT_TO_DISPATCHER_MODE: Dictionary = {
	0x00: 0x40, 0x10: 0x46, 0xfe: 0x38,
	0x20: 0x47, 0x24: 0x48, 0x25: 0x52, 0x26: 0x52, 0x27: 0x52,
	0x30: 0x4e, 0x31: 0x4f,
	0x32: 0x44, 0x33: 0x44, 0x40: 0x35, 0x41: 0x4c,
	0x42: 0x4d, 0x43: 0x3f, 0x44: 0x44, 0x45: 0x44,
	0x46: 0x44, 0x47: 0x4b, 0x48: 0x4e, 0x49: 0x58,
	0x4a: 0x3f, 0x80: 0x50, 0xfc: 0x32, 0xff: 0x34,
}

# Maps native GameMode enum values (include/game_enums.h) to OverlayDispatcher catalog mode IDs.
# The two id spaces overlap: dispatcher 0x57/0x59/0x5A/0x5C are RPG_BAT/TACO/TT/ENDING,
# while native 0x57/0x59/0x5A are GEKO modes and native 0x5C is DATE2. A native key
# maps by its include/game_enums.h entry, never by identity with a dispatcher id.
# Native 0x3A (OLH help) stays unmapped: its exit returns to the command screen
# (0x34), which _overlay_mode's day-queue completion does not do.
# Not yet re-checked against the enum: 0x36, 0x37 and 0x41..0x53.
# Native 0x39/0x41/0x42 are main-executable handlers bound by the school-day
# controller, not overlays. Native 0x52/0x53 (SHOUGATU Christmas/New Year) share
# numbers with the dispatcher's MASTER/EN_NICHI ids, so they cannot be keyed
# here; seasonal routing reads rules/seasonal_mode_identity.gd instead.
const NATIVE_TO_DISPATCHER_MODE: Dictionary = {
	0x01: 0x01, # GAME_MODE_OPENING
	0x35: 0x35, # GAME_MODE_EXAM
	0x36: 0x36, # GAME_MODE_OPTION
	0x37: 0x37, # GAME_MODE_OLH
	0x3D: 0x3D, # GAME_MODE_TEL
	0x3E: 0x3E, # GAME_MODE_TEL_ACTION
	0x3F: 0x3F, # GAME_MODE_ETC
	0x43: 0x3D, # GAME_MODE_TELEPHONE -> TEL
	0x45: 0x3E, # GAME_MODE_TELEPHONE_PROFILE -> TEL_ACTION
	0x47: 0x47, # GAME_MODE_TAIIKU
	0x48: 0x48, # GAME_MODE_BUNKASAI
	0x49: 0x49, # GAME_MODE_BUNKA_SD
	0x4A: 0x4A, # GAME_MODE_BUNKAKEN
	0x4B: 0x4B, # GAME_MODE_OMIMAI
	0x4C: 0x4C, # GAME_MODE_SHUGAKU
	0x4D: 0x4D, # GAME_MODE_KANGEI
	0x4E: 0x4E, # GAME_MODE_SHOUGATU
	0x4F: 0x4F, # GAME_MODE_VALEN
	0x50: 0x50, # GAME_MODE_DATE
	0x51: 0x51, # GAME_MODE_RENSYU
	0x52: 0x52, # GAME_MODE_MASTER
	0x53: 0x53, # GAME_MODE_EN_NICHI
	0x54: 0x4F, # GAME_MODE_VALENTINE -> VALEN
	0x55: 0x4E, # GAME_MODE_SEASONAL_PARTY -> SHOUGATU
	0x56: 0x44, # GAME_MODE_PLAYER_BIRTHDAY (GEKO 0x80134040) -> GEKO
	0x57: 0x44, # GAME_MODE_GIRL_BIRTHDAY (GEKO D_80134C90, queue 0x32) -> GEKO, not RPG_BAT
	0x58: 0x58, # GAME_MODE_AMUSEMENT_PARK_DATE
	0x59: 0x44, # GAME_MODE_GIRL_ENCOUNTER (GEKO D_80132244, queue 0x44) -> GEKO, not TACO (native TACO is 0x90)
	0x5A: 0x44, # GAME_MODE_GIRL_INVITATION (GEKO 0x801426D0) -> GEKO, not TT (native TT is 0xD0)
	0x5B: 0x4E, # GAME_MODE_MEETING (SHOUGATU LBA 0x988A, D_80137144) -> SHOUGATU
	# Native 0x5C is AMUSEMENT_PARK_2 (DATE2 0x80137F64), but SchoolDayController's graduation
	# still enters 0x5C instead of GAME_MODE_ENDING 0x63 (scheduler.c:1559), so this row keeps
	# the ending until that caller moves to 0x63.
	0x5C: 0x5C, # ENDING (see above)
	0x5D: 0x44, # GAME_MODE_SPECIAL_EVENT -> GEKO (0x8013882C)
	0x5E: 0x5E, # GAME_MODE_EVENT_CG
	0x5F: 0x4D, # GAME_MODE_CLUB_CAMP (KANGEI overlay) -> KANGEI
	0x60: 0x4D, # GAME_MODE_CLUB_CAMP_2 (KANGEI overlay) -> KANGEI
	0x61: 0x4B, # GAME_MODE_SICK_VISIT -> OMIMAI
	0x62: 0x4B, # GAME_MODE_SICK_VISIT_2 (OMIMAI 0x80132000) -> OMIMAI
	0x63: 0x5C, # GAME_MODE_ENDING (enum) -> ENDING
	0x70: 0x48, # GAME_MODE_CULTURE_FESTIVAL_A -> BUNKASAI
	0x71: 0x4A, # GAME_MODE_CULTURE_FESTIVAL_B -> BUNKAKEN
	0x72: 0x47, # GAME_MODE_SPORTS_FESTIVAL -> TAIIKU
	0x73: 0x51, # GAME_MODE_PRACTICE_MATCH -> RENSYU
	0x74: 0x52, # GAME_MODE_CLUB_MASTER_A -> MASTER
	0x75: 0x52, # GAME_MODE_CLUB_MASTER_B -> MASTER
	0x76: 0x52, # GAME_MODE_CLUB_MASTER_C -> MASTER
	0x80: 0x49, # GAME_MODE_CULTURE_STAGE -> BUNKA_SD
	0x81: 0x57, # GAME_MODE_RPG_BATTLE -> RPG_BATTLE
	0x82: 0x53, # GAME_MODE_FESTIVAL_STALLS -> EN_NICHI
	0x90: 0x59, # GAME_MODE_SHOOTER -> TACO
	0xC1: 0x5C, # EndingOverlay.GAME_MODE_ENDING -> ENDING
	0xC2: 0x3F, # EndingOverlay.GAME_MODE_ETC_C2 -> ETC
	0xD0: 0x5A, # GAME_MODE_TT_GAME -> TT
}

static func mode_for(event_type: int, club_id: int) -> int:
	if event_type == 0x24:
		return 0x70 if club_id < 6 else 0x71
	return int(EVENT_MODES.get(event_type, 0x34))

static func dispatcher_mode_for(native_mode: int, event_type: int = -1) -> int:
	if NATIVE_TO_DISPATCHER_MODE.has(native_mode) and native_mode in [0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x80, 0x81, 0x82, 0x90, 0xC1, 0xC2, 0xD0, 0x61, 0x63, 0x5F, 0x60, 0x62]:
		return int(NATIVE_TO_DISPATCHER_MODE[native_mode])
	# The event row decides only for the native mode that event sets; a caller
	# that passes a catalog key with an unrelated event keeps the catalog key.
	if event_type >= 0 and EVENT_TO_DISPATCHER_MODE.has(event_type) and _event_sets_mode(event_type, native_mode):
		return int(EVENT_TO_DISPATCHER_MODE[event_type])
	if NATIVE_TO_DISPATCHER_MODE.has(native_mode):
		return int(NATIVE_TO_DISPATCHER_MODE[native_mode])
	return native_mode

static func _event_sets_mode(event_type: int, native_mode: int) -> bool:
	if event_type == 0x24:
		return native_mode in [0x70, 0x71]
	return int(EVENT_MODES.get(event_type, -1)) == native_mode

static func all_overlay_native_modes() -> Array[int]:
	var modes: Array[int] = []
	for key: Variant in NATIVE_TO_DISPATCHER_MODE.keys():
		modes.append(int(key))
	return modes

