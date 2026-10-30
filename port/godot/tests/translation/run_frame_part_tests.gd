extends SceneTree
## T-39004: multi-part HUD frame replacement (pack kind `frame_part`).
## Synthetic store checks always run; no original content is read.
## With HUD_MANIFEST, HUD_SHA256, HUD_FONT_ROOT and DRAFT_IMAGE_MANIFEST/
## DRAFT_IMAGE_SHA256 (the draft_images member of a bundle built by
## tools/port/build_english_bundle.py --with-drafts), the real presenter checks:
## English off keeps every source image exactly, only the parts the pack lists
## change, a pack whose frame part hashes differ binds nothing, and (with
## HUD_STATE, a captured command-select state) the Japanese render is
## byte-identical before and after English. With HOVER_TEXELS (the installed
## command_hover_texels/hover_texels.json) English on and then off must give
## back the scaled hover phase of every icon frame a pack replaced. The shipped
## packs replace no icon frame, so that check binds a synthetic pack that
## replaces every icon frame with its own source parts.
const Store = preload("res://services/translated_image_store.gd")
const Presenter = preload("res://presentation/command_hud_presenter.gd")
const Identity = preload("res://services/gameplay_content_identity.gd")
const ROOT := "/tmp/translated-image-frame-parts"
const CONTAINER := "1111111111111111111111111111111111111111111111111111111111111111"
const SNAPSHOT := "2222222222222222222222222222222222222222222222222222222222222222"
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _manifest(name: String, value: Dictionary) -> String:
	var path := ROOT.path_join(name)
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(value))
	file.close()
	return path

func _open(store: RefCounted, name: String, rows: Array, drafts: bool = true, schema: String = Store.SCHEMA_V2) -> bool:
	var path := _manifest(name, {"schema": schema, "language": "en", "development_drafts": drafts, "snapshot_sha256": SNAPSHOT, "runtime_verified": false, "assets": rows})
	return store.open_pack(path, FileAccess.get_sha256(path), true)

func _initialize() -> void:
	DirAccess.make_dir_recursive_absolute(ROOT)
	_synthetic()
	_presenter()
	print("Translation frame part failures: ", failures)
	quit(0 if failures.is_empty() else 1)

func _synthetic() -> void:
	var english := Image.create(4, 4, false, Image.FORMAT_RGBA8)
	english.fill(Color8(200, 20, 20))
	english.save_png(ROOT.path_join("part.png"))
	var shas := ["a".repeat(64), "b".repeat(64), "c".repeat(64)]
	var row := {"kind": "frame_part", "source_container_sha256": CONTAINER, "frame": 60, "palette": "base", "part_index": 1,
		"source_png_sha256": shas[1], "frame_parts_sha256": shas, "output": "part.png",
		"output_sha256": FileAccess.get_sha256(ROOT.path_join("part.png")), "dimensions": [4, 4], "style": "unreviewed"}
	var store := Store.new()
	check(_open(store, "parts.json", [row]), "frame part pack opens: " + store.error)
	check(store.kind_counts().get("frame_part") == 1 and store.asset_count() == 1, "one frame part counted")
	var image: Image = store.part_replacement(CONTAINER, 60, "base", 1, shas)
	check(image != null and image.get_pixel(0, 0) == Color8(200, 20, 20), "exact frame and part resolve")
	if image != null:
		image.fill(Color.MAGENTA)
		check(store.part_replacement(CONTAINER, 60, "base", 1, shas).get_pixel(0, 0) == Color8(200, 20, 20), "caller cannot mutate the cached part")
	check(store.part_replacement(CONTAINER, 60, "base", 0, shas) == null, "a part without a row keeps the original")
	check(store.part_replacement(CONTAINER, 60, "base", 1, ["a".repeat(64), "b".repeat(64), "d".repeat(64)]) == null, "another part of the frame differs: refused")
	check(store.part_replacement(CONTAINER, 60, "base", 1, ["a".repeat(64), "e".repeat(64), "c".repeat(64)]) == null, "the part's own source differs: refused")
	check(store.part_replacement(CONTAINER, 60, "base", 1, shas.slice(0, 2)) == null, "a frame with other part count: refused")
	check(store.part_replacement(CONTAINER, 60, "hud-43", 1, shas) == null and store.part_replacement(CONTAINER, 61, "base", 1, shas) == null, "other palette or frame: refused")
	check(store.part_replacement("0".repeat(64), 60, "base", 1, shas) == null, "other container: refused")
	check(store.part_replacement(CONTAINER, 60, "base", 5, shas) == null, "part index outside the frame: refused")
	check(store.replacement(CONTAINER, 60, "base", shas[1]) == null, "a part row never binds as a whole frame")
	check(not Store.new().open_pack(ROOT.path_join("parts.json"), FileAccess.get_sha256(ROOT.path_join("parts.json")), false), "draft part pack needs the opt-in")
	var bad := Store.new()
	for change: Array in [["part_index", 3], ["source_png_sha256", "d".repeat(64)], ["frame_parts_sha256", [shas[1]]], ["frame_parts_sha256", "x"], ["output_sha256", "9".repeat(64)], ["dimensions", [5, 4]]]:
		var broken := row.duplicate(true)
		broken[change[0]] = change[1]
		check(not _open(bad, "broken.json", [broken]), "invalid frame part row refused: " + str(change[0]))
	check(not _open(bad, "duplicate.json", [row, row.duplicate()]), "duplicate frame part refused")
	# Schema v1 has no kinds: the row is a whole-frame row and binds no part.
	var v1 := Store.new()
	check(_open(v1, "v1.json", [row], true, Store.SCHEMA_V1), "v1 pack opens")
	check(v1.part_replacement(CONTAINER, 60, "base", 1, shas) == null, "v1 rows never bind as parts")

