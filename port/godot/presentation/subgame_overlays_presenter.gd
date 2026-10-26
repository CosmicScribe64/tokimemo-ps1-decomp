class_name SubgameOverlaysPresenter
extends RefCounted
## T-29018 (Lane 7): Authentic 320x240 presentation for the three interactive
## subgame overlays:
## 1. RPG_BAT (src/ovl/rpg_battle/) — Party Battle HUD, Command Menu & Message Box
## 2. TACO    (src/ovl/shooter_3d/) — 3D Rail Shooter Horizon, Reticle, Shield & Bomb HUD
## 3. TT      (src/ovl/TT/)         — TwinBee Time Attack 256x232 Playfield, Bells, Boss & Rankings

const Glyphs = preload("res://presentation/native_glyphs.gd")
const Overlays = preload("res://overlays/subgame_overlays.gd")

# PS1 Bayer 4x4 dither matrix
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Exact CP932 hex strings computed via Python .encode("cp932").hex() (CODING_STANDARDS.md 12a)
const STR_HEX := {
	"cmd_attack": "82bd82bd82a982a4", # たたかう
	"cmd_guard": "82da82a482ac82e5", # ぼうぎょ
	"cmd_special": "82ed82b4", # わざ
	"player_fukuhara": "959f8cb4", # 福原
	"girl_shiori": "8e8d9044", # 詩織
	"girl_mio": "96a28f8f", # 未緒
	"girl_yuina": "8c8b93de", # 結奈
	"girl_ayako": "8dca8e71", # 彩子
	"girl_saki": "8db98af3", # 沙希
	"girl_yukari": "82e482a982e8", # ゆかり
	"girl_nozomi": "965d", # 望
	"girl_mira": "96a39785", # 魅羅
	"girl_yuko": "975b8e71", # 夕子
	"girl_megumi": "88a4", # 愛
	"girl_yumi": "974494fc", # 優美
	"girl_yoshio": "8d449759", # 好雄
	"enemy_delinquents": "957397c79242", # 不良達
	"enemy_bancho": "94d492b7", # 番長
	"enemy_snake": "836e83758145838d815b8368", # ハブ・ロード
	"guest_mongoose": "837d8393834f815b8358", # マングース
	"msg_club_1_drama": "8140814081408140814081408140924597cd969f8dcb8140814081408140814081408140", # 　　　　　　　脱力漫才　　　　　　　
	"assist_shiori_1": "93a18de8817582e082a48141", # 藤崎「もう、
	"assist_shiori_2": "82c882c9836f834a82c882b182c68cbe82c182c482e982cc82e68142", # なにバカなこと言ってるのよ。
	"fw_score": "82728262826e82718264", # ＳＣＯＲＥ
	"fw_time": "82738268826c8264", # ＴＩＭＥ
	"fw_shield": "8272826782688264826b8263", # ＳＨＩＥＬＤ
	"fw_bomb": "8261826e826c8261", # ＢＯＭＢ
	"fw_stage1": "8272827382608266826481408250", # ＳＴＡＧＥ　１
	"fw_ranking": "82718260826d826a8268826d8266", # ＲＡＮＫＩＮＧ
	"fw_1st": "825082728273", # １ＳＴ
	"fw_2nd": "8251826d8263", # ２ＮＤ
	"fw_3rd": "825282718263", # ３ＲＤ
	"fw_ko": "826a8144826e", # Ｋ．Ｏ
	"fw_na": "826d81448260", # Ｎ．Ａ
	"fw_mi": "826c81448268", # Ｍ．Ｉ
	"fw_twin": "827382768268826d", # ＴＷＩＮ
}

# Fullwidth digits ０..９ (0x824F..0x8258)
const FW_DIGIT_HEX := [
	"824f", "8250", "8251", "8252", "8253",
	"8254", "8255", "8256", "8257", "8258"
]

var _font := Glyphs.new()
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []

func init_resources(font_root: String) -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	var err: String = _font.open_external_font(font_root)
	if not err.is_empty():
		return "Failed to open native font: " + err
	return ""

