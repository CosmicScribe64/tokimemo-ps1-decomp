extends SceneTree
## Independent differential checks for authenticated HUD/font snapshots.

const Provider = preload("res://presentation/prologue_hud_source_provider.gd")
const NativeGlyphs = preload("res://presentation/native_glyphs.gd")

var checks := 0
var failures := 0
var compared_frames := 0
var compared_calendar := 0
var compared_glyph_views := 0
var detached_mutations := 0
var snapshot_reads_after_disk_mutation := 0

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ", label)

func _initialize() -> void:
	call_deferred("run")

func _hash(bytes: PackedByteArray) -> String:
	var h := HashingContext.new()
	h.start(HashingContext.HASH_SHA256)
	h.update(bytes)
	return h.finish().hex_encode()

func _copy_image(image: Image) -> Image:
	return image.duplicate()

func _first_glyph_code(font: NativeGlyphs) -> PackedByteArray:
	# Pick a numeric code from the descriptor range and return only a code unit;
	# this test never prints or serializes a character.
	var metadata: Variant = JSON.parse_string(FileAccess.get_file_as_string(
		OS.get_environment("PROLOGUE_FONT_ROOT").path_join("font.json")))
	if not metadata is Dictionary: return PackedByteArray()
	var ranges: Array = metadata.get("metadata", {}).get("ranges", [])
	for index in range(1, ranges.size()):
		var code := int(ranges[index]["first"]) - 1
		var pair := PackedByteArray([code >> 8, code & 255])
		if font.get_glyph(pair).get("status") == "glyph": return pair
	return PackedByteArray()

