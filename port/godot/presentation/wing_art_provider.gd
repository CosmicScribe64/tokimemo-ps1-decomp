extends RefCounted
## T-39220: decorative wing art from the optional wing_art pack
## (tools/port/wing_art_content.py). A wing provider for WideFrameComposer, and
## the source of the window backdrop that NativeDisplayHost draws behind the
## frame (wings, side bars and the top/bottom bands).
##
## Presentation only. The provider never reads or writes game state or the
## game RNG: the month, scene and location arrive in the context dictionary,
## and motion follows the presentation tick the caller passes in (30 Hz).
## Particle motion uses the pack's integer tables and a fixed-seed LCG baked
## into the pack, so one (style, tick, side width, flags) always yields the
## same bytes. The composer copies the native 320x240 Image last, so nothing
## here can reach the centre.
##
## Modes (display.wing_art): off | seasonal | scene | fixed:<style>.
## seasonal follows the in-game month; scene looks up the pack's override table
## (stage, background, location, mode, tag, class; user rules first) and falls
## back to seasonal; fixed shows one style (a season or a location). Each
## scene class may override the mode (mode_for_scene). A missing or damaged pack makes
## every call fail softly: render() returns null and fill_wings() an error, so
## the host shows black wings.
##
## Context keys: month (1..12), scene (mode id or tag, as SceneAperturePolicy),
## stage, background (BS name or path), location, tick (presentation ticks),
## visible_side (logical columns from the picture edge to the window edge),
## reduce_motion, reduce_flashes.

const Policy = preload("res://presentation/scene_aperture_policy.gd")

const SCHEMA := "tokimemo.wing-art.v1"
const OVERRIDES_SCHEMA := "tokimemo.wing-art.overrides.v1"
const MODES := ["off", "seasonal", "scene"]
const FIXED_STYLES := ["yozakura", "spring_day", "summer", "autumn", "winter",
	"school", "park", "town", "seaside", "legend_tree"]
## Per scene type side art (display.wing_art_<type>): "auto" follows
## display.wing_art; any mode replaces it for scenes of that class ("off"
## means black sides there). Only used while display.wing_art is not "off".
const SCENE_TYPE_KEYS := {"A": "wing_art_command", "B": "wing_art_story", "C": "wing_art_other"}
const MATCH_KEYS := ["stage", "background_prefix", "location", "mode", "tag", "class"]
const SEASON_OF_MONTH := {1: "winter", 2: "winter", 3: "spring", 4: "spring", 5: "spring", 6: "summer",
	7: "summer", 8: "summer", 9: "autumn", 10: "autumn", 11: "autumn", 12: "winter"}
const DEFAULT_MONTH := 4
const TICK_HZ := 30
## Tick shown when motion is reduced: one fixed frame, no fireworks.
const STILL_TICK := 44
## Bayer 4x4 order for the dissolve between styles (16 levels).
const BAYER := [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]
const FADE_TICKS_PER_LEVEL := 2
const MAX_PACK_BYTES := 64 * 1024 * 1024
const MAX_MANIFEST_BYTES := 4 * 1024 * 1024
const MAX_USER_RULES := 64
const FIREWORK_MARGIN := 4

var error := ""
var pack_root := ""
var manifest: Dictionary = {}
var geometry := {"plate_width": 720, "plate_height": 300, "aperture_x": 200, "aperture_y": 30}
## Last rendered backdrop (RGBA8, plate size) or null for black.
var backdrop: Image
var last_style := ""
## Count of fills that failed because no backdrop could be rendered.
var fallbacks := 0
var _ready := false
var _plates: Dictionary = {}
var _atlases: Dictionary = {}
var _rules: Array = []
var _user_rules: Array = []
var _default_style := "seasonal"
var _sin: PackedInt32Array
var _env: PackedInt32Array
var _render_key := ""
## Dissolve state: the style shown before the current one and the tick the
## change was first seen.
var _shown_style := ""
var _from_style := ""
var _fade_start := -1
var _masks: Dictionary = {}

# ------------------------------------------------------------------ pack