# ==============================================================================
# 1. RPG_BAT Party Battle HUD & Command Menu (320x240)
# ==============================================================================
func render_rpg_battle(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Battle arena sky & school courtyard ground (y: 0..168)
	for y in range(168):
		var col: Color
		if y < 92:
			var t: float = float(y) / 92.0
			col = Color8(int(18 + t * 38), int(26 + t * 52), int(64 + t * 76), 255)
		elif y < 104:
			# Courtyard wall & hedge silhouette
			var hedge: int = 28 + ((y * 7) % 14)
			col = Color8(hedge, hedge + 24, hedge + 12, 255)
		else:
			var gt: float = float(y - 104) / 64.0
			var d: int = DITHER[(y & 3) * 4]
			col = Color8(clampi(int(76 + gt * 36) + d, 0, 255), clampi(int(68 + gt * 30) + d, 0, 255), clampi(int(58 + gt * 24) + d, 0, 255), 255)
		for x in range(320):
			target.set_pixel(x, y, col)

	# 2. Top message window (rpg_message_update: text at (-0x7E, -0x66) => screen (34, 18))
	var msg_box := Rect2i(24, 12, 272, 28)
	_draw_ps1_window(target, msg_box, Color8(16, 28, 76), Color8(8, 14, 42))
	var msg_hex_list: Array = state.get("message_lines_hex", [])
	var top_hex: String = str(msg_hex_list[0]) if not msg_hex_list.is_empty() else STR_HEX["assist_shiori_1"]
	_draw_text_hex_checked(target, top_hex, Vector2i(34, 18), 0, "rpg_top_message", msg_box)

	# 3. Left side: 3 Delinquent enemies (不良達) at slots 0, 1, 2
	var enemy_positions := [Vector2i(40, 108), Vector2i(76, 98), Vector2i(108, 114)]
	var gauges: Array = state.get("enemy_slot_gauges", [5000, 5000, 5000])
	for i in range(3):
		if i < gauges.size() and int(gauges[i]) > 0:
			_draw_rpg_delinquent_sprite(target, enemy_positions[i], i)

	# 4. Right side: Hero (福原) and Partner Girl (詩織) sprites
	_draw_rpg_hero_sprite(target, Vector2i(226, 104), (int(state.get("player_action", 0)) & Overlays.RPG_PLAYER_GUARD) != 0)
	if int(state.get("girl_id", 0)) != 14:
		_draw_rpg_partner_sprite(target, Vector2i(268, 100), int(state.get("girl_id", 0)))

	# 5. Center Command Menu Window (たたかう / ぼうぎょ / わざ)
	var cmd_box := Rect2i(124, 96, 84, 62)
	_draw_ps1_window(target, cmd_box, Color8(20, 40, 96), Color8(10, 20, 52))
	var cursor: int = clampi(int(state.get("command_cursor", 0)), 0, 2)
	var cmd_hexes := [STR_HEX["cmd_attack"], STR_HEX["cmd_guard"], STR_HEX["cmd_special"]]
	for i in range(3):
		var cy: int = 102 + i * 18
		if i == cursor:
			_draw_cursor_arrow(target, Vector2i(130, cy + 2))
		var pal: int = 0 if (i < 2 or int(state.get("special_ready", 1)) == 1) else 7
		_draw_text_hex_checked(target, cmd_hexes[i], Vector2i(140, cy), pal, "rpg_command_menu", cmd_box)

	# 6. Bottom HUD Frame & Party Status (y: 168..240, matching battle_dispatch.c:55-63 & labels_intro_and_wave_transition.c:9-64)
	# Outer metallic gradient bars: (-0xA0, 0x30, 8, 0x48), (-0xA0, 0x70, 0x140, 8), (0x98, 0x30, 8, 0x48)
	_draw_ps1_window(target, Rect2i(8, 168, 152, 64), Color8(16, 32, 80), Color8(8, 16, 44))
	_draw_ps1_window(target, Rect2i(160, 168, 152, 64), Color8(16, 32, 80), Color8(8, 16, 44))
	_draw_gradient_bar(target, Rect2i(0, 168, 8, 72))
	_draw_gradient_bar(target, Rect2i(312, 168, 8, 72))
	_draw_gradient_bar(target, Rect2i(0, 232, 320, 8))

	# Left HUD box: Enemy group name at (0x13, 0xBB) => screen (19, 187)
	var enemy_panel := Rect2i(10, 170, 148, 60)
	var enemy_hex: String = str(state.get("enemy_group_hex", STR_HEX["enemy_delinquents"]))
	_draw_text_hex_checked(target, enemy_hex, Vector2i(19, 187), 0, "rpg_enemy_hud", enemy_panel)
	for i in range(3):
		var g_val: int = int(gauges[i]) if i < gauges.size() else 0
		var ratio: float = clampf(float(g_val) / 5000.0, 0.0, 1.0)
		_draw_gauge_bar(target, Rect2i(84, 180 + i * 14, 64, 8), ratio, Color8(232, 180, 40))

	# Right HUD box: Party names at x=177, y=179/195/211 and 4-digit HP at x=267,276,285,294
	var party_panel := Rect2i(162, 170, 148, 60)
	var p_name_hex: String = str(state.get("player_name_hex", STR_HEX["player_fukuhara"]))
	_draw_text_hex_checked(target, p_name_hex, Vector2i(177, 179), 0, "rpg_party_hud", party_panel)
	_draw_hud_number_4digit(target, int(state.get("player_hp", 970)), Vector2i(267, 179))

	var g_name_hex: String = str(state.get("girl_short_name_hex", STR_HEX["girl_shiori"]))
	_draw_text_hex_checked(target, g_name_hex, Vector2i(177, 195), 0, "rpg_party_hud", party_panel)
	_draw_hud_number_4digit(target, int(state.get("girl_hp", 256)), Vector2i(267, 195))

	# Special gauge bar on 3rd row (y = 211)
	var sp_ratio: float = clampf(float(state.get("special_gauge", 100)) / 100.0, 0.0, 1.0)
	_draw_text_hex_checked(target, STR_HEX["cmd_special"], Vector2i(177, 211), 0, "rpg_party_hud", party_panel)
	_draw_gauge_bar(target, Rect2i(220, 213, 82, 8), sp_ratio, Color8(64, 224, 128))

	return target


# ==============================================================================
# 2. TACO — 3D Rail Shooter Horizon & HUD (320x240)
# ==============================================================================
func render_taco_shooter(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. 15-band Gouraud Sky Gradient (shooter_draw_sky_gradient in opening_and_sky_gradient.c:121-151)
	var horizon_y: int = 144
	for y in range(horizon_y):
		var t: float = float(y) / float(horizon_y)
		var r: int = clampi(int(8.0 + pow(t, 1.8) * 180.0), 0, 255)
		var g: int = clampi(int(16.0 + pow(t, 1.3) * 195.0), 0, 255)
		var b: int = clampi(int(64.0 + t * 185.0), 0, 255)
		var col := Color8(r, g, b, 255)
		for x in range(320):
			target.set_pixel(x, y, col)

	# 2. 3D Perspective Checkerboard Ground Plane (y: 144..240)
	for y in range(horizon_y, 240):
		var dy: int = maxi(1, y - horizon_y)
		var z_depth: int = 2400 / dy
		var z_band: int = (z_depth / 6) & 1
		for x in range(320):
			var world_x: int = ((x - 160) * z_depth) / 48
			var x_band: int = (absi(world_x) / 16) & 1
			if world_x < 0:
				x_band ^= 1
			var checker: int = z_band ^ x_band
			var shade: float = clampf(float(dy) / 96.0, 0.25, 1.0)
			var col: Color
			if checker == 0:
				col = Color8(int(36 * shade), int(148 * shade), int(92 * shade), 255)
			else:
				col = Color8(int(20 * shade), int(96 * shade), int(64 * shade), 255)
			target.set_pixel(x, y, col)

	# 3. 3D Enemies & Morph Core (projected via shooter_project_3d)
	for enemy: Dictionary in state.get("enemies", []):
		if int(enemy.get("hp", 0)) <= 0:
			continue
		var p2d: Vector2i = Overlays.shooter_project_3d(int(enemy["x"]), int(enemy["y"]), int(enemy["z"]))
		var scale_px: int = clampi((0x200 * 28) / maxi(0x200, int(enemy["z"])), 8, 28)
		_draw_taco_enemy_3d(target, p2d, scale_px, str(enemy.get("kind", "taco_fighter")))

	# 4. Player Reticle, Twin Laser Beams, Wingman & Player Ship
	var p: Dictionary = state.get("player", {})
	var ship_draw_pos := Vector2i(160, 176)
	var reticle_pos := Vector2i(160 + int(p.get("x", 0)) / 32, 92 + int(p.get("y", -0x200)) / 48)
	_draw_taco_laser_beams(target, ship_draw_pos + Vector2i(0, -4), reticle_pos)
	_draw_taco_reticle(target, reticle_pos)
	_draw_taco_wingman(target, Vector2i(104, 182))
	_draw_taco_player_ship(target, ship_draw_pos)

	# 5. Top HUD Bar (shooter_draw_hud in hud_secondary_weapons.c:5-35)
	var top_hud_box := Rect2i(8, 6, 304, 22)
	_draw_ps1_window(target, top_hud_box, Color8(12, 24, 60), Color8(6, 12, 32))
	_draw_text_hex_checked(target, STR_HEX["fw_score"], Vector2i(14, 11), 0, "taco_top_hud", top_hud_box)
	var total_disp: int = int(state.get("score", 142500)) + int(state.get("stages_cleared", 0)) * 1000000
	_draw_ascii_digits(target, "%07d" % total_disp, Vector2i(92, 12), Color8(248, 240, 96))

	_draw_text_hex_checked(target, STR_HEX["fw_time"], Vector2i(172, 11), 0, "taco_top_hud", top_hud_box)
	var t_left: int = int(state.get("time_left", 5400))
	var mins: int = t_left / 3600
	var secs: int = (t_left % 3600) / 60
	var csecs: int = ((t_left % 60) * 60) / 100
	_draw_ascii_digits(target, "%02d:%02d.%02d" % [mins, secs, csecs], Vector2i(236, 12), Color8(120, 248, 220))

	# 6. Bottom-left Shield Gauge (hud_secondary_weapons.c:18-29: (-0xBF, 0x68, 14, -stats[1] >> 2))
	var shield: int = clampi(int(p.get("shield", 255)), 0, 255)
	var shield_h: int = shield >> 2 # max 63px
	var shield_col: Color = Color8(64, 160, 255) if shield > 0xC0 else (Color8(64, 232, 96) if shield > 0x80 else (Color8(248, 216, 48) if shield > 0x40 else Color8(248, 64, 64)))
	_draw_ps1_window(target, Rect2i(14, 156, 22, 72), Color8(16, 24, 48), Color8(8, 12, 24))
	for y in range(224 - shield_h, 224):
		for x in range(18, 32):
			target.set_pixel(x, y, shield_col)

	# 7. Bottom-right Bomb Stock Panel (hud_secondary_weapons.c:30-34)
	var bomb_box := Rect2i(236, 198, 74, 32)
	_draw_ps1_window(target, bomb_box, Color8(16, 24, 52), Color8(8, 12, 28))
	var bomb_slots: Array = p.get("bomb_slots", [4, 3, 0])
	for i in range(3):
		var bkind: int = int(bomb_slots[i]) if i < bomb_slots.size() else 0
		_draw_taco_bomb_icon(target, Vector2i(246 + i * 22, 214), bkind)

	return target


# ==============================================================================
# 3. TT — TwinBee Time Attack Playfield & Scoreboard (320x240)
# ==============================================================================
func render_tt_twinbee(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	# Outer frame black pillars: x in [0, 32), [288, 320), y in [0, 8) (tt_video_init in engine_initialization.c:129-146)
	target.fill(Color.BLACK)

	var playfield := Rect2i(32, 8, 256, 232)

	# 1. Scrolling Sky & Island Ground Tilemap inside 256x232 playfield
	for y in range(playfield.position.y, playfield.end.y):
		for x in range(playfield.position.x, playfield.end.x):
			var tx: int = (x - 32) / 16
			var ty: int = (y + 24) / 16
			var is_island: bool = ((tx * 5 + ty * 3) % 11) < 3
			var col: Color
			if is_island:
				col = Color8(56, 168, 84) if ((x + y) & 4) == 0 else Color8(44, 144, 72)
			else:
				col = Color8(48, 132, 224) if ((x ^ y) & 8) == 0 else Color8(40, 118, 208)
			target.set_pixel(x, y, col)

	# Fluffy white scrolling clouds (tt_clouds_draw)
	_draw_tt_cloud(target, Vector2i(76, 84))
	_draw_tt_cloud(target, Vector2i(232, 148))

	# 2. Stage 1 Rope-Arm Boss (tt_boss1_update in rope_arm_boss.c)
	var boss: Dictionary = state.get("boss", {})
	if int(boss.get("active", 1)) != 0:
		var bx: int = 160 + int(boss.get("x", 0))
		var by: int = int(boss.get("y", 54))
		_draw_tt_boss1(target, Vector2i(bx, by))

	# 3. Cycling Bell Powerups (tt_bells_draw in cycling_pickups.c:224-275)
	for bell: Dictionary in state.get("bells", []):
		if int(bell.get("active", 0)) == 0:
			continue
		var bpos := Vector2i(160 + int(bell["x"]), int(bell["y"]))
		_draw_tt_bell(target, bpos, int(bell.get("kind", 0)))

	# 4. Air Enemies & Enemy Shots
	for enemy: Dictionary in state.get("air_enemies", []):
		if int(enemy.get("active", 0)) == 0:
			continue
		_draw_tt_air_enemy(target, Vector2i(160 + int(enemy["x"]), int(enemy["y"])))
	for eshot: Dictionary in state.get("enemy_shots", []):
		if int(eshot.get("active", 0)) == 0:
			continue
		_draw_circle_filled(target, Vector2i(160 + int(eshot["x"]), int(eshot["y"])), 3, Color8(255, 96, 64))

	# 5. TwinBee Player Ship, Arms, 3 Option Followers & Twin Shots (tt_player_draw in player_and_asset_uploads.c:771-808)
	var p: Dictionary = state.get("player", {})
	var ppos := Vector2i(160 + int(p.get("x", 0)), int(p.get("y", 192)))
	if int(p.get("has_options", 1)) != 0:
		_draw_tt_option(target, ppos + Vector2i(-24, 12))
		_draw_tt_option(target, ppos + Vector2i(24, 12))
		_draw_tt_option(target, ppos + Vector2i(0, 22))
	_draw_tt_twin_shots(target, ppos + Vector2i(-10, -28))
	_draw_tt_twin_shots(target, ppos + Vector2i(10, -56))
	_draw_tt_player(target, ppos)

	# 6. Top-left Time HUD & Life Icons (tt_state_stage in play_flow_and_scroll_scripts.c:226-274)
	var time_info: Dictionary = Overlays.tt_format_time(int(state.get("elapsed_frames", 4250)))
	var hud_box := Rect2i(36, 10, 136, 20)
	_draw_ps1_window(target, hud_box, Color8(12, 24, 64), Color8(6, 12, 36))
	_draw_text_hex_checked(target, STR_HEX["fw_time"], Vector2i(40, 14), 0, "tt_hud", hud_box)
	_draw_ascii_digits(target, str(time_info["text"]), Vector2i(104, 15), Color8(255, 240, 80))

	# Life icons at x = -0x70, -0x60 => screen (48, 34), (64, 34)
	var lives: int = clampi(int(state.get("lives", 2)), 0, 2)
	for i in range(lives):
		_draw_tt_life_icon(target, Vector2i(44 + i * 16, 36))

	# 6-segment Speed Gauge at bottom-left (x = -0x78 => screen 40, y = 0xD8 - i*8)
	var speed_step: int = clampi(int(state.get("scroll_speed", 0x18000)) / 0x8000, 1, 6)
	for i in range(6):
		var gy: int = 216 - i * 8
		var seg_col: Color = Color8(255, 220, 48) if (i + 1) <= speed_step else Color8(48, 64, 96)
		_fill_rect(target, Rect2i(40, gy, 14, 6), seg_col)

	# 7. Compact Time-Attack Ranking Overlay Card at bottom-right (tt_ranking_rows_init in ranking_view.c:23-69)
	var rank_box := Rect2i(192, 184, 92, 48)
	_draw_ps1_window(target, rank_box, Color8(16, 28, 72), Color8(8, 14, 38))
	var rankings: Array = state.get("rankings", [])
	for i in range(mini(3, rankings.size())):
		var r_entry: Dictionary = rankings[i]
		var r_time: Dictionary = Overlays.tt_format_time(int(r_entry["time"]))
		var ry: int = 188 + i * 14
		_draw_ascii_digits(target, "%d." % (i + 1), Vector2i(196, ry), Color8(255, 200, 80))
		_draw_ascii_digits(target, str(r_time["text"]), Vector2i(212, ry), Color8(224, 248, 255))

	return target


# ==============================================================================
# Drawing Helpers & Glyph Bounding-Box Verification
# ==============================================================================

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i,
		palette_row: int, region: String, bounds: Rect2i) -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0
	for i in range(0, codes.size(), 2):
		if i + 1 >= codes.size():
			break
		var code_unit := codes.slice(i, i + 2)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y
		recorded_glyphs.append({
			"code": code_unit.hex_encode(),
			"pos": Vector2i(gx_pos, gy_pos),
			"region": region
		})
		if gx_pos < bounds.position.x or gx_pos + 12 > bounds.end.x or gy_pos < bounds.position.y or gy_pos + 12 > bounds.end.y:
			bounding_box_violations.append("Glyph %s at (%d, %d) in %s overflows %s" % [
				code_unit.hex_encode(), gx_pos, gy_pos, region, str(bounds)
			])
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			for gy in range(12):
				var py: int = gy_pos + gy
				if py < 0 or py >= 240:
					continue
				for gx in range(12):
					var px: int = gx_pos + gx
					if px < 0 or px >= 320:
						continue
					if g_img.get_pixel(gx, gy).a > 0:
						if px + 1 < 320 and py + 1 < 240:
							target.set_pixel(px + 1, py + 1, Color8(0, 0, 0, 255))
						target.set_pixel(px, py, g_img.get_pixel(gx, gy))
		x_offset += 13

func _draw_ps1_window(target: Image, rect: Rect2i, top_col: Color, bot_col: Color) -> void:
	for y in range(rect.position.y, rect.end.y):
		if y < 0 or y >= 240:
			continue
		var t: float = float(y - rect.position.y) / float(maxi(1, rect.size.y - 1))
		var c := top_col.lerp(bot_col, t)
		for x in range(rect.position.x, rect.end.x):
			if x < 0 or x >= 320:
				continue
			target.set_pixel(x, y, c)
	# Beveled silver/gold frame
	var border_light := Color8(208, 220, 248)
	var border_dark := Color8(64, 80, 128)
	for x in range(rect.position.x, rect.end.x):
		if x >= 0 and x < 320:
			if rect.position.y >= 0 and rect.position.y < 240:
				target.set_pixel(x, rect.position.y, border_light)
			if rect.end.y - 1 >= 0 and rect.end.y - 1 < 240:
				target.set_pixel(x, rect.end.y - 1, border_dark)
	for y in range(rect.position.y, rect.end.y):
		if y >= 0 and y < 240:
			if rect.position.x >= 0 and rect.position.x < 320:
				target.set_pixel(rect.position.x, y, border_light)
			if rect.end.x - 1 >= 0 and rect.end.x - 1 < 320:
				target.set_pixel(rect.end.x - 1, y, border_dark)

func _draw_gradient_bar(target: Image, rect: Rect2i) -> void:
	for y in range(rect.position.y, rect.end.y):
		if y < 0 or y >= 240:
			continue
		for x in range(rect.position.x, rect.end.x):
			if x < 0 or x >= 320:
				continue
			var shade: int = 80 + ((x + y) % 4) * 16
			target.set_pixel(x, y, Color8(shade, shade + 12, shade + 32))

func _draw_gauge_bar(target: Image, rect: Rect2i, ratio: float, fill_col: Color) -> void:
	_fill_rect(target, rect, Color8(24, 24, 36))
	var fill_w: int = int(float(rect.size.x - 2) * clampf(ratio, 0.0, 1.0))
	if fill_w > 0:
		_fill_rect(target, Rect2i(rect.position.x + 1, rect.position.y + 1, fill_w, rect.size.y - 2), fill_col)

func _draw_hud_number_4digit(target: Image, value: int, pos: Vector2i) -> void:
	var clamped: int = clampi(value, 0, 9999)
	var text := "%4d" % clamped
	_draw_ascii_digits(target, text, pos, Color8(248, 248, 216))

# Compact 5x9 pixel bitmap font for crisp numeric counters (HP, SCORE, TIME)
const MINI_FONT := {
	"0": [0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E],
	"1": [0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E],
	"2": [0x0E, 0x11, 0x01, 0x06, 0x08, 0x10, 0x1F],
	"3": [0x1F, 0x02, 0x04, 0x06, 0x01, 0x11, 0x0E],
	"4": [0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02],
	"5": [0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E],
	"6": [0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E],
	"7": [0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08],
	"8": [0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E],
	"9": [0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C],
	":": [0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00],
	".": [0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06],
	" ": [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],
}

func _draw_ascii_digits(target: Image, text: String, pos: Vector2i, color: Color) -> void:
	var x_cur: int = pos.x
	for i in range(text.length()):
		var ch: String = text.substr(i, 1)
		var rows: Array = MINI_FONT.get(ch, MINI_FONT[" "])
		for ry in range(rows.size()):
			var mask: int = int(rows[ry])
			for rx in range(5):
				if ((mask >> (4 - rx)) & 1) != 0:
					var px: int = x_cur + rx
					var py: int = pos.y + ry
					if px + 1 < 320 and py + 1 < 240:
						target.set_pixel(px + 1, py + 1, Color.BLACK)
					if px >= 0 and px < 320 and py >= 0 and py < 240:
						target.set_pixel(px, py, color)
		x_cur += 7

func _draw_cursor_arrow(target: Image, pos: Vector2i) -> void:
	for dy in range(-4, 5):
		var span: int = 4 - absi(dy)
		for dx in range(span + 1):
			var px: int = pos.x + dx
			var py: int = pos.y + dy
			if px >= 0 and px < 320 and py >= 0 and py < 240:
				target.set_pixel(px, py, Color8(255, 232, 96))

func _draw_rpg_delinquent_sprite(target: Image, pos: Vector2i, variant: int) -> void:
	var coat_col: Color = Color8(64, 48, 96) if variant == 0 else (Color8(48, 64, 88) if variant == 1 else Color8(88, 48, 64))
	# Pompadour hair + head + gakuran jacket + legs
	_fill_rect(target, Rect2i(pos.x - 8, pos.y - 28, 14, 6), Color8(32, 24, 20))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 22, 12, 10), Color8(240, 196, 160))
	_fill_rect(target, Rect2i(pos.x - 10, pos.y - 12, 20, 22), coat_col)
	_fill_rect(target, Rect2i(pos.x - 8, pos.y + 10, 6, 14), Color8(36, 36, 56))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 10, 6, 14), Color8(36, 36, 56))

