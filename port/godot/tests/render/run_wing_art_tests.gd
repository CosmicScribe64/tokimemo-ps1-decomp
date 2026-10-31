extends SceneTree
## T-39220: WingArtProvider and its NativeDisplayHost wiring.
## Original-free: builds a synthetic wing_art pack under user://. Set
## WING_ART_PACK to a real pack directory to repeat the pack-dependent checks
## against it (the real pack is generated from the user's disc).
##
## Covers: the native Image is byte-identical in every mode and width
## (fidelity matrix), deterministic output for (style, tick, side), reduce
## motion gives one static frame, fireworks stay inside the visible side area,
## season and scene resolution with user overrides, the dissolve, and the
## fallback to black wings when the pack is missing or damaged; per scene
## type modes; and the read-only scene feed across a real calendar month
## change (rules/calendar.gd advance_day).

const WingArt = preload("res://presentation/wing_art_provider.gd")
const Composer = preload("res://presentation/wide_frame_composer.gd")
const Host = preload("res://presentation/native_display_host.gd")
const Layout = preload("res://presentation/native_frame_layout.gd")
const Feed = preload("res://presentation/wing_art_scene_feed.gd")
const RuleFixture = preload("res://tests/rules/run_rule_tests.gd")
const Calendar = preload("res://rules/calendar.gd")
const WIDTHS := [320, 322, 384, 426, 500, 560]

var _checks := 0
var _failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

static func _hash(image: Image) -> String:
	var hashing := HashingContext.new()
	hashing.start(HashingContext.HASH_SHA256)
	hashing.update(var_to_bytes([image.get_width(), image.get_height(), image.get_format()]))
	hashing.update(image.get_data())
	return hashing.finish().hex_encode()

## A native frame with a pattern in every channel, as hud_compositor renders.
static func _native(seed: int) -> Image:
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	for y: int in 240:
		for x: int in 320:
			image.set_pixel(x, y, Color8((x * 7 + seed) & 255, (y * 5 + seed * 3) & 255, (x ^ y) & 255, 255))
	return image

# ------------------------------------------------------------------ pack

func _write_json(path: String, value: Variant) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(value, "\t", true) + "\n")
	file.close()

