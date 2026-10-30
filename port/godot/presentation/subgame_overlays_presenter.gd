class_name SubgameOverlaysPresenter
extends RefCounted
## T-29018 / T-29024 (Lane 5): 320x240 presentation & scene-node builder for
## the three interactive subgame overlays:
## 1. RPG_BAT (src/ovl/rpg_battle/): Party Battle HUD, Command Menu, Encounters & Message Box
## 2. TACO    (src/ovl/shooter_3d/): 3D Rail Shooter Horizon, Reticle, Bosses, Shield & Bomb HUD
## 3. TT      (src/ovl/TT/): TwinBee Time Attack 256x232 Playfield, Bells, Boss & Rankings
## Missing resources can select diagnostic fallback art; rendering alone is not fidelity evidence.

const Glyphs = preload("res://presentation/native_glyphs.gd")
const Overlays = preload("res://overlays/subgame_overlays.gd")
const TextBinding = preload("res://presentation/presenter_text_binding.gd")

# PS1 Bayer 4x4 dither matrix
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Exact CP932 hex strings computed via Python .encode("cp932").hex() (CODING_STANDARDS.md 12a)
const STR_HEX := {
	"cmd_attack": "82bd82bd82a982a4", # たたかう
	"cmd_guard": "82da82a482ac82e5", # ぼうぎょ
	"cmd_special": "82ed82b4", # わざ
	"cmd_item": "8341834383658380", # アイテム
	"cmd_run": "82c982b082e9", # にげる
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
	"fw_game_over": "82668260826c82648140826e827582648271", # ＧＡＭＥ　ＯＶＥＲ
	"fw_stage_clear": "8272827382608266826481408262826b826482608271", # ＳＴＡＧＥ　ＣＬＥＡＲ
	"fw_mission_complete": "826c8268827282728268826e826d81408262826e826c826f826b826482738264", # ＭＩＳＳＩＯＮ　ＣＯＭＰＬＥＴＥ
	"fw_time_over": "82738268826c82648140826e827582648271", # ＴＩＭＥ　ＯＶＥＲ
	"rpg_victory": "959f8cb4924282cd8f9f979882b582bd8149", # 福原達は勝利した！
	"rpg_defeat": "959f8cb4924282cd9473966b82b582bd8163", # 福原達は敗北した…
	"rpg_attack_msg": "959f8cb482cc8d558c828149", # 福原の攻撃！
	"rpg_guard_msg": "959f8cb482cc91cc97cd82aa8254819389f1959c82b582bd", # 福原の体力が５％回復した
	"enemy_deer": "8ead8145834f838c815b8367", # 鹿・グレート
	"guest_monk": "8b9596b3916d", # 虚無僧
	"enemy_bear": "834c8389815b8371834f837d", # キラーヒグマ
	"guest_hunter": "837d835e834d", # マタギ
	"enemy_robot": "905e814590a28a4590aa959e838d837b", # 真・世界征服ロボ
	"enemy_aliens": "89469288906c", # 宇宙人
	"taco_stage2": "8272827382608266826481408251", # ＳＴＡＧＥ　２
	"taco_stage3": "8272827382608266826481408252", # ＳＴＡＧＥ　３
	"taco_boss": "8261826e82728272", # ＢＯＳＳ
	"taco_result": "8271826482728274826b8273", # ＲＥＳＵＬＴ
	"taco_total": "8273826e82738260826b", # ＴＯＴＡＬ
}

const GIRL_PALETTE := {
	0: Color8(216, 72, 92),    # Shiori
	1: Color8(64, 88, 176),    # Mio
	2: Color8(240, 200, 72),   # Yuina
	3: Color8(216, 112, 56),   # Ayako
	4: Color8(152, 92, 60),    # Saki
	5: Color8(224, 140, 184),  # Yukari
	6: Color8(48, 112, 144),   # Nozomi
	7: Color8(156, 84, 204),   # Mira
	8: Color8(236, 184, 68),   # Yuko
	9: Color8(168, 96, 76),    # Megumi
	10: Color8(240, 108, 128), # Yumi
	13: Color8(112, 84, 56),   # Yoshio
}

# Fullwidth digits ０..９ (0x824F..0x8258)
const FW_DIGIT_HEX := [
	"824f", "8250", "8251", "8252", "8253",
	"8254", "8255", "8256", "8257", "8258"
]

var _font := Glyphs.new()
var _font_initialized: bool = false
var _rpg_arena_bg: Image
var _rpg_hero_sd: Image
var _rpg_shiori_sd: Image
var _rpg_enemy_sds: Array[Image] = []
var _rpg_encounter_sprites: Dictionary = {}
var _rpg_bat_ready: bool = false
var _rpg_bat_bins: Dictionary = {}
var _rpg_bat_ram_cache: Dictionary = {}
var _rpg_bat_girl_bins: Dictionary = {}
var _taco_bg: Image
var _taco_bg_1: Image
var _taco_exn_bytes: PackedByteArray = PackedByteArray()
var _taco_vram: PackedInt32Array = PackedInt32Array()
var _taco_vram_ready: bool = false
var _taco_active_boss_bank: int = -1
var _taco_nbn_banks: Dictionary = {}
var _taco_models: Dictionary = {}
var _tt_tbta_bytes: PackedByteArray = PackedByteArray()
var _tt_exn_bytes: PackedByteArray = PackedByteArray()
var _tt_vram: PackedInt32Array = PackedInt32Array()
var _tt_vram_ready: bool = false
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []
## Phase 4 WP-I: empty until bind_english; original bytes are drawn.
var english_text := TextBinding.new()

const TACO_EXN_BASE := 0x80132000
const TACO_MTX_BASE := 0x801E0000
const TACO_DIGIT_UV_ADDR := 0x8015F5A0
const TACO_LETTER_UV_ADDR := 0x8015F5B4
const TACO_BS01_POSE_PTRS_ADDR := 0x8015F840
const TACO_DRA_POSE_PTRS_ADDR := 0x8015FAE0
const TACO_TAKO_POSE_PTRS_ADDR := 0x8015FD10
const TACO_WANG_PART_PTRS_ADDR := 0x80160A4C

const TT_EXN_BASE := 0x80132000
const TT_SPRITE_SET_TABLE_ADDR := 0x80151A50
const TT_STAGE_TILE_MAP_ADDR := 0x80156494
const TT_VRAM_PALETTE_ROW_ADDR := 0x80156788
const TT_TEXT_STRINGS_ADDR := 0x8015694C

static func _load_first_existing_image(rel_path: String) -> Image:
	var candidates := [
		"/workspace/" + rel_path,
		"/work/" + rel_path,
		ProjectSettings.globalize_path("res://../../" + rel_path).simplify_path(),
		ProjectSettings.globalize_path("res://../../../../../../" + rel_path).simplify_path(),
	]
	var env_root: String = OS.get_environment("TOKIMEMO_ROOT")
	if not env_root.is_empty():
		candidates.push_front(env_root.path_join(rel_path))
	for p in candidates:
		if FileAccess.file_exists(p):
			return Image.load_from_file(p)
	return null

static func _load_first_existing_bytes(rel_path: String) -> PackedByteArray:
	var candidates := [
		"/workspace/" + rel_path,
		"/work/" + rel_path,
		ProjectSettings.globalize_path("res://../../" + rel_path).simplify_path(),
		ProjectSettings.globalize_path("res://../../../../../../" + rel_path).simplify_path(),
	]
	var env_root: String = OS.get_environment("TOKIMEMO_ROOT")
	if not env_root.is_empty():
		candidates.push_front(env_root.path_join(rel_path))
	for p in candidates:
		if FileAccess.file_exists(p):
			return FileAccess.get_file_as_bytes(p)
	return PackedByteArray()

static func _blit_alpha_centered(dst: Image, src: Image, center: Vector2i, flip_h: bool = false) -> void:
	if dst == null or src == null:
		return
	var sw: int = src.get_width()
	var sh: int = src.get_height()
	var ox: int = center.x - sw / 2
	var oy: int = center.y - sh / 2
	for sy in range(sh):
		var dy: int = oy + sy
		if dy < 0 or dy >= 240:
			continue
		for sx in range(sw):
			var dx: int = ox + (sw - 1 - sx if flip_h else sx)
			if dx < 0 or dx >= 320:
				continue
			var c: Color = src.get_pixel(sx, sy)
			if c.a > 0.05:
				dst.set_pixel(dx, dy, Color8(c.r8, c.g8, c.b8, 255))

func init_resources(font_root: String) -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	_rpg_arena_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-9b79cd6fd2a3ee9acc3f85d1fcd53b5c86c3e8c70a055cf6134bea7eb908a80a/asset-9b79cd6fd2a3ee9acc3f85d1fcd53b5c86c3e8c70a055cf6134bea7eb908a80a.background.png")
	_rpg_hero_sd = _load_first_existing_image(".asset-cache/media/ui-reviewed-v4/assets/asset-b4e86a4b600c95a2dd59f4adcee1df16884430d06860ed317759142fca3da9f4/asset-b4e86a4b600c95a2dd59f4adcee1df16884430d06860ed317759142fca3da9f4.frame-000.png")
	_rpg_shiori_sd = _load_first_existing_image(".asset-cache/media/ui-reviewed-v4/assets/asset-a72e5e886738abdd8d5e982bd4bd40e90c2872584f8ee9522245b1981f58de52/asset-a72e5e886738abdd8d5e982bd4bd40e90c2872584f8ee9522245b1981f58de52.frame-008.png")
	_rpg_enemy_sds = [
		_load_first_existing_image(".asset-cache/media/ui-reviewed-v4/assets/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0.frame-000.png"),
		_load_first_existing_image(".asset-cache/media/ui-reviewed-v4/assets/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0.frame-002.png"),
		_load_first_existing_image(".asset-cache/media/ui-reviewed-v4/assets/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0/asset-3a09f78e6cf4bb23870282feb35a3e9ed0ee3957c90251fe88bf7504716915c0.frame-001.png"),
	]
	_rpg_encounter_sprites = {
		"snake": _load_first_existing_image(".asset-cache/media/battle-verified-slice/assets/asset-9f97e8c083440d4f92bd950ac3b7946f1cc40be9cf23865c7ebfd7630fb3619c/asset-9f97e8c083440d4f92bd950ac3b7946f1cc40be9cf23865c7ebfd7630fb3619c.frame-000.png"),
		"deer": _load_first_existing_image(".asset-cache/media/battle-verified-slice/assets/asset-fd624ac434bc807a4959c0b220263a9b024d23c97987cc03824d8cf0de307f95/asset-fd624ac434bc807a4959c0b220263a9b024d23c97987cc03824d8cf0de307f95.frame-000.png"),
		"bear": _load_first_existing_image(".asset-cache/media/battle-verified-slice/assets/asset-18a9fa1893bd4305025614a0c36ec0c510a2cd68d718f3834319d34db125bfba/asset-18a9fa1893bd4305025614a0c36ec0c510a2cd68d718f3834319d34db125bfba.frame-000.png"),
		"robot": _load_first_existing_image(".asset-cache/media/battle-verified-slice/assets/asset-95060f47b78ca797791b03ef4c55b528091f9cb70fa492f0f30816455eb1a7e8/asset-95060f47b78ca797791b03ef4c55b528091f9cb70fa492f0f30816455eb1a7e8.frame-000.png"),
		"aliens": _load_first_existing_image(".asset-cache/media/battle-verified-slice/assets/asset-142a160c7469f2fc5d957f809b32fdbf78b00e7c85c51660ec48f1bd2cf8e968/asset-142a160c7469f2fc5d957f809b32fdbf78b00e7c85c51660ec48f1bd2cf8e968.frame-000.png"),
	}
	_init_rpg_bat_bins()
	_taco_bg = _load_first_existing_image(".asset-cache/media/mdec-v3-taco-0/taco-0.background.png")
	_taco_bg_1 = _load_first_existing_image(".asset-cache/media/mdec-v3-taco-1/taco-1.background.png")
	_taco_exn_bytes = _load_first_existing_bytes("disc/files/CDROM/EXEDIR/TACO.EXN")
	var tacotex_bytes := _load_first_existing_bytes("disc/files/CDROM/TACO/TACOTEX.NBN")
	var jimen_bytes := _load_first_existing_bytes("disc/files/CDROM/TACO/JIMEN.NBN")
	var jiki_tmd_bytes := _load_first_existing_bytes("disc/files/CDROM/TACO/JIKI.TMD")
	if _taco_exn_bytes.size() >= 0x30000 and not tacotex_bytes.is_empty() and not jimen_bytes.is_empty() and not jiki_tmd_bytes.is_empty():
		_init_taco_vram_and_tmd(tacotex_bytes, jimen_bytes, jiki_tmd_bytes)
	else:
		_taco_vram_ready = false
	_tt_tbta_bytes = _load_first_existing_bytes("disc/files/CDROM/TWINSGD/TBTA.DAT")
	_tt_exn_bytes = _load_first_existing_bytes("disc/files/CDROM/EXEDIR/TT.EXN")
	if _tt_tbta_bytes.size() >= 0x54900 and _tt_exn_bytes.size() >= 0x30000:
		_init_tt_vram_from_tbta()
	else:
		_tt_vram_ready = false
	var err: String = _font.open_external_font(font_root)
	if not err.is_empty():
		return "Failed to open native font: " + err
	_font_initialized = true
	return ""

func _upload_taco_vram_square(src: PackedByteArray, src_off: int, vx: int, vy: int, vw: int, vh: int) -> void:
	var idx: int = 0
	for ry in range(vh):
		var row_base: int = ((vy + ry) & 0x1FF) * 1024
		for rx in range(vw):
			var byte_off: int = src_off + idx * 2
			var val: int = src.decode_u16(byte_off) if (byte_off + 2 <= src.size()) else 0
			_taco_vram[row_base + ((vx + rx) & 0x3FF)] = val
			idx += 1

func _upload_taco_vram_tpage(src: PackedByteArray, tpage_id: int, src_off: int = 0) -> void:
	## Exact port of `shooter_load_texture_page` (`src/ovl/shooter_3d/text_assets_and_camera.c:115-133`).
	var x: int = (tpage_id & 0xF) << 6
	var y: int = (tpage_id & 0x10) << 4
	_upload_taco_vram_square(src, src_off + 0x0000, x, y, 0x40, 0x80)
	_upload_taco_vram_square(src, src_off + 0x4000, x + 0x40, y, 0x40, 0x80)
	_upload_taco_vram_square(src, src_off + 0x8000, x, y + 0x80, 0x40, 0x80)
	_upload_taco_vram_square(src, src_off + 0xC000, x + 0x40, y + 0x80, 0x40, 0x80)

func _ensure_taco_boss_vram_bank(boss_bank: int) -> void:
	if _taco_active_boss_bank == boss_bank:
		return
	match boss_bank:
		1: # Stage 1 Boss (`BS01.TMD` + `BS01.NBN` via `load_csr_tp` at `(0x200, 0)` in `morph_boss_scene.c:39`)
			var bs01_nbn: PackedByteArray = _taco_nbn_banks.get("bs01_nbn", PackedByteArray())
			if not bs01_nbn.is_empty():
				_upload_taco_vram_square(bs01_nbn, 0, 0x200, 0, 0x80, 0x100)
		2: # Stage 2 Boss (`DRA.TMD` + `T_DRA.NBN` via `shooter_load_texture_page(..., 8)` in `arm_tail_boss_scene.c:43`)
			var tdra_nbn: PackedByteArray = _taco_nbn_banks.get("tdra_nbn", PackedByteArray())
			if not tdra_nbn.is_empty():
				_upload_taco_vram_tpage(tdra_nbn, 0x08)
		3: # Stage 3 Boss (`TAKO.TMD` + `T_TAKO1.NBN` at 0x08 & `T_TAKO2.NBN` at 0x0A in `morph_and_collapse_bosses.c:40-41`)
			var tako1_nbn: PackedByteArray = _taco_nbn_banks.get("ttako1_nbn", PackedByteArray())
			var tako2_nbn: PackedByteArray = _taco_nbn_banks.get("ttako2_nbn", PackedByteArray())
			if not tako1_nbn.is_empty():
				_upload_taco_vram_tpage(tako1_nbn, 0x08)
			if not tako2_nbn.is_empty():
				_upload_taco_vram_tpage(tako2_nbn, 0x0A)
	_taco_active_boss_bank = boss_bank

func _parse_taco_tmd_object(data: PackedByteArray, obj_idx: int) -> Dictionary:
	var hdr_off: int = 12 + obj_idx * 28
	if hdr_off + 28 > data.size():
		return {"verts": [], "norms": [], "prims": []}
	var vtop: int = data.decode_u32(hdr_off + 0)
	var nvert: int = data.decode_u32(hdr_off + 4)
	var ntop: int = data.decode_u32(hdr_off + 8)
	var nnorm: int = data.decode_u32(hdr_off + 12)
	var ptop: int = data.decode_u32(hdr_off + 16)
	var nprim: int = data.decode_u32(hdr_off + 20)
	var verts: Array[Vector3i] = []
	verts.resize(nvert)
	for i in range(nvert):
		var vo: int = 12 + vtop + i * 8
		verts[i] = Vector3i(data.decode_s16(vo), data.decode_s16(vo + 2), data.decode_s16(vo + 4))
	var norms: Array[Vector3i] = []
	norms.resize(nnorm)
	for i in range(nnorm):
		var no: int = 12 + ntop + i * 8
		norms[i] = Vector3i(data.decode_s16(no), data.decode_s16(no + 2), data.decode_s16(no + 4))
	var prims: Array[Dictionary] = []
	var pos: int = 12 + ptop
	for _pi in range(nprim):
		if pos + 4 > data.size():
			break
		var ilen: int = data[pos + 1]
		var mode: int = data[pos + 3]
		var body: int = pos + 4
		if mode == 0x20: # F3
			prims.append({
				"mode": 0x20,
				"v0": data.decode_u16(body + 6),
				"v1": data.decode_u16(body + 8),
				"v2": data.decode_u16(body + 10),
				"n0": data.decode_u16(body + 4),
				"rgb": Vector3i(data[body + 0], data[body + 1], data[body + 2]),
			})
		elif mode == 0x21: # NF3
			prims.append({
				"mode": 0x21,
				"v0": data.decode_u16(body + 4),
				"v1": data.decode_u16(body + 6),
				"v2": data.decode_u16(body + 8),
				"rgb": Vector3i(data[body + 0], data[body + 1], data[body + 2]),
			})
		elif mode == 0x24: # FT3
			prims.append({
				"mode": 0x24,
				"v0": data.decode_u16(body + 14),
				"v1": data.decode_u16(body + 16),
				"v2": data.decode_u16(body + 18),
				"n0": data.decode_u16(body + 12),
				"uv0": Vector2i(data[body + 0], data[body + 1]),
				"cba": data.decode_u16(body + 2),
				"uv1": Vector2i(data[body + 4], data[body + 5]),
				"tsb": data.decode_u16(body + 6),
				"uv2": Vector2i(data[body + 8], data[body + 9]),
			})
		elif mode == 0x25: # NFT3
			prims.append({
				"mode": 0x25,
				"v0": data.decode_u16(body + 16),
				"v1": data.decode_u16(body + 18),
				"v2": data.decode_u16(body + 20),
				"uv0": Vector2i(data[body + 0], data[body + 1]),
				"cba": data.decode_u16(body + 2),
				"uv1": Vector2i(data[body + 4], data[body + 5]),
				"tsb": data.decode_u16(body + 6),
				"uv2": Vector2i(data[body + 8], data[body + 9]),
				"rgb": Vector3i(data[body + 12], data[body + 13], data[body + 14]),
			})
		elif mode == 0x34: # GT3
			prims.append({
				"mode": 0x34,
				"n0": data.decode_u16(body + 12),
				"v0": data.decode_u16(body + 14),
				"n1": data.decode_u16(body + 16),
				"v1": data.decode_u16(body + 18),
				"n2": data.decode_u16(body + 20),
				"v2": data.decode_u16(body + 22),
				"uv0": Vector2i(data[body + 0], data[body + 1]),
				"cba": data.decode_u16(body + 2),
				"uv1": Vector2i(data[body + 4], data[body + 5]),
				"tsb": data.decode_u16(body + 6),
				"uv2": Vector2i(data[body + 8], data[body + 9]),
			})
		pos += 4 + ilen * 4
	return {"verts": verts, "norms": norms, "prims": prims}