## Opens and authenticates a pack directory: schema, every file's size and
## SHA-256 against the manifest inventory, plates and atlases decodable.
func open_pack(root: String) -> bool:
	_ready = false; error = ""; manifest = {}; _plates = {}; _atlases = {}; _rules = []
	_render_key = ""; backdrop = null; _masks = {}; _shown_style = ""; _from_style = ""; _fade_start = -1
	pack_root = root
	if root.is_empty() or not DirAccess.dir_exists_absolute(root):
		return _fail("wing art pack is not installed")
	var path := root.path_join("manifest.json")
	if not FileAccess.file_exists(path) or FileAccess.get_size(path) > MAX_MANIFEST_BYTES:
		return _fail("wing art manifest is missing or too large")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or parsed.get("schema") != SCHEMA or int(parsed.get("version", 0)) != 1:
		return _fail("wing art manifest schema differs")
	var files: Variant = parsed.get("files")
	if not files is Dictionary or files.is_empty():
		return _fail("wing art inventory is missing")
	var total := 0
	for relative: Variant in files:
		var record: Variant = files[relative]
		if not relative is String or not _safe_relative(relative) or not record is Dictionary:
			return _fail("wing art inventory has an unsafe entry")
		var file_path: String = root.path_join(relative)
		total += int(record.get("size", -1))
		if total > MAX_PACK_BYTES or not FileAccess.file_exists(file_path) or FileAccess.get_size(file_path) != int(record.get("size", -1)) or FileAccess.get_sha256(file_path) != str(record.get("sha256", "")):
			return _fail("wing art file is missing or changed: " + str(relative))
	var geo: Variant = parsed.get("geometry")
	if not geo is Dictionary or int(geo.get("plate_width", 0)) < 400 or int(geo.get("plate_height", 0)) < 240:
		return _fail("wing art geometry is malformed")
	geometry = {"plate_width": int(geo.plate_width), "plate_height": int(geo.plate_height),
		"aperture_x": int(geo.aperture_x), "aperture_y": int(geo.aperture_y)}
	var tables: Variant = parsed.get("tables")
	if not tables is Dictionary or not tables.get("sin_q12") is Array or tables.sin_q12.size() != 64 or not tables.get("envelope_q8") is Array or tables.envelope_q8.size() != 64:
		return _fail("wing art motion tables are malformed")
	_sin = PackedInt32Array(tables.sin_q12); _env = PackedInt32Array(tables.envelope_q8)
	if not parsed.get("styles") is Dictionary or parsed.styles.is_empty():
		return _fail("wing art styles are missing")
	for style: Variant in parsed.styles:
		var spec: Variant = parsed.styles[style]
		if not spec is Dictionary or not files.has(spec.get("plate", "")):
			return _fail("wing art style has no inventoried plate: " + str(style))
	for season: String in ["spring", "summer", "autumn", "winter"]:
		if not parsed.get("seasons") is Dictionary or not parsed.styles.has(parsed.seasons.get(season, "")):
			return _fail("wing art season map is incomplete")
	if not parsed.get("atlases") is Dictionary:
		return _fail("wing art atlases are missing")
	for key: Variant in parsed.atlases:
		var atlas: Variant = parsed.atlases[key]
		if not atlas is Dictionary or not files.has(atlas.get("path", "")):
			return _fail("wing art atlas is not inventoried: " + str(key))
		var image := Image.load_from_file(root.path_join(atlas.path))
		if image == null or image.is_empty():
			return _fail("wing art atlas does not decode: " + str(key))
		image.convert(Image.FORMAT_RGBA8)
		_atlases[key] = {"data": image.get_data(), "width": image.get_width(), "height": image.get_height(), "spec": atlas}
	var overrides_rel: Variant = parsed.get("overrides")
	if not overrides_rel is String or not files.has(overrides_rel):
		return _fail("wing art override table is missing")
	var table: Variant = JSON.parse_string(FileAccess.get_file_as_string(root.path_join(overrides_rel)))
	manifest = parsed
	var rules := validate_overrides(table, parsed.styles.keys())
	if rules.is_empty() and not (table is Dictionary and table.get("rules") is Array and table.rules.is_empty()):
		manifest = {}
		return _fail("wing art override table is malformed")
	_rules = rules
	_default_style = table.default
	_ready = true
	return true

