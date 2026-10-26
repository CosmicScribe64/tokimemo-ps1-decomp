extends "res://presentation/command_hud_presenter.gd"
## T-29012: Authentic 320x240 PS1 Prologue Visual Presentation (`GAME_MODE_PROLOGUE` = `0x15`).
##
## Implements exact decomp parity with `src/ovl/name_entry/prologue.c`,
## `src/main/hud_panels.c`, `src/main/menu_speaker_girl.c`, and `src/main/hud_screens.c`:
## 1. Outer frame loop (`prologue_mode_handler`, `src/ovl/name_entry/prologue.c:2812-2818`):
##    - `message_window_title_update()` (`line 2812`)
##    - `hud_screen_frame_draw(0)` when `mode_stage == 0` (`line 2814`) vs
##      `hud_screen_frame_draw(2)` when `mode_stage != 0` (`line 2816`)
##    - `message_window_show()` (`line 2818`)
## 2. Seasonal 32x32 checkerboard border (`hud_screen_frame_draw`, `src/main/hud_panels.c:1366-1461`):
##    - Exact 10x8 tile enable mask for `a == 0`, `a == 1`, `a == 2`
##    - Exact UV scroll `(D_800B5BCC & 31) + 128`, `(D_800B5BD0 & 31) + 128`
## 3. Calendar date pillar (`hizuke_init` / `hizuke_show` / `hizuke_disp_switch`,
##    `src/main/hud_panels.c:640-739`, `src/ovl/name_entry/prologue.c:215,220,308,364,371`):
##    - Displayed in Stage 0 (`1995-04-04` Tuesday `95 4/4 火`) and Stage 1 Step 0 substep 0;
##    - Hidden (`hizuke_disp_switch(0)`) from Stage 1 Step 0 substep 1 through Stage 21.
##    - Queued at OT 5 (gradient) and OT 4 (Frame 60 + digits), rendering in front of
##      `message_window_show()` (OT 8 gradient / OT 7 Frame 62).
## 4. Top-left `"プロローグ"` title badge (`message_window_set_title` / `message_window_title_update`,
##    `src/ovl/name_entry/prologue.c:209`, `src/main/hud_panels.c:1173-1276`):
##    - 5 glyphs (`"プロローグ"`, `8376838d838d815b834f`), `Rect2i(16, 30, 78, 20)` gradient at OT 2,
##      wooden cap/segment actors `3..9` (`0x73`, `0x72`x5, `0x71`) with `STP=1` shadow at OT 1,
##      and slide offset curve driven by `D_80125CA0` (`-64..184`) and `PAD_TRIANGLE`.
## 5. Dialogue formatting, name expansion, and 3-row scroll (`prologue_line_start`,
##    `prologue_line_run`, `gnsx`, `sndisp`, `sndi`, `get_p_name`,
##    `src/ovl/name_entry/prologue.c:84-180`, `src/main/menu_speaker_girl.c:522-843`):
##    - Speaker centering via `gnsx` (`x = 46, 39, 32` at `y = 170`)
##    - Block row 0 bracket prefix (`『` for `0x0F`, `（` for `0x1F`, none for `0x2F`, `「` otherwise) at `x = 74`
##    - Subsequent block rows at `x = 88`, 3 visible rows at `y = 170, 186, 202`, glyph advance `14` (`0x0E`).

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"

# Canonical unshadowed 32x32 spring checkerboard texture at VRAM (128..159, 128..159)
# normalized to (D_800B5BCC & 31) == 0, (D_800B5BD0 & 31) == 0 (hud_panels.c:1449-1450).
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA8ElEQVR4nM2WoRHDMAxFX3MBhoWBGaWjFHaEwo6SMQIDO0KgRyg0a1nuco6lnO2TKyrL76EvXSY/fRFqvI48Vi89EWudF6b7Ldnvsn+uVKKAc67o8xBCvoAFPClgBT8UsIRHAtbwnUAL+CbQCg7QtYRDYQ5ocK3vnMvPgRpwyMyBWvBDAUt4JGANB+gHN4hD/uNZ50V8I84/X/jgk/1e2/XaPlcFghfvif++B0pD5kzIJQUs4EkBK/ihgCU8ErCG7wRawDeBVnCAriUcCnNAg5/ZLdk5UAMOmTlQC34oYAmPBKzhAL1f3uKQts+10u6JH97tla2x++MhAAAAAElFTkSuQmCC"