func run() -> void:
	var root := OS.get_environment("PROLOGUE_HUD_ROOT")
	var snapshot_root := OS.get_environment("PROLOGUE_HUD_SNAPSHOT_ROOT")
	var corrupt_root := OS.get_environment("PROLOGUE_HUD_CORRUPT_ROOT")
	var profile_rel := "packs/prologue_hud/manifest.json"
	if root.is_empty() or snapshot_root.is_empty() or corrupt_root.is_empty():
		check(false, "original, mutable snapshot-fixture, and corrupt-fixture roots are supplied")
		_finish(); return
	var pin := FileAccess.get_sha256(root.path_join(profile_rel))
	var baseline = Provider.new()
	var snap = Provider.new()
	check(baseline.open_installed(root, profile_rel, pin).is_empty(), "legacy provider opens accepted bundle")
	check(snap.open_installed(snapshot_root, profile_rel, pin).is_empty(), "snapshot provider opens accepted bundle")
	check(snap.snapshot_runtime_assets().is_empty(), "all profiled frame/calendar assets snapshot successfully")
	if not baseline._open or not snap._open:
		_finish(); return

	var frame_ids: Array = Provider.INSTALLED_FRAME_IDS
	for frame_id in frame_ids:
		var old_parts: Dictionary = baseline.frame_parts(int(frame_id))
		var new_parts: Dictionary = snap.frame_parts(int(frame_id))
		check(old_parts.get("status") == "frame_parts" and new_parts.get("status") == "frame_parts",
			"frame parts load on both live and snapshot providers")
		if old_parts.get("status") != "frame_parts" or new_parts.get("status") != "frame_parts": continue
		check(old_parts["bounds"] == new_parts["bounds"] and old_parts["origin"] == new_parts["origin"]
			and old_parts["parts"].size() == new_parts["parts"].size(), "frame metadata is identical")
		for part_index in range(old_parts["parts"].size()):
			var a: Dictionary = old_parts["parts"][part_index]
			var b: Dictionary = new_parts["parts"][part_index]
			for key in ["color", "stp_mask"]:
				var old_image: Image = a[key]["image"]
				var new_image: Image = b[key]["image"]
				check(old_image.get_size() == new_image.get_size() and old_image.get_data() == new_image.get_data(),
					"frame color/STP bytes match across live and snapshot paths")
		compared_frames += 1
		var old_render := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		var new_render := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		old_render.fill(Color(0.125, 0.25, 0.375, 1.0))
		new_render.fill(Color(0.125, 0.25, 0.375, 1.0))
		check(baseline._draw_frame(old_render, int(frame_id), Vector2i(160, 120)).is_empty()
			and snap._draw_frame(new_render, int(frame_id), Vector2i(160, 120)).is_empty(),
			"complete frame render succeeds")
		check(old_render.get_data() == new_render.get_data(), "complete frame pixels match")
		if int(frame_id) == int(frame_ids[0]):
			check(baseline._draw_frame(old_render, int(frame_id), Vector2i(160, 120)).is_empty()
				and snap._draw_frame(new_render, int(frame_id), Vector2i(160, 120)).is_empty()
				and old_render.get_data() == new_render.get_data(),
				"repeated draw from live and snapshot resources remains pixel-identical")

	var profile: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(root.path_join(profile_rel)))
	var assets_root: String = root.path_join(profile["resources"]["prologue_assets"]["directory"])
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(
		assets_root.path_join(profile["resources"]["prologue_assets"]["manifest"])))
	for record in manifest["content"]["calendar"]:
		var old_cal: Dictionary = baseline.calendar_image(int(record["year"]), int(record["month"]), str(record["variant"]))
		var new_cal: Dictionary = snap.calendar_image(int(record["year"]), int(record["month"]), str(record["variant"]))
		check(old_cal.get("status") == "asset" and new_cal.get("status") == "asset", "calendar entry loads")
		if old_cal.get("status") == "asset" and new_cal.get("status") == "asset":
			check(old_cal["image"].get_data() == new_cal["image"].get_data()
				and old_cal["stp_mask"]["image"].get_data() == new_cal["stp_mask"]["image"].get_data(),
				"calendar image and mask bytes match")
			compared_calendar += 1

	# Returned frame images must be detached from the provider's private cache.
	var detached_before: Dictionary = snap.frame_parts(int(frame_ids[0]))
	var original_data: PackedByteArray = detached_before["parts"][0]["color"]["image"].get_data()
	detached_before["parts"][0]["color"]["image"].fill(Color.MAGENTA)
	var detached_after: Dictionary = snap.frame_parts(int(frame_ids[0]))
	check(detached_after["parts"][0]["color"]["image"].get_data() == original_data,
		"mutating a returned Image cannot poison the private snapshot")
	detached_mutations += 1
	# Manifest arrays are part of the public view too. Mutating them must not
	# alter the provider's retained metadata or change subsequent frame output.
	var metadata_view: Dictionary = snap.frame_parts(int(frame_ids[0]))
	var expected_bounds: Array = metadata_view["bounds"].duplicate(true)
	var expected_origin: Array = metadata_view["origin"].duplicate(true)
	var expected_palette: Array = metadata_view["parts"][0]["source_palette_words"].duplicate(true)
	metadata_view["bounds"][0] = int(metadata_view["bounds"][0]) + 1
	metadata_view["origin"][0] = int(metadata_view["origin"][0]) + 1
	metadata_view["parts"][0]["source_palette_words"][0] = int(metadata_view["parts"][0]["source_palette_words"][0]) ^ 1
	var metadata_after: Dictionary = snap.frame_parts(int(frame_ids[0]))
	check(metadata_after["bounds"] == expected_bounds and metadata_after["origin"] == expected_origin,
		"returned frame geometry arrays cannot mutate retained metadata")
	check(metadata_after["parts"][0]["source_palette_words"] == expected_palette,
		"returned frame palette metadata cannot mutate retained data")
	var bounds_image_before := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var bounds_image_after := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	bounds_image_before.fill(Color(0.125, 0.25, 0.375, 1.0))
	bounds_image_after.fill(Color(0.125, 0.25, 0.375, 1.0))
	check(snap._draw_frame(bounds_image_before, int(frame_ids[0]), Vector2i(160, 120)).is_empty()
		and snap._draw_frame(bounds_image_after, int(frame_ids[0]), Vector2i(160, 120)).is_empty()
		and bounds_image_before.get_data() == bounds_image_after.get_data(),
		"mutating returned geometry cannot change later frame pixels")
	var first_calendar: Dictionary = manifest["content"]["calendar"][0]
	var calendar_view: Dictionary = snap.calendar_image(int(first_calendar["year"]),
		int(first_calendar["month"]), str(first_calendar["variant"]))
	var expected_calendar_region: Array = calendar_view["source_region"].duplicate(true)
	var expected_calendar_palette: Array = calendar_view["palette_words"].duplicate(true)
	calendar_view["source_region"][0] = int(calendar_view["source_region"][0]) + 1
	calendar_view["palette_words"][0] = int(calendar_view["palette_words"][0]) ^ 1
	var calendar_after: Dictionary = snap.calendar_image(int(first_calendar["year"]),
		int(first_calendar["month"]), str(first_calendar["variant"]))
	check(calendar_after["source_region"] == expected_calendar_region,
		"returned calendar region cannot mutate retained metadata")
	check(calendar_after["palette_words"] == expected_calendar_palette,
		"returned calendar palette cannot mutate retained metadata")

	# A post-open on-disk change is invisible to an authenticated snapshot, while
	# the legacy path continues to revalidate and reject the changed resource.
	var asset_record: Dictionary = manifest["frames"]["%d:base" % int(frame_ids[0])]["parts"][0]["png"]
	# Reopen a valid snapshot first, then mutate the very same backing file it
	# authenticated. This distinguishes snapshot stability from an unrelated copy.
	check(snap.open_installed(snapshot_root, profile_rel, "0".repeat(64)) != "", "wrong-pin reopen is rejected")
	check(snap.frame_parts(int(frame_ids[0])).get("status") == "unsupported",
		"failed profile switch clears previously opened snapshot")
	check(snap.open_installed(snapshot_root, profile_rel, pin).is_empty()
		and snap.snapshot_runtime_assets().is_empty(), "valid profile can be reopened and resnapshotted")
	var mutated_path := snapshot_root.path_join(profile["resources"]["prologue_assets"]["directory"]).path_join(asset_record["path"])
	var changed := FileAccess.open(mutated_path, FileAccess.WRITE)
	if changed != null:
		changed.store_buffer(PackedByteArray([0, 1, 2, 3, 4, 5]))
		changed = null
		var frozen_after_disk_change: Dictionary = snap.frame_parts(int(frame_ids[0]))
		check(frozen_after_disk_change.get("status") == "frame_parts"
			and frozen_after_disk_change["parts"][0]["color"]["image"].get_data() == original_data,
			"snapshot continues from private authenticated bytes after disk mutation")
		snapshot_reads_after_disk_mutation += 1
		var live_corrupt = Provider.new()
		check(live_corrupt.open_installed(corrupt_root, profile_rel, pin) != "",
			"corruption present at open is rejected")
		check(live_corrupt.frame_parts(int(frame_ids[0])).get("status") == "unsupported",
			"failed corrupt open exposes no stale frame view")
	else:
		check(false, "corrupt copy can be modified for open/revalidation probes")
	_test_glyph_snapshot()
	_finish()