## Synthetic pack with the real schema: 720x300 plates (a distinct ramp per
## style), a 2-frame petal atlas, fireworks atlases and the default rules.
func _make_pack(root: String) -> String:
	DirAccess.make_dir_recursive_absolute(root.path_join("plates"))
	DirAccess.make_dir_recursive_absolute(root.path_join("atlas"))
	var styles := {}
	var ids := ["yozakura", "spring_day", "summer", "autumn", "winter", "school", "park", "town", "seaside", "legend_tree"]
	for index: int in ids.size():
		var plate := Image.create(720, 300, false, Image.FORMAT_RGB8)
		for y: int in 300:
			for x: int in 720:
				var v := ((x + y + index * 9) % 32)
				plate.set_pixel(x, y, Color8((v << 3) | (v >> 2), ((index * 3) << 3), (((31 - v) << 3) | ((31 - v) >> 2))))
		var rel := "plates/%s.png" % ids[index]
		plate.save_png(root.path_join(rel))
		var particles: Dictionary = {"kind": "none"}
		if ids[index] in ["yozakura", "spring_day"]:
			particles = {"kind": "drift", "atlas": "petals", "loop": 120, "rate": 10, "items": [
				{"side": 0, "ox": 10, "y0": 60, "fall": 60, "drift": 8, "amp": 3, "cyc": 1, "ph": 5, "t0": 0, "f0": 0, "k": 200, "cap": 205, "depth": 10},
				{"side": 1, "ox": 30, "y0": 120, "fall": 50, "drift": -6, "amp": 2, "cyc": 2, "ph": 40, "t0": 30, "f0": 1, "k": 160, "cap": 205, "depth": 90}]}
		elif ids[index] == "summer":
			particles = {"kind": "fireworks", "loop": 240, "step_ticks": 4, "strength_q8": 92,
				"bursts": [{"side": 0, "t0": 0, "y": 82}, {"side": 1, "t0": 120, "y": 82}]}
		elif ids[index] in ["school", "park", "legend_tree"]:
			particles = {"kind": "season"}
		styles[ids[index]] = {"label": ids[index], "kind": "seasonal" if index < 5 else "location", "plate": rel,
			"source": "synthetic", "outdoor": true, "particles": particles}
	var petals := Image.create(8, 4, false, Image.FORMAT_RGBA8)
	for y: int in 4:
		for x: int in 8:
			petals.set_pixel(x, y, Color8(248, 160 + x * 8, 200, 255 if (x + y) % 3 != 0 else 0))
	petals.save_png(root.path_join("atlas/petals.png"))
	var atlases := {"petals": {"path": "atlas/petals.png", "frames": [[0, 0, 4, 4], [4, 0, 4, 4]]},
		"leaves": {"path": "atlas/petals.png", "frames": [[0, 0, 4, 4], [4, 0, 4, 4]]}}
	for radius: int in [30, 58]:
		var size := 2 * radius + 1
		var sheet := Image.create(size * 16, size, false, Image.FORMAT_RGBA8)
		for step: int in 16:
			for y: int in size:
				for x: int in size:
					var d := Vector2(x - radius, y - radius).length()
					if d <= radius and (x + y + step) % 5 == 0:
						sheet.set_pixel(step * size + x, y, Color8(248, 200, 64, 255))
		var rel := "atlas/fireworks-%d.png" % radius
		sheet.save_png(root.path_join(rel))
		atlases["fireworks-%d" % radius] = {"path": rel, "radius": radius, "size": size, "steps": 16}
	var overrides := {"schema": WingArt.OVERRIDES_SCHEMA, "default": "seasonal", "rules": [
		{"match": {"stage": "prologue"}, "style": "legend_tree"},
		{"match": {"background_prefix": "D_10_"}, "style": "park"},
		{"match": {"location": "school"}, "style": "school"},
		{"match": {"class": "A"}, "style": "school"}]}
	_write_json(root.path_join("overrides.json"), overrides)
	var sin_table: Array = []
	var env_table: Array = []
	for k: int in 64:
		sin_table.append(int(floor(sin(TAU * k / 64.0) * 4096.0 + 0.5)))
		env_table.append(int(floor(pow(sin(PI * k / 64.0), 0.8) * 256.0 + 0.5)))
	var files := {}
	for rel: String in ["overrides.json", "atlas/petals.png", "atlas/fireworks-30.png", "atlas/fireworks-58.png"]:
		files[rel] = {"sha256": FileAccess.get_sha256(root.path_join(rel)), "size": FileAccess.get_file_as_bytes(root.path_join(rel)).size()}
	for id: String in ids:
		var rel := "plates/%s.png" % id
		files[rel] = {"sha256": FileAccess.get_sha256(root.path_join(rel)), "size": FileAccess.get_file_as_bytes(root.path_join(rel)).size()}
	_write_json(root.path_join("manifest.json"), {"schema": WingArt.SCHEMA, "version": 1,
		"geometry": {"plate_width": 720, "plate_height": 300, "aperture_x": 200, "aperture_y": 30},
		"sources": [], "styles": styles, "seasons": {"spring": "yozakura", "summer": "summer", "autumn": "autumn", "winter": "winter"},
		"season_particles": {"spring": styles.yozakura.particles, "summer": {"kind": "none"}, "autumn": styles.yozakura.particles, "winter": {"kind": "none"}},
		"atlases": atlases, "overrides": "overrides.json", "tables": {"sin_q12": sin_table, "envelope_q8": env_table}, "files": files})
	return root

# ------------------------------------------------------------------ checks

func _open(path: String) -> RefCounted:
	var art: RefCounted = WingArt.new()
	_check(art.open_pack(path), "pack opens: %s" % art.error)
	return art