func _build_taco_multipart_model(tmd_bytes: PackedByteArray, parts_spec: Array) -> Array[Dictionary]:
	var assembled: Array[Dictionary] = []
	for spec: Dictionary in parts_spec:
		var obj_idx: int = int(spec["obj_idx"])
		var parsed: Dictionary = _parse_taco_tmd_object(tmd_bytes, obj_idx)
		parsed["tx"] = int(spec.get("tx", 0))
		parsed["ty"] = int(spec.get("ty", 0))
		parsed["tz"] = int(spec.get("tz", 0))
		parsed["rot"] = spec.get("rot", null)
		assembled.append(parsed)
	return assembled

func _build_taco_boss_parts_spec(mtx_bytes: PackedByteArray, ptr_table_addr: int, count: int, dy_off: int = 0, dz_off: int = 0) -> Array[Dictionary]:
	var specs: Array[Dictionary] = []
	var tbl_off: int = ptr_table_addr - TACO_EXN_BASE
	for i in range(count):
		var t_ptr: int = _taco_exn_bytes.decode_u32(tbl_off + i * 8 + 0)
		var r_ptr: int = _taco_exn_bytes.decode_u32(tbl_off + i * 8 + 4)
		var t_off: int = t_ptr - TACO_MTX_BASE
		var r_off: int = r_ptr - TACO_MTX_BASE
		var tx: int = mtx_bytes.decode_s16(t_off + 0)
		var ty: int = mtx_bytes.decode_s16(t_off + 2) + dy_off
		var tz: int = mtx_bytes.decode_s16(t_off + 4) + dz_off
		var rot := PackedInt32Array([
			mtx_bytes.decode_s16(r_off + 0), mtx_bytes.decode_s16(r_off + 2), mtx_bytes.decode_s16(r_off + 4),
			mtx_bytes.decode_s16(r_off + 6), mtx_bytes.decode_s16(r_off + 8), mtx_bytes.decode_s16(r_off + 10),
			mtx_bytes.decode_s16(r_off + 12), mtx_bytes.decode_s16(r_off + 14), mtx_bytes.decode_s16(r_off + 16),
		])
		specs.append({"obj_idx": i, "tx": tx, "ty": ty, "tz": tz, "rot": rot})
	return specs

func _init_taco_vram_and_tmd(tacotex: PackedByteArray, jimen: PackedByteArray, jiki_tmd: PackedByteArray) -> void:
	## Reconstructs TACO's 1024x512x16-bit PS1 VRAM (`asset_loading.c`, `game_start.c`,
	## `morph_boss_scene.c`, `arm_tail_boss_scene.c`, `morph_and_collapse_bosses.c`)
	## and parses the PS1 `.TMD` 3D polygon models from `CDROM/TACO/`.
	_taco_vram.resize(1024 * 512)
	_taco_vram.fill(0)
	_taco_active_boss_bank = -1

	# 1. TACOTEX.NBN (`shooter_upload_ui_textures` in `asset_loading.c:1031-1064`)
	_upload_taco_vram_square(tacotex, 0x0000, 0x360, 0x180, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0x0000, 0x3C0, 0x180, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0x2000, 0x340, 0x180, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0x4000, 0x340, 0x100, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0x6000, 0x300, 0x100, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0x8000, 0x300, 0x180, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0xA000, 0, 0x1EF, 0x100, 1)
	_upload_taco_vram_square(tacotex, 0xA200, 0x360, 0x100, 0x20, 0x80)
	_upload_taco_vram_square(tacotex, 0xC200, 0x3C0, 0x100, 0x40, 0x80)
	_upload_taco_vram_square(tacotex, 0x10200, 0x100, 0x1F8, 0x100, 3)

	# 2. TACOETC.NBN (`shooter_upload_effect_textures` in `asset_loading.c:1066-1089`)
	var tacoetc := _load_first_existing_bytes("disc/files/CDROM/TACO/TACOETC.NBN")
	if not tacoetc.is_empty():
		_upload_taco_vram_square(tacoetc, 0x0000, 0x380, 0x100, 0x20, 0x80)
		_upload_taco_vram_square(tacoetc, 0x2000, 0x3A0, 0x100, 0x20, 0x80)
		_upload_taco_vram_square(tacoetc, 0x4000, 0x380, 0x180, 0x20, 0x80)
		_upload_taco_vram_square(tacoetc, 0x6000, 0x3A0, 0x180, 0x20, 0x80)
		for s0 in range(15):
			_upload_taco_vram_square(tacoetc, 0x8000 + s0 * 0x200, 0, 0x1E0 + s0, 0x100, 1)

	# 3. ENE_COL.NBN & SYS_COL.NBN (`game_start.c:48-55`)
	var enecol := _load_first_existing_bytes("disc/files/CDROM/TACO/ENE_COL.NBN")
	if not enecol.is_empty():
		_upload_taco_vram_square(enecol, 0, 0x100, 0x1E0, 0x100, 20)
	var syscol := _load_first_existing_bytes("disc/files/CDROM/TACO/SYS_COL.NBN")
	if not syscol.is_empty():
		_upload_taco_vram_square(syscol, 0, 0, 0x1F0, 0x100, 12)

	# 4. JIMEN.NBN (`tpage 0x18`), TACOJIKI.NBN (`tpage 0x1A`), ZK_1.NBN (`tpage 0x0E`), ZK_2.NBN (`tpage 0x0C`)
	_upload_taco_vram_tpage(jimen, 0x18)
	var tacojiki := _load_first_existing_bytes("disc/files/CDROM/TACO/TACOJIKI.NBN")
	if not tacojiki.is_empty():
		_upload_taco_vram_tpage(tacojiki, 0x1A)
	var zk1_nbn := _load_first_existing_bytes("disc/files/CDROM/TACO/ZK_1.NBN")
	if not zk1_nbn.is_empty():
		_upload_taco_vram_square(zk1_nbn, 0, 0x380, 0, 0x80, 0x100)
	var zk2_nbn := _load_first_existing_bytes("disc/files/CDROM/TACO/ZK_2.NBN")
	if not zk2_nbn.is_empty():
		_upload_taco_vram_square(zk2_nbn, 0, 0x300, 0, 0x80, 0x100)

	# 5. Cache boss texture banks (`BS01.NBN`, `T_DRA.NBN`, `T_TAKO1.NBN`, `T_TAKO2.NBN`)
	_taco_nbn_banks = {
		"bs01_nbn": _load_first_existing_bytes("disc/files/CDROM/TACO/BS01.NBN"),
		"tdra_nbn": _load_first_existing_bytes("disc/files/CDROM/TACO/T_DRA.NBN"),
		"ttako1_nbn": _load_first_existing_bytes("disc/files/CDROM/TACO/T_TAKO1.NBN"),
		"ttako2_nbn": _load_first_existing_bytes("disc/files/CDROM/TACO/T_TAKO2.NBN"),
	}

	# 6. Parse `.TMD` 3D polygon models (`JIKI.TMD`, `ZK00..02.TMD`, `WANG.TMD`, `BS01.TMD`, `DRA.TMD`, `TAKO.TMD`)
	_taco_models.clear()
	_taco_models["jiki_player"] = _build_taco_multipart_model(jiki_tmd, [{"obj_idx": 0}])
	_taco_models["jiki_wingman"] = _build_taco_multipart_model(jiki_tmd, [{"obj_idx": 1}])

	var zk00_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/ZK00.TMD")
	if not zk00_tmd.is_empty():
		_taco_models["zk00"] = _build_taco_multipart_model(zk00_tmd, [{"obj_idx": 0}])
	var zk02_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/ZK02.TMD")
	if not zk02_tmd.is_empty():
		_taco_models["zk02"] = _build_taco_multipart_model(zk02_tmd, [{"obj_idx": 0}])

	var wang_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/WANG.TMD")
	if not wang_tmd.is_empty():
		var wang_specs: Array[Dictionary] = []
		var w_tbl: int = TACO_WANG_PART_PTRS_ADDR - TACO_EXN_BASE
		for j in range(5):
			var ptr: int = _taco_exn_bytes.decode_u32(w_tbl + j * 4)
			var roff: int = ptr - TACO_EXN_BASE
			wang_specs.append({
				"obj_idx": j,
				"tx": _taco_exn_bytes.decode_s16(roff + 0),
				"ty": _taco_exn_bytes.decode_s16(roff + 2),
				"tz": _taco_exn_bytes.decode_s16(roff + 4),
				"rot": null,
			})
		_taco_models["wang"] = _build_taco_multipart_model(wang_tmd, wang_specs)

	var bs01_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/BS01.TMD")
	var bs01_mtx := _load_first_existing_bytes("disc/files/CDROM/TACO/BS01_MTX.NBN")
	if not bs01_tmd.is_empty() and not bs01_mtx.is_empty():
		_taco_models["bs01"] = _build_taco_multipart_model(
			bs01_tmd,
			_build_taco_boss_parts_spec(bs01_mtx, TACO_BS01_POSE_PTRS_ADDR, 30, 0, 0)
		)

	var dra_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/DRA.TMD")
	var dra_mtx := _load_first_existing_bytes("disc/files/CDROM/TACO/DRA_MTX.NBN")
	if not dra_tmd.is_empty() and not dra_mtx.is_empty():
		_taco_models["dra"] = _build_taco_multipart_model(
			dra_tmd,
			_build_taco_boss_parts_spec(dra_mtx, TACO_DRA_POSE_PTRS_ADDR, 35, 0, 1500)
		)

	var tako_tmd := _load_first_existing_bytes("disc/files/CDROM/TACO/TAKO.TMD")
	var tako_mtx := _load_first_existing_bytes("disc/files/CDROM/TACO/TAKO_MTX.NBN")
	if not tako_tmd.is_empty() and not tako_mtx.is_empty():
		_taco_models["tako"] = _build_taco_multipart_model(
			tako_tmd,
			_build_taco_boss_parts_spec(tako_mtx, TACO_TAKO_POSE_PTRS_ADDR, 33, 400, 1800)
		)

	_taco_vram_ready = true

func _upload_tt_vram_rect(src: PackedByteArray, src_off: int, vx: int, vy: int, vw: int, vh: int, clear_clut_zero: bool = false) -> void:
	var idx: int = 0
	for row in range(vh):
		var vram_row: int = (vy + row) * 1024
		for col in range(vw):
			var byte_off: int = src_off + idx * 2
			var val: int = src.decode_u16(byte_off) if (byte_off + 2 <= src.size()) else 0
			if clear_clut_zero and ((idx & 0xF) == 0):
				val = 0
			_tt_vram[vram_row + vx + col] = val
			idx += 1

func _init_tt_vram_from_tbta() -> void:
	## Exact reconstruction of `tt_assets_load()` (`src/ovl/TT/player_and_asset_uploads.c:62-114`):
	## Loads `CDROM/TWINSGD/TBTA.DAT` (`0x54900` bytes at `0x80162000`, LBA `0xBECA`) into the
	## 1024x512x16-bit PS1 VRAM buffer alongside `tt_vram_palette_row` (`0x80156788` in `TT.EXN`).
	_tt_vram.resize(1024 * 512)
	_tt_vram.fill(0)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x00000, 0, 0xF0, 0x100, 0x10, true)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x02000, 0x100, 0xF0, 0x100, 0x10, true)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x04000, 0x280, 0, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x06000, 0x280, 0x80, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x08000, 0x2A0, 0, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x0A000, 0x2A0, 0x80, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x0C000, 0x2C0, 0, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x0E000, 0x2C0, 0x80, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x10000, 0x2E0, 0, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x12000, 0x380, 0, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x14000, 0x380, 0x80, 0x20, 0x80)
	var off: int = 0x16000
	for i in range(11):
		_upload_tt_vram_rect(_tt_tbta_bytes, off, i * 0x20, 0x100, 0x20, 0x80)
		_upload_tt_vram_rect(_tt_tbta_bytes, off + 0x2000, i * 0x20, 0x180, 0x20, 0x80)
		off += 0x4000
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x3E000, 0x200, 0x100, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x40000, 0x200, 0x180, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x42000, 0x260, 0x100, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x44000, 0x260, 0x180, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x46000, 0x280, 0x100, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x48000, 0x280, 0x180, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x4A000, 0x2A0, 0x100, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x4C000, 0x240, 0x100, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x4E000, 0x240, 0x180, 0x20, 0x80)
	_upload_tt_vram_rect(_tt_tbta_bytes, 0x54000, 0x3C0, 0x180, 0x18, 0x30)
	_upload_tt_vram_rect(_tt_exn_bytes, TT_VRAM_PALETTE_ROW_ADDR - TT_EXN_BASE, 0x3C0, 0x1FF, 0x10, 1)
	_tt_vram_ready = true

func _tt_exn_u8(addr: int) -> int:
	var off: int = addr - TT_EXN_BASE
	if off < 0 or off >= _tt_exn_bytes.size():
		return 0
	return _tt_exn_bytes[off]

func _tt_exn_u16(addr: int) -> int:
	var off: int = addr - TT_EXN_BASE
	if off < 0 or off + 2 > _tt_exn_bytes.size():
		return 0
	return _tt_exn_bytes.decode_u16(off)

func _tt_exn_s16(addr: int) -> int:
	var off: int = addr - TT_EXN_BASE
	if off < 0 or off + 2 > _tt_exn_bytes.size():
		return 0
	return _tt_exn_bytes.decode_s16(off)

func _tt_exn_u32(addr: int) -> int:
	var off: int = addr - TT_EXN_BASE
	if off < 0 or off + 4 > _tt_exn_bytes.size():
		return 0
	return _tt_exn_bytes.decode_u32(off)

static func _ps1_bgr555_to_color(c16: int) -> Color:
	var r5: int = c16 & 0x1F
	var g5: int = (c16 >> 5) & 0x1F
	var b5: int = (c16 >> 10) & 0x1F
	return Color8((r5 << 3) | (r5 >> 2), (g5 << 3) | (g5 >> 2), (b5 << 3) | (b5 >> 2), 255)

func _blit_tt_vram_sprite(target: Image, screen_x: int, screen_y: int, w: int, h: int, u: int, v: int, tpage: int, clut: int, flip_flags: int = 0) -> void:
	if not _tt_vram_ready:
		return
	var tp_x: int = (tpage & 0xF) * 64
	var tp_y: int = ((tpage >> 4) & 1) * 256
	var clut_x: int = (clut & 0x3F) * 16
	var clut_y: int = (clut >> 6) & 0x1FF
	var clut_base: int = clut_y * 1024 + clut_x
	var flip_y: bool = (flip_flags & 1) != 0
	var flip_x: bool = (flip_flags & 2) != 0
	for dy in range(h):
		var py: int = screen_y + dy
		if py < 8 or py >= 240:
			continue
		var src_dy: int = (h - 1 - dy) if flip_y else dy
		var vv: int = (v + src_dy) & 0xFF
		var vram_row: int = (tp_y + vv) * 1024 + tp_x
		for dx in range(w):
			var px: int = screen_x + dx
			if px < 32 or px >= 288:
				continue
			var src_dx: int = (w - 1 - dx) if flip_x else dx
			var uu: int = (u + src_dx) & 0xFF
			var word: int = _tt_vram[vram_row + (uu >> 2)]
			var nib: int = (word >> ((uu & 3) * 4)) & 0xF
			var c16: int = _tt_vram[clut_base + nib]
			if c16 != 0:
				target.set_pixel(px, py, _ps1_bgr555_to_color(c16))

func _draw_tt_part_sprite(target: Image, center: Vector2i, set_idx: int, frame_idx: int, flip: int = 0, clut_override: bool = false, ovr_clut: int = 0) -> void:
	## Exact port of `tt_part_sprite_draw` (`src/ovl/TT/sprite_animation_and_followers.c:51-128`).
	if not _tt_vram_ready:
		return
	var set_ptr: int = _tt_exn_u32(TT_SPRITE_SET_TABLE_ADDR + set_idx * 4)
	if set_ptr < TT_EXN_BASE:
		return
	var frames_ptr: int = _tt_exn_u32(set_ptr + 0)
	var seqs_ptr: int = _tt_exn_u32(set_ptr + 4)
	var parts_ptr: int = _tt_exn_u32(set_ptr + 8)
	var attr_ptr: int = _tt_exn_u32(set_ptr + 12)
	var base_clut: int = _tt_exn_u16(set_ptr + 16)
	var seq_idx: int = _tt_exn_u16(frames_ptr + frame_idx * 4)
	var count: int = _tt_exn_u16(seqs_ptr + seq_idx * 4 + 0)
	var start: int = _tt_exn_u16(seqs_ptr + seq_idx * 4 + 2)
	for n in range(count):
		var part_addr: int = parts_ptr + (start + count - 1 - n) * 8
		var tile: int = _tt_exn_u16(part_addr + 0)
		var part_x: int = _tt_exn_s16(part_addr + 2)
		var part_y: int = _tt_exn_s16(part_addr + 4)
		var part_clut: int = _tt_exn_u8(part_addr + 6)
		var part_flags: int = _tt_exn_u8(part_addr + 7)
		var flip_flags: int = part_flags ^ flip
		var py: int = (center.y - part_y - 16) if ((flip & 1) != 0) else (center.y + part_y)
		var px: int = (center.x - part_x - 16) if ((flip & 2) != 0) else (center.x + part_x)
		var c: int = _tt_exn_u8(attr_ptr + (tile >> 6))
		var code: int = tile & 0x3F
		var u: int = (c & 0x80) + (code & 7) * 16
		var v: int = (c & 0x40) * 2 + (code >> 3) * 16
		var tpage: int = c & 0x1F
		var b: int = ovr_clut if clut_override else part_clut
		var clut_word: int = base_clut + (b & 0xF) + (b & 0xF0) * 4
		_blit_tt_vram_sprite(target, px, py, 16, 16, u, v, tpage, clut_word, flip_flags)

func _draw_tt_bg_tilemap(target: Image, scroll_progress: int) -> void:
	## Exact port of `tt_bg_tilemap_draw` (`src/ovl/TT/settings_and_background.c:494-532`).
	if not _tt_vram_ready:
		return
	var tm_index_ptr: int = _tt_exn_u32(TT_STAGE_TILE_MAP_ADDR + 0)
	var tm_record_ptr: int = _tt_exn_u32(TT_STAGE_TILE_MAP_ADDR + 4)
	var y: int = 0x300 - (scroll_progress & 0x3FF)
	var scroll_aligned: int = y & ~0xF
	var sub_y: int = y & 0xF
	for row in range(0, 0x100, 0x10):
		var sy: int = sub_y + row - 0x10
		for col in range(16):
			var sx: int = 160 + (col * 16) - 0x80
			var map_idx: int = _tt_exn_u8(tm_index_ptr + ((row + col + scroll_aligned) & 0x3FF))
			var r_addr: int = tm_record_ptr + map_idx * 3
			var r0: int = _tt_exn_u8(r_addr + 0)
			var r1: int = _tt_exn_u8(r_addr + 1)
			var r2: int = _tt_exn_u8(r_addr + 2)
			var u: int = (r1 & 7) << 4
			var v: int = (r0 << 7) + ((r1 >> 3) << 4)
			var clut: int = (((r2 >> 4) + 0xF0) << 6) + (r2 & 0xF)
			_blit_tt_vram_sprite(target, sx, sy, 16, 16, u, v, 0xE, clut, 0)

func _draw_tt_vram_glyph_codes(target: Image, screen_x: int, screen_y: int, codes: Array[int], palette: int = 1) -> void:
	## Exact port of `tt_text_draw` (`src/ovl/TT/text_and_time_display.c:17-64`):
	## 8x8 sprite-font glyphs on `tpage = 0xA`, `clut = (palette & 0xF) + 0x3C00`.
	var x_cur: int = screen_x
	var clut: int = (palette & 0xF) + 0x3C00
	for c in codes:
		if c == 0xFF:
			break
		if c != 0x3F:
			var u: int = (c & 0xF) * 8
			var v: int = (c >> 4) * 8
			_blit_tt_vram_sprite(target, x_cur, screen_y, 8, 8, u, v, 0xA, clut, 0)
		x_cur += 8

# ==============================================================================
# 1. RPG_BAT Party Battle HUD, Command Menu & Actor 130 (320x240)
# ==============================================================================

