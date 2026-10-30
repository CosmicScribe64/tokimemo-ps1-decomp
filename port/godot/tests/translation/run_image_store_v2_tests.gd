extends SceneTree
## Phase 4 WP-B: translated image pack v2 record kinds (page, bs) with v1
## back-compat. Synthetic images only; no original content is read. The
## frame_part kind has its own suite (run_frame_part_tests.gd).
## Optional: TRANSLATION_IMAGE_V2_MANIFEST/_SHA256 (+ _ALLOW_DRAFTS=1) opens a
## pack emitted by tools/translation_draft_pack.py and checks each row resolves.
const Store = preload("res://services/translated_image_store.gd")
const ROOT := "/tmp/translated-image-store-v2"
const CONTAINER := "1111111111111111111111111111111111111111111111111111111111111111"
const SNAPSHOT := "2222222222222222222222222222222222222222222222222222222222222222"
# Python tools/translation_assets.py decoder_identity() of the pinned model.
const PINNED_DECODER := "82a5c2516f2b66c14234d54e6fbd277d2e6b28f9f1e793842c038dc2b98fc84b"
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _png(image: Image, name: String) -> Dictionary:
	var path := ROOT.path_join(name)
	image.save_png(path)
	return {"output": name, "output_sha256": FileAccess.get_sha256(path), "dimensions": [image.get_width(), image.get_height()]}

func _manifest(name: String, value: Dictionary) -> String:
	var path := ROOT.path_join(name)
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(value))
	file.close()
	return path

func _filled(width: int, height: int, color: Color) -> Image:
	var image := Image.create(width, height, false, Image.FORMAT_RGBA8)
	image.fill(color)
	return image

func _pack(schema: String, rows: Array, drafts: bool = false) -> Dictionary:
	return {"schema": schema, "language": "en", "development_drafts": drafts, "snapshot_sha256": SNAPSHOT, "runtime_verified": false, "assets": rows}

func _open(store: RefCounted, name: String, value: Dictionary, allow_drafts: bool = false) -> bool:
	var path := _manifest(name, value)
	return store.open_pack(path, FileAccess.get_sha256(path), allow_drafts)

