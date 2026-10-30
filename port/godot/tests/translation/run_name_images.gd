extends SceneTree
## T-39004: source binding and reversibility; these are not actual-game captures.
const Media = preload("res://services/name_entry_image_store.gd")
const Translations = preload("res://services/translated_image_store.gd")
const Presenter = preload("res://presentation/name_entry_image_presenter.gd")
const Text = preload("res://presentation/vn_scene_presenter.gd")
const Entry = preload("res://flow/new_game_entry.gd")
var failures: Array[String] = []

func check(value: bool, label: String) -> void:
	if not value: failures.append(label)

func _initialize() -> void:
	var root_path := ProjectSettings.globalize_path("res://../..").simplify_path()
	var bundle := OS.get_environment("TOKIMEMO_TEST_INSTALLED_PACK")
	if bundle.is_empty(): bundle = root_path.path_join(".asset-cache/port/installed-runtime-v7")
	var installed: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(bundle.path_join("manifest.json")))
	var media_path := bundle.path_join(installed.roles.name_entry_media)
	if not OS.get_environment("TOKIMEMO_TEST_NAME_MEDIA").is_empty(): media_path = OS.get_environment("TOKIMEMO_TEST_NAME_MEDIA")
	var translated_path := root_path.path_join(".asset-cache/translation-assets/name-runtime-drafts-v1/manifest.json")
	var native := Media.new()
	check(native.open_pack(media_path,FileAccess.get_sha256(media_path)),"native media: "+native.error)
	if native.manifest.is_empty(): printerr(failures," ",native.error); quit(1); return
	var source_manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(media_path))
	var rejected_path := media_path.get_base_dir().path_join("malformed-test-manifest.json")
	for defect: int in range(7):
		var malformed := source_manifest.duplicate(true)
		match defect:
			0: malformed.inputs.name = []
			1: malformed.frames["0"].bounds[2] += 1
			2: malformed.prompts.erase("birthday")
			3: malformed.digits["small-0"].path = "../outside.png"
			4: malformed.erase("base_background")
			5: malformed.inputs.background.sha256 = "0".repeat(64)
			6: malformed.base_background.png.width = 319
		var handle := FileAccess.open(rejected_path,FileAccess.WRITE)
		handle.store_string(JSON.stringify(malformed)); handle.close()
		check(not native.open_pack(rejected_path,FileAccess.get_sha256(rejected_path)),"malformed media rejected: %d" % defect)
		check(native.images.is_empty() and native.manifest.is_empty(),"failed reopen clears all prior resources")
	DirAccess.remove_absolute(rejected_path)
	check(native.open_pack(media_path,FileAccess.get_sha256(media_path)),"valid media reopens")
	var translations := Translations.new()
	check(not translations.open_pack(translated_path,FileAccess.get_sha256(translated_path)),"drafts rejected without explicit opt-in")
	check(translations.open_pack(translated_path,FileAccess.get_sha256(translated_path),true),"development pack: "+translations.error)
	var count := 0
	for index: int in range(29):
		var original: Image = native.frame_image(index)
		var english: Image = native.frame_image(index,translations)
		if original.get_data() != english.get_data(): count += 1
	check(count == 14,"all fourteen translated frames bind; other frames unchanged")
	var text := Text.new()
	check(text.open_font_only(bundle.path_join(installed.roles.font_root)),"native glyph resources: "+text.error)
	var presenter := Presenter.new()
	check(presenter.open_pack(media_path,FileAccess.get_sha256(media_path),text),"source renderer opens")
	var entry := Entry.new()
	var content_path := bundle.path_join(installed.roles.name_entry_content)
	entry._content = Entry.load_content(content_path)
	entry.names = {"family":"82608261".hex_decode(),"given":"8262".hex_decode(),"nickname":"8263".hex_decode()}
	entry.player = {"month":0,"day":0,"blood":0}
	entry.shiori = entry.player.duplicate()
	for state: Array in [[1,1],[1,7],[1,9],[2,1],[2,3]]:
		entry.stage = state[0]; entry.step_id = state[1]
		presenter.translations = null
		var original := presenter.render(entry)
		presenter.translations = translations
		var english := presenter.render(entry)
		check(original.get_data() != english.get_data(),"translated live-state composition differs")
		presenter.translations = null
		check(presenter.render(entry).get_data() == original.get_data(),"Japanese fallback restored exactly")
		original.save_png(root_path.path_join(".asset-cache/translation-assets/name-runtime-drafts-v1/native-%d-%d.png" % state))
		english.save_png(root_path.path_join(".asset-cache/translation-assets/name-runtime-drafts-v1/english-%d-%d.png" % state))
	print("Name image checks: ",failures)
	quit(0 if failures.is_empty() else 1)
