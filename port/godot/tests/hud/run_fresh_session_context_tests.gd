extends SceneTree
## T-39013: HUD session model against the original source (no game data).
## Expected values cite src/main and asm/data; the geometry below is a test
## fixture only (14 distinct points), not the D_800B5AE4 table. Every state is
## reached through source mirror calls; the gating checks show that direct
## writes are refused.

const HudSession = preload("res://presentation/hud_session_context.gd")
const Math = preload("res://core/native_math.gd")

var checks: int = 0
var failures: int = 0


func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL " + label)


func _positions() -> Array:
	var points: Array = []
	for index: int in range(14):
		points.append([-120 + index * 20, 80])
	return points


func _built() -> HudSession:
	var session := HudSession.new()
	session.game_state_init(true)
	session.hud_icon_bar_init(_positions()) # boot_hud_init (boot_assets.c:136)
	session.command_screen_build(_positions(), 0, true)
	return session


func _settled() -> HudSession:
	var session := _built()
	for frame: int in range(40):
		session.begin_outer_frame()
		session.command_screen_frame(0, 0)
	return session


func _initialize() -> void:
	_process_image_and_game_state_init()
	_boundary_ordering()
	_foreign_menu_set_after_build()
	_menu_set_source_semantics()
	_direct_writes_are_refused()
	_first_frames_after_build()
	_pulse_phases()
	_dpad_restart_lags_one_frame()
	_unsigned_signed_compare()
	_cursor_style_and_guard()
	_other_cursor_writers()
	_l2_skips_menu_check()
	_unsupported_inputs_are_rejected_unchanged()
	_empty_menu_is_rejected_and_restored()
	if failures != 0:
		printerr("FAIL: %d/%d fresh HUD session checks" % [failures, checks])
		quit(1)
		return
	print("PASS: %d fresh HUD session checks" % checks)
	quit(0)


func _process_image_and_game_state_init() -> void:
	var session := HudSession.new()
	check(session.cursor_style == 0x9C, "cursor_style starts at executable value 0x9C")
	check(session.hover_last_index == -1 and session.hover_countdown == 0, "D_800B5A9C/D_800B5AA0 start at 0xFF/0")
	check(session.cursor_x == 0 and session.cursor_y == 0 and session.menu_mode == 0, "bss cursor and menu state start at zero")
	session.game_state_init(true)
	check(session.hud_cursor_update(0x2000 | 0x4000, 0).is_empty(), "cursor update runs after game_state_init")
	var moved := Vector2i(session.cursor_x, session.cursor_y)
	session.game_state_init(true)
	check(Vector2i(session.cursor_x, session.cursor_y) == moved and moved == Vector2i(3, 3), "game_state_init keeps the cursor actor position")
	check(session.menu_mode == 1 and session.cursor_speed == 3 and session.icon_mode == 0 and session.icon_count == 14, "pad/settings defaults are source values")
	check(session.at_command_select() == false and session.hover_index() == -1, "no command boundary or hover after init")


func _boundary_ordering() -> void:
	var session := HudSession.new()
	check(not session.enter_command_select(0x34, 1, 0).is_empty(), "command select without build is rejected")
	check(not session.hud_icon_bar_init(_positions()).is_empty(), "icon bar before game_state_init is rejected")
	session.game_state_init(true)
	check(not session.command_screen_build(_positions(), 0, false).is_empty(), "non-ordinary build branch is rejected")
	check(not session.command_screen_build(_positions().slice(0, 13), 0, true).is_empty() and session.menu_count == 0, "short icon table is rejected unchanged")
	check(session.trace().size() == 2 and session.trace()[1] == "game_state_init(1)", "rejected builds leave no trace")
	check(session.command_screen_build(_positions(), 0x13, true).is_empty() and session.menu_cursor() == 3, "build sets menu_cursor_0 = last_command & 15")
	check(not session.enter_command_select(0x34, 0xFF, 0).is_empty(), "reload stage is not the select boundary")
	check(session.enter_command_select(0x34, 1, 0).is_empty() and session.at_command_select(), "mode 0x34/1/0 after build is the boundary")
	session.menu_set([[0, 0, 10, 10]])
	check(not session.at_command_select(), "another menu_set leaves the command boundary")


func _foreign_menu_set_after_build() -> void:
	var session := _built()
	check(session.menu_set([[0, 0, 10, 10], [20, 0, 10, 10]]).is_empty() and session.menu_count == 2, "foreign two-item menu registers")
	check(not session.enter_command_select(0x34, 1, 0).is_empty() and not session.at_command_select(), "command select over a foreign menu is rejected")
	check(not session.command_screen_frame(0, 0).is_empty(), "icon bar update over a foreign menu is rejected")
	var standalone := _built()
	standalone.hud_icon_bar_init(_positions())
	check(not standalone.enter_command_select(0x34, 1, 0).is_empty(), "icon bar re-init after the build needs a new build")