func is_ready() -> bool:
	return _ready

## A second provider over the same opened pack with its own render and
## dissolve state (for settings previews, so they never disturb the host).
func spawn() -> RefCounted:
	var other: RefCounted = get_script().new()
	if not _ready:
		other.error = error
		return other
	other.pack_root = pack_root; other.manifest = manifest; other.geometry = geometry.duplicate()
	other._plates = _plates; other._atlases = _atlases; other._rules = _rules; other._user_rules = _user_rules
	other._default_style = _default_style; other._sin = _sin; other._env = _env; other._ready = true
	return other

## The installed wing_art role, or "" when the install has none. This
## validates the whole installed pack, so callers do it once, lazily.
static func discover_pack_path() -> String:
	var store_script: Script = load("res://services/installed_pack_store.gd")
	if store_script == null:
		return ""
	var store: RefCounted = store_script.new()
	var result: Dictionary = store.check_installed()
	var profile: Variant = result.get("launch_profile")
	if profile is Dictionary and profile.get("paths") is Dictionary:
		return str(profile.paths.get("wing_art", ""))
	return ""

func styles() -> Array:
	return manifest.styles.keys() if _ready else []

func style_label(style: String) -> String:
	return str(manifest.styles[style].get("label", style)) if _ready and manifest.styles.has(style) else style

## Optional user rules (same schema), tried before the pack's own rules. An
## invalid table is rejected whole and the previous user rules stay.
func set_user_overrides(table: Variant) -> bool:
	if not _ready:
		return false
	if table == null:
		_user_rules = []; _render_key = ""; return true
	var rules := validate_overrides(table, manifest.styles.keys())
	if rules.is_empty() or rules.size() > MAX_USER_RULES:
		error = "user wing art overrides are malformed"
		return false
	_user_rules = rules; _render_key = ""
	return true

## Returns the rule list, or [] when the table is malformed.
static func validate_overrides(table: Variant, style_ids: Array) -> Array:
	if not table is Dictionary or table.get("schema") != OVERRIDES_SCHEMA or not table.get("rules") is Array:
		return []
	var allowed := style_ids.duplicate(); allowed.append_array(["seasonal", "black"])
	if table.get("default") not in allowed or table.rules.size() > 256:
		return []
	var rules: Array = []
	for rule: Variant in table.rules:
		if not rule is Dictionary or rule.size() != 2 or not rule.get("match") is Dictionary or rule.match.size() != 1 or rule.get("style") not in allowed:
			return []
		var key: Variant = rule.match.keys()[0]
		var value: Variant = rule.match[key]
		if key not in MATCH_KEYS or not value is String or value.is_empty() or value.length() > 64:
			return []
		rules.append({"key": key, "value": value, "style": rule.style})
	return rules

# ------------------------------------------------------------------ modes

## display.wing_art from effective settings or a flat dictionary.
static func mode_preference(prefs: Dictionary) -> String:
	var value: Variant = prefs.get("wing_art")
	var display: Variant = prefs.get("display")
	if display is Dictionary and display.has("wing_art"):
		value = display.wing_art
	return value if value is String and value in all_modes() else "off"

## The mode for one scene: display.wing_art, replaced by the scene type's
## own choice when that is not "auto". Wing art stays off while
## display.wing_art is "off", whatever the per-type choices say.
static func mode_for_scene(prefs: Dictionary, scene: Variant) -> String:
	var base := mode_preference(prefs)
	if base == "off" or scene == null:
		return base
	var key: String = SCENE_TYPE_KEYS.get(Policy.classify(scene), "")
	var display: Variant = prefs.get("display")
	var value: Variant = display.get(key) if display is Dictionary else prefs.get(key)
	return value if value is String and value in all_modes() else base

static func scene_type_values() -> Array:
	var result := ["auto"]
	result.append_array(all_modes())
	return result

static func all_modes() -> Array:
	var result := MODES.duplicate()
	for style: String in FIXED_STYLES:
		result.append("fixed:" + style)
	return result