func _presenter() -> void:
	var hud := OS.get_environment("HUD_MANIFEST")
	var pack := OS.get_environment("DRAFT_IMAGE_MANIFEST")
	if hud.is_empty() or pack.is_empty():
		return
	var presenter := Presenter.new()
	check(presenter.open_manifest(hud, OS.get_environment("HUD_SHA256"), OS.get_environment("HUD_FONT_ROOT")), "HUD opens: " + presenter.error)
	var store := Store.new()
	check(store.open_pack(pack, OS.get_environment("DRAFT_IMAGE_SHA256"), true), "draft pack opens: " + store.error)
	var sources: Dictionary = presenter._images.duplicate()
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(pack))
	var expected := {}
	var frame_parts := 0
	for row: Dictionary in manifest["assets"]:
		if row.get("kind", "frame") != "frame_part":
			continue
		frame_parts += 1
		var path: String = presenter._manifest["frames"]["%d:%s" % [int(row["frame"]), row["palette"]]]["parts"][int(row["part_index"])]["png"]["path"]
		var image := Image.new()
		image.load(pack.get_base_dir().path_join(row["output"]))
		expected[path] = image
	check(frame_parts > 0, "the draft pack carries frame parts")
	presenter.set_translated_images(null)
	for key: String in sources:
		check(presenter._images[key].get_data() == sources[key].get_data(), "English off keeps the exact source: " + key)
	var bound: int = presenter.set_translated_images(store)
	check(bound >= frame_parts, "every frame part binds (%d of %d)" % [bound, frame_parts])
	for key: String in expected:
		var image: Image = presenter._images[key]
		var want: Image = expected[key]
		check(image.get_data() == want.get_data() and image.get_format() == want.get_format(), "frame part replaced by its pack image: " + key)
	for record: Dictionary in presenter._manifest["frames"].values():
		if record["parts"].size() < 2:
			continue
		for part: Dictionary in record["parts"]:
			var key: String = part["png"]["path"]
			if not expected.has(key):
				check(presenter._images[key].get_data() == sources[key].get_data(), "unlisted part keeps the original: " + key)
	_render_identity(presenter, store)
	_hover_restored()
	presenter.set_translated_images(null)
	for key: String in sources:
		check(presenter._images[key].get_data() == sources[key].get_data(), "Japanese restored exactly: " + key)
	# A pack whose frame part hashes name another frame binds no part.
	var copied := manifest.duplicate(true)
	copied["assets"] = []
	DirAccess.make_dir_recursive_absolute(ROOT.path_join("drafts"))
	for row: Dictionary in manifest["assets"]:
		if row.get("kind", "frame") != "frame_part":
			continue
		var changed := row.duplicate(true)
		var other: int = (int(changed["part_index"]) + 1) % int(changed["frame_parts_sha256"].size())
		changed["frame_parts_sha256"][other] = "0".repeat(64)
		DirAccess.copy_absolute(pack.get_base_dir().path_join(row["output"]), ROOT.path_join(row["output"]))
		copied["assets"].append(changed)
	var mismatch := Store.new()
	var mismatch_path := _manifest("mismatch.json", copied)
	check(mismatch.open_pack(mismatch_path, FileAccess.get_sha256(mismatch_path), true), "mismatched pack opens: " + mismatch.error)
	check(presenter.set_translated_images(mismatch) == 0, "frame part hashes that differ from the HUD bind nothing")
	for key: String in sources:
		check(presenter._images[key].get_data() == sources[key].get_data(), "a refused pack keeps the source: " + key)
	presenter.set_translated_images(null)

func _render_identity(presenter: Presenter, store: RefCounted) -> void:
	var state_path := OS.get_environment("HUD_STATE")
	if state_path.is_empty():
		return
	var captured: Dictionary = _ints(JSON.parse_string(FileAccess.get_file_as_string(state_path)))
	var rule: Dictionary = captured["state"]["rule_state"]
	var family := ""
	for b: int in rule["profile_family_bytes"]:
		if b == 0:
			break
		family += "%02x" % b
	var context := {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0, "icon_count": 14,
		"cursor_style": Presenter.DEFAULT_CURSOR_STYLE, "hover_ticks": 32, "family_name_hex": family}
	var fresh := Presenter.new()
	fresh.open_manifest(OS.get_environment("HUD_MANIFEST"), OS.get_environment("HUD_SHA256"), OS.get_environment("HUD_FONT_ROOT"))
	var state := fresh.create_state(rule, context)
	check(not state.is_empty(), "captured state builds: " + fresh.error)
	if state.is_empty():
		return
	var native := fresh.render(state)
	presenter.set_translated_images(store)
	var english := presenter.render(presenter.create_state(rule, context))
	presenter.set_translated_images(null)
	var restored := presenter.render(presenter.create_state(rule, context))
	check(native != null and english != null and restored != null, "renders: " + presenter.error)
	if native != null and english != null and restored != null:
		check(restored.get_data() == native.get_data(), "Japanese render is byte-identical to a presenter that never saw English")
		check(english.get_data() != native.get_data(), "English render differs from the Japanese one")

