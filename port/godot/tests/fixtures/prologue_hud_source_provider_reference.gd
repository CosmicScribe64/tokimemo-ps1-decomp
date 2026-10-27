class_name PrologueHudSourceProviderReference
extends RefCounted
## Isolated source-resource access for the first prologue HUD entry.
## This provider returns individual reviewed assets and draw metadata. It does
## not assemble a scene, advance game state, or substitute captured imagery.

const PROFILE_RELATIVE := ".claude/orchestrator/state/T-29025/prologue-hud-provider-v2/resources/manifest.json"
const PROFILE_SCHEMA := "tokimemo.prologue-hud-source-profile.v2"
const INSTALLED_PROFILE_SCHEMA := "tokimemo.prologue-hud-installed-profile.v1"
const INSTALLED_PROFILE_SHA256 := "a0756a103e1b01e2e0c5ac6d4c9635ade80d2a067c7d649c3af7f5f73feacd36"
const INSTALLED_SOURCE_EVIDENCE := [
	["asm/data/data_800B5950.data.s", "4482f3ec44d7a64fbcb1b305e015452e014382d6d819dd8759e730d3f0b2d439"],
	["asm/data/main/text_system.data.s", "9eafc7dd540f7823b7afa43c8b06dc39b9fab41ad7bdf39df1b921e077931510"],
	["asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s", "cfac6e0ef3b45316f6b75ea13e49cfe7f36075008489d32ebefe9d48e98872b4"],
	["asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_arrival_scene_init.s", "38586534cee5fa8c3d1fa5dca44ad2ae7aa647917b6e5c45638a7faf56eb7df9"],
	["include/main/hud.h", "dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3"],
	["src/main/debug_hud_palette.c", "a2bf8f42fb2857ebe80177d6c1ef5222e5952440bc9c8424a9b88cecf8961f7b"],
	["src/main/game_init.c", "ecd91c6bc45e2670ce332b5d53fb9e5fe2cfb767bb9e0c46e09f3741595bed53"],
	["src/main/hud_panels.c", "b942acb5863f26326c044720768384cf23b156f4736c69f1eda0d34831016ae1"],
	["src/main/hud_screens.c", "4572545b4566949100d038bff53ab9cdcc0dc63fb31c3de155119ff7141251b0"],
	["src/main/main_loop.c", "c32d3d6d4d9440ebcbc2f45a5f8a7ebc0774a2cf7e1790c8b5b195c5c93d80a7"],
	["src/ovl/name_entry/file_select.c", "ff3e5ca243684c1a90023f4fa33a9ce5666d7b5ca773056b972b6f8420b856f5"],
	["src/ovl/name_entry/opening.c", "d4cbc22124d25f33d958c82f07f2f7c5674acf61803b5ac7969ae9bba761e5b1"],
	["src/ovl/name_entry/prologue.c", "cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e"]
]
const INSTALLED_FRAME_IDS := [60, 62, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
	82, 83, 84, 85, 86, 87, 88, 113, 114, 115]
const PrologueHudAssetsSchema := "tokimemo.prologue-hud-assets.v1"
const CheckerSchema := "tokimemo.native-hud-checker-indexed.v2"
const FontSchema := "tokimemo.native-font.v1"
const NativeGlyphsScript = preload("res://presentation/native_glyphs.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const Presenter = preload("res://presentation/prologue_presenter.gd")
const Math = preload("res://core/native_math.gd")

var _repo_root := ""
var _profile: Dictionary = {}
var _hud_assets: Dictionary = {}
var _checker: Dictionary = {}
var _glyphs
var _open := false
var _checker_indices := PackedByteArray()
var _checker_palettes := PackedByteArray()

func open(repo_root: String, expected_profile_sha256: String) -> String:
	_close()
	if not repo_root.is_absolute_path() or repo_root.simplify_path() != repo_root:
		return "repository root must be a canonical absolute path"
	var profile_path := repo_root.path_join(PROFILE_RELATIVE)
	if not _external_file(profile_path):
		return "source profile is missing, linked, or inside the runtime package"
	if not _is_sha256(expected_profile_sha256) or FileAccess.get_sha256(profile_path) != expected_profile_sha256:
		return "source profile hash differs from the caller's reviewed pin"
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(profile_path))
	if not parsed is Dictionary or parsed.get("schema") != PROFILE_SCHEMA:
		return "unsupported prologue HUD profile schema"
	if parsed.get("profile_id") != "prologue_stage0_entry.v2" or parsed.get("runtime_execution_proven") != false:
		return "profile identity or evidence boundary is unsupported"
	var pin_error := _verify_source_evidence(repo_root, parsed)
	if not pin_error.is_empty():
		return pin_error
	var resource_error := _load_resource_manifests(repo_root, parsed)
	if not resource_error.is_empty():
		_close()
		return resource_error
	var identity_error := _validate_resource_identity(parsed)
	if not identity_error.is_empty():
		_close()
		return identity_error
	var checker_error := _load_indexed_checker(repo_root, parsed)
	if not checker_error.is_empty():
		_close()
		return checker_error
	var font_dir: String = repo_root.path_join(str(parsed["resources"]["font"]["directory"]))
	_glyphs = NativeGlyphsScript.new()
	var font_error: String = _glyphs.open_external_font(font_dir)
	if not font_error.is_empty():
		_close()
		return "original font rejected: " + font_error
	_repo_root = repo_root
	_profile = parsed
	_open = true
	return ""

