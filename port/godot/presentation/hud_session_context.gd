extends RefCounted
## T-39013: mutable command-HUD session state, separate from the immutable
## HUD manifest. An instance starts as the loaded executable image and changes
## only through mirrors of the source functions named below. There is no
## constructor from dictionaries, captured boundaries or caller defaults: the
## live route must drive every call in source order from boot.
##
## All state lives in private storage (_s). The public fields are read-only
## getters; assigning one is refused and breaks the session. Each mirror call
## seals _s with a per-instance digest and every reader checks it, so a
## direct write to _s also breaks the session. GDScript has no access
## control: this stops field assignment and stray mutation, not code that
## deliberately recomputes the private seal.
##
## Supported subset (anything else is rejected with an explicit error and no
## state change): TypeA icon bar, fourteen icons, menu control mode 1
## (MENU_MODE_A, the pad_defaults_set value), the ordinary command build branch.

const Math = preload("res://core/native_math.gd")

# asm/data/data_800B5950.data.s initial values (executable .data).
const EXE_CURSOR_STYLE := 0x9C # cursor_style, 0x800B6708
const EXE_HOVER_LAST_INDEX := -1 # D_800B5A9C, 0x800B5A9C byte 0xFF (s8)
const EXE_HOVER_COUNTDOWN := 0 # D_800B5AA0, 0x800B5AA0
# src/main/hud_panels.c:1088-1093: a selection change restarts this countdown.
const HOVER_RESTART := 0x20
# include/game_enums.h, include/main/settings.h.
const MENU_MODE_A := 1
const INPUT_PAD := 1
const CURSOR_SPEED_NORMAL := 3
const ICON_MODE_TYPE_A := 0
const ICON_COUNT_ORDINARY := 14
const PAD_L2 := 0x0001
const PAD_L1 := 0x0004
const PAD_SQUARE := 0x0080
const PAD_SELECT := 0x0100
const MENU_ITEM_SLOTS := 16
const MODE_COMMAND_SCREEN := 0x34

var error: String = ""

# Read-only views of the private state.
var cursor_style: int:
	get: return _s["cursor_style"]
	set(_value): _refuse_write("cursor_style")
var cursor_style_saved: int: # D_8011ECE8 == actor_pool[0].unk_18 (0x8011ECE8)
	get: return _s["cursor_style_saved"]
	set(_value): _refuse_write("cursor_style_saved")
var hover_last_index: int:
	get: return _s["hover_last_index"]
	set(_value): _refuse_write("hover_last_index")
var hover_countdown: int:
	get: return _s["hover_countdown"]
	set(_value): _refuse_write("hover_countdown")
var cursor_x: int: # actor_pool[0].x == cursor_x (0x8011ECF6)
	get: return _s["cursor_x"]
	set(_value): _refuse_write("cursor_x")
var cursor_y: int: # actor_pool[0].y == cursor_y (0x8011ECFA)
	get: return _s["cursor_y"]
	set(_value): _refuse_write("cursor_y")
var cursor_guard: int: # D_8011ECA4
	get: return _s["cursor_guard"]
	set(_value): _refuse_write("cursor_guard")
var menu_mode: int:
	get: return _s["menu_mode"]
	set(_value): _refuse_write("menu_mode")
var cursor_speed: int:
	get: return _s["cursor_speed"]
	set(_value): _refuse_write("cursor_speed")
var icon_mode: int:
	get: return _s["icon_mode"]
	set(_value): _refuse_write("icon_mode")
var input_device: int: # game_state.input_device
	get: return _s["input_device"]
	set(_value): _refuse_write("input_device")
var icon_count: int:
	get: return _s["icon_count"]
	set(_value): _refuse_write("icon_count")
var menu_count: int: # game_state.menu_count_0 (menu 0)
	get: return _s["menu_count"]
	set(_value): _refuse_write("menu_count")
var menu_current: int: # game_state.menu_cursor_0, stored as s8
	get: return _s["menu_current"]
	set(_value): _refuse_write("menu_current")
var menu_items: Array:
	get: return _s["menu_items"].duplicate(true)
	set(_value): _refuse_write("menu_items")
var icon_steps: Array: # actor_pool[80 + i] sequence phase
	get: return _s["icon_steps"].duplicate()
	set(_value): _refuse_write("icon_steps")
var icon_elapsed: Array:
	get: return _s["icon_elapsed"].duplicate()
	set(_value): _refuse_write("icon_elapsed")