static func accessibility_flag(prefs: Dictionary, key: String) -> bool:
	var access: Variant = prefs.get("accessibility")
	return access is Dictionary and access.get(key) == true

static func season_for_month(month: Variant) -> String:
	var value := int(month) if (month is int or month is float) else DEFAULT_MONTH
	return SEASON_OF_MONTH.get(value, SEASON_OF_MONTH[DEFAULT_MONTH])

static func _background_id(value: Variant) -> String:
	if not value is String:
		return ""
	var name: String = value.get_file().to_upper()
	return name.trim_suffix(".BS").trim_suffix(".PNG")

## The style a mode shows for a context: a style id, or "black".
func resolve_style(mode: String, context: Dictionary) -> String:
	if not _ready or mode == "off" or mode not in all_modes():
		return "black"
	var season := season_for_month(context.get("month", DEFAULT_MONTH))
	var seasonal: String = manifest.seasons[season]
	if mode == "seasonal":
		return seasonal
	if mode.begins_with("fixed:"):
		var fixed := mode.substr(6)
		return fixed if manifest.styles.has(fixed) else seasonal
	var found := _lookup(context)
	if found == "seasonal" or found.is_empty():
		return seasonal
	return found if found == "black" or manifest.styles.has(found) else seasonal

func _lookup(context: Dictionary) -> String:
	var scene: Variant = context.get("scene")
	var facts := {"stage": str(context.get("stage", "")), "background_prefix": _background_id(context.get("background")),
		"location": str(context.get("location", "")), "class": Policy.classify(scene) if scene != null else ""}
	var tag := ""
	var mode_id := -1
	if scene is Dictionary:
		tag = str(scene.get("tag", "")).to_upper()
		mode_id = _mode_int(scene.get("mode"))
	elif scene is String and scene.to_upper() in Policy.CLASS_B_TAGS:
		tag = scene.to_upper()
	else:
		mode_id = _mode_int(scene)
	for key: String in MATCH_KEYS:
		for rules: Array in [_user_rules, _rules]:
			for rule: Dictionary in rules:
				if rule.key != key:
					continue
				var hit := false
				match key:
					"background_prefix": hit = not facts.background_prefix.is_empty() and facts.background_prefix.begins_with(rule.value.to_upper())
					"mode": hit = mode_id >= 0 and mode_id == _mode_int(rule.value)
					"tag": hit = not tag.is_empty() and tag == rule.value.to_upper()
					"class": hit = scene != null and facts["class"] == rule.value.to_upper()
					_: hit = not str(facts[key]).is_empty() and facts[key] == rule.value
				if hit:
					return rule.style
	return _default_style

static func _mode_int(value: Variant) -> int:
	if value is int:
		return value
	if value is String:
		var text: String = value.strip_edges().to_lower()
		if text.begins_with("0x") and text.length() > 2 and text.substr(2).is_valid_hex_number():
			return text.substr(2).hex_to_int()
		if text.is_valid_int():
			return text.to_int()
	return -1

# ------------------------------------------------------------------ render

## Tick of a presentation clock in microseconds (30 Hz).
static func tick_for_usec(usec: int) -> int:
	return int(usec * TICK_HZ / 1000000)