func _menu_set_source_semantics() -> void:
	var session := HudSession.new()
	# menu_speaker_girl.c:74 tests the s16 product; w/h are then stored as u8.
	session.menu_set([[0, 0, 256, 16], [0, 0, 0, 16]])
	var items: Array = session.menu_items
	check(items[0]["disabled"] == 0 and items[0]["w"] == 0, "width 256 stays enabled and stores u8 0")
	check(items[1]["disabled"] == 1, "zero width disables the item")
	check(not session.menu_set([[0, 0, 0x8000, 1]]).is_empty(), "values outside s16 are rejected")
	var many: Array = []
	for index: int in range(17):
		many.append([index, 0, 10, 10])
	var message := session.menu_set(many)
	items = session.menu_items
	check(not message.is_empty() and session.menu_count == 2 and items[0]["w"] == 0 and items[0]["x"] == 0, "count 17 returns -1 after clearing w/h/disabled")
	check(items[1]["disabled"] == 0, "count 17 clears disabled of the old records")


func _direct_writes_are_refused() -> void:
	var probe := HudSession.new()
	var handle: Variant = probe
	handle.set("_command_select", true)
	probe.icon_mode = 0
	probe.icon_count = 14
	check(not probe.error.is_empty() and not probe.intact(), "assigning a session field is refused and breaks the session")
	check(not probe.at_command_select() and probe.hud_context(0x34, 1, 0, "82a0").is_empty(), "a broken session exposes no boundary or context")
	check(not probe.game_state_init(true).is_empty(), "a broken session rejects every mirror call")

	var session := _built()
	session.enter_command_select(0x34, 1, 0)
	var before := session.at_command_select()
	var internal: Variant = session.get("_s")
	internal["icon_count"] = 15
	check(before and not session.at_command_select() and not session.intact(), "mutating private storage breaks the seal")
	check(not session.command_screen_frame(0, 0).is_empty(), "mirror calls do not re-seal tampered storage")

	var copy := _built()
	var items: Array = copy.menu_items
	items[0]["disabled"] = 1
	var steps: Array = copy.icon_steps
	steps[0] = 3
	check(copy.intact() and copy.menu_items[0]["disabled"] == 0 and copy.icon_steps[0] == 0, "getters return copies")


func _first_frames_after_build() -> void:
	var session := _built()
	session.begin_outer_frame()
	check(session.command_screen_frame(0, 0).is_empty(), "first command frame runs")
	# previous (0) differs from D_800B5A9C, which still holds -1 from the image.
	check(session.hover_countdown == 32 and session.hover_last_index == 0 and session.hover_ticks() == 0, "first frame restarts the 32-tick countdown")
	check(session.hover_index() == 0, "menu_check_2 marks the current item")
	check(Vector2i(session.cursor_x, session.cursor_y) == Vector2i(-120 + 6, 80 - 2), "mode 1 cursor is item x + w - 4, y + 8")
	for frame: int in range(5):
		session.begin_outer_frame()
		session.command_screen_frame(0, 0)
	check(session.hover_countdown == 27 and session.hover_ticks() == 5, "countdown decrements once per frame")
	for frame: int in range(40):
		session.begin_outer_frame()
		session.command_screen_frame(0, 0)
	check(session.hover_countdown == 0 and session.hover_ticks() == 32, "countdown settles at zero")


func _pulse_phases() -> void:
	check(HudSession.hover_scale(32) == 0x1000, "restart frame has unit scale (rsin(0))")
	check(HudSession.hover_scale(0) == 0x1000, "settled phase has unit scale (rsin(0x800))")
	check(HudSession.hover_scale(16) == 0x2000, "mid pulse is rsin(0x400) + 0x1000")
	check(HudSession.hover_scale(16, 1) == 0x1000, "a != 0 keeps unit scale")
	var symmetric := true
	var rising := true
	for countdown: int in range(1, 32):
		var scale: int = HudSession.hover_scale(countdown)
		symmetric = symmetric and scale == HudSession.hover_scale(32 - countdown)
		rising = rising and scale > 0x1000
		symmetric = symmetric and scale == Math.rsin((32 - countdown) << 6) + 0x1000
	check(symmetric and rising, "pulse follows rsin((32 - countdown) << 6) + 0x1000 and exceeds unit scale")


func _dpad_restart_lags_one_frame() -> void:
	var session := _settled()
	session.begin_outer_frame()
	session.command_screen_frame(0x2000, 0x2000)
	# Same frame: previous (0) == D_800B5A9C (0), cursor moves, countdown stays settled.
	check(session.menu_cursor() == 1 and session.hover_countdown == 0 and session.hover_index() == 0, "press moves the cursor; hover/restart follow next frame")
	check(Vector2i(session.cursor_x, session.cursor_y) == Vector2i(-100 + 6 + 3, 78), "held right moves the actor after the item placement")
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	check(session.hover_countdown == 32 and session.hover_last_index == 1 and session.hover_index() == 1, "restart happens one frame after the move")
	session.begin_outer_frame()
	session.command_screen_frame(0x8000, 0x8000)
	session.begin_outer_frame()
	session.command_screen_frame(0x8000, 0x8000)
	check(session.menu_cursor() == 13, "left from item 0 wraps to the last item")