var _s: Dictionary = {}
var _nonce: String = ""
var _seal: String = ""


func _init() -> void:
	# Executable .data / zeroed .bss at process start.
	_s = {"cursor_style": EXE_CURSOR_STYLE, "cursor_style_saved": 0,
		"hover_last_index": EXE_HOVER_LAST_INDEX, "hover_countdown": EXE_HOVER_COUNTDOWN,
		"cursor_x": 0, "cursor_y": 0, "cursor_guard": 0,
		"menu_mode": 0, "cursor_speed": 0, "input_device": 0, "icon_mode": 0, "icon_count": 0,
		"menu_count": 0, "menu_current": 0, "menu_items": _zero_items(),
		"icon_steps": _zeros(ICON_COUNT_ORDINARY), "icon_elapsed": _zeros(ICON_COUNT_ORDINARY),
		"game_state_initialized": false, "icon_bar_initialized": false,
		"command_built": false, "command_select": false, "trace": ["process_image"]}
	_nonce = Crypto.new().generate_random_bytes(16).hex_encode()
	_commit()


## Copy of the session for a caller that must undo a refused frame. Only a
## checkpoint taken from this instance restores it.
func checkpoint() -> Dictionary:
	if not intact():
		return {}
	var copy: Dictionary = _s.duplicate(true)
	return {"s": copy, "seal": (_nonce + var_to_str(copy)).sha256_text()}


func restore(data: Dictionary) -> String:
	if not data.get("s") is Dictionary or not data.get("seal") is String \
			or (_nonce + var_to_str(data["s"])).sha256_text() != data["seal"]:
		return _reject("HUD session restore needs a checkpoint of this session")
	_s = (data["s"] as Dictionary).duplicate(true)
	error = ""
	return _commit()


func intact() -> bool:
	return not _seal.is_empty() and _seal == _digest()


func trace() -> Array:
	return _s["trace"].duplicate() if intact() else []


## src/main/game_init.c:142 game_state_init(arg0). clear_game_state mirrors
## arg0 != 0 (bzero of game_state). actor_clear_range (actor_sprites.c:649)
## keeps cursor_x/cursor_y and D_8011ECE8. hud_cursor_actor_init(0)
## (game_init.c:194, hud_screens.c:1479) keeps cursor_x/cursor_y and writes
## actor_pool[0].unk_18 (== D_8011ECE8) from cursor_style. The retained cursor
## is overwritten by menu_cursor_update case 1 on the first command frame,
## before the command select boundary.
func game_state_init(clear_game_state: bool) -> String:
	if not _begin():
		return error
	if clear_game_state:
		_s["menu_count"] = 0
		_s["menu_current"] = 0
		_s["menu_items"] = _zero_items()
	# pad_defaults_set (game_init.c:287). pad_device_detect (pad_input.c:50)
	# keeps INPUT_PAD while a pad answers; the port's only device is the pad.
	_s["input_device"] = INPUT_PAD
	_s["menu_mode"] = MENU_MODE_A
	_s["cursor_speed"] = CURSOR_SPEED_NORMAL
	# boot_settings_defaults_set (game_init.c:470). No modeled call writes
	# another icon mode or count, so the TypeA fourteen-icon bar is the only one.
	_s["icon_mode"] = ICON_MODE_TYPE_A
	_s["icon_count"] = ICON_COUNT_ORDINARY
	_cursor_actor_init()
	_s["game_state_initialized"] = true
	_s["command_built"] = false
	_s["command_select"] = false
	_s["trace"].append("game_state_init(%d)" % (1 if clear_game_state else 0))
	return _commit()


## src/main/main_loop.c:88 boot_frame_begin clears the hud_cursor_update guard.
func begin_outer_frame() -> String:
	if not _begin():
		return error
	_s["cursor_guard"] = 0
	return _commit()


## src/ovl/name_entry/file_select.c:2675 and 2713 store D_8011ECA4 = 1, so
## hud_cursor_update skips the rest of that outer frame.
func name_entry_cursor_hold() -> String:
	if not _begin():
		return error
	_s["cursor_guard"] = 1
	return _commit()


## src/main/pad_input.c:95 pad_state_clear (from pad_device_detect on an
## input-device change) zeroes cursor_x/cursor_y (lines 96-97). The mouse and
## pad words it also clears are not part of the HUD session.
func pad_state_clear() -> String:
	if not _begin():
		return error
	_s["cursor_x"] = 0
	_s["cursor_y"] = 0
	_s["trace"].append("pad_state_clear")
	return _commit()