const STR_HEX := {
	"title_prologue": "8376838d838d815b834f", # プロローグ
	"speaker_fukuhara": "959f8cb4", # 福原
	"speaker_fujisaki": "93a18de8", # 藤崎
	"speaker_yoshio": "8d449759", # 好雄
	"speaker_rei": "838c8343", # レイ
	"speaker_girl_a": "8f978e718260", # 女子Ａ
	"speaker_girl_b": "8f978e718261", # 女子Ｂ
	"given_masaaki": "89eb8fbb", # 雅晶
	"suffix_kun": "82ad82f1", # くん
	"suffix_san": "82b382f1", # さん
	"bracket_double_open": "8177", # 『
	"bracket_paren_open": "8169", # （
	"bracket_quote_open": "8175", # 「
	# Line 0 (prologue_line_text_00, speaker 0x0F):
	# "俺の名前はP。 ここ、私立きらめき高校の 新入生だ。"
	"line0_row0_prefix": "89b482cc96bc914f82cd", # 俺の名前は
	"line0_row0_suffix": "8142", # 。
	"line0_row1": "82b182b181418e8497a782ab82e782df82ab8d828d5a82cc", # ここ、私立きらめき高校の
	"line0_row2": "905693fc90b682be8142", # 新入生だ。
	# Line 1 (prologue_line_text_01, speaker 0x0F):
	# "無理だと思っていた 第一志望だったが、 運良く合格することができた。"
	"line1_row0": "96b3979d82be82c68e7682c182c482a282bd", # 無理だと思っていた
	"line1_row1": "91e688ea8e75965d82be82c182bd82aa8141", # 第一志望だったが、
	"line1_row2": "895e97c782ad8d878a6982b782e982b182c682aa82c582ab82bd8142", # 運良く合格することができた。
	# Line 18 (prologue_line_text_18, speaker 0x00 = 藤崎):
	# "みなさんはじめまして。 名前は藤崎詩織です。"
	"line18_row0": "82dd82c882b382f182cd82b682df82dc82b582c48142", # みなさんはじめまして。
	"line18_row1": "96bc914f82cd93a18de88e8d904482c582b78142", # 名前は藤崎詩織です。
	# Line 19 (prologue_line_text_19, speaker 0x00 = 藤崎):
	# "趣味は音楽鑑賞で、 クラシックとかを 良く聴きます。 みなさんこれから、 よろしくお願いします。"
	"line19_row0": "8eef96a182cd89b98a798ad38fdc82c58141", # 趣味は音楽鑑賞で、
	"line19_row1": "834e838983568362834e82c682a982f0", # クラシックとかを
	"line19_row2": "97c782ad92ae82ab82dc82b78142", # 良く聴きます。
	"line19_row3": "82dd82c882b382f182b182ea82a982e78141", # みなさんこれから、
	"line19_row4": "82e682eb82b582ad82a88ae882a282b582dc82b78142", # よろしくお願いします。
}

var _checker_tile: Image
var _scene_plates: Dictionary = {}
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []

