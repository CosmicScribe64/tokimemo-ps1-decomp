extends RefCounted
## Native mode 0x39 (GAME_MODE_DAY_NOTICE), main executable:
## scheduler.c::schedule_day_notice_mode_handler and
## schedule_vacation_notice_step (src/main/scheduler.c:2073-2153).
## Queue type 0xFD reaches this mode from day_event_dispatch after
## girl_flags_bit0_clear. This is the only source writer of game_state.vacation
## on the school-day route; calendar_advance_day never touches it.

const VACATION_NONE: int = 0
const VACATION_SUMMER: int = 1
const VACATION_WINTER: int = 2
const VACATION_SPRING: int = 3

## sndisp line chosen in mode_step 0; "" is the source else-branch, which shows
## no line. The ids name the source strings; the text lives in the text pack.
const NOTICE_NEW_TERM: String = "new_term"
const NOTICE_SUMMER: String = "summer_vacation"
const NOTICE_WINTER: String = "winter_vacation"
const NOTICE_SPRING: String = "spring_vacation"

## Mode 0x39 itself stays unbound (core/native_route_registry.gd): after this
## write, mode_step 0 needs text_system_reset/sndisp text, hud_day_bgm_load and
## k_disp_start, and mode_step 1 the bank poll; none has a port service yet.

## mode_step 0 state effect of schedule_vacation_notice_step. Returns the
## chosen notice id and the new vacation value; writes only data["vacation"].
static func vacation_notice(data: Dictionary) -> Dictionary:
	for key: String in ["vacation", "day", "month"]:
		if not data.get(key) is int:
			return {"error": "day notice requires native field: " + key}
	var vacation: int = data["vacation"]
	if vacation < VACATION_NONE or vacation > VACATION_SPRING:
		return {"error": "day notice vacation is outside the native enum"}
	var notice: String = ""
	# The first two source branches have the same effect; both are kept so the
	# order of the source tests is visible.
	if vacation != VACATION_NONE and data["day"] == 2:
		vacation = VACATION_NONE
		notice = NOTICE_NEW_TERM
	elif vacation != VACATION_NONE:
		vacation = VACATION_NONE
		notice = NOTICE_NEW_TERM
	elif data["month"] == 7:
		vacation = VACATION_SUMMER
		notice = NOTICE_SUMMER
	elif data["month"] == 12:
		vacation = VACATION_WINTER
		notice = NOTICE_WINTER
	elif data["month"] == 3:
		vacation = VACATION_SPRING
		notice = NOTICE_SPRING
	else:
		vacation = VACATION_NONE
	data["vacation"] = vacation
	return {"error": "", "notice": notice, "vacation": vacation}