func _resolution_checks(art: RefCounted) -> void:
	_check(art.resolve_style("seasonal", {"month": 4}) == "yozakura", "April is spring (night sakura)")
	_check(art.resolve_style("seasonal", {"month": 7}) == "summer", "July is summer")
	_check(art.resolve_style("seasonal", {"month": 10}) == "autumn", "October is autumn")
	_check(art.resolve_style("seasonal", {"month": 1}) == "winter", "January is winter")
	_check(art.resolve_style("seasonal", {"month": 12}) == "winter", "December is winter")
	_check(art.resolve_style("seasonal", {}) == "yozakura", "unknown month falls back to April")
	_check(art.resolve_style("fixed:autumn", {"month": 4}) == "autumn", "fixed ignores the month")
	_check(art.resolve_style("off", {}) == "black", "off is black")
	_check(art.resolve_style("bogus", {}) == "black", "unknown mode is black")
	_check(art.resolve_style("scene", {"scene": 0x34, "month": 7}) == "school", "class A scene matches school")
	_check(art.resolve_style("scene", {"background": "CDROM/BS/D_10_08.BS"}) == "park", "background prefix matches")
	_check(art.resolve_style("scene", {"stage": "prologue", "background": "D_10_08"}) == "legend_tree", "stage outranks background")
	_check(art.resolve_style("scene", {"location": "school"}) == "school", "location matches")
	_check(art.resolve_style("scene", {"scene": 0x01, "month": 10}) == "autumn", "no rule falls back to seasonal")
	_check(art.set_user_overrides({"schema": WingArt.OVERRIDES_SCHEMA, "default": "seasonal",
		"rules": [{"match": {"class": "B"}, "style": "black"}, {"match": {"background_prefix": "D_10_"}, "style": "winter"}]}), "user overrides accepted")
	_check(art.resolve_style("scene", {"scene": 0x01}) == "black", "user rule can force black for a scene class")
	_check(art.resolve_style("scene", {"background": "D_10_08"}) == "winter", "user rule wins over pack rule")
	_check(not art.set_user_overrides({"schema": WingArt.OVERRIDES_SCHEMA, "default": "seasonal",
		"rules": [{"match": {"class": "B"}, "style": "invented"}]}), "unknown style in user rules is rejected")
	_check(art.resolve_style("scene", {"scene": 0x01}) == "black", "rejected table keeps the previous user rules")
	_check(art.set_user_overrides(null), "user overrides can be cleared")

func _determinism_checks(path: String) -> void:
	var a: RefCounted = _open(path)
	var b: RefCounted = _open(path)
	for mode: String in ["seasonal", "fixed:summer", "fixed:autumn", "scene"]:
		for tick: int in [0, 37, 119, 1234]:
			var ctx := {"month": 4, "tick": tick, "visible_side": 160, "scene": 0x34}
			var first: Image = a.render(mode, ctx)
			var second: Image = b.render(mode, ctx)
			_check(first != null and second != null and _hash(first) == _hash(second), "deterministic %s tick %d" % [mode, tick])
			a = _open(path); b = _open(path)  # fresh state: no dissolve history
	var c: RefCounted = _open(path)
	var t10 := _hash(c.render("fixed:yozakura", {"tick": 10}))
	var t11 := _hash(c.render("fixed:yozakura", {"tick": 11}))
	var t10b := _hash(c.render("fixed:yozakura", {"tick": 10}))
	_check(t10 != t11, "petals move with the presentation tick")
	_check(t10 == t10b, "same tick gives the same bytes again")
	# Reduce motion: one static frame whatever the clock.
	var still_a := _hash(c.render("fixed:yozakura", {"tick": 3, "reduce_motion": true}))
	var still_b := _hash(c.render("fixed:yozakura", {"tick": 900, "reduce_motion": true}))
	var still_c := _hash(c.render("fixed:yozakura", {"tick": WingArt.STILL_TICK}))
	_check(still_a == still_b, "reduce_motion is static")
	_check(still_a == still_c, "reduce_motion shows the still tick")