const RPG_BAT_RAM_BASE := 0x80180000
const RPG_BAT_GIRL_SPECS := {
	0: {"file": "VS_HUJI.BIN", "sh5": 0x801CB164, "fr5": 0x801C7114},
	1: {"file": "VS_KISA.BIN", "sh5": 0x801CF2CC, "fr5": 0x801CF250},
	2: {"file": "VS_HIMO.BIN", "sh5": 0x801D776C, "fr5": 0x801D777C},
	3: {"file": "VS_KATA.BIN", "sh5": 0x801D31F0, "fr5": 0x801D31FC},
	4: {"file": "VS_NIJI.BIN", "sh5": 0x801CF380, "fr5": 0x801CF2EC},
	5: {"file": "VS_KOSI.BIN", "sh5": 0x801CF228, "fr5": 0x801CF1B8},
	6: {"file": "VS_KIYO.BIN", "sh5": 0x801D32F4, "fr5": 0x801D3300},
	7: {"file": "VS_KAGA.BIN", "sh5": 0x801D3348, "fr5": 0x801D32AC},
	8: {"file": "VS_ASA.BIN",  "sh5": 0x801CF2B0, "fr5": 0x801C7230},
	9: {"file": "VS_MIKI.BIN", "sh5": 0x801CB1FC, "fr5": 0x801CB18C},
	10: {"file": "VS_YUMI.BIN", "sh5": 0x801CF2EC, "fr5": 0x801CF254},
	13: {"file": "VS_YOSI.BIN", "sh5": 0x801D32C8, "fr5": 0x801D32D4},
}

func _init_rpg_bat_bins() -> void:
	_rpg_bat_bins.clear()
	_rpg_bat_ram_cache.clear()
	_rpg_bat_girl_bins.clear()
	var bin_files := {
		"vs_ban": "disc/files/CDROM/BATTLE/VS_BAN.BIN",
		"park": "disc/files/CDROM/BATTLE/PARK.BIN",
		"utyu": "disc/files/CDROM/BATTLE/UTYU.BIN",
		"vs_habu": "disc/files/CDROM/BATTLE/VS_HABU.BIN",
		"vs_sika": "disc/files/CDROM/BATTLE/VS_SIKA.BIN",
		"vs_kuma": "disc/files/CDROM/BATTLE/VS_KUMA.BIN",
		"vs_robo": "disc/files/CDROM/BATTLE/VS_ROBO.BIN",
		"vs_ufo": "disc/files/CDROM/BATTLE/VS_UFO.BIN",
	}
	for k in bin_files.keys():
		_rpg_bat_bins[k] = _load_first_existing_bytes(str(bin_files[k]))
	var vs_ban: PackedByteArray = _rpg_bat_bins.get("vs_ban", PackedByteArray())
	var park: PackedByteArray = _rpg_bat_bins.get("park", PackedByteArray())
	_rpg_bat_ready = vs_ban.size() >= 0x30000 and park.size() >= 0x15000

func _get_rpg_bat_girl_bytes(girl_id: int) -> PackedByteArray:
	if _rpg_bat_girl_bins.has(girl_id):
		return _rpg_bat_girl_bins[girl_id]
	var gspec: Dictionary = RPG_BAT_GIRL_SPECS.get(girl_id, {})
	var fname: String = str(gspec.get("file", ""))
	if fname.is_empty():
		return PackedByteArray()
	var data := _load_first_existing_bytes("disc/files/CDROM/BATTLE/" + fname)
	_rpg_bat_girl_bins[girl_id] = data
	return data

func _resolve_rpg_bat_encounter_key(state: Dictionary) -> String:
	var ek: String = str(state.get("encounter_kind", ""))
	if not ek.is_empty():
		return ek
	var enc: int = int(state.get("encounter", Overlays.RPG_ENCOUNTER_DELINQUENTS))
	match enc:
		Overlays.RPG_ENCOUNTER_SNAKE:
			return "snake"
		Overlays.RPG_ENCOUNTER_DEER:
			return "deer"
		Overlays.RPG_ENCOUNTER_BEAR:
			return "bear"
		Overlays.RPG_ENCOUNTER_ROBOT:
			return "robot"
		Overlays.RPG_ENCOUNTER_ALIENS:
			return "aliens"
		_:
			return "delinquents"

func _get_rpg_bat_encounter_spec(enc_kind: String, bancho_escalated: bool) -> Dictionary:
	## Exact port of `rpg_stage_load_assets_2` (`battle_audio_and_asset_loading.c:120-261`),
	## `rpg_stage_asset_addresses` (`battle_asset_pointers.c:5-252`), and
	## `rpg_stage_palettes_load` (`battle_palette_loading.c:5-131`).
	var key: String = "bancho" if (enc_kind == "bancho" or bancho_escalated) else enc_kind
	if _rpg_bat_ram_cache.has(key):
		return _rpg_bat_ram_cache[key]
	var ram := PackedByteArray()
	ram.resize(0x58000)
	var spec := {}
	match key:
		"snake":
			var b_habu: PackedByteArray = _rpg_bat_bins.get("vs_habu", PackedByteArray())
			for i in range(mini(b_habu.size(), ram.size())):
				ram[i] = b_habu[i]
			spec = {
				"ram": ram,
				"sh0": 0x801B5758, "fr0": 0x801B576C, "pal0": 0x801BD94C,
				"sh1": 0x801948E0, "fr1": 0x801948EC, "pal1": 0x801B594C,
				"sh2": 0x8018C1F8, "fr2": 0x8018C204, "pal2": 0x801B994C,
				"sh3": 0x801BF9A4, "fr3": 0x80194598, "pal3": 0x801B794C,
				"pal4": 0x801BB94C,
				"clut_0": 0x801BD94C,
				"clut_bg": 0x801BD94C + 1 * 512,
				"clut_enemy": 0x801B594C + 0 * 512,
				"clut_guest": 0x801B994C + 0 * 512,
				"clut_hero": 0x801BB94C + 0 * 512,
			}
		"deer":
			var b_sika: PackedByteArray = _rpg_bat_bins.get("vs_sika", PackedByteArray())
			for i in range(mini(b_sika.size(), ram.size())):
				ram[i] = b_sika[i]
			spec = {
				"ram": ram,
				"sh0": 0x801AD620, "fr0": 0x801AD634, "pal0": 0x801B5814,
				"sh1": 0x8018C7E4, "fr1": 0x8018C7F0, "pal1": 0x801AD814,
				"sh2": 0x801B781C, "fr2": 0x80184160, "pal2": 0x801B1814,
				"sh3": 0x801B7864, "fr3": 0x8018C4E0, "pal3": 0x801AF814,
				"pal4": 0x801B3814,
				"clut_0": 0x801B5814,
				"clut_bg": 0x801B5814 + 2 * 512,
				"clut_enemy": 0x801AD814 + 1 * 512,
				"clut_guest": 0x801B1814 + 1 * 512,
				"clut_hero": 0x801B3814 + 0 * 512,
			}
		"bear":
			var b_kuma: PackedByteArray = _rpg_bat_bins.get("vs_kuma", PackedByteArray())
			for i in range(mini(b_kuma.size(), ram.size())):
				ram[i] = b_kuma[i]
			spec = {
				"ram": ram,
				"sh0": 0x801AD638, "fr0": 0x801AD64C, "pal0": 0x801B582C,
				"sh1": 0x8018C810, "fr1": 0x8018C81C, "pal1": 0x801AD82C,
				"sh2": 0x801B7864, "fr2": 0x801841FC, "pal2": 0x801B182C,
				"sh3": 0x801B78AC, "fr3": 0x8018C59C, "pal3": 0x801AF82C,
				"pal4": 0x801B382C,
				"clut_0": 0x801B582C,
				"clut_bg": 0x801B582C + 3 * 512,
				"clut_enemy": 0x801AD82C + 2 * 512,
				"clut_guest": 0x801B182C + 2 * 512,
				"clut_hero": 0x801B382C + 0 * 512,
			}
		"robot":
			var b_robo: PackedByteArray = _rpg_bat_bins.get("vs_robo", PackedByteArray())
			for i in range(mini(b_robo.size(), ram.size())):
				ram[i] = b_robo[i]
			spec = {
				"ram": ram,
				"sh0": 0x801BFFA0, "fr0": 0x801BFFB4, "pal0": 0x801C8194,
				"sh1": 0x80189030, "fr1": 0x8018904C, "pal1": 0x801C0194,
				"sh2": 0, "fr2": 0, "pal2": 0,
				"sh3": 0x801CA1CC, "fr3": 0x8018833C, "pal3": 0x801C2194,
				"pal4": 0x801C6194,
				"clut_0": 0x801C8194,
				"clut_bg": 0x801C8194 + 1 * 512,
				"clut_enemy": 0x801C0194 + 4 * 512,
				"clut_guest": 0x801C8194,
				"clut_hero": 0x801C6194 + 0 * 512,
			}
		"aliens":
			var b_ufo: PackedByteArray = _rpg_bat_bins.get("vs_ufo", PackedByteArray())
			for i in range(mini(b_ufo.size(), ram.size())):
				ram[i] = b_ufo[i]
			spec = {
				"ram": ram,
				"sh0": 0x801C20A4, "fr0": 0x801C20B8, "pal0": 0x801CA298,
				"sh1": 0x801A1234, "fr1": 0x801A1240, "pal1": 0x801C2298,
				"sh2": 0, "fr2": 0, "pal2": 0,
				"sh3": 0x801CC360, "fr3": 0x801A0C08, "pal3": 0x801C6298,
				"pal4": 0x801C8298,
				"clut_0": 0x801CA298,
				"clut_bg": 0x801CA298 + 6 * 512,
				"clut_enemy": 0x801C2298 + 5 * 512,
				"clut_guest": 0x801CA298,
				"clut_hero": 0x801C8298 + 0 * 512,
			}
		_:
			var b_ban: PackedByteArray = _rpg_bat_bins.get("vs_ban", PackedByteArray())
			var b_bg: PackedByteArray = _rpg_bat_bins.get("utyu" if key == "bancho" else "park", PackedByteArray())
			for i in range(mini(b_ban.size(), ram.size())):
				ram[i] = b_ban[i]
			var bg_off: int = 0x801B1000 - RPG_BAT_RAM_BASE
			for i in range(mini(b_bg.size(), ram.size() - bg_off)):
				ram[bg_off + i] = b_bg[i]
			spec = {
				"ram": ram,
				"sh0": 0x801C5DC8, "fr0": 0x801C5DDC, "pal0": 0x801AEB90,
				"sh1": 0x80188AC0, "fr1": 0x80188ADC, "pal1": 0x801A6B90,
				"sh2": 0, "fr2": 0, "pal2": 0,
				"sh3": 0x801B0BC8, "fr3": 0x8018833C, "pal3": 0x801A8B90,
				"pal4": 0x801ACB90,
				"clut_0": 0x801AEB90,
				"clut_bg": 0x801AEB90 + (5 if key == "bancho" else 4) * 512,
				"clut_enemy": (0x801A4B90 if key == "bancho" else (0x801A6B90 + 3 * 512)),
				"clut_guest": 0x801AEB90,
				"clut_hero": 0x801ACB90 + 0 * 512,
			}
	_rpg_bat_ram_cache[key] = spec
	return spec

func _draw_rpg_bat_frame(target: Image, ram: PackedByteArray, sheet_table_addr: int, frames_table_addr: int, clut_addr: int, frame_idx: int, offset_x: int = 0, offset_y: int = 0, use_origin_offset: bool = false) -> void:
	## Exact port of `actor_draw_all` (`src/main/actor_sprites.c:100-310`) for 8bpp 128x128
	## sprite sheets (`unk_04 == 8`) in `RPG_BAT`:
	## - `f_ptr = ((u32 *)frames_table_addr)[frame_idx]`
	## - 8-byte `SpriteFrame` header (`cnt, origin_x, origin_y, flags`)
	## - `cnt` 14-byte `SpritePart` records (`x, y, tex_id, pal_info, flip, w, h`)
	var fr_tbl_off: int = frames_table_addr - RPG_BAT_RAM_BASE
	var sh_tbl_off: int = sheet_table_addr - RPG_BAT_RAM_BASE
	var pal_off: int = clut_addr - RPG_BAT_RAM_BASE
	if fr_tbl_off < 0 or fr_tbl_off + (frame_idx + 1) * 4 > ram.size() or pal_off < 0 or pal_off + 512 > ram.size():
		return
	var f_ptr: int = ram.decode_u32(fr_tbl_off + frame_idx * 4)
	var f_off: int = f_ptr - RPG_BAT_RAM_BASE
	if f_off < 0 or f_off + 8 > ram.size():
		return
	var cnt: int = ram.decode_s16(f_off + 0)
	var ox: int = ram.decode_s16(f_off + 2)
	var oy: int = ram.decode_s16(f_off + 4)
	var base_dx: int = (offset_x - ox) if use_origin_offset else offset_x
	var base_dy: int = (offset_y - oy) if use_origin_offset else offset_y
	for j in range(cnt):
		var po: int = f_off + 8 + j * 14
		if po + 14 > ram.size():
			break
		var px: int = base_dx + ram.decode_s16(po + 0)
		var py: int = base_dy + ram.decode_s16(po + 2)
		var tex_id: int = ram.decode_u16(po + 4)
		var flip: int = ram.decode_u16(po + 8)
		var pw: int = ram.decode_s16(po + 10)
		var ph: int = ram.decode_s16(po + 12)
		var sheet_idx: int = tex_id >> 8
		if sh_tbl_off + (sheet_idx + 1) * 4 > ram.size():
			continue
		var sh_ptr: int = ram.decode_u32(sh_tbl_off + sheet_idx * 4)
		var sh_base: int = sh_ptr - RPG_BAT_RAM_BASE
		if sh_base < 0 or sh_base + 16384 > ram.size():
			continue
		var rem: int = tex_id & 0xFF
		var u0: int = (rem & 0xF) * 8
		var v0: int = (rem >> 4) * 8
		var flip_y: bool = (flip & 1) != 0
		var flip_x: bool = (flip & 2) != 0
		for dy in range(ph):
			var sy: int = py + dy
			var src_dy: int = (ph - 1 - dy) if flip_y else dy
			var vv: int = v0 + src_dy
			if sy < 0 or sy >= 240 or vv < 0 or vv >= 128:
				continue
			var sh_row: int = sh_base + vv * 128
			for dx in range(pw):
				var sx: int = px + dx
				var src_dx: int = (pw - 1 - dx) if flip_x else dx
				var uu: int = u0 + src_dx
				if sx < 0 or sx >= 320 or uu < 0 or uu >= 128:
					continue
				var idx8: int = ram[sh_row + uu]
				var c16: int = ram.decode_u16(pal_off + idx8 * 2)
				if c16 != 0:
					target.set_pixel(sx, py + dy, _ps1_bgr555_to_color(c16))

func _draw_rpg_bat_hud_number_4digit(target: Image, spec: Dictionary, value: int, y_screen: int) -> void:
	## Exact port of `rpg_hud_numbers_update` & `rpg_stage_hud_setup` (`battle_sprites_and_hud.c:44-169, 245-280`):
	## `ACT(112..123)` using `rpg_asset_frames_0` `frames 9..18` (digits `0..9`) at `x = 267, 276, 285, 294`, `y = 179, 195, 211`.
	var ram: PackedByteArray = spec["ram"]
	var sh0: int = int(spec["sh0"])
	var fr0: int = int(spec["fr0"])
	var clut0: int = int(spec["clut_0"])
	var val: int = clampi(value, 0, 9999)
	if val >= 1000:
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 9 + ((val / 1000) % 10), 267, y_screen)
	if val >= 100:
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 9 + ((val / 100) % 10), 276, y_screen)
	if val >= 10:
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 9 + ((val / 10) % 10), 285, y_screen)
	_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 9 + (val % 10), 294, y_screen)

