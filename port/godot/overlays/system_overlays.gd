extends RefCounted
class_name SystemOverlays
## T-29018: System Overlays implementation for:
## 1. OPTION (src/ovl/options/settings_sound_test_and_extras.c)
## 2. OLH (src/ovl/help/help_questions_and_answers_page.c)
## 3. ETC (src/ovl/misc_scenes/ & src/main/title_screen.c)
##
## Provides state machines, in-memory settings, input handling,
## and 320x240 rendering through VnScenePresenter. Callers own persistence.

const VnPresenter = preload("res://presentation/vn_scene_presenter.gd")

# PS1 Controller Button Constants (matching common.h / pad.h)
const PAD_SELECT   := 0x0100
const PAD_START    := 0x0800
const PAD_UP       := 0x1000
const PAD_RIGHT    := 0x2000
const PAD_DOWN     := 0x4000
const PAD_LEFT     := 0x8000
const PAD_TRIANGLE := 0x0010
const PAD_CIRCLE   := 0x0020
const PAD_CROSS    := 0x0040
const PAD_SQUARE   := 0x0080

# Exact CP932 hex byte encodings for system typography
const STR_HEX := {
	# Options Overlay & In-Game System Settings (home_screens.c:1462-1703 & settings_sound_test_and_extras.c)
	"title_options": "83498376835683878393", # オプション
	"title_system_settings": "835683588365838090dd92e889e696ca", # システム設定画面
	"prompt_change_settings": "835683588365838090dd92e882f095cf8d5882b582dc82b7", # システム設定を変更します
	"opt_speed": "95b68e9a955c8ea691ac9378", # 文字表示速度
	"opt_icon": "83418343835283938382815b8368", # アイコンモード
	"opt_mouse": "837d8345835890dd92e8", # マウス設定
	"opt_window": "8381836a8385815b8382815b8368", # メニューモード
	"opt_cursor": "834a815b835c838b91ac9378", # カーソル速度
	"opt_voice": "89b990ba8382815b8368", # 音声モード
	"opt_extras": "82a882dc82af", # おまけ
	"opt_mascot": "837d8358835283628367", # マスコット
	"opt_exit": "834983768356838783938f4997b9", # オプション終了
	"opt_settings_exit": "90dd92e88f4997b9", # 設定終了

	# Values
	"val_fast": "82cd82e282a2", # はやい
	"val_normal": "82d382c282a4", # ふつう
	"val_slow": "82a882bb82a2", # おそい
	"val_on": "82a082e8", # あり
	"val_off": "82c882b5", # なし
	"val_stereo": "83588365838c8349", # ステレオ
	"val_mono": "8382836d8389838b", # モノラル
	"val_style0": "835e834383768260", # タイプＡ
	"val_style1": "835e834383768261", # タイプＢ
	"val_style2": "835e834383768262", # タイプＣ

	# Help Overlay (OLH)
	"title_olh": "8270819582608352815b8369815b", # Ｑ＆Ａコーナー
	"prompt_start_return": "8358835e815b8367837b835e839382c58351815b838082c996df82ea82dc82b7", # スタートボタンでゲームに戻れます
	"prompt_select_item": "8d8096da82f0914982f182c582ad82be82b382a2", # 項目を選んでください
	"olh_cat_system": "8145835683588365838090dd92e8", # ・システム設定
	"olh_cat_memcard": "814583818382838a815b834a815b8368", # ・メモリーカード
	"olh_cat_filesel": "8145837483408343838b835a838c834e8367", # ・ファイルセレクト
	"olh_cat_biorhythm": "8145836f83438349838a83598380", # ・バイオリズム
	"olh_cat_icons": "81458341834383528393", # ・アイコン
	"olh_cat_subgame": "8145835483758351815b8380", # ・サブゲーム
	"olh_cat_tips": "8145837e836a926d8eaf", # ・ミニ知識
	"olh_cat_return": "81458351815b838082c996df82e9", # ・ゲームに戻る

	# Title / ETC Overlay
	"title_logo": "82c682ab82df82ab83818382838a8341838b", # ときめきメモリアル
	"menu_new_game": "82cd82b682df82a982e7", # はじめから
	"menu_continue": "82c282c382ab82a982e7", # つづきから
	"menu_option": "83498376835683878393", # オプション
}

# ==============================================================================
# 1. Options Overlay (OPTION)
# ==============================================================================