func _draw_rpg_hero_sprite(target: Image, pos: Vector2i, guarding: bool) -> void:
	var jacket: Color = Color8(56, 112, 208) if not guarding else Color8(96, 168, 248)
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 26, 12, 6), Color8(40, 32, 28))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 20, 12, 10), Color8(248, 204, 168))
	_fill_rect(target, Rect2i(pos.x - 9, pos.y - 10, 18, 20), jacket)
	_fill_rect(target, Rect2i(pos.x - 7, pos.y + 10, 6, 14), Color8(40, 48, 72))
	_fill_rect(target, Rect2i(pos.x + 1, pos.y + 10, 6, 14), Color8(40, 48, 72))

func _draw_rpg_partner_sprite(target: Image, pos: Vector2i, _girl_id: int) -> void:
	# Shiori / Partner heroine school uniform & ribbon
	_fill_rect(target, Rect2i(pos.x - 7, pos.y - 26, 14, 14), Color8(184, 72, 88))
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 20, 10, 10), Color8(252, 212, 180))
	_fill_rect(target, Rect2i(pos.x - 8, pos.y - 10, 16, 14), Color8(236, 240, 248))
	_fill_rect(target, Rect2i(pos.x - 4, pos.y - 8, 8, 5), Color8(224, 64, 80))
	_fill_rect(target, Rect2i(pos.x - 9, pos.y + 4, 18, 10), Color8(44, 68, 124))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 14, 4, 10), Color8(240, 240, 248))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 14, 4, 10), Color8(240, 240, 248))