func render_rpg_battle(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var enc_kind: String = _resolve_rpg_bat_encounter_key(state)
	var bancho_escalated: bool = bool(state.get("bancho_escalated", false)) or enc_kind == "bancho"
	var gauges: Array = state.get("enemy_slot_gauges", [5000, 5000, 5000])
	var p_action: int = int(state.get("player_action", 0))
	var is_guarding: bool = (p_action & Overlays.RPG_PLAYER_GUARD) != 0
	var is_attacking: bool = (p_action & Overlays.RPG_PLAYER_ATTACK) != 0
	var is_special: bool = (p_action & Overlays.RPG_PLAYER_SPECIAL) != 0
	var gid: int = int(state.get("girl_id", 0))
	var guest_active: bool = bool(state.get("guest_active", false)) or not str(state.get("guest_hex", "")).is_empty()
	var is_victory: bool = bool(state.get("victory", false)) or int(state.get("stage", 9)) == 10
	var is_defeat: bool = bool(state.get("defeat", false)) or int(state.get("stage", 9)) == 12
	var msg_hex_list: Array = state.get("message_lines_hex", [])

	# Exact `actor130` (`ACT(96 + 0x22)`) command menu state (`rpg_command_menu_update`, `hud_messages_and_swimming_effect.c:302-363`)
	var cmd_cursor: int = clampi(int(state.get("command_cursor", 0)), 0, 2)
	var actor130_seq: int = 5 + cmd_cursor
	var hit_y: int = 57 + 15 * cmd_cursor
	var screen_y: int = 177 + 15 * cmd_cursor
	var actor130_meta := {
		"actor_id": 130,
		"slot_offset": 0x22,
		"sequence": actor130_seq,
		"frame": actor130_seq,
		"command_cursor": cmd_cursor,
		"hit_rect": Rect2i(-43, hit_y, 35, 15),
		"screen_rect": Rect2i(117, screen_y, 35, 15),
		"highlight_sprite_rect": Rect2i(117, screen_y, 40, 16),
		"label_sprite_rect": Rect2i(120, 176 + 16 * cmd_cursor, 32, 16),
	}
	state["actor130"] = actor130_meta
	target.set_meta("actor130", actor130_meta)

	if _rpg_bat_ready:
		state["render_provenance"] = "authentic_rpg_bat_vram"
		target.set_meta("render_provenance", "authentic_rpg_bat_vram")
		var spec: Dictionary = _get_rpg_bat_encounter_spec(enc_kind, bancho_escalated)
		var ram: PackedByteArray = spec["ram"]
		var sh0: int = int(spec["sh0"])
		var fr0: int = int(spec["fr0"])
		var clut0: int = int(spec["clut_0"])
		var clut_bg: int = int(spec["clut_bg"])

		# 1. Authentic Battle Arena Background (`actor 127` / `0x1F`, `rpg_asset_frames_0` `frame 0`, `y = 0..168`)
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut_bg, 0, 0, 0)

		# 2. Authentic Enemies (`rpg_asset_frames_1` / `set 1`, `labels_intro_and_wave_transition.c:173-195`)
		var sh1: int = int(spec["sh1"])
		var fr1: int = int(spec["fr1"])
		var clut_enemy: int = int(spec["clut_enemy"])
		if bancho_escalated:
			if int(state.get("enemy_hp", 1)) > 0:
				_draw_rpg_bat_frame(target, ram, sh1, fr1, clut_enemy, 6, 0, 0)
		elif enc_kind == "delinquents":
			var delinq_targets := [Vector2i(0x78, 0x68), Vector2i(0x40, 0x58), Vector2i(0x38, 0x88)]
			for i in range(3):
				if i < gauges.size() and int(gauges[i]) > 0:
					_draw_rpg_bat_frame(target, ram, sh1, fr1, clut_enemy, 0, delinq_targets[i].x, delinq_targets[i].y, true)
		elif enc_kind == "robot":
			if int(state.get("enemy_hp", 1)) > 0:
				_draw_rpg_bat_frame(target, ram, sh1, fr1, clut_enemy, 0, 0x48, 0x70, true)
		else:
			if int(state.get("enemy_hp", 1)) > 0:
				_draw_rpg_bat_frame(target, ram, sh1, fr1, clut_enemy, 0, 0, 0)

		# 3. Authentic Hero (`ACT(96 + 0x30)`, `set 3`), Partner Girl (`ACT(96 + 0x1D)`, `set 5`), and Guest (`set 2`)
		var sh3: int = int(spec["sh3"])
		var fr3: int = int(spec["fr3"])
		var clut_hero: int = int(spec["clut_hero"])
		var hero_dx: int = -32 if is_attacking else 0
		_draw_rpg_bat_frame(target, ram, sh3, fr3, clut_hero, 0, hero_dx, 0)
		if is_guarding:
			_fill_rect(target, Rect2i(248, 64, 3, 48), Color8(96, 224, 255))

		if gid != 14:
			var pal3_base: int = int(spec["pal3"])
			var girl_pal_row: int = 3 if (gid == 2 and enc_kind == "aliens") else gid
			var clut_girl: int = pal3_base + girl_pal_row * 512
			if enc_kind == "aliens" and gid == 2:
				_draw_rpg_bat_frame(target, ram, 0x80180790, 0x801807A8, clut_girl, 0, 0, 0)
			elif RPG_BAT_GIRL_SPECS.has(gid):
				var gspec: Dictionary = RPG_BAT_GIRL_SPECS[gid]
				var g_bytes: PackedByteArray = _get_rpg_bat_girl_bytes(gid)
				if not g_bytes.is_empty():
					var g_off: int = 0x801C7000 - RPG_BAT_RAM_BASE
					for i in range(mini(g_bytes.size(), ram.size() - g_off)):
						ram[g_off + i] = g_bytes[i]
					_draw_rpg_bat_frame(target, ram, int(gspec["sh5"]), int(gspec["fr5"]), clut_girl, 0, 0, 0)
			else:
				_draw_rpg_partner_sprite(target, Vector2i(268, 100), gid)

		if guest_active:
			var sh2: int = int(spec["sh2"])
			var fr2: int = int(spec["fr2"])
			var clut_guest: int = int(spec["clut_guest"])
			if sh2 != 0 and fr2 != 0:
				_draw_rpg_bat_frame(target, ram, sh2, fr2, clut_guest, 0, 0, 0)

		# 4. Club Special Move visual effect, Damage Popups, Partner Assist Cut-In & Victory/Defeat Banners
		if is_special and not is_victory and not is_defeat:
			_draw_rpg_club_special_effect(target, int(state.get("player_club", 0)))
		var last_dmg: int = int(state.get("last_damage", 0))
		if last_dmg > 0 and not is_victory and not is_defeat:
			_draw_ascii_digits(target, "%d" % last_dmg, Vector2i(62, 82), Color8(255, 232, 64))
		var enemy_last_dmg: int = int(state.get("enemy_last_damage", 0))
		if enemy_last_dmg > 0 and not is_victory and not is_defeat:
			_draw_ascii_digits(target, "%d" % enemy_last_dmg, Vector2i(218, 76), Color8(255, 112, 96))

		var assist: Dictionary = state.get("last_assist", {})
		if bool(assist.get("supported", false)) and not is_victory and not is_defeat:
			var assist_lines: Array = assist.get("lines_hex", [])
			if not assist_lines.is_empty():
				var box_h: int = 38 if assist_lines.size() >= 2 else 26
				var assist_box := Rect2i(20, 42, 280, box_h)
				_draw_ps1_window(target, assist_box, Color8(48, 20, 76), Color8(24, 10, 40))
				_draw_text_hex_checked(target, str(assist_lines[0]), Vector2i(28, 46), 0, "rpg_assist_cutin", assist_box)
				if assist_lines.size() >= 2:
					_draw_text_hex_checked(target, str(assist_lines[1]), Vector2i(28, 62), 0, "rpg_assist_cutin", assist_box)
				_draw_rpg_assist_effect(target, str(assist.get("effect", "effect_a")))

		if is_victory:
			var vic_box := Rect2i(44, 52, 232, 28)
			_draw_ps1_window(target, vic_box, Color8(24, 68, 112), Color8(12, 36, 64))
			_draw_text_hex_checked(target, STR_HEX["rpg_victory"], Vector2i(58, 58), 0, "rpg_victory_banner", vic_box)
		elif is_defeat:
			var def_box := Rect2i(44, 52, 232, 28)
			_draw_ps1_window(target, def_box, Color8(88, 20, 32), Color8(44, 10, 18))
			_draw_text_hex_checked(target, STR_HEX["rpg_defeat"], Vector2i(58, 58), 0, "rpg_defeat_banner", def_box)

		# 5. Authentic Top Message Window (`actor 132` / `0x24`, `frame 3`, `x = 16..304, y = 8..40`)
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 3, 0, 0)
		var msg_bounds := Rect2i(16, 8, 288, 36)
		if msg_hex_list.size() >= 2:
			_draw_text_hex_checked(target, str(msg_hex_list[0]), Vector2i(28, 11), 0, "rpg_top_message", msg_bounds)
			_draw_text_hex_checked(target, str(msg_hex_list[1]), Vector2i(28, 25), 0, "rpg_top_message", msg_bounds)
		else:
			var top_hex: String = str(msg_hex_list[0]) if not msg_hex_list.is_empty() else STR_HEX["assist_shiori_1"]
			var top_x: int = 28 if (top_hex.length() / 4) >= 18 else 34
			_draw_text_hex_checked(target, top_hex, Vector2i(top_x, 18), 0, "rpg_top_message", msg_bounds)

		# 6. Authentic Bottom Status HUD (`actor 134` / `0x26`, `frame 1`, `x = 8..312, y = 168..232`) + 3 black border bars (`battle_dispatch.c:16-18`)
		_fill_rect(target, Rect2i(0, 168, 8, 72), Color.BLACK)
		_fill_rect(target, Rect2i(312, 168, 8, 72), Color.BLACK)
		_fill_rect(target, Rect2i(0, 232, 320, 8), Color.BLACK)
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 1, 0, 0)

		# Enemy group name at `(19, 187)` (`0x13, 0xBB` in `labels_intro_and_wave_transition.c:25-45`)
		var enemy_panel := Rect2i(10, 170, 148, 60)
		var enemy_hex: String = str(state.get("enemy_group_hex", STR_HEX["enemy_delinquents"]))
		_draw_text_hex_checked(target, enemy_hex, Vector2i(19, 187), 0, "rpg_enemy_hud", enemy_panel)

		# Party names at `x = 177, y = 179 / 195 / 211` (`0xB1, 0xB3 / 0xC3 / 0xD3`) + `ＨＰ` labels (`frame 8`) + 4-digit HP sprites (`frames 9..18`)
		var party_panel := Rect2i(162, 170, 148, 60)
		var p_name_hex: String = str(state.get("player_name_hex", STR_HEX["player_fukuhara"]))
		_draw_text_hex_checked(target, p_name_hex, Vector2i(177, 179), 0, "rpg_party_hud", party_panel)
		_draw_rpg_bat_hud_number_4digit(target, spec, int(state.get("player_hp", 970)), 179)

		if gid != 14:
			var g_name_hex: String = str(state.get("girl_short_name_hex", STR_HEX["girl_shiori"]))
			_draw_text_hex_checked(target, g_name_hex, Vector2i(177, 195), 0, "rpg_party_hud", party_panel)
			_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 8, 248, 200)
			_draw_rpg_bat_hud_number_4digit(target, spec, int(state.get("girl_hp", 256)), 195)

		if guest_active:
			var guest_hex: String = str(state.get("guest_hex", STR_HEX["guest_mongoose"]))
			if guest_hex.is_empty():
				guest_hex = STR_HEX["guest_mongoose"]
			_draw_text_hex_checked(target, guest_hex, Vector2i(177, 211), 0, "rpg_party_hud", party_panel)
			_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 8, 248, 216)
			_draw_rpg_bat_hud_number_4digit(target, spec, int(state.get("guest_hp", 117)), 211)

		# 7. Authentic Command Menu Window (`actor 133` / `0x25`, `frame 2`, `x = 108..164, y = 168..232`)
		# + `actor 131` (`0x23`, `frame 4`, `必殺` at `(120, 208, 32, 16)` when `special_ready == 1`)
		# + `actor 130` (`0x22`, `sequences 5/6/7` -> `frames 5/6/7` for Attack/Guard/Special selection)
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 2, 0, 0)
		if int(state.get("special_ready", 1)) == 1:
			_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, 4, 0, 0)
		_draw_rpg_bat_frame(target, ram, sh0, fr0, clut0, actor130_seq, 0, 0)

		return target

	# Fallback path if disc/files/CDROM/BATTLE/*.BIN is unavailable
	for y in range(168):
		var col := Color8(36, 48, 84, 255)
		for x in range(320):
			target.set_pixel(x, y, col)
	var msg_box := Rect2i(16, 8, 288, 32)
	_draw_ps1_window(target, msg_box, Color8(16, 28, 76), Color8(8, 14, 42))
	var top_hex_fb: String = str(msg_hex_list[0]) if not msg_hex_list.is_empty() else STR_HEX["assist_shiori_1"]
	_draw_text_hex_checked(target, top_hex_fb, Vector2i(28, 18), 0, "rpg_top_message", msg_box)
	return target


# ==============================================================================
# 2. TACO: 3D Rail Shooter Horizon, TMD Models & VRAM HUD (320x240)
# ==============================================================================

func _sample_taco_vram_tpage(tpage: int, clut: int, u: int, v: int) -> int:
	var tp_x: int = (tpage & 0xF) << 6
	var tp_y: int = ((tpage >> 4) & 1) << 8
	var tp_mode: int = (tpage >> 7) & 3
	var cx: int = (clut & 0x3F) << 4
	var cy: int = (clut >> 6) & 0x1FF
	var uu: int = u & 0xFF
	var vv: int = v & 0xFF
	var row_base: int = ((tp_y + vv) & 0x1FF) * 1024
	if tp_mode == 0:
		var w: int = _taco_vram[row_base + ((tp_x + (uu >> 2)) & 0x3FF)]
		var nib: int = (w >> ((uu & 3) * 4)) & 0xF
		return _taco_vram[cy * 1024 + ((cx + nib) & 0x3FF)]
	elif tp_mode == 1:
		var w8: int = _taco_vram[row_base + ((tp_x + (uu >> 1)) & 0x3FF)]
		var b_idx: int = (w8 >> ((uu & 1) * 8)) & 0xFF
		return _taco_vram[cy * 1024 + ((cx + b_idx) & 0x3FF)]
	return _taco_vram[row_base + ((tp_x + uu) & 0x3FF)]

func _blit_taco_vram_4bpp_512(target: Image, sx512: int, sy240: int, w512: int, h240: int, tx: int, ty: int, u0: int, v0: int, cx: int, cy: int) -> void:
	## Exact port of PS1 `SPRT` / `POLY_FT4` HUD blitting (`hud_secondary_weapons.c:5-270`,
	## `text_assets_and_camera.c:6-113`), mapping native `512x240` coordinates to `320x240`.
	var ty_base: int = ty & 0x100
	var x0: int = maxi(0, ((256 + sx512) * 5) / 8)
	var x1: int = mini(320, ((256 + sx512 + w512) * 5 + 7) / 8)
	var clut_row: int = (cy & 0x1FF) * 1024
	for dy in range(h240):
		var py: int = 120 + sy240 + dy
		if py < 0 or py >= 240:
			continue
		var vv: int = (v0 + dy) & 0xFF
		var row_base: int = ((ty_base + vv) & 0x1FF) * 1024
		for pxx in range(x0, x1):
			var dx512: int = (pxx * 8) / 5 - (256 + sx512)
			if dx512 < 0 or dx512 >= w512:
				continue
			var uu: int = (u0 + dx512) & 0xFF
			var w: int = _taco_vram[row_base + ((tx + (uu >> 2)) & 0x3FF)]
			var nib: int = (w >> ((uu & 3) * 4)) & 0xF
			var c16: int = _taco_vram[clut_row + ((cx + nib) & 0x3FF)]
			if c16 != 0:
				target.set_pixel(pxx, py, _ps1_bgr555_to_color(c16))

func _draw_taco_hud_char(target: Image, code: int, x512: int, y240: int, pal: int) -> void:
	var u: int = 0
	var v: int = 0
	if code >= 0x30 and code <= 0x39:
		var d_off: int = (TACO_DIGIT_UV_ADDR - TACO_EXN_BASE) + (code - 0x30) * 2
		u = _taco_exn_bytes[d_off] + 0x80
		v = _taco_exn_bytes[d_off + 1]
	elif code >= 0x41 and code <= 0x5A:
		var l_off: int = (TACO_LETTER_UV_ADDR - TACO_EXN_BASE) + (code - 0x41) * 2
		u = _taco_exn_bytes[l_off] + 0x80
		v = _taco_exn_bytes[l_off + 1]
	elif code == 0x2E: # '.'
		u = 0x98 + 0x80
		v = 0xD0
	elif code == 0x2D: # '-'
		u = 0x8C + 0x80
		v = 0xD0
	else:
		return
	_blit_taco_vram_4bpp_512(target, x512, y240, 12, 16, 0x340, 0x180, u, v, pal * 16, 0x1EF)

func _draw_taco_hud_text(target: Image, text: String, x512: int, y240: int, pal: int) -> void:
	## Exact port of `shooter_draw_text` (`src/ovl/shooter_3d/text_assets_and_camera.c:59-113`).
	for i in range(text.length()):
		_draw_taco_hud_char(target, text.unicode_at(i), x512 + i * 12, y240, pal)

func _draw_taco_hud_number(target: Image, val: int, arg1_x512: int, arg2_y240: int, pal: int) -> void:
	## Exact port of `shooter_draw_number` (`src/ovl/shooter_3d/text_assets_and_camera.c:9-56`).
	var n: int = absi(val)
	var xoff: int = 0
	while true:
		var digit: int = n % 10
		_draw_taco_hud_char(target, 0x30 + digit, arg1_x512 - xoff, arg2_y240, pal)
		if n < 10:
			break
		xoff += 12
		n /= 10

func _raster_taco_tri(target: Image, p0: Vector2i, p1: Vector2i, p2: Vector2i, uv0: Vector2i, uv1: Vector2i, uv2: Vector2i, tsb: int, cba: int, s0: Vector3i, s1: Vector3i, s2: Vector3i, is_tex: bool) -> void:
	var min_x: int = maxi(0, mini(p0.x, mini(p1.x, p2.x)))
	var max_x: int = mini(319, maxi(p0.x, maxi(p1.x, p2.x)))
	var min_y: int = maxi(0, mini(p0.y, mini(p1.y, p2.y)))
	var max_y: int = mini(239, maxi(p0.y, maxi(p1.y, p2.y)))
	if min_x > max_x or min_y > max_y:
		return
	var denom: int = (p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y)
	if denom == 0:
		return
	var inv_d: float = 1.0 / float(denom)
	for py in range(min_y, max_y + 1):
		var dy2: int = py - p2.y
		for px in range(min_x, max_x + 1):
			var dx2: int = px - p2.x
			var w0: float = float((p1.y - p2.y) * dx2 + (p2.x - p1.x) * dy2) * inv_d
			var w1: float = float((p2.y - p0.y) * dx2 + (p0.x - p2.x) * dy2) * inv_d
			var w2: float = 1.0 - w0 - w1
			if w0 >= -0.001 and w1 >= -0.001 and w2 >= -0.001:
				var sr: float = w0 * float(s0.x) + w1 * float(s1.x) + w2 * float(s2.x)
				var sg: float = w0 * float(s0.y) + w1 * float(s1.y) + w2 * float(s2.y)
				var sb: float = w0 * float(s0.z) + w1 * float(s1.z) + w2 * float(s2.z)
				if is_tex:
					var u: int = int(w0 * float(uv0.x) + w1 * float(uv1.x) + w2 * float(uv2.x) + 0.5)
					var v: int = int(w0 * float(uv0.y) + w1 * float(uv1.y) + w2 * float(uv2.y) + 0.5)
					var c16: int = _sample_taco_vram_tpage(tsb, cba, u, v)
					if c16 == 0:
						continue
					var r5: int = c16 & 0x1F
					var g5: int = (c16 >> 5) & 0x1F
					var b5: int = (c16 >> 10) & 0x1F
					var tr: int = (r5 << 3) | (r5 >> 2)
					var tg: int = (g5 << 3) | (g5 >> 2)
					var tb: int = (b5 << 3) | (b5 >> 2)
					target.set_pixel(px, py, Color8(
						clampi(int(float(tr) * sr / 128.0), 0, 255),
						clampi(int(float(tg) * sg / 128.0), 0, 255),
						clampi(int(float(tb) * sb / 128.0), 0, 255),
						255
					))
				else:
					target.set_pixel(px, py, Color8(
						clampi(int(sr), 0, 255),
						clampi(int(sg), 0, 255),
						clampi(int(sb), 0, 255),
						255
					))

func _render_taco_tmd_model(target: Image, model_parts: Array, cx: int, cy: int, proj_scale: float, rx_deg: float, ry_deg: float, rz_deg: float) -> void:
	if model_parts.is_empty():
		return
	var rx: float = deg_to_rad(rx_deg)
	var ry: float = deg_to_rad(ry_deg)
	var rz: float = deg_to_rad(rz_deg)
	var cx_r: float = cos(rx)
	var sx_r: float = sin(rx)
	var cy_r: float = cos(ry)
	var sy_r: float = sin(ry)
	var cz_r: float = cos(rz)
	var sz_r: float = sin(rz)
	var all_tris: Array[Dictionary] = []

	for part: Dictionary in model_parts:
		var verts: Array = part["verts"]
		var norms: Array = part["norms"]
		var prims: Array = part["prims"]
		var tx: float = float(part["tx"])
		var ty: float = float(part["ty"])
		var tz: float = float(part["tz"])
		var rot: Variant = part["rot"]
		var has_rot: bool = (rot is PackedInt32Array) and (rot as PackedInt32Array).size() == 9
		var r_arr: PackedInt32Array = rot if has_rot else PackedInt32Array()

		var t_verts: Array[Vector2i] = []
		var t_depths: PackedFloat32Array = PackedFloat32Array()
		t_verts.resize(verts.size())
		t_depths.resize(verts.size())

		for vi in range(verts.size()):
			var v: Vector3i = verts[vi]
			var lx: float = float(v.x)
			var ly: float = float(v.y)
			var lz: float = float(v.z)
			if has_rot:
				var rx_v: float = (float(r_arr[0]) * lx + float(r_arr[1]) * ly + float(r_arr[2]) * lz) / 4096.0
				var ry_v: float = (float(r_arr[3]) * lx + float(r_arr[4]) * ly + float(r_arr[5]) * lz) / 4096.0
				var rz_v: float = (float(r_arr[6]) * lx + float(r_arr[7]) * ly + float(r_arr[8]) * lz) / 4096.0
				lx = rx_v + tx
				ly = ry_v + ty
				lz = rz_v + tz
			else:
				lx += tx
				ly += ty
				lz += tz
			var y1: float = ly * cx_r - lz * sx_r
			var z1: float = ly * sx_r + lz * cx_r
			var x1: float = lx
			var x2: float = x1 * cz_r - y1 * sz_r
			var y2: float = x1 * sz_r + y1 * cz_r
			var z2: float = z1
			var wx: float = x2 * cy_r + z2 * sy_r
			var wz: float = -x2 * sy_r + z2 * cy_r
			var wy: float = y2
			var sx: int = int(float(cx) + (-wx) * proj_scale * 0.82)
			var sy: int = int(float(cy) + wy * proj_scale)
			t_verts[vi] = Vector2i(sx, sy)
			t_depths[vi] = -wz

		var norm_shades: Array[Vector3i] = []
		norm_shades.resize(norms.size())
		for ni in range(norms.size()):
			var n: Vector3i = norms[ni]
			var l: float = clampf(0.85 + (-float(n.y) * 0.35 - float(n.z) * 0.35) / 4096.0, 0.55, 1.35)
			var s_val: int = int(128.0 * l)
			norm_shades[ni] = Vector3i(s_val, s_val, s_val)

		for p: Dictionary in prims:
			var i0: int = int(p["v0"])
			var i1: int = int(p["v1"])
			var i2: int = int(p["v2"])
			var z_avg: float = (t_depths[i0] + t_depths[i1] + t_depths[i2]) / 3.0
			var mode: int = int(p["mode"])
			var s0 := Vector3i(128, 128, 128)
			var s1 := Vector3i(128, 128, 128)
			var s2 := Vector3i(128, 128, 128)
			var is_tex: bool = true
			if mode == 0x20: # F3
				is_tex = false
				var ns: Vector3i = norm_shades[int(p["n0"])] if int(p["n0"]) < norm_shades.size() else Vector3i(128, 128, 128)
				var rgb: Vector3i = p["rgb"]
				s0 = Vector3i((rgb.x * ns.x) >> 7, (rgb.y * ns.y) >> 7, (rgb.z * ns.z) >> 7)
				s1 = s0
				s2 = s0
			elif mode == 0x21: # NF3
				is_tex = false
				s0 = p["rgb"]
				s1 = s0
				s2 = s0
			elif mode == 0x24: # FT3
				s0 = norm_shades[int(p["n0"])] if int(p["n0"]) < norm_shades.size() else Vector3i(128, 128, 128)
				s1 = s0
				s2 = s0
			elif mode == 0x25: # NFT3
				s0 = p["rgb"]
				s1 = s0
				s2 = s0
			elif mode == 0x34: # GT3
				s0 = norm_shades[int(p["n0"])] if int(p["n0"]) < norm_shades.size() else Vector3i(128, 128, 128)
				s1 = norm_shades[int(p["n1"])] if int(p["n1"]) < norm_shades.size() else Vector3i(128, 128, 128)
				s2 = norm_shades[int(p["n2"])] if int(p["n2"]) < norm_shades.size() else Vector3i(128, 128, 128)
			all_tris.append({
				"z": z_avg,
				"p0": t_verts[i0],
				"p1": t_verts[i1],
				"p2": t_verts[i2],
				"is_tex": is_tex,
				"uv0": p.get("uv0", Vector2i.ZERO),
				"uv1": p.get("uv1", Vector2i.ZERO),
				"uv2": p.get("uv2", Vector2i.ZERO),
				"tsb": int(p.get("tsb", 0)),
				"cba": int(p.get("cba", 0)),
				"s0": s0,
				"s1": s1,
				"s2": s2,
			})

	all_tris.sort_custom(func(a: Dictionary, b: Dictionary) -> bool: return float(a["z"]) > float(b["z"]))
	for tri: Dictionary in all_tris:
		_raster_taco_tri(
			target,
			tri["p0"], tri["p1"], tri["p2"],
			tri["uv0"], tri["uv1"], tri["uv2"],
			int(tri["tsb"]), int(tri["cba"]),
			tri["s0"], tri["s1"], tri["s2"],
			bool(tri["is_tex"])
		)