## Open the portable pack inside an installed bundle. Unlike `open`, this
## validates only pinned package metadata and resource files; it never reads
## repository source, the disc, or a development cache.
func open_installed(bundle_root: String, profile_relative_path: String,
		expected_profile_sha256: String) -> String:
	_close()
	if not bundle_root.is_absolute_path() or bundle_root.simplify_path() != bundle_root \
			or not DirAccess.dir_exists_absolute(bundle_root) or not ExternalPath.is_external_path(bundle_root):
		return "installed bundle root must be a canonical external directory"
	if not _safe_relative_path(profile_relative_path):
		return "installed profile path must be a safe relative path"
	var profile_path := bundle_root.path_join(profile_relative_path)
	if not _bundle_file(bundle_root, profile_relative_path):
		return "installed profile is missing, linked, or outside the bundle"
	if not _is_sha256(expected_profile_sha256) or expected_profile_sha256 != INSTALLED_PROFILE_SHA256 \
			or FileAccess.get_sha256(profile_path) != expected_profile_sha256:
		return "installed profile hash differs from the caller's reviewed pin"
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(profile_path))
	if not parsed is Dictionary or parsed.get("schema") != INSTALLED_PROFILE_SCHEMA:
		return "unsupported installed prologue HUD profile schema"
	if not _json_integer_is(parsed.get("version"), 1) or parsed.get("release_profile") != "original-ja-ps-the-best" \
			or parsed.get("profile_id") != "prologue_stage0_entry.v2" \
			or parsed.get("runtime_execution_proven") != false:
		return "installed profile identity or evidence boundary is unsupported"
	var contract_error := _validate_installed_profile_metadata(parsed)
	if not contract_error.is_empty():
		return contract_error
	var inputs: Variant = parsed.get("inputs")
	if not inputs is Dictionary:
		return "installed source input identities are malformed"
	var expected_inputs := {
		"executable": ["SLPM_86.053", "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"],
		"mainrom": ["CDROM/FUKUTEST/MAINROM.NBN", "f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c"],
		"calrom": ["CDROM/FUKUTEST/CALROM.NBN", "775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867"],
		"name_ent_overlay": ["CDROM/EXEDIR/NAME_ENT.EXN", "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2"]}
	for key: String in expected_inputs:
		var item: Variant = inputs.get(key)
		if not item is Dictionary or item.get("path") != expected_inputs[key][0] \
				or item.get("sha256") != expected_inputs[key][1]:
			return "installed profile source release identity differs: " + key
	var resources: Variant = parsed.get("resources")
	if not resources is Dictionary:
		return "installed resource bindings are malformed"
	var expected_resources := {
		"checker": ["packs/prologue_hud/checker", "manifest.json", CheckerSchema],
		"prologue_assets": ["packs/prologue_hud", "assets-manifest.json", PrologueHudAssetsSchema],
		"font": [".asset-cache/port/fonts/native-font-v2", "font.json", FontSchema]}
	for key: String in expected_resources:
		var item: Variant = resources.get(key)
		if not item is Dictionary or item.get("directory") != expected_resources[key][0] \
				or item.get("manifest") != expected_resources[key][1] \
				or item.get("schema") != expected_resources[key][2] \
				or not _is_sha256(str(item.get("manifest_sha256", ""))):
			return "installed resource locator/schema is unsupported: " + key
	var evidence: Variant = parsed.get("source_evidence")
	if not evidence is Array or evidence.is_empty():
		return "installed source provenance is empty"
	for record: Variant in evidence:
		if not record is Dictionary or not _safe_relative_path(str(record.get("path", ""))) \
				or not _is_sha256(str(record.get("sha256", ""))):
			return "installed source provenance record is malformed"
	var resource_error := _load_resource_manifests(bundle_root, parsed)
	if not resource_error.is_empty():
		_close()
		return resource_error
	var identity_error := _validate_resource_identity(parsed)
	if not identity_error.is_empty():
		_close()
		return identity_error
	var asset_error := _preflight_installed_assets(bundle_root, parsed)
	if not asset_error.is_empty():
		_close()
		return asset_error
	var checker_error := _load_indexed_checker(bundle_root, parsed)
	if not checker_error.is_empty():
		_close()
		return checker_error
	var font_dir: String = bundle_root.path_join(str(parsed["resources"]["font"]["directory"]))
	_glyphs = NativeGlyphsScript.new()
	var font_error: String = _glyphs.open_external_font(font_dir)
	if not font_error.is_empty():
		_close()
		return "installed original font rejected: " + font_error
	_repo_root = bundle_root
	_profile = parsed
	_open = true
	return ""

func stage0_contract(live_state: Dictionary) -> Dictionary:
	if not _open:
		return _unsupported("provider is not open")
	var required := ["game_mode", "mode_stage", "stage_init_substep", "title_timer",
		"screen_brightness", "year", "month", "day", "weekday", "day_kind",
		"checker_scroll_u", "checker_scroll_v"]
	for key: String in required:
		if not live_state.has(key) or typeof(live_state[key]) != TYPE_INT:
			return _unsupported("missing integer live state: " + key)
	var p: Dictionary = _profile["stage0_contract"]
	var expect := {"game_mode": 21, "mode_stage": 0, "stage_init_substep": 0,
		"title_timer": -64, "screen_brightness": 0, "year": 95, "month": 4,
		"day": 4, "weekday": 1, "day_kind": 0}
	for key: String in expect:
		if int(live_state[key]) != int(expect[key]):
			return _unsupported("live stage0 value differs at " + key)
	if int(live_state["screen_brightness"]) < 0 or int(live_state["screen_brightness"]) > 128:
		return _unsupported("screen brightness is outside source range")
	if str(live_state.get("checker_scroll_phase", "")) != "after_hud_screen_frame_draw_scroll_update":
		return _unsupported("checker scroll values must be sampled after the source scroll update")
	var scroll_u: int = int(live_state["checker_scroll_u"])
	var scroll_v: int = int(live_state["checker_scroll_v"])
	var season_index: int = _season_for_month(int(live_state["month"]))
	return {
		"status": "stage0_resources_supported",
		"profile_id": _profile["profile_id"],
		"canvas": p["canvas"],
		"game_mode": 21,
		"mode_stage": 0,
		"hud_screen_frame_draw_argument": 0,
		"initialization_order": p["initialization_order"],
		"title": {"raw_bytes": title_bytes(), "timer_before_first_update": -64,
			"timer_after_first_update": -63, "source_actor_frames": [113, 114, 115],
			"glyph_advance": 14, "gradient_origin_screen": [16, 30], "gradient_size": [78, 20]},
		"message_window": {"frame_id": 62, "frame_local_position": [0, 75],
			"gradient_rect_screen": [27, 166, 266, 52]},
		"date_pillar": {"frame_id": 60, "frame_local_position": [144, -18],
			"background_gradient_screen": [294, 45, 20, 114],
			"date": [int(live_state["year"]), int(live_state["month"]), int(live_state["day"]), int(live_state["weekday"])],
			"day_kind": int(live_state["day_kind"]), "resource_family": "MAINROM actor frames"},
		"checker": {"season_index": season_index, "brightness": int(live_state["screen_brightness"]),
			"raw_scroll": [scroll_u, scroll_v],
			"uv_origin": [(scroll_u & 31) + 128, (scroll_v & 31) + 128],
			"tile_pixels": [32, 32], "tiles": [10, 8]},
		"calendar_month_art": "optional CALROM calendar-screen resource; not the stage0 date pillar",
		"scope": "resource and draw contract only; no scene composition, route, or pixel parity"
	}

