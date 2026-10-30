extends SceneTree
## Original extended table and action behavior; actual input/render tests are separate.
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
var failures: Array[String] = []

func check(value: bool, message: String) -> void:
	if not value: failures.append(message)

func _initialize() -> void:
	var base := OS.get_environment("TOKIMEMO_TEST_INSTALLED_PACK")
	if base.is_empty(): base = ProjectSettings.globalize_path("res://../../.asset-cache/port/installed-runtime-v8").simplify_path()
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(base.path_join("manifest.json")))
	var content := Entry.load_content(base.path_join(manifest.roles.name_entry_content))
	if content.is_empty():
		printerr("expanded original content could not load from ",base)
		quit(1)
		return
	check(Entry.content_identity(content) == Entry.EXPANDED_CONTENT_SHA,"expanded source identity")
	var rng := Rng.new()
	check(rng.configure(Rng.OPENBIOS_PROFILE,1),"test RNG configuration")
	var entry := Entry.new()
	check(entry.begin(Rules.load_tables(base.path_join(manifest.roles.name_entry_rules)),content,rng).is_empty(),"original entry begin")
	check(entry.act({"kind":"tick"}).is_empty(),"source initialization")
	check(entry.act({"kind":"tab","value":2}).is_empty(),"Symbols tab available")
	check(entry.cell(0,0) == PackedByteArray(content.candidate_lists[38].slice(0,2)),"first symbol source bytes")
	check(entry.cell(16,0).is_empty(),"symbol arrow column cannot insert glyph")
	for index: int in 40: check(entry.act({"kind":"scroll","delta":1}).is_empty(),"symbol scroll action")
	check(entry.page_scroll == 23,"symbol scroll clamps at original final page")
	check(entry.cell(5,5) == PackedByteArray(content.space),"past-final symbol is native blank")
	check(entry.act({"kind":"tab","value":3}).is_empty(),"Kanji tab available")
	check(entry.page_scroll == 6,"changing tab clamps but preserves shared scroll")
	check(entry.act({"kind":"reading","column":1,"row":0}).is_empty(),"changed reading resets shared scroll")
	for reading: int in 45:
		var column := reading%15%5+(reading%15/5)*6
		check(entry.act({"kind":"reading","column":column,"row":reading/15}).is_empty(),"reading selection")
		check(entry.reading == reading,"reading-grid index matches source")
		var row := 0 if reading == 38 else 1
		check(entry.cell(0,row) == PackedByteArray(content.candidate_lists[reading].slice(0,2)),"candidate bytes for each reading")
		if reading != 38:
			check(entry.cell(0,0) == PackedByteArray(content.reading_labels[reading].slice(0,2)),"selectable reading label bytes")
	check(rng.draw_count == 0,"tab/reading/scroll selection does not draw RNG")
	check(entry.act({"kind":"cell","column":0,"row":1}).is_empty(),"insert selected candidate")
	check(entry.names.family.slice(0,2) == PackedByteArray(content.candidate_lists[44].slice(0,2)),"candidate committed to active name")
	print("Extended NAME checks: ",failures)
	quit(0 if failures.is_empty() else 1)