func render_taco_shooter(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var stg: int = int(state.get("stage", Overlays.SHOOTER_STAGE_STAGE1))
	var p: Dictionary = state.get("player", {})
	var px_world: int = int(p.get("x", 0))
	var py_world: int = int(p.get("y", -0x200))
	var frame_cnt: int = int(state.get("frame_count", 0))

	if _taco_vram_ready:
		state["render_provenance"] = "authentic_taco_vram_tmd"
		target.set_meta("render_provenance", "authentic_taco_vram_tmd")

		# 1. Authentic 15-band Gouraud Sky + `shooter_draw_horizon_bands` (`JIMEN.NBN` + CLUT `0x1EE`)
		# + `shooter_draw_floor_grid` (`JIMEN.NBN` + CLUT `0x1ED`) (`background_and_models.c:10-420`)
		var sky_pals := [
			Vector3i(0x10, 0x20, 0x58), Vector3i(0x18, 0x2C, 0x6C), Vector3i(0x20, 0x38, 0x80), Vector3i(0x2C, 0x48, 0x94),
			Vector3i(0x38, 0x58, 0xA8), Vector3i(0x48, 0x68, 0xB8), Vector3i(0x58, 0x78, 0xC8), Vector3i(0x68, 0x88, 0xD4),
			Vector3i(0x78, 0x98, 0xDF), Vector3i(0x8C, 0xAC, 0xE8), Vector3i(0xA0, 0xBC, 0xF0), Vector3i(0xB4, 0xCC, 0xF4),
			Vector3i(0xC8, 0xDC, 0xF8), Vector3i(0xDC, 0xEC, 0xFC), Vector3i(0xEC, 0xF4, 0xFF), Vector3i(0xF8, 0xF8, 0xFF),
		]
		var tpage_jimen: int = (1 << 7) | ((0x100 & 0x100) >> 4) | ((0x200 & 0x3FF) >> 6)
		var clut_horizon: int = (0x1EE << 6) | 0
		var clut_floor: int = (0x1ED << 6) | 0
		var scroll_x_off: int = (px_world / 64) + frame_cnt * 2
		var scroll_z_off: int = frame_cnt * 8

		for y in range(240):
			var dy: int = y - 120
			if dy < 20:
				var t: float = clampf(float(dy + 120) / 140.0 * 15.0, 0.0, 14.99)
				var idx: int = int(t)
				var f: float = t - float(idx)
				var c0: Vector3i = sky_pals[idx]
				var c1: Vector3i = sky_pals[idx + 1]
				var sky_col := Color8(
					int(float(c0.x) * (1.0 - f) + float(c1.x) * f),
					int(float(c0.y) * (1.0 - f) + float(c1.y) * f),
					int(float(c0.z) * (1.0 - f) + float(c1.z) * f),
					255
				)
				for x in range(320):
					target.set_pixel(x, y, sky_col)
				if dy >= -12 and dy < 20:
					var v_h: int = dy + 12
					for x in range(320):
						var dx512: int = (x * 8) / 5 - 256
						var u_h: int = (dx512 + 256 + scroll_x_off) & 0xFF
						var c16_h: int = _sample_taco_vram_tpage(tpage_jimen, clut_horizon, u_h, v_h)
						if c16_h != 0:
							target.set_pixel(x, y, _ps1_bgr555_to_color(c16_h))
			else:
				var row_dist: int = maxi(1, dy - 16)
				var z: int = 0x2000 / row_dist
				var v_f: int = (((z * 4) + scroll_z_off) & 0x7F) + 0x80
				var shade: float = clampf(float(row_dist) / 85.0, 0.45, 1.15)
				for x in range(320):
					var dx512: int = (x * 8) / 5 - 256
					var wx: int = ((dx512 * z) / 180) + scroll_x_off
					var u_f: int = wx & 0x7F
					var c16_f: int = _sample_taco_vram_tpage(tpage_jimen, clut_floor, u_f, v_f)
					var r5: int = c16_f & 0x1F
					var g5: int = (c16_f >> 5) & 0x1F
					var b5: int = (c16_f >> 10) & 0x1F
					var r8: int = (r5 << 3) | (r5 >> 2)
					var g8: int = (g5 << 3) | (g5 >> 2)
					var b8: int = (b5 << 3) | (b5 >> 2)
					target.set_pixel(x, y, Color8(
						clampi(int(float(r8) * shade), 0, 255),
						clampi(int(float(g8) * shade), 0, 255),
						clampi(int(float(b8) * shade), 0, 255),
						255
					))

		# 2. Active Stage Boss (`BS01.TMD` Stage 1, `DRA.TMD` Stage 2, `TAKO.TMD` Stage 3)
		var boss: Dictionary = state.get("boss", {})
		var boss_active: bool = int(boss.get("active", 0)) != 0 or stg in [
			Overlays.SHOOTER_STAGE_BOSS1, Overlays.SHOOTER_STAGE_BOSS2, Overlays.SHOOTER_STAGE_BOSS3
		]
		if boss_active:
			var b_kind: String = str(boss.get("kind", "arm_tail_boss"))
			if stg == Overlays.SHOOTER_STAGE_BOSS2 or b_kind == "morph_boss":
				_ensure_taco_boss_vram_bank(2)
				_render_taco_tmd_model(target, _taco_models.get("dra", []), 160, 96, 0.065, -10.0, 180.0, 0.0)
			elif stg == Overlays.SHOOTER_STAGE_BOSS3 or b_kind == "collapse_boss":
				_ensure_taco_boss_vram_bank(3)
				_render_taco_tmd_model(target, _taco_models.get("tako", []), 160, 96, 0.055, 25.0, 180.0, 0.0)
			else:
				_ensure_taco_boss_vram_bank(1)
				_render_taco_tmd_model(target, _taco_models.get("bs01", []), 160, 88, 0.055, 10.0, 190.0, 0.0)
		else:
			# 3. Wave Enemies (`ZK00.TMD`, `ZK02.TMD`, `WANG.TMD`) & Explosions (`TACOETC.NBN`)
			var enemies: Array = state.get("enemies", [])
			for ei in range(enemies.size()):
				var enemy: Dictionary = enemies[ei]
				var ex: int = int(enemy.get("x", 0))
				var ey: int = int(enemy.get("y", -0x300))
				var ez: int = maxi(0x600, int(enemy.get("z", 0x1800)))
				var ecx: int = clampi(160 + (ex * 0x160) / ez, 56, 264)
				var ecy: int = clampi(120 + (ey * 0x140) / ez, 52, 112)
				var e_hp: int = int(enemy.get("hp", 6))
				if e_hp > 0:
					var ekind: String = str(enemy.get("kind", "taco_fighter"))
					if ekind == "morph_core" or int(enemy.get("class_id", 4)) == 5:
						_render_taco_tmd_model(target, _taco_models.get("wang", []), ecx, ecy, 0.16, 15.0, 180.0, 0.0)
					elif (ei & 1) == 0:
						_render_taco_tmd_model(target, _taco_models.get("zk00", []), ecx, ecy, 0.16, 15.0, 160.0, 0.0)
					else:
						_render_taco_tmd_model(target, _taco_models.get("zk02", []), ecx, ecy, 0.16, 15.0, 200.0, 0.0)
				else:
					# Authentic explosion sprite from `TACOETC.NBN` (`tpage (0x380, 0x100)`, `clut 0x1E2`)
					var ex512: int = (ecx * 8) / 5 - 256 - 16
					var ey240: int = ecy - 120 - 16
					_blit_taco_vram_4bpp_512(target, ex512, ey240, 32, 32, 0x380, 0x100, 0x00, 0x40, 0, 0x1E2)

		# 3b. Enemy Projectiles (`TACOTEX.NBN` `tpage (0x3C0, 0x100)`)
		for eshot: Dictionary in state.get("enemy_shots", []):
			var es_pos := Overlays.shooter_project_3d(int(eshot.get("x", 0)), int(eshot.get("y", -0x200)), int(eshot.get("z", 0x1000)))
			var esx512: int = (es_pos.x * 8) / 5 - 256 - 8
			var esy240: int = es_pos.y - 120 - 8
			_blit_taco_vram_4bpp_512(target, esx512, esy240, 16, 16, 0x3C0, 0x100, 0x00, 0x00, 0, 0x1F8)

		# 4. Wingman (`JIKI.TMD` obj 1), Player Ship (`JIKI.TMD` obj 0), Barrel Roll & Weapon Fire
		var p_mode: int = int(p.get("mode", 1))
		var ship_cx: int = 160 + px_world / 48
		var ship_cy: int = 184 + (py_world + 0x200) / 96
		var rz_deg: float = clampf(float(px_world) / -64.0, -12.0, 12.0)
		if p_mode == 0x81: # Left Barrel Roll (`player_control.c:6-240`)
			rz_deg = -42.0
			ship_cx -= 10
		elif p_mode == 0x82: # Right Barrel Roll
			rz_deg = 42.0
			ship_cx += 10

		var reticle_pos := Vector2i(
			int(p.get("crosshair_x", 160 + px_world / 32)),
			int(p.get("crosshair_y", 92 + py_world / 48))
		)
		var weapon_kind: int = int(p.get("weapon", Overlays.SHOOTER_PRIMARY_WIDE))
		var active_shots: Array = state.get("shots", [])
		if not active_shots.is_empty():
			_draw_taco_laser_beams(target, Vector2i(ship_cx, ship_cy - 6), reticle_pos, weapon_kind)
		for shot: Dictionary in active_shots:
			var s_pos := Overlays.shooter_project_3d(int(shot.get("x", 0)), int(shot.get("y", -0x200)), int(shot.get("z", 0x900)))
			var sx512: int = (s_pos.x * 8) / 5 - 256 - 8
			var sy240: int = s_pos.y - 120 - 8
			var shot_clut_y: int = 0x1E0 if weapon_kind == Overlays.SHOOTER_PRIMARY_WIDE else (0x1E1 if weapon_kind == Overlays.SHOOTER_PRIMARY_BEAM else 0x1E3)
			var shot_u: int = 0x80 if weapon_kind == Overlays.SHOOTER_PRIMARY_WIDE else (0x00 if weapon_kind == Overlays.SHOOTER_PRIMARY_BEAM else 0x40)
			_blit_taco_vram_4bpp_512(target, sx512, sy240, 16, 16, 0x380, 0x100, shot_u, 0x00, 0, shot_clut_y)

		_render_taco_tmd_model(target, _taco_models.get("jiki_wingman", []), 104 + px_world / 64, 168, 0.16, -22.0, 0.0, -4.0)
		_render_taco_tmd_model(target, _taco_models.get("jiki_player", []), ship_cx, ship_cy, 0.19, -22.0, 0.0, rz_deg)

		# 5. Authentic PS1 VRAM HUD (`shooter_draw_hud` in `hud_secondary_weapons.c:5-270`)
		var total_disp: int = int(state.get("score", 142500)) + int(state.get("stages_cleared", 0)) * 1000000
		var t_left: int = int(state.get("time_left", 5400))
		var mins: int = t_left / 3600
		var secs: int = (t_left % 3600) / 60
		var csecs: int = ((t_left % 60) * 60) / 100
		_draw_taco_hud_text(target, "SCORE", -0xE0, -0x64, 0)
		_draw_taco_hud_number(target, total_disp, -0x50, -0x64, 0)
		_draw_taco_hud_text(target, "TIME", 0x40, -0x64, 0)
		_draw_taco_hud_number(target, mins, 0x90, -0x64, 1)
		_draw_taco_hud_number(target, secs, 0xB0, -0x64, 2)
		_draw_taco_hud_number(target, csecs, 0xD0, -0x64, 3)

		# Reticle (`shooter_draw_reticle` in `hud_secondary_weapons.c:37-104`)
		var ret_x512: int = (reticle_pos.x * 8) / 5 - 256 - 15
		var ret_y240: int = reticle_pos.y - 120 - 15
		_blit_taco_vram_4bpp_512(target, ret_x512, ret_y240, 30, 30, 0x340, 0x100, 0x81, 0x01, 0x10, 0x1EF)
		_blit_taco_vram_4bpp_512(target, ret_x512, ret_y240, 30, 30, 0x340, 0x100, 0x81, 0x21, 0x10, 0x1EF)

		# Shield Gauge (`shooter_draw_shield_gauge` in `hud_secondary_weapons.c:195-270`)
		var shield: int = clampi(int(p.get("shield", 255)), 0, 255)
		var sh_h: int = shield >> 2
		var sh_x0: int = maxi(0, ((256 - 0xBF) * 5) / 8)
		var sh_x1: int = mini(320, ((256 - 0xBF + 14) * 5 + 7) / 8)
		for dy in range(sh_h):
			var py: int = 120 + 0x68 - sh_h + dy
			if py >= 0 and py < 240:
				var frac: float = float(dy) / float(maxi(1, sh_h - 1))
				var bar_col: Color
				if shield > 0xC0:
					bar_col = Color8(int(64.0 * frac), int(128.0 * frac), int(255.0 * (1.0 - frac) + 64.0 * frac), 255)
				elif shield > 0x80:
					bar_col = Color8(int(64.0 * frac), int(240.0 * (1.0 - frac) + 96.0 * frac), int(96.0 * frac), 255)
				elif shield > 0x40:
					bar_col = Color8(248, int(216.0 * (1.0 - frac) + 96.0 * frac), 48, 255)
				else:
					bar_col = Color8(248, 64, 64, 255)
				for pxx in range(sh_x0, sh_x1):
					target.set_pixel(pxx, py, bar_col)
		_blit_taco_vram_4bpp_512(target, -0xC8, 0x28, 0x20, 0x40, 0x340, 0x100, 0x80, 0x40, 0x10, 0x1EF)
		_blit_taco_vram_4bpp_512(target, -0xC8, ((0xFF - shield) >> 2) + 0x18, 0x20, 0x20, 0x340, 0x100, 0xA0, 0x60, 0x20, 0x1EF)
		_blit_taco_vram_4bpp_512(target, -0xCE, 0x68, 0x28, 0x18, 0x340, 0x100, 0xA0, 0x40, 0x20, 0x1EF)

		# Wireframe Status Ships & Bomb Stock (`shooter_draw_bomb_panel` & `shooter_draw_bomb_stock` in `hud_secondary_weapons.c:106-193`)
		_blit_taco_vram_4bpp_512(target, 0xC8, 0x28, 0x20, 0x20, 0x340, 0x100, 0xE0, 0x00, 0, 0x1EF)
		_blit_taco_vram_4bpp_512(target, 0xC8, 0x48, 0x20, 0x20, 0x340, 0x100, 0xE0, 0x20, 0, 0x1EF)
		var bomb_slots: Array = p.get("bomb_slots", [4, 3, 0])
		for i in range(mini(3, bomb_slots.size())):
			var bkind: int = int(bomb_slots[i])
			if bkind >= 3:
				var bclut_y: int = 0x1E9 if bkind == Overlays.SHOOTER_BOMB_BLAST else 0x1EA
				_blit_taco_vram_4bpp_512(target, 0xB4 + i * 0x14, 0x60, 16, 16, 0x380, 0x100, 0xC0, bkind * 16 - 0x30, 0, bclut_y)
	else:
		state["render_provenance"] = "fallback_procedural"
		target.set_meta("render_provenance", "fallback_procedural")
		var horizon_y: int = 144
		for y in range(horizon_y):
			var t: float = float(y) / float(horizon_y)
			var col := Color8(clampi(int(20.0 + pow(t, 1.8) * 170.0), 0, 255), clampi(int(28.0 + pow(t, 1.3) * 185.0), 0, 255), clampi(int(72.0 + t * 175.0), 0, 255), 255)
			for x in range(320):
				target.set_pixel(x, y, col)
		for y in range(horizon_y, 240):
			var dy: int = maxi(1, y - horizon_y)
			var shade: float = clampf(float(dy) / 96.0, 0.35, 1.0)
			var col := Color8(int(120 * shade), int(96 * shade), int(64 * shade), 255)
			for x in range(320):
				target.set_pixel(x, y, col)

	# Post-stage Result Tally / Score Rankings / Game End Banner Overlays (`result_screen.c`, `score_rankings.c`, `game_end_and_screen_wipe.c`)
	if stg == Overlays.SHOOTER_STAGE_RESULT or state.has("result_tally"):
		var tally: Dictionary = state.get("result_tally", Overlays.shooter_compute_result_tally(state))
		var res_box := Rect2i(48, 44, 224, 96)
		_draw_ps1_window(target, res_box, Color8(16, 32, 76), Color8(8, 16, 40))
		_draw_text_hex_checked(target, STR_HEX["taco_result"], Vector2i(60, 50), 0, "taco_result_hud", res_box)
		_draw_ascii_digits(target, "%07d" % int(tally.get("stage_bonus", 0)), Vector2i(164, 68), Color8(180, 240, 255))
		_draw_ascii_digits(target, "%07d" % int(tally.get("shield_bonus", 0)), Vector2i(164, 82), Color8(120, 248, 168))
		_draw_ascii_digits(target, "%07d" % int(tally.get("time_bonus", 0)), Vector2i(164, 96), Color8(248, 220, 120))
		_draw_ascii_digits(target, "%07d" % int(tally.get("bomb_stock_bonus", 0)), Vector2i(164, 110), Color8(255, 160, 120))
		_draw_text_hex_checked(target, STR_HEX["taco_total"], Vector2i(60, 122), 0, "taco_result_hud", res_box)
		_draw_ascii_digits(target, "%07d" % int(tally.get("total_score", 0)), Vector2i(164, 124), Color8(255, 248, 96))
	elif stg == Overlays.SHOOTER_STAGE_SCORES or bool(state.get("show_rankings", false)):
		var r_box := Rect2i(52, 44, 216, 96)
		_draw_ps1_window(target, r_box, Color8(16, 28, 72), Color8(8, 14, 38))
		_draw_text_hex_checked(target, STR_HEX["fw_ranking"], Vector2i(64, 50), 0, "taco_ranking_hud", r_box)
		var t_ranks: Array = state.get("rankings", [])
		for ri in range(mini(5, t_ranks.size())):
			var re: Dictionary = t_ranks[ri]
			var ry: int = 68 + ri * 13
			_draw_ascii_digits(target, "%d" % (ri + 1), Vector2i(68, ry), Color8(255, 216, 80))
			_draw_ascii_digits(target, "%07d" % int(re.get("score", 0)), Vector2i(104, ry), Color8(232, 248, 255))
			_draw_ascii_digits(target, "%d" % int(re.get("stage", 1)), Vector2i(196, ry), Color8(160, 240, 180))
	elif stg == Overlays.SHOOTER_STAGE_GAME_END:
		var end_box := Rect2i(36, 76, 248, 28)
		_draw_ps1_window(target, end_box, Color8(20, 36, 84), Color8(10, 18, 44))
		var reason: int = int(state.get("game_end_reason", 0))
		if reason == 0:
			_draw_text_hex_checked(target, STR_HEX["fw_mission_complete"], Vector2i(56, 84), 0, "taco_game_end_hud", end_box)
		elif reason == 1:
			_draw_text_hex_checked(target, STR_HEX["fw_time_over"], Vector2i(100, 84), 0, "taco_game_end_hud", end_box)
		else:
			_draw_text_hex_checked(target, STR_HEX["fw_game_over"], Vector2i(100, 84), 0, "taco_game_end_hud", end_box)

	return target


# ==============================================================================
# 3. TT: TwinBee Time Attack Playfield & Scoreboard (320x240)
# ==============================================================================
func render_tt_twinbee(state: Dictionary) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	# Outer frame black pillars: x in [0, 32), [288, 320), y in [0, 8) (tt_video_init in engine_initialization.c:129-146)
	target.fill(Color.BLACK)

	var playfield := Rect2i(32, 8, 256, 232)
	var tt_mode: int = int(state.get("state", Overlays.TT_STATE_STAGE))
	var stg_idx: int = clampi(int(state.get("stage_index", 0)), 0, 3)

	# 1. Playfield Background: Authentic PS1 `tt_bg_tilemap_draw` (`settings_and_background.c:494-532`)
	# decoded from `CDROM/TWINSGD/TBTA.DAT` (`tpage = 0xE`) + `TT.EXN` (`tt_stage_tile_map`),
	# falling back to sky/sea gradient only if `TBTA.DAT` is unavailable.
	if _tt_vram_ready:
		state["render_provenance"] = "authentic_tbta_vram"
		target.set_meta("render_provenance", "authentic_tbta_vram")
		_draw_tt_bg_tilemap(target, int(state.get("scroll_progress", 0)))
	else:
		state["render_provenance"] = "diagnostic_preview_no_tt_tilemap_pack"
		target.set_meta("render_provenance", "diagnostic_preview_no_tt_tilemap_pack")
		var scroll: int = 0x300 - int(state.get("scroll_progress", 0))
		var scroll_sub: int = scroll & 0xF
		var sky_top := Color8(0x00, 0x68, 0xF0) # settings_and_background.c:688 (r0=0, g0=0x68, b0=0xF0)
		var sky_mid := Color8(0xB0, 0xF0, 0xD8) # settings_and_background.c:690 (r2=0xB0, g2=0xF0, b2=0xD8)
		var sea_top := Color8(0x28, 0x84, 0xD8)
		var sea_bot := Color8(0x14, 0x4C, 0x9C)
		for y in range(playfield.position.y, playfield.end.y):
			var col: Color
			if y < 168:
				var t_sky: float = clampf(float(y - playfield.position.y) / 160.0, 0.0, 1.0)
				col = sky_top.lerp(sky_mid, t_sky)
			else:
				var shifted_y: int = clampi(y - 168 + scroll_sub, 0, 88)
				var t_sea: float = clampf(float(shifted_y) / 88.0, 0.0, 1.0)
				col = sea_top.lerp(sea_bot, t_sea)
			for x in range(playfield.position.x, playfield.end.x):
				target.set_pixel(x, y, col)

	# Dedicated Ranking View (`ranking_view.c`: `tt_state_ranking_view`, state 7 / `tt_state_name_entry`, state 6)
	# Clears active playfield sprites before drawing the ranking table.
	if tt_mode == Overlays.TT_STATE_RANKING_VIEW or tt_mode == Overlays.TT_STATE_NAME_ENTRY:
		var full_rank_box := Rect2i(48, 36, 224, 168)
		_draw_ps1_window(target, full_rank_box, Color8(16, 28, 72), Color8(8, 14, 38))
		_draw_text_hex_checked(target, STR_HEX["fw_ranking"], Vector2i(114, 46), 0, "tt_ranking_view", full_rank_box)
		var rankings_full: Array = state.get("rankings", [])
		var hi_slot_full: int = int(state.get("ranking_slot", -1))
		for i in range(mini(5, rankings_full.size())):
			var r_entry: Dictionary = rankings_full[i]
			var r_time: Dictionary = Overlays.tt_format_time(int(r_entry["time"]))
			var ry: int = 74 + i * 22
			var row_col: Color = Color8(120, 255, 160) if i == hi_slot_full else Color8(224, 248, 255)
			_draw_ascii_digits(target, "%d." % (i + 1), Vector2i(68, ry), Color8(255, 200, 80))
			_draw_ascii_digits(target, str(r_time["text"]), Vector2i(104, ry), row_col)
		return target

	# Fluffy white scrolling clouds (`tt_clouds_draw` in `falling_sprite_pool.c:133-191`)
	_draw_tt_cloud(target, Vector2i(96, 92))
	_draw_tt_cloud(target, Vector2i(224, 148))

	# 1b. Ground Targets (`tt_ground_targets_draw` in `ground_target_pool.c:287-323`)
	for gtarget: Dictionary in state.get("ground_targets", []):
		if int(gtarget.get("active", 0)) == 0:
			continue
		var gpos := Vector2i(160 + int(gtarget.get("x", 0)), int(gtarget.get("y", 140)))
		var gstate: int = int(gtarget.get("state", 0x40))
		if _tt_vram_ready:
			if gstate == 0x71:
				_draw_tt_part_sprite(target, gpos, 2, 0xE)
			else:
				_blit_tt_vram_sprite(target, gpos.x - 8, gpos.y - 8, 16, 16, 0x80, 0x00, 0xA, 0x3CC7, 0)
		else:
			_fill_rect(target, Rect2i(gpos.x - 7, gpos.y - 7, 14, 14), Color8(148, 108, 72))
			_draw_circle_filled(target, gpos, 4, Color8(248, 192, 64))

	# 2. Stage Boss (0: Rope-Arm, 1: Swing-Ring, 2: Multipart, 3: Beam/Ring)
	var boss: Dictionary = state.get("boss", {})
	if int(boss.get("active", 1)) != 0:
		var bx: int = 160 + int(boss.get("x", 0))
		var by: int = int(boss.get("y", 54))
		var bid: int = int(boss.get("boss_id", stg_idx))
		_draw_tt_boss(target, Vector2i(bx, by), bid)

	# 3. Cycling Bell Powerups (`tt_bells_draw` in `cycling_pickups.c:224-275`)
	for bell: Dictionary in state.get("bells", []):
		if int(bell.get("active", 0)) == 0:
			continue
		var bpos := Vector2i(160 + int(bell["x"]), int(bell["y"]))
		_draw_tt_bell(target, bpos, int(bell.get("kind", 0)), int(bell.get("state", 0x40)))

	# 4. Air Enemies & Enemy Shots (`tt_air_enemies_draw`, `tt_enemy_shots_draw`)
	for enemy: Dictionary in state.get("air_enemies", []):
		if int(enemy.get("active", 0)) == 0:
			continue
		_draw_tt_air_enemy(target, Vector2i(160 + int(enemy["x"]), int(enemy["y"])), int(enemy.get("state", 0x40)))
	for eshot: Dictionary in state.get("enemy_shots", []):
		if int(eshot.get("active", 0)) == 0:
			continue
		var espos := Vector2i(160 + int(eshot["x"]), int(eshot["y"]))
		if _tt_vram_ready:
			_blit_tt_vram_sprite(target, espos.x - 8, espos.y - 8, 16, 16, 0x80, 0xE0, 0xA, 0x3C8B, 0)
		else:
			_draw_circle_filled(target, espos, 3, Color8(255, 96, 64))

	# 4b. Active Player Shots & Ground Bombs (`tt_shots_draw` in `title_and_projectile_pools.c:1028-1060`, `tt_bombs_draw` in `bomb_pool.c:172-203`)
	for pshot: Dictionary in state.get("player_shots", []):
		if int(pshot.get("active", 0)) == 0:
			continue
		var ps_pos := Vector2i(160 + int(pshot.get("x", 0)), int(pshot.get("y", 160)))
		var ps_kind: int = int(pshot.get("kind", 9))
		if _tt_vram_ready:
			var su: int = 0x90 if ps_kind == 9 else 0x80
			var sv: int = 0xD0 if ps_kind == 10 else 0xC0
			_blit_tt_vram_sprite(target, ps_pos.x - 8, ps_pos.y - 8, 16, 16, su, sv, 0xA, 0x3C87, 0)
		else:
			_fill_rect(target, Rect2i(ps_pos.x - 2, ps_pos.y - 4, 4, 8), Color8(255, 248, 120))
	for pbomb: Dictionary in state.get("player_bombs", []):
		if int(pbomb.get("active", 0)) == 0:
			continue
		var pb_pos := Vector2i(160 + int(pbomb.get("x", 0)), int(pbomb.get("y", 160)))
		if _tt_vram_ready:
			_blit_tt_vram_sprite(target, pb_pos.x - 8, pb_pos.y - 8, 16, 16, 0xA0, 0xD0, 0xA, 0x3C81, 0)
		else:
			_draw_circle_filled(target, pb_pos, 4, Color8(255, 144, 64))

	# 5. TwinBee Player Ship, Arms, 3 Option Followers & Twin Shots (`tt_player_draw` in `player_and_asset_uploads.c:771-808`)
	var p: Dictionary = state.get("player", {})
	var ppos := Vector2i(160 + int(p.get("x", 0)), int(p.get("y", 192)))
	if int(p.get("has_options", 1)) != 0:
		_draw_tt_option(target, ppos + Vector2i(-24, 12))
		_draw_tt_option(target, ppos + Vector2i(24, 12))
		_draw_tt_option(target, ppos + Vector2i(0, 22))
	var shot_kind: int = int(p.get("shot_kind", 9))
	if shot_kind == 8:
		if _tt_vram_ready:
			_blit_tt_vram_sprite(target, ppos.x - 8, ppos.y - 36, 16, 16, 0x80, 0xC0, 0xA, 0x3C87, 0)
			_blit_tt_vram_sprite(target, ppos.x - 8, ppos.y - 64, 16, 16, 0x80, 0xC0, 0xA, 0x3C87, 0)
		else:
			_fill_rect(target, Rect2i(ppos.x - 2, ppos.y - 32, 4, 8), Color8(255, 240, 120))
			_fill_rect(target, Rect2i(ppos.x - 2, ppos.y - 60, 4, 8), Color8(255, 240, 120))
	else:
		_draw_tt_twin_shots(target, ppos + Vector2i(-10, -28))
		_draw_tt_twin_shots(target, ppos + Vector2i(10, -56))
	_draw_tt_player(target, ppos)

	# 5b. Stage Clear / Game Over Banners (`level_clear_and_collisions.c`, `game_over_flow.c`)
	if tt_mode == Overlays.TT_STATE_STAGE_CLEAR:
		var clr_box := Rect2i(64, 96, 192, 28)
		_draw_ps1_window(target, clr_box, Color8(20, 56, 104), Color8(10, 28, 52))
		_draw_text_hex_checked(target, STR_HEX["fw_stage_clear"], Vector2i(88, 104), 0, "tt_stage_clear_banner", clr_box)
	elif tt_mode == Overlays.TT_STATE_GAME_OVER:
		var go_box := Rect2i(76, 96, 168, 28)
		_draw_ps1_window(target, go_box, Color8(88, 20, 36), Color8(44, 10, 18))
		_draw_text_hex_checked(target, STR_HEX["fw_game_over"], Vector2i(100, 104), 0, "tt_game_over_banner", go_box)

	# 6. Top-left Time HUD & Life Icons (`tt_state_stage` in `play_flow_and_scroll_scripts.c:226-274`)
	var elapsed_frames: int = int(state.get("elapsed_frames", 4250))
	if _tt_vram_ready:
		# Exact 8x8 sprite-font HUD (`text_and_time_display.c:17-108`, `play_flow_and_scroll_scripts.c:226-242`):
		# Text 0: "TIME" (`tt_text_strings[0xD]`) at x = -0x70 (screen 48), y = 0x10 (16), palette = 1
		# Text 1: "M'SS\"CC" (`tt_text_strings[0x27]`) at x = -0x48 (screen 88), y = 0x10 (16), palette = 1
		var time_codes: Array[int] = [0x1D, 0x12, 0x16, 0x0E]
		_draw_tt_vram_glyph_codes(target, 160 - 0x70, 0x10, time_codes, 1)
		var t_clamped: int = mini(elapsed_frames, 0x8CA0)
		var d_min: int = 9 if t_clamped == 0x8CA0 else (t_clamped / 3600) % 10
		var d_s10: int = 5 if t_clamped == 0x8CA0 else (t_clamped % 3600) / 600
		var d_s01: int = 9 if t_clamped == 0x8CA0 else (t_clamped % 600) / 60
		var d_c10: int = 9 if t_clamped == 0x8CA0 else ((t_clamped % 60) * 10) / 60
		var d_c01: int = 9 if t_clamped == 0x8CA0 else ((t_clamped * 10) / 6) % 10
		var timer_codes: Array[int] = [d_min, 0x2B, d_s10, d_s01, 0x2C, d_c10, d_c01]
		_draw_tt_vram_glyph_codes(target, 160 - 0x48, 0x10, timer_codes, 1)
	else:
		var time_info: Dictionary = Overlays.tt_format_time(elapsed_frames)
		var hud_box := Rect2i(36, 10, 136, 20)
		_draw_ps1_window(target, hud_box, Color8(12, 24, 64), Color8(6, 12, 36))
		_draw_text_hex_checked(target, STR_HEX["fw_time"], Vector2i(40, 14), 0, "tt_hud", hud_box)
		_draw_ascii_digits(target, str(time_info["text"]), Vector2i(104, 15), Color8(255, 240, 80))

	# Life icons at x = -0x70, -0x60 => screen (48, 24), (64, 24) (`play_flow_and_scroll_scripts.c:244-259`)
	var lives: int = clampi(int(state.get("lives", 2)), 0, 2)
	for i in range(lives):
		_draw_tt_life_icon(target, Vector2i(48 + i * 16, 24))

	# 6-segment Speed Gauge at bottom-left (`play_flow_and_scroll_scripts.c:14-38, 260-274`:
	# x = -0x78 => screen 40, y = 0xD8 - i*8, w = 16, h = 8, u = (i/2)*16 + 0x50, v = (i&1)*8 + 0x30)
	var base_speed_step: int = int(state.get("scroll_speed", 0x18000)) / 0x8000
	var p_spd_lvl: int = int(p.get("speed_level", 4))
	var speed_step: int = clampi(base_speed_step + (p_spd_lvl - 4), 1, 6)
	for i in range(6):
		var gy: int = 0xD8 - i * 8
		if _tt_vram_ready:
			var gu: int = (i / 2) * 0x10 + 0x50
			var gv: int = (i & 1) * 8 + 0x30
			var gclut: int = 0x3C09 if (i + 1) <= speed_step else 0x3C0A
			_blit_tt_vram_sprite(target, 160 - 0x78, gy, 16, 8, gu, gv, 0xA, gclut, 0)
		else:
			var seg_col: Color = Color8(255, 220, 48) if (i + 1) <= speed_step else Color8(48, 64, 96)
			_fill_rect(target, Rect2i(40, gy, 14, 6), seg_col)

	# 7. Diagnostic-only compact ranking card when `show_rankings == true` is explicitly requested
	# (In original `src/ovl/TT/ranking_view.c`, rankings are drawn only in `TT_STATE_RANKING_VIEW` / `TT_STATE_NAME_ENTRY`.)
	if bool(state.get("show_rankings", false)):
		var hi_slot: int = int(state.get("ranking_slot", -1))
		var rank_box := Rect2i(192, 184, 92, 48)
		_draw_ps1_window(target, rank_box, Color8(16, 28, 72), Color8(8, 14, 38))
		var rankings: Array = state.get("rankings", [])
		for i in range(mini(3, rankings.size())):
			var r_entry: Dictionary = rankings[i]
			var r_time: Dictionary = Overlays.tt_format_time(int(r_entry["time"]))
			var ry: int = 188 + i * 14
			var row_col: Color = Color8(120, 255, 160) if i == hi_slot else Color8(224, 248, 255)
			_draw_ascii_digits(target, "%d." % (i + 1), Vector2i(196, ry), Color8(255, 200, 80))
			_draw_ascii_digits(target, str(r_time["text"]), Vector2i(212, ry), row_col)

	return target


# ==============================================================================
# 4. Scene-Node Builders for Interactive Playfields & HUD (`Control` Hierarchy)
# ==============================================================================

func present_subgame(parent: Node, subgame_id: Variant, state: Dictionary) -> Control:
	var root: Control = build_subgame_nodes(subgame_id, state)
	if parent != null:
		var existing: Node = parent.get_node_or_null(NodePath(root.name))
		if existing != null:
			parent.remove_child(existing)
			existing.free()
		parent.add_child(root)
	return root

func build_subgame_nodes(subgame_id: Variant, state: Dictionary) -> Control:
	var kind: String = str(state.get("subgame_id", subgame_id)).to_upper()
	if subgame_id is int:
		if int(subgame_id) == Overlays.OVERLAY_ID_TACO:
			kind = "TACO"
		elif int(subgame_id) == Overlays.OVERLAY_ID_TT:
			kind = "TT"
		elif int(subgame_id) == Overlays.OVERLAY_ID_RPG_BAT:
			kind = "RPG_BAT"
	match kind:
		"TACO", "SHOOTER", "3":
			return build_taco_nodes(state)
		"TT", "TWINBEE", "6":
			return build_tt_nodes(state)
		_:
			return build_rpg_battle_nodes(state)

func build_taco_nodes(state: Dictionary) -> Control:
	var root := Control.new()
	root.name = "TacoSubgameView"
	root.size = Vector2(320, 240)
	root.set_meta("subgame_id", "TACO")
	root.set_meta("stage", int(state.get("stage", Overlays.SHOOTER_STAGE_STAGE1)))
	root.set_meta("wave_index", int(state.get("wave_index", 0)))

	var backdrop := _make_backdrop_node(state, "TACO")
	root.add_child(backdrop)

	var playfield := Control.new()
	playfield.name = "Playfield"
	playfield.size = Vector2(320, 240)
	root.add_child(playfield)

	# Boss node (`arm_tail_boss.c`, `morph_boss_and_enemy_ai.c`, `boss_collapse_and_title.c`)
	var boss_dict: Dictionary = state.get("boss", {})
	var boss_node := Control.new()
	boss_node.name = "Boss"
	boss_node.visible = int(boss_dict.get("active", 0)) != 0
	var b_pos := Overlays.shooter_project_3d(int(boss_dict.get("x", 0)), int(boss_dict.get("y", -0x240)), int(boss_dict.get("z", 0x1A00)))
	boss_node.position = Vector2(b_pos.x - 24, b_pos.y - 18)
	boss_node.size = Vector2(48, 36)
	boss_node.set_meta("kind", str(boss_dict.get("kind", "none")))
	boss_node.set_meta("hp", int(boss_dict.get("hp", 0)))
	boss_node.set_meta("max_hp", int(boss_dict.get("max_hp", 0)))
	playfield.add_child(boss_node)

	# Enemies container
	var enemies_node := Control.new()
	enemies_node.name = "Enemies"
	playfield.add_child(enemies_node)
	var enemy_list: Array = state.get("enemies", [])
	for idx in range(enemy_list.size()):
		var enemy: Dictionary = enemy_list[idx]
		var e_pos := Overlays.shooter_project_3d(int(enemy.get("x", 0)), int(enemy.get("y", -0x300)), int(enemy.get("z", 0x1800)))
		var en := ColorRect.new()
		en.name = "Enemy_%d" % idx
		en.position = Vector2(e_pos.x - 8, e_pos.y - 8)
		en.size = Vector2(16, 16)
		en.color = Color8(232, 96, 56) if int(enemy.get("hp", 0)) > 0 else Color8(255, 220, 96)
		en.set_meta("hp", int(enemy.get("hp", 0)))
		en.set_meta("kind", str(enemy.get("kind", "taco_fighter")))
		enemies_node.add_child(en)

	# Projectiles container (player shots + enemy shots)
	var projectiles_node := Control.new()
	projectiles_node.name = "Projectiles"
	playfield.add_child(projectiles_node)
	var shots: Array = state.get("shots", [])
	for idx in range(shots.size()):
		var shot: Dictionary = shots[idx]
		var s_pos := Overlays.shooter_project_3d(int(shot.get("x", 0)), int(shot.get("y", -0x200)), int(shot.get("z", 0x900)))
		var sn := ColorRect.new()
		sn.name = "PlayerShot_%d" % idx
		sn.position = Vector2(s_pos.x - 2, s_pos.y - 2)
		sn.size = Vector2(4, 4)
		sn.color = Color8(112, 248, 255)
		projectiles_node.add_child(sn)
	var enemy_shots: Array = state.get("enemy_shots", [])
	for idx in range(enemy_shots.size()):
		var eshot: Dictionary = enemy_shots[idx]
		var es_pos := Overlays.shooter_project_3d(int(eshot.get("x", 0)), int(eshot.get("y", -0x200)), int(eshot.get("z", 0x1000)))
		var esn := ColorRect.new()
		esn.name = "EnemyShot_%d" % idx
		esn.position = Vector2(es_pos.x - 3, es_pos.y - 3)
		esn.size = Vector2(6, 6)
		esn.color = Color8(255, 88, 64)
		projectiles_node.add_child(esn)

	# Player & Crosshair nodes
	var p: Dictionary = state.get("player", {})
	var player_node := ColorRect.new()
	player_node.name = "Player"
	player_node.position = Vector2(160 + int(p.get("x", 0)) / 64 - 16, 168)
	player_node.size = Vector2(32, 16)
	player_node.color = Color8(192, 212, 248)
	player_node.set_meta("shield", int(p.get("shield", 255)))
	player_node.set_meta("ammo", int(p.get("ammo", Overlays.SHOOTER_MAX_AMMO)))
	playfield.add_child(player_node)

	var crosshair_node := ColorRect.new()
	crosshair_node.name = "Crosshair"
	var cx: int = int(p.get("crosshair_x", 160 + int(p.get("x", 0)) / 32))
	var cy: int = int(p.get("crosshair_y", 92 + int(p.get("y", -0x200)) / 48))
	crosshair_node.position = Vector2(cx - 8, cy - 8)
	crosshair_node.size = Vector2(16, 16)
	crosshair_node.color = Color8(96, 255, 128, 180)
	playfield.add_child(crosshair_node)

	# HUD hierarchy
	var hud := Control.new()
	hud.name = "HUD"
	hud.size = Vector2(320, 240)
	root.add_child(hud)

	var score_lbl := Label.new()
	score_lbl.name = "ScoreLabel"
	score_lbl.position = Vector2(14, 8)
	var total_disp: int = int(state.get("score", 0)) + int(state.get("stages_cleared", 0)) * 1000000
	score_lbl.text = "SCORE %07d" % total_disp
	hud.add_child(score_lbl)

	var stage_lbl := Label.new()
	stage_lbl.name = "StageWaveLabel"
	stage_lbl.position = Vector2(124, 8)
	stage_lbl.text = "STG %d WAVE %d" % [int(state.get("stage", 11)), int(state.get("wave_index", 0)) + 1]
	hud.add_child(stage_lbl)

	var time_lbl := Label.new()
	time_lbl.name = "TimeLabel"
	time_lbl.position = Vector2(216, 8)
	time_lbl.text = "TIME %d" % int(state.get("time_left", 0))
	hud.add_child(time_lbl)

	var shield_bar := ColorRect.new()
	shield_bar.name = "ShieldBar"
	var shield_val: int = clampi(int(p.get("shield", 255)), 0, 255)
	shield_bar.position = Vector2(18, 224 - (shield_val >> 2))
	shield_bar.size = Vector2(14, maxi(1, shield_val >> 2))
	shield_bar.color = Color8(64, 160, 255)
	shield_bar.set_meta("value", shield_val)
	hud.add_child(shield_bar)

	var ammo_bar := ColorRect.new()
	ammo_bar.name = "AmmoBar"
	var ammo_val: int = clampi(int(p.get("ammo", Overlays.SHOOTER_MAX_AMMO)), 0, Overlays.SHOOTER_MAX_AMMO)
	ammo_bar.position = Vector2(40, 216)
	ammo_bar.size = Vector2(ammo_val * 2, 8)
	ammo_bar.color = Color8(255, 224, 72)
	ammo_bar.set_meta("ammo", ammo_val)
	ammo_bar.set_meta("reload_frames", int(p.get("reload_frames", 0)))
	hud.add_child(ammo_bar)

	var bomb_stock := Control.new()
	bomb_stock.name = "BombStock"
	bomb_stock.position = Vector2(236, 198)
	bomb_stock.size = Vector2(74, 32)
	var b_slots: Array = p.get("bomb_slots", [4, 3, 0])
	for i in range(b_slots.size()):
		if int(b_slots[i]) != 0:
			var bi := ColorRect.new()
			bi.name = "Bomb_%d" % i
			bi.position = Vector2(6 + i * 22, 10)
			bi.size = Vector2(12, 12)
			bi.color = Color8(248, 96, 64)
			bomb_stock.add_child(bi)
	hud.add_child(bomb_stock)

	return root

func build_tt_nodes(state: Dictionary) -> Control:
	var root := Control.new()
	root.name = "TwinBeeSubgameView"
	root.size = Vector2(320, 240)
	root.set_meta("subgame_id", "TT")
	root.set_meta("state", int(state.get("state", Overlays.TT_STATE_STAGE)))
	root.set_meta("stage_index", int(state.get("stage_index", 0)))
	root.set_meta("tt_stages_cleared", int(state.get("tt_stages_cleared", 0)))

	var backdrop := _make_backdrop_node(state, "TT")
	root.add_child(backdrop)

	var playfield := Control.new()
	playfield.name = "Playfield"
	playfield.position = Vector2(32, 8)
	playfield.size = Vector2(256, 232)
	root.add_child(playfield)

	# Boss node (`rope_arm_boss.c`, `swing_ring_boss.c`, `multipart_form_boss.c`, `beam_and_ring_boss.c`)
	var boss_dict: Dictionary = state.get("boss", {})
	var boss_node := Control.new()
	boss_node.name = "Boss"
	boss_node.visible = int(boss_dict.get("active", 0)) != 0
	boss_node.position = Vector2(128 + int(boss_dict.get("x", 0)) - 28, int(boss_dict.get("y", 54)) - 16)
	boss_node.size = Vector2(56, 32)
	boss_node.set_meta("boss_id", int(boss_dict.get("boss_id", 0)))
	boss_node.set_meta("hp", int(boss_dict.get("hp", 0)))
	playfield.add_child(boss_node)

	# Bells container (`cycling_pickups.c`)
	var bells_node := Control.new()
	bells_node.name = "Bells"
	playfield.add_child(bells_node)
	var bells_list: Array = state.get("bells", [])
	for idx in range(bells_list.size()):
		var bell: Dictionary = bells_list[idx]
		if int(bell.get("active", 0)) == 0:
			continue
		var bn := ColorRect.new()
		bn.name = "Bell_%d" % idx
		bn.position = Vector2(128 + int(bell.get("x", 0)) - 6, int(bell.get("y", 0)) - 6)
		bn.size = Vector2(12, 12)
		bn.color = Color8(255, 224, 48)
		bn.set_meta("kind", int(bell.get("kind", 0)))
		bn.set_meta("hits", int(bell.get("hits", 0)))
		bells_node.add_child(bn)

	# Enemies container (`air_enemies.c` + `ground_enemies.c`)
	var enemies_node := Control.new()
	enemies_node.name = "Enemies"
	playfield.add_child(enemies_node)
	var air_list: Array = state.get("air_enemies", [])
	for idx in range(air_list.size()):
		var enemy: Dictionary = air_list[idx]
		if int(enemy.get("active", 0)) == 0:
			continue
		var en := ColorRect.new()
		en.name = "AirEnemy_%d" % idx
		en.position = Vector2(128 + int(enemy.get("x", 0)) - 7, int(enemy.get("y", 0)) - 7)
		en.size = Vector2(14, 14)
		en.color = Color8(240, 88, 72)
		enemies_node.add_child(en)

	# Projectiles container (`player_shots`, `player_bombs`, `enemy_shots`)
	var projectiles_node := Control.new()
	projectiles_node.name = "Projectiles"
	playfield.add_child(projectiles_node)
	var pshots: Array = state.get("player_shots", [])
	for idx in range(pshots.size()):
		var pshot: Dictionary = pshots[idx]
		if int(pshot.get("active", 0)) == 0:
			continue
		var psn := ColorRect.new()
		psn.name = "PlayerShot_%d" % idx
		psn.position = Vector2(128 + int(pshot.get("x", 0)) - 2, int(pshot.get("y", 0)) - 4)
		psn.size = Vector2(4, 8)
		psn.color = Color8(255, 248, 120)
		projectiles_node.add_child(psn)
	var eshots: Array = state.get("enemy_shots", [])
	for idx in range(eshots.size()):
		var eshot: Dictionary = eshots[idx]
		if int(eshot.get("active", 0)) == 0:
			continue
		var esn := ColorRect.new()
		esn.name = "EnemyShot_%d" % idx
		esn.position = Vector2(128 + int(eshot.get("x", 0)) - 3, int(eshot.get("y", 0)) - 3)
		esn.size = Vector2(6, 6)
		esn.color = Color8(255, 96, 64)
		projectiles_node.add_child(esn)

	# Player TwinBee node
	var p: Dictionary = state.get("player", {})
	var player_node := ColorRect.new()
	player_node.name = "Player"
	player_node.position = Vector2(128 + int(p.get("x", 0)) - 9, int(p.get("y", 192)) - 9)
	player_node.size = Vector2(18, 18)
	player_node.color = Color8(64, 152, 248)
	player_node.set_meta("shot_kind", int(p.get("shot_kind", 9)))
	player_node.set_meta("speed_level", int(p.get("speed_level", 4)))
	player_node.set_meta("has_options", int(p.get("has_options", 1)))
	playfield.add_child(player_node)

	# HUD hierarchy
	var hud := Control.new()
	hud.name = "HUD"
	hud.size = Vector2(320, 240)
	root.add_child(hud)

	var time_info: Dictionary = Overlays.tt_format_time(int(state.get("elapsed_frames", 0)))
	var time_lbl := Label.new()
	time_lbl.name = "TimeLabel"
	time_lbl.position = Vector2(40, 12)
	time_lbl.text = "TIME %s" % str(time_info["text"])
	hud.add_child(time_lbl)

	var score_lbl := Label.new()
	score_lbl.name = "ScoreLabel"
	score_lbl.position = Vector2(180, 12)
	score_lbl.text = "SCORE %d" % int(state.get("score", 0))
	hud.add_child(score_lbl)

	var stage_lbl := Label.new()
	stage_lbl.name = "StageWaveLabel"
	stage_lbl.position = Vector2(180, 28)
	stage_lbl.text = "STAGE %d WAVE %d" % [int(state.get("stage_index", 0)) + 1, int(state.get("wave_index", 0)) + 1]
	hud.add_child(stage_lbl)

	var lives_container := Control.new()
	lives_container.name = "LivesContainer"
	lives_container.position = Vector2(40, 32)
	var lives_count: int = clampi(int(state.get("lives", 2)), 0, 2)
	lives_container.set_meta("lives", lives_count)
	for i in range(lives_count):
		var li := ColorRect.new()
		li.name = "Life_%d" % i
		li.position = Vector2(i * 16, 0)
		li.size = Vector2(10, 10)
		li.color = Color8(80, 176, 255)
		lives_container.add_child(li)
	hud.add_child(lives_container)

	var speed_gauge := Control.new()
	speed_gauge.name = "SpeedGauge"
	speed_gauge.position = Vector2(40, 176)
	speed_gauge.size = Vector2(14, 48)
	speed_gauge.set_meta("speed_level", int(p.get("speed_level", 4)))
	hud.add_child(speed_gauge)

	return root

func build_rpg_battle_nodes(state: Dictionary) -> Control:
	var root := Control.new()
	root.name = "RpgBattleSubgameView"
	root.size = Vector2(320, 240)
	root.set_meta("subgame_id", "RPG_BAT")
	root.set_meta("stage", int(state.get("stage", Overlays.RPG_STAGE_BATTLE_ACTIVE)))
	root.set_meta("turn_count", int(state.get("turn_count", 0)))

	var backdrop := _make_backdrop_node(state, "RPG_BAT")
	root.add_child(backdrop)

	var playfield := Control.new()
	playfield.name = "Playfield"
	playfield.size = Vector2(320, 168)
	root.add_child(playfield)

	# Enemies container
	var enemies_node := Control.new()
	enemies_node.name = "Enemies"
	playfield.add_child(enemies_node)
	var gauges: Array = state.get("enemy_slot_gauges", [5000, 5000, 5000])
	var enemy_positions := [Vector2(40, 108), Vector2(76, 98), Vector2(108, 114)]
	for i in range(mini(3, gauges.size())):
		if int(gauges[i]) <= 0:
			continue
		var en := _make_sprite_or_rect("Enemy_%d" % i, _rpg_enemy_sds[i] if i < _rpg_enemy_sds.size() else null, enemy_positions[i] - Vector2(12, 24), Vector2(24, 48), Color8(64, 48, 96))
		en.set_meta("gauge", int(gauges[i]))
		enemies_node.add_child(en)

	# Party container (Hero + Partner Girl + Guest)
	var party_node := Control.new()
	party_node.name = "Party"
	playfield.add_child(party_node)
	var hero_node := _make_sprite_or_rect("Player", _rpg_hero_sd, Vector2(214, 80), Vector2(24, 48), Color8(56, 112, 208))
	hero_node.set_meta("hp", int(state.get("player_hp", 0)))
	party_node.add_child(hero_node)

	var gid: int = int(state.get("girl_id", 0))
	if gid != 14:
		var partner_node := _make_sprite_or_rect("Partner", _rpg_shiori_sd, Vector2(256, 76), Vector2(24, 48), GIRL_PALETTE.get(gid, Color8(216, 72, 92)))
		partner_node.set_meta("girl_id", gid)
		partner_node.set_meta("hp", int(state.get("girl_hp", 0)))
		party_node.add_child(partner_node)

	# Command Menu (`actor 133` frame 2 + `actor 130` sequences 5/6/7) & Message Box (`actor 132` frame 3)
	var cmd_cursor: int = clampi(int(state.get("command_cursor", 0)), 0, 2)
	var cmd_menu := Control.new()
	cmd_menu.name = "CommandMenu"
	cmd_menu.position = Vector2(108, 168)
	cmd_menu.size = Vector2(56, 64)
	cmd_menu.set_meta("cursor", cmd_cursor)
	cmd_menu.set_meta("actor_id", 130)
	cmd_menu.set_meta("sequence", 5 + cmd_cursor)
	cmd_menu.set_meta("hit_rect", Rect2i(-43, 57 + 15 * cmd_cursor, 35, 15))
	cmd_menu.set_meta("screen_rect", Rect2i(117, 177 + 15 * cmd_cursor, 35, 15))
	playfield.add_child(cmd_menu)

	var msg_box := Control.new()
	msg_box.name = "MessageBox"
	msg_box.position = Vector2(16, 8)
	msg_box.size = Vector2(288, 32)
	var msg_lbl := Label.new()
	msg_lbl.name = "MessageLabel"
	msg_lbl.text = str(state.get("last_command_name", "battle"))
	msg_box.add_child(msg_lbl)
	playfield.add_child(msg_box)

	# Bottom HUD gauges
	var hud := Control.new()
	hud.name = "HUD"
	hud.position = Vector2(0, 168)
	hud.size = Vector2(320, 72)
	root.add_child(hud)

	var php_bar := ColorRect.new()
	php_bar.name = "PlayerHpBar"
	php_bar.position = Vector2(177, 11)
	php_bar.size = Vector2(80, 8)
	php_bar.color = Color8(96, 232, 128)
	php_bar.set_meta("hp", int(state.get("player_hp", 0)))
	php_bar.set_meta("max_hp", int(state.get("player_max_hp", 1)))
	hud.add_child(php_bar)

	var ghp_bar := ColorRect.new()
	ghp_bar.name = "GirlHpBar"
	ghp_bar.position = Vector2(177, 27)
	ghp_bar.size = Vector2(80, 8)
	ghp_bar.color = Color8(240, 140, 184)
	ghp_bar.set_meta("hp", int(state.get("girl_hp", 0)))
	ghp_bar.set_meta("max_hp", int(state.get("girl_max_hp", 1)))
	hud.add_child(ghp_bar)

	var mp_bar := ColorRect.new()
	mp_bar.name = "MpGauge"
	mp_bar.position = Vector2(220, 36)
	mp_bar.size = Vector2(82, 6)
	mp_bar.color = Color8(96, 184, 255)
	mp_bar.set_meta("mp", int(state.get("player_mp", 0)))
	mp_bar.set_meta("max_mp", int(state.get("player_max_mp", 100)))
	hud.add_child(mp_bar)

	var sp_bar := ColorRect.new()
	sp_bar.name = "SpecialGauge"
	sp_bar.position = Vector2(220, 45)
	sp_bar.size = Vector2(82, 8)
	sp_bar.color = Color8(64, 224, 128)
	sp_bar.set_meta("special_gauge", int(state.get("special_gauge", 0)))
	hud.add_child(sp_bar)

	return root

func _make_backdrop_node(state: Dictionary, kind: String) -> Control:
	var bg_img: Image = null
	if kind == "TACO":
		bg_img = _taco_bg_1 if _taco_bg_1 != null else _taco_bg
	elif kind == "RPG_BAT":
		bg_img = _rpg_arena_bg
	if _font_initialized:
		if kind == "TACO":
			bg_img = render_taco_shooter(state)
		elif kind == "TT":
			bg_img = render_tt_twinbee(state)
		else:
			bg_img = render_rpg_battle(state)
	if bg_img != null:
		var tex_rect := TextureRect.new()
		tex_rect.name = "FrameBackdrop"
		tex_rect.size = Vector2(320, 240)
		tex_rect.texture = ImageTexture.create_from_image(bg_img)
		return tex_rect
	var fallback := ColorRect.new()
	fallback.name = "FrameBackdrop"
	fallback.size = Vector2(320, 240)
	fallback.color = Color8(24, 36, 72)
	return fallback

func _make_sprite_or_rect(node_name: String, img: Image, pos: Vector2, sz: Vector2, fallback_col: Color) -> Control:
	if img != null:
		var tr := TextureRect.new()
		tr.name = node_name
		tr.position = pos
		tr.size = sz
		tr.texture = ImageTexture.create_from_image(img)
		return tr
	var cr := ColorRect.new()
	cr.name = node_name
	cr.position = pos
	cr.size = sz
	cr.color = fallback_col
	return cr


# ==============================================================================
# Drawing Helpers & Glyph Bounding-Box Verification
# ==============================================================================

## Phase 4 WP-I: bind English text rows (presenter_text_binding.gd). Subgame
## sprites (BATTLE VS_*, TAIIKU, the 16x8 miss variants) are drawn from VRAM
## or loose PNGs with no container/frame/png identity, so an image store is
## not applied here and the rows stay original; the result says so.
func bind_english(layer: RefCounted, bindings: Array, image_store: RefCounted = null) -> Dictionary:
	var text: int = english_text.bind(layer, _font, bindings, 13)
	var fallbacks: Array[Dictionary] = english_text.fallback_log()
	if image_store != null:
		fallbacks.append({"key": "images", "reason": "subgame sprites have no identity-keyed draw site"})
	return {"text": text, "images": 0, "fallbacks": fallbacks}

func clear_english() -> void:
	english_text.clear()

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i,
		palette_row: int, region: String, bounds: Rect2i) -> void:
	var bound: Dictionary = english_text.translate(hex_code)
	if bound.is_empty():
		_draw_text_hex_native(target, hex_code, pos, palette_row, region, bounds)
		return
	# One native glyph draw per translated unit at its trimmed offset.
	for index: int in bound.glyph_x.size():
		_draw_text_hex_native(target, bound.hex.substr(index * 4, 4), pos + Vector2i(bound.glyph_x[index], 0), palette_row, region, bounds)

