extends RefCounted
## Original main-executable and connected opening overlay modes bound here.
## Source: src/main/game_mode.c::game_mode_dispatch (0x15,0x30,0x31,0x32,0x34,0x40,0x46).
## Registering a callback does not establish parity for its implementation.

const NATIVE_HANDLERS: Dictionary = {
	0x15: "src/ovl/name_entry/prologue.c::prologue_mode_handler",
	0x46: "src/main/holiday.c::holiday_mode_handler",
	0x32: "src/main/scheduler.c::schedule_week_end_mode_handler",
	0x30: "src/main/scheduler.c::schedule_init",
	0x31: "src/main/scheduler.c::schedule_day_screen_handler",
	0x34: "src/main/home_screens.c::settings_command_screen_mode_handler",
	0x40: "src/main/school_day.c::week_day",
}

const OVERLAY_HANDLERS: Dictionary = {
	0x01: "src/ovl/misc_scenes/::opening_mode_handler",
	0x35: "src/main/school_events.c::exam_mode_handler",
	0x36: "src/ovl/options/::option_mode_handler",
	0x37: "src/ovl/help/::olh_mode_handler",
	0x3d: "src/ovl/telephone/::tel_mode_handler",
	0x3e: "src/ovl/telephone/::tel_action_mode_handler",
	0x3f: "src/ovl/misc_scenes/::etc_mode_handler",
	0x44: "src/ovl/girl_encounters/::geko_mode_handler",
	0x47: "src/ovl/sports_festival/::taiiku_mode_handler",
	0x48: "src/ovl/culture_festival/::bunkasai_mode_handler",
	0x49: "src/ovl/culture_festival_sd/::bunka_sd_mode_handler",
	0x4a: "src/ovl/culture_club_tour/::bunkaken_mode_handler",
	0x4b: "src/ovl/sick_visit/::omimai_mode_handler",
	0x4c: "src/ovl/school_trip/::shugaku_mode_handler",
	0x4d: "src/ovl/welcome_party/::kangei_mode_handler",
	0x4e: "src/ovl/new_year/::shougatu_mode_handler",
	0x4f: "src/ovl/valentine/::valen_mode_handler",
	0x50: "src/ovl/date/::date_mode_handler",
	0x51: "src/ovl/club_practice/::rensyu_mode_handler",
	0x52: "src/ovl/club_master/::master_mode_handler",
	0x53: "src/ovl/night_stalls/::en_nichi_mode_handler",
	0x57: "src/ovl/rpg_battle/::rpg_battle_mode_handler",
	0x58: "src/ovl/amusement_park_date/::date2_mode_handler",
	0x59: "src/ovl/shooter_3d/::taco_mode_handler",
	0x5a: "src/ovl/TT/::tt_mode_handler",
	0x5c: "src/ovl/ending/::ending_mode_handler",
	0x5e: "src/ovl/date_spots/::event_cg_mode_handler",
}

var _handlers: Dictionary = {}


func bind_mode(native_mode: int, handler: Callable) -> String:
	if not NATIVE_HANDLERS.has(native_mode):
		return "mode outside bounded command/school-day slice: 0x%02x" % native_mode
	if not handler.is_valid():
		return "mode handler is not callable"
	_handlers[native_mode] = handler
	return ""


func bind_overlay_mode(native_mode: int, handler: Callable) -> String:
	if not OVERLAY_HANDLERS.has(native_mode) and not NATIVE_HANDLERS.has(native_mode):
		return "unsupported overlay mode: 0x%02x" % native_mode
	if not handler.is_valid():
		return "mode handler is not callable"
	_handlers[native_mode] = handler
	return ""


func missing_handler(native_mode: int) -> String:
	if not _handlers.has(native_mode):
		return "unsupported native mode 0x%02x: no implemented handler" % native_mode
	return ""


func handler(native_mode: int) -> Callable:
	return _handlers[native_mode]