func init_resources(font_root: String, hud_manifest_path: String, scene_plates_dir: String = "") -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	_scene_plates.clear()

	_checker_tile = Image.new()
	var err: Error = _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64))
	if err != OK:
		return "Failed to decode canonical checkerboard tile"

	if not open_manifest(hud_manifest_path, HUD_SHA, font_root):
		return "Failed to open HUD manifest: " + error

	if not scene_plates_dir.is_empty():
		var plate_files := {
			"stage0_arrival": "stage0_arrival.png",
			"stage1_board": "stage1_board.png",
			"stage13_shiori_line18": "stage13_shiori_line18.png",
			"stage14_shiori_line19": "stage14_shiori_line19.png",
		}
		for key: String in plate_files.keys():
			var full_path: String = scene_plates_dir.path_join(plate_files[key])
			if FileAccess.file_exists(full_path):
				var img := Image.load_from_file(full_path)
				if img == null or img.is_empty():
					return "Failed to load scene plate: " + full_path
				_scene_plates[key] = img

	_ensure_compositor_coverage()
	return ""

func _ensure_compositor_coverage() -> void:
	if _compositor != null and _compositor.last_coverage == null:
		_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
		_compositor.last_coverage.fill(Color.BLACK)

# ==============================================================================
# Decomp Parity Rules & State Queries
# ==============================================================================

## Outer frame loop (`prologue_mode_handler`, `src/ovl/name_entry/prologue.c:2813-2817`):
## Returns `0` when `mode_stage == 0`, else `2`.
static func get_hud_screen_frame_arg(mode_stage: int) -> int:
	return 0 if mode_stage == 0 else 2

## Exact 10x8 tile enable mask from `hud_screen_frame_draw(a)` (`src/main/hud_panels.c:1412-1438`).
static func is_checker_tile_enabled(frame_arg: int, col: int, row: int) -> bool:
	if col < 0 or col >= 10 or row < 0 or row >= 8:
		return false
	match frame_arg:
		0:
			return true
		1:
			if row >= 1 and row < 6 and col >= 1 and col < 9:
				return false
			return true
		2:
			if row < 5:
				return false
			if row == 5:
				return col == 0 or col == 9
			return true
		_:
			return true

## Exact `hizuke_show()` visibility across Prologue stages (`src/ovl/name_entry/prologue.c:220,308,364,371,378`):
## - Stage 0 (`prologue_mode_stage_arrival`): `hizuke_disp_switch(1)` set at substep 0 (`line 220`), `hizuke_show()` called (`line 308`).
## - Stage 1 (`prologue_board_load`): `hizuke_show()` called at Step 0 substep 0 (`line 364`), then `hizuke_disp_switch(0)` called when CD read completes at Step 0 substep 1 (`line 371`).
## - Stages 1 (Step >= 1) through 21: hidden (`false`).
static func is_hizuke_displayed(mode_stage: int, mode_step: int = 0, substep: int = 0) -> bool:
	if mode_stage == 0:
		return not (mode_step == 0)
	if mode_stage == 1 and mode_step == 0 and substep == 0:
		return true
	return false

## Exact `message_window_title_update()` slide state (`src/main/hud_panels.c:1207-1276`).
## Returns `{"visible": bool, "offset": Vector2i, "next_timer": int, "rect": Rect2i, "text_pos": Vector2i}`.
static func compute_title_badge_state(title_timer: int, pad_held_triangle: bool = false, n_glyphs: int = 5) -> Dictionary:
	var next_timer: int = title_timer
	var offset := Vector2i(-64, -64)
	if title_timer < 0:
		next_timer = title_timer + 1
		offset = Vector2i(next_timer, next_timer)
	elif title_timer < 120:
		next_timer = title_timer + 1
		offset = Vector2i.ZERO
	elif title_timer < 184:
		next_timer = title_timer + 1
		var slide: int = 120 - next_timer
		offset = Vector2i(slide, slide)
	elif pad_held_triangle:
		offset = Vector2i.ZERO
	else:
		offset = Vector2i(-64, -64)

	var rect := Rect2i(16 + offset.x, 30 + offset.y, n_glyphs * 14 + 8, 20)
	var text_pos := Vector2i(18 + offset.x, 32 + offset.y)
	var visible: bool = (rect.position.x + rect.size.x > 0 and rect.position.y + rect.size.y > 0)
	return {
		"visible": visible,
		"offset": offset,
		"next_timer": next_timer,
		"rect": rect,
		"text_pos": text_pos,
	}