func _draw_text_hex_native(target: Image, hex_code: String, pos: Vector2i,
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

func _draw_rpg_delinquent_sprite(target: Image, pos: Vector2i, variant: int) -> void:
	if variant >= 0 and variant < _rpg_enemy_sds.size() and _rpg_enemy_sds[variant] != null:
		_blit_alpha_centered(target, _rpg_enemy_sds[variant], pos, true)
		return
	var coat_col: Color = Color8(64, 48, 96) if variant == 0 else (Color8(48, 64, 88) if variant == 1 else Color8(88, 48, 64))
	# Pompadour hair + head + gakuran jacket + legs
	_fill_rect(target, Rect2i(pos.x - 8, pos.y - 28, 14, 6), Color8(32, 24, 20))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 22, 12, 10), Color8(240, 196, 160))
	_fill_rect(target, Rect2i(pos.x - 10, pos.y - 12, 20, 22), coat_col)
	_fill_rect(target, Rect2i(pos.x - 8, pos.y + 10, 6, 14), Color8(36, 36, 56))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 10, 6, 14), Color8(36, 36, 56))

func _draw_rpg_hero_sprite(target: Image, pos: Vector2i, guarding: bool) -> void:
	if _rpg_hero_sd != null:
		_blit_alpha_centered(target, _rpg_hero_sd, pos, false)
		if guarding:
			_fill_rect(target, Rect2i(pos.x - 16, pos.y - 22, 3, 44), Color8(96, 224, 255))
			_fill_rect(target, Rect2i(pos.x - 13, pos.y - 24, 26, 2), Color8(160, 244, 255))
			_fill_rect(target, Rect2i(pos.x - 13, pos.y + 20, 26, 2), Color8(160, 244, 255))
		return
	var jacket: Color = Color8(56, 112, 208) if not guarding else Color8(96, 168, 248)
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 26, 12, 6), Color8(40, 32, 28))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y - 20, 12, 10), Color8(248, 204, 168))
	_fill_rect(target, Rect2i(pos.x - 9, pos.y - 10, 18, 20), jacket)
	_fill_rect(target, Rect2i(pos.x - 7, pos.y + 10, 6, 14), Color8(40, 48, 72))
	_fill_rect(target, Rect2i(pos.x + 1, pos.y + 10, 6, 14), Color8(40, 48, 72))

