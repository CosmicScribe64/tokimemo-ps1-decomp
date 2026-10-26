extends RefCounted
class_name SystemOverlays
## T-29018: System Overlays implementation for:
## 1. OPTION (src/ovl/options/settings_sound_test_and_extras.c)
## 2. OLH (src/ovl/help/help_questions_and_answers_page.c)
## 3. ETC (src/ovl/misc_scenes/ & src/main/title_screen.c)
##
## Provides complete state machines, settings persistence, input handling,
## and authentic 320x240 PS1 rendering through VnScenePresenter.

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
	# Options Overlay
	"title_options": "83498376835683878393", # オプション
	"opt_speed": "95b68e9a955c8ea691ac9378", # 文字表示速度
	"opt_icon": "83418343835283938382815b8368", # アイコンモード
	"opt_mouse": "837d8345835890dd92e8", # マウス設定
	"opt_window": "8381836a8385815b8382815b8368", # メニューモード
	"opt_cursor": "834a815b835c838b91ac9378", # カーソル速度
	"opt_voice": "89b990ba8382815b8368", # 音声モード
	"opt_extras": "82a882dc82af", # おまけ
	"opt_exit": "834983768356838783938f4997b9", # オプション終了

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
	var sound_mode: int = 0         # 0=Stereo, 1=Mono
	var checker_scroll_pattern: int = 0 # 0..5 (player_flags.b[1] & 15)
	var button_confirm: int = 0     # 0=Circle, 1=Cross

	# Overlay state machine
	var mode_stage: int = STAGE_SELECT
	var cursor: int = 0             # 0..7
	var exited: bool = false
	var frame_count: int = 0
	var scroll_u: int = 0
	var scroll_v: int = 0

	func _init() -> void:
		mode_stage = STAGE_SELECT
		cursor = 0
		exited = false
		frame_count = 0
		scroll_u = 0
		scroll_v = 0

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

		# 1. Background checkerboard frame (Mode 0, full screen)
		presenter.draw_checkerboard_frame(target, 0, Vector2i(scroll_u, scroll_v))

		# 2. Base Gouraud panel (cal_base_show in hud_screens.c:640: Rect2i(20, 20, 280, 200))
		presenter.draw_cal_base(target, Rect2i(20, 20, 280, 200))

		# 3. Title badge: オプション (OT 2)
		presenter.draw_title_badge(target, STR_HEX["title_options"], Vector2i(0, -14), 5)

		# 4. 8 Option items
		var item_keys := [
			"opt_speed", "opt_icon", "opt_mouse", "opt_window",
			"opt_cursor", "opt_voice", "opt_extras", "opt_exit"
		]

		var start_y: int = 56
		for i in range(8):
			var sy: int = start_y + i * 18
			var sx: int = 40

			# Menu bar highlight for active item
			if i == cursor:
				presenter.draw_menu_bar_highlight(target, Rect2i(sx - 4, sy - 2, 248, 16), window_style)

			# Label
			presenter._draw_text_hex_checked(target, STR_HEX[item_keys[i]], Vector2i(sx, sy), 0, "options_menu")

			# Value indicator
			var val_hex: String = _get_value_hex(i)
			if not val_hex.is_empty():
				presenter._draw_text_hex_checked(target, val_hex, Vector2i(190, sy), 0, "options_menu")

		# 5. Hand cursor pointing at active option slot
		var cur_y: int = start_y + cursor * 18 + 8
		presenter.draw_hand_cursor(target, Vector2i(14, cur_y))

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

		# 1. Background checkerboard frame (Mode 0, full screen)
		presenter.draw_checkerboard_frame(target, 0, Vector2i.ZERO)

		# 2. Cal base panel (Rect2i(24, 32, 272, 128))
		presenter.draw_cal_base(target, Rect2i(24, 32, 272, 128))

		# 3. Title badge: Ｑ＆Ａコーナー
		presenter.draw_title_badge(target, STR_HEX["title_olh"], Vector2i(0, -14), 7)

		# 4. Message window at bottom (Frame 62, OT 8..7)
		presenter.draw_message_window(target)

		# 5. Bottom prompt: スタートボタンでゲームに戻れます
		presenter._draw_text_hex_checked(target, STR_HEX["prompt_start_return"], Vector2i(40, 184), 0, "message_window")

		if mode_stage == STAGE_MENU_INPUT or mode_stage == STAGE_MENU_INIT:
			# Category Menu View
			var cat_keys := [
				"olh_cat_system", "olh_cat_memcard", "olh_cat_filesel", "olh_cat_biorhythm",
				"olh_cat_icons", "olh_cat_subgame", "olh_cat_tips", "olh_cat_return"
			]

			# 8 categories matching help_category_menu_draw coordinates (a[i]=-0x60 -> 64, b[i]=60+i*16)
			for i in range(8):
				var sx: int = 64
				var sy: int = 40 + i * 14

				if i == cursor:
					presenter.draw_menu_bar_highlight(target, Rect2i(sx - 4, sy - 1, 140, 13), 0)

				presenter._draw_text_hex_checked(target, STR_HEX[cat_keys[i]], Vector2i(sx, sy), 0, "help_menu")

			# Hand cursor pointing at active category
			var cur_sy: int = 40 + cursor * 14 + 6
			presenter.draw_hand_cursor(target, Vector2i(38, cur_sy))
		else:
			# Topic Detail View
			var topic_title_key := "olh_cat_system"
			match mode_stage:
				STAGE_MEMCARD: topic_title_key = "olh_cat_memcard"
				STAGE_FILESEL: topic_title_key = "olh_cat_filesel"
				STAGE_BIORHYTHM: topic_title_key = "olh_cat_biorhythm"
				STAGE_ICONS: topic_title_key = "olh_cat_icons"
				STAGE_SUBGAME: topic_title_key = "olh_cat_subgame"
				STAGE_TIPS: topic_title_key = "olh_cat_tips"

			presenter._draw_text_hex_checked(target, STR_HEX[topic_title_key], Vector2i(40, 48), 0, "help_topic")
			presenter._draw_text_hex_checked(target, STR_HEX["prompt_select_item"], Vector2i(40, 72), 0, "help_topic")

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

	func render(presenter: VnPresenter) -> Image:
		var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

		# 1. Background checkerboard frame (Mode 1, outer border)
		presenter.draw_checkerboard_frame(target, 1, Vector2i.ZERO)

		# 2. Base Gouraud panel for center display
		presenter.draw_cal_base(target, Rect2i(32, 28, 256, 184))

		# 3. Title logo / headline: ときめきメモリアル
		presenter._draw_text_hex_checked(target, STR_HEX["title_logo"], Vector2i(76, 52), 0, "title_screen")

		# 4. Menu options: はじめから (Start/Continue) and オプション (Option)
		# Coordinates matching title_menu_setup_step: y[0]=0x42 (186), y[1]=0x52 (202)
		var menu_y := [120, 150]

		for i in range(2):
			var my: int = menu_y[i]
			var mx: int = 112

			if i == cursor:
				# Highlight active choice bar
				presenter.draw_menu_bar_highlight(target, Rect2i(mx - 8, my - 2, 112, 18), 0)

			var text_key := "menu_new_game" if i == 0 else "menu_option"
			presenter._draw_text_hex_checked(target, STR_HEX[text_key], Vector2i(mx, my), 0, "title_screen")

		# 5. Hand cursor pointing at active item
		var cur_y: int = menu_y[cursor] + 6
		presenter.draw_hand_cursor(target, Vector2i(90, cur_y))

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