## src/main/title_screen.c:355 title_menu_cursor_update, pad branch: the cursor
## is placed from scene_work before the d-pad switch updates scene_work, so a
## selection change reaches the cursor one call later. The title screen owns
## scene_work; the mouse branch (menu_check pointer path) is not modeled.
func title_menu_cursor_update(scene_work: int, input_device_mouse: bool) -> String:
	if not _begin():
		return error
	if input_device_mouse:
		return _reject("the title mouse cursor path is outside the HUD session subset")
	_s["cursor_y"] = Math.s16(Math.s32(scene_work * 16 + 0x46))
	_s["cursor_x"] = 0x40
	return _commit()


## src/main/menu_speaker_girl.c:38 menu_set for any screen that registers
## menu 0. rects: [x, y, w, h] records from the caller's s16 arrays. A foreign
## menu replaces the icon bar menu, so the command boundary and the icon bar
## menu are no longer known.
func menu_set(rects: Array) -> String:
	if not _begin():
		return error
	var message := _check_rects(rects)
	if not message.is_empty():
		return _reject(message)
	_s["command_built"] = false
	_s["command_select"] = false
	_s["icon_bar_initialized"] = false
	_s["trace"].append("menu_set")
	_menu_set_rects(rects)
	var result := _commit()
	if result.is_empty() and rects.size() >= 17:
		# The source returns -1 here after clearing w/h/disabled (lines 54-63).
		return _reject("menu_set returned -1: count %d exceeds sixteen records" % rects.size())
	return result


## src/main/hud_panels.c:825 hud_icon_bar_init followed by hud_icon_layout_apply
## (hud_panels.c:908). positions is the immutable TypeA table D_800B5AE4 from
## the authenticated HUD manifest (tables.icon_positions).
func hud_icon_bar_init(positions: Array) -> String:
	if not _begin():
		return error
	var message := _icon_bar_init(positions)
	if not message.is_empty():
		return _reject(message)
	_s["command_built"] = false
	_s["command_select"] = false
	return _commit()


## school_day_screen_init substep 0 (school_day.c:150-155): hud_icon_bar_init,
## icon_disp_switch(1), schedule_command_icons_lock_to_record (rule
## icon_enabled) and hud_cursor_actor_init(0). The menu is the icon bar's again
## and the command-select boundary is left.
func school_day_screen_init(positions: Array) -> String:
	if not _begin():
		return error
	var message := _icon_bar_init(positions)
	if not message.is_empty():
		return _reject(message)
	_cursor_actor_init()
	_s["command_built"] = false
	_s["command_select"] = false
	_s["trace"].append("school_day_screen_init")
	return _commit()


## holiday_open_init (holiday.c:203-205, 254): icon_count = 0xE,
## hud_icon_bar_init, hud_icon_layout_apply (TypeA keeps fourteen), then
## hud_cursor_actor_init(0). The menu is the icon bar's again and the
## command-select boundary is left. The notice branches that set
## menu_cursor_0 (appointment, club practice, remedial, sick) are refused by
## the holiday rules before this runs.
func holiday_open_init(positions: Array) -> String:
	if not _begin():
		return error
	var message := _icon_bar_init(positions)
	if not message.is_empty():
		return _reject(message)
	_cursor_actor_init()
	_s["command_built"] = false
	_s["command_select"] = false
	_s["trace"].append("holiday_open_init")
	return _commit()


## Ordinary branch of src/main/home_screens.c:221 settings_command_screen_build:
## hud_icon_bar_init, menu_cursor_0 = last_command & 15, hud_cursor_actor_init(0)
## (home_screens.c:326: keeps cursor x/y and cursor_style, rewrites D_8011ECE8).
## Calendar/sick/forced branches are rejected.
func command_screen_build(positions: Array, last_command: int, ordinary_branch: bool) -> String:
	if not _begin():
		return error
	if not ordinary_branch:
		return _reject("only the ordinary command build branch is modeled")
	var message := _icon_bar_init(positions)
	if not message.is_empty():
		return _reject(message)
	_s["menu_current"] = Math.s8(last_command & 15)
	_cursor_actor_init()
	_s["command_built"] = true
	_s["command_select"] = false
	_s["trace"].append("command_screen_build")
	return _commit()