## Transaction callbacks mutate only the caller's transaction-owned dictionaries.
## They reject missing source state rather than inventing boot-time globals.
func scene_reset(state: Dictionary, result: Dictionary) -> String:
	if not _open:
		return "provider is not open"
	var live := _live_hud(state)
	if live.is_empty():
		return "transaction lacks live prologue HUD scroll/season state"
	if not _int_field(state, "game_mode", 21) or not _int_field(state, "mode_stage", 0):
		return "scene reset is limited to prologue mode stage 0"
	if typeof(state.get("frame_count", null)) != TYPE_INT:
		return "scene reset requires the live frame count for source srand(frame_count)"
	if _brightness(state) < 0:
		return "scene reset requires Arrival's explicit brightness owner"
	var next := live.duplicate(true)
	next["title_timer"] = -64
	next["title_bytes"] = title_bytes()
	next["title_visible"] = true
	next["date_visible"] = true
	next["message_visible"] = true
	next["screen_brightness"] = _brightness(state)
	next["checker_fade"] = 0xfe
	next.erase("checker_clut_fade")
	next["checker_scroll_phase"] = "before_outer_scroll_update"
	state["opening"]["prologue_hud_live"] = next
	result["prologue_hud"] = {"callback": "scene_reset", "rng_reseed_request": int(state["frame_count"]), "effects": [
		"D_800B5BC8=0xFE", "text_system_reset", "message_window_set_title", "hizuke_init", "message_window_init",
		"dec_bg_reset", "hizuke_disp_switch(1)"],
		"brightness_owner":"opening.screen_brightness", "brightness_mutated":false}
	return ""

func advance_outer(state: Dictionary, rng: Object, result: Dictionary) -> String:
	if not _open:
		return "provider is not open"
	var live := _live_hud(state)
	if live.is_empty():
		return "transaction lacks initialized prologue HUD state"
	if not _int_field(state, "game_mode", 21) or not _int_field(state, "mode_stage", 0):
		return "outer HUD update is limited to prologue mode stage 0"
	if typeof(state.get("frame_count")) != TYPE_INT or _brightness(state) < 0:
		return "outer HUD update requires live frame and Arrival brightness owner"
	if state.has("pad_held_triangle") and typeof(state["pad_held_triangle"]) != TYPE_BOOL:
		return "outer HUD update requires a boolean triangle-held observation"
	var frame: int = state["frame_count"]
	var flags: Variant = state.get("player_flags", null)
	if frame < 0 or frame > 0xffffffff or not flags is PackedByteArray or flags.size() < 2:
		return "outer HUD update requires frame_count and two live player flag bytes"
	var next := live.duplicate(true)
	var u: int = int(next["checker_scroll_u"])
	var v: int = int(next["checker_scroll_v"])
	var draws: Array[String] = []
	match int(flags[1]) & 15:
		1:
			if (frame & 3) == 3: v -= 1
			if (frame & 1) == 1: u += 1
		2:
			if (frame & 7) == 7: v -= 1
			if (frame & 3) == 3: u += 1
		3:
			if (frame & 0x15) == 0x15: v -= 1
			if (frame & 7) == 7: u += 1
		4:
			if rng == null or not rng.has_method("next"):
				return "checker mode 4 requires the caller's transaction RNG"
			var draw_v: int = int(rng.call("next", "src/main/hud_panels.c::hud_screen_frame_draw/mode4_v"))
			if draw_v < 0: return "transaction RNG rejected the source mode-4 V draw"
			v += 16 - (draw_v & 31)
			draws.append("src/main/hud_panels.c::hud_screen_frame_draw/mode4_v")
			var draw_u: int = int(rng.call("next", "src/main/hud_panels.c::hud_screen_frame_draw/mode4_u"))
			if draw_u < 0: return "transaction RNG rejected the source mode-4 U draw"
			u += 16 - (draw_u & 31)
			draws.append("src/main/hud_panels.c::hud_screen_frame_draw/mode4_u")
			if rng.has_method("step_error") and not str(rng.call("step_error")).is_empty():
				return "transaction RNG reports a failed checker update"
		5:
			var trig = load("res://rules/calendar_phone_screens.gd")
			v += int(trig.rsin(frame)) / 32
			u += int(trig.rsin(frame + 0x400)) / 32
		_:
			if (frame & 1) == 1: v -= 1
			u += 1
	u = Math.u8(u)
	v = Math.u8(v)
	next["checker_scroll_u"] = u
	next["checker_scroll_v"] = v
	next["checker_scroll_phase"] = "after_hud_screen_frame_draw_scroll_update"
	var brightness: int = _brightness(state)
	next["screen_brightness"] = brightness
	var title: Dictionary = Presenter.compute_title_badge_state(int(next.get("title_timer", -64)),
		bool(state.get("pad_held_triangle", false)), 5)
	next["title_timer"] = int(title["next_timer"])
	next["title_badge"] = title
	var frame_count: int = frame
	var fade: int = int(next.get("checker_fade", 255))
	var season: int = _season_for_month(int(next.get("month", -1)))
	if (frame_count & 3) == 3 and fade >= 239:
		fade = (fade + 1) & 255
		if fade == 255:
			next["checker_clut_reload_season"] = season
			next.erase("checker_clut_fade")
		elif fade >= 240:
			next["checker_clut_fade"] = fade
	next["checker_fade"] = fade
	next["checker_season"] = season
	state["opening"]["prologue_hud_live"] = next
	result["prologue_hud"] = {"callback": "advance_outer", "frame_count": frame,
		"mode": int(flags[1]) & 15, "rng_draws": draws,
		"scroll": [u, v], "screen_brightness": brightness, "title_timer": next["title_timer"],
		"checker_season": season, "checker_fade": fade}
	return ""