func _draw_taco_enemy_3d(target: Image, pos: Vector2i, radius: int, kind: String) -> void:
	if kind == "morph_core":
		_draw_circle_filled(target, pos, radius, Color8(224, 72, 128))
		_draw_circle_filled(target, pos, maxi(4, radius - 5), Color8(255, 192, 96))
	else:
		_draw_circle_filled(target, pos, radius, Color8(232, 96, 56))
		_fill_rect(target, Rect2i(pos.x - radius - 4, pos.y - 3, radius * 2 + 8, 6), Color8(180, 200, 232))

func _draw_taco_reticle(target: Image, pos: Vector2i) -> void:
	var c := Color8(96, 255, 128)
	for dx in range(-14, 15):
		if absi(dx) >= 4:
			_set_px(target, pos.x + dx, pos.y, c)
	for dy in range(-14, 15):
		if absi(dy) >= 4:
			_set_px(target, pos.x, pos.y + dy, c)
	_fill_rect(target, Rect2i(pos.x - 12, pos.y - 12, 5, 2), c)
	_fill_rect(target, Rect2i(pos.x + 8, pos.y - 12, 5, 2), c)
	_fill_rect(target, Rect2i(pos.x - 12, pos.y + 11, 5, 2), c)
	_fill_rect(target, Rect2i(pos.x + 8, pos.y + 11, 5, 2), c)