## src/main/hud_screens.c:1414 hud_cursor_update.
func hud_cursor_update(pad_held: int, pad_pressed: int) -> String:
	if not _begin():
		return error
	_cursor_update(pad_held, pad_pressed)
	return _commit()


## src/main/hud_panels.c:1024 hud_icon_bar_update(a) for TypeA. Only menu
## control mode 1 is modeled; any other mode is rejected unchanged.
func hud_icon_bar_update(a: int, pad_held: int, pad_pressed: int) -> String:
	if not _begin():
		return error
	var message := _icon_bar_update(pad_held, pad_pressed)
	if not message.is_empty():
		return message
	return _commit()


## Settings command screen per-frame tail (home_screens.c:93-96 and 125-126):
## hud_icon_bar_update(0) then hud_cursor_update().
func command_screen_frame(pad_held: int, pad_pressed: int) -> String:
	if not _begin():
		return error
	var message := _icon_bar_update(pad_held, pad_pressed)
	if not message.is_empty():
		return message
	_cursor_update(pad_held, pad_pressed)
	return _commit()


## actor_draw_all sequence advance for icon actors 80 + i (flags 1 only when
## enabled, hud_panels.c:1097-1103). sequences is the manifest sequence table.
func advance_icon_animation(sequences: Array, icon_enabled: Array) -> String:
	if not _begin():
		return error
	if not _s["icon_bar_initialized"] or sequences.size() < ICON_COUNT_ORDINARY or icon_enabled.size() < ICON_COUNT_ORDINARY:
		return _reject("icon animation requires the initialized bar and source sequences")
	var steps_now: Array = _s["icon_steps"]
	var elapsed_now: Array = _s["icon_elapsed"]
	for icon: int in range(ICON_COUNT_ORDINARY):
		if int(icon_enabled[icon]) != 1:
			continue
		var steps: Array = sequences[icon]["steps"]
		var step: int = steps_now[icon]
		var elapsed: int = int(elapsed_now[icon]) + 1
		if elapsed >= int(steps[step]["duration"]):
			step = (step + 1) % steps.size()
			elapsed = 0
		steps_now[icon] = step
		elapsed_now[icon] = elapsed
	return _commit()


## The route reports the committed transition into mode 0x34 stage 1 step 0.
func enter_command_select(game_mode: int, mode_stage: int, mode_step: int) -> String:
	if not _begin():
		return error
	if not _s["command_built"]:
		return _reject("command select requires a source command build in this session")
	if game_mode != MODE_COMMAND_SCREEN or mode_stage != 1 or mode_step != 0:
		return _reject("command select boundary is mode 0x34 stage 1 step 0")
	_s["command_select"] = true
	_s["trace"].append("enter_command_select")
	return _commit()


## True only for an intact session whose trace runs process_image ->
## game_state_init -> command_screen_build -> enter_command_select with no
## other menu registration after the build.
func at_command_select() -> bool:
	if not intact() or not _s["command_select"] or not _s["command_built"]:
		return false
	var entries: Array = _s["trace"]
	var built: int = entries.rfind("command_screen_build")
	var selected: int = entries.rfind("enter_command_select")
	var initialized: int = -1
	for index: int in range(entries.size()):
		if String(entries[index]).begins_with("game_state_init("):
			initialized = index
	if entries.is_empty() or entries[0] != "process_image" or initialized < 0 or built < initialized or selected < built:
		return false
	for index: int in range(built + 1, entries.size()):
		if entries[index] in ["menu_set", "hud_icon_bar_init"]:
			return false
	return true


func menu_cursor() -> int:
	return Math.u8(_s["menu_current"])


## Index whose menu_items hover is set, or -1 (menu_check_2 marks one item).
func hover_index() -> int:
	var items: Array = _s["menu_items"]
	for index: int in range(_s["menu_count"]):
		if items[index]["hover"] != 0:
			return index
	return -1


## Elapsed hover ticks as the presenter expects them: 32 - D_800B5AA0.
func hover_ticks() -> int:
	return HOVER_RESTART - int(_s["hover_countdown"])


## Scale written to actor_pool[80 + i].scale_x/scale_y for the hovered icon.
static func hover_scale(countdown: int, a: int = 0) -> int:
	if a != 0:
		return 0x1000
	return Math.s16(Math.rsin((HOVER_RESTART - Math.u8(countdown)) << 6) + 0x1000)


