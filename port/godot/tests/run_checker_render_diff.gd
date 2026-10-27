extends SceneTree
## Pixel-level differential for the original and optimized checker raster paths.
## Uses only the pinned indexed checker payload from the fresh source pack.
const CurrentProvider = preload("res://presentation/prologue_hud_source_provider.gd")
const ReferenceProvider = preload("res://tests/fixtures/prologue_hud_source_provider_reference.gd")
const PROFILE_SHA := "a0756a103e1b01e2e0c5ac6d4c9635ade80d2a067c7d649c3af7f5f73feacd36"
const CHECKER_SHA := "5f6f22999f9fa92c217d711fc0645b970377b6aeeb5b2f0d136a8714c2202142"

var checks := 0
var failures := 0
var first_mismatch := ""

func _check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		if first_mismatch.is_empty(): first_mismatch = label

func _initialize() -> void:
	call_deferred("run")

func _configure_provider(provider: RefCounted, root: String) -> String:
	var checker_root := root.path_join("packs/prologue_hud/checker")
	var manifest_path := checker_root.path_join("manifest.json")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.native-hud-checker-indexed.v2":
		return "checker manifest schema or path invalid"
	for pair in [["indices", 16384], ["palettes", 128]]:
		var name: String = pair[0]
		var desc: Variant = parsed.get("payloads", {}).get(name)
		var path := checker_root.path_join(name + ".bin")
		if not desc is Dictionary or desc.get("size") != pair[1] \
				or not FileAccess.file_exists(path) or FileAccess.get_size(path) != pair[1] \
				or FileAccess.get_sha256(path) != desc.get("sha256"):
			return "checker payload identity/bounds failed: " + name
		if name == "indices": provider._checker_indices = FileAccess.get_file_as_bytes(path)
		else: provider._checker_palettes = FileAccess.get_file_as_bytes(path)
	provider._open = true
	return ""

func _state(month: int, u: int, v: int, fade: int, brightness: int) -> Dictionary:
	var live := {"checker_scroll_owner":"independent-differential-fixture",
		"checker_scroll_u":u, "checker_scroll_v":v, "month":month,
		"year":95, "day":4, "weekday":1, "day_kind":0,
		"screen_brightness":brightness, "title_timer":-64, "checker_fade":fade,
		"checker_scroll_phase":"after_hud_screen_frame_draw_scroll_update",
		"checker_clut_fade":fade}
	return {"opening":{"screen_brightness":brightness,"prologue_hud_live":live}}

func _background(i: int) -> Color:
	match i % 3:
		0: return Color8(13, 101, 227, 0)
		1: return Color8(47, 139, 11, 127)
		_: return Color8(239, 31, 173, 255)

func run() -> void:
	var root := OS.get_environment("PROLOGUE_HUD_ROOT")
	if root.is_empty() or not root.is_absolute_path() or root.simplify_path() != root:
		_check(false, "fresh original-content bundle path is configured")
		_finish()
		return
	_check(FileAccess.get_sha256(root.path_join("packs/prologue_hud/manifest.json")) == PROFILE_SHA,
		"fresh prologue HUD profile matches accepted pin")
	var checker_manifest_sha := FileAccess.get_sha256(root.path_join("packs/prologue_hud/checker/manifest.json"))
	_check(checker_manifest_sha == CHECKER_SHA, "fresh indexed checker manifest matches accepted pin")
	var current = CurrentProvider.new()
	var reference = ReferenceProvider.new()
	_check(_configure_provider(current, root).is_empty(), "current provider receives exact pinned checker payload")
	_check(_configure_provider(reference, root).is_empty(), "reference provider receives exact pinned checker payload")
	if failures != 0:
		_finish()
		return
	var months := [3, 6, 9, 12]
	var wraps := [0, 1, 31]
	var fades := [240, 254, 255]
	var brightnesses := [0, 1, 127, 128]
	var edge_pixels := [Vector2i(0,0), Vector2i(31,31), Vector2i(32,32),
		Vector2i(319,223), Vector2i(0,224), Vector2i(319,239)]
	var case_index := 0
	var current_usec := 0
	var reference_usec := 0
	var started_all := Time.get_ticks_usec()
	for month in months:
		for u in wraps:
			for v in wraps:
				for fade in fades:
					for brightness in brightnesses:
						var background := _background(case_index)
						var state := _state(month, u, v, fade, brightness)
						var current_image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
						var reference_image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
						current_image.fill(background)
						reference_image.fill(background)
						var t := Time.get_ticks_usec()
						var current_error: String = current.draw_checker(current_image, state)
						current_usec += Time.get_ticks_usec() - t
						t = Time.get_ticks_usec()
						var reference_error: String = reference.draw_checker(reference_image, state)
						reference_usec += Time.get_ticks_usec() - t
						var label := "month=%d,u=%d,v=%d,fade=%d,brightness=%d,bg=%d" % [month,u,v,fade,brightness,case_index % 3]
						_check(current_error.is_empty() and reference_error.is_empty(), label + ":both draw")
						_check(current_image.get_data() == reference_image.get_data(), label + ":full RGBA byte equality")
						for point in edge_pixels:
							_check(current_image.get_pixelv(point) == reference_image.get_pixelv(point),
								label + ":edge pixel %s" % point)
						case_index += 1
	var elapsed := Time.get_ticks_usec() - started_all
	var report := {"status":"PASS" if failures == 0 else "FAIL",
		"cases":case_index,"checks":checks,"failures":failures,"first_mismatch":first_mismatch,
		"season_months":months,"uv_wrap_values":wraps,"fade_values":fades,
		"brightness_values":brightnesses,"background_alpha_values":[0,127,255],
		"edge_pixels_checked_per_case":edge_pixels.size(),"total_elapsed_usec":elapsed,
		"current_draw_elapsed_usec":current_usec,"reference_draw_elapsed_usec":reference_usec,
		"profile_sha256":PROFILE_SHA,"checker_manifest_sha256":FileAccess.get_sha256(root.path_join("packs/prologue_hud/checker/manifest.json")),
		"current_script_sha256":FileAccess.get_sha256("res://presentation/prologue_hud_source_provider.gd"),
		"reference_script_sha256":FileAccess.get_sha256("res://tests/fixtures/prologue_hud_source_provider_reference.gd"),
		"runtime_or_native_equivalence_claim":false}
	var output_path := OS.get_environment("CHECKER_DIFF_REPORT")
	if not output_path.is_empty():
		var file := FileAccess.open(output_path, FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(report, "\t") + "\n")
			file = null
	print(JSON.stringify(report))
	_finish()

func _finish() -> void:
	print("CHECKER_DIFF: %d checks, %d failures" % [checks, failures])
	quit(0 if failures == 0 else 1)