## Pure raster callbacks. The target is caller-owned and is only modified by these draws.
func draw_checker(target: Image, state: Dictionary) -> String:
	if not _open or target == null or target.get_size() != Vector2i(320, 240):
		return "checker target/provider must be open and 320x240"
	var live := _live_hud(state)
	if live.is_empty() or live.get("checker_scroll_phase") != "after_hud_screen_frame_draw_scroll_update":
		return "checker draw requires committed post-update HUD state"
	var season: int = _season_for_month(live["month"])
	var brightness: int = _brightness(state)
	if season < 0 or brightness < 0:
		return "checker requires source month and Arrival-owned brightness"
	if live.has("checker_clut_fade") and typeof(live["checker_clut_fade"]) != TYPE_INT:
		return "checker CLUT fade must be an integer"
	var fade: int = live.get("checker_clut_fade", 255)
	if fade < 240 or fade > 255:
		return "checker CLUT fade outside source transition"
	var u: int = live["checker_scroll_u"] & 31
	var v: int = live["checker_scroll_v"] & 31
	for y in range(240):
		for x in range(320):
			# Each SPRT restarts at UV(128+scroll,128+scroll). Do not wrap
			# the source pixel at 32; its uploaded indexed region is 128x128.
			var index: int = _checker_indices[((y & 31) + v) * 128 + (x & 31) + u]
			var word: int = _checker_palettes.decode_u16(season * 32 + index * 2)
			var rgb := _word_rgb5(word)
			if fade <= 254:
				var old_rgb := _word_rgb5(_checker_palettes.decode_u16(((season + 3) & 3) * 32 + index * 2))
				rgb = Vector3i(((255 - fade) * old_rgb.x + (fade - 239) * rgb.x) >> 4,
					((255 - fade) * old_rgb.y + (fade - 239) * rgb.y) >> 4,
					((255 - fade) * old_rgb.z + (fade - 239) * rgb.z) >> 4)
			# PS1 texture modulation quantizes back to RGB555; black texels
			# are transparent before modulation, while brightness zero draws black.
			if rgb == Vector3i.ZERO and (word & 0x8000) == 0: continue
			rgb = Vector3i((rgb.x * brightness) >> 7, (rgb.y * brightness) >> 7, (rgb.z * brightness) >> 7)
			target.set_pixel(x, y, Color8(rgb.x << 3, rgb.y << 3, rgb.z << 3, 255))
	return ""

func _brightness(state: Dictionary) -> int:
	if not state.get("opening") is Dictionary: return -1
	var value: Variant = state["opening"].get("screen_brightness")
	return value if typeof(value) == TYPE_INT and value >= 0 and value <= 128 else -1

## Explicit handoff of the original HUD globals; never infer scroll from tick.
## A caller must own the accumulator through the entry prefix. This API validates
## transport, not the truth of an arbitrary owner's claimed native history.
func import_scroll(live_hud: Dictionary, transport: Dictionary) -> Dictionary:
	if not _open: return _unsupported("provider is not open")
	if transport.size() != 5 or not transport.get("owner") is String or transport["owner"].is_empty():
		return _unsupported("explicit owned original checker scroll transport required")
	var globals: Variant = transport.get("source_globals")
	if not globals is Array or globals.size() != 2:
		return _unsupported("checker source_globals must be the two original names")
	for name: Variant in globals:
		if typeof(name) != TYPE_STRING:
			return _unsupported("checker source_globals elements must be strings")
	if globals[0] != "D_800B5BCC" or globals[1] != "D_800B5BD0":
		return _unsupported("checker source_globals differ from the original names")
	var phase: Variant = transport.get("phase")
	if typeof(phase) != TYPE_STRING or phase not in ["before_outer_scroll_update", "after_hud_screen_frame_draw_scroll_update"]:
		return _unsupported("explicit owned original checker scroll transport required")
	for key in ["u", "v"]:
		if typeof(transport.get(key)) != TYPE_INT or transport[key] < 0 or transport[key] > 255:
			return _unsupported("checker transport requires original u8 u/v")
	var next := live_hud.duplicate(true)
	next["checker_scroll_u"] = transport["u"]
	next["checker_scroll_v"] = transport["v"]
	next["checker_scroll_phase"] = transport["phase"]
	next["checker_scroll_owner"] = transport["owner"]
	return {"status":"scroll_imported", "hud":next}