## Exact speaker name screen position from `gnsx(speaker_name)` (`src/main/menu_speaker_girl.c:522-535`).
static func get_speaker_screen_pos(speaker_hex: String) -> Vector2i:
	var byte_len: int = speaker_hex.length() / 2
	match byte_len:
		2:
			return Vector2i(160 - 0x72, 120 + 0x32) # (46, 170)
		4:
			return Vector2i(160 - 0x79, 120 + 0x32) # (39, 170)
		_:
			return Vector2i(160 - 0x80, 120 + 0x32) # (32, 170)

## Exact `sndi` row decoration and screen coordinates (`src/main/menu_speaker_girl.c:653-688`).
static func format_sndi_row(row_hex: String, block_row_index: int, speaker_style: int) -> Dictionary:
	var decorated_hex: String = row_hex
	var screen_x: int = 160 - 0x48 # 88
	if block_row_index == 0:
		if speaker_style == 0x0F:
			decorated_hex = STR_HEX["bracket_double_open"] + row_hex
		elif speaker_style == 0x1F:
			decorated_hex = STR_HEX["bracket_paren_open"] + row_hex
		elif speaker_style == 0x2F:
			decorated_hex = row_hex
		else:
			decorated_hex = STR_HEX["bracket_quote_open"] + row_hex
		screen_x = (160 - 0x80) if speaker_style == 0xFF else (160 - 0x56) # 32 or 74
	else:
		screen_x = (160 - 0x80) if speaker_style == 0xFF else (160 - 0x48) # 32 or 88
	return {
		"hex": decorated_hex,
		"x": screen_x,
	}

## Builds the 3 visible dialogue rows for `prologue_line_start` / `prologue_line_run`
## (`src/ovl/name_entry/prologue.c:84-180`, `src/main/menu_speaker_girl.c:538-689`).
static func build_visible_dialogue_rows(block_rows_hex: Array[String], speaker_style: int, first_visible_row: int = 0) -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	var y_slots: Array[int] = [120 + 0x32, 120 + 0x42, 120 + 0x52] # 170, 186, 202
	for slot_idx in range(3):
		var row_idx: int = first_visible_row + slot_idx
		if row_idx < 0 or row_idx >= block_rows_hex.size():
			break
		var formatted: Dictionary = format_sndi_row(block_rows_hex[row_idx], row_idx, speaker_style)
		result.append({
			"block_row": row_idx,
			"hex": formatted["hex"],
			"pos": Vector2i(int(formatted["x"]), y_slots[slot_idx]),
		})
	return result

## Exact `hud_text_marker_set` / `hud_text_marker_bob` (`src/main/hud_screens.c:1740-1768`).
static func compute_text_marker_state(enabled: bool, current_y: int = 0x48, frame_count: int = 0) -> Dictionary:
	var next_y: int = current_y
	if current_y < 0x4F:
		if (frame_count & 3) == 0:
			next_y = current_y + 1
	else:
		next_y = 0x48
	return {
		"actor_index": 2,
		"enabled": enabled,
		"sprite_id": 0xF0,
		"x": 0x74, # 116 -> screen 276
		"y": next_y, # 72..79 -> screen 192..199
		"screen_pos": Vector2i(160 + 0x74, 120 + next_y),
	}

# ==============================================================================
# Frame Rendering Entrypoints
# ==============================================================================