func _fireworks_checks(path: String) -> void:
	var art: RefCounted = _open(path)
	var plate: Image = art._plate("summer").duplicate()
	var ax: int = art.geometry.aperture_x
	for side: int in [80, 160]:
		var any_burst := false
		for tick: int in range(0, 240, 6):
			var frame: Image = art.render("fixed:summer", {"tick": tick, "visible_side": side})
			var data := frame.get_data()
			var base := plate.get_data()
			var outside := 0
			var changed := 0
			for y: int in 300:
				for x: int in 720:
					var i := (y * 720 + x) * 4
					if data[i] != base[i] or data[i + 1] != base[i + 1] or data[i + 2] != base[i + 2]:
						changed += 1
						var left_ok := x >= ax - side and x < ax
						var right_ok := x >= ax + 320 and x < ax + 320 + side
						if not (left_ok or right_ok):
							outside += 1
			any_burst = any_burst or changed > 0
			_check(outside == 0, "fireworks stay inside the %d-column side area (tick %d)" % [side, tick])
		_check(any_burst, "fireworks appear with a %d-column side area" % side)
	var narrow: Image = art.render("fixed:summer", {"tick": 8, "visible_side": 50})
	_check(_hash(narrow) == _hash(plate), "no burst fits a 50-column side area, so none is drawn")
	var calm: Image = art.render("fixed:summer", {"tick": 8, "visible_side": 160, "reduce_flashes": true})
	_check(_hash(calm) == _hash(plate), "reduce_flashes removes fireworks")

func _dissolve_checks(path: String) -> void:
	var art: RefCounted = _open(path)
	var plain: RefCounted = _open(path)
	art.render("seasonal", {"month": 4, "tick": 100})
	var mid: Image = art.render("seasonal", {"month": 7, "tick": 110})
	var pure_summer: Image = plain.render("seasonal", {"month": 7, "tick": 110})
	_check(_hash(mid) != _hash(pure_summer), "a season change dissolves instead of cutting")
	var done: Image = art.render("seasonal", {"month": 7, "tick": 150})
	var pure_done: Image = _open(path).render("seasonal", {"month": 7, "tick": 150})
	_check(_hash(done) == _hash(pure_done), "the dissolve ends after 32 ticks")
	var instant: RefCounted = _open(path)
	instant.render("seasonal", {"month": 4, "tick": 5, "reduce_motion": true})
	var cut: Image = instant.render("seasonal", {"month": 10, "tick": 6, "reduce_motion": true})
	_check(_hash(cut) == _hash(_open(path).render("seasonal", {"month": 10, "reduce_motion": true})), "reduce_motion changes style without a dissolve")

## Every mode and width: the composed centre equals the native Image and the
## native Image is unchanged.
func _fidelity_matrix(path: String) -> void:
	var composer := Composer.new()
	for seed: int in [1, 77]:
		var native := _native(seed)
		var before := _hash(native)
		for mode: String in WingArt.all_modes():
			for month: int in [1, 4, 7, 10]:
				var art: RefCounted = _open(path)
				for width: int in WIDTHS:
					var ctx := {"wing_art_mode": mode, "month": month, "tick": 17 * width, "visible_side": (width - 320) / 2 + 27, "scene": 0x34}
					var composed := composer.compose(native, width, art, ctx)
					_check(composed != null, "compose %s %d: %s" % [mode, width, composer.error])
					if composed != null:
						_check(_hash(Composer.centre_crop(composed)) == before, "centre exact: %s month %d width %d" % [mode, month, width])
		_check(_hash(native) == before, "native Image unchanged after the matrix")