## Renders the backdrop for a mode and context. Returns null (and keeps the
## previous error) when the mode is off, the style is "black" or the pack is
## not usable. The result is cached for identical inputs.
func render(mode: String, context: Dictionary) -> Image:
	if not _ready:
		if error.is_empty(): error = "wing art pack is not open"
		backdrop = null; return null
	var style := resolve_style(mode, context)
	var reduce_motion: bool = context.get("reduce_motion") == true
	var tick: int = STILL_TICK if reduce_motion else maxi(int(context.get("tick", 0)), 0)
	var season := season_for_month(context.get("month", DEFAULT_MONTH))
	var side: int = clampi(int(context.get("visible_side", 80)), 0, 4096)
	var no_fireworks: bool = reduce_motion or context.get("reduce_flashes") == true
	if style != _shown_style:
		# A change of style starts a dissolve from the style shown until now.
		_from_style = _shown_style
		_shown_style = style
		_fade_start = tick
	var level := 16
	if not reduce_motion and not _from_style.is_empty() and _fade_start >= 0:
		level = clampi((tick - _fade_start) / FADE_TICKS_PER_LEVEL + 1, 0, 16)
	if reduce_motion or level >= 16:
		_from_style = ""; _fade_start = -1; level = 16
	var key := "%s|%s|%d|%d|%s|%d|%s" % [style, _from_style, tick, side, season, level, no_fireworks]
	if key == _render_key and (backdrop != null or style == "black"):
		return backdrop
	_render_key = key
	last_style = style
	if style == "black":
		backdrop = null if _from_style.is_empty() or _from_style == "black" else _dissolve(_frame(_from_style, season, -1, side, true), null, level)
		return backdrop
	var image := _frame(style, season, tick, side, no_fireworks)
	if image == null:
		backdrop = null; return null
	if level < 16 and not _from_style.is_empty():
		var from_image: Image = null if _from_style == "black" else _frame(_from_style, season, -1, side, true)
		image = _dissolve(from_image, image, level)
	backdrop = image
	return backdrop

## One style's frame: plate plus particles at `tick` (tick < 0: plate only).
func _frame(style: String, season: String, tick: int, side: int, no_fireworks: bool) -> Image:
	var plate := _plate(style)
	if plate == null:
		return null
	var image := plate.duplicate()
	if tick < 0:
		return image
	var spec: Dictionary = manifest.styles[style].get("particles", {})
	if spec.get("kind") == "season":
		spec = manifest.get("season_particles", {}).get(season, {})
	match spec.get("kind"):
		"drift":
			var data: PackedByteArray = image.get_data()
			_draw_drifters(data, image.get_width(), image.get_height(), spec, tick)
			image.set_data(image.get_width(), image.get_height(), false, Image.FORMAT_RGBA8, data)
		"fireworks":
			if not no_fireworks:
				var data: PackedByteArray = image.get_data()
				_draw_fireworks(data, image.get_width(), image.get_height(), spec, tick, side)
				image.set_data(image.get_width(), image.get_height(), false, Image.FORMAT_RGBA8, data)
	return image

func _plate(style: String) -> Image:
	if _plates.has(style):
		return _plates[style]
	if not manifest.styles.has(style):
		return null
	var image := Image.load_from_file(pack_root.path_join(manifest.styles[style].plate))
	if image == null or image.is_empty() or image.get_width() != geometry.plate_width or image.get_height() != geometry.plate_height:
		error = "wing art plate does not decode: " + style
		return null
	image.convert(Image.FORMAT_RGBA8)
	_plates[style] = image
	return image

## Dithered dissolve (PS1-style ordered pattern): pixels whose Bayer rank is
## below `level` show `to`, the rest show `from`. null means black.
func _dissolve(from: Image, to: Image, level: int) -> Image:
	var width: int = geometry.plate_width
	var height: int = geometry.plate_height
	var result := from.duplicate() if from != null else Image.create(width, height, false, Image.FORMAT_RGBA8)
	if from == null: result.fill(Color(0, 0, 0, 1))
	var target := to if to != null else Image.create(width, height, false, Image.FORMAT_RGBA8)
	if to == null: target.fill(Color(0, 0, 0, 1))
	if level <= 0:
		return result
	result.blit_rect_mask(target, _mask(level), Rect2i(0, 0, width, height), Vector2i.ZERO)
	return result

func _mask(level: int) -> Image:
	if _masks.has(level):
		return _masks[level]
	var width: int = geometry.plate_width
	var height: int = geometry.plate_height
	var tile := Image.create(4, 4, false, Image.FORMAT_RGBA8)
	for y: int in 4:
		for x: int in 4:
			tile.set_pixel(x, y, Color(1, 1, 1, 1 if BAYER[y][x] < level else 0))
	var mask := Image.create(width, height, false, Image.FORMAT_RGBA8)
	mask.blit_rect(tile, Rect2i(0, 0, 4, 4), Vector2i.ZERO)
	var w := 4
	while w < width:
		mask.blit_rect(mask, Rect2i(0, 0, w, 4), Vector2i(w, 0)); w *= 2
	var h := 4
	while h < height:
		mask.blit_rect(mask, Rect2i(0, 0, width, h), Vector2i(0, h)); h *= 2
	if _masks.size() > 20:
		_masks.clear()
	_masks[level] = mask
	return mask