func _draw_taco_laser_beams(target: Image, ship_pos: Vector2i, reticle_pos: Vector2i) -> void:
	_draw_line_solid(target, ship_pos + Vector2i(-14, 0), reticle_pos + Vector2i(-4, 0), Color8(112, 248, 255))
	_draw_line_solid(target, ship_pos + Vector2i(14, 0), reticle_pos + Vector2i(4, 0), Color8(112, 248, 255))

func _draw_taco_player_ship(target: Image, pos: Vector2i) -> void:
	# Futuristic 3D fighter fuselage & wings
	_fill_rect(target, Rect2i(pos.x - 22, pos.y + 2, 44, 6), Color8(192, 212, 248))
	_fill_rect(target, Rect2i(pos.x - 10, pos.y - 6, 20, 14), Color8(232, 240, 255))
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 4, 10, 6), Color8(64, 192, 255))
	# Twin thruster glow
	_fill_rect(target, Rect2i(pos.x - 8, pos.y + 6, 5, 5), Color8(255, 160, 48))
	_fill_rect(target, Rect2i(pos.x + 3, pos.y + 6, 5, 5), Color8(255, 160, 48))

func _draw_taco_wingman(target: Image, pos: Vector2i) -> void:
	_fill_rect(target, Rect2i(pos.x - 12, pos.y, 24, 4), Color8(160, 224, 192))
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 4, 10, 8), Color8(216, 248, 232))