func draw_front(target: Image, state: Dictionary) -> String:
	if not _open or target == null or target.get_size() != Vector2i(320, 240):
		return "front target/provider must be open and 320x240"
	var live := _live_hud(state)
	if live.is_empty():
		return "front draw requires committed prologue HUD state"
	if bool(live.get("message_visible", false)):
		_draw_gradient(target, Rect2i(27, 166, 266, 52))
		var err := _draw_frame(target, 62, Vector2i(160, 195))
		if not err.is_empty(): return err
	if bool(live.get("date_visible", false)):
		_draw_gradient(target, Rect2i(294, 45, 20, 114))
		var err := _draw_frame(target, 60, Vector2i(304, 102))
		if not err.is_empty(): return err
		var year_text := str(int(live.get("year", 95)) % 100).pad_zeros(2)
		var year_x := 300
		for digit_text in year_text:
			var e := _draw_digit(target, int(digit_text), Vector2i(year_x, 55))
			if not e.is_empty(): return e
			year_x += 8
		for date_value in [[int(live.get("month", 4)), 86], [int(live.get("day", 4)), 119]]:
			var date_text := str(date_value[0])
			var x: int = 300 if date_text.length() == 2 else 304
			for digit_text in date_text:
				var e := _draw_digit(target, int(digit_text), Vector2i(x, int(date_value[1])))
				if not e.is_empty(): return e
				x += 8
		var weekday_frame := frame_parts(int(live.get("weekday", 1)) + 0x52, "base")
		var weekday_error := _draw_parts(target, weekday_frame, Vector2i(306, 151))
		if not weekday_error.is_empty(): return weekday_error
	if bool(live.get("title_visible", false)):
		var badge: Dictionary = live.get("title_badge", Presenter.compute_title_badge_state(int(live.get("title_timer", -64))))
		if bool(badge.get("visible", false)):
			_draw_gradient(target, Rect2i(Vector2i(16, 30) + badge["offset"], Vector2i(78, 20)))
			var slide: int = int(badge["offset"].x)
			for i in range(7):
				var title_frame: int = 115 if i == 0 else (113 if i == 6 else 114)
				var local := Vector2i((5 - i) * 14 + slide - 135, slide - 81)
				if i == 0: local.x -= 2
				if i == 6: local.x += 2
				var err := _draw_frame(target, title_frame, Vector2i(160, 120) + local)
				if not err.is_empty(): return err
			var raw: PackedByteArray = title_bytes()
			for i in range(0, raw.size(), 2):
				var glyph := glyph_image(raw.slice(i, i + 2).hex_encode(), 7, 0, 128)
				if glyph.get("status") != "glyph": return "title glyph rejected by native font source"
				var pos: Vector2i = badge["text_pos"] + Vector2i(i / 2 * 14, 0)
				_blit_glyph(target, glyph["image"], pos)
	return ""

func _live_hud(state: Dictionary) -> Dictionary:
	if not state.get("opening") is Dictionary:
		return {}
	var value: Variant = state["opening"].get("prologue_hud_live", null)
	if not value is Dictionary:
		return {}
	if not value.get("checker_scroll_owner") is String or value["checker_scroll_owner"].is_empty():
		return {}
	for key in ["checker_scroll_u", "checker_scroll_v", "year", "month", "day", "weekday",
			"day_kind", "screen_brightness", "title_timer", "checker_fade"]:
		if typeof(value.get(key, null)) != TYPE_INT: return {}
	for key in ["checker_scroll_u", "checker_scroll_v"]:
		if value[key] < 0 or value[key] > 255: return {}
	if typeof(value.get("checker_scroll_phase")) != TYPE_STRING \
		or value["checker_scroll_phase"] not in ["before_outer_scroll_update", "after_hud_screen_frame_draw_scroll_update"]:
		return {}
	for key in ["title_visible", "date_visible", "message_visible"]:
		if value.has(key) and typeof(value[key]) != TYPE_BOOL: return {}
	if value.has("checker_clut_fade") and typeof(value["checker_clut_fade"]) != TYPE_INT: return {}
	if value.has("title_badge"):
		var badge: Variant = value["title_badge"]
		if not badge is Dictionary or typeof(badge.get("visible")) != TYPE_BOOL: return {}
		if badge["visible"] and (typeof(badge.get("offset")) != TYPE_VECTOR2I or typeof(badge.get("text_pos")) != TYPE_VECTOR2I):
			return {}
	return value

func _int_field(state: Dictionary, key: String, wanted: int) -> bool:
	return typeof(state.get(key, null)) == TYPE_INT and int(state[key]) == wanted

func _draw_frame(target: Image, frame_id: int, actor_position: Vector2i) -> String:
	return _draw_parts(target, frame_parts(frame_id), actor_position)

func _draw_digit(target: Image, digit: int, actor_position: Vector2i) -> String:
	if digit < 0 or digit > 9: return "date digit is outside source glyph table"
	return _draw_parts(target, frame_parts(0x44 + digit, "base"), actor_position)

func _draw_parts(target: Image, frame: Dictionary, actor_position: Vector2i) -> String:
	if frame.get("status") != "frame_parts": return str(frame.get("diagnostic", "frame unavailable"))
	var origin: Vector2i = Vector2i(frame["origin"][0], frame["origin"][1])
	for part: Dictionary in frame["parts"]:
		var image: Image = part["color"]["image"]
		var mask: Image = part["stp_mask"]["image"]
		var pos: Vector2i = actor_position + Vector2i(part["source_xy"][0], part["source_xy"][1]) - origin
		for y in range(image.get_height()):
			for x in range(image.get_width()):
				var p: Vector2i = pos + Vector2i(x, y)
				if p.x < 0 or p.y < 0 or p.x >= target.get_width() or p.y >= target.get_height(): continue
				var c: Color = image.get_pixel(x, y)
				if c.a == 0: continue
				var rgb := Vector3i(c.r8 >> 3, c.g8 >> 3, c.b8 >> 3)
				if mask.get_pixel(x, y).r8 != 0:
					var back: Color = target.get_pixelv(p)
					rgb = Vector3i((back.r8 >> 4) + (rgb.x >> 1), (back.g8 >> 4) + (rgb.y >> 1), (back.b8 >> 4) + (rgb.z >> 1))
				target.set_pixelv(p, Color8(rgb.x << 3, rgb.y << 3, rgb.z << 3, 255))
	return ""

func _draw_gradient(target: Image, rect: Rect2i) -> void:
	const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]
	for y in range(rect.size.y):
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			var py: int = rect.position.y + y
			if px < 0 or py < 0 or px >= target.get_width() or py >= target.get_height(): continue
			var factor: float = 10.0 - 2.0 * x / rect.size.x - 4.0 * y / rect.size.y
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var red: int = clampi((int(3 * factor) + d) >> 3, 0, 31)
			var green: int = clampi((int(14 * factor) + d) >> 3, 0, 31)
			target.set_pixel(px, py, Color8(red << 3, green << 3, 0, 255))