func _unsigned_signed_compare() -> void:
	var session := _built()
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	session.begin_outer_frame()
	session.command_screen_frame(0, 0x100) # Select: menu_cursor_0 = -1, menu_mode 2
	check(session.menu_cursor() == 0xFF and session.hover_last_index == 0 and session.hover_countdown == 31, "Select clears the cursor; the compare still saw 0")
	# L2 held skips menu_cursor_update, so menu mode 2 is not reached.
	# lbu 0xFF never equals lb -1: the restart repeats while the cursor is 0xFF.
	session.begin_outer_frame()
	check(session.hud_icon_bar_update(0, 0x1, 0).is_empty(), "L2 frame runs in menu mode 2")
	check(session.hover_countdown == 32 and session.hover_last_index == -1, "0xFF cursor restarts and stores -1")
	session.begin_outer_frame()
	session.hud_icon_bar_update(0, 0x1, 0)
	check(session.hover_countdown == 32 and session.hover_last_index == -1, "0xFF cursor keeps restarting against stored -1")


func _cursor_style_and_guard() -> void:
	var session := _built()
	session.begin_outer_frame()
	session.command_screen_frame(0, 0x80)
	check(session.cursor_style == 0x9D and session.cursor_style_saved == 0x9D, "Square advances the cursor style")
	session.hud_cursor_update(0, 0x80)
	check(session.cursor_style == 0x9D, "hud_cursor_update runs once per outer frame")
	while session.cursor_style != 0xAC:
		session.begin_outer_frame()
		session.hud_cursor_update(0, 0x80)
	session.begin_outer_frame()
	session.hud_cursor_update(0, 0x80)
	check(session.cursor_style == 0, "style 0xAC wraps to 0")
	session.begin_outer_frame()
	session.hud_cursor_update(0, 0x80)
	check(session.cursor_style == 0x9B, "style 0 advances to 0x9B")


func _other_cursor_writers() -> void:
	var session := HudSession.new()
	session.game_state_init(true)
	check(session.title_menu_cursor_update(1, false).is_empty() and Vector2i(session.cursor_x, session.cursor_y) == Vector2i(0x40, 0x56), "title cursor is (0x40, scene_work * 16 + 0x46)")
	check(not session.title_menu_cursor_update(0, true).is_empty() and session.cursor_y == 0x56, "title mouse path is rejected unchanged")
	session.pad_state_clear()
	check(session.cursor_x == 0 and session.cursor_y == 0, "pad_state_clear zeroes the cursor")
	session.begin_outer_frame()
	session.name_entry_cursor_hold()
	session.hud_cursor_update(0x2000, 0x80)
	check(session.cursor_x == 0 and session.cursor_style == 0x9C, "name-entry D_8011ECA4 = 1 skips hud_cursor_update")


func _l2_skips_menu_check() -> void:
	var session := _built()
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	session.begin_outer_frame()
	session.command_screen_frame(0x1 | 0x2000, 0x2000)
	check(session.menu_cursor() == 0 and session.hover_countdown == 31, "TypeA L2 frame skips menu update but counts down")


func _unsupported_inputs_are_rejected_unchanged() -> void:
	var session := _built()
	session.begin_outer_frame()
	session.command_screen_frame(0, 0x100)
	check(session.menu_mode == 2 and session.menu_cursor() == 0xFF, "Select steps menu_mode and clears the cursor")
	var countdown: int = session.hover_countdown
	var position := Vector2i(session.cursor_x, session.cursor_y)
	session.begin_outer_frame()
	var message: String = session.command_screen_frame(0, 0)
	check(not message.is_empty() and session.hover_countdown == countdown and Vector2i(session.cursor_x, session.cursor_y) == position, "menu mode 2 is rejected without state change")


func _empty_menu_is_rejected_and_restored() -> void:
	# game_state_init(1) bzeroes menu 0 while the icon bar stays; a left press then
	# drives the cursor to -1 on a zero-count menu (the source reads item -1).
	var session := _built()
	session.begin_outer_frame()
	session.command_screen_frame(0, 0)
	session.game_state_init(true)
	session.begin_outer_frame()
	var countdown: int = session.hover_countdown
	var last: int = session.hover_last_index
	var position := Vector2i(session.cursor_x, session.cursor_y)
	var items: Array = session.menu_items
	var message: String = session.command_screen_frame(0x8000, 0x8000)
	check(message.contains("empty menu"), "empty-menu cursor update is rejected: " + message)
	check(session.intact() and session.hover_countdown == countdown and session.hover_last_index == last, "rejected update restores the countdown it had already stepped")
	check(session.menu_current == 0 and Vector2i(session.cursor_x, session.cursor_y) == position and session.menu_items == items, "rejected update leaves cursor and menu items unchanged")
	check(session.cursor_guard == 0, "rejected frame skips hud_cursor_update")
	session.begin_outer_frame()
	check(session.command_screen_frame(0, 0).is_empty() and session.menu_current == 0 and Vector2i(session.cursor_x, session.cursor_y) == Vector2i(-4, 8), "zero-count menu without a press reads item 0 like the source")
