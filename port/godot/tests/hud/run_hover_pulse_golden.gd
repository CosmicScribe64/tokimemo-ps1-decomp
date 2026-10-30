extends SceneTree
## Scaled hover pulse (hud_icon_bar_update -> actor_draw_all -> func_80098680).
##
## Without env: geometry pins from the golden's POLY_FT4 packets.
## TOKIMEMO_HUD_MANIFEST + TOKIMEMO_FONT_ROOT + TOKIMEMO_HOVER_TEXELS: the
## presenter accepts the hover texels only against the pinned HUD manifest,
## refuses a scaled phase without them, and draws it with them.
## TOKIMEMO_HOVER_GOLDEN (.asset-cache/emu/hover-pulse-golden-v1): every
## scaled shot of the golden (RIGHT at command select; countdown 32 -> 0) is
## redrawn from the fixture's RAM values onto the golden scan-out (scaled icon
## 81 and cursor actor 0) and must leave it unchanged inside the scaled quad.

const Presenter = preload("res://presentation/command_hud_presenter.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const NativeMath = preload("res://core/native_math.gd")
const Identity = preload("res://services/gameplay_content_identity.gd")
const FIXTURE := "res://tests/fixtures/hover_pulse_golden.json"

var checks := 0
var failures := 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr("FAIL: " + label)

func _initialize() -> void:
	_geometry()
	var manifest := OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	var texels := OS.get_environment("TOKIMEMO_HOVER_TEXELS")
	if not manifest.is_empty() and not texels.is_empty():
		var presenter := _presenter(manifest)
		if presenter != null:
			_capability(presenter, texels)
			var golden := OS.get_environment("TOKIMEMO_HOVER_GOLDEN")
			if not golden.is_empty():
				_golden(presenter, golden)
	print("%s: hover pulse %d checks, %d failures" % ["PASS" if failures == 0 else "FAIL", checks, failures])
	quit(0 if failures == 0 else 1)

## Golden POLY_FT4 packets of icon 1 (position (-114, -41), frame origin
## (12, 8), part 24 x 16 at (0, 0) -> 25 x 17 texels): scale 0x2000 gave
## (22, 63)-(72, 97) and scale 0x1FEC (countdown 17/15) (22, 63)-(71, 96).
func _geometry() -> void:
	var position := Vector2i(-114, -41)
	var origin := Vector2i(12, 8)
	check(HudCompositor.scaled_part_quad(position, origin, Vector2i.ZERO, Vector2i(25, 17), 0x2000) == [22, 63, 72, 97], "scale 0x2000 quad equals the golden packet")
	check(HudCompositor.scaled_part_quad(position, origin, Vector2i.ZERO, Vector2i(25, 17), 0x1FEC) == [22, 63, 71, 96], "scale 0x1FEC quad equals the golden packet (arithmetic shift)")
	# Nearest texel: 50 pixels over texels 0..24 reach texel 24 (the edge column).
	check(HudCompositor.scaled_texel(49, 24, 50) == 24 and HudCompositor.scaled_texel(0, 24, 50) == 0 and HudCompositor.scaled_texel(1, 24, 50) == 0 and HudCompositor.scaled_texel(2, 24, 50) == 1, "texel stepping is nearest")

func _presenter(manifest: String) -> Presenter:
	var presenter := Presenter.new()
	var opened := presenter.open_manifest(manifest, Identity.HUD_MANIFEST_SHA, OS.get_environment("TOKIMEMO_FONT_ROOT"))
	check(opened, "HUD manifest opens: " + presenter.error)
	return presenter if opened else null

func _state(presenter: Presenter, hover_ticks: int) -> Dictionary:
	var calendar := []
	for i: int in range(2):
		var kind := []
		var marker := []
		for d: int in range(32):
			kind.append(30 if i == 0 and d == 0 else 0)
			marker.append(0)
		calendar.append({"day_kind": kind, "marker": marker})
	var icons := []
	for i: int in range(15):
		icons.append(1 if i < 14 else 0)
	var steps := []
	var elapsed := []
	for i: int in range(14):
		steps.append(0)
		elapsed.append(0)
	return {"rule": {"year": 95, "month": 4, "day": 4, "weekday": 2, "params": [0, 0, 0, 0, 0, 0, 0, 0, 0],
		"calendar": calendar, "menu_cursor": 1, "icon_enabled": icons, "player_flags": 0, "player_flags_upper": 0,
		"memory_card_busy": 0},
		"context": {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0, "icon_count": 14,
			"cursor_style": 0x9C, "hover_ticks": hover_ticks, "family_name_hex": "82a2",
			"hover_index": 1, "cursor_x": -108, "cursor_y": -43},
		"icon_steps": steps, "icon_elapsed": elapsed}

func _capability(presenter: Presenter, texels: String) -> void:
	var scaled := presenter.create_state(_state(presenter, 16)["rule"], _state(presenter, 16)["context"])
	check(scaled.is_empty() and presenter.error.contains("scaled hover texels"), "scaled phase is refused without hover texels: " + presenter.error)
	check(not presenter.open_hover_texels(texels, "0".repeat(64)).is_empty(), "hover texels with another identity are refused")
	check(presenter._compositor.hover_texels.is_empty(), "a refused hover pack leaves no texels")
	var error := presenter.open_hover_texels(texels, Identity.HOVER_TEXELS_SHA)
	check(error.is_empty(), "pinned hover texels open: " + error)
	if not error.is_empty():
		return
	var view := presenter.create_state(_state(presenter, 16)["rule"], _state(presenter, 16)["context"])
	check(not view.is_empty() and int(presenter._view(view).get("hover_scale", 0)) == 0x2000, "countdown 16 gives scale 0x2000: " + presenter.error)
	var settled := presenter.create_state(_state(presenter, 32)["rule"], _state(presenter, 32)["context"])
	check(not settled.is_empty() and int(presenter._view(settled).get("hover_scale", 0)) == 0x1000, "settled phase keeps unit scale")
	if view.is_empty() or settled.is_empty():
		return
	var big: Image = presenter.render(view, null, true)
	var unit: Image = presenter.render(settled, null, true)
	check(big != null and unit != null, "scaled and settled frames render: " + presenter.error)
	if big != null and unit != null:
		# Outside icon 1's own cell, the 2x icon covers pixels of its neighbours.
		var changed := 0
		for y: int in range(63, 97):
			for x: int in range(22, 72):
				if big.get_pixel(x, y) != unit.get_pixel(x, y):
					changed += 1
		check(changed > 300, "the scaled icon covers its 2x quad (%d pixels changed)" % changed)

## Every scaled shot of the golden is rendered as a whole HUD frame by the
## presenter from the fixture's RAM values (all fourteen icon actors, the
## hover countdown and the cursor actor) and compared with the scan-out over
## WINDOW, the golden POLY_FT4 of scale 0x2000. Every pulse quad lies inside
## it, so a pixel the port leaves out shows the port's own background there
## and counts as a mismatch, unlike a redraw over the scan-out.
const WINDOW := [22, 63, 72, 97]

var _clear_texels: Dictionary = {}
## Background pixels (no scaled icon drawn) one RGB555 step off the scan-out,
## summed over the best candidate of every reproduced shot.
var _background_steps := 0

func _golden(presenter: Presenter, root: String) -> void:
	var fixture: Variant = JSON.parse_string(FileAccess.get_file_as_string(FIXTURE))
	check(fixture is Dictionary and fixture.get("schema") == "tokimemo.hover-pulse-golden.v1", "golden fixture schema")
	if not fixture is Dictionary:
		return
	for key: String in presenter._compositor.hover_texels:
		_clear_texels[key] = []
		for part: Dictionary in presenter._compositor.hover_texels[key]:
			_clear_texels[key].append({"image": Image.create(part["image"].get_width(), part["image"].get_height(), false, Image.FORMAT_RGBA8), "stp": part["stp"]})
	var shots: Array = []
	for shot: Dictionary in fixture["shots"]:
		var image := Image.load_from_file(root.path_join("png/v%d.png" % int(shot["vsync"])))
		if image == null:
			check(false, "golden scan-out missing: v%d" % int(shot["vsync"]))
			continue
		image.convert(Image.FORMAT_RGBA8)
		# A shot whose candidate frames include a unit-scale one (the first
		# and last pulse frames) may show the settled icon: not scored here.
		var scaled: Array = []
		for candidate: Dictionary in shot["candidates"]:
			if int(candidate["icon_scale"][0]) == 0x1000:
				scaled = []
				break
			if int(candidate["icon_scale"][1]) == int(candidate["icon_scale"][0]) and candidate["icons"].all(func(icon: Array) -> bool: return icon[2] != null):
				scaled.append(candidate.duplicate(true))
		if not scaled.is_empty():
			shots.append({"vsync": int(shot["vsync"]), "image": image, "candidates": scaled})
	var exact := 0
	var discriminated := 0
	for shot: Dictionary in shots:
		var best := -1
		for candidate: Dictionary in shot["candidates"]:
			var countdown: int = int(candidate["countdown"])
			var mismatches := _mismatches(presenter, shot["image"], candidate, countdown, true)
			best = mismatches if best < 0 else mini(best, mismatches)
			if mismatches == 0 and not shot.has("match"):
				shot["match"] = candidate
			# The next pulse phase (countdown - 1) must not also reproduce the
			# frame, unless the two phases give the same scale.
			var other := countdown - 1
			if NativeMath.rsin((32 - other) << 6) == NativeMath.rsin((32 - countdown) << 6):
				other = countdown + 1
			if mismatches == 0 and _mismatches(presenter, shot["image"], candidate, other) > 0:
				discriminated += 1
		if best == 0:
			exact += 1
		else:
			printerr("v%d best mismatch %d" % [shot["vsync"], best])
	print("hover golden: %d scaled shots, %d exact, %d discriminated (whole HUD frame over %s; %d background pixels one RGB555 step off)" % [shots.size(), exact, discriminated, WINDOW, _background_steps])
	check(shots.size() >= 28, "golden covers the 32-frame pulse (%d scaled shots)" % shots.size())
	check(exact == shots.size(), "every scaled shot is reproduced over the 0x2000 packet quad (%d/%d)" % [exact, shots.size()])
	# Adjacent pulse phases can map to the same quad and texels.
	check(discriminated * 10 >= exact * 8, "a neighbouring phase's scale changes most reproduced shots (%d/%d)" % [discriminated, exact])
	_mutations(presenter, shots)

## The comparison must fail when the port leaves out icon pixels. Without the
## (w + 1, h + 1) edge column and row, every reproduced shot whose own frame
## changes by more than one RGB555 step must differ (nearest stepping reaches
## the edge texel only near 2x, so this is the top of the pulse); without
## every other texel, every shot must differ.
func _mutations(presenter: Presenter, shots: Array) -> void:
	var compositor: HudCompositor = presenter._compositor
	var original: Dictionary = compositor.hover_texels
	var edges := {}
	var sparse := {}
	for key: String in original:
		edges[key] = []
		sparse[key] = []
		for part: Dictionary in original[key]:
			var edge_image: Image = part["image"].duplicate()
			var sparse_image: Image = part["image"].duplicate()
			var size := edge_image.get_size()
			for y: int in range(size.y):
				for x: int in range(size.x):
					if x == size.x - 1 or y == size.y - 1:
						edge_image.set_pixel(x, y, Color(0, 0, 0, 0))
					if (x + y) % 2 == 0:
						sparse_image.set_pixel(x, y, Color(0, 0, 0, 0))
			edges[key].append({"image": edge_image, "stp": part["stp"]})
			sparse[key].append({"image": sparse_image, "stp": part["stp"]})
	var expected_edge := 0
	var edge_failed := 0
	var sparse_failed := 0
	for shot: Dictionary in shots:
		if shot.has("match"):
			var countdown: int = int(shot["match"]["countdown"])
			compositor.hover_texels = original
			var whole := _frame(presenter, shot["match"], countdown)
			compositor.hover_texels = edges
			var cut := _frame(presenter, shot["match"], countdown)
			if whole != null and cut != null and _steps(whole, cut, 1) > 0:
				expected_edge += 1
		for mutation: Array in [[edges, "edge"], [sparse, "sparse"]]:
			compositor.hover_texels = mutation[0]
			var best := -1
			for candidate: Dictionary in shot["candidates"]:
				var mismatches := _mismatches(presenter, shot["image"], candidate, int(candidate["countdown"]))
				best = mismatches if best < 0 else mini(best, mismatches)
			if best > 0 and mutation[1] == "edge":
				edge_failed += 1
			elif best > 0:
				sparse_failed += 1
	compositor.hover_texels = original
	print("hover golden mutations: edge texels dropped -> %d/%d shots differ (%d whose frame shows them), every other texel dropped -> %d/%d" % [edge_failed, shots.size(), expected_edge, sparse_failed, shots.size()])
	check(expected_edge >= 8 and edge_failed >= expected_edge, "dropping the edge column and row fails every shot whose frame shows them (%d/%d)" % [edge_failed, expected_edge])
	check(sparse_failed == shots.size(), "dropping every other texel changes every shot (%d/%d)" % [sparse_failed, shots.size()])

## Whole presenter frame for one RAM candidate at the given hover countdown.
func _frame(presenter: Presenter, candidate: Dictionary, countdown: int) -> Image:
	var hover: int = int(candidate["hover_icon"])
	var base := _state(presenter, 32 - countdown)
	var rule: Dictionary = base["rule"]
	var context: Dictionary = base["context"]
	rule["menu_cursor"] = hover
	context["hover_index"] = hover
	context["cursor_x"] = int(candidate["cursor"][0])
	context["cursor_y"] = int(candidate["cursor"][1])
	context["cursor_style"] = int(candidate["cursor"][2])
	var steps := []
	var elapsed := []
	for i: int in range(14):
		# hud_icon_bar_update (hud_panels.c:1101-1108): actor 80 + i flags
		# follow icon_enabled[i].
		rule["icon_enabled"][i] = 1 if int(candidate["icons"][i][0]) != 0 else 0
		steps.append(int(candidate["icons"][i][2]))
		elapsed.append(0)
	var state := presenter.create_state(rule, context)
	if state.is_empty() or not presenter.set_animation_phase(state, steps, elapsed).is_empty():
		printerr("golden state refused: " + presenter.error)
		return null
	var frame: Image = presenter.render(state, null, true)
	if frame == null:
		printerr("golden frame refused: " + presenter.error)
	return frame

## RGB555 mismatches over WINDOW between the port's frame and the scan-out.
## Every pixel of the scaled icon (where the frame differs from the same frame
## with the icon's texels cleared) must match exactly. A pixel where the port
## draws nothing of the icon shows the port's background, which may be one
## RGB555 step off the scan-out: the MDEC room C_02_02H.BS and the HUD's
## semi-transparent cells round differently in PCSX-Redux, and that
## background is not this test's subject. Any larger difference counts, so a
## skipped icon pixel (the scan-out shows the icon's texel there) fails.
func _mismatches(presenter: Presenter, golden: Image, candidate: Dictionary, countdown: int, tally: bool = false) -> int:
	var compositor: HudCompositor = presenter._compositor
	var frame := _frame(presenter, candidate, countdown)
	if not candidate.has("_back"):
		var texels: Dictionary = compositor.hover_texels
		compositor.hover_texels = _clear_texels
		candidate["_back"] = _frame(presenter, candidate, countdown)
		compositor.hover_texels = texels
	var back: Image = candidate["_back"]
	if frame == null or back == null:
		return 1 << 20
	var mismatches := 0
	var background := 0
	for y: int in range(WINDOW[1], WINDOW[3]):
		for x: int in range(WINDOW[0], WINDOW[2]):
			var a := frame.get_pixel(x, y)
			var b := golden.get_pixel(x, y)
			var step := _step(a, b)
			if step == 0:
				continue
			if step == 1 and a == back.get_pixel(x, y):
				background += 1
			else:
				mismatches += 1
	if tally and mismatches == 0:
		_background_steps += background
	return mismatches

static func _step(a: Color, b: Color) -> int:
	return maxi(maxi(absi((a.r8 >> 3) - (b.r8 >> 3)), absi((a.g8 >> 3) - (b.g8 >> 3))), absi((a.b8 >> 3) - (b.b8 >> 3)))

## Pixels over WINDOW where two frames differ by more than `allowed` steps.
static func _steps(a: Image, b: Image, allowed: int) -> int:
	var count := 0
	for y: int in range(WINDOW[1], WINDOW[3]):
		for x: int in range(WINDOW[0], WINDOW[2]):
			if _step(a.get_pixel(x, y), b.get_pixel(x, y)) > allowed:
				count += 1
	return count