func _blit_glyph(target: Image, glyph: Image, position: Vector2i) -> void:
	for y in range(glyph.get_height()):
		for x in range(glyph.get_width()):
			var p := position + Vector2i(x, y)
			if p.x < 0 or p.y < 0 or p.x >= target.get_width() or p.y >= target.get_height(): continue
			var color: Color = glyph.get_pixel(x, y)
			if color.a > 0: target.set_pixelv(p, color)

func _word_rgb5(word: int) -> Vector3i:
	return Vector3i(word & 31, (word >> 5) & 31, (word >> 10) & 31)

func _rgb_key(word: int) -> int:
	var rgb := _word_rgb5(word)
	return (rgb.x << 10) | (rgb.y << 5) | rgb.z

func frame_parts(frame_id: int, variant: String = "base") -> Dictionary:
	if not _open:
		return _unsupported("provider is not open")
	var key := "%d:%s" % [frame_id, variant]
	var frames: Dictionary = _hud_assets.get("frames", {})
	if not frames.has(key):
		return _unsupported("requested actor frame variant is absent")
	var frame: Dictionary = frames[key]
	var result: Array = []
	for part: Variant in frame.get("parts", []):
		if not part is Dictionary:
			return _unsupported("actor part record is malformed")
		var color: Dictionary = _load_asset(_hud_assets_root(), part.get("png", {}))
		if color.get("status") != "asset":
			return color
		var stp: Dictionary = _load_asset(_hud_assets_root(), part.get("stp_png", {}))
		if stp.get("status") != "asset":
			return stp
		result.append({"part_index": int(part.get("part_index", -1)),
			"source_xy": [int(part.get("x", 0)), int(part.get("y", 0))],
			"size": [int(part.get("width", 0)), int(part.get("height", 0))],
			"color": color, "stp_mask": stp,
			"source_palette_words": part.get("source_palette_words", [])})
	return {"status": "frame_parts", "frame_id": frame_id, "variant": variant,
		"bounds": frame.get("bounds", []), "origin": frame.get("origin", []), "parts": result}

func checker_tile(season_index: int) -> Dictionary:
	if not _open or season_index < 0 or season_index >= 4:
		return _unsupported("checker source/season unavailable")
	var image := Image.create(32, 32, false, Image.FORMAT_RGBA8)
	var words: Array[int] = []
	for index in range(16): words.append(_checker_palettes.decode_u16(season_index * 32 + index * 2))
	for y in range(32):
		for x in range(32):
			var word: int = words[_checker_indices[y * 128 + x]]
			image.set_pixel(x, y, Color8((word & 31) << 3, ((word >> 5) & 31) << 3,
				((word >> 10) & 31) << 3, 0 if word == 0 else 255))
	return {"status":"asset", "image":image, "season_index":season_index,
		"palette_words":words, "indexed_source":true}

func _load_indexed_checker(root: String, profile: Dictionary) -> String:
	var wanted: Dictionary = {
		"mainrom_offset":0xc000,"upload_vram_rect":[992,384,32,128],"tpage":[960,256],
		"uv_base":[128,128],"sprite_size":[32,32],"source_stride_bytes":64,
		"palette_exe_offset":0x75328,"clut_vram":[160,481]}
	if _checker.get("dimensions") != JSON.parse_string("[128,128]") or _checker.get("source_mapping") != JSON.parse_string(JSON.stringify(wanted)):
		return "checker source mapping differs from native upload/UV"
	var directory: String = root.path_join(profile["resources"]["checker"]["directory"])
	for name in ["indices", "palettes"]:
		var record: Variant = _checker.get("payloads", {}).get(name)
		if not record is Dictionary or record.get("path") != name + ".bin" or record.get("size") != (16384 if name == "indices" else 128):
			return "indexed checker payload descriptor malformed"
		var path := directory.path_join(record["path"])
		if not _external_file(path) or FileAccess.get_size(path) != record["size"] or FileAccess.get_sha256(path) != record.get("sha256"):
			return "indexed checker payload hash/size differs"
		if name == "indices": _checker_indices = FileAccess.get_file_as_bytes(path)
		else: _checker_palettes = FileAccess.get_file_as_bytes(path)
	for index: int in _checker_indices:
		if index > 15: return "checker index exceeds 4bpp range"
	return ""

func calendar_image(year: int, month: int, variant: String) -> Dictionary:
	if not _open:
		return _unsupported("provider is not open")
	for record: Variant in _hud_assets.get("content", {}).get("calendar", []):
		if record is Dictionary and int(record.get("year", -1)) == year \
				and int(record.get("month", -1)) == month and str(record.get("variant", "")) == variant:
			var loaded := _load_asset(_hud_assets_root(), record.get("png", {}))
			if loaded.get("status") != "asset":
				return loaded
			var stp := _load_asset(_hud_assets_root(), record.get("stp_png", {}))
			if stp.get("status") != "asset":
				return stp
			loaded["stp_mask"] = stp
			loaded["palette_words"] = record.get("source_palette_words", [])
			loaded["source_region"] = record.get("source_region", [])
			loaded["variant"] = variant
			loaded["year"] = year
			loaded["month"] = month
			return loaded
	return _unsupported("requested calendar resource is absent")

func glyph_image(raw_two_byte_hex: String, palette_row: int,
		palette_family: int = 0, brightness: int = 128) -> Dictionary:
	if not _open:
		return _unsupported("provider is not open")
	if raw_two_byte_hex.length() != 4:
		return _unsupported("glyph input must be exactly two raw bytes in hex")
	var code_unit := PackedByteArray()
	for offset in [0, 2]:
		var pair := raw_two_byte_hex.substr(offset, 2)
		if not _hex_pair(pair):
			return _unsupported("glyph input contains a non-hex byte")
		code_unit.append(pair.hex_to_int())
	var glyph: Dictionary = _glyphs.get_glyph_image(code_unit, palette_row, palette_family, brightness)
	if glyph.get("status") != "glyph":
		return glyph
	return glyph

func title_bytes() -> PackedByteArray:
	if not _open and _profile.is_empty():
		return PackedByteArray()
	var raw: String = str(_profile.get("title", {}).get("raw_hex", ""))
	var result := PackedByteArray()
	for offset in range(0, raw.length(), 2):
		result.append(raw.substr(offset, 2).hex_to_int())
	return result