func _fallback_checks(good: String) -> void:
	var missing: RefCounted = WingArt.new()
	_check(not missing.open_pack("user://no-such-wing-art"), "missing pack does not open")
	_check(not missing.is_ready() and missing.render("seasonal", {}) == null, "missing pack renders nothing")
	var target := Image.create(426, 240, false, Image.FORMAT_RGBA8)
	_check(missing.fill_wings(target, 53, {"wing_art_mode": "seasonal"}) != "", "missing pack fails fill_wings")
	# Corrupt copy: one changed byte in a plate.
	var bad := "user://wing-art-bad"
	_copy_tree(good, bad)
	var bytes := FileAccess.get_file_as_bytes(bad.path_join("plates/winter.png"))
	bytes[bytes.size() - 20] = bytes[bytes.size() - 20] ^ 0xff
	var file := FileAccess.open(bad.path_join("plates/winter.png"), FileAccess.WRITE)
	file.store_buffer(bytes); file.close()
	var corrupt: RefCounted = WingArt.new()
	_check(not corrupt.open_pack(bad), "changed file fails authentication")
	_check(corrupt.error.contains("changed"), "corruption is reported: %s" % corrupt.error)
	var broken_manifest := "user://wing-art-broken"
	_copy_tree(good, broken_manifest)
	var f2 := FileAccess.open(broken_manifest.path_join("manifest.json"), FileAccess.WRITE)
	f2.store_string("{not json"); f2.close()
	_check(not WingArt.new().open_pack(broken_manifest), "unparsable manifest is rejected")
	# Host: wing art on with a damaged pack keeps black wings and an exact centre.
	var host := _host({"display": {"aspect": "wide", "wing_art": "seasonal"}}, bad)
	var native := _native(5)
	_check(host.present(native, 0x01), "host presents with a damaged pack")
	_check(host.last_wing_source == "black", "damaged pack falls back to black wings (%s)" % host.last_wing_source)
	_check(not host.backdrop_display.visible, "no backdrop with a damaged pack")
	_check(host.error.begins_with("wing art unavailable"), "host records why: %s" % host.error)
	_check(_hash(Composer.centre_crop(host.presented_image)) == _hash(native), "centre exact with a damaged pack")
	host.free()

func _copy_tree(from: String, to: String) -> void:
	DirAccess.make_dir_recursive_absolute(to.path_join("plates"))
	DirAccess.make_dir_recursive_absolute(to.path_join("atlas"))
	for sub: String in ["", "plates", "atlas"]:
		var dir := DirAccess.open(from.path_join(sub))
		for name: String in dir.get_files():
			DirAccess.copy_absolute(from.path_join(sub).path_join(name), to.path_join(sub).path_join(name))

func _host(prefs: Dictionary, pack: String) -> Control:
	var host: Control = Host.new()
	host.settings_node_path = NodePath("/root/NoSuchSettings")
	root.add_child(host)
	host.size = Vector2(1920, 1080)
	host.clock_usec = func() -> int: return 0
	host.set_wing_art_pack(pack)
	host.set_preferences_override(prefs)
	return host