class OptionsOverlay extends RefCounted:
	const STAGE_SETUP := 0
	const STAGE_SELECT := 1
	const STAGE_ADJUST := 2
	const STAGE_EXTRAS := 3
	const STAGE_EXIT := 4

	# Settings state
	var text_speed: int = 1         # 0=Slow, 1=Normal, 2=Fast, 3=Instant
	var icon_mode: int = 0          # 0=Normal (14 icons), 1=Simple (8 icons)
	var mouse_enabled: bool = false # Mouse support toggle
	var mouse_speed: int = 1        # 0=Slow, 1=Normal, 2=Fast
	var window_style: int = 0       # 0=Type A (Blue), 1=Type B (Green), 2=Type C (Purple)
	var cursor_speed: int = 1       # 0=Slow, 1=Normal, 2=Fast
	var voice_mode: int = 1         # 0=Off, 1=Voiced, 2=All
	var mascot_unlocked: bool = false # settings_unlocked_mascot_count() != 0 (home_screens.c:1659)
	var sound_mode: int = 0         # 0=Stereo, 1=Mono
	var checker_scroll_pattern: int = 0 # 0..5 (player_flags.b[1] & 15)
	var button_confirm: int = 0     # 0=Circle, 1=Cross

	# Overlay state machine
	var mode_stage: int = STAGE_SELECT
	var cursor: int = 0             # 0..7
	var exited: bool = false
	var frame_count: int = 0
	var scroll_u: int = 30
	var scroll_v: int = 17

	func _init() -> void:
		mode_stage = STAGE_SELECT
		cursor = 0
		exited = false
		frame_count = 0
		scroll_u = 30
		scroll_v = 17

	## Advance checkerboard scroll offsets according to player_flags.b[1] & 15 formula
	## (src/main/hud_panels.c:1385-1411).
	func update_scroll_offsets(ticks: int = 1) -> void:
		for _t in range(ticks):
			frame_count += 1
			match checker_scroll_pattern & 15:
				1:
					if (frame_count & 3) == 3:
						scroll_v -= 1
					if (frame_count & 1) == 1:
						scroll_u += 1
				2:
					if (frame_count & 7) == 7:
						scroll_v -= 1
					if (frame_count & 3) == 3:
						scroll_u += 1
				3:
					if (frame_count & 0x15) == 0x15:
						scroll_v -= 1
					if (frame_count & 7) == 7:
						scroll_u += 1
				4:
					# Deterministic pseudo-random jitter
					var r: int = (frame_count * 1103515245 + 12345) & 0x7FFFFFFF
					scroll_v += 16 - (r & 31)
					scroll_u += 16 - ((r >> 5) & 31)
				5:
					# Sine wave oscillation
					var s: float = sin(float(frame_count) * 0.1)
					var c: float = cos(float(frame_count) * 0.1)
					scroll_v += int(s * 2.0)
					scroll_u += int(c * 2.0)
				0, _:
					if (frame_count & 1) == 1:
						scroll_v -= 1
					scroll_u += 1

	func process_input(pad_pressed: int) -> Dictionary:
		update_scroll_offsets(1)

		if mode_stage == STAGE_SELECT:
			if pad_pressed & PAD_UP:
				cursor = (cursor + 7) % 8
			elif pad_pressed & PAD_DOWN:
				cursor = (cursor + 1) % 8
			elif pad_pressed & PAD_LEFT:
				_adjust_item(cursor, -1)
			elif pad_pressed & PAD_RIGHT:
				_adjust_item(cursor, 1)
			elif pad_pressed & PAD_CIRCLE:
				if cursor == 6:
					mode_stage = STAGE_EXTRAS
				elif cursor == 7:
					mode_stage = STAGE_EXIT
					exited = true
					return {"action": "exit", "settings": get_settings()}
				else:
					_adjust_item(cursor, 1)
			elif pad_pressed & PAD_CROSS:
				mode_stage = STAGE_EXIT
				exited = true
				return {"action": "exit", "settings": get_settings()}
		elif mode_stage == STAGE_EXTRAS:
			if (pad_pressed & PAD_CROSS) or (pad_pressed & PAD_CIRCLE):
				mode_stage = STAGE_SELECT

		return {"action": "none", "cursor": cursor, "stage": mode_stage}

	func _adjust_item(item_idx: int, dir: int) -> void:
		match item_idx:
			0: # Text speed: 0..3
				text_speed = (text_speed + dir + 4) % 4
			1: # Icon mode: 0..1
				icon_mode = (icon_mode + dir + 2) % 2
			2: # Mouse settings: toggle
				mouse_enabled = not mouse_enabled
			3: # Window style: 0..2
				window_style = (window_style + dir + 3) % 3
			4: # Cursor speed: 0..2
				cursor_speed = (cursor_speed + dir + 3) % 3
			5: # Voice mode: 0..2
				voice_mode = (voice_mode + dir + 3) % 3
			6: # Scroll pattern toggle: 0..5
				checker_scroll_pattern = (checker_scroll_pattern + dir + 6) % 6

	func get_settings() -> Dictionary:
		return {
			"text_speed": text_speed,
			"icon_mode": icon_mode,
			"mouse_enabled": mouse_enabled,
			"mouse_speed": mouse_speed,
			"window_style": window_style,
			"cursor_speed": cursor_speed,
			"voice_mode": voice_mode,
			"sound_mode": sound_mode,
			"checker_scroll_pattern": checker_scroll_pattern,
			"button_confirm": button_confirm,
		}

	func render(presenter: VnPresenter) -> Image:
		var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

		# 1. Background checkerboard frame (hud_screen_frame_draw(1), outer border frame)
		presenter.draw_checkerboard_frame(target, 0, Vector2i(scroll_u, scroll_v))

		# 2. Top wooden parameter panel (parameter_show in home_screens.c:1567)
		var stats: Array[int] = [97, 43, 40, 40, 38, 33, 59, 4, 3]
		presenter.draw_parameter_panel(target, stats)

		# 3. Bottom message window & prompt: システム設定を変更します (home_screens.c:1566, 1596)
		presenter.draw_message_window(target)
		presenter._draw_text_hex_checked(target, STR_HEX["prompt_change_settings"], Vector2i(32, 170), 0, "message_window")

		# 4. Base Gouraud panel (cal_base_show in hud_screens.c:640: Rect2i(0, 33, 283, 125), OT 5)
		presenter.draw_cal_base(target, Rect2i(0, 33, 283, 125))

		# 5. Date pillar (hizuke_show in home_screens.c:1568, OT 5..4)
		presenter.draw_date_pillar(target, 95, 4, 9, 6, 1)

		# 6. Menu bar highlight for active item (menu_bar_show(0) at OT 3)
		var start_y: int = 56
		presenter.draw_menu_bar_highlight(target, Rect2i(36, start_y + cursor * 12, 96, 12), window_style)

		# 7. Notebook ring-binder & vertical divider (data_save_load_class_init in hud_screens.c:752-767:
		#    Actor 96, Frame 0x43 = 67 at (-16, -19), unk_01 = 3 -> OT 2)
		presenter._draw_actor(target, 67, Vector2i(-16, -19))

		# 8. 8 Setting items (settings_root_init / settings_labels_draw in home_screens.c:1584-1663, disp_string at OT 1 HEAD:
		#    x[i] = -0x7C -> 36, y[i] = -0x40 + i * 0xC -> 56 + i * 12, w[i] = 0x60 -> 96, h[i] = 0xC -> 12)
		var item_keys := [
			"opt_speed", "opt_icon", "opt_mouse", "opt_window",
			"opt_cursor", "opt_voice", "opt_mascot", "opt_settings_exit"
		]

		for i in range(8):
			if i == 6 and not mascot_unlocked:
				continue
			var sy: int = start_y + i * 12
			var sx: int = 36

			# Label (colour[i] = 3 normal)
			presenter._draw_text_hex_checked(target, STR_HEX[item_keys[i]], Vector2i(sx, sy), 3, "options_menu")

		# 9. Title badge: システム設定画面 (message_window_set_title("システム設定画面") in home_screens.c:1581:
		#    green fill at OT 2, badge frame + drop shadow at OT 1 TAIL drawn after disp_string)
		presenter.draw_title_badge(target, STR_HEX["title_system_settings"], Vector2i.ZERO, 8)

		# 10. Hand cursor pointing at active option slot (menu_cursor_update(0), Actor 0 at OT 0: item.x + item.w - 4, item.y + 8)
		var cur_y: int = start_y + cursor * 12 + 8
		presenter.draw_hand_cursor(target, Vector2i(36 + 96 - 4, cur_y))

		return target

	func _get_value_hex(item_idx: int) -> String:
		match item_idx:
			0: # Text speed
				return STR_HEX["val_fast"] if text_speed >= 2 else (STR_HEX["val_normal"] if text_speed == 1 else STR_HEX["val_slow"])
			1: # Icon mode
				return STR_HEX["val_style0"] if icon_mode == 0 else STR_HEX["val_style1"]
			2: # Mouse
				return STR_HEX["val_on"] if mouse_enabled else STR_HEX["val_off"]
			3: # Window style
				match window_style:
					1: return STR_HEX["val_style1"]
					2: return STR_HEX["val_style2"]
					_: return STR_HEX["val_style0"]
			4: # Cursor speed
				return STR_HEX["val_fast"] if cursor_speed >= 2 else (STR_HEX["val_normal"] if cursor_speed == 1 else STR_HEX["val_slow"])
			5: # Voice mode
				return STR_HEX["val_on"] if voice_mode > 0 else STR_HEX["val_off"]
			_:
				return ""