## Scaled hover pulse state (hud/run_hover_pulse_golden.gd): icon 1 hovered,
## countdown 16 gives scale 0x2000, so the scaled path is taken.
func _hover_state(hover_ticks: int) -> Dictionary:
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
	return {"rule": {"year": 95, "month": 4, "day": 4, "weekday": 2, "params": [0, 0, 0, 0, 0, 0, 0, 0, 0],
		"calendar": calendar, "menu_cursor": 1, "icon_enabled": icons, "player_flags": 0, "player_flags_upper": 0,
		"memory_card_busy": 0},
		"context": {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0, "icon_count": 14,
			"cursor_style": 0x9C, "hover_ticks": hover_ticks, "family_name_hex": "82a2",
			"hover_index": 1, "cursor_x": -108, "cursor_y": -43}}

## A v2 draft pack that replaces every part of every icon frame with a copy
## of its source image (a frame row for one part, frame_part rows otherwise).
func _icon_pack(presenter: Presenter) -> RefCounted:
	DirAccess.make_dir_recursive_absolute(ROOT.path_join("icons"))
	var container: String = presenter._manifest["provenance"]["inputs"]["mainrom"]["sha256"]
	var rows: Array = []
	for key: String in presenter._hover_texels:
		var record: Dictionary = presenter._manifest["frames"][key]
		var shas: Array = []
		for part: Dictionary in record["parts"]:
			shas.append(part["png"]["sha256"])
		for index: int in range(shas.size()):
			var image: Image = presenter._source_images[record["parts"][index]["png"]["path"]]
			var output := "icons/%s.png" % shas[index]
			image.save_png(ROOT.path_join(output))
			var row := {"kind": "frame", "source_container_sha256": container, "frame": int(record["frame_index"]),
				"palette": record["variant"], "source_png_sha256": shas[index], "output": output,
				"output_sha256": FileAccess.get_sha256(ROOT.path_join(output)),
				"dimensions": [image.get_width(), image.get_height()], "style": "unreviewed"}
			if shas.size() > 1:
				row.merge({"kind": "frame_part", "part_index": index, "frame_parts_sha256": shas}, true)
			rows.append(row)
	var store := Store.new()
	check(_open(store, "icons.json", rows), "synthetic icon pack opens: " + store.error)
	return store

func _hover_restored() -> void:
	var texels := OS.get_environment("HOVER_TEXELS")
	if texels.is_empty():
		return
	var presenter := Presenter.new()
	presenter.open_manifest(OS.get_environment("HUD_MANIFEST"), OS.get_environment("HUD_SHA256"), OS.get_environment("HUD_FONT_ROOT"))
	var error: String = presenter.open_hover_texels(texels, Identity.HOVER_TEXELS_SHA)
	check(error.is_empty(), "hover texels open: " + error)
	if not error.is_empty():
		return
	var store := _icon_pack(presenter)
	var opened: int = presenter._hover_texels.size()
	var scaled := _hover_state(16)
	var before := presenter.create_state(scaled["rule"], scaled["context"])
	check(not before.is_empty() and int(presenter._view(before).get("hover_scale", 0)) == 0x2000, "scaled hover phase builds before English: " + presenter.error)
	var native: Image = presenter.render(before) if not before.is_empty() else null
	presenter.set_translated_images(store)
	check(presenter._hover_texels.size() < opened and presenter._compositor.hover_texels.size() == presenter._hover_texels.size(), "English drops the scaled texels of replaced icon frames")
	presenter.set_translated_images(null)
	check(presenter._hover_texels.size() == opened and presenter._compositor.hover_texels.size() == opened, "English off restores every scaled hover frame")
	var after := presenter.create_state(scaled["rule"], scaled["context"])
	check(not after.is_empty(), "scaled hover phase builds after English on and off: " + presenter.error)
	if native != null and not after.is_empty():
		var restored: Image = presenter.render(after)
		check(restored != null and restored.get_data() == native.get_data(), "scaled hover render after English off is byte-identical")

func _ints(v: Variant) -> Variant:
	# JSON numbers parse as floats; the captured rule state holds integers.
	if v is float:
		return int(v)
	if v is Array:
		var a: Array = []
		for x: Variant in v:
			a.append(_ints(x))
		return a
	if v is Dictionary:
		var d: Dictionary = {}
		for k: Variant in v:
			d[k] = _ints(v[k])
		return d
	return v