## Renders a complete 320x240 Prologue frame matching PS1 hardware OT layering.
func render_prologue_frame(config: Dictionary) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	var mode_stage: int = int(config.get("mode_stage", 0))
	var mode_step: int = int(config.get("mode_step", 2 if mode_stage == 0 else 0))
	var substep: int = int(config.get("substep", 1))
	var checker_x: int = int(config.get("checker_x", 0))
	var checker_y: int = int(config.get("checker_y", 0))
	var frame_arg: int = int(config.get("hud_frame_arg", get_hud_screen_frame_arg(mode_stage)))

	# 1. OT 15: hud_screen_frame_draw(frame_arg) (src/main/hud_panels.c:1366-1461)
	_draw_checkerboard_frame(target, frame_arg, checker_x, checker_y)

	# 2. OT 9..13: 320x160 scene background + upper scene actors (dec_bg_draw(1) + actors 96..103)
	var plate_key: String = str(config.get("scene_plate_key", ""))
	if not plate_key.is_empty() and _scene_plates.has(plate_key):
		var plate_img: Image = _scene_plates[plate_key]
		_blit_opaque_region(target, plate_img, Vector2i.ZERO, Rect2i(0, 0, 320, 160))
	elif config.get("scene_plate") is Image:
		_blit_opaque_region(target, config["scene_plate"], Vector2i.ZERO, Rect2i(0, 0, 320, 160))

	# 3. OT 8 & OT 7: message_window_show() (src/main/hud_panels.c:760-766)
	# Drawn BEFORE hizuke_show() (OT 5/4) and title badge (OT 2/1) per PS1 OT traversal order.
	_draw_message_window(target)

	# 4. OT 5 & OT 4: hizuke_show() (src/main/hud_panels.c:640-739, prologue.c:308)
	var show_hizuke: bool = bool(config.get("show_hizuke", is_hizuke_displayed(mode_stage, mode_step, substep)))
	if show_hizuke:
		var year: int = int(config.get("year", 95))
		var month: int = int(config.get("month", 4))
		var day: int = int(config.get("day", 4))
		var weekday: int = int(config.get("weekday", 1)) # 1 = Tuesday (火)
		var day_kind: int = int(config.get("day_kind", 0))
		_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# 5. OT 2 & OT 1: message_window_title_update() ("プロローグ", src/main/hud_panels.c:1173-1276)
	var title_timer: int = int(config.get("title_timer", 184))
	var pad_triangle: bool = bool(config.get("pad_held_triangle", false))
	var title_state: Dictionary = compute_title_badge_state(title_timer, pad_triangle, 5)
	if bool(title_state["visible"]):
		_draw_title_badge(target, STR_HEX["title_prologue"], title_state["offset"], 5)

	# 6. OT 0: Dialogue speaker (gnsx) and 3-row scrolled text (sndisp / sndi / prologue_line_run)
	var speaker_hex: String = str(config.get("speaker_hex", ""))
	if not speaker_hex.is_empty():
		var speaker_pos: Vector2i = get_speaker_screen_pos(speaker_hex)
		_draw_text_hex_checked(target, speaker_hex, speaker_pos, 0, "message_window")

	var block_rows_hex: Array[String] = []
	for r in config.get("block_rows_hex", []):
		block_rows_hex.append(str(r))
	var speaker_style: int = int(config.get("speaker_style", 0x0F))
	var first_visible_row: int = int(config.get("first_visible_row", 0))
	var visible_rows: Array[Dictionary] = build_visible_dialogue_rows(block_rows_hex, speaker_style, first_visible_row)
	for row_dict: Dictionary in visible_rows:
		_draw_text_hex_checked(target, str(row_dict["hex"]), row_dict["pos"], 0, "message_window")

	# 7. Optional text advance marker (Actor 2, hud_text_marker_set)
	if bool(config.get("show_text_marker", false)):
		var marker_y: int = int(config.get("text_marker_y", 0x48))
		_draw_actor(target, 0xF0, Vector2i(0x74, marker_y))

	# 8. 1px hardware border mask (border_black_draw: x=0 and y=0)
	_draw_black_border_1px(target)

	return target

