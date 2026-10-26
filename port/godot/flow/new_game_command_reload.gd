extends RefCounted
## Source: home_screens.c::settings_command_screen_mode_stage_reload (stage 0xFF)
## and settings_command_screen_mode_stage_setup (stage 0).
## Callbacks must perform actual service work and report completion explicitly.

const State = preload("res://rules/new_game_route_state.gd")
const Session = preload("res://core/game_session.gd")
const Math = preload("res://core/native_math.gd")
const OrdinaryWeek = preload("res://rules/ordinary_week.gd")

const MODE_COMMAND_SCREEN: int = 0x34
const STAGE_SETUP: int = 0
const STAGE_READY: int = 1
const STAGE_RELOAD: int = 0xff
const HUD_SECTOR_COUNT: int = 0x95
const HUD_READ_DEST: int = 0x801a0000
const HUD_READ_LBA: int = 0x9b94
const MENU_MODE_MASK: int = 3
const DEFAULT_B5BC8: int = 0xfe
const MASCOT_COUNT: int = 14
const COMMAND_ICON_COUNT: int = 14
const ICON_ARRAY_SIZE: int = 15
const CLUB_FLAG_MASK: int = 0xf000
const ICON_CLUB: int = 4
const ICON_SPRING_VACATION: int = 8
const ICON_SUMMER_VACATION: int = 9
const ICON_WINTER_VACATION: int = 10
const ICON_PHONE: int = 13
const PHONE_GIRL_LIMIT: int = 13
const LAST_COMMAND_MASK: int = 15
const FULL_BRIGHTNESS: int = 128
const BRIGHTNESS_STEP: int = 4

const REQUIRED: Array[String] = [
	"setup_reset",
	"bank_poll",
	"bank_start",
	"hud_read_start",
	"cd_sync",
	"calendar_upload",
	"mascot_set",
	"command_build",
	"room_finish",
	"frame_draw",
	"outer_draw",
]


static func build_command_screen(state: State) -> void:
	var data: Dictionary = state.rule_state
	if data.has("params") and data.has("girls") and data.has("player_flags") and data.has("player_birthday"):
		OrdinaryWeek.snapshot_parameters(state)
	var icons: Array = []
	icons.resize(ICON_ARRAY_SIZE)
	icons.fill(0)
	var disabled: Array = []
	disabled.resize(COMMAND_ICON_COUNT)
	disabled.fill(0)
	for index: int in range(COMMAND_ICON_COUNT):
		icons[index] = 1
	var club_id: int = (int(data.get("player_flags", 0)) >> 12) & 15
	if club_id >= 11:
		icons[ICON_CLUB] = 0
		disabled[ICON_CLUB] = 1
	var vacation: int = int(data.get("vacation", 0))
	if vacation != 1:
		icons[ICON_SPRING_VACATION] = 0
		disabled[ICON_SPRING_VACATION] = 1
	if vacation != 2:
		icons[ICON_SUMMER_VACATION] = 0
		disabled[ICON_SUMMER_VACATION] = 1
	if vacation != 3:
		icons[ICON_WINTER_VACATION] = 0
		disabled[ICON_WINTER_VACATION] = 1
	var has_girl_phone: bool = false
	var girls: Array = data.get("girls", [])
	var girl_limit: int = mini(girls.size(), PHONE_GIRL_LIMIT)
	for index: int in range(girl_limit):
		var girl: Dictionary = girls[index]
		if (int(girl.get("flags", 0)) & 1) != 0:
			has_girl_phone = true
			break
	if not has_girl_phone:
		icons[ICON_PHONE] = 0
		disabled[ICON_PHONE] = 1
	data["icon_enabled"] = icons
	data["menu_disabled"] = disabled
	data["menu_cursor"] = int(data.get("last_command", 0)) & LAST_COMMAND_MASK
	state.opening["icon_mode"] = 0
	state.opening["icon_count"] = COMMAND_ICON_COUNT
	state.next_step()


static func finish_room_load(state: State) -> void:
	state.opening["screen_brightness"] = FULL_BRIGHTNESS
	state.opening["command_ready"] = true
	state.set_stage(STAGE_READY)


static func step(state: State, services: Dictionary) -> Session.FrameResult:
	var result: Session.FrameResult = Session.FrameResult.new()
	if state.game_mode != MODE_COMMAND_SCREEN or state.mode_stage not in [STAGE_SETUP, STAGE_RELOAD] or state.mode_step not in [0, 1, 2, 3]:
		result.error = "command reload requires native mode 0x34 stage 0 or FF"
		return result

	for key: String in REQUIRED:
		if not services.get(key) is Callable or not services[key].is_valid():
			result.error = "command reload needs service: " + key
			return result

	var before: Dictionary = state.projection()
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)

	match state.mode_step:
		0:
			services["setup_reset"].call(state, result)
		1:
			var hud_substep: int = state.substep if state.mode_stage == STAGE_RELOAD else state.substep + 1
			if hud_substep == 0:
				var bank_status: Variant = services["bank_poll"].call(state, result)
				if result.error.is_empty() and typeof(bank_status) != TYPE_INT:
					result.error = "bank poll lacks actual native status"
				elif result.error.is_empty() and int(bank_status) == 1:
					services["bank_start"].call(state, 0, 0, result)
					state.substep += 1
			elif hud_substep == 1:
				services["hud_read_start"].call(state, HUD_SECTOR_COUNT, HUD_READ_DEST, HUD_READ_LBA, result)
				state.substep += 1
			elif hud_substep == 2:
				var cd_status: Variant = services["cd_sync"].call(state, result)
				if result.error.is_empty() and typeof(cd_status) != TYPE_INT:
					result.error = "CD sync lacks actual native status"
				elif result.error.is_empty() and (int(cd_status) & 1) != 0:
					services["calendar_upload"].call(state, int(state.rule_state["year"]), int(state.rule_state["month"]), 0, result)
					state.rule_state["menu_mode"] = int(state.rule_state["menu_mode"]) & MENU_MODE_MASK
					state.opening["D_800B5BC8"] = DEFAULT_B5BC8
					var mascot_choice: int = int(state.opening.get("hud_mascot_choice", 255))
					if mascot_choice < MASCOT_COUNT:
						services["mascot_set"].call(state, mascot_choice, 1, result)
					state.next_step()
			else:
				result.error = "command reload substep outside source dispatch"
		2:
			services["command_build"].call(state, result)
		3:
			services["room_finish"].call(state, result)

	if result.error.is_empty():
		services["frame_draw"].call(state, state.mode_step >= 2, result)
		if state.mode_step >= 2 and int(state.opening.get("screen_brightness", 0)) < FULL_BRIGHTNESS:
			state.opening["screen_brightness"] = Math.u8(int(state.opening.get("screen_brightness", 0)) + BRIGHTNESS_STEP)
		if result.error.is_empty():
			services["outer_draw"].call(state, result)

	if not result.error.is_empty():
		state.restore(before)
		result.audio.clear()
		result.presentation.clear()
		result.transitions.clear()

	return result