func _draw_taco_bomb_icon(target: Image, pos: Vector2i, kind: int) -> void:
	if kind == 0:
		_draw_circle_filled(target, pos, 6, Color8(40, 48, 68))
	elif kind == Overlays.SHOOTER_BOMB_BLAST:
		_draw_circle_filled(target, pos, 6, Color8(248, 96, 64))
		_draw_circle_filled(target, pos, 3, Color8(255, 224, 128))
	else:
		_draw_circle_filled(target, pos, 6, Color8(64, 192, 255))
		_draw_circle_filled(target, pos, 3, Color8(208, 248, 255))

func _draw_tt_cloud(target: Image, pos: Vector2i) -> void:
	_draw_circle_filled(target, pos, 14, Color8(236, 244, 255))
	_draw_circle_filled(target, pos + Vector2i(-10, 4), 10, Color8(220, 232, 248))
	_draw_circle_filled(target, pos + Vector2i(10, 4), 10, Color8(220, 232, 248))

func _draw_tt_boss1(target: Image, pos: Vector2i) -> void:
	# Rope-Arm Boss (rope_arm_boss.c): central armored core + 4 side pods + jointed rope arms
	for i in range(6):
		var seg_l := pos + Vector2i(-24 - i * 5, 10 + i * 6)
		var seg_r := pos + Vector2i(24 + i * 5, 10 + i * 6)
		_draw_circle_filled(target, seg_l, 5, Color8(208, 160, 64))
		_draw_circle_filled(target, seg_r, 5, Color8(208, 160, 64))
	_fill_rect(target, Rect2i(pos.x - 28, pos.y - 16, 56, 30), Color8(184, 72, 88))
	_draw_circle_filled(target, pos, 12, Color8(255, 216, 72))
	_draw_circle_filled(target, pos, 6, Color8(248, 64, 64))