## Convenience entrypoint for Stage 0 Arrival outside Kirameki High School gate (`prologue_line = 0`).
func render_stage0_arrival(with_title_badge: bool = false, checker_x: int = 94, checker_y: int = 81,
		player_family_hex: String = STR_HEX["speaker_fukuhara"],
		player_given_hex: String = STR_HEX["given_masaaki"]) -> Image:
	var full_name_hex: String = player_family_hex + player_given_hex
	var row0_hex: String = STR_HEX["line0_row0_prefix"] + full_name_hex + STR_HEX["line0_row0_suffix"]
	var rows: Array[String] = [row0_hex, STR_HEX["line0_row1"], STR_HEX["line0_row2"]]
	return render_prologue_frame({
		"mode_stage": 0,
		"mode_step": 2,
		"substep": 1,
		"checker_x": checker_x,
		"checker_y": checker_y,
		"scene_plate_key": "stage0_arrival",
		"show_hizuke": true,
		"year": 95,
		"month": 4,
		"day": 4,
		"weekday": 1,
		"day_kind": 0,
		"title_timer": 60 if with_title_badge else 184,
		"pad_held_triangle": with_title_badge,
		"speaker_hex": player_family_hex,
		"speaker_style": 0x0F,
		"block_rows_hex": rows,
		"first_visible_row": 0,
	})

## Convenience entrypoint for Stage 1 Entrance Ceremony Board (`prologue_line = 1`).
func render_stage1_board(checker_x: int = 174, checker_y: int = 169,
		player_family_hex: String = STR_HEX["speaker_fukuhara"]) -> Image:
	var rows: Array[String] = [STR_HEX["line1_row0"], STR_HEX["line1_row1"], STR_HEX["line1_row2"]]
	return render_prologue_frame({
		"mode_stage": 1,
		"mode_step": 1,
		"substep": 1,
		"checker_x": checker_x,
		"checker_y": checker_y,
		"scene_plate_key": "stage1_board",
		"show_hizuke": false,
		"title_timer": 184,
		"speaker_hex": player_family_hex,
		"speaker_style": 0x0F,
		"block_rows_hex": rows,
		"first_visible_row": 0,
	})

## Convenience entrypoint for Stage 13 Shiori Arrival / Self-Introduction (`prologue_line = 18`).
func render_stage13_shiori_line18(checker_x: int = 72, checker_y: int = 220) -> Image:
	var rows: Array[String] = [STR_HEX["line18_row0"], STR_HEX["line18_row1"]]
	return render_prologue_frame({
		"mode_stage": 13,
		"mode_step": 0,
		"substep": 2,
		"checker_x": checker_x,
		"checker_y": checker_y,
		"scene_plate_key": "stage13_shiori_line18",
		"show_hizuke": false,
		"title_timer": 184,
		"speaker_hex": STR_HEX["speaker_fujisaki"],
		"speaker_style": 0x00,
		"block_rows_hex": rows,
		"first_visible_row": 0,
	})

## Convenience entrypoint for Stage 14 Shiori Portrait Self-Introduction (`prologue_line = 19`, scrolled to rows 2..4).
func render_stage14_shiori_line19(checker_x: int = 79, checker_y: int = 216, first_visible_row: int = 2) -> Image:
	var rows: Array[String] = [
		STR_HEX["line19_row0"],
		STR_HEX["line19_row1"],
		STR_HEX["line19_row2"],
		STR_HEX["line19_row3"],
		STR_HEX["line19_row4"],
	]
	return render_prologue_frame({
		"mode_stage": 14,
		"mode_step": 0,
		"substep": 1,
		"checker_x": checker_x,
		"checker_y": checker_y,
		"scene_plate_key": "stage14_shiori_line19",
		"show_hizuke": false,
		"title_timer": 184,
		"speaker_hex": STR_HEX["speaker_fujisaki"],
		"speaker_style": 0x00,
		"block_rows_hex": rows,
		"first_visible_row": first_visible_row,
	})

# ==============================================================================
# Low-Level Primitive Drawing Routines
# ==============================================================================