## Presenter context; empty when the session was modified outside the mirrors.
func hud_context(game_mode: int, mode_stage: int, mode_step: int, family_name_hex: String) -> Dictionary:
	if not intact():
		return {}
	return {"game_mode": game_mode, "mode_stage": mode_stage, "mode_step": mode_step,
		"icon_mode": _s["icon_mode"], "icon_count": _s["icon_count"], "cursor_style": _s["cursor_style"],
		"hover_ticks": hover_ticks(), "family_name_hex": family_name_hex,
		"hover_index": hover_index(), "cursor_x": _s["cursor_x"], "cursor_y": _s["cursor_y"]}


func _begin() -> bool:
	error = ""
	if not intact():
		error = "HUD session state was modified outside the source mirrors"
		return false
	return true


func _commit() -> String:
	_seal = _digest()
	return ""


func _digest() -> String:
	return (_nonce + var_to_str(_s)).sha256_text()


func _refuse_write(field: String) -> void:
	error = "HUD session field %s is read-only; it changes only through source mirrors" % field
	# A refused write invalidates the session: every later call is rejected.
	_seal = ""


func _check_rects(rects: Array) -> String:
	if rects.size() > 255:
		return "menu_set count is a u8 argument"
	for rect: Variant in rects:
		if not rect is Array or rect.size() != 4:
			return "menu_set records must be four integers"
		for value: Variant in rect:
			if not value is int or value < -0x8000 or value > 0x7FFF:
				return "menu_set records must be s16 integers"
	return ""


## menu_set body (menu_speaker_girl.c:54-80) for menu 0, rects already checked.
func _menu_set_rects(rects: Array) -> void:
	var items: Array = _s["menu_items"]
	# All sixteen records lose w/h/disabled first; x, y and hover persist.
	for item: Dictionary in items:
		item["w"] = 0
		item["h"] = 0
		item["disabled"] = 0
	if rects.size() >= 17:
		return
	for index: int in range(rects.size()):
		var rect: Array = rects[index]
		# disabled tests the s16 product (line 74); w/h are then stored as u8.
		items[index] = {"x": rect[0], "y": rect[1],
			"w": Math.u8(rect[2]), "h": Math.u8(rect[3]),
			"hover": 0, "disabled": 1 if rect[2] * rect[3] == 0 else 0}
	_s["menu_count"] = rects.size()


func _icon_bar_init(positions: Array) -> String:
	if not _s["game_state_initialized"]:
		return "hud_icon_bar_init before game_state_init"
	if positions.size() != ICON_COUNT_ORDINARY:
		return "TypeA icon bar requires fourteen source positions"
	var rects: Array = []
	for point: Variant in positions:
		if not point is Array or point.size() != 2 or not point.all(func(v: Variant) -> bool: return v is int):
			return "icon position must be two integers"
		rects.append([point[0] - 10, point[1] - 10, 20, 16])
	var message := _check_rects(rects)
	if not message.is_empty():
		return message
	# hud_icon_layout_apply keeps the count at fourteen for TypeA.
	_menu_set_rects(rects)
	_s["icon_steps"] = _zeros(ICON_COUNT_ORDINARY)
	_s["icon_elapsed"] = _zeros(ICON_COUNT_ORDINARY)
	_s["icon_bar_initialized"] = true
	_s["trace"].append("hud_icon_bar_init")
	return ""


## hud_cursor_actor_init (hud_screens.c:1479-1510): actor_clear(0) then, for
## both arguments, unk_18 = cursor_style when 0x9B <= cursor_style < 0xAE,
## else 0. x/y are saved and restored around the clear.
func _cursor_actor_init() -> void:
	var style: int = _s["cursor_style"]
	_s["cursor_style_saved"] = style if style >= 0x9B and style < 0xAE else 0