func _draw_rpg_partner_sprite(target: Image, pos: Vector2i, girl_id: int) -> void:
	var g_col: Color = GIRL_PALETTE.get(girl_id, Color8(216, 72, 92))
	if _rpg_shiori_sd != null:
		_blit_alpha_centered(target, _rpg_shiori_sd, pos, false)
		if girl_id != 0:
			# Distinct hair crest & ribbon badge per partner heroine (0..10, 13)
			_fill_rect(target, Rect2i(pos.x - 8, pos.y - 24, 16, 5), g_col)
			_draw_circle_filled(target, Vector2i(pos.x + 10, pos.y - 18), 4, g_col)
		return
	# Partner heroine school uniform & ribbon
	_fill_rect(target, Rect2i(pos.x - 7, pos.y - 26, 14, 14), g_col)
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 20, 10, 10), Color8(252, 212, 180))
	_fill_rect(target, Rect2i(pos.x - 8, pos.y - 10, 16, 14), Color8(236, 240, 248))
	_fill_rect(target, Rect2i(pos.x - 4, pos.y - 8, 8, 5), g_col)
	_fill_rect(target, Rect2i(pos.x - 9, pos.y + 4, 18, 10), Color8(44, 68, 124))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 14, 4, 10), Color8(240, 240, 248))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 14, 4, 10), Color8(240, 240, 248))