func _draw_checkerboard_frame(target: Image, frame_arg: int, checker_x: int, checker_y: int) -> void:
	if _checker_tile == null:
		return
	var u0: int = checker_x & 31
	var v0: int = checker_y & 31
	for row in range(8):
		for col in range(10):
			if not is_checker_tile_enabled(frame_arg, col, row):
				continue
			var base_x: int = col * 32
			var base_y: int = row * 32
			for dy in range(32):
				var py: int = base_y + dy
				if py < 0 or py >= 240:
					continue
				var vt: int = (v0 + dy) & 31
				for dx in range(32):
					var px: int = base_x + dx
					if px < 0 or px >= 320:
						continue
					var ut: int = (u0 + dx) & 31
					target.set_pixelv(Vector2i(px, py), _checker_tile.get_pixel(ut, vt))

func _blit_opaque_region(target: Image, source: Image, dst_origin: Vector2i, src_rect: Rect2i) -> void:
	if source == null:
		return
	var max_w: int = mini(src_rect.size.x, source.get_width() - src_rect.position.x)
	var max_h: int = mini(src_rect.size.y, source.get_height() - src_rect.position.y)
	for y in range(max_h):
		var py: int = dst_origin.y + y
		if py < 0 or py >= 240:
			continue
		for x in range(max_w):
			var px: int = dst_origin.x + x
			if px < 0 or px >= 320:
				continue
			var c: Color = source.get_pixel(src_rect.position.x + x, src_rect.position.y + y)
			target.set_pixelv(Vector2i(px, py), Color8(c.r8, c.g8, c.b8, 255))

func _draw_message_window(target: Image) -> void:
	# OT 8: prim_gradient_rect(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0) -> Rect2i(27, 166, 266, 52)
	_compositor._gradient(target, Rect2i(27, 166, 266, 52))
	# OT 7: Actor 1 Frame 62 (0x3E) at (0, 75)
	_draw_actor(target, 62, Vector2i(0, 75))

func _draw_date_pillar(target: Image, year: int, month: int, day: int, weekday: int, day_kind: int) -> void:
	# OT 5: prim_gradient_rect(0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0) -> Rect2i(294, 45, 20, 114)
	_compositor._gradient(target, Rect2i(294, 45, 20, 114))
	# OT 4: Actor 16 Frame 60 (0x3C) at (144, -18)
	_draw_actor(target, 60, Vector2i(144, -18))

	# Year digits (Actors 17, 18)
	_draw_actor(target, ((year / 10) % 10) + 0x44, Vector2i(140, -65))
	_draw_actor(target, (year % 10) + 0x44, Vector2i(148, -65))

	# Month digit(s) (Actors 19, 20)
	if month >= 10:
		_draw_actor(target, ((month / 10) % 10) + 0x44, Vector2i(140, -34))
		_draw_actor(target, (month % 10) + 0x44, Vector2i(148, -34))
	else:
		_draw_actor(target, month + 0x44, Vector2i(144, -34))

	# Day digit(s) (Actors 21, 22)
	if day >= 10:
		_draw_actor(target, ((day / 10) % 10) + 0x44, Vector2i(140, -1))
		_draw_actor(target, (day % 10) + 0x44, Vector2i(148, -1))
	else:
		_draw_actor(target, day + 0x44, Vector2i(144, -1))

	# Weekday kanji (Actor 23: frame weekday + 0x52)
	var var_name := "hud-44" if (weekday == 6 or day_kind in [1, 2]) else ("hud-43" if weekday == 5 else "base")
	_draw_actor(target, weekday + 0x52, Vector2i(146, 31), var_name)