func _initialize() -> void:
	DirAccess.make_dir_recursive_absolute(ROOT)
	check(Store.current_decoder_identity() == PINNED_DECODER, "decoder identity matches the Python emitter")
	check(Store.decoder_identity("other-model", {"image_mdec.py": "0"}) != PINNED_DECODER, "decoder identity depends on the model")

	# Shared synthetic sources.
	var frame_source := _filled(8, 8, Color8(10, 20, 30))
	var frame_english := _filled(8, 8, Color8(200, 10, 10))
	var page_source := _filled(16, 16, Color8(0, 0, 0, 0))
	for x: int in range(16):
		page_source.set_pixel(x, 4, Color8(40, 40, 40))
	var page_english := page_source.duplicate()
	page_english.fill_rect(Rect2i(2, 2, 6, 5), Color8(250, 250, 250))
	page_english.set_pixel(12, 12, Color8(9, 9, 9))  # outside the regions: never applied
	var page_mask := _filled(16, 16, Color8(0, 0, 0))
	page_mask.set_pixel(3, 3, Color8(255, 255, 255))
	var still_source := Image.create(32, 16, false, Image.FORMAT_RGB8)
	for y: int in range(16):
		for x: int in range(32):
			still_source.set_pixel(x, y, Color8(x * 8, y * 16, 100))
	var still_english := Store._rgba(still_source)
	still_english.fill_rect(Rect2i(16, 0, 16, 16), Color8(255, 255, 0))
	var still_sha := Store.rgba_sha256(still_source)
	var page_sha := Store.rgba_sha256(page_source)

	var frame_row: Dictionary = _png(frame_english, "frame.png")
	frame_row.merge({"source_container_sha256": CONTAINER, "frame": 3, "palette": "base", "source_png_sha256": "3".repeat(64), "style": "approve"})
	var page_row: Dictionary = _png(page_english, "page.png")
	page_row.merge({"kind": "page", "source_container_sha256": CONTAINER, "page": 2, "source_png_sha256": "4".repeat(64), "source_rgba_sha256": page_sha, "regions": [[2, 2, 3, 5], [5, 2, 3, 5]], "style": "approve"})
	var still_row: Dictionary = _png(still_english, "still.png")
	still_row.merge({"kind": "bs", "source_container_sha256": CONTAINER, "bs_index": 0, "decoder_identity_sha256": PINNED_DECODER, "source_rgba_sha256": still_sha, "regions": [[16, 0, 16, 16]], "style": "approve"})

	# v1 back-compat: a v1 frame row loads unchanged and v2-only kinds are absent.
	var v1 := Store.new()
	check(_open(v1, "v1.json", _pack(Store.SCHEMA_V1, [frame_row.duplicate()])), "v1 pack opens: " + v1.error)
	check(v1.schema == Store.SCHEMA_V1 and v1.asset_count() == 1, "v1 counts one frame")
	var v1_image: Image = v1.replacement(CONTAINER, 3, "base", "3".repeat(64))
	check(v1_image != null and v1_image.get_pixel(0, 0) == Color8(200, 10, 10), "v1 frame resolves")
	var kinded: Dictionary = page_row.duplicate()
	check(not _open(v1, "v1-page.json", _pack(Store.SCHEMA_V1, [kinded])), "v1 schema ignores kind and rejects a page row without a frame identity")

	# v2 with all three kinds.
	var store := Store.new()
	check(_open(store, "v2.json", _pack(Store.SCHEMA_V2, [frame_row, page_row, still_row])), "v2 pack opens: " + store.error)
	check(store.schema == Store.SCHEMA_V2 and store.kind_counts() == {"frame": 1, "page": 1, "bs": 1, "frame_part": 0}, "v2 counts each kind")
	check(store.replacement(CONTAINER, 3, "base", "3".repeat(64)) != null, "v2 frame resolves")
	check(store.replacement(CONTAINER, 3, "base", "0".repeat(64)) == null, "v2 frame mismatch falls back")

	# Page: positive, regions, STP mask, negatives.
	var page_before := page_source.get_data()
	var page: Image = store.page_replacement(CONTAINER, 2, "4".repeat(64), [], page_source)
	check(page != null, "page resolves with stored regions")
	if page != null:
		check(page.get_pixel(2, 2) == Color8(250, 250, 250) and page.get_pixel(7, 6) == Color8(250, 250, 250), "page region pixels replaced")
		check(page.get_pixel(12, 12) == page_source.get_pixel(12, 12) and page.get_pixel(0, 4) == page_source.get_pixel(0, 4), "page pixels outside regions keep the source")
	check(page_source.get_data() == page_before, "caller source image is not mutated")
	var explicit: Image = store.page_replacement(CONTAINER, 2, "4".repeat(64), [[2, 2, 3, 5], [5, 2, 3, 5]], page_source)
	check(explicit != null and explicit.get_data() == page.get_data(), "page resolves with matching explicit regions")
	var masked: Image = store.page_replacement(CONTAINER, 2, "4".repeat(64), [], page_source, page_mask)
	check(masked != null and masked.get_pixel(3, 3) == page_source.get_pixel(3, 3) and masked.get_pixel(4, 3) == Color8(250, 250, 250), "STP-marked page pixel keeps the source")
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [[2, 2, 6, 5]], page_source) == null, "different region rects fall back")
	check(store.page_replacement(CONTAINER, 1, "4".repeat(64), [], page_source) == null, "different page index falls back")
	check(store.page_replacement(CONTAINER, 2, "5".repeat(64), [], page_source) == null, "different source page sha falls back")
	check(store.page_replacement("6".repeat(64), 2, "4".repeat(64), [], page_source) == null, "different container falls back")
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [], _filled(8, 8, Color.BLACK)) == null, "different page size falls back")
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [], page_source, _filled(8, 8, Color.BLACK)) == null, "mask size mismatch falls back")
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [], null) == null, "missing source falls back")
	# The caller's page sha is only a key: other pixels under the right sha are refused.
	var other_page: Image = page_source.duplicate()
	other_page.set_pixel(15, 15, Color8(1, 1, 1))
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [], other_page) == null, "right page sha with different source pixels falls back")
	var palette_variant := _filled(16, 16, Color8(0, 0, 0, 0))
	for x: int in range(16):
		palette_variant.set_pixel(x, 4, Color8(90, 40, 40))
	check(store.page_replacement(CONTAINER, 2, "4".repeat(64), [], palette_variant, page_mask) == null, "same-size palette variant under the pinned sha falls back")

	# BS: positive, decoder identity, decoded-source identity.
	var still: Image = store.bs_replacement(CONTAINER, 0, PINNED_DECODER, still_source)
	check(still != null, "bs resolves on exact decoder and decoded source")
	if still != null:
		check(still.get_pixel(20, 5) == Color8(255, 255, 0) and still.get_pixel(3, 5) == Store._rgba(still_source).get_pixel(3, 5), "bs replaces only the edit region")
	check(store.bs_replacement(CONTAINER, 0, Store.decoder_identity("tokimemo-mdec-fixed-point-v2", {}), still_source) == null, "changed decoder identity falls back")
	check(store.bs_replacement(CONTAINER, 1, PINNED_DECODER, still_source) == null, "different bs index falls back")
	var drifted: Image = still_source.duplicate()
	drifted.set_pixel(0, 0, Color8(1, 2, 3))
	check(store.bs_replacement(CONTAINER, 0, PINNED_DECODER, drifted) == null, "one changed decoded source pixel falls back")
	check(store.bs_replacement("7".repeat(64), 0, PINNED_DECODER, still_source) == null, "different bs container falls back")
	# MDEC output sets STP on every word, so a mask taken from the decode is all
	# set. bs refuses any mask instead of returning a silent copy of the source.
	var full_mask := _filled(32, 16, Color8(255, 255, 255))
	check(store.bs_replacement(CONTAINER, 0, PINNED_DECODER, still_source, full_mask) == null, "bs with a fully set STP mask returns null, not the Japanese source")
	check(store.bs_replacement(CONTAINER, 0, PINNED_DECODER, still_source, _filled(32, 16, Color8(0, 0, 0))) == null, "bs refuses any STP mask")

	# Draft gate applies to v2 rows and v2 draft packs.
	var draft_row: Dictionary = still_row.duplicate()
	draft_row["style"] = "unreviewed"
	var drafts := Store.new()
	check(not _open(drafts, "draft-row.json", _pack(Store.SCHEMA_V2, [draft_row])), "unreviewed v2 row rejected without allow_drafts")
	check(not _open(drafts, "draft-pack.json", _pack(Store.SCHEMA_V2, [draft_row], true)), "v2 draft pack rejected without allow_drafts")
	check(drafts.asset_count() == 0, "rejected draft pack exposes nothing")
	check(_open(drafts, "draft-pack-ok.json", _pack(Store.SCHEMA_V2, [draft_row], true), true), "v2 draft pack opens with allow_drafts: " + drafts.error)
	check(drafts.bs_replacement(CONTAINER, 0, PINNED_DECODER, still_source) != null, "opted-in draft resolves")
	# The pack-level flag gates on its own, even when every row is approved.
	var approved_drafts := Store.new()
	check(not _open(approved_drafts, "draft-pack-approved.json", _pack(Store.SCHEMA_V2, [frame_row, page_row, still_row], true)), "development_drafts pack of approved rows rejected without allow_drafts")
	check(approved_drafts.asset_count() == 0, "rejected approved draft pack exposes nothing")

	# Malformed v2 rows fail the whole pack.
	var bad := Store.new()
	var unknown: Dictionary = page_row.duplicate()
	unknown["kind"] = "str"
	check(not _open(bad, "unknown-kind.json", _pack(Store.SCHEMA_V2, [unknown])), "unknown kind (no STR replacement) rejected")
	var unknown_bs: Dictionary = still_row.duplicate()
	unknown_bs["kind"] = "str"
	check(not _open(bad, "unknown-kind-bs-fields.json", _pack(Store.SCHEMA_V2, [unknown_bs])) and bad.error == "unsupported translation record kind", "unknown kind carrying valid bs fields rejected: " + bad.error)
	var numeric_kind: Dictionary = still_row.duplicate()
	numeric_kind["kind"] = 2
	check(not _open(bad, "numeric-kind.json", _pack(Store.SCHEMA_V2, [numeric_kind])) and bad.error == "unsupported translation record kind", "non-string kind rejected: " + bad.error)
	var no_page_rgba: Dictionary = page_row.duplicate()
	no_page_rgba.erase("source_rgba_sha256")
	check(not _open(bad, "page-no-rgba.json", _pack(Store.SCHEMA_V2, [no_page_rgba])), "page row without a source RGBA identity rejected")
	var short_page_rgba: Dictionary = page_row.duplicate()
	short_page_rgba["source_rgba_sha256"] = "abc"
	check(not _open(bad, "page-short-rgba.json", _pack(Store.SCHEMA_V2, [short_page_rgba])), "page row with a malformed source RGBA identity rejected")
	var holed := still_english.duplicate()
	holed.set_pixel(20, 5, Color8(0, 0, 0, 0))
	var holed_row: Dictionary = _png(holed, "holed-still.png")
	holed_row.merge({"kind": "bs", "source_container_sha256": CONTAINER, "bs_index": 0, "decoder_identity_sha256": PINNED_DECODER, "source_rgba_sha256": still_sha, "regions": [[16, 0, 16, 16]], "style": "approve"})
	check(not _open(bad, "holed-still.json", _pack(Store.SCHEMA_V2, [holed_row])), "bs PNG with an alpha-0 texel rejected")
	var outside: Dictionary = page_row.duplicate()
	outside["regions"] = [[10, 10, 8, 8]]
	check(not _open(bad, "outside.json", _pack(Store.SCHEMA_V2, [outside])), "region outside the page rejected")
	var empty_regions: Dictionary = still_row.duplicate()
	empty_regions["regions"] = []
	check(not _open(bad, "empty-regions.json", _pack(Store.SCHEMA_V2, [empty_regions])), "bs row without regions rejected")
	var short_decoder: Dictionary = still_row.duplicate()
	short_decoder["decoder_identity_sha256"] = "abc"
	check(not _open(bad, "short-decoder.json", _pack(Store.SCHEMA_V2, [short_decoder])), "malformed decoder identity rejected")
	var duplicate_page: Dictionary = page_row.duplicate()
	check(not _open(bad, "duplicate.json", _pack(Store.SCHEMA_V2, [page_row, duplicate_page])), "duplicate page identity rejected")
	var translucent := page_english.duplicate()
	translucent.set_pixel(0, 0, Color8(1, 1, 1, 128))
	var translucent_row: Dictionary = _png(translucent, "translucent.png")
	translucent_row.merge({"kind": "page", "source_container_sha256": CONTAINER, "page": 2, "source_png_sha256": "4".repeat(64), "source_rgba_sha256": page_sha, "regions": [[0, 0, 4, 4]], "style": "approve"})
	check(not _open(bad, "translucent.json", _pack(Store.SCHEMA_V2, [translucent_row])), "semi-transparent page replacement rejected")
	var tampered: Dictionary = page_row.duplicate()
	tampered["output_sha256"] = "8".repeat(64)
	check(not _open(bad, "tampered.json", _pack(Store.SCHEMA_V2, [tampered])), "changed PNG bytes rejected")
	check(bad.asset_count() == 0 and bad.language.is_empty(), "failed open clears the store")

	# Clearing restores the original for every kind (presenters draw native).
	store.clear()
	check(store.replacement(CONTAINER, 3, "base", "3".repeat(64)) == null and store.page_replacement(CONTAINER, 2, "4".repeat(64), [], page_source) == null and store.bs_replacement(CONTAINER, 0, PINNED_DECODER, still_source) == null, "cleared store returns null for every kind")

	_check_emitted_pack()
	print("Translation image store v2 failures: ", failures)
	quit(0 if failures.is_empty() else 1)