func _draw_rpg_guest_sprite(target: Image, pos: Vector2i, guest_kind: String) -> void:
	var c := Color8(216, 168, 88)
	if guest_kind == "monk":
		c = Color8(168, 184, 216)
	elif guest_kind == "hunter":
		c = Color8(96, 168, 88)
	_draw_circle_filled(target, pos, 7, c)
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 4, 12, 8), c)

func _draw_rpg_club_special_effect(target: Image, club_id: int) -> void:
	var c_primary := Color8(255, 224, 80)
	var c_secondary := Color8(96, 216, 255)
	match club_id:
		0: # Literature (メガトン辞書)
			_fill_rect(target, Rect2i(58, 72, 28, 22), Color8(168, 48, 56))
			_fill_rect(target, Rect2i(62, 76, 20, 14), Color8(248, 240, 216))
		1: # Drama (脱力漫才)
			_draw_circle_filled(target, Vector2i(84, 92), 14, Color8(255, 232, 120))
		2: # Science (世界征服ロボ)
			_draw_line_solid(target, Vector2i(204, 88), Vector2i(56, 104), Color8(255, 72, 224))
			_draw_circle_filled(target, Vector2i(56, 104), 10, Color8(255, 192, 248))
		3: # Computer (サイバーファング)
			for i in range(4):
				_fill_rect(target, Rect2i(48 + i * 16, 84 + (i & 1) * 10, 12, 8), Color8(64, 248, 192))
		4: # Art (色素分解)
			_draw_circle_filled(target, Vector2i(64, 92), 8, Color8(255, 80, 96))
			_draw_circle_filled(target, Vector2i(80, 88), 8, Color8(80, 240, 120))
			_draw_circle_filled(target, Vector2i(96, 94), 8, Color8(80, 160, 255))
		5: # Brass Band (ハーメルンのホラ吹き)
			for i in range(3):
				_draw_circle_filled(target, Vector2i(60 + i * 22, 86 - i * 4), 5, c_primary)
		6: # Baseball (超球百裂弾)
			for i in range(4):
				_draw_circle_filled(target, Vector2i(52 + i * 18, 90 + (i & 1) * 12), 5, Color8(248, 248, 248))
		7: # Soccer (レッドカード乱舞)
			_fill_rect(target, Rect2i(68, 78, 14, 20), Color8(248, 48, 48))
		8: # Tennis (火の鳥サーブ)
			_draw_line_solid(target, Vector2i(210, 92), Vector2i(60, 102), Color8(255, 128, 40))
			_draw_circle_filled(target, Vector2i(60, 102), 8, Color8(255, 216, 64))
		9: # Swimming (水竜破)
			for i in range(5):
				_draw_circle_filled(target, Vector2i(50 + i * 14, 104 - i * 4), 7, c_secondary)
		10: # Basketball (ナイアガラダンク)
			_draw_circle_filled(target, Vector2i(76, 82), 9, Color8(240, 120, 40))
		_: # No-Club (気分最高)
			_draw_circle_filled(target, Vector2i(226, 74), 6, c_primary)

func _draw_rpg_assist_effect(target: Image, effect_kind: String) -> void:
	match effect_kind:
		"effect_a":
			_draw_circle_filled(target, Vector2i(76, 102), 11, Color8(255, 196, 64))
			_draw_circle_filled(target, Vector2i(76, 102), 5, Color8(255, 255, 224))
		"effect_b":
			_draw_line_solid(target, Vector2i(54, 88), Vector2i(98, 118), Color8(120, 232, 255))
			_draw_line_solid(target, Vector2i(98, 88), Vector2i(54, 118), Color8(120, 232, 255))
		"yuina_flash_x5":
			for i in range(5):
				_draw_circle_filled(target, Vector2i(44 + i * 16, 96 + (i & 1) * 10), 6, Color8(255, 240, 128))
		"sweatdrop":
			_fill_rect(target, Rect2i(280, 84, 5, 10), Color8(112, 208, 255))
		"laugh":
			_draw_circle_filled(target, Vector2i(252, 84), 4, Color8(255, 160, 216))
			_draw_circle_filled(target, Vector2i(282, 82), 4, Color8(255, 216, 96))

func _draw_taco_boss_3d(target: Image, pos: Vector2i, kind: String, alive: bool) -> void:
	if not alive:
		_draw_circle_filled(target, pos, 22, Color8(255, 168, 48))
		_draw_circle_filled(target, pos, 14, Color8(255, 244, 160))
		return
	match kind:
		"morph_boss":
			_draw_circle_filled(target, pos, 20, Color8(184, 64, 168))
			_draw_circle_filled(target, pos, 11, Color8(255, 200, 96))
		"collapse_boss":
			_fill_rect(target, Rect2i(pos.x - 28, pos.y - 16, 56, 32), Color8(96, 112, 168))
			_draw_circle_filled(target, pos, 12, Color8(248, 72, 88))
		_: # arm_tail_boss
			_fill_rect(target, Rect2i(pos.x - 24, pos.y - 14, 48, 28), Color8(192, 80, 72))
			_draw_circle_filled(target, pos + Vector2i(-28, 6), 7, Color8(232, 168, 64))
			_draw_circle_filled(target, pos + Vector2i(28, 6), 7, Color8(232, 168, 64))
			_draw_circle_filled(target, pos, 9, Color8(255, 232, 96))

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

func _draw_taco_laser_beams(target: Image, ship_pos: Vector2i, reticle_pos: Vector2i, weapon_kind: int = 0) -> void:
	var col: Color = Color8(112, 248, 255)
	if weapon_kind == Overlays.SHOOTER_PRIMARY_BEAM:
		col = Color8(255, 120, 232)
		_draw_line_solid(target, ship_pos, reticle_pos, Color8(255, 240, 160))
	elif weapon_kind == Overlays.SHOOTER_PRIMARY_VULCAN:
		col = Color8(255, 224, 72)
	_draw_line_solid(target, ship_pos + Vector2i(-14, 0), reticle_pos + Vector2i(-4, 0), col)
	_draw_line_solid(target, ship_pos + Vector2i(14, 0), reticle_pos + Vector2i(4, 0), col)

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
	## Exact port of `tt_clouds_draw` (`src/ovl/TT/falling_sprite_pool.c:133-191`):
	## `w = 0x40, h = 0x20, u = 0, v = 0x60, tpage = 0xB, clut = 0x3C8B`.
	if _tt_vram_ready:
		_blit_tt_vram_sprite(target, pos.x - 0x1F, pos.y - 0xF, 0x40, 0x20, 0, 0x60, 0xB, 0x3C8B, 0)
		return
	_draw_circle_filled(target, pos, 14, Color8(236, 244, 255))
	_draw_circle_filled(target, pos + Vector2i(-10, 4), 10, Color8(220, 232, 248))
	_draw_circle_filled(target, pos + Vector2i(10, 4), 10, Color8(220, 232, 248))

func _draw_tt_boss(target: Image, pos: Vector2i, boss_id: int = 0) -> void:
	match boss_id:
		1: # Swing-Ring Boss (`swing_ring_boss.c:122-162`: `tt_boss2_part_set = [3, 4, 4, 4, 4, 4]`, `tt_boss2_part_frame = [0, 33, 32, 45, 38, 42]`, ring parts `set = 4, frame = 0`)
			if _tt_vram_ready:
				for i in range(8):
					var ang: float = float(i) * PI / 4.0
					var rpos := pos + Vector2i(int(cos(ang) * 32.0), int(sin(ang) * 22.0))
					_draw_tt_part_sprite(target, rpos, 4, 0, 0, false, 0xF)
				var b2_sets := [3, 4, 4, 4, 4, 4]
				var b2_frames := [0, 33, 32, 45, 38, 42]
				for i in range(6):
					_draw_tt_part_sprite(target, pos, b2_sets[i], b2_frames[i], 0, false, 0xF)
				return
			for i in range(8):
				var ang: float = float(i) * PI / 4.0
				var rpos := pos + Vector2i(int(cos(ang) * 26.0), int(sin(ang) * 18.0))
				_draw_circle_filled(target, rpos, 5, Color8(96, 216, 248))
			_draw_circle_filled(target, pos, 13, Color8(72, 112, 216))
			_draw_circle_filled(target, pos, 6, Color8(255, 224, 80))
		2: # Multipart Form Boss (`multipart_form_boss.c:174-296`: `set = 9`, `clut = 0x3E50`)
			if _tt_vram_ready:
				_draw_tt_part_sprite(target, pos + Vector2i(41, 3), 9, 4, 2, false, 0xF)
				_draw_tt_part_sprite(target, pos + Vector2i(-41, 3), 9, 4, 0, false, 0xF)
				_draw_tt_part_sprite(target, pos + Vector2i(26, 36), 9, 2, 2, false, 0xF)
				_draw_tt_part_sprite(target, pos + Vector2i(-26, 36), 9, 2, 0, false, 0xF)
				_draw_tt_part_sprite(target, pos + Vector2i(38, 52), 9, 0xE, 2, false, 0xF)
				_draw_tt_part_sprite(target, pos + Vector2i(-38, 52), 9, 0xE, 0, false, 0xF)
				_draw_tt_part_sprite(target, pos, 9, 0, 0, false, 0xF)
				return
			_fill_rect(target, Rect2i(pos.x - 34, pos.y - 14, 68, 28), Color8(96, 136, 168))
			_fill_rect(target, Rect2i(pos.x - 26, pos.y - 8, 16, 16), Color8(232, 168, 64))
			_fill_rect(target, Rect2i(pos.x + 10, pos.y - 8, 16, 16), Color8(232, 168, 64))
			_draw_circle_filled(target, pos, 9, Color8(248, 80, 96))
		3: # Beam & Ring Boss (`beam_and_ring_boss.c:105-135`: `set = 10`, `tt_boss4_part_flip`, `tt_boss4_part_frame`)
			if _tt_vram_ready:
				var b4_flips := [0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0]
				var b4_frames := [2, 14, 14, 49, 49, 42, 42, 34, 34, 22, 0]
				for i in range(10, -1, -1):
					_draw_tt_part_sprite(target, pos, 10, b4_frames[i], b4_flips[i], false, 0xF)
				return
			_fill_rect(target, Rect2i(pos.x - 6, pos.y + 12, 12, 40), Color8(144, 240, 255))
			_draw_circle_filled(target, pos, 16, Color8(168, 80, 216))
			_draw_circle_filled(target, pos, 8, Color8(255, 240, 128))
		_: # Stage 1 Rope-Arm Boss (`rope_arm_boss.c:213-266, 584-612`)
			_draw_tt_boss1(target, pos)

func _draw_tt_boss1(target: Image, pos: Vector2i) -> void:
	## Exact port of `tt_boss1_init` & `tt_boss1_side_parts_init` (`src/ovl/TT/rope_arm_boss.c:213-345, 584-612`):
	## `tt_boss1_part_set = [6, 5, 6, 6]`, `tt_boss1_part_frame = [19, 0, 1, 1]`,
	## side pods (`set = 6, frame = 0x1F / 0x14`), and 7 jointed rope segments (`u=0x90, v=0x80, tpage=0x12, clut=0x3CD9`).
	if _tt_vram_ready:
		_draw_tt_part_sprite(target, pos + Vector2i(0, 0), 5, 0, 0, false, 0xF)
		_draw_tt_part_sprite(target, pos + Vector2i(0, -6), 6, 19, 0, false, 0xF)
		for i in range(7):
			var rx_off: int = 0x1C + int(sin(float(i) * 0.55) * 6.0)
			var ry_off: int = -0x12 + i * 9
			_blit_tt_vram_sprite(target, pos.x - rx_off - 8, pos.y + ry_off - 8, 16, 16, 0x90, 0x80, 0x12, 0x3CD9, 0)
			_blit_tt_vram_sprite(target, pos.x + rx_off - 8, pos.y + ry_off - 8, 16, 16, 0x90, 0x80, 0x12, 0x3CD9, 0)
		var core_y := [-8, 0, 0, -8, -16]
		for idx in range(5):
			var p_idx: int = idx + 2
			var px_off: int = 0 if p_idx == 6 else (p_idx * 16 - 0x38)
			var p_fr: int = 0x14 if p_idx == 6 else 0x1F
			_draw_tt_part_sprite(target, pos + Vector2i(px_off, core_y[idx]), 6, p_fr, 0, false, 0xF)
		_draw_tt_part_sprite(target, pos + Vector2i(0x18, 0x34), 6, 1, 0, false, 0xF)
		_draw_tt_part_sprite(target, pos + Vector2i(-0x18, 0x34), 6, 1, 0, false, 0xF)
		return
	for i in range(6):
		var seg_l := pos + Vector2i(-24 - i * 5, 10 + i * 6)
		var seg_r := pos + Vector2i(24 + i * 5, 10 + i * 6)
		_draw_circle_filled(target, seg_l, 5, Color8(208, 160, 64))
		_draw_circle_filled(target, seg_r, 5, Color8(208, 160, 64))
	_fill_rect(target, Rect2i(pos.x - 28, pos.y - 16, 56, 30), Color8(184, 72, 88))
	_draw_circle_filled(target, pos, 12, Color8(255, 216, 72))
	_draw_circle_filled(target, pos, 6, Color8(248, 64, 64))

func _draw_tt_bell(target: Image, pos: Vector2i, kind: int, bstate: int = 0x40) -> void:
	## Exact port of `tt_bells_draw` (`src/ovl/TT/cycling_pickups.c:41-78, 224-275`):
	## `tpage = 0xA, clut = 0x3C84 + kind`, `u = 0xC0, v = 0x70 + 0x80 = 0xF0` (`anim_list[0]`).
	if _tt_vram_ready:
		if bstate >= 0x90:
			_blit_tt_vram_sprite(target, pos.x - 0x1F, pos.y - 7, 0x40, 0x10, 0x40, 0x00, 0xB, 0x3C81, 0)
		elif bstate == 0x60 or bstate == 0x70:
			_blit_tt_vram_sprite(target, pos.x - 8, pos.y - 8, 16, 16, 0xD0, 0xC0, 0xA, 0x3C86, 0)
		else:
			var bclut: int = 0x3C84 + clampi(kind, 0, 7)
			_blit_tt_vram_sprite(target, pos.x - 8, pos.y - 8, 16, 16, 0xC0, 0xF0, 0xA, bclut, 0)
		return
	if bstate == 0x90:
		_draw_circle_filled(target, pos, 8, Color8(255, 248, 160))
		_draw_circle_filled(target, pos, 4, Color8(120, 248, 255))
		return
	var c: Color = Color8(255, 224, 48) # 0: Yellow
	if bstate == 0x60 or bstate == 0x70:
		c = Color8(248, 72, 72) # Over-cycled hazard bell
	elif kind == 1:
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

func _draw_tt_air_enemy(target: Image, pos: Vector2i, estate: int = 0x40) -> void:
	## Exact port of `tt_air_enemies_draw` (`src/ovl/TT/air_enemies_and_sprite_drawing.c:34-70, 848-933`).
	if _tt_vram_ready:
		if estate == 0x70 or estate == 0x71:
			_draw_tt_part_sprite(target, pos, 2, 0)
		else:
			_blit_tt_vram_sprite(target, pos.x - 8, pos.y - 8, 16, 16, 0x90, 0x00, 0xA, 0x3CC0, 0)
		return
	if estate == 0x70:
		_draw_circle_filled(target, pos, 9, Color8(255, 160, 48))
		_draw_circle_filled(target, pos, 5, Color8(255, 244, 144))
		return
	_draw_circle_filled(target, pos, 7, Color8(240, 88, 72))
	_draw_circle_filled(target, pos + Vector2i(0, -2), 3, Color8(255, 232, 160))

func _draw_tt_player(target: Image, pos: Vector2i) -> void:
	## Exact port of `tt_player_draw` (`src/ovl/TT/player_and_asset_uploads.c:771-808`):
	## Left arm (`set = 0, frame = 5`), right arm (`set = 0, frame = 10`), and TwinBee body (`set = 0, frame = 0`).
	if _tt_vram_ready:
		_draw_tt_part_sprite(target, pos, 0, 5)
		_draw_tt_part_sprite(target, pos, 0, 10)
		_draw_tt_part_sprite(target, pos, 0, 0)
		return
	_fill_rect(target, Rect2i(pos.x - 13, pos.y - 3, 5, 8), Color8(240, 244, 255))
	_fill_rect(target, Rect2i(pos.x + 8, pos.y - 3, 5, 8), Color8(240, 244, 255))
	_draw_circle_filled(target, pos, 9, Color8(64, 152, 248))
	_draw_circle_filled(target, pos + Vector2i(0, -3), 5, Color8(255, 220, 96))
	_fill_rect(target, Rect2i(pos.x - 6, pos.y + 7, 4, 4), Color8(255, 200, 48))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y + 7, 4, 4), Color8(255, 200, 48))

func _draw_tt_option(target: Image, pos: Vector2i) -> void:
	## Exact port of `tt_options_reset` & `tt_options_draw` (`src/ovl/TT/sprite_animation_and_followers.c:147-167`):
	## `set = 0, frame = 0x37`.
	if _tt_vram_ready:
		_draw_tt_part_sprite(target, pos, 0, 0x37)
		return
	_draw_circle_filled(target, pos, 5, Color8(255, 168, 96))

func _draw_tt_twin_shots(target: Image, pos: Vector2i) -> void:
	## Exact port of `tt_shots_draw` (`src/ovl/TT/title_and_projectile_pools.c:898-901, 1028-1060`):
	## Twin Shot (`shot_level = 9`) at `u = 0x90, v = 0xC0, tpage = 0xA, clut = 0x3C87`.
	if _tt_vram_ready:
		_blit_tt_vram_sprite(target, pos.x - 8, pos.y - 8, 16, 16, 0x90, 0xC0, 0xA, 0x3C87, 0)
		return
	_fill_rect(target, Rect2i(pos.x - 5, pos.y - 4, 3, 8), Color8(255, 248, 120))
	_fill_rect(target, Rect2i(pos.x + 2, pos.y - 4, 3, 8), Color8(255, 248, 120))

func _draw_tt_life_icon(target: Image, pos: Vector2i) -> void:
	## Exact port of `tt_state_stage` life icon sprites (`src/ovl/TT/play_flow_and_scroll_scripts.c:244-259`):
	## `w = 16, h = 16, u = 0, v = 0x70, tpage = 0xA, clut = 0x3C0B`.
	if _tt_vram_ready:
		_blit_tt_vram_sprite(target, pos.x, pos.y, 16, 16, 0, 0x70, 0xA, 0x3C0B, 0)
		return
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