static func _posmod(a: int, b: int) -> int:
	var r := a % b
	return r + b if r < 0 else r

## Integer division truncating toward zero (matches tools/port tdiv).
static func _tdiv(a: int, b: int) -> int:
	return -((-a) / b) if a < 0 else a / b

## Drifting petals/leaves. Per item at tick t (integers, logical px):
##   u = (t - t0) mod loop; alpha = env[u*64/loop] * cap >> 8
##   off = ox + tdiv(drift*u, loop) + (amp * sin[(cyc*u*64/loop + ph) & 63] >> 12)
##   y = y0 + fall*u/loop; frame = (f0 + t/rate) % frames
## Left sprites end `off` px left of the picture; right ones start `off` px
## right of it, mirrored. Colour: src5 * k >> 8 blended by alpha (Q8).
func _draw_drifters(data: PackedByteArray, width: int, height: int, spec: Dictionary, tick: int) -> void:
	var atlas: Dictionary = _atlases.get(spec.get("atlas", ""), {})
	if atlas.is_empty():
		return
	var frames: Array = atlas.spec.frames
	var loop: int = maxi(int(spec.get("loop", 120)), 1)
	var rate: int = maxi(int(spec.get("rate", 10)), 1)
	var ax: int = geometry.aperture_x
	var adata: PackedByteArray = atlas.data
	var aw: int = atlas.width
	for item: Dictionary in spec.get("items", []):
		var u := _posmod(tick - int(item.t0), loop)
		var alpha: int = (_env[u * 64 / loop] * int(item.cap)) >> 8
		if alpha <= 0:
			continue
		var offset: int = int(item.ox) + _tdiv(int(item.drift) * u, loop) + ((int(item.amp) * _sin[(int(item.cyc) * u * 64 / loop + int(item.ph)) & 63]) >> 12)
		var y0: int = int(item.y0) + int(item.fall) * u / loop
		var rect: Array = frames[(int(item.f0) + tick / rate) % frames.size()]
		var fw: int = int(rect[2])
		var fh: int = int(rect[3])
		var rx: int = int(rect[0])
		var ry: int = int(rect[1])
		var mirrored: bool = int(item.side) == 1
		var x0: int = ax + 320 + offset if mirrored else ax - offset - fw
		var k: int = int(item.k)
		for sy: int in fh:
			var py := y0 + sy
			if py < 0 or py >= height:
				continue
			for sx: int in fw:
				var px := x0 + sx
				if px < 0 or px >= width:
					continue
				var ax_col: int = rx + (fw - 1 - sx if mirrored else sx)
				var src := ((ry + sy) * aw + ax_col) * 4
				if adata[src + 3] == 0:
					continue
				var dst := (py * width + px) * 4
				for c: int in 3:
					var s5: int = ((adata[src + c] >> 3) * k) >> 8
					var d5: int = data[dst + c] >> 3
					var o5: int = (d5 * (256 - alpha) + s5 * alpha + 128) >> 8
					data[dst + c] = (o5 << 3) | (o5 >> 2)

