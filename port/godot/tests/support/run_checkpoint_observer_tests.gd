extends SceneTree
## T-39019 (T-39018 WP-02): synthetic tests of checkpoint_observer.gd. No game
## data: the canvas is a generated 320x240 pattern. Proves that the receipt
## binds the PNG at write time and that tampering with the PNG, the receipt or
## the clocks breaks the binding.
## Run: godot --headless --path port/godot --script res://tests/support/run_checkpoint_observer_tests.gd
## Set OBSERVER_TEST_OUT to keep the receipts (for schema validation).
const Observer = preload("res://tests/support/checkpoint_observer.gd")
var checks := 0
var failed := 0

func check(value: bool, message: String) -> void:
	checks += 1
	if not value:
		failed += 1
		printerr("FAIL: " + message)

func canvas() -> Image:
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	for y: int in 240:
		for x: int in 320:
			image.set_pixel(x, y, Color8((x * 8) & 0xF8, (y * 8) & 0xF8, ((x + y) * 8) & 0xF8, 255))
	return image

func clocks(frame: int) -> Dictionary:
	return {"frame_count": frame, "game_mode": 0x15, "mode_stage": 8, "mode_step": 0, "substep": 2}

func _initialize() -> void:
	var dir := OS.get_environment("OBSERVER_TEST_OUT")
	if dir.is_empty(): dir = "user://checkpoint-observer-test-%d" % Time.get_ticks_usec()
	DirAccess.make_dir_recursive_absolute(dir)
	var observer := Observer.new(dir, "res://tests/support/run_checkpoint_observer_tests.gd")
	var image := canvas()
	var before := Observer.rgba_sha256(image)
	var settings := Observer.settings_identity(self)
	var receipt := observer.record("stage8-2", image, "cpu_composed_image", clocks(3695), {"draw_count": 12, "state": 99},
		settings, {"pack_manifest_sha256": "0".repeat(64)}, {"prologue_scene_counter": 6})
	check(not receipt.is_empty(), "valid canvas is recorded: %s" % [observer.errors])
	check(Observer.rgba_sha256(image) == before, "recording does not change the canvas")
	check(receipt.get("schema") == "tokimemo.checkpoint-receipt.v2", "schema id")
	check(receipt.native_canvas.png_sha256 == FileAccess.get_sha256(dir.path_join("stage8-2.png")), "PNG hash bound at write time")
	check(receipt.observer.sha256 == FileAccess.get_sha256(Observer.SELF_PATH) and receipt.observer.sha256.length() == 64, "observer script hash recorded")
	check(receipt.observer.producer_sha256.length() == 64, "producer script hash recorded")
	check(settings.language == "ja" and settings.aspect == "original" and settings.filter == "integer", "default preferences report ja, original aspect and integer filter")
	check(receipt.renderer.engine.begins_with("4."), "renderer identity names the engine")
	check(Observer.verify(receipt, dir) == "", "fresh receipt verifies: " + Observer.verify(receipt, dir))
	check(observer.write() == "", "receipt set written")
	var reloaded = JSON.parse_string(FileAccess.get_file_as_string(dir.path_join("checkpoint-receipts.json")))
	check(reloaded is Dictionary and Observer.verify(reloaded.receipts["stage8-2"], dir) == "", "receipt read back from JSON still verifies")

	# Negative: refused records.
	check(observer.record("stage8-2", image, "cpu_composed_image", clocks(3695), {"draw_count": 12}, settings, {}).is_empty(), "duplicate key refused")
	check(observer.record("wide", Image.create(640, 480, false, Image.FORMAT_RGBA8), "cpu_composed_image", clocks(1), {"draw_count": 0}, settings, {}).is_empty(), "non-native canvas size refused")
	check(observer.record("viewport", image, "window_viewport", clocks(1), {"draw_count": 0}, settings, {}).is_empty(), "undeclared image source refused")
	var float_clocks := clocks(1); float_clocks.frame_count = 1.5
	check(observer.record("floaty", image, "cpu_composed_image", float_clocks, {"draw_count": 0}, settings, {}).is_empty(), "non-integer clock refused")
	var missing_clock := clocks(1); missing_clock.erase("substep")
	check(observer.record("nosub", image, "cpu_composed_image", missing_clock, {"draw_count": 0}, settings, {}).is_empty(), "missing substep refused")
	check(observer.record("norng", image, "cpu_composed_image", clocks(1), {}, settings, {}).is_empty(), "missing RNG count refused")
	check(observer.record("../escape", image, "cpu_composed_image", clocks(1), {"draw_count": 0}, settings, {}).is_empty(), "path-like key refused")

	# Negative: tampering breaks the binding.
	var changed: Dictionary = receipt.duplicate(true)
	changed.clocks.frame_count = 3696
	check(Observer.verify(changed, dir).contains("receipt fields changed"), "changed clock breaks the receipt digest")
	changed = receipt.duplicate(true)
	changed.native_canvas.png_sha256 = "f".repeat(64)
	check(Observer.verify(changed, dir).contains("receipt fields changed"), "rewritten PNG hash breaks the receipt digest")
	var tampered := canvas()
	tampered.set_pixel(0, 0, Color8(8, 0, 0, 255))
	tampered.save_png(dir.path_join("stage8-2.png"))
	check(Observer.verify(receipt, dir).contains("PNG changed"), "tampered PNG breaks the binding")
	var file := FileAccess.open(dir.path_join("stage8-2.png"), FileAccess.WRITE)
	file.store_buffer(canvas().save_png_to_buffer() + PackedByteArray([0]))
	file.close()
	check(Observer.verify(receipt, dir).contains("PNG changed"), "appended byte breaks the binding")
	DirAccess.remove_absolute(dir.path_join("stage8-2.png"))
	check(Observer.verify(receipt, dir).contains("missing"), "missing PNG breaks the binding")
	check(Observer.verify({"schema": "tokimemo.checkpoint-receipt.v1"}, dir) == "not a v2 receipt", "v1 receipts are not v2")

	# Restore the bound PNG so a kept output directory stays verifiable.
	var restore := FileAccess.open(dir.path_join("stage8-2.png"), FileAccess.WRITE)
	restore.store_buffer(image.save_png_to_buffer()); restore.close()
	check(Observer.verify(receipt, dir) == "", "restored PNG verifies again")
	print(("PASS" if failed == 0 else "FAIL") + ": checkpoint observer %d checks, %d failed" % [checks, failed])
	quit(1 if failed else 0)
