extends SceneTree
## T-39004: exact identity lookup, isolation and fallback with the retained pack.
const Store = preload("res://services/translated_image_store.gd")
const Presenter = preload("res://presentation/command_hud_presenter.gd")
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _initialize() -> void:
	var path := OS.get_environment("TRANSLATION_IMAGE_MANIFEST")
	var expected := OS.get_environment("TRANSLATION_IMAGE_SHA256")
	var store := Store.new()
	check(store.open_pack(path, expected), "retained pack opens: " + store.error)
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(path))
	check(store.asset_count() == manifest["assets"].size(), "all mapped assets loaded")
	for row: Dictionary in manifest["assets"]:
		var image: Image = store.replacement(row["source_container_sha256"], int(row["frame"]), row["palette"], row["source_png_sha256"])
		check(image != null, "exact source resolves")
		if image == null:
			continue
		var original := Image.new()
		original.load(path.get_base_dir().path_join(row["output"]))
		check(image.get_data() == original.get_data(), "replacement bytes preserved")
		image.fill(Color.MAGENTA)
		var again: Image = store.replacement(row["source_container_sha256"], int(row["frame"]), row["palette"], row["source_png_sha256"])
		check(again.get_data() == original.get_data(), "caller cannot mutate cached artwork")
		check(store.replacement(row["source_container_sha256"], int(row["frame"]), row["palette"], "0".repeat(64)) == null, "different source retains Japanese fallback")
	check(not store.open_pack(path, "0".repeat(64)), "wrong manifest hash rejected")
	check(store.asset_count() == 0 and store.language.is_empty(), "failed reopen clears previous pack")
	if not OS.get_environment("HUD_MANIFEST").is_empty():
		store.open_pack(path, expected)
		var presenter := Presenter.new()
		check(presenter.open_manifest(OS.get_environment("HUD_MANIFEST"), OS.get_environment("HUD_SHA256"), OS.get_environment("HUD_FONT_ROOT")), "HUD opens: " + presenter.error)
		var sources: Dictionary = presenter._images.duplicate()
		check(presenter.set_translated_images(store) == 12, "twelve exact weekday images bind to native renderer")
		var changes := 0
		for key: String in sources:
			if sources[key].get_data() != presenter._images[key].get_data():
				changes += 1
		check(changes == 12, "only twelve color images change; STP masks preserved")
		presenter.set_translated_images(null)
		for key: String in sources:
			check(sources[key].get_data() == presenter._images[key].get_data(), "Japanese source restored exactly")
	print("Translation image store failures: ", failures)
	quit(0 if failures.is_empty() else 1)