func _cursor_update(pad_held: int, pad_pressed: int) -> void:
	if Math.u8(_s["cursor_guard"]) != 0:
		return
	var base_speed: int = _s["cursor_speed"]
	_s["cursor_guard"] = Math.u8(int(_s["cursor_guard"]) + 1)
	if base_speed == 0:
		base_speed = 1
	_s["cursor_speed"] = base_speed
	var movement: int = base_speed
	if (int(_s["menu_mode"]) & 3) == 0 and (pad_held & 0xFF) != 0:
		movement = base_speed * 2
	var x: int = _s["cursor_x"]
	var y: int = _s["cursor_y"]
	if (pad_held & 0xF) == 0:
		match pad_held & 0x5000:
			0x1000: y = Math.s16(y - movement)
			0x4000: y = Math.s16(y + movement)
		match pad_held & 0xA000:
			0x2000: x = Math.s16(x + movement)
			0x8000: x = Math.s16(x - movement)
	if x >= 0x8D:
		x = 0x8C
	if x < -0x96:
		x = -0x96
	if y < -0x64:
		y = -0x64
	if y >= 0x65:
		y = 0x64
	_s["cursor_x"] = x
	_s["cursor_y"] = y
	if (pad_pressed & PAD_SQUARE) != 0:
		var style: int = _s["cursor_style"]
		if style == 0:
			style = 0x9B
		elif style == 0xAC:
			style = 0
		else:
			style = Math.u8(style + 1)
		_s["cursor_style"] = style
		_s["cursor_style_saved"] = style


## On error the state is restored to the call's entry state.
func _icon_bar_update(pad_held: int, pad_pressed: int) -> String:
	if not _s["icon_bar_initialized"]:
		return _reject("hud_icon_bar_update before the icon bar menu is known")
	if (pad_held & PAD_L2) == 0 and (int(_s["menu_mode"]) & 3) != MENU_MODE_A:
		return _reject("menu control mode %d is outside the HUD session subset" % (int(_s["menu_mode"]) & 3))
	var before: Dictionary = _s.duplicate(true)
	var previous: int = Math.u8(_s["menu_current"])
	if (pad_held & PAD_L2) == 0:
		# icon_mode % 3 == 2 alone moves icons under L2; TypeA does nothing there.
		_menu_check_2()
		_menu_cursor_update(pad_held, pad_pressed)
	# lbu previous vs lb D_800B5A9C (hud_icon_bar_update.s 0x80065D24..30).
	if previous != Math.u32(_s["hover_last_index"]):
		_s["hover_last_index"] = Math.s8(_s["menu_current"])
		_s["hover_countdown"] = HOVER_RESTART
	elif int(_s["hover_countdown"]) != 0:
		_s["hover_countdown"] = int(_s["hover_countdown"]) - 1
	if not error.is_empty():
		_s = before
		return error
	return ""


func _menu_check_2() -> void:
	# menu_speaker_girl.c:152; i is u32 and current is s8 sign-extended.
	var items: Array = _s["menu_items"]
	for index: int in range(_s["menu_count"]):
		items[index]["hover"] = 10 if Math.u32(index) == Math.u32(_s["menu_current"]) else 0


func _menu_cursor_update(pad_held: int, pad_pressed: int) -> void:
	# menu_speaker_girl.c:181, case 1.
	if (pad_pressed & PAD_SELECT) != 0:
		_s["menu_mode"] = Math.u8(int(_s["menu_mode"]) + 1)
		_s["menu_current"] = -1
		return
	var count: int = _s["menu_count"]
	var items: Array = _s["menu_items"]
	var direction: int = 0
	var budget: int = 16
	var current: int = _s["menu_current"]
	if (pad_held & 0xF) == 0:
		match pad_pressed & 0xF000:
			0x1000, 0x8000:
				direction = -1
				current = Math.s8(current - 1)
			0x2000, 0x4000:
				direction = 1
				current = Math.s8(current + 1)
	if current >= count:
		current = 0
	if current < 0:
		current = Math.s8(count - 1)
	while current >= 0 and items[current]["disabled"] == 1:
		if budget == 0:
			break
		budget -= 1
		current = Math.s8(current + direction)
		if current >= count:
			current = 0
		if current < 0:
			current = Math.s8(count - 1)
	if current < 0:
		# Only an empty menu with a negative cursor reaches here; the source then
		# reads menu_items[-1], outside the array.
		error = "menu cursor update with an empty menu is outside the HUD session subset"
		return
	_s["menu_current"] = current
	_s["cursor_x"] = Math.s16(int(items[current]["x"]) + int(items[current]["w"]) - 4)
	_s["cursor_y"] = Math.s16(int(items[current]["y"]) + 8)


func _reject(message: String) -> String:
	error = message
	return message


static func _zero_items() -> Array:
	var items: Array = []
	for index: int in range(MENU_ITEM_SLOTS):
		items.append({"x": 0, "y": 0, "w": 0, "h": 0, "hover": 0, "disabled": 0})
	return items


static func _zeros(count: int) -> Array:
	var values: Array = []
	values.resize(count)
	values.fill(0)
	return values