# ==============================================================================
# 2. Online Help Overlay (OLH)
# ==============================================================================

class HelpOverlay extends RefCounted:
	const STAGE_MENU_INIT   := 0x00
	const STAGE_MENU_INPUT  := 0x01
	const STAGE_RETURN      := 0x02
	const STAGE_SYSTEM      := 0x10
	const STAGE_MEMCARD     := 0x20
	const STAGE_FILESEL     := 0x30
	const STAGE_BIORHYTHM   := 0x40
	const STAGE_ICONS       := 0x50
	const STAGE_SUBGAME     := 0x60
	const STAGE_TIPS        := 0x70

	var mode_stage: int = STAGE_MENU_INPUT
	var cursor: int = 0 # 0..7
	var topic_index: int = 0
	var exited: bool = false

	func _init() -> void:
		mode_stage = STAGE_MENU_INPUT
		cursor = 0
		topic_index = 0
		exited = false

	func process_input(pad_pressed: int) -> Dictionary:
		# Start button at any time immediately returns to game (help_questions_and_answers_page.c:39-41)
		if pad_pressed & PAD_START:
			mode_stage = STAGE_RETURN
			exited = true
			return {"action": "return_to_game"}

		if mode_stage == STAGE_MENU_INPUT:
			if pad_pressed & PAD_UP:
				cursor = (cursor + 7) % 8
			elif pad_pressed & PAD_DOWN:
				cursor = (cursor + 1) % 8
			elif pad_pressed & PAD_CIRCLE:
				match cursor:
					0: mode_stage = STAGE_SYSTEM
					1: mode_stage = STAGE_MEMCARD
					2: mode_stage = STAGE_FILESEL
					3: mode_stage = STAGE_BIORHYTHM
					4: mode_stage = STAGE_ICONS
					5: mode_stage = STAGE_SUBGAME
					6: mode_stage = STAGE_TIPS
					7:
						mode_stage = STAGE_RETURN
						exited = true
						return {"action": "return_to_game"}
				topic_index = 0
				return {"action": "enter_topic", "topic": mode_stage}
			elif pad_pressed & PAD_CROSS:
				mode_stage = STAGE_RETURN
				exited = true
				return {"action": "return_to_game"}
		else:
			# In topic page: Cross or Circle returns to category menu
			if (pad_pressed & PAD_CROSS) or (pad_pressed & PAD_CIRCLE):
				mode_stage = STAGE_MENU_INPUT
				return {"action": "back_to_menu"}

		return {"action": "none", "cursor": cursor, "stage": mode_stage}

	func render(presenter: VnPresenter) -> Image:
		var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

		# 1. Clear framebuffer to 0xA0A0A0 (back_clear_switch(1) with clear_color_r=g=b=0xA0 in help_category_menu_draw:57-58)
		target.fill(Color8(0xA0, 0xA0, 0xA0, 255))

		# 2. Help background gradient rect (help_background_gradient_draw: prim_gradient_rect(-0x90, -0x58, 0x120, 0xC4, 4, 0xA0A0A0, 2); dtd_on(4);)
		presenter.draw_cal_base(target, Rect2i(16, 32, 288, 196), 0xA0)

		# 3. Title badge: Ｑ＆Ａコーナー at settled origin (0, 0) -> (16, 30) (message_window_set_title("Ｑ＆Ａコーナー") in help_category_menu_draw:59)
		presenter.draw_title_badge(target, STR_HEX["title_olh"], Vector2i.ZERO, 7)

		# 4. Bottom prompt: スタートボタンでゲームに戻れます at (-0x68, 0x5C) -> (56, 212), attribute 2 (help_category_menu_draw:60)
		presenter._draw_text_hex_checked(target, STR_HEX["prompt_start_return"], Vector2i(56, 212), 2, "help_screen")

		if mode_stage == STAGE_MENU_INPUT or mode_stage == STAGE_MENU_INIT:
			# Top-right prompt: 項目を選んでください at (-0x10, -0x50) -> (144, 40), attribute 0xF (help_category_menu_draw:70)
			presenter._draw_text_hex_checked(target, STR_HEX["prompt_select_item"], Vector2i(144, 40), 15, "help_menu")

			# Category Menu View
			var cat_keys := [
				"olh_cat_system", "olh_cat_memcard", "olh_cat_filesel", "olh_cat_biorhythm",
				"olh_cat_icons", "olh_cat_subgame", "olh_cat_tips", "olh_cat_return"
			]

			# 8 categories matching help_category_menu_draw:62-68 (a[i]=-0x60->64, b[i]=i*0x10-0x3C->60+i*16, c[i]=0x80->128, d[i]=0x10->16, attr=0xF)
			for i in range(8):
				var sx: int = 64
				var sy: int = 60 + i * 16

				if i == cursor:
					presenter.draw_menu_bar_highlight(target, Rect2i(sx, sy, 128, 16), 0)

				presenter._draw_text_hex_checked(target, STR_HEX[cat_keys[i]], Vector2i(sx, sy), 15, "help_menu")

			# Hand cursor pointing at active category (menu_cursor_update(0): a[cursor]+c[cursor]-4 = 188, b[cursor]+8 = 68+cursor*16)
			var cur_sy: int = 60 + cursor * 16 + 8
			presenter.draw_hand_cursor(target, Vector2i(64 + 128 - 4, cur_sy))
		else:
			# Topic Detail View (help_system_settings_menu_draw:47-55: header at (-0x50, -0x40) -> (80, 56), attr 0xF)
			var topic_title_key := "olh_cat_system"
			match mode_stage:
				STAGE_MEMCARD: topic_title_key = "olh_cat_memcard"
				STAGE_FILESEL: topic_title_key = "olh_cat_filesel"
				STAGE_BIORHYTHM: topic_title_key = "olh_cat_biorhythm"
				STAGE_ICONS: topic_title_key = "olh_cat_icons"
				STAGE_SUBGAME: topic_title_key = "olh_cat_subgame"
				STAGE_TIPS: topic_title_key = "olh_cat_tips"

			presenter._draw_text_hex_checked(target, STR_HEX[topic_title_key], Vector2i(80, 56), 15, "help_topic")
			presenter._draw_text_hex_checked(target, STR_HEX["prompt_select_item"], Vector2i(64, 70), 15, "help_topic")

		return target