func _draw_title_badge(target: Image, title_hex: String, offset: Vector2i = Vector2i.ZERO, n_glyphs: int = 5) -> void:
	# OT 2: prim_gradient_rect(offset.x - 144, offset.y - 90, n_glyphs * 14 + 8, 20, 2, 0x30E00, 0)
	var gx: int = clampi(160 - 144 + offset.x, 0, 320)
	var gy: int = clampi(120 - 90 + offset.y, 0, 240)
	var rx: int = clampi(160 - 144 + offset.x + n_glyphs * 14 + 8, 0, 320)
	var ry: int = clampi(120 - 90 + offset.y + 20, 0, 240)
	if rx > gx and ry > gy:
		_compositor._gradient(target, Rect2i(gx, gy, rx - gx, ry - gy))

	# OT 1: Actors 3..9 drawn in descending actor index order (i = n_glyphs + 1 down to 0)
	for i in range(n_glyphs + 1, -1, -1):
		var fid: int = 113 if i == n_glyphs + 1 else (115 if i == 0 else 114)
		var ax: int = offset.x + (n_glyphs - i) * 14 - 135 + (2 if i == n_glyphs + 1 else (-2 if i == 0 else 0))
		var ay: int = offset.y - 81
		_draw_actor(target, fid, Vector2i(ax, ay))

	# OT 0: Title text at (-142 + offset.x, -88 + offset.y) -> screen (18 + offset.x, 32 + offset.y)
	_draw_text_hex_checked(target, title_hex, Vector2i(18 + offset.x, 32 + offset.y), 0, "title_badge",
		Rect2i(16 + offset.x, 30 + offset.y, n_glyphs * 14 + 8, 20))

func _draw_actor(target: Image, frame_id: int, pos: Vector2i, variant: String = "base", semi: bool = true) -> void:
	var actor := {
		"kind": "actor",
		"id": 96,
		"priority": 0,
		"frame": frame_id,
		"position": pos,
		"variant": variant,
		"semi": semi,
	}
	_compositor._draw_actor(target, actor)

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0,
		region: String = "", custom_bounds: Rect2i = Rect2i()) -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0

	for i in range(0, codes.size(), 2):
		if i + 1 >= codes.size():
			break
		var code_unit := codes.slice(i, i + 2)
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y

		recorded_glyphs.append({
			"code": code_unit.hex_encode(),
			"pos": Vector2i(gx_pos, gy_pos),
			"region": region,
		})

		if region == "message_window":
			if gx_pos < 27 or gx_pos + 12 > 293 or gy_pos < 166 or gy_pos + 12 > 218:
				bounding_box_violations.append(
					"Message window glyph %s at (%d, %d) overflows Rect2i(27, 166, 266, 52)" % [
						code_unit.hex_encode(), gx_pos, gy_pos
					]
				)
		elif region == "title_badge" and custom_bounds.size.x > 0:
			if gx_pos < custom_bounds.position.x or gx_pos + 12 > custom_bounds.position.x + custom_bounds.size.x or gy_pos < custom_bounds.position.y or gy_pos + 12 > custom_bounds.position.y + custom_bounds.size.y:
				bounding_box_violations.append(
					"Title badge glyph %s at (%d, %d) overflows %s" % [
						code_unit.hex_encode(), gx_pos, gy_pos, str(custom_bounds)
					]
				)

		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			var has_shadow: bool = not (palette_row in [3, 4, 5, 14, 15])
			if has_shadow:
				for gy in range(12):
					var py: int = gy_pos + gy + 1
					if py < 0 or py >= 240:
						continue
					for gx in range(12):
						var px: int = gx_pos + gx + 1
						if px < 0 or px >= 320:
							continue
						if g_img.get_pixel(gx, gy).a > 0:
							target.set_pixelv(Vector2i(px, py), Color8(0, 0, 0, 255))
			for gy in range(12):
				var py: int = gy_pos + gy
				if py < 0 or py >= 240:
					continue
				for gx in range(12):
					var px: int = gx_pos + gx
					if px < 0 or px >= 320:
						continue
					if g_img.get_pixel(gx, gy).a > 0:
						target.set_pixelv(Vector2i(px, py), g_img.get_pixel(gx, gy))

		x_offset += 14

func _draw_black_border_1px(target: Image) -> void:
	for x in range(320):
		target.set_pixelv(Vector2i(x, 0), Color8(0, 0, 0, 255))
	for y in range(240):
		target.set_pixelv(Vector2i(0, y), Color8(0, 0, 0, 255))
