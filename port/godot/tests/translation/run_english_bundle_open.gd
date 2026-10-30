extends SceneTree
## Phase 4 WP-C: every member of a built English bundle opens through the
## runtime store that the Language setting will bind, at the pinned hash.
## ENGLISH_BUNDLE=<bundle dir> (tools/port/build_english_bundle.py output).
## The coverage keys are those LanguageBundleService (T-30002 WP2) checks.
## PROLOGUE_FONT_ROOT=<native font root> also lays out every bundled dialogue
## line through PrologueTextPresenter.translation_layout(): each must fit the
## three-row textbox, so the coverage line count is what the player sees.
const TextLayer = preload("res://services/text_layer_store.gd")
const Dialogue = preload("res://services/text_translation_store.gd")
const Images = preload("res://services/translated_image_store.gd")
const Presenter = preload("res://presentation/prologue_text_presenter.gd")
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _initialize() -> void:
	_check_opened_parenthesis()
	var root := OS.get_environment("ENGLISH_BUNDLE")
	var bundle_path := root.path_join("english_bundle.json")
	check(FileAccess.file_exists(bundle_path), "bundle manifest exists")
	if not failures.is_empty():
		_finish()
		return
	var bundle: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(bundle_path))
	check(bundle.get("schema") == "tokimemo.english-bundle.v1" and bundle.get("human_approval") == false, "bundle schema and no approval claim")
	check(str(bundle.get("disclosure", "")).contains("AI-translated"), "bundle carries the AI disclosure")
	var members: Dictionary = bundle["members"]
	for name: String in members:
		var member: Dictionary = members[name]
		var path := root.path_join(member["path"])
		var sha: String = member["sha256"]
		check(FileAccess.get_sha256(path) == sha, name + " file matches its pinned hash")
		check(str(member.get("disclosure", "")) != "", name + " has a disclosure")
		match name:
			"ui_text":
				var store := TextLayer.new()
				check(store.open_pack(path, sha), "ui_text opens: " + store.error)
				check(store.entry_count() == int(member["coverage"]["approved_records"]), "ui_text entry count equals coverage")
				check(not TextLayer.new().open_pack(path, "0".repeat(64)), "ui_text refuses another hash")
			"dialogue":
				var store := Dialogue.new()
				check(store.open_pack(path, sha), "dialogue opens: " + store.error)
				check(store.entries.size() + store.unsupported_entries.size() == int(member["coverage"]["lines"]), "dialogue entries equal coverage")
				check(store.unsupported_entries.is_empty(), "every bundled prologue line uses supported original glyphs")
				check(not Dialogue.new().open_pack(path, "0".repeat(64)), "dialogue refuses another hash")
				for key: String in store.entries:
					var text: String = store.entries[key].text
					check(text.count("(") >= text.count(")"), "dialogue line %s closes a parenthesis it never opened: %s" % [key, text])
				_check_dialogue_fit(store, member["coverage"])
			"images":
				var store := Images.new()
				check(store.open_pack(path, sha, false), "approved images open without drafts: " + store.error)
				check(store.asset_count() == int(member["coverage"]["reviewed_images"]), "approved image count equals coverage")
				check(not Images.new().open_pack(path, "0".repeat(64)), "images refuse another hash")
			"draft_images":
				check(not Images.new().open_pack(path, sha, false), "draft images refused without opt-in")
				var store := Images.new()
				check(store.open_pack(path, sha, true), "draft images open with opt-in: " + store.error)
			_:
				failures.append("unknown member " + name)
	_finish()

## A translation that closes a parenthesis drawn by a source decoration piece
## gets the opening one back; balanced or parenthesis-free text is unchanged.
func _check_opened_parenthesis() -> void:
	check(Dialogue.opened_parenthesis("Oh, her turn.)") == "(Oh, her turn.)", "unopened parenthesis gets its opener")
	check(Dialogue.opened_parenthesis("(Oh, her turn.)") == "(Oh, her turn.)", "balanced parenthesis is unchanged")
	check(Dialogue.opened_parenthesis("Huh? Why?") == "Huh? Why?", "text without parentheses is unchanged")
	check(Dialogue.opened_parenthesis("Wait (really)") == "Wait (really)", "inner parentheses are unchanged")

func _check_dialogue_fit(store: RefCounted, coverage: Dictionary) -> void:
	var font_root := OS.get_environment("PROLOGUE_FONT_ROOT")
	if font_root.is_empty():
		return
	var presenter := Presenter.new()
	var failure: String = presenter.open_font(font_root)
	check(failure.is_empty(), "prologue font opens: " + failure)
	if not failure.is_empty():
		return
	# Any two-byte native speaker label; it sits on its own row at 14 px a glyph.
	var speaker := PackedByteArray([0x8f, 0x97, 0x8e, 0x71])
	var overflow: Array[String] = []
	for key: String in store.entries:
		if presenter.translation_layout(store.entries[key], speaker).is_empty():
			overflow.append(key)
	check(overflow.is_empty(), "every bundled prologue line fits the textbox; overflow: " + str(overflow))
	for key: Variant in coverage.get("layout_overflow_line_ids", []):
		check(not store.entries.has(str(key)), "overflowing line %s is not bundled" % key)
	print("Dialogue fit: ", store.entries.size() - overflow.size(), " of ", store.entries.size(), " lines fit")

func _finish() -> void:
	print("English bundle open failures: ", failures)
	quit(0 if failures.is_empty() else 1)