func _test_glyph_snapshot() -> void:
	var font_root := OS.get_environment("PROLOGUE_FONT_ROOT")
	var snapshot_font_root := OS.get_environment("PROLOGUE_FONT_SNAPSHOT_ROOT")
	var old_font = NativeGlyphs.new()
	var snap_font = NativeGlyphs.new()
	check(old_font.open_external_font(font_root).is_empty() and snap_font.open_external_font(snapshot_font_root).is_empty(),
		"both native font readers open the pinned external font")
	var pair := _first_glyph_code(old_font)
	check(pair.size() == 2, "numeric in-range source glyph can be selected")
	if pair.size() == 2:
		for family in range(2):
			for shadow in [false, true]:
				for brightness in [0, 1, 64, 128]:
					var a: Dictionary = old_font.get_glyph_image(pair, 3, family, brightness, shadow)
					var b: Dictionary = snap_font.get_glyph_image(pair, 3, family, brightness, shadow)
					check(a.get("status") == "glyph" and b.get("status") == "glyph"
						and a["foreground_word"] == b["foreground_word"]
						and a["palette_label"] == b["palette_label"]
						and a["image"].get_data() == b["image"].get_data(),
						"glyph pixels/palette/brightness match")
					compared_glyph_views += 1
		var expected_cached_image: PackedByteArray = snap_font.get_glyph_image(pair, 3, 1, 128, true)["image"].get_data()
		var bank_path := snapshot_font_root.path_join("glyph_bank.bin")
		var bank_file := FileAccess.open(bank_path, FileAccess.READ_WRITE)
		if bank_file != null:
			bank_file.seek(0)
			bank_file.store_8(bank_file.get_8() ^ 0xFF)
			bank_file = null
			var after_disk_change: Dictionary = snap_font.get_glyph_image(pair, 3, 1, 128, true)
			check(after_disk_change.get("status") == "glyph"
				and after_disk_change["image"].get_data() == expected_cached_image,
				"font lookup remains stable after post-open glyph-bank disk mutation")
		else:
			check(false, "mutable font fixture can be changed after open")
		var before: Dictionary = snap_font.get_glyph_image(pair, 3, 1, 128, true)
		var bytes: PackedByteArray = before["image"].get_data()
		before["image"].fill(Color.MAGENTA)
		check(snap_font.get_glyph_image(pair, 3, 1, 128, true)["image"].get_data() == bytes,
			"glyph render result is caller-owned and cannot mutate font bank/palettes")
	var failed := snap_font.open_external_font(font_root.path_join("missing"))
	check(not failed.is_empty() and snap_font.get_glyph(pair).get("status") == "unsupported",
		"failed font reopen clears the previous validated snapshot")

func _finish() -> void:
	var result := {"checks": checks, "failures": failures, "frames_compared": compared_frames,
		"calendar_entries_compared": compared_calendar, "glyph_views_compared": compared_glyph_views,
		"detached_image_mutations": detached_mutations,
		"snapshot_reads_after_disk_mutation": snapshot_reads_after_disk_mutation}
	print("RUNTIME_SNAPSHOT_RESULT=" + JSON.stringify(result))
	quit(0 if failures == 0 else 1)