func _host_checks(path: String) -> void:
	var native := _native(9)
	var digest := _hash(native)
	for size: Vector2 in [Vector2(1920, 1080), Vector2(2560, 1080)]:
		var host := _host({"display": {"aspect": "wide", "filter": "integer", "wing_art": "seasonal"}}, path)
		host.size = size
		host.relayout()
		host.set_calendar_month(7)
		_check(host.present(native, 0x01), "host presents")
		_check(host.last_wing_source == "wing_art", "wing art fills the wings (%s)" % host.last_wing_source)
		_check(host.last_wing_art_style == "summer", "month 7 shows summer (%s)" % host.last_wing_art_style)
		_check(host.backdrop_display.visible, "backdrop visible for bars and bands")
		var factor: float = host.layout.factor
		var origin: Vector2 = host.backdrop_display.position + Vector2(200, 30) * factor
		_check(origin.is_equal_approx(host.layout.aperture.position), "backdrop pixel grid is aligned with the picture")
		_check(is_equal_approx(host.backdrop_display.size.x, 720.0 * factor), "backdrop uses the picture's pixel scale")
		_check(_hash(Composer.centre_crop(host.presented_image)) == digest, "host centre exact at %s" % size)
		# Wing columns of the frame equal the backdrop pixels behind them.
		var wing: int = host.layout.wing_width
		var frame_col: Color = host.presented_image.get_pixel(wing - 1, 100)
		var bd: Image = host.wing_art.backdrop
		_check(frame_col == bd.get_pixel(200 - 1, 30 + 100), "frame wing continues the backdrop seamlessly")
		host.free()
	# Original aspect: the native Image itself is shown; wing art fills the bars.
	var host_o := _host({"display": {"aspect": "original", "wing_art": "fixed:autumn"}}, path)
	host_o.present(native, 0x01)
	_check(host_o.presented_image == native, "original aspect shows the native Image object")
	_check(host_o.backdrop_display.visible, "original aspect fills the side bars with wing art")
	host_o.free()
	# Checker continuation (Class A + wing_fill=source) wins over wing art.
	var host_c := _host({"display": {"aspect": "wide", "wing_fill": "source", "wing_art": "seasonal"}}, path)
	host_c.present(native, 0x34, {"checker_uv": Vector2i(0, 0), "checker_fade": 255, "month": 4})
	_check(host_c.last_wing_source != "wing_art" and not host_c.backdrop_display.visible, "checker wings win on Class A")
	host_c.free()
	# Off: nothing changes from the pre-T-39220 behaviour.
	var host_off := _host({"display": {"aspect": "wide", "wing_art": "off"}}, path)
	host_off.present(native, 0x01)
	_check(host_off.last_wing_source == "black" and not host_off.backdrop_display.visible, "off keeps black wings")
	_check(host_off.wing_art == null, "off never opens the pack")
	host_off.free()
	# Animation follows the presentation clock only.
	var host_a := _host({"display": {"aspect": "wide", "wing_art": "fixed:yozakura"}}, path)
	host_a.present(native, 0x01)
	var first := _hash(host_a.presented_image)
	host_a.clock_usec = func() -> int: return 500000
	host_a._process(0.0)
	_check(_hash(host_a.presented_image) != first, "the presentation clock animates the wings")
	_check(_hash(Composer.centre_crop(host_a.presented_image)) == digest, "animated frame keeps the centre exact")
	host_a.set_preferences_override({"display": {"aspect": "wide", "wing_art": "fixed:yozakura"}, "accessibility": {"reduce_motion": true}})
	var still := _hash(host_a.presented_image)
	host_a.clock_usec = func() -> int: return 900000
	host_a._process(0.0)
	_check(_hash(host_a.presented_image) == still, "reduce_motion stops the animation")
	var preview: Image = host_a.wing_art_preview("fixed:autumn")
	_check(preview != null and preview.get_size() == Vector2i(480, 270), "settings preview renders")
	_check(host_a.wing_art.last_style == "yozakura", "preview does not disturb the host's provider")
	host_a.free()

func _scene_type_checks(path: String) -> void:
	var prefs := {"display": {"wing_art": "seasonal", "wing_art_command": "fixed:school",
		"wing_art_story": "auto", "wing_art_other": "off"}}
	_check(WingArt.mode_for_scene(prefs, 0x34) == "fixed:school", "command screens use their own side art")
	_check(WingArt.mode_for_scene(prefs, 0x01) == "seasonal", "auto follows Side art")
	_check(WingArt.mode_for_scene(prefs, "CG") == "off", "a scene type can be set to black")
	_check(WingArt.mode_for_scene(prefs, null) == "seasonal", "no scene uses Side art")
	prefs.display.wing_art = "off"
	_check(WingArt.mode_for_scene(prefs, 0x34) == "off", "Side art Off wins over every scene type")
	prefs.display.wing_art = "seasonal"; prefs.display.wing_art_story = "fixed:bogus"
	_check(WingArt.mode_for_scene(prefs, 0x01) == "seasonal", "an unknown per-type value falls back to Side art")
	for style: String in ["school", "park", "town", "seaside", "legend_tree"]:
		_check(("fixed:" + style) in WingArt.all_modes(), "location style %s is a fixed option" % style)
	var native := _native(4)
	var host := _host({"display": {"aspect": "wide", "filter": "integer", "wing_art": "seasonal",
		"wing_art_command": "fixed:seaside", "wing_art_other": "off"}}, path)
	host.set_calendar_month(10)
	host.present(native, 0x34)
	_check(host.last_wing_art_style == "seaside" and host.last_wing_source == "wing_art", "host applies the command screen choice (%s)" % host.last_wing_art_style)
	host.present(native, 0x01)
	_check(host.last_wing_art_style == "autumn", "host applies Side art to dates (%s)" % host.last_wing_art_style)
	host.present(native, "CG")
	_check(host.last_wing_source == "black" and not host.backdrop_display.visible, "host shows black for a scene type set to Off")
	_check(_hash(Composer.centre_crop(host.presented_image)) == _hash(native), "per-type modes keep the centre exact")
	host.free()