func _draw_tt_bell(target: Image, pos: Vector2i, kind: int) -> void:
	var c: Color = Color8(255, 224, 48) # 0: Yellow
	if kind == 1:
		c = Color8(72, 176, 255) # 1: Blue (Twin Shot)
	elif kind == 2:
		c = Color8(80, 240, 120) # 2: Green (Speed Up)
	elif kind == 3:
		c = Color8(248, 248, 255) # 3: White (Option Followers)
	elif kind == 6:
		c = Color8(192, 96, 248) # 6: Purple (Speed Down)
	_draw_circle_filled(target, pos, 6, c)
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 1, 13, 5), c)
	_fill_rect(target, Rect2i(pos.x - 2, pos.y + 6, 5, 2), Color8(248, 128, 64))

func _draw_tt_air_enemy(target: Image, pos: Vector2i) -> void:
	_draw_circle_filled(target, pos, 7, Color8(240, 88, 72))
	_draw_circle_filled(target, pos + Vector2i(0, -2), 3, Color8(255, 232, 160))

func _draw_tt_player(target: Image, pos: Vector2i) -> void:
	# TwinBee iconic blue/white round body + arms + yellow feet
	_fill_rect(target, Rect2i(pos.x - 13, pos.y - 3, 5, 8), Color8(240, 244, 255))
	_fill_rect(target, Rect2i(pos.x + 8, pos.y - 3, 5, 8), Color8(240, 244, 255))
	_draw_circle_filled(target, pos, 9, Color8(64, 152, 248))
	_draw_circle_filled(target, pos + Vector2i(0, -3), 5, Color8(255, 220, 96))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 7, 4, 4), Color8(255, 200, 48))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 7, 4, 4), Color8(255, 200, 48))