func _check_emitted_pack() -> void:
	var path := OS.get_environment("TRANSLATION_IMAGE_V2_MANIFEST")
	if path.is_empty():
		return
	var store := Store.new()
	var allow := OS.get_environment("TRANSLATION_IMAGE_V2_ALLOW_DRAFTS") == "1"
	check(store.open_pack(path, OS.get_environment("TRANSLATION_IMAGE_V2_SHA256"), allow), "emitted v2 pack opens: " + store.error)
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(path))
	check(store.asset_count() == manifest["assets"].size(), "emitted v2 pack loads every row")
	if allow:
		var refused := Store.new()
		check(not refused.open_pack(path, OS.get_environment("TRANSLATION_IMAGE_V2_SHA256"), false), "emitted draft pack refused without opt-in")
	var sources := OS.get_environment("TRANSLATION_IMAGE_V2_SOURCES")
	for row: Dictionary in manifest["assets"]:
		var kind: String = row.get("kind", "frame")
		if kind == "frame":
			check(store.replacement(row["source_container_sha256"], int(row["frame"]), row["palette"], row["source_png_sha256"]) != null, "emitted frame resolves")
			continue
		if kind == "frame_part":
			check(store.part_replacement(row["source_container_sha256"], int(row["frame"]), row["palette"], int(row["part_index"]), row["frame_parts_sha256"]) != null, "emitted frame part resolves")
			continue
		if sources.is_empty() or not row.has("source_png"):
			continue
		var source := Image.new()
		if source.load(sources.path_join(row["source_png"])) != OK:
			failures.append("emitted row source missing: " + str(row["source_png"]))
			continue
		var english := Image.new()
		english.load(path.get_base_dir().path_join(row["output"]))
		var result: Image = null
		if kind == "page":
			result = store.page_replacement(row["source_container_sha256"], int(row["page"]), row["source_png_sha256"], row["regions"], source)
		else:
			result = store.bs_replacement(row["source_container_sha256"], int(row["bs_index"]), Store.current_decoder_identity(), source)
			check(store.bs_replacement(row["source_container_sha256"], int(row["bs_index"]), "0".repeat(64), source) == null, "emitted bs refuses another decoder")
		check(result != null, "emitted %s resolves against its original source" % kind)
		if result != null:
			english.convert(Image.FORMAT_RGBA8)
			check(result.get_data() == english.get_data(), "emitted %s composes to the English output exactly" % kind)
