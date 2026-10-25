extends SceneTree
const Game = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
var checks: int = 0
var failures: int = 0
func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		printerr("FAIL: "+label)
		failures += 1
func _initialize() -> void:
	var tables: Dictionary = {"compatibility":[],"initial_scores":[],"space":[253,254],"cheat_names":[],"girl_birthdays":[],"girl_bloods":[],"girl_fixed_clubs":[]}
	for i in 16: tables.compatibility.append(i%4+1)
	for i in 20: tables.initial_scores.append(10+i)
	for i in 9: tables.cheat_names.append([240,i+1])
	for i in 14: tables.girl_birthdays.append(1); tables.girl_birthdays.append(i+1)
	for i in 12: tables.girl_bloods.append(i%4)
	for i in 7: tables.girl_fixed_clubs.append(i%12)
	var fixture_dir: String = "/tmp/t29012-path-checks"
	DirAccess.make_dir_recursive_absolute(fixture_dir)
	var fixture: String = fixture_dir+"/invented.json"
	var writer: FileAccess = FileAccess.open(fixture,FileAccess.WRITE)
	writer.store_string("{}"); writer.close()
	var disk: DirAccess = DirAccess.open("/")
	check(Game._external_manifest_path_valid(fixture),"canonical absolute external path accepted")
	check(not Game._external_manifest_path_valid("relative-new-game.json"),"relative resource path rejected")
	check(not Game._external_manifest_path_valid("res://relative-new-game.json"),"res protocol rejected")
	check(not Game._external_manifest_path_valid(fixture_dir+"/../t29012-path-checks/invented.json"),"parent traversal rejected")
	check(not Game._external_manifest_path_valid(fixture_dir+"//invented.json"),"noncanonical separators rejected")
	check(not Game._external_manifest_path_valid(ProjectSettings.globalize_path("res://project.godot")),"absolute project path rejected")
	check(not Game._external_manifest_path_valid("/"+fixture),"leading double slash external alias rejected")
	check(not Game._external_manifest_path_valid("/"+ProjectSettings.globalize_path("res://project.godot")),"leading double slash project alias rejected")
	var link: String = fixture_dir+"/linked.json"
	check(disk.create_link(fixture,link) == OK,"create invented file symlink")
	check(not Game._external_manifest_path_valid(link) and Game.load_tables(link).is_empty(),"file symlink rejected before reading")
	var ancestry: String = fixture_dir+"-linked"
	check(disk.create_link(fixture_dir,ancestry) == OK,"create invented directory symlink")
	check(not Game._external_manifest_path_valid(ancestry+"/invented.json"),"directory symlink ancestry rejected")
	var large: String = fixture_dir+"/oversized.json"
	writer = FileAccess.open(large,FileAccess.WRITE)
	writer.store_string("x".repeat(Game.MAX_MANIFEST_BYTES+1)); writer.close()
	check(Game.load_tables(large).is_empty(),"oversized manifest rejected before JSON parsing")
	var bytes := PackedByteArray()
	bytes.resize(Game.SIZE); bytes.fill(0xa5)
	var start = Game.boot_data(bytes,tables)
	check(start.error.is_empty(),"boot accepts full prior state")
	check(bytes[0x44] == 0xa5,"boot preserves caller buffer")
	check(start.bytes[0x41] == 1,"Tuesday native enum")
	check(start.bytes[0xf5] == 0xb0 and start.bytes.decode_u16(0xfe) == 100,"player widths")
	check(start.bytes[0x13e] == 0 and start.bytes[0x13f] == 0,"player clear overlaps progress prefix")
	check(start.bytes[0x668] == 0 and start.bytes[0x66b] == 0,"clear includes saved settings")
	check(start.bytes[0x53c] == 0xa5 and start.bytes[0x71d] == 0xa5,"unowned data preserved")
	check(start.bytes[0x1c8] == 0xca and start.bytes[0x1ca] == 0xb0,"girl flags")
	check(start.bytes[0x140+0x31] == 0xb0,"snapshot precedes entered profile")
	check(Game.boot_data(PackedByteArray(),tables).bytes.is_empty(),"bad state rejected")
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE,1)
	var profile: Dictionary = {"month":255,"day":255,"blood":255}
	var endpoint: Dictionary = rng.checkpoint()
	var finalized = Game.finalize_birthday(profile,rng,"test")
	check(finalized.rng_draws == 3 and finalized.selections[2] < 3,"unset random blood excludes O index")
	rng.restore(endpoint)
	check(Game.finalize_birthday(profile,rng,"test").selections == finalized.selections,"deterministic replay")
	for month in [1,3,5,8,10]:
		var fixed = Game.finalize_birthday({"month":month,"day":30,"blood":3},rng,"test")
		check(fixed.selections[1] == (27 if month == 1 else 29),"invalid calendar day corrected")
	var names: Dictionary = {"family":PackedByteArray([1,2,253,254,3,4]),"given":PackedByteArray([5,6,0,0,7,8]),"nickname":PackedByteArray([9,10,0,0,253,254,11,12,0,0])}
	var player: Dictionary = {"month":0,"day":0,"blood":0}
	for club in 11:
		var shiori: Dictionary = {}
		for month in 12:
			for day in 28:
				if ((day+1)*3+month+1)%11 == club: shiori = {"month":month,"day":day,"blood":3}
		rng.reseed(123)
		var before: Dictionary = rng.checkpoint()
		var result = Game.setup(bytes,names,player,shiori,tables,rng)
		check(result.error.is_empty() and result.bytes.size() == Game.SIZE,"setup returns complete projection")
		check(result.rng_draws == (3 if club < 8 else 4),"conditional club RNG count")
		check(result.bytes[0x1ca]>>4 == club,"all eleven Shiori clubs")
		check(result.bytes.slice(0xd4,0xdc) == PackedByteArray([1,2,3,4,0,0,0,0]),"native name pair compaction")
		check(result.bytes[0x1ca+5*0x38]>>4 == 0 and result.bytes[0x1ca+10*0x38]>>4 == 5,"fixed girl clubs")
		check((result.bytes.decode_u32(0x1cc+12*0x38)>>9)&3 == 3,"Yoshio blood")
		var after: Dictionary = rng.checkpoint()
		rng.restore(before)
		check(Game.setup(bytes,names,player,shiori,tables,rng).bytes == result.bytes and rng.checkpoint() == after,"full replay and RNG endpoint")
	var before: Dictionary = rng.checkpoint()
	var invalid: Dictionary = player.duplicate(); invalid.month = 1.2
	check(not Game.setup(bytes,names,invalid,player,tables,rng).error.is_empty() and rng.checkpoint() == before,"invalid input rolls back RNG")
	tables.initial_scores[0] = -1
	check(not Game.setup(bytes,names,player,player,tables,rng).error.is_empty() and rng.checkpoint() == before,"missing content rejected before RNG")
	print("PASS: "+str(checks)+" New Game checks")
	quit(1 if failures else 0)