func _load_resource_manifests(root: String, profile: Dictionary) -> String:
	var resources: Dictionary = profile.get("resources", {})
	for key: String in ["checker", "prologue_assets", "font"]:
		if not resources.has(key):
			return "profile resource missing: " + key
		var item: Dictionary = resources[key]
		var path := root.path_join(str(item["directory"])).path_join(str(item["manifest"]))
		if not _external_file(path) or FileAccess.get_sha256(path) != str(item["manifest_sha256"]):
			return "resource manifest missing or hash mismatch: " + key
		var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
		if not value is Dictionary:
			return "resource manifest is not an object: " + key
		var expected_schema: String = {"checker": CheckerSchema, "prologue_assets": PrologueHudAssetsSchema,
			"font": FontSchema}[key]
		if str(value.get("schema", "")) != expected_schema:
			return "resource schema mismatch: " + key
		match key:
			"checker": _checker = value
			"prologue_assets": _hud_assets = value
	return ""

func _validate_installed_profile_metadata(profile: Dictionary) -> String:
	if not _json_integer_is(profile.get("version"), 1):
		return "installed profile version must be the supported integer"
	var evidence: Variant = profile.get("source_evidence")
	if not evidence is Array or evidence.size() != INSTALLED_SOURCE_EVIDENCE.size():
		return "installed source evidence does not match the accepted pin set"
	for index in range(INSTALLED_SOURCE_EVIDENCE.size()):
		var record: Variant = evidence[index]
		if not record is Dictionary or record.size() != 2 \
				or typeof(record.get("path", null)) != TYPE_STRING \
				or typeof(record.get("sha256", null)) != TYPE_STRING \
				or record.path != INSTALLED_SOURCE_EVIDENCE[index][0] \
				or record.sha256 != INSTALLED_SOURCE_EVIDENCE[index][1]:
			return "installed source evidence differs from accepted pin index %d" % index
	var title: Variant = profile.get("title")
	if not title is Dictionary:
		return "installed title extent metadata is malformed"
	if not _json_integer_is(title.get("byte_length"), 10) \
			or typeof(title.get("encoding", null)) != TYPE_STRING or title.encoding != "Shift-JIS source bytes" \
			or typeof(title.get("raw_hex", null)) != TYPE_STRING or title.raw_hex.length() != 20 \
			or not _json_integer_is(title.get("source_address"), 0x8014c920) \
			or not _json_integer_is(title.get("source_offset"), 0x1a920) \
			or title.get("source_path") != "CDROM/EXEDIR/NAME_ENT.EXN" \
			or title.get("source_sha256") != "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2" \
			or title.get("source_label") != "D_8014C920" \
			or title.get("source_assembly") != "asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s":
		return "installed title extent does not match the accepted source label"
	for offset in range(0, title.raw_hex.length(), 2):
		if not _hex_pair(title.raw_hex.substr(offset, 2)):
			return "installed title byte extent is not hexadecimal"
	var stage: Variant = profile.get("stage0_contract")
	if not stage is Dictionary:
		return "installed stage0 source contract is malformed"
	if not _integer_array_is(stage.get("canvas"), [320, 240]) \
			or not stage.get("initialization_order") is Array \
			or stage.initialization_order != ["text_system_reset", "message_window_set_title", "hizuke_init", "message_window_init", "dec_bg_reset", "hizuke_disp_switch(1)"]:
		return "installed stage0 canvas or initialization order is unsupported"
	if not _json_integer_is(stage.get("title_timer_before_first_update"), -64) \
			or not _json_integer_is(stage.get("title_timer_after_first_update"), -63):
		return "installed title timing metadata is unsupported"
	var checker: Variant = stage.get("checker")
	var date: Variant = stage.get("date_pillar")
	var message_window: Variant = stage.get("message_window")
	var calendar: Variant = stage.get("calendar_month_art")
	if not checker is Dictionary or checker.get("raw_scroll_inputs") != ["D_800B5BCC", "D_800B5BD0"] \
			or not checker.get("scroll_policy") is String \
			or not date is Dictionary or not _json_integer_is(date.get("frame_id"), 60) \
			or not _integer_array_is(date.get("frame_local_position"), [144, -18]) \
			or not message_window is Dictionary or not _json_integer_is(message_window.get("frame_id"), 62) \
			or not _integer_array_is(message_window.get("frame_local_position"), [0, 75]) \
			or not calendar is Dictionary or not _json_integer_is(calendar.get("month_count"), 36):
		return "installed stage0 resource geometry or selection metadata is unsupported"
	return ""

func _preflight_installed_assets(root: String, profile: Dictionary) -> String:
	var resources: Dictionary = profile["resources"]
	var assets_root: String = root.path_join(str(resources["prologue_assets"]["directory"]))
	var manifest_frames: Variant = _hud_assets.get("frames")
	var selected: Variant = _hud_assets.get("profile")
	var selected_frames: Variant = selected.get("frames") if selected is Dictionary else null
	if not manifest_frames is Dictionary or manifest_frames.size() != INSTALLED_FRAME_IDS.size() \
			or not selected_frames is Array or selected_frames.size() != INSTALLED_FRAME_IDS.size():
		return "installed HUD frame manifest is incomplete"
	for index in range(INSTALLED_FRAME_IDS.size()):
		var frame_id: int = INSTALLED_FRAME_IDS[index]
		var frame_key := "%d:base" % frame_id
		if selected_frames[index] != frame_key:
			return "installed HUD frame selection order differs from the accepted profile"
		var frame: Variant = manifest_frames.get(frame_key)
		if not frame is Dictionary or not frame.get("parts") is Array or frame.parts.is_empty():
			return "installed HUD frame part list is absent"
		for part: Variant in frame.parts:
			if not part is Dictionary:
				return "installed HUD frame part descriptor is malformed"
			for key: String in ["png", "stp_png"]:
				var loaded := _load_asset(assets_root, part.get(key))
				if loaded.get("status") != "asset":
					return "installed HUD frame asset preflight failed"
				loaded.clear()
	var content: Variant = _hud_assets.get("content")
	var calendar_records: Variant = content.get("calendar") if content is Dictionary else null
	if not calendar_records is Array or calendar_records.size() != 72:
		return "installed calendar asset inventory is incomplete"
	for record: Variant in calendar_records:
		if not record is Dictionary:
			return "installed calendar asset descriptor is malformed"
		for key: String in ["png", "stp_png"]:
			var loaded := _load_asset(assets_root, record.get(key))
			if loaded.get("status") != "asset":
				return "installed calendar asset preflight failed"
			loaded.clear()
	return ""