## HANABI bursts, additive: out5 = min(31, dst5 + src5 * a * strength >> 16).
## Each burst sits in the middle of the visible side area of its side and is
## drawn only with the largest radius that fits; none fits below ~69 columns.
func _draw_fireworks(data: PackedByteArray, width: int, height: int, spec: Dictionary, tick: int, side: int) -> void:
	var atlas_key := ""
	var radius := 0
	for candidate: int in [58, 30]:
		if _atlases.has("fireworks-%d" % candidate) and side >= 2 * candidate + 1 + 2 * FIREWORK_MARGIN:
			atlas_key = "fireworks-%d" % candidate; radius = candidate; break
	if atlas_key.is_empty():
		return
	var atlas: Dictionary = _atlases[atlas_key]
	var adata: PackedByteArray = atlas.data
	var aw: int = atlas.width
	var size := 2 * radius + 1
	var loop: int = maxi(int(spec.get("loop", 240)), 1)
	var step_ticks: int = maxi(int(spec.get("step_ticks", 4)), 1)
	var strength: int = clampi(int(spec.get("strength_q8", 92)), 0, 102)
	var ax: int = geometry.aperture_x
	var half := side / 2
	for burst: Dictionary in spec.get("bursts", []):
		var step := _posmod(tick - int(burst.t0), loop) / step_ticks
		if step > 15:
			continue
		var cx: int = ax - half if int(burst.side) == 0 else ax + 319 + half
		var x0 := cx - radius
		var y0: int = int(burst.y) - radius
		for sy: int in size:
			var py := y0 + sy
			if py < 0 or py >= height:
				continue
			for sx: int in size:
				var px := x0 + sx
				if px < 0 or px >= width:
					continue
				var src := (sy * aw + step * size + sx) * 4
				var a: int = adata[src + 3]
				if a == 0:
					continue
				var dst := (py * width + px) * 4
				for c: int in 3:
					var add: int = ((adata[src + c] >> 3) * a * strength) >> 16
					var o5: int = mini(31, (data[dst + c] >> 3) + add)
					data[dst + c] = (o5 << 3) | (o5 >> 2)

# ------------------------------------------------------------------ composer

## WideFrameComposer provider. Renders (or reuses) the backdrop for
## context.wing_art_mode and copies its wing columns; never the centre.
func fill_wings(target: Image, wing_width: int, context: Dictionary) -> String:
	if target == null or target.get_height() != 240 or target.get_width() != 320 + 2 * wing_width or wing_width < 0:
		return "target does not match the wing width"
	if wing_width == 0:
		return ""
	var mode: String = str(context.get("wing_art_mode", "seasonal"))
	var image := render(mode, context)
	if image == null:
		if last_style == "black" and _ready:
			return ""  # the composer's black fill is the requested result
		fallbacks += 1
		return error if not error.is_empty() else "wing art unavailable"
	var ax: int = geometry.aperture_x
	var ay: int = geometry.aperture_y
	var source := image
	if image.get_format() != target.get_format():
		source = image.duplicate(); source.convert(target.get_format())
	var span := mini(wing_width, ax)
	target.blit_rect(source, Rect2i(ax - span, ay, span, 240), Vector2i(wing_width - span, 0))
	target.blit_rect(source, Rect2i(ax + 320, ay, mini(wing_width, geometry.plate_width - ax - 320), 240), Vector2i(wing_width + 320, 0))
	return ""

## A preview of a mode around a native-size centre at logical size
## (width x 270, the 16:9 window at 1080p is 480 x 270). centre may be null.
func preview(mode: String, context: Dictionary, centre: Image, width: int = 480) -> Image:
	var height := 270
	var out := Image.create(width, height, false, Image.FORMAT_RGBA8)
	out.fill(Color(0, 0, 0, 1))
	var ctx := context.duplicate()
	ctx["visible_side"] = (width - 320) / 2
	var image := render(mode, ctx)
	var ox: int = (width - 320) / 2
	var oy: int = (height - 240) / 2
	if image != null:
		var gx: int = geometry.aperture_x - ox
		var gy: int = geometry.aperture_y - oy
		out.blit_rect(image, Rect2i(gx, gy, width, height), Vector2i.ZERO)
	if centre != null and centre.get_size() == Vector2i(320, 240):
		var c := centre
		if c.get_format() != Image.FORMAT_RGBA8:
			c = centre.duplicate(); c.convert(Image.FORMAT_RGBA8)
		out.blit_rect(c, Rect2i(0, 0, 320, 240), Vector2i(ox, oy))
	else:
		out.fill_rect(Rect2i(ox, oy, 320, 240), Color(0, 0, 0, 1))
	return out

# ------------------------------------------------------------------ helpers

func _fail(message: String) -> bool:
	error = message
	_ready = false
	return false

static func _safe_relative(path: String) -> bool:
	if path.is_empty() or path.begins_with("/") or path.contains("\\") or path.contains(":"):
		return false
	for part: String in path.split("/"):
		if part.is_empty() or part == "." or part == "..":
			return false
	return true
