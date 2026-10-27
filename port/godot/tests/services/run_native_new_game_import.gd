extends SceneTree
const Import = preload("res://services/native_new_game_import.gd")
var checks := 0
var failures := 0
func check(value: bool, label: String) -> void:
	if not value:
		push_error(label); failures += 1
	checks += 1
func _initialize() -> void:
	var exe := FileAccess.get_file_as_bytes(OS.get_environment("ORIGINAL_EXE"))
	var overlay := FileAccess.get_file_as_bytes(OS.get_environment("ORIGINAL_NAME_ENT"))
	var root := OS.get_environment("IMPORT_TEST_ROOT")
	check(Import.canonical_json({"z":[1,true,null],"a":"ascii"}) == '{"a":"ascii","z":[1,true,null]}',"canonical sorted serializer")
	check(Import.canonical_json(1.5).is_empty(),"float rejected")
	check(Import.stage(PackedByteArray(),overlay,root).error != "","wrong source rejected")
	var altered := exe.duplicate(); altered[3000] ^= 1
	check(Import.stage(altered,overlay,root).error != "","changed executable rejected")
	altered = overlay.duplicate(); altered[3000] ^= 1
	check(Import.stage(exe,altered,root).error != "","changed overlay rejected")
	check(Import.stage(exe,overlay,"relative").error != "","relative output rejected")
	check(Import.stage(exe,overlay,root+"/../escape").error != "","traversal output rejected")
	check(Import.canonical_json({&"a":1,"z":2}) == '{"a":1,"z":2}',"StringName keys sort canonically")
	check(Import.canonical_json({1:"invalid"}).is_empty(),"non-string key rejected")
	var code := PackedByteArray(); code.resize(16)
	code.encode_u32(0,0x3c081234) # LUI t0,0x1234
	code.encode_u32(4,0x91090010) # LBU t1,16(t0)
	code.encode_u32(8,0x312900f0) # ANDI t1,t1,0xf0
	code.encode_u32(12,0xa1090010) # SB t1,16(t0)
	var stores: Array = Import._field_stores(code,0,0,16)
	check(Import._field_value(stores,0x12340010,0xf0,1,0) == 0,"invented bitfield witness decoded")
	check(Import._field_value(stores+stores,0x12340010,0xf0,1,0) == -1,"duplicate witness rejected")
	code.encode_u32(8,0x00004810) # MFHI t1 invalidates symbolic source.
	check(Import._field_stores(code,0,0,16).is_empty(),"unknown write invalidates symbolic source")
	var linked := root+"-link"
	check(DirAccess.open("/").create_link(root.get_base_dir(),linked) == OK,"symlink fixture created")
	check(Import.stage(exe,overlay,linked.path_join("runtime")).error != "","symlink staging root rejected")
	var result := Import.stage(exe,overlay,root)
	check(result.error == "","actual original extraction: "+result.error)
	if not result.error.is_empty():
		quit(1); return
	check(result.roles.size() == 3,"role allowlist")
	check(Import.stage(exe,overlay,root).error != "","immutable output collision")
	print(JSON.stringify({"checks":checks,"failures":failures,"status":"PASS" if failures == 0 else "FAIL","entry_sha256":FileAccess.get_sha256(root.path_join(result.roles.name_entry_content)),"font_sha256":FileAccess.get_sha256(root.path_join(result.roles.font_root).path_join("font.json"))}))
	quit(0 if failures == 0 else 1)