func _integer_array_is(value: Variant, wanted: Array[int]) -> bool:
	if not value is Array or value.size() != wanted.size():
		return false
	for index in range(wanted.size()):
		if not _json_integer_is(value[index], wanted[index]):
			return false
	return true

func _json_integer_is(value: Variant, wanted: int) -> bool:
	var kind := typeof(value)
	if kind == TYPE_INT:
		return int(value) == wanted
	if kind == TYPE_FLOAT:
		return float(value) == float(wanted)
	return false


func _validate_resource_identity(profile: Dictionary) -> String:
	var wanted: Dictionary = profile.get("inputs", {})
	if _hud_assets.get("profile", {}).get("id") != "prologue_stage0_hud_assets.v1":
		return "prologue HUD asset profile identity is unsupported"
	var hud_inputs: Dictionary = _hud_assets.get("provenance", {}).get("inputs", {})
	for pair in [["mainrom", "mainrom"], ["calrom", "calendar"], ["executable", "executable"]]:
		var profile_key: String = pair[0]
		var manifest_key: String = pair[1]
		if not wanted.has(profile_key) or not hud_inputs.has(manifest_key) \
				or str(hud_inputs[manifest_key].get("sha256", "")) != str(wanted[profile_key].get("sha256", "")):
			return "prologue HUD original input identity differs from the profile: " + profile_key
	var checker_inputs: Dictionary = _checker.get("inputs", {})
	for profile_key in ["mainrom", "executable"]:
		if not wanted.has(profile_key) or not checker_inputs.has(profile_key) \
				or str(checker_inputs[profile_key].get("sha256", "")) != str(wanted[profile_key].get("sha256", "")):
			return "checker original input identity differs from the profile: " + profile_key
	return ""

func _verify_source_evidence(root: String, profile: Dictionary) -> String:
	var evidence: Variant = profile.get("source_evidence", [])
	if not evidence is Array or evidence.is_empty():
		return "profile source evidence is empty"
	for record: Variant in evidence:
		if not record is Dictionary:
			return "profile source evidence record is malformed"
		var path := root.path_join(str(record.get("path", "")))
		if not _external_file(path) or FileAccess.get_sha256(path) != str(record.get("sha256", "")):
			return "source witness missing or hash mismatch: " + str(record.get("path", ""))
	return ""

func _load_asset(root: String, record: Variant) -> Dictionary:
	if not record is Dictionary or not record.has("path") or not record.has("sha256"):
		return _unsupported("asset record is malformed")
	var rel: String = str(record["path"])
	if rel.is_empty() or rel.begins_with("/") or rel.contains("..") or rel.contains("\\"):
		return _unsupported("asset locator is not a safe relative path")
	var path := root.path_join(rel)
	if not _external_file(path):
		return _unsupported("asset is missing, linked, or in the runtime package")
	if FileAccess.get_size(path) != int(record.get("size", -1)) or FileAccess.get_sha256(path) != str(record["sha256"]):
		return _unsupported("asset size or hash mismatch")
	var image := Image.new()
	if image.load(path) != OK or image.get_width() != int(record.get("width", -1)) \
			or image.get_height() != int(record.get("height", -1)):
		return _unsupported("asset image decode or dimension mismatch")
	return {"status": "asset", "image": image, "sha256": record["sha256"],
		"size": int(record["size"]), "width": image.get_width(), "height": image.get_height(),
		"source_path": path}

func _season_for_month(month: int) -> int:
	if month in [3, 4, 5]:
		return 0
	if month in [6, 7, 8]:
		return 1
	if month in [9, 10, 11]:
		return 2
	if month in [1, 2, 12]:
		return 3
	return -1

func _hud_assets_root() -> String:
	return _repo_root.path_join(str(_profile["resources"]["prologue_assets"]["directory"]))

func _checker_root() -> String:
	return _repo_root.path_join(str(_profile["resources"]["checker"]["directory"]))

func _external_file(path: String) -> bool:
	return ExternalPath.is_external_file(path)

func _safe_relative_path(value: String) -> bool:
	if value.is_empty() or value.begins_with("/") or value.contains("\\") or value.contains(":"):
		return false
	for component: String in value.split("/", true):
		if component.is_empty() or component == "." or component == "..":
			return false
	return value.simplify_path() == value

func _bundle_file(root: String, relative_path: String) -> bool:
	if not _safe_relative_path(relative_path):
		return false
	var path := root.path_join(relative_path)
	return path.begins_with(root.trim_suffix("/") + "/") and _external_file(path)

func _hex_pair(value: String) -> bool:
	if value.length() != 2:
		return false
	for index in range(2):
		var c := value.to_lower().unicode_at(index)
		if not ((c >= 48 and c <= 57) or (c >= 97 and c <= 102)):
			return false
	return true

func _close() -> void:
	_repo_root = ""
	_profile.clear()
	_hud_assets.clear()
	_checker.clear()
	_checker_indices.clear()
	_checker_palettes.clear()
	_glyphs = null
	_open = false

static func _unsupported(diagnostic: String) -> Dictionary:
	return {"status": "unsupported", "diagnostic": diagnostic}

static func _is_sha256(value: String) -> bool:
	if value.length() != 64:
		return false
	for index in range(value.length()):
		var c := value.to_lower().unicode_at(index)
		if not ((c >= 48 and c <= 57) or (c >= 97 and c <= 102)):
			return false
	return true