func _draw_tt_option(target: Image, pos: Vector2i) -> void:
	_draw_circle_filled(target, pos, 5, Color8(255, 168, 96))

func _draw_tt_twin_shots(target: Image, pos: Vector2i) -> void:
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 4, 3, 8), Color8(255, 248, 120))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y - 4, 3, 8), Color8(255, 248, 120))

func _draw_tt_life_icon(target: Image, pos: Vector2i) -> void:
	_draw_circle_filled(target, pos, 5, Color8(80, 176, 255))
	_draw_circle_filled(target, pos + Vector2i(0, -1), 2, Color8(255, 232, 120))

func _fill_rect(target: Image, rect: Rect2i, col: Color) -> void:
	for y in range(rect.position.y, rect.end.y):
		if y < 0 or y >= 240:
			continue
		for x in range(rect.position.x, rect.end.x):
			if x < 0 or x >= 320:
				continue
			target.set_pixel(x, y, col)

func _draw_circle_filled(target: Image, center: Vector2i, radius: int, col: Color) -> void:
	var r2: int = radius * radius
	for dy in range(-radius, radius + 1):
		var py: int = center.y + dy
		if py < 0 or py >= 240:
			continue
		for dx in range(-radius, radius + 1):
			var px: int = center.x + dx
			if px < 0 or px >= 320:
				continue
			if dx * dx + dy * dy <= r2:
				target.set_pixel(px, py, col)

func _draw_line_solid(target: Image, p0: Vector2i, p1: Vector2i, col: Color) -> void:
	var dx: int = absi(p1.x - p0.x)
	var dy: int = -absi(p1.y - p0.y)
	var sx: int = 1 if p0.x < p1.x else -1
	var sy: int = 1 if p0.y < p1.y else -1
	var err: int = dx + dy
	var x: int = p0.x
	var y: int = p0.y
	while true:
		_set_px(target, x, y, col)
		if x == p1.x and y == p1.y:
			break
		var e2: int = 2 * err
		if e2 >= dy:
			err += dy
			x += sx
		if e2 <= dx:
			err += dx
			y += sy

func _set_px(target: Image, x: int, y: int, col: Color) -> void:
	if x >= 0 and x < 320 and y >= 0 and y < 240:
		target.set_pixel(x, y, col)
