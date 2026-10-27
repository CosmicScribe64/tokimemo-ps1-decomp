extends SceneTree
const Store = preload("res://services/prologue_text_store.gd")
var checks := 0
var failures := 0
func check(value: bool, message: String) -> void:
	checks += 1
	if not value: failures += 1; printerr("FAIL: ",message)
func _initialize() -> void:
	var root := OS.get_environment("PROLOGUE_TEXT_ROOT")
	check(not Store._safe(ProjectSettings.globalize_path("res://").trim_suffix("/")),"project content is not external")
	check(not Store._safe(OS.get_executable_path()),"player executable is not external content")
	var expected := FileAccess.get_sha256(root.path_join("manifest.json"))
	var store := Store.new()
	check(not store.open("relative",expected).is_empty(),"relative root rejected")
	check(not store.open(root,"0".repeat(64)).is_empty(),"wrong manifest hash rejected")
	check(store.open(root,expected).is_empty(),"actual original pack opens")
	var state := {"profile_family_bytes":[130,160,0,0,0,0,0,0],"profile_given_bytes":[130,162,0,0,0,0,0,0],
		"girls":[{"birthday":12|(31<<4)|(3<<9)}],"rng_state":12345,"rng_draw_count":7}
	var before := JSON.stringify(state)
	for line in 43:
		var view := store.resolve(line,state)
		check(view.error.is_empty(),"static line resolves: "+str(line)+" "+view.error)
		if view.error.is_empty():
			check(view.line_id == line and not view.rows.is_empty() and view.clip1.size() == 2 and view.clip2.size() == 2,"static identity and clips preserved")
			check(view.tokens.size() > 0 and not view.speaker_raw.is_empty(),"actual text and speaker data resolved")
	for month in [1,12]:
		for day in [1,9,10,31]:
			state.girls[0].birthday=month|(day<<4)
			var birthday := store.resolve(100,state)
			check(birthday.error.is_empty(),"dynamic birthday native bounds")
			if birthday.error.is_empty(): check(birthday.clip1 == null and not birthday.audio_request,"dynamic text has no fabricated clip")
	for blood in 4:
		state.girls[0].birthday=1|(1<<4)|(blood<<9)
		check(store.resolve(101,state).error.is_empty(),"dynamic blood selector")
	state.girls[0].birthday=12|(31<<4)|(3<<9)
	check(JSON.stringify(state) == before,"resolution preserves complete names/state/RNG")
	check(not store.resolve(43,state).error.is_empty(),"unknown line rejected")
	var bad := state.duplicate(true); bad.girls[0].birthday=0
	check(not store.resolve(100,bad).error.is_empty(),"unset birthday rejected")
	bad=state.duplicate(true);bad.profile_family_bytes=[1,0,0,0,0,0,0,0]
	check(not store.resolve(0,bad).error.is_empty(),"odd name code unit rejected")
	bad=state.duplicate(true);bad.profile_family_bytes=[1,1,0,0,0,0,0,0]
	check(not store.resolve(0,bad).error.is_empty(),"unsupported retained double-byte encoding rejected")
	check(not store.resolve(0,{}).error.is_empty(),"missing retained name rejected")
	var first := store.resolve(0,state);first.clip1[0]=999
	check(store.resolve(0,state).clip1[0]!=999,"returned clips cannot mutate authenticated pack")
	check(Store._split_rows(PackedByteArray([65,32,66,32,67,68])).size()==1,"invented row splitting honors byte parity")
	check(Store._split_rows(PackedByteArray([129,64,32,129,64])).size()==2,"even-byte source space splits rows")
	var payload: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(root.path_join("prologue.json")))
	var expected_birthday := str(payload.dynamic_pieces.birthday_numeric_rows[12].text.raw_hex)+str(payload.dynamic_pieces.month_suffix.text.raw_hex).substr(0,4)+str(payload.dynamic_pieces.birthday_numeric_rows[3].text.raw_hex).substr(0,4)+str(payload.dynamic_pieces.birthday_numeric_rows[1].text.raw_hex).substr(0,4)+str(payload.dynamic_pieces.day_suffix.text.raw_hex).substr(0,8)
	check(store.resolve(100,state).source_raw.hex_encode()==expected_birthday,"December31 uses native one-based source pieces")
	var expected_blood := str(payload.dynamic_pieces.blood_rows[3].text.raw_hex)+str(payload.dynamic_pieces.blood_suffix.text.raw_hex).substr(0,8)
	check(store.resolve(101,state).source_raw.hex_encode()==expected_blood,"blood source selector and bounded suffix preserved")
	var first_row: PackedByteArray = store.resolve(0,state).rows[0].raw
	var name_offset: int = str(payload.renderer_pieces.player_quote_open.text.raw_hex).length()/2+10
	check(first_row.slice(name_offset,name_offset+4)==PackedByteArray([130,160,130,162]),"P form15 resolves retained family then given name")
	var output := OS.get_environment("PROLOGUE_TEST_OUTPUT")
	DirAccess.make_dir_recursive_absolute(output)
	var file := FileAccess.open(output.path_join("prologue.json"),FileAccess.WRITE)
	var raw := FileAccess.get_file_as_bytes(root.path_join("prologue.json"));raw[raw.size()-2]^=1
	file.store_buffer(raw);file.close()
	file=FileAccess.open(output.path_join("manifest.json"),FileAccess.WRITE)
	file.store_buffer(FileAccess.get_file_as_bytes(root.path_join("manifest.json")));file.close()
	check(not store.open(output,expected).is_empty(),"changed payload rejected")
	check(not store.resolve(0,state).error.is_empty(),"failed reopen revokes prior content")
	check(store.open(root,expected).is_empty(),"valid reopen restores content")
	var report := {"checks":checks,"failures":failures,"status":"PASS" if failures == 0 else "FAIL","static_lines":43,"dynamic_lines":2}
	file=FileAccess.open(output.path_join("test-proof.json"),FileAccess.WRITE);file.store_string(JSON.stringify(report));file.close()
	print(JSON.stringify(report));quit(0 if failures == 0 else 1)