# ==============================================================================
# 3. Title Screen / Opening Menu Overlay (ETC)
# ==============================================================================

class TitleMenuOverlay extends RefCounted:
	const STAGE_SETUP   := 0
	const STAGE_ATTRACT := 1
	const STAGE_MENU    := 2
	const STAGE_CONFIRM := 3

	const MODE_FILE_SELECT := 0x35
	const MODE_OPTION      := 0x36
	const MODE_OPENING     := 0x01

	var stage: int = STAGE_MENU
	var cursor: int = 0 # 0=Start/File Select, 1=Options
	var confirm_timer: int = 0
	var attract_timer: int = 0
	var dispatched_mode: int = 0

	func _init() -> void:
		stage = STAGE_MENU
		cursor = 0
		confirm_timer = 0
		attract_timer = 0
		dispatched_mode = 0

	func process_input(pad_pressed: int) -> Dictionary:
		if stage == STAGE_MENU:
			attract_timer += 1
			# Select button or timeout -> back to opening attract
			if (pad_pressed & PAD_SELECT) or attract_timer >= 600:
				stage = STAGE_ATTRACT
				dispatched_mode = MODE_OPENING
				return {"action": "dispatch", "mode": MODE_OPENING}

			if (pad_pressed & PAD_UP) or (pad_pressed & PAD_DOWN):
				cursor = 1 - cursor
				attract_timer = 0
			elif pad_pressed & (PAD_CIRCLE | PAD_CROSS):
				stage = STAGE_CONFIRM
				confirm_timer = 0
				attract_timer = 0
		elif stage == STAGE_CONFIRM:
			confirm_timer += 1
			if confirm_timer >= 15:
				dispatched_mode = MODE_FILE_SELECT if cursor == 0 else MODE_OPTION
				return {"action": "dispatch", "mode": dispatched_mode}

		return {"action": "none", "cursor": cursor, "stage": stage}

	static func _load_first_existing_image(rel_path: String) -> Image:
		var candidates := [
			"/workspace/" + rel_path,
			"/work/" + rel_path,
			ProjectSettings.globalize_path("res://../../" + rel_path).simplify_path(),
		]
		for p in candidates:
			if FileAccess.file_exists(p):
				return Image.load_from_file(p)
		return null

	static func _blit_alpha(dst: Image, src: Image, pos: Vector2i) -> void:
		if dst == null or src == null:
			return
		var sw: int = src.get_width()
		var sh: int = src.get_height()
		for sy in range(sh):
			var dy: int = pos.y + sy
			if dy < 0 or dy >= 240:
				continue
			for sx in range(sw):
				var dx: int = pos.x + sx
				if dx < 0 or dx >= 320:
					continue
				var c: Color = src.get_pixel(sx, sy)
				if c.a > 0.05:
					dst.set_pixel(dx, dy, Color8(c.r8, c.g8, c.b8, 255))

	func render(presenter: VnPresenter) -> Image:
		var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

		# 1. Title Menu White Clear Color (title_menu_setup_step in src/main/title_screen.c:327:
		#    clear_color_r = clear_color_g = clear_color_b = 0xFF)
		target.fill(Color8(255, 255, 255, 255))

		# 2. Extracted TITLE.NBN sprites (title_menu_setup_step in src/main/title_screen.c:333-351
		#    + .asset-cache/media/title-reviewed-final/metadata.json):
		#    actor_pool[96]: frame 0 (288x146 logo + copyright, bbox=(16,28)) at (x=-0xA0, y=-0x78) -> (16, 28)
		#    actor_pool[97]: frame 1 (144x16, bbox_x=112) / frame 2 (112x16, bbox_x=120) at x=-0xAA (-10) -> (102/110, 180)
		#    actor_pool[98]: frame 3 (144x16, bbox_x=112) / frame 4 (80x16, bbox_x=120) at x=-0xAA (-10), y=-0x78 (0) -> (102/110, 200)
		var logo_img := _load_first_existing_image(".asset-cache/media/title-reviewed-final/title-slice.frame-00.png")
		var start_img := _load_first_existing_image(
			".asset-cache/media/title-reviewed-final/title-slice.frame-01.png" if cursor == 0 else ".asset-cache/media/title-reviewed-final/title-slice.frame-02.png"
		)
		var opt_img := _load_first_existing_image(
			".asset-cache/media/title-reviewed-final/title-slice.frame-03.png" if cursor == 1 else ".asset-cache/media/title-reviewed-final/title-slice.frame-04.png"
		)
		if logo_img != null and start_img != null and opt_img != null:
			_blit_alpha(target, logo_img, Vector2i(16, 28))
			_blit_alpha(target, start_img, Vector2i(102 if cursor == 0 else 110, 180))
			_blit_alpha(target, opt_img, Vector2i(102 if cursor == 1 else 110, 200))
		else:
			presenter._draw_text_hex_checked(target, STR_HEX["title_logo"], Vector2i(76, 52), 4, "title_screen")
			var menu_y := [180, 200]
			for i in range(2):
				var my: int = menu_y[i]
				var mx: int = 110
				if i == cursor:
					presenter.draw_menu_bar_highlight(target, Rect2i(mx, my, 160, 20), 0)
				var text_key := "menu_new_game" if i == 0 else "menu_option"
				presenter._draw_text_hex_checked(target, STR_HEX[text_key], Vector2i(mx, my), 3, "title_screen")

		# 3. Hand cursor pointing at active item (title_menu_cursor_update in title_screen.c:357-358: cursor_x=0x40->224)
		var cur_y: int = 184 + cursor * 20
		presenter.draw_hand_cursor(target, Vector2i(224, cur_y))

		return target

# ==============================================================================
# Overlay Factory Methods
# ==============================================================================

static func create_options() -> OptionsOverlay:
	return OptionsOverlay.new()

static func create_help() -> HelpOverlay:
	return HelpOverlay.new()

static func create_title_menu() -> TitleMenuOverlay:
	return TitleMenuOverlay.new()