static func _state_digest(state: Object) -> String:
	return JSON.stringify([state.to_words(), state.rule_state]).sha256_text()

## Integration: the game's own calendar rule moves the month across a season
## boundary; the feed reports it and the host's style follows, read only.
func _feed_checks(path: String) -> void:
	var native := _native(6)
	var host := _host({"display": {"aspect": "wide", "filter": "integer", "wing_art": "seasonal"}}, path)
	_check(not host.capabilities().wing_art_follows_game, "a host without a feed says it does not follow the game")
	var feed := Feed.new()
	_check(feed.attach(null) != "", "the feed needs a display host")
	_check(feed.attach(host, {"paths": {"wing_art": path}}).is_empty(), "the feed attaches with the installed pack path")
	_check(host.capabilities().wing_art_follows_game and host.wing_art_pack_path == path, "the host follows the game and uses the profile's pack")
	var state: Object = RuleFixture.new().synthetic_state()
	state.rule_state["allow_multi_week"] = true
	state.rule_state["month"] = 5; state.rule_state["day"] = 30; state.rule_state["weekday"] = 2
	var styles: Array = []
	var months: Array = []
	for day: int in 3:
		var before := _state_digest(state)
		_check(feed.present(native, state), "feed presents day %d" % day)
		_check(_state_digest(state) == before, "the feed leaves the game state unchanged (day %d)" % day)
		_check(_hash(Composer.centre_crop(host.presented_image)) == _hash(native), "centre exact with the feed (day %d)" % day)
		styles.append(host.last_wing_art_style)
		months.append(state.rule_state["month"])
		_check(Calendar.advance_day(state).is_empty(), "calendar advances (day %d)" % day)
	_check(months == [5, 5, 6], "the calendar rule crosses May into June: %s" % [months])
	_check(styles == ["yozakura", "yozakura", "summer"], "Seasonal follows the game month across the boundary: %s" % [styles])
	_check(feed.month == 6 and host.calendar_month == 6, "the host holds the reported month")
	_check(feed.last_context.get("month") == 6 and not feed.last_context.has("stage"), "the context carries the month and only known facts")
	# Scene facts from the caller drive Match the scene.
	host.set_preferences_override({"display": {"aspect": "wide", "filter": "integer", "wing_art": "scene"}})
	feed.present(native, state, {"background": "CDROM/BS/D_10_08.BS"})
	_check(host.last_wing_art_style == "park", "a background fact matches its place (%s)" % host.last_wing_art_style)
	feed.present(native, state, {"stage": "prologue"})
	_check(host.last_wing_art_style == "legend_tree", "a stage fact matches its place (%s)" % host.last_wing_art_style)
	state.game_mode = 0x34
	feed.present(native, state)
	_check(host.last_wing_art_style == "school", "the game mode classifies the command screen (%s)" % host.last_wing_art_style)
	feed.detach()
	_check(not host.capabilities().wing_art_follows_game, "detaching clears follows_game")
	host.free()

func _initialize() -> void:
	var path := OS.get_environment("WING_ART_PACK")
	var synthetic := _make_pack("user://wing-art-synthetic")
	for pack: String in ([synthetic] if path.is_empty() else [synthetic, path]):
		var art := _open(pack)
		_resolution_checks(art)
		_determinism_checks(pack)
		_fireworks_checks(pack)
		_dissolve_checks(pack)
		_fidelity_matrix(pack)
		_host_checks(pack)
	_fallback_checks(synthetic)
	_scene_type_checks(synthetic)
	_feed_checks(synthetic)
	if _failures.is_empty():
		print("PASS: wing art (%d checks%s)" % [_checks, "" if path.is_empty() else ", real pack"])
	else:
		for failure: String in _failures.slice(0, 40):
			print("FAIL: " + failure)
		print("FAIL: wing art (%d of %d checks failed)" % [_failures.size(), _checks])
	quit(0 if _failures.is_empty() else 1)
